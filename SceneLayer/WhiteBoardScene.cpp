#include "WhiteBoardScene.h"
#include "SceneLayer/BackgroundItem.h"
#include "SceneController.h"

#include <QGraphicsSceneMouseEvent>

namespace ADEV {

WhiteBoardScene::WhiteBoardScene(SceneController* controller, BackgroundItem* background)
    : QGraphicsScene()
    , m_backgroundItem(background)
    , m_controller(controller)
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

void WhiteBoardScene::resize(QSizeF size)
{
    m_backgroundItem->resize(size);
    setSceneRect(QRectF(QPointF(0, 0), m_backgroundItem->size()));
}

void WhiteBoardScene::inputDevicePress(const QPointF& startPos)
{
    // 需要将不同的工具的处理逻辑进行分离
    m_controller->tool()->devicePress(this, startPos);
}

void WhiteBoardScene::inputDeviceMove(const QPointF& scenePos, const QPointF& lastScenePos)
{
    m_controller->tool()->deviceMove(this, scenePos, lastScenePos);
}

void WhiteBoardScene::inputDeviceRelease()
{
    m_controller->tool()->deviceRelease(this);
}

void WhiteBoardScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted()) {
        return;
    }
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
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted()) {
        return;
    }
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
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted()) {
        return;
    }
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

QSharedPointer<QUndoStack> WhiteBoardScene::undoStack() const
{
    return m_controller->undoStack();
}

void WhiteBoardScene::setController(SceneController* controller)
{
    m_controller = controller;
}



} // end of namespace ADEV
