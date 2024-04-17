#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>

class DataReceiver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float rps READ rps NOTIFY rpsChanged FINAL)
    Q_PROPERTY(double battery READ battery NOTIFY batteryChanged FINAL)
    Q_PROPERTY(QString gear READ gear NOTIFY gearChanged FINAL)
    Q_PROPERTY(QString indicator READ indicator NOTIFY indicatorChanged FINAL)
public:
    explicit DataReceiver(QObject *parent = nullptr);
    Q_INVOKABLE float receiveRPSData();
    Q_INVOKABLE double receiveBatteryData();
    Q_INVOKABLE QString receiveGearInformation();
    Q_INVOKABLE QString receiveIndicatorInformation();

public slots:
    float rps();
    double battery();
    QString gear();
    QString indicator();

signals:
    void rpsChanged();
    void batteryChanged();
    void gearChanged();
    void indicatorChanged();

private:
    float m_rps;
    double m_battery;
    QString m_gear;
    QString m_indicator;
};

#endif // DATARECEIVER_H
