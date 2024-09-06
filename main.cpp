#include "WhiteBoard.h"
#include "ItemLayer/ControlPointGroup.h"
#include "ItemLayer/ControlPointItem.h"
#include "ItemLayer/BaseGraphicsItem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

int main(int argc, char *argv[])
{
    using namespace ADEV;
    QApplication a(argc, argv);
    EightWayMovementGroup item(QRectF(QPointF(100, 100), QPointF(200, 200)));
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 1024, 512);
    scene.addItem(&item);
    QGraphicsView view(&scene);
    view.show();
    return a.exec();
}
