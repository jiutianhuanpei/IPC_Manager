#ifndef HBSOCKETTHREAD_H
#define HBSOCKETTHREAD_H

#include <QThread>
#include "HBNet_global.h"
#include <QLocalSocket>

class HBNET_EXPORT HBSocketThread : public QThread
{
    Q_OBJECT
public:
    explicit HBSocketThread(quintptr socketDescriptor, QObject *parent = nullptr);


    QString m_clientId;

    void sendMsg(QByteArray msg);

signals:
    void signal_socketDisconnected();
    void signal_socketReadyRead(QByteArray msg);

private:
    quintptr m_socketDescriptor;


    // QThread interface
protected:
    void run();

private:
    QLocalSocket *m_socket = nullptr;

};

#endif // HBSOCKETTHREAD_H
