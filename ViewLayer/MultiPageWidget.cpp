#include "MultiPageWidget.h"

#include <QGraphicsView>
#include <QWheelEvent>

namespace ADEV {


MultiPageWidget::MultiPageWidget(QWidget *parent)
    : QWidget{parent}
    , m_pageLayout(new MultiPageLayout(QMargins(10,10,10,10),10,this))
{}

void MultiPageWidget::addScene(WhiteBoardScene *scene)
{
    QGraphicsView* v = new QGraphicsView(scene);
    m_pageLayout->addWidget(v);
}

void MultiPageWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        qDebug() << "zoom";
        qreal factor = m_pageLayout->zoomFactor();
        if (event->angleDelta().y() < 0)
        {
            factor = qMax(0.5, factor - 0.05);
        }
        else
        {
            factor = qMin(2.0, factor + 0.05);
        }
        m_pageLayout->setZoomFactor(factor);
    }
    else if (event->modifiers() == Qt::NoModifier)
    {
        qDebug() << "up down move";
        if (event->angleDelta().y() < 0)
        {
            m_pageLayout->moveY(-40);
        }
        else
        {
            m_pageLayout->moveY(40);
        }
    }
    update();
}


} // ADEV
