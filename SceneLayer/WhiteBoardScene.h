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
#include "ItemLayer/BaseGraphicsItem.h"
#include "ItemLayer/ControlGroupObserver.h"
#include "ItemLayer/BaseGraphicsItemGroup.h"
#include "SceneLayer/BackgroundItem.h"


namespace ADEV {

class BackgroundItem;
class WhiteBoardAbstractTool;
class SceneController;

class WhiteBoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    // 若背景指定为空则采用默认实现
    WhiteBoardScene(SceneController* controller, BackgroundItem* background = nullptr);
    ~WhiteBoardScene();
    void addItem(const QSharedPointer<BaseGraphicsItem>& pItem);
    void addItem(QGraphicsItem* item);
    QSharedPointer<BaseGraphicsItem> getItem(BaseGraphicsItem* pItem) ;
    void resize(QSizeF size);

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
    int backgroundZValue() const { return m_backgroundItem->zValue(); }
    BackgroundItem* background() const { return m_backgroundItem.get(); }
    QSharedPointer<QUndoStack> undoStack() const;
    void setController(SceneController* controller);

private:
    QScopedPointer<BackgroundItem> m_backgroundItem;
    QMap<BaseGraphicsItem*, QWeakPointer<BaseGraphicsItem>> m_existItemMap;
    QPointer<SceneController> m_controller;
};


} // end of namespace ADEV

#endif // WHITEBOARDSCENE_H
