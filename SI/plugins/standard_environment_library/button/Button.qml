import QtQuick 2.7

Item
{
    id: container
    visible: true

   Item {
       id: iconcontainer
       visible: true

       Image {
           id: image
           anchors.left: parent.left
           anchors.top: parent.top

           visible: true
       }
    }

    Connections {
        target: Region
        onDataChanged: {
            image.width = data.icon_width;
            image.height = data.icon_height;
            image.source = data.img_path;
        }
    }
}