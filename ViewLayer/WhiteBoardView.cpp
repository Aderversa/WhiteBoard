#include "ViewLayer/WhiteBoardView.h"
#include "SceneLayer/WhiteBoardScene.h"

#include <QResizeEvent>

namespace ADEV
{

WhiteBoardView::WhiteBoardView(WhiteBoardScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void WhiteBoardView::resizeEvent(QResizeEvent *event)
{
    WhiteBoardScene* m_scene = dynamic_cast<WhiteBoardScene*>(scene());
    if (m_scene)
    {
        m_scene->resize(event->size().toSizeF());
    }
}

} // ADEV
