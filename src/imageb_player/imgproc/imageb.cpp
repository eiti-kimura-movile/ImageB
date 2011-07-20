#include "imageb.h"

//! global variable to signal acquisition information
extern Acquisition aq;

//! default constructor
ImageB::ImageB(){

}


//! takes data columns distinguished by a separator (char). Comma for example
/*!
  \param line a string data line from file
  \param separator a character used as separator
*/
vector<double> ImageB::procDataLine(string line, char separator)
{
   vector<int> idx;
   vector<double> cols;
   double rf = 0;
   int start=0, end = 0;

  idx.push_back(0);//indice do primeiro elemento

  //retorna o vetor vazio se a linha estiver em branco
  if ( line.size() == 0 )
      return cols;
  
  for(uint i=0; i < line.size(); i++) 
    {
      if ( line[i] == separator )
          idx.push_back(i); 
    }
  
   //não encontrou nenhum separador na string, não sendo portanto um formato valido
  if (idx.size() == 1)
     return cols;

  //idx.push_back(idx[idx.size()-1]+20); //cria indice para ler o ultimo elemento
  
  start = idx[0];
  
  for (uint i=1; i < idx.size(); i++)
  {
    end = idx[i];
    rf = atof(line.substr( start, end ).c_str());//converte para double
    cols.push_back(rf);
    start = end+1;
  }
  
  return cols;
}



//! loads matrix data from a file
/*!
 \param filename the full path of file
*/
Matrix ImageB::loadMatrixData(string filename)
{
   int n_line=0, n_col=0;

   Matrix aux = loadData(filename, n_line, n_col);
 
   Matrix mat(n_line, n_col);
   
   for (int line=1; line <= n_line; line++)
       {
         for (int col=1; col <= n_col; col++ )
             mat(line,col) = aux(line,col);
       }
  
   return mat;
}

//! load a binary file forma
/*!
  this method is still in test
*/
Matrix ImageB::loadBinaryData(string filename)
{
  const int buffersize = BUFSIZ;
  double buffer[buffersize] = {0};
  Matrix mat(513,buffersize);

  FILE* p_infile;

  int length = 0;
  int done = 0;

  p_infile = fopen( filename.c_str(), "rb" );

  if( !p_infile )
  {
      cerr << "error opening %s for reading" << filename << endl;
      return mat;
  }

  int j=0;
  do
  {
     length = fread( buffer, 1, sizeof( buffer ), p_infile );
     done = length < (int)sizeof( buffer );
     
     for(int i=0;i< buffersize;i++)
     { 
       mat(j+1, i+1) = buffer[i];
     }

  }
  while( !done );
  
  fclose(p_infile);  

  return mat;
  
}

//! loads a file and puts the number of lines and columns into variables
/*!
  \param filename the full path of a file
  \param n_line number of lines of a file
  \param n_col number of columns of a file
*/
Matrix ImageB::loadData(string filename, int& n_line, int& n_col)
{
   Matrix mat(2048,2048);
   string line;
   ifstream datafile ( filename.c_str() );
   int min_col; //pega a linha com menor quantidade de colunas para evitar erros de indexação da matrix

   n_line = n_col = 0;
   min_col = 2048;

  if (datafile.is_open())
  {
    while (! datafile.eof() )
    {
      getline (datafile,line);
      vector<double> vec;
      //verifica para os separadores válidos:  , ; \t
      vec = procDataLine(line,';');
      if (vec.size() == 0)
      {
         vec = procDataLine(line,'\t');
         if (vec.size() == 0)
               vec = procDataLine(line,',');
      }
    
      /*verifica se a linha não está em branco. Isso evita erros de arquivos 
       defeituosos ou com uma linha em branco o final*/
      if (vec.size() != 0 && line != "")//tamanho em colunas diferente de zero
      {   
        n_col = vec.size();
        
        if (n_col <= min_col)//encontra a menor coluna
            min_col = n_col;
        
        ++n_line;
        
        for( uint i=1; i <= vec.size(); i++)
          {
            mat(n_line,i) = vec[i-1];
        }
      }
    
    }
    datafile.close();
  }
  else cout << "Unable to open file"; 
   
  n_col = min_col;//seta a linha com menos colunas
  if (n_col <= 0 || n_line <= 0)
        n_col = n_line = 1;

  return mat;
}


