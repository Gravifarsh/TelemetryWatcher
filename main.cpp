#include "mainwindow.h"
#include <QApplication>

#include "rtdplot.h"
#include "q3dviewer.h"
#include "labelshower.h"
#include "datagenerator.h"

#include <Qt3DCore>
#include <Qt3DExtras>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UDPDataGenerator gen;

    QPen bmp_1_pen = QPen(QColor(234, 89, 123));
    QPen bmp_2_pen = QPen(QColor(34, 189, 123));

    QPen tsl_1_pen = bmp_1_pen;
    QPen tsl_2_pen = bmp_2_pen;

    QPen mpu_1_pen[] = {QPen(QColor(255, 0, 0)),
                        QPen(QColor(0, 255, 0)),
                        QPen(QColor(0, 0, 255))};
    QPen mpu_2_pen[] = {QPen(QColor(255, 255, 0)),
                        QPen(QColor(0, 255, 255)),
                        QPen(QColor(255, 0, 255))};



    RTDPlot temp;
    temp.setXLabel("Время, с");
    temp.setYLabel("Температура, С");
    temp.setXOffset(5);
    temp.setYOffset(5);
    temp.addGraph("BMP_1_temp");
    temp.addGraph("BMP_2_temp");
    temp.graph("BMP_1_temp")->setPen(bmp_1_pen);
    temp.graph("BMP_2_temp")->setPen(bmp_2_pen);

    temp.addGenerator(&gen);

    RTDPlot press;
    press.setXLabel("Время, с");
    press.setYLabel("Давление, Па");
    press.setXOffset(5);
    press.setYOffset(1000);
    press.addGraph("BMP_1_press");
    press.addGraph("BMP_2_press");
    press.graph("BMP_1_press")->setPen(bmp_1_pen);
    press.graph("BMP_2_press")->setPen(bmp_2_pen);

    press.addGenerator(&gen);

    RTDPlot height;
    height.setXLabel("Время, с");
    height.setYLabel("Высота, м");
    height.setXOffset(5);
    height.setYOffset(100);
    height.addGraph("GPS_h");
    height.addGraph("BMP_1_height");
    height.addGraph("BMP_2_height");
    height.graph("BMP_1_height")->setPen(bmp_1_pen);
    height.graph("BMP_2_height")->setPen(bmp_2_pen);

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
    accel.graph("MPU_1_accelX")->setPen(mpu_1_pen[0]);
    accel.graph("MPU_1_accelY")->setPen(mpu_1_pen[1]);
    accel.graph("MPU_1_accelZ")->setPen(mpu_1_pen[2]);
    accel.graph("MPU_2_accelX")->setPen(mpu_2_pen[0]);
    accel.graph("MPU_2_accelY")->setPen(mpu_2_pen[1]);
    accel.graph("MPU_2_accelZ")->setPen(mpu_2_pen[2]);

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
    gyro.graph("MPU_1_gyroX")->setPen(mpu_1_pen[0]);
    gyro.graph("MPU_1_gyroY")->setPen(mpu_1_pen[1]);
    gyro.graph("MPU_1_gyroZ")->setPen(mpu_1_pen[2]);
    gyro.graph("MPU_2_gyroX")->setPen(mpu_2_pen[0]);
    gyro.graph("MPU_2_gyroY")->setPen(mpu_2_pen[1]);
    gyro.graph("MPU_2_gyroZ")->setPen(mpu_2_pen[2]);

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
    compass.graph("MPU_1_compassX")->setPen(mpu_1_pen[0]);
    compass.graph("MPU_1_compassY")->setPen(mpu_1_pen[1]);
    compass.graph("MPU_1_compassZ")->setPen(mpu_1_pen[2]);
    compass.graph("MPU_2_compassX")->setPen(mpu_2_pen[0]);
    compass.graph("MPU_2_compassY")->setPen(mpu_2_pen[1]);
    compass.graph("MPU_2_compassZ")->setPen(mpu_2_pen[2]);

    compass.addGenerator(&gen);

    RTDPlot tsl;
    tsl.addGraph("TSL_ch0");
    tsl.addGraph("TSL_ch1");
    tsl.setXLabel("Время, с");
    tsl.setYLabel("Освещённость");
    tsl.graph("TSL_ch0")->setPen(tsl_1_pen);
    tsl.graph("TSL_ch1")->setPen(tsl_2_pen);

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

    Q3DViewer viewer;
    viewer.addGenerator(&gen);

    QWidget wgt;

    QHBoxLayout up;
    up.addWidget(&shower, 1);

    up.addWidget(&temp, 2);
    up.addWidget(&press, 2);
    up.addWidget(&height, 4);
    up.addWidget(&tsl, 3);

    QHBoxLayout down;
    down.addWidget(&viewer, 2);

    down.addWidget(&accel, 2);
    down.addWidget(&gyro, 2);
    down.addWidget(&lidar, 3);
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
    QObject::connect(&erase, SIGNAL(pressed()), &viewer, SLOT(clearData()));
    QObject::connect(&erase, SIGNAL(pressed()), &shower, SLOT(clearData()));

    wgt.setLayout(&layout);
    wgt.resize(500, 500);
    wgt.show();

    return a.exec();
}
