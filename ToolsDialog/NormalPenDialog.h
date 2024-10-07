#pragma once
#include "SceneLayer/SceneController.h"

#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QSlider;
class QFrame;
class QPushButton;
class QLabel;

namespace ADEV
{

class NormalPenDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NormalPenDialog(SceneController* controller, QWidget* parent = nullptr);

signals:
    void colorChanged(QColor color);

private slots:
    void widthChanged(int width);
    void openColorDialog();
    void onConfirm();
    void onCancel();

private:
    void setFrameColor(QColor color);
    QColor getNormalPenColor();
    qreal getNormalPenWidth();
    QLabel* m_widthLabel;
    QSlider* m_widthSlider;
    QFrame* m_colorFrame;
    QPushButton* m_colorDialogBtn;
    QPushButton* m_confirmBtn;
    QPushButton* m_cancelBtn;
    QVBoxLayout* m_layout;
    SceneController* m_controller;
    QColor m_color;
    qreal m_width;
};

} // ADEV
