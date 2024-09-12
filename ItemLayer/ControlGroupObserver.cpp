#include "ControlGroupObserver.h"
#include <QGraphicsScene>

namespace ADEV {
// ControlGroupObserver
ControlGroupObserver::ControlGroupObserver(ItemShaper *itemShaper)
    : m_itemShaper(itemShaper)
{}

ControlGroupObserver::~ControlGroupObserver()
{}

void ControlGroupObserver::setControlGroup(ControlPointGroup *controlGroup)
{
    m_controlGroup = controlGroup;
    formItem(controlGroup->boundingRect(), 0.0);
    connect(m_controlGroup, &ControlPointGroup::rectInfo,
            this, &ControlGroupObserver::formItem);
    connect(m_controlGroup, &ControlPointGroup::needToDestroy,
            this, &ControlGroupObserver::handleControlPointDestroy);
}

ItemShaper* ControlGroupObserver::itemShaper() const
{
    return m_itemShaper;
}

void ControlGroupObserver::handleControlPointDestroy()
{
    if (!m_controlGroup.isNull()) {
        delete m_controlGroup.data();
    }
    emit needToDestroy();
}
// end of ControlGroupObserver
// ----------------------------------------------------------------------
// ControlRectangleObserver
ControlRectangleObserver::ControlRectangleObserver(ItemShaper* itemShaper)
    : ControlGroupObserver(itemShaper)
{
}

void ControlRectangleObserver::formItem(QRectF rect, qreal angle)
{
    Q_UNUSED(angle);
    qreal w = itemShaper()->strokeWidth();
    QPainterPath left   = itemShaper()->lineToStroke(QLineF(rect.bottomLeft(), rect.topLeft()), w);
    QPainterPath top    = itemShaper()->lineToStroke(QLineF(rect.topRight(), rect.topLeft()), w);
    QPainterPath right  = itemShaper()->lineToStroke(QLineF(rect.topRight(), rect.bottomRight()), w);
    QPainterPath bottom = itemShaper()->lineToStroke(QLineF(rect.bottomLeft(), rect.bottomRight()), w);
    QPainterPath ret;
    ret = ret.united(left);
    ret = ret.united(top);
    ret = ret.united(right);
    ret = ret.united(bottom);
    itemShaper()->setStrokePath(ret);
}
// end of ControlRectangleObserver
// ----------------------------------------------------------------------
// ControlEllipseObserver
ControlEllipseObserver::ControlEllipseObserver(ItemShaper* itemShaper)
    : ControlGroupObserver(itemShaper)
{
}

void ControlEllipseObserver::formItem(QRectF rect, qreal angle)
{
    qreal left = rect.left() + itemShaper()->strokeWidth();
    qreal right = rect.right() - itemShaper()->strokeWidth();
    qreal top = rect.top() + itemShaper()->strokeWidth();
    qreal bottom = rect.bottom() - itemShaper()->strokeWidth();
    QPainterPath outerEllipse;
    outerEllipse.addEllipse(rect);
    QPainterPath innerEllipse;
    innerEllipse.addEllipse(QRectF(QPointF(left, top), QPointF(right, bottom)));
    itemShaper()->setStrokePath(outerEllipse.subtracted(innerEllipse));
}
// end of ControlEllipseObserver
// ----------------------------------------------------------------------
// ControlCurveObserver
ControlCurveObserver::ControlCurveObserver(ItemShaper* itemShaper)
    : ControlGroupObserver(itemShaper)
{
}

void ControlCurveObserver::addPointToCurve(QPointF point)
{
    qreal w = itemShaper()->strokeWidth();
    QLineF line;
    if (m_isTheFirstPoint){
        line.setP1(point);
        m_isTheFirstPoint = false;
    }
    else {
        line.setP1(m_lastPoint);
    }
    line.setP2(point);
    itemShaper()->setStrokePath(itemShaper()->strokePath().united(itemShaper()->lineToStroke(line, w)));
    m_lastPoint = point;
}

void ControlCurveObserver::formItem(QRectF rect, qreal angle)
{
    Q_UNUSED(angle);
    // 传进来的rect都被当作一个点，这个点就是rect的左上点来处理
    addPointToCurve(rect.topLeft());
}
// end of ControlCurveObserver
// ----------------------------------------------------------------------
// ControlLineObserver
ControlLineObserver::ControlLineObserver(ItemShaper* itemShaper)
    : ControlCurveObserver(itemShaper)
{}

void ControlLineObserver::addPointToCurve(QPointF point)
{
    QPainterPath path;
    if (m_isTheFirstPoint)
    {
        m_lastPoint = point;
        path = itemShaper()->lineToStroke(QLineF(point, point), itemShaper()->strokeWidth());
        m_isTheFirstPoint = false;
    }
    else
    {
        // m_lastPoint作为初始点不需要更新
        path = itemShaper()->lineToStroke(QLineF(m_lastPoint, point), itemShaper()->strokeWidth());
    }
    itemShaper()->setStrokePath(path);
}
// end of ControlCurveObserver
// ----------------------------------------------------------------------
} // end of namespace ADEV
