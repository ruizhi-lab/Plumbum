import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Top status bar: PAC mode selector + connection name + realtime speed + total traffic
Rectangle {
    id: root
    height: 56
    color: window.cSurface

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 24
        spacing: 16


        // -------- PAC mode selector (v2rayN style) --------
        ColumnLayout {
            id: pacCol
            spacing: 2
            Layout.preferredWidth: 240

            Text {
                text: qsTr("PAC Mode")
                font.pixelSize: 9
                color: window.cTextDim
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                spacing: 2
                Layout.alignment: Qt.AlignHCenter

                // Whitelist (bypass mainland)
                Rectangle {
                    Layout.preferredWidth: 76
                    Layout.preferredHeight: 24
                    radius: 4
                    color: plumbum.pacMode === 0 ? window.cPrimary : window.cSurfaceAlt
                    border.color: plumbum.pacMode === 0 ? window.cPrimary : window.cBorder

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Whitelist")
                        font.pixelSize: 10
                        font.bold: plumbum.pacMode === 0
                        color: plumbum.pacMode === 0 ? "#ffffff" : window.cTextDim
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: plumbum.pacMode = 0
                    }
                }
                // Blacklist (GFW)
                Rectangle {
                    Layout.preferredWidth: 76
                    Layout.preferredHeight: 24
                    radius: 4
                    color: plumbum.pacMode === 1 ? window.cPrimary : window.cSurfaceAlt
                    border.color: plumbum.pacMode === 1 ? window.cPrimary : window.cBorder

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Blacklist")
                        font.pixelSize: 10
                        font.bold: plumbum.pacMode === 1
                        color: plumbum.pacMode === 1 ? "#ffffff" : window.cTextDim
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: plumbum.pacMode = 1
                    }
                }
                // Global
                Rectangle {
                    Layout.preferredWidth: 76
                    Layout.preferredHeight: 24
                    radius: 4
                    color: plumbum.pacMode === 2 ? window.cPrimary : window.cSurfaceAlt
                    border.color: plumbum.pacMode === 2 ? window.cPrimary : window.cBorder

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Global")
                        font.pixelSize: 10
                        font.bold: plumbum.pacMode === 2
                        color: plumbum.pacMode === 2 ? "#ffffff" : window.cTextDim
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: plumbum.pacMode = 2
                    }
                }
            }
        }

        // Connection name
        ColumnLayout {
            id: connNameCol
            Layout.fillWidth: true
            spacing: 1

            Text {
                text: plumbum.connected
                      ? qsTr("Current connection")
                      : qsTr("No active connection")
                font.pixelSize: 10
                color: window.cTextDim
            }
            Text {
                text: plumbum.connected ? plumbum.connectedName : "—"
                font.pixelSize: 15
                font.bold: true
                color: window.cText
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }

        // Speed widget
        Rectangle {
            id: speedBox
            Layout.preferredWidth: 284
            Layout.preferredHeight: 46
            radius: 8
            color: window.cSurfaceAlt
            border.color: window.cBorder

            GridLayout {
                anchors.fill: parent
                anchors.leftMargin: 14
                anchors.rightMargin: 16
                anchors.topMargin: 8
                anchors.bottomMargin: 8
                columns: 4
                columnSpacing: 6
                rowSpacing: 4

                // ---- Row 1: upload ----
                // Col 1: arrow (right-aligned, fixed)
                Text {
                    text: "↑"
                    font.pixelSize: 10
                    font.bold: true
                    color: window.cGreen
                    Layout.preferredWidth: 14
                    horizontalAlignment: Text.AlignRight
                }
                // Col 2: speed (fixed width, right-aligned, monospace)
                Text {
                    text: plumbum.upSpeedText || "0 B/s"
                    font.pixelSize: 12
                    font.bold: true
                    font.family: "monospace"
                    color: window.cText
                    Layout.preferredWidth: 92
                    horizontalAlignment: Text.AlignRight
                }
                // Col 3: total label (fixed, right-aligned)
                Text {
                    text: qsTr("Total ↑")
                    font.pixelSize: 9
                    color: window.cTextDim
                    Layout.preferredWidth: 46
                    horizontalAlignment: Text.AlignRight
                }
                // Col 4: total value (fixed, right-aligned, monospace)
                Text {
                    text: plumbum.upTotalText || "0 B"
                    font.pixelSize: 10
                    font.family: "monospace"
                    color: window.cTextDim
                    Layout.preferredWidth: 84
                    horizontalAlignment: Text.AlignRight
                }

                // ---- Row 2: download ----
                Text {
                    text: "↓"
                    font.pixelSize: 10
                    font.bold: true
                    color: window.cPrimaryAlt
                    Layout.preferredWidth: 14
                    horizontalAlignment: Text.AlignRight
                }
                Text {
                    text: plumbum.downSpeedText || "0 B/s"
                    font.pixelSize: 12
                    font.bold: true
                    font.family: "monospace"
                    color: window.cText
                    Layout.preferredWidth: 92
                    horizontalAlignment: Text.AlignRight
                }
                Text {
                    text: qsTr("Total ↓")
                    font.pixelSize: 9
                    color: window.cTextDim
                    Layout.preferredWidth: 46
                    horizontalAlignment: Text.AlignRight
                }
                Text {
                    text: plumbum.downTotalText || "0 B"
                    font.pixelSize: 10
                    font.family: "monospace"
                    color: window.cTextDim
                    Layout.preferredWidth: 84
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        // Disconnect button (visible when connected)
        Button {
            id: disconnectBtn
            visible: plumbum.connected
            text: qsTr("Disconnect")
            highlighted: true
            Material.accent: window.cRed
            onClicked: plumbum.disconnectConnection()
        }
    }
}
