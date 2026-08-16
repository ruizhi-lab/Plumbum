import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"

// Subscription management page
Rectangle {
    id: root
    color: "transparent"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Toolbar section
        SectionCard {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 10

                Text {
                    text: qsTr("Subscriptions")
                    font.pixelSize: 17
                    font.bold: true
                    color: window.cText
                }

                Item { Layout.fillWidth: true }

                Button {
                    Layout.preferredHeight: 34
                    text: qsTr("Update All")
                    onClicked: plumbum.updateAllSubscriptions()
                }
                Button {
                    Layout.preferredHeight: 34
                    text: qsTr("Add Subscription")
                    highlighted: true
                    onClicked: addSubDialog.open()
                }
            }
        }

        // Subscription list section
        SectionCard {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: subList
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                clip: true
                model: plumbum.groupModel

            delegate: Rectangle {
                required property string groupId
                required property string displayName
                required property bool isSubscription
                required property int connectionCount
                required property string subscriptionAddress

                visible: isSubscription
                height: visible ? 76 : 0
                width: subList.width
                radius: 10
                color: window.cSurface
                border.color: window.cBorder

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 18
                    anchors.rightMargin: 12
                    spacing: 14

                    Text {
                        text: "⇄"
                        font.pixelSize: 18
                        color: window.cPrimaryAlt
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        RowLayout {
                            spacing: 8
                            Text {
                                text: displayName
                                font.pixelSize: 14
                                font.bold: true
                                color: window.cText
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                            Text {
                                text: qsTr("%1 nodes").arg(connectionCount)
                                font.pixelSize: 10
                                color: window.cTextDim
                            }
                        }

                        Text {
                            text: subscriptionAddress
                            font.pixelSize: 11
                            color: window.cTextDim
                            elide: Text.ElideMiddle
                            Layout.fillWidth: true
                        }
                    }

                    Button {
                        Layout.preferredHeight: 30
                        text: qsTr("Update")
                        onClicked: plumbum.updateSubscription(groupId)
                    }
                    Button {
                        Layout.preferredHeight: 30
                        text: qsTr("Open")
                        onClicked: {
                            plumbum.currentGroupId = groupId
                            stackView.currentIndex = 0
                        }
                    }
                    Button {
                        Layout.preferredHeight: 30
                        text: qsTr("Delete")
                        Material.accent: window.cRed
                        onClicked: {
                            deleteSubDialog.groupToDelete = groupId
                            deleteSubDialog.groupName = displayName
                            deleteSubDialog.open()
                        }
                    }
                }
            }

            // Empty state
            ColumnLayout {
                anchors.centerIn: parent
                visible: subList.count === 0 || !hasAnySubscription
                spacing: 8

                Text {
                    text: "📡"
                    font.pixelSize: 40
                    Layout.alignment: Qt.AlignHCenter
                }
                Text {
                    text: qsTr("No subscriptions yet")
                    font.pixelSize: 13
                    color: window.cTextDim
                    Layout.alignment: Qt.AlignHCenter
                }
                Text {
                    text: qsTr("Add a subscription URL to auto-import server lists")
                    font.pixelSize: 11
                    color: window.cTextDim
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
        }
    }

    readonly property bool hasAnySubscription: {
        for (var i = 0; i < plumbum.groupModel.rowCount(); i++) {
            if (plumbum.groupModel.data(plumbum.groupModel.index(i, 0), 0x0102))
                return true
        }
        return false
    }

    // Add subscription dialog
    Dialog {
        id: addSubDialog
        title: qsTr("Add subscription")
        anchors.centerIn: parent
        width: 520
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            TextField {
                id: subNameInput
                Layout.fillWidth: true
                placeholderText: qsTr("Subscription name")
            }
            TextField {
                id: subUrlInput
                Layout.fillWidth: true
                placeholderText: "https://example.com/subscribe"
                inputMethodHints: Qt.ImhUrlCharactersOnly
            }
        }

        onAccepted: {
            var gid = plumbum.createSubscription(subNameInput.text, subUrlInput.text)
            if (gid.length > 0)
                plumbum.updateSubscription(gid)
        }
        onOpened: {
            subNameInput.text = ""
            subUrlInput.text = ""
        }
    }

    // Delete subscription confirm
    Dialog {
        id: deleteSubDialog
        property string groupToDelete: ""
        property string groupName: ""
        title: qsTr("Confirm")
        anchors.centerIn: parent
        width: 420
        modal: true
        standardButtons: Dialog.Yes | Dialog.No

        Text {
            anchors.fill: parent
            text: qsTr("Delete subscription \"%1\" and all its connections?").arg(deleteSubDialog.groupName)
            wrapMode: Text.Wrap
        }

        onAccepted: plumbum.deleteGroup(deleteSubDialog.groupToDelete)
    }
}
