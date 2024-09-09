#include "BaseGraphicsItem.h"

#include <QPainter>
#include <QGraphicsScene>

namespace ADEV
{

BaseGraphicsItem::BaseGraphicsItem(qreal strokeWidth, const QBrush& brush, QGraphicsItem* parent)
    : QGraphicsItem{parent}
    , m_strokeWidth(strokeWidth)
    , m_brush(brush)
{
    m_pen.setStyle(Qt::NoPen);
}

// ItemShaper接口实现
qreal BaseGraphicsItem::strokeWidth() const
{
    return m_strokeWidth;
}

void BaseGraphicsItem::setStrokePath(const QPainterPath &path)
{
    m_strokePath = path;
    this->scene()->update();
}

QPainterPath BaseGraphicsItem::lineToStroke(const QLineF &line, qreal width) const
{
    // 处理
    QPointF startPoint;
    // 保证startPoint是QLineF的起始点
    startPoint.setX(line.x1());
    startPoint.setY(line.y1());

    QTransform transform; // transform是旋转矩阵
    transform.translate(startPoint.x(), startPoint.y()); // 设置旋转的原点
    transform.rotateRadians(qDegreesToRadians(-line.angle()));
    transform.translate(-startPoint.x(), -startPoint.y()); // 重新设置会原来的点

    // 将直线放平，startPoint->endPoint，两点的距离为distance
    qreal distance = qSqrt(qPow(line.x1() - line.x2(), 2) + qPow(line.y1() - line.y2(), 2));
    QPointF endPoint(startPoint.x() + distance, startPoint.y());
    QRectF rect(QPointF(startPoint.x(), startPoint.y() - width / 2),
                QPointF(endPoint.x(), endPoint.y() + width / 2));

    // 放平后进行“线”填充区域的绘制
    QPainterPath tempPath;
    tempPath.addRect(rect);
    QPainterPath ellipsePath1;
    ellipsePath1.addEllipse(QRectF(startPoint.x() - width / 2, startPoint.y() - width / 2, width , width));
    QPainterPath ellipsePath2;
    ellipsePath2.addEllipse(QRectF(endPoint.x() - width / 2, endPoint.y() - width / 2, width , width));
    tempPath = tempPath.united(ellipsePath1);
    tempPath = tempPath.united(ellipsePath2);

    // 返回旋转后的“线”填充区域
    return transform.map(tempPath);
}
// end of ItemShaper接口实现
// ----------------------------------------------------------------------
// 部分成员变量的Getter或Setter
QPen BaseGraphicsItem::pen() const
{
    return m_pen;
}

QBrush BaseGraphicsItem::brush() const
{
    return m_brush;
}

void BaseGraphicsItem::setBrush(const QBrush& brush)
{
    m_brush = brush;
}

void BaseGraphicsItem::setStrokeWidth(qreal width)
{
    m_strokeWidth = width;
}

QPainterPath BaseGraphicsItem::strokePath() const
{
    return m_strokePath;
}
// end of 部分成员变量的Getter或Setter
// ----------------------------------------------------------------------
// 重写QGraphicsItem的一些方法
QRectF BaseGraphicsItem::boundingRect() const
{
    return m_strokePath.boundingRect();
}

QPainterPath BaseGraphicsItem::shape() const
{
    return m_strokePath;
}

bool BaseGraphicsItem::collidesWithPath(const QPainterPath &path, Qt::ItemSelectionMode mode) const
{
    // BaseGraphicsItem::collidesWithPath()中，不再考虑mode，默认mode为Qt::IntersectsItemShape
    Q_UNUSED(mode);
    return m_strokePath.intersects(path);
}

void BaseGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPath(m_strokePath);
}
// end of 重写QGraphicsItem的一些方法
// ----------------------------------------------------------------------
// 满足业务逻辑的方法
/*
 *  handleCollides()
 *  当m_strokePath和path发生碰撞时，消除与path碰撞的部分。
 *  如果碰撞导致了一个笔画stroke分段，就会以QList的形式返回多个QPainterPath
 *  如果碰撞没有导致笔画分段，则QList中只有一个QPainterPath
 */
QList<QPainterPath> BaseGraphicsItem::handleCollides(const QPainterPath& path)
{
    QList<QPolygonF> remainPolygons = m_strokePath.subtracted(path).toFillPolygons();
    QList<QPainterPath> remainStrokes;
    while (!remainPolygons.empty())
    {
        QPolygonF polygon = remainPolygons.front();
        remainPolygons.pop_front();
        QPainterPath stroke;
        stroke.addPolygon(polygon);
        remainStrokes.push_back(qMove(stroke));
    }
    return remainStrokes;
}

BaseGraphicsItem::Memento BaseGraphicsItem::Memento::save(const BaseGraphicsItem &item)
{
    Memento memento;
    memento.width = item.m_strokeWidth;
    memento.brush = item.m_brush;
    memento.strokePath = item.m_strokePath;
    memento.opacity = item.opacity();
    memento.zValue = item.zValue();
    return memento;
}

void BaseGraphicsItem::Memento::restore(const Memento& memento,BaseGraphicsItem &item)
{
    item.m_brush = memento.brush;
    item.m_strokeWidth = memento.width;
    item.m_strokePath = memento.strokePath;
    item.setOpacity(memento.opacity);
    item.setZValue(memento.zValue);
}

BaseGraphicsItem::Memento BaseGraphicsItem::save()
{
    return Memento::save(*this);
}

void BaseGraphicsItem::restore(const Memento& memento)
{
    Memento::restore(memento, *this);
}

} // end of namespace ADEV
