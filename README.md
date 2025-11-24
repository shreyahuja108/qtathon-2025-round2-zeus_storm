🛡️ Surveillance Panel
Intelligent Multi-Camera Monitoring System

Built with Qt 6, C++, QML, OpenCV, YOLOv8 (ONNX)
Hackathon 2025 – Round 2 (Team Zeus Storm)

🌟 1. Introduction

surveillance_panel is a real-time, privacy-focused, AI-enhanced multi-camera monitoring system built using the required stack: Qt 6 (C++/QML) and OpenCV.

The system offers:

Reliable multi-camera streaming

Motion analysis

User-defined regions (ROI/tripwire)

Event logging

Snapshot capture

AI detection (YOLOv8 ONNX)

All processing happens locally, ensuring low latency and complete user control.

🔧 2. Tech Stack (Why These Choices?)
Qt 6 (C++ + QML)

Used to build a responsive, modern UI with GPU-accelerated rendering and smooth animations.
Qt’s QML layout system is ideal for multi-camera dashboards.

OpenCV 4.x

Provides:

Video capture (USB + RTSP)

Motion detection

Preprocessing for AI

BGR → RGB conversion

YOLOv8n (ONNX)

Lightweight object detection model that runs entirely on the CPU through OpenCV DNN.

CMake

Cross-platform, modern build system required for Qt 6.

🏛️ 3. System Architecture
## 🏛️ 3. System Architecture

The system is split into four main layers:

1. **CameraStream (C++)**
   - Captures frames from USB / RTSP / IP cameras
   - Estimates FPS
   - Performs basic motion detection
   - Evaluates ROI/tripwire rules and raises low-level events

2. **AI ObjectDetector (C++)**
   - Runs YOLOv8n (ONNX) inference on selected frames
   - Detects objects (person, car, dog, cat, bicycle, etc.)
   - Normalizes bounding boxes for QML overlay
   - Provides class labels + confidence scores

3. **Alert Manager**
   - Receives events from Motion / ROI / Tripwire / AI
   - Creates structured alert entries (time, camera, type, message)
   - Manages snapshots on disk
   - Supports CSV/JSON export for the alert log

4. **QML Frontend UI**
   - Grid layout for 2–4 camera tiles
   - Fullscreen camera view with controls (motion, ROI, tripwire, AI)
   - Alert Log panel with filtering, preview, export, and clearing
   - Toast notifications for important events

🎥 4. Features
✔ Multi-Camera Support

2–4 camera tiles

USB webcams

RTSP / IP cameras

Phone cameras via IP Webcam / DroidCam

Per-tile FPS

Compact tile mode + fullscreen mode

✔ Motion Detection

Lightweight pixel-diff detection

Adjustable sensitivity

Real-time alert generation

Optional automatic snapshot

✔ ROI (Region of Interest)

Draw polygon ROI

Alerts only inside the defined region

Editable and clearable

✔ Tripwire Tool

Draw a single directional line

Alerts when an object crosses the line

Direction-aware (left→right / right→left)

✔ Snapshots

Manual snapshot per camera

Snapshots added to Alert Log Panel

Optional auto-snapshot for:

Motion

ROI

Tripwire

Save as PNG through alert panel

Multi-selection + multi-export

✔ Alert Log Panel

Timestamped logs

Event types:

Motion

ROI

Tripwire

Manual snapshot

AI detection (if enabled)

Export selected or all alerts:

CSV

JSON

Delete selected or all

Smooth toast notifications

🤖 5. AI Intelligence (Bonus A – Implemented)

We integrated optional AI detection using YOLOv8n (ONNX) with:

Person

Dog

Cat

Car

Bicycle

Bounding boxes include:

Class label

Confidence

Color-coded outline

Smooth QML drawing

Combined with tracking, this enables:

Stable detections

Better tripwire performance

Direction estimation

Loitering detection foundations

AI processing runs every 5 frames for excellent performance.

🧱 6. Challenges & Engineering Notes

Aligning OpenCV coordinates with QML layout required careful normalization

Maintaining smooth performance with 4 cameras + AI

Ensuring thread safety between camera threads, AI threads, and UI

Creating clean QML overlays for bounding boxes and ROI/tripwire tools

Managing snapshots efficiently without blocking UI

📦 7. Project Structure
surveillance_panel/
    src/
    qml/
    assets/models/
    CMakeLists.txt


ONNX model & COCO labels must be placed in assets/models/.

🏁 8. Build & Run
Requirements

Qt 6.10+

CMake

OpenCV 4.x

YOLOv8n ONNX model (yolov8n.onnx)

COCO labels (coco.names)

Steps

Open surveillance_panel/CMakeLists.txt in Qt Creator

Select MSVC 2022 64-bit kit

Configure

Build

Run

Add any USB webcam or mobile RTSP stream and begin monitoring.

🙌 9. Team

Team Zeus Storm - shrey.ahuja@smu.ca
Designed and implemented for Hackathon 2025 – Round 2.
