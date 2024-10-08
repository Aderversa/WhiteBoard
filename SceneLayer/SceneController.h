#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include <QObject>
#include "SceneLayer/WhiteBoardScene.h"

class QTimer;
class QUndoStack;

namespace ADEV {

class SceneController : public QObject
{
    Q_OBJECT
public:
    explicit SceneController(QObject *parent = nullptr);
    QSharedPointer<WhiteBoardAbstractTool> tool() const;
    QSharedPointer<QUndoStack> undoStack() { return m_undoStack; }
    void useNormalPen();
    void useHighlightPen();
    void useLaserPen();
    void useEraser();
    void useShapePen();
    void useRubberBand();

public slots:
    void reloadToolSettings();

signals:
    void toolChanged();

private:
    QSharedPointer<WhiteBoardAbstractTool> m_tool;
    QSharedPointer<QUndoStack> m_undoStack;
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
    WhiteBoardAbstractTool(SceneController* controller) : m_controller(controller){}
    virtual ~WhiteBoardAbstractTool() = default;
    SceneController* controller() { return m_controller.get(); }

    virtual void devicePress(WhiteBoardScene* scene, const QPointF& startPos) = 0;
    virtual void deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos) = 0;
    virtual void deviceRelease(WhiteBoardScene* scene) = 0;
    virtual void loadSettings() = 0;

private:
    QPointer<SceneController> m_controller;
};

class WhiteBoardNormalPen : public WhiteBoardAbstractTool{
public:
    WhiteBoardNormalPen(SceneController* controller);
    void devicePress(WhiteBoardScene* scene, const QPointF& startPos) override;
    void deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease(WhiteBoardScene* scene) override;
    void loadSettings() override;

public:
    static constexpr char COLOR_KEY[] = "Tools/NormalPen/color";
    static constexpr char WIDTH_KEY[] = "Tools/NormalPen/width";
    static constexpr qreal DEFAULT_WIDTH = 3.0;
    static constexpr Qt::GlobalColor DEFAULT_COLOR = Qt::red;

private:
    QSharedPointer<BaseGraphicsItem> m_eventTempItem = nullptr;
    QSharedPointer<ControlCurveObserver> m_curveObserver = nullptr;

private:
    qreal m_width;
    QColor m_color;
};

class WhiteBoardHighlightPen : public WhiteBoardAbstractTool{
public:
    WhiteBoardHighlightPen(SceneController* controller);
    void devicePress(WhiteBoardScene* scene, const QPointF& startPos) override;
    void deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease(WhiteBoardScene* scene) override;
    void loadSettings() override;
public:
    static constexpr char COLOR_KEY[] = "Tools/HighlightPen/color";
    static constexpr char WIDTH_KEY[] = "Tools/HighlightPen/width";
    static constexpr char OPACITY_KEY[] = "Tools/HighlightPen/opacity";
    static constexpr char MODE_KEY[] = "Tools/HighlightPen/openStraightLineMode";
    static constexpr qreal DEFAULT_WIDTH = 3.0;
    static constexpr Qt::GlobalColor DEFAULT_COLOR = Qt::red;
    static constexpr qreal DEFAULT_OPACITY = 1.0;
    static constexpr bool DEFAULT_STARIGHT_LINE_MODE = false;

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
    WhiteBoardLaserPen(SceneController* controller);
    void devicePress(WhiteBoardScene* scene, const QPointF& startPos) override;
    void deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease(WhiteBoardScene* scene) override;
    void loadSettings() override;

public:
    static constexpr char COLOR_KEY[] = "Tools/LaserPen/color";
    static constexpr Qt::GlobalColor DEFAULT_COLOR = Qt::red;

private:
    LaserStrokeTempList m_laserItemTempList;
    QSharedPointer<ControlCurveObserver> m_curveObserver = nullptr;

private:
    QColor m_color;
    constexpr static qreal FIXED_WIDTH = 6;
};

class WhiteBoardEraser : public WhiteBoardAbstractTool{
public:
    WhiteBoardEraser(SceneController* controller);
    void devicePress(WhiteBoardScene* scene, const QPointF& startPos) override;
    void deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease(WhiteBoardScene* scene) override;
    void loadSettings() override;

public: // 碰撞处理
    void handleCollidingItems(WhiteBoardScene* scene,
                              const QPainterPath& collidesArea,
                              const QSharedPointer<BaseGraphicsItem>& pointer,
                              EraseItemsCommand* commands);
    void eraseCollidingWholeItems(WhiteBoardScene* scene,
                                  const QPainterPath& collidesArea,
                                  const QSharedPointer<BaseGraphicsItem>& pointer);

public:
    constexpr static char RADIUS_KEY[] = "Tools/Eraser/radius";
    static constexpr char ERASE_WHOLE_KEY[] = "Tools/Eraser/eraseWholeItem";
    static constexpr qreal DEFAULT_RADIUS = 100;
    static constexpr bool DEFAULT_ERASE_WHOLE = false;

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
        Ellipse,
    };

public:
    WhiteBoardShapePen(SceneController* controller);
    void devicePress(WhiteBoardScene* scene, const QPointF& startPos) override;
    void deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease(WhiteBoardScene* scene) override;
    void loadSettings() override;

public:
    static constexpr char COLOR_KEY[] = "Tools/ShapePen/color";
    static constexpr char WIDTH_KEY[] = "Tools/ShapePen/width";
    static constexpr char OPACITY_KEY[] = "Tools/ShapePen/opacity";
    static constexpr char SHAPE_KEY[] = "Tools/ShapePen/shape";
    static constexpr qreal DEFAULT_WIDTH = 3.0;
    static constexpr Qt::GlobalColor DEFAULT_COLOR = Qt::red;
    static constexpr qreal DEFAULT_OPACITY = 1.0;
    static constexpr ItemShape DEFAULT_SHAPE = Rectangle;

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

class WhiteBoardRubberBand : public QObject, public WhiteBoardAbstractTool {
    Q_OBJECT
public:
    WhiteBoardRubberBand(SceneController* controller);
    void devicePress(WhiteBoardScene* scene, const QPointF& startPos) override;
    void deviceMove(WhiteBoardScene* scene, const QPointF& scenePos, const QPointF& lastScenePos) override;
    void deviceRelease(WhiteBoardScene* scene) override;
    void loadSettings() override;


private slots:
    void destroyGroup();

private:
    QSharedPointer<BaseGraphicsItemGroup> m_group{nullptr};
};


} // ADEV

#endif // SCENECONTROLLER_H
