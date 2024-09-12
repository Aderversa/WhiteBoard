#ifndef WHITEBOARDSCENE_H
#define WHITEBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QScopedPointer>
#include <QPointer>
#include <QMap>
#include "SceneLayer/BasicCommand.h"
#include "SceneLayer/WhiteBoardTools.h"
#include "ItemLayer/BaseGraphicsItem.h"
#include "ItemLayer/ControlGroupObserver.h"
#include "SceneLayer/BackgroundItem.h"

class QTimer;
class QUndoStack;

namespace ADEV {

class BackgroundItem;
class WhiteBoardAbstractTool;

class WhiteBoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    // 若背景指定为空则采用默认实现
    WhiteBoardScene(BackgroundItem* background = nullptr);
    ~WhiteBoardScene();
    void addItem(const QSharedPointer<BaseGraphicsItem>& pItem);
    void addItem(QGraphicsItem* item);
    QSharedPointer<BaseGraphicsItem> getItem(BaseGraphicsItem* pItem) ;

public: // 碰撞处理
    void handleCollidingItems(const QPainterPath& collidesArea,
                              const QSharedPointer<BaseGraphicsItem>& pointer,
                              EraseItemsCommand* commands);
    void eraseCollidingWholeItems(const QPainterPath& collidesArea,
                                  const QSharedPointer<BaseGraphicsItem>& pointer);

private: // 对实际事件的抽象处理
    void inputDevicePress(const QPointF& startPos);
    void inputDeviceMove(const QPointF& scenePos, const QPointF& lastScenePos);
    void inputDeviceRelease();

protected: // 对实际事件的处理
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public: // getter & setter
    // 如果BackgroundItem是BackgroundImageItem，则不可更换
    // 更换的时候若新background不是原来的background，则原本的background会被释放
    bool changeBackground(BackgroundItem* background);
    QSharedPointer<QUndoStack> undoStack() {return m_undoStack;}
    int backgroundZValue()const {return m_backgroundItem->zValue();}

public slots: // 更换工具的函数
    void useNormalPen();
    void useHighlightPen();
    void useLaserPen();
    void useEraser();
    void useShapePen();

signals:
    void toolChanged();

private:
    QScopedPointer<BackgroundItem> m_backgroundItem;
    QSharedPointer<QUndoStack> m_undoStack;
    QMap<BaseGraphicsItem*, QWeakPointer<BaseGraphicsItem>> m_existItemMap;
    QScopedPointer<WhiteBoardAbstractTool> m_tool;
};

class LaserStrokeTempList : public QObject
{
    Q_OBJECT
public:
    LaserStrokeTempList(int countdownSecond);
    ~LaserStrokeTempList();
    void push(const QSharedPointer<BaseGraphicsItem>& laserStroke);
    void startTimer();

public slots:
    // 工具改变需要立刻清除激光笔
    void toolChanged();

private slots:
    void handleTimeout();

private:
    QList<QSharedPointer<BaseGraphicsItem>> m_laserList;
    QScopedPointer<QTimer> m_timer;
    int m_countdown;
    int m_countTimes = 0;
    constexpr static int unit = 10;
};

class WhiteBoardAbstractTool
{
public:
    WhiteBoardAbstractTool(WhiteBoardScene* scene) : m_scene(scene) {}
    virtual ~WhiteBoardAbstractTool() = default;

    virtual void devicePress(const QPointF& startPos) = 0;
    virtual void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos) = 0;
    virtual void deviceRelease() = 0;

protected:
    QPointer<WhiteBoardScene> m_scene;
};

class WhiteBoardNormalPen : public WhiteBoardAbstractTool{
public:
    WhiteBoardNormalPen(WhiteBoardScene* scene);
    void devicePress(const QPointF& startPos) override;
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease() override;

private:
    QSharedPointer<BaseGraphicsItem> m_eventTempItem = nullptr;
    QSharedPointer<ControlCurveObserver> m_curveObserver = nullptr;

private:
    qreal m_width;
    QColor m_color;
};

class WhiteBoardHighlightPen : public WhiteBoardAbstractTool{
public:
    WhiteBoardHighlightPen(WhiteBoardScene* scene);
    void devicePress(const QPointF& startPos) override;
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease() override;

private:
    QSharedPointer<BaseGraphicsItem> m_eventTempItem = nullptr;
    QSharedPointer<ControlCurveObserver> m_curveObserver = nullptr;

private:
    qreal m_width;
    QColor m_color;
    qreal m_opacity;
    bool m_openStraightLineMode;
};

class WhiteBoardLaserPen : public QObject, public WhiteBoardAbstractTool{
    Q_OBJECT
public:
    WhiteBoardLaserPen(WhiteBoardScene* scene);
    void devicePress(const QPointF& startPos) override;
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease() override;

private:
    LaserStrokeTempList m_laserItemTempList;
    QSharedPointer<ControlCurveObserver> m_curveObserver = nullptr;

private:
    QColor m_color;
    constexpr static qreal FIXED_WIDTH = 6;
};

class WhiteBoardEraser : public WhiteBoardAbstractTool{
public:
    WhiteBoardEraser(WhiteBoardScene* scene);
    void devicePress(const QPointF& startPos) override;
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease() override;

private:
    EraseItemsCommand* m_eraseItemsCommand = nullptr;
    QSharedPointer<BaseGraphicsItem> m_eventTempItem = nullptr;

private:
    qreal m_radius;
    bool m_eraseWholeItem;
    constexpr static qreal MIN_RADIUS = 10;
};

class WhiteBoardShapePen : public QObject, public WhiteBoardAbstractTool {
    Q_OBJECT
public:
    enum ItemShape {
        Rectangle = 0,
    };

public:
    WhiteBoardShapePen(WhiteBoardScene* scene);
    void devicePress(const QPointF& startPos) override;
    void deviceMove(const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease() override;

private slots:
    void destroyObserver();

private:
    QSharedPointer<BaseGraphicsItem> m_eventTempItem{nullptr};
    QPointer<ControlGroupObserver> m_observer{nullptr};
    QScopedPointer<QPointF> m_startPoint{nullptr};

private:
    qreal m_width;
    qreal m_opacity;
    QColor m_color;
    ItemShape m_shape;
};


} // end of namespace ADEV

#endif // WHITEBOARDSCENE_H
