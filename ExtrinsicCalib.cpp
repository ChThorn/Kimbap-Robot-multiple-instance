//#include "ExtrinsicCalib.h"
//#include <QTextStream>

//ExtrinsicCalib::ExtrinsicCalib(QObject *parent)
//    : QObject(parent)
//    , m_boardSize(9, 6)
//    , m_squareSize(25.0f, 25.0f)
//    , m_numImages(10)
//{
//}

//ExtrinsicCalib::~ExtrinsicCalib()
//{
//}

//bool ExtrinsicCalib::calibrate(const QString &dataPath, bool useGripperOffset)
//{
//    QFile outputFile(dataPath + "/output.txt");
//    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        emit progressUpdated("Failed to open output file");
//        return false;
//    }
//    QTextStream ofs(&outputFile);

//    // Load camera calibration data
//    cv::Mat cameraMatrix, distCoeffs;
//    if (!loadCalibrationData(dataPath + "/intrinsic_calibrated.yaml",
//                            cameraMatrix, distCoeffs)) {
//        return false;
//    }

//    emit progressUpdated("Reading images...");
//    QVector<cv::Mat> images;
//    for (int i = 0; i < m_numImages; ++i) {
//        QString imagePath = dataPath + QString("/%1.png").arg(i);
//        cv::Mat image = cv::imread(imagePath.toStdString(), 0);
//        if (image.empty()) {
//            emit progressUpdated("Cannot find " + imagePath);
//            return false;
//        }
//        images.push_back(image);
//    }

//    emit progressUpdated("Extracting chessboard corners...");
//    QVector<std::vector<cv::Point2f>> imagePointsSeq;
//    std::vector<cv::Point3f> objectPointTemplate;

//    // Create template for object points
//    for (int i = 0; i < m_boardSize.height; ++i) {
//        for (int j = 0; j < m_boardSize.width; ++j) {
//            objectPointTemplate.push_back(
//                cv::Point3f(j * m_squareSize.width, i * m_squareSize.height, 0));
//        }
//    }

//    // Find corners in each image
//    for (int i = 0; i < images.size(); ++i) {
//        std::vector<cv::Point2f> corners;
//        bool found = cv::findChessboardCorners(images[i], m_boardSize, corners);

//        if (found) {
//            cv::cornerSubPix(images[i], corners, cv::Size(11, 11),
//                            cv::Size(-1, -1),
//                            cv::TermCriteria(cv::TermCriteria::EPS +
//                                           cv::TermCriteria::MAX_ITER, 30, 0.1));
//            imagePointsSeq.push_back(corners);

//            // Optional: Draw and save corners
//            cv::Mat coloredImg;
//            cv::cvtColor(images[i], coloredImg, cv::COLOR_GRAY2BGR);
//            cv::drawChessboardCorners(coloredImg, m_boardSize, corners, found);
//            cv::imwrite((dataPath + QString("/corners_%1.png").arg(i)).toStdString(),
//                       coloredImg);
//        }
//    }

//    // Calculate extrinsic parameters
//    QVector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d>> cHw;
//    for (const auto &corners : imagePointsSeq) {
//        cv::Mat rvec, tvec;
//        cv::solvePnP(objectPointTemplate, corners, cameraMatrix,
//                     distCoeffs, rvec, tvec);

//        cv::Mat R;
//        cv::Rodrigues(rvec, R);
//        Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
//        Eigen::Matrix3d REigen;
//        cv::cv2eigen(R, REigen);
//        transform.block<3,3>(0,0) = REigen;
//        transform.block<3,1>(0,3) = Eigen::Vector3d(tvec.at<double>(0),
//                                                   tvec.at<double>(1),
//                                                   tvec.at<double>(2));
//        cHw.push_back(transform);
//    }

//    // Load robot poses
//    emit progressUpdated("Reading robot poses...");
//    QVector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d>> bHg;
//    if (!loadRobotPoses(dataPath + "/robot_poses.yaml", bHg)) {
//        return false;
//    }

//    // Perform calibration
//    if (bHg.size() == cHw.size() && !bHg.empty()) {
//        emit progressUpdated("Performing hand-eye calibration...");
//        Eigen::Matrix4d eHc = handEye(bHg, cHw);

//        // Example gripper offset values
//        double xOffset = 0.0;
//        double yOffset = 0.0;
//        double zOffset = 100.0;
//        double roll = 0.0;
//        double pitch = 45.0;
//        double yaw = 0.0;

//        Eigen::Matrix4d gHe = createTcpToGripperTransform(useGripperOffset,
//                                                         xOffset, yOffset, zOffset,
//                                                         roll, pitch, yaw);
//        m_gHc = gHe * eHc;

//        // Save results
//        ofs << "Hand-eye calibration result (gHc):\n"
//            << m_gHc << "\n\n";

//        if (useGripperOffset) {
//            ofs << "Gripper offset transform (gHe - Gripper to TCP):\n"
//                << gHe << "\n\n";
//            ofs << "Final transform with gripper offset (gHc - Gripper to Camera):\n"
//                << m_gHc << "\n";
//        }

