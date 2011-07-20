#ifndef SFSAFT_H
#define SFSAFT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imageproc.h"
#include "imageb.h"

#include <QtGui>

using namespace std;

//! This class implements the synthetic aperture methods: SAFT and SF 
class SFSAFT : public QThread
{
	Q_OBJECT
  
  private:
       //! flag to indicate if processing is running
       bool running;
       
       //! number of samples of file
       int N_SAMPLES;
       
       //!the file used for processing
       FILE *file;
  
       //! the file full path
       QString filename;
  
       //! class for process ultrasound images
       ImageB imageb;
  
       //! class for image processing
       ImageProc imgproc;

  public:
        SFSAFT();
        SFSAFT(QString);
	void run();
        void stop_process();
        
        Matrix shortbuffer2matrix( short int[]);
        Matrix getData(Matrix, int); //transforma dado da imagem
        void procSFSAFT(); //faz processamento para criar a imagem
  
        void setN_SAMPLES(int);
        void setFile(QString);  
  
  signals:
        void changeValue( int, int );
        void finished();
};

#endif 
 