//! loads a set of files into a Matrix
/*!
  \param filenames a QStringList containing a list of files to be open
  \param num_files the number of files
  \param num_rows number of rows of files
*/
Matrix ImageB::getProcDataMatrix(QStringList filenames, int num_files, int num_rows = 1024)
{
  /* o tamanho original dos sinais é de 1000 pontos, para calcular a fft e ifft o vetor foi
   redimensionado para 1024 (multiplo de 2). O valor do shift é o que deve ser subtraído do 
  final do vetor para que ele permaneça com valor origial antes do processamento */
  
  int shift = 0;
  if (num_rows >= 1024)
     shift = 24;

  Matrix mat = Matrix(num_rows-shift, num_files);
  Matrix rfdata;

  for (int i=0; i < num_files; i++)
  {
     rfdata = imgproc.loadFileM( filenames[i].toStdString() );
     imgproc.norm(rfdata); //normaliza (remove offset) rfdata - mean(rfdata);
     imgproc.hilbert(rfdata);//calcula envoltoria
     imgproc.normTo255(rfdata);//normaliza entre valoes 0-255
     
     for (int j=1; j <= num_rows-shift; j++ )
        mat(j,i+1) = rfdata(j); //i+1 porque o indice da matriz inicia em 1
  }

  return mat;
}


//! loads normalized data files list
/*!
   \param filenames a QStringList containing a list of files to be open
   \param num_files the number of files
   \param num_rows the number of rows
   
   \return returns the data from files into a normalized Matrix 
*/
Matrix ImageB::getDataMatrix(QStringList filenames, int num_files, int num_rows)
{
 
  Matrix mat = Matrix(num_rows, num_files);
  Matrix rfdata;

  for (int i=0; i < num_files; i++)
  {
     rfdata = imgproc.loadFileM( filenames[i].toStdString() );
     imgproc.norm(rfdata); //normaliza (remove offset) rfdata - mean(rfdata);
     
     for (int j=1; j <= num_rows; j++ )
        mat(j,i+1) = rfdata(j); //i+1 porque o indice da matriz inicia em 1
  }

  return mat;
}


//! process a BMode Image
/*!
   \param mat the matrix of RF data 
   \param width  the width of new image
   \param height the height of new image
   \param _colormap indicates which colormap this process uses
   
   \return returns a QImage from processed data 
*/
QImage ImageB::getBModeImage(int width, int height, Matrix mat, int _colormap)
{
  curr_colormap = _colormap; //seta o colormap atual do objeto

  QImage image;

  try
  {
      image = getBModeImage(mat);
      
      //redimensiona a imagem 
      if (!image.isNull())
          image = image.scaled(width,height, Qt::IgnoreAspectRatio, Qt::FastTransformation);
  }
  catch(...)
    {
      cout << "Error getBModeImage(int width, int height, Matrix mat, int _colormap)" << endl;
    }
    
  return image;
}



//retorna um uma matrix unidimensional com foco dado por determinado angulo theta
//! gets focused data vector
/*!
  this method retuns a Matrix focused by a given steer angle.

  \param aq data acquisition information
  \param R a matrix of RF data
  \param theta the steering angle
  \return a Matrix with new coordinates
*/
Matrix ImageB::getVecBFocus(Acquisition aq, Matrix R, float theta)
{
  Matrix mat, wave, x, y, t_ij,imdata;
  const double pi = 3.141592654;
  
  imdata = Matrix(R.nRow());
  mat = Matrix(R.nRow(), aq.elements ); 
  x = Matrix(R.nRow());
  y = Matrix(R.nRow());
  t_ij = Matrix(R.nRow());
 
  //calcula X e Y  
  for (int i = 1; i <= R.nRow(); i++)
  {
     x(i) = R(i) * cos( theta * pi/180 ); 
     y(i) = R(i) * sin( theta * pi/180 );   
  }

  for (int el=1; el <= aq.elements; el++)
  {
    wave = imgproc.createWave(imgproc.zeros((int)aq.delay_size), imgproc.getCol(aq.rf_matrix, el), imgproc.zeros(2000));  

    for (int i=1; i<=R.nRow(); i++)
      {
        t_ij(i) = 1 + round( (2 * sqrt( pow(x(i),2) + (pow(y(i) -aq.d * ( el - 6.5 ), 2)) )/aq.c) / aq.deltat );
        imdata(i) += wave( (int) t_ij(i) ); 
      }
    
  }

  return imdata;
}


//! gets focused image
/*!
  this method retuns a QImage focused by a given steer angle.

  \param aq data acquisition information
  \param width  the width of new image
  \param height the height of new image
  \param _colormap indicates which colormap this process uses
  \param theta the steering angle

  \return returns a transformed QImage with new coordinates
*/
QImage ImageB::getBModeFocusImage(int width, int height, int theta, Acquisition aq, int _colormap)
{
  Matrix R = imgproc.getVectorRef(10e-3,90e-3,0.05e-3); //cria o espaço da imagem
  Matrix mat = Matrix( R.nRow(), theta*2+1); //cria matrix de dados da imagem com o número de posições referente ao angulo

  int index = 0;
  
  //focaliza para todos os angulos entre -30 até 30 por exemplo
  for (int ang = theta*-1; ang <= theta; ang++){
      Matrix col = getVecBFocus(aq,R,ang);
      imgproc.hilbert(col);//calcula envoltoria            
      imgproc.normTo255(col);//normaliza entre valoes 0-255
      imgproc.setCol(++index,col, mat);
  }
  
  return getBModeImage(mat);

}

