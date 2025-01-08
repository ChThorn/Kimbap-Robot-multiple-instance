#ifndef DIALOGDETECTOR_H
#define DIALOGDETECTOR_H

#include <QDebug>
#include <QDialog>
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>

struct Detection {
    cv::Point2f center;
    float confidence;
    float width;
    float height;
};

class DialogDetector
{
public:
    DialogDetector();
    ~DialogDetector();

    // Initialize YOLO model
    void loadYOLOModel(const std::string& modelConfig, const std::string& modelWeights);

    // Detect objects in grid cells
    std::vector<int>detectObjectsInGridCells(cv::Mat& image, const cv::Rect& roi,
                                             int rows, int cols);

    // Draw results on image
    void drawGridAndLabels(cv::Mat& image, const cv::Rect& roi,
                           int rows, int cols, const std::vector<int>& cellResults);
    void drawDebugInfo(cv::Mat& image, const cv::Rect& roi, const std::vector<int>& detections);

private:
    cv::dnn::Net net;
    float confThreshold;
    float nmsThreshold;
    bool isInitialized;
    std::vector<std::string> getOutputLayerNames();
};

#endif // DIALOGDETECTOR_H
