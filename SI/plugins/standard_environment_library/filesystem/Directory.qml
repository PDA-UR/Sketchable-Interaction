import QtQuick 2.7
import QtQuick.Controls 2.7

Item
{
    id: container

    visible: true

    Item
    {
        id: iconcontainer
        visible: false

        Image {
            id: image
            anchors.left: parent.left
            anchors.top: parent.top

            visible: true
        }

        Text {
            id: icondirectoryname
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

    Item
        {
            id: openedcontainer
            visible: true

            Text {
                id: openenddirectoryname
                visible: true

                width: 400
                fontSizeMode: Text.Fit
                minimumPixelSize: 20
                font.pixelSize: 72
                color: "white"
                anchors.top: parent.top
                anchors.left: parent.left

                wrapMode: TextEdit.Wrap
            }

            Text {
                id: opendirectorypagecount
                visible: true

                fontSizeMode: Text.Fit
                minimumPixelSize: 16
                font.pixelSize: 72
                color: "white"
                width: 150
                anchors.top: openenddirectoryname.top
                anchors.left: openenddirectoryname.left

                anchors.topMargin: 50
                anchors.leftMargin: width - width * 0.25

                wrapMode: TextEdit.Wrap
                text: "0000 / 0000"
            }
        }

    Connections {
        target: Region

        onDataChanged: {
            container.visible = data.is_visible;

            image.width = data.icon_width;
            image.height = data.icon_height;
            image.anchors.leftMargin = image.width / 2;

            image.source = data.img_path;
            image.visible = data.is_icon_visible;

            icondirectoryname.color = data.color;
            icondirectoryname.text = data.name;
            openenddirectoryname.text = data.name;
            opendirectorypagecount.text = data.page_name;
            openenddirectoryname.color = data.color;
            opendirectorypagecount.color = data.color;
            icondirectoryname.visible = data.is_text_visible;
        }
    }
}