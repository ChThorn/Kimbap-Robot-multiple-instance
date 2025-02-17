//#ifndef EXTRINSICCALIB_H
//#define EXTRINSICCALIB_H

//#include <QObject>
//#include <QString>
//#include <QVector>
//#include <QDebug>
//#include <QDir>
//#include <QFile>

//// Eigen headers
//#include <eigen3/Eigen/Core>
//#include <eigen3/Eigen/Geometry>
//#include <eigen3/Eigen/LU>
//#include <eigen3/Eigen/Dense>
//#include <eigen3/Eigen/StdVector>

//// OpenCV headers
//#include <opencv2/core.hpp>
//#include <opencv2/calib3d.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/core/eigen.hpp>

//class ExtrinsicCalib : public QObject
//{
//    Q_OBJECT

//public:
//    explicit ExtrinsicCalib(QObject *parent = nullptr);
//    ~ExtrinsicCalib();

//    // Main calibration function
//    bool calibrate(const QString &dataPath, bool useGripperOffset = false);

//    // Getter for calibration result
//    Eigen::Matrix4d getCalibrationResult() const { return m_gHc; }

//    // Configuration setters
//    void setBoardSize(const cv::Size &size) { m_boardSize = size; }
//    void setSquareSize(const cv::Size2f &size) { m_squareSize = size; }
//    void setNumImages(int num) { m_numImages = num; }

//signals:
//    void progressUpdated(const QString &message);
//    void calibrationCompleted(bool success);

//protected:
//    // Core calibration functions
//    bool loadCalibrationData(const QString &filename, cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
//    bool loadRobotPoses(const QString &filename,
//                       QVector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d>> &bHg);
//    Eigen::Matrix4d createTcpToGripperTransform(bool useGripperOffset,
//                                               double xOffset = 0.0,
//                                               double yOffset = 0.0,
//                                               double zOffset = 0.0,
//                                               double roll = 0.0,
//                                               double pitch = 0.0,
//                                               double yaw = 0.0);
//    Eigen::Matrix4d handEye(const QVector<Eigen::Matrix4d,
//                           Eigen::aligned_allocator<Eigen::Matrix4d>> &bHg,
//                           const QVector<Eigen::Matrix4d,
//                           Eigen::aligned_allocator<Eigen::Matrix4d>> &cHw);

//    // Helper functions
//    static Eigen::Matrix3d skew(const Eigen::Vector3d &v);
//    static Eigen::Matrix4d quat2rot(const Eigen::Vector3d &q);
//    static Eigen::Vector3d rot2quat(const Eigen::MatrixXd &R);
//    static Eigen::Matrix4d transl(const Eigen::Vector3d &x);

//private:
//    // Class members
//    Eigen::Matrix4d m_gHc;
//    cv::Size m_boardSize;
//    cv::Size2f m_squareSize;
//    int m_numImages;
//    static constexpr double PI = 3.14159265358979323846;
//};

//#endif // EXTRINSICCALIB_H
