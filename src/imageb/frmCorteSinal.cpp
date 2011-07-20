#include <QtGui>

#include "frmCorteSinal.h"
#include "ui_frmCorteSinal.h"
#include "setupsinal.h"

//! max size of vector to be plotted
const int PLOT_SIZE = 1000;

//! class default constructor
frmCorteSinal::frmCorteSinal(QWidget *parent)
{
    setupUi(this);
    loadData();  
    configPlots();

    connect(cCut1, SIGNAL(valueChanged(double)), this, SLOT(chgData()) );
    connect(cCut2, SIGNAL(valueChanged(double)), this, SLOT(chgData()) );

    connect(botCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(botApply, SIGNAL(clicked()), this, SLOT(botApplyClicked()));

}

//! this method creates a local copy of global signal_setup variable
void frmCorteSinal::loadData()
{
    ssetup.copyData(signal_setup, ssetup);
}


//! reads data from form and prepare it to be plotted
void frmCorteSinal::configPlots()
{
    rfdata = ssetup.getRFData();

    //cout << "tam:" << rfdata.nRow() << endl;

    double d_rfdata[PLOT_SIZE];

    double axis_x[PLOT_SIZE];

    for (int j=0; j < PLOT_SIZE; j++ )
     {
       d_rfdata[j] = rfdata(j+1);
       axis_x[j] = j;
     }

   //seta componentes gráficos do formulário
   cCut1->setValue(ssetup.s_cut);
   cCut2->setValue(ssetup.e_cut);
   
   
    //clear plotting chart
    plotSinal->clear();
   
    // Insert new curves
    QwtPlotCurve *cRF = new QwtPlotCurve("RF");   
    cRF->attach(plotSinal); //attach curve to chart
     
   
    // Set curve styles
    cRF->setPen(QPen(Qt::blue));
   
    // Attach (don't copy) data
    cRF->setData(axis_x, d_rfdata, PLOT_SIZE);
    
    //paint chart
    plotSinal->replot();
}

//! simple writes Matrix data to a file
/*!
  \param mat the Matrix data
  \param filename the fullpath of file 
*/
void frmCorteSinal::writeMatrixData(Matrix mat,  string filename )
{
   ofstream dataf(filename.c_str()); 

  if (mat.nCol() == 1)
  {
    for(int i=1; i<=mat.nRow(); i++ )
      dataf << mat(i) << endl;
  }
  else
  {
       for (int lin=1; lin <= mat.nRow(); lin++)
         {
            //grava linhas e colunas
           for(int col=1; col <= mat.nCol(); col++)
               dataf << mat(lin,col) << '\t';
           
           dataf << endl;
         }
  }
}


//! sets new parameters (read from form) to cut off signal data 
void frmCorteSinal::chgData()
{
    ssetup.applyTimeCut((int)(cCut1->value()),(int)(cCut2->value()));
    configPlots();
}

//! perform time cut operation based in the configuration form
void frmCorteSinal::botApplyClicked()
{
   signal_setup = ssetup;
   emit setupChanged(); //emite sinal para atualização da imagem
   this->close();
}
