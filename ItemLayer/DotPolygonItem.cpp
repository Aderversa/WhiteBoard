#include "DotPolygonItem.h"

#include <QPainter>
#include <QBrush>
#include <QRadialGradient>

namespace ADEV
{

DotPolygonItem::DotPolygonItem(QColor color,
                        qreal radius,
                        qreal opacity,
                        bool openLaserMode,
                        QGraphicsItem* parent)
    : QAbstractGraphicsShapeItem(parent)
    , m_color(color)
    , m_radius(radius)
    , m_openLaserMode(openLaserMode)
{
    this->setOpacity(opacity);

    QPen* pen = new QPen;
    pen->setColor(Qt::white);
    pen->setWidth(0);
    this->setPen(*pen);

    m_foreBrush = new QBrush;
    m_foreBrush->setStyle(Qt::SolidPattern);
    if (openLaserMode) // 开启激光笔模式
    {
        // 这里的数据都是经过调整的，将激光笔点的效果调整到想要的效果
        m_radius = 3;
        m_foreBrush->setColor(Qt::white);
        m_radialGradient = new QRadialGradient(QPointF(0, 0), 6.25, QPointF(0, 0));
        m_radialGradient->setColorAt(0.6, m_color);
        m_radialGradient->setColorAt(1, Qt::white);
        m_backBrush = new QBrush(*m_radialGradient);
    }
    else
    {
        m_foreBrush->setColor(m_color);
    }
}

DotPolygonItem::~DotPolygonItem()
{
    if (!m_foreBrush)
        delete m_foreBrush;
    if (!m_backBrush)
        delete m_backBrush;
    if (!m_radialGradient)
        delete m_radialGradient;
}

QRectF DotPolygonItem::boundingRect() const
{
    return QRectF(QPointF(-m_radius, -m_radius), QPointF(m_radius, m_radius));
}

QPainterPath DotPolygonItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void DotPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    if (m_openLaserMode)
    {
        painter->setBrush(*m_backBrush);
        qreal temp = 6.25; // 这是一个经过调整的数据
        painter->drawEllipse(QRectF(QPointF(-temp, -temp), QPointF(temp, temp)));
    }
    painter->setBrush(*m_foreBrush);
    painter->drawEllipse(boundingRect());
}

void DotPolygonItem::setColor(QColor color)
{
    m_color = color;
}

void DotPolygonItem::setRadius(qreal radius)
{
    if (m_openLaserMode)
        return;
    m_radius = radius;
}

} // namespace ADEV
