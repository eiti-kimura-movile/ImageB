#ifndef frmSettings_H
#define frmSettings_H

#include "ui_frmSettings.h"

#include "imgproc/imageproc.h"
#include "imgproc/imageb.h"

#include "imgproc/setupsinal.h"

//! global variable for signal setup options
extern SetupSinal signal_setup;

//! global variable to signal acquisition information
extern Acquisition aq;

using namespace std;

//! A class to control form frmSAFSAFT
/*!
  this class provides an interface to process RF data from Michigan University dataset.
*/
class frmSettings : public QWidget, private Ui::frmSettings
{
    Q_OBJECT

public:
    frmSettings(QWidget *parent = 0);
 
private:
    //! class for image processing
    ImageProc imgproc;
    //! matrix data from RF signal
    Matrix rfdata;
    //! Signal configuration setup
    SetupSinal ssetup;

private slots:
    void botCloseClicked();
    void botCancelClicked();

signals:
    ;
};

#endif

