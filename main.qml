import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: root

    visible: true
    width: 1080
    height: 600
    minimumWidth: 640
    minimumHeight: 480
    title: qsTr("Aura")
    color: "#204051"

    Item {
        id: chipItem

        anchors.fill: parent
        layer.enabled: true

        Rectangle {
            id: circle

            width: imageChip.width + videoChip.width + audioChip.width
            height: imageChip.height + videoChip.height
            anchors.centerIn: parent
            border.width: 1
            border.color: "white"
            color: "transparent"
            radius: width/2

            Chip {
                id: imageChip


                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: circle.top
                name.text: "Image"
                mouseArea.onClicked: {
                    chipItem.enabled = false
                    loader.source = "AImage.qml"
                }
            }

            Chip {
                id: videoChip

                anchors.top: imageChip.bottom
                anchors.horizontalCenter: circle.horizontalCenter
                anchors.horizontalCenterOffset: -imageChip.width
                name.text: "Video"
                mouseArea.onClicked: {
                    chipItem.enabled = false
                    loader.source = "Video.qml"
                }
            }

            Chip {
                id: audioChip

                anchors.top: imageChip.bottom
                anchors.horizontalCenter: circle.horizontalCenter
                anchors.horizontalCenterOffset: imageChip.width
                name.text: "Audio"
                mouseArea.onClicked: {
                    chipItem.enabled = false
                    loader.source = "Audio.qml"
                }
            }
        }
    }

    Loader {
        id: loader

        anchors.fill: parent
    }

    Backbutton {
        id: backButton

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10

        mouseArea.onClicked: {
            chipItem.enabled = true
            loader.source = ""
        }
    }

}
