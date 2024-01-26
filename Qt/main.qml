import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4
import QtQuick.Extras.Private 1.0

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
            width: parent.width
            height: parent.height

            gradient: Gradient {
                GradientStop { position: 0.04; color: "#000080" } // Dark Blue
                GradientStop { position: 1.0; color: "#000000" } // Black
            }
        }

        // Speedometer
        CircularGauge {
            id:speedometer
            style: CircularGaugeStyle {
                background: Rectangle {
                    implicitHeight: speedometer.height
                    implicitWidth: speedometer.width
                    color: "black"
                    radius: 360
                    Image {
                        anchors.fill: parent
                        source: "qrc:/assets/background.svg"
                        asynchronous: true
                        sourceSize {
                            width: width
                        }
                    }
                }
            }
            value: Instrument_Cluster.rps * 3.14 * 2.5
            stepSize: 0.5
            maximumValue: 100
            minimumValue: 0
            width: 410
            height: 410
            anchors.left: battery.right
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 15
        }


        // Battery
        Rectangle{
            id: battery
            rotation: -90
            anchors.centerIn: parent
            width:280
            height:130
            color: "transparent"
            Rectangle {
                id: batteryfill
                anchors.bottom: parent.bottom
                width: Instrument_Cluster.battery * 2.6
                height: 120
                color: "green"
            }
            Image {
                width:320
                height:160
                anchors.centerIn: parent
                id: batteryImage
                source: "qrc:/assets/battery2.png"
            }
            Text {
                anchors.centerIn: batteryfill
                font.pixelSize: 24
                color: "white"
                text: Instrument_Cluster.battery + "%"
                rotation: -270
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
                background: Rectangle {
                    implicitHeight: speedometer.height
                    implicitWidth: speedometer.width
                    color: "black"
                    radius: 360
                    Image {
                        anchors.fill: parent
                        source: "qrc:/assets/background.svg"
                        asynchronous: true
                        sourceSize {
                            width: width
                        }
                    }
                }
                labelStepSize: 1
            }
            // value: Instrument_Cluster.rps
            maximumValue: 10
            stepSize: 0.5
            width: 410
            height: 410
            anchors.right: battery.left
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 15
        }
    }
}
