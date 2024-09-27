#ifndef WHITEBOARDVIEW_H
#define WHITEBOARDVIEW_H

#include <QGraphicsView>

namespace ADEV
{

class WhiteBoardScene;

class WhiteBoardView : public QGraphicsView
{
    Q_OBJECT
public:
    WhiteBoardView(WhiteBoardScene* scene, QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
};

} // ADEV


#endif  // WHITEBOARDVIEW_H
