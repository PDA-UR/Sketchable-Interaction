import QtQuick 2.7

Item
{
    function updateData(data)
    {
        if(data !== undefined)
        {
            container.width = data.containerwidth;
            container.height = data.containerheight;

            if(data.message !== undefined)
                notification.text = data.message;
        }
    }

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
            text: "Hello World"
        }
    }
}