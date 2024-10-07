#pragma once
#include "SceneLayer/SceneController.h"
#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QSlider;
class QFrame;
class QPushButton;
class QLabel;
class QComboBox;

namespace ADEV
{
class SceneController;
class ShapePenDialog: public QDialog
{
    Q_OBJECT
public:
    explicit ShapePenDialog(SceneController* controller, QWidget* parent = nullptr);

signals:
    void colorChanged(QColor color);

private slots:
    void widthChanged(int width);
    void opacityChanged(int opacity);
    void shapeChanged(int index);
    void openColorDialog();
    void onConfirm();
    void onCancel();

private:
    void setFrameColor(QColor color);
    QColor getShapePenColor();
    qreal  getShapePenWidth();
    qreal  getShapePenOpacity();
    WhiteBoardShapePen::ItemShape getShape();
    QLabel* m_widthLabel;
    QSlider* m_widthSlider;

    QFrame* m_colorFrame;
    QPushButton* m_colorDialogBtn;

    QLabel* m_opacityLabel;
    QSlider* m_opacitySlider;

    QComboBox* m_comboBox;

    QPushButton* m_confirmBtn;
    QPushButton* m_cancelBtn;

    QVBoxLayout* m_layout;
    SceneController* m_controller;
    QColor m_color;
    qreal m_width;
    qreal m_opacity;
    WhiteBoardShapePen::ItemShape m_shape;
};

} // ADEV
