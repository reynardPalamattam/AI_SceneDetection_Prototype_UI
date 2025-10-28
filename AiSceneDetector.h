#ifndef AI_SCENE_DETECTOR_H
#define AI_SCENE_DETECTOR_H

#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "AqControlManager.h"
#include "PqControlManager.h"

enum SceneEvent {
    SCENE_EVENT_INIT_NO_EVENT,
    EVENT_PQ_STATUS_CHANGE,
    EVENT_AQ_STATUS_CHANGE,
    EVENT_AQ_ENABLE,
    EVENT_AQ_DISABLE
};

struct EventParams {
    SceneEvent event;
    bool status = false;
};

class AqControlManager;
class PqControlManager;

class AiSceneDetector {
private:
    static AiSceneDetector *s_instance;
    static PqControlManager *m_pqControlMgr;
    static AqControlManager *m_aqControlMgr;

    std::thread tidAiSceneDetection;
	std::thread tidAiNotifyEvent; 
    std::mutex EventHandl;
    sem_t semNotifyEvent;
    bool exitSceneDetectionThread = false;

    EventParams currentParams;
    SceneEvent currentEvent;

    cv::dnn::Net net;

    AiSceneDetector(const std::string &modelPath = "default_model.onnx");
    void init();

public:
    ~AiSceneDetector();

    static AiSceneDetector *getInstance();

    void notifyEventReciever(const EventParams &params);
    void eventHandler();

    void onPqStatusChanged(bool &status);
    void onAqStatusChanged(bool &status);

    void sceneDetectionFun();
    std::string detectScene(const cv::Mat &frame);
};

#endif

