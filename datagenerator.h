#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QPointF>
#include <QDebug>
#include <QtMath>
#include <QFile>
#include <QUdpSocket>

class DataGenerator : public QObject
{
    Q_OBJECT
public:
    DataGenerator(QObject *parent = 0) : QObject(parent) {}

signals:
    void riseData(QMap<QString, QPointF>);
};

class CSVDataGenerator : public DataGenerator
{
    Q_OBJECT

private:
    QTimer *mTimer;
    QTextStream  *mTextStream;
    QFile *mFile;
    QStringList mFieldNames;
    QString mTimeFieldName;
    double mTime;

public:
    explicit CSVDataGenerator(QObject *parent = 0);
    ~CSVDataGenerator();

    void selectFile(const QString &filename);
    void startGeneration();

public slots:
    void generationStep();
};

class UDPDataGenerator : public DataGenerator {
    Q_OBJECT
private:
    QByteArray mBuffer;
    QUdpSocket mSocket;

    template<typename T>
    bool findPacket(uint8_t flag, T* retval);
    void tryParse();
public:
    UDPDataGenerator(QObject* parent = 0);
    ~UDPDataGenerator();

public slots:
    void processDatagram();
};

typedef struct
{
    uint32_t time;
    float temperature;
    float pressure;
    float height;
} data_BMP280_t;

typedef struct
{
    uint32_t time;
    float accel[3];
    float compass[3];
    union
    {
    float gyro[3];
    float quaternion[4];
    };
} data_MPU9255_t;

typedef enum
{
    EMAP_STATE_START,
    EMAP_STATE_READY,
    EMAP_STATE_PAYLOAD,
    EMAP_STATE_FALLING
} EMAP_state_t;

typedef struct
{
    uint32_t time;
    EMAP_state_t GlobalState;
    uint8_t MPU9255_1;
    uint8_t BMP280_1;
    uint8_t MPU9255_2;
    uint8_t BMP280_2;
    uint8_t SD;
    uint8_t	nRF;
    uint8_t GPS;
} system_state_t;

typedef struct
{
    uint32_t time;
    float coords[3];
} data_GPS_t;

typedef struct
{
    uint32_t time;
    uint16_t ch0;
    uint16_t ch1;
    uint16_t lux;
} data_TSL_t;

typedef struct
{
    uint32_t time;
    uint32_t dist;
} data_LIDAR_t;

#endif // DATAGENERATOR_H
