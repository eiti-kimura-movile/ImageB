#ifndef IMAGEB_H
#define IMAGEB_H

#include <QImage>
#include <QStringList>
#include "acquisition.h"
#include "imageproc.h"
#include "setupsinal.h"

//!A class for ultrasound image processing
/*!
  this class has operations to:
    -process RF data to be displayed in B-Mode
    -convert a data Matrix into a displayable image
    -convert an image to grayscale
    -create polar image
    
    \author J.P.E. Kimura 
*/
class ImageB
{
     public:
       ImageB();
       Matrix getProcDataMatrix(QStringList, int, int);//retorna em escala de cinza
       Matrix getDataMatrix(QStringList, int, int);//retorna normalizado
       QImage getBModeImage(Matrix);
       QImage getBModeImage(int, int, Matrix, int);
       QImage getBModeFocusImage(int, int, int, Acquisition, int);
       Matrix getBModeFocusMatrix(int, int, Acquisition, int);
       Matrix getVecBFocus(Acquisition, Matrix, float);
       Matrix loadMatrixData(string);
       Matrix loadData(string, int&, int&);
       vector<double> procDataLine( string, char );
       Matrix procData2BMode(Matrix);
       
       Matrix createPolarImage( Matrix );
       Matrix createConeImage( Matrix, Matrix, Matrix );
       
       Matrix loadBinaryData(string);
       
       void setSignalSetup(SetupSinal);
       void removeSignalSetup();
     
       //pixel value
       QRgb getPixelRGB(int);
       
       Matrix convertRGB2Gray( QImage );//converts a color image to grayscale
       
       //! used like a flag variable that indicates if image has or has not some colormap
       int has_colormap;
       //! stores a complete matrix of color values
       Matrix colormap; 
       //! remains the current color map
       int curr_colormap;
     
     private:
       //! class for image processing
       ImageProc imgproc;
       //! indicates if the signal setup process will be executed
       bool applyProc; //variável para indicar se o setup do sinal será executado
       //! Signal configuration setup
       SetupSinal ssetup;
};

#endif
