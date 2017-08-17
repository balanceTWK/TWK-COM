#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include "AboutDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStatusBar* sb = statusBar();

    serial = new QSerialPort;

    StatusLabelSend.setMinimumWidth(150);
    StatusLabelSend.setAlignment(Qt::AlignLeft);
    StatusLabelSend.setText("发送数(Send):0  ");

    StatusLabelReceive.setMinimumWidth(150);
    StatusLabelReceive.setAlignment(Qt::AlignLeft);
    StatusLabelReceive.setText("接收数(Receive):0  ");

    sb->addPermanentWidget(&StatusLabelReceive);
    sb->addPermanentWidget(&StatusLabelSend);

    /*QT tabWidget 设置背景透明. 这样保证背景透明而上面的控件正常显示*/
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }");//设置选项卡为透明的
    ui->ReceivePlainTextEdit->setReadOnly(true);

    initMenu();
    initqcharts();
    initSelectItem();
    searchSerialPort();

    ///---实例化消息对话框对象

    msgBox = new QMessageBox("说明",       ///--这里是设置消息框标题
        "如果需要使用波形图功能,下位机发送的字符串格式必须为:\n\n      #-_-#%f#^_^#\n\n%f表示你要发送的浮点数,#-_-#是这个数据的开头,#^_^#是这个数据的结尾.\n\n注意!是字符串!\n\n建议下位机300ms printf一次数据. ",                       ///--这里是设置消息框显示的内容
        QMessageBox::Information,                          ///--这里是在消息框显示的图标
        QMessageBox::Ok ,     ///---这里是显示消息框上的按钮情况
        0,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
        0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮

    QObject::connect(ui->OpenSerialportButton,SIGNAL(clicked(bool)),this,SLOT (OpenSerialportButton()),Qt::UniqueConnection);
    QObject::connect(ui->ClearReceiveButton,SIGNAL(clicked(bool)),this,SLOT (clearAllTextEdit()),Qt::UniqueConnection);
    QObject::connect(ui->SearchButton,SIGNAL(clicked(bool)),this,SLOT (searchSerialPort()),Qt::UniqueConnection);
    QObject::connect(ui->SendButton,SIGNAL(clicked(bool)),this,SLOT (sendButton_clicked()),Qt::UniqueConnection);
}

void MainWindow::initSelectItem()
{
    ui->setBaudRate->addItem("1200",1200);
    ui->setBaudRate->addItem("2400",2400);
    ui->setBaudRate->addItem("4800",4800);
    ui->setBaudRate->addItem("9600",9600);
    ui->setBaudRate->addItem("14400",14400);
    ui->setBaudRate->addItem("19200",19200);
    ui->setBaudRate->addItem("28400",28400);
    ui->setBaudRate->addItem("43000",43000);
    ui->setBaudRate->addItem("57600",57600);
    ui->setBaudRate->addItem("76800",76800);
    ui->setBaudRate->addItem("115200",115200);
    ui->setBaudRate->setCurrentIndex(3);

    ui->setStopBits->addItem("1",1);
    ui->setStopBits->addItem("1.5",1.5);
    ui->setStopBits->addItem("2",2);

    ui->setParitycheck->addItem("无",0);
    ui->setParitycheck->addItem("奇校验",1);
    ui->setParitycheck->addItem("偶校验",2);

    ui->setDataBits->addItem("8",8);
    ui->setDataBits->addItem("7",7);
    ui->setDataBits->addItem("6",6);
    ui->setDataBits->addItem("5",5);
}

void MainWindow::initqcharts()
{
    mychart=new QChart;
    line=new QSplineSeries(this);
    m_axis=new QValueAxis;
    QPen lineColor(Qt::red);
    lineColor.setWidth(1);
    line->setPen(lineColor);
    line->append(m_x, m_y);

    mychart->addSeries(line);
    mychart->createDefaultAxes();
    mychart->setAxisX(m_axis,line);

    m_axis->setTickCount(11);

    mychart->axisX()->setRange(-45,5);
    mychart->axisY()->setRange(-25,25);

    mychart->setTitle("波形图");
    mychart->legend()->hide();
    mychart->setAnimationOptions(QChart::SeriesAnimations);
    mychart->setAnimationDuration(300);
    mychart->setTheme(QChart::ChartThemeDark);

    ChartView.setChart(mychart);
    ChartView.setRenderHint(QPainter::Antialiasing);

    ChartView.setParent( ui->tab_2);

    layout.addWidget(&ChartView);
    ui->tab_2->setLayout(&layout);

}

void MainWindow::initMenu()
{
    QMenu *aboutMenu=new QMenu("帮助",ui->menuBar);

    QAction *action=new QAction;

    makeAction(action,aboutMenu,"关于(A)", Qt::CTRL + Qt::Key_A);
    connect(action,SIGNAL(triggered()),this,SLOT(aboutSlots()));
    aboutMenu->addAction(action);

    makeAction(action,aboutMenu,"开源(O)", Qt::CTRL + Qt::Key_O);
    connect(action,SIGNAL(triggered()),this,SLOT(OpenSourceDialog()));
    aboutMenu->addAction(action);

    makeAction(action,aboutMenu,"使用说明(U)", Qt::CTRL + Qt::Key_U);
    connect(action,SIGNAL(triggered()),this,SLOT(useDialog()));
    aboutMenu->addAction(action);

    ui->menuBar->addMenu(aboutMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString text, int key)
{
    bool ret = true;

    action = new QAction(text, parent);

    if( action != NULL )
    {
        action->setShortcut(QKeySequence(key));
    }
    else
    {
        ret = false;
    }

    return ret;
}
bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString tip, QString icon)//配置Qaction选项 用于工具栏里的action
{
    bool ret = true;

    action = new QAction("", parent);

    if( action != NULL )
    {
        action->setToolTip(tip);
        action->setIcon(QIcon(icon));
    }
    else
    {
        ret = false;
    }

    return ret;
}
