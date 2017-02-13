/*
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
*  Version :
*    v0.1.0, 2016.12.08, created by yato
*    v0.1.1, 2016.12.29, modified the format, by yato
*    v0.2.0, 2016.12.30, #include<MRPC_Typedef.h>, by yato
*    v0.2.1, 2016.12.31, modified with changes in <MRPC_Typedef.h>, by yato
*    v0.3.0, 2017.01.02, add #ifdef option for single file usage, by yato
*/

#include<cstring>

#include "MRPC_Typedef.h"

//#define MULTI_FILE	// Merge for 1 Exp .root
//#define EXP_SCAN	// Scan Directory and 1 .root for 1 raw data

#if defined(MULTI_FILE) || defined(EXP_SCAN)
  #include<dirent.h>
#endif

const int BUF_SIZE = 1024;
const int N_COL = 112;

int MRPC_ReadRawData(){
  // Experiment Title
  TString exp_str = "MRPC Teach Experiment 2016";
  MRPC::GenerateInfo(exp_str);
  cout << exp_str;

#if defined(MULTI_FILE) || defined(EXP_SCAN)
  // Data Folder Path
  string path = "../DATA/mrpc_teach_exp/test/";
  string file;
  // Directory Read
  DIR* dp = opendir(path.c_str());
  dirent* dir;
  dir = readdir(dp); // .
  dir = readdir(dp); // ..
#else // ONLY 1 file to read
  string file = "../DATA/mrpc_teach_exp/test/PMT#3_Test_2016-12-13_21_35.txt";
#endif // v0.3

  ifstream fin;
  stringstream sstr;
  char buffer[BUF_SIZE];
  
  // Event Data
  MRPC::_MEvent* m_ev = new MRPC::_MEvent;
  MRPC::Initialize(m_ev);
  
  TFile* root_file = new TFile("new_test_v0_3.root","RECREATE");
  TTree* ev_tree = new TTree("mrpc","MRPC Exp Data");
  ev_tree->Branch("EventBr","MRPC::_MEvent",&m_ev,32000,99);

  /*
  *  BEGIN - Raw Data File Reading
  */
  int file_count = 0;
  int ev_count = 0;
  int trash; // Empty or Unused Column

#if defined(MULTI_FILE) || defined(EXP_SCAN)
  // Directory traverse
  while((dir = readdir(dp))!= NULL){
    file = path + dir->d_name;
#endif // v0.3

    fin.open(file);
    if(fin.is_open()){
      file_count ++;
      cout << file << endl;
    }else{
      cout << " ERROR - File Not Found " << endl
           << "--> FileName : " << file << endl;
      return 1;
    }// open file
    
    
    // Read - 1 line
    while(fin.getline(buffer,BUF_SIZE) 
      && strlen(buffer) > N_COL*2){
      ev_count ++;
      sstr.str("");
      sstr << buffer;
      
/******* 
**
**  MUST be CUSTOMIZED for specific Exp set-up
**
*******/
      for(int i = 0 ; i < 16 ; i++) sstr >> m_ev->m_lt [0][i];
      for(int i = 0 ; i < 12 ; i++) sstr >> m_ev->m_rt [0][15-i];
      for(int i = 0 ; i <  4 ; i++) sstr >> m_ev->pmt_t[i];
      for(int i = 0 ; i < 16 ; i++) sstr >> MRPC::mrpc_left_trail [0][i];
      for(int i = 0 ; i < 12 ; i++) sstr >> MRPC::mrpc_right_trail[0][15-i];
      for(int i = 0 ; i <  4 ; i++) sstr >> trash; //pmt_trai[i];
      for(int i = 0 ; i < 32 ; i++) sstr >> trash; // V1290n
      for(int i = 0 ; i <  4 ; i++) sstr >> m_ev->pmt_q[i];
      for(int i = 0 ; i < 12 ; i++) sstr >> trash; // Unused QDC CH
      
      MRPC::GenerateTOT(m_ev);

      sstr.clear();
      ev_tree->Fill();
    }// for each event/line
    
    cout << ev_count << endl;
    
    fin.close();

#if defined(MULTI_FILE) || defined(EXP_SCAN)
  }// for each file
  closedir(dp);
  cout << file_count << endl;
#endif // v0.3
  /*END*/
  
  // Write to ROOT file
  ev_tree->Write();
  root_file->WriteObjectAny(&exp_str, exp_str.Class(), "ExpInfo");
  // root_file->Close();
  return 0;
}