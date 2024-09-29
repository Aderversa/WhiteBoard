#ifndef MULTIPAGELAYOUT_H
#define MULTIPAGELAYOUT_H

#include <QWidget>
#include <QLayout>
#include <QList>

namespace ADEV {

class MultiPageLayout : public QLayout
{
    Q_OBJECT
public:
    MultiPageLayout(QWidget *parent = nullptr);
    ~MultiPageLayout();

    // 通过 QLayout 继承
    QSize sizeHint() const override;

    void addItem(QLayoutItem* item) override;

    QLayoutItem* itemAt(int index) const override;

    QLayoutItem* takeAt(int index) override;

    int count() const override;

    void setGeometry(const QRect& rect) override;

    void doLayout();

    void setY(int dy);
    void setX(int dx);

    qreal zoomFactor() const { return zoomFactor_; }
    void setZoomFactor(qreal factor, QPoint zoomPos = QPoint());

private:
    QList<QPair<QLayoutItem*, QSize>> items;
    QPoint offset;
    QSize virtualSize;
    qreal zoomFactor_;
};

} // ADEV

#endif // MULTIPAGELAYOUT_H
