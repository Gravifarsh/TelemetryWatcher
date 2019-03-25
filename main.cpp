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

    plt.setOffset(10.0);

    QWidget wgt;

    QVBoxLayout layout;

    QRadioButton btnVal("Value");
    QRadioButton btnKey("Key");

    btnVal.setAutoExclusive(false);
    btnKey.setAutoExclusive(false);

    layout.addWidget(&plt);
    layout.addWidget(&btnVal);
    layout.addWidget(&btnKey);

    QObject::connect(&btnVal, SIGNAL(clicked(bool)),
                     &plt, SLOT(setValRangeLastOnly(bool)));

    QObject::connect(&btnKey, SIGNAL(clicked(bool)),
                     &plt, SLOT(setKeyRangeLastOnly(bool)));

    wgt.setLayout(&layout);

    wgt.resize(500, 500);
    wgt.show();

    return a.exec();
}
