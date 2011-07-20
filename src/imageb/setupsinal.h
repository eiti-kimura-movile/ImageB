#ifndef SETUPSINAL_H
#define SETUPSINAL_H

#include "imageproc.h"
#include "NumMeth.h"

class SetupSinal
{
  public:
      //! default constructor
      SetupSinal();
   
      void setRFData(Matrix);
      Matrix getRFData();
  
      void setEnv(Matrix);
      Matrix getEnv();
  
      void setGain(int, int, int);
      void setGainVector(Matrix);
      Matrix getGainVector();
  
      void setEnvGain(Matrix);
      Matrix getEnvGain();
      
      void setThreshold(double);
      Matrix getThresholdVec();
      void setThresholdVec(Matrix);
      
      Matrix getTimeCut();
  
      void setAttenuation(int, int, double);
      
      void setTimeCutPoints(int, int);
      
      void setRefSignal(Matrix);
      
      void applyTimeCut(int,int);
      
      void applyGainLimiar();
      
      Matrix getProcData();
  
      void copyData(SetupSinal, SetupSinal&);
  
      void setDefaults();
      
      int s_ramp;//valor de inicio da rampa
      int e_ramp;//valor final da rampa
      
      int s_att;//valor de inicio da atenuação
      int e_att;//valor final da atenuação
      double att_factor;//fator de atenuação
      
      //pontos de início e fim do corte
      int s_cut;
      int e_cut;
      
      double v_limiar;//valor do limiar
      Matrix time_cut;
      
  private:
      ImageProc imgproc;
  
      Matrix rfdata;
      Matrix ref_signal; //sinal de referencia
      
      Matrix env;
      Matrix env_gain;
      Matrix gain;
      Matrix limiar;
      
      
      Matrix proc_data;

};

#endif
