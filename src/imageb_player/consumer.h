#include <QThread>
#include <QImage>
#include <QSemaphore>
#include <QMutex>
#include <QtGui>
 
#include "imgproc/imageb.h"

extern const int BufferSize;
extern QSemaphore freeBytes;
extern QSemaphore usedBytes;
extern Matrix buffer[];

//!global variable to Signal Setup
extern SetupSinal signal_setup;

class Consumer: public QThread
{
    Q_OBJECT
 
    public:
        Consumer(QObject *parent = 0);
        void stop();
        void run();
    
    signals:
        void renderedImage(const QPixmap &image, double scaleFactor);
        void numFramesPerSecond(int fps); //calcula o número de frames por segundo
    
    private: 
       QMutex mutex;
    
       bool abort;
       long int tail;
       ImageB imageB;
       int c_fps; //contador de frames/sec
       int color_map;
    
    public slots:
       void setColorMap(int);
};

