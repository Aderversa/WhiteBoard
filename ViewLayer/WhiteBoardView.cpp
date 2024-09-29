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
    horizontalScrollBar()->hide();
    verticalScrollBar()->hide();
}

void WhiteBoardView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    WhiteBoardScene* ws = dynamic_cast<WhiteBoardScene*>(scene());
    if (ws)
    {
        this->fitInView(ws->sceneRect(), Qt::KeepAspectRatio);
    }
}

} // ADEV
