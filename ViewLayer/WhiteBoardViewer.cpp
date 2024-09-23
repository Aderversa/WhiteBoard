#include "WhiteBoardViewer.h"

#include <QScrollBar>
#include <QPainter>

namespace ADEV {

WhiteBoardViewer::WhiteBoardViewer(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    verticalScrollBar()->setSingleStep(20);
    horizontalScrollBar()->setSingleStep(20);
    verticalScrollBar()->setRange(0, 20000);
    horizontalScrollBar()->setRange(0, 300);
    setMouseTracking(true);
}

void WhiteBoardViewer::paintEvent(QPaintEvent *)
{
}

void WhiteBoardViewer::scrollContentsBy(int dx, int dy)
{
    QAbstractScrollArea::scrollContentsBy(dx, dy);
}

} // end of ADEV
