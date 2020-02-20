import QtQuick 2.7
import QtQuick.Controls 2.7

Item
{
    id: container

    visible: true

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
        visible: true
    }

    Text {
        id: directoryname
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

    Connections {
        target: Region
        onTransformChanged: {
            container.tform = tform
        }

        onDataChanged: {
            container.visible = data.is_visible;

            image.width = data.width;
            image.height = data.height;
            image.source = data.img_path;
            image.visible = data.is_icon_visible;

            directoryname.color = data.color;
            directoryname.text = data.name;
            directoryname.visible = data.is_text_visible;
        }
    }
}