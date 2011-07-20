#ifndef FRMSFSAFT_H
#define FRMSFSAFT_H

#include "ui_frmSFSAFT.h"

#include "imageproc.h"
#include "imageb.h"
#include "SFSAFT.h"

#include "setupsinal.h"

//! global variable for signal setup options
extern SetupSinal signal_setup;

//! global variable to signal acquisition information
extern Acquisition aq;

using namespace std;

//! A class to control form frmSAFSAFT
/*!
  this class provides an interface to process RF data from Michigan University dataset.
*/
class frmSFSAFT : public QWidget, private Ui::frmSFSAFT
{
    Q_OBJECT

public:
    frmSFSAFT(QWidget *parent = 0);
 
private:
    //! class for image processing
    ImageProc imgproc;
    //! matrix data from RF signal
    Matrix rfdata;
    //! Signal configuration setup
    SetupSinal ssetup;
    //!Object to execute SF and SAFT operation over the RF dataset
    SFSAFT sfsaft;
    //! represents a File name
    QString filename;

private slots:
    void botCloseClicked();
    void botCancelClicked();
    void botOpenClicked();
    void botProcClicked();
 
    void updateBar(int,int);
    void procFinished();

signals:
   void finishedSFSAFT();
};

#endif

