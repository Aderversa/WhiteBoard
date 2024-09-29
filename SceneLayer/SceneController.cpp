#include "SceneController.h"
#include "Config/WhiteBoardSettings.h"

#include <QTimer>
#include <QUndoStack>

namespace ADEV {

SceneController::SceneController(QObject *parent)
    : QObject{parent}
    , m_undoStack(new QUndoStack(this))
{
    useNormalPen();
}

QSharedPointer<WhiteBoardAbstractTool> SceneController::tool() const
{
    return m_tool;
}

void SceneController::reloadToolSettings()
{
    m_tool->loadSettings();
}

void SceneController::useNormalPen()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardNormalPen*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardNormalPen(this));
        emit toolChanged();
    }
}

void SceneController::useHighlightPen()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardHighlightPen*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardHighlightPen(this));
        emit toolChanged();
    }
}

void SceneController::useLaserPen()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardLaserPen*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardLaserPen(this));
        emit toolChanged();
    }
}

void SceneController::useEraser()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardEraser*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardEraser(this));
        emit toolChanged();
    }
}
void SceneController::useShapePen()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardShapePen*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardShapePen(this));
        emit toolChanged();
    }
}
void SceneController::useRubberBand()
{
    // dynamic_cast失败说明本来就是这个工具与原本的工具不同，需要更换
    if (dynamic_cast<WhiteBoardRubberBand*>(m_tool.get()) == nullptr) {
        m_tool.reset(new WhiteBoardRubberBand(this));
        emit toolChanged();
    }
}


WhiteBoardNormalPen::WhiteBoardNormalPen(SceneController* controller)
    : WhiteBoardAbstractTool(controller)
{
    loadSettings();
}

void WhiteBoardNormalPen::loadSettings()
{
    WhiteBoardSettings& settings = WhiteBoardSettings::getInstance();
    QString prefix("Tools/NormalPen/");
    m_width = settings.value(prefix + "width", 4).value<qreal>();
    m_color = settings.value(prefix + "color", QColor(Qt::red)).value<QColor>();
}

void WhiteBoardNormalPen::devicePress(WhiteBoardScene* scene, const QPointF& startPos)
{
    m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(m_width, QBrush(m_color)));
    m_eventTempItem->setZValue(scene->backgroundZValue()  + 1);
    // 添加操作使用Command进行
    AddItemCommand* command = new AddItemCommand(scene, m_eventTempItem);
    controller()->undoStack()->push(command);
    m_curveObserver = QSharedPointer<ControlCurveObserver>(new ControlCurveObserver(m_eventTempItem.data()));
    // 后面的QSizeF是随便给的，因为我知道ControlCurveObserver在其formItem只处理leftTop点
    m_curveObserver->addPointToCurve(startPos);
}

void WhiteBoardNormalPen::deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(scene);
    Q_UNUSED(lastScenePos);
    if (m_curveObserver.isNull())
        return;
    m_curveObserver->addPointToCurve(scenePos);
}

void WhiteBoardNormalPen::deviceRelease(WhiteBoardScene* scene)
{
    Q_UNUSED(scene);
    // 同一时间只处理同一个工具的一套(press-move-release)流程，所以不用担心同步问题
    m_curveObserver.reset(nullptr);
    // item已经在本次事件中塑造完成，后续交给Scene管理
    // m_eventTempItem不再能操作已经塑造完成的Item
    m_eventTempItem.reset(nullptr);
}


WhiteBoardHighlightPen::WhiteBoardHighlightPen(SceneController* controller)
    : WhiteBoardAbstractTool(controller)
    , m_color(Qt::yellow)
    , m_width(20)
    , m_opacity(0.5)
    , m_openStraightLineMode(false)
{
}

void WhiteBoardHighlightPen::loadSettings()
{
    WhiteBoardSettings& settings = WhiteBoardSettings::getInstance();
    QString prefix("Tools/HighlightPen/");
    m_color = settings.value(prefix + "color", QColor(Qt::yellow)).value<QColor>();
    m_width = settings.value(prefix + "width", 20).value<qreal>();
    m_opacity = settings.value(prefix + "opacity", 0.5).value<qreal>();
    m_openStraightLineMode = settings.value(prefix + "openStraightLineMode", false).toBool();
}

