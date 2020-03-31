import QtQuick 2.0

Rectangle {
    id: headingRect

    property alias name: headingText.text
    property alias border: headingRect.border
    property alias pixelSize: headingText.font.pixelSize
    property alias weight: headingText.font.weight

    width: headingText.width + 10
    height: 20
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.top: parent.top
    anchors.topMargin: 5
    color: "transparent"

    Text {
        id: headingText

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 5
        font.weight: Font.Medium
        text: qsTr("Nameadfsasdfasdfasdfasdfasdf")
    }
}
