#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>
#include <vector>

struct Detection {
    int classId;
    float score;  // Confidence score in range [0, 1]
    cv::Rect box; // Bounding box in pixel coordinates relative to original frame
};

class ObjectDetector {
public:
    ObjectDetector(const std::string &modelPath,
                   const std::string &classNamesPath,
                   float confThreshold = 0.4f,    // Lower default for better detection
                   float nmsThreshold = 0.45f);

    bool isLoaded() const;

    void setConfidenceThreshold(float conf);
    float confidenceThreshold() const;

    std::vector<Detection> infer(const cv::Mat &frameBgr); // BGR frame

    const std::vector<std::string> &classNames() const;

private:
    cv::dnn::Net m_net;
    std::vector<std::string> m_classNames;
    float m_confThreshold;
    float m_nmsThreshold;
    bool m_loaded = false;

    // YOLOv8 specific constants
    static constexpr int INPUT_WIDTH = 640;
    static constexpr int INPUT_HEIGHT = 640;
    
    // Helper function
    cv::Mat letterbox(const cv::Mat &source);
};

#endif // OBJECTDETECTOR_H