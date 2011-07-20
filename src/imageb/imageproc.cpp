#include "imageproc.h"
#include "acquisition.h"

extern Acquisition aq;

ImageProc::ImageProc()
{
  ;
}


vector<double> ImageProc::loadFile(string filename)
{
   vector<double> rfdata;
  double rf = 0;
  string line;
  
  ifstream datafile ( filename.c_str() );
  
  if (datafile.is_open())
  {
    while (! datafile.eof() )
    {
      getline (datafile,line);
      //valor da segunda coluna
      
      if(line.find(",",0) != string::npos)
        {
          rf = atof(line.substr( line.find(",",0)+1, 10 ).c_str());//converte para double
          rfdata.push_back(rf);
        }
    }
    datafile.close();
  }
  else cout << "Unable to open file"; 
  
  //seta vetor para ficar com 1024 posicoes
  while( rfdata.size() < 1024 )
        rfdata.push_back(0.0);

  return rfdata;
}



Matrix ImageProc::loadFileM(string filename)
{
  int num_rows=1024;//, num_cols=0;

  Matrix rfdata = Matrix(num_rows);
  double rf = 0;
  QString line;
  string line_aux;
  int index = 1;

  ifstream datafile ( filename.c_str() );
  
  if (datafile.is_open())
  {
    while (! datafile.eof() )
    {
      getline (datafile, line_aux); //faz a leitura de uma linha
   
       if(line_aux.find(",",0) != string::npos)
        {
          rf = atof(line_aux.substr( line_aux.find(",",0)+1, 10 ).c_str());//converte para double
          rfdata(index++) = rf;
        }

    }
    datafile.close();
  }
  else cout << "Unable to open file"; 

  return rfdata;
}



Matrix ImageProc::loadFileM2(string filename)
{
  Matrix mat = vector2matrix( loadFile(filename) );
  return mat;
}



Matrix ImageProc::loadMatrixFiles(QStringList filenames, int num_files, int num_rows = 1024)
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
     rfdata = loadFileM( filenames[i].toStdString() );
     norm(rfdata); //normaliza (remove offset) rfdata - mean(rfdata);
     hilbert(rfdata);//calcula envoltoria
     normTo255(rfdata);//normaliza entre valoes 0-255
     
     for (int j=1; j <= num_rows-shift; j++ )
        mat(j,i+1) = rfdata(j); //i+1 porque o indice da matriz inicia em 1
     
     rfdata.set(0);
  }

  return mat;
}




double ImageProc::mean( vector<double> data )
{
   double mean = 0;
   for(uint i=0; i < data.size(); i++)
       mean += data[i];

   return mean/data.size(); 
}


vector<double> ImageProc::norm(vector<double> data)
{
   double _mean = mean(data);
   vector <double> norm_data;

   for (uint i=0; i < data.size(); i++)
       norm_data.push_back(data[i] - _mean);

   return norm_data;
}

double ImageProc::min( vector<double> data)
{
   double xmin = data[0];
   
   for(uint i=1; i < data.size(); i++)
      {
        if ( data[i] < xmin )
           xmin = data[i];
      }
   
    return xmin;
}

double ImageProc::max( vector<double> data)
{
   double xmax = data[0];
   
   for(uint i=1; i < data.size(); i++)
      {
        if ( data[i] > xmax )
           xmax = data[i];
      }
   
    return xmax;
}


double ImageProc::max(Matrix mat)
{
   double _max = 0;

   if ( mat.nCol() == 1 )
      _max = max( matrix2vector(mat) );
   else
     {
       _max = mat(1,1);
       
       //percorre todas as linhas e colunas em busca do maior elemento
       for (int col=1; col <= mat.nCol(); col++)
         {
            for( int line=1; line <= mat.nRow(); line++ )
                {
                   if ( mat(line,col) > _max )
                       _max = mat(line,col);
                }
         }
     }
  
   return _max;
}


