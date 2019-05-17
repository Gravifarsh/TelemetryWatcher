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

typedef struct
{
    float time;
    float temperature;
    float pressure;
    float height;
} data_BMP280_t;

typedef struct
{
    float time;
    float accel[3];
    float compass[3];
    union
    {
    float gyro[3];
    float quaternion[4];
    };
} data_MPU9255_t;

typedef struct
{
    float time;
    uint8_t	GlobalState;
    uint8_t MPU9255_1;
    uint8_t BMP280_1;
    uint8_t MPU9255_2;
    uint8_t BMP280_2;
    uint8_t SD;
    uint8_t	nRF;
} system_state_t;

void UDPDataGenerator::tryParse() {
    qDebug() << "Parsing";

    QMap<QString, QPointF> data;
    int index, size;

    size = sizeof(data_BMP280_t);
    index = findPacket(0xFD, size);
    if(index != -1) {
        data_BMP280_t* packet = (data_BMP280_t*)(mBuffer.data() + index);
        data["BMP_1_temp"] = {packet->time, packet->temperature};
        data["BMP_1_press"] = {packet->time, packet->pressure};
        data["BMP_1_height"] = {packet->time, packet->height};

        mBuffer.remove(index - 1, size + 2);
    }

    size = sizeof(data_BMP280_t);
    index = findPacket(0xFE, size);
    if(index != -1) {
        data_BMP280_t* packet = (data_BMP280_t*)(mBuffer.data() + index);
        data["BMP_2_temp"] = {packet->time, packet->temperature};
        data["BMP_2_press"] = {packet->time, packet->pressure};
        data["BMP_2_height"] = {packet->time, packet->height};

        mBuffer.remove(index - 1, size + 2);
    }

    size = sizeof(data_MPU9255_t);
    index = findPacket(0xFA, size);
    if(index != -1) {
        data_MPU9255_t* packet = (data_MPU9255_t*)(mBuffer.data() + index);
        data["MPU_1_accelX"] = {packet->time, packet->accel[0]};
        data["MPU_1_accelY"] = {packet->time, packet->accel[1]};
        data["MPU_1_accelZ"] = {packet->time, packet->accel[2]};

        data["MPU_1_gyroX"] = {packet->time, packet->gyro[0]};
        data["MPU_1_gyroY"] = {packet->time, packet->gyro[1]};
        data["MPU_1_gyroZ"] = {packet->time, packet->gyro[2]};

        data["MPU_1_compassX"] = {packet->time, packet->compass[0]};
        data["MPU_1_compassY"] = {packet->time, packet->compass[1]};
        data["MPU_1_compassZ"] = {packet->time, packet->compass[2]};

        mBuffer.remove(index - 1, size + 2);
    }

    size = sizeof(data_MPU9255_t);
    index = findPacket(0xFB, size);
    if(index != -1) {
        data_MPU9255_t* packet = (data_MPU9255_t*)(mBuffer.data() + index);
        data["MPU_2_accelX"] = {packet->time, packet->accel[0]};
        data["MPU_2_accelY"] = {packet->time, packet->accel[1]};
        data["MPU_2_accelZ"] = {packet->time, packet->accel[2]};

        data["MPU_2_gyroX"] = {packet->time, packet->gyro[0]};
        data["MPU_2_gyroY"] = {packet->time, packet->gyro[1]};
        data["MPU_2_gyroZ"] = {packet->time, packet->gyro[2]};

        data["MPU_2_compassX"] = {packet->time, packet->compass[0]};
        data["MPU_2_compassY"] = {packet->time, packet->compass[1]};
        data["MPU_2_compassZ"] = {packet->time, packet->compass[2]};

        mBuffer.remove(index - 1, size + 2);
    }

    riseData(data);
}

int UDPDataGenerator::findPacket(uint8_t flag, size_t size) {
    int left = mBuffer.indexOf(char(flag));
    while(left != - 1) {
        int right = left;

        while((right - left - 1) < int(size) && (right != - 1))
            right = mBuffer.indexOf(char(flag), right + 1);

        if((right - left - 1) == int(size)) {
            qDebug() << "Found packet";

            return (left + 1);
        }
        else {
            left = mBuffer.indexOf(char(flag), left + 1);
        }
    }
    return -1;
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
