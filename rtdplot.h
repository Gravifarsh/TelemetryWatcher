#ifndef RTDPLOT_H
#define RTDPLOT_H

#include <QWidget>
#include <QList>

#include "qcustomplot.h"

#include "rtdgraph.h"
#include "datagenerator.h"

class RTDPlot : public QWidget
{
    Q_OBJECT

private:
    QCustomPlot* mPlot;
    QMap<QString, RTDGraph*> mGraphs;
    double mXOffset;
    double mYOffset;
    bool mValRangeLastOnly, mKeyRangeLastOnly;


    QHBoxLayout* mHLayout;
    QVBoxLayout* mVLayout;
    QRadioButton *mBtnVal;
    QRadioButton *mBtnKey;

    void updateRange(bool valueAxis, bool lastOnly);
    void updateRanges();
public:
    RTDPlot(QWidget *parent = 0);
    ~RTDPlot();

    RTDGraph* addGraph(const QString &name);
    RTDGraph* graph(const QString &name);

    void setXOffset(double offset);
    void setYOffset(double offset);
    void setXLabel(QString xlbl);
    void setYLabel(QString ylbl);

    QCustomPlot* plot();

    QWidget* createWidgetWithControls();

    void addGenerator(DataGenerator *gen);

signals:

public slots:
    void addData(const QMap<QString, QPointF> &data);
    void setValRangeLastOnly(bool valRangeLastOnly);
    void setKeyRangeLastOnly(bool keyRangeLastOnly);

    void clearData();
};

#endif // RTDPLOT_H
