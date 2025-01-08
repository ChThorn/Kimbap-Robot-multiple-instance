#include "DialogDetector.h"

DialogDetector::DialogDetector()
    : confThreshold(0.8f)
    , nmsThreshold(0.4f)
    , isInitialized(false)
{}

DialogDetector::~DialogDetector(){}

void DialogDetector::loadYOLOModel(const std::string &modelConfig, const std::string &modelWeights)
{
    net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    isInitialized = true;
}

std::vector<std::string> DialogDetector::getOutputLayerNames()
{
    return net.getUnconnectedOutLayersNames();
}

//std::vector<int> DialogDetector::detectObjectsInGridCells(cv::Mat &image, const cv::Rect &roi, int rows, int cols)
//{
//    if(!isInitialized)
//    {
//        std::cerr << "YOLO model not initialized!" << std::endl;
//        return std::vector<int>(rows*cols, -1);
//    }

//    std::vector<int>cellResults(rows*cols, -1);
//    int cellWidth = roi.width/cols;
//    int cellHeight = roi.height/rows;

//    for(int row=0; row<rows; ++row)
//    {
//        for(int col=0; col<cols; ++col)
//        {
//            cv::Rect cell(roi.x+col*cellWidth, roi.y+row*cellHeight, cellWidth, cellHeight);
//            cv::Mat cellImage = image(cell);

//            if(cellImage.empty() || cellImage.rows<10 || cellImage.cols<10)
//            {
//                continue;
//            }

//            try
//            {
//                // Prepare image for YOLO
//                cv::Mat blob = cv::dnn::blobFromImage(cellImage, 1/255.0,
//                                                      cv::Size(416, 416),
//                                                      cv::Scalar(0, 0, 0),
//                                                      true,
//                                                      false);
//                net.setInput(blob);

//                std::vector<cv::Mat>outs;
//                net.forward(outs, getOutputLayerNames());

//                // Process detections
//                std::vector<int>classIds;
//                std::vector<float>confidences;
//                std::vector<cv::Rect>boxes;

//                // Only consider high-confidence detections
//                float minConfidence = 0.7f; // Confidence threshold

//                for(auto& output : outs)
//                {
//                    for(int i=0; i<output.rows; ++i)
//                    {
//                        float* data = (float*)output.ptr(i);
//                        float confidence = data[4];

//                        if(confidence > minConfidence)
//                        {
//                            // Additional size checks for detected objects
//                            float width = data[2]*cellImage.cols;
//                            float height = data[3]*cellImage.rows;

//                            // Skip if detected object is too small relative to cell size
//                            if(width < cellWidth*0.2 || height<cellHeight*0.2)
//                            {
//                                continue;
//                            }

//                            int centerX = (int)(data[0]*cellImage.cols);
//                            int centerY = (int)(data[1]*cellImage.rows);
//                            int left = centerX - width/2;
//                            int top = centerY - height/2;

//                            classIds.push_back((int)data[5]);
//                            confidences.push_back(confidence);
//                            boxes.push_back(cv::Rect(left, top, width, height));
//                        }
//                    }
//                }

//                // Apply NMS with stricter thershold
//                std::vector<int>indices;
//                cv::dnn::NMSBoxes(boxes, confidences, minConfidence, 0.3f, indices);

//                if(!indices.empty())
//                {
//                    cellResults[row*cols+col] = row*cols+col+1;
//                }
//            }
//            catch(const cv::Exception& e)
//            {
//                std::cerr << "Error processing cell "<<(row*cols+col) << ":" << e.what() << std::endl;
//            }
//        }
//    }
//    return cellResults;
//}

//std::vector<int> DialogDetector::detectObjectsInGridCells(cv::Mat &image, const cv::Rect &roi, int rows, int cols)
//{
//    if(!isInitialized) {
//        std::cerr << "YOLO model not initialized!" << std::endl;
//        return std::vector<int>(rows*cols, -1);
//    }

