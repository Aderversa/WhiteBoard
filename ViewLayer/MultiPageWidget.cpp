#include "MultiPageWidget.h"
#include "MultiPageLayout.h"

#include <QGraphicsView>
#include <QWheelEvent>

namespace ADEV {

MultiPageWidget::MultiPageWidget(QWidget* parent)
    : QWidget(parent)
    , pageLayout(new MultiPageLayout(this))
{
}

MultiPageWidget::~MultiPageWidget()
{
}

void MultiPageWidget::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->angleDelta().y() < 0)
        {
            pageLayout->setZoomFactor(qMax(0.5, pageLayout->zoomFactor() - 0.05), event->position().toPoint());
        }
        else
        {
            pageLayout->setZoomFactor(qMin(2.0, pageLayout->zoomFactor() + 0.05), event->position().toPoint());
        }
    }
    else
    {
        if (event->angleDelta().y() < 0)
        {
            pageLayout->moveY(-40);
        }
        else
        {
            pageLayout->moveY(40);
        }
    }
    update();
}

void MultiPageWidget::addWidget(QWidget* w)
{
    w->setParent(this);
    pageLayout->addWidget(w);
}



} // ADEV
