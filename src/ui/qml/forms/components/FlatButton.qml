import QtQuick

// Flat rounded button matching the PAC selector style.
// Usage:
//   FlatButton { text: "Import"; onClicked: ... }
//   FlatButton { text: "Delete"; danger: true; onClicked: ... }
//   FlatButton { text: "Active"; active: true; onClicked: ... }
Rectangle {
    id: root

    property string text: ""
    property bool active: false   // highlighted / primary action
    property bool danger: false   // destructive action (red tint)
    property int controlHeight: 36
    property int controlMinWidth: 0
    signal clicked()

    height: controlHeight
    radius: 8
    color: {
        if (active) return window.cPrimary
        if (danger) return Qt.rgba(1.0, 0.42, 0.42, 0.12)
        return window.cSurfaceAlt
    }
    border.color: {
        if (active) return window.cPrimary
        if (danger) return Qt.rgba(1.0, 0.42, 0.42, 0.45)
        return window.cBorder
    }
    border.width: 1

    Behavior on color { ColorAnimation { duration: 120 } }

    Text {
        id: label
        anchors.centerIn: parent
        text: root.text
        font.pixelSize: 11
        font.bold: root.active
        color: {
            if (root.active) return "#ffffff"
            if (root.danger) return window.cRed
            return window.cText
        }
    }

    // Size hint so controls with variable text keep consistent layout
    implicitWidth: Math.max(controlMinWidth, label.implicitWidth + 28)
    implicitHeight: controlHeight

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: if (!root.active) root.opacity = 0.85
        onExited: root.opacity = 1.0
        onClicked: root.clicked()
    }
}
