import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1

import swoc 1.0

ApplicationWindow {
    visibility: ApplicationWindow.Windowed
    visible: true
    width: 480
    height: 640

    RemoteController {
        id: remoteController
    }

    Item {
        anchors.fill: parent
        GridLayout {
            anchors.left: parent.left
            anchors.leftMargin: 32
            anchors.top: parent.top
            anchors.topMargin: 32
            columnSpacing: 16
            columns: 2
            TextField {
                id: addressField
                placeholderText: "address"
                Layout.fillWidth: true
                background.width: width
            }
            Button {
                text: "Connect"
                onClicked: remoteController.connect(addressField.text)
            }
            Rectangle {
                Layout.topMargin: 16
                Layout.bottomMargin: 16
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                Layout.columnSpan: 2
                color: "black"
            }
            Label {
                text: "Maze size"
            }

            SpinBox {
                id: mazeSizeSpinBox
                from: 3
                to: 11
                value: 10
            }
            CheckBox {
                id: removeDeadEndsCheckBox
                checked: true
                text: "Remove dead ends"
            }
            Spacer {}
            Label {
                text: "Number of walls to remove"
            }
            SpinBox {
                id: numberOfWallsToRemoveSpinBox
                from: 0
                to: 100
                value: 20
            }
            Row {
                Layout.columnSpan: 2
                Layout.topMargin: 16
                spacing: 16
                Button {
                    text: "Create Game"
                    onClicked: remoteController.createGame(Qt.size(mazeSizeSpinBox.value, mazeSizeSpinBox.value), removeDeadEndsCheckBox.checked, numberOfWallsToRemoveSpinBox.value)
                }
                Button {
                    text: "Start Game"
                    onClicked: remoteController.startGame()
                }
                Button {
                    text: "Stop Game"
                    onClicked: remoteController.stopGame()
                }
            }
            Spacer {}
            Rectangle {
                Layout.topMargin: 16
                Layout.bottomMargin: 16
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                Layout.columnSpan: 2
                color: "black"
            }
            TextField {
                id: arucoIdTextField
                placeholderText: "arucoId"
            }
            Spacer {}
            TextField {
                id: teamNameTextField
                placeholderText: "teamName"
            }
            Spacer {}
            Row {
                spacing: 16
                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 32
                    height: 32
                    color: colorDialog.color
                }
                Button {
                    text: "Choose color"
                    onClicked: colorDialog.open()
                    ColorDialog {
                        id: colorDialog
                        title: "Please choose a color"
                        color: "red"
                    }
                }
            }
            Spacer {}
            Button {
                text: "Register"
                onClicked: {}
            }
        }
    }
}
