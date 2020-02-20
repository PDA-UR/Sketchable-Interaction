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
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Text {
        id: directoryname
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
        text: ""
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

            directoryname.color = data.color;
            directoryname.text = data.name;
        }
    }
}