double ImageProc::min( Matrix mat)
{
   double _min = 0;

   if ( mat.nCol() == 1 )
      _min = min( matrix2vector(mat) );
   else
     {
       _min = mat(1,1);
       
       //percorre todas as linhas e colunas em busca do menor elemento
       for (int col=1; col <= mat.nCol(); col++)
         {
            for( int line=1; line <= mat.nRow(); line++ )
                {
                   if ( mat(line,col) < _min )
                       _min = mat(line,col);
                }
         }
     }
  
   return _min;
}

double ImageProc::mean( Matrix mat)
{
   //return mean( matrix2vector(mat) );
   double _mean = 0;

   for (int i=1; i <= mat.nRow(); i++)
     {
         for (int j=1; j <= mat.nCol(); j++)
             _mean += (double)mat(i,j);
     }
   
   return _mean/(mat.nRow()*mat.nCol());
}

void ImageProc::norm( Matrix& mat )
{
   double _mean = mean(mat);
   
   for (int i=1; i <= mat.nRow(); i++)
     {
        for (int j=1; j <= mat.nCol(); j++)
            mat(i,j) = mat(i,j) - _mean;
     }
}


//faz um resize em largura na matriz
void ImageProc::resize( Matrix& mat, int dim )
{
   Matrix resized = Matrix(mat.nRow(), dim);
  
   for(int i=1; i <= dim; i++)
     {
       for (int j=1; j <= mat.nRow(); j++)
            resized(j,i) = mat(j);
     }
   
   mat = resized;
}



void ImageProc::normTo255(double vec_max, double vec_min, Matrix& mat)
{
  

   //percorre todos os elementos da matrix
  if (mat.nCol() == 1 )
  {
    for(int i=1; i<=mat.nRow(); i++ )
      mat(i) = round((mat(i)*255)/vec_max);
  }
  else
  {
       for (int j=1; j <= mat.nRow(); j++)
         {
            //grava linhas e colunas
           for(int i=1; i <= mat.nCol(); i++)
               mat(j,i) = round((mat(j,i)*255)/vec_max);
         }
  }

}

//normaliza para valores entre 0 e 255
void ImageProc::normTo255(Matrix& mat)
{
  
  double v_max = 0, v_min = 0;
  /*
  v_max = max(mat);
  v_min = min(mat);
  
  //percorre todos os elementos da matrix
  if (mat.nCol() == 1 )
  {
    for(int i=1; i<=mat.nRow(); i++ )
      mat(i) = round((mat(i)*255)/v_max);
  }
  else
  {
       for (int j=1; j <= mat.nRow(); j++)
         {
           
            //grava linhas e colunas
           for(int i=1; i <= mat.nCol(); i++)
               mat(j,i) = round((mat(j,i)*255)/v_max);
         }
  }
  */
  
  v_max = 255;
  v_min = 0;
  
  double span = v_max - v_min;
  double vec_max, vec_min;
  
  vec_max = max(mat);
  vec_min = min(mat);
  
  //cout << "VMAX:" << vec_max << " VMIN:" << vec_min << endl;
  
  for (int i=1; i <= mat.nCol();i++)
  {
    
    for(int j=1; j <= mat.nRow();j++)
      {
        mat(j,i) = mat(j,i) - vec_min; //ajuste de zero
        mat(j,i) = (int)round((mat(j,i)*v_max)/(vec_max));
      }
  }
  
}



Matrix ImageProc::getCol(Matrix mat, int col)
{
   Matrix mat_col(mat.nRow(),1);
   
   for(int i=1; i <= mat.nRow(); i++)
      mat_col(i) = mat(i,col);
   
   //writeMatrixData(mat_col, "matcol.txt");
   return mat_col;
}



Matrix ImageProc::getLine(Matrix mat, int line)
{
   Matrix mat_lin(512,1);
   
   for(int i=1; i <= mat.nCol(); i++)
      mat_lin(i) = mat(line, i);
   
   //writeMatrixData(mat_lin, "matcol.txt");
   return mat_lin;
}


void ImageProc::setLine(int line, Matrix mat_line, Matrix& mat)
{
   for(int i=1; i <= mat.nCol(); i++)
      mat(line, i) = mat_line(i);
}


