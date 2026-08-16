import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Simple sidebar navigation button
Rectangle {
    id: root
    property string buttonText: ""
    property string buttonIcon: ""
    property bool isActive: false
    signal clicked()

    height: 44
    radius: 6
    color: isActive ? Qt.rgba(0.36, 0.55, 1.0, 0.16) : "transparent"
    border.color: isActive ? Qt.rgba(0.36, 0.55, 1.0, 0.35) : "transparent"

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 12

        Text {
            text: root.buttonIcon
            font.pixelSize: 15
            color: root.isActive ? window.cPrimaryAlt : window.cTextDim
            width: 20
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            text: root.buttonText
            font.pixelSize: 13
            color: root.isActive ? window.cText : window.cTextDim
            font.bold: root.isActive
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
        onEntered: if (!root.isActive) root.color = Qt.rgba(1, 1, 1, 0.04)
        onExited: if (!root.isActive) root.color = "transparent"
    }
}
