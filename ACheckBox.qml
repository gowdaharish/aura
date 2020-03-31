import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: checkBox

    implicitHeight: 40
    implicitWidth: parent.width / 2
    color: "#819ca9"
    radius: 10

    property bool isToggled: false
    property alias name: name.text


    Rectangle {
        id: iconRect

        height: 20
        width: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        radius: 3

        Image {
            anchors.fill: parent
            source: checkBox.isToggled ? "icons/checkmark_16px.svg" : ""
        }
    }

    Text
    {
        id: name

        anchors.left: iconRect.right
        anchors.leftMargin: 10
        anchors.verticalCenter: iconRect.verticalCenter
        color: "black"
        text: "Checkbox text"
        font.weight: Font.Medium
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onClicked: {
            checkBox.isToggled = !checkBox.isToggled
        }
    }
}
