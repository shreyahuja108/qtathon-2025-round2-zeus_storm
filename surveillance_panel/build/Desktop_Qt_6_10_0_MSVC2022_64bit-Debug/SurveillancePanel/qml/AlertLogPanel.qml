import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Rectangle {
    id: root
    
    color: "#34495e"
    
    // Signals
    signal exportRequested(string filePath, string format)
    signal alertCleared(int count)
    
    // Selection mode state
    property bool selectionMode: false
    property var selectedIndices: ([])
    
    // Popover state
    property int activePopoverIndex: -1
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12
        
        // Header
        Text {
            text: "Alert Log"
            font.pixelSize: 20
            font.bold: true
            color: "#ecf0f1"
            Layout.fillWidth: true
        }
        
        // Toolbar
        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            
            Text {
                text: selectionMode && selectedIndices.length > 0 ? 
                      selectedIndices.length + " selected" : 
                      "Total: " + alertLog.count
                font.pixelSize: 14
                color: selectionMode && selectedIndices.length > 0 ? "#3498db" : "#95a5a6"
                Layout.fillWidth: true
            }
            
            // Select button
            Button {
                text: selectionMode ? "Cancel" : "Select"
                Layout.preferredHeight: 32
                
                background: Rectangle {
                    color: parent.hovered ? "#2874a6" : "#3498db"
                    radius: 4
                }
                
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 12
                    font.bold: true
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                onClicked: {
                    selectionMode = !selectionMode
                    if (!selectionMode) {
                        selectedIndices = []
                    }
                }
            }
            
            // Action button (only visible in selection mode)
            Button {
                text: "Action"
                Layout.preferredHeight: 32
                visible: selectionMode
                enabled: selectedIndices.length > 0
                
                background: Rectangle {
                    color: parent.enabled ? (parent.hovered ? "#27ae60" : "#2ecc71") : "#95a5a6"
                    radius: 4
                }
                
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 12
                    font.bold: true
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                onClicked: actionMenu.open()
                
                Menu {
                    id: actionMenu
                    y: parent.height
                    
                    MenuItem {
                        text: "Export selected as CSV"
                        onTriggered: {
                            var timestamp = Qt.formatDateTime(new Date(), "yyyyMMdd_HHmmss")
                            var filePath = logsDir + "/alerts_selected_" + timestamp + ".csv"
                            if (alertLog.exportSelectedToCsv(filePath, selectedIndices)) {
                                root.exportRequested(filePath, "csv")
                            }
                        }
                    }
                    
                    MenuItem {
                        text: "Export selected as JSON"
                        onTriggered: {
                            var timestamp = Qt.formatDateTime(new Date(), "yyyyMMdd_HHmmss")
                            var filePath = logsDir + "/alerts_selected_" + timestamp + ".json"
                            if (alertLog.exportSelectedToJson(filePath, selectedIndices)) {
                                root.exportRequested(filePath, "json")
                            }
                        }
                    }
                }
            }
            
            // Clear button
            Button {
                text: "Clear"
                Layout.preferredHeight: 32
                enabled: selectionMode ? selectedIndices.length > 0 : alertLog.count > 0
                
                background: Rectangle {
                    color: parent.enabled ? (parent.hovered ? "#c0392b" : "#e74c3c") : "#95a5a6"
                    radius: 4
                }
                
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 12
                    font.bold: true
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                onClicked: {
                    if (selectionMode) {
                        // Remove selected alerts
                        alertLog.removeAlerts(selectedIndices)
                        var count = selectedIndices.length
                        selectedIndices = []
                        selectionMode = false
                        root.alertCleared(count)
                    } else {
                        // Show confirmation dialog for clearing all
                        clearConfirmDialog.open()
                    }
                }
            }
        }
        
        // Alert list
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#2c3e50"
            radius: 4
            border.color: "#7f8c8d"
            border.width: 1
            
            ListView {
                id: alertListView
                anchors.fill: parent
                anchors.margins: 4
                clip: true
                model: alertLog
                spacing: 4
                
                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
                
                delegate: Rectangle {
                    id: alertDelegate
                    
                    width: alertListView.width - 8
                    height: 80
                    color: isSelected ? "#455a64" : "#34495e"
                    radius: 4
                    border.color: {
                        if (isSelected) return "#3498db"
                        if (model.type === "snapshot") return "#3498db"
                        if (model.type === "motion") return "#e67e22"
                        if (model.type === "motion_roi") return "#9b59b6"
                        if (model.type === "tripwire") return "#e74c3c"
                        return "#95a5a6"
                    }
                    border.width: isSelected ? 3 : 2
                    
                    property bool isSelected: selectedIndices.indexOf(index) >= 0
                    property bool isSnapshot: model.type === "snapshot"
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8
                        
                        // Checkbox (only visible in selection mode)
                        CheckBox {
                            visible: selectionMode
                            checked: alertDelegate.isSelected
                            Layout.alignment: Qt.AlignVCenter
                            
                            onToggled: {
                                var newSelected = selectedIndices.slice()
                                var idx = newSelected.indexOf(index)
                                if (checked && idx < 0) {
                                    newSelected.push(index)
                                } else if (!checked && idx >= 0) {
                                    newSelected.splice(idx, 1)
                                }
                                selectedIndices = newSelected
                            }
                            
                            indicator: Rectangle {
                                width: 20
                                height: 20
                                radius: 3
                                border.color: "#3498db"
                                border.width: 2
                                color: parent.checked ? "#3498db" : "transparent"
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: "✓"
                                    font.pixelSize: 14
                                    font.bold: true
                                    color: "#ffffff"
                                    visible: parent.parent.checked
                                }
                            }
                        }
                        
                        // Alert content
                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: 4
                            
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8
                                
                                Text {
                                    text: Qt.formatDateTime(model.timestamp, "hh:mm:ss")
                                    font.pixelSize: 12
                                    font.bold: true
                                    color: "#ecf0f1"
                                }
                                
                                Rectangle {
                                    width: 60
                                    height: 18
                                    radius: 3
                                    color: {
                                        if (model.type === "snapshot") return "#3498db"
                                        if (model.type === "motion") return "#e67e22"
                                        if (model.type === "motion_roi") return "#9b59b6"
                                        if (model.type === "tripwire") return "#e74c3c"
                                        return "#95a5a6"
                                    }
                                    
                                    Text {
                                        anchors.centerIn: parent
                                        text: {
                                            if (model.type === "snapshot") return "SNAP"
                                            if (model.type === "motion") return "MOTION"
                                            if (model.type === "motion_roi") return "ROI"
                                            if (model.type === "tripwire") return "TRIP"
                                            return model.type.toUpperCase()
                                        }
                                        font.pixelSize: 10
                                        font.bold: true
                                        color: "#ffffff"
                                    }
                                }
                                
                                Item { Layout.fillWidth: true }
                            }
                            
                            Text {
                                text: model.cameraName
                                font.pixelSize: 14
                                font.bold: true
                                color: "#ecf0f1"
                                Layout.fillWidth: true
                            }
                            
                            Text {
                                text: model.message
                                font.pixelSize: 12
                                color: "#95a5a6"
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                        }
                    }
                    
                    // Mouse area for click handling
                    MouseArea {
                        anchors.fill: parent
                        enabled: !selectionMode
                        
                        onClicked: {
                            // Show popover for this alert
                            activePopoverIndex = index
                            alertPopover.alertIndex = index
                            alertPopover.alertType = model.type
                            alertPopover.parent = alertDelegate
                            alertPopover.open()
                        }
                    }
                    
                    // Visual feedback on hover
                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }
                
                // Empty state
                Text {
                    anchors.centerIn: parent
                    visible: alertListView.count === 0
                    text: "No alerts yet"
                    font.pixelSize: 14
                    color: "#7f8c8d"
                }
            }
        }
    }
    
    // Alert action popover
    // Find the alertPopover Popup and update its content:

