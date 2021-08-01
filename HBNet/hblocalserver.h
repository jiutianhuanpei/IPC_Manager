#ifndef HBLOCALSERVER_H
#define HBLOCALSERVER_H

#include <QLocalServer>
#include "HBNet_global.h"
#include "hbsocketthread.h"
#include <QJsonDocument>
#include <QJsonObject>

class HBNET_EXPORT HBLocalServer : public QLocalServer
{
    Q_OBJECT
public:
    explicit HBLocalServer(QObject *parent = nullptr);

    ~HBLocalServer();

    //主进程开启服务
    void startServer();

    //发送消息到指定子进程
    void sendMsgToClient(QString clientId, QJsonObject msg);

signals:
    //接收到子进程发来的消息
     void signal_serverDidReceived(QJsonObject msg);

protected:
    void incomingConnection(quintptr socketDescriptor);

private:
    QMap<QString, HBSocketThread*> m_clientMap;

};

#endif // HBLOCALSERVER_H
