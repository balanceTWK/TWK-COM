#include "AboutDialog.h"
#include <QPixmap>
#include <QPalette>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint), m_logoLbl(this), m_qqLbl(this), m_infoEdit(this), m_closeBtn(this)
{
    QPixmap pm(":/res/pic/wolf.jpg");

    pm = pm.scaled(120, 120, Qt::KeepAspectRatio);

    m_logoLbl.setPixmap(pm);
    m_logoLbl.move(20, 20);
    m_logoLbl.resize(120, 120);

    QPalette p = m_infoEdit.palette();

    p.setColor(QPalette::Active, QPalette::Base, palette().color(QPalette::Active, QPalette::Background));
    p.setColor(QPalette::Inactive, QPalette::Base, palette().color(QPalette::Inactive, QPalette::Background));

    m_infoEdit.move(200, 30);
    m_infoEdit.resize(180, 130);
    m_infoEdit.setPalette(p);
    m_infoEdit.setFrameStyle(QFrame::NoFrame);
    m_infoEdit.setReadOnly(true);
    m_infoEdit.insertPlainText("串口助手  Project\n\nPlatform: Qt 5.8.0\n\nVersion:  1.0.3\n\nAuthor:   TWK");

    m_qqLbl.move(22, 184);
    m_qqLbl.setText("邮箱(Email): linuxtwk@outlook.com\n微信公众号:laiyiketang");

    m_closeBtn.setText("Close");
    m_closeBtn.move(273, 175);
    m_closeBtn.resize(100, 30);

    setFixedSize(390, 230);
    setWindowTitle("About 串口助手");

    connect(&m_closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}
