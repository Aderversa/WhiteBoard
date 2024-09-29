#include "ViewLayer/WhiteBoardView.h"
#include "SceneLayer/WhiteBoardScene.h"

#include <QResizeEvent>
#include <QScrollBar>

namespace ADEV
{

WhiteBoardView::WhiteBoardView(WhiteBoardScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setSceneRect(scene->sceneRect());
}

void WhiteBoardView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    WhiteBoardScene* ws = dynamic_cast<WhiteBoardScene*>(scene());
    if (ws)
    {
        this->fitInView(ws->sceneRect(), Qt::KeepAspectRatio);
    }
}

} // ADEV
