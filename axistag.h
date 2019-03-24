#ifndef AXISTAG_H
#define AXISTAG_H

#include <QObject>
#include "qcustomplot.h"

class AxisTag : public QObject
{
  Q_OBJECT

private:
  QCPAxis *mAxis;
  QPointer<QCPItemTracer> mDummyTracer;
  QPointer<QCPItemLine> mArrow;
  QPointer<QCPItemText> mLabel;

public:
  explicit AxisTag(QCPAxis *parentAxis);
  virtual ~AxisTag();

  void setPen(const QPen &pen);
  void setText(const QString &text);

  void updatePosition(double value);
};


#endif // AXISTAG_H