//! gets focused data Matrix
/*!
  this method retuns a Matrix focused using theta = 30 degrees.

  \param aq data acquisition information
  \param width  the width of new image
  \param height the height of new image
  \param _colormap indicates which colormap this process uses

  \return returns a transformed Matrix with new coordinates
*/
Matrix ImageB::getBModeFocusMatrix(int width, int height, Acquisition aq, int _colormap)
{
  int theta = ((int)aq.theta);
  Matrix R = imgproc.getVectorRef(10e-3,90e-3,0.05e-3); //cria o espaço da imagem
  Matrix mat = Matrix( R.nRow(), theta*2+1); //cria matrix de dados da imagem com o número de posições referente ao angulo

  //applica ganho na matrix de rf antes do processamento da focalização
  if (applyProc)
     {
          for (int i = 1; i <= aq.rf_matrix.nCol(); i++)
              {
                 Matrix col = imgproc.getCol(aq.rf_matrix, i);
                 imgproc.timeCut(ssetup.s_cut, ssetup.e_cut,col);
                 imgproc.applyGain(ssetup.getGainVector(), col); //aplica vetor de ganho a coluna da matriz
                 imgproc.setCol(i,col, aq.rf_matrix);  
              }
  }


  int index = 0;
  
  //focaliza para todos os angulos entre -30 até 30 por exemplo
  for (int ang = theta*-1; ang <= theta; ang++){
      Matrix col = getVecBFocus(aq,R,ang);
      imgproc.norm(col);
      imgproc.adjustMatrixPow2(col);//ajusta tamanho para fft
      imgproc.hilbert(col);//calcula envoltoria
   
      if (applyProc) //aplica corte por limiar
         imgproc.limiarCut(ssetup.v_limiar, col);
     
      imgproc.setCol(++index,col, mat);
  }

  index = 0;

  double v_max = imgproc.max(mat);
  double v_min = imgproc.min(mat);
   
  for (int ang = theta*-1; ang <= theta; ang++){
      Matrix col = imgproc.getCol(mat,++index);
      imgproc.normTo255(v_max, v_min, col );
      imgproc.setCol(index, col, mat);
  }

  //imgproc.writeMatrixData(mat,"imagecpp.txt");

  return mat;
}

//!  gets a RGB pixel value
/*!
  Create pixel values for grayscale, red and jet colormaps.

  \param color the value of color
  \return a QRgb which is a pixel representation using Qt Toolkit 4.0
*/
QRgb ImageB::getPixelRGB( int color )
{
   const int gray=0, jet=1, pink=2;
   QRgb value;

   if (has_colormap != 1)
   {
     colormap = imgproc.createColorMap();
     has_colormap = 1;
   }
 
   if (color > 255)
     {
       //cout << "Color Out of Range:" << color << endl;
       color = 255;
     }
   
   switch ( curr_colormap )
   {
     case gray:
       value = qRgb(color,color,color);
       break;
    
     case jet:
       //transporta pra jet
       color = (int) round( (color*63)/255 ); //seta proporcional a 63, pois o vetor jet tem no máximo 63 cores
       if (color <= 0) color =1;
      
       if (color > 63) //garante valores entre 1 e 63
           color = 63;
          

       value = qRgb( (int)colormap(color,1), (int) colormap(color,2), (int) colormap(color,3) );
       break;
     
     case pink:
       value = qRgb((int) round((color*80)/100),0,(int) round((color*20)/100));
       break;
     
     default:
       value = qRgb(0,0,0);
   }
 
  return value;
}




//retorna uma QImage com os valores dos pixels
//! gets a new Image based in Matrix values
/*!
  Converts a Matrix data into an image.

  \param mat the processed RF data signal
  \return returns a QImage
*/
QImage ImageB::getBModeImage(Matrix mat)
{
   int color = 0;

   //cria um objeto do tipo imagem
   QImage image(mat.nCol(), mat.nRow(), QImage::Format_RGB32);
   image.fill(0); //inicializa a imagem em preto
  
  try
    {
      for (int col=0; col < mat.nCol(); col++)
       {
           //grava linhas e colunas
           for(int line=0; line < mat.nRow(); line++){
                color = int(round(mat(line+1,col+1))); //recebe valor referente a cor
                image.setPixel(col, line, getPixelRGB(color)); //seta valor do pixel
            }
       }
  }
  catch(...)
    {
      cout << "Error getBModeImage(mat)" << endl;
    }
  
  return image;
}



