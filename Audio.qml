import QtQuick 2.0
import QtQuick.Controls 2.12
//import audioProvider 1.0
import QtCharts 2.3

Item {
    id: root

    anchors.fill: parent
    layer.enabled: true

    property bool providerRunning: false
    property bool canvasDataAquired: false
    property int format
    property int channels
    property int sampleRate
    property int frames

//    AudioProvider {
//        id: audioProvider

//        onFileInfoUpdated: {
//            root.format = fileFormat
//            root.channels = fileChannels
//            root.sampleRate = fileSampleRate
//            root.frames = fileFramesNum
//            providerRunning = true
//        }
//    }

    Connections {
        target: AudioProvider

        onFileInfoUpdated: {
            root.format = fileFormat
            root.channels = fileChannels
            root.sampleRate = fileSampleRate
            root.frames = fileFramesNum
            providerRunning = true
        }
    }

    Component.onDestruction: clearAudioProvider()

    function clearAudioProvider() {
        AudioProvider.terminate()
    }

    Rectangle {
        id: rootRect

        anchors.fill: parent
        color: "#204051"

        ComponentLabel {
            id: label

            name: "Audio"
            pixelSize: 20
            weight: Font.Bold
        }

        Rectangle {
            id: displayRect

            width: parent.width * (3/4) * 0.65
            height: parent.height * (3/4) * 0.65
            anchors.left: parent.left
            anchors.right: settingsRect.left
            anchors.margins: 10
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                id: placeHolder

                anchors.fill: parent
                color: !root.canvasDataAquired ? "#204051" : "transparent"
                border.width: !root.imageLoaded ? 1 : 0
                border.color: "black"

                Image {
                    id: placeHolderImage

                    anchors.centerIn: parent
                    source: "icons/graphic_eq-black-48dp.svg"
                    visible: !root.canvasDataAquired
                }
            }
        }

        Column {
            id: menuColumn

            width: displayRect.width
            anchors.top: displayRect.bottom
            anchors.margins: 10

            Text {
                id: imageNameText

                anchors.horizontalCenter: parent.horizontalCenter
                font.weight: Font.Medium
                font.pixelSize: 16
                text: "Image: " + (root.providerRunning ? AudioProvider.filePath() : "None")
            }

            Row {
                id: emptySpaceRow

                height: 10
                x:10

                Rectangle {
                    width: displayRect.width
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
                    id: loadButton

                    height: parent.height
                    width: 60
                    name: "Load file"
                    mouseArea.onClicked: AudioProvider.loadAudioFile()
                }

                Rectangle {
                    width: 10
                    height: 10
                    color: "transparent"
                }

                AButton {
                    id: playButton

                    height: parent.height
                    width: 60
                    name: "Play file"
                    mouseArea.onClicked: AudioProvider.playAudioFile()
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
                    name: "Stop file"
                    mouseArea.onClicked: AudioProvider.stopAudioFile()
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

                anchors.centerIn: parent
                spacing: 20

                ASlider {
                    id: alphaSlider

                    width: (settingsRect.width) * (0.7)
                    slider.from: 0
                    slider.to: 1
                    slider.stepSize: 0.1
                    slider.value: 0.1
                    name: "Volume"

                    //                    slider.onValueChanged: liveImageItem.alpha = slider.value
                }

                ACheckBox {
                    id: lowpassCheckbox

                    name: "Low pass"
                    onIsToggledChanged: isToggled ? console.log("Low pass filter set") : console.log("Low pass filter unset")
                }

                ACheckBox {
                    id: highpassCheckbox

                    name: "High pass"
                    onIsToggledChanged: isToggled ? console.log("High pass filter set") : console.log("High pass filter unset")
                }

                ACheckBox {
                    id: bandpassCheckbox

                    name: "Low pass"
                    onIsToggledChanged: isToggled ? console.log("High pass filter set") : console.log("High pass filter unset")
                }

                Rectangle {
                    width: parent.width
                    height: 100
                    radius: 3
                    color: "#819ca9"

                    Column {
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 5

                        Text {
                            id: formatText
                            text: qsTr("<b>Format: </b>") + (root.providerRunning ? root.format : "none")
                        }

                        Text {
                            id: channelsText
                            text: "<b>Channels: </b>" + (root.providerRunning ? root.channels : "none")
                        }

                        Text {
                            id: sampleRateText
                            text: "<b>Sample rate: </b>" + (root.providerRunning ? root.sampleRate : "none")
                        }

                        Text {
                            id: numberofFramesText
                            text: "<b>Frames total: </b>" + (root.providerRunning ? root.frames : "none")
                        }
                    }
                }
            }
        }
    }
}