void ImageProc::setCol(int col, Matrix mat_col, Matrix& mat)
{
   for(int i=1; i <= mat.nRow(); i++)
      mat(i,col) = mat_col(i);
}


vector<double> ImageProc::matrix2vector(Matrix mat)
{
   vector<double> vec;
   for(int i=1; i <= mat.nRow(); i++)
       vec.push_back( mat(i) );
   
   return vec;
}




Matrix ImageProc::vector2matrix(vector<double> vec)
{
   cout << "vecsize: " << vec.size() << endl;
   Matrix mat = Matrix(vec.size(),1);

   for(uint i=1; i <= vec.size(); i++)
       mat(i) = vec[i-1];
   
   return mat;
}


//cria vetor com zeros de tamanho num
Matrix ImageProc::zeros(int num)
{
     if (num > 0)
      return Matrix(num);
   else
      return Matrix(0,0);
}


//concatena os 3 vetores
Matrix ImageProc::createWave(Matrix begin, Matrix middle, Matrix end)
{
   int size = begin.nRow() + middle.nRow() + end.nRow();
   
   Matrix wave(size);
  
  for(int i=1; i <= begin.nRow(); i++ )
      wave(i) = begin(i);
  
  for(int i=1; i <= middle.nRow(); i++ )
      wave(begin.nRow()+i) = middle(i);

  for(int i=1; i <= end.nRow(); i++ )
      wave(begin.nRow()+middle.nRow()+i) = end(i);

  return wave;
}




/*
*  Transformada Rápida de Fourier FFT
*  Routine to compute discrete Fourier transform using FFT algorithm
* Inputs
*    RealA, ImagA         Real and imaginary parts of data vector
* Outputs
*    RealA, ImagA         Real and imaginary parts of transform
*/
void ImageProc::fft( Matrix& RealA, Matrix& ImagA) {

  double RealU, RealW, RealT, ImagU, ImagW, ImagT;

  //* Determine size of input data and check that it is power of 2
  int N = RealA.nRow();  // Number of data points
  int M = (int)(log( (double)N )/log(2.0) + 0.5);  // N = 2^M
  int NN = (int)(pow(2.0,(double)M) + 0.5);

  //ajusta o vetor para uma potencia de 2 maior que o tamanho do vetor atual
  while(NN < N)
  {
    M += 1;
    NN = (int)(pow(2.0,(double)M) + 0.5);
  }
  //cout << "N : " << N << " M : " << M << " NM : " << NN <<endl;

  if( N != NN ) {
    //cerr << "ERROR in fft(): Number of data points not power of 2" << endl;
    //return;
    
    cout << "FFT ajustando vetor para:" << NN << endl; 
  
    
    //ajusta o vetor para uma potencia de 2 válida
    RealA = createWave(zeros(1),RealA,zeros(NN-N-1));
    ImagA = createWave(zeros(1),ImagA,zeros(NN-N-1));
    cout << RealA.nRow() << ImagA.nRow() << endl;
  
    N = RealA.nRow();  // Number of data points
    M = (int)(log( (double)N )/log(2.0) + 0.5);  // N = 2^M
    NN = (int)(pow(2.0,(double)M) + 0.5);
  }

  const double pi = 3.141592654;
  int N_half = N/2;
  int Nm1 = N-1;
  
  //* Bit-scramble the input data by swapping elements
  int i,k,j=1;
  for( i=1; i<=Nm1; i++ ) {
    if( i < j ) {
      RealT = RealA(j);     ImagT = ImagA(j);	  // Swap elements i and j 
      RealA(j) = RealA(i);  ImagA(j) = ImagA(i);  // of RealA and ImagA
      RealA(i) = RealT;     ImagA(i) = ImagT;
    }
    k = N_half;
    while( k < j ) {
      j -= k;
      k /= 2;
    }
    j += k;
  }

  //* Loop over number of layers, M = log_2(N)
  for( k=1; k<=M; k++ ) {
    int ke = (int)(pow(2.0,(double)k) + 0.5);
    int ke1 = ke/2;
	//* Compute lowest, non-zero power of W for this layer
    RealU = 1.0;  ImagU = 0.0;
    double angle = -pi/ke1;
    RealW = cos(angle);  ImagW = sin(angle);
	//* Loop over elements in binary order (outer loop)
    for( j=1; j<=ke1; j++ ) {
	  //* Loop over elements in binary order (inner loop)
      for( i=j; i<=N; i+=ke ) {
        int ip = i + ke1;
		//* Compute the y(.)*W^. factor for this element
        RealT = RealA(ip)*RealU - ImagA(ip)*ImagU;	// T = A(ip)*U
        ImagT = RealA(ip)*ImagU + ImagA(ip)*RealU;
		//* Update the current element and its binary pair
        RealA(ip) = RealA(i)-RealT;
        ImagA(ip) = ImagA(i)-ImagT;	   // A(ip) = A(i) - T
        RealA(i) += RealT;
        ImagA(i) += ImagT;			   // A(i) = A(i) + T
      }
	  //* Increment the power of W for next set of elements
      double temp = RealU*RealW - ImagU*ImagW;
      ImagU = RealU*ImagW + ImagU*RealW;	   // U = U * W
      RealU = temp;
    }
  }
}

