#ifndef WHITEBOARDSCENE_H
#define WHITEBOARDSCENE_H

#include <QGraphicsScene>
#include "SceneLayer/BasicCommand.h"
#include "SceneLayer/WhiteBoardTools.h"
#include "ItemLayer/BaseGraphicsItem.h"
#include "ItemLayer/ControlGroupObserver.h"
#include "SceneLayer/BackgroundItem.h"

class QUndoStack;

namespace ADEV {

class BackgroundItem;

class WhiteBoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    // 若背景指定为空则采用默认实现
    WhiteBoardScene(BackgroundItem* background = nullptr);
    ~WhiteBoardScene();

private: // 用于标识某个工具的处理方法的Tag，专门用来定位模板方法的实现
    struct NormalPenTag {};
    struct HightlightPenTag {};
    struct LaserPenTag {};
    struct EraserTag {};

public: // 抽象的设备事件处理
    void inputDevicePress(const QPointF& startPos);
    void inputDeviceMove(const QPointF& scenePos, const QPointF& lastScenePos);
    void inputDeviceRelease();

public: // 不同工具对Press事件的处理
    // 这里我采用模板+Tag的方式定位到不同工具的处理函数，是为了获得一致的方法名
    // 这里还有一种解决方案是使用不同名的函数来进行处理
    // 没人规定你该使用什么方法，适合自己就好
    template<typename T = void> // 这个T没有任何意义
    void devicePress(const QPointF& startPos, NormalPenTag);
    template<typename T = void> // 这个T没有任何意义
    void devicePress(const QPointF& startPos, HightlightPenTag);
    template<typename T = void> // 这个T没有任何意义
    void devicePress(const QPointF& startPos, LaserPenTag);
    template<typename T = void> // 这个T没有任何意义
    void devicePress(const QPointF& startPos, EraserTag);

public: // 不同工具对Move事件的处理
    template<typename T = void>
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, NormalPenTag);
    template<typename T = void>
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, HightlightPenTag);
    template<typename T = void>
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, LaserPenTag);
    template<typename T = void>
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, EraserTag);

public: // 不同工具对Release事件的处理
    template<typename T = void>
    void deviceRelease(NormalPenTag);
    template<typename T = void>
    void deviceRelease(HightlightPenTag);
    template<typename T = void>
    void deviceRelease(LaserPenTag);
    template<typename T = void>
    void deviceRelease(EraserTag);

public: // 碰撞处理
    void handleCollidingItems(const QPainterPath& collidesArea);
    void eraseCollidingWholeItems(const QPainterPath& collidesArea);

public: // 对实际事件的处理
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public: // getter & setter
    // 如果BackgroundItem是BackgroundImageItem，则不可更换
    // 更换的时候若新background不是原来的background，则原本的background会被释放
    bool changeBackground(BackgroundItem* background);
    void setNormalPen(const WhiteBoardNormalPen& normalPen);
    void setHighlightPen(const WhiteBoardHighlightPen& hightlightPen);
    void setLaserPen(const WhiteBoardLaserPen& laserPen);
    void setEraser(const WhiteBoardEraser& eraser);
    WhiteBoardNormalPen normalPen() const;
    WhiteBoardHighlightPen hightlightPen() const;
    WhiteBoardLaserPen laserPen() const;
    WhiteBoardEraser eraser() const;
    void selectTool(WhiteBoardTool toolType);

private:  // 利用配置对象初始化工具的方法
    void initNormalPen();
    void initHighlightPen();
    void initLaserPen();
    void initEraser();


private:
    BackgroundItem* m_backgroundItem;
    QUndoStack* m_undoStack;

    BaseGraphicsItem* m_eventTempItem = nullptr;
    ControlCurveObserver* m_curveObserver = nullptr;
    EraseItemsCommand* m_eraseItemsCommand = nullptr;

    WhiteBoardTool m_nowUseTool; // 标志现在使用的工具
    WhiteBoardNormalPen  m_normalPen;
    WhiteBoardHighlightPen m_highlightPen;
    WhiteBoardLaserPen m_laserPen;
    WhiteBoardEraser m_eraser;

    // 配置类用来读取配置信息
};

template<typename T>
void WhiteBoardScene::devicePress(const QPointF& startPos, NormalPenTag)
{
    m_eventTempItem = new BaseGraphicsItem(m_normalPen.width, QBrush(m_normalPen.color));
    m_eventTempItem->setZValue(m_backgroundItem->zValue()  + 1);
    // 添加操作使用Command进行
    AddItemCommand* command = new AddItemCommand(this, m_eventTempItem);
    m_undoStack->push(command); // add操作真正执行是在push方法内
    m_curveObserver = new ControlCurveObserver(m_eventTempItem);
    // 后面的QSizeF是随便给的，因为我知道ControlCurveObserver在其formItem只处理leftTop点
    m_curveObserver->addPointToCurve(startPos);
}

