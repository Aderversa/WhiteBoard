#ifndef BASEGRAPHICSITEM_H
#define BASEGRAPHICSITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QBrush>

namespace ADEV
{

class DotPolygonItem : public QAbstractGraphicsShapeItem
{
public:
    explicit DotPolygonItem(QColor color,
                            qreal radius,
                            qreal opacity,
                            bool openLaserMode = false,
                            QGraphicsItem* parent = nullptr);
    explicit DotPolygonItem(const DotPolygonItem& that);
    ~DotPolygonItem();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setColor(QColor color);
    void setRadius(qreal radius);
    qreal getRadius() const {return m_radius;}

private:
    QColor m_color;
    qreal m_radius;
    // 这里本来设置有opacity，但后面发现设置透明度的工作可以完全交给父类的方法，自己完全可以不用记录
    // 激光模式，中心为白色，周围有m_color颜色的阴影
    bool m_openLaserMode;

    QBrush m_foreBrush;
    QBrush m_backBrush;
};

}

#endif // BASEGRAPHICSITEM_H