//    std::vector<int> cellResults(rows*cols, -1);

//    // Process entire ROI first
//    cv::Mat roiImage = image(roi).clone();

//    // Debug: Save ROI image to check what's being processed
//    cv::imwrite("debug_roi.jpg", roiImage);

//    try {
//        // Pre-process the image to enhance features
//        cv::Mat processedROI;
//        cv::cvtColor(roiImage, processedROI, cv::COLOR_BGR2RGB);

//        // Create blob with the same parameters as your working implementation
//        cv::Mat blob = cv::dnn::blobFromImage(processedROI, 1/255.0,
//                                             cv::Size(416, 416),
//                                             cv::Scalar(0, 0, 0),
//                                             true, false);

//        net.setInput(blob);
//        std::vector<cv::Mat> outs;
//        net.forward(outs, getOutputLayerNames());

//        // Process each cell
//        int cellWidth = roi.width/cols;
//        int cellHeight = roi.height/rows;

//        for(int row = 0; row < rows; ++row) {
//            for(int col = 0; col < cols; ++col) {
//                cv::Rect cellRect(col * cellWidth, row * cellHeight,
//                                cellWidth, cellHeight);

//                // Check detections in this cell's area
//                float maxConfidence = 0.0f;

//                for(const auto& out : outs) {
//                    float* data = (float*)out.data;
//                    for(int i = 0; i < out.rows; ++i, data += out.cols) {
//                        // Get detection coordinates in cell space
//                        float x = data[0] * roiImage.cols;
//                        float y = data[1] * roiImage.rows;

//                        // Check if detection center is in this cell
//                        if(cellRect.contains(cv::Point2f(x, y))) {
//                            float confidence = data[4] * data[5];  // obj_conf * class_conf
//                            maxConfidence = std::max(maxConfidence, confidence);
//                        }
//                    }
//                }

//                // Mark cell if confidence exceeds threshold
//                if(maxConfidence > 0.5f) {  // Using 0.5 threshold as in working example
//                    cellResults[row * cols + col] = row * cols + col + 1;
//                    std::cout << "Detection in cell [" << row << "," << col << "] with conf: "
//                             << maxConfidence << std::endl;
//                }
//            }
//        }
//    }
//    catch(const cv::Exception& e) {
//        std::cerr << "Error processing ROI: " << e.what() << std::endl;
//    }

//    return cellResults;
//}

