#include "hblocalserver.h"
#include <QUuid>

HBLocalServer::HBLocalServer(QObject *parent) : QLocalServer(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
}

HBLocalServer::~HBLocalServer()
{
    QStringList list = m_clientMap.keys();
    for (int i = 0; i < list.count(); i++) {
        HBSocketThread *t = m_clientMap[list[i]];
        if (nullptr != t) {
            t->quit();
            t->wait();
        }
    }
}

void HBLocalServer::startServer()
{
    QString serverName = QUuid::createUuid().toString();
    qDebug() << __FUNCTION__ << serverName;
    removeServer(serverName);
    listen(serverName);
}

void HBLocalServer::sendMsgToClient(QString clientId, QJsonObject msg)
{
    HBSocketThread *thread = m_clientMap[clientId];

    QJsonDocument doc(msg);

    thread->sendMsg(doc.toJson());
}


void HBLocalServer::incomingConnection(quintptr socketDescriptor)
{
    qDebug() << __FUNCTION__ << socketDescriptor << QThread::currentThread();
    HBSocketThread *thread = new HBSocketThread(socketDescriptor);


    connect(thread, &HBSocketThread::signal_socketDisconnected, this, [&] {

        HBSocketThread *t = static_cast<HBSocketThread *>(sender());
        if (nullptr == t) {
            return ;
        }

        qDebug() << "signal_socketDisconnected block: " << t->m_clientId;
        m_clientMap.remove(t->m_clientId);
    });
    connect(thread, &HBSocketThread::signal_socketReadyRead, this, [&](QByteArray msg) {

        QJsonDocument doc = QJsonDocument::fromJson(msg);
        QJsonObject json = doc.object();

        if (json.contains(HBClientIdKey)) {
            QString clientId = json.value(HBClientIdKey).toString();

            HBSocketThread *t = static_cast<HBSocketThread *>(sender());
            if (nullptr != t) {
                t->m_clientId = clientId;
                m_clientMap.insert(clientId, t);
            }

            return ;
        }
        emit signal_serverDidReceived(doc.object());
    });
    thread->start();
}
