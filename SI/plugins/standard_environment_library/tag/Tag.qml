import QtQuick 2.7
import QtQuick.Controls 2.7

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
        id: image
        width: container.width
        height: container.height
        anchors.left: parent.left
        anchors.top: parent.top
        visible: true
    }

    Connections {
        target: Region
        onTransformChanged: {
            container.tform = tform
        }

        onDataChanged: {
            image.anchors.leftMargin = data.margin_left - image.width / 2;
            image.anchors.topMargin = data.margin_top - image.height / 2;

            image.source = data.img_path;
        }
    }
}