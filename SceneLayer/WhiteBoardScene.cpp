#include "WhiteBoardScene.h"
#include "SceneLayer/BackgroundItem.h"

#include <QUndoStack>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

namespace ADEV {

WhiteBoardScene::WhiteBoardScene(BackgroundItem* background)
    : QGraphicsScene()
    , m_backgroundItem(background)
    , m_undoStack(new QUndoStack)
    , m_laserItemTempList(3)
{
    initNormalPen();
    initHighlightPen();
    initLaserPen();
    initEraser();

    connect(this, &WhiteBoardScene::toolChanged, &m_laserItemTempList, &LaserStrokeTempList::toolChanged);

    if (m_backgroundItem == nullptr)
    {
        // 这里默认的大小应该由配置文件指定的，固定在配置文件上用户一般不可以更改
        m_backgroundItem.reset(new BackgroundItem(Qt::black, QSizeF(1024, 512)));
    }
    // backgroundItem属于Scene的属性，不能够将其归类管理
    this->addItem(m_backgroundItem.data());
    m_nowUseTool = WhiteBoardTool::NormalPen;
}

WhiteBoardScene::~WhiteBoardScene()
{}

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
    // QGraphicsScene::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        inputDevicePress(event->scenePos());
    }
    else
    {
        event->ignore();
    }
}

void WhiteBoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // QGraphicsScene::mouseMoveEvent(event);
    if (event->buttons() == Qt::LeftButton)
    {
        inputDeviceMove(event->scenePos(), event->lastScenePos());
    }
    else
    {
        event->ignore();
    }
}

void WhiteBoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // QGraphicsScene::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        inputDeviceRelease();
    }
    else
    {
        event->ignore();
    }
}

bool WhiteBoardScene::changeBackground(BackgroundItem* background)
{
    if (dynamic_cast<BackgroundImageItem*>(m_backgroundItem.get()) != nullptr){
        return false;
    }
    m_backgroundItem.reset();
    if (background == nullptr) {
        m_backgroundItem.reset(new BackgroundItem(Qt::white, QSizeF(1024, 512)));
    }
    else {
        m_backgroundItem.reset(background);
    }
    return true;
}

void WhiteBoardScene::handleCollidingItems(const QPainterPath& collidesArea)
{
    if (m_nowUseTool != WhiteBoardTool::Eraser)
    {
        return;
    }
    QList<QGraphicsItem*> items = m_eventTempItem->collidingItems();
    for (auto pItem : items)
    {
        if (pItem != m_backgroundItem.get())
        {
            BaseGraphicsItem* eraseItem = dynamic_cast<BaseGraphicsItem*>(pItem);
            if (eraseItem != nullptr) {
                QSharedPointer<BaseGraphicsItem> pEraseItem = getItem(eraseItem);
                if (!pEraseItem.isNull()) {
                    QSharedPointer<EraseItemCommand> command(new EraseItemCommand(this, pEraseItem, collidesArea));
                    m_eraseItemsCommand->push(command);
                }
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
        if (pItem != m_backgroundItem.get() && pItem != m_eventTempItem)
        {
            BaseGraphicsItem* eraseItem = dynamic_cast<BaseGraphicsItem*>(pItem);
            if (eraseItem) {
                QSharedPointer<BaseGraphicsItem> pEraseItem = getItem(eraseItem);
                DeleteItemCommand* command = new DeleteItemCommand(this, pEraseItem);
                m_undoStack->push(command);
            }
        }
    }
}

void WhiteBoardScene::addItem(const QSharedPointer<BaseGraphicsItem>& pItem)
{
    m_existItemMap[pItem.data()] = pItem.toWeakRef();
    QGraphicsScene::addItem(pItem.data());
}

void WhiteBoardScene::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);
}

QSharedPointer<BaseGraphicsItem> WhiteBoardScene::getItem(BaseGraphicsItem* pItem)
{
    auto it = m_existItemMap.find(pItem);
    if (it == m_existItemMap.end())
    {
        return QSharedPointer<BaseGraphicsItem>(nullptr);
    }
    else
    {
        // 升格失败则返回空指针
        QSharedPointer<BaseGraphicsItem> pointer = m_existItemMap[pItem].toStrongRef();
        if (pointer.isNull()) // 删除无效指针
        {
            m_existItemMap.remove(pItem);
        }
        return pointer;
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
    emit toolChanged(m_nowUseTool);
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
    m_eraser.radius = 70;
    m_eraser.eraseWholeItem = false;
}
// WhiteBoardScene::LaserStrokeTepmList
LaserStrokeTempList::LaserStrokeTempList(int countdownSecond)
    : QObject()
    , m_countdown(countdownSecond * 1000) // 用户指定秒，实际记录成毫秒
{
}

LaserStrokeTempList::~LaserStrokeTempList()
{
}

void LaserStrokeTempList::push(const QSharedPointer<BaseGraphicsItem>& laserStroke)
{
    m_laserList.push_back(laserStroke);
    m_countTimes = 0;
    // 重置透明度
    for (auto& item : m_laserList) {
        item->setOpacity(1);
    }
    m_timer.reset();
}

void LaserStrokeTempList::startTimer()
{
    m_timer.reset(new QTimer(this));
    connect(m_timer.get(), &QTimer::timeout, this, &LaserStrokeTempList::handleTimeout);
    m_timer->start(200);
}

void LaserStrokeTempList::handleTimeout()
{
    int remainCount = m_countdown - m_countTimes;
    switch(remainCount)
    {
    case 0:
        // 释放所有激光笔Item
        m_laserList.clear();
        m_countTimes = 0;
        m_timer.reset();
        break;
    default:
        break;
    }
    m_countTimes += 200;
}

void LaserStrokeTempList::toolChanged(WhiteBoardTool tool)
{
    if (tool != WhiteBoardTool::LaserPen)
    {
        m_laserList.clear();
        m_countTimes = 0;
        m_timer.reset();
    }
}


} // end of namespace ADEV
