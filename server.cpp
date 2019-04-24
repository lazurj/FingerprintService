#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->port = 9999;
}

bool Server::initServer() {
    bool running = this->server.listen(QHostAddress::LocalHost,port);
    this->server.setMaxPendingConnections(MAX_CONNECTIONS);
    running ? qDebug() << "Server is running..." : qDebug() << "Problems ocured while starting server.";
    return running;
}

bool Server::closeServer() {
    if(server.isListening()) {
        removeAllWorkers();
        server.close();
        qDebug() << "Server was closed";
        return true;
    }
    return false;
}


ClientWorker* Server::registerClient() {
    int clientID = getFreeMinimalClientID();
    qDebug() << "New connection has just arrived. Id: "  << clientID;
    QTcpSocket *activeClient =  this->server.nextPendingConnection();
    if(clientID < 0 ) {
        activeClient->disconnectFromHost();
    }
    ClientWorker *worker = new ClientWorker(clientID, this);
    worker->init(activeClient, clientID);
    worker->start(); //Which will cause the run() function
    /*QThread* thread = new QThread;
    ClientWorker* worker = new ClientWorker;
*/

    /*worker->moveToThread(thread);*/
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //connect(worker->thread(), SIGNAL (finished()), worker->thread(), SLOT (deleteLater()));
    this->clientMap.insert(clientID, worker);
    return worker;

}

// remove
int Server::removeWorker(QTcpSocket* socket) {
    qDebug() << "Removing disconnected worker";
    foreach ( int clientId, this->clientMap.keys()) {
        ClientWorker* worker = clientMap.value(clientId);
        QTcpSocket* clientSocket = worker->getSocket();
        if((socket->peerPort() == clientSocket->peerPort()) && (socket->peerAddress().toString() == clientSocket->peerAddress().toString())) {
            if(this->clientMap.remove(clientId)){
                qDebug() << "Client with ID: " << worker->getClientID()<< " successfully deleted.";
                qDebug() << "Number of remaining clients: " << this->clientMap.size();
            }
            else{
                qDebug() << "Failed to remove client with ID: " << worker->getClientID();
            }
            emit worker->finished();
            return clientId;
        }
    }
    return -1;
}

// Client disctoneced by server, this will signal removeWorker and remove worker from clientMap
int Server::disconnectSocket(int clientID) {
    qDebug() << "Removing worker from list by server";
    ClientWorker* worker = clientMap.value(clientID);
    worker->getSocket()->disconnectFromHost();
    return clientID;
}

void Server::removeAllWorkers() {
    foreach ( int clientId, this->clientMap.keys()) {
        ClientWorker* worker = clientMap.value(clientId);
        worker->getSocket()->disconnectFromHost();
        qDebug() << "Client with ID: " << worker->getClientID()<< " successfully disconnected.";
    }
}

int Server::getFreeMinimalClientID(){
    for(int clientID = 0; clientID < MAX_CONNECTIONS ; clientID++) {
        if(!clientMap.contains(clientID)){
            return clientID;
        }
    }
    return -1;
}

