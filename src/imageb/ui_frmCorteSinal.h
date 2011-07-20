#ifndef UI_FRMCORTESINAL_H
#define UI_FRMCORTESINAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "qwt_counter.h"
#include "qwt_plot.h"

class Ui_frmCorteSinal
{
public:
    QGroupBox *grpCorte;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLabel *label;
    QwtCounter *cCut1;
    QwtCounter *cCut2;
    QPushButton *botCancel;
    QPushButton *botApply;
    QwtPlot *plotSinal;

    void setupUi(QWidget *frmCorteSinal)
    {
    frmCorteSinal->setObjectName(QString::fromUtf8("frmCorteSinal"));
    frmCorteSinal->resize(QSize(477, 229).expandedTo(frmCorteSinal->minimumSizeHint()));
    grpCorte = new QGroupBox(frmCorteSinal);
    grpCorte->setObjectName(QString::fromUtf8("grpCorte"));
    grpCorte->setGeometry(QRect(10, 10, 461, 211));
    groupBox = new QGroupBox(grpCorte);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(290, 20, 161, 81));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 50, 31, 16));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(10, 20, 31, 16));
    cCut1 = new QwtCounter(groupBox);
    cCut1->setObjectName(QString::fromUtf8("cCut1"));
    cCut1->setGeometry(QRect(50, 20, 101, 20));
    cCut1->setNumButtons(2);
    cCut1->setProperty("basicstep", QVariant(1));
    cCut1->setMaxValue(900);
    cCut1->setValue(0);
    cCut2 = new QwtCounter(groupBox);
    cCut2->setObjectName(QString::fromUtf8("cCut2"));
    cCut2->setGeometry(QRect(50, 50, 101, 20));
    cCut2->setNumButtons(2);
    cCut2->setProperty("basicstep", QVariant(1));
    cCut2->setMaxValue(900);
    cCut2->setValue(0);
    botCancel = new QPushButton(grpCorte);
    botCancel->setObjectName(QString::fromUtf8("botCancel"));
    botCancel->setGeometry(QRect(370, 180, 75, 23));
    botApply = new QPushButton(grpCorte);
    botApply->setObjectName(QString::fromUtf8("botApply"));
    botApply->setGeometry(QRect(370, 150, 75, 23));
    plotSinal = new QwtPlot(grpCorte);
    plotSinal->setObjectName(QString::fromUtf8("plotSinal"));
    plotSinal->setGeometry(QRect(10, 20, 261, 181));
    plotSinal->setFrameShape(QFrame::NoFrame);
    plotSinal->setFrameShadow(QFrame::Plain);
    plotSinal->setCanvasBackground(QColor(255, 255, 255));
    plotSinal->setCanvasLineWidth(1);
    plotSinal->setProperty("xTopAxis", QVariant(false));
    retranslateUi(frmCorteSinal);

    QMetaObject::connectSlotsByName(frmCorteSinal);
    } // setupUi

    void retranslateUi(QWidget *frmCorteSinal)
    {
    frmCorteSinal->setWindowTitle(QApplication::translate("frmCorteSinal", "Signal Cut Configuration ", 0, QApplication::UnicodeUTF8));
    grpCorte->setTitle(QApplication::translate("frmCorteSinal", "Signal Time Cut", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("frmCorteSinal", "Number of Points", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("frmCorteSinal", "End:", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("frmCorteSinal", "Start:", 0, QApplication::UnicodeUTF8));
    botCancel->setText(QApplication::translate("frmCorteSinal", "Cancel", 0, QApplication::UnicodeUTF8));
    botApply->setText(QApplication::translate("frmCorteSinal", "Apply", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(frmCorteSinal);
    } // retranslateUi

};

namespace Ui {
    class frmCorteSinal: public Ui_frmCorteSinal {};
} // namespace Ui

#endif // UI_FRMCORTESINAL_H
