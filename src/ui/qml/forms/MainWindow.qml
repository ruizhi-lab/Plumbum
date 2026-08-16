import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "components"

ApplicationWindow {
    id: window
    width: 1100
    height: 720
    minimumWidth: 860
    minimumHeight: 560
    visible: true
    title: qsTr("Plumbum - Xray/V2Ray Client")

    // Modern dark palette
    readonly property color cBackground: "#0f1115"
    readonly property color cSurface: "#161a22"
    readonly property color cSurfaceAlt: "#1c212c"
    readonly property color cBorder: "#262d3b"
    readonly property color cPrimary: "#5b8cff"
    readonly property color cPrimaryAlt: "#7aa2ff"
    readonly property color cText: "#dbe2f0"
    readonly property color cTextDim: "#7f8ba3"
    readonly property color cGreen: "#3ddc97"
    readonly property color cRed: "#ff6b6b"
    readonly property color cOrange: "#ffb86c"
    readonly property color cAccent: "#bd93f9"

    Material.theme: Material.Dark
    Material.accent: cPrimary
    Material.background: cBackground
    Material.foreground: cText

    background: Rectangle {
        color: cBackground
    }

    // ============================= Layout =============================
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // -------- Sidebar --------
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 210
            color: cSurface

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 0
                spacing: 0

                // Logo & Title
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 84
                    color: "transparent"

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 2

                        Text {
                            text: "⚡ Plumbum"
                            font.pixelSize: 20
                            font.bold: true
                            color: cText
                        }
                        Text {
                            text: "Xray / V2Ray Client"
                            font.pixelSize: 11
                            color: cTextDim
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: cBorder
                }

                // Navigation
                SideBarButton {
                    id: navConnections
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    buttonText: qsTr("Connections")
                    buttonIcon: "▤"
                    isActive: stackView.currentIndex === 0
                    onClicked: stackView.currentIndex = 0
                }
                SideBarButton {
                    id: navSubscriptions
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    buttonText: qsTr("Subscriptions")
                    buttonIcon: "⇄"
                    isActive: stackView.currentIndex === 1
                    onClicked: stackView.currentIndex = 1
                }
                SideBarButton {
                    id: navSettings
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    buttonText: qsTr("Settings")
                    buttonIcon: "⚙"
                    isActive: stackView.currentIndex === 2
                    onClicked: stackView.currentIndex = 2
                }

                Item { Layout.fillHeight: true }

                // Connection status indicator
                Rectangle {
                    Layout.fillWidth: true
                    Layout.margins: 12
                    Layout.preferredHeight: 46
                    radius: 8
                    color: cSurfaceAlt

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        Rectangle {
                            id: statusDot
                            width: 10
                            height: 10
                            radius: 5
                            color: plumbum.connected ? cGreen : cTextDim
                            Behavior on color { ColorAnimation { duration: 200 } }
                        }
                        Text {
                            Layout.fillWidth: true
                            text: plumbum.connected
                                  ? qsTr("Connected")
                                  : qsTr("Disconnected")
                            color: plumbum.connected ? cGreen : cTextDim
                            font.pixelSize: 12
                            elide: Text.ElideRight
                        }
                    }
                }

                // Version
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.bottomMargin: 10
                    text: "v2.7.0 · Qt6"
                    font.pixelSize: 10
                    color: cTextDim
                }
            }
        }

        // -------- Content --------
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: cBackground

            // Top status bar
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                TopStatusBar {
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: cBorder
                }

                StackLayout {
                    id: stackView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: 0

                    ConnectionPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                    SubscriptionPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                    SettingsPage {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }

    // Toast notifications
    Rectangle {
        id: toast
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 28
        width: toastText.implicitWidth + 48
        height: 42
        radius: 21
        color: Qt.rgba(0.1, 0.12, 0.18, 0.92)
        border.color: cBorder
        z: 100

        Text {
            id: toastText
            anchors.centerIn: parent
            color: cText
            font.pixelSize: 13
        }

        NumberAnimation on opacity {
            from: 0
            to: 1
            duration: 150
        }

        SequentialAnimation {
            id: toastAnim
            running: false
            NumberAnimation { target: toast; property: "opacity"; to: 1; duration: 120 }
            PauseAnimation { duration: 2200 }
            NumberAnimation { target: toast; property: "opacity"; to: 0; duration: 400 }
            PropertyAction { target: toast; property: "visible"; value: false }
        }

        function show(msg: string) {
            toastText.text = msg
            toast.visible = true
            toast.opacity = 1
            toastAnim.restart()
        }
    }

    Connections {
        target: plumbum
        function onToastMessage(message: string) { toast.show(message) }
    }
}
