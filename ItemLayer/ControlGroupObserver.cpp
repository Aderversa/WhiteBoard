#include "ControlGroupObserver.h"
#include <QGraphicsScene>

namespace ADEV {

RectangleItem::RectangleItem(const QPointF& start,
                  const QPointF& end,
                  const DotPolygonItem& prototype,
                  WhiteBoardItem* parent)
    : WhiteBoardItem(parent)
    , m_dotPrototype(prototype)
{
    QPointF center = (start + end) / qreal(2);
    qreal width = qAbs(start.x() - end.x());
    qreal height = qAbs(start.y() - end.y());
    m_leftTop = new ControlDotItem(QPointF(center.x() - width / 2, center.y() - height / 2), this);
    m_leftTop->setZValue(10);

    m_rightTop = new ControlDotItem(QPointF(center.x() + width / 2, center.y() - height / 2), this);
    m_rightTop->setZValue(10);

    m_leftBottom = new ControlDotItem(QPointF(center.x() - width / 2, center.y() + height / 2), this);
    m_leftBottom->setZValue(10);

    m_rightBottom = new ControlDotItem(QPointF(center.x() + width / 2, center.y() + height / 2), this);
    m_rightBottom->setZValue(10);

    m_leftMid = new ControlDotItem((m_leftTop->pos() + m_leftBottom->pos()) / 2.0, this);
    m_leftMid->setZValue(10);

    m_topMid = new ControlDotItem((m_leftTop->pos() + m_rightTop->pos()) / 2.0, this);
    m_topMid->setZValue(10);

    m_rightMid = new ControlDotItem((m_rightBottom->pos() + m_rightTop->pos()) / 2.0, this);
    m_rightMid->setZValue(10);

    m_bottomMid = new ControlDotItem((m_leftBottom->pos() + m_rightBottom->pos()) / 2.0, this);
    m_bottomMid->setZValue(10);

    // 总共八个点作为控制点
    connect(m_leftTop, &ControlDotItem::pointMove, this, &RectangleItem::leftTopMove);
    connect(m_leftBottom, &ControlDotItem::pointMove, this, &RectangleItem::leftBottomMove);
    connect(m_rightTop, &ControlDotItem::pointMove, this, &RectangleItem::rightTopMove);
    connect(m_rightBottom, &ControlDotItem::pointMove, this, &RectangleItem::rightBottomMove);
    connect(m_leftMid, &ControlDotItem::pointMove, this, &RectangleItem::leftMidMove);
    connect(m_rightMid, &ControlDotItem::pointMove, this, &RectangleItem::rightMidMove);
    connect(m_topMid, &ControlDotItem::pointMove, this, &RectangleItem::topMidMove);
    connect(m_bottomMid, &ControlDotItem::pointMove, this, &RectangleItem::bottomMidMove);
    // 添加必要的DotPolygonItem组成
    adjustShape();
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setFocus(Qt::MouseFocusReason);
}

void RectangleItem::adjustShape()
{
    while (!m_atomDots.isEmpty()) {
        DotPolygonItem* dot = m_atomDots.front();
        m_atomDots.pop_front();
        delete dot;
    }
    qreal step = m_dotPrototype.getRadius() / 4.0;
    qreal begin = m_leftTop->pos().x() < m_rightTop->pos().x() ? m_leftTop->pos().x() : m_rightTop->pos().x();
    qreal end = m_leftTop->pos().x() > m_rightTop->pos().x() ? m_leftTop->pos().x() : m_rightTop->pos().x();
    qreal pos = begin;
    qreal z = m_leftTop->zValue() - 1;
    while (pos <= end) {
        DotPolygonItem* item1 = new DotPolygonItem(m_dotPrototype);
        item1->setParentItem(this);
        // item1->setZValue(z);
        DotPolygonItem* item2 = new DotPolygonItem(m_dotPrototype);
        item2->setParentItem(this);
        // item2->setZValue(z);

        item1->setPos(pos, m_leftTop->pos().y());
        item2->setPos(pos, m_leftBottom->pos().y());

        m_atomDots << item1 << item2;
        pos += step;
    }
    begin = m_leftTop->pos().y() < m_leftBottom->pos().y() ? m_leftTop->pos().y() : m_leftBottom->pos().y();
    end = m_leftTop->pos().y() > m_leftBottom->pos().y() ? m_leftTop->pos().y() : m_leftBottom->pos().y();
    pos = begin;
    z = m_leftTop->zValue() - 1;
    while(pos <= end) {
        DotPolygonItem* item1 = new DotPolygonItem(m_dotPrototype);
        item1->setParentItem(this);
        // item1->setZValue(z);

        DotPolygonItem* item2 = new DotPolygonItem(m_dotPrototype);
        item2->setParentItem(this);
        // item2->setZValue(z);

        item1->setPos(m_rightTop->pos().x(), pos);
        item2->setPos(m_leftTop->pos().x(), pos);
        m_atomDots << item1 << item2;
        pos += step;
    }
}

QRectF RectangleItem::boundingRect() const
{
    // 对角线的中点是矩形的中心
    QPointF center = (m_leftTop->pos() + m_rightBottom->pos()) / 2.0;
    qreal width = qAbs(m_leftTop->pos().x() - m_rightTop->pos().x());
    qreal height = qAbs(m_rightTop->pos().y() - m_rightBottom->pos().y());
    return QRectF(QPointF(center.x() - width / 2.0, center.y() - height / 2.0),
                  QPointF(center.x() + width / 2.0, center.y() + height / 2.0));
}

void RectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
    for (auto& dot : m_atomDots)
        dot->update();
}

