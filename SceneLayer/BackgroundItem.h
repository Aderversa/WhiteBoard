#ifndef BACKGROUNDITEM_H
#define BACKGROUNDITEM_H

#include <QGraphicsItem>
#include <QObject>

namespace ADEV {

class BackgroundItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public: // getter或setter
    QSizeF size() const;
    QColor color() const;
    void setColor(QColor color);
    virtual void resize(QSizeF size);

public: // 来自QGraphicsItem
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void colorChanged(QColor color);

public slots:
    // 其实这个槽等效于setColor()
    void changeSceneBackgroundColor(QColor color);

public: // 构造、析构
    // 后续会使用配置系统来设置其初始化时的默认参数
    // 现在需要手动指定
    explicit BackgroundItem(QColor color, const QSizeF& size, QGraphicsItem* parent = nullptr);

private:
    QSizeF m_size;
    QColor m_color;
};

class BackgroundImageItem : public BackgroundItem
{
public:
    BackgroundImageItem(const QImage& image, const QSizeF& imageSize);
    void resize(QSizeF size) override;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    const QImage m_image; // 仅在初始化时得到构造
};

class BackgroundPathItem : public BackgroundItem
{
public:
    BackgroundPathItem(const QPainterPath& path, QGraphicsItem* parent = nullptr);
    void resize(QSizeF size) override;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    const QPainterPath m_path;
};

} // end of namespace ADEV

#endif // BACKGROUNDITEM_H
