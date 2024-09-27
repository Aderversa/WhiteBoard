#include "MultiPageLayout.h"

#include <QWidget>

namespace ADEV {

MultiPageLayout::MultiPageLayout(QWidget* parent)
    : QLayout(parent)
    , offset(0, 0)
    , zoomFactor_(1.0)
{
    setSpacing(5);
    setContentsMargins(QMargins(5, 5, 5, 5));
    doLayout();
}

MultiPageLayout::~MultiPageLayout()
{
}

QSize MultiPageLayout::sizeHint() const
{
    return virtualSize;
}

void MultiPageLayout::addItem(QLayoutItem* item)
{
    if (item->widget())
    {
        items.push_back({ item, item->widget()->size()});
        doLayout();
    }
}

QLayoutItem* MultiPageLayout::itemAt(int index) const
{
    if (index >= 0 && index < items.size())
    {
        return items.value(index).first;
    }
    return nullptr;
}

QLayoutItem* MultiPageLayout::takeAt(int index)
{
    if (index >= 0 && index < items.size())
    {
        return items.takeAt(index).first;
        doLayout();
    }
    return nullptr;
}

int MultiPageLayout::count() const
{
    return items.size();
}

void MultiPageLayout::moveY(int dy)
{
    offset.setY(offset.y() + dy);
    doLayout();
}

void MultiPageLayout::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    doLayout();
}

void MultiPageLayout::doLayout()
{
    int totalWidth = 0;
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    int pageY = offset.y() + top;
    // 调整Y轴坐标
    for (auto pair : items)
    {
        QWidget* w = pair.first->widget();
        if (w)
        {
            w->move(QPoint(0, pageY));
            pageY += w->size().height() + spacing();
            totalWidth = qMax(totalWidth, w->size().width());
            if (parentWidget())
            {
                QSize parentSize = parentWidget()->size();
                int pageX = (parentSize.width() - pair.second.width()) / 2;
                w->move(QPoint(pageX, w->y()));
            }
        }
    }
    totalWidth += left + right;
    // 调整X轴坐标
    virtualSize = QSize(totalWidth, pageY + bottom - offset.y());
}

void MultiPageLayout::setZoomFactor(qreal factor, QPoint zoomPos)
{
    offset.setY(offset.y() / zoomFactor());
    offset.setY(offset.y() * factor);
    offset.setY(qMin(0, offset.y()));
    for (auto& pair : items)
    {
        QWidget* w = pair.first->widget();
        if (w)
        {
            w->resize(w->size().scaled(w->size() / zoomFactor(), Qt::KeepAspectRatio));
            w->resize(w->size().scaled(w->size() * factor, Qt::KeepAspectRatio));
            pair.second = w->size();
        }
    }
    zoomFactor_ = factor;
    doLayout();
}


}
