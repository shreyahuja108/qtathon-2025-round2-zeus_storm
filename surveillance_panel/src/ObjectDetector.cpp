#include "ObjectDetector.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <chrono>

ObjectDetector::ObjectDetector(const std::string &modelPath,
                               const std::string &classNamesPath,
                               float confThreshold,
                               float nmsThreshold)
    : m_confThreshold(confThreshold), m_nmsThreshold(nmsThreshold) {
    
    try {
        // Load the ONNX model
        m_net = cv::dnn::readNetFromONNX(modelPath);
        m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        
        // Load class names
        std::ifstream inputFile(classNamesPath);
        if (inputFile.is_open()) {
            std::string classLine;
            while (std::getline(inputFile, classLine)) {
                if (!classLine.empty()) {
                    m_classNames.push_back(classLine);
                }
            }
            inputFile.close();
            std::cout << "✓ Loaded " << m_classNames.size() << " class names" << std::endl;
            
            // Print first few classes for verification
            for (size_t i = 0; i < std::min((size_t)5, m_classNames.size()); ++i) {
                std::cout << "  Class " << i << ": " << m_classNames[i] << std::endl;
            }
        } else {
            std::cerr << "✗ Failed to open class names file: " << classNamesPath << std::endl;
        }
        
        // Check if model loaded successfully
        if (!m_net.empty() && !m_classNames.empty()) {
            m_loaded = true;
            std::cout << "✓ YOLOv8 ObjectDetector initialized successfully!" << std::endl;
            std::cout << "  Default confidence threshold: " << m_confThreshold << std::endl;
        } else {
            std::cerr << "✗ Failed to load model or class names" << std::endl;
        }
    } catch (const cv::Exception &e) {
        std::cerr << "✗ Error loading model: " << e.what() << std::endl;
        m_loaded = false;
    }
}

bool ObjectDetector::isLoaded() const {
    return m_loaded;
}

void ObjectDetector::setConfidenceThreshold(float conf) {
    m_confThreshold = conf;
    std::cout << "Confidence threshold updated to: " << m_confThreshold << std::endl;
}

float ObjectDetector::confidenceThreshold() const {
    return m_confThreshold;
}

const std::vector<std::string> &ObjectDetector::classNames() const {
    return m_classNames;
}

