#include "ShapePenDialog.h"
#include "Config/WhiteBoardSettings.h"
#include <QSlider>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QComboBox>

namespace ADEV
{
ShapePenDialog::ShapePenDialog(SceneController* controller, QWidget* parent)
    : QDialog(parent)
    , m_widthLabel(new QLabel(this))
    , m_widthSlider(new QSlider(Qt::Horizontal, this))
    , m_colorFrame(new QFrame(this))
    , m_colorDialogBtn(new QPushButton(tr("选择颜色"), this))
    , m_opacityLabel(new QLabel(this))
    , m_opacitySlider(new QSlider(Qt::Horizontal, this))
    , m_comboBox(new QComboBox(this))
    , m_confirmBtn(new QPushButton(tr("确认"), this))
    , m_cancelBtn(new QPushButton(tr("取消"), this))
    , m_layout(new QVBoxLayout(this))
    , m_controller(controller)
{
    m_color = getShapePenColor();
    m_width = getShapePenWidth();
    m_opacity = getShapePenOpacity();
    m_shape = getShape();

    QHBoxLayout* m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_widthLabel);
    m_horizontalLayout->addWidget(m_widthSlider);
    m_layout->addLayout(m_horizontalLayout);
    m_widthSlider->setRange(1, 70);
    connect(m_widthSlider, &QSlider::valueChanged, this, &ShapePenDialog::widthChanged);
    m_widthSlider->setValue(static_cast<int>(m_width));

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_opacityLabel);
    m_horizontalLayout->addWidget(m_opacitySlider);
    m_layout->addLayout(m_horizontalLayout);
    m_opacitySlider->setRange(1, 100);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &ShapePenDialog::opacityChanged);
    m_opacitySlider->setValue(static_cast<int>(m_opacity * 100));

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_colorFrame);
    m_horizontalLayout->addWidget(m_colorDialogBtn);
    m_layout->addLayout(m_horizontalLayout);
    m_colorFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_colorFrame->setAutoFillBackground(true);
    connect(m_colorDialogBtn, &QPushButton::clicked, this, &ShapePenDialog::openColorDialog);
    connect(this, &ShapePenDialog::colorChanged, this, &ShapePenDialog::setFrameColor);
    setFrameColor(m_color);

    m_layout->addWidget(m_comboBox);
    m_comboBox->addItem(tr("矩形"), WhiteBoardShapePen::Rectangle);
    m_comboBox->addItem(tr("椭圆"), WhiteBoardShapePen::Ellipse);
    m_comboBox->setCurrentIndex(m_comboBox->findData(m_shape));
    connect(m_comboBox, &QComboBox::currentIndexChanged, this, &ShapePenDialog::shapeChanged);

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_confirmBtn);
    m_horizontalLayout->addWidget(m_cancelBtn);
    m_layout->addLayout(m_horizontalLayout);
    connect(m_confirmBtn, &QPushButton::clicked, this, &ShapePenDialog::onConfirm);
    connect(m_cancelBtn, &QPushButton::clicked, this, &ShapePenDialog::onCancel);
}

void ShapePenDialog::setFrameColor(QColor color)
{
    QPalette p = m_colorFrame->palette();
    p.setBrush(QPalette::ColorRole::Window, QBrush(color));
    m_colorFrame->setPalette(p);
}

void ShapePenDialog::widthChanged(int width)
{
    m_widthLabel->setText(QString("%1").arg(width));
    m_width = static_cast<qreal>(width);
}

void ShapePenDialog::opacityChanged(int opacity)
{
    m_opacityLabel->setText(QString("%1").arg(opacity));
    m_opacity = opacity / 100.0;
}

void ShapePenDialog::shapeChanged(int index)
{
    m_shape = m_comboBox->itemData(index).value<WhiteBoardShapePen::ItemShape>();
}

void ShapePenDialog::openColorDialog()
{
    QColor color = QColorDialog::getColor(m_color, this);
    if (color.isValid())
    {
        m_color = color;
        emit colorChanged(color);
    }
}

QColor ShapePenDialog::getShapePenColor()
{
    return WhiteBoardSettings::getInstance()
                  .value(WhiteBoardShapePen::COLOR_KEY, QColor(WhiteBoardShapePen::DEFAULT_COLOR))
                  .value<QColor>();
}

qreal ShapePenDialog::getShapePenWidth()
{
    return WhiteBoardSettings::getInstance()
                  .value(WhiteBoardShapePen::WIDTH_KEY, WhiteBoardShapePen::DEFAULT_WIDTH)
                  .value<qreal>();
}

qreal ShapePenDialog::getShapePenOpacity()
{
    qreal default_opacity = WhiteBoardShapePen::DEFAULT_OPACITY;
    return WhiteBoardSettings::getInstance()
        .value(WhiteBoardShapePen::OPACITY_KEY, default_opacity)
        .value<qreal>();
}

WhiteBoardShapePen::ItemShape ShapePenDialog::getShape()
{
    return WhiteBoardSettings::getInstance()
        .value(WhiteBoardShapePen::SHAPE_KEY, WhiteBoardShapePen::DEFAULT_SHAPE)
        .value<WhiteBoardShapePen::ItemShape>();
}

void ShapePenDialog::onConfirm()
{
    WhiteBoardSettings::getInstance().setValue(WhiteBoardShapePen::WIDTH_KEY, m_width);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardShapePen::COLOR_KEY, m_color);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardShapePen::OPACITY_KEY, m_opacity);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardShapePen::SHAPE_KEY, m_shape);
    WhiteBoardSettings::getInstance().sync();
    m_controller->reloadToolSettings();
    accept();
}

void ShapePenDialog::onCancel()
{
    reject();
}

} // ADEV
