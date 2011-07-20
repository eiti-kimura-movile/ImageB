#ifndef DB25PORT_H
#define DB25PORT_H

#include <QLibrary>
#include <iostream>

#include <windows.h>
#include <time.h>

/* Definitions in the build of inpout32.dll are:            */
/*   short _stdcall Inp32(short PortAddress);               */
/*   void _stdcall Out32(short PortAddress, short data);    */


/* prototype (function typedef) for DLL function Inp32: */
typedef short _stdcall (*inpfuncPtr)(short portaddr);
typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);
//typedef void (*oupfuncPtr)(short,short);

//typedef short (*inpfuncPtr)(short portaddr);

using namespace std;

class DB25PP: public QObject
{
    Q_OBJECT

   public:
      DB25PP();
      virtual ~DB25PP();
   
      void clear(); //seta todos os bits para zero
      void reset();   //seta o bit 8
      void nextBit(); //vai para o proximo bit
      void firstBit();//vai para o primeiro bit
      void lastBit(); //vai para o ultimo bit
   
      void nextChannel();
      int read();
      void write(int);
      void writeBit(int);
      
      void setBit(int,bool); //seta um bit para ativo/inativo

  private:
     QLibrary *lib;
     
     inpfuncPtr inp32;
     oupfuncPtr oup32;
  
     int rst_bit; //indicacao de bit de reset 
     int act_bit; //indicacao de bit efetivo (envia dado ao multiplex)
     int pport_addr; //endereço da paralela 0x378
  
     int curr_bit; //bit corrente
  
     int bit[8]; 
   
     int sleep_time;
  
     void sleep(unsigned int mseconds);
};


#endif

