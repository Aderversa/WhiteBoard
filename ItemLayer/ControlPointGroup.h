#ifndef CONTROLPOINTGROUP_H
#define CONTROLPOINTGROUP_H

#include <QObject>
#include <QGraphicsItem>

namespace ADEV {

class ControlPointGroup : public QObject, public QGraphicsItem
{
    Q_OBJECT
signals: // 为其子类定义好信号
    // 子类需要在控制点位置发生变化时，发出rectInfo信号
    void rectInfo(QRectF rect, qreal angle);

    // 子类需要在自身和其子项失去焦点时，发出destroy信号，通知外界回收
    void destroy();
};

class EightWayMovementGroup
{
public:

private:
};

} // end of namespace ADEV

#endif // CONTROLPOINTGROUP_H
