#include "rtdgraph.h"

RTDGraph::RTDGraph(QObject *parentPlot) : QObject(parentPlot)
{
    auto mPlot = qobject_cast<QCustomPlot*>(parentPlot);

    mGraph = mPlot->addGraph(mPlot->xAxis, mPlot->yAxis2);
    dGraph = mPlot->addGraph(mPlot->xAxis, mPlot->yAxis2);
    mTag   = new AxisTag(mPlot->yAxis2);

    QPen pen = QPen(QColor(200, 25, 125));
    setPen(pen);

    mGraph->setLineStyle(QCPGraph::lsLine);
    dGraph->setLineStyle(QCPGraph::lsNone);

    mGraph->setScatterStyle(QCPScatterStyle::ssNone);
    dGraph->setScatterStyle(QCPScatterStyle::ssDisc);
}

RTDGraph::~RTDGraph(){
    delete mTag;
    delete mGraph;
    delete dGraph;
}

void RTDGraph::addData(const QPointF &point){
    mGraph->addData(point.x(), point.y());
    dGraph->data()->clear();
    dGraph->addData(point.x(), point.y());
    mTag->updatePosition(point.y());
    mTag->setText(QString::number(point.y(), 'f', 2));
}

void RTDGraph::setPen(QPen pen){
    mGraph->setPen(pen);
    dGraph->setPen(pen);
    mTag->setPen(pen);
}

bool RTDGraph::containsValues(){
    return dGraph->dataCount();
}

QCPRange RTDGraph::getRange(bool valueAxis, bool lastOnly, bool &found){
    QCPGraph *g = lastOnly ? dGraph : mGraph;
    return valueAxis ? g->getValueRange(found) : g->getKeyRange(found);
}

void RTDGraph::clear() {
    mGraph->data()->clear();
    dGraph->data()->clear();
    mTag->setText("No data");
}
