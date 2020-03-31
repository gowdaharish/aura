import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: root

    anchors.fill: parent
    layer.enabled: true

    property bool providerRunning
    property string cameraName: providerRunning ? VideoProvider.getCameraName() : "None"

    Component.onDestruction: clearVideoProvider()

    function clearVideoProvider() {
        VideoProvider.terminate()
    }

    Connections {
        target: VideoProvider

        onRunningChanged: root.providerRunning = running
    }

    Rectangle {
        id: rootRect

        anchors.fill: parent
        color: "#204051"

        ComponentLabel {
            id: label

            name: "Video"
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

            property int frameNumber

            Image {
                id: video

                anchors.fill: parent
                asynchronous: true
                source: root.providerRunning ? "image://VideoProvider/image" + videoRect.frameNumber : ""
            }

            Rectangle {
                anchors.fill: parent
                color: root.providerRunning ? "transparent" : "#204051"
                border.width: root.providerRunning ? 0 : 1
                border.color: "black"

                Image {
                    id: placeHolderImage

                    anchors.centerIn: parent
                    source: "icons/linked_camera-black-48dp.svg"
                    visible: !root.providerRunning
                }
            }

            Timer {
                id: frameTimer

                interval: 100
                running: true
                repeat: true
                onTriggered: videoRect.frameNumber++
            }
        }

        Column {
            id: menuColumn

            width: videoRect.width
            anchors.top: videoRect.bottom
            anchors.margins: 10

            Text {
                id: camNameText

                anchors.horizontalCenter: parent.horizontalCenter
                font.weight: Font.Medium
                font.pixelSize: 16
                text: "Camera name: " + cameraName
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
                    name: "Play"
                    mouseArea.onClicked: VideoProvider.initiate()
                }

                Rectangle {
                    width: 10
                    height: 10
                    color: "transparent"
                }

                AButton {
                    id: stopButton

                    height: parent.height
                    width: 60
                    name: "Stop"
                    mouseArea.onClicked: VideoProvider.terminate()
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

                ComboBox {
                    id: formatList

                    width: parent.width
                    y: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    currentIndex: formatModel.count - 1

                    model: ListModel {
                        id: formatModel

                        ListElement { text: "Format_Mono"}
                        ListElement { text: "Format_RGB16"}
                        ListElement { text: "Format_RGB32"}
                        ListElement { text: "Format_RGB444"}
                        ListElement { text: "Format_RGB555"}
                        ListElement { text: "Format_RGB666"}
                        ListElement { text: "Format_RGB888"}
                    }

                    onCurrentIndexChanged: VideoProvider.format = formatModel.get(currentIndex).text
                }

                ASlider {
                    id: fpsSlider

                    width: parent.width
                    slider.from: 0
                    slider.to: 120
                    slider.stepSize: 1
                    slider.value: 30
                    name: "FPS"

                    slider.onValueChanged: VideoProvider.fps = slider.value
                }

                ResolutionBox {
                    id: resolutionBox

                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    applyButton.mouseArea.onClicked: VideoProvider.resolution = resolutionBox.resWidth + ":" +resolutionBox.resHeight
                }

                ASlider {
                    id: brightnessSlider

                    width: parent.width
                    slider.from: 0
                    slider.to: 10
                    slider.stepSize: 1
                    slider.value: 1
                    name: "Brightness"

                    slider.onValueChanged: VideoProvider.brightness = slider.value
                }

                ASlider {
                    id: contrastSlider

                    width: parent.width
                    slider.from: -10
                    slider.to: 10
                    slider.stepSize: 1
                    slider.value: 1
                    name: "Contrast"

                    slider.onValueChanged: VideoProvider.contrast = slider.value
                }

                ASlider {
                    id: saturationSlider

                    width: parent.width
                    slider.from: 0
                    slider.to: 1
                    slider.stepSize: 0.1
                    slider.value: 0.5
                    name: "Saturation"

                    slider.onValueChanged: VideoProvider.saturation = slider.value
                }

                ASlider {
                    id: exposureSlider

                    width: parent.width
                    slider.from: 0
                    slider.to: 1
                    slider.stepSize: 0.1
                    slider.value: 0.5
                    name: "Exposure"

                    slider.onValueChanged:  VideoProvider.exposure = slider.value
                }
            }
        }
    }
}
