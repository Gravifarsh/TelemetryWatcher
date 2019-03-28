#include "rtdplot.h"


RTDPlot::RTDPlot(QWidget *parent) : QWidget(parent),
    mPlot(0),
    mLayout(0)
{
    mOffset = 1;

    mPlot = new QCustomPlot;

    mPlot->yAxis2->setTickLabels(false);
    connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)),
            mPlot->yAxis, SLOT(setRange(QCPRange)));
    mPlot->yAxis2->setVisible(true);
    mPlot->yAxis2->setPadding(70);

    mLayout = new QHBoxLayout;
    mLayout->setMargin(0);
    mLayout->addWidget(mPlot);
    setLayout(mLayout);
    this->setMinimumSize(QSize(150, 100));

    mValRangeLastOnly = false;
    mKeyRangeLastOnly = true;
}

RTDPlot::~RTDPlot(){
    delete mPlot;
    delete mLayout;
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

    if(!found){
        range = QCPRange(-mOffset, mOffset);
        range = QCPRange::validRange(range) ? range : QCPRange(-1, 1);
    }
    else {
        QCPRange tmpRange;
        for(auto graph: mGraphs.values()){
            tmpRange = graph->getRange(valueAxis, lastOnly, found);
            if(found) range.expand(tmpRange);
        }

        range.upper += mOffset;
        range.lower -= mOffset;
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

void RTDPlot::setOffset(double offset){
    mOffset = offset;
    updateRange(true, mValRangeLastOnly);
    updateRange(false, mKeyRangeLastOnly);
    mPlot->replot();
}

QWidget* RTDPlot::createWidgetWithControls(){
    QRadioButton *btnVal = new QRadioButton("Value");
    QRadioButton *btnKey = new QRadioButton("Key");

    btnVal->setAutoExclusive(false);
    btnKey->setAutoExclusive(false);

    QObject::connect(btnVal, SIGNAL(clicked(bool)),
                     this, SLOT(setValRangeLastOnly(bool)));

    QObject::connect(btnKey, SIGNAL(clicked(bool)),
                     this, SLOT(setKeyRangeLastOnly(bool)));

    QHBoxLayout *btnLayout = new QHBoxLayout;

    btnLayout->addWidget(btnVal);
    btnLayout->addWidget(btnKey);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(this);
    layout->addLayout(btnLayout);

    QWidget *wgt = new QWidget;

    wgt->setLayout(layout);

    return wgt;
}

void RTDPlot::addGenerator(DataGenerator *gen){
    connect(gen, SIGNAL(riseData(QMap<QString, QPointF>)),
            this, SLOT(addData(QMap<QString, QPointF>)));
}