//        emit progressUpdated("Calibration complete. Results saved to output.txt");
//        emit calibrationCompleted(true);
//        return true;
//    }

//    emit progressUpdated("Error: Mismatched number of poses and images");
//    emit calibrationCompleted(false);
//    return false;
//}

//bool ExtrinsicCalib::loadCalibrationData(const QString &filename,
//                                        cv::Mat &cameraMatrix,
//                                        cv::Mat &distCoeffs)
//{
//    cv::FileStorage fs(filename.toStdString(), cv::FileStorage::READ);
//    if (!fs.isOpened()) {
//        emit progressUpdated("Failed to open " + filename);
//        return false;
//    }

//    fs["camera_matrix"] >> cameraMatrix;
//    fs["distortion_coefficients"] >> distCoeffs;

//    fs.release();

//    if (cameraMatrix.rows != 3 || cameraMatrix.cols != 3) {
//        emit progressUpdated("Invalid camera matrix format");
//        return false;
//    }

//    if (distCoeffs.cols != 5 || distCoeffs.rows != 1) {
//        emit progressUpdated("Invalid distortion coefficients format");
//        return false;
//    }

//    return true;
//}

//bool ExtrinsicCalib::loadRobotPoses(const QString &filename,
//                                   QVector<Eigen::Matrix4d,
//                                   Eigen::aligned_allocator<Eigen::Matrix4d>> &bHg)
//{
//    cv::FileStorage fs(filename.toStdString(), cv::FileStorage::READ);
//    if (!fs.isOpened()) {
//        emit progressUpdated("Failed to open " + filename);
//        return false;
//    }

//    int numPoses = (int)fs["num_poses"];
//    cv::FileNode poses = fs["poses"];

//    for (const auto &poseEntry : poses) {
//        try {
//            std::vector<double> position, orientation;
//            cv::FileNode poseData = poseEntry.begin().operator*();
//            poseData["position"] >> position;
//            poseData["orientation"] >> orientation;

//            if (position.size() != 3 || orientation.size() != 3) {
//                emit progressUpdated("Invalid position or orientation data in pose file");
//                continue;
//            }

//            Eigen::AngleAxisd rollAngle(orientation[0] * PI/180, Eigen::Vector3d::UnitX());
//            Eigen::AngleAxisd pitchAngle(orientation[1] * PI/180, Eigen::Vector3d::UnitY());
//            Eigen::AngleAxisd yawAngle(orientation[2] * PI/180, Eigen::Vector3d::UnitZ());

//            Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
//            transform.block<3,3>(0,0) = (yawAngle * pitchAngle * rollAngle).matrix();
//            transform.block<3,1>(0,3) = Eigen::Vector3d(position[0], position[1], position[2]);

//            bHg.push_back(transform.inverse());
//        }
//        catch (const std::exception& e) {
//            emit progressUpdated(QString("Error processing pose: %1").arg(e.what()));
//            continue;
//        }
//    }

//    fs.release();

//    if (bHg.size() != numPoses) {
//        emit progressUpdated(QString("Warning: Number of loaded poses (%1) doesn't match expected number (%2)")
//                           .arg(bHg.size())
//                           .arg(numPoses));
//    }

//    return !bHg.isEmpty();
//}

//Eigen::Matrix4d ExtrinsicCalib::createTcpToGripperTransform(bool useGripperOffset,
//                                                           double xOffset,
//                                                           double yOffset,
//                                                           double zOffset,
//                                                           double roll,
//                                                           double pitch,
//                                                           double yaw)
//{
//    Eigen::Matrix4d gHe = Eigen::Matrix4d::Identity();

//    if (!useGripperOffset) {
//        return gHe;
//    }

//    // Convert angles to radians
//    roll *= PI/180.0;
//    pitch *= PI/180.0;
//    yaw *= PI/180.0;

//    // Create rotation matrix
//    Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitX());
//    Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitY());
//    Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitZ());

//    // Set rotation
//    gHe.block<3,3>(0,0) = (yawAngle * pitchAngle * rollAngle).matrix();

//    // Set translation
//    gHe.block<3,1>(0,3) = Eigen::Vector3d(xOffset, yOffset, zOffset);

//    return gHe;
//}

//Eigen::Matrix3d ExtrinsicCalib::skew(const Eigen::Vector3d &v)
//{
//    Eigen::Matrix3d S;
//    S << 0, -v(2), v(1),
//         v(2), 0, -v(0),
//         -v(1), v(0), 0;
//    return S;
//}

//Eigen::Matrix4d ExtrinsicCalib::quat2rot(const Eigen::Vector3d &q)
//{
//    double p = q.transpose() * q;
//    if (p > 1) {
//        qDebug() << "Warning: quat2rot: quaternion greater than 1";
//    }
//    double w = sqrt(1 - p);

//    Eigen::Matrix4d R = Eigen::Matrix4d::Identity();
//    Eigen::Matrix3d res = 2 * (q*q.transpose()) + 2 * w*
