#include "clientworker.h"

ClientWorker::~ClientWorker() {

}

ClientWorker::ClientWorker(int ID, QObject *parent) : QThread(parent) {
  this->clientID = ID;
}

void ClientWorker::init(QTcpSocket *socket, int id) {
    this->status = WorkerStatus::INITIALIZING;
    this->clientID = id;
    this->resultData.clientID = id;
    this->socket = socket;
    this->fragmentCounter = 0;
}

void ClientWorker::run() {
  //thread starts here
  process();
}

void ClientWorker::process() {
    QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readData()), Qt::DirectConnection);
    QObject::connect(&(this->preprocessingModule),SIGNAL(preprocessingDoneSignal(PREPROCESSING_RESULTS)), this, SLOT(preprocessingResult(PREPROCESSING_RESULTS)));
    QObject::connect(&(this->extractionModule),SIGNAL(extractionDoneSignal(EXTRACTION_RESULTS)), this, SLOT(extractionResult(EXTRACTION_RESULTS)));
    QObject::connect(&(this->matcherModule),SIGNAL(verificationDoneSignal(bool)), this, SLOT(matchingResult(bool)));
    setStatus(WorkerStatus::WAITING);
}

void ClientWorker::readData() {
    this->socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray readData = this->socket->readAll();
    if (readData.size() == 0) {
        return;
    }
    qDebug() << readData.size() << " B received.";

    if(this->fragmentCounter == 0) {
        if(readData.size() >= GABOR_PARAMS_SIZE) {
            paramsData = readData.left(GABOR_PARAMS_SIZE);
            //remove params data from recieved data
            readData = readData.mid(GABOR_PARAMS_SIZE, readData.size());
        } else {
            paramsData.append(readData);
            return;
        }
        QByteArray blockSizeBA, sigmaBA, lambdaBA, gammaBA, psiBA, gaussBasicBlockSizeBA,  gaussBasicSigmaBA,  gaussAdvancedBlockSizeBA, gaussAdvancedSigmaBA, holeSizeBA;

        QDataStream dataStream1(&blockSizeBA, QIODevice::ReadWrite);
        QDataStream dataStream2(&sigmaBA, QIODevice::ReadWrite);
        QDataStream dataStream3(&lambdaBA, QIODevice::ReadWrite);
        QDataStream dataStream4(&gammaBA, QIODevice::ReadWrite);
        QDataStream dataStream5(&psiBA, QIODevice::ReadWrite);
        QDataStream dataStream6(&gaussBasicBlockSizeBA, QIODevice::ReadWrite);
        QDataStream dataStream7(&gaussBasicSigmaBA, QIODevice::ReadWrite);
        QDataStream dataStream8(&gaussAdvancedBlockSizeBA, QIODevice::ReadWrite);
        QDataStream dataStream9(&gaussAdvancedSigmaBA, QIODevice::ReadWrite);
        QDataStream dataStream10(&holeSizeBA, QIODevice::ReadWrite);

        blockSizeBA = paramsData.left(4);
        sigmaBA = paramsData.mid(4,8);
        lambdaBA = paramsData.mid(12,8);
        gammaBA = paramsData.mid(20,8);
        psiBA = paramsData.mid(28,8);

        gaussBasicBlockSizeBA = paramsData.mid(36,4);
        gaussBasicSigmaBA = paramsData.mid(40,8);
        gaussAdvancedBlockSizeBA = paramsData.mid(48,4);
        gaussAdvancedSigmaBA = paramsData.mid(52,8);
        holeSizeBA= paramsData.mid(60,4);

        quint32 blockSize;
        double sigma;
        double lambda;
        double gamma;
        double psi;
        int gaussBasicBlockSize;
        double gaussBasicSigma;
        int gaussAdvancedBlockSize;
        double gaussAdvancedSigma;
        int holeSize;

        dataStream1 >> blockSize;
        dataStream2 >> sigma;
        dataStream3 >> lambda;
        dataStream4 >> gamma;
        dataStream5 >> psi;
        dataStream6 >> gaussBasicBlockSize;
        dataStream7 >> gaussBasicSigma;
        dataStream8 >> gaussAdvancedBlockSize;
        dataStream9 >> gaussAdvancedSigma;
        dataStream10 >> holeSize;

        qDebug() << "Gabor params:\n" << "blockSize: " << blockSize << "\n sigma: " << sigma << "\n lambda: " << lambda << "\n gamma: " << gamma << "\n psi:  " << psi
                    << "basicBlockSize: " << gaussBasicBlockSize << "\n basicSigma: " << gaussBasicSigma << "\n advBlockSIze: " << gaussAdvancedBlockSize << "\n advSigma: " << gaussAdvancedSigma << "\n holeSize:  " << holeSize;
        ++this->fragmentCounter;
        preprocessingModule.setPreprocessingParams(blockSize, lambda, sigma, gaussBasicBlockSize, gaussBasicSigma, gaussAdvancedBlockSize, gaussAdvancedSigma, holeSize);
        preprocessingModule.setGaborParams(blockSize, lambda, sigma, gamma, psi);
        paramsData.clear();
    }
    if(this->fragmentCounter > 0) {
        this->img.append(readData);
        if(this->img.size() != (SUPREMA_IMG_WIDTH*SUPREMA_IMG_HEIGHT)) {
            return;
        }
        setStatus(WorkerStatus::RUNNING);
        this->lastImageData = (unsigned char*)calloc(this->img.size(),sizeof(unsigned char));
        memcpy(this->lastImageData,(unsigned char*)this->img.data(),this->img.size());

        cv::Mat cv_img(SUPREMA_IMG_HEIGHT, SUPREMA_IMG_WIDTH, CV_8UC1,(unsigned char*)this->img.data());
        qDebug() << QThread::idealThreadCount() << "worker thread(s) available.";
        this->fragmentCounter = 0;
        preprocessingModule.loadInput(cv_img);
        preprocessingModule.start();
        this->img.clear();
    }
}

