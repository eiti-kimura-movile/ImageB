#ifndef UI_FRMSFSAFT_H
#define UI_FRMSFSAFT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "qwt_counter.h"

class Ui_frmSFSAFT
{
public:
    QPushButton *botOpen;
    QPushButton *botCancel;
    QPushButton *botClose;
    QLabel *lblFilePath;
    QGroupBox *groupBox_2;
    QLabel *label_9;
    QProgressBar *progBar;
    QPushButton *botProc;
    QLabel *lblStatus;
    QGroupBox *groupBox;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_2;
    QLineEdit *txtTimeOff;
    QComboBox *cmbSample;
    QLabel *label_8;
    QwtCounter *cNChan;
    QLineEdit *txtElSpace;
    QLineEdit *txtAD;
    QLineEdit *txtSoundV;

    void setupUi(QWidget *frmSFSAFT)
    {
    frmSFSAFT->setObjectName(QString::fromUtf8("frmSFSAFT"));
    frmSFSAFT->resize(QSize(430, 328).expandedTo(frmSFSAFT->minimumSizeHint()));
    botOpen = new QPushButton(frmSFSAFT);
    botOpen->setObjectName(QString::fromUtf8("botOpen"));
    botOpen->setGeometry(QRect(10, 10, 61, 23));
    botCancel = new QPushButton(frmSFSAFT);
    botCancel->setObjectName(QString::fromUtf8("botCancel"));
    botCancel->setGeometry(QRect(250, 300, 75, 23));
    botClose = new QPushButton(frmSFSAFT);
    botClose->setObjectName(QString::fromUtf8("botClose"));
    botClose->setGeometry(QRect(340, 300, 75, 23));
    lblFilePath = new QLabel(frmSFSAFT);
    lblFilePath->setObjectName(QString::fromUtf8("lblFilePath"));
    lblFilePath->setGeometry(QRect(80, 10, 341, 20));
    groupBox_2 = new QGroupBox(frmSFSAFT);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(10, 220, 411, 71));
    label_9 = new QLabel(groupBox_2);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(20, 50, 88, 16));
    progBar = new QProgressBar(groupBox_2);
    progBar->setObjectName(QString::fromUtf8("progBar"));
    progBar->setGeometry(QRect(20, 20, 291, 23));
    progBar->setValue(24);
    progBar->setOrientation(Qt::Horizontal);
    botProc = new QPushButton(groupBox_2);
    botProc->setObjectName(QString::fromUtf8("botProc"));
    botProc->setGeometry(QRect(320, 20, 75, 23));
    lblStatus = new QLabel(groupBox_2);
    lblStatus->setObjectName(QString::fromUtf8("lblStatus"));
    lblStatus->setGeometry(QRect(110, 50, 151, 16));
    groupBox = new QGroupBox(frmSFSAFT);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 40, 411, 171));
    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(10, 50, 116, 16));
    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(40, 80, 96, 16));
    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(40, 110, 81, 16));
    label_7 = new QLabel(groupBox);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(30, 140, 97, 16));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(60, 20, 64, 16));
    txtTimeOff = new QLineEdit(groupBox);
    txtTimeOff->setObjectName(QString::fromUtf8("txtTimeOff"));
    txtTimeOff->setGeometry(QRect(130, 80, 51, 20));
    txtTimeOff->setMaxLength(6);
    cmbSample = new QComboBox(groupBox);
    cmbSample->setObjectName(QString::fromUtf8("cmbSample"));
    cmbSample->setGeometry(QRect(130, 110, 221, 22));
    label_8 = new QLabel(groupBox);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(260, 140, 97, 16));
    cNChan = new QwtCounter(groupBox);
    cNChan->setObjectName(QString::fromUtf8("cNChan"));
    cNChan->setGeometry(QRect(130, 20, 111, 20));
    cNChan->setNumButtons(2);
    cNChan->setProperty("basicstep", QVariant(1));
    cNChan->setMinValue(1);
    cNChan->setMaxValue(192);
    cNChan->setValue(1);
    txtElSpace = new QLineEdit(groupBox);
    txtElSpace->setObjectName(QString::fromUtf8("txtElSpace"));
    txtElSpace->setGeometry(QRect(130, 140, 51, 20));
    txtElSpace->setMaxLength(6);
    txtAD = new QLineEdit(groupBox);
    txtAD->setObjectName(QString::fromUtf8("txtAD"));
    txtAD->setGeometry(QRect(130, 50, 51, 20));
    txtAD->setMaxLength(6);
    txtSoundV = new QLineEdit(groupBox);
    txtSoundV->setObjectName(QString::fromUtf8("txtSoundV"));
    txtSoundV->setGeometry(QRect(370, 140, 31, 20));
    txtSoundV->setMaxLength(4);
    retranslateUi(frmSFSAFT);

    QMetaObject::connectSlotsByName(frmSFSAFT);
    } // setupUi

    void retranslateUi(QWidget *frmSFSAFT)
    {
    frmSFSAFT->setWindowTitle(QApplication::translate("frmSFSAFT", "Synthetic Focusing Technique", 0, QApplication::UnicodeUTF8));
    botOpen->setText(QApplication::translate("frmSFSAFT", "Open File", 0, QApplication::UnicodeUTF8));
    botCancel->setText(QApplication::translate("frmSFSAFT", "Cancel", 0, QApplication::UnicodeUTF8));
    botClose->setText(QApplication::translate("frmSFSAFT", "Close", 0, QApplication::UnicodeUTF8));
    lblFilePath->setText(QApplication::translate("frmSFSAFT", "File []", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("frmSFSAFT", "", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("frmSFSAFT", "Processing Angle: ", 0, QApplication::UnicodeUTF8));
    botProc->setText(QApplication::translate("frmSFSAFT", "Process", 0, QApplication::UnicodeUTF8));
    lblStatus->setText(QApplication::translate("frmSFSAFT", "0/0", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("frmSFSAFT", "File Parameters", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("frmSFSAFT", "A/D sampling rate (MHz)", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("frmSFSAFT", "Time Offset (usec)", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("frmSFSAFT", "No. Samples/Rec", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("frmSFSAFT", "Element Space (mm)", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("frmSFSAFT", "No. Channels", 0, QApplication::UnicodeUTF8));
    txtTimeOff->setInputMask(QApplication::translate("frmSFSAFT", "###.99;0", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("frmSFSAFT", "Sound Velocity (m/s)", 0, QApplication::UnicodeUTF8));
    txtElSpace->setInputMask(QApplication::translate("frmSFSAFT", "###.99;0", 0, QApplication::UnicodeUTF8));
    txtAD->setInputMask(QApplication::translate("frmSFSAFT", "###.99;0", 0, QApplication::UnicodeUTF8));
    txtSoundV->setInputMask(QApplication::translate("frmSFSAFT", "9999; ", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(frmSFSAFT);
    } // retranslateUi

};

namespace Ui {
    class frmSFSAFT: public Ui_frmSFSAFT {};
} // namespace Ui

#endif // UI_FRMSFSAFT_H
