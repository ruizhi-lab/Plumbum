import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

// Modern sidebar navigation button: SVG icon + label + active indicator
Rectangle {
    id: root
    property string buttonText: ""
    property string buttonIcon: ""      // SVG path content (monochrome)
    property bool isActive: false
    signal clicked()

    Layout.fillWidth: true
    height: 46
    radius: 10
    color: "transparent"

    property bool hovered: false

    // Active indicator bar (left)
    Rectangle {
        width: 3
        height: 20
        radius: 1.5
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 6
        color: window.cPrimary
        visible: root.isActive
    }

    // Background
    Rectangle {
        anchors.fill: parent
        radius: 10
        color: root.isActive ? (window.isDark ? Qt.rgba(0.36, 0.55, 1.0, 0.16) : Qt.rgba(0.24, 0.42, 1.0, 0.10))
                             : (root.hovered ? (window.isDark ? Qt.rgba(1, 1, 1, 0.05) : Qt.rgba(0, 0, 0, 0.04)) : "transparent")
        Behavior on color { ColorAnimation { duration: 150 } }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 12
        spacing: 12

        // Icon (monochrome SVG tinted by state)
        Item {
            Layout.preferredWidth: 18
            Layout.preferredHeight: 18

            Image {
                id: navIcon
                anchors.fill: parent
                anchors.margins: 1
                source: "data:image/svg+xml;utf8," + root.buttonIcon
                sourceSize.width: 18
                sourceSize.height: 18
                antialiasing: true
            }
            MultiEffect {
                anchors.fill: navIcon
                source: navIcon
                colorizationColor: root.isActive ? window.cPrimary
                                 : root.hovered ? window.cText
                                 : window.cTextDim
                colorization: 1.0
                Behavior on colorizationColor { ColorAnimation { duration: 150 } }
            }
        }

        Text {
            text: root.buttonText
            font.pixelSize: 13
            font.bold: root.isActive
            color: root.isActive ? window.cText : (root.hovered ? window.cText : window.cTextDim)
            Behavior on color { ColorAnimation { duration: 150 } }
            elide: Text.ElideRight
            Layout.fillWidth: true
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: root.hovered = true
        onExited: root.hovered = false
        onClicked: root.clicked()
    }
}
