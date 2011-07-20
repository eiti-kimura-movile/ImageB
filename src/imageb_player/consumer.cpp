#include "consumer.h" 
#include <time.h>

time_t start_time, end_time;

Consumer::Consumer(QObject *parent)
: QThread(parent)
 {
     abort = false;
     c_fps = 0;
     color_map = 0;
     emit numFramesPerSecond(c_fps);
     this->setTerminationEnabled(true); //permite que o processo seja terminado a qualquer momento
}

void Consumer::setColorMap( int map )
{
  mutex.lock();
  color_map = map;
  mutex.unlock();
}

//pára processo
void Consumer::stop()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
    this->quit();
    this->terminate();
    wait();
}


void Consumer::run()
{
      tail = 0;
      (void) time(&start_time);

      while(!abort) {
      
            //só processa se existirem bytes usados      
            if ( usedBytes.available() >= 1 )
              {
                  
                  if (abort)
                    {
                      qDebug() << "CONSUMER: Saiu do loop \n";
                      return;
                    }
                              
                  usedBytes.acquire();
              
                  Matrix mat = buffer[tail % BufferSize];
                  
                  //percorre todas as linhas e colunas e multiplica por um valor aleatório
                  for (int col=1; col <= mat.nCol(); col++)
                      {
                        for( int line=1; line <= mat.nRow(); line++ )
                           {
                               mat(line,col) = mat(line,col) * ((int)rand() % 10);
                           }
                      }
                  
                  //configurações de processamento de sinal
                  imageB.setSignalSetup(signal_setup);
                    
                  Matrix mat_proc = imageB.procData2BMode( mat );
                  
                  QImage image = imageB.getBModeImage(600,400, mat_proc, color_map);
                  
                  emit renderedImage( QPixmap::fromImage(image), 0.00403897f );
                  
                  qDebug() << "CONSUMIDO frame[" << tail % BufferSize << "]";
                  tail++;
                  
                  c_fps++; //conta quantos frames por segundo
                  
                  //efetua contagem de 1s  
                  (void) time(&end_time);
                  double diff = difftime(end_time, start_time);
                  
                  if (diff >= 1)
                    {
                      emit numFramesPerSecond(c_fps);//emite sinal com o total de frames por segundo
                      (void) time(&start_time);
                      c_fps = 0;
                    }
                  
                  
                  if (tail == 100000)
                    {
                     abort = true;
                     cout << "Aborted by Tail " << endl;
                    }
                  
                  freeBytes.release();
              }
            
        }
      
}