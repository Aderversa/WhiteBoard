#ifndef MULTIPAGEWIDGET_H
#define MULTIPAGEWIDGET_H

#include <QWidget>
#include <QScrollBar>

namespace ADEV {

class MultiPageLayout;

class MultiPageWidget : public QWidget
{
    Q_OBJECT
public:
    MultiPageWidget(QWidget* parent = nullptr);
    ~MultiPageWidget();
    void addWidget(QWidget* w);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateScrollBars();

private:
    MultiPageLayout* pageLayout;
    QScrollBar hBar;
    QScrollBar vBar;
};


} // ADEV
#endif // MULTIPAGEWIDGET_H
