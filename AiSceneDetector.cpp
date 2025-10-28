#include "AiSceneDetector.h"
#define TAG "AiSceneDetector"
using namespace std;

AiSceneDetector* AiSceneDetector::s_instance = nullptr;
PqControlManager* AiSceneDetector::m_pqControlMgr = nullptr;
AqControlManager* AiSceneDetector::m_aqControlMgr = nullptr;

bool exitEventHandleThread = false;

AiSceneDetector::AiSceneDetector(const std::string& modelPath) {
	
    cout << TAG << " Constructor" << endl;

    try {
		
        net = cv::dnn::readNetFromONNX(modelPath);
        cout << TAG << " Model loaded successfully from: " << modelPath << endl;
		
    } catch (const cv::Exception& e) {
		
        cerr << TAG << " Failed to load model: " << e.what() << endl;
    }

    sem_init(&semNotifyEvent, 0, 0);
}

AiSceneDetector::~AiSceneDetector() {
	
    cout << TAG << " Destructor" << endl;
	
    exitEventHandleThread = true;
    sem_post(&semNotifyEvent);
	
	if (tidAiNotifyEvent.joinable()) {
		
        tidAiNotifyEvent.join();
    }

    exitSceneDetectionThread = true;
	
    if (tidAiSceneDetection.joinable()) {
		
        tidAiSceneDetection.join();
    }
}

AiSceneDetector *AiSceneDetector::getInstance() {
	
    cout << TAG << " getInstance" << endl;

    if (s_instance == nullptr) {
		
        s_instance = new AiSceneDetector("model.onnx");
        m_pqControlMgr = PqControlManager::getInstance(s_instance);
        m_aqControlMgr = AqControlManager::getInstance(s_instance);
        s_instance->init();

    }

    return s_instance;
}

void AiSceneDetector::init() {
	
    cout << TAG << " init !!" << endl;
    exitEventHandleThread = false;
    tidAiNotifyEvent = std::thread(&AiSceneDetector::eventHandler, this);
	
}

void AiSceneDetector::notifyEventReciever(const EventParams &params) {
	
    EventHandl.lock();

    if (params.event != SCENE_EVENT_INIT_NO_EVENT) {
        currentEvent = params.event;
        currentParams = params;
        sem_post(&semNotifyEvent);
    }
	
	EventHandl.unlock();
}

void AiSceneDetector::eventHandler() {
	
    while (true) {
		
        cout << TAG << " sem_wait in semNotifyEvent" << endl;
        sem_wait(&semNotifyEvent);

        if (exitEventHandleThread) {
			
            break;
        }

        EventHandl.lock();

        switch (currentParams.event) {
			
            case EVENT_PQ_STATUS_CHANGE:
			{
                onPqStatusChanged(currentParams.status);
                break;
			}
            case EVENT_AQ_STATUS_CHANGE:
			{
                onAqStatusChanged(currentParams.status);
                break;
			}
            case EVENT_AQ_ENABLE:
			{
                if (m_pqControlMgr->getPqStatus()) {
					
                    cout << TAG << " PQ is Enabled, disable PQ" << endl;
                    m_pqControlMgr->controlPq(false);
					
                } else {
					
                    cout << TAG << " PQ is already disabled, Enable AQ" << endl;
                    m_aqControlMgr->controlAq(true);
                }
                break;
			}
            case EVENT_AQ_DISABLE:
			{
                cout << TAG << " AQ Disable request, Disable AQ" << endl;
                m_aqControlMgr->controlAq(false);
                break;
			}
            default:
                cout << TAG << " Undefined Event, No action taken!!" << endl;
                break;
        }
		
		EventHandl.unlock();
		
    }
}

void AiSceneDetector::onPqStatusChanged(bool &status) {
	
    if (status) {
		
        cout << TAG << " PQ Enabled Successfully" << endl;
		
    } else {
		
        cout << TAG << " PQ Disabled, Enabling AQ" << endl;
		
        if (m_aqControlMgr) {
			
			m_aqControlMgr->controlAq(true);
		}
    }
}

void AiSceneDetector::onAqStatusChanged(bool &status) {
	
    if (status) {
		
        cout << TAG << " AQ Enabled Successfully, Start AQ Scene Detection" << endl;

        exitSceneDetectionThread = true;
        if (tidAiSceneDetection.joinable()) {
			
            tidAiSceneDetection.join();
        }

        exitSceneDetectionThread = false;
        tidAiSceneDetection = std::thread(&AiSceneDetector::sceneDetectionFun, this);

    } else {
		
        cout << TAG << " AQ Disabled Successfully, Stop AQ Scene Detection" << endl;

        exitSceneDetectionThread = true;
        if (tidAiSceneDetection.joinable()) {
			
            tidAiSceneDetection.join();
        }

        if (m_pqControlMgr) {
			
			m_pqControlMgr->controlPq(true);
		}
    }
}

void AiSceneDetector::sceneDetectionFun() {
	
    cv::VideoCapture cap(0); // webcam
    if (!cap.isOpened()) {
		
        cerr << TAG << " Failed to open camera!" << endl;
        return;
    }

    while (!exitSceneDetectionThread) {
		
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
			
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        std::string scene = detectScene(frame);
        cout << TAG << " Detected Scene: " << scene << endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

std::string AiSceneDetector::detectScene(const cv::Mat &frame) {
	
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0 / 255.0, cv::Size(224, 224));
    net.setInput(blob);
    cv::Mat output = net.forward();

    cv::Point classIdPoint;
    double confidence;
    cv::minMaxLoc(output, nullptr, &confidence, nullptr, &classIdPoint);
    int classId = classIdPoint.x;

    std::vector<std::string> sceneLabels = {"Sports", "Movie", "News", "Animation", "Concert", "Documentary"};

    if (classId < 0 || classId >= static_cast<int>(sceneLabels.size())) {
		
        return "Unknown";
    }

    return sceneLabels[classId];
}

