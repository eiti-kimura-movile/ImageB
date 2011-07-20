#include "producer.h"

Producer::Producer(QObject *parent)
: QThread(parent)
 {
     abort = false;
     this->setTerminationEnabled(true); //permite que o processo seja terminado a qualquer momento
}

void Producer::setSource( QStringList files )
{
   this->files = files;
}

void Producer::stop()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
    db25.clear();
    terminate();
    wait();
}

//inicia ou termina o processo
void Producer::play_pause()
{
    mutex.lock();
    abort = !abort;
    mutex.unlock();
    wait();

    if ( this->isRunning() )
      {
         this->quit(); //end loops
         this->terminate();
         wait();
      }
    else
       {
         this->start(); 
       }

}

Matrix Producer::acquire()
 {
  
    Matrix mat, aux;
    mat.set(0);
 
    //quando não há média, retorna diretamente a matriz
    if ( aq.avg <= 1 )
      {
        
        db25.reset();
        for(int i=2; i < aq.elements; i++)
             db25.nextChannel();
          
        mat = imageB.getDataMatrix(files, files.size(), 1024);
        return mat;
      }
    
    if ( aq.avg > 1 )
      {
        db25.clear();

        //efetua a média
        for (int i=1; i <= aq.avg; i++)
        {
           //adquire os canais
           db25.reset();
           for(int j=2; j < aq.elements; j++)
              db25.nextChannel();

             if (i == 1)
                mat = imageB.getDataMatrix(files, files.size(), 1024);
             else
                {
                   aux = imageB.getDataMatrix(files, files.size(), 1024);
                   //soma todas as linhas e colunas
                   for(int L=1; L <= mat.nRow(); L++)
                     {
                        for (int C=1; C <= mat.nCol(); C++)
                             mat(L,C) += aux(L,C); 
                     }
                }
        }
        
         //faz a divisão efetuando a média
         for(int L=1; L <= mat.nRow(); L++)
            {
               for (int C=1; C <= mat.nCol(); C++)
                    mat(L,C) /=  aq.avg; //divide pelo número de amostras adquiridas
            }
      }
    
    return mat;
 }


void Producer::run()
  {
      head = 0;
  
      forever {
            
            if (abort)
                return;
            
            freeBytes.acquire();
            
            Matrix mat = acquire();
            buffer[head % BufferSize] = mat;
            qDebug() << "PRODUZIDO frame[" << head % BufferSize << "]";
            head++;
            
            usedBytes.release();
            
            if (head == 100000)
               abort = true;
          
        }
      
  }




