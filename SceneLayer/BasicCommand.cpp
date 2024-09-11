#include "BasicCommand.h"
#include "ItemLayer/BaseGraphicsItem.h"
#include "SceneLayer/WhiteBoardScene.h"

#include <QGraphicsItem>
#include <QDebug>

namespace ADEV {

AddItemCommand::AddItemCommand(WhiteBoardScene* scene, const QSharedPointer<BaseGraphicsItem>& item)
    : QUndoCommand()
    , m_scene(scene)
    , m_addedItem(item)
{
    // 由于当我们将command添加到QUndoStack时，后者会自动调用redo操作
    // 所以不需要手动在构造函数进行redo
}

AddItemCommand::~AddItemCommand()
{
}

void AddItemCommand::redo()  // 执行完后Item在Scene
{
    m_scene->addItem(m_addedItem);
    m_scene->update();
}

void AddItemCommand::undo()  // 执行完之后Item不在Scene
{
    m_scene->removeItem(m_addedItem.data());
    m_scene->update();
}

DeleteItemCommand::DeleteItemCommand(WhiteBoardScene* scene, const QSharedPointer<BaseGraphicsItem>& item)
    : QUndoCommand()
    , m_scene(scene)
    , m_deletedItem(item)
{
    // 由于当我们将command添加到QUndoStack时，后者会自动调用redo操作
}

DeleteItemCommand::~DeleteItemCommand()
{
}

void DeleteItemCommand::redo()
{
    m_scene->removeItem(m_deletedItem.data());
    m_scene->update();
}

void DeleteItemCommand::undo()
{
    m_scene->addItem(m_deletedItem);
    m_scene->update();
}

EraseItemCommand::EraseItemCommand(WhiteBoardScene* scene, const QSharedPointer<BaseGraphicsItem>& item, QPainterPath collidesPath)
    : QUndoCommand()
    , m_scene(scene)
{
    QPainterPath path = item->mapToScene(item->shape());
    QList<QPolygonF> polygons = path.subtracted(collidesPath).toFillPolygons();
    BaseGraphicsItem::Memento memento = item->save();
    m_deleteItemCommand = QSharedPointer<DeleteItemCommand>(new DeleteItemCommand(scene, item));
    // 添加分裂后的Item
    for (const QPolygonF& polygon : polygons)
    {
        QPainterPath newPath;
        newPath.addPolygon(polygon);
        // 初始化的参数是随便给的，后面使用备忘录memento重置
        QSharedPointer<BaseGraphicsItem> newItem(new BaseGraphicsItem(0, QBrush()));
        newItem->restore(memento);
        newItem->setStrokePath(newPath); // 重置其path为分裂后的子path
        QSharedPointer<AddItemCommand> command(new AddItemCommand(m_scene, newItem)); // 构造即command完成
        m_addItemCommands.push_back(command);
    }
    // 这里不进行command的redo是因为后续将EraseItemCommand加入QUndoStack后
    // QUndoStack会在command入栈时自动将其执行一次`
    // 这就意味着创建EraseItemCommand的时候不意味着Scene的刷新，
    // 只有当release事件将EraseItemsCommand加入QUndoStack时，才会刷新
    // 这个command必须在初始化时即执行，以实时反馈Scene的变化
    // 在后面合成EraseItemsCommand的时候调用一次undo然后再加入QUndoStack以适配它
    m_deleteItemCommand->redo();
    // 重新浮现分裂后的Item
    for (auto& command : m_addItemCommands)
    {
        command->redo();
    }
    m_scene->update();
}

EraseItemCommand::~EraseItemCommand()
{
}

void EraseItemCommand::redo()
{
    // 原始Item消失在Scene中
    m_deleteItemCommand->redo();
    // 重新浮现分裂后的Item
    for (auto& command : m_addItemCommands)
    {
        command->redo();
    }
    m_scene->update();
}

void EraseItemCommand::undo()
{
    // 原始的Item重现在Scene中
    m_deleteItemCommand->undo();
    // 分裂后的Item消失在Scene中
    for (auto& command : m_addItemCommands)
    {
        command->undo();
    }
    m_scene->update();
}

EraseItemsCommand::EraseItemsCommand()
    : QUndoCommand()
{
}

EraseItemsCommand::~EraseItemsCommand()
{
}

void EraseItemsCommand::push(const QSharedPointer<EraseItemCommand>& command)
{
    m_eraseCommandList.push_back(command);
}

void EraseItemsCommand::redo()
{
    // 必须顺序遍历
    // 加入QUndoStack时，被执行一次
    // EraseItemCommand::redo()
    for (auto it = m_eraseCommandList.begin(); it != m_eraseCommandList.end(); ++it) {
        (*it)->redo();
    }
}

void EraseItemsCommand::undo()
{
    // 必须逆序遍历
    // 需要在加入QUndoStack前调用一次undo()以抵消QUndoStack的push方法自动调用redo的效果。
    auto it = m_eraseCommandList.rbegin();
    for (;it != m_eraseCommandList.rend(); ++it) {
        (*it)->undo(); // 这样子调用是有效的，虽然智能提示没有效果
    }
}

int EraseItemsCommand::size() const
{
    return m_eraseCommandList.size();
}

} // end of namespace ADEV
