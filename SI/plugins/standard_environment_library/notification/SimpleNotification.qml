import QtQuick 2.7

Item
{
    id: container
    visible: true

    Rectangle {
            id: rect
            visible: true
            color: "white"

            anchors.fill: parent

            Text {
                id: notification
                visible: true

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                fontSizeMode: Text.Fit
                minimumPixelSize: 16
                font.pixelSize: 48
                color: "black"
                text: "HELLO WORLD"
            }
        }

    Connections {
        target: Region
        onDataChanged: {
            container.width = data.rect_width;
            container.height = data.rect_height;

            notification.text = data.message;
        }
    }
}