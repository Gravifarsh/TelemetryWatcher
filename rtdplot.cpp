#include "rtdplot.h"


RTDPlot::RTDPlot(QWidget *parent) : QWidget(parent),
    mPlot(new QCustomPlot),
    mXOffset(1),
    mYOffset(1),
    mValRangeLastOnly(false),
    mKeyRangeLastOnly(true),
    mHLayout(new QHBoxLayout),
    mVLayout(new QVBoxLayout),
    mBtnVal(new QRadioButton("Only current vals")),
    mBtnKey(new QRadioButton("All keys"))
{
    mPlot->yAxis2->setTickLabels(false);
    connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)),
            mPlot->yAxis, SLOT(setRange(QCPRange)));
    mPlot->yAxis2->setVisible(true);
    mPlot->yAxis2->setPadding(70);

    mBtnKey->setAutoExclusive(false);
    mBtnVal->setAutoExclusive(false);

    mHLayout->setMargin(0);
    mHLayout->addWidget(mBtnVal);
    mHLayout->addWidget(mBtnKey);

    mVLayout->setMargin(0);
    mVLayout->addWidget(mPlot);
    mVLayout->addLayout(mHLayout);

    setLayout(mVLayout);
    setMinimumSize(QSize(150, 100));

    connect(mBtnVal, SIGNAL(clicked(bool)),
            this, SLOT(setValRangeLastOnly(bool)));

    connect(mBtnKey, SIGNAL(clicked(bool)),
            this, SLOT(setKeyRangeLastOnly(bool)));
}

RTDPlot::~RTDPlot(){
    delete mPlot;
    delete mBtnKey;
    delete mBtnVal;
    delete mHLayout;
    delete mVLayout;
}

RTDGraph* RTDPlot::addGraph(const QString &name)
{
    RTDGraph *add = new RTDGraph(mPlot);
    mGraphs[name] = add;
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

void RTDPlot::updateRange(bool valueAxis, bool lastOnly){
    QCPRange range;
    bool found = false;

    for(auto graph: mGraphs.values()){
        range = graph->getRange(valueAxis, lastOnly, found);
        if(found) break;
    }

    double offset = valueAxis ? mYOffset : mXOffset;
    if(!found){
        range = QCPRange(-offset, offset);
        range = QCPRange::validRange(range) ? range : QCPRange(-1, 1);
    }
    else {
        QCPRange tmpRange;
        for(auto graph: mGraphs.values()){
            tmpRange = graph->getRange(valueAxis, lastOnly, found);
            if(found) range.expand(tmpRange);
        }

        range.upper += offset;
        range.lower -= offset;
    }

    if(valueAxis) mPlot->yAxis2->setRange(range);
    else mPlot->xAxis->setRange(range);
}

void RTDPlot::addData(const QMap<QString, QPointF> &data){
    bool changed = false;

    for(auto name: data.keys())
        if(mGraphs.contains(name)){
            changed = true;
            mGraphs[name]->addData(data[name]);
        }

    if(changed){
        updateRange(true, mValRangeLastOnly);
        updateRange(false, mKeyRangeLastOnly);
        mPlot->replot();
    }
}

void RTDPlot::setKeyRangeLastOnly(bool keyRangeLastOnly){
    mKeyRangeLastOnly = !keyRangeLastOnly;
    updateRange(false, mKeyRangeLastOnly);
    mPlot->replot();
}

void RTDPlot::setValRangeLastOnly(bool valRangeLastOnly){
    mValRangeLastOnly = valRangeLastOnly;
    updateRange(true, mValRangeLastOnly);
    mPlot->replot();
}

void RTDPlot::setXOffset(double offset){
    mXOffset = offset;
    updateRanges();
}

void RTDPlot::setYOffset(double offset){
    mYOffset = offset;
    updateRanges();
}

void RTDPlot::updateRanges(){
    updateRange(true, mValRangeLastOnly);
    updateRange(false, mKeyRangeLastOnly);
    mPlot->replot();
}

void RTDPlot::addGenerator(DataGenerator *gen){
    connect(gen, SIGNAL(riseData(QMap<QString, QPointF>)),
            this, SLOT(addData(QMap<QString, QPointF>)));
}
