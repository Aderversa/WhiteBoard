#ifndef MULTIPAGEWIDGET_H
#define MULTIPAGEWIDGET_H

#include <QWidget>
#include "MultiPageLayout.h"
#include "SceneLayer/WhiteBoardScene.h"

namespace ADEV {

class MultiPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiPageWidget(QWidget *parent = nullptr);
    void addScene(WhiteBoardScene* scene);

protected:
    void wheelEvent(QWheelEvent* event);

private:
    MultiPageLayout* m_pageLayout;
};

} // ADEV
#endif // MULTIPAGEWIDGET_H
