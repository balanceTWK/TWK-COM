#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include "AboutDialog.h"
#include <QtSerialPort/QSerialPort>//提供访问串口的功能
#include <QErrorMessage>
#include <QMessageBox>

void MainWindow::searchSerialPort()
{

    ui->setPort->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort ser;
        ser.clear();
        ser.setPort(info);
        if(ser.open(QIODevice::ReadWrite))
        {
            ui->setPort->addItem(ser.portName());

            ser.close();
        }
    }
}

void MainWindow::OpenSerialportButton()
{

    if(ui->OpenSerialportButton->text()==tr("打开串口"))
    {
        //searchSerialPort();




        //设置串口名
        serial->setPortName(ui->setPort->currentText());
        //serial->setPortName(ui->setPort->currentText());


        //设置波特率
        serial->setBaudRate(ui->setBaudRate->currentText().toInt(),QSerialPort::AllDirections);

        //设置数据位数
        switch(ui->setDataBits->currentIndex())
        {
            case 0:serial->setDataBits(QSerialPort::Data8);break;
            case 1:serial->setDataBits(QSerialPort::Data7);break;
            case 2:serial->setDataBits(QSerialPort::Data6);break;
            case 3:serial->setDataBits(QSerialPort::Data5);break;
            default: break;
        }


        //设置奇偶校验
        switch(ui->setParitycheck->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        case 1: serial->setParity(QSerialPort::OddParity); break;
        case 2: serial->setParity(QSerialPort::EvenParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->setStopBits->currentIndex())
        {
        case 0: serial->setStopBits(QSerialPort::OneStop); break;
        case 1: serial->setStopBits(QSerialPort::OneAndHalfStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //serial->setFlowControl(QSerialPort::SoftwareControl);
        //关闭设置菜单使能
        //ui->PortBox->setEnabled(false);
        ui->setBaudRate->setEnabled(false);
        ui->setStopBits->setEnabled(false);
        ui->setParitycheck->setEnabled(false);
        ui->setDataBits->setEnabled(false);
        ui->setPort->setEnabled(false);
        ui->SearchButton->setEnabled(false);

        ui->SendButton->setEnabled(true);

        ui->OpenSerialportButton->setText(tr("关闭串口"));
        //连接信号槽
        QObject::connect(serial,SIGNAL(readyRead()),this,SLOT (Read_Data()),Qt::UniqueConnection);

        //打开串口
        serial->open(QIODevice::ReadWrite);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        //恢复设置使能
        ui->setBaudRate->setEnabled(true);
        ui->setStopBits->setEnabled(true);
        ui->setParitycheck->setEnabled(true);
        ui->setDataBits->setEnabled(true);
        ui->setPort->setEnabled(true);
        ui->SearchButton->setEnabled(true);

        ui->SendButton->setEnabled(false);

        ui->OpenSerialportButton->setText(tr("打开串口"));
        QObject::disconnect(serial,SIGNAL(readyRead()),this,SLOT (Read_Data()));
    }
}

//清屏
void MainWindow::clearAllTextEdit()
{
    ui->ReceivePlainTextEdit->clear();
    ui->SendPlainTextEdit->clear();
    RceiveStr.clear();
    m_x=0;
    line->clear();
}

//读取接收到的数据
void MainWindow::Read_Data()
{
    QByteArray Rceivebuff;
    static int i=0;
    //static int sw=0;
    Rceivebuff = serial->readAll();
    if(!Rceivebuff.isEmpty())
    {
        i++;
        if(ui->tabWidget->currentIndex()==0)
        {
            if(RceiveStr.isNull())RceiveStr =ui->ReceivePlainTextEdit->toPlainText();
            RceiveStr+=QString::fromLocal8Bit(Rceivebuff);
            ui->ReceivePlainTextEdit->clear();
            ui->ReceivePlainTextEdit->appendPlainText(RceiveStr);

            RceiveStr.clear();
        }
        else if(ui->tabWidget->currentIndex()==1)
        {
            QString str;
            QString valfirst = "#-_-#";
            QString vallast = "#^_^#";
            QString val;
            int indexFirst;
            int indexLast;
            double dou;

            if(RceiveStr.isNull())RceiveStr =ui->ReceivePlainTextEdit->toPlainText();
            RceiveStr+=QString::fromLocal8Bit(Rceivebuff);
            str=QString::fromLocal8Bit(Rceivebuff);

            indexFirst=str.indexOf(valfirst,0);
            indexLast=str.indexOf(vallast,0);
            val=str.mid(indexFirst+5,indexLast-5-indexFirst);

            dou=val.toDouble();
            //dou=val.toFloat();
            m_x += 5;
            m_y = qreal(dou);

            if(line->count()>120)line->clear();
            if(m_x>600)m_x=0;
            line->append(m_x,m_y);

            mychart->axisX()->setRange(m_x-45,m_x+5);

            //indexFirst+1;
            //indexLast-indexFirst;

            //handleTimeout();
        }
        StatusLabelReceive.setText("接收数(Receive):" + QString::number(i)+"  ");
    }
    Rceivebuff.clear();

}

//发送数据
void MainWindow::sendButton_clicked()
{
    static int i=0;
    i++;
    QByteArray buf;
    buf=ui->SendPlainTextEdit->toPlainText().toUtf8();
    if(ui->radioButton->isChecked())
    {
        buf.append(0x0d);
        buf.append(0x0a);
    }
    serial->write(buf);
    StatusLabelSend.setText("发送数(Send):" + QString::number(i)+"  ");
}

void MainWindow::useDialog()
{
    msgBox->show();                                  ///---显示消息框
}

void MainWindow::aboutSlots()
{
    AboutDialog(this).exec();
}

void MainWindow::OpenSourceDialog()
{
    QDesktopServices::openUrl(QUrl("https://github.com/balanceTWK"));
}
