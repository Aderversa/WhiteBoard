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
{
    if (m_backgroundItem == nullptr)
    {
        // 这里默认的大小应该由配置文件指定的，固定在配置文件上用户一般不可以更改
        m_backgroundItem.reset(new BackgroundItem(Qt::white, QSizeF(1024, 512)));
    }
    // backgroundItem属于Scene的属性，不能够将其归类管理
    this->addItem(m_backgroundItem.data());
}

WhiteBoardScene::~WhiteBoardScene()
{
}

void WhiteBoardScene::inputDevicePress(const QPointF& startPos)
{
    // 需要将不同的工具的处理逻辑进行分离
    m_tool->devicePress(startPos);
}

void WhiteBoardScene::inputDeviceMove(const QPointF& scenePos, const QPointF& lastScenePos)
{
    m_tool->deviceMove(scenePos, lastScenePos);
}

void WhiteBoardScene::inputDeviceRelease()
{
    m_tool->deviceRelease();
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

void WhiteBoardScene::handleCollidingItems(const QPainterPath& collidesArea,
                                           const QSharedPointer<BaseGraphicsItem>& pointer,
                                           EraseItemsCommand* commands)
{
    if (dynamic_cast<WhiteBoardEraser*>(m_tool.data()) == nullptr) {
        return;
    }
    QList<QGraphicsItem*> items = this->items(collidesArea);
    for (auto pItem : items)
    {
        if (pItem != m_backgroundItem.get() && pItem != pointer.get())
        {
            BaseGraphicsItem* eraseItem = dynamic_cast<BaseGraphicsItem*>(pItem);
            if (eraseItem != nullptr) {
                QSharedPointer<BaseGraphicsItem> pEraseItem = getItem(eraseItem);
                if (!pEraseItem.isNull()) {
                    QSharedPointer<EraseItemCommand> command(new EraseItemCommand(this, pEraseItem, pointer->shape()));
                    commands->push(command);
                }
            }
        }
    }
}

void WhiteBoardScene::eraseCollidingWholeItems(const QPainterPath& collidesArea,
                                               const QSharedPointer<BaseGraphicsItem>& pointer)
{
    if (dynamic_cast<WhiteBoardEraser*>(m_tool.get()) == nullptr) {
        return;
    }
    QList<QGraphicsItem*> items = this->items(collidesArea);
    for (auto pItem : items)
    {
        if (pItem != m_backgroundItem.get() && pItem != pointer.get())
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

void WhiteBoardScene::useNormalPen()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardNormalPen*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardNormalPen(this));
        emit toolChanged();
    }
}

void WhiteBoardScene::useHighlightPen()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardHighlightPen*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardHighlightPen(this));
        emit toolChanged();
    }
}

void WhiteBoardScene::useLaserPen()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardLaserPen*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardLaserPen(this));
        emit toolChanged();
    }
}

void WhiteBoardScene::useEraser()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardEraser*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardEraser(this));
        emit toolChanged();
    }
}

// ------------------------------------------------------------------------------------
// 工具相关的类
WhiteBoardNormalPen::WhiteBoardNormalPen(WhiteBoardScene* scene)
    : WhiteBoardAbstractTool(scene)
    , m_width(4)
    , m_color(Qt::red)
{
}

void WhiteBoardNormalPen::devicePress(const QPointF& startPos)
{
    m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(m_width, QBrush(m_color)));
    m_eventTempItem->setZValue(m_scene->backgroundZValue()  + 1);
    // 添加操作使用Command进行
    AddItemCommand* command = new AddItemCommand(m_scene.get(), m_eventTempItem);
    m_scene->undoStack()->push(command);
    m_curveObserver = QSharedPointer<ControlCurveObserver>(new ControlCurveObserver(m_eventTempItem.data()));
    // 后面的QSizeF是随便给的，因为我知道ControlCurveObserver在其formItem只处理leftTop点
    m_curveObserver->addPointToCurve(startPos);
}

void WhiteBoardNormalPen::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(lastScenePos);
    if (m_curveObserver.isNull())
        return;
    m_curveObserver->addPointToCurve(scenePos);
}

void WhiteBoardNormalPen::deviceRelease()
{
    // 同一时间只处理同一个工具的一套(press-move-release)流程，所以不用担心同步问题
    m_curveObserver.reset(nullptr);
    // item已经在本次事件中塑造完成，后续交给Scene管理
    // m_eventTempItem不再能操作已经塑造完成的Item
    m_eventTempItem.reset(nullptr);
}


WhiteBoardHighlightPen::WhiteBoardHighlightPen(WhiteBoardScene* scene)
    : WhiteBoardAbstractTool(scene)
    , m_color(Qt::yellow)
    , m_width(40)
    , m_opacity(0.5)
    , m_openStraightLineMode(true)
{
}

