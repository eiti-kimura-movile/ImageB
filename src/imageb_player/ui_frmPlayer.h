#ifndef UI_FRMPLAYER_H
#define UI_FRMPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QWidget>

class Ui_frmPlayer
{
public:

    void setupUi(QWidget *frmPlayer)
    {
    frmPlayer->setObjectName(QString::fromUtf8("frmPlayer"));
    frmPlayer->resize(QSize(606, 487).expandedTo(frmPlayer->minimumSizeHint()));
    retranslateUi(frmPlayer);

    QMetaObject::connectSlotsByName(frmPlayer);
    } // setupUi

    void retranslateUi(QWidget *frmPlayer)
    {
    frmPlayer->setWindowTitle(QApplication::translate("frmPlayer", "ImageB Player - Ultrasound Imaging System 0.0.1", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(frmPlayer);
    } // retranslateUi

};

namespace Ui {
    class frmPlayer: public Ui_frmPlayer {};
} // namespace Ui

#endif // UI_FRMPLAYER_H
