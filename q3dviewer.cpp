#include "q3dviewer.h"

Q3DViewer::Q3DViewer(QWidget* parent) :
    QWidget(parent) {
    m_view = new Qt3DExtras::Qt3DWindow();
    m_view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    m_container = QWidget::createWindowContainer(m_view);
    QSize screenSize = m_view->screen()->size();
    m_container->setMinimumSize(QSize(200, 100));
    m_container->setMaximumSize(screenSize);

    hLayout = new QHBoxLayout(this);
    hLayout->addWidget(m_container);

    // Root entity
    m_rootEntity = new Qt3DCore::QEntity();

    // Camera
    m_cameraEntity = m_view->camera();

    m_cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    m_cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    m_cameraEntity->setUpVector(QVector3D(0, 1, 0));
    m_cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    m_lightEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_light = new Qt3DRender::QPointLight(m_lightEntity);
    m_light->setColor("white");
    m_light->setIntensity(1);
    m_lightEntity->addComponent(m_light);
    m_lightTransform = new Qt3DCore::QTransform(m_lightEntity);
    m_lightTransform->setTranslation(m_cameraEntity->position());
    m_lightEntity->addComponent(m_lightTransform);

    // For camera controls
    //Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    //camController->setCamera(cameraEntity);

    // Set root object of the scene
    m_view->setRootEntity(m_rootEntity);

    // Cylinder shape data
    m_cylinder = new Qt3DExtras::QCylinderMesh();
    m_cylinder->setRadius(1);
    m_cylinder->setLength(3);
    m_cylinder->setRings(100);
    m_cylinder->setSlices(20);

    // CylinderMesh Transform
    m_cylinderTransform = new Qt3DCore::QTransform();
    m_cylinderTransform->setScale(3.f);
    m_cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.f, 0.0f, 0.0f), 45.0f));
    m_cylinderTransform->setTranslation(QVector3D(0.f, 0.f, 0.f));

    m_cylinderMaterial = new Qt3DExtras::QPhongMaterial();
    m_cylinderMaterial->setDiffuse(QColor(QRgb(0x928327)));

    // Cylinder
    m_cylinderEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_cylinderEntity->addComponent(m_cylinder);
    m_cylinderEntity->addComponent(m_cylinderMaterial);
    m_cylinderEntity->addComponent(m_cylinderTransform);
}

Q3DViewer::~Q3DViewer() {

}

void Q3DViewer::addData(const QMap<QString, QPointF> &data) {
    if(data.contains("ISC_q_0") &&
       data.contains("ISC_q_1") &&
       data.contains("ISC_q_2") &&
       data.contains("ISC_q_3")) {
       m_cylinderTransform->setRotation(QQuaternion(
                                        data["ISC_q_0"].y(),
                                        data["ISC_q_2"].y(),
                                        data["ISC_q_3"].y(),
                                        data["ISC_q_1"].y()));
    }
}

void Q3DViewer::clearData() {
    m_cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.f, 0.0f, 0.0f), 45.0f));
}

void Q3DViewer::addGenerator(DataGenerator *gen){
    connect(gen, SIGNAL(riseData(QMap<QString, QPointF>)),
            this, SLOT(addData(QMap<QString, QPointF>)));
}
