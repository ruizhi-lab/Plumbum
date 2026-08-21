import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Settings page
Rectangle {
    id: root
    color: "transparent"

    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        contentWidth: availableWidth
        contentHeight: settingsColumn.implicitHeight
        clip: true

        ColumnLayout {
            id: settingsColumn
            width: Math.min(parent.width, 920)
            x: Math.max(0, (parent.width - width) / 2)
            spacing: 16

            Text {
                text: qsTr("Settings")
                font.pixelSize: 24
                font.bold: true
                color: window.cText
                Layout.bottomMargin: 2
            }

            // -------- Appearance / theme card --------
            Rectangle {
                id: appearanceCard
                Layout.fillWidth: true
                implicitHeight: appearanceLayout.implicitHeight + 36
                Layout.preferredHeight: implicitHeight
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    id: appearanceLayout
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
                        Layout.fillWidth: true
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

                        Item { Layout.fillWidth: true }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        Text {
                            text: qsTr("Language")
                            font.pixelSize: 12
                            color: window.cTextDim
                            Layout.fillWidth: true
                        }
                        ComboBox {
                            id: langSelector
                            Layout.preferredWidth: 160
                            Layout.preferredHeight: 36
                            model: [
                                { code: "en_US", label: "English" },
                                { code: "zh_CN", label: "简体中文" },
                                { code: "zh_TW", label: "繁體中文" },
                                { code: "ru_RU", label: "Русский" }
                            ]
                            textRole: "label"
                            Component.onCompleted: {
                                for (var i = 0; i < model.length; i++) {
                                    if (model[i].code === plumbum.language)
                                        currentIndex = i
                                }
                            }
                            onActivated: function(index) {
                                plumbum.setLanguage(model[index].code)
                            }
                        }
                    }
                }
            }

            // -------- Core settings card --------
            Rectangle {
                id: coreCard
                Layout.fillWidth: true
                implicitHeight: coreLayout.implicitHeight + 36
                Layout.preferredHeight: implicitHeight
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    id: coreLayout
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
                            Layout.alignment: Qt.AlignVCenter
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
                            Layout.alignment: Qt.AlignVCenter
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
                            Layout.alignment: Qt.AlignVCenter
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
                            Layout.alignment: Qt.AlignVCenter
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
                id: tunCard
                Layout.fillWidth: true
                implicitHeight: tunLayout.implicitHeight + 36
                Layout.preferredHeight: implicitHeight
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    id: tunLayout
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
                        text: qsTr("⚠ TUN requires an Xray core and root privileges or CAP_NET_ADMIN. Please select Xray and grant the required capability if needed.")
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
                            Layout.alignment: Qt.AlignVCenter
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
                            Layout.alignment: Qt.AlignVCenter
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
                id: aboutCard
                Layout.fillWidth: true
                implicitHeight: aboutLayout.implicitHeight + 36
                Layout.preferredHeight: implicitHeight
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                ColumnLayout {
                    id: aboutLayout
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
