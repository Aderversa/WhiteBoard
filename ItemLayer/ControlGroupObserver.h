#ifndef CONTROLGROUPOBSERVER_H
#define CONTROLGROUPOBSERVER_H

#include "ItemLayer/ItemShaper.h"
#include "ItemLayer/ControlPointGroup.h"

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
    void destroy();

private:
    ItemShaper* m_itemShaper;
    ControlPointGroup* m_controlGroup;
};

class ControlRectangleObserver : public ControlGroupObserver
{
    Q_OBJECT
public:
    ControlRectangleObserver(ItemShaper* itemShaper);

public slots:
    void formItem(QRectF rect, qreal angle) override final;
};

} // end of namespace ADEV

#endif // CONTROLGROUPOBSERVER_H
