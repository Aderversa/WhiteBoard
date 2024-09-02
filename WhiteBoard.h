#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QMainWindow>

class WhiteBoard : public QMainWindow
{
    Q_OBJECT

public:
    WhiteBoard(QWidget *parent = nullptr);
    ~WhiteBoard();
};
#endif // WHITEBOARD_H
