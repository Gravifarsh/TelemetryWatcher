#include "mainwindow.h"
#include <QApplication>

#include "rtdplot.h"
#include "datagenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RTDPlot plt;
    plt.setOffset(2);
    plt.addGraph("First");
    plt.addGraph("Second");

    CSVDataGenerator gen;
    gen.selectFile("data.csv");

    plt.addGenerator(&gen);

    gen.startGeneration();

    QWidget *wgt = plt.createWidgetWithControls();
    wgt->resize(500, 500);
    wgt->show();

    return a.exec();
}
