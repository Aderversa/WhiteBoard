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
    if (m_controlGroup) {
        // 不能调用delete，因为你不知道这个指针指向的对象是否是new创建的
        // delete和new的不配对会发生编译错误
        m_controlGroup->~ControlPointGroup();
        m_controlGroup = nullptr;
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
} // end of namespace ADEV
