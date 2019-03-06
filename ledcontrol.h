#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <QDialog>

namespace Ui {
class LedControl;
}

class LedControl : public QDialog
{
    Q_OBJECT

public:
    explicit LedControl(QWidget *parent = 0);
    ~LedControl();

private slots:
    void on_Button_Return_clicked();

    void on_button_led1_ctl_clicked();

    void on_button_led2_ctl_clicked();

    void on_button_led3_ctl_clicked();

    void on_button_leds_ctl_clicked();

private:
    Ui::LedControl *ui;
};

#endif // LEDCONTROL_H
