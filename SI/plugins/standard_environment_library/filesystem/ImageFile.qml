import QtQuick 2.7
import QtQuick.Controls 2.7

Item
{
    function updateData(data)
    {
        container.visible = data.is_visible;

        image.width = data.icon_width;
        image.height = data.icon_height;

        if(data.is_in_preview)
            image.anchors.leftMargin = 0;
        else
            image.anchors.leftMargin = image.width / 2;

        image.source = data.img_path;

        container.width = data.container_width
        container.height = data.container_height

        filename.color = data.color;
        filename.text = data.name;
        filename.visible = !data.is_in_preview;
    }

    id: container

    visible: true

    Image {
        id: image
        anchors.left: parent.left
        anchors.top: parent.top
        fillMode: Image.PreserveAspectFit

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