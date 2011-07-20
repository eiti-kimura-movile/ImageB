#ifndef ACQUISITION_H
#define ACQUISITION_H

#include "NumMeth.h"

//! A class to represent acquired Data
/*!
   This class stores information about acquired data.
*/

class Acquisition
{
 public:
   
   //dados sobre transdutor e parametros da aquisicao de dados
  
   //! steer angle (degrees)
   float theta;                 //angulacao de deflexao em graus
   
   //! number of transducer elements 
   int elements;                //numero de elementos do vetor 
   
   //! distance between elements (mm)
   double d;                    //distancia entre os elementos 
   
   //! sound velocity (m/s)
   int c;                       //velocidade de propagaçao do ultra-som no meio em m/s
   
   //! oscilloscope delay (us)
   double delay;                //delay do osciloscopio em us
   
   //! sampling rate (MHz)
   double deltat;               //taxa  de amostragem
 
   //! delay size in number of points
   double delay_size;           //valor do delay em pontos
   
   //! Matrix that contains RF signal data points
   Matrix rf_matrix;
 
    Acquisition()
    {
      theta = 30;             //angulacao de deflexao em graus
      elements = 12;          //numero de elementos do vetor 
      d = 1.32e-3;            //distancia entre os elementos 1,32 milimetros 
      c = 1480;               //velocidade de propagaçao do ultra-som no meio em m/s
      delay = 66.6765e-6;     //delay do osciloscopio em us
      deltat = 1/20e6;        //taxa  de amostragem 20MHz
      delay_size = delay/deltat; //calculo do valor do delay em numero de pontos
    }

};

#endif