/*
*  Transformada Rápida Inversa de Fourier IFFT
*  Routine to compute discrete Inverse Fourier transform using iFFT algorithm
* Inputs
*    RealA, ImagA         Real and imaginary parts of data vector
* Outputs
*    RealA, ImagA         Real and imaginary parts of transform
*/
void ImageProc::ifft( Matrix& RealA, Matrix& ImagA) {
// Routine to compute inverse Fourier transform using FFT algorithm
// Inputs
//    RealA, ImagA         Real and imaginary parts of transform
// Outputs
//    RealA, ImagA         Real and imaginary parts of time series

  int i, N = RealA.nRow();   // Number of data points

  //* Take complex conjugate of input transform
  for( i=1; i<=N; i++ )
    ImagA(i) *= -1.0;        // Complex conjugate

  //* Evaluate fast fourier transform  
  fft( RealA, ImagA );       
  
  //* Take complex conjugate and normalize by N
  double invN = 1.0/N;
  for( i=1; i<=N; i++ ) {
    RealA(i) *= invN;
    ImagA(i) *= -invN;    // Normalize and complex conjugate
  }
}


// Routine to compute two dimensional Fourier transform 
// using FFT algorithm
// Inputs
//    RealA, ImagA         Real and imaginary parts of data array
// Outputs
//    RealA, ImagA         Real and imaginary parts of transform
void ImageProc::fft2(Matrix& RealA, Matrix& ImagA)
{

  int i, j, N = RealA.nRow(), M = RealA.nCol();
  Matrix RealT(N), ImagT(N);  // Temporary work vector

  //* Loop over the columns of the matrix
  for( j=1; j<=M; j++ ) {
	//* Copy out a column into a vector
    for( i=1; i<=N; i++ ) {
      RealT(i) = RealA(i,j);  
      ImagT(i) = ImagA(i,j);
    }
    
    //* Take FFT of the vector
    adjustMatrixPow2(RealT);
    adjustMatrixPow2(ImagT);
    fft(RealT,ImagT);         
	//* Copy the transformed vector back into the column
    for( i=1; i<=N; i++ ) {
      RealA(i,j) = RealT(i); 
      ImagA(i,j) = ImagT(i);
    }
  }

  //* Loop over the rows of the matrix
  for( i=1; i<=N; i++ ) {
	//* Copy out a row into a vector
    for( j=1; j<=M; j++ ) {
      RealT(j) = RealA(i,j);  
      ImagT(j) = ImagA(i,j);
    }
    
    //* Take FFT of the vector
    adjustMatrixPow2(RealT);
    adjustMatrixPow2(ImagT);
    fft(RealT,ImagT);         
    
    //* Copy the transformed vector back into the row
    for( j=1; j<=M; j++ ) {
      RealA(i,j) = RealT(j);  
      ImagA(i,j) = ImagT(j);
    }
  }

}




