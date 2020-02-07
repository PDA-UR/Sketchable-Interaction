import QtQuick 2.7

Item
{
    id: container

    width:100
    height:100

    property var tform: Qt.matrix4x4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1)

    transform: Matrix4x4 { matrix: tform }

    Image {
        id: cursorImage
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Connections {
        target: Region
        onTransformChanged: {
            container.tform = tform
        }

        onDataChanged: {
            cursorImage.width = data.width;
            cursorImage.height = data.height;
            cursorImage.source = data.img_path;
        }
    }
}