#include "HighlightPenDialog.h"
#include "Config/WhiteBoardSettings.h"
#include "SceneLayer/SceneController.h"

#include <QSlider>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QCheckBox>

namespace ADEV
{
HighlightPenDialog::HighlightPenDialog(SceneController* controller, QWidget* parent)
    : QDialog(parent)
    , m_widthLabel(new QLabel(this))
    , m_widthSlider(new QSlider(Qt::Horizontal, this))
    , m_colorFrame(new QFrame(this))
    , m_colorDialogBtn(new QPushButton(tr("选择颜色"), this))
    , m_opacityLabel(new QLabel(this))
    , m_opacitySlider(new QSlider(Qt::Horizontal, this))
    , m_checkMode(new QCheckBox(this))
    , m_confirmBtn(new QPushButton(tr("确认"), this))
    , m_cancelBtn(new QPushButton(tr("取消"), this))
    , m_layout(new QVBoxLayout(this))
    , m_controller(controller)
{
    m_color = getHighlightPenColor();
    m_width = getHighlightPenWidth();
    qDebug() << "width = " << m_width;
    m_opacity = getHighlightPenOpacity();
    qDebug() << "opacity = " << m_opacity;
    m_openStraightMode = getHighlightMode();

    QHBoxLayout* m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_widthLabel);
    m_horizontalLayout->addWidget(m_widthSlider);
    m_layout->addLayout(m_horizontalLayout);
    m_widthSlider->setRange(1, 70);
    connect(m_widthSlider, &QSlider::valueChanged, this, &HighlightPenDialog::widthChanged);
    m_widthSlider->setValue(static_cast<int>(m_width));

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_opacityLabel);
    m_horizontalLayout->addWidget(m_opacitySlider);
    m_layout->addLayout(m_horizontalLayout);
    m_opacitySlider->setRange(1, 100);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &HighlightPenDialog::opacityChanged);
    m_opacitySlider->setValue(static_cast<int>(m_opacity * 100));

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_colorFrame);
    m_horizontalLayout->addWidget(m_colorDialogBtn);
    m_layout->addLayout(m_horizontalLayout);
    m_colorFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_colorFrame->setAutoFillBackground(true);
    connect(m_colorDialogBtn, &QPushButton::clicked, this, &HighlightPenDialog::openColorDialog);
    connect(this, &HighlightPenDialog::colorChanged, this, &HighlightPenDialog::setFrameColor);
    setFrameColor(m_color);

    m_layout->addWidget(m_checkMode);
    m_checkMode->setText(tr("开启直线绘制模式"));
    connect(m_checkMode, &QCheckBox::stateChanged, this, &HighlightPenDialog::modeChanged);
    m_checkMode->setCheckState(m_openStraightMode? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_confirmBtn);
    m_horizontalLayout->addWidget(m_cancelBtn);
    m_layout->addLayout(m_horizontalLayout);
    connect(m_confirmBtn, &QPushButton::clicked, this, &HighlightPenDialog::onConfirm);
    connect(m_cancelBtn, &QPushButton::clicked, this, &HighlightPenDialog::onCancel);
}

void HighlightPenDialog::setFrameColor(QColor color)
{
    QPalette p = m_colorFrame->palette();
    p.setBrush(QPalette::ColorRole::Window, QBrush(color));
    m_colorFrame->setPalette(p);
}

void HighlightPenDialog::widthChanged(int width)
{
    m_widthLabel->setText(QString("%1").arg(width));
    m_width = static_cast<qreal>(width);
}

void HighlightPenDialog::opacityChanged(int opacity)
{
    m_opacityLabel->setText(QString("%1").arg(opacity));
    m_opacity = opacity / 100.0;
}

void HighlightPenDialog::modeChanged(int state)
{
    m_openStraightMode = state == Qt::Checked;
}

void HighlightPenDialog::openColorDialog()
{
    QColor color = QColorDialog::getColor(getHighlightPenColor(), this);
    if (color.isValid())
    {
        m_color = color;
        emit colorChanged(color);
    }
}

QColor HighlightPenDialog::getHighlightPenColor()
{
    return WhiteBoardSettings::getInstance()
                  .value(WhiteBoardHighlightPen::COLOR_KEY, QColor(WhiteBoardHighlightPen::DEFAULT_COLOR))
                  .value<QColor>();
}

qreal HighlightPenDialog::getHighlightPenWidth()
{
    return WhiteBoardSettings::getInstance()
                  .value(WhiteBoardHighlightPen::WIDTH_KEY, WhiteBoardHighlightPen::DEFAULT_WIDTH)
                  .value<qreal>();
}

qreal HighlightPenDialog::getHighlightPenOpacity()
{
    qreal default_opacity = WhiteBoardHighlightPen::DEFAULT_OPACITY;
    return WhiteBoardSettings::getInstance()
        .value(WhiteBoardHighlightPen::OPACITY_KEY, default_opacity)
        .value<qreal>();
}

bool HighlightPenDialog::getHighlightMode()
{
    return WhiteBoardSettings::getInstance()
        .value(WhiteBoardHighlightPen::MODE_KEY, WhiteBoardHighlightPen::DEFAULT_STARIGHT_LINE_MODE).toBool();
}

void HighlightPenDialog::onConfirm()
{
    WhiteBoardSettings::getInstance().setValue(WhiteBoardHighlightPen::WIDTH_KEY, m_width);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardHighlightPen::COLOR_KEY, m_color);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardHighlightPen::OPACITY_KEY, m_opacity);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardHighlightPen::MODE_KEY, m_openStraightMode);
    WhiteBoardSettings::getInstance().sync();
    m_controller->reloadToolSettings();
    accept();
}

void HighlightPenDialog::onCancel()
{
    reject();
}

} // ADEV
