#ifndef BASICCOMMAND_H
#define BASICCOMMAND_H

#include <QUndoCommand>
#include <QUndoStack>
#include <QPainterPath>

class QGraphicsScene;

namespace ADEV {

class BaseGraphicsItem;
class AddItemCommand : public QUndoCommand
{
public:
    explicit AddItemCommand(QGraphicsScene* scene, BaseGraphicsItem* item);
    explicit AddItemCommand(const AddItemCommand& command);
    AddItemCommand& operator=(const AddItemCommand& command);
    ~AddItemCommand();
    void redo() override;
    void undo() override;

private:
    QGraphicsScene* m_scene;
    BaseGraphicsItem* m_addedItem;
};

class DeleteItemCommand : public QUndoCommand
{
public:
    explicit DeleteItemCommand(QGraphicsScene* scene, BaseGraphicsItem* item);
    ~DeleteItemCommand();
    void redo() override;
    void undo() override;

private:
    QGraphicsScene* m_scene;
    BaseGraphicsItem* m_deletedItem;
};

class EraseItemCommand : public QUndoCommand
{
public:
    explicit EraseItemCommand(QGraphicsScene* scene, BaseGraphicsItem* item, QPainterPath collidesPath);
    explicit EraseItemCommand(const EraseItemCommand& command);
    EraseItemCommand& operator=(const EraseItemCommand& command);
    ~EraseItemCommand();
    void redo() override;
    void undo() override;

private:
    QGraphicsScene* m_scene;
    BaseGraphicsItem* m_erasedItem;

    DeleteItemCommand* m_deleteItemCommand;
    // AddItemCommand里面的Item由它自己的析构函数来确定是否释放
    QList<AddItemCommand*> m_addItemCommands;
};

class EraseItemsCommand : public QUndoCommand
{
public:
    EraseItemsCommand();
    ~EraseItemsCommand();
    void push(EraseItemCommand* command);
    void redo() override;
    void undo() override;

private:
    QList<EraseItemCommand*> m_eraseCommandList;
};

} // end of namespace ADEV

#endif // BASICCOMMAND_H
