/*
**  MRPC Data Format Typdef and Utility Functions
**
*
*  Read Raw Data of MRPC & PMT
*
*  Raw Data Format
*    1 event 1 line
*    
*    example : MRPC_Teach_Exp_2016
*    TDC-1290A - 32 lead + 32 trail
*    TDC-1290N - 16 lead + 16 trail
*    QDC-792N  - 16 q
*
*  Hardware Set-up :
*    1290A
*      00-15 MRPC_Left (Pad_01-16)
*      16-27 MRPC_Right (Pad_16-04)
*      28-31 PMT (#1-#4)
*    1290N - Unused
*    792N
*      00-03 PMT (#1-#4)
*
*  Data Path : ../DATA/mrpc_teach_exp/
*
*  Tree Format
*    MORE in MRPC_Typedef.h
*
*  Output :
*    <Path/FileName>
*    <Line Count>
*    ...
*    <Total Line Count>
*
*/

#ifndef YATO_MRPC_H
#define YATO_MRPC_H

#include "TString.h"

#include<iostream>
#include<strstream>

const int BUF_SIZE = 1024;
const int N_COL = 112;

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
  
  // TEMP for Raw Data Read
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

  int ReadDataFromFile(TString& fileName, _MEvent* m_ev, TTree* ev_tree){
    
    ifstream fin(fileName.Data());
    if( ! fin.is_open()){
      std::cout << "[X] ERROR - File Fail to Open "
        << std::endl << "\t"
        << fileName << std::endl;
      return -1;
    }

    std::cout << fileName << std::endl;
 
    int ev_count = 0;
    int trash; // Empty or unused column
    char buffer[BUF_SIZE];

    // Check a good line for read
    stringstream sstr;
    while(fin.getline(buffer, BUF_SIZE) && strlen(buffer) > N_COL * 2){
      ev_count++;
      sstr.str(""); // reset stream
      sstr << buffer;
 
    //
    // MUST be CUSTOMIZED for specific Exp set-up
    //
      for(int i = 0 ; i < 16 ; i++) sstr >> m_ev->m_lt [0][i];
      for(int i = 0 ; i < 12 ; i++) sstr >> m_ev->m_rt [0][15-i];
      for(int i = 0 ; i <  4 ; i++) sstr >> m_ev->pmt_t[i];
      for(int i = 0 ; i < 16 ; i++) sstr >> MRPC::mrpc_left_trail [0][i];
      for(int i = 0 ; i < 12 ; i++) sstr >> MRPC::mrpc_right_trail[0][15-i];
      for(int i = 0 ; i <  4 ; i++) sstr >> trash; //pmt_trai[i];
      for(int i = 0 ; i < 32 ; i++) sstr >> trash; // V1290n
      for(int i = 0 ; i <  4 ; i++) sstr >> m_ev->pmt_q[i];
      for(int i = 0 ; i < 12 ; i++) sstr >> trash; // Unused QDC CH

      GenerateTOT(m_ev);

      sstr.clear();
      ev_tree->Fill();
    }// line by line - 1 line 1 event
    fin.close();
    std::cout << "[-] INFO - Event Count : " << ev_count << std::endl;
    
    return ev_count;
  }// method - ReadDataFromFile

  bool ReadDataFromDir(TString& pathName, _MEvent* m_ev, TTree* ev_tree, const char* pattern = ""){
    void* dir = gSystem->OpenDirectory(pathName.Data());
    if(!dir){
      std::cout << "[X] ERROR - Fail to open directory : "
        << pathName << std::endl;
      return false;
    }
    const char* fileName = NULL;
    TString filePath;
    fileName = gSystem->GetDirEntry(dir); // .
    fileName = gSystem->GetDirEntry(dir); // ..

    // Directory Traverse
    int file_count = 0;
    int ev_count = 0, ev_total = 0;
    while((fileName = gSystem->GetDirEntry(dir)) != NULL){
      filePath = fileName;
      if( ! filePath.Contains(pattern) ) continue;

      filePath = pathName + fileName;
      if((ev_count = 
        ReadDataFromFile(filePath, m_ev, ev_tree)) < 0)
        //skip or return?;
        return false;
      else{
        file_count ++;
        ev_total += ev_count;
      }// Check file ok?
    }// Dir Traverse
    std::cout << "[-] INFO - File Count : " 
      << file_count << std::endl
      << "[-] INFO - Event Count : " << ev_total << std::endl;

    gSystem->FreeDirectory(dir);
    return true;
  }// method - ReadDataFromDir

}// namespace : MRPC

#endif // YATO_MRPC_H
