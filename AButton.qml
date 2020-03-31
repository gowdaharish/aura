import QtQuick 2.0

Rectangle {
    id: buttonRect

    property alias name: buttonText.text
    property alias mouseArea: buttonMouseArea

    implicitHeight: 30
    implicitWidth: 100
    border.width: 1
    border.color: "black"
    color: "#78909c"
    radius: 4

    states: [
        State {
            name: "pressed"
            when: buttonMouseArea.pressed

            PropertyChanges {
                target: buttonRect
                color: "#29434e"
            }
        },
        State {
            name: "hovered"
            when: buttonMouseArea.containsMouse

            PropertyChanges {
                target: buttonRect
                color: "#546e7a"
            }

            PropertyChanges {
                target: buttonMouseArea
                cursorShape: Qt.PointingHandCursor
            }
        }
    ]

    Text {
        id: buttonText

        anchors.centerIn: parent
        text: qsTr("Button")
    }

    MouseArea {
        id: buttonMouseArea

        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        cursorShape: Qt.PointingHandCursor
    }
}
