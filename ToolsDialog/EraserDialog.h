#pragma once
#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QSlider;
class QLabel;
class QCheckBox;

namespace ADEV
{
class SceneController;
class EraserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EraserDialog(SceneController* controller, QWidget* parent = nullptr);

signals:
    void colorChanged(QColor color);

private slots:
    void radiusChanged(int radius);
    void modeChanged(int state);
    void onConfirm();
    void onCancel();

private:
    qreal getEraserRadius();
    bool getMode();
    QLabel* m_radiusLabel;
    QSlider* m_radiusSlider;

    QCheckBox* m_checkMode;

    QPushButton* m_confirmBtn;
    QPushButton* m_cancelBtn;
    QVBoxLayout* m_layout;
    SceneController* m_controller;
    qreal m_radius;
    bool m_eraseWholeItem;
};

} // ADEV
