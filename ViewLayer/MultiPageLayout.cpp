#include "MultiPageLayout.h"

#include <QWidget>

namespace ADEV {


MultiPageLayout::MultiPageLayout(QMargins margins, int pageSpacing, QWidget *parent)
    : QLayout(parent)
    , margins(margins)
    , pageSpacing(pageSpacing)
    , offset(0, 0)
    , m_zoomFactor(1.0)
    , pageSize(1000, 400)
{
    calculateSize();
}

MultiPageLayout::~MultiPageLayout()
{
}

QSize MultiPageLayout::sizeHint() const
{
    return virtualSize;
}

void MultiPageLayout::addWidget(QWidget* w)
{
    int max = 16777215;
    w->setMinimumSize(0, 0);
    w->setMaximumSize(max, max);
    QLayout::addWidget(w);
}

void MultiPageLayout::addItem(QLayoutItem* item)
{
    itemList.append(item);
    calculateSize();
}

QLayoutItem* MultiPageLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem* MultiPageLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size()) {
        auto retItem = itemList.takeAt(index);
        calculateSize();
        return retItem;
    }
    return nullptr;
}

int MultiPageLayout::count() const
{
    return itemList.size();
}

void MultiPageLayout::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    int pageY = offset.y() + margins.top();
    for (auto pItem : itemList)
    {
        auto w = pItem->widget();
        if (!w)
        {
            pItem->setGeometry(rect);
            continue;
        }
        int totalWidth = pageSize.width() + margins.left() + margins.right();
        int pageX = (qMax(totalWidth, rect.width()) - pItem->sizeHint().width()) / 2;
        w->resize(pItem->sizeHint());
        w->move(QPoint(pageX + offset.x(), pageY));
        pageY += w->size().height() + pageSpacing;
    }
}

void MultiPageLayout::moveX(int dx)
{
    offset.setX(offset.x() + dx);
    setGeometry(parentWidget()->rect());
}

void MultiPageLayout::moveY(int dy)
{
    offset.setY(offset.y() + dy);
    setGeometry(parentWidget()->rect());
}

void MultiPageLayout::calculateSize()
{
    int n = itemList.size();
    int height = n * pageSize.height() + (n - 1) * pageSpacing + margins.top() + margins.bottom();
    int width = pageSize.width() + margins.left() + margins.right();
    virtualSize = QSize(width, height);
}

void MultiPageLayout::setZoomFactor(qreal factor)
{
    m_zoomFactor = factor;
    pageSize = pageSize.scaled(pageSize * m_zoomFactor, Qt::KeepAspectRatio);
    calculateSize();
    setGeometry(parentWidget()->rect());
}

}
