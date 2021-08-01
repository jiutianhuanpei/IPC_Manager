#include "hblocalclient.h"
#include <QLocalSocket>
#include <QJsonDocument>

HBLocalClient::HBLocalClient(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
}

void HBLocalClient::connectServer(QString serverName, QString clientId)
{

    if (nullptr != m_socket) {
        m_socket->disconnectFromServer();
    }

    m_socket = new QLocalSocket();
    m_socket->connectToServer(serverName);

    connect(m_socket, &QLocalSocket::readyRead, [&]{
       QByteArray byte = m_socket->readAll();

       qDebug() << "Client received: " << byte.data();
       QJsonDocument doc = QJsonDocument::fromJson(byte);

       QJsonObject json = doc.object();

       qDebug() << "CLientKeys:" << json.keys();

       emit signal_clientDidReceivedMsg(json);
    });

    connect(m_socket, &QLocalSocket::disconnected, this, &HBLocalClient::signal_clientDisconnectedFromServer);


    QJsonObject json;
    json[HBClientIdKey] = clientId;
    sendMsgToServer(json);
}

void HBLocalClient::sendMsgToServer(QJsonObject msg)
{
    QJsonDocument doc(msg);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
}
