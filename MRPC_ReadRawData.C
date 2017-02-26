/*
*
*  Read Raw Data of MRPC Test Experiment
*    - ROOT script
*  CUSTOMIZED macro, string & raw data format
*
*/

#include "MRPC_Typedef.h"

#define MULTI_FILE	// Merge for 1 Exp .root
//#define EXP_SCAN	// Scan Directory and 1 .root for 1 raw data

int MRPC_ReadRawData(){

// USER CUSTOMIZED STRING
  // Experiment Title
  TString exp_str = "MRPC Experiment";
  // Data Path - can be dir. or filename
  TString path = "./data/";
  // Read File match pattern - EMPTY means all
  TString pattern = "MRPC_Teach_Exp";
  // Output ROOT File
  TFile* root_file = new TFile("./test_MRPC.root","RECREATE");

// Initialize
  // Experiment Info. 
  MRPC::GenerateInfo(exp_str);
  cout << exp_str;
  // Event Data
  MRPC::_MEvent* m_ev = new MRPC::_MEvent;
  MRPC::Initialize(m_ev);
  // Tree
  TTree* ev_tree = new TTree("mrpc","MRPC Exp Data");
  ev_tree->Branch("EventBr","MRPC::_MEvent",&m_ev,32000,99);

#if defined(MULTI_FILE) || defined(EXP_SCAN)
  MRPC::ReadDataFromDir(path,m_ev,ev_tree,pattern.Data());
#else
  MRPC::ReadDataFromFile(path,m_ev,ev_tree);
#endif

  // Write to ROOT file
  ev_tree->Write();
  root_file->WriteObjectAny(&exp_str, exp_str.Class(), "ExpInfo");
  // root_file->Close();
  return 0;
}