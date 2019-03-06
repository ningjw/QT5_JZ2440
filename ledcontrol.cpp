#include "ledcontrol.h"
#include "ui_ledcontrol.h"
#include "widget.h"
#include <QDebug>

#include <fcntl.h>
#include <unistd.h>

LedControl::LedControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LedControl)
{
    ui->setupUi(this);
}

LedControl::~LedControl()
{
    delete ui;
}

void LedControl::on_Button_Return_clicked()
{
    this->hide();//必须执行,否则返回显示主界面
    Widget w;
    w.show();
}

void control_led(char num,bool value)
{
    int fd;
    char led_val[2];
    led_val[0] = num;
    led_val[1] =value;
    fd = open("/dev/leds",O_RDWR);
    if (fd < 0){
        qDebug() << "can't open leds\n";
        return ;
    }
    write(fd, led_val, 2);
}

void LedControl::on_button_led1_ctl_clicked()
{
    bool value = this->ui->radio_led1_open->isChecked();
    control_led(1,value);
}

void LedControl::on_button_led2_ctl_clicked()
{
    bool value = this->ui->radio_led2_open->isChecked();
    control_led(2,value);
}

void LedControl::on_button_led3_ctl_clicked()
{
    bool value = this->ui->radio_led3_open->isChecked();
    control_led(3,value);
}

void LedControl::on_button_leds_ctl_clicked()
{
    bool value = this->ui->radio_leds_open->isChecked();
    control_led(0,value);
}