std::vector<int> DialogDetector::detectObjectsInGridCells(cv::Mat &image, const cv::Rect &roi, int rows, int cols)
{
    if(!isInitialized) {
        std::cerr << "YOLO model not initialized!" << std::endl;
        return std::vector<int>(rows*cols, -1);
    }

    std::vector<int> cellResults(rows*cols, -1);

    // Process entire ROI first
    cv::Mat roiImage = image(roi).clone();

    try {
        // Pre-process the image to enhance features
        cv::Mat processedROI;
        cv::cvtColor(roiImage, processedROI, cv::COLOR_BGR2RGB);

        // Create blob with the same parameters as your working implementation
        cv::Mat blob = cv::dnn::blobFromImage(processedROI, 1/255.0,
                                             cv::Size(416, 416),
                                             cv::Scalar(0, 0, 0),
                                             true, false);

        net.setInput(blob);
        std::vector<cv::Mat> outs;
        net.forward(outs, getOutputLayerNames());

        // Process each cell
        int cellWidth = roi.width/cols;
        int cellHeight = roi.height/rows;

        // Store all detections first
        std::vector<Detection> allDetections;

        for(const auto& out : outs) {
            float* data = (float*)out.data;
            for(int i = 0; i < out.rows; ++i, data += out.cols) {
                float confidence = data[4];
                float classProb = data[5];
                float totalConfidence = confidence * classProb;

                // Increased confidence threshold
                if(totalConfidence > 0.7f) {  // Increased from 0.5 to 0.7
                    float centerX = data[0] * roiImage.cols;
                    float centerY = data[1] * roiImage.rows;
                    float width = data[2] * roiImage.cols;
                    float height = data[3] * roiImage.rows;

                    // Add size check - object should be reasonably sized
                    if(width > cellWidth * 0.2 && height > cellHeight * 0.2) {
                        allDetections.push_back({
                            cv::Point2f(centerX, centerY),
                            totalConfidence,
                            width,
                            height
                        });
                    }
                }
            }
        }

        // Apply non-maximum suppression to remove overlapping detections
        std::vector<int> keepIndices;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        for(const auto& det : allDetections) {
            cv::Rect box(
                det.center.x - det.width/2,
                det.center.y - det.height/2,
                det.width,
                det.height
            );
            boxes.push_back(box);
            confidences.push_back(det.confidence);
        }

        cv::dnn::NMSBoxes(boxes, confidences, 0.7f, 0.3f, keepIndices);

        // Process surviving detections
        for(int idx : keepIndices) {
            const auto& det = allDetections[idx];
            int col = det.center.x / cellWidth;
            int row = det.center.y / cellHeight;

            if(row >= 0 && row < rows && col >= 0 && col < cols) {
                // Additional validation - check if detection takes up significant portion of cell
                float cellArea = cellWidth * cellHeight;
                float detectionArea = det.width * det.height;
                float areaRatio = detectionArea / cellArea;

                if(areaRatio > 0.1f && areaRatio < 0.9f) {  // Detection should take up between 10% and 90% of cell
                    cellResults[row * cols + col] = row * cols + col + 1;
                    std::cout << "Valid detection in cell [" << row << "," << col << "] with conf: "
                             << det.confidence << " area ratio: " << areaRatio << std::endl;
                }
            }
        }
    }
    catch(const cv::Exception& e) {
        std::cerr << "Error processing ROI: " << e.what() << std::endl;
    }

    return cellResults;
}

//void DialogDetector::drawGridAndLabels(cv::Mat &image, const cv::Rect &roi,
//                                     int rows, int cols,
//                                     const std::vector<int> &cellResults)
//{
//    int cellWidth = roi.width/cols;
//    int cellHeight = roi.height/rows;

//    // Draw ROI outline
//    cv::rectangle(image, roi, cv::Scalar(0, 255, 0), 2);

//    // Draw grid
//    for(int row = 1; row < rows; ++row) {
//        cv::line(image,
//                cv::Point(roi.x, roi.y + row * cellHeight),
//                cv::Point(roi.x + roi.width, roi.y + row * cellHeight),
//                cv::Scalar(0, 255, 0), 1);
//    }

//    for(int col = 1; col < cols; ++col) {
//        cv::line(image,
//                cv::Point(roi.x + col * cellWidth, roi.y),
//                cv::Point(roi.x + col * cellWidth, roi.y + roi.height),
//                cv::Scalar(0, 255, 0), 1);
//    }

//    // Draw cell labels
//    for(int row = 0; row < rows; ++row) {
//        for(int col = 0; col < cols; ++col) {
//            int cellIdx = row * cols + col;
//            cv::Point textPos(
//                roi.x + col * cellWidth + 5,
//                roi.y + (row + 1) * cellHeight - 5
//            );

//            if(cellResults[cellIdx] > 0) {
//                // Draw both background and text for better visibility
//                std::string text = std::to_string(cellResults[cellIdx]);
//                int baseline;
//                cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX,
//                                                  0.7, 2, &baseline);

//                cv::rectangle(image,
//                            cv::Point(textPos.x - 2, textPos.y - textSize.height - 2),
//                            cv::Point(textPos.x + textSize.width + 2, textPos.y + 2),
//                            cv::Scalar(0, 0, 0), cv::FILLED);

//                cv::putText(image, text, textPos,
//                          cv::FONT_HERSHEY_SIMPLEX, 0.7,
//                          cv::Scalar(0, 255, 0), 2);
//            }
//        }
//    }
//}


