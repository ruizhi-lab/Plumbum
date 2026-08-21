import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

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
            Layout.preferredWidth: 250

            RowLayout {
                spacing: 4
                Layout.alignment: Qt.AlignHCenter

                // PAC mode icon
                Item {
                    Layout.preferredWidth: 14
                    Layout.preferredHeight: 14
                    Image {
                        anchors.fill: parent
                        source: "data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' fill='%23000000'><path d='M0 0h24v24H0z' fill='none'/><path d='M19 19h-5v-5h5v5zm-10 0H4v-5h5v5zm10-10h-5V4h5v5zm-10 0H4V4h5v5z'/></svg>"
                        sourceSize.width: 14
                        sourceSize.height: 14
                    }
                    ColorOverlay {
                        anchors.fill: parent
                        source: parent
                        color: window.cPrimaryAlt
                    }
                }
                Text {
                    text: qsTr("PAC Mode")
                    font.pixelSize: 9
                    font.bold: true
                    color: window.cTextDim
                }
            }

            RowLayout {
                spacing: 2
                Layout.alignment: Qt.AlignHCenter

                // Whitelist (bypass mainland)
                FlatButton {
                    controlHeight: 30
                    controlMinWidth: 70
                    text: qsTr("Whitelist")
                    active: plumbum.pacMode === 0
                    onClicked: plumbum.pacMode = 0
                }
                // Blacklist (GFW)
                FlatButton {
                    controlHeight: 30
                    controlMinWidth: 70
                    text: qsTr("Blacklist")
                    active: plumbum.pacMode === 1
                    onClicked: plumbum.pacMode = 1
                }
                // Global
                FlatButton {
                    controlHeight: 30
                    controlMinWidth: 70
                    text: qsTr("Global")
                    active: plumbum.pacMode === 2
                    onClicked: plumbum.pacMode = 2
                }
            }
        }

        // Connection name
        ColumnLayout {
            id: connNameCol
            Layout.fillWidth: true
            Layout.minimumWidth: 140
            Layout.preferredWidth: 220
            Layout.maximumWidth: 360
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
                Layout.minimumWidth: 0
            }
            Text {
                visible: !plumbum.connected && plumbum.kernelStatusText.length > 0
                text: plumbum.kernelStatusText
                font.pixelSize: 9
                color: window.cOrange
                elide: Text.ElideRight
                Layout.fillWidth: true
                Layout.minimumWidth: 0
            }
        }

        // Speed widget
        Rectangle {
            id: speedBox
            Layout.preferredWidth: 250
            Layout.minimumWidth: 230
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
                    Layout.preferredWidth: 72
                    horizontalAlignment: Text.AlignRight
                }
                // Col 3: total label (fixed, right-aligned)
                Text {
                    text: qsTr("Total ↑")
                    font.pixelSize: 9
                    color: window.cTextDim
                    Layout.preferredWidth: 38
                    horizontalAlignment: Text.AlignRight
                }
                // Col 4: total value (fixed, right-aligned, monospace)
                Text {
                    text: plumbum.upTotalText || "0 B"
                    font.pixelSize: 10
                    font.family: "monospace"
                    color: window.cTextDim
                    Layout.preferredWidth: 68
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
                    Layout.preferredWidth: 72
                    horizontalAlignment: Text.AlignRight
                }
                Text {
                    text: qsTr("Total ↓")
                    font.pixelSize: 9
                    color: window.cTextDim
                    Layout.preferredWidth: 38
                    horizontalAlignment: Text.AlignRight
                }
                Text {
                    text: plumbum.downTotalText || "0 B"
                    font.pixelSize: 10
                    font.family: "monospace"
                    color: window.cTextDim
                    Layout.preferredWidth: 68
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        // Disconnect button (visible when connected)
        FlatButton {
            id: disconnectBtn
            visible: plumbum.connected
            text: qsTr("Disconnect")
            danger: true
            onClicked: plumbum.disconnectConnection()
        }
    }
}
