#include "WhiteBoard.h"

#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

WhiteBoard::WhiteBoard(QWidget *parent)
    : QWidget(parent)
    , scene()
    , view(&scene)
{
    doc.load(QString("E://test.pdf"));
    renderer.setDocument(&doc);
    renderer.setRenderMode(QPdfPageRenderer::RenderMode::MultiThreaded);
    connect(&renderer, &QPdfPageRenderer::pageRendered, this, &WhiteBoard::handlePageRendered);
    qreal screenResolution = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 72.0;
    qreal ratio = devicePixelRatioF();
    renderer.requestPage(1, QSizeF(doc.pagePointSize(1) * screenResolution).toSize() * ratio);

    QRectF rect(200, 200, 100, 100);
    rectItem = new ADEV::BaseGraphicsItem(4, QBrush(Qt::red));
    observer = new ADEV::ControlRectangleObserver(rectItem);
    observer->formItem(rect, 0.0);
    group = new ADEV::EightWayMovementGroup(rect);
    observer->setControlGroup(group);
    scene.addItem(rectItem);
    scene.addItem(group);
    setLayout(&layout);
    layout.addWidget(&view);
}

WhiteBoard::~WhiteBoard() {}

void WhiteBoard::handlePageRendered(int pageNumber, QSize imageSize, const QImage &image,
                      QPdfDocumentRenderOptions options, quint64 requestId)
{
    backgroundItem = new ADEV::BackgroundImageItem(image, imageSize);
    scene.addItem(backgroundItem);
    scene.update();
    qDebug() << "1111";
}
