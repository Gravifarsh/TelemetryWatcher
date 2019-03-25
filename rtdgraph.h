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

    QCPRange getRange(bool valueAxis, bool lastOnly, bool &found);

    void setPen(QPen pen);

    bool containsValues();

signals:

public slots:
};

#endif // RTDGRAPH_H
