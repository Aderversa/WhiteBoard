#ifndef WHITEBOARDSCENE_H
#define WHITEBOARDSCENE_H

#include <QGraphicsScene>
#include "SceneLayer/WhiteBoardTools.h"

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

public: // 抽象的设备事件处理
    void inputDevicePress(const QPointF& startPos);
    void inputDeviceMove(const QPointF& scenePos, const QPointF& lastScenePos);
    void inputDeviceRelease();

public: // 不同工具对Press事件的处理
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

private: // 用于标识某个工具的处理方法的Tag，专门用来定位模板方法的实现
    struct NormalPenTag {};
    struct HightlightPenTag {};
    struct LaserPenTag {};
    struct EraserTag {};

private:
    BackgroundItem* m_backgroundItem;
    QUndoStack* m_undoStack;

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

}

template<typename T>
void WhiteBoardScene::devicePress(const QPointF& startPos, HightlightPenTag)
{

}

template<typename T>
void WhiteBoardScene::devicePress(const QPointF& startPos, LaserPenTag)
{

}

template<typename T>
void WhiteBoardScene::devicePress(const QPointF& startPos, EraserTag)
{

}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, NormalPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, HightlightPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, LaserPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceMove(const QPointF& scenePos, const QPointF& lastScenePos, EraserTag)
{

}

template<typename T>
void WhiteBoardScene::deviceRelease(NormalPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceRelease(HightlightPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceRelease(LaserPenTag)
{

}

template<typename T>
void WhiteBoardScene::deviceRelease(EraserTag)
{

}

} // end of namespace ADEV

#endif // WHITEBOARDSCENE_H
