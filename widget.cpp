#include "widget.h"
#include "ui_widget.h"
#include "ledcontrol.h"
#include "serial.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
//    QIcon led_ctl_icon(":/img/led_control.png");
//    QPixmap icon1(":/img/led_control.png");
    ui->setupUi(this);
//    ui->led_ctl_entrance->setIcon(led_ctl_icon);
//    ui->led_ctl_entrance->setIconSize(QSize(50,50));
//    ui->label->setAttribute(Qt::WA_TranslucentBackground);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_led_ctl_entrance_clicked()
{
    LedControl led_dialog;
    led_dialog.exec();
}

void Widget::on_led_ctl_entrance_2_clicked()
{
    serial serial_dialog;
    serial_dialog.exec();
}
