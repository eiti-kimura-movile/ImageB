#ifndef UI_FRMBRIGHTCONTRAST_H
#define UI_FRMBRIGHTCONTRAST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

class Ui_frmBrightContrast
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QSlider *slideB;
    QSlider *slideC;
    QLineEdit *txtB;
    QLineEdit *txtC;
    QPushButton *botApply;
    QPushButton *botCancel;
    QCheckBox *chkPreview;

    void setupUi(QWidget *frmBrightContrast)
    {
    frmBrightContrast->setObjectName(QString::fromUtf8("frmBrightContrast"));
    frmBrightContrast->resize(QSize(413, 128).expandedTo(frmBrightContrast->minimumSizeHint()));
    groupBox = new QGroupBox(frmBrightContrast);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 10, 281, 111));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(10, 10, 46, 14));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(10, 60, 54, 16));
    slideB = new QSlider(groupBox);
    slideB->setObjectName(QString::fromUtf8("slideB"));
    slideB->setGeometry(QRect(10, 80, 221, 21));
    slideB->setMaximum(200);
    slideB->setSliderPosition(100);
    slideB->setOrientation(Qt::Horizontal);
    slideB->setInvertedAppearance(false);
    slideB->setTickPosition(QSlider::NoTicks);
    slideC = new QSlider(groupBox);
    slideC->setObjectName(QString::fromUtf8("slideC"));
    slideC->setGeometry(QRect(10, 30, 221, 21));
    slideC->setMaximum(200);
    slideC->setSliderPosition(100);
    slideC->setOrientation(Qt::Horizontal);
    slideC->setInvertedAppearance(false);
    slideC->setTickPosition(QSlider::NoTicks);
    txtB = new QLineEdit(groupBox);
    txtB->setObjectName(QString::fromUtf8("txtB"));
    txtB->setGeometry(QRect(240, 80, 31, 20));
    txtC = new QLineEdit(groupBox);
    txtC->setObjectName(QString::fromUtf8("txtC"));
    txtC->setGeometry(QRect(240, 30, 31, 20));
    botApply = new QPushButton(frmBrightContrast);
    botApply->setObjectName(QString::fromUtf8("botApply"));
    botApply->setGeometry(QRect(300, 10, 101, 23));
    botCancel = new QPushButton(frmBrightContrast);
    botCancel->setObjectName(QString::fromUtf8("botCancel"));
    botCancel->setGeometry(QRect(300, 40, 101, 23));
    chkPreview = new QCheckBox(frmBrightContrast);
    chkPreview->setObjectName(QString::fromUtf8("chkPreview"));
    chkPreview->setGeometry(QRect(300, 70, 71, 18));
    chkPreview->setChecked(true);
    retranslateUi(frmBrightContrast);

    QMetaObject::connectSlotsByName(frmBrightContrast);
    } // setupUi

    void retranslateUi(QWidget *frmBrightContrast)
    {
    frmBrightContrast->setWindowTitle(QApplication::translate("frmBrightContrast", "Adjust Brightness/Contrast", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("frmBrightContrast", "", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("frmBrightContrast", "Contrast:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("frmBrightContrast", "Brightness:", 0, QApplication::UnicodeUTF8));
    txtB->setText(QApplication::translate("frmBrightContrast", "0", 0, QApplication::UnicodeUTF8));
    txtC->setText(QApplication::translate("frmBrightContrast", "0", 0, QApplication::UnicodeUTF8));
    botApply->setText(QApplication::translate("frmBrightContrast", "Apply", 0, QApplication::UnicodeUTF8));
    botCancel->setText(QApplication::translate("frmBrightContrast", "Cancel", 0, QApplication::UnicodeUTF8));
    chkPreview->setText(QApplication::translate("frmBrightContrast", "Preview", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(frmBrightContrast);
    } // retranslateUi

};

namespace Ui {
    class frmBrightContrast: public Ui_frmBrightContrast {};
} // namespace Ui

#endif // UI_FRMBRIGHTCONTRAST_H
