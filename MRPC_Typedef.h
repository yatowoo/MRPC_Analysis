/*
**  MRPC Data Format Typdef
**
**  Version
**    v0.1.0, 2016.12.30 17:30, created - by yato
**    v0.2.0, 2016.12.30 19:33, add Initialize method - by yato
**    v0.3.0, 2016.12.31 15:34, add Generate method - by yato
**/

#ifndef YATO_MRPC
#define YATO_MRPC

#include<TString.h>
#include<strstream>

namespace MRPC { //v0.3

  // SYSTEM Info
  const int   MRPC_N = 1;
  const float MRPC_Z[MRPC_N] = {0.}; // default unit: cm
  const int   PMT_N = 4;
  
  // MRPC Info
  const float SIZE_X[MRPC_N] = {32.0};
  const float SIZE_Y[MRPC_N] = {20.0};
  const int   PAD_N [MRPC_N] = {16};
  const int   GAP   [MRPC_N] = {220}; // unit: um
  const int   HV    [MRPC_N] = {6000}; // unit: V
  const float PAD_L [MRPC_N] = {30.0};
  const float PAD_W [MRPC_N] = {0.7};
  const float PAD_G [MRPC_N] = {0.3};
  
  // for struct settings
  const int   PAD_N_MAX = 16;
  
  typedef struct MRPC_Event {
    int m_lt  [MRPC_N][PAD_N_MAX];
    int m_ltot[MRPC_N][PAD_N_MAX];
    int m_rt  [MRPC_N][PAD_N_MAX];
    int m_rtot[MRPC_N][PAD_N_MAX];
    int pmt_t [PMT_N];
    int pmt_q [PMT_N];
  }_MEvent;
  
  // ONLY for Raw Data Read - v0.3
  int mrpc_left_trail [MRPC_N][PAD_N_MAX];
  int mrpc_right_trail[MRPC_N][PAD_N_MAX];
  
  // v0.2
  void Initialize(_MEvent* m_ev){
    for(int imrpc = 0 ; imrpc < MRPC_N ; imrpc ++){
      for(int ipad = 0 ; ipad < PAD_N_MAX ; ipad ++){
        m_ev->m_lt  [imrpc][ipad] = 0;
        m_ev->m_ltot[imrpc][ipad] = 0;
        m_ev->m_rt  [imrpc][ipad] = 0;
        m_ev->m_rtot[imrpc][ipad] = 0;
        
        mrpc_left_trail [imrpc][ipad] = 0;
        mrpc_right_trail[imrpc][ipad] = 0;
      }  
    }
    for(int ipmt = 0 ; ipmt < PMT_N ; ipmt++){
      m_ev->pmt_t[ipmt] = 0;
      m_ev->pmt_q[ipmt] = 0;
    }
  }// method : Initialize
  
  // v0.3
  void GenerateTOT(_MEvent* m_ev){
    for(int imrpc = 0 ; imrpc < MRPC_N ; imrpc ++){
      for(int ipad = 0 ; ipad < PAD_N_MAX ; ipad ++){
        m_ev->m_ltot[imrpc][ipad] = 
          mrpc_left_trail[imrpc][ipad] - m_ev->m_lt[imrpc][ipad];
        m_ev->m_rtot[imrpc][ipad] =
          mrpc_right_trail[imrpc][ipad] - m_ev->m_rt[imrpc][ipad];
      }  
    }
  	
  }// method : GenerateTOT
  
  // v0.3
  void GenerateInfo(TString& exp_str){
    stringstream sstr;
    sstr << exp_str << "\n";
    sstr << "PMT_Number\t" << PMT_N << "\n";
    sstr << "MRPC_Number\t" << MRPC_N << "\n";
    
    sstr << "M_Position/cm";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << MRPC_Z[imprc];
    sstr << "\n";
    
    sstr << "M_SizeX\t";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << SIZE_X[imprc];
    sstr << "\n";  
    
    sstr << "M_SizeY\t";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << SIZE_Y[imprc];
    sstr << "\n";
    
    sstr << "M_Pad_Number";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << PAD_N[imprc];
    sstr << "\n";
  
    sstr << "M_Pad_Length";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << PAD_L[imprc];
    sstr << "\n";
  
    sstr << "M_Pad_Width";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << PAD_W[imprc];
    sstr << "\n";
  
    sstr << "M_Pad_Gap";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << PAD_G[imprc];
    sstr << "\n";
    
    sstr << "M_Gap_Width/um";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << GAP[imprc];
    sstr << "\n";  
    
    sstr << "M_HV/V\t";
    for (int imprc = 0 ; imprc < MRPC_N ; imprc ++)
      sstr << "\t" << HV[imprc];
    sstr << "\n";
    
    TString tmp = sstr.str();
    exp_str = tmp;
  }// method : GenerateInfo

}// namespace : MRPC

#endif // YATO_MRPC