Popup {
    id: alertPopover
    
    property int alertIndex: -1
    property string alertType: ""
    
    width: 200
    height: alertType === "snapshot" ? 140 : 60  // Increased height for PNG option
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    
    x: parent ? (parent.width - width) / 2 : 0
    y: parent ? parent.height + 4 : 0
    
    background: Rectangle {
        color: "#2c3e50"
        radius: 6
        border.color: "#3498db"
        border.width: 2
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 4
        
        // Export as PNG (only for snapshots)
        Button {
            visible: alertPopover.alertType === "snapshot"
            text: "Export as PNG..."
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            
            background: Rectangle {
                color: parent.hovered ? "#34495e" : "transparent"
                radius: 4
            }
            
            contentItem: Text {
                text: parent.text
                font.pixelSize: 12
                color: "#ecf0f1"
                horizontalAlignment: Text.AlignLeft
            }
            
            onClicked: {
                alertPopover.close()
                pngExportDialog.alertIndex = alertPopover.alertIndex
                pngExportDialog.open()
            }
        }
        
        // Save metadata submenu (only for snapshots)
        Button {
            visible: alertPopover.alertType === "snapshot"
            text: "Export metadata..."
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            
            background: Rectangle {
                color: parent.hovered ? "#34495e" : "transparent"
                radius: 4
            }
            
            contentItem: RowLayout {
                Text {
                    text: parent.parent.text
                    font.pixelSize: 12
                    color: "#ecf0f1"
                    Layout.fillWidth: true
                }
                Text {
                    text: "▶"
                    font.pixelSize: 10
                    color: "#95a5a6"
                }
            }
            
            onClicked: {
                alertPopover.close()
                saveMenu.alertIndex = alertPopover.alertIndex
                saveMenu.parent = alertPopover.parent
                saveMenu.open()
            }
        }
        
        // Clear this alert
        Button {
            text: "Clear this alert"
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            
            background: Rectangle {
                color: parent.hovered ? "#c0392b" : "transparent"
                radius: 4
            }
            
            contentItem: Text {
                text: parent.text
                font.pixelSize: 12
                color: "#e74c3c"
                horizontalAlignment: Text.AlignLeft
            }
            
            onClicked: {
                alertLog.removeAlert(alertPopover.alertIndex)
                alertPopover.close()
                root.alertCleared(1)
            }
        }
    }
}

