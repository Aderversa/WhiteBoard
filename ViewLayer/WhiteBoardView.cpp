#include "ViewLayer/WhiteBoardView.h"

#include <QImage>
#include <QDebug>
#include <QPaintEvent>

namespace ADEV {

WhiteBoardView::WhiteBoardView(WhiteBoardScene *scene, QWidget *parent)
    : QWidget{parent}
    , m_normalPenBtn(new QPushButton(tr("普通笔"), this))
    , m_hightlightPenBtn(new QPushButton(tr("荧光笔"), this))
    , m_laserPenBtn(new QPushButton(tr("激光笔"), this))
    , m_eraserBtn(new QPushButton(tr("橡皮擦"), this))
    , m_undoBtn(new QPushButton(tr("撤销"), this))
    , m_redoBtn(new QPushButton(tr("重做"), this))
    , m_saveBtn(new QPushButton(tr("保存到桌面"), this))
    , m_btnLayout(new QHBoxLayout)
    , m_primeLayout(new QVBoxLayout(this))
{
    m_scene = scene;
    m_view = new QGraphicsView(m_scene.get(), this);
    m_btnLayout->addWidget(m_normalPenBtn);
    m_btnLayout->addWidget(m_hightlightPenBtn);
    m_btnLayout->addWidget(m_laserPenBtn);
    m_btnLayout->addWidget(m_eraserBtn);
    m_btnLayout->addWidget(m_undoBtn);
    m_btnLayout->addWidget(m_redoBtn);
    m_btnLayout->addWidget(m_saveBtn);
    m_primeLayout->addLayout(m_btnLayout);
    m_primeLayout->addWidget(m_view);

    connect(m_normalPenBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToNormalPen);
    connect(m_hightlightPenBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToHighlightPen);
    connect(m_laserPenBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToLaserPen);
    connect(m_eraserBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToEraser);
    connect(m_undoBtn, &QPushButton::clicked, m_scene->undoStack().get(), &QUndoStack::undo);
    connect(m_redoBtn, &QPushButton::clicked, m_scene->undoStack().get(), &QUndoStack::redo);
    connect(m_scene->undoStack().get(), &QUndoStack::indexChanged, this, &WhiteBoardView::printIndex);
    connect(m_saveBtn, &QPushButton::clicked, this, &WhiteBoardView::save);

}

void WhiteBoardView::changeToNormalPen()
{
    m_scene->selectTool(WhiteBoardTool::NormalPen);
}

void WhiteBoardView::changeToHighlightPen()
{
    m_scene->selectTool(WhiteBoardTool::HighlightPen);
}

void WhiteBoardView::changeToLaserPen()
{
    m_scene->selectTool(WhiteBoardTool::LaserPen);
}

void WhiteBoardView::changeToEraser()
{
    m_scene->selectTool(WhiteBoardTool::Eraser);
}

void WhiteBoardView::save()
{
    QImage image(m_scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    QPainter painter(&image);
    m_scene->render(&painter);
    image.save(tr("E://Desktop//whiteboard.png"));
}

void WhiteBoardView::printIndex(int index)
{
    qDebug() << index;
}

} // ADEV