void ImageProc::ifft2( Matrix& RealA, Matrix& ImagA) {
// Routine to compute inverse two dimensional Fourier transform 
// using FFT algorithm
// Inputs
//    RealA, ImagA         Real and imaginary parts of transform array
// Outputs
//    RealA, ImagA         Real and imaginary parts of data array

  int i, j, N = RealA.nRow();
  Matrix RealT(N), ImagT(N);  // Temporary work vector

  //* Loop over the columns of the matrix
  for( j=1; j<=N; j++ ) {
	//* Copy out a column into a vector and take its complex conjugate
    for( i=1; i<=N; i++ ) {
      RealT(i) = RealA(i,j);      
      ImagT(i) = -1.0*ImagA(i,j); 
    }
  
    //* Take FFT of the vector
    adjustMatrixPow2(RealT);
    adjustMatrixPow2(ImagT);
    fft(RealT,ImagT);         
	//* Copy the transformed vector back into the column
    for( i=1; i<=N; i++ ) {
      RealA(i,j) = RealT(i);      
      ImagA(i,j) = ImagT(i);
    }
  }

  //* Loop over the rows of the matrix
  double invN2 = 1.0/(N*N);
  for( i=1; i<=N; i++ ) {
	//* Copy out a row into a vector and take its complex conjugate
    for( j=1; j<=N; j++ ) {
      RealT(j) = RealA(i,j);  
      ImagT(j) = ImagA(i,j);
    }
	//* Take FFT of the vector
    adjustMatrixPow2(RealT);
    adjustMatrixPow2(ImagT);
    fft(RealT,ImagT);         
	//* Copy the transformed vector back, taking its complex conjugate
	//  and applying the 1/N normalization
    for( j=1; j<=N; j++ ) {
      RealA(i,j) = RealT(j)*invN2;      
      ImagA(i,j) = -1.0*ImagT(j)*invN2; 
    }                                   
  }
}



void ImageProc::writeMatrixData(Matrix mat,  string filename )
{
   ofstream dataf(filename.c_str()); 

  if (mat.nCol() == 1 )
  {
    for(int i=1; i<=mat.nRow(); i++ )
      dataf << mat(i) << endl;
  }
  else
  {
       for (int j=1; j <= mat.nRow(); j++)
         {
           
            //grava linhas e colunas
           for(int i=1; i <= mat.nCol(); i++)
               dataf << mat(j,i) << '\t';
          
           dataf << endl;
           
         }
  }
}


void ImageProc::hilbert( Matrix& mat ) {

  int n_rows = mat.nRow(); //numero de linhas do vetor
  Matrix imag = mat;//Matrix(n_rows,1);
  
  
   //calcula a fft
   fft(mat, imag);
   int half = (int) round(mat.nRow()/2);//pega o índice do meio do vetor
  
   //cout << "valor half:" << half << "matrow: " << mat.nRow() <<endl;
   //seta segunda metade do vetor (parte negativa) para 0;
   for( int i=half; i <= n_rows; i++)
   {
     mat(i) = 0.0;
     imag(i) = 0.0;
   }
 
   
   //writeMatrixData(mat,"hil_fft_proc.txt");
   //writeMatrixData(imag,"hil_fft_imag.txt");
 
   //calcula transformada de fourier inversa
   ifft(mat, imag);
 
   //writeMatrixData(mat,"hil_ifft_proc.txt");
   //writeMatrixData(imag,"hil_ifft_imag.txt");
 
   //multiplica todo vetor por 2 e tira valor absoluto;
   for( int i=1; i <= n_rows; i++ )
     {
        mat(i) = mat(i)*2;
        imag(i) = imag(i)*2;
     }
   
   mat = modcomplex(mat,imag);
}


void ImageProc::hilbert2D( Matrix& mat ) {
   
   norm(mat);
   
   for (int i=1; i <= mat.nCol(); i++)
     {
        Matrix col = getCol(mat,i);   
        adjustMatrixPow2(col);
        hilbert(col);
        setCol(i,col,mat);
     }  
  
}