void WhiteBoardHighlightPen::devicePress(WhiteBoardScene* scene, const QPointF& startPos)
{
    m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(m_width, QBrush(m_color)));
    m_eventTempItem->setZValue(scene->backgroundZValue()  + 1);
    m_eventTempItem->setOpacity(m_opacity);
    // 添加操作使用Command进行
    AddItemCommand* command = new AddItemCommand(scene, m_eventTempItem);
    controller()->undoStack()->push(command); // add操作真正执行是在push方法内
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

void WhiteBoardHighlightPen::deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(scene);
    Q_UNUSED(lastScenePos);
    if (m_curveObserver.isNull())
        return;
    if (m_eventTempItem->shape().length() < 100000)
        m_curveObserver->addPointToCurve(scenePos);
}

void WhiteBoardHighlightPen::deviceRelease(WhiteBoardScene* scene)
{
    Q_UNUSED(scene);
    qDebug() << m_eventTempItem->shape().length();
    m_curveObserver.reset(nullptr);
    // item已经在本次事件中塑造完成，后续交给Scene管理
    // m_eventTempItem不再能操作已经塑造完成的Item
    m_eventTempItem.reset(nullptr);
}

WhiteBoardLaserPen::WhiteBoardLaserPen(SceneController* controller)
    : WhiteBoardAbstractTool(controller)
    , m_laserItemTempList(3)
{
    connect(controller, &SceneController::toolChanged, &m_laserItemTempList, &LaserStrokeTempList::toolChanged);
    loadSettings();
}

void WhiteBoardLaserPen::loadSettings()
{
    WhiteBoardSettings& settings = WhiteBoardSettings::getInstance();
    QString prefix("Tools/LaserPen/");
    m_color = settings.value(prefix + "color", QColor(Qt::red)).value<QColor>();
}

void WhiteBoardLaserPen::devicePress(WhiteBoardScene* scene, const QPointF& startPos)
{
    QSharedPointer<BaseGraphicsItem> laserItem(new BaseGraphicsItem(FIXED_WIDTH, QBrush(Qt::white)));
    laserItem->setZValue(scene->backgroundZValue()  + 2);
    // 添加阴影效果
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
    effect->setOffset(0, 0);
    effect->setBlurRadius(15);
    effect->setColor(m_color);
    laserItem->setGraphicsEffect(effect);
    scene->addItem(laserItem.data());
    // 加入新Item, 重置计时时间
    m_laserItemTempList.push(laserItem);
    m_curveObserver = QSharedPointer<ControlCurveObserver>(new ControlCurveObserver(laserItem.data()));
    m_curveObserver->addPointToCurve(startPos);

}

void WhiteBoardLaserPen::deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(scene);
    Q_UNUSED(lastScenePos);
    if (m_curveObserver.isNull())
        return;
    m_curveObserver->addPointToCurve(scenePos);
}

void WhiteBoardLaserPen::deviceRelease(WhiteBoardScene* scene)
{
    Q_UNUSED(scene);
    m_curveObserver.reset(nullptr);
    // 操作结束，开始计时
    m_laserItemTempList.startTimer();
}


WhiteBoardEraser::WhiteBoardEraser(SceneController* controller)
    : WhiteBoardAbstractTool(controller)
{
    loadSettings();
}

void WhiteBoardEraser::loadSettings()
{
    WhiteBoardSettings& settings = WhiteBoardSettings::getInstance();
    QString prefix("Tools/Eraser/");
    m_radius = settings.value(prefix + "radius", 100).value<qreal>();
    m_eraseWholeItem = settings.value(prefix + "eraseWholeItem", false).toBool();
}

void WhiteBoardEraser::handleCollidingItems(WhiteBoardScene* scene,
                                            const QPainterPath& collidesArea,
                                            const QSharedPointer<BaseGraphicsItem>& pointer,
                                            EraseItemsCommand* commands)
{
    if (dynamic_cast<WhiteBoardEraser*>(controller()->tool().get()) == nullptr) {
        return;
    }
    QList<QGraphicsItem*> items = scene->items(collidesArea);
    for (auto pItem : items)
    {
        if (pItem != scene->background() && pItem != pointer.get())
        {
            BaseGraphicsItem* eraseItem = dynamic_cast<BaseGraphicsItem*>(pItem);
            if (eraseItem != nullptr) {
                QSharedPointer<BaseGraphicsItem> pEraseItem = scene->getItem(eraseItem);
                if (!pEraseItem.isNull()) {
                    QSharedPointer<EraseItemCommand> command(new EraseItemCommand(scene, pEraseItem, collidesArea));
                    commands->push(command);
                }
            }
        }
    }
}

