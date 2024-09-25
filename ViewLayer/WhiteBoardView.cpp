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
    , m_rubberBandBtn(new QPushButton(tr("选择框"), this))
    , m_shapeBtn(new QPushButton(tr("图形"), this))
    , m_undoBtn(new QPushButton(tr("撤销"), this))
    , m_redoBtn(new QPushButton(tr("重做"), this))
    , m_saveBtn(new QPushButton(tr("保存到桌面"), this))
    , m_btnLayout(new QHBoxLayout)
    , m_primeLayout(new QVBoxLayout(this))
    , m_pageLayout(new MultiPageWidget(this))
{
    int n = 10;
    QImage image(QString(":/BackgroundImages/3.png"));
    for (int i = 0; i < n; i++)
    {
        BackgroundImageItem* backgroundItem = new BackgroundImageItem(image, image.size());
        WhiteBoardScene* m_scene = new WhiteBoardScene(backgroundItem);
        QRectF rect(0, 0, 1024, 512);
        m_scene->setSceneRect(rect);
        connect(m_normalPenBtn, &QPushButton::clicked, m_scene, &WhiteBoardScene::useNormalPen);
        connect(m_hightlightPenBtn, &QPushButton::clicked, m_scene, &WhiteBoardScene::useHighlightPen);
        connect(m_laserPenBtn, &QPushButton::clicked, m_scene, &WhiteBoardScene::useLaserPen);
        connect(m_eraserBtn, &QPushButton::clicked, m_scene, &WhiteBoardScene::useEraser);
        connect(m_shapeBtn, &QPushButton::clicked, m_scene, &WhiteBoardScene::useShapePen);
        connect(m_rubberBandBtn, &QPushButton::clicked, m_scene, &WhiteBoardScene::useRubberBand);
        connect(m_undoBtn, &QPushButton::clicked, m_scene->undoStack().get(), &QUndoStack::undo);
        connect(m_redoBtn, &QPushButton::clicked, m_scene->undoStack().get(), &QUndoStack::redo);
        m_pageLayout->addScene(m_scene);
    }
    m_btnLayout->addWidget(m_normalPenBtn);
    m_btnLayout->addWidget(m_hightlightPenBtn);
    m_btnLayout->addWidget(m_laserPenBtn);
    m_btnLayout->addWidget(m_eraserBtn);
    m_btnLayout->addWidget(m_shapeBtn);
    m_btnLayout->addWidget(m_rubberBandBtn);
    m_btnLayout->addWidget(m_undoBtn);
    m_btnLayout->addWidget(m_redoBtn);
    m_btnLayout->addWidget(m_saveBtn);
    m_primeLayout->addLayout(m_btnLayout);
    m_primeLayout->addWidget(m_pageLayout);
}

void WhiteBoardView::printIndex(int index)
{
    qDebug() << index;
}

} // ADEV
