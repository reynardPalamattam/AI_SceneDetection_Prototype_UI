#ifndef PQ_CONTROL_MANAGER_H
#define PQ_CONTROL_MANAGER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include "AiSceneDetector.h"

class AiSceneDetector;

class PqControlManager {
private:
    static PqControlManager *s_instance;
    static AiSceneDetector *m_AiSceneDetecton;

    std::thread tidAiPqSceneCb;
    sem_t semPqCallback;
    bool PqStatus = true;
    bool exitThreadPqCB = false;

    PqControlManager();
    void init();
    void statusCallBack();

public:
    ~PqControlManager();

    static PqControlManager *getInstance(AiSceneDetector *Scene_detection_instance);

    bool getPqStatus();
    void controlPq(bool status);
};

#endif

