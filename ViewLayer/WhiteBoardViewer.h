#ifndef WHITEBOARDVIEWER_H
#define WHITEBOARDVIEWER_H

#include <QAbstractScrollArea>

namespace ADEV {

class WhiteBoardViewer : public QAbstractScrollArea
{
    Q_OBJECT
public:
    WhiteBoardViewer(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void scrollContentsBy(int dx, int dy) override;
};

} // end of ADEV

#endif // WHITEBOARDVIEWER_H
