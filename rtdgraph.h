#ifndef RTDGRAPH_H
#define RTDGRAPH_H

#include <QObject>

#include "qcustomplot.h"
#include "axistag.h"


class RTDGraph : public QObject
{
    Q_OBJECT

private:
    QCPGraph    *mGraph;
    QCPGraph    *dGraph;
    AxisTag     *mTag;
public:
    explicit RTDGraph(QObject *parentPlot = nullptr);

    void addData(const QPointF &point);

    QCPRange getRangeX(double offset = 0);
    QCPRange getRangeY(double offset = 0);

    void setPen(QPen pen);

    bool containsValues();

signals:

public slots:
};

#endif // RTDGRAPH_H