void WhiteBoardEraser::eraseCollidingWholeItems(WhiteBoardScene* scene,
                                                const QPainterPath& collidesArea,
                                                const QSharedPointer<BaseGraphicsItem>& pointer)
{
    if (dynamic_cast<WhiteBoardEraser*>(controller()->tool().get()) == nullptr) {
        return;
    }
    QList<QGraphicsItem*> items = scene->items(collidesArea);
    for (auto pItem : items)
    {
        if (pItem != scene->background() && pItem != pointer.get())
        {
            BaseGraphicsItem* eraseItem = dynamic_cast<BaseGraphicsItem*>(pItem);
            if (eraseItem) {
                QSharedPointer<BaseGraphicsItem> pEraseItem = scene->getItem(eraseItem);
                DeleteItemCommand* command = new DeleteItemCommand(scene, pEraseItem);
                controller()->undoStack()->push(command);
            }
        }
    }
}

void WhiteBoardEraser::devicePress(WhiteBoardScene* scene, const QPointF& startPos)
{
    if (!m_eventTempItem.isNull()) {
        return;
    }
    if (!m_eraseWholeItem)
    {
        m_eraseItemsCommand = new EraseItemsCommand;
        m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(m_radius, QBrush(Qt::darkGray)));
        m_eventTempItem->setZValue(scene->backgroundZValue() + 2);
        m_eventTempItem->setOpacity(0.6);
        scene->addItem(m_eventTempItem.data()); // 临时Item不需要加入UndoStack
        QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(startPos, startPos), m_radius);
        // 橡皮擦是临时用来显示的Item，不需要使用Command
        m_eventTempItem->setStrokePath(circle);
        handleCollidingItems(scene, circle, m_eventTempItem, m_eraseItemsCommand);
    }
    else
    {
        m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(MIN_RADIUS, QBrush(Qt::black)));

        scene->addItem(m_eventTempItem.data()); // 临时Item不需要加入UndoStack
        QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(startPos, startPos), MIN_RADIUS);
        m_eventTempItem->setStrokePath(circle);

        eraseCollidingWholeItems(scene, circle, m_eventTempItem);
    }
}

void WhiteBoardEraser::deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos)
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
        handleCollidingItems(scene, track, m_eventTempItem, m_eraseItemsCommand);
    else
        eraseCollidingWholeItems(scene, track, m_eventTempItem);
    QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(scenePos, scenePos), radius);
    m_eventTempItem->setStrokePath(circle);
    scene->update();
}

