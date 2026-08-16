import QtQuick

// Unified rounded-bordered section container, consistent with the app's card style.
// Usage: SectionCard { children... }
Rectangle {
    id: root

    radius: 10
    color: window.cSurface
    border.color: window.cBorder
    border.width: 1
    clip: true
}
