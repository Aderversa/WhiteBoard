#include "WhiteBoard.h"
#include "ItemLayer/DotPolygonItem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    using namespace ADEV;
    QApplication a(argc, argv);
    DotPolygonItem item(Qt::red, 10, 0.5);
    QGraphicsScene scene;
    scene.addItem(&item);
    scene.setSceneRect(0, 0, 1024, 512);
    item.setPos(100, 100);
    QGraphicsView view(&scene);
    view.show();
    return a.exec();
}
