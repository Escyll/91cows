import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1

import swoc 1.0

ApplicationWindow {
    visibility: ApplicationWindow.Windowed
    visible: true
    width: 480
    height: 1000

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
            Label {
                text: "Number of coins"
            }
            SpinBox {
                id: numberOfCoins
                from: 0
                to: 100
                value: 10
            }
            Label {
                text: "Number of treasure chests"
            }
            SpinBox {
                id: numberOfTreasureChests
                from: 0
                to: 10
                value: 2
            }
            Label {
                text: "Number of empty chests"
            }
            SpinBox {
                id: numberOfEmptyChests
                from: 0
                to: 10
                value: 1
            }
            Label {
                text: "Number of mimic chests"
            }
            SpinBox {
                id: numberOfMimicChests
                from: 0
                to: 10
                value: 1
            }
            Label {
                text: "Number of spike traps"
            }
            SpinBox {
                id: numberOfSpikeTraps
                from: 0
                to: 10
                value: 4
            }
            Label {
                text: "Number of wall bottles"
            }
            SpinBox {
                id: numberOfWallBottles
                from: 0
                to: 10
                value: 4
            }
            Label {
                text: "Number of spike trap bottles"
            }
            SpinBox {
                id: numberOfSpikeTrapBottles
                from: 0
                to: 10
                value: 4
            }
            Row {
                Layout.columnSpan: 2
                Layout.topMargin: 16
                spacing: 16
                Button {
                    text: "Create Game"
                    onClicked: remoteController.createGame(Qt.size(mazeSizeSpinBox.value, mazeSizeSpinBox.value),
                                                           removeDeadEndsCheckBox.checked,
                                                           numberOfWallsToRemoveSpinBox.value,
                                                           numberOfCoins.value,
                                                           numberOfTreasureChests.value,
                                                           numberOfEmptyChests.value,
                                                           numberOfMimicChests.value,
                                                           numberOfSpikeTraps.value,
                                                           numberOfWallBottles.value,
                                                           numberOfSpikeTrapBottles.value
                                                           )
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
                onClicked: {
                    console.log("clicked")
                    var arucoId = Number(arucoIdTextField.text)
                    if (!isNaN(arucoId)) {
                        console.log("check")
                        remoteController.registerTeam(arucoId, teamNameTextField.text, colorDialog.color)
                    }
                }
            }
        }
    }
}
