#include "PqControlManager.h"
#define TAG "PqControlMgr"
using namespace std;

PqControlManager* PqControlManager::s_instance = nullptr;
AiSceneDetector* PqControlManager::m_AiSceneDetecton = nullptr;

PqControlManager::PqControlManager() {
	
    cout << TAG << " Constructor" << endl;
    sem_init(&semPqCallback, 0, 0);

    exitThreadPqCB = false;
    tidAiPqSceneCb = std::thread(&PqControlManager::statusCallBack, this);
}

PqControlManager::~PqControlManager() {
	
    cout << TAG << " Destructor" << endl;
	
    exitThreadPqCB = true;
    sem_post(&semPqCallback);

    if (tidAiPqSceneCb.joinable()) {
		
        tidAiPqSceneCb.join();
    }
}

PqControlManager* PqControlManager::getInstance(AiSceneDetector *Scene_detection_instance) {
	
    if (m_AiSceneDetecton == nullptr) {
		
        m_AiSceneDetecton = Scene_detection_instance;
    }

    if (s_instance == nullptr) {
		
        s_instance = new PqControlManager();
        s_instance->init();
    }

    return s_instance;
}

void PqControlManager::init() {
	
    cout << TAG << " Init!!!" << endl;
    PqStatus = true;
}

bool PqControlManager::getPqStatus() {
	
    return PqStatus;
}

void PqControlManager::controlPq(bool status) {
	
    PqStatus = status;
    cout << TAG << (status ? " Enable PQ" : " Disable PQ") << endl;
    sem_post(&semPqCallback);
}

void PqControlManager::statusCallBack() {
	
    while (true) {
		
        sem_wait(&semPqCallback);

        if (exitThreadPqCB) {
			
            break;
        }

        bool returnStatus = PqStatus;
        EventParams newEvent = { EVENT_PQ_STATUS_CHANGE, returnStatus };

        if (m_AiSceneDetecton != nullptr) {
            m_AiSceneDetecton->notifyEventReciever(newEvent);
        }
    }
}

