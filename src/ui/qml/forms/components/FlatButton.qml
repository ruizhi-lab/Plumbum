import QtQuick
import Qt5Compat.GraphicalEffects

// Modern flat button: rounded, with hover/press feedback and subtle elevation.
Rectangle {
    id: root

    property string text: ""
    property bool active: false   // highlighted / primary action
    property bool danger: false   // destructive action (red tint)
    property int controlHeight: 36
    property int controlMinWidth: 0
    signal clicked()

    // Interaction state
    property bool hovered: false
    property bool pressed: false

    height: controlHeight
    radius: 9

    color: {
        if (pressed) {
            if (active) return window.cPrimaryAlt
            if (danger) return Qt.rgba(1.0, 0.42, 0.42, 0.20)
            return window.isDark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.08)
        }
        if (active) return window.cPrimary
        if (danger) return Qt.rgba(1.0, 0.42, 0.42, 0.12)
        return hovered ? (window.isDark ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(0, 0, 0, 0.05))
                       : window.cSurfaceAlt
    }
    border.color: {
        if (active) return window.cPrimary
        if (danger) return Qt.rgba(1.0, 0.42, 0.42, 0.45)
        return window.cBorder
    }
    border.width: 1

    Behavior on color { ColorAnimation { duration: 120 } }

    // Shadow for active buttons (elevation)
    layer.enabled: root.active
    layer.smooth: true
    layer.effect: DropShadow {
        horizontalOffset: 0
        verticalOffset: 2
        radius: 6
        samples: 14
        color: window.isDark ? "#59000000" : "#33203b5e"
    }

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
        Behavior on color { ColorAnimation { duration: 120 } }
    }

    implicitWidth: Math.max(controlMinWidth, label.implicitWidth + 30)
    implicitHeight: controlHeight

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: root.hovered = true
        onExited: { root.hovered = false; root.pressed = false }
        onPressed: root.pressed = true
        onReleased: root.pressed = false
        onClicked: root.clicked()
    }
}
