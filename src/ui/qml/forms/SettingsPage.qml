import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Settings page
Rectangle {
    id: root
    color: "transparent"

    Flickable {
        anchors.fill: parent
        anchors.margins: 20
        contentHeight: settingsColumn.implicitHeight
        clip: true

        ColumnLayout {
            id: settingsColumn
            width: parent.width
            spacing: 14

            Text {
                text: qsTr("Settings")
                font.pixelSize: 18
                font.bold: true
                color: window.cText
            }

            // -------- Core settings card --------
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: coreGrid.implicitHeight + 36
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 12

                    Text {
                        text: qsTr("V2Ray / Xray Core")
                        font.pixelSize: 13
                        font.bold: true
                        color: window.cPrimaryAlt
                    }

                    GridLayout {
                        id: coreGrid
                        columns: 2
                        columnSpacing: 12
                        rowSpacing: 10

                        Text {
                            text: qsTr("Core executable:")
                            font.pixelSize: 12
                            color: window.cTextDim
                            Layout.preferredWidth: 140
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            TextField {
                                id: corePathInput
                                Layout.fillWidth: true
                                text: plumbum.v2rayCorePath
                                placeholderText: qsTr("e.g. /usr/local/bin/xray or v2ray")
                            }
                            Button {
                                text: qsTr("Apply")
                                enabled: corePathInput.text !== plumbum.v2rayCorePath
                                onClicked: plumbum.setV2rayCorePath(corePathInput.text)
                            }
                        }

                        Text {
                            text: qsTr("Assets directory:")
                            font.pixelSize: 12
                            color: window.cTextDim
                            Layout.preferredWidth: 140
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            TextField {
                                id: assetsPathInput
                                Layout.fillWidth: true
                                text: plumbum.v2rayAssetsPath
                                placeholderText: qsTr("geoip.dat / geosite.dat directory")
                            }
                            Button {
                                text: qsTr("Apply")
                                enabled: assetsPathInput.text !== plumbum.v2rayAssetsPath
                                onClicked: plumbum.setV2rayAssetsPath(assetsPathInput.text)
                            }
                        }

                        Text {
                            text: qsTr("API Statistics:")
                            font.pixelSize: 12
                            color: window.cTextDim
                            Layout.preferredWidth: 140
                        }
                        Switch {
                            checked: plumbum.kernelApiEnabled
                            onToggled: plumbum.setKernelApiEnabled(checked)
                        }

                        Text {
                            text: qsTr("Stats port:")
                            font.pixelSize: 12
                            color: window.cTextDim
                            Layout.preferredWidth: 140
                        }
                        SpinBox {
                            from: 1024
                            to: 65535
                            value: plumbum.statsPort
                            editable: true
                            onValueModified: plumbum.setStatsPort(value)
                        }
                    }
                }
            }

            // -------- About card --------
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 140
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 6

                    Text {
                        text: "⚡ Plumbum"
                        font.pixelSize: 16
                        font.bold: true
                        color: window.cText
                    }
                    Text {
                        text: qsTr("A modern Qt6 Xray / V2Ray client")
                        font.pixelSize: 12
                        color: window.cTextDim
                    }
                    Text {
                        text: qsTr("Protocols: VMess, VLESS, Shadowsocks, Trojan, HTTP, SOCKS and more")
                        font.pixelSize: 11
                        color: window.cTextDim
                    }
                    Item { Layout.fillHeight: true }
                    Text {
                        text: qsTr("Kernel: xray-core / v2ray-core (v5 config)")
                        font.pixelSize: 11
                        color: window.cTextDim
                    }
                }
            }
        }
    }
}
