#ifndef Q3DVIEWER_H
#define Q3DVIEWER_H

#include <QtCore>
#include <QLayout>
#include <QWidget>
#include <Qt3DCore>
#include <Qt3DLogic>
#include <Qt3DExtras>

#include "datagenerator.h"

class Q3DViewer : public QWidget
{
    Q_OBJECT
private:
    Qt3DRender::QCamera *m_cameraEntity;

    Qt3DCore::QEntity *m_lightEntity;
    Qt3DRender::QPointLight *m_light;
    Qt3DCore::QTransform *m_lightTransform;

    Qt3DCore::QEntity* m_cylinderEntity;
    Qt3DExtras::QCylinderMesh *m_cylinder;
    Qt3DCore::QTransform *m_cylinderTransform;
    Qt3DExtras::QPhongMaterial *m_cylinderMaterial;

    Qt3DCore::QEntity *m_rootEntity;

    QHBoxLayout *hLayout;

    Qt3DExtras::Qt3DWindow *m_view;

    QWidget *m_container;
public:
    Q3DViewer(QWidget* parent=0);

    void addGenerator(DataGenerator *gen);

    ~Q3DViewer();
public slots:
    void clearData();
    void addData(const QMap<QString, QPointF> &data);
};

#endif // 3DVIEWER_H
