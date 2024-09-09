#ifndef WHITEBOARDTOOLS_H
#define WHITEBOARDTOOLS_H

#include <QColor>

namespace ADEV {

struct WhiteBoardPen {
    qreal width;
    QColor color;
};

struct WhiteBoardHighlightPen {
    qreal width;
    QColor color;
    qreal opacity;
    bool openStraightLineMode;
};

struct WhiteBoardLaserPen {
    QColor color;
};

struct WhiteBoardEraser {
    qreal radius;
    bool eraseWholeItem;
};


} // end of namespace ADEV

#endif // WHITEBOARDTOOLS_H
