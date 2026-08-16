import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Top status bar: connection name + realtime speed + total traffic
Rectangle {
    id: root
    height: 56
    color: window.cSurface

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 16

        // Connection name
        ColumnLayout {
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
            Layout.preferredWidth: 190
            Layout.preferredHeight: 40
            radius: 8
            color: window.cSurfaceAlt
            border.color: window.cBorder

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 14

                ColumnLayout {
                    spacing: 0
                    Text {
                        text: qsTr("↑")
                        font.pixelSize: 11
                        color: window.cGreen
                    }
                    Text {
                        text: qsTr("↓")
                        font.pixelSize: 11
                        color: window.cPrimaryAlt
                    }
                }
                ColumnLayout {
                    spacing: 0
                    Text {
                        text: plumbum.upSpeedText || "0 B/s"
                        font.pixelSize: 11
                        color: window.cText
                        font.family: "monospace"
                    }
                    Text {
                        text: plumbum.downSpeedText || "0 B/s"
                        font.pixelSize: 11
                        color: window.cText
                        font.family: "monospace"
                    }
                }
                Item { Layout.fillWidth: true }
                ColumnLayout {
                    spacing: 0
                    Text {
                        text: qsTr("Total ↑") + "  " + (plumbum.upTotalText || "0 B")
                        font.pixelSize: 9
                        color: window.cTextDim
                    }
                    Text {
                        text: qsTr("Total ↓") + "  " + (plumbum.downTotalText || "0 B")
                        font.pixelSize: 9
                        color: window.cTextDim
                    }
                }
            }
        }

        // Disconnect button (visible when connected)
        Button {
            visible: plumbum.connected
            text: qsTr("Disconnect")
            highlighted: true
            Material.accent: window.cRed
            onClicked: plumbum.disconnectConnection()
        }
    }
}
