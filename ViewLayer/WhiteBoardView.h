#ifndef WHITEBOARDVIEW_H
#define WHITEBOARDVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPointer>

#include "SceneLayer/WhiteBoardScene.h"
#include "ViewLayer/MultiPageWidget.h"

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
    void printIndex(int index);

private:

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
    MultiPageWidget* m_pageLayout;
};

} // ADEV


#endif  // WHITEBOARDVIEW_H
