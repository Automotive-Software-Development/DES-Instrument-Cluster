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

    QTimer *timer3 = new QTimer(this);
    connect(timer3, &QTimer::timeout, this, &DataReceiver::receiveGearInformation);
    timer3->start(10);

    QTimer *timer4 = new QTimer(this);
    connect(timer4, &QTimer::timeout, this, &DataReceiver::receiveIndicatorInformation);
    timer4->start(10);
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

QString DataReceiver::receiveGearInformation()
{
    qDebug() << "Trying to connect D-Bus to receive gear information...";
    QDBusInterface dbusInterface("com.example.dbus.gear", "/com/example/dbus/gear", "com.example.dbus.gear", QDBusConnection::sessionBus());

    // Show error if connection is failed
    if(!dbusInterface.isValid()) {
        qDebug() << "Failed to create DBusInterface to receive gear information...";
    }

    QDBusReply<QString> gear = dbusInterface.call("Get_Gear_Information");

    if(!gear.isValid()) {
        qWarning() << "Failed to call method for Gear Information:" << gear.error().message();
        qDebug() << "Error printing gear information";
    } else {
        m_gear = gear.value();
        qDebug() << "Gear: " << gear.value();
    }

    emit gearChanged();
    return gear.value();
}

QString DataReceiver::receiveIndicatorInformation()
{
    qDebug() << "Trying to connect D-Bus to receive indicator information...";
    QDBusInterface dbusInterface("com.example.dbus.gear", "/com/example/dbus/gear", "com.example.dbus.gear", QDBusConnection::sessionBus());

    // Show error if connection is failed
    if(!dbusInterface.isValid()) {
        qDebug() << "Failed to create DBusInterface to receive indicator information...";
    }

    QDBusReply<QString> indicator = dbusInterface.call("Get_Indicator_Information");

    if(!indicator.isValid()) {
        qWarning() << "Failed to call method for Indicator Information:" << indicator.error().message();
        qDebug() << "Error printing indicator information";
    } else {
        m_indicator = indicator.value();
        qDebug() << "Indicator: " << indicator.value();
    }

    emit indicatorChanged();
    return indicator.value();
}

float DataReceiver::rps()
{
    return m_rps;
}

double DataReceiver::battery()
{
    return m_battery;
}

QString DataReceiver::gear()
{
    return m_gear;
}

QString DataReceiver::indicator()
{
    return m_indicator;
}
