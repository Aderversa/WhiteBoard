#include "ControlPointGroup.h"
#include "ControlPointItem.h"

#include <QGraphicsScene>
#include <QVariant>
#include <QCursor>
#include <QPainter>

namespace ADEV {
// ControlPointGroup
ControlPointGroup::ControlPointGroup(QGraphicsItem *parent)
    : QGraphicsItem{parent}
{}
// end of ControlPointGroup
// ----------------------------------------------------------------------
// EightWayMovementGroup
    // 构造和析构
EightWayMovementGroup::EightWayMovementGroup(const QRectF& rect, QGraphicsItem* parent)
    : ControlPointGroup{parent}
{
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    m_leftMid     = new ControlPointItem((rect.topLeft() + rect.bottomLeft()) / 2, this);
    m_topLeft     = new ControlPointItem(rect.topLeft(), this);
    m_topMid      = new ControlPointItem((rect.topLeft() + rect.topRight()) / 2, this);
    m_topRight    = new ControlPointItem(rect.topRight(), this);
    m_rightMid    = new ControlPointItem((rect.topRight() + rect.bottomRight()) / 2, this);
    m_bottomRight = new ControlPointItem(rect.bottomRight(), this);
    m_bottomMid   = new ControlPointItem((rect.bottomLeft() + rect.bottomRight()) / 2, this);
    m_bottomLeft  = new ControlPointItem(rect.bottomLeft(), this);

    m_leftMid->setCursor(Qt::SizeHorCursor);
    m_topLeft->setCursor(Qt::SizeFDiagCursor);
    m_topMid->setCursor(Qt::SizeVerCursor);
    m_topRight->setCursor(Qt::SizeBDiagCursor);
    m_rightMid->setCursor(Qt::SizeHorCursor);
    m_bottomRight->setCursor(Qt::SizeFDiagCursor);
    m_bottomMid->setCursor(Qt::SizeVerCursor);
    m_bottomLeft->setCursor(Qt::SizeBDiagCursor);

#define ADEV_SET_FLAGS(point) \
    point->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);

    ADEV_SET_FLAGS(m_leftMid);
    ADEV_SET_FLAGS(m_topLeft);
    ADEV_SET_FLAGS(m_topMid);
    ADEV_SET_FLAGS(m_topRight);
    ADEV_SET_FLAGS(m_rightMid);
    ADEV_SET_FLAGS(m_bottomRight);
    ADEV_SET_FLAGS(m_bottomMid);
    ADEV_SET_FLAGS(m_bottomLeft);
#undef ADEV_SET_FLAGS

    connect(m_leftMid , &ControlPointItem::posMove, this, &EightWayMovementGroup::leftMidMove);
    connect(m_topLeft , &ControlPointItem::posMove, this, &EightWayMovementGroup::topLeftMove);
    connect(m_topMid  , &ControlPointItem::posMove, this, &EightWayMovementGroup::topMidMove);
    connect(m_topRight, &ControlPointItem::posMove, this, &EightWayMovementGroup::topRightMove);
    connect(m_rightMid, &ControlPointItem::posMove, this, &EightWayMovementGroup::rightMidMove);
    connect(m_bottomRight, &ControlPointItem::posMove, this, &EightWayMovementGroup::bottomRightMove);
    connect(m_bottomMid, &ControlPointItem::posMove, this, &EightWayMovementGroup::bottomMidMove);
    connect(m_bottomLeft, &ControlPointItem::posMove, this, &EightWayMovementGroup::bottomLeftMove);

}

EightWayMovementGroup::~EightWayMovementGroup()
{
    if (scene()) {
        QGraphicsScene* s = scene();
        scene()->removeItem(this);
        s->update();
    }
#define ADEV_DELETE(v) if (v) delete v;

    ADEV_DELETE(m_leftMid);
    ADEV_DELETE(m_topLeft);
    ADEV_DELETE(m_topMid);
    ADEV_DELETE(m_topRight);
    ADEV_DELETE(m_rightMid);
    ADEV_DELETE(m_bottomRight);
    ADEV_DELETE(m_bottomMid);
    ADEV_DELETE(m_bottomLeft);
#undef ADEV_DELETE
}
    // end of 构造和析构
    // ----------------------------------------------------------------------
    // 控制点移动逻辑处理的槽
