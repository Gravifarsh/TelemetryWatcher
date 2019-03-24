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

    void updateRangeX();
    void updateRangeY();
public:
    explicit RTDPlot(QWidget *parent = nullptr);

    RTDGraph* addGraph(const QString &name);
    RTDGraph* graph(const QString &name);

    QCustomPlot* plot();

signals:

public slots:
    void addData(const QMap<QString, QPointF> &data);
};

#endif // RTDPLOT_H
