#include "DialogIntrinsic.h"
#include "ui_DialogIntrinsic.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include "DialogCalibration.h"

DialogIntrinsic::DialogIntrinsic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIntrinsic),
    isRunning(false),
    isPaused(false),
    imageCount(0),
    board_size(9, 6),
    square_size(0.025f)  // 25mm squares
{
    ui->setupUi(this);
//    dialogCal = nullptr;

    // Create calibration images directory
    calibrationDir = QDir::currentPath() + "/calibration_images";
    QDir().mkpath(calibrationDir);

    // Clean up any previous calibration data
    cleanupPreviousCalibration();

    // Initialize graphics scene
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Initialize camera timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DialogIntrinsic::onTimer);

    // Connect list widget selection changed signal
    connect(ui->imageList, &QListWidget::itemSelectionChanged,
            this, &DialogIntrinsic::onImageSelectionChanged);

    Bannertimer = new QTimer(this);
    connect(Bannertimer, &QTimer::timeout, this, &DialogIntrinsic::BannerStyle);
    Bannertimer->start(300);

    // Initially disable delete button
    ui->DELETE->setEnabled(false);

    // Initialize calibration matrices
    camera_matrix = cv::Mat::eye(3, 3, CV_64F);
    dist_coeffs = cv::Mat::zeros(5, 1, CV_64F);

    // Set initial button text
    ui->TOGGLE->setText("Capture");

    // Start camera
    try {
        rs2::config cfg;
        cfg.enable_stream(RS2_STREAM_COLOR, CAMERA_WIDTH, CAMERA_HEIGHT, RS2_FORMAT_BGR8, 30);
        pipe.start(cfg);

        // Allow auto-exposure to settle
        for(int i = 0; i < 30; i++) {
            pipe.wait_for_frames();
        }

        isRunning = true;
        timer->start(33); // ~30 fps

    } catch (const rs2::error& e) {
        QMessageBox::critical(this, "Error",
            QString("Failed to initialize camera: %1").arg(e.what()));
    }

    show();
    raise();
    activateWindow();
    setFocus();
}

DialogIntrinsic::~DialogIntrinsic()
{
    if (isRunning) {
        timer->stop();
        pipe.stop();
    }

//    if(dialogCal)
//    {
//        delete dialogCal;
//        dialogCal = nullptr;
//    }

    if(Bannertimer)
    {
        Bannertimer->stop();
        delete Bannertimer;
        Bannertimer = nullptr;
    }
    if(ui->BANNER->graphicsEffect())
    {
        delete ui->BANNER->graphicsEffect();
        ui->BANNER->setGraphicsEffect(nullptr);
    }
    delete ui;
}

void DialogIntrinsic::onTimer()
{
    if (!isRunning) return;

    try {
        // Get frames
        auto frames = pipe.wait_for_frames();
        auto color_frame = frames.get_color_frame();

        // Convert to OpenCV mat
        cv::Mat color_image(
            cv::Size(CAMERA_WIDTH, CAMERA_HEIGHT),
            CV_8UC3,
            (void*)color_frame.get_data(),
            cv::Mat::AUTO_STEP);

        cv::Mat display_image = color_image.clone();

        // Find and draw chessboard corners
        std::vector<cv::Point2f> corners;
        bool found = findChessboardPoints(color_image, corners);

        if (found) {
            cv::drawChessboardCorners(display_image, board_size, corners, true);

            // Show status text
            QString status = QString("Found checkerboard! Images: %1/20")
                           .arg(imageCount);
            cv::putText(display_image, status.toStdString(),
                       cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX,
                       0.8, cv::Scalar(0, 255, 0), 2);

            if (isPaused && !calibrationImages.empty()) {
                ui->TOGGLE->setText("Confirm");
            }
        }

        // Convert to QImage and display
        QImage qimg(display_image.data, display_image.cols, display_image.rows,
                   display_image.step, QImage::Format_BGR888);
        QPixmap pixmap = QPixmap::fromImage(qimg);

        // Clear and set new pixmap
        scene->clear();
        scene->addPixmap(pixmap);
        ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    } catch (const rs2::error& e) {
        QMessageBox::critical(this, "Error",
            QString("Camera error: %1").arg(e.what()));
        timer->stop();
        isRunning = false;
    }
}

