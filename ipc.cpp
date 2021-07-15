#include "ipc.h"
#include <QDebug>

#define CustomPrefix "SHBPrefix: "

IPC::IPC(QString serverName, QObject *parent):QObject(parent),
    m_serverName(serverName)
{
}

//Server
bool IPC::startServer()
{
    m_server = new QLocalServer(this);
    QLocalServer::removeServer(m_serverName);

    bool ret = m_server->listen(m_serverName);

    if (!ret) {
        return false;
    }

    connect(m_server, SIGNAL(newConnection()), this, SLOT(slot_serverNewClientConnected()));

    return true;
}

void IPC::sendMsgToClient(QString clientId, QByteArray msg)
{
    QLocalSocket *socket = m_clients.value(clientId);
    if (socket == nullptr)
        return;
    socket->write(msg);
}

void IPC::slot_serverNewClientConnected()
{
    qDebug() << __FUNCTION__;
    QLocalSocket *socket = m_server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_serverReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(slot_serverDisconnect()));
}

void IPC::slot_serverReadyRead()
{
    QLocalSocket *socket = static_cast<QLocalSocket *>(sender());

    if (!socket) {

        qDebug() << __FUNCTION__ << "Socket is null";
        return;
    }

    qDebug() << socket->serverName();

    QByteArray byte = socket->readAll();
    QString msg = byte.data();
    if (msg.startsWith(CustomPrefix)) {

        QString key = msg.replace(CustomPrefix, "");
        m_clients.insert(key, socket);
        emit signal_serverNewClientDidConnect(key);
        return;
    }

    QString key;
    QStringList keys = m_clients.keys();
    for (int i = 0; i < keys.count(); i++) {
        QString k = keys[i];
        auto tempSock = m_clients[k];
        if (socket == tempSock) {
            key = k;
            break;
        }
    }

    emit signal_serverDidRecievedMsg(key, byte);
}

void IPC::slot_serverDisconnect()
{
    QStringList keys = m_clients.keys();
    for (int i = 0; i < keys.count(); i++) {

        QString key = keys[i];
        auto socket = m_clients.value(key);
        if (socket->state() != QLocalSocket::ConnectedState) {
            socket->deleteLater();
            m_clients.remove(key);

            emit signal_serverDidDisconnect(key);
        }
    }
}





//Client
void IPC::connectToServer(QString clientIndetifier)
{
    m_socket = new QLocalSocket(this);
    m_socket->abort();
    m_socket->connectToServer(m_serverName);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(slot_clientReadyRead()));

    QString msg = CustomPrefix + clientIndetifier;
    m_socket->write(msg.toUtf8());
}

void IPC::sendMsgToServer(QByteArray msg)
{
    m_socket->write(msg);
}

void IPC::slot_clientReadyRead()
{
    QByteArray byte = m_socket->readAll();
    emit signal_clientDidRecievedMsg(byte);
}


