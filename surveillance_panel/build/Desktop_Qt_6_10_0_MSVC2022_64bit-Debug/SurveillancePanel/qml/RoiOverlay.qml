import QtQuick
import QtQuick.Shapes

Item {
    id: root
    
    property int cameraIndex: 1
    property bool roiEditMode: false
    property bool tripwireEditMode: false
    property var cameraStream: null
    
    // Watch for edit mode changes and initialize properly
    onRoiEditModeChanged: {
        if (roiEditMode) {
            // Starting ROI edit - clear any existing temp data
            tempRoiPoints = []
            canvas.requestPaint()
        }
    }
    
    onTripwireEditModeChanged: {
        if (tripwireEditMode) {
            // Starting tripwire edit - clear any existing temp data
            tempTripwireStart = null
            canvas.requestPaint()
        }
    }
    
    signal editingFinished()

    
    // Current editing data
    property var tempRoiPoints: []
    property var tempTripwireStart: null
    property point mousePos: Qt.point(0, 0)
    
    // Stored ROI and Tripwire from CameraManager
    property var storedRoiPoints: []
    property var storedTripwire: null
    
    Component.onCompleted: {
        loadStoredData()
    }
    
    Connections {
        target: cameraManager
        function onRoiChanged(index) {
            if (index === cameraIndex) {
                loadStoredData()
            }
        }
        function onTripwireChanged(index) {
            if (index === cameraIndex) {
                loadStoredData()
            }
        }
    }
    
    function loadStoredData() {
        storedRoiPoints = cameraManager.roiPoints(cameraIndex)
        storedTripwire = cameraManager.tripwire(cameraIndex)
        canvas.requestPaint()
    }
    
    function startRoiEdit() {
        tempRoiPoints = []
        roiEditMode = true
        tripwireEditMode = false
        canvas.requestPaint()
    }
    
    function startTripwireEdit() {
        tempTripwireStart = null
        tripwireEditMode = true
        roiEditMode = false
        canvas.requestPaint()
    }
    
    function cancelEdit() {
        // Don't set roiEditMode/tripwireEditMode = false - let parent handle via editingFinished
        tempRoiPoints = []
        tempTripwireStart = null
        canvas.requestPaint()
        editingFinished()
    }
    
    function finishRoiEdit() {
        if (tempRoiPoints.length >= 3) {
            // Convert pixel coordinates to normalized 0-1
            var normalizedPoints = []
            for (var i = 0; i < tempRoiPoints.length; i++) {
                var pt = tempRoiPoints[i]
                normalizedPoints.push({
                    x: pt.x / root.width,
                    y: pt.y / root.height
                })
            }
            cameraManager.setRoi(cameraIndex, normalizedPoints)
        }
        // Don't set roiEditMode = false here - let parent handle it via editingFinished signal
        tempRoiPoints = []
        canvas.requestPaint()
        editingFinished()
    }
    
    function finishTripwireEdit(endX, endY) {
        if (tempTripwireStart) {
            // Convert to normalized coordinates
            var startX = tempTripwireStart.x / root.width
            var startY = tempTripwireStart.y / root.height
            var normEndX = endX / root.width
            var normEndY = endY / root.height
            
            cameraManager.setTripwire(cameraIndex, startX, startY, normEndX, normEndY)
        }
        // Don't set tripwireEditMode = false here - let parent handle it via editingFinished signal
        tempTripwireStart = null
        canvas.requestPaint()
        editingFinished()
    }
    
    // Mouse handling
    MouseArea {
        anchors.fill: parent
        enabled: roiEditMode || tripwireEditMode
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        
        onPositionChanged: {
            root.mousePos = Qt.point(mouse.x, mouse.y)
            if (tripwireEditMode && tempTripwireStart) {
                canvas.requestPaint()
            }
        }
        
        onClicked: (mouse) => {
            if (mouse.button === Qt.RightButton) {
                root.cancelEdit()
                return
            }
            
            if (roiEditMode) {
                tempRoiPoints.push({x: mouse.x, y: mouse.y})
                tempRoiPoints = tempRoiPoints // Force binding update
                canvas.requestPaint()
            } else if (tripwireEditMode) {
                if (!tempTripwireStart) {
                    tempTripwireStart = {x: mouse.x, y: mouse.y}
                    canvas.requestPaint()
                } else {
                    root.finishTripwireEdit(mouse.x, mouse.y)
                }
            }
        }
        
        onDoubleClicked: {
            if (roiEditMode) {
                root.finishRoiEdit()
            }
        }
    }
    
    // Main canvas for all drawing
    Canvas {
        id: canvas
        anchors.fill: parent
        
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            
            // Draw stored ROI (when not editing)
            if (!roiEditMode && storedRoiPoints && storedRoiPoints.length >= 3) {
                // Use brighter color if ROI alert is active
                var isAlert = cameraStream && cameraStream.roiAlertActive
                ctx.fillStyle = isAlert ? "rgba(231, 76, 60, 0.4)" : "rgba(52, 152, 219, 0.3)"
                ctx.strokeStyle = isAlert ? "#e74c3c" : "#3498db"
                ctx.lineWidth = isAlert ? 4 : 3
                ctx.setLineDash([])
                
                ctx.beginPath()
                ctx.moveTo(storedRoiPoints[0].x * width, storedRoiPoints[0].y * height)
                for (var i = 1; i < storedRoiPoints.length; i++) {
                    ctx.lineTo(storedRoiPoints[i].x * width, storedRoiPoints[i].y * height)
                }
                ctx.closePath()
                ctx.fill()
                ctx.stroke()
            }
            
            // Draw editing ROI
            if (roiEditMode && tempRoiPoints.length > 0) {
                // Draw points
                ctx.fillStyle = "#e74c3c"
                for (var j = 0; j < tempRoiPoints.length; j++) {
                    ctx.beginPath()
                    ctx.arc(tempRoiPoints[j].x, tempRoiPoints[j].y, 6, 0, 2 * Math.PI)
                    ctx.fill()
                    
                    // White border
                    ctx.strokeStyle = "#ffffff"
                    ctx.lineWidth = 2
                    ctx.stroke()
                }
                
                // Draw lines between points
                if (tempRoiPoints.length > 1) {
                    ctx.strokeStyle = "#e74c3c"
                    ctx.lineWidth = 2
                    ctx.setLineDash([5, 3])
                    
                    ctx.beginPath()
                    ctx.moveTo(tempRoiPoints[0].x, tempRoiPoints[0].y)
                    for (var k = 1; k < tempRoiPoints.length; k++) {
                        ctx.lineTo(tempRoiPoints[k].x, tempRoiPoints[k].y)
                    }
                    // Preview line back to start
                    ctx.lineTo(tempRoiPoints[0].x, tempRoiPoints[0].y)
                    ctx.stroke()
                }
            }
            
            // Draw stored Tripwire (when not editing)
            if (!tripwireEditMode && storedTripwire && storedTripwire.has) {
                var startX = storedTripwire.startX * width
                var startY = storedTripwire.startY * height
                var endX = storedTripwire.endX * width
                var endY = storedTripwire.endY * height
                
                // Use brighter color if tripwire alert is active
                var isTripAlert = cameraStream && cameraStream.tripwireAlertActive
                ctx.strokeStyle = isTripAlert ? "#e74c3c" : "#e67e22"
                ctx.lineWidth = isTripAlert ? 6 : 4
                ctx.setLineDash([])
                
                ctx.beginPath()
                ctx.moveTo(startX, startY)
                ctx.lineTo(endX, endY)
                ctx.stroke()
                
                // Draw endpoints
                ctx.fillStyle = isTripAlert ? "#e74c3c" : "#e67e22"
                ctx.beginPath()
                ctx.arc(startX, startY, isTripAlert ? 8 : 6, 0, 2 * Math.PI)
                ctx.fill()
                ctx.beginPath()
                ctx.arc(endX, endY, isTripAlert ? 8 : 6, 0, 2 * Math.PI)
                ctx.fill()
            }
            
            // Draw editing Tripwire
            if (tripwireEditMode && tempTripwireStart) {
                // Draw start point
                ctx.fillStyle = "#e74c3c"
                ctx.beginPath()
                ctx.arc(tempTripwireStart.x, tempTripwireStart.y, 6, 0, 2 * Math.PI)
                ctx.fill()
                
                ctx.strokeStyle = "#ffffff"
                ctx.lineWidth = 2
                ctx.stroke()
                
                // Draw preview line to mouse
                ctx.strokeStyle = "#e74c3c"
                ctx.lineWidth = 3
                ctx.setLineDash([5, 3])
                
                ctx.beginPath()
                ctx.moveTo(tempTripwireStart.x, tempTripwireStart.y)
                ctx.lineTo(root.mousePos.x, root.mousePos.y)
                ctx.stroke()
            }
        }
    }
    
    // Timer to update overlay when alerts are active
    Timer {
        running: (cameraStream && cameraStream.roiAlertActive) || 
                 (cameraStream && cameraStream.tripwireAlertActive)
        interval: 100
        repeat: true
        onTriggered: canvas.requestPaint()
    }
    
    // Help text
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        width: helpText.width + 20
        height: 40
        color: "#2c3e50"
        border.color: "#3498db"
        border.width: 2
        radius: 4
        opacity: 0.9
        visible: roiEditMode || tripwireEditMode
        
        Text {
            id: helpText
            anchors.centerIn: parent
            text: {
                if (roiEditMode) {
                    if (tempRoiPoints.length < 3) {
                        return "Click to add points (need at least 3)"
                    }
                    return "Click more points, double-click to finish, right-click to cancel"
                } else if (tripwireEditMode) {
                    if (!tempTripwireStart) {
                        return "Click first point to start tripwire"
                    }
                    return "Click second point to finish, right-click to cancel"
                }
                return ""
            }
            color: "#ecf0f1"
            font.pixelSize: 12
        }
    }
}