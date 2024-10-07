#pragma once
#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QSlider;
class QFrame;
class QPushButton;
class QLabel;
class QCheckBox;

namespace ADEV
{
class SceneController;
class HighlightPenDialog: public QDialog
{
    Q_OBJECT
public:
    explicit HighlightPenDialog(SceneController* controller, QWidget* parent = nullptr);

signals:
    void colorChanged(QColor color);

private slots:
    void widthChanged(int width);
    void opacityChanged(int opacity);
    void modeChanged(int state);
    void openColorDialog();
    void onConfirm();
    void onCancel();

private:
    void setFrameColor(QColor color);
    QColor getHighlightPenColor();
    qreal getHighlightPenWidth();
    qreal getHighlightPenOpacity();
    bool getHighlightMode();
    QLabel* m_widthLabel;
    QSlider* m_widthSlider;

    QFrame* m_colorFrame;
    QPushButton* m_colorDialogBtn;

    QLabel* m_opacityLabel;
    QSlider* m_opacitySlider;

    QCheckBox* m_checkMode;

    QPushButton* m_confirmBtn;
    QPushButton* m_cancelBtn;

    QVBoxLayout* m_layout;
    SceneController* m_controller;
    QColor m_color;
    qreal m_width;
    qreal m_opacity;
    bool m_openStraightMode;
};

} // ADEV
