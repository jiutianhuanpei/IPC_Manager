#ifndef IPC_H
#define IPC_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMap>

class IPC : public QObject
{
    Q_OBJECT
public:
    explicit IPC(QString serverName, QObject *parent = nullptr);


public: //server
    bool startServer();
    void sendMsgToClient(QString clientId, QByteArray msg);

signals:
    void signal_serverNewClientDidConnect(QString clientIdentifier);
    void signal_serverDidRecievedMsg(QString clientIdentifier, QByteArray msg);
    void signal_serverDidDisconnect(QString clientIdentifier);


private slots:
    void slot_serverNewClientConnected();
    void slot_serverReadyRead();
    void slot_serverDisconnect();


private://Server
    QLocalServer *m_server;
    QMap<QString, QLocalSocket*> m_clients;


public://client
    void connectToServer(QString clientIndetifier);
    void sendMsgToServer(QByteArray msg);

signals:
    void signal_clientDidRecievedMsg(QByteArray msg);

private slots: //client
    void slot_clientReadyRead();

private://client
    QLocalSocket *m_socket;




private:
    QString m_serverName;

};

#endif // IPC_H
