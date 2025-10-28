#include "AqControlManager.h"
#define TAG "AqControlMgr"
using namespace std;

AqControlManager* AqControlManager::s_instance = nullptr;
AiSceneDetector* AqControlManager::m_AiSceneDetecton = nullptr;

AqControlManager::AqControlManager() {
	
    cout << TAG << " Constructor" << endl;
    sem_init(&semAqCallback, 0, 0);

    exitThreadAqCB = false;
    tidAiPqSceneCb = std::thread(&AqControlManager::statusCallBack, this);
}

AqControlManager::~AqControlManager() {
	
    cout << TAG << " Destructor" << endl;
	
    exitThreadAqCB = true;
    sem_post(&semAqCallback);

    if (tidAiPqSceneCb.joinable()) {
		
        tidAiPqSceneCb.join();
    }
}

AqControlManager* AqControlManager::getInstance(AiSceneDetector *Scene_detection_instance) {
	
    if (m_AiSceneDetecton == nullptr) {
		
        m_AiSceneDetecton = Scene_detection_instance;
    }

    if (s_instance == nullptr) {
		
        s_instance = new AqControlManager();
        s_instance->init();
    }

    return s_instance;
}

void AqControlManager::init() {
	
    cout << TAG << " Init!!!" << endl;
    AqStatus = true;
}

bool AqControlManager::getAqStatus() {
	
    return AqStatus;
}

void AqControlManager::controlAq(bool status) {
	
    AqStatus = status;
    cout << TAG << (status ? " Enable AQ" : " Disable AQ") << endl;
    sem_post(&semAqCallback);
}

void AqControlManager::statusCallBack() {
	
    while (true) {
		
        sem_wait(&semAqCallback);

        if (exitThreadAqCB) {
			
            break;
        }

        bool returnStatus = AqStatus;
        EventParams newEvent = { EVENT_AQ_STATUS_CHANGE, returnStatus };

        if (m_AiSceneDetecton != nullptr) {
			
            m_AiSceneDetecton->notifyEventReciever(newEvent);
        }
    }
}