void RectangleItem::resetMidControlDot()
{
    m_leftMid->setPos((m_leftTop->pos() + m_leftBottom->pos()) / 2.0);
    m_topMid->setPos((m_leftTop->pos() + m_rightTop->pos()) / 2.0);
    m_rightMid->setPos((m_rightTop->pos() + m_rightBottom->pos()) / 2.0);
    m_bottomMid->setPos((m_leftBottom->pos() + m_rightBottom->pos()) / 2.0);
}

void RectangleItem::leftTopMove(QPointF diff)
{
    m_leftTop->moveBy(diff.x(), diff.y());
    m_leftBottom->moveBy(diff.x(), 0.0);
    m_rightTop->moveBy(0.0, diff.y());
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

void RectangleItem::leftBottomMove(QPointF diff)
{
    m_leftBottom->moveBy(diff.x(), diff.y());
    m_leftTop->moveBy(diff.x(), 0.0);
    m_rightBottom->moveBy(0.0, diff.y());
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

void RectangleItem::rightTopMove(QPointF diff)
{
    m_rightTop->moveBy(diff.x(), diff.y());
    m_leftTop->moveBy(0.0, diff.y());
    m_rightBottom->moveBy(diff.x(), 0.0);
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

void RectangleItem::rightBottomMove(QPointF diff)
{
    m_rightBottom->moveBy(diff.x(), diff.y());
    m_leftBottom->moveBy(0.0, diff.y());
    m_rightTop->moveBy(diff.x(), 0.0);
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

void RectangleItem::leftMidMove(QPointF diff)
{
    m_leftTop->moveBy(diff.x(), 0.0);
    m_leftBottom->moveBy(diff.x(), 0.0);
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

void RectangleItem::rightMidMove(QPointF diff)
{
    m_rightTop->moveBy(diff.x(), 0.0);
    m_rightBottom->moveBy(diff.x(), 0.0);
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

void RectangleItem::topMidMove(QPointF diff)
{
    m_leftTop->moveBy(0.0, diff.y());
    m_rightTop->moveBy(0.0, diff.y());
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

void RectangleItem::bottomMidMove(QPointF diff)
{
    m_leftBottom->moveBy(0.0, diff.y());
    m_rightBottom->moveBy(0.0, diff.y());
    resetMidControlDot();
    adjustShape();
    scene()->update();
}

} // end of namespace ADEV
