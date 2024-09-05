#ifndef CONTROLGROUPOBSERVER_H
#define CONTROLGROUPOBSERVER_H

#include "WhiteBoardItem.h"
#include <QSharedPointer>
#include <QScopedPointer>

namespace ADEV {

class RectangleItem : public WhiteBoardItem
{
public:
    RectangleItem(const QPointF& start,
                  const QPointF& end,
                  const DotPolygonItem& prototype,
                  WhiteBoardItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    // 必须保证不能修改这个对象，只能进行读取操作
    DotPolygonItem m_dotPrototype;
    ControlDotItem* m_leftTop;
    ControlDotItem* m_rightTop;
    ControlDotItem* m_leftBottom;
    ControlDotItem* m_rightBottom;
    ControlDotItem* m_leftMid;
    ControlDotItem* m_rightMid;
    ControlDotItem* m_topMid;
    ControlDotItem* m_bottomMid;

protected:

private:
    void resetMidControlDot();
    void adjustShape();

private slots:
    void leftTopMove(QPointF diff);
    void leftBottomMove(QPointF diff);
    void rightTopMove(QPointF diff);
    void rightBottomMove(QPointF diff);
    void leftMidMove(QPointF diff);
    void rightMidMove(QPointF diff);
    void topMidMove(QPointF diff);
    void bottomMidMove(QPointF diff);
};

} // end of namespace ADEV

#endif // CONTROLGROUPOBSERVER_H
