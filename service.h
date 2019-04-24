#include "config.h"
#include "server.h"
#include "clientworker.h"

namespace Ui {
class Service;
}

#ifndef SERVICE_H
#define SERVICE_H

class Service : public QMainWindow
{
    Q_OBJECT

public:
    explicit Service(QWidget *parent = 0);
    ~Service();

public slots:
    void registerConection();
    void preprocessingDone(WORKER_RESULT data);
    void extractionDone(WORKER_RESULT data);
    void clientStatusChanged(CHANGE_STATUS_DATA data);
    void on_startServerBT_clicked();
    void on_stopServerBT_clicked();
    void showMessage(MessageType type, QString name, QString messageText);
    void removeClient();
private slots:
    void addClientToTable(ClientWorker* worker);
    int getRowByClientId(int clientID);
    void deleteClientFromTable(int removedClientID);
private:
    Ui::Service *ui;
    Server connection;
};

#endif // SERVICE_H
