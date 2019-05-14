#include "mainwindow.h"
#include <QApplication>

#include "rtdplot.h"
#include "datagenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UDPDataGenerator gen;

    RTDPlot plt1;
    plt1.setXOffset(10);
    plt1.setYOffset(10);
    plt1.addGraph("Data");

    plt1.addGenerator(&gen);

    QWidget wgt;
    QHBoxLayout layout;

    layout.addWidget(&plt1);

    wgt.setLayout(&layout);
    wgt.resize(500, 500);
    wgt.show();

    return a.exec();
}
