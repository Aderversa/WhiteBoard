#include "WhiteBoardScene.h"
#include "SceneLayer/BackgroundItem.h"

#include <QUndoStack>
#include <QGraphicsSceneMouseEvent>

namespace ADEV {

WhiteBoardScene::WhiteBoardScene(BackgroundItem* background)
    : QGraphicsScene()
    , m_backgroundItem(background)
    , m_undoStack(new QUndoStack)
{
    initNormalPen();
    initHighlightPen();
    initLaserPen();
    initEraser();
    if (m_backgroundItem == nullptr)
    {
        // 这里默认的大小应该由配置文件指定的，固定在配置文件上用户一般不可以更改
        m_backgroundItem = new BackgroundItem(Qt::black, QSizeF(1024, 512));
    }
    // backgroundItem属于Scene的属性，不能够将其归类管理
    this->addItem(m_backgroundItem);
    m_nowUseTool = WhiteBoardTool::NormalPen;
}

WhiteBoardScene::~WhiteBoardScene()
{
    if (m_backgroundItem) {
        delete m_backgroundItem; // 外界必须保证Item是new来的
    }
    if (m_undoStack) {
        delete m_undoStack;  // 其中的undoCommand会被自动地delete
    }
}

void WhiteBoardScene::inputDevicePress(const QPointF& startPos)
{
    // 需要将不同的工具的处理逻辑进行分离
    switch(m_nowUseTool)
    {
    case WhiteBoardTool::NormalPen:
        devicePress(startPos, NormalPenTag{});
        break;
    case WhiteBoardTool::HighlightPen:
        devicePress(startPos, HightlightPenTag{});
        break;
    case WhiteBoardTool::LaserPen:
        devicePress(startPos, LaserPenTag{});
        break;
    case WhiteBoardTool::Eraser:
        devicePress(startPos, EraserTag{});
        break;
    default:
        break;
    }
}

void WhiteBoardScene::inputDeviceMove(const QPointF& scenePos, const QPointF& lastScenePos)
{
    // 需要将不同的工具的处理逻辑进行分离
    switch(m_nowUseTool)
    {
    case WhiteBoardTool::NormalPen:
        deviceMove(scenePos, lastScenePos, NormalPenTag{});
        break;
    case WhiteBoardTool::HighlightPen:
        deviceMove(scenePos, lastScenePos, HightlightPenTag{});
        break;
    case WhiteBoardTool::LaserPen:
        deviceMove(scenePos, lastScenePos, LaserPenTag{});
        break;
    case WhiteBoardTool::Eraser:
        deviceMove(scenePos, lastScenePos, EraserTag{});
        break;
    default:
        break;
    }
}

void WhiteBoardScene::inputDeviceRelease()
{
    // 需要将不同的工具的处理逻辑进行分离
    switch(m_nowUseTool)
    {
    case WhiteBoardTool::NormalPen:
        deviceRelease(NormalPenTag{});
        break;
    case WhiteBoardTool::HighlightPen:
        deviceRelease(HightlightPenTag{});
        break;
    case WhiteBoardTool::LaserPen:
        deviceRelease(LaserPenTag{});
        break;
    case WhiteBoardTool::Eraser:
        deviceRelease(EraserTag{});
        break;
    default:
        break;
    }
}

void WhiteBoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        inputDevicePress(event->scenePos());
    }
}

void WhiteBoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        inputDeviceMove(event->scenePos(), event->lastScenePos());
    }
}

void WhiteBoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        inputDeviceRelease();
    }
}

bool WhiteBoardScene::changeBackground(BackgroundItem* background)
{
    if (dynamic_cast<BackgroundImageItem*>(m_backgroundItem) != nullptr){
        return false;
    }
    delete m_backgroundItem;
    if (background == nullptr) {
        m_backgroundItem = new BackgroundItem(Qt::white, QSizeF(1024, 512));
    }
    else {
        m_backgroundItem = background;
    }
    return true;
}

void WhiteBoardScene::handleCollidingItems(const QPainterPath& collidesArea)
{
    if (m_nowUseTool != WhiteBoardTool::Eraser)
    {
        return;
    }
    QList<QGraphicsItem*> items = this->items(collidesArea);
    for (auto pItem : items)
    {
        if (pItem != m_backgroundItem && pItem != m_eventTempItem)
        {
            BaseGraphicsItem* eraseItem = dynamic_cast<BaseGraphicsItem*>(pItem);
            if (eraseItem != nullptr) {
                EraseItemCommand* command = new EraseItemCommand(this, eraseItem, collidesArea);
                m_eraseItemsCommand->push(command);
            }
        }
    }
}

void WhiteBoardScene::eraseCollidingWholeItems(const QPainterPath& collidesArea)
{
    if (m_nowUseTool != WhiteBoardTool::Eraser) {
        return;
    }
    QList<QGraphicsItem*> items = this->items(collidesArea);
    for (auto pItem : items)
    {
        if (pItem != m_backgroundItem && pItem != m_eventTempItem)
        {
            BaseGraphicsItem* eraseItem = dynamic_cast<BaseGraphicsItem*>(pItem);
            if (eraseItem) {
                DeleteItemCommand* command = new DeleteItemCommand(this, eraseItem);
                m_undoStack->push(command);
            }
        }
    }
}

void WhiteBoardScene:: setNormalPen(const WhiteBoardNormalPen& normalPen)
{
    m_normalPen = normalPen;
}

void WhiteBoardScene::setHighlightPen(const WhiteBoardHighlightPen& hightlightPen)
{
    m_highlightPen = hightlightPen;
}

void WhiteBoardScene::setLaserPen(const WhiteBoardLaserPen& laserPen)
{
    m_laserPen = laserPen;
}

void WhiteBoardScene::setEraser(const WhiteBoardEraser& eraser)
{
    m_eraser = eraser;
}

WhiteBoardNormalPen WhiteBoardScene::normalPen() const
{
    return m_normalPen;
}

WhiteBoardHighlightPen WhiteBoardScene::hightlightPen() const
{
    return m_highlightPen;
}

WhiteBoardLaserPen WhiteBoardScene::laserPen() const
{
    return m_laserPen;
}

WhiteBoardEraser WhiteBoardScene::eraser() const
{
    return m_eraser;
}

void WhiteBoardScene::selectTool(WhiteBoardTool toolType)
{
    m_nowUseTool = toolType;
}

void WhiteBoardScene::initNormalPen()
{
    // 这里因为没有实现配置类，所以采用硬编码的方式来初始化
    // 后续扩展了配置类的时候再回来对此处进行优化
    m_normalPen.color = Qt::red;
    m_normalPen.width = 4;
}

void WhiteBoardScene::initHighlightPen()
{
    // 这里因为没有实现配置类，所以采用硬编码的方式来初始化
    // 后续扩展了配置类的时候再回来对此处进行优化
    m_highlightPen.color = Qt::darkYellow;
    m_highlightPen.width = 40;
    m_highlightPen.opacity = 0.5;
    m_highlightPen.openStraightLineMode = true;
}

void WhiteBoardScene::initLaserPen()
{
    // 这里因为没有实现配置类，所以采用硬编码的方式来初始化
    // 后续扩展了配置类的时候再回来对此处进行优化
    m_laserPen.color = Qt::green;
}

void WhiteBoardScene::initEraser()
{
    // 这里因为没有实现配置类，所以采用硬编码的方式来初始化
    // 后续扩展了配置类的时候再回来对此处进行优化
    m_eraser.radius = 40;
    m_eraser.eraseWholeItem = true;
}


} // end of namespace ADEV
