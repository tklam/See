/*

Copyright (C) 2015 LAM Tak-Kei

This file is part of See.

See is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

See is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
See. If not, see <http://www.gnu.org/licenses/>.

*/

import QtQuick 2.3
import QtQuick.Window 2.2
import QtMultimedia 5.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.2
import HKer 1.0

Window {
    id: root
    visible: true
    width: Screen.width
    signal detectColourClicked
    signal calibrateWhiteClicked

    //----------------------------- content holders
    PresentationPropertyHolder {
        id: longitudeHolder
        objectName: "longitudeHolder"
    }
    PresentationPropertyHolder {
        id: latitudeHolder
        objectName: "latitudeHolder"
    }
    PresentationPropertyHolder {
        id: colourTextHolder
        objectName: "colourTextHolder"
    }
    PresentationPropertyHolder {
        id: colourValueHolder
        objectName: "colourValueHolder"
    }

    //----------------------------- signal connections
    Connections {
        target: longitudeHolder
        onContentChanged: {
            longitudeLabel.text = longitudeHolder.content
        }
    }

    Connections {
        target: latitudeHolder
        onContentChanged: {
            latitudeLabel.text = latitudeHolder.content
        }
    }

    Connections {
        target: colourTextHolder
        onContentChanged: {
            colourDetectionResult.text = colourTextHolder.content
        }
    }

    Connections {
        target: colourValueHolder
        onContentChanged: {
            colourMatchedRect.color = colourValueHolder.content
        }
    }

    //state management
    Item {
        id: viewState
        states: [
            State {
                name: "MAIN"
                PropertyChanges {
                    target: mainView
                    visible: true
                }
                PropertyChanges {
                    target: scanner
                    visible: false
                }
                StateChangeScript {
                    script: {
                        stackView.push(mainView)
                    }
                }
            },
            State {
                name: "SCANNER"
                PropertyChanges {
                    target: mainView
                    visible: false
                }
                PropertyChanges {
                    target: scanner
                    visible: true
                }
                StateChangeScript {
                    script: {
                        stackView.push(scanner)
                    }
                }
            }
        ]
        onStateChanged: {
            //this happens before StateChangeScript
            stackView.pop()
        }
        state: "MAIN"
    }

    //--------------------------------------- main view
    Item {
        id: mainView
        width: parent.width
        height: parent.height

        Column {
            anchors.centerIn: parent
            width: parent.width

            Row {
                width: parent.width

                Label {
                    width: Math.round(parent.width / 3)
                    id: locationLabel
                    text: qsTr("Location: ")
                }

                Label {
                    width: Math.round(parent.width / 3)
                    id: longitudeLabel
                    text: "0.0"
                }

                Label {
                    width: Math.round(parent.width / 3)
                    id: latitudeLabel
                    text: "0.0"
                }
            }

            Column {
                width: parent.width
                spacing: 50

                Row {
                    width: parent.width

                    Button {
                        anchors.margins: 20
                        width: Math.round(parent.width * 0.3)
                        text: qsTr("Detect Colour")
                        onClicked: {
                            viewState.state = "SCANNER"
                        }
                    }
                }
            } //end of column
        }
    }

    //---------------------------------------  scanner
    Item {
        id: scanner
        width: parent.width
        height: parent.height

        property var videoOutHorizontalCentre
        property var videoOutVerticalCentre

        Camera {
            id: camera
            objectName: "camera"
        }

        Column {
            anchors.fill: parent
            spacing: 2

            VideoOutput {
                id: videoOut
                width: parent.width
                //height: parent.height
                source: camera
                orientation: 270
                focus: visible

                Rectangle {
                    z: 1
                    border {
                        width: 1
                        color: "#ffffff"
                    }
                    color: "transparent"
                    x: (parent.width - parent.width / 21) / 2
                    y: (parent.height - parent.height / 21) / 2
                    width: parent.width / 21
                    height: parent.height / 21 //hardcoded XXX
                }

                Component.onCompleted: {
                    scanner.videoOutHorizontalCentre = x
                    scanner.videoOutVerticalCentre = y
                }
            }

            Row {
                width: parent.width
                Rectangle {
                    id: colourMatchedRect
                    width: parent.width / 10
                    height: parent.width / 10
                    color: "#000000"
                    border.width: 0
                }
            }

            Row {
                width: parent.width

                Button {
                    anchors.margins: 20
                    text: qsTr("Cancel")
                    onClicked: {
                        viewState.state = "MAIN"
                    }
                }

                Button {
                    anchors.margins: 20
                    width: parent.width / 2
                    text: qsTr("Detect Colour")
                    onClicked: {
                        detectColourClicked()
                    }
                }

                Button {
                    anchors.margins: 20
                    text: qsTr("Calibrate")
                    onClicked: {
                        calibrateWhiteClicked()
                    }
                }
            }

            Row {
                width: parent.width

                Label {
                    width: Math.round(parent.width / 3)
                    id: colourLabel
                    text: qsTr("RGB: ")
                }

                Label {
                    id: colourDetectionResult
                    text: ""
                }
            }
        }

        //horizontal centre line
        Item {
            z: 1
            x: scanner.videoOutHorizontalCentre
            y: scanner.videoOutVerticalCentre
            width: Screen.width
            height: videoOut.height
            Rectangle {
                anchors.centerIn: parent
                width: Screen.width
                height: 1
                color: "#FF9999"
                border.color: "#FF0000"
                border.width: 1
                opacity: .5
                ColorAnimation on color {
                    from: "#FF9999"
                    to: "#FFFFFF"
                    duration: 1000
                    loops: Animation.Infinite
                }
            }
        }
    }

    //--------------------------------------- stack view
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainView
    }
}
