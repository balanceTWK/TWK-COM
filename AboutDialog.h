#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT

    QLabel m_logoLbl;
    QLabel m_qqLbl;
    QPlainTextEdit m_infoEdit;
    QPushButton m_closeBtn;
public:
    explicit AboutDialog(QWidget *parent = 0);

};

#endif // _ABOUTDIALOG_H_
