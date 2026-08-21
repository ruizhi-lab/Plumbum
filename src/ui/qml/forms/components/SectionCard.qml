import QtQuick
import QtQuick.Effects

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

    MultiEffect {
        anchors.fill: cardRect
        source: cardRect
        shadowEnabled: true
        shadowVerticalOffset: 2
        shadowBlur: 0.8
        shadowColor: window.isDark ? "#33000000" : "#14203345"
    }
}
