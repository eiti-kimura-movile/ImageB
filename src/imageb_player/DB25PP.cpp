#include "DB25PP.h"

DB25PP::DB25PP()
{
   //abrindo a biblioteca
   lib = new QLibrary();

   //cout << "is library:" << lib->isLibrary("inpout32.dll") << endl;
   lib->setFileName("inpout32.dll");   
   lib->load();
   
  if (!lib->isLoaded())
      cout << "Erro ao abrir a biblioteca" << endl;

   /* get the address of the function */

   inp32 = (inpfuncPtr) lib->resolve("Inp32");
  
   if (!inp32) {
        printf("GetProcAddress for Inp32 Failed.\n");
   }

   oup32 = (oupfuncPtr) lib->resolve("Out32");

   if (!oup32) {
        printf("GetProcAddress for Oup32 Failed.\n");
   }
   
 
  //tempo de espera em ms para escrita de um bit 
   sleep_time = 1;
 
   //1000000 - ultimo bit eh o de reset (reset bit)
   rst_bit = 0x40; 
 
   //0100000 - penultimo bit, envio de dados (action bit)
   act_bit = 0x80;
 
   //endereço da paralela 0x378
   pport_addr = 0x378;
 
   //vertor com o dado de cada bit
   bit[0] = 1;   //00000001
   bit[1] = 2;   //00000010
   bit[2] = 4;   //00000100
   bit[3] = 8;   //00001000
   bit[4] = 16;  //00010000
   bit[5] = 32;  //00100000
   bit[6] = 64;  //01000000
   bit[7] = 128; //10000000
   curr_bit =0;
   
   write(rst_bit);
 
}

DB25PP::~DB25PP()
{
   //libera memória ocupada pela biblioteca
   lib->unload();
}


void DB25PP::clear()
{
  (oup32)(pport_addr, 0x00);
  (oup32)(pport_addr, rst_bit);
}


void DB25PP::writeBit(int i_bit)
{
  clear();
  write(bit[i_bit]);
}

void DB25PP::nextBit()
{
  //passa por todos os bits
  if ( !(curr_bit >= 0 && curr_bit < 8) )
     curr_bit =0;
  
  (oup32)(pport_addr, bit[curr_bit]);
  curr_bit++;
}

void DB25PP::reset()
{
   int word = read(); //le byte atual 
   
   //ativa o setimo pino
   write(word-rst_bit);
   Sleep(sleep_time);
   //volta a palavra origial
   write(word);
}

//gera um pulso no pino 8
void DB25PP::nextChannel()
{
   
   int word = read(); //le byte atual 
   
   //ativa o oitavo pino
   write(word+act_bit);
   Sleep(sleep_time);
   //volta a palavra origial
   write(word);
}


void DB25PP::setBit(int i_bit, bool turn_on)
{
   int word = read(); //le byte atual 

   int data = bit[i_bit];
   
   //ativa/desativa um bit
   if (! turn_on)
       data *= -1; 

   write(word+data);
}

void DB25PP::write(int byte)
{
  (oup32)(pport_addr, byte);
}


int DB25PP::read()
{
  return (inp32)(pport_addr);
}
