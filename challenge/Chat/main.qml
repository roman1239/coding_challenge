import QtQuick 2.6
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import io.qt.test.BackEnd 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 800
    property alias messageGuest_textArea: messageGuest_textArea
    property alias historyGuest_textEdit: historyGuest_textEdit
    property alias sendGuest_button: sendGuest_button
    property alias sendMe_button: sendMe_button
    property alias historyMe_textEdit: historyMe_textEdit
    property alias messageMe_textArea: messageMe_textArea
    visible: true

    BackEnd
    {
        id: backEnd
    }

    Grid {
        id: grid
        y: 277
        height: 400
        anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        clip: false
        transformOrigin: Item.Center
        topPadding: 0
        spacing: 0
        columns: 1
        rows: 2

        Row {
            id: row
            width: 800
            height: 400

            TextEdit {
                id: historyMe_textEdit
                width: 760
                height: 300
                text: qsTr("")
                readOnly: true
                verticalAlignment: Text.AlignTop
                layer.enabled: false
                anchors.top: parent.top
                anchors.topMargin: 20
                enabled: false
                visible: true
                clip: false
                anchors.left: parent.left
                anchors.leftMargin: 20
                font.pixelSize: 16
            }

            Button {
                id: sendMe_button
                x: 700
                y: 330
                width: 80
                height: 50
                text: qsTr("Send")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20

                onClicked: {
                    backEnd.msgMyChanged( messageMe_textArea.text )
                    historyMe_textEdit.text += "Roman: " + backEnd.getMsgMy + "\n"
                    historyGuest_textEdit.text += "Roman: " + backEnd.getMsgMy + "\n"
                    messageMe_textArea.text = ""
                }
            }

            TextArea {
                id: messageMe_textArea
                y: 330
                width: 680
                height: 50
                font.pointSize: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                visible: true
                enabled: true
                anchors.left: parent.left
                anchors.leftMargin: 20
                placeholderText: qsTr("Enter message")

                onLineCountChanged:{
                    if( messageMe_textArea.lineCount != 1 )
                    {
                        backEnd.msgMyChanged( messageMe_textArea.text )
                        historyMe_textEdit.text += "Roman: " + backEnd.getMsgMy
                        historyGuest_textEdit.text += "Roman: " + backEnd.getMsgMy
                        messageMe_textArea.text = ""
                    }
                }
            }
        }

        Row {
            id: row1
            width: 800
            height: 400
            TextEdit {
                id: historyGuest_textEdit
                width: 760
                height: 300
                text: qsTr("")
                readOnly: true
                verticalAlignment: Text.AlignTop
                anchors.topMargin: 20
                visible: true
                enabled: false
                layer.enabled: false
                clip: false
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 20
                font.pixelSize: 16
            }

            Button {
                id: sendGuest_button
                x: 700
                y: 330
                width: 80
                height: 50
                text: qsTr("Send")
                anchors.bottomMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottom: parent.bottom

                onClicked: {
                        backEnd.msgGuestChanged( messageGuest_textArea.text )
                        historyGuest_textEdit.text += "Guest: " + backEnd.getMsgGuest + "\n"
                        historyMe_textEdit.text += "Guest: " + backEnd.getMsgGuest + "\n"
                        messageGuest_textArea.text = ""
                }
            }

            TextArea {
                id: messageGuest_textArea
                y: 330
                width: 680
                height: 50
                font.pointSize: 10
                enabled: true
                visible: true
                anchors.bottomMargin: 20
                placeholderText: qsTr("Enter message")
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.bottom: parent.bottom

                onLineCountChanged:{
                    if( messageGuest_textArea.lineCount != 1 )
                    {
                        backEnd.msgGuestChanged( messageGuest_textArea.text )
                        historyGuest_textEdit.text += "Guest: " + backEnd.getMsgGuest
                        historyMe_textEdit.text += "Guest: " + backEnd.getMsgGuest
                        messageGuest_textArea.text = ""
                    }
                }
            }
        }
    }
}
