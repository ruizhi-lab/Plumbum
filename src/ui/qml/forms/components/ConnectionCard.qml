import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// A single connection card in the list
Rectangle {
    id: root

    property string connId: ""
    property string displayName: ""
    property string protocol: ""
    property string address: ""
    property int port: 0
    property string latencyText: ""
    property bool isConnected: false
    property string upTotal: ""
    property string downTotal: ""
    property string lastConnected: ""
    property int groupIndex: 0

    signal connectRequested(string id)
    signal disconnectRequested(string id)
    signal editRequested(string id)
    signal copyLinkRequested(string id)
    signal latencyRequested(string id)
    signal deleteRequested(string id)

    height: 68
    radius: 10
    color: isConnected ? Qt.rgba(0.24, 0.86, 0.59, 0.08) : window.cSurface
    border.color: isConnected ? Qt.rgba(0.24, 0.86, 0.59, 0.35) : window.cBorder

    // Protocol badge color
    function badgeColor(p: string): string {
        switch (p) {
        case "VMESS": return "#7aa2ff"
        case "VLESS": return "#bd93f9"
        case "TROJAN": return "#ffb86c"
        case "SHADOWSOCKS": return "#3ddc97"
        case "SSR": return "#f7768e"
        case "HTTP": return "#7dcfff"
        case "SOCKS": return "#73daca"
        case "NAIVEPROXY": return "#e0af68"
        default: return window.cTextDim
        }
    }

    // Convert #RRGGBB hex color to Qt.rgba with given alpha
    function withAlpha(c, a) {
        return Qt.rgba(parseInt(c.substring(1, 3), 16) / 255,
                       parseInt(c.substring(3, 5), 16) / 255,
                       parseInt(c.substring(5, 7), 16) / 255, a)
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 18
        anchors.rightMargin: 12
        spacing: 14

        // Status indicator
        Rectangle {
            id: statusDot
            width: 10
            height: 10
            radius: 5
            color: isConnected ? window.cGreen : window.cTextDim

            SequentialAnimation {
                running: root.isConnected
                loops: Animation.Infinite
                NumberAnimation { target: statusDot; property: "opacity"; from: 1; to: 0.3; duration: 800 }
                NumberAnimation { target: statusDot; property: "opacity"; from: 0.3; to: 1; duration: 800 }
            }
        }

        // Name + protocol
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            RowLayout {
                spacing: 8
                Text {
                    text: root.displayName
                    font.pixelSize: 14
                    font.bold: true
                    color: window.cText
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
                Rectangle {
                    visible: root.protocol.length > 0
                    radius: 4
                    color: root.withAlpha(root.badgeColor(root.protocol), 0.18)
                    border.color: root.withAlpha(root.badgeColor(root.protocol), 0.4)
                    Layout.preferredWidth: badgeText.implicitWidth + 12
                    Layout.preferredHeight: 18

                    Text {
                        id: badgeText
                        anchors.centerIn: parent
                        text: root.protocol
                        font.pixelSize: 9
                        font.bold: true
                        color: root.badgeColor(root.protocol)
                    }
                }
            }

            Text {
                text: root.address + (root.port > 0 ? ":" + root.port : "")
                font.pixelSize: 11
                color: window.cTextDim
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }

        // Latency
        ColumnLayout {
            spacing: 2
            Layout.preferredWidth: 70
            Text {
                text: root.latencyText
                font.pixelSize: 11
                font.bold: root.latencyText.endsWith("ms")
                color: root.latencyText === "Timeout"
                       ? window.cRed
                       : (root.latencyText.endsWith("ms") ? window.cGreen : window.cTextDim)
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: qsTr("latency")
                font.pixelSize: 9
                color: window.cTextDim
                Layout.alignment: Qt.AlignHCenter
            }
        }

        // Traffic
        ColumnLayout {
            spacing: 2
            Layout.preferredWidth: 110
            Text {
                text: "↑ " + root.upTotal
                font.pixelSize: 10
                color: window.cTextDim
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "↓ " + root.downTotal
                font.pixelSize: 10
                color: window.cTextDim
                Layout.alignment: Qt.AlignHCenter
            }
        }

        // Action button
        Button {
            Layout.preferredWidth: 86
            text: root.isConnected ? qsTr("Disconnect") : qsTr("Connect")
            highlighted: true
            Material.accent: root.isConnected ? window.cRed : window.cPrimary
            onClicked: root.isConnected
                       ? root.disconnectRequested(root.connId)
                       : root.connectRequested(root.connId)
        }
    }

    // Context menu
    Menu {
        id: ctxMenu
        x: 0
        y: root.height
        width: 200

        MenuItem {
            text: qsTr("Copy share link")
            onTriggered: root.copyLinkRequested(root.connId)
        }
        MenuItem {
            text: qsTr("Test latency")
            onTriggered: root.latencyRequested(root.connId)
        }
        MenuSeparator {}
        MenuItem {
            text: qsTr("Delete")
            onTriggered: root.deleteRequested(root.connId)
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: ctxMenu.popup()
    }
}
