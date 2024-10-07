#include "LaserPenDialog.h"
#include "SceneLayer/SceneController.h"
#include "Config/WhiteBoardSettings.h"

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
namespace ADEV
{

LaserPenDialog::LaserPenDialog(SceneController* controller, QWidget* parent)
    : QDialog(parent)
    , m_colorFrame(new QFrame(this))
    , m_colorDialogBtn(new QPushButton(tr("选择颜色"), this))
    , m_confirmBtn(new QPushButton(tr("确认"), this))
    , m_cancelBtn(new QPushButton(tr("取消"), this))
    , m_layout(new QVBoxLayout(this))
    , m_controller(controller)
{
    m_color = getLaserPenColor();

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(m_colorFrame);
    hLayout->addWidget(m_colorDialogBtn);
    m_layout->addLayout(hLayout);
    m_colorFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_colorFrame->setAutoFillBackground(true);
    connect(m_colorDialogBtn, &QPushButton::clicked, this, &LaserPenDialog::openColorDialog);
    connect(this, &LaserPenDialog::colorChanged, this, &LaserPenDialog::setFrameColor);
    setFrameColor(m_color);

    hLayout = new QHBoxLayout;
    hLayout->addWidget(m_confirmBtn);
    hLayout->addWidget(m_cancelBtn);
    m_layout->addLayout(hLayout);
    connect(m_confirmBtn, &QPushButton::clicked, this, &LaserPenDialog::onConfirm);
    connect(m_cancelBtn, &QPushButton::clicked, this, &LaserPenDialog::onCancel);
}

void LaserPenDialog::openColorDialog()
{
    QColor color = QColorDialog::getColor(m_color, this);
    if (color.isValid())
    {
        m_color = color;
        emit colorChanged(color);
    }
}

void LaserPenDialog::setFrameColor(QColor color)
{
    QPalette p = m_colorFrame->palette();
    p.setBrush(QPalette::ColorRole::Window, QBrush(color));
    m_colorFrame->setPalette(p);
}

QColor LaserPenDialog::getLaserPenColor()
{
    return WhiteBoardSettings::getInstance()
            .value(WhiteBoardLaserPen::COLOR_KEY, QColor(WhiteBoardLaserPen::DEFAULT_COLOR))
            .value<QColor>();
}

void LaserPenDialog::onConfirm()
{
    WhiteBoardSettings::getInstance().setValue(WhiteBoardLaserPen::COLOR_KEY, m_color);
    WhiteBoardSettings::getInstance().sync();
    m_controller->reloadToolSettings();
    accept();
}

void LaserPenDialog::onCancel()
{
    reject();
}

}
