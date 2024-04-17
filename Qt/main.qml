import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4

Window {
    id:root
    width: 400
    height: 1280
    visible: true
    title: "Instrument Cluster"


    Item {
        id: container
        width: 1280
        height: 400
        anchors.centerIn: parent
        rotation: -90

        //background of application
        Rectangle {
            anchors.fill: parent
            width: parent.width
            height: parent.height
            color: "black"
        }

        // Speedometer
        CircularGauge {
            id:speedometer
            style: CircularGaugeStyle {
                background: Canvas {
                    id: speedometerCanvas
                    width: speedometer.implicitWidth
                    height: speedometer.implicitHeight
                    onPaint: {
                        var ctx = getContext("2d");
                        var gradient = ctx.createRadialGradient(speedometerCanvas.width / 2, speedometerCanvas.height / 2, 0, speedometerCanvas.width / 2, speedometerCanvas.height / 2, speedometerCanvas.width / 2);
                        gradient.addColorStop(0, "#000");
                        gradient.addColorStop(0.82, "#000");
                        gradient.addColorStop(1, socketClient.themeColor);

                        ctx.fillStyle = gradient;
                        ctx.arc(speedometerCanvas.width / 2, speedometerCanvas.height / 2, speedometerCanvas.width / 2, 0, 2 * Math.PI);
                        ctx.fill();
                    }
                    Connections {
                        target: socketClient
                        onThemeColorReceived: {
                            // Trigger repaint of the canvas whenever themeColor changes
                            speedometerCanvas.requestPaint();
                        }
                    }
                }
                foreground: Item {
                    clip: true
                    Rectangle {
                        width: 360 * 0.5
                        height: width
                        radius: width * 0.5
                        color: "black"
                        anchors.centerIn: parent
                        border.color: "#fff"
                        border.width: 3
                        anchors.bottomMargin: -border.width
                        Canvas {
                            id: innerSpeedometerCanvas
                            width: parent.width
                            height: parent.height
                            onPaint: {
                                var ctx = getContext("2d");
                                var gradient = ctx.createRadialGradient(innerSpeedometerCanvas.width / 2, innerSpeedometerCanvas.height / 2, 0, innerSpeedometerCanvas.width / 2, innerSpeedometerCanvas.height / 2, innerSpeedometerCanvas.width / 2);

                                gradient.addColorStop(0, "#000");
                                gradient.addColorStop(0.82, "#000");
                                gradient.addColorStop(1, socketClient.themeColor);

                                ctx.fillStyle = gradient;
                                ctx.arc(innerSpeedometerCanvas.width / 2, innerSpeedometerCanvas.height / 2, innerSpeedometerCanvas.width / 2, 0, 2 * Math.PI);
                                ctx.fill();
                            }
                            Connections {
                                target: socketClient
                                onThemeColorReceived: {
                                    // Trigger repaint of the canvas whenever themeColor changes
                                    innerSpeedometerCanvas.requestPaint();
                                }
                            }
                        }
                        Text {
                            id: speed
                            text: parseInt(Instrument_Cluster.rps * 3.14 * 2.5)
                            color: "#fff"
                            font.pointSize: 40
                            anchors.centerIn: parent
                        }
                        Text {
                            text: "cm/s"
                            color: "#fff"
                            font.pointSize: 14
                            anchors.top: speed.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
            value: Instrument_Cluster.rps * 3.14 * 2.5
            stepSize: 0.5
            maximumValue: 100
            minimumValue: 0
            width: 370
            height: 370
            anchors.left: bgImageContainer.right
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 15
        }

        Image {
            id: leftIndicator
            anchors {
                right: bgImageContainer.left
                rightMargin: -40
                top: parent.top
                topMargin: 15
            }
            source: "qrc:/assets/left.svg"
            width: 40
            height: 40
            fillMode: Image.PreserveAspectFit
            opacity: 0.5
        }

        Rectangle {
            id: bgImageContainer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: speedometer.top
            anchors.topMargin: 25
            width: 400
            height: 250
            color: "transparent"
            Image {
                id: bgImage
                anchors.centerIn: parent
                source: "qrc:/assets/bg-mask.png"
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                rotation: 0
            }
            Image {
                id: bgImageHighlights
                anchors.centerIn: parent
                source: "qrc:/assets/car-highlights.png"
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                rotation: 0
            }
        }

        Image {
            id: rightIndicator
            anchors {
                left: bgImageContainer.right
                leftMargin: -40
                top: parent.top
                topMargin: 15
            }
            source: "qrc:/assets/right.svg"
            width: 40
            height: 40
            fillMode: Image.PreserveAspectFit
            rotation: 180
            opacity: 0.5
        }

        Timer {
            interval: 500
            running: true
            repeat: true
            onTriggered: {
                if(Instrument_Cluster.gear !== "P" && Instrument_Cluster.indicator === "L") {
                    leftIndicator.visible = !leftIndicator.visible;
                    leftIndicator.opacity = 1;
                    rightIndicator.opacity = 0.5;
                    bgImage.rotation = bgImage.rotation - 3;
                    bgImageHighlights.rotation = bgImageHighlights.rotation - 3;
                }
                else if(Instrument_Cluster.gear !== "P" && Instrument_Cluster.indicator === "R") {
                    rightIndicator.visible = !rightIndicator.visible;
                    rightIndicator.opacity = 1;
                    leftIndicator.opacity = 0.5;
                    bgImage.rotation = bgImage.rotation + 3;
                    bgImageHighlights.rotation = bgImageHighlights.rotation + 3;
                }
                else{
                    leftIndicator.visible = true;
                    leftIndicator.opacity = 0.5;
                    rightIndicator.visible = true;
                    rightIndicator.opacity = 0.5;
                    bgImage.rotation = 0;
                    bgImageHighlights.rotation = 0;
                }
            }
        }

        // Battery
        Rectangle{
            id: battery
            anchors {
                bottom: parent.bottom
                bottomMargin: 50
                horizontalCenter: parent.horizontalCenter
            }
            color: "transparent"
            Rectangle {
                id: batteryfill
                anchors {
                    top: batteryImage.top
                    topMargin: 8
                    left: batteryImage.left
                    leftMargin: 10
                }
                width: parseInt(Instrument_Cluster.battery) * 1.2
                height: 58
                color: "green"
            }
            Image {
                width: 140
                height: 70
                anchors.centerIn: parent
                id: batteryImage
                source: "qrc:/assets/battery.png"
            }
            Text {
                anchors.centerIn: batteryImage
                font.pixelSize: 20
                color: "white"
                text: Instrument_Cluster.battery ? parseInt(Instrument_Cluster.battery) + "%" : "0%"
            }
        }

        // RPS Guage
        CircularGauge {
            id:rpsguage
            property real rpsValue: Instrument_Cluster.rps
            NumberAnimation {
                id: rpsAnimation
                target: rpsguage
                property: "value"
                duration: 100
                easing.type: Easing.Linear
            }
            onRpsValueChanged: {
                rpsAnimation.from = rpsguage.value
                rpsAnimation.to = rpsValue
                rpsAnimation.start()
            }
            style: CircularGaugeStyle {
                background: Canvas {
                    id: rpsCanvas
                    width: rpsguage.implicitWidth
                    height: rpsguage.implicitHeight
                    onPaint: {
                        var ctx = getContext("2d");
                        var gradient = ctx.createRadialGradient(rpsCanvas.width / 2, rpsCanvas.height / 2, 0, rpsCanvas.width / 2, rpsCanvas.height / 2, rpsCanvas.width / 2);

                        gradient.addColorStop(0, "#000");
                        gradient.addColorStop(0.82, "#000");
                        gradient.addColorStop(1, socketClient.themeColor);

                        ctx.fillStyle = gradient;
                        ctx.arc(rpsCanvas.width / 2, rpsCanvas.height / 2, rpsCanvas.width / 2, 0, 2 * Math.PI);
                        ctx.fill();
                    }
                    Connections {
                        target: socketClient
                        onThemeColorReceived: {
                            // Trigger repaint of the canvas whenever themeColor changes
                            rpsCanvas.requestPaint();
                        }
                    }
                }
                foreground: Item {
                    Rectangle {
                        width: 360 * 0.5
                        height: width
                        radius: width * 0.5
                        color: "black"
                        anchors.centerIn: parent
                        border.color: "#fff"
                        border.width: 3
                        anchors.bottomMargin: -border.width
                        Canvas {
                            id: innerRPSCanvas
                            width: parent.width
                            height: parent.height
                            onPaint: {
                                var ctx = getContext("2d");
                                var gradient = ctx.createRadialGradient(innerRPSCanvas.width / 2, innerRPSCanvas.height / 2, 0, innerRPSCanvas.width / 2, innerRPSCanvas.height / 2, innerRPSCanvas.width / 2);

                                gradient.addColorStop(0, "#000");
                                gradient.addColorStop(0.82, "#000");
                                gradient.addColorStop(1, socketClient.themeColor);

                                ctx.fillStyle = gradient;
                                ctx.arc(innerRPSCanvas.width / 2, innerRPSCanvas.height / 2, innerRPSCanvas.width / 2, 0, 2 * Math.PI);
                                ctx.fill();
                            }
                            Connections {
                                target: socketClient
                                onThemeColorReceived: {
                                    // Trigger repaint of the canvas whenever themeColor changes
                                    innerRPSCanvas.requestPaint();
                                }
                            }
                        }
                        Text {
                            id: rps
                            text: Instrument_Cluster.gear // need to change
                            color: "#fff"
                            font.pointSize: 50
                            anchors.centerIn: parent
                        }
                    }
                }
                labelStepSize: 1
            }
            // value: Instrument_Cluster.rps
            maximumValue: 10
            stepSize: 0.5
            width: 370
            height: 370
            anchors.right: bgImageContainer.left
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 15
        }
    }
    Component.onCompleted: {
        socketClient.connectToServer("192.168.1.145", 8000); // Replace with your server address and port
    }
}