void WhiteBoardEraser::deviceRelease(WhiteBoardScene* scene)
{
    Q_UNUSED(scene);
    m_eventTempItem.reset(nullptr);
    if (!m_eraseWholeItem)
    {
        if (m_eraseItemsCommand && m_eraseItemsCommand->size() != 0) {
            m_eraseItemsCommand->undo();
            controller()->undoStack()->push(m_eraseItemsCommand);
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
    connect(m_timer.get(), &QTimer::timeout, this, &LaserStrokeTempList::handleTimeout);
    m_timer->start(unit);
}

void LaserStrokeTempList::handleTimeout()
{
    int remainCount = m_countdown - m_countTimes;
    qreal opacity = remainCount / 500.0;
    if (opacity > 1) {
        remainCount = 1;
    }
    else if (opacity < 0) {
        remainCount = 0;
    }
    for (auto& pItem : m_laserList)
    {
        pItem->setOpacity(opacity);
    }
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
    m_countTimes += unit;
}

void LaserStrokeTempList::toolChanged()
{
    m_laserList.clear();
    m_countTimes = 0;
    m_timer.reset();
}

WhiteBoardShapePen::WhiteBoardShapePen(SceneController* controller)
    : WhiteBoardAbstractTool(controller)
{
    loadSettings();
}

void WhiteBoardShapePen::loadSettings()
{
    WhiteBoardSettings& settings = WhiteBoardSettings::getInstance();
    QString prefix("Tools/ShapePen/");
    m_width = settings.value(prefix + "width", 6).value<qreal>();
    m_opacity = settings.value(prefix + "opacity", 1).value<qreal>();
    m_color = settings.value(prefix + "color", QColor(Qt::red)).value<QColor>();
    m_shape = settings.value(prefix + "shape", Rectangle).value<WhiteBoardShapePen::ItemShape>();
}

void WhiteBoardShapePen::devicePress(WhiteBoardScene* scene, const QPointF& startPos)
{
    m_eventTempItem = QSharedPointer<BaseGraphicsItem>(new BaseGraphicsItem(m_width, QBrush(m_color)));

    AddItemCommand* command = new AddItemCommand(scene, m_eventTempItem);
    controller()->undoStack()->push(command);

    m_startPoint.reset(new QPointF(startPos));
    switch (m_shape) {
    case Rectangle:
        m_observer = new ControlRectangleObserver(m_eventTempItem.get());
        break;
    case Ellipse:
        m_observer = new ControlEllipseObserver(m_eventTempItem.get());
        break;
    default:
        m_observer = nullptr;
        break;
    }
    m_observer->formItem(QRectF(startPos, startPos), 0.0);
}

void WhiteBoardShapePen::deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(lastScenePos);
    Q_UNUSED(scene);
    if (m_observer.isNull() || m_startPoint.isNull()) {
        return;
    }
    QPointF center = (scenePos + *m_startPoint) / 2;
    qreal width = qAbs(scenePos.x() - m_startPoint->x());
    qreal height = qAbs(scenePos.y() - m_startPoint->y());
    center.setX(center.x() - width / 2);
    center.setY(center.y() - height / 2);
    QRectF rect(center.x(), center.y(), width, height);
    m_observer->formItem(rect, 0.0);
}

void WhiteBoardShapePen::deviceRelease(WhiteBoardScene* scene)
{
    if (m_eventTempItem.isNull() || m_startPoint.isNull() || m_observer.isNull()) {
        return;
    }
    EightWayMovementGroup* group = new EightWayMovementGroup(m_eventTempItem->boundingRect());
    m_observer->setControlGroup(group);
    scene->addItem(group);
    connect(m_observer.get(), &ControlGroupObserver::needToDestroy, this, &WhiteBoardShapePen::destroyObserver);
    m_startPoint.reset(nullptr);
    m_eventTempItem.reset(nullptr);
    m_observer = nullptr;
    scene->update();
}

void WhiteBoardShapePen::destroyObserver()
{
    ControlGroupObserver* observer = qobject_cast<ControlGroupObserver*>(sender());
    delete observer;
}

WhiteBoardRubberBand::WhiteBoardRubberBand(SceneController* controller)
    : WhiteBoardAbstractTool(controller)
{
    loadSettings();
}

void WhiteBoardRubberBand::loadSettings()
{
}

void WhiteBoardRubberBand::devicePress(WhiteBoardScene* scene, const QPointF& startPos)
{
    m_group = QSharedPointer<BaseGraphicsItemGroup>(new BaseGraphicsItemGroup(startPos));
    connect(m_group.get(), &BaseGraphicsItemGroup::needToDestroy, this, &WhiteBoardRubberBand::destroyGroup);
    scene->addItem(m_group.get());
}

void WhiteBoardRubberBand::deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos)
{
    Q_UNUSED(lastScenePos);
    m_group->appendPoint(scenePos);
    scene->update();
}

void WhiteBoardRubberBand::destroyGroup()
{
    if (!m_group.isNull()) {
        m_group.clear();
        // scene->update();
    }
}

void WhiteBoardRubberBand::deviceRelease(WhiteBoardScene* scene)
{
    m_group->complete();
    QList<QGraphicsItem*> items = scene->collidingItems(m_group.get(), Qt::IntersectsItemShape);
    if (items.empty()) {
        m_group.reset(nullptr);
    }
    else if (items.size() == 1 && items.front() == scene->background()) {
        m_group.reset(nullptr);
        qDebug() << "only background";
    }
    else {
        for (auto& pItem : items) {
            if (pItem != scene->background()){
                m_group->addToGroup(pItem);
            }
        }
    }
    scene->update();
}

} // ADEV
