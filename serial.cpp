#include "serial.h"
#include "ui_serial.h"
#include "widget.h"
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct termios opt;

serial::serial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);
}

serial::~serial()
{
    delete ui;
}

int set_uart(int fd,int baud,int databits,int parity,int stopbits,int flow_ctrl)
{
    int i;
    speed_t baud_rates[] = { B9600, B19200, B38400, B57600, B115200 };
    int speed_rates[] = {9600, 19200, 38400, 57600, 115200};

    /*得到与fd指向对象的相关参数，并保存*/
    tcgetattr(fd, &opt);
    for ( i= 0;  i < 5;  i++){
        if (baud == speed_rates[i]){
            cfsetispeed(&opt, baud_rates[i]);
            cfsetospeed(&opt, baud_rates[i]);
        }
    }
    opt.c_cflag |= CLOCAL;/*修改控制模式，保证程序不会占用串口*/
    opt.c_cflag |= CREAD;/*修改控制模式，使得能够从串口中读取输入数据 */

    /*设置数据流控制 */
    switch(flow_ctrl){
    case 0 ://不使用流控制
            opt.c_cflag &= ~CRTSCTS;
            break;
    case 1 ://使用硬件流控制
            opt.c_cflag |= CRTSCTS;
            break;
    case 2 ://使用软件流控制
            opt.c_cflag |= IXON | IXOFF | IXANY;
            break;
    default:
            qDebug() << "Unsupported mode flow\n";
            break;
        }

    /*屏蔽其他标志位  */
        opt.c_cflag &= ~CSIZE;
    /*设置数据位*/
    switch (databits){
        case 5 :
             opt.c_cflag |= CS5;
             break;
        case 6 :
             opt.c_cflag |= CS6;
             break;
        case 7 :
            opt.c_cflag |= CS7;
            break;
        case 8:
            opt.c_cflag |= CS8;
            break;
        default:
            qDebug() << "Unsupported data size\n";
            break;
     }

    /*设置校验位 */
     switch (parity){
        case 'n':
        case 'N': //无奇偶校验位。
            opt.c_cflag &= ~PARENB;
            opt.c_iflag &= ~INPCK;
            break;
        case 'o':
        case 'O'://设置为奇校验
             opt.c_cflag |= (PARODD | PARENB);
             opt.c_iflag |= INPCK;
             break;
        case 'e':
        case 'E'://设置为偶校验
             opt.c_cflag |= PARENB;
             opt.c_cflag &= ~PARODD;
             opt.c_iflag |= INPCK;
             break;
        case 's':
        case 'S': //设置为空格
            opt.c_cflag &= ~PARENB;
            opt.c_cflag &= ~CSTOPB;
            break;
         default:
            qDebug() << "Unsupported parity\n";
         break;
     }

     /* 设置停止位 */
     switch (stopbits){
        case 1:
            opt.c_cflag &= ~CSTOPB;
            break;
        case 2:
            opt.c_cflag |= CSTOPB;
            break;
        default:
             qDebug() << "Unsupported stop bits\n";
             break;
     }

    tcflush(fd, TCIFLUSH);/*如果发生数据溢出，接收数据，但是不再读取*/

    /*如果有数据可用，则read最多返回所要求的字节数。如果无数据可用，则read立即返回0*/
    opt.c_cc[VTIME] = 0;        //设置超时
    opt.c_cc[VMIN] = 0;        //Update the Opt and do it now

    /*
    *使能配置
    *TCSANOW：立即执行而不等待数据发送或者接受完成
    *TCSADRAIN：等待所有数据传递完成后执行
    *TCSAFLUSH：输入和输出buffers  改变时执行
    */
    tcsetattr(fd,TCSANOW,&opt);
    return 0;
}


void open_uart(QString *uart_dev,int baud,int databits,int parity,int stopbits,int flow_ctrl)
{
    int fd;
    const char*  ch;
//    fd_set uart_read;
//    struct timeval time;
//    pid_t fpid; //fpid表示fork函数返回的值
    char str[] = "uart0 send test\n";
    QByteArray ba = uart_dev->toLatin1();
    ch =ba.data();

    fd = open(ch, O_RDWR | O_NOCTTY);
    if (fd < 0){
        qDebug() << "can't open ttySAC0\n";
        return ;
    }
    /*测试是否为终端设备 */
    if(0 == isatty(STDIN_FILENO))
        qDebug() << "standard input is not a terminal device/n";
    /*获取界面设置*/

    set_uart(fd,baud,databits,parity,stopbits,flow_ctrl);
//    FD_ZERO(&uart_read);/* 将uart_read清零使集合中不含任何fd*/
//    FD_SET(fd,&uart_read);/* 将fd加入uart_read集合 */
//    time.tv_sec = 0;/*不阻塞*/
//    time.tv_usec = 0;
//    fpid=fork();
//    if (fpid < 0){
//        qDebug() << "error in fork!\n";
//        return ;
//    }
    write(fd, str, strlen(str));
}

void serial::on_bt_uart_open_clicked()
{
    QString uart_dev = "/dev/";
    int baudrate = ui->comboBox_baud->currentText().toInt();//波特率
    int databits = ui->comboBox_data_bit->currentText().toInt();//数据位
    int parity = ui->comboBox_parity->currentIndex();
    int stopbits = ui->comboBox_stop_bit->currentText().toInt();//停止位
    int flow_ctrl = ui->comboBox_flow_ctrl->currentIndex();
    uart_dev += ui->comboBox_uart_num->currentText();//设备号

    qDebug() << "打开串口:"+uart_dev +" "+
                QString::number(baudrate) +" "+
                QString::number(databits) +" "+
                QString::number(parity)+" "+
                QString::number(stopbits);
    open_uart(&uart_dev,baudrate,databits,parity,stopbits,flow_ctrl);
}

void serial::on_bt_uart_send_clicked()
{

}

void serial::on_bt_return_clicked()
{
    this->hide();//必须执行,否则返回显示主界面
    Widget w;
    w.show();
}
