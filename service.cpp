#include "service.h"
#include "ui_service.h"

Service::Service(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Service)
{
    ui->setupUi(this);
    qRegisterMetaType < WORKER_RESULT >("WORKER_RESULT");
    qRegisterMetaType < CHANGE_STATUS_DATA >("CHANGE_STATUS_DATA");
    ui->clientTable->setRowCount(0);
    ui->clientTable->setColumnCount(5);

    QStringList tableHeader;
    tableHeader <<"Id"<<"Address"<<"Status"<<"Last activity"<<"";
    ui->clientTable->setHorizontalHeaderLabels(tableHeader);
    ui->clientTable->setColumnWidth(0, ui->clientTable->width()/12);
    ui->clientTable->setColumnWidth(1, ui->clientTable->width()/4);
    ui->clientTable->setColumnWidth(2, ui->clientTable->width()/6);
    ui->clientTable->setColumnWidth(3, ui->clientTable->width()/6);
    ui->clientTable->setColumnWidth(4, ui->clientTable->width()/6);
    ui->clientTable->verticalHeader()->setVisible(false);
    ui->clientTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->clientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->clientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->clientTable->setShowGrid(false);
    ui->clientTable->setStyleSheet("QTableView {selection-background-color: blue;}");
    //ui->clientTable->setGeometry(QApplication::desktop()->screenGeometry());

    QObject::connect(&(this->connection.server), SIGNAL(newConnection()), this, SLOT(registerConection()));
}

Service::~Service()
{
    this->connection.closeServer();
    delete ui;
}

void Service::on_startServerBT_clicked() {
    if(this->connection.server.isListening()) {
       showMessage(MessageType::Info, "Server", "Server already running!");
       return;
    }
    this->connection.initServer() ? showMessage(MessageType::Info, "Server", "Server started successfuly!") : showMessage(MessageType::Error, "Server", "Something went wrong!");
}

void Service::on_stopServerBT_clicked() {
    this->connection.closeServer();
    showMessage(MessageType::Info, "Server", "Server was stopped!");
}

void Service::registerConection() {
    ClientWorker* worker = this->connection.registerClient();
    QObject::connect(worker,SIGNAL(preprocesingDoneSignal(WORKER_RESULT)), this, SLOT(preprocessingDone(WORKER_RESULT)));
    QObject::connect(worker,SIGNAL(extractionDoneSignal(WORKER_RESULT)), this, SLOT(extractionDone(WORKER_RESULT)));
    QObject::connect(worker,SIGNAL(statusChanged(CHANGE_STATUS_DATA)), this, SLOT(clientStatusChanged(CHANGE_STATUS_DATA)));
    QObject::connect(worker->getSocket(), SIGNAL(disconnected()),this, SLOT(removeClient()));
    addClientToTable(worker);

}

void Service::preprocessingDone(WORKER_RESULT results) {
      QImage skeleton(results.prepResults.imgSkeleton.data, results.prepResults.imgSkeleton.cols, results.prepResults.imgSkeleton.rows, QImage::Format_Grayscale8);
      ui->skeletonImg->setFixedHeight(SUPREMA_IMG_HEIGHT);
      ui->skeletonImg->setFixedWidth(SUPREMA_IMG_WIDTH);
      ui->skeletonImg->setPixmap(QPixmap::fromImage(skeleton));

      QImage original(results.prepResults.imgOriginal.data, results.prepResults.imgOriginal.cols, results.prepResults.imgOriginal.rows, QImage::Format_Grayscale8);
      ui->originalImg->setFixedHeight(SUPREMA_IMG_HEIGHT);
      ui->originalImg->setFixedWidth(SUPREMA_IMG_WIDTH);
      ui->originalImg->setPixmap(QPixmap::fromImage(original));

}

