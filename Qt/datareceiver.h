#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>

class DataReceiver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float rps READ rps NOTIFY rpsChanged FINAL)
    Q_PROPERTY(double battery READ battery NOTIFY batteryChanged FINAL)
public:
    explicit DataReceiver(QObject *parent = nullptr);
    Q_INVOKABLE float receiveRPSData();
    Q_INVOKABLE double receiveBatteryData();

public slots:
    float rps();
    double battery();

signals:
    void rpsChanged();
    void batteryChanged();

private:
    float m_rps;
    double m_battery;
};

#endif // DATARECEIVER_H