void ClientWorker::preprocessingResult(PREPROCESSING_RESULTS results) {
    qDebug() << "Preprocessing done.";
    this->resultData.prepResults = results;
    this->skeletonImage = QImage(results.imgSkeleton.data,results.imgSkeleton.cols,results.imgSkeleton.rows,QImage::Format_Grayscale8);

    this->originalImage =  QImage(results.imgOriginal.data,results.imgOriginal.cols,results.imgOriginal.rows,QImage::Format_Grayscale8);

    emit preprocesingDoneSignal(this->resultData);
    //this->extractionModule.loadInput(results);
    //this->extractionModule.start();

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    this->skeletonImage.save(&buffer, "PNG");
    unsigned char* img_data = (unsigned char*)bytes.data();
    this->socket->write((const char*)img_data,sizeof(unsigned char)*results.imgSkeleton.cols*results.imgSkeleton.rows);
    qDebug() << "Skeleton img size : " << bytes.size();

}

void ClientWorker::extractionResult(EXTRACTION_RESULTS results) {
    qDebug() << "Extraction done.";
    this->resultData.extrResults = results;
    emit extractionDoneSignal(this->resultData);

    this->fingerprints.push_back(results.minutiaePredictedFixed);
    if(this->fingerprints.size()==2){
        MATCHER matcher = suprema;
        this->matcherModule.setMatcher(matcher);
        QVector<QVector<MINUTIA>> tmp_v;
        tmp_v.push_back(this->fingerprints[1]);
        qDebug() << this->fingerprints.size() << this->fingerprints[0].size() << " vs. " << tmp_v[0].size();
        this->matcherModule.verify(this->fingerprints[0],tmp_v);
        this->fingerprints.clear();
    }
}

void ClientWorker::matchingResult(bool success){
        qDebug() << "VERIFICATION DONE:" << success;
}

void ClientWorker::setStatus(WorkerStatus status){
    this->status = status;
    CHANGE_STATUS_DATA data;
    data.clientID = this->clientID;
    data.statusString = getStatusString();
    data.status = status;
    emit statusChanged(data);
}

QString ClientWorker::getStatusString(){
    switch (status) {
    case WorkerStatus::INITIALIZING:
        return "INITIALIZING";
    case WorkerStatus::READY:
        return "READY";
    case WorkerStatus::RUNNING:
        return "RUNNING";
    case WorkerStatus::STOPED:
        return "STOPED";
    case WorkerStatus::WAITING:
        return "WAITING";
    default:
        return "";
    }
}

WorkerStatus ClientWorker::getStatus(){
    return this->status;
}

int ClientWorker::getClientID(){
    return this->clientID;
}

QTcpSocket* ClientWorker::getSocket(){
    return this->socket;
}