// Add PNG export file dialog
// Add PNG export file dialog
FileDialog {
    id: pngExportDialog
    
    property int alertIndex: -1
    
    title: "Export Snapshot as PNG"
    fileMode: FileDialog.SaveFile
    nameFilters: ["PNG Images (*.png)"]
    defaultSuffix: "png"
    currentFolder: "file:///" + logsDir
    
    onAccepted: {
        var filePath = pngExportDialog.selectedFile.toString()
        // Remove file:// prefix if present
        if (filePath.startsWith("file:///")) {
            filePath = filePath.substring(8)  // Windows: file:///C:/...
        } else if (filePath.startsWith("file://")) {
            filePath = filePath.substring(7)   // Unix: file:///home/...
        }
        
        if (alertLog.exportSnapshotAsPng(pngExportDialog.alertIndex, filePath)) {
            root.exportRequested(filePath, "png")
        }
    }
    
    Component.onCompleted: {
        // Set initial suggested filename when dialog is created
        if (alertIndex >= 0) {
            var suggestedName = alertLog.getSuggestedPngFilename(alertIndex)
            pngExportDialog.selectedFile = "file:///" + logsDir + "/" + suggestedName
        }
    }
    
    // Update suggested filename when alertIndex changes
    onAlertIndexChanged: {
        if (alertIndex >= 0) {
            var suggestedName = alertLog.getSuggestedPngFilename(alertIndex)
            pngExportDialog.selectedFile = "file:///" + logsDir + "/" + suggestedName
        }
    }
}
    
    // Save submenu for snapshots
    Menu {
        id: saveMenu
        
        property int alertIndex: -1
        
        y: parent ? 0 : 0
        x: parent ? parent.width + 4 : 0
        
        MenuItem {
            text: "Export as CSV"
            onTriggered: {
                var timestamp = Qt.formatDateTime(new Date(), "yyyyMMdd_HHmmss")
                var filePath = logsDir + "/alert_" + saveMenu.alertIndex + "_" + timestamp + ".csv"
                if (alertLog.exportSelectedToCsv(filePath, [saveMenu.alertIndex])) {
                    root.exportRequested(filePath, "csv")
                }
            }
        }
        
        MenuItem {
            text: "Export as JSON"
            onTriggered: {
                var timestamp = Qt.formatDateTime(new Date(), "yyyyMMdd_HHmmss")
                var filePath = logsDir + "/alert_" + saveMenu.alertIndex + "_" + timestamp + ".json"
                if (alertLog.exportSelectedToJson(filePath, [saveMenu.alertIndex])) {
                    root.exportRequested(filePath, "json")
                }
            }
        }
    }
    
    // Clear all confirmation dialog
    Dialog {
        id: clearConfirmDialog
        
        title: "Clear All Alerts"
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        
        width: 350
        
        background: Rectangle {
            color: "#2c3e50"
            radius: 8
            border.color: "#3498db"
            border.width: 2
        }
        
        header: Rectangle {
            width: parent.width
            height: 50
            color: "#34495e"
            radius: 8
            
            Text {
                anchors.centerIn: parent
                text: "Clear All Alerts"
                font.pixelSize: 16
                font.bold: true
                color: "#ecf0f1"
            }
        }
        
        contentItem: Text {
            text: "Are you sure you want to remove all " + alertLog.count + " entries from the alert log?"
            font.pixelSize: 14
            color: "#ecf0f1"
            wrapMode: Text.WordWrap
        }
        
        onAccepted: {
            var count = alertLog.count
            alertLog.clear()
            root.alertCleared(count)
        }
    }
}