void ImageProc::hilbert2( Matrix& mat ) {

  int i, j, N = mat.nRow(), M = mat.nCol();
  Matrix TmpMat(N);  // Temporary work vector
  
  adjustMatrixPow2(TmpMat);
  
  //* Loop over the columns of the matrix
  for( j=1; j<=M; j++ ) {
  
    TmpMat.set(0.0);
  
    //* Copy out a column into a vector and take its complex conjugate
    for( i=1; i<=N; i++ )
      TmpMat(i) = mat(i,j);
    
    //* Take hilbert transform
    hilbert(TmpMat);
    
	//* Copy the transformed vector back into the column
    for( i=1; i<=N; i++ )
      mat(i,j) = TmpMat(i);
  }
 
  /*
  writeMatrixData(TmpMat, "tmpmat.txt");
  writeMatrixData(mat, "hilbert1.txt");

  cout << "hilbert2: " << "passou primeira parte COLS" << endl;

  TmpMat = Matrix(N);
  adjustMatrixPow2(TmpMat);
  */

  //* Loop over the rows of the matrix
  for( i=1; i<=N; i++ ) {
  
    TmpMat.set(0.0);
  
    for( j=1; j<=M; j++ ) {
      TmpMat(j) = mat(i,j);  
    }
	//* Take FFT of the vector
    //cout << i << "-" << TmpMat.nRow() << endl;
    //cout << "Entrando calculo " << i << "L:" << TmpMat.nRow() << "C:" << TmpMat.nCol() << endl;
    hilbert(TmpMat);
    //cout << i << "-" << TmpMat.nRow() << endl;
  
    //* Copy the transformed vector back into the row
    for( j=1; j<=M; j++) {
      mat(i,j) = TmpMat(j);
    }
    
    //cout << "passou" << endl;
  }

 //cout << "hilbert2: " << "passou  parte ROWS" << endl;
 //writeMatrixData(mat, "hilbert2.txt");

}

//calcula o módulo de um número complexo  sqrt(real^2 + imaginario^2);
Matrix ImageProc::modcomplex(Matrix real, Matrix imag)
{
   Matrix modulus = Matrix(real.nRow(),real.nCol());

   if  (real.nCol() == 1 )
     {
       for ( int i=1; i <= modulus.nRow(); i++)
            modulus(i) = sqrt( pow(real(i),2) + pow(imag(i),2) );
    }
  else
    {
        for ( int i=1; i <= modulus.nRow(); i++)
          {
          
             for ( int j=1; j <= modulus.nCol(); j++)
               {
                  modulus(i,j) = sqrt( pow(real(i,j),2) + pow(imag(i,j),2) );
               }
          
          }
    
    }
 
   return modulus;
}


//ajusta vetor para um tamanho de potencia de 2 válida
void ImageProc::adjustMatrixPow2( Matrix& col)
{
      //* Determine size of input data and check that it is power of 2
      int N = col.nRow();  // Number of data points
      int M = (int)(log( (double)N )/log(2.0) + 0.5);  // N = 2^M
      int NN = (int)(pow(2.0,(double)M) + 0.5);
    
      //recursão para encontrar um número potencia de 2 maior que o tamanho do vetor
      //para que não se percam dados
      if (NN < N || NN < 255)
      {
        col = createWave(zeros(1),col,zeros(4));
        adjustMatrixPow2(col);
      }
      else
      {
         //cout <<"N:" << N << " M:" << M << " NN:" << NN << " valor:" << NN-N-1 << endl;
         if( N != NN )
           //ajusta o vetor para uma potencia de 2 válida
           col = createWave(zeros(1),col,zeros(NN-N-1));
      }
}


