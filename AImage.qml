import QtQuick 2.0
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1
import imageProvider 1.0

Item {
    id: root

    anchors.fill: parent
    layer.enabled: true

    property bool imageLoaded
    property string imagePathName

    Component.onDestruction: clearImageSource()

    function clearImageSource() {
        liveImageItem.clearImage()
        root.imageLoaded = false
    }

    Connections {
        target: liveImageItem

        onImageChanged: {
            root.imageLoaded = true
            root.imagePathName = liveImageItem.fileNmae()
        }
    }

    Rectangle {
        id: rootRect

        anchors.fill: parent
        border.width: 1
        border.color: "#1b262c"
        color: "#204051"

        ComponentLabel {
            id: label

            name: "Image"
            pixelSize: 20
            weight: Font.Bold
        }

        Rectangle {
            id: videoRect

            width: parent.width * (3/4) * 0.65
            height: parent.height * (3/4) * 0.65
            anchors.left: parent.left
            anchors.right: settingsRect.left
            anchors.margins: 10
            anchors.verticalCenter: parent.verticalCenter

            ImageItem {
              id: liveImageItem

              anchors.fill: parent
            }

            Rectangle {
                id: placeHolder

                anchors.fill: parent
                color: !root.imageLoaded ? "#204051" : "transparent"
                border.width: !root.imageLoaded ? 1 : 0
                border.color: "black"

                Image {
                    id: placeHolderImage

                    anchors.centerIn: parent
                    source: "icons/perm_media-black-48dp.svg"
                    visible: !root.imageLoaded
                }
            }
        }

        Column {
            id: menuColumn

            width: videoRect.width
            anchors.top: videoRect.bottom
            anchors.margins: 10

            Text {
                id: imageNameText

                anchors.horizontalCenter: parent.horizontalCenter
                font.weight: Font.Medium
                font.pixelSize: 16
                text: "Image path: " + root.imagePathName
                wrapMode: Text.Wrap
            }

            Row {
                id: emptySpaceRow

                height: 10
                x:10

                Rectangle {
                    width: videoRect.width
                    height: parent.height
                    color: "transparent"
                }
            }

            Row {
                id: menuRow

                height: 25
                anchors.margins: 10
                x: 10

                AButton {
                    id: playButton

                    height: parent.height
                    width: 60
                    name: "Load file"
                    mouseArea.onClicked: liveImageItem.readImage()
                }
            }
        }

        Rectangle {
            id: settingsRect

            height: rootRect.height
            width: rootRect.width/2 - 10
            anchors.right: rootRect.right

            color: "#204051"

            Column {
                id: settingsColumn

                width: (parent.width) * (0.7)
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                ASlider {
                    id: alphaSlider

                    width: parent.width
                    slider.from: 0
                    slider.to: 10
                    slider.stepSize: 1
                    slider.value: 1
                    name: "Alpha"

                    slider.onValueChanged: liveImageItem.alpha = slider.value
                }

                ASlider {
                    id: betaSlider

                    width: parent.width
                    slider.from: 0
                    slider.to: 100
                    slider.stepSize: 1
                    slider.value: 1
                    name: "Beta"

                    slider.onValueChanged: liveImageItem.beta = slider.value
                }

                ASlider {
                    id: gammaCorrectionSlider

                    width: parent.width
                    slider.from: 0
                    slider.to: 10
                    slider.stepSize: 1
                    slider.value: 1
                    name: "Gamma correction"

                    slider.onValueChanged: liveImageItem.gamma = slider.value
                }
            }
        }
    }
}
