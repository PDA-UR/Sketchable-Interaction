import QtQuick 2.7
import QtQuick.Controls 2.7

Item
{
    function updateData(data)
    {
        image.width = data.icon_width;
        image.height = data.icon_height;
        image.anchors.leftMargin = image.width / 2;

        image.source = data.img_path;

        filename.color = data.color;
        filename.text = data.name;
    }

    id: container

    visible: true

    Image {
        id: image
        anchors.left: parent.left
        anchors.top: parent.top

        visible: true
    }

    Text {
        id: filename
        visible: true

        fontSizeMode: Text.Fit
        minimumPixelSize: 16
        font.pixelSize: 72
        color: "white"

        width: image.width * 2
        anchors.top: image.bottom
        anchors.left: image.left
        anchors.topMargin: -10
        anchors.leftMargin: -width * 0.25

        wrapMode: TextEdit.Wrap
    }
}