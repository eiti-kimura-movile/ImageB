#include <QtGui>
#include <QStringList>

#include "frmProcSinal.h"
#include "ui_frmProcSinal.h"

#include "setupsinal.h"

//! the max size of vector to be plotted
const int PLOT_SIZE = 1000;


//! default constructor, initializes class variables
frmProcSinal::frmProcSinal(QWidget *parent)
{
    setupUi(this);
    loadData();  
    configPlots();

    connect(cRamp1, SIGNAL(valueChanged(double)), this, SLOT(chgData()) );
    connect(cRamp2, SIGNAL(valueChanged(double)), this, SLOT(chgData()) );

    connect(cAtWinStart, SIGNAL(valueChanged(double)), this, SLOT(chgData()) );
    connect(cAtWinEnd, SIGNAL(valueChanged(double)), this, SLOT(chgData()) );

    connect(cLimiar, SIGNAL(valueChanged(double)), this, SLOT(chgData()) );

    connect(botCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(botApply, SIGNAL(clicked()), this, SLOT(botApplyClicked()));
    
    connect(botReset, SIGNAL(clicked()), this, SLOT(botResetClicked()));
}


//! makes a local copy of signal_setup variable
void frmProcSinal::loadData()
{
    
   // cout << "rf:" << signal_setup.getRFData().nRow() << "gain:" << signal_setup.getGainVector().nRow() << 
   //     " lim: " << signal_setup.getThresholdVec().nRow()  << " env:" << signal_setup.getEnv().nRow() << 
   //     "env_gain: " << signal_setup.getEnvGain().nRow()  << endl;

    ssetup.copyData(signal_setup, ssetup);
 
}


//! reads and prepare all data for plotting
void frmProcSinal::configPlots()
{

   rfdata = ssetup.getRFData();
   limiar = ssetup.getThresholdVec();
   env = ssetup.getEnv();
   gain = ssetup.getGainVector();
   env_gain = ssetup.getEnvGain();

   double d_rfdata[PLOT_SIZE];
   double d_env[PLOT_SIZE];
   double d_limiar[PLOT_SIZE];
   double d_gain[PLOT_SIZE];
   double d_env_gain[PLOT_SIZE];
   
   double axis_x[PLOT_SIZE];

   for (int j=0; j < PLOT_SIZE; j++ )
     {
       d_limiar[j] = limiar(j+1);
       d_rfdata[j] = rfdata(j+1);
       d_env[j] = env(j+1);
       axis_x[j] = j;
       d_gain[j] = gain(j+1);
       d_env_gain[j] = env_gain(j+1);
     }

    //seta componentes gráficos do formulário
    cRamp1->setValue(ssetup.s_ramp);
    cRamp2->setValue(ssetup.e_ramp);
    cAtWinStart->setValue(ssetup.s_att);
    cAtWinEnd->setValue(ssetup.e_att);
    lineAtVal->setText(QString::number(ssetup.att_factor));
    cLimiar->setValue(ssetup.v_limiar);
   
   //clear plots
    plotRF->clear();
    plotEnv->clear();
    plotGain->clear();
    plotSinal->clear();
   
    // Insert new curves
    QwtPlotCurve *cRF = new QwtPlotCurve("RF");
    QwtPlotCurve *cEnv = new QwtPlotCurve("Envelope");
    QwtPlotCurve *cLim = new QwtPlotCurve("Limiar"); 
    QwtPlotCurve *cGain = new QwtPlotCurve("Curva Ganho"); 
    QwtPlotCurve *cEnvGain = new QwtPlotCurve("Ganho no Evelope");
   
   
    cEnv->attach(plotEnv);
    cLim->attach(plotEnv);
   
    cEnvGain->attach(plotSinal);
    cGain->attach(plotGain);
    cRF->attach(plotRF);
     
   
    // Set curve styles
    cRF->setPen(QPen(Qt::red));
    cEnv->setPen(QPen(Qt::blue));
    cLim->setPen(QPen(Qt::red));
    cGain->setPen(QPen(Qt::blue));
    cEnvGain->setPen(QPen(Qt::black));
   
    // Attach (don't copy) data. Both curves use the same x array.
    cRF->setData(axis_x, d_rfdata, PLOT_SIZE);
    cEnv->setData(axis_x, d_env, PLOT_SIZE);
    cLim->setData(axis_x,d_limiar,PLOT_SIZE);
    cGain->setData(axis_x,d_gain,PLOT_SIZE);
    cEnvGain->setData(axis_x,d_env_gain,PLOT_SIZE);
    
    plotRF->replot();
    plotEnv->replot();
    plotGain->replot();
    plotSinal->replot();
    
    
    proc_data = imageB.procData2BMode(env_gain);
  
    //objeto retorna uma imagem redimensionada 
    QImage image = imageB.getBModeImage(211,121, proc_data, 0);
    imageLabel->setPixmap(QPixmap::fromImage(image));//seta a imagem a ser exibida
    imageLabel->adjustSize();
    
    setDefault = false; //flag controla a atualização dos controles gráficos, utilizada para resetar os componentes
}


void frmProcSinal::writeMatrixData(Matrix mat,  string filename )
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


//! reads new configurations from from and refreshes main variables for processing
void frmProcSinal::chgData()
{
   
    //ssetup.setRFData(rfdata); 
    //imgproc.writeMatrixData(mat,"mat.txt");
    //imgproc.writeMatrixData(rfdata,"rfdata.txt");

    if (! setDefault)
      {
        ssetup.setGain(rfdata.nRow(),(int)(cRamp1->value()),(int)(cRamp2->value()));
        imgproc.writeMatrixData(ssetup.getGainVector(),"gain.txt");
        ssetup.setAttenuation((int)(cAtWinStart->value()),(int)(cAtWinEnd->value()),lineAtVal->text().toDouble());
        ssetup.setThreshold((double)(cLimiar->value()));
        ssetup.applyGainLimiar();
        configPlots();
     }

}

//! perform Signal Processing 
void frmProcSinal::botApplyClicked()
{
   signal_setup = ssetup;
   emit setupChanged(); //emite sinal para atualização da imagem
   this->close();
}

//! resets configuration
void frmProcSinal::botResetClicked()
{
  setDefault = true;
  ssetup.setDefaults();
  configPlots();
}