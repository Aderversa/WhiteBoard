#include "WhiteBoard.h"
#include "ItemLayer/ControlPointGroup.h"
#include "ItemLayer/BaseGraphicsItem.h"
#include "ItemLayer/ControlGroupObserver.h"
#include "SceneLayer/BackgroundItem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <iostream>

// EightWayMovementGroup, BaseGraphicsItem, ControlRectangleObserver的集成测试
int testForBase(int argc, char *argv[])
{
    using namespace ADEV;
    QApplication a(argc, argv);
    BaseGraphicsItem baseItem(4, QBrush(Qt::red));
    ControlRectangleObserver* rectangleObserver = new ControlRectangleObserver(&baseItem);
    QObject::connect(rectangleObserver, &ADEV::ControlRectangleObserver::needToDestroy, [=](){
        qDebug() << "Destroy ControlRectangleObserver.";
    });
    // 模拟鼠标事件，本来是产出一个
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    rectangleObserver->formItem(rect, 0.0);
    // 鼠标事件结束，后续可以使用EightWayMovementGroup进行调整
    EightWayMovementGroup eightWayMovement(rect);
    rectangleObserver->setControlGroup(&eightWayMovement);

    // 集成到Scene和View中看效果
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 1024, 512);
    scene.addItem(&baseItem);
    scene.addItem(&eightWayMovement);
    QGraphicsView view(&scene);
    view.show();
    return a.exec();
}

int testForCurve(int argc, char* argv[])
{
    using namespace ADEV;
    QApplication a(argc, argv);
    BaseGraphicsItem baseItem(3, QBrush(Qt::black));
    ControlCurveObserver controlCurve(&baseItem);
    controlCurve.addPointToCurve(QPointF(100, 100));
    controlCurve.addPointToCurve(QPointF(101, 101));
    controlCurve.addPointToCurve(QPointF(100, 101));
    controlCurve.addPointToCurve(QPointF(102, 110));

    // 集成到Scene和View中看效果
    QGraphicsScene scene;
    scene.setBackgroundBrush(QBrush(Qt::gray));
    scene.setSceneRect(0, 0, 1024, 512);
    scene.addItem(&baseItem);
    QGraphicsView view(&scene);
    view.show();
    return a.exec();
}

int main(int argc, char** argv)
{
    using namespace ADEV;
    QApplication a(argc, argv);
    BaseGraphicsItem baseItem(3, QBrush(Qt::black));
    ControlRectangleObserver* rectangleObserver = new ControlRectangleObserver(&baseItem);
    QObject::connect(rectangleObserver, &ADEV::ControlRectangleObserver::needToDestroy, [=](){
        qDebug() << "Destroy ControlRectangleObserver.";
    });
    // 模拟鼠标事件，本来是产出一个
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    rectangleObserver->formItem(rect, 0.0);
    // 鼠标事件结束，后续可以使用EightWayMovementGroup进行调整
    EightWayMovementGroup eightWayMovement(rect);
    rectangleObserver->setControlGroup(&eightWayMovement);

    BackgroundImageItem backgroundItem(QImage(QString(":/BackgroundImages/3.png")));
    /*
    QPainterPath path;
    for (int i = 1; i <= 10; ++i)
    {
        path.moveTo(0, i * 100);
        path.lineTo(500, i * 100);
    }

    BackgroundPathItem backgroundItem(path);
    backgroundItem.setColor(Qt::gray);
    */

    // 集成到Scene和View中看效果
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 1024, 512);
    scene.addItem(&baseItem);
    scene.addItem(&backgroundItem);
    scene.addItem(&eightWayMovement);
    QGraphicsView view(&scene);
    view.show();
    return a.exec();
}
