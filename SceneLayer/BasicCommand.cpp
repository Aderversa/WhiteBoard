#include "BasicCommand.h"
#include "ItemLayer/BaseGraphicsItem.h"

#include <QGraphicsScene>
#include <QGraphicsItem>

namespace ADEV {

AddItemCommand::AddItemCommand(QGraphicsScene* scene, BaseGraphicsItem* item)
    : QUndoCommand()
    , m_scene(scene)
    , m_addedItem(item)
{
    // 由于当我们将command添加到QUndoStack时，后者会自动调用redo操作
    // 所以不需要手动在构造函数进行redo
}

AddItemCommand::AddItemCommand(const AddItemCommand& command)
{
    *this = command;
}

AddItemCommand& AddItemCommand::operator=(const AddItemCommand& command)
{
    QUndoCommand();
    m_scene = command.m_scene;
    m_addedItem = command.m_addedItem;
    return *this;
}

AddItemCommand::~AddItemCommand()
{
    if (!m_isItemInScene) { // 不在Scene中，则需要进行释放
        delete m_addedItem; // 需要保证这个Item是new创建的
    }
}

void AddItemCommand::redo()  // 执行完后Item在Scene
{
    m_scene->addItem(m_addedItem);
    m_isItemInScene = true;
    m_scene->update();
}

void AddItemCommand::undo()  // 执行完之后Item不在Scene
{
    m_scene->removeItem(m_addedItem);
    m_isItemInScene = false;
    m_scene->update();
}

DeleteItemCommand::DeleteItemCommand(QGraphicsScene* scene, BaseGraphicsItem* item)
    : QUndoCommand()
    , m_scene(scene)
    , m_deletedItem(item)
{
    // 由于当我们将command添加到QUndoStack时，后者会自动调用redo操作
    // 所以不需要手动在构造函数进行redo
}

DeleteItemCommand::~DeleteItemCommand()
{
    if (!m_isItemInScene) {
        delete m_deletedItem;
    }
}

void DeleteItemCommand::redo()
{
    m_scene->removeItem(m_deletedItem);
    m_isItemInScene = false;
    m_scene->update();
}

void DeleteItemCommand::undo()
{
    m_scene->addItem(m_deletedItem);
    m_isItemInScene = true;
    m_scene->update();
}

EraseItemCommand::EraseItemCommand(QGraphicsScene* scene, BaseGraphicsItem* item, QPainterPath collidesPath)
    : QUndoCommand()
    , m_scene(scene)
    , m_erasedItem(item)
{
    QPainterPath path = m_erasedItem->shape();
    QList<QPolygonF> polygons = path.subtracted(collidesPath).toFillPolygons();
    BaseGraphicsItem::Memento memento = m_erasedItem->save();
    m_deleteItemCommand = new DeleteItemCommand(scene, m_erasedItem);
    // 添加分裂后的Item
    for (const QPolygonF& polygon : polygons)
    {
        QPainterPath newPath;
        newPath.addPolygon(polygon);
        // 初始化的参数是随便给的，后面使用备忘录memento重置
        BaseGraphicsItem* newItem = new BaseGraphicsItem(0, QBrush());
        newItem->restore(memento);
        newItem->setStrokePath(newPath); // 重置其path为分裂后的子path
        AddItemCommand* command = new AddItemCommand(m_scene, newItem); // 构造即command完成
        m_addItemCommands << command;
    }
    // 这里不进行command的redo是因为后续将EraseItemCommand加入QUndoStack后
    // QUndoStack会在command入栈时自动将其执行一次`
    // 这就意味着创建EraseItemCommand的时候不意味着Scene的刷新，
    // 只有当release事件将EraseItemsCommand加入QUndoStack时，才会刷新
    // 这个command必须在初始化时即执行，以实时反馈Scene的变化
    // 在后面合成EraseItemsCommand的时候调用一次undo然后再加入QUndoStack以适配它
    this->redo(); // 执行redo操作，让变化呈现在Scene上
}

EraseItemCommand::EraseItemCommand(const EraseItemCommand& command)
    : QUndoCommand()
{
    m_scene = command.m_scene;
    m_erasedItem = command.m_erasedItem;
    m_addItemCommands = command.m_addItemCommands;
    m_isItemInScene = command.m_isItemInScene;
}

EraseItemCommand& EraseItemCommand::operator=(const EraseItemCommand& command)
{
    m_scene = command.m_scene;
    m_erasedItem = command.m_erasedItem;
    m_addItemCommands = command.m_addItemCommands;
    m_isItemInScene = command.m_isItemInScene;
    return *this;
}

EraseItemCommand::~EraseItemCommand()
{
    if (m_deleteItemCommand) {
        delete m_deleteItemCommand;
    }
    while (!m_addItemCommands.empty()) {
        auto command = m_addItemCommands.front();
        m_addItemCommands.pop_front();
        if (command)
            delete command;
    }
}

void EraseItemCommand::redo()
{
    // 原始Item消失在Scene中
    m_scene->removeItem(m_erasedItem);
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
    m_scene->addItem(m_erasedItem);
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
    while(!m_eraseCommandList.empty()) {
        auto command = m_eraseCommandList.front();
        m_eraseCommandList.pop_front();
        if (command)
            delete command;
    }
}

void EraseItemsCommand::push(const EraseItemCommand& command)
{
    EraseItemCommand* newCommand = new EraseItemCommand(command);
    m_eraseCommandList.push_back(newCommand);
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

} // end of namespace ADEV
