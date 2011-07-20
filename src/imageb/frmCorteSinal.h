#ifndef FRMCORTESINAL_H
#define FRMCORTESINAL_H

#include "ui_frmCorteSinal.h"
#include "imageproc.h"
#include "imageb.h"
#include "setupsinal.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

//! global variable
extern SetupSinal signal_setup;

using namespace std;

//! A signal time cut class 
/*!
  this class provides methods that togheter SignalSetup object could perform time cut operation over
  the RF signal. Here the data can be cutted of from the begining or ending of signal. 
*/
class frmCorteSinal : public QWidget, private Ui::frmCorteSinal
{
    Q_OBJECT

public:
    frmCorteSinal(QWidget *parent = 0);
    void loadData();
    void configPlots();
    void writeMatrixData(Matrix, string);

private:
    //! class for image processing
    ImageProc imgproc;
    //! the RF data points of signal
    Matrix rfdata;
    //! object that stores signal processing information
    SetupSinal ssetup;

signals:
   void setupChanged();

private slots:
    void chgData();
    void botApplyClicked();
};

#endif

