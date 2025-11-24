import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    
    property string message: ""
    property int duration: 3000
    
    width: Math.min(parent.width * 0.8, 400)
    height: 60
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 80
    
    color: "#2c3e50"
    border.color: "#3498db"
    border.width: 2
    radius: 8
    opacity: 0
    
    z: 1000
    
    Text {
        anchors.centerIn: parent
        text: root.message
        color: "#ecf0f1"
        font.pixelSize: 14
        font.bold: true
        wrapMode: Text.WordWrap
        width: parent.width - 20
        horizontalAlignment: Text.AlignHCenter
    }
    
    function show(msg) {
        message = msg
        showAnimation.start()
    }
    
    SequentialAnimation {
        id: showAnimation
        
        NumberAnimation {
            target: root
            property: "opacity"
            to: 1.0
            duration: 300
            easing.type: Easing.OutQuad
        }
        
        PauseAnimation {
            duration: root.duration
        }
        
        NumberAnimation {
            target: root
            property: "opacity"
            to: 0
            duration: 300
            easing.type: Easing.InQuad
        }
    }
}