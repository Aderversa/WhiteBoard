#include "WhiteBoardViewer.h"
#include "ViewLayer/MultiPageWidget.h"
#include "ViewLayer/WhiteBoardView.h"
#include "SceneLayer/WhiteBoardScene.h"
#include "SceneLayer/SceneController.h"
#include "ToolsDialog/EraserDialog.h"
#include "ToolsDialog/NormalPenDialog.h"
#include "ToolsDialog/HighlightPenDialog.h"
#include "ToolsDialog/LaserPenDialog.h"
#include "ToolsDialog/ShapePenDialog.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace ADEV
{

WhiteBoardViewer::WhiteBoardViewer(QWidget* parent)
    : QWidget(parent)
    , m_sceneController(new SceneController(this))
    , m_pagesWidget(new MultiPageWidget(this))
    , m_btnsLayout(new QHBoxLayout)
    , m_primeLayout(new QVBoxLayout(this))
{
    m_primeLayout->addLayout(m_btnsLayout);
    QHBoxLayout* hLayout = new QHBoxLayout();
    m_primeLayout->addLayout(hLayout);
    m_primeLayout->addWidget(m_pagesWidget.get());

    m_normal = new QPushButton(tr("普通笔"), this);
    m_highlight = new QPushButton(tr("荧光笔"), this);
    m_laser = new QPushButton(tr("激光笔"), this);
    m_eraser = new QPushButton(tr("橡皮擦"), this);
    m_shape = new QPushButton(tr("图形"), this);
    m_rubberBand = new QPushButton(tr("选择框"), this);
    m_undo = new QPushButton(tr("撤销"), this);
    m_redo = new QPushButton(tr("重做"), this);
    m_btnsLayout->addWidget(m_normal);
    m_btnsLayout->addWidget(m_highlight);
    m_btnsLayout->addWidget(m_laser);
    m_btnsLayout->addWidget(m_eraser);
    m_btnsLayout->addWidget(m_shape);
    m_btnsLayout->addWidget(m_rubberBand);
    m_btnsLayout->addWidget(m_undo);
    m_btnsLayout->addWidget(m_redo);

    m_normalDialog = new QPushButton(tr("普通笔选项"), this);
    m_highlightDialog = new QPushButton(tr("荧光笔选项"), this);
    m_laserDialog = new QPushButton(tr("激光笔选项"), this);
    m_eraserDialog = new QPushButton(tr("橡皮擦选项"), this);
    m_shapeDialog = new QPushButton(tr("图形笔选项"), this);
    hLayout->addWidget(m_normalDialog);
    hLayout->addWidget(m_highlightDialog);
    hLayout->addWidget(m_laserDialog);
    hLayout->addWidget(m_eraserDialog);
    hLayout->addWidget(m_shapeDialog);

    connect(m_normal, &QPushButton::clicked, m_sceneController.get(), &SceneController::useNormalPen);
    connect(m_highlight, &QPushButton::clicked, m_sceneController.get(), &SceneController::useHighlightPen);
    connect(m_laser, &QPushButton::clicked, m_sceneController.get(), &SceneController::useLaserPen);
    connect(m_eraser, &QPushButton::clicked, m_sceneController.get(), &SceneController::useEraser);
    connect(m_shape, &QPushButton::clicked, m_sceneController.get(), &SceneController::useShapePen);
    connect(m_rubberBand, &QPushButton::clicked, m_sceneController.get(), &SceneController::useRubberBand);
    connect(m_undo, &QPushButton::clicked, m_sceneController->undoStack().get(), &QUndoStack::undo);
    connect(m_redo, &QPushButton::clicked, m_sceneController->undoStack().get(), &QUndoStack::redo);

    connect(m_normalDialog, &QPushButton::clicked, this, &WhiteBoardViewer::openNormalDialog);
    connect(m_highlightDialog, &QPushButton::clicked, this, &WhiteBoardViewer::openHighlightDialog);
    connect(m_laserDialog, &QPushButton::clicked, this, &WhiteBoardViewer::openLaserDialog);
    connect(m_eraserDialog, &QPushButton::clicked, this, &WhiteBoardViewer::openEraserDialog);
    connect(m_shapeDialog, &QPushButton::clicked, this, &WhiteBoardViewer::openShapeDialog);
}

WhiteBoardViewer::~WhiteBoardViewer()
{
}

void WhiteBoardViewer::addPage(WhiteBoardScene* page)
{
    page->setController(m_sceneController.get());
    WhiteBoardView* w = new WhiteBoardView(page);
    m_pagesWidget->addWidget(w);
}

void WhiteBoardViewer::openNormalDialog()
{
    NormalPenDialog dlg(m_sceneController.get(), this);
    dlg.exec();
}

void WhiteBoardViewer::openHighlightDialog()
{
    HighlightPenDialog dlg(m_sceneController.get(), this);
    dlg.exec();
}

void WhiteBoardViewer::openLaserDialog()
{
    LaserPenDialog dlg(m_sceneController.get(), this);
    dlg.exec();
}

void WhiteBoardViewer::openEraserDialog()
{
    EraserDialog dlg(m_sceneController.get(), this);
    dlg.exec();
}

void WhiteBoardViewer::openShapeDialog()
{
    ShapePenDialog dlg(m_sceneController.get(), this);
    dlg.exec();
}

} // end of ADEV