void EightWayMovementGroup::leftMidMove(qreal dx, qreal dy)
{
    Q_UNUSED(dy);
    prepareGeometryChange();
    // 只移动左边的2个顶点，且只在x轴上移动，不能超过右边
    // 边上中点随着顶点的移动而调整
    if (m_leftMid->pos().x() + dx > m_rightMid->pos().x())
        dx = m_rightMid->pos().x() - m_leftMid->pos().x();
    m_topLeft->moveBy(dx, 0);
    m_bottomLeft->moveBy(dx, 0);
    updateControlGroup();
}

void EightWayMovementGroup::topLeftMove(qreal dx, qreal dy)
{
    prepareGeometryChange();
    if (m_topLeft->pos().x() + dx > m_rightMid->pos().x())
        dx = m_rightMid->pos().x() - m_topLeft->pos().x();
    if (m_topLeft->pos().y() + dy > m_bottomMid->pos().y())
        dy = m_bottomMid->pos().y() - m_topLeft->pos().y();
    m_bottomLeft->moveBy(dx, 0);
    m_topRight->moveBy(0, dy);
    m_topLeft->moveBy(dx, dy);
    updateControlGroup();
}

void EightWayMovementGroup::topMidMove(qreal dx, qreal dy)
{
    Q_UNUSED(dx);
    prepareGeometryChange();
    // 只移动上边的2个顶点，且只在y轴上移动，不能超过下边
    // 边上中点随着顶点的移动而调整
    if (m_topMid->pos().y() + dy > m_bottomMid->pos().y())
        dy = m_bottomMid->pos().y() - m_topMid->pos().y();
    m_topLeft->moveBy(0, dy);
    m_topRight->moveBy(0, dy);
    updateControlGroup();
}

void EightWayMovementGroup::topRightMove(qreal dx, qreal dy)
{
    prepareGeometryChange();
    if (m_topRight->pos().x() + dx < m_leftMid->pos().x())
        dx = m_topLeft->pos().x() - m_topRight->pos().x();
    if (m_topRight->pos().y() + dy > m_bottomMid->pos().y())
        dy = m_bottomMid->pos().y() - m_topRight->pos().y();
    m_topLeft->moveBy(0, dy);
    m_bottomRight->moveBy(dx, 0);
    m_topRight->moveBy(dx, dy);
    updateControlGroup();
}

void EightWayMovementGroup::rightMidMove(qreal dx, qreal dy)
{
    Q_UNUSED(dy);
    prepareGeometryChange();
    // 只移动右边的2个顶点，且只在x轴上移动，不能超过左边
    // 边上中点随着顶点的移动而调整
    if (m_rightMid->pos().x() + dx < m_leftMid->pos().x())
        dx = m_leftMid->pos().x() - m_rightMid->pos().x();
    m_topRight->moveBy(dx, 0);
    m_bottomRight->moveBy(dx, 0);
    updateControlGroup();
}

void EightWayMovementGroup::bottomRightMove(qreal dx, qreal dy)
{
    prepareGeometryChange();
    if (m_bottomRight->pos().x() + dx < m_leftMid->pos().x())
        dx = m_leftMid->pos().x() - m_bottomRight->pos().x();
    if (m_bottomRight->pos().y() + dy < m_topMid->pos().y())
        dy = m_topMid->pos().y() - m_bottomRight->pos().y();
    m_topRight->moveBy(dx, 0);
    m_bottomLeft->moveBy(0, dy);
    m_bottomRight->moveBy(dx, dy);
    updateControlGroup();
}

