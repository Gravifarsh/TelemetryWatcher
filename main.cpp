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

    CSVDataGenerator gen;
    gen.selectFile("data.csv");
    gen.startGeneration();


    QObject::connect(&gen, SIGNAL(riseData(QMap<QString, QPointF>)),
                     &plt, SLOT(addData(QMap<QString, QPointF>)));

    plt.setOffset(2);

    QWidget *wgt = plt.createWidgetWithControls();
    wgt->resize(500, 500);
    wgt->show();

    return a.exec();
}
