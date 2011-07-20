#ifndef GLOBALS_H
#define GLOBALS_H
    
#include <QImage>
#include <QSemaphore>    

#include "imgproc/imageproc.h"
#include "imgproc/imageb.h"
    
const int BufferSize = 5;
char buffer[BufferSize];
     
QImage imgbuffer[BufferSize];
      
QSemaphore freeBytes(BufferSize);
QSemaphore usedBytes;

#endif

