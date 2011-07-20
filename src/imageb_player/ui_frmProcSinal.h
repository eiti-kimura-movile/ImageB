#ifndef UI_FRMPROCSINAL_H
#define UI_FRMPROCSINAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "qwt_counter.h"
#include "qwt_plot.h"

class Ui_frmProcSinal
{
public:
    QGroupBox *grpSinal;
    QwtPlot *plotRF;
    QwtPlot *plotEnv;
    QGroupBox *grpLimiar;
    QwtPlot *plotSinal;
    QLabel *imageLabel;
    QGroupBox *grpGanho;
    QGroupBox *groupBox_2;
    QLabel *label_3;
    QLineEdit *lineAtVal;
    QwtCounter *cAtWinStart;
    QwtCounter *cAtWinEnd;
    QGroupBox *groupBox;
    QwtCounter *cRamp1;
    QwtCounter *cRamp2;
    QGroupBox *groupBox_3;
    QwtCounter *cLimiar;
    QwtPlot *plotGain;
    QPushButton *botApply;
    QPushButton *botCancel;
    QPushButton *botReset;

    void setupUi(QWidget *frmProcSinal)
    {
    frmProcSinal->setObjectName(QString::fromUtf8("frmProcSinal"));
    frmProcSinal->resize(QSize(499, 568).expandedTo(frmProcSinal->minimumSizeHint()));
    grpSinal = new QGroupBox(frmProcSinal);
    grpSinal->setObjectName(QString::fromUtf8("grpSinal"));
    grpSinal->setGeometry(QRect(10, 0, 481, 171));
    plotRF = new QwtPlot(grpSinal);
    plotRF->setObjectName(QString::fromUtf8("plotRF"));
    plotRF->setGeometry(QRect(10, 20, 221, 141));
    plotRF->setFrameShape(QFrame::NoFrame);
    plotRF->setFrameShadow(QFrame::Plain);
    plotRF->setCanvasBackground(QColor(255, 255, 255));
    plotRF->setCanvasLineWidth(1);
    plotRF->setProperty("xTopAxis", QVariant(false));
    plotEnv = new QwtPlot(grpSinal);
    plotEnv->setObjectName(QString::fromUtf8("plotEnv"));
    plotEnv->setGeometry(QRect(250, 20, 221, 141));
    plotEnv->setFrameShape(QFrame::NoFrame);
    plotEnv->setFrameShadow(QFrame::Plain);
    plotEnv->setCanvasBackground(QColor(255, 255, 255));
    plotEnv->setCanvasLineWidth(1);
    plotEnv->setProperty("xTopAxis", QVariant(false));
    grpLimiar = new QGroupBox(frmProcSinal);
    grpLimiar->setObjectName(QString::fromUtf8("grpLimiar"));
    grpLimiar->setGeometry(QRect(10, 360, 481, 171));
    plotSinal = new QwtPlot(grpLimiar);
    plotSinal->setObjectName(QString::fromUtf8("plotSinal"));
    plotSinal->setGeometry(QRect(20, 30, 211, 131));
    QFont font;
    font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
    font.setPointSize(7);
    font.setBold(false);
    font.setItalic(false);
    font.setUnderline(false);
    font.setWeight(50);
    font.setStrikeOut(false);
    plotSinal->setFont(font);
    plotSinal->setFrameShape(QFrame::NoFrame);
    plotSinal->setFrameShadow(QFrame::Plain);
    plotSinal->setCanvasBackground(QColor(255, 255, 255));
    plotSinal->setCanvasLineWidth(1);
    plotSinal->setProperty("xTopAxis", QVariant(false));
    imageLabel = new QLabel(grpLimiar);
    imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
    imageLabel->setGeometry(QRect(260, 30, 211, 121));
    imageLabel->setFrameShape(QFrame::Box);
    grpGanho = new QGroupBox(frmProcSinal);
    grpGanho->setObjectName(QString::fromUtf8("grpGanho"));
    grpGanho->setGeometry(QRect(10, 180, 481, 171));
    groupBox_2 = new QGroupBox(grpGanho);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(350, 20, 121, 131));
    label_3 = new QLabel(groupBox_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(10, 100, 106, 16));
    lineAtVal = new QLineEdit(groupBox_2);
    lineAtVal->setObjectName(QString::fromUtf8("lineAtVal"));
    lineAtVal->setGeometry(QRect(50, 100, 31, 20));
    cAtWinStart = new QwtCounter(groupBox_2);
    cAtWinStart->setObjectName(QString::fromUtf8("cAtWinStart"));
    cAtWinStart->setGeometry(QRect(10, 30, 101, 20));
    cAtWinStart->setNumButtons(1);
    cAtWinStart->setProperty("basicstep", QVariant(10));
    cAtWinStart->setMaxValue(1000);
    cAtWinStart->setValue(0);
    cAtWinEnd = new QwtCounter(groupBox_2);
    cAtWinEnd->setObjectName(QString::fromUtf8("cAtWinEnd"));
    cAtWinEnd->setGeometry(QRect(10, 60, 101, 20));
    cAtWinEnd->setNumButtons(1);
    cAtWinEnd->setProperty("basicstep", QVariant(10));
    cAtWinEnd->setMaxValue(1000);
    cAtWinEnd->setValue(0);
    groupBox = new QGroupBox(grpGanho);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(220, 20, 121, 81));
    cRamp1 = new QwtCounter(groupBox);
    cRamp1->setObjectName(QString::fromUtf8("cRamp1"));
    cRamp1->setGeometry(QRect(10, 20, 101, 20));
    cRamp1->setNumButtons(2);
    cRamp1->setProperty("basicstep", QVariant(1));
    cRamp1->setMaxValue(100);
    cRamp1->setValue(0);
    cRamp2 = new QwtCounter(groupBox);
    cRamp2->setObjectName(QString::fromUtf8("cRamp2"));
    cRamp2->setGeometry(QRect(10, 50, 101, 20));
    cRamp2->setNumButtons(2);
    cRamp2->setProperty("basicstep", QVariant(1));
    cRamp2->setMaxValue(100);
    cRamp2->setValue(100);
    groupBox_3 = new QGroupBox(grpGanho);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setGeometry(QRect(220, 100, 121, 51));
    cLimiar = new QwtCounter(groupBox_3);
    cLimiar->setObjectName(QString::fromUtf8("cLimiar"));
    cLimiar->setGeometry(QRect(10, 20, 101, 20));
    cLimiar->setNumButtons(1);
    cLimiar->setProperty("basicstep", QVariant(0.001));
    cLimiar->setMaxValue(100);
    cLimiar->setValue(0);
    plotGain = new QwtPlot(grpGanho);
    plotGain->setObjectName(QString::fromUtf8("plotGain"));
    plotGain->setGeometry(QRect(10, 30, 191, 131));
    plotGain->setFrameShape(QFrame::NoFrame);
    plotGain->setFrameShadow(QFrame::Plain);
    plotGain->setCanvasBackground(QColor(255, 255, 255));
    plotGain->setCanvasLineWidth(1);
    plotGain->setProperty("xTopAxis", QVariant(false));
    botApply = new QPushButton(frmProcSinal);
    botApply->setObjectName(QString::fromUtf8("botApply"));
    botApply->setGeometry(QRect(410, 540, 75, 23));
    botCancel = new QPushButton(frmProcSinal);
    botCancel->setObjectName(QString::fromUtf8("botCancel"));
    botCancel->setGeometry(QRect(330, 540, 75, 23));
    botReset = new QPushButton(frmProcSinal);
    botReset->setObjectName(QString::fromUtf8("botReset"));
    botReset->setGeometry(QRect(220, 540, 101, 23));
    retranslateUi(frmProcSinal);

    QMetaObject::connectSlotsByName(frmProcSinal);
    } // setupUi

    void retranslateUi(QWidget *frmProcSinal)
    {
    frmProcSinal->setWindowTitle(QApplication::translate("frmProcSinal", "Signal Processing Setup", 0, QApplication::UnicodeUTF8));
    grpSinal->setTitle(QApplication::translate("frmProcSinal", "Signal Plotting", 0, QApplication::UnicodeUTF8));
    grpLimiar->setTitle(QApplication::translate("frmProcSinal", "Processed Data and Preview", 0, QApplication::UnicodeUTF8));
    imageLabel->setText(QApplication::translate("frmProcSinal", "", 0, QApplication::UnicodeUTF8));
    grpGanho->setTitle(QApplication::translate("frmProcSinal", "Gain and Threshold", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("frmProcSinal", "Atenuation Window", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("frmProcSinal", "Value:                %", 0, QApplication::UnicodeUTF8));
    lineAtVal->setText(QApplication::translate("frmProcSinal", "-30", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("frmProcSinal", "Gain Ramp (%)", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("frmProcSinal", "Threshold", 0, QApplication::UnicodeUTF8));
    botApply->setText(QApplication::translate("frmProcSinal", "Apply", 0, QApplication::UnicodeUTF8));
    botCancel->setText(QApplication::translate("frmProcSinal", "Cancel", 0, QApplication::UnicodeUTF8));
    botReset->setText(QApplication::translate("frmProcSinal", "Reset to Default", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(frmProcSinal);
    } // retranslateUi

};

namespace Ui {
    class frmProcSinal: public Ui_frmProcSinal {};
} // namespace Ui

#endif // UI_FRMPROCSINAL_H
