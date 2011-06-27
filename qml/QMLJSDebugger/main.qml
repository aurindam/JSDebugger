import QtQuick 1.0
import "helper.js" as Helper

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
                    text: index + 1
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
            width: mainWindow.width - breakpointLineWindow.width; height: 28
            anchors.top: infoWindow.top; anchors.right: infoWindow.right
            color: "grey"

            Row {
                anchors.verticalCenter: controlWindow.verticalCenter
                anchors.left: controlWindow.left; anchors.right: controlWindow.right
                spacing: 2

                Rectangle{
                    width: 28; height: 28
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        width: 24; height: 24
                        source: "images/debug.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { console.log ("Start Debug");
//                            debugService.setScriptPath(fileHandler.filepath());
//                            debugService.runScript();
//                            debugService.executeCommand(Helper.setBreakpointInMain());
//                            debugService.runMain();
debugClient.startDebugService(fileHandler.filepath()); /*debugClient.sendCommand(Helper.setBreakpointInMain());*/}
                    }
                }

                Rectangle{
                    width: 28; height: 28
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        width: 24; height: 24
                        source: "images/play.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: console.log ("Start Continue")
                    }
                }

                Rectangle{
                    width: 28; height: 28
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        width: 24; height: 24
                        source: "images/step_over.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: console.log ("Start Step over")
                    }
                }

                Rectangle{
                    width: 28; height: 28
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        width: 24; height: 24
                        source: "images/step_into.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: console.log ("Start Step Into")
                    }
                }

                Rectangle{
                    width: 28; height: 28
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        width: 24; height: 24
                        source: "images/step_out.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: console.log ("Start Step Out")
                    }
                }

            }
        }//controlWindow

        Rectangle {
            id: stackWindow
            width: infoWindow.width/2; height: infoWindow.height - controlWindow.height
            anchors.bottom: infoWindow.bottom; anchors.left: infoWindow.left
            color: "lightblue"
        }//StackWindow

        Rectangle {
            id: localsWindow
            width: infoWindow.width/2; height: infoWindow.height - controlWindow.height
            anchors.bottom: infoWindow.bottom; anchors.right: infoWindow.right
            color: "lightgreen"
        }//localsWindow

    }//infoWindow

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

    Component.onCompleted: fileHandler.openFile()
    Connections {
            target: fileHandler
            onFileLoaded: { lineNumberModel.model = fileHandler.lines; fileContents.text = fileHandler.contents }
        }

}
