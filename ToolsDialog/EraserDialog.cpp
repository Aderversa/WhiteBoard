#include "ToolsDialog/EraserDialog.h"
#include "Config/WhiteBoardSettings.h"
#include "SceneLayer/SceneController.h"

#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>

namespace ADEV
{
EraserDialog::EraserDialog(SceneController* controller, QWidget* parent)
    : QDialog(parent)
    , m_radiusLabel(new QLabel(this))
    , m_radiusSlider(new QSlider(Qt::Horizontal, this))
    , m_checkMode(new QCheckBox(this))
    , m_confirmBtn(new QPushButton(tr("确认"), this))
    , m_cancelBtn(new QPushButton(tr("取消"), this))
    , m_layout(new QVBoxLayout(this))
    , m_controller(controller)
{
    m_radius = getEraserRadius();
    m_eraseWholeItem = getMode();

    QHBoxLayout* m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_radiusLabel);
    m_horizontalLayout->addWidget(m_radiusSlider);
    m_layout->addLayout(m_horizontalLayout);
    m_radiusSlider->setRange(10, 120);
    connect(m_radiusSlider, &QSlider::valueChanged, this, &EraserDialog::radiusChanged);
    m_radiusSlider->setValue(static_cast<int>(m_radius));

    m_layout->addWidget(m_checkMode);
    m_checkMode->setText(tr("擦除整个笔画"));
    connect(m_checkMode, &QCheckBox::stateChanged, this, &EraserDialog::modeChanged);
    m_checkMode->setCheckState(m_eraseWholeItem? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

    m_horizontalLayout = new QHBoxLayout;
    m_horizontalLayout->addWidget(m_confirmBtn);
    m_horizontalLayout->addWidget(m_cancelBtn);
    m_layout->addLayout(m_horizontalLayout);
    connect(m_confirmBtn, &QPushButton::clicked, this, &EraserDialog::onConfirm);
    connect(m_cancelBtn, &QPushButton::clicked, this, &EraserDialog::onCancel);
}

void EraserDialog::modeChanged(int state)
{
    m_eraseWholeItem = state == Qt::Checked;
}

void EraserDialog::radiusChanged(int radius)
{
    m_radiusLabel->setText(QString("%1").arg(radius));
    m_radius = static_cast<qreal>(radius);
}

qreal EraserDialog::getEraserRadius()
{
    return WhiteBoardSettings::getInstance()
                          .value(WhiteBoardEraser::RADIUS_KEY, WhiteBoardEraser::DEFAULT_RADIUS)
                          .value<qreal>();
}

bool EraserDialog::getMode()
{
    return WhiteBoardSettings::getInstance()
                          .value(WhiteBoardEraser::ERASE_WHOLE_KEY, WhiteBoardEraser::DEFAULT_ERASE_WHOLE)
                          .toBool();
}

void EraserDialog::onConfirm()
{
    WhiteBoardSettings::getInstance().setValue(WhiteBoardEraser::RADIUS_KEY, m_radius);
    WhiteBoardSettings::getInstance().setValue(WhiteBoardEraser::ERASE_WHOLE_KEY, m_eraseWholeItem);
    WhiteBoardSettings::getInstance().sync();
    m_controller->reloadToolSettings();
    accept();
}

void EraserDialog::onCancel()
{
    reject();
}

} // ADEV