void Service::extractionDone(WORKER_RESULT results) {
    QImage tmp = ui->originalImg->grab().toImage();

    QPainter painter(&tmp);
    QPen redpen;
    QPen bluepen;
    redpen.setWidth(1);
    bluepen.setWidth(1);
    redpen.setColor(QColor(255,0,0,255));
    bluepen.setColor(QColor(0,0,255,255));

    foreach (MINUTIA m, results.extrResults.minutiaePredictedFixed) {
        if(m.type == 0){
            painter.setPen(redpen);
        }
        else{
            painter.setPen(bluepen);
        }
        painter.drawEllipse(m.xy,3,3);
    }
}

void Service::clientStatusChanged(CHANGE_STATUS_DATA data) {
    int row = getRowByClientId(data.clientID);
    if(row > -1) {
        ui->clientTable->setItem(row,2, new QTableWidgetItem(data.statusString));

    }

}

void Service::addClientToTable(ClientWorker* worker) {
    ui->clientTable->insertRow(ui->clientTable->rowCount());
    int rowId = ui->clientTable->rowCount()-1;
    ui->clientTable->setItem(rowId,0, new QTableWidgetItem(QString::number(worker->getClientID())));
    ui->clientTable->setItem(rowId,1, new QTableWidgetItem(worker->getSocket()->localAddress().toString() + ":" + worker->getSocket()->localPort()));
    ui->clientTable->setItem(rowId,2, new QTableWidgetItem(worker->getStatusString()));
    QDateTime time = QDateTime::currentDateTime();

    ui->clientTable->setItem(rowId,3, new QTableWidgetItem(time.toString("yyyy-M-d : h-mm-s")));

    QPushButton* disconnectButton = new QPushButton();
        disconnectButton->setText("Disconnect");
        QPalette pal = disconnectButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::red));
        disconnectButton->setPalette(pal);
        disconnectButton->update();
        disconnectButton->setObjectName("disconectBtn_" + QString::number(worker->getClientID()));
        QObject::connect(disconnectButton, SIGNAL(clicked()),this, SLOT(removeClient()));
        QWidget* pWidget = new QWidget();
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->addWidget(disconnectButton);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);
        ui->clientTable->setCellWidget(rowId, 4, pWidget);
}

void Service::deleteClientFromTable(int removedClientID){
   int row = getRowByClientId(removedClientID);
   if(row > -1) {
       ui->clientTable->removeRow(row);
   }
}

void Service::showMessage(MessageType type, QString name, QString messageText) {
    switch (type) {
    case MessageType::Info :
        QMessageBox::information(
            this,
            tr(name.toLocal8Bit().data()),
            tr(messageText.toLocal8Bit().data()));
        break;
    case MessageType::Warning:
        QMessageBox::warning(
            this,
            tr(name.toLocal8Bit().data()),
            tr(messageText.toLocal8Bit().data()));
        break;
    case MessageType::Error :
        QMessageBox::critical(
            this,
            tr(name.toLocal8Bit().data()),
            tr(messageText.toLocal8Bit().data()));
        break;
    default:
        break;
    }
}

int Service::getRowByClientId(int clientID) {
    QString removedClientIdString = QString::number(clientID);
    for (int row = 0; row < ui->clientTable->rowCount(); row++) {
        if(removedClientIdString ==  ui->clientTable->item(row,0)->text()) {
            return row;
        }
    }
    return -1;
}

void Service::removeClient(){
    int clientID = -1;
    QTcpSocket* disconectedSocket = qobject_cast<QTcpSocket*>(sender());
    QPushButton* disconectedButton = qobject_cast<QPushButton*>(sender());
    //Client disconnected by server
    if(disconectedButton != NULL) {
        QStringList list = disconectedButton->objectName().split("_");
        QString clientID_str = list.value(1);
        clientID = clientID_str.toInt();
        clientID = connection.disconnectSocket(clientID);

    //Remove disconnected worker
    } else if (disconectedSocket != NULL){
        clientID = connection.removeWorker(disconectedSocket);
    }

    if(clientID > -1 ) {
        deleteClientFromTable(clientID);
    }
}