Matrix ImageProc::createColorMap()
{
   int r=0,g=0,b=127;  
   int step = 16;
   int flag_red = 0;
   Matrix colormap(64,3);

   //condição de parada vermelho (128,0,0)

   //while( r != 128 && g != 0 && b != 0 )
   for (int i=0; i < 63; i++ )
   {
       //tons de azul
       if ( b < 255 && g == 0 && r == 0 )
          b += step;
      
       //indica mudanca para amarelo
       if ( b >= 255 && g >= 255 && r < 128)
           flag_red = 1;
      
       //transicao azul -> verde
       if ( b >= 255 && g <= 255 && r == 0)
          g += step;
             
      if ( r >= 255 && g != 0 )
          { 
            g -= step;
            flag_red = 0;
          }
               
       //transição azul para amarelo
       if (flag_red == 1)
          {
            r += step;
            b -= step;
          }
       
       //transicao laranja -> vermelho 
       if ( r > 128 && g == 0 && b == 0)
            r -= step;
      
       //seta valores para um range valido
       if (r > 255) r = 255;
       if (g > 255) g = 255;
       if (b > 255) b = 255;
          
       if (r < 0) r = 0;
       if (g < 0) g = 0;
       if (b < 0) b = 0;
          
      //cout << "R:" << r << " G:" << g << " B:" << b << endl; 
      
       colormap(i+1,1) = r; colormap(i+1,2) = g; colormap(i+1,3) = b;
    }
  
   return colormap;
}



/*
  Cria vetor com valores entre <start> e <end> variando com 
  passos de <step>
*/
Matrix ImageProc::getVectorRef(double start, double end, double step)
{
   Matrix mat;
   double value = start;
   vector<double> data;
   
   if (value < end)
   {
       while (value <= end)
       {
          data.push_back(value);
          value += step; 
       }
   }
   else
   {
       while (end <= value)
       {
          data.push_back(value);
          value += step; 
       }
   }

   return vector2matrix(data);
}



//faz operação de mesh grid, 
//x completa com as linhas
//y a linha passa a ser coluna
void ImageProc::meshgrid(Matrix& x, Matrix& y, Matrix mat)
{
   int size = mat.nRow();
   x = Matrix( size, size );
   y = Matrix( size, size );
   
   for (int line=1; line <= size; line++)
    {
        for(int col=1; col <= size; col++)
          {
            x(line,col) = mat(col);
            y(line,col) = mat(line);
          }
    }
}


void ImageProc::meshgrid2D(Matrix& x, Matrix& y, Matrix mat1, Matrix mat2)
{
   int L = mat2.nRow();
   int C = mat1.nRow();

   x = Matrix( L, C );
   y = Matrix( L, C );
   
   for (int line=1; line <= L; line++)
    {
        for(int col=1; col <= C; col++)
          {
            x(line,col) = mat1(col);
            y(line,col) = mat2(line);
          }
    }
}


//normaliza a um determinado tamanho
void ImageProc::normToSize( Matrix& mat, int size )
{
  double vec_max = max(mat);
  //double vec_min = min(mat);

  //percorre todos os elementos da matrix
  if (mat.nCol() == 1 )
  {
    for(int i=1; i<=mat.nRow(); i++ )
      mat(i) = round((mat(i)*size)/vec_max);
  }
  else
  {
       for (int j=1; j <= mat.nRow(); j++)
         {
            //grava linhas e colunas
           for(int i=1; i <= mat.nCol(); i++)
               mat(j,i) = round((mat(j,i)*size)/vec_max);
         }
  }
}



//converte de coordenada polar para cartesiana
void ImageProc::pol2cart( double theta, Matrix vec, Matrix& x, Matrix& y )
{
   x = Matrix( vec.nRow(), vec.nCol() );
   y = Matrix( vec.nRow(), vec.nCol() );

   for (int col=1; col <= vec.nCol(); col ++)
   {
     for( int line=1; line<= vec.nRow(); line++ )
         {
            x(line,col) = vec(line,col) * cos(theta);
            y(line,col) = vec(line,col) * sin(theta);
         }
   }
}



void ImageProc::pol2cart( Matrix vec, Matrix theta, Matrix& x, Matrix& y )
{
   x = Matrix( vec.nRow(), vec.nCol() );
   y = Matrix( vec.nRow(), vec.nCol() );

   for (int col=1; col <= vec.nCol(); col ++)
   {
     for( int line=1; line<= vec.nRow(); line++ )
         {
            x(line,col) = vec(line,col) * cos(theta(line,col));
            y(line,col) = vec(line,col) * sin(theta(line,col));
         }
   }
}



