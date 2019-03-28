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
    mTimer->start();
}

void CSVDataGenerator::selectFile(const QString &filename){
    if(mFile) delete mFile;
    if(mTextStream) delete mTextStream;

    mFile = new QFile(filename);

    if(!mFile->open(QFile::ReadOnly)){
        qDebug() << "IO ERR";
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
