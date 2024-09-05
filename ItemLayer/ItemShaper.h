#ifndef ITEMSHAPER_H
#define ITEMSHAPER_H

#include <QPainterPath>

namespace ADEV {

class /*Interface*/ ItemShaper
{
public:
    virtual ~ItemShaper() = default;
    virtual qreal strokeWidth() const = 0;
    virtual void setStrokePath(const QPainterPath& path) = 0;
    virtual QPainterPath lineToStroke(const QLineF& line, qreal width) = 0;
};

}

#endif // ITEMSHAPER_H
