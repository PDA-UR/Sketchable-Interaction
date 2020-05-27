import QtQuick 2.7

Item
{
    function updateData(data)
    {
        cursorImage.width = data.width;
        cursorImage.height = data.height;
    }

    id: container

    width:100
    height:100

    Image {
        id: cursorImage
        anchors.left: parent.left
        anchors.top: parent.top
    }
}