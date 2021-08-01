#ifndef HBLOCALCLIENT_H
#define HBLOCALCLIENT_H

#include "HBNet_global.h"
#include <QJsonObject>

class QLocalSocket;

class HBNET_EXPORT HBLocalClient : public QObject
{
    Q_OBJECT
public:
    explicit HBLocalClient(QObject *parent = nullptr);

    //连接到主进程服务
    void connectServer(QString serverName, QString clientId);

    //发送消息到主进程
    void sendMsgToServer(QJsonObject msg);

signals:
    //收到主进程发来的消息
    void signal_clientDidReceivedMsg(QJsonObject msg);

    //断开了与主进程的连接（eg：主进程关闭时）
    void signal_clientDisconnectedFromServer();

private:
    QLocalSocket *m_socket = nullptr;

};

#endif // HBLOCALCLIENT_H
