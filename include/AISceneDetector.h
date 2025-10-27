#ifndef AI_SCENE_DETECTOR_H
#define AI_SCENE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>
#include <vector>

class AiSceneDetector{
  public : 
    AISceneDetector(const std::string& modelPath);
    std::string detectScene(const cv::Mat& frame);
  private:
    cv::dnn::Net net;
    std::vector<std::string> sceneLabels;
};

#endif
