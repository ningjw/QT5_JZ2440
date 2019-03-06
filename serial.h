#ifndef SERIAL_H
#define SERIAL_H

#include <QDialog>

namespace Ui {
class serial;
}

class serial : public QDialog
{
    Q_OBJECT

public:
    explicit serial(QWidget *parent = 0);
    ~serial();
private slots:

    void on_bt_uart_open_clicked();

    void on_bt_uart_send_clicked();

    void on_bt_return_clicked();

private:
    Ui::serial *ui;
};

#endif // SERIAL_H
