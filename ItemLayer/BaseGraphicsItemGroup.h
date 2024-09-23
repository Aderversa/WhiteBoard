#ifndef BASEGRAPHICSITEMGROUP_H
#define BASEGRAPHICSITEMGROUP_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QVector>
#include <QHash>

namespace ADEV {

class BaseGraphicsItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit BaseGraphicsItemGroup(const QPointF& startPos, QGraphicsItem* parent = nullptr);
    ~BaseGraphicsItemGroup();
    // 默认采集的点来自Scene，该函数会将传入的点映射到本地坐标系，并调整m_shape
    void appendPoint(const QPointF& scenePos);
    void addToGroup(QGraphicsItem *item);
    void complete();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const;
    QPainterPath shape() const;

signals:
    void needToDestroy();

protected:
    void focusOutEvent(QFocusEvent *event) override;

private:
    void calculateShape();
    QVector<QPointF> m_points; // 收集到的轨迹点
    // 在m_points没有变化时，m_shape保持不变
    QPainterPath m_shape;
    QHash<QGraphicsItem*, QPointF> m_itemMapStartPos;
};

} // end of ADEV

#endif // BASEGRAPHICSITEMGROUP_H
