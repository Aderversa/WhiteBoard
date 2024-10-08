#ifndef WHITEBOARDVIEWER_H
#define WHITEBOARDVIEWER_H
#include "ViewLayer/MultiPageWidget.h"

#include <QWidget>
#include <QPointer>

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;

namespace ADEV
{

class WhiteBoardScene;
class SceneController;

class WhiteBoardViewer : public QWidget
{
    Q_OBJECT
public:
    WhiteBoardViewer(QWidget* parent = nullptr);
    ~WhiteBoardViewer();
    void addPage(WhiteBoardScene* page);

private slots:
    void openNormalDialog();
    void openHighlightDialog();
    void openLaserDialog();
    void openEraserDialog();
    void openShapeDialog();

private:
    QPointer<SceneController> m_sceneController;
    QPointer<MultiPageWidget> m_pagesWidget;

    QPushButton* m_normal;
    QPushButton* m_highlight;
    QPushButton* m_laser;
    QPushButton* m_eraser;
    QPushButton* m_shape;
    QPushButton* m_rubberBand;
    QPushButton* m_undo;
    QPushButton* m_redo;

    QPushButton* m_normalDialog;
    QPushButton* m_highlightDialog;
    QPushButton* m_laserDialog;
    QPushButton* m_eraserDialog;
    QPushButton* m_shapeDialog;

    QHBoxLayout* m_btnsLayout;
    QVBoxLayout* m_primeLayout;
};

} // end of ADEV

#endif // WHITEBOARDVIEWER_H
