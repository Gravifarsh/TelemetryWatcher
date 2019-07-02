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
    mSocket.bind(QHostAddress::AnyIPv4, 11001);

    connect(&mSocket, SIGNAL(readyRead()),
            this, SLOT(processDatagram()));
}

uint8_t hash(uint8_t* data, size_t size) {
    uint8_t retval = 0;
    for(size_t i = 0; i < size; i++)
        retval = retval ^ data[i];
    return retval;
}

void UDPDataGenerator::tryParse() {
    //qDebug() << "Parsing";

    //qDebug() << "Buffer size: " << mBuffer.size();

    QMap<QString, QPointF> data;

    data_BMP280_t data_BMP_1;
    if(findPacket<data_BMP280_t>(0xFD, &data_BMP_1)) {
        data["BMP_1_temp"] = {float(data_BMP_1.time) / 1000, data_BMP_1.temperature};
        data["BMP_1_press"] = {float(data_BMP_1.time) / 1000, data_BMP_1.pressure};
        data["BMP_1_height"] = {float(data_BMP_1.time) / 1000, data_BMP_1.height};

        qDebug() << "BMP1_height " << data_BMP_1.height;
        //qDebug() << "BMP1 packet with time: " << data_BMP_1.time;
    }

    data_BMP280_t data_BMP_2;
    if(findPacket<data_BMP280_t>(0xFE, &data_BMP_2)) {
        data["BMP_2_temp"] = {float(data_BMP_2.time) / 1000, data_BMP_2.temperature};
        data["BMP_2_press"] = {float(data_BMP_2.time) / 1000, data_BMP_2.pressure};
        data["BMP_2_height"] = {float(data_BMP_2.time) / 1000, data_BMP_2.height};

        qDebug() << "BMP2_height " << data_BMP_2.height;
        //qDebug() << "BMP2 packet with time: " << data_BMP_2.time;
    }

    data_MPU9255_t data_MPU_1;
    if(findPacket<data_MPU9255_t>(0xFA, &data_MPU_1)) {
        data["MPU_1_accelX"] = {float(data_MPU_1.time) / 1000, data_MPU_1.accel[0]};
        data["MPU_1_accelY"] = {float(data_MPU_1.time) / 1000, data_MPU_1.accel[1]};
        data["MPU_1_accelZ"] = {float(data_MPU_1.time) / 1000, data_MPU_1.accel[2]};

        data["MPU_1_gyroX"] = {float(data_MPU_1.time) / 1000, data_MPU_1.gyro[0]};
        data["MPU_1_gyroY"] = {float(data_MPU_1.time) / 1000, data_MPU_1.gyro[1]};
        data["MPU_1_gyroZ"] = {float(data_MPU_1.time) / 1000, data_MPU_1.gyro[2]};

        data["MPU_1_compassX"] = {float(data_MPU_1.time) / 1000, data_MPU_1.compass[0]};
        data["MPU_1_compassY"] = {float(data_MPU_1.time) / 1000, data_MPU_1.compass[1]};
        data["MPU_1_compassZ"] = {float(data_MPU_1.time) / 1000, data_MPU_1.compass[2]};

        //qDebug() << "MPU 1 Packet with time: " << data_MPU_1.time;
    }

    data_MPU9255_t data_MPU_2;
    if(findPacket<data_MPU9255_t>(0xFB, &data_MPU_2)) {
        data["MPU_2_accelX"] = {float(data_MPU_2.time) / 1000, data_MPU_2.accel[0]};
        data["MPU_2_accelY"] = {float(data_MPU_2.time) / 1000, data_MPU_2.accel[1]};
        data["MPU_2_accelZ"] = {float(data_MPU_2.time) / 1000, data_MPU_2.accel[2]};

        data["MPU_2_gyroX"] = {float(data_MPU_2.time) / 1000, data_MPU_2.gyro[0]};
        data["MPU_2_gyroY"] = {float(data_MPU_2.time) / 1000, data_MPU_2.gyro[1]};
        data["MPU_2_gyroZ"] = {float(data_MPU_2.time) / 1000, data_MPU_2.gyro[2]};

        data["MPU_2_compassX"] = {float(data_MPU_2.time) / 1000, data_MPU_2.compass[0]};
        data["MPU_2_compassY"] = {float(data_MPU_2.time) / 1000, data_MPU_2.compass[1]};
        data["MPU_2_compassZ"] = {float(data_MPU_2.time) / 1000, data_MPU_2.compass[2]};

        //qDebug() << "MPU 2 Packet with time: " << data_MPU_2.time;
    }

    data_LIDAR_t data_LIDAR;
    if(findPacket<data_LIDAR_t>(0xF7, &data_LIDAR))
        data["LIDAR_dist"] = {float(data_LIDAR.time) / 1000, data_LIDAR.dist};

    data_TSL_t data_TSL;
    if(findPacket<data_TSL_t>(0xF8, &data_TSL)) {
        data["TSL_ch0"] = {float(data_TSL.time) / 1000, data_TSL.ch0};
        data["TSL_ch1"] = {float(data_TSL.time) / 1000, data_TSL.ch1};
    }

    data_GPS_t data_GPS;
    if(findPacket<data_GPS_t>(0xF6, &data_GPS)) {
        data["GPS_lon"] = {float(data_GPS.time) / 1000, data_GPS.coords[0]};
        data["GPS_lat"] = {float(data_GPS.time) / 1000, data_GPS.coords[1]};
        data["GPS_h"] = {float(data_GPS.time) / 1000, data_GPS.coords[2]};
    }

    system_state_t data_STATE;
    if(findPacket<system_state_t>(0xFF, &data_STATE)) {
        data["STATE_nRF"] = {data_STATE.time, data_STATE.nRF};
        data["STATE_BMP_1"] = {data_STATE.time, data_STATE.BMP280_1};
        data["STATE_BMP_2"] = {data_STATE.time, data_STATE.BMP280_2};
        data["STATE_SD"] = {data_STATE.time, data_STATE.SD};
        data["STATE_GPS"] = {data_STATE.time, data_STATE.GPS};
        data["STATE_MPU_1"] = {data_STATE.time, data_STATE.MPU9255_1};
        data["STATE_MPU_2"] = {data_STATE.time, data_STATE.MPU9255_2};
        data["STATE_GLOBAL"] = {data_STATE.time, data_STATE.GlobalState};
        data["STATE_TIME"] = {data_STATE.time, float(data_STATE.time) / 1000};
    }

    riseData(data);
}

template<typename T>
bool UDPDataGenerator::findPacket(uint8_t flag, T* retval) {
    size_t size = sizeof(T);
    int left = mBuffer.indexOf(char(flag)), right;
    while(left != - 1) {
        right = left;

        while((right - left - 1) < (int(size) + 1) && (right != - 1))
            right = mBuffer.indexOf(char(flag), right + 1);

        if((right - left - 1) == int(size) + 1) {
            uint8_t* data = (uint8_t*)mBuffer.data() + left + 2;
            uint8_t packet_hash = mBuffer[left + 1];
            uint8_t _hash = hash(data, size);

            if(_hash == packet_hash) {
                memcpy(retval, data, size);
                mBuffer.remove(left, right);
                return true;
            }
        }
        left = mBuffer.indexOf(char(flag), left + 1);
    }
    return false;
}

void UDPDataGenerator::processDatagram() {
    QByteArray tmp;
    while(mSocket.hasPendingDatagrams()) {
        tmp.resize(mSocket.pendingDatagramSize());
        mSocket.readDatagram(tmp.data(), tmp.size());
        mBuffer.append(tmp);
    }
    tryParse();
}



UDPDataGenerator::~UDPDataGenerator() { }
