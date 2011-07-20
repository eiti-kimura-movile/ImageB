#ifndef FRMPROCSINAL_H
#define FRMPROCSINAL_H

#include "ui_frmProcSinal.h"
#include "imageproc.h"
#include "imageb.h"
#include "setupsinal.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

//! global variable
extern SetupSinal signal_setup;

using namespace std;

class frmProcSinal : public QWidget, private Ui::frmProcSinal
{
    Q_OBJECT

public:
    frmProcSinal(QWidget *parent = 0);
    void loadData();
    void configPlots();
    void writeMatrixData(Matrix, string);
    void copyData(SetupSinal, SetupSinal&);

private:
    //! class for image processing
    ImageProc imgproc;
  
    //! class for process ultrasound images
    ImageB imageB;
    //! matrix data from RF signal
    Matrix rfdata;
    //! envelope data from RF signal
    Matrix env;
    //! envelope and gain data from RF signal
    Matrix env_gain;
    //! gain vector to be applied to RF signal
    Matrix gain;
    //! threshold vector
    Matrix limiar;
    //! processed matrix data from RF signal
    Matrix proc_data;
    //! buffer matrix data from RF signal
    Matrix mat;
    
    //! Signal configuration setup
    SetupSinal ssetup;

    bool setDefault;

signals:
   void setupChanged();

private slots:
    void chgData();
    void botApplyClicked();
    void botResetClicked();
};

#endif

