#include "BaseGraphicsItemGroup.h"
#include "SceneLayer/BasicCommand.h"
#include "SceneLayer/WhiteBoardScene.h".h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace ADEV {

BaseGraphicsItemGroup::BaseGraphicsItemGroup(const QPointF& startPos, QGraphicsItem* parent)
    : QGraphicsItemGroup(parent)
{
    setPos(0.0, 0.0);
    m_points.push_back(startPos);
    m_points.push_back(startPos);
    setFlags(QGraphicsItem::ItemIsMovable   |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemIsSelectable);
    setFocus(Qt::MouseFocusReason);
}

BaseGraphicsItemGroup::~BaseGraphicsItemGroup()
{
    prepareGeometryChange();
    WhiteBoardScene* sc = nullptr;
    if (scene()) {
        sc = dynamic_cast<WhiteBoardScene*>(scene());
    }
    QList<QGraphicsItem*> items = childItems();
    MoveItemsCommand* finalCommand = new MoveItemsCommand();
    for (auto& pItem : items)
    {
        removeFromGroup(pItem);
        BaseGraphicsItem* movedItem = dynamic_cast<BaseGraphicsItem*>(pItem);
        if (sc && movedItem) {
            auto command = QSharedPointer<MoveItemCommand>(
                                      new MoveItemCommand(m_itemMapStartPos[pItem],
                                                          pItem->pos(),
                                                          sc->getItem(movedItem)));
            finalCommand->push(command);
        }
    }
    if (sc && finalCommand->size() > 0)
        sc->undoStack()->push(finalCommand);
    else
        delete finalCommand;
}


void BaseGraphicsItemGroup::calculateShape()
{
    prepareGeometryChange();
    if (m_points.size() == 2 || m_points.size() % 5 != 0) {
        return;
    }
    QPainterPath path;
    for (int i = 0; i < m_points.size(); i += 2) {
        if (i == 0) {
            path.moveTo(m_points[i]);
        }
        else if (i == m_points.size() - 1) {
            break;
        }
        path.quadTo(m_points[i + 1], m_points[i + 2]);
    }
    m_shape = path;
    if (scene())
        scene()->update();
}

void BaseGraphicsItemGroup::appendPoint(const QPointF& scenePos)
{
    QPointF pos = scenePos;
    if (m_points.size() > 2) {
        m_points.pop_back();
    }
    m_points.push_back((m_points.back() + pos) / 2.0);
    m_points.push_back(pos);
    m_points.push_back(pos);
    calculateShape();
}

void BaseGraphicsItemGroup::addToGroup(QGraphicsItem *item)
{
    m_itemMapStartPos[item] = item->pos();
    QGraphicsItemGroup::addToGroup(item);
}

void BaseGraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(6);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::NoBrush));
    painter->drawPath(shape());
}

QRectF BaseGraphicsItemGroup::boundingRect() const
{
    return m_shape.boundingRect();
}

QPainterPath BaseGraphicsItemGroup::shape() const
{
    return m_shape;
}

void BaseGraphicsItemGroup::complete()
{
    m_shape.closeSubpath();
}

void BaseGraphicsItemGroup::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    emit needToDestroy();
}

}// end of ADEV