std::vector<Detection> ObjectDetector::infer(const cv::Mat &frameBgr) {
    // Always start with empty detections
    std::vector<Detection> detections;
    
    if (!m_loaded || frameBgr.empty()) {
        return detections;
    }
    
    try {
        auto start = std::chrono::high_resolution_clock::now();
        
        // Store original dimensions
        int origW = frameBgr.cols;
        int origH = frameBgr.rows;
        
        // Prepare input image with letterboxing
        cv::Mat blob;
        cv::Mat input = letterbox(frameBgr);
        
        // Calculate scale and padding used in letterbox
        float scale = std::min(INPUT_WIDTH / (float)origW, INPUT_HEIGHT / (float)origH);
        int newW = (int)(origW * scale);
        int newH = (int)(origH * scale);
        int padX = (INPUT_WIDTH - newW) / 2;
        int padY = (INPUT_HEIGHT - newH) / 2;
        
        // Create blob
        cv::dnn::blobFromImage(input, blob, 1.0/255.0, 
                               cv::Size(INPUT_WIDTH, INPUT_HEIGHT), 
                               cv::Scalar(), true, false);
        
        // Set input
        m_net.setInput(blob);
        
        // Forward pass
        std::vector<cv::Mat> outputs;
        m_net.forward(outputs, m_net.getUnconnectedOutLayersNames());
        
        // Process output
        cv::Mat output = outputs[0];
        
        // YOLOv8 output is [1, 84, 8400] - reshape for easier processing
        int dimensions = output.size[1];  // 84
        int rows = output.size[2];        // 8400
        
        // Reshape to [84, 8400] and then transpose to [8400, 84]
        output = output.reshape(1, dimensions);
        cv::transpose(output, output);
        
        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;
        
        // Use a reasonable threshold - for hackathon demo, 0.4 works well
        float threshold = std::max(0.4f, m_confThreshold);
        
        // Process each detection
        float* data = (float*)output.data;
        
        for (int i = 0; i < rows; ++i) {
            float* scores = data + 4;  // Skip x,y,w,h
            cv::Mat scoresMat(1, dimensions - 4, CV_32F, scores);
            
            cv::Point classIdPoint;
            double maxScore;
            cv::minMaxLoc(scoresMat, nullptr, &maxScore, nullptr, &classIdPoint);
            
            // Check threshold
            if (maxScore < threshold) {
                data += dimensions;
                continue;
            }
            
            // Get box coordinates
            float cx = data[0];
            float cy = data[1];
            float w = data[2];
            float h = data[3];
            
            // Skip invalid boxes
            if (w <= 0 || h <= 0) {
                data += dimensions;
                continue;
            }
            
            // Convert to original image coordinates
            int x = (int)((cx - w/2 - padX) / scale);
            int y = (int)((cy - h/2 - padY) / scale);
            int width = (int)(w / scale);
            int height = (int)(h / scale);
            
            // Clamp to image bounds
            x = std::max(0, std::min(x, origW - 1));
            y = std::max(0, std::min(y, origH - 1));
            width = std::min(width, origW - x);
            height = std::min(height, origH - y);
            
            // Only keep reasonable sized boxes
            if (width > 20 && height > 20 && width < origW && height < origH) {
                boxes.push_back(cv::Rect(x, y, width, height));
                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)maxScore);
            }
            
            data += dimensions;
        }
        
        // Apply NMS
        std::vector<int> indices;
        if (!boxes.empty()) {
            cv::dnn::NMSBoxes(boxes, confidences, threshold, m_nmsThreshold, indices);
            
            // Build final detections with much tighter boxes
            for (int idx : indices) {
                Detection det;
                det.classId = classIds[idx];
                det.score = confidences[idx];
                
                // Tighten bounding box aggressively by shrinking 22% on each side
                // This makes boxes much tighter around actual objects
                cv::Rect box = boxes[idx];
                int shrinkX = static_cast<int>(box.width * 0.22);
                int shrinkY = static_cast<int>(box.height * 0.22);
                box.x += shrinkX;
                box.y += shrinkY;
                box.width -= shrinkX * 2;
                box.height -= shrinkY * 2;
                
                // Ensure box stays within bounds and has minimum size
                box.x = std::max(0, std::min(box.x, origW - 1));
                box.y = std::max(0, std::min(box.y, origH - 1));
                box.width = std::max(10, std::min(box.width, origW - box.x));
                box.height = std::max(10, std::min(box.height, origH - box.y));
                
                det.box = box;
                detections.push_back(det);
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        // Log results
        if (detections.empty()) {
            std::cout << "No detections (inference: " << ms << "ms)" << std::endl;
        } else {
            std::cout << "Detected " << detections.size() << " objects in " << ms << "ms:" << std::endl;
            for (const auto& det : detections) {
                if (det.classId >= 0 && det.classId < (int)m_classNames.size()) {
                    std::cout << "  • " << m_classNames[det.classId] 
                              << " [" << (int)(det.score * 100) << "%]"
                              << " at (" << det.box.x << "," << det.box.y 
                              << " " << det.box.width << "x" << det.box.height << ")" 
                              << std::endl;
                }
            }
        }
        
    } catch (const cv::Exception &e) {
        std::cerr << "✗ Inference error: " << e.what() << std::endl;
    }
    
    return detections;
}

cv::Mat ObjectDetector::letterbox(const cv::Mat &source) {
    int col = source.cols;
    int row = source.rows;
    int _max = std::max(col, row);
    cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
    source.copyTo(result(cv::Rect(0, 0, col, row)));
    return result;
}