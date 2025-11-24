cmake_minimum_required(VERSION 3.16)

project(SurveillancePanel VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Qml
    Multimedia
    Network
)

# Configure OpenCV
set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Define the executable target
qt_add_executable(surveillance_panel
    src/main.cpp
    src/CameraStream.h
    src/CameraStream.cpp
    src/CameraImageProvider.h
    src/CameraImageProvider.cpp
    src/CameraManager.h
    src/CameraManager.cpp
    src/AlertLogModel.h
    src/AlertLogModel.cpp
    src/ObjectDetector.h
    src/ObjectDetector.cpp
    src/HttpServer.h
    src/HttpServer.cpp
)

# Add QML module with resources
qt_add_qml_module(surveillance_panel
    URI SurveillancePanel
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/CameraTile.qml
        qml/Toast.qml
        qml/RoiOverlay.qml
        qml/AIDetectionOverlay.qml
        qml/AlertLogPanel.qml
)

# Link libraries
target_link_libraries(surveillance_panel PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Multimedia
    Qt6::Network
    ${OpenCV_LIBS}
)

# Set target properties for Windows
if(WIN32)
    set_target_properties(surveillance_panel PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Installation rules (optional)
install(TARGETS surveillance_panel
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)