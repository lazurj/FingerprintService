#ifndef CLIENTWORKER_H
#define CLIENTWORKER_H

#include "config.h"

//DBOX Modules
#include "preprocessing.h"
#include "extraction.h"
#include "matcher.h"

#define SUPREMA_IMG_WIDTH 320
#define SUPREMA_IMG_HEIGHT 480

// control data size in bytes
#define GABOR_PARAMS_SIZE 64

typedef struct result {
    int clientID;
    PREPROCESSING_RESULTS prepResults;
    EXTRACTION_RESULTS extrResults;
} WORKER_RESULT;

typedef struct stausChangedData {
    int clientID;
    QString statusString;
    WorkerStatus status;
} CHANGE_STATUS_DATA;

class ClientWorker : public QThread {
    Q_OBJECT
public:
     explicit ClientWorker(int ID, QObject *parent =0);
    ~ClientWorker();
     void run();

public slots:
    void init(QTcpSocket* socket, int id);
    void readData();
    void preprocessingResult(PREPROCESSING_RESULTS results);
    void extractionResult(EXTRACTION_RESULTS results);
    void matchingResult(bool success);
    void process();
    void setStatus(WorkerStatus status);
    WorkerStatus getStatus();
    QString getStatusString();
    int getClientID();
    QTcpSocket* getSocket();


signals:
    void preprocesingDoneSignal(WORKER_RESULT resultData);
    void extractionDoneSignal(WORKER_RESULT resultData);
    void statusChanged(CHANGE_STATUS_DATA statusData);
    void finished();
private:
    WorkerStatus status;
    int clientID;
    QTcpSocket* socket;
    Preprocessing preprocessingModule;
    Extraction extractionModule;
    Matcher matcherModule;
    QImage originalImage, skeletonImage;
    int fragmentCounter;
    unsigned char* lastImageData;
    QByteArray paramsData;
    QByteArray img;
    WORKER_RESULT resultData;
    QVector<QVector<MINUTIA>> fingerprints;
    bool returnSkeleton;
};

#endif // CLIENTWORKER_H
