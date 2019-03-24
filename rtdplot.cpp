#include "rtdplot.h"


RTDPlot::RTDPlot(QWidget *parent) : QWidget(parent),
    mPlot(0),
    mLayout(0)
{
    mPlot = new QCustomPlot;

    mPlot->yAxis->setTickLabels(false);
    connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)),
            mPlot->yAxis, SLOT(setRange(QCPRange)));
    mPlot->yAxis2->setVisible(true);
    mPlot->yAxis2->setPadding(50);


    mLayout = new QHBoxLayout;
    mLayout->addWidget(mPlot);
    setLayout(mLayout);
}

RTDGraph* RTDPlot::addGraph(const QString &name)
{
    RTDGraph *add = new RTDGraph(mPlot);
    mGraphs[name] = add;
    //lol
    //add->setPen(QPen(QColor(name[0].unicode(), name[1].unicode(), name[2].unicode()), 5));
    return add;
}

RTDGraph* RTDPlot::graph(const QString &name)
{
    if(mGraphs.contains(name))
        return mGraphs[name];
    return 0;
}

QCustomPlot* RTDPlot::plot(){
    return mPlot;
}

void RTDPlot::updateRangeX(){
    QCPRange rangeX;

    for(auto graph: mGraphs.values())
        if(graph->containsValues()){
            rangeX = graph->getRangeX(10);
            break;
        }

    for(auto graph: mGraphs.values())
        if(graph->containsValues())
            rangeX.expand(graph->getRangeX(10));

    mPlot->xAxis->setRange(rangeX);
}

void RTDPlot::updateRangeY(){
    QCPRange rangeY;

    for(auto graph: mGraphs.values())
        if(graph->containsValues()){
            rangeY = graph->getRangeY(10);
            break;
        }

    for(auto graph: mGraphs.values())
        if(graph->containsValues())
            rangeY.expand(graph->getRangeY(10));

    mPlot->yAxis2->setRange(rangeY);
}

void RTDPlot::addData(const QMap<QString, QPointF> &data){
    bool changed = false;

    for(auto name: data.keys())
        if(mGraphs.contains(name)){
            changed = true;
            mGraphs[name]->addData(data[name]);
        }

    if(changed){
        updateRangeX();
        updateRangeY();
        mPlot->replot();
    }
}

