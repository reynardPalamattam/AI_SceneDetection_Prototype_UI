#include <iostream>
#include <thread>
#include <chrono>
#include "AiSceneDetector.h"

using namespace std;

int main() {
	
    cout << "===== AI Scene Detection Demo =====" << endl;

    AiSceneDetector *sceneDetector = AiSceneDetector::getInstance();

    bool running = true;
	
    while (running) {
        cout << "\n----------------------------" << endl;
        cout << "1. Enable AQ" << endl;
        cout << "2. Disable AQ" << endl;
        cout << "3. Exit" << endl;
        cout << "----------------------------" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "[MAIN] Request to enable AQ" << endl;
				EventParams newEvent =  { EVENT_AQ_ENABLE}; 
	
				if(sceneDetector != nullptr){
					
					sceneDetector->notifyEventReciever(newEvent);
					
				}
                break;

            case 2:
                cout << "[MAIN] Request to disable AQ" << endl;
                EventParams newEvent =  { EVENT_AQ_DISABLE}; 
	
				if(sceneDetector != nullptr){
					
					sceneDetector->notifyEventReciever(newEvent);
					
				}
                break;

            case 3:
                 cout << "[MAIN] Exiting program..." << endl;
                running = false;
                break;

            default:
                cout << "Invalid option, try again!" << endl;
                break;
        }

        this_thread::sleep_for(chrono::milliseconds(5000));
    }

    cout << "Program complete. Goodbye!!!" << endl;
    return 0;
}

