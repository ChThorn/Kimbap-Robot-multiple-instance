#include "DialogExtrinsic.h"
#include "ui_DialogExtrinsic.h"
#include "DialogCalibration.h"

DialogExtrinsic::DialogExtrinsic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExtrinsic),
    isRunning(false),
    isPaused(false),
    imageCount(0),
    board_size(9, 6)
{
    ui->setupUi(this);
//    dialogCal = nullptr;

    DisplayUI();

    show();
    raise();
    activateWindow();
    setFocus();
}

void DialogExtrinsic::DisplayUI()
{
    // Initialize graphics scene
    scene = new QGraphicsScene(this);  // "this" as parent ensures proper cleanup
    ui->graphicsView->setScene(scene);

    // Initialize camera timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DialogExtrinsic::onTimer);

    Bannertimer = new QTimer(this);
    connect(Bannertimer, &QTimer::timeout, this, &DialogExtrinsic::BannerStyle);
    Bannertimer->start(300);

    // Set initial button text
    ui->TOGGLE->setText("Capture");

    // Initially disable delete button
    ui->DELETE->setEnabled(false);

    // Start Camera
    StartCamera();
}

DialogExtrinsic::~DialogExtrinsic()
{
    if(isRunning)
    {
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

void DialogExtrinsic::onTimer()
{
    if(!isRunning) return;

    try
    {
        // Get frames
        auto frames = pipe.wait_for_frames();
        auto color_frame = frames.get_color_frame();

        // Convert to OpenCV Mat
        cv::Mat color_image(
                    cv::Size(CAMERA_WIDTH, CAMERA_HEIGHT),
                    CV_8UC3,
                    (void*)color_frame.get_data(),
                    cv::Mat::AUTO_STEP);

        cv::Mat display_image = color_image.clone();

        // Find and draw chessboard corners
        std::vector<cv::Point2f> corners;
        bool found = findChessboardPoints(color_image, corners);

        if(found)
        {
            cv::drawChessboardCorners(display_image, board_size, corners, true);

            // Show status text
            QString status = QString("Images: %1/20").arg(imageCount);
            cv::putText(display_image, status.toStdString(),
                        cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX,
                        0.8, cv::Scalar(0, 255, 0), 2);

            if(isPaused && !calibrationImages.empty())
            {
                ui->TOGGLE->setText("Confirm");
            }
        }
        // Convert to QPixmap and display
        QPixmap pixmap = matToPixmap(display_image);
        displayImage(pixmap);
    }
    catch (const rs2::error& e)
    {
        QMessageBox::critical(this, "Error",
                              QString("Camera error: %1").arg(e.what()));
        timer->stop();
        isRunning = false;
    }
}

QPixmap DialogExtrinsic::matToPixmap(const cv::Mat& image)
{
    QImage qimg(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
    return QPixmap::fromImage(qimg);
}

void DialogExtrinsic::displayImage(const QPixmap& pixmap)
{
    scene->clear(); // Clear any existing content in the scene
    scene->addPixmap(pixmap);
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void DialogExtrinsic::StartCamera()
{
    try
    {
        rs2::config cfg;
        cfg.enable_stream(RS2_STREAM_COLOR, CAMERA_WIDTH, CAMERA_HEIGHT, RS2_FORMAT_BGR8, 30);
        pipe.start(cfg);

        // Allow auto-exposure to settle
        for(int i = 0; i < 30; i++)
        {
            pipe.wait_for_frames();
        }

        isRunning = true;
        timer->start(33); // ~30 fps
    }
    catch(const rs2::error& e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to initialize camera: %1").arg(e.what()));
    }
}

bool DialogExtrinsic::findChessboardPoints(const cv::Mat &image, std::vector<cv::Point2f> &corners)
{
    cv::Mat gray;
    if(image.channels() == 3)
    {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray = image;
    }

    bool found = cv::findChessboardCorners(gray, board_size, corners,
        cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

    if(found)
    {
        cv::cornerSubPix(gray, corners, cv::Size(11, 11),
                         cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::EPS +
                                          cv::TermCriteria::COUNT, 30, 0.1));
    }

    return found;
}

void DialogExtrinsic::on_BACK_clicked()
{
    if(isRunning)
    {
        timer->stop();
        pipe.stop();
        isRunning = false;
    }


    if(timer)
    {
        timer->stop();
    }

    // Create and show the calibration dialog
    auto dialogCal = new DialogCalibration();
    dialogCal->show();
    this->close();
}

void DialogExtrinsic::closeEvent(QCloseEvent* event)
{
    // Stop camera and timer before closing
    if(isRunning)
    {
        timer->stop();
        pipe.stop();
        isRunning = false;
    }

    if(Bannertimer)
    {
        Bannertimer->stop();
    }


    // Call parent class close event
    QDialog::closeEvent(event);
}

void DialogExtrinsic::BannerStyle()
{
    static int colorIndex = 0;
    const QStringList colorList = {"#A0D468", "#4FC1E9", "#FFCE54", "#FC6E51", "#ED5565", "#AC92EC"};

    // Change text color
    QString color = colorList.at(colorIndex);
    ui->BANNER->setStyleSheet("QLabel {color: " + color + "; font-size: 22px; font:bold;}");

    // Create drop shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setColor(QColor(color));
    shadowEffect->setBlurRadius(70);
    shadowEffect->setOffset(7, 7);

    ui->BANNER->setGraphicsEffect(shadowEffect);

    colorIndex = (colorIndex + 1)%colorList.size();
}
