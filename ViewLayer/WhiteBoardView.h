#ifndef WHITEBOARDVIEW_H
#define WHITEBOARDVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "SceneLayer/WhiteBoardScene.h"

namespace ADEV {

class WhiteBoardView : public QWidget
{
    Q_OBJECT
public:
    explicit WhiteBoardView(WhiteBoardScene* scene, QWidget* parent = nullptr);

public slots:
    void changeToNormalPen();
    void changeToHighlightPen();
    void changeToLaserPen();
    void changeToEraser();

private:
    QGraphicsView* m_view;
    WhiteBoardScene* m_scene;

    QPushButton* m_normalPenBtn;
    QPushButton* m_hightlightPenBtn;
    QPushButton* m_laserPenBtn;
    QPushButton* m_eraserBtn;
    QHBoxLayout* m_btnLayout;
    QVBoxLayout* m_primeLayout;
};

} // ADEV


#endif  // WHITEBOARDVIEW_H