void ImageProc::adjustYAxis( Matrix& Y )
{

 cout << "ajusta eixo: theta -> " << aq.theta << endl;
 Matrix vec = getVectorRef(aq.theta*-1,aq.theta,1);

  for (int j=1; j <= Y.nRow(); j++)
     {
        for(int i=1; i <= Y.nCol(); i++)
          {
            Y(j,i) = vec.find(Y(j,i));
          }
     }

}


//aplica ganho a matriz
void ImageProc::applyGain(Matrix& mat, int v_start, int v_end)
{
  int m_size = mat.nRow();
  double step = 0; //passo da criação do vetor

  //calcula valor do passo para que o vetor de ganho fique com o mesmo
  //número de pontos que a matriz
  if (v_end >= v_start)
     step = v_end/(m_size-1);
  else
     step = v_start/(m_size-1);
  
  Matrix gain = getVectorRef(v_start,v_end,step);
  
  //aplica ganho a todas as colunas
  for (int j=1; j <= mat.nCol(); j++)
     {
        for(int i=1; i <= m_size; i++)
          {
                mat(i,j) = mat(i,j) + mat(i,j)*gain(i)/100;
          }
     }

}


//cria um vetor de ganho tipo rampa
void ImageProc::createGainVector(Matrix& v_gain, int vec_size, int v_start, int v_end)
{
  double step = 0; //passo da criação do vetor

  //calcula valor do passo para que o vetor de ganho fique com o mesmo
  //número de pontos que a matriz
  if (v_end >= v_start)
  {
     
      if ( (abs(v_start) % 2) == 0)
        v_start = abs(v_start)+1;
      
      if ( (v_end % 2) != 0)
        v_end = abs(v_end)-1;
        
      step = (abs(v_end)-abs(v_start))/double(vec_size-1); 
  
  }
  else 
  {
      if ( (abs(v_start) % 2) != 0)
        v_start = abs(v_start)-1;
      
      if ( (v_end % 2) == 0)
        v_end = abs(v_end)+1;

      step = (abs(v_start)-abs(v_end))/double(vec_size-1)*-1;
  
  } 
  
  cout << "step:" << step << " s:" << v_start << " e:" <<  v_end << " sz:" << vec_size;
  v_gain = getVectorRef(abs(v_start),abs(v_end),step);
  
}


//cria valores de atenuação no início e/ou no fim da curva de ganho
void ImageProc::setAttenuation(int npts_start, int npts_end, double attenuation_factor, Matrix& v_gain)
{
   for (int i=1; i<=npts_start; i++)
       v_gain(i) = attenuation_factor;
   
   for (int j = v_gain.nRow()-npts_end; j <= v_gain.nRow(); j++)
       v_gain(j) = attenuation_factor;
}


//aplica o vetor de ganho ao sinals
void ImageProc::applyGain(Matrix v_gain, Matrix& data)
{
   for (int j=1; j <= data.nCol(); j++)
      {
         for (int i=1; i <= v_gain.nRow(); i++)
              data(i,j) = data(i,j) + data(i,j)*(v_gain(i)/100); 
      }
}


//corta os dados a partir de um limiar
void ImageProc::limiarCut(double v_limiar, Matrix& data)
{
   for (int j=1; j <= data.nCol(); j++)
      {
         for (int i=1; i <= data.nRow(); i++)
             { 
               if ( data(i,j) <= v_limiar)
                  data(i,j) = 0; 
             }
      }
}



void ImageProc::timeCut(int start, int end, Matrix& data)
{
   //assert(start > 0 && end <= data.nRow());

   for (int j=1; j <= data.nCol(); j++)
      {
           //corte do início do sinal
           for (int i=1; i <= start; i++)
             { 
               data(i,j) = 0; 
             }
           
           for (int i=data.nRow()-end; i <= data.nRow(); i++)
             { 
               data(i,j) = 0; 
             }
          
      }
}
