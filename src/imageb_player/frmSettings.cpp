#include <QtGui>

#include "frmSettings.h"
#include "ui_frmSettings.h"

//! default constructor
frmSettings::frmSettings(QWidget *parent)
{
    setupUi(this);
    connect(botCancel, SIGNAL(clicked()), this, SLOT(botCancelClicked()));
    connect(botClose, SIGNAL(clicked()), this, SLOT(botCloseClicked()));

    txtSteer->setText(QString::number(aq.theta)); 
    cNChan->setValue(aq.elements);
    txtElSpace->setText(QString::number(aq.d*1e3));
    txtSoundV->setText(QString::number(aq.c));
    txtAD->setText(QString::number(aq.samp_rate*1e-6));
    txtTimeOff->setText(QString::number(aq.delay*1e6));
    spinAVG->setValue(aq.avg);
}


void frmSettings::botCancelClicked()
{
    this->close();
}

//! close the form
void frmSettings::botCloseClicked()
{
   aq.theta = (int)(txtSteer->text()).toInt();
   aq.elements = (int) cNChan->value();
   aq.d = (txtElSpace->text()).toDouble() * 1e-3; //mm
   aq.c = (txtSoundV->text()).toInt();
   aq.delay = (txtTimeOff->text()).toDouble() * 1e-6; //us
   aq.avg = (int) spinAVG->value();
   aq.samp_rate = (txtAD->text()).toDouble() * 1e6; //MHz
   aq.deltat = 1/aq.samp_rate;

  this->close();
}