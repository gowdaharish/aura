import QtQuick 2.0

Item {
    implicitWidth: 150
    implicitHeight: 150

    property alias name: text
    property alias mouseArea: mouseArea

    Rectangle {
        id: chipRect

        anchors.fill: parent
        color: "#1b262c"
        border.width: 1
        border.color: "#0f4c75"
        radius: 3

        states: [
            State {
                name: "pressed"
                when: mouseArea.pressed

                PropertyChanges {
                    target: chipRect
                    color: "#29434e"
                }
            },
            State {
                name: "hovered"
                when: mouseArea.containsMouse

                PropertyChanges {
                    target: chipRect
                    color: "#546e7a"
                }

                PropertyChanges {
                    target: mouseArea
                    cursorShape: Qt.PointingHandCursor
                }

                PropertyChanges {
                    target: text
                    color: "black"
                }
            }
        ]

        Text {
            id: text

            anchors.centerIn: parent
            color: "white"
        }

        MouseArea {
            id: mouseArea

            anchors.fill: chipRect
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton
            cursorShape: Qt.PointingHandCursor
        }
    }
}