bool DialogIntrinsic::findChessboardPoints(const cv::Mat& image,
                                         std::vector<cv::Point2f>& corners)
{
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image;
    }

    bool found = cv::findChessboardCorners(gray, board_size, corners,
        cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

    if (found) {
        cv::cornerSubPix(gray, corners, cv::Size(11,11),
                        cv::Size(-1,-1),
                        cv::TermCriteria(cv::TermCriteria::EPS +
                                       cv::TermCriteria::COUNT, 30, 0.1));
    }

    return found;
}

void DialogIntrinsic::createObjectPoints(std::vector<cv::Point3f>& obj_points)
{
    obj_points.clear();
    for (int i = 0; i < board_size.height; i++) {
        for (int j = 0; j < board_size.width; j++) {
            obj_points.push_back(cv::Point3f(j * square_size,
                                           i * square_size,
                                           0.0f));
        }
    }
}

bool DialogIntrinsic::calibrate()
{
    if (calibrationImages.empty()) return false;

    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    cv::Size image_size = calibrationImages[0].size();

    // Process each calibration image
    for (const auto& image : calibrationImages) {
        std::vector<cv::Point2f> corners;
        if (findChessboardPoints(image, corners)) {
            imagePoints.push_back(corners);

            std::vector<cv::Point3f> obj;
            createObjectPoints(obj);
            objectPoints.push_back(obj);
        }
    }

    if (imagePoints.empty()) return false;

    // Perform calibration
    rms_error = cv::calibrateCamera(
        objectPoints, imagePoints,
        image_size,
        camera_matrix,
        dist_coeffs,
        cv::noArray(), cv::noArray(),
        cv::CALIB_FIX_K4 | cv::CALIB_FIX_K5);

    return rms_error < 1.0;
}

bool DialogIntrinsic::saveCalibration(const std::string& filename)
{
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        return false;
    }

    fs << "camera_matrix" << camera_matrix;
    fs << "dist_coeffs" << dist_coeffs;
    fs << "rms_error" << rms_error;
    fs.release();
    return true;
}

void DialogIntrinsic::onImageSelectionChanged()
{
    // Enable delete button only when items are selected
    ui->DELETE->setEnabled(!ui->imageList->selectedItems().isEmpty());
}

void DialogIntrinsic::on_DELETE_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->imageList->selectedItems();
    if (selectedItems.isEmpty()) return;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete the selected image(s)?",
        QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        foreach(QListWidgetItem* item, selectedItems) {
            QString filename = item->data(Qt::UserRole).toString();
            QFile::remove(filename);

            // Remove from calibrationImages vector
            int index = item->data(Qt::UserRole + 1).toInt();
            if (index >= 0 && index < calibrationImages.size()) {
                calibrationImages.erase(calibrationImages.begin() + index);
            }

            delete item;
            imageCount--;
        }

        // Rename remaining files and update list items to maintain sequential order
        for (int i = 0; i < ui->imageList->count(); i++) {
            QListWidgetItem* item = ui->imageList->item(i);
            QString oldFilename = item->data(Qt::UserRole).toString();

            // Create new filename
            QString newFilename = QString("%1/image_%2.png")
                                .arg(calibrationDir)
                                .arg(i + 1, 2, 10, QLatin1Char('0'));

            // Rename file
            QFile::rename(oldFilename, newFilename);

            // Update list item
            item->setText(QString("Image %1").arg(i + 1, 2, 10, QLatin1Char('0')));
            item->setData(Qt::UserRole, newFilename);
            item->setData(Qt::UserRole + 1, i);
        }

        // Update UI
        if (imageCount < 20) {
            ui->TOGGLE->setText("Capture");
            isPaused = false;
        }

        // Update status
        QString status = QString("Images: %1/20").arg(imageCount);
        ui->statusLabel->setText(status);
    }
}

