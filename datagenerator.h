#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QPointF>
#include <QDebug>
#include <QtMath>

class RandomDataGenerator : public QObject
{
    Q_OBJECT

private:
    QTimer mTimer;
    double t;
public:
    RandomDataGenerator(QObject *parent = 0): QObject(parent)
    {
        connect(&mTimer, SIGNAL(timeout()), this, SLOT(generateData()));

        mTimer.start(40);

        qDebug() << "here\n";
    }

public slots:
    void generateData(){
        QMap<QString, QPointF> map;
        map["First"] = QPointF(t, 50 * (qSin(qAbs(t) / 10)) + 20);
        map["Second"] = QPointF(t, 50 * (qCos(qAbs(t) / 10)));
        emit riseData(map);
        t += 0.1;
    }

signals:
    void riseData(QMap<QString, QPointF>);
};

#endif // DATAGENERATOR_H