//normaliza,calcula envoltoria e normaliza entre 0-255 os dados de uma matriz
//! data processing for the RF data
/*!
  In this method the RF Matrix data is normalized, envelope computed, applyed gain and cut process,
  normalized to 8 bits (0-255).
  \param mat the matrix of RF data
  \return returns the processed matrix
*/
Matrix ImageB::procData2BMode(Matrix mat)
{
  Matrix _mat = mat;

  imgproc.norm(_mat); //normaliza a matrix
  imgproc.hilbert2D(_mat); //calcula a envoltória da matrix toda
  
  if (applyProc) //aplica processamento de ganho e corte por limiar
     {
        for (int i=1; i <= _mat.nCol(); i++)
        {
           Matrix col;
           col = imgproc.getCol(_mat,i);

           imgproc.timeCut(ssetup.s_cut, ssetup.e_cut,col);  //corte no tempo
           imgproc.limiarCut(ssetup.v_limiar, col); //corte por limiar
           imgproc.applyGain(ssetup.getGainVector(), col); //aplica vetor de ganho a coluna da matriz
         
           imgproc.setCol(i,col,_mat); 
        }
   }
 
  imgproc.normTo255(_mat); //normaliza para formar a imagem
 
  return _mat;
}


//! creates a cone image
/*!
  this method rebuilds and matrix based into coordinate matrix X and Y. 
  It is a simplified version of surf() function of MatLab(c)
 
  \param X matrix of coordinates to X axis.
  \param Y matrix of coordinates to Y axis.
  \param data original Matrix data
  \return returns a new transformed Matrix (see Matlab surf)
*/
//semelhante a função surf do matlab
Matrix ImageB::createConeImage( Matrix X, Matrix Y, Matrix data )
{
  Matrix mat( data.nRow(), data.nCol()-1 );
  
  for(int i=1; i <= mat.nCol(); i++)
  {
      for (int j=1; j <= mat.nRow(); j++)
       {
          if ( (int)X(j,i) <= mat.nRow() && (int)Y(j,i) <= mat.nCol() )
             mat((int) X(j,i),(int) Y(j,i)) = data(j,i);
          else
             cout << "ERR: " << X(j,i) << "Y:" << Y(j,i) << endl;
       }
  }

  return mat;
}

//! creates a polar image
/*!
  \param _matrix the data Matrix
  \return returns a transformed data Matrix
*/
Matrix ImageB::createPolarImage( Matrix _matrix )
{
  const double pi = 3.141592654;

  Matrix R,X,Y, ang, Z, cone;
  double start, end;  

  cout << "theta:" << aq.theta << endl;

  start = (-1*aq.theta) * pi/180;
  end = aq.theta * pi/180;

  R = imgproc.getVectorRef(10e-3,150e-3,0.1e-3);
  
  //cria o meshgrid
  imgproc.meshgrid2D(ang, Z, imgproc.getVectorRef(start, end , 0.0174), R) ;
  
  Y.abs();//para não perder a parte positiva
  imgproc.pol2cart( Z, ang, X, Y );

  imgproc.normToSize(X, X.nRow() );
  imgproc.normToSize(Y, (int) round(Y.nCol()/2)-1);
  
  //cout << _matrix.nRow() << ", " << _matrix.nCol() << endl;
  //cout << X.nRow() << ", " << X.nCol() << endl;
  //cout << Y.nRow() << ", " << Y.nCol() << endl;
  
  imgproc.adjustYAxis(Y);//ajusta para valores de índices inteiros
  cone = createConeImage(X,Y,_matrix); //reconstroi a matrix com coordenadas polares
  
  //imgproc.writeMatrixData(cone,"cone.txt");
  return cone;
}


void ImageB::setSignalSetup(SetupSinal signal_setup)
{
  applyProc = true;
  ssetup = signal_setup;
}

//! avoid the signal setup to be applied
void ImageB::removeSignalSetup()
{
  applyProc = false;
}


//!convert a color image to grayscale
/*!
  Used to convert RGB Jpg files to grayscale.

  \param image a QImage with original image data points
  \return a grayscale Matrix from QImage
*/
Matrix ImageB::convertRGB2Gray( QImage image )
{
   QRgb pix;
   Matrix mat = Matrix(image.height(),image.width());

   for (long i=1; i <= image.width();i++)
       {
          for(long j=1; j <= image.height();j++) 
            {
              int h =0;
              pix = image.pixel(i,j); //pega valor do pixel
              
              //média dos pixels
              h = int((qRed(pix) + qGreen(pix) + qBlue(pix))/3);            
              mat(j,i) = h; //seta dado na matriz
            } 
       }
  
  return mat;
}

