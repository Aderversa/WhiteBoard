#ifndef WHITEBOARDVIEW_H
#define WHITEBOARDVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPointer>

#include "SceneLayer/WhiteBoardScene.h"

namespace ADEV {

class WhiteBoardView : public QWidget
{
    Q_OBJECT
public:
    explicit WhiteBoardView(WhiteBoardScene* scene, QWidget* parent = nullptr);
    ~WhiteBoardView()
    {
    }

public slots:
    void changeToNormalPen();
    void changeToHighlightPen();
    void changeToLaserPen();
    void changeToEraser();
    void printIndex(int index);
    void save();

private:
    QGraphicsView* m_view;
    QPointer<WhiteBoardScene> m_scene;

    QPushButton* m_normalPenBtn;
    QPushButton* m_hightlightPenBtn;
    QPushButton* m_laserPenBtn;
    QPushButton* m_eraserBtn;
    QPushButton* m_shapeBtn;
    QPushButton* m_rubberBandBtn;
    QPushButton* m_undoBtn;
    QPushButton* m_redoBtn;
    QPushButton* m_saveBtn;
    QHBoxLayout* m_btnLayout;
    QVBoxLayout* m_primeLayout;
};

} // ADEV


#endif  // WHITEBOARDVIEW_H
