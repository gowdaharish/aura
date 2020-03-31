import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    id: root

    property alias slider: slider
    property alias name: label.name

    implicitHeight: 60
    implicitWidth: parent.width
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.topMargin: 10
    anchors.bottomMargin: 10
    color: "#819ca9"
    radius: 10

    ComponentLabel {
        id: label
    }

    Slider {
        id: slider

        anchors.left: parent.left
        anchors.right: valueText.left
        anchors.top: label.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 5
    }

    Text {
        id: valueText

        width: 20
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        anchors.verticalCenter: parent.verticalCenter
        font.weight: Font.Medium
        text: Math.round(slider.value * 100) / 100
    }
}
