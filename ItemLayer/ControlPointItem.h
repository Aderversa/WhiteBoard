#ifndef CONTROLPOINTITEM_H
#define CONTROLPOINTITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>

namespace ADEV {

// 控制点将以正方形的形式呈现，白底，外圈为蓝色，其边长可以被自定义
class ControlPointItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public: // 重写QGraphicsItem的一些方法
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

protected: // 重写部分QGraphicsItem的事件处理函数
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

public: // 构造和析构
    explicit ControlPointItem(const QPointF& pos, QGraphicsItem* parent = nullptr);

signals: // 发出的信号
    void posMove(qreal dx, qreal dy);

private:
    constexpr static qreal WIDTH = 4;
    QPen m_pen;
    QBrush m_brush;
};

} // end of namespace ADEV

#endif // CONTROLPOINTITEM_H