void DialogDetector::drawGridAndLabels(cv::Mat &image, const cv::Rect &roi,
                                     int rows, int cols,
                                     const std::vector<int> &cellResults)
{
    int cellWidth = roi.width/cols;
    int cellHeight = roi.height/rows;

    // Draw outer boundaries first with solid thin line
    cv::rectangle(image, roi, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

    // Draw inner grid lines with slightly different style
    // Vertical lines
    for(int col = 1; col < cols; ++col) {
        int x = roi.x + col * cellWidth;
        cv::line(image,
                 cv::Point(x, roi.y),
                 cv::Point(x, roi.y + roi.height),
                 cv::Scalar(0, 255, 0),
                 1,
                 cv::LINE_AA);
    }

    // Horizontal lines
    for(int row = 1; row < rows; ++row) {
        int y = roi.y + row * cellHeight;
        cv::line(image,
                 cv::Point(roi.x, y),
                 cv::Point(roi.x + roi.width, y),
                 cv::Scalar(0, 255, 0),
                 1,
                 cv::LINE_AA);
    }

    // Draw cell labels
    for(int row = 0; row < rows; ++row)
    {
        for(int col = 0; col < cols; ++col)
        {
            int x = roi.x + col * cellWidth;
            int y = roi.y + row * cellHeight;
            int cellIndex = row * cols + col;

            std::string label = "-1"; // Default label for empty cells
            cv::Scalar textColor(0, 0, 255); // Red for empty cells

            if(cellResults[cellIndex] > 0)
            {
                label = std::to_string(cellResults[cellIndex]);
                textColor = cv::Scalar(0, 255, 0); // Green for detected cells
            }

            // Calculate text position for center alignment
            int fontFace = cv::FONT_HERSHEY_SIMPLEX;
            double fontScale = 0.6;  // Slightly smaller text
            int thickness = 1;  // Thinner text
            cv::Size textSize = cv::getTextSize(label, fontFace, fontScale, thickness, nullptr);

            // Ensure text is centered in cell
            cv::Point textPos(
                x + (cellWidth - textSize.width) / 2,
                y + (cellHeight + textSize.height) / 2
            );

            // Draw text with thin outline for better visibility
            cv::putText(image, label, textPos,
                       fontFace, fontScale,
                       cv::Scalar(0, 0, 0), thickness + 1,
                       cv::LINE_AA);

            cv::putText(image, label, textPos,
                       fontFace, fontScale,
                       textColor, thickness,
                       cv::LINE_AA);
        }
    }
}

void DialogDetector::drawDebugInfo(cv::Mat& image, const cv::Rect& roi, const std::vector<int>& detections)
{
    // Draw ROI boundary
    cv::rectangle(image, roi, cv::Scalar(255, 0, 0), 2);

    // Draw cell grid
    int cellWidth = roi.width / 10;  // Using 10 columns
    int cellHeight = roi.height / 2;  // Using 2 rows

    for(int i = 1; i < 10; i++) {
        cv::line(image,
                 cv::Point(roi.x + i * cellWidth, roi.y),
                 cv::Point(roi.x + i * cellWidth, roi.y + roi.height),
                 cv::Scalar(0, 255, 0), 1);
    }

    cv::line(image,
             cv::Point(roi.x, roi.y + cellHeight),
             cv::Point(roi.x + roi.width, roi.y + cellHeight),
             cv::Scalar(0, 255, 0), 1);

    // Draw detection results
    for(size_t i = 0; i < detections.size(); i++) {
        if(detections[i] > 0) {
            int row = i / 10;
            int col = i % 10;
            cv::Point center(roi.x + col * cellWidth + cellWidth/2,
                           roi.y + row * cellHeight + cellHeight/2);
            cv::circle(image, center, 5, cv::Scalar(0, 0, 255), -1);
        }
    }
}
