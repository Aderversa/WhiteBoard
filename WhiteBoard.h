#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPdfDocument>
#include <QPdfPageRenderer>
#include <ItemLayer/BaseGraphicsItem.h>
#include <ItemLayer/ControlPointGroup.h>
#include <ItemLayer/ControlGroupObserver.h>
#include "SceneLayer/BackgroundItem.h"
#include <QVBoxLayout>

class WhiteBoard : public QWidget
{
    Q_OBJECT
public:
    WhiteBoard(QWidget *parent = nullptr);
    ~WhiteBoard();

public slots:
    void handlePageRendered(int pageNumber, QSize imageSize, const QImage &image,
                      QPdfDocumentRenderOptions options, quint64 requestId);

private:
    QPdfDocument doc;
    QPdfPageRenderer renderer;
    QGraphicsScene scene;
    QGraphicsView view;
    ADEV::BackgroundItem* backgroundItem;
    ADEV::BaseGraphicsItem* rectItem;
    ADEV::EightWayMovementGroup* group;
    ADEV::ControlRectangleObserver* observer;
    QVBoxLayout layout;
};
#endif // WHITEBOARD_H