template<typename T>
void WhiteBoardScene::devicePress(const QPointF& startPos, HightlightPenTag)
{
    m_eventTempItem = new BaseGraphicsItem(m_highlightPen.width, QBrush(m_highlightPen.color));
    m_eventTempItem->setZValue(m_backgroundItem->zValue()  + 1);
    m_eventTempItem->setOpacity(m_highlightPen.opacity);
    // 添加操作使用Command进行
    AddItemCommand* command = new AddItemCommand(this, m_eventTempItem);
    m_undoStack->push(command); // add操作真正执行是在push方法内
    if (!m_highlightPen.openStraightLineMode)
    {
        m_curveObserver = new ControlCurveObserver(m_eventTempItem);
    }
    else
    {
        m_curveObserver = new ControlLineObserver(m_eventTempItem);
    }
    m_curveObserver->addPointToCurve(startPos);
}

template<typename T>
void WhiteBoardScene::devicePress(const QPointF& startPos, LaserPenTag)
{

}

template<typename T>
void WhiteBoardScene::devicePress(const QPointF& startPos, EraserTag)
{
    if (!m_eraser.eraseWholeItem)
    {
        m_eraseItemsCommand = new EraseItemsCommand;
        m_eventTempItem = new BaseGraphicsItem(m_eraser.radius, QBrush(Qt::darkGray));
        m_eventTempItem->setZValue(m_backgroundItem->zValue() + 2);
        m_eventTempItem->setOpacity(0.6);

        addItem(m_eventTempItem); // 临时Item不需要加入UndoStack
        QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(startPos, startPos), m_eraser.radius);
        // 橡皮擦是临时用来显示的Item，不需要使用Command
        m_eventTempItem->setStrokePath(circle);

        handleCollidingItems(m_eventTempItem->shape());
    }
    else
    {
        m_eventTempItem = new BaseGraphicsItem(m_eraser.MIN_RADIUS, QBrush(Qt::black));

        addItem(m_eventTempItem); // 临时Item不需要加入UndoStack
        QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(startPos, startPos), m_eraser.MIN_RADIUS);
        m_eventTempItem->setStrokePath(circle);

        eraseCollidingWholeItems(circle);
    }
}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, NormalPenTag)
{
    Q_UNUSED(lastScenePos);
    m_curveObserver->addPointToCurve(scenePos);
}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, HightlightPenTag)
{
    Q_UNUSED(lastScenePos);
    m_curveObserver->addPointToCurve(scenePos);
}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, LaserPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, EraserTag)
{
    // 定义空指针异常，防止意外的事件进行错误的处理
    if (!m_eventTempItem) // 不知道什么原因，明明没有定义鼠标滚轮事件，滚动滚轮却会触发这个事件
        return;
    qreal radius;
    if (!m_eraser.eraseWholeItem)
        radius = m_eraser.radius;
    else
        radius = m_eraser.MIN_RADIUS;
    QPainterPath track = m_eventTempItem->lineToStroke(QLineF(lastScenePos, scenePos), radius);
    eraseCollidingWholeItems(track);
    QPainterPath circle = m_eventTempItem->lineToStroke(QLineF(scenePos, scenePos), radius);
    m_eventTempItem->setStrokePath(circle);
    this->update();
}

template<typename T>
void WhiteBoardScene::deviceRelease(NormalPenTag)
{
    // 同一时间只处理同一个工具的一套(press-move-release)流程，所以不用担心同步问题
    if (m_curveObserver) {// 已经没有用了，需要释放
        delete m_curveObserver;
        m_curveObserver = nullptr;
    }
    // item已经在本次事件中塑造完成，后续交给Scene管理
    // m_eventTempItem不再能操作已经塑造完成的Item
    m_eventTempItem = nullptr;
}

template<typename T>
void WhiteBoardScene::deviceRelease(HightlightPenTag)
{
    if (m_curveObserver) {
        delete m_curveObserver;
        m_curveObserver = nullptr;
    }
    // item已经在本次事件中塑造完成，后续交给Scene管理
    // m_eventTempItem不再能操作已经塑造完成的Item
    m_eventTempItem = nullptr;
}

template<typename T>
void WhiteBoardScene::deviceRelease(LaserPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceRelease(EraserTag)
{
    if (m_eventTempItem) {
        // QGraphicsItem的析构函数会考虑其父Item和Scene的关系断联操作
        delete m_eventTempItem;
        m_eventTempItem = nullptr;
    }
    if (!m_eraser.eraseWholeItem)
    {
        m_eraseItemsCommand->undo();
        m_undoStack->push(m_eraseItemsCommand);
        m_eraseItemsCommand = nullptr;
    }
    this->update();
}

} // end of namespace ADEV

#endif // WHITEBOARDSCENE_H
