import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    
    // Window properties
    width: 1400
    height: 800
    visible: true
    title: "Multi-Camera Surveillance Panel"
    
    // State management for full-screen view
    property int fullScreenIndex: -1  // -1 = grid view, 1-4 = specific camera
    property bool alertLogVisible: false
    
    // Toast message function
    function showToast(message) {
        toast.show(message)
    }
    
    // Main content area with split layout
    Item {
        anchors.fill: parent
        
        // Main SplitView (for grid mode)
        SplitView {
            id: mainSplitView
            anchors.fill: parent
            orientation: Qt.Horizontal
            visible: fullScreenIndex === -1
            
            // Left side: Camera grid
            Rectangle {
                SplitView.fillWidth: true
                SplitView.minimumWidth: 600
                color: "#2c3e50"
                
                // Grid view (2x2 layout)
                Grid {
                    id: cameraGrid
                    anchors.fill: parent
                    anchors.margins: 16
                    columns: 2
                    rows: 2
                    spacing: 16
                    
                    // Camera 1
                    CameraTile {
                        width: (parent.width - parent.spacing) / 2
                        height: (parent.height - parent.spacing) / 2
                        cameraIndex: 1
                        cameraStream: cameraManager.camera1
                        cameraName: cameraManager.cameraName(1)
                        available: cameraManager.cameraAvailable(1)
                        onFullScreenRequested: fullScreenIndex = 1
                    }
                    
                    // Camera 2
                    CameraTile {
                        width: (parent.width - parent.spacing) / 2
                        height: (parent.height - parent.spacing) / 2
                        cameraIndex: 2
                        cameraStream: cameraManager.camera2
                        cameraName: cameraManager.cameraName(2)
                        available: cameraManager.cameraAvailable(2)
                        onFullScreenRequested: fullScreenIndex = 2
                    }
                    
                    // Camera 3
                    CameraTile {
                        width: (parent.width - parent.spacing) / 2
                        height: (parent.height - parent.spacing) / 2
                        cameraIndex: 3
                        cameraStream: cameraManager.camera3
                        cameraName: cameraManager.cameraName(3)
                        available: cameraManager.cameraAvailable(3)
                        onFullScreenRequested: fullScreenIndex = 3
                    }
                    
                    // Camera 4
                    CameraTile {
                        width: (parent.width - parent.spacing) / 2
                        height: (parent.height - parent.spacing) / 2
                        cameraIndex: 4
                        cameraStream: cameraManager.camera4
                        cameraName: cameraManager.cameraName(4)
                        available: cameraManager.cameraAvailable(4)
                        onFullScreenRequested: fullScreenIndex = 4
                    }
                }
                
                // Toast notification
                Toast {
                    id: toast
                }
            }
            
            // Right side: Alert Log Panel
            AlertLogPanel {
                SplitView.preferredWidth: 400
                SplitView.minimumWidth: 300
                
                onExportRequested: (filePath, format) => {
                    if (format === "png") {
                        showToast("Snapshot exported to PNG")
                    } else if (format === "csv") {
                        showToast("Exported to CSV")
                    } else if (format === "json") {
                        showToast("Exported to JSON")
                    }
                }
                
                onAlertCleared: (count) => {
                    if (count === 1) {
                        showToast("Alert removed")
                    } else {
                        showToast(count + " alerts removed")
                    }
                }
            }
        }
        
        // Fullscreen single camera view
        Loader {
            id: fullscreenLoader
            anchors.fill: parent
            active: fullScreenIndex > 0
            
            sourceComponent: Component {
                CameraTile {
                    anchors.fill: parent
                    cameraIndex: fullScreenIndex
                    cameraStream: {
                        switch(fullScreenIndex) {
                            case 1: return cameraManager.camera1
                            case 2: return cameraManager.camera2
                            case 3: return cameraManager.camera3
                            case 4: return cameraManager.camera4
                            default: return null
                        }
                    }
                    cameraName: cameraManager.cameraName(fullScreenIndex)
                    available: cameraManager.cameraAvailable(fullScreenIndex)
                    isFullScreen: true
                    alertLogVisible: root.alertLogVisible
                    
                    onFullScreenRequested: {
                        // Set to -1 to show grid, not 0
                        fullScreenIndex = -1
                        alertLogVisible = false
                    }
                    
                    onAlertLogToggleRequested: {
                        alertLogVisible = !alertLogVisible
                    }
                }
            }
        }
        
        // Toast for fullscreen mode
        Toast {
            id: fullscreenToast
            visible: fullScreenIndex > 0
        }
    }
    
    // Menu bar
    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem {
                text: "Exit"
                onTriggered: Qt.quit()
            }
        }
        
        Menu {
            title: "&View"
            MenuItem {
                text: "Grid View"
                enabled: fullScreenIndex > 0
                onTriggered: {
                    fullScreenIndex = -1
                    alertLogVisible = false
                }
            }
            MenuSeparator {}
            MenuItem {
                text: "Camera 1 Fullscreen"
                enabled: cameraManager.cameraAvailable(1) && fullScreenIndex !== 1
                onTriggered: fullScreenIndex = 1
            }
            MenuItem {
                text: "Camera 2 Fullscreen"
                enabled: cameraManager.cameraAvailable(2) && fullScreenIndex !== 2
                onTriggered: fullScreenIndex = 2
            }
            MenuItem {
                text: "Camera 3 Fullscreen"
                enabled: cameraManager.cameraAvailable(3) && fullScreenIndex !== 3
                onTriggered: fullScreenIndex = 3
            }
            MenuItem {
                text: "Camera 4 Fullscreen"
                enabled: cameraManager.cameraAvailable(4) && fullScreenIndex !== 4
                onTriggered: fullScreenIndex = 4
            }
            MenuSeparator {}
            MenuItem {
                text: alertLogVisible ? "Hide Alert Log" : "Show Alert Log"
                enabled: fullScreenIndex > 0
                onTriggered: alertLogVisible = !alertLogVisible
            }
        }
        
        Menu {
            title: "&Help"
            MenuItem {
                text: "About"
                enabled: false
            }
        }
    }
}