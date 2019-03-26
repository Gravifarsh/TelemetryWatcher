#include "datagenerator.h"

CSVDataGenerator::CSVDataGenerator(QObject *parent) : DataGenerator(parent),
    mTimer(0)
{
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(generationStep()));
    mTimer->setSingleShot(true);

    mTimeFieldName = "time";

    mTime = 0;
}

void CSVDataGenerator::generationStep(){
    bool atEnd = mTextStream->atEnd();

    if(atEnd) return;

    qDebug() << "Reading Line";
    QString string = mTextStream->readLine();

    qDebug() << "Splitting Line";
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
    mTimer->start();
}

void CSVDataGenerator::selectFile(const QString &filename){
    QFile *file = new QFile(filename);

    if(!file->open(QFile::ReadOnly)){
        qDebug() << "IO ERR";
        return;
    }

    mTextStream = new QTextStream(file);
    mFieldNames = mTextStream->readLine().split(",");
}

CSVDataGenerator::~CSVDataGenerator(){
    qDebug() << "Dying";
}
