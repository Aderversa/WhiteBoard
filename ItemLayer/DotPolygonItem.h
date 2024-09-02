#ifndef DOTPOLYGONITEM_H
#define DOTPOLYGONITEM_H

#include <QAbstractGraphicsShapeItem>
class QBrush;
class QRadialGradient;

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
    ~DotPolygonItem();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setColor(QColor color);
    void setRadius(qreal radius);

private:
    QColor m_color;
    qreal m_radius;
    // 这里本来设置有opacity，但后面发现设置透明度的工作可以完全交给父类的方法，自己完全可以不用记录
    // 激光模式，中心为白色，周围有m_color颜色的阴影
    bool m_openLaserMode;

    QBrush* m_foreBrush = nullptr;
    QBrush* m_backBrush = nullptr;
    QRadialGradient* m_radialGradient = nullptr;
};

}

#endif // DOTPOLYGONITEM_H
