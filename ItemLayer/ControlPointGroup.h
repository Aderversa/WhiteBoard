#ifndef CONTROLPOINTGROUP_H
#define CONTROLPOINTGROUP_H

#include <QObject>
#include <QGraphicsItem>

namespace ADEV {

class ControlPointItem;
class ControlPointGroup : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    virtual ~ControlPointGroup() = default;
    ControlPointGroup(QGraphicsItem* parent = nullptr);

signals: // 为其子类定义好信号
    // 子类需要在控制点位置发生变化时，发出rectInfo信号
    void rectInfo(QRectF rect, qreal angle);

    // 子类需要在自身和其子项失去焦点时，发出destroy信号，通知外界回收
    void destroy();
};

class EightWayMovementGroup : public ControlPointGroup
{
    Q_OBJECT
public slots: // 控制点移动逻辑处理的槽
    void leftMidMove(qreal dx, qreal dy);
    void topLeftMove(qreal dx, qreal dy);
    void topMidMove(qreal dx, qreal dy);
    void topRightMove(qreal dx, qreal dy);
    void rightMidMove(qreal dx, qreal dy);
    void bottomRightMove(qreal dx, qreal dy);
    void bottomMidMove(qreal dx, qreal dy);
    void bottomLeftMove(qreal dx, qreal dy);

private slots: // 初始聚焦后失焦的处理逻辑，这里的聚焦指的是：Item或其控制点子Item处于焦点状态都叫聚焦。
    void handleFocusItemChanged(QGraphicsItem *newFocusItem, QGraphicsItem *oldFocusItem, Qt::FocusReason reason);

public: // 重写来自QGraphicsItem的一些方法
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public: // 构造和析构
    EightWayMovementGroup(const QRectF& rect, QGraphicsItem* parent = nullptr);
    ~EightWayMovementGroup();

private: // 一些辅助函数
    bool isChildControlPointItem(ControlPointItem* item);
    void recalculateAllMidPoint();
    void updateControlGroup();

private: // 私有成员变量
    ControlPointItem* m_leftMid;
    ControlPointItem* m_topLeft;
    ControlPointItem* m_topMid;
    ControlPointItem* m_topRight;
    ControlPointItem* m_rightMid;
    ControlPointItem* m_bottomRight;
    ControlPointItem* m_bottomMid;
    ControlPointItem* m_bottomLeft;
};

} // end of namespace ADEV

#endif // CONTROLPOINTGROUP_H
