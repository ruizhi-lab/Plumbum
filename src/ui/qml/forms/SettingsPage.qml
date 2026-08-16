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

            // -------- Appearance / theme card --------
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 96
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 12

                    Text {
                        text: qsTr("Appearance")
                        font.pixelSize: 13
                        font.bold: true
                        color: window.cPrimaryAlt
                    }

                    RowLayout {
                        spacing: 10

                        // Follow system
                        Rectangle {
                            Layout.preferredWidth: 110
                            Layout.preferredHeight: 34
                            radius: 6
                            color: plumbum.themeMode === 0 ? window.cPrimary : window.cSurfaceAlt
                            border.color: plumbum.themeMode === 0 ? window.cPrimary : window.cBorder

                            Text {
                                anchors.centerIn: parent
                                text: qsTr("Follow System")
                                font.pixelSize: 11
                                font.bold: plumbum.themeMode === 0
                                color: plumbum.themeMode === 0 ? "#ffffff" : window.cTextDim
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: plumbum.setThemeMode(0)
                            }
                        }
                        // Light
                        Rectangle {
                            Layout.preferredWidth: 110
                            Layout.preferredHeight: 34
                            radius: 6
                            color: plumbum.themeMode === 1 ? window.cPrimary : window.cSurfaceAlt
                            border.color: plumbum.themeMode === 1 ? window.cPrimary : window.cBorder

                            Text {
                                anchors.centerIn: parent
                                text: qsTr("Light")
                                font.pixelSize: 11
                                font.bold: plumbum.themeMode === 1
                                color: plumbum.themeMode === 1 ? "#ffffff" : window.cTextDim
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: plumbum.setThemeMode(1)
                            }
                        }
                        // Dark
                        Rectangle {
                            Layout.preferredWidth: 110
                            Layout.preferredHeight: 34
                            radius: 6
                            color: plumbum.themeMode === 2 ? window.cPrimary : window.cSurfaceAlt
                            border.color: plumbum.themeMode === 2 ? window.cPrimary : window.cBorder

                            Text {
                                anchors.centerIn: parent
                                text: qsTr("Dark")
                                font.pixelSize: 11
                                font.bold: plumbum.themeMode === 2
                                color: plumbum.themeMode === 2 ? "#ffffff" : window.cTextDim
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: plumbum.setThemeMode(2)
                            }
                        }
                    }
                }
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

            // -------- TUN system proxy card --------
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: tunGrid.implicitHeight + 36
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 12

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Text {
                            text: qsTr("TUN System Proxy")
                            font.pixelSize: 13
                            font.bold: true
                            color: window.cPrimaryAlt
                            Layout.fillWidth: true
                        }
                        Switch {
                            checked: plumbum.tunEnabled
                            enabled: plumbum.tunAvailable
                            onToggled: plumbum.setTunEnabled(checked)
                        }
                    }

                    Text {
                        visible: plumbum.tunAvailable && !plumbum.tunEnabled
                        text: qsTr("Route all system traffic through the proxy via a virtual TUN interface.")
                        font.pixelSize: 10
                        color: window.cTextDim
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                    }

                    Text {
                        visible: !plumbum.tunAvailable
                        text: qsTr("⚠ TUN requires root privileges or CAP_NET_ADMIN. Please run Plumbum with sudo or grant the capability: sudo setcap cap_net_admin,cap_net_raw+eip $(which xray)")
                        font.pixelSize: 10
                        color: window.cOrange
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                    }

                    GridLayout {
                        id: tunGrid
                        columns: 2
                        columnSpacing: 12
                        rowSpacing: 10

                        Text {
                            text: qsTr("TUN IPv4:")
                            font.pixelSize: 12
                            color: window.cTextDim
                            Layout.preferredWidth: 140
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            TextField {
                                id: tunIpv4Input
                                Layout.fillWidth: true
                                text: plumbum.tunIpv4
                                validator: RegularExpressionValidator { regularExpression: /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/ }
                            }
                            Button {
                                text: qsTr("Apply")
                                enabled: tunIpv4Input.text !== plumbum.tunIpv4 && tunIpv4Input.acceptableInput
                                onClicked: plumbum.setTunIpv4(tunIpv4Input.text)
                            }
                        }

                        Text {
                            text: qsTr("MTU:")
                            font.pixelSize: 12
                            color: window.cTextDim
                            Layout.preferredWidth: 140
                        }
                        SpinBox {
                            from: 576
                            to: 65535
                            value: plumbum.tunMtu
                            editable: true
                            onValueModified: plumbum.setTunMtu(value)
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
