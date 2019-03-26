#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QPointF>
#include <QDebug>
#include <QtMath>
#include <QFile>

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
    QStringList mFieldNames;
    QString mTimeFieldName;
    double mTime;

public:
    CSVDataGenerator(QObject *parent = 0);
    ~CSVDataGenerator();

    void selectFile(const QString &filename);
    void startGeneration();

public slots:
    void generationStep();
};

#endif // DATAGENERATOR_H
