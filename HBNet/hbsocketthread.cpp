#include "hbsocketthread.h"
#include <QDebug>

HBSocketThread::HBSocketThread(quintptr socketDescriptor, QObject *parent) : QThread(parent), m_socketDescriptor(socketDescriptor)
{
    qRegisterMetaType<QLocalSocket::LocalSocketError>("QLocalSocket::LocalSocketError");
}

void HBSocketThread::sendMsg(QByteArray msg)
{

    m_socket->write(msg);
}


void HBSocketThread::run()
{

    m_socket = new QLocalSocket();

    connect(m_socket, &QLocalSocket::disconnected, this, &HBSocketThread::signal_socketDisconnected);
    connect(m_socket, &QLocalSocket::readyRead, [&]{
        QByteArray msg = m_socket->readAll();
        qDebug() << "Thread received: " << msg.data();
        emit signal_socketReadyRead(msg);
    });

    connect(m_socket, &QLocalSocket::errorOccurred, this, [&](QLocalSocket::LocalSocketError socketError) {
        qDebug() << "HB Error: " << socketError;
    });

    m_socket->setSocketDescriptor(m_socketDescriptor);
    exec();
}
