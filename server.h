#ifndef CONNECTION_H
#define CONNECTION_H

#include "config.h"
#include "clientworker.h"

//Maximum allowed pennding connections to server
#define MAX_CONNECTIONS 10
class Server : public QObject
{
    Q_OBJECT public:
    explicit Server(QObject *parent = nullptr);
    signals:

private slots:

private:
    quint16 port;

public:
    QTcpServer server;
    QMap<int, ClientWorker*> clientMap;

public slots:
    bool initServer();
    bool closeServer();
    int getFreeMinimalClientID();
    ClientWorker* registerClient();
    int removeWorker(QTcpSocket* socket);
    int disconnectSocket(int clientID);
    void removeAllWorkers();

public slots:
};

#endif // CONNECTION_H