void DialogIntrinsic::on_TOGGLE_clicked()
{
    if (!isRunning) return;

    if (ui->TOGGLE->text() == "Capture") {
        if(imageCount == 0)
        {
            cleanupPreviousCalibration();
        }

        // Capture current frame
        auto frames = pipe.wait_for_frames();
        auto color_frame = frames.get_color_frame();

        cv::Mat color_image(
                    cv::Size(CAMERA_WIDTH, CAMERA_HEIGHT),
                    CV_8UC3,
                    (void*)color_frame.get_data(),
                    cv::Mat::AUTO_STEP);
        std::vector<cv::Point2f> corners;
        bool found = findChessboardPoints(color_image, corners);

        if(found)
        {
            QString filename = QString("%1/image_%2.png")
                              .arg(calibrationDir)
                              .arg(imageCount+1, 2, 10, QLatin1Char('0')); // Pad with zeros: 01, 02, etc.,
            cv::imwrite(filename.toStdString(), color_image);
            calibrationImages.push_back(color_image.clone());

            QListWidgetItem* item = new QListWidgetItem(
                        QString("Image %1").arg(imageCount+1, 2, 10, QLatin1Char('0')));
            item->setData(Qt::UserRole, filename);
            item->setData(Qt::UserRole + 1, imageCount);
            ui->imageList->addItem(item);

            imageCount++;

            if(imageCount >= 20)
            {
                ui->TOGGLE->setText("confirm");
                isPaused = true;
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning", "No checkerboard detected! Please try again.");
        }
    } else if (ui->TOGGLE->text() == "Confirm") {
        timer->stop();
        QApplication::setOverrideCursor(Qt::WaitCursor);

        if (calibrate()) {
            saveCalibration(calibrationDir.toStdString() + "/intrinsic_params.yml");

            QMessageBox::information(this, "Success",
                QString("Calibration successful!\nRMS error: %1\nParameters saved in: %2")
                .arg(rms_error)
                .arg(calibrationDir + "/intrinsic_params.yml"));

            // Clean up camera
            if (isRunning) {
                pipe.stop();
                isRunning = false;
            }

            // Create new calibration dialog and delete this one
            DialogCalibration* newDialog = new DialogCalibration();
            newDialog->show();
            this->close();
        } else {
            QMessageBox::critical(this, "Error",
                "Calibration failed! Please try again.");

            // Reset and restart
            calibrationImages.clear();
            imageCount = 0;
            ui->TOGGLE->setText("Capture");
            isPaused = false;
            timer->start();
            ui->imageList->clear();
        }

        QApplication::restoreOverrideCursor();
    }
}

void DialogIntrinsic::cleanupPreviousCalibration()
{
    // Clear the calibration directory
    QDir dir(calibrationDir);
    dir.setNameFilters(QStringList() << "*.png" << "*.jpg" << "*.yml");
    dir.setFilter(QDir::Files);
    foreach(QString file, dir.entryList()) {
        dir.remove(file);
    }

    // Clear the list widget
    ui->imageList->clear();

    // Clear vectors and reset counters
    calibrationImages.clear();
    imageCount = 0;

    // Reset UI state
    ui->TOGGLE->setText("Capture");
    isPaused = false;
}

void DialogIntrinsic::closeEvent(QCloseEvent *event)
{
    // Stop camera and timer before closing
    if (isRunning) {
        timer->stop();
        pipe.stop();
        isRunning = false;
    }

    // Call parent class close event
    QDialog::closeEvent(event);
}

void DialogIntrinsic::on_BACK_clicked()
{
    if (isRunning) {
        timer->stop();
        pipe.stop();
        isRunning = false;
    }

    // Create and show the calibratioin dialog
    auto dialogCal = new DialogCalibration();
    dialogCal->show();
    this->close();

//    reject(); // Close dialog and return to previous window
}

void DialogIntrinsic::BannerStyle()
{
    static int colorIndex = 0;
    const QStringList colorList = {"#A0D468", "#4FC1E9", "#FFCE54", "#FC5E51", "#ED5565", "#AC92EC"};

    // Change text color
    QString color = colorList.at(colorIndex);
    ui->BANNER->setStyleSheet("QLabel {color: " + color + "; font-size: 22px; font:bold;}");

    // Create drop shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setColor(QColor(color));
    shadowEffect->setBlurRadius(70);
    shadowEffect->setOffset(7, 7);

    // Apply stylesheet and drop shadow effect to the label
    ui->BANNER->setGraphicsEffect(shadowEffect);

    // Increment color index loop back to start if needed
    colorIndex = (colorIndex + 1)%colorList.size();
}
