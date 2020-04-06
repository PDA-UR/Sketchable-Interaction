import QtQuick 2.7

Item
{
    id: container

    width:100
    height:100

    Image {
        id: cursorImage
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Connections {
        target: Region
        onDataChanged: {
            cursorImage.width = data.width;
            cursorImage.height = data.height;
//            cursorImage.source = data.img_path;
        }
    }
}