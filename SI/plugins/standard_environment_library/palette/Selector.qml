import QtQuick 2.7

Item
{
    id: container
    visible: true

    Image {
        id: image
        anchors.left: parent.left
        anchors.top: parent.top

        visible: true
    }

    Connections {
        target: Region
        onDataChanged: {
            image.width = data.img_width;
            image.height = data.img_height;
//            image.source = data.img_path;

            image.anchors.leftMargin = data.widget_width / 2 - image.width / 2;
            image.anchors.topMargin = data.widget_height / 2 - image.height / 2;
        }
    }
}