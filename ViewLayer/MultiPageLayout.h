#ifndef MULTIPAGELAYOUT_H
#define MULTIPAGELAYOUT_H

#include <QLayout>

namespace ADEV {
class MultiPageLayout : public QLayout
{
    Q_OBJECT
public:
    MultiPageLayout(QMargins margins, int pageSpacing, QWidget *parent = nullptr);
    ~MultiPageLayout();

    QSize sizeHint() const override;

    void addWidget(QWidget* w);

    void addItem(QLayoutItem* item) override;

    QLayoutItem* itemAt(int index) const override;

    QLayoutItem* takeAt(int index) override;

    int count() const override;

    void setGeometry(const QRect& rect) override;

    // dx > 0 整体右移; dx < 0 整体左移
    void moveX(int dx);
    // dy > 0 整体下移; dy < 0 整体上移
    void moveY(int dy);

    qreal zoomFactor() const { return m_zoomFactor; }
    void setZoomFactor(qreal factor);

private:
    void calculateSize();

private:
    QMargins margins;
    int pageSpacing;
    QPoint offset;
    QSize pageSize;
    QList<QLayoutItem*> itemList;
    QSize virtualSize;
    qreal m_zoomFactor;
};
} // ADEV

#endif // MULTIPAGELAYOUT_H
