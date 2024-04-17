#include "socketclient.h"
#include <QDebug>
#include <QTimer>

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    connect(&socket, &QTcpSocket::connected, this, &SocketClient::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &SocketClient::disconnected);
    connect(&socket, &QTcpSocket::readyRead, this, &SocketClient::readyRead);

    connectToServer();
}

void SocketClient::connectToServer()
{
    quint16 port = 8000;
    socket.connectToHost("192.168.1.145", port);
}

void SocketClient::connected()
{
    qDebug() << "Connected to server";
    // Send request to server
    socket.write("GET_COLOR");
    socket.waitForBytesWritten();
}

void SocketClient::disconnected()
{
    qDebug() << "Disconnected from server";
    socket.close();
}

void SocketClient::readyRead()
{
    qDebug() << "Reading theme color from server";
    QByteArray data = socket.readAll();
    qDebug() << data << "socket data";
    m_themeColor = QString::fromUtf8(data);
    qDebug() << m_themeColor << "converted socket data";
    emit themeColorReceived(m_themeColor);
}

QString SocketClient::themeColor() const
{
    return m_themeColor;
}
