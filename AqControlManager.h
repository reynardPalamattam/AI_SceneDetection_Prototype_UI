#ifndef AQ_CONTROL_MANAGER_H
#define AQ_CONTROL_MANAGER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include "AiSceneDetector.h"

class AiSceneDetector;

class AqControlManager {
private:
    static AqControlManager *s_instance;
    static AiSceneDetector *m_AiSceneDetecton;

    std::thread tidAiPqSceneCb;
    sem_t semAqCallback;
    bool AqStatus = true;
    bool exitThreadAqCB = false;

    AqControlManager();
    void init();
    void statusCallBack();

public:
    ~AqControlManager();

    static AqControlManager *getInstance(AiSceneDetector *Scene_detection_instance);

    bool getAqStatus();
    void controlAq(bool status);
};

#endif

