#include "labelshower.h"

LabelShower::LabelShower(QWidget *parent) :
    QWidget(parent),
    mLayout(new QVBoxLayout(this)){
    this->setLayout(mLayout);
}

void LabelShower::addLabel(QString name, QString string){
    strings[name] = string;
    labels[name] = new QLabel(this);
    mLayout->addWidget(labels[name]);
    labels[name]->setText(strings[name]);
}

void LabelShower::addData(const QMap<QString, QPointF> &data) {
    for(auto i = data.begin(); i != data.end(); i++)
        if(strings.contains(i.key()))
            labels[i.key()]->setText(strings[i.key()] +
                    QString::number(i.value().y()));
}

void LabelShower::addGenerator(DataGenerator *gen){
    connect(gen, SIGNAL(riseData(QMap<QString, QPointF>)),
            this, SLOT(addData(QMap<QString, QPointF>)));
}

void LabelShower::clearData() {
    for(auto it = strings.begin(); it != strings.end(); it++)
        labels[it.key()]->setText(it.value());
}

LabelShower::~LabelShower() {
    for(auto i = labels.begin(); i != labels.end(); i++)
        delete i.value();
}
