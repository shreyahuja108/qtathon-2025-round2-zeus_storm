import QtQuick
import QtQuick.Controls

Item {
    id: root

    property var cameraStream: null
    property bool running: cameraStream && cameraStream.running
    property bool aiEnabled: cameraStream && cameraStream.aiEnabled
    property var seenDetections: ({})
    property var stableDetections: []
    property var _prevStableDetections: []
    property int _detCounter: 0

    readonly property int detectionsCount: cameraStream && cameraStream.detections ? cameraStream.detections.length : 0

    function colorForLabel(label) {
        switch (label) {
        case "person":
            return "#00E676"  // Bright green for people
        case "car":
            return "#FF5252"  // Bright red for vehicles
        case "dog":
        case "cat":
            return "#FFD740"  // Bright yellow for animals
        case "bicycle":
            return "#40C4FF"  // Bright cyan for bicycles
        default:
            return "#E0E0E0"  // Light gray for unknown
        }
    }

    function iconForLabel(label) {
        switch (label) {
        case "person":
            return "\uD83D\uDEB6" // person
        case "car":
            return "\uD83D\uDE97" // car
        case "dog":
            return "\uD83D\uDC15" // dog
        case "cat":
            return "\uD83D\uDC31" // cat
        case "bicycle":
            return "\uD83D\uDEB2" // bicycle
        default:
            return "\uD83E\uDD16" // robot
        }
    }

    function barColor(score) {
        if (score >= 0.8)
            return "#00E676"  // Excellent confidence - bright green
        if (score >= 0.6)
            return "#FFD740"  // Good confidence - yellow
        if (score >= 0.4)
            return "#FF9800"  // Medium confidence - orange
        return "#FF5252"      // Low confidence - red
    }

    function iou(a, b) {
        const x1 = Math.max(a.x, b.x)
        const y1 = Math.max(a.y, b.y)
        const x2 = Math.min(a.x + a.width, b.x + b.width)
        const y2 = Math.min(a.y + a.height, b.y + b.height)
        const iw = Math.max(0, x2 - x1)
        const ih = Math.max(0, y2 - y1)
        const inter = iw * ih
        const ua = a.width * a.height + b.width * b.height - inter
        return ua > 0 ? inter / ua : 0
    }

    function assignStableDetections() {
        const dets = cameraStream && cameraStream.detections ? cameraStream.detections : []
        const prev = _prevStableDetections || []
        const next = []

        for (let i = 0; i < dets.length; ++i) {
            const d = dets[i]
            let bestKey = ""
            let bestIoU = 0
            for (let j = 0; j < prev.length; ++j) {
                const p = prev[j]
                if (p.label !== d.label)
                    continue
                const score = iou(p, d)
                if (score > bestIoU) {
                    bestIoU = score
                    bestKey = p.stableKey
                }
            }
            const stableKey = bestIoU >= 0.15 && bestKey !== "" ? bestKey : ("d" + (_detCounter++))
            next.push({
                x: d.x,
                y: d.y,
                width: d.width,
                height: d.height,
                label: d.label,
                score: d.score,
                stableKey: stableKey
            })
        }

        _prevStableDetections = next
        stableDetections = next
    }

    Component.onCompleted: assignStableDetections()

    Connections {
        target: cameraStream
        function onDetectionsChanged() { assignStableDetections() }
    }

    Repeater {
        model: stableDetections

        delegate: Item {
            id: detectionItem

            property real absX: modelData.x * root.width
            property real absY: modelData.y * root.height
            property real absWidth: modelData.width * root.width
            property real absHeight: modelData.height * root.height
            property color accentColor: root.colorForLabel(modelData.label)
            property string detectionKey: modelData.stableKey
            property bool isNew: !root.seenDetections[detectionKey]

            x: absX
            y: absY
            width: absWidth
            height: absHeight
            scale: isNew ? 0.8 : 1
            opacity: isNew ? 0 : 1
            transformOrigin: Item.Center

            Behavior on x { NumberAnimation { duration: 120; easing.type: Easing.OutCubic } }
            Behavior on y { NumberAnimation { duration: 120; easing.type: Easing.OutCubic } }
            Behavior on opacity { enabled: isNew; NumberAnimation { duration: 200; easing.type: Easing.OutCubic } }
            Behavior on scale { enabled: isNew; NumberAnimation { duration: 200; easing.type: Easing.OutCubic } }

            Component.onCompleted: {
                root.seenDetections[detectionKey] = true
                opacity = 1
                scale = 1
            }

            Rectangle {
                anchors.fill: parent
                radius: 4
                border.width: 2.5
                border.color: accentColor
                color: Qt.rgba(border.color.r, border.color.g, border.color.b, 0.1)
            }

            Rectangle {
                id: labelBox
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: -2
                radius: 4
                color: "#D0000000"
                border.width: 1.5
                border.color: accentColor
                height: labelText.implicitHeight + 10
                width: labelText.implicitWidth + 16

                Row {
                    anchors.centerIn: parent
                    spacing: 6
                    Text {
                        text: root.iconForLabel(modelData.label)
                        font.pixelSize: 16
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Text {
                        id: labelText
                        text: modelData.label.toUpperCase() + " " + Math.round(modelData.score * 100) + "%"
                        font.pixelSize: 13
                        font.bold: true
                        font.letterSpacing: 0.5
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Rectangle {
                id: confidenceTrack
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -12
                width: Math.max(60, parent.width * 0.85)
                height: 5
                radius: 2.5
                color: "#70000000"
                border.color: "#50ffffff"
                border.width: 1

                Rectangle {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height - 2
                    width: Math.max(0, Math.min(parent.width * modelData.score, parent.width))
                    radius: 2
                    color: root.barColor(modelData.score)
                    
                    Behavior on width {
                        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                    }
                }
            }

            // Corner accents - top-left
            Item {
                width: 20
                height: 20
                anchors.left: parent.left
                anchors.top: parent.top
                
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: 20
                    height: 3
                    color: accentColor
                }
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: 3
                    height: 20
                    color: accentColor
                }
            }
            
            // Corner accents - top-right
            Item {
                width: 20
                height: 20
                anchors.right: parent.right
                anchors.top: parent.top
                
                Rectangle {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: 20
                    height: 3
                    color: accentColor
                }
                Rectangle {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: 3
                    height: 20
                    color: accentColor
                }
            }
            
            // Corner accents - bottom-left
            Item {
                width: 20
                height: 20
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                
                Rectangle {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: 20
                    height: 3
                    color: accentColor
                }
                Rectangle {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: 3
                    height: 20
                    color: accentColor
                }
            }
            
            // Corner accents - bottom-right
            Item {
                width: 20
                height: 20
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                
                Rectangle {
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    width: 20
                    height: 3
                    color: accentColor
                }
                Rectangle {
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    width: 3
                    height: 20
                    color: accentColor
                }
            }
        }
    }

    Rectangle {
        id: countBadge
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 16
        radius: height / 2
        color: "#E0000000"
        border.color: "#00E676"
        border.width: 2
        visible: cameraStream && aiEnabled && running && detectionsCount > 0
        opacity: visible ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 250; easing.type: Easing.OutCubic } }

        Text {
            id: countText
            anchors.centerIn: parent
            text: "\uD83E\uDD16 " + detectionsCount + (detectionsCount === 1 ? " OBJECT" : " OBJECTS")
            color: "#00E676"
            font.pixelSize: 13
            font.bold: true
            font.letterSpacing: 0.5
        }

        width: countText.implicitWidth + 28
        height: countText.implicitHeight + 16
    }
}
