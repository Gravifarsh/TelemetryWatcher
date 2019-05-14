#include "datagenerator.h"

CSVDataGenerator::CSVDataGenerator(QObject *parent) : DataGenerator(parent),
    mTimer(0),
    mTextStream(0),
    mFile(0)
{
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(generationStep()));
    mTimer->setSingleShot(true);

    mTimeFieldName = "time";

    mTime = 0;
}

void CSVDataGenerator::generationStep(){
    if(mTextStream->atEnd()) return;

    QString string = mTextStream->readLine();

    QStringList strings = string.split(",");

    QMap<QString, double> values;
    for(int i = 0; i < mFieldNames.size(); i++)
        values[mFieldNames[i]] = strings[i].toDouble();

    QMap<QString, QPointF> data;
    for(int i = 0; i < mFieldNames.size(); i++)
        data[mFieldNames[i]] = QPointF(values[mTimeFieldName],
                                       values[mFieldNames[i]]);

    emit riseData(data);
    mTimer->start(int(values[mTimeFieldName] - mTime));
    mTime = values[mTimeFieldName];
}

void CSVDataGenerator::startGeneration(){
    if(!mTextStream) return;
    mTimer->start();
}

void CSVDataGenerator::selectFile(const QString &filename){
    if(mFile) delete mFile;
    if(mTextStream) delete mTextStream;

    mFile = new QFile(filename);

    if(!mFile->open(QFile::ReadOnly)){
        qDebug() << mFile->errorString();
        delete mFile;
        return;
    }

    mTextStream = new QTextStream(mFile);
    mFieldNames = mTextStream->readLine().split(",");
}

CSVDataGenerator::~CSVDataGenerator(){
    if(mFile) delete mFile;
    if(mTextStream) delete mTextStream;
    delete mTimer;
}



UDPDataGenerator::UDPDataGenerator(QObject* parent) :
    DataGenerator(parent),
    mSocket(this) {
    mSocket.bind(QHostAddress::LocalHost, 11001);

    connect(&mSocket, SIGNAL(readyRead()),
            this, SLOT(processDatagram()));
}

struct packet {
    unsigned char time;
    unsigned char data;
};

void UDPDataGenerator::tryParse() {
    qDebug() << "Parsing";

    QMap<QString, QPointF> data;

    int left = mBuffer.indexOf(char(0xFA));
    while(left != - 1) {
        int right = left;

        while((right - left - 1) < int(sizeof(packet)) &&
              (right != - 1))
            right = mBuffer.indexOf(char(0xFA), right + 1);

        if((right - left - 1) == sizeof(packet)) {
            qDebug() << "Found packet";

            packet* p = (packet*)(mBuffer.data() + left + 1);
            data["Data"] = QPointF(p->time, p->data);

            riseData(data);

            mBuffer.remove(left, right - left + 1);
            left = mBuffer.indexOf(char(0xFA));
        }
        else {
            left = mBuffer.indexOf(char(0xFA), left + 1);
        }
    }
}

void UDPDataGenerator::processDatagram() {
    qDebug() << "Has datagram";
    QByteArray tmp;
    while(mSocket.hasPendingDatagrams()) {
        tmp.resize(mSocket.pendingDatagramSize());
        mSocket.readDatagram(tmp.data(), tmp.size());
        mBuffer.append(tmp);
        qDebug() << tmp;
    }
    tryParse();
}



UDPDataGenerator::~UDPDataGenerator() { }
