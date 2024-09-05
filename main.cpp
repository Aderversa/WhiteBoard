#include "WhiteBoard.h"
#include "ItemLayer/BaseGraphicsItem.h"
#include "ItemLayer/ControlGroupObserver.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

int main(int argc, char *argv[])
{
    using namespace ADEV;
    QApplication a(argc, argv);
    DotPolygonItem item(Qt::red, 3, 1);
    DotPolygonItem item1(Qt::red, 3, 1);
    DotPolygonItem item2(Qt::red, 3, 1);
    DotPolygonItem item3(Qt::red, 3, 1);
    DotPolygonItem item4(Qt::red, 3, 1);
    DotPolygonItem item5(Qt::red, 3, 1);
    qDebug() << "0";
    RectangleItem myItem(QPointF(100, 100), QPointF(200, 200), item);
    qDebug() << "1";
    QGraphicsScene scene;
    scene.addItem(&myItem);
    scene.addItem(&item);
    scene.addItem(&item1);
    scene.addItem(&item2);
    scene.addItem(&item3);
    scene.addItem(&item4);
    scene.addItem(&item5);
    qDebug() << "2";
    scene.setSceneRect(0, 0, 1024, 512);
    qDebug() << "3";
    item.setPos(100, 100);
    QGraphicsView view(&scene);
    qDebug() << "4";
    view.show();
    return a.exec();
}
