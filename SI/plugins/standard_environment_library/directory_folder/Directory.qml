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
        id: image
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Connections {
            target: Region
            onTransformChanged: {
                container.tform = tform
            }

            onDataChanged: {
                image.width = data.width;
                image.height = data.height;
                image.source = data.img_path;
            }
        }