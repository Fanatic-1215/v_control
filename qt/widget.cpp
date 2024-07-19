#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    serialport = new QSerialPort(this);
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        ui->comboBox->addItem(info.portName());
    }

    // 读取数据
    connect(serialport,SIGNAL(readyRead()),this,SLOT(readData()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked(bool checked)
{
    if (checked == true) {
        // 串口配置到打开串口
        serialport->setPortName(ui->comboBox->currentText());
        serialport->setBaudRate(ui->comboBox_2->currentText().toInt());
        serialport->setStopBits(QSerialPort::StopBits(1));
        serialport->setDataBits(QSerialPort::DataBits(8));
        serialport->setParity(QSerialPort::NoParity);
        serialport->setFlowControl(QSerialPort::NoFlowControl); // 设置流控，问题二：流控是什么
        if (!serialport->open(QIODevice::ReadWrite)) {
            QMessageBox::about(this,"错误","串口打开失败可能被占用了");
            return;
        }
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton->setText("关闭串口");
    }
    if (checked == false) {
        serialport->close();
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton->setText("打开串口");
    }
}

void Widget::readData()
{

    // 接收数据
    QString data = serialport->readAll();
    ui->textBrowser->insertPlainText(data);

}

void Widget::on_pushButton_2_clicked()
{

    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) {
        ui->comboBox->addItem(info.portName());
    }
}

void Widget::on_pushButton_3_clicked()
{
    double value_v = ui->doubleSpinBox->value();
    int value_t = (int)(value_v * (4095 / 3.3 /4));
    char txbuf[20];
    sprintf(txbuf,"SV%d",value_t);
    serialport->write(txbuf);
    qDebug()<<txbuf;
    qDebug()<<value_t;
}
