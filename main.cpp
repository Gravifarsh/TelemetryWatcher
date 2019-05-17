#include "mainwindow.h"
#include <QApplication>

#include "rtdplot.h"
#include "datagenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UDPDataGenerator gen;

    RTDPlot temp;
    temp.setXOffset(10);
    temp.setYOffset(10);
    temp.addGraph("BMP_1_temp");
    temp.addGraph("BMP_2_temp");

    temp.addGenerator(&gen);

    RTDPlot press;
    press.setXOffset(10);
    press.setYOffset(10);
    press.addGraph("BMP_1_press");
    press.addGraph("BMP_2_press");

    press.addGenerator(&gen);

    RTDPlot height;
    height.setXOffset(10);
    height.setYOffset(10);
    height.addGraph("BMP_1_height");
    height.addGraph("BMP_2_height");

    height.addGenerator(&gen);

    RTDPlot accel;
    accel.setXOffset(10);
    accel.setYOffset(10);
    accel.addGraph("MPU_1_accelX");
    accel.addGraph("MPU_1_accelY");
    accel.addGraph("MPU_1_accelZ");
    accel.addGraph("MPU_2_accelX");
    accel.addGraph("MPU_2_accelY");
    accel.addGraph("MPU_2_accelZ");

    accel.addGenerator(&gen);

    RTDPlot gyro;
    gyro.setXOffset(10);
    gyro.setYOffset(10);
    gyro.addGraph("MPU_1_gyroX");
    gyro.addGraph("MPU_1_gyroY");
    gyro.addGraph("MPU_1_gyroZ");
    gyro.addGraph("MPU_2_gyroX");
    gyro.addGraph("MPU_2_gyroY");
    gyro.addGraph("MPU_2_gyroZ");

    gyro.addGenerator(&gen);

    RTDPlot compass;
    compass.setXOffset(10);
    compass.setYOffset(10);
    compass.addGraph("MPU_1_compassX");
    compass.addGraph("MPU_1_compassY");
    compass.addGraph("MPU_1_compassZ");
    compass.addGraph("MPU_2_compassX");
    compass.addGraph("MPU_2_compassY");
    compass.addGraph("MPU_2_compassZ");

    compass.addGenerator(&gen);

    QWidget wgt;

    QHBoxLayout bmp;
    bmp.addWidget(&temp);
    bmp.addWidget(&press);
    bmp.addWidget(&height);

    QHBoxLayout mpu;
    mpu.addWidget(&accel);
    mpu.addWidget(&gyro);
    mpu.addWidget(&compass);

    QVBoxLayout layout;
    layout.addLayout(&bmp);
    layout.addLayout(&mpu);

    wgt.setLayout(&layout);
    wgt.resize(500, 500);
    wgt.show();

    return a.exec();
}
