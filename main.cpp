#include "mainwindow.h"
#include <QApplication>

#include "rtdplot.h"
#include "datagenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RTDPlot plt;
    plt.addGraph("First");
    plt.addGraph("Second");

    RandomDataGenerator gen;
    QObject::connect(&gen, SIGNAL(riseData(QMap<QString, QPointF>)),
                     &plt, SLOT(addData(QMap<QString, QPointF>)));

    plt.show();
    plt.resize(400, 400);

    return a.exec();
}
