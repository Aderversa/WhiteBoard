#ifndef WHITEBOARDTOOLS_H
#define WHITEBOARDTOOLS_H

#include <QColor>

namespace ADEV {

enum class WhiteBoardTool
{
    NormalPen = 0,  // 普通的钢笔
    HighlightPen,   // 荧光笔
    LaserPen,       // 激光笔
    Eraser,         // 橡皮擦
    GraphcsiItemPen //用来绘制标准图元，如矩形、椭圆的笔
};

struct WhiteBoardNormalPen {
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
