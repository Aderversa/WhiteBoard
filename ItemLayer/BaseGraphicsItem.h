#ifndef BASEGRAPHICSITEM_H
#define BASEGRAPHICSITEM_H

#include "ItemLayer/ItemShaper.h"

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>

namespace ADEV
{

class BaseGraphicsItem : public ItemShaper, public QGraphicsItem
{
public:
    class Memento {
    public:
        static Memento save(const BaseGraphicsItem& item);
        static void restore(const Memento& memento, BaseGraphicsItem& item);

    private:
        qreal opacity;
        QBrush brush;
        qreal width;
        QPainterPath strokePath;
        qreal zValue;
    };
    Memento save(); // 保存当前
    void restore(const Memento& memento);

public: // 实现ItemShaper接口
    qreal strokeWidth() const;
    void setStrokePath(const QPainterPath &path);
    QPainterPath lineToStroke(const QLineF &line, qreal width) const;
    QPainterPath strokePath() const;

public: // 提供一些成员变量的Getter和Setter
    QPen pen() const;
    QBrush brush() const;
    void setBrush(const QBrush& brush);
    void setStrokeWidth(qreal width);

public: // 重写QGraphicsItem的一些方法
    QRectF boundingRect() const;
    QPainterPath shape() const;
    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const override;
    bool collidesWithPath(const QPainterPath &path, Qt::ItemSelectionMode mode) const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public: // 构造、析构以及一些业务逻辑方法
    explicit BaseGraphicsItem(qreal strokeWidth, const QBrush& brush, QGraphicsItem* parent = nullptr);
    ~BaseGraphicsItem();
    QList<QPainterPath> handleCollides(const QPainterPath& path); // 该方法用于满足擦除的业务逻辑

private: // 私有成员变量的声明
    QPainterPath m_strokePath;   // 将来通过ItemShaper接口的方法获得修改
    qreal m_strokeWidth;         // 构造时指定，在ItemShaper时需要使用
    QBrush m_brush;
    QPen m_pen;                  // m_pen不可修改，构造时决定其状态
};

}

#endif // BASEGRAPHICSITEM_H
