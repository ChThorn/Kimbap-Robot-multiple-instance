#ifndef DIALOGINTRINSIC_H
#define DIALOGINTRINSIC_H

#include <QDialog>
#include <QTimer>
#include <QGraphicsScene>
#include "librealsense2/rs.hpp"
#include <opencv2/opencv.hpp>
#include <QListWidget>
#include <QGraphicsDropShadowEffect>

//class DialogCalibration;

namespace Ui {
class DialogIntrinsic;
}

class DialogIntrinsic : public QDialog
{
    Q_OBJECT

public:
    explicit DialogIntrinsic(QWidget *parent = nullptr);
    ~DialogIntrinsic();

private slots:
    void onTimer();
    void on_BACK_clicked();
    void on_TOGGLE_clicked();
    void on_DELETE_clicked();
    void onImageSelectionChanged();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    // UI members
    Ui::DialogIntrinsic *ui;
    QTimer *timer;
    QTimer *Bannertimer;
    QGraphicsScene *scene;
//    DialogCalibration *dialogCal;
    QString calibrationDir;

    // Camera members
    rs2::pipeline pipe;
    rs2::colorizer color_map;
    bool isRunning;
    bool isPaused;
    int imageCount;

    // Camera parameters
    const int CAMERA_WIDTH = 640;
    const int CAMERA_HEIGHT = 480;

    // Calibration members
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;
    cv::Size board_size;
    float square_size;
    std::vector<cv::Mat> calibrationImages;
    double rms_error;

    // Calibration methods
    bool findChessboardPoints(const cv::Mat& image, std::vector<cv::Point2f>& corners);
    void createObjectPoints(std::vector<cv::Point3f>& obj_points);
    bool calibrate();
    bool saveCalibration(const std::string& filename);
    void cleanupPreviousCalibration();
    void BannerStyle();
};

#endif // DIALOGINTRINSIC_H
