#ifndef BASICCOMMAND_H
#define BASICCOMMAND_H

#include <QUndoCommand>
#include <QUndoStack>
#include <QPainterPath>
#include <QSharedPointer>


namespace ADEV {

class BaseGraphicsItem;
class WhiteBoardScene;

class AddItemCommand : public QUndoCommand
{
public:
    explicit AddItemCommand(WhiteBoardScene* scene, const QSharedPointer<BaseGraphicsItem>& item);
    ~AddItemCommand();
    void redo() override;
    void undo() override;

private:
    WhiteBoardScene* m_scene;
    QSharedPointer<BaseGraphicsItem> m_addedItem;
};

class DeleteItemCommand : public QUndoCommand
{
public:
    explicit DeleteItemCommand(WhiteBoardScene* scene, const QSharedPointer<BaseGraphicsItem>& item);
    ~DeleteItemCommand();
    void redo() override;
    void undo() override;

private:
    WhiteBoardScene* m_scene;
    QSharedPointer<BaseGraphicsItem> m_deletedItem;
};

class EraseItemCommand : public QUndoCommand
{
public:
    explicit EraseItemCommand(WhiteBoardScene* scene, const QSharedPointer<BaseGraphicsItem>& item, QPainterPath collidesPath);
    ~EraseItemCommand();
    void redo() override;
    void undo() override;

private:
    WhiteBoardScene* m_scene;

    QSharedPointer<DeleteItemCommand> m_deleteItemCommand;
    // AddItemCommand里面的Item由它自己的析构函数来确定是否释放
    QList<QSharedPointer<AddItemCommand>> m_addItemCommands;
};

class EraseItemsCommand : public QUndoCommand
{
public:
    EraseItemsCommand();
    ~EraseItemsCommand();
    void push(const QSharedPointer<EraseItemCommand>& command);
    void redo() override;
    void undo() override;
    int size() const;

private:
    QList<QSharedPointer<EraseItemCommand>> m_eraseCommandList;
};

class MoveItemCommand : public QUndoCommand
{
public:
    MoveItemCommand(const QPointF& beforeMove, const QPointF& afterMove, const QSharedPointer<BaseGraphicsItem>& item);
    ~MoveItemCommand();
    void redo() override;
    void undo() override;

private:
    QSharedPointer<BaseGraphicsItem> m_movedItem;
    QPointF m_beforeMovePos;
    QPointF m_afterMovePos;
};

class MoveItemsCommand : public QUndoCommand
{
public:
    MoveItemsCommand();
    ~MoveItemsCommand();
    void push(const QSharedPointer<MoveItemCommand>& command);
    void redo() override;
    void undo() override;
    int size() const;

private:
    QList<QSharedPointer<MoveItemCommand>> m_moveCommandList;
};

} // end of namespace ADEV

#endif // BASICCOMMAND_H
