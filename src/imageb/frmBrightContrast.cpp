#include <QtGui>

#include "frmBrightContrast.h"
#include "ui_frmBrightContrast.h"


frmBrightContrast::frmBrightContrast(QWidget *parent)
{
    setupUi(this);
    connect(botCancel, SIGNAL(clicked()), this, SLOT(botCancelClicked()));
    connect(botApply, SIGNAL(clicked()), this, SLOT(botApplyClicked()));
    connect(slideB, SIGNAL(valueChanged(int)), this, SLOT(process(int)));
    connect(slideC, SIGNAL(valueChanged(int)), this, SLOT(process(int)));
}


void frmBrightContrast::botApplyClicked()
{
   //emite sinal para atualizar a imagem com a matriz transformada
   emit reloadImage(pixel_data);
   this->close();
}

void frmBrightContrast::botCancelClicked()
{
   //emite sinal para atualizar a image para que se mantenha a original
   emit reloadImage(pixel_image);
   this->close();
}

//! set method, sets the RF data to be processed in this class
/*!
  param m_image_data matrix of RF data points
*/
void frmBrightContrast::setImageData(Matrix m_image_data)
{
   pixel_image = m_image_data;
   
   //é feita uma cópia da imagem original para que se possa restaurar 
   //o estado anterior da imagem caso a alteração seja cancelada
   pixel_data = pixel_image;
}

Matrix frmBrightContrast::getImageData()
{
  return pixel_data;
}


/*!
  perform the brightness operation based in value;
  \param value percentual value to be applied to image brightness
*/
void frmBrightContrast::brightness(int value)
{
  int w,h; 
  int offset = 100;
  //evita loop infito de sinais e slots
  if ( txtB->text().toInt() != (value-offset)  )
     txtB->setText( QString::number( int(value)-offset ) );
 
  float perc = float( (value-offset)/100.0);
  
  w = pixel_data.nRow(); h = pixel_data.nCol(); //dimensões da matrix altura e largura

  for (int i=1; i<=w; i++)
    {
      for (int j=1; j <= h; j++)
           pixel_data(i,j) = pixel_image(i,j) + ( float(pixel_image(i,j)) * perc );
    }
  
  
   //atualiza imagem em tempo real
   if (chkPreview->isChecked())
       emit reloadImage(pixel_data);  
}


/*!
  perform the contrast operation based in value;
  \param value percentual value to be applied to image contrast
*/
void frmBrightContrast::contrast(int value)
{
  int w,h; 
  int offset = 100;

  //evita loop infito de sinais e slots
  if ( txtC->text().toInt() != (value-offset)  )
     txtC->setText( QString::number( int(value)-offset ) );

  float perc = float((value-offset)/100.0);
  
  w = pixel_data.nRow(); h = pixel_data.nCol(); //dimensões da matrix altura e largura

  for (int i=1; i<=w; i++)
    {
      for (int j=1; j <= h; j++)
        {
           if ( pixel_data(i,j) > 128 )
              pixel_data(i,j) = pixel_data(i,j) + ( float(pixel_data(i,j)) * perc );
           else
              pixel_data(i,j) = pixel_data(i,j) - ( float(pixel_data(i,j)) * perc );
        }
    }
  
   //atualiza imagem em tempo real
   if (chkPreview->isChecked())
       emit reloadImage(pixel_data);
}

void frmBrightContrast::process(int value)
{
   brightness( slideB->value() );
   contrast( slideC->value() );
}

void frmBrightContrast::txtCChanged(QString str)
{
   slideC->setValue(str.toInt());
}


void frmBrightContrast::txtBChanged(QString str)
{
   slideB->setValue(str.toInt());
}
