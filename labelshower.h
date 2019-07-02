#ifndef LABELSHOWER_H
#define LABELSHOWER_H

#include <QMap>
#include <QString>
#include <QLabel>
#include <QLayout>

#include "datagenerator.h"

class LabelShower : public QWidget
{
    Q_OBJECT

private:
    QMap<QString, QLabel*> labels;
    QMap<QString, QString> strings;
    QLayout* mLayout;
public:
    LabelShower(QWidget* parent=0);

    void addLabel(QString name, QString string);
    void addGenerator(DataGenerator *gen);

    ~LabelShower();

public slots:
    void addData(const QMap<QString, QPointF> &data);
};

#endif // LABELSHOWER_H
