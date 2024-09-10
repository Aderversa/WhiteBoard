#include "ViewLayer/WhiteBoardView.h"

#include <QDebug>

namespace ADEV {

WhiteBoardView::WhiteBoardView(WhiteBoardScene *scene, QWidget *parent)
    : QWidget{parent}
    , m_scene(scene)
    , m_view(new QGraphicsView(scene))
    , m_normalPenBtn(new QPushButton(tr("普通笔")))
    , m_hightlightPenBtn(new QPushButton(tr("荧光笔")))
    , m_laserPenBtn(new QPushButton(tr("激光笔(待完成)")))
    , m_eraserBtn(new QPushButton(tr("橡皮擦(待完成)")))
    , m_btnLayout(new QHBoxLayout)
    , m_primeLayout(new QVBoxLayout(this))
{
    m_btnLayout->addWidget(m_normalPenBtn);
    m_btnLayout->addWidget(m_hightlightPenBtn);
    m_btnLayout->addWidget(m_laserPenBtn);
    m_btnLayout->addWidget(m_eraserBtn);
    m_primeLayout->addLayout(m_btnLayout);
    m_primeLayout->addWidget(m_view);

    connect(m_normalPenBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToNormalPen);
    connect(m_hightlightPenBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToHighlightPen);
    connect(m_laserPenBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToLaserPen);
    connect(m_eraserBtn, &QPushButton::clicked, this, &WhiteBoardView::changeToEraser);
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

}

void WhiteBoardView::changeToEraser()
{
    m_scene->selectTool(WhiteBoardTool::Eraser);
}


} // ADEV
