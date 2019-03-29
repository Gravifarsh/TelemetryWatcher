#include "mainwindow.h"
#include <QApplication>

#include "rtdplot.h"
#include "datagenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RTDPlot plt1;
    plt1.setXOffset(1000);
    plt1.setYOffset(10);
    plt1.addGraph("First");
    plt1.addGraph("Second");

    RTDPlot plt2;
    plt2.setXOffset(1000);
    plt2.setYOffset(10);
    plt2.addGraph("Third");

    CSVDataGenerator gen;
    gen.selectFile("data.csv");

    plt1.addGenerator(&gen);
    plt2.addGenerator(&gen);

    gen.startGeneration();

    QWidget wgt;
    QHBoxLayout layout;

    layout.addWidget(&plt1);
    layout.addWidget(&plt2);

    wgt.setLayout(&layout);
    wgt.resize(500, 500);
    wgt.show();

    return a.exec();
}
