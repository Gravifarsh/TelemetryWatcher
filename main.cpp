#include "mainwindow.h"
#include <QApplication>

#include "rtdplot.h"
#include "labelshower.h"
#include "datagenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UDPDataGenerator gen;

    RTDPlot temp;
    temp.setXLabel("Время, с");
    temp.setYLabel("Температура, С");
    temp.setXOffset(5);
    temp.setYOffset(5);
    temp.addGraph("BMP_1_temp");
    temp.addGraph("BMP_2_temp");
    temp.graph("BMP_1_temp")->setPen(QPen(QColor(234, 89, 123)));
    temp.graph("BMP_2_temp")->setPen(QPen(QColor(34, 189, 123)));

    temp.addGenerator(&gen);

    RTDPlot press;
    press.setXLabel("Время, с");
    press.setYLabel("Давление, Па");
    press.setXOffset(5);
    press.setYOffset(1000);
    press.addGraph("BMP_1_press");
    press.addGraph("BMP_2_press");
    press.graph("BMP_1_press")->setPen(QPen(QColor(78, 213, 98)));
    press.graph("BMP_2_press")->setPen(QPen(QColor(142, 67, 23)));

    press.addGenerator(&gen);

    RTDPlot height;
    height.setXLabel("Время, с");
    height.setYLabel("Высота, м");
    height.setXOffset(5);
    height.setYOffset(100);
    height.addGraph("GPS_h");
    height.addGraph("BMP_1_height");
    height.addGraph("BMP_2_height");

    height.addGenerator(&gen);

    RTDPlot accel;
    accel.setXLabel("Время, с");
    accel.setYLabel("Ускорение");
    accel.setXOffset(5);
    accel.setYOffset(10);
    accel.addGraph("MPU_1_accelX");
    accel.addGraph("MPU_1_accelY");
    accel.addGraph("MPU_1_accelZ");
    accel.addGraph("MPU_2_accelX");
    accel.addGraph("MPU_2_accelY");
    accel.addGraph("MPU_2_accelZ");

    accel.addGenerator(&gen);

    RTDPlot gyro;
    gyro.setXLabel("Время, с");
    gyro.setYLabel("Скорость вращения");
    gyro.setXOffset(5);
    gyro.setYOffset(10);
    gyro.addGraph("MPU_1_gyroX");
    gyro.addGraph("MPU_1_gyroY");
    gyro.addGraph("MPU_1_gyroZ");
    gyro.addGraph("MPU_2_gyroX");
    gyro.addGraph("MPU_2_gyroY");
    gyro.addGraph("MPU_2_gyroZ");

    gyro.addGenerator(&gen);

    RTDPlot lidar;
    lidar.addGraph("LIDAR_dist");

    lidar.addGenerator(&gen);

    RTDPlot compass;
    compass.setXOffset(5);
    compass.setYOffset(10);
    compass.addGraph("MPU_1_compassX");
    compass.addGraph("MPU_1_compassY");
    compass.addGraph("MPU_1_compassZ");
    compass.addGraph("MPU_2_compassX");
    compass.addGraph("MPU_2_compassY");
    compass.addGraph("MPU_2_compassZ");

    compass.addGenerator(&gen);

    RTDPlot tsl;
    tsl.addGraph("TSL_ch0");
    tsl.addGraph("TSL_ch1");

    tsl.addGenerator(&gen);

    LabelShower shower;
    shower.addLabel("STATE_BMP_1", "BMP_1: ");
    shower.addLabel("STATE_BMP_2", "BMP_2: ");
    shower.addLabel("STATE_SD", "SD: ");
    shower.addLabel("STATE_GPS", "GPS: ");
    shower.addLabel("STATE_MPU_1", "MPU_1: ");
    shower.addLabel("STATE_MPU_2", "MPU_2: ");
    shower.addLabel("STATE_GLOBAL", "GLOBAL: ");
    shower.addLabel("STATE_TIME", "TIME: ");

    shower.addGenerator(&gen);

    QWidget wgt;

    QHBoxLayout up;
    up.addWidget(&shower, 1);

    up.addWidget(&temp, 2);
    up.addWidget(&press, 2);
    up.addWidget(&height, 3);
    up.addWidget(&tsl, 2);

    QHBoxLayout down;
    down.addWidget(&accel, 3);
    down.addWidget(&gyro, 3);
    down.addWidget(&lidar, 2);
    down.addWidget(&compass, 1);

    QVBoxLayout layout;
    layout.addLayout(&up);
    layout.addLayout(&down);

    QPushButton erase("Очистить графики");
    layout.addWidget(&erase);

    QObject::connect(&erase, SIGNAL(pressed()), &temp, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &press, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &height, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &accel, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &gyro, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &compass, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &lidar, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &tsl, SLOT(clearData()));

    wgt.setLayout(&layout);
    wgt.resize(500, 500);
    wgt.show();

    return a.exec();
}
