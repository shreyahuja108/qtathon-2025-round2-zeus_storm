import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    
    // Properties
    property int cameraIndex: 1
    property var cameraStream: null
    property string cameraName: "Camera"
    property bool available: false
    property bool isFullScreen: false
    property bool alertLogVisible: false  // Add this property
    
    // Signals
    signal fullScreenRequested()
    signal alertLogToggleRequested()  

    // ROI/Tripwire edit state
    property bool roiEditActive: false
    property bool tripwireEditActive: false
    
    color: "#34495e"
    radius: 8
    border.color: available && cameraStream && cameraStream.running ? "#3498db" : "#7f8c8d"
    border.width: 2
    
    // ===========================================
    // GRID VIEW LAYOUT (Vertical - when not fullscreen)
    // ===========================================
    ColumnLayout {
        id: gridLayout
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8
        visible: !isFullScreen
        
        // Title bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 35
            color: "#2c3e50"
            radius: 4
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8
                
                Label {
                    text: root.cameraName
                    color: "white"
                    font.pixelSize: 14
                    font.bold: true
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
                
                // Status indicator
                Rectangle {
                    width: 10
                    height: 10
                    radius: 5
                    color: {
                        if (!available || !cameraStream) return "#95a5a6"
                        return cameraStream.running ? "#2ecc71" : "#e74c3c"
                    }
                }
                
                Label {
                    text: {
                        if (!available || !cameraStream) return "N/A"
                        return cameraStream.status
                    }
                    color: "white"
                    font.pixelSize: 12
                }
                
                // Motion indicator
                Rectangle {
                    width: 18
                    height: 18
                    radius: 9
                    color: "#e74c3c"
                    visible: available && cameraStream && cameraStream.motionActive
                    
                    SequentialAnimation on opacity {
                        running: parent.visible
                        loops: Animation.Infinite
                        NumberAnimation { from: 1.0; to: 0.3; duration: 500 }
                        NumberAnimation { from: 0.3; to: 1.0; duration: 500 }
                    }
                }
                
                // Fullscreen button
                Button {
                    text: "⛶"
                    font.pixelSize: 16
                    implicitWidth: 30
                    implicitHeight: 25
                    visible: !isFullScreen && available
                    onClicked: fullScreenRequested()
                    
                    background: Rectangle {
                        color: parent.pressed ? "#1abc9c" : "transparent"
                        radius: 3
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "#ecf0f1"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                
                // Alert log button
                Button {
                    text: "📋"
                    font.pixelSize: 14
                    implicitWidth: 30
                    implicitHeight: 25
                    visible: !isFullScreen
                    onClicked: alertLogToggleRequested()
                    
                    background: Rectangle {
                        color: parent.pressed ? "#e67e22" : "transparent"
                        radius: 3
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "#ecf0f1"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
        
        // Video display area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#000000"
            radius: 4
            clip: true
            
            // Unavailable camera placeholder
            Column {
                anchors.centerIn: parent
                spacing: 12
                visible: !available
                
                Text {
                    text: "❌"
                    font.pixelSize: 48
                    color: "#7f8c8d"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                
                Text {
                    text: "No Camera Configured"
                    font.pixelSize: 16
                    color: "#95a5a6"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                
                Text {
                    text: "Edit config/cameras.json to enable"
                    font.pixelSize: 12
                    color: "#7f8c8d"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
            
            // Available camera video display
            Item {
                id: videoContainer
                anchors.fill: parent
                anchors.margins: 4
                visible: available && cameraStream
                
                Image {
                    id: cameraImage
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: available && cameraStream ? 
                            ("image://camera" + cameraIndex + "/frame?" + 
                             (cameraIndex === 1 ? frameCounter1 :
                              cameraIndex === 2 ? frameCounter2 :
                              cameraIndex === 3 ? frameCounter3 : frameCounter4)) : ""
                    cache: false
                    asynchronous: false
                    
                    // Placeholder when camera stopped (GRID VIEW - Small icon, visible text)
                    Rectangle {
                        anchors.centerIn: parent
                        width: parent.width * 0.8
                        height: parent.height * 0.8
                        color: "transparent"
                        visible: available && cameraStream && !cameraStream.running
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 12
                            
                            Text {
                                text: "📹"
                                font.pixelSize: 32  // Small icon in grid
                                color: "#7f8c8d"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            Text {
                                text: "Camera Stopped"
                                font.pixelSize: 14
                                color: "#ecf0f1"  // Light, visible text
                                font.bold: true
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            Text {
                                text: "Click START to begin streaming"
                                font.pixelSize: 11
                                color: "#bdc3c7"  // Lighter grey
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }
                
                // ROI/Tripwire Overlay
                RoiOverlay {
                    id: roiOverlay
                    anchors.fill: cameraImage
                    cameraIndex: root.cameraIndex
                    cameraStream: root.cameraStream
                    roiEditMode: root.roiEditActive
                    tripwireEditMode: root.tripwireEditActive
                    visible: available && cameraStream && cameraStream.running
                    
                    onEditingFinished: {
                        // Exit edit mode when drawing is complete
                        root.roiEditActive = false
                        root.tripwireEditActive = false
                    }
                }
                
                // AI Detection Overlay
                AIDetectionOverlay {
                    anchors.fill: cameraImage
                    cameraStream: root.cameraStream
                    visible: available && cameraStream && cameraStream.running && cameraStream.aiEnabled
                }
                
                // FPS counter
                Label {
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 4
                    visible: available && cameraStream && cameraStream.running
                    
                    text: available && cameraStream ? (Math.round(cameraStream.fps) + " FPS") : "0 FPS"
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                    
                    background: Rectangle {
                        color: {
                            if (!available || !cameraStream) return "#95a5a6"
                            return cameraStream.fps > 20 ? "#2ecc71" : "#e67e22"
                        }
                        opacity: 0.8
                        radius: 3
                    }
                    
                    padding: 4
                }
            }
        }
        
        // Control buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            spacing: 10
            visible: available && cameraStream

            Button {
                text: "Start"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                enabled: available && cameraStream && !cameraStream.running
                
                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#27ae60" : "#2ecc71") : "#95a5a6"
                    radius: 4
                }
                
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 14
                    font.bold: true
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                onClicked: if (cameraStream) cameraStream.start()
            }
            
            Button {
                text: "Stop"
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                enabled: available && cameraStream && cameraStream.running
                
                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#c0392b" : "#e74c3c") : "#95a5a6"
                    radius: 4
                }
                
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 14
                    font.bold: true
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                onClicked: if (cameraStream) cameraStream.stop()
            }
            
            Button {
                text: "📷"
                Layout.preferredWidth: 60
                Layout.preferredHeight: 40
                enabled: available && cameraStream && cameraStream.running
                
                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#2980b9" : "#3498db") : "#95a5a6"
                    radius: 4
                }
                
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 20
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                onClicked: {
                    if (cameraStream && cameraStream.takeSnapshot()) {
                        // Success feedback could be added here
                    }
                }
            }
        }
    }
    
    // ===========================================
    // FULLSCREEN LAYOUT (Horizontal - side by side)
    // ===========================================
    RowLayout {
        id: fullscreenLayout
        anchors.fill: parent
        spacing: 0
        visible: isFullScreen
        
        // LEFT SIDE: Video Stream (50% width)
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.5
            color: "#1a1a1a"
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4
                
                // Title bar (minimal)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "#2c3e50"
                    radius: 4
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        
                        Label {
                            text: root.cameraName
                            color: "white"
                            font.pixelSize: 18
                            font.bold: true
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        // Status indicator
                        Rectangle {
                            width: 14
                            height: 14
                            radius: 7
                            color: {
                                if (!available || !cameraStream) return "#95a5a6"
                                return cameraStream.running ? "#2ecc71" : "#e74c3c"
                            }
                        }
                        
                        Label {
                            text: {
                                if (!available || !cameraStream) return "N/A"
                                return cameraStream.status
                            }
                            color: "white"
                            font.pixelSize: 14
                        }
                        
                        // Motion indicator
                        Rectangle {
                            width: 22
                            height: 22
                            radius: 11
                            color: "#e74c3c"
                            visible: available && cameraStream && cameraStream.motionActive
                            
                            SequentialAnimation on opacity {
                                running: parent.visible
                                loops: Animation.Infinite
                                NumberAnimation { from: 1.0; to: 0.3; duration: 500 }
                                NumberAnimation { from: 0.3; to: 1.0; duration: 500 }
                            }
                        }
                        
                        // Back button
                        Button {
                            text: "✕"
                            font.pixelSize: 18
                            implicitWidth: 36
                            implicitHeight: 36
                            onClicked: fullScreenRequested()
                            
                            background: Rectangle {
                                color: parent.pressed ? "#e74c3c" : "transparent"
                                radius: 4
                            }
                            
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "#ecf0f1"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
                
                // Video display (fills remaining space)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "#000000"
                    
                    Image {
                        id: fullscreenVideo
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: available && cameraStream ? 
                                ("image://camera" + cameraIndex + "/frame?" + 
                                 (cameraIndex === 1 ? frameCounter1 :
                                  cameraIndex === 2 ? frameCounter2 :
                                  cameraIndex === 3 ? frameCounter3 : frameCounter4)) : ""
                        cache: false
                        asynchronous: false
                        
                        // Camera stopped overlay (FULLSCREEN - Larger icon)
                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width * 0.6
                            height: parent.height * 0.6
                            color: "transparent"
                            visible: available && cameraStream && !cameraStream.running
                            
                            Column {
                                anchors.centerIn: parent
                                spacing: 20
                                
                                Text {
                                    text: "📹"
                                    font.pixelSize: 72
                                    color: "#7f8c8d"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                                
                                Text {
                                    text: "Camera Stopped"
                                    font.pixelSize: 22
                                    color: "#ecf0f1"
                                    font.bold: true
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                                
                                Text {
                                    text: "Click START to begin streaming"
                                    font.pixelSize: 16
                                    color: "#bdc3c7"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                            }
                        }
                    }
                    
                    // ROI/Tripwire Overlay
                    RoiOverlay {
                        anchors.fill: fullscreenVideo
                        cameraIndex: root.cameraIndex
                        cameraStream: root.cameraStream
                        roiEditMode: root.roiEditActive
                        tripwireEditMode: root.tripwireEditActive
                        visible: available && cameraStream && cameraStream.running
                        
                        onEditingFinished: {
                            // Exit edit mode when drawing is complete
                            root.roiEditActive = false
                            root.tripwireEditActive = false
                        }
                    }
                    
                    // AI Detection Overlay
                    AIDetectionOverlay {
                        anchors.fill: fullscreenVideo
                        cameraStream: root.cameraStream
                        visible: available && cameraStream && cameraStream.running && cameraStream.aiEnabled
                    }
                    
                    // FPS Counter (larger in fullscreen, positioned on left)
                    Label {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.margins: 12
                        text: available && cameraStream ? (Math.round(cameraStream.fps) + " FPS") : "0 FPS"
                        color: "white"
                        font.pixelSize: 16
                        font.bold: true
                        visible: available && cameraStream && cameraStream.running
                        
                        background: Rectangle {
                            color: {
                                if (!available || !cameraStream) return "#95a5a6"
                                return cameraStream.fps > 20 ? "#2ecc71" : "#e67e22"
                            }
                            opacity: 0.85
                            radius: 4
                        }
                        
                        padding: 6
                    }
                }
            }
        }
        
        // RIGHT SIDE: Controls (50% width)
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.5
            color: "#34495e"
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                
                // Alert Log Panel (if visible, takes top 50% of right side)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: alertLogVisible ? parent.height * 0.5 : 0
                    visible: alertLogVisible
                    color: "#2c3e50"
                    clip: true
                    
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0
                        
                        // Alert log header
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            color: "#34495e"
                            
                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                
                                Label {
                                    text: "Alert Log"
                                    color: "white"
                                    font.pixelSize: 16
                                    font.bold: true
                                }
                                
                                Item { Layout.fillWidth: true }
                                
                                Button {
                                    text: "✕"
                                    onClicked: alertLogToggleRequested()
                                    implicitWidth: 32
                                    implicitHeight: 32
                                    
                                    background: Rectangle {
                                        color: parent.pressed ? "#e74c3c" : "transparent"
                                        radius: 4
                                    }
                                    
                                    contentItem: Text {
                                        text: parent.text
                                        font.pixelSize: 16
                                        color: "#ecf0f1"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }
                        
                        // Alert log content
                        AlertLogPanel {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            
                            onExportRequested: (filePath, format) => {
                                // Handle export - could show a toast notification
                            }
                            
                            onAlertCleared: (count) => {
                                // Handle alert cleared - could show a toast notification
                            }
                        }
                    }
                }
                
                // Controls Section (takes remaining space or full height if alert log closed)
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    contentWidth: availableWidth  // Prevent horizontal scrolling
                    
                    ColumnLayout {
                        width: parent.width
                        spacing: 16
                        
                        Item { height: 12 } // Top padding
                        
                        // START/STOP/SNAPSHOT buttons
                        RowLayout {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 60
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            spacing: 12
                            
                            Button {
                                text: "▶ START"
                                Layout.fillWidth: true
                                Layout.preferredHeight: 60
                                enabled: available && cameraStream && !cameraStream.running
                                
                                background: Rectangle {
                                    color: parent.enabled ? (parent.pressed ? "#27ae60" : "#2ecc71") : "#7f8c8d"
                                    radius: 6
                                }
                                
                                contentItem: Text {
                                    text: parent.text
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                
                                onClicked: if (cameraStream) cameraStream.start()
                            }
                            
                            Button {
                                text: "⏹ STOP"
                                Layout.fillWidth: true
                                Layout.preferredHeight: 60
                                enabled: available && cameraStream && cameraStream.running
                                
                                background: Rectangle {
                                    color: parent.enabled ? (parent.pressed ? "#c0392b" : "#e74c3c") : "#7f8c8d"
                                    radius: 6
                                }
                                
                                contentItem: Text {
                                    text: parent.text
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                
                                onClicked: if (cameraStream) cameraStream.stop()
                            }
                            
                            Button {
                                text: "📷"
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 60
                                enabled: available && cameraStream && cameraStream.running
                                
                                background: Rectangle {
                                    color: parent.enabled ? (parent.pressed ? "#2980b9" : "#3498db") : "#7f8c8d"
                                    radius: 6
                                }
                                
                                contentItem: Text {
                                    text: parent.text
                                    font.pixelSize: 28
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                
                                onClicked: {
                                    if (cameraStream && cameraStream.takeSnapshot()) {
                                        // Success feedback
                                    }
                                }
                            }
                        }
                        
                        // Motion Detection section
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 140
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            color: "#2c3e50"
                            radius: 6
                            visible: available && cameraStream
                            
                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 16
                                spacing: 12
                                
                                Label {
                                    text: "🔍 Motion Detection"
                                    color: "white"
                                    font.pixelSize: 16
                                    font.bold: true
                                }
                                
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 12
                                    
                                    Label {
                                        text: "Enable"
                                        color: "white"
                                        font.pixelSize: 14
                                        Layout.fillWidth: true
                                    }
                                    
                                    Switch {
                                        checked: cameraStream ? cameraStream.motionEnabled : false
                                        onToggled: if (cameraStream) cameraStream.motionEnabled = checked
                                    }
                                }
                                
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 12
                                    
                                    Label {
                                        text: "Sensitivity"
                                        color: "white"
                                        font.pixelSize: 13
                                    }
                                    
                                    Slider {
                                        Layout.fillWidth: true
                                        from: 0.1
                                        to: 1.0
                                        value: cameraStream ? cameraStream.motionSensitivity : 0.5
                                        onMoved: if (cameraStream) cameraStream.motionSensitivity = value
                                        
                                        background: Rectangle {
                                            x: parent.leftPadding
                                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                                            implicitWidth: 200
                                            implicitHeight: 6
                                            width: parent.availableWidth
                                            height: implicitHeight
                                            radius: 3
                                            color: "#34495e"
                                            
                                            Rectangle {
                                                width: parent.parent.visualPosition * parent.width
                                                height: parent.height
                                                color: "#3498db"
                                                radius: 3
                                            }
                                        }
                                        
                                        handle: Rectangle {
                                            x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                                            implicitWidth: 24
                                            implicitHeight: 24
                                            radius: 12
                                            color: parent.pressed ? "#2980b9" : "#3498db"
                                            border.color: "white"
                                            border.width: 3
                                        }
                                    }
                                    
                                    Label {
                                        text: cameraStream ? Math.round(cameraStream.motionSensitivity * 100) + "%" : "50%"
                                        color: "#3498db"
                                        font.pixelSize: 14
                                        font.bold: true
                                        Layout.preferredWidth: 55
                                    }
                                }
                            }
                        }
                        
                        // ROI/Tripwire section
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 200
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            color: "#2c3e50"
                            radius: 6
                            visible: available && cameraStream
                            
                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 16
                                spacing: 16
                                
                                Label {
                                    text: "🎯 ROI & Tripwire Detection"
                                    color: "white"
                                    font.pixelSize: 16
                                    font.bold: true
                                }
                                
                                // ROI controls
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 10
                                    
                                    Label {
                                        text: "ROI:"
                                        color: "#3498db"
                                        font.pixelSize: 13
                                        font.bold: true
                                        Layout.preferredWidth: 50
                                    }
                                    
                                    Button {
                                        text: "Edit"
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 40
                                        
                                        background: Rectangle {
                                            color: parent.pressed ? "#2980b9" : "#3498db"
                                            radius: 5
                                        }
                                        
                                        contentItem: Text {
                                            text: parent.text
                                            font.pixelSize: 13
                                            font.bold: true
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        
                                        onClicked: {
                                            roiEditActive = !roiEditActive
                                            if (roiEditActive && tripwireEditActive) {
                                                tripwireEditActive = false
                                            }
                                        }
                                    }
                                    
                                    Button {
                                        text: "Clear"
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 40
                                        
                                        background: Rectangle {
                                            color: parent.pressed ? "#c0392b" : "#e74c3c"
                                            radius: 5
                                        }
                                        
                                        contentItem: Text {
                                            text: parent.text
                                            font.pixelSize: 13
                                            font.bold: true
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        
                                        onClicked: {
                                            cameraManager.clearRoi(root.cameraIndex)
                                            roiEditActive = false
                                        }
                                    }
                                    
                                    CheckBox {
                                        id: roiAutoSnap
                                        checked: cameraStream ? cameraStream.autoSnapshotOnRoi : false
                                        onToggled: if (cameraStream) cameraStream.autoSnapshotOnRoi = checked
                                        Layout.preferredWidth: 40
                                        
                                        ToolTip.visible: hovered
                                        ToolTip.text: "Auto-snapshot"
                                        
                                        indicator: Rectangle {
                                            implicitWidth: 28
                                            implicitHeight: 28
                                            radius: 5
                                            border.color: "#3498db"
                                            border.width: 2
                                            color: parent.checked ? "#3498db" : "#34495e"
                                            
                                            Text {
                                                anchors.centerIn: parent
                                                text: "📷"
                                                font.pixelSize: 16
                                                visible: parent.parent.checked
                                            }
                                        }
                                    }
                                }
                                
                                // Tripwire controls
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 10
                                    
                                    Label {
                                        text: "Line:"
                                        color: "#e67e22"
                                        font.pixelSize: 13
                                        font.bold: true
                                        Layout.preferredWidth: 50
                                    }
                                    
                                    Button {
                                        text: "Edit"
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 40
                                        
                                        background: Rectangle {
                                            color: parent.pressed ? "#d68910" : "#e67e22"
                                            radius: 5
                                        }
                                        
                                        contentItem: Text {
                                            text: parent.text
                                            font.pixelSize: 13
                                            font.bold: true
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        
                                        onClicked: {
                                            tripwireEditActive = !tripwireEditActive
                                            if (tripwireEditActive && roiEditActive) {
                                                roiEditActive = false
                                            }
                                        }
                                    }
                                    
                                    Button {
                                        text: "Clear"
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 40
                                        
                                        background: Rectangle {
                                            color: parent.pressed ? "#c0392b" : "#e74c3c"
                                            radius: 5
                                        }
                                        
                                        contentItem: Text {
                                            text: parent.text
                                            font.pixelSize: 13
                                            font.bold: true
                                            color: "white"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        
                                        onClicked: {
                                            cameraManager.clearTripwire(root.cameraIndex)
                                            tripwireEditActive = false
                                        }
                                    }
                                    
                                    CheckBox {
                                        id: tripwireAutoSnap
                                        checked: cameraStream ? cameraStream.autoSnapshotOnTripwire : false
                                        onToggled: if (cameraStream) cameraStream.autoSnapshotOnTripwire = checked
                                        Layout.preferredWidth: 40
                                        
                                        ToolTip.visible: hovered
                                        ToolTip.text: "Auto-snapshot"
                                        
                                        indicator: Rectangle {
                                            implicitWidth: 28
                                            implicitHeight: 28
                                            radius: 5
                                            border.color: "#e67e22"
                                            border.width: 2
                                            color: parent.checked ? "#e67e22" : "#34495e"
                                            
                                            Text {
                                                anchors.centerIn: parent
                                                text: "📷"
                                                font.pixelSize: 16
                                                visible: parent.parent.checked
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        
                        // AI Detection section
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 170
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            color: "#2c3e50"
                            radius: 6
                            visible: available && cameraStream
                            
                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 16
                                spacing: 12
                                
                                Label {
                                    text: "🤖 AI Object Detection"
                                    color: "white"
                                    font.pixelSize: 16
                                    font.bold: true
                                }
                                
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 12
                                    
                                    Label {
                                        text: "Enable AI"
                                        color: "white"
                                        font.pixelSize: 14
                                        Layout.fillWidth: true
                                    }
                                    
                                    Switch {
                                        checked: cameraStream ? cameraStream.aiEnabled : false
                                        onToggled: if (cameraStream) cameraStream.aiEnabled = checked
                                    }
                                }
                                
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 12
                                    
                                    Label {
                                        text: "Confidence"
                                        color: "white"
                                        font.pixelSize: 13
                                    }
                                    
                                    Slider {
                                        Layout.fillWidth: true
                                        from: 0.3
                                        to: 0.9
                                        value: cameraStream ? cameraStream.aiConfidenceThreshold : 0.5
                                        onMoved: if (cameraStream) cameraStream.aiConfidenceThreshold = value
                                        
                                        background: Rectangle {
                                            x: parent.leftPadding
                                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                                            implicitWidth: 200
                                            implicitHeight: 6
                                            width: parent.availableWidth
                                            height: implicitHeight
                                            radius: 3
                                            color: "#34495e"
                                            
                                            Rectangle {
                                                width: parent.parent.visualPosition * parent.width
                                                height: parent.height
                                                color: "#9b59b6"
                                                radius: 3
                                            }
                                        }
                                        
                                        handle: Rectangle {
                                            x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                                            y: parent.topPadding + parent.availableHeight / 2 - height / 2
                                            implicitWidth: 24
                                            implicitHeight: 24
                                            radius: 12
                                            color: parent.pressed ? "#8e44ad" : "#9b59b6"
                                            border.color: "white"
                                            border.width: 3
                                        }
                                    }
                                    
                                    Label {
                                        text: cameraStream ? Math.round(cameraStream.aiConfidenceThreshold * 100) + "%" : "50%"
                                        color: "#9b59b6"
                                        font.pixelSize: 14
                                        font.bold: true
                                        Layout.preferredWidth: 55
                                    }
                                }
                                
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 12
                                    
                                    Label {
                                        text: "Auto-snapshot on detection"
                                        color: "white"
                                        font.pixelSize: 13
                                        Layout.fillWidth: true
                                    }
                                    
                                    CheckBox {
                                        checked: cameraStream ? cameraStream.autoSnapshotOnMotion : false
                                        onToggled: if (cameraStream) cameraStream.autoSnapshotOnMotion = checked
                                        
                                        indicator: Rectangle {
                                            implicitWidth: 28
                                            implicitHeight: 28
                                            radius: 5
                                            border.color: "#9b59b6"
                                            border.width: 2
                                            color: parent.checked ? "#9b59b6" : "#34495e"
                                            
                                            Text {
                                                anchors.centerIn: parent
                                                text: "✓"
                                                color: "white"
                                                font.pixelSize: 16
                                                font.bold: true
                                                visible: parent.parent.checked
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        
                        // Alert Log toggle button (only when not already visible)
                        Button {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 60
                            Layout.leftMargin: 16
                            Layout.rightMargin: 16
                            visible: !alertLogVisible
                            text: "📋 Show Alert Log"
                            
                            background: Rectangle {
                                color: parent.pressed ? "#d68910" : "#e67e22"
                                radius: 6
                            }
                            
                            contentItem: Text {
                                text: parent.text
                                font.pixelSize: 16
                                font.bold: true
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            
                            onClicked: alertLogToggleRequested()
                        }
                        
                        Item { height: 16 } // Bottom padding
                    }
                }
            }
        }
    }
}
