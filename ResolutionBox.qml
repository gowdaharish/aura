import QtQuick 2.0

Item {
    id: root

    width: parent.width/2
    height: 100

    property alias resWidth: widhtRectText.text
    property alias resHeight: heightRectText.text
    property alias applyButton: resolutionApplyButton

    Rectangle {
        id: mainRect

        anchors.fill: parent
        color: "#819ca9"
        radius: 6

        Rectangle {
            id: headingRect

            width: text.width + 10
            height: 20
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            color: "transparent"

            Text {
                id: text

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 5
                text: qsTr("Resolution")
            }
        }

        Rectangle {
            id: widthRect

            width: parent.width/2 - 20
            height: 20
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            border.width: 1
            border.color: "black"
            color: "#b0bec5"
            radius: 4

            TextEdit {
                id: widhtRectText

                anchors.centerIn: parent
                text: "640"
                font.family: "Helvetica"
                selectByMouse: true
                focus: true
                // should limit to four characters
            }
        }

        Rectangle {
            id: heightRect

            width: parent.width/2 - 20
            height: 20
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            border.width: 1
            border.color: "black"
            color: "#b0bec5"
            radius: 4

            TextEdit {
                id: heightRectText

                anchors.centerIn: parent
                text: "480"
                font.family: "Helvetica"
                selectByMouse: true
                focus: true
                // should limit to four characters
            }
        }

        AButton {
            id: resolutionApplyButton

            name: "Apply"
            width: parent.width/2 -10
            height: 20
            anchors.top: heightRect.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
