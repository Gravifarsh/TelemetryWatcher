#ifndef RTDPLOT_H
#define RTDPLOT_H

#include <QWidget>
#include <QList>

#include "qcustomplot.h"

#include "rtdgraph.h"

class RTDPlot : public QWidget
{
    Q_OBJECT

private:
    QCustomPlot* mPlot;
    QHBoxLayout* mLayout;
    QMap<QString, RTDGraph*> mGraphs;
    double mOffset;
    bool mValRangeLastOnly, mKeyRangeLastOnly;

    void updateRange(bool valueAxis, bool lastOnly);
public:
    explicit RTDPlot(QWidget *parent = nullptr);

    RTDGraph* addGraph(const QString &name);
    RTDGraph* graph(const QString &name);

    void setOffset(double offset);

    QCustomPlot* plot();

signals:

public slots:
    void addData(const QMap<QString, QPointF> &data);
    void setValRangeLastOnly(bool valRangeLastOnly);
    void setKeyRangeLastOnly(bool keyRangeLastOnly);
};

#endif // RTDPLOT_H
