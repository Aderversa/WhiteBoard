#include "ToolsDialog/NormalPenDialog.h"
#include "Config/WhiteBoardSettings.h"

#include <QSlider>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QColorDialog>

namespace ADEV
{
NormalPenDialog::NormalPenDialog(SceneController* controller, QWidget* parent)
    : QDialog(parent)
    , m_widthLabel(new QLabel(this))
    , m_widthSlider(new QSlider(Qt::Horizontal, this))
    , m_colorFrame(new QFrame(this))
    , m_colorDialogBtn(new QPushButton(tr("选择颜色"), this))
    , m_confirmBtn(new QPushButton(tr("确认"), this))
    , m_cancelBtn(new QPushButton(tr("取消"), this))
    , m_layout(new QVBoxLayout(this))
    , m_controller(controller)
{
    m_color = getNormalPenColor();
    m_width = getNormalPenWidth();

    QHBoxLayout* m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_widthLabel);
    m_horizontalLayout->addWidget(m_widthSlider);
    m_layout->addLayout(m_horizontalLayout);
    connect(m_widthSlider, &QSlider::valueChanged, this, &NormalPenDialog::widthChanged);
    m_widthSlider->setRange(1, 12);
    m_widthSlider->setValue(static_cast<int>(getNormalPenWidth()));

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_colorFrame);
    m_horizontalLayout->addWidget(m_colorDialogBtn);
    m_layout->addLayout(m_horizontalLayout);
    m_colorFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_colorFrame->setAutoFillBackground(true);
    connect(m_colorDialogBtn, &QPushButton::clicked, this, &NormalPenDialog::openColorDialog);
    connect(this, &NormalPenDialog::colorChanged, this, &NormalPenDialog::setFrameColor);
    setFrameColor(getNormalPenColor());

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_confirmBtn);
    m_horizontalLayout->addWidget(m_cancelBtn);
    m_layout->addLayout(m_horizontalLayout);
    connect(m_confirmBtn, &QPushButton::clicked, this, &NormalPenDialog::onConfirm);
    connect(m_cancelBtn, &QPushButton::clicked, this, &NormalPenDialog::onCancel);
}

void NormalPenDialog::setFrameColor(QColor color)
{
    QPalette p = m_colorFrame->palette();
    p.setBrush(QPalette::ColorRole::Window, QBrush(color));
    m_colorFrame->setPalette(p);
}

void NormalPenDialog::widthChanged(int width)
{
    m_widthLabel->setText(QString("%1").arg(width));
    m_width = static_cast<qreal>(width);
}

void NormalPenDialog::openColorDialog()
{
    QColor color = QColorDialog::getColor(getNormalPenColor(), this);
    if (color.isValid())
    {
        m_color = color;
        emit colorChanged(color);
    }
}

QColor NormalPenDialog::getNormalPenColor()
{
    return WhiteBoardSettings::getInstance()
                          .value(WhiteBoardNormalPen::COLOR_KEY, QColor(WhiteBoardNormalPen::DEFAULT_COLOR))
                          .value<QColor>();
}

qreal NormalPenDialog::getNormalPenWidth()
{
    return WhiteBoardSettings::getInstance()
                          .value(WhiteBoardNormalPen::WIDTH_KEY, WhiteBoardNormalPen::DEFAULT_WIDTH)
                          .value<qreal>();
}

void NormalPenDialog::onConfirm()
{
    WhiteBoardSettings::getInstance().setValue(WhiteBoardNormalPen::WIDTH_KEY, m_width);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardNormalPen::COLOR_KEY, m_color);
    WhiteBoardSettings::getInstance().sync();
    m_controller->reloadToolSettings();
    accept();
}

void NormalPenDialog::onCancel()
{
    reject();
}

} // ADEV
