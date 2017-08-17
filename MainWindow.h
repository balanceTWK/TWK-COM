#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets/QApplication>//加上这两行 就能用了QChart
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QCategoryAxis>
QT_CHARTS_USE_NAMESPACE//加上这两行 就能用了QChart
#include <QPointF>
#include <QList>

#include <QtSerialPort/QSerialPort>//提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>//提供系统中存在的串口的信息

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QtCore/QTimer>
#include <QMessageBox>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString RceiveStr;
    QString nameString;
    QString valString;

    QLabel StatusLabelSend;
    QLabel StatusLabelReceive;

    QSerialPort *serial;
    QSplineSeries *line=0;
    QChart *mychart;
    QValueAxis *m_axis;

    QGridLayout layout;

    QMessageBox *msgBox;

    QChartView ChartView;

    //qreal m_step;
    qreal m_x=0;
    qreal m_y=0;

    //QList<QPointF> pointfList;

    //QPushButton bt;

    void initMenu();
    void initSelectItem();
    void initqcharts();


    bool makeAction(QAction*& action, QWidget* parent, QString text, int key);//配置Qaction选项 用于菜单栏里的action
    bool makeAction(QAction*& action, QWidget* parent, QString tip, QString icon);//配置Qaction选项 用于工具栏里的action

private slots:
    void clearAllTextEdit();
    void Read_Data();
    void sendButton_clicked();
    void searchSerialPort();
    void OpenSerialportButton();
    void aboutSlots();
    void OpenSourceDialog();
    void useDialog();
};

#endif // MAINWINDOW_H
