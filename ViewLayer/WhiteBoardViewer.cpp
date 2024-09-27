#include "WhiteBoardViewer.h"
#include "ViewLayer/MultiPageWidget.h"
#include "ViewLayer/WhiteBoardView.h"
#include "SceneLayer/WhiteBoardScene.h"
#include "SceneLayer/SceneController.h"

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

    connect(m_normal, &QPushButton::clicked, m_sceneController.get(), &SceneController::useNormalPen);
    connect(m_highlight, &QPushButton::clicked, m_sceneController.get(), &SceneController::useHighlightPen);
    connect(m_laser, &QPushButton::clicked, m_sceneController.get(), &SceneController::useLaserPen);
    connect(m_eraser, &QPushButton::clicked, m_sceneController.get(), &SceneController::useEraser);
    connect(m_shape, &QPushButton::clicked, m_sceneController.get(), &SceneController::useShapePen);
    connect(m_rubberBand, &QPushButton::clicked, m_sceneController.get(), &SceneController::useRubberBand);
    connect(m_undo, &QPushButton::clicked, m_sceneController->undoStack().get(), &QUndoStack::undo);
    connect(m_redo, &QPushButton::clicked, m_sceneController->undoStack().get(), &QUndoStack::redo);
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

} // end of ADEV
