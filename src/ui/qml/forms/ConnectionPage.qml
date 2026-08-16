import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Main connections page
Rectangle {
    id: root
    color: "transparent"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // -------- Toolbar --------
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 64
            color: "transparent"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 10

                Text {
                    text: qsTr("Connections")
                    font.pixelSize: 18
                    font.bold: true
                    color: window.cText
                }

                Item { Layout.fillWidth: true }

                // Group selector
                ComboBox {
                    id: groupSelector
                    Layout.preferredWidth: 180
                    model: qv2ray.groupModel
                    textRole: "displayName"
                    currentIndex: groupModelIndexFor(qv2ray.currentGroupId)

                    function groupModelIndexFor(gid: string): int {
                        for (var i = 0; i < qv2ray.groupModel.rowCount(); i++) {
                            if (qv2ray.groupModel.data(qv2ray.groupModel.index(i, 0), 0x0101) === gid)
                                return i
                        }
                        return 0
                    }

                    onActivated: function(index) {
                        qv2ray.setCurrentGroupId(qv2ray.groupModel.data(qv2ray.groupModel.index(index, 0), 0x0101))
                    }

                    Component.onCompleted: {
                        qv2ray.setCurrentGroupId(qv2ray.groupModel.data(qv2ray.groupModel.index(currentIndex, 0), 0x0101))
                    }
                }

                Button {
                    text: qsTr("New Group")
                    onClicked: newGroupDialog.open()
                }

                Button {
                    text: qsTr("Import")
                    highlighted: true
                    onClicked: qv2ray.importFromClipboard()
                }

                Button {
                    text: qsTr("Import URL")
                    onClicked: importDialog.open()
                }

                Button {
                    text: qsTr("Latency Test")
                    onClicked: qv2ray.startLatencyTest()
                }

                Button {
                    visible: isCurrentGroupSubscription
                    text: qsTr("Update Subscription")
                    onClicked: qv2ray.updateSubscription(qv2ray.currentGroupId)
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: window.cBorder
        }

        // -------- Connection list --------
        ListView {
            id: connList
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 16
            spacing: 10
            clip: true
            model: qv2ray.connectionModel

            delegate: ConnectionCard {
                width: connList.width
                connId: connectionId
                displayName: displayName
                protocol: protocol
                address: address
                port: port
                latencyText: latencyText
                isConnected: isConnected
                upTotal: upTotal
                downTotal: downTotal

                onConnectRequested: function(id) { qv2ray.connectConnection(id) }
                onDisconnectRequested: function(id) { qv2ray.disconnectConnection() }
                onCopyLinkRequested: function(id) { qv2ray.copyConnectionLink(id) }
                onLatencyRequested: function(id) { qv2ray.startLatencyTestFor(id) }
                onDeleteRequested: function(id) {
                    deleteConfirm.text = qsTr("Delete connection \"%1\"?").arg(qv2ray.connectionDisplayName(id))
                    deleteConfirm.connToDelete = id
                    deleteConfirm.open()
                }
            }

            // Empty state
            Rectangle {
                anchors.centerIn: parent
                visible: connList.count === 0
                width: parent.width - 40
                height: 160
                color: "transparent"

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 8

                    Text {
                        text: "📭"
                        font.pixelSize: 40
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Text {
                        text: qsTr("No connections in this group")
                        font.pixelSize: 13
                        color: window.cTextDim
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Text {
                        text: qsTr("Click \"Import\" to add a server link (vmess://, vless://, ss://, trojan:// ...)")
                        font.pixelSize: 11
                        color: window.cTextDim
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
        }
    }

    readonly property bool isCurrentGroupSubscription: {
        for (var i = 0; i < qv2ray.groupModel.rowCount(); i++) {
            if (qv2ray.groupModel.data(qv2ray.groupModel.index(i, 0), 0x0101) === qv2ray.currentGroupId)
                return qv2ray.groupModel.data(qv2ray.groupModel.index(i, 0), 0x0103)
        }
        return false
    }

    // -------- Import URL dialog --------
    Dialog {
        id: importDialog
        title: qsTr("Import from link")
        anchors.centerIn: parent
        width: 520
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            Text {
                text: qsTr("Paste one or more share links below:")
                font.pixelSize: 12
                color: window.cTextDim
            }
            TextArea {
                id: importText
                Layout.fillWidth: true
                Layout.preferredHeight: 140
                placeholderText: "vmess://...\nvless://...\nss://...\ntrojan://..."
                wrapMode: TextEdit.Wrap
            }
        }

        onAccepted: qv2ray.importFromLink(importText.text)
        onOpened: importText.text = ""
    }

    // -------- New group dialog --------
    Dialog {
        id: newGroupDialog
        title: qsTr("Create new group")
        anchors.centerIn: parent
        width: 380
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            TextField {
                id: groupNameInput
                Layout.fillWidth: true
                placeholderText: qsTr("Group name")
            }
        }

        onAccepted: qv2ray.createGroup(groupNameInput.text)
        onOpened: groupNameInput.text = ""
    }

    // -------- Delete confirm dialog --------
    Dialog {
        id: deleteConfirm
        property string connToDelete: ""
        title: qsTr("Confirm")
        anchors.centerIn: parent
        width: 400
        modal: true
        standardButtons: Dialog.Yes | Dialog.No

        Text {
            id: deleteConfirmText
            anchors.fill: parent
            text: deleteConfirm.text
            wrapMode: Text.Wrap
        }

        onAccepted: qv2ray.deleteConnection(deleteConfirm.connToDelete)
    }
}
