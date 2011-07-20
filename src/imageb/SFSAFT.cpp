#include "SFSAFT.h"

//! global variabel for acquisition data information
extern Acquisition aq; 

SFSAFT::SFSAFT()
{
  N_SAMPLES = 4096;
  filename = "";
  running  = false;
}

//! constructor for filename
/*!
  this constructor sets automatically the file to be processed
  \param file_name the file name full path
*/
SFSAFT::SFSAFT( QString file_name )
{
   SFSAFT();
   filename = file_name;
}


void SFSAFT::setN_SAMPLES(int n_samples)
{
   N_SAMPLES = n_samples;
}

void SFSAFT::setFile(QString file_name)
{
   filename = file_name;
}

//! converts a short array into a Matrix class
/*!
  \param buffer[] the short data vector
  \return returns a Matrix
*/
Matrix SFSAFT::shortbuffer2matrix( short int buffer[])
{
  Matrix mat(N_SAMPLES);
  
  for (int i=0; i < N_SAMPLES; i++)
      mat(i+1) = buffer[i];
  
  return mat;
}


//! get adjusted data for processing SF
/*!
  \param R matrix of focal points distance
  \param theta the steering angle (degrees)
  \return returns a data Matrix
*/
Matrix SFSAFT::getData(Matrix R, int theta)
{
  Matrix mat, wave, x, y, t_ij, imdata;
  const double pi = 3.141592654;
  
  int N = aq.elements;
  double d = aq.d * 1e-3;
  double deltat = 1/(aq.deltat*1e6);
  double c = (double) aq.c;

  //cout << N << "," << d << "," << deltat << "," << c << "," << aq.theta << endl; 

  short int buffer[N_SAMPLES]; //inteiro de 2bytes equivalente ao  int16 do matlab

  imdata = Matrix(R.nRow());
   
  x = Matrix(R.nRow());
  y = Matrix(R.nRow());
  t_ij = Matrix(R.nRow());
 
  
  file=fopen( (filename.toStdString()).c_str() , "rb");  
  //cout << "abriu arquivo: " <<  (filename.toStdString()).c_str() << ";" << endl;

  //calcula X e Y  
  for (int i = 1; i <= R.nRow(); i++)
  {
     x(i) = R(i) * cos( theta * pi/180 ); 
     y(i) = R(i) * sin( theta * pi/180 );   
  }
  
  for (int i=1; i <= N; i++)
    {
       //printf("\t\t i:%d\n",i);
    
       for (int j=1; j <= N; j++)
         {
         
             if (!running)
              return Matrix(0);
             
             memset(buffer, 0, sizeof(buffer)); //limpa a memória do buffer
             int num = fread( buffer, sizeof(short int), N_SAMPLES, file ); 
             
             if (num)
              {
               Matrix mdata = shortbuffer2matrix(buffer);
               
               imgproc.norm(mdata);
               
               wave = imgproc.createWave(imgproc.zeros(177), mdata, imgproc.zeros(1200));
               
               t_ij.set(0.0);
               for (int k=1; k <= R.nRow();k++)
               { 
                 t_ij(k) = sqrt(pow(x(k),2) + pow((y(k) -d * (i - ((double)(N/2)+0.5))), 2) )/c + sqrt(pow(x(k),2) + (pow((y(k) -d * (j - ((double)(N/2)+0.5))), 2)) )/c;
                 imdata(k) += wave( (int) (1 + round(t_ij(k)/deltat)) ); 
               }
             
            }
            else
               printf("ERRO DE LEITURA %d, %d", i,j);
         }
    }

  fclose(file);
  
  return imdata;
}


//! process SF algorithm
void SFSAFT::procSFSAFT()
{
   int theta = (int) aq.theta;

   Matrix R = imgproc.getVectorRef(10e-3,150e-3,0.1e-3); //cria o espaço da imagem
   Matrix mat = Matrix( R.nRow(), theta*2+1); //cria matrix de dados da imagem com o número de posições referente ao angulo

   int index = 0;
  
   //focaliza para todos os angulos entre -30 até 30 por exemplo
   for (int ang = theta*-1; ang <= theta; ang++){
        
        if (!running)
           return;
       
       cout << "angulo :" << ang << endl;
       Matrix col = getData(R,ang);
       
       imgproc.writeMatrixData(col,"coldata.txt");

       imgproc.setCol(++index,col, mat);
      
       emit changeValue( (int) round((index*100)/(theta*2+1)), index );
   }
 
   imgproc.writeMatrixData(mat,"matrix.txt");
 
   //ImageB imageB;
 
   printf("Processing...");
   
   //Matrix mat_proc = mat;
 
  /*
   imgproc.norm(mat_proc);
   imgproc.hilbert2D(mat_proc);
   imgproc.normTo255(mat_proc);
   imgproc.writeMatrixData(mat_proc,"processado.txt");
 */
 
   aq.rf_matrix = mat;
   
   emit finished();
   
   cout << "Finalizado" << endl;

}

//! stops the execution
void SFSAFT::stop_process()
{
   running = false;
}

//! starts the thread
void SFSAFT::run()
{
        running = true;

	qDebug() << "Executing in new independant thread, GUI is NOT blocked";
        
/*
	for(int i=0;i<10;i++)
	{
		qDebug() << "Time: " << 10-i;	

		int t=1;

		// some OS specific stuff
		// mingw (3.4.2) sleep on windows is called _sleep and uses microseconds
		#ifdef Q_OS_WIN32
		t = t * 1000;
		_sleep(t);
		#else
		sleep(t);
		#endif
                
                emit changeValue((i+1)*10,i);
	}
	
	qDebug() << "Execution done";
*/

        procSFSAFT();
	exec();
}
