#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>

class SocketClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString themeColor READ themeColor NOTIFY themeColorReceived)
public:
    explicit SocketClient(QObject *parent = nullptr);
    Q_INVOKABLE void connectToServer();
    QString themeColor() const;

signals:
    void themeColorReceived(const QString &color);

private slots:
    void connected();
    void disconnected();
    void readyRead();

private:
    QTcpSocket socket;
    QString m_themeColor;
};

#endif // SOCKETCLIENT_H
