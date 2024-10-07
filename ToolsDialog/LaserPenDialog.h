#pragma once

#include <QDialog>

class QFrame;
class QVBoxLayout;
class QPushButton;

namespace ADEV
{
class SceneController;
class LaserPenDialog : public QDialog
{
    Q_OBJECT
public:
    LaserPenDialog(SceneController* controller, QWidget* prarent = nullptr);

signals:
    void colorChanged(QColor color);

private slots:
    void openColorDialog();
    void onConfirm();
    void onCancel();

private:
    void setFrameColor(QColor color);
    QColor getLaserPenColor();

    QFrame* m_colorFrame;
    QPushButton* m_colorDialogBtn;

    QPushButton* m_confirmBtn;
    QPushButton* m_cancelBtn;
    QVBoxLayout* m_layout;
    SceneController* m_controller;
    QColor m_color;
};

} // ADEV

