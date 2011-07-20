#ifndef UI_FRMSETTINGS_H
#define UI_FRMSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include "qwt_counter.h"

class Ui_frmSettings
{
public:
    QPushButton *botClose;
    QPushButton *botCancel;
    QGroupBox *groupBox;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_7;
    QLabel *label_2;
    QwtCounter *cNChan;
    QLabel *label_5;
    QLineEdit *txtSoundV;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *txtSteer;
    QLineEdit *txtAD;
    QLineEdit *txtElSpace;
    QLineEdit *txtTimeOff;
    QSpinBox *spinAVG;

    void setupUi(QWidget *frmSettings)
    {
    frmSettings->setObjectName(QString::fromUtf8("frmSettings"));
    frmSettings->resize(QSize(429, 207).expandedTo(frmSettings->minimumSizeHint()));
    botClose = new QPushButton(frmSettings);
    botClose->setObjectName(QString::fromUtf8("botClose"));
    botClose->setGeometry(QRect(350, 180, 75, 23));
    botCancel = new QPushButton(frmSettings);
    botCancel->setObjectName(QString::fromUtf8("botCancel"));
    botCancel->setGeometry(QRect(260, 180, 75, 23));
    groupBox = new QGroupBox(frmSettings);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 0, 411, 171));
    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(10, 50, 116, 16));
    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(40, 80, 96, 16));
    label_7 = new QLabel(groupBox);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(30, 140, 97, 16));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(60, 20, 64, 16));
    cNChan = new QwtCounter(groupBox);
    cNChan->setObjectName(QString::fromUtf8("cNChan"));
    cNChan->setGeometry(QRect(130, 20, 111, 20));
    cNChan->setNumButtons(2);
    cNChan->setProperty("basicstep", QVariant(1));
    cNChan->setMinValue(1);
    cNChan->setMaxValue(192);
    cNChan->setValue(1);
    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(50, 110, 72, 16));
    txtSoundV = new QLineEdit(groupBox);
    txtSoundV->setObjectName(QString::fromUtf8("txtSoundV"));
    txtSoundV->setGeometry(QRect(370, 140, 31, 20));
    txtSoundV->setMaxLength(4);
    label_8 = new QLabel(groupBox);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(260, 140, 97, 16));
    label_9 = new QLabel(groupBox);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(280, 110, 71, 16));
    txtSteer = new QLineEdit(groupBox);
    txtSteer->setObjectName(QString::fromUtf8("txtSteer"));
    txtSteer->setGeometry(QRect(370, 110, 31, 20));
    txtSteer->setMaxLength(4);
    txtAD = new QLineEdit(groupBox);
    txtAD->setObjectName(QString::fromUtf8("txtAD"));
    txtAD->setGeometry(QRect(130, 50, 51, 20));
    txtAD->setMaxLength(32767);
    txtElSpace = new QLineEdit(groupBox);
    txtElSpace->setObjectName(QString::fromUtf8("txtElSpace"));
    txtElSpace->setGeometry(QRect(130, 140, 51, 20));
    txtElSpace->setMaxLength(32767);
    txtTimeOff = new QLineEdit(groupBox);
    txtTimeOff->setObjectName(QString::fromUtf8("txtTimeOff"));
    txtTimeOff->setGeometry(QRect(130, 80, 51, 20));
    txtTimeOff->setMaxLength(32767);
    spinAVG = new QSpinBox(groupBox);
    spinAVG->setObjectName(QString::fromUtf8("spinAVG"));
    spinAVG->setGeometry(QRect(130, 110, 44, 22));
    spinAVG->setMaximum(16);
    spinAVG->setMinimum(1);
    retranslateUi(frmSettings);

    QMetaObject::connectSlotsByName(frmSettings);
    } // setupUi

    void retranslateUi(QWidget *frmSettings)
    {
    frmSettings->setWindowTitle(QApplication::translate("frmSettings", "Acquisition Data Settings", 0, QApplication::UnicodeUTF8));
    botClose->setText(QApplication::translate("frmSettings", "Close", 0, QApplication::UnicodeUTF8));
    botCancel->setText(QApplication::translate("frmSettings", "Cancel", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("frmSettings", "Data Parameters", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("frmSettings", "A/D sampling rate (MHz)", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("frmSettings", "Time Offset (usec)", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("frmSettings", "Element Space (mm)", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("frmSettings", "No. Channels", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("frmSettings", "Signal Average", 0, QApplication::UnicodeUTF8));
    txtSoundV->setInputMask(QApplication::translate("frmSettings", "9999; ", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("frmSettings", "Sound Velocity (m/s)", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("frmSettings", "Steering Angle", 0, QApplication::UnicodeUTF8));
    txtSteer->setInputMask(QApplication::translate("frmSettings", "9999; ", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(frmSettings);
    } // retranslateUi

};

namespace Ui {
    class frmSettings: public Ui_frmSettings {};
} // namespace Ui

#endif // UI_FRMSETTINGS_H
