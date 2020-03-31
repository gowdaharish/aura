import QtQuick 2.0
import QtQuick.Window 2.12

Window {
    visible: false
    width: 200
    height: 200
    title: qsTr("temp window")

    Text {
        id: name
        anchors.centerIn: parent
        text: qsTr("add something here later")
    }
}
