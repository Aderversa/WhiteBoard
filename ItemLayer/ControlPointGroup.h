#ifndef CONTROLPOINTGROUP_H
#define CONTROLPOINTGROUP_H

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QList>
#include "ItemShaper.h"
#include "BaseGraphicsItem.h"

namespace ADEV {

class WhiteBoardItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    WhiteBoardItem(QAbstractGraphicsShapeItem* parent = nullptr);

protected:
    QList<DotPolygonItem*> m_atomDots;
};

} // end of namespace ADEV

#endif // CONTROLPOINTGROUP_H
