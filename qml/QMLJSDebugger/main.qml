import QtQuick 1.0

Rectangle {
    id: mainWindow
    width: 1200; height: 800
    color: "white"

    Rectangle {
        id: breakpointLineWindow
        width: 20; height: mainWindow.height
        color: "lightgrey"
        Column {
            anchors.right: breakpointLineWindow.right; anchors.rightMargin: 2
            spacing: 2
            Repeater {
                id: lineNumberModel
                Text {
                    font { family: "Helvetica[Neue]"; pixelSize: 10 }
                    color: "darkgrey"
                    text: index
                }
            }
        }
    }

    Rectangle {
        id: infoWindow
        width: mainWindow.width - breakpointLineWindow.width; height: mainWindow.height/4
        anchors.bottom: mainWindow.bottom; anchors.right: mainWindow.right
        color: "black"
        Rectangle {
            id: controlWindow
            width: mainWindow.width - breakpointLineWindow.width; height: 20
            anchors.bottom: infoWindow.top; anchors.right: infoWindow.right
            color: "grey"
        }
    }

    Rectangle {
        id: fileWindow
        width: infoWindow.width; height: mainWindow.height * 3/4
        anchors.top: mainWindow.top; anchors.right: mainWindow.right

        Text {
            id: fileContents
            anchors.fill: parent
            font { family: "Monaco"; pixelSize: 12 }
        }
    }

    Component.onCompleted: file.browse()
    Connections {
            target: file
            onFileLoaded: { lineNumberModel.model = file.lines(); fileContents.text = file.contents() }
        }

}