void EightWayMovementGroup::bottomMidMove(qreal dx, qreal dy)
{
    Q_UNUSED(dx);
    prepareGeometryChange();
    // 只移动下边的2个顶点，且只在y轴上移动，不能超过上边
    // 边上中点随着顶点的移动而调整
    if (m_bottomMid->pos().y() + dy < m_topMid->pos().y())
        dy = m_topMid->pos().y() - m_bottomMid->pos().y();
    m_bottomLeft->moveBy(0, dy);
    m_bottomRight->moveBy(0, dy);
    updateControlGroup();
}

void EightWayMovementGroup::bottomLeftMove(qreal dx, qreal dy)
{
    prepareGeometryChange();
    if (m_bottomLeft->pos().x() + dx > m_rightMid->pos().x())
        dx = m_rightMid->pos().x() - m_bottomLeft->pos().x();
    if (m_bottomLeft->pos().y() + dy < m_topMid->pos().y())
        dy = m_topMid->pos().y() - m_bottomLeft->pos().y();
    m_topLeft->moveBy(dx, 0);
    m_bottomRight->moveBy(0, dy);
    m_bottomLeft->moveBy(dx, dy);
    updateControlGroup();
}
    // end of 控制点移动逻辑处理的槽
    // ----------------------------------------------------------------------
    // EightWayMovementGroup的其他方法
bool EightWayMovementGroup::isChildControlPointItem(ControlPointItem* item)
{
    return item == m_leftMid     ||
           item == m_topLeft     ||
           item == m_topMid      ||
           item == m_topRight    ||
           item == m_rightMid    ||
           item == m_bottomRight ||
           item == m_bottomMid   ||
           item == m_bottomLeft;
}

void EightWayMovementGroup::handleFocusItemChanged(QGraphicsItem *newFocusItem, QGraphicsItem *oldFocusItem, Qt::FocusReason reason)
{
    Q_UNUSED(oldFocusItem);
    Q_UNUSED(reason);
    // 焦点既不在子控制点上，也不再控制组对象身上
    if (!isChildControlPointItem(dynamic_cast<ControlPointItem*>(newFocusItem)) && newFocusItem != this)
    {
        m_leftMid->hide();
        m_topLeft->hide();
        m_topMid->hide();
        m_topRight->hide();
        m_rightMid->hide();
        m_bottomRight->hide();
        m_bottomMid->hide();
        m_bottomLeft->hide();
        emit needToDestroy();
    }
}

QVariant EightWayMovementGroup::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == GraphicsItemChange::ItemSceneHasChanged)
    {
        QGraphicsScene* scene = value.value<QGraphicsScene*>();
        if (scene != nullptr)
        {
            connect(scene, &QGraphicsScene::focusItemChanged,
                    this, &EightWayMovementGroup::handleFocusItemChanged);
            this->setFocus(Qt::MouseFocusReason);
        }
    }
    return ControlPointGroup::itemChange(change, value);
}

QRectF EightWayMovementGroup::boundingRect() const
{
    return QRectF(m_topLeft->pos(), m_bottomRight->pos());
}

void EightWayMovementGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen pen(Qt::DashLine);
    pen.setColor(Qt::white);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::NoBrush));
    painter->drawRect(boundingRect());
}

void EightWayMovementGroup::recalculateAllMidPoint()
{
    m_leftMid->setPos((m_bottomLeft->pos() + m_topLeft->pos()) / 2);
    m_topMid->setPos((m_topLeft->pos() + m_topRight->pos()) / 2);
    m_rightMid->setPos((m_topRight->pos() + m_bottomRight->pos()) / 2);
    m_bottomMid->setPos((m_bottomLeft->pos() + m_bottomRight->pos()) / 2);
}

void EightWayMovementGroup::updateControlGroup()
{
    recalculateAllMidPoint();
    scene()->update();
    emit rectInfo(mapRectToScene(boundingRect()), 0.0);
}
// end of EightWayMovementGroup
}// end of namespace ADEV