void WhiteBoardHighlightPen::devicePress(const QPointF& startPos)
{
    m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(m_width, QBrush(m_color)));
    m_eventTempItem->setZValue(m_scene->backgroundZValue()  + 1);
    m_eventTempItem->setOpacity(m_opacity);
    // 添加操作使用Command进行
    AddItemCommand* command = new AddItemCommand(m_scene.get(), m_eventTempItem);
    m_scene->undoStack()->push(command); // add操作真正执行是在push方法内
    if (!m_openStraightLineMode)
    {
        m_curveObserver = QSharedPointer<ControlCurveObserver>(new ControlCurveObserver(m_eventTempItem.data()));
    }
    else
    {
        m_curveObserver = QSharedPointer<ControlCurveObserver>(new ControlLineObserver(m_eventTempItem.data()));
    }
    m_curveObserver->addPointToCurve(startPos);
}

void WhiteBoardHighlightPen::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(lastScenePos);
    if (m_curveObserver.isNull())
        return;
    m_curveObserver->addPointToCurve(scenePos);
}

void WhiteBoardHighlightPen::deviceRelease()
{
    m_curveObserver.reset(nullptr);
    // item已经在本次事件中塑造完成，后续交给Scene管理
    // m_eventTempItem不再能操作已经塑造完成的Item
    m_eventTempItem.reset(nullptr);
}

WhiteBoardLaserPen::WhiteBoardLaserPen(WhiteBoardScene* scene)
    : WhiteBoardAbstractTool(scene)
    , m_color(Qt::red)
    , m_laserItemTempList(6)
{
    connect(m_scene.get(), &WhiteBoardScene::toolChanged, &m_laserItemTempList, &LaserStrokeTempList::toolChanged);
}

void WhiteBoardLaserPen::devicePress(const QPointF& startPos)
{
    QSharedPointer<BaseGraphicsItem> laserItem(new BaseGraphicsItem(FIXED_WIDTH, QBrush(Qt::white)));
    laserItem->setZValue(m_scene->backgroundZValue()  + 2);
    // 添加阴影效果
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
    effect->setOffset(0, 0);
    effect->setBlurRadius(15);
    effect->setColor(m_color);
    laserItem->setGraphicsEffect(effect);
    m_scene->addItem(laserItem.data());
    // 加入新Item, 重置计时时间
    m_laserItemTempList.push(laserItem);

    m_curveObserver = QSharedPointer<ControlCurveObserver>(new ControlCurveObserver(laserItem.data()));
    m_curveObserver->addPointToCurve(startPos);

}

void WhiteBoardLaserPen::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(lastScenePos)
    if (m_curveObserver.isNull())
        return;
    m_curveObserver->addPointToCurve(scenePos);
}

void WhiteBoardLaserPen::deviceRelease()
{
    m_curveObserver.reset(nullptr);
    // 操作结束，开始计时
    m_laserItemTempList.startTimer();
}


WhiteBoardEraser::WhiteBoardEraser(WhiteBoardScene* scene)
    : WhiteBoardAbstractTool(scene)
    , m_radius(60)
    , m_eraseWholeItem(false)
{
}

void WhiteBoardEraser::devicePress(const QPointF& startPos)
{
    if (!m_eraseWholeItem)
    {
        m_eraseItemsCommand = new EraseItemsCommand;
        m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(m_radius, QBrush(Qt::darkGray)));
        m_eventTempItem->setZValue(m_scene->backgroundZValue() + 2);
        m_eventTempItem->setOpacity(0.6);

        m_scene->addItem(m_eventTempItem.data()); // 临时Item不需要加入UndoStack
        QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(startPos, startPos), m_radius);
        // 橡皮擦是临时用来显示的Item，不需要使用Command
        m_eventTempItem->setStrokePath(circle);

        m_scene->handleCollidingItems(circle, m_eventTempItem, m_eraseItemsCommand);
    }
    else
    {
        m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(MIN_RADIUS, QBrush(Qt::black)));

        m_scene->addItem(m_eventTempItem.data()); // 临时Item不需要加入UndoStack
        QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(startPos, startPos), MIN_RADIUS);
        m_eventTempItem->setStrokePath(circle);

        m_scene->eraseCollidingWholeItems(circle, m_eventTempItem);
    }
}

void WhiteBoardEraser::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos)
{
    if (m_eventTempItem.isNull()){
        return;
    }
    qreal radius;
    if (!m_eraseWholeItem)
        radius = m_radius;
    else
        radius = MIN_RADIUS;
    QPainterPath track = m_eventTempItem->lineToStroke(QLineF(lastScenePos, scenePos), radius);
    if (!m_eraseWholeItem)
        m_scene->handleCollidingItems(track, m_eventTempItem, m_eraseItemsCommand);
    else
        m_scene->eraseCollidingWholeItems(track, m_eventTempItem);
    QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(scenePos, scenePos), radius);
    m_eventTempItem->setStrokePath(circle);
    m_scene->update();
}

void WhiteBoardEraser::deviceRelease()
{
    m_eventTempItem.reset(nullptr);
    if (!m_eraseWholeItem)
    {
        if (m_eraseItemsCommand->size() != 0) {
            m_eraseItemsCommand->undo();
            m_scene->undoStack()->push(m_eraseItemsCommand);
            m_eraseItemsCommand = nullptr;
        }
    }
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

void LaserStrokeTempList::toolChanged()
{
    m_laserList.clear();
    m_countTimes = 0;
    m_timer.reset();
}


} // end of namespace ADEV
