#include "MultiPageWidget.h"
#include "MultiPageLayout.h"

#include <QGraphicsView>
#include <QWheelEvent>

namespace ADEV {

MultiPageWidget::MultiPageWidget(QWidget* parent)
    : QWidget(parent)
    , pageLayout(new MultiPageLayout(this))
{
    updateScrollBars();
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
            pageLayout->setZoomFactor(qMin(3.0, pageLayout->zoomFactor() + 0.05), event->position().toPoint());
        }
        updateScrollBars();
        vBar.setValue(qMax(vBar.minimumHeight(), qMin(vBar.maximumHeight(), vBar.value())));
        pageLayout->setY(vBar.value());
        hBar.setValue(qMax(hBar.minimumHeight(), qMin(hBar.maximumHeight(), hBar.value())));
        pageLayout->setX(hBar.value());
    }
    else
    {
        if (event->angleDelta().y() < 0)
        {
            //vBar.setValue(qMin(vBar.maximumHeight(), qMax(vBar.minimumHeight(), vBar.value() + vBar.pageStep())));
            vBar.setValue(vBar.value() + vBar.pageStep());
            pageLayout->setY(vBar.value());
        }
        else
        {
            //vBar.setValue(qMin(vBar.maximumHeight(), qMax(vBar.minimumHeight(), vBar.value() - vBar.pageStep())));
            vBar.setValue(vBar.value() - vBar.pageStep());
            pageLayout->setY(vBar.value());
        }
    }
    update();
}

void MultiPageWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateScrollBars();
}

void MultiPageWidget::addWidget(QWidget* w)
{
    w->setParent(this);
    pageLayout->addWidget(w);
    updateScrollBars();
}

void MultiPageWidget::updateScrollBars()
{
    const QSize p = this->size();
    const QSize v = pageLayout->sizeHint();
    hBar.setRange(0, v.width() - p.width());
    hBar.setPageStep(20);
    vBar.setRange(0, v.height() - p.height());
    vBar.setPageStep(40);
}



} // ADEV
