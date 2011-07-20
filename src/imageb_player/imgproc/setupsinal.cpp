#include "setupsinal.h"

SetupSinal::SetupSinal()
{
  s_ramp = 0;
  e_ramp = 100;
  
  s_cut = 0;
  e_cut = 0;

  s_att = 0;
  e_att = 0;
  att_factor = -30;
}


void SetupSinal::setRefSignal( Matrix _data )
{
   ref_signal = _data;
}

void SetupSinal::setRFData(Matrix rf)
{
  rfdata = rf;
  env = rfdata;
  time_cut = rfdata;

  //para garantir que o valor do vetor de limiar seja do mesmo tamanho do sinal
  limiar = rfdata;
  setThreshold(v_limiar);

  imgproc.hilbert(env);
  env_gain = env;
}

Matrix SetupSinal::getRFData()
{
  return rfdata;
}

void SetupSinal::setGain(int num_pts, int start, int end)
{
  imgproc.createGainVector(gain,num_pts,start,end);

  //atribui parâmetros de ganho 
  s_ramp = start; 
  e_ramp = end;
}

Matrix SetupSinal::getTimeCut()
{
  return time_cut;
}

Matrix SetupSinal::getGainVector()
{
  return gain;
}

void SetupSinal::setEnvGain(Matrix envelope_gain)
{
  env_gain = envelope_gain;
}

void SetupSinal::setGainVector(Matrix data)
{
  gain = data;
}

Matrix SetupSinal::getEnvGain()
{
  return env_gain;
}

void SetupSinal::setThreshold( double threshold)
{ 
  
  limiar.set(threshold);
  v_limiar = threshold;
}

Matrix SetupSinal::getThresholdVec()
{
  return limiar;
}

void SetupSinal::setThresholdVec( Matrix data )
{
   limiar = data;
}

void SetupSinal::setAttenuation(int start, int end, double factor)
{
  s_att = start;
  e_att = end;
  att_factor = factor;
  
  imgproc.setAttenuation(s_att,e_att,factor, gain);
}

void SetupSinal::setTimeCutPoints(int start, int end)
{
   s_cut = start;
   e_cut = end;
}

void SetupSinal::setEnv(Matrix data)
{
   env = data;
}

Matrix SetupSinal::getEnv()
{
   return env;
}

void SetupSinal::setDefaults()
{
  limiar.set(0);
  v_limiar = 0;
  
  s_att = 0;
  e_att = 0;
  att_factor = -30;

  s_ramp = 0;
  e_ramp = 100;

  s_cut = 0;
  e_cut = 0;
  
  rfdata = ref_signal;
  env_gain = env;
  imgproc.createGainVector(gain,rfdata.nRow(),s_ramp,e_ramp);
  imgproc.setAttenuation(s_att,e_att,att_factor, gain);
  imgproc.limiarCut(v_limiar, env_gain);
  imgproc.applyGain(gain,env_gain);
  
}

//aplica a operação de ganho no sinal + corte limiar
void SetupSinal::applyGainLimiar()
{
   env_gain = env;
   imgproc.limiarCut(v_limiar, env_gain);
   imgproc.applyGain(gain,env_gain);
}

Matrix SetupSinal::getProcData()
{
  return env_gain;
}


void SetupSinal::applyTimeCut(int start, int end)
{
   s_cut = start;
   e_cut = end;
   time_cut = ref_signal;  

   if (s_cut == 0)
      s_cut = 1;
   
      imgproc.timeCut(start,end,time_cut);
   
   rfdata = time_cut; //seta o corte ao sinal de rf
}



void SetupSinal::copyData(SetupSinal _obj_src, SetupSinal& _obj_dest)
{
    _obj_dest.setRFData(_obj_src.getRFData());
    _obj_dest.s_ramp = _obj_src.s_ramp;
    _obj_dest.e_ramp = _obj_src.e_ramp;
    _obj_dest.s_att  = _obj_src.s_att;
    _obj_dest.e_att  = _obj_src.e_att;
    _obj_dest.s_cut  = _obj_src.s_cut;
    _obj_dest.e_cut  = _obj_src.e_cut;

    _obj_dest.att_factor = _obj_src.att_factor;
    _obj_dest.ref_signal = _obj_src.ref_signal;
    _obj_dest.time_cut = _obj_src.time_cut;
     
    _obj_dest.setGainVector( _obj_src.getGainVector() );
    _obj_dest.setEnvGain( _obj_src.getEnvGain() );
    _obj_dest.setThreshold( _obj_src.v_limiar );

}


/*
SetupSinal& operator=(const SetupSinal& sinal) {
    if( this == &sinal ) return *this;  // If two sides equal, do nothing.
    
    this->s_ramp = sinal.s_ramp;
    this->e_ramp = sinal.e_ramp;
    this->s_att = sinal.s_att;
    this->e_att = sinal.e_att;
    this->att_factor = sinal.att_factor;
    
    this->gain = sinal.gain;
    this->env_gain = sinal.env_gain;
    this->limiar = sinal.limiar;
    this->rfdata = sinal.rfdata;
    this->v_limiar = sinal.v_limiar;
    return *this;
}
*/
