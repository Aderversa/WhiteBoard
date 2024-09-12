#ifndef CONTROLGROUPOBSERVER_H
#define CONTROLGROUPOBSERVER_H

#include "ItemLayer/ItemShaper.h"
#include "ItemLayer/ControlPointGroup.h"
#include <QPointer>

namespace ADEV {

class ControlGroupObserver : public QObject
{
    Q_OBJECT
public: // 构造、析构和其他方法
    explicit ControlGroupObserver(ItemShaper* itemShaper);
    virtual ~ControlGroupObserver();
    void setControlGroup(ControlPointGroup* controlGroup);
    ItemShaper* itemShaper() const;

public slots:
    // 处理m_controlGroup发出的rectInfo的槽函数，
    // 负责处理矩形信息，然后将变化映射到m_itemShaper中
    virtual void formItem(QRectF rect, qreal angle) = 0;

    // 在m_controlGroup发出destroy信号后对其进行回收操作，
    // 并发出自己的destroy信号让外界回收自己
    void handleControlPointDestroy();

signals:
    void needToDestroy();

private:
    ItemShaper* m_itemShaper;
    QPointer<ControlPointGroup> m_controlGroup;
};

class ControlRectangleObserver : public ControlGroupObserver
{
    Q_OBJECT
public:
    ControlRectangleObserver(ItemShaper* itemShaper);

public slots:
    void formItem(QRectF rect, qreal angle) override final;
};

class ControlEllipseObserver : public ControlGroupObserver
{
    Q_OBJECT
public:
    ControlEllipseObserver(ItemShaper* itemShaper);

public slots:
    void formItem(QRectF rect, qreal angle) override final;
};

class ControlCurveObserver : public ControlGroupObserver
{
    Q_OBJECT
public:
    ControlCurveObserver(ItemShaper* itemShaper);
    virtual ~ControlCurveObserver() = default;

public slots:
    void formItem(QRectF rect, qreal angle) override final;

    // 独属于ControlCurveObserver的槽函数。
    // ControlCurveObserver在创建时即被绑定这个槽函数，所以不需要担心接口问题。
    // 后续利用到其父类指针都是用来回收其本身的资源的。
    virtual void addPointToCurve(QPointF point);

protected:
    bool m_isTheFirstPoint = true;
    QPointF m_lastPoint;
};

class ControlLineObserver : public ControlCurveObserver // 直线也是一种特殊的曲线
{
    Q_OBJECT
public:
    ControlLineObserver(ItemShaper* itemShaper);
    void addPointToCurve(QPointF point);
};

} // end of namespace ADEV

#endif // CONTROLGROUPOBSERVER_H
