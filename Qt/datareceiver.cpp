#include "datareceiver.h"
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusReply>
#include <QTimer>

DataReceiver::DataReceiver(QObject *parent)
    : QObject{parent}
{
    QTimer *timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &DataReceiver::receiveRPSData);
    timer1->start(100);

    QTimer *timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &DataReceiver::receiveBatteryData);
    timer2->start(10000);
}

// Receiver function
float DataReceiver::receiveRPSData() {
    qDebug() << "Trying to connect D-Bus to receive RPS data...";
    QDBusInterface dbusInterface("com.example.dBus.rps", "/com/example/dBus/rps", "com.example.dBus.rps", QDBusConnection::sessionBus());

    // Show error if connection is failed
    if(!dbusInterface.isValid()) {
        qDebug() << "Failed to create DBusInterface to receive RPS data";
    }

    QDBusReply<QString> rps = dbusInterface.call("RPS");

    if(!rps.isValid()) {
        qWarning() << "Failed to call method for rps:" << rps.error().message();
        qDebug() << "Error printing rps data";
    } else {
        m_rps = rps.value().toFloat();
        qDebug() << "RPS: " << rps.value().toFloat();
    }

    emit rpsChanged();
    return rps.value().toFloat();
}

double DataReceiver::receiveBatteryData() {
    qDebug() << "Trying to connect D-Bus to receive Battery data...";
    QDBusInterface dbusInterface("com.example.dBus.battery", "/com/example/dbus/battery", "com.example.dBus.battery", QDBusConnection::sessionBus());

    // Show error if connection is failed
    if(!dbusInterface.isValid()) {
        qDebug() << "Failed to create DBusInterface to receive Battery data";
    }

    QDBusReply<QString> battery = dbusInterface.call("getBatteryLevel");

    if(!battery.isValid()) {
        qWarning() << "Failed to call method for battery:" << battery.error().message();
        qDebug() << "Error printing battery data";
    } else {
        m_battery = battery.value().toDouble();
        qDebug() << "Battery: " << battery.value();
    }

    emit batteryChanged();
    return battery.value().toDouble();
}

float DataReceiver::rps()
{
    return m_rps;
}

double DataReceiver::battery()
{
    return m_battery;
}
