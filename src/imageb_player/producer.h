#include <QThread>
#include <QImage>
#include <QSemaphore>
#include <QMutex>
#include <QtGui>

#include "imgproc/imageb.h"
#include "imgproc/imageproc.h"
#include "imgproc/acquisition.h"
#include "DB25PP.h"

extern const int BufferSize;
extern QSemaphore freeBytes;
extern QSemaphore usedBytes;

extern Matrix buffer[];

extern Acquisition aq;

class Producer : public QThread
{
    Q_OBJECT
 
    public:
        Producer(QObject *parent = 0);
        void setSource( QStringList );
        Matrix acquire();
        void stop();
        void run();
    
    private: 
      
       QMutex mutex;

       bool abort;
      
       ImageB imageB;
       QStringList files;
    
       long int head; //indice do vetor
    
       DB25PP db25;
  
    public slots:
      void play_pause(); 
};
