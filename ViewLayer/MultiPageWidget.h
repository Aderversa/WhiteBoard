#ifndef MULTIPAGEWIDGET_H
#define MULTIPAGEWIDGET_H

#include <QWidget>

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

private:
    MultiPageLayout* pageLayout;
};


} // ADEV
#endif // MULTIPAGEWIDGET_H
