import QtQuick
import Qt5Compat.GraphicalEffects

// Modern elevated card: rounded surface with soft drop shadow.
// Usage: SectionCard { children... }
Item {
    id: root

    default property alias content: cardRect.data

    Rectangle {
        id: cardRect
        anchors.fill: parent
        radius: 12
        color: window.cSurface
        border.color: window.cBorder
        border.width: 1
        clip: true
        layer.enabled: true
        layer.smooth: true
        layer.samples: 4
    }

    DropShadow {
        anchors.fill: cardRect
        source: cardRect
        horizontalOffset: 0
        verticalOffset: 2
        radius: 10
        samples: 20
        color: window.isDark ? "#33000000" : "#14203345"
    }
}
