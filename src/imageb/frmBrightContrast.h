#ifndef frmBrightContrast_H
#define frmBrightContrast_H

#include "ui_frmBrightContrast.h"
#include "imageproc.h"
#include "imageb.h"

using namespace std;

//! Class to control brightness and contrast of image
/*!
  this class changes image brightness and contrast. 
  The values can be adjusted from the form using slides.
*/
class frmBrightContrast : public QWidget, private Ui::frmBrightContrast
{
    Q_OBJECT

public:
    //!Default constructor
    frmBrightContrast(QWidget *parent = 0);
    void setImageData(Matrix);
    Matrix getImageData();

private:
    ImageProc imgproc;
    
    //! pixel data to be processed
    Matrix pixel_data;  //pixels a serem processados
    //! original pixel data
    Matrix pixel_image; //pixels originais da imagem

    void brightness(int);
    void contrast(int);

signals:
   //! emist a signal to reload the image in main window
   void reloadImage(Matrix);

private slots:
    void botCancelClicked();
    void botApplyClicked();
    void process(int);
    void txtCChanged(QString);
    void txtBChanged(QString);
};

#endif

