#ifndef DIALOGEXTRINSIC_H
#define DIALOGEXTRINSIC_H

#include <QDialog>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include "librealsense2/rs.hpp"
#include <opencv2/opencv.hpp>
#include <QGraphicsScene>
#include <QMessageBox>

class DialogCalibration;

namespace Ui {
class DialogExtrinsic;
}

class DialogExtrinsic : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExtrinsic(QWidget *parent = nullptr);
    ~DialogExtrinsic();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::DialogExtrinsic *ui;
    QTimer *Bannertimer;
    QTimer *timer;
    QGraphicsScene *scene;

//    DialogCalibration* dialogCal;

    // Text or Banner style
    void BannerStyle();

    // UI setup display
    void DisplayUI();

    // Start Camera
    void StartCamera();

    // Camera members
    rs2::pipeline pipe;
    rs2::colorizer color_map;
    bool isRunning;
    bool isPaused;
    int imageCount;

    // Camera parameters
    const int CAMERA_WIDTH = 640;
    const int CAMERA_HEIGHT = 480;

    // Mat to Pixmap
    QPixmap matToPixmap(const cv::Mat& image);

    // display image
    void displayImage(const QPixmap& pixmap);

    // Calibration members
    cv::Size board_size;
    std::vector<cv::Mat> calibrationImages;

    // Calibration methods
    bool findChessboardPoints(const cv::Mat& image, std::vector<cv::Point2f>& corners);

private slots:
    void onTimer();
    void on_BACK_clicked();
};

#endif // DIALOGEXTRINSIC_H
