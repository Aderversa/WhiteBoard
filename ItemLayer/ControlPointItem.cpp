#include "ControlPointItem.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

namespace ADEV
{
// 构造和析构
ControlPointItem::ControlPointItem(const QPointF &pos, QGraphicsItem* parent)
    : QGraphicsItem{parent}
{
    setPos(pos);
    setZValue(parent->zValue() + 1);
    m_pen.setColor(Qt::blue);
    m_pen.setWidth(2);
    m_brush.setColor(Qt::blue);
}
// end of 构造和析构
// ----------------------------------------------------------------------
// 重写QGraphicsItem的一些方法
QRectF ControlPointItem::boundingRect() const
{
    qreal d = WIDTH / 2;
    return QRectF(QPointF(-d, -d), QPointF(d, d));
}

void ControlPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    // 由于控制点并不是一个可以被擦除的图元，它存在的目的是为了组成一个组调整另一个图元的形状
    // 当你试图使用橡皮擦进行擦除时，焦点早已不在控制点组，而没有焦点的控制点组会被隐藏然后释放
    // 因此，控制点不需要变成填充区域
    painter->drawRect(boundingRect());
}
// end of 重写QGraphicsItem的一些方法
// ----------------------------------------------------------------------
// 重写部分QGraphicsItem的事件处理函数
void ControlPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal dx = event->pos().x() - event->lastPos().x();
    qreal dy = event->pos().y() - event->lastPos().y();
    emit posMove(dx, dy);
}
// end of 重写部分QGraphicsItem的事件处理函数
// ----------------------------------------------------------------------

} // end of namespace ADEV
