# 🎬 AI Scene Detector with AQ/PQ Control

This project is a C++ prototype demonstrating **AI-based Scene Detection** and **automatic AQ/PQ (Audio/Video Quality) switching**.  
It uses **OpenCV (DNN module)** for scene classification and simulates AQ/PQ control managers using multithreading, events, and semaphores.

---

## 🧠 Features
- 🎥 Real-time scene detection using OpenCV DNN  
- 🔊 Automatic AQ (Audio Quality) control based on scene type  
- 🎚️ PQ (Picture Quality) control toggle  
- 🔁 Thread-safe event communication between modules  
- 🧵 Uses C++ threads, semaphores, and mutexes  
- 💡 Modular architecture with 3 managers:
  - `AiSceneDetector` → handles AI model & detection logic  
  - `AqControlManager` → handles Audio Quality states  
  - `PqControlManager` → handles Picture Quality states  

---

## 🗂️ Project Structure

```
AI_SceneDetector/
│
├── AiSceneDetector.cpp
├── AiSceneDetector.h
│
├── AqControlManager.cpp
├── AqControlManager.h
│
├── PqControlManager.cpp
├── PqControlManager.h
│
└── main.cpp
```

---

## ⚙️ Build Instructions

### 🪟 **For Windows (Recommended: MSYS2)**

1. **Install [MSYS2](https://www.msys2.org/)**
2. Open **MSYS2 MINGW64** terminal and run:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-pkg-config mingw-w64-x86_64-opencv make
   ```
3. Verify:
   ```bash
   g++ --version
   pkg-config --cflags --libs opencv4
   ```
4. Clone and build:
   ```bash
   git clone https://github.com/<your-username>/AI_SceneDetector.git
   cd AI_SceneDetector
   g++ main.cpp AiSceneDetector.cpp AqControlManager.cpp PqControlManager.cpp -o AiSceneApp `pkg-config --cflags --libs opencv4` -pthread
   ```
5. Run:
   ```bash
   ./AiSceneApp
   ```

---

### 🐧 **For Linux**
Make sure you have OpenCV and g++ installed:
```bash
sudo apt update
sudo apt install g++ pkg-config libopencv-dev -y
```

Then build:
```bash
g++ main.cpp AiSceneDetector.cpp AqControlManager.cpp PqControlManager.cpp -o AiSceneApp `pkg-config --cflags --libs opencv4` -pthread
```

Run:
```bash
./AiSceneApp
```

---

## 🧩 Usage

Once you run the program:

```
===== AI Scene Detection Demo =====
1. Enable AQ
2. Disable AQ
3. Exit
```

- Selecting **Enable AQ** starts the scene detection thread.  
- Selecting **Disable AQ** stops it and re-enables PQ.  
- Logs show the detected scenes and applied AQ/PQ states.

Example console output:
```
AiSceneDetector Detected Scene: Movie
Applying AQ profile: Cinematic
AQ Enabled Successfully, Start AQ Scene Detection
```

---

## 🎞️ Scene-to-AQ Mapping (Example)

| Detected Scene | Applied AQ Mode |
|----------------|----------------|
| Sports         | Dynamic        |
| Movie          | Cinematic      |
| News           | Voice          |
| Animation      | Balanced       |
| Concert        | Music          |
| Documentary    | Natural        |

---

## 🧱 Dependencies

| Library | Purpose |
|----------|----------|
| OpenCV 4.x | Image capture, DNN model inference |
| pthread | Thread synchronization |
| C++17 | Language standard used |

---

## 🧑‍💻 Author
**Reynard Wilson**  
Manual Tester • Aspiring Automation Engineer  
🔗 [GitHub Profile](https://github.com/reynardPalamattam)

---

## 📄 License
This project is licensed under the **MIT License** — feel free to use and modify for educational or prototyping purposes.

---
