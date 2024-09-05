#include "ControlPointGroup.h"
#include "ControlPointItem.h"

#include <QGraphicsScene>
#include <QVariant>

namespace ADEV {
// ControlPointGroup
ControlPointGroup::ControlPointGroup(QGraphicsItem *parent)
    : QGraphicsItem{parent}
{}
// end of ControlPointGroup
// ----------------------------------------------------------------------
// EightWayMovementGroup
    // 构造和析构
EightWayMovementGroup::EightWayMovementGroup(const QRectF& rect, QGraphicsItem* parent = nullptr)
    : ControlPointItem{parent}
{
    m_leftMid     = new ControlPointItem((rect.topLeft() + rect.bottomLeft()) / 2);
    m_topLeft     = new ControlPointItem(rect.topLeft());
    m_topMid      = new ControlPointItem((rect.topLeft() + rect.topRight()) / 2);
    m_topRight    = new ControlPointItem(rect.topRight());
    m_rightMid    = new ControlPointItem((rect.topRight() + rect.bottomRight()) / 2);
    m_bottomRight = new ControlPointItem(rect.bottomRight());
    m_bottomMid   = new ControlPointItem((rect.bottomLeft() + rect.bottomRight()) / 2);
    m_bottomLeft  = new ControlPointItem(rect.bottomLeft());

    connect(m_leftMid , &ControlPointItem::posMove, this, &EightWayMovementGroup::leftMidMove);
    connect(m_topLeft , &ControlPointItem::posMove, this, &EightWayMovementGroup::topLeftMove);
    connect(m_topMid  , &ControlPointItem::posMove, this, &EightWayMovementGroup::topMidMove);
    connect(m_topRight, &ControlPointItem::posMove, this, &EightWayMovementGroup::topRightMove);
    connect(m_rightMid, &ControlPointItem::posMove, this, &EightWayMovementGroup::rightMidMove);
    connect(m_bottomRight, &ControlPointItem::posMove, this, &EightWayMovementGroup::bottomRightMove);
    connect(m_bottomMid, &ControlPointItem::posMove, this, &EightWayMovementGroup::bottomMidMove);
    connect(m_bottomLeft, &ControlPointItem::posMove, this, &EightWayMovementGroup::bottomLeftMove);
}
    // end of 构造和析构
    // ----------------------------------------------------------------------
    // 控制点移动逻辑处理的槽
void EightWayMovementGroup::leftMidMove(qreal dx, qreal dy)
{

}

void EightWayMovementGroup::topLeftMove(qreal dx, qreal dy)
{

}

void EightWayMovementGroup::topMidMove(qreal dx, qreal dy)
{

}

void EightWayMovementGroup::topRightMove(qreal dx, qreal dy)
{

}

void EightWayMovementGroup::rightMidMove(qreal dx, qreal dy)
{

}

void EightWayMovementGroup::bottomRightMove(qreal dx, qreal dy)
{

}

void EightWayMovementGroup::bottomMidMove(qreal dx, qreal dy)
{

}

void EightWayMovementGroup::bottomLeftMove(qreal dx, qreal dy)
{

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
    // 焦点既不在子控制点上，也不再控制组对象身上
    if (!isChildControlPointItem(newFocusItem) && newFocusItem != this)
    {
        emit destroy();
    }
}

QVariant EightWayMovementGroup::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == GraphicsItemChange::ItemSceneChange)
    {
        QGraphicsScene* scene = value.value<QGraphicsScene*>();
        if (scene != nullptr)
        {
            connect(scene, &QGraphicsScene::focusItemChanged,
                    this, &EightWayMovementGroup::handleFocusItemChanged);
        }
    }
}
// end of EightWayMovementGroup
}// end of namespace ADEV
