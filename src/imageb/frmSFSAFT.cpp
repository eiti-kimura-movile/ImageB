#include <QtGui>

#include "frmSFSAFT.h"
#include "ui_frmSFSAFT.h"
#include "setupsinal.h"

//! default constructor
frmSFSAFT::frmSFSAFT(QWidget *parent)
{
    setupUi(this);
    connect(botCancel, SIGNAL(clicked()), this, SLOT(botCancelClicked()));
    connect(botClose, SIGNAL(clicked()), this, SLOT(botCloseClicked()));
    connect(botOpen, SIGNAL(clicked()), this, SLOT(botOpenClicked()));
    connect(botProc, SIGNAL(clicked()), this, SLOT(botProcClicked()));

    connect(&sfsaft, SIGNAL(changeValue(int,int)), this, SLOT(updateBar(int,int)));
    connect(&sfsaft, SIGNAL(finished()), this, SLOT(procFinished()));

    progBar->setValue(0);
    
    //configura Combo
    cmbSample->addItem("2048 samples/2 bytes", QVariant(2048) );
    cmbSample->addItem("4096 samples/2 bytes", QVariant(4096) );

}

//! opens a RF dataset
void frmSFSAFT::botOpenClicked()
{
   
   filename = "";
   filename = QFileDialog::getOpenFileName(this, tr("Open Data File"), QDir::currentPath());
        
   if (filename.isEmpty())
       return;
   else
       lblFilePath->setText("File [ " + filename + " ]");

}

void frmSFSAFT::updateBar(int value, int curr)
{
   progBar->setValue(value);
   lblStatus->setText( QString::number(curr) + " of " + QString::number(aq.theta*2+1) );
}

//starts processing with a constant theta value equals to 40
void frmSFSAFT::botProcClicked()
{
   aq.theta = 40;
   aq.elements = (int) cNChan->value();
   aq.d = (txtElSpace->text()).toDouble();
   aq.c = (txtSoundV->text()).toInt();
   aq.deltat = (txtAD->text()).toDouble();

   int samples = cmbSample->itemData( cmbSample->currentIndex() ).toInt();
   //cout << "Nsamples" << samples << endl;
   sfsaft.setN_SAMPLES(samples);
   sfsaft.setFile(filename);
   sfsaft.start();
}

void frmSFSAFT::procFinished()
{
   lblStatus->setText( "FINISHED! cols:" + QString::number(aq.theta*2+1) );
   emit finishedSFSAFT();
}

void frmSFSAFT::botCancelClicked()
{
  ;
}

//! close the form
void frmSFSAFT::botCloseClicked()
{
  sfsaft.stop_process();
  this->close();
}