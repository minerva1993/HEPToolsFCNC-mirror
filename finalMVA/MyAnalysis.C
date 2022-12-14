#define MyAnalysis_cxx
#include <boost/algorithm/string/replace.hpp>

#include "MyAnalysis.h"
#include <TH2.h>
#include <TStyle.h>

void MyAnalysis::Begin(TTree * /*tree*/)
{
   TString option = GetOption();
}

void MyAnalysis::SlaveBegin(TTree * /*tree*/)
{
  TString option = GetOption();
  string sample = option.Data();
  string era = sample.substr(sample.find_first_of("-")+1,4); //STTH1L3BHct_000-2017Hct_0101010101__syst
  string train_scheme = sample.substr(sample.find_first_of("-")+5,string::npos); //Hct_0101010101__syst
  sample.erase(sample.find_first_of("-"),string::npos); //STTH1L3BHct_000
  string sig_ch = train_scheme.substr(0,3);
  string ver_j3b2 = train_scheme.substr(train_scheme.find_first_of("_")+1,2);
  string ver_j3b3 = train_scheme.substr(train_scheme.find_first_of("_")+3,2);
  string ver_j4b2 = train_scheme.substr(train_scheme.find_first_of("_")+5,2);
  string ver_j4b3 = train_scheme.substr(train_scheme.find_first_of("_")+7,2);
  string ver_j4b4 = train_scheme.substr(train_scheme.find_first_of("_")+9,2);

//  cout << sig_ch << " " << ver_j3b2 << " " << ver_j3b3 << " " << ver_j4b2 << " " << ver_j4b3 << " " << ver_j4b4 << endl;

  dosyst = true;
  if( option.Contains("Run201") ) dosyst = false;

  //Delete ntuple number and data era so that we can merge histos w.r.t. dataset, prepare assign ntuple
  if( train_scheme.find("jec") != string::npos or train_scheme.find("jer") != string::npos 
    or train_scheme.find("TuneCP5") != string::npos or train_scheme.find("hdamp") != string::npos ){
    dosyst = false;//make another hist file
    if     ( train_scheme.find("jecup") != string::npos )       syst_ext = "jecup";
    else if( train_scheme.find("jecdown") != string::npos )     syst_ext = "jecdown";
    else if( train_scheme.find("jerup") != string::npos )       syst_ext = "jerup";
    else if( train_scheme.find("jerdown") != string::npos )     syst_ext = "jerdown";
    else if( train_scheme.find("TuneCP5up") != string::npos )   syst_ext = "TuneCP5up";
    else if( train_scheme.find("TuneCP5down") != string::npos ) syst_ext = "TuneCP5down";
    else if( train_scheme.find("hdampup") != string::npos )     syst_ext = "hdampup";
    else if( train_scheme.find("hdampdown") != string::npos )   syst_ext = "hdampdown";
  }
  string syst_str = "-" + train_scheme.substr(train_scheme.find_last_of("_")+1,string::npos );
  if( syst_ext == "" ) syst_str.erase(0,1);
  train_scheme.erase( train_scheme.find_last_of("_"),string::npos );

  const char* score_file_j3b2 = "";
  const char* score_file_j3b3 = "";
  const char* score_file_j4b2 = "";
  const char* score_file_j4b3 = "";
  const char* score_file_j4b4 = "";

  score_file_j3b2 = Form("./scores/%s/%s_j3b2_%s%s/score_finalMVA_%s.root", era.c_str(), sig_ch.c_str(), ver_j3b2.c_str(), syst_str.c_str(), sample.c_str());
  score_file_j3b3 = Form("./scores/%s/%s_j3b3_%s%s/score_finalMVA_%s.root", era.c_str(), sig_ch.c_str(), ver_j3b3.c_str(), syst_str.c_str(), sample.c_str());
  score_file_j4b2 = Form("./scores/%s/%s_j4b2_%s%s/score_finalMVA_%s.root", era.c_str(), sig_ch.c_str(), ver_j4b2.c_str(), syst_str.c_str(), sample.c_str());
  score_file_j4b3 = Form("./scores/%s/%s_j4b3_%s%s/score_finalMVA_%s.root", era.c_str(), sig_ch.c_str(), ver_j4b3.c_str(), syst_str.c_str(), sample.c_str());
  score_file_j4b4 = Form("./scores/%s/%s_j4b4_%s%s/score_finalMVA_%s.root", era.c_str(), sig_ch.c_str(), ver_j4b4.c_str(), syst_str.c_str(), sample.c_str());

  string file_tmp_path_j3b2 = score_file_j3b2;
  ifstream file_tmp_j3b2(file_tmp_path_j3b2);
  if( file_tmp_j3b2.is_open() ){
    scoreF_j3b2 = TFile::Open(score_file_j3b2, "READ");
    scoreT_j3b2 = (TTree*) scoreF_j3b2->Get("tree");
    int nevt = scoreT_j3b2->GetEntries();
    if( nevt > 0){
      for(int i = 0; i < nevt; i++){
        scoreT_j3b2->GetEntry(i);
        float pt = scoreT_j3b2->GetLeaf("lepPt")->GetValue(0);
        float met = scoreT_j3b2->GetLeaf("missinget")->GetValue(0);
        lepPt_j3b2.push_back(pt);
        missET_j3b2.push_back(met);
      }
    }
  }
  else cout << "Not opened in j3b2 : " << option.Data() << " - " << file_tmp_path_j3b2 << endl;

  string file_tmp_path_j3b3 = score_file_j3b3;
  ifstream file_tmp_j3b3(file_tmp_path_j3b3);
  if( file_tmp_j3b3.is_open() ){
    scoreF_j3b3 = TFile::Open(score_file_j3b3, "READ");
    scoreT_j3b3 = (TTree*) scoreF_j3b3->Get("tree");
    int nevt = scoreT_j3b3->GetEntries();
    if( nevt > 0){
      for(int i = 0; i < nevt; i++){
        scoreT_j3b3->GetEntry(i);
        float pt = scoreT_j3b3->GetLeaf("lepPt")->GetValue(0);
        float met = scoreT_j3b3->GetLeaf("missinget")->GetValue(0);
        lepPt_j3b3.push_back(pt);
        missET_j3b3.push_back(met);
      }
    }
  }
  else cout << "Not opened in j3b3 : " << option.Data() << " - " << file_tmp_path_j3b3 << endl;

  string file_tmp_path_j4b2 = score_file_j4b2;
  ifstream file_tmp_j4b2(file_tmp_path_j4b2);
  if( file_tmp_j4b2.is_open() ){
    scoreF_j4b2 = TFile::Open(score_file_j4b2, "READ");
    scoreT_j4b2 = (TTree*) scoreF_j4b2->Get("tree");
    int nevt = scoreT_j4b2->GetEntries();
    if( nevt > 0){
      for(int i = 0; i < nevt; i++){
        scoreT_j4b2->GetEntry(i);
        float pt = scoreT_j4b2->GetLeaf("lepPt")->GetValue(0);
        float met = scoreT_j4b2->GetLeaf("missinget")->GetValue(0);
        lepPt_j4b2.push_back(pt);
        missET_j4b2.push_back(met);
      }
    }
  }
  else cout << "Not opened in j4b2 : " << option.Data() << " - " << file_tmp_path_j4b2 << endl;

  string file_tmp_path_j4b3 = score_file_j4b3;
  ifstream file_tmp_j4b3(file_tmp_path_j4b3);
  if( file_tmp_j4b3.is_open() ){
    scoreF_j4b3 = TFile::Open(score_file_j4b3, "READ");
    scoreT_j4b3 = (TTree*) scoreF_j4b3->Get("tree");
    int nevt = scoreT_j4b3->GetEntries();
    if( nevt > 0){
      for(int i = 0; i < nevt; i++){
        scoreT_j4b3->GetEntry(i);
        float pt = scoreT_j4b3->GetLeaf("lepPt")->GetValue(0);
        float met = scoreT_j4b3->GetLeaf("missinget")->GetValue(0);
        lepPt_j4b3.push_back(pt);
        missET_j4b3.push_back(met);
      }
    }
  }
  else cout << "Not opened in j4b3 : " << option.Data() << " - " << file_tmp_path_j4b3 << endl;

  string file_tmp_path_j4b4 = score_file_j4b4;
  ifstream file_tmp_j4b4(file_tmp_path_j4b4);
  if( file_tmp_j4b4.is_open() ){
    scoreF_j4b4 = TFile::Open(score_file_j4b4, "READ");
    scoreT_j4b4 = (TTree*) scoreF_j4b4->Get("tree");
    int nevt = scoreT_j4b4->GetEntries();
    if( nevt > 0){
      for(int i = 0; i < nevt; i++){
        scoreT_j4b4->GetEntry(i);
        float pt = scoreT_j4b4->GetLeaf("lepPt")->GetValue(0);
        float met = scoreT_j4b4->GetLeaf("missinget")->GetValue(0);
        lepPt_j4b4.push_back(pt);
        missET_j4b4.push_back(met);
      }
    }
  }
  else cout << "Not opened in j4b4 : " << option.Data() << " - " << file_tmp_path_j4b4 << endl;

//  const char* score_file = "";
//  score_file = Form("./scores/%s/%s%s/score_finalMVA_%s.root", era.c_str(), train_scheme.c_str(), syst_str.c_str(), sample.c_str());
//
//  string file_tmp_path = score_file;
//  ifstream file_tmp(file_tmp_path);
//  if( file_tmp.is_open() ){
//    scoreF = TFile::Open(score_file, "READ");
//    scoreT = (TTree*) scoreF->Get("tree");
//    int nevt = scoreT->GetEntries();
//    if( nevt > 0){
//      for(int i = 0; i < nevt; i++){
//        scoreT->GetEntry(i);
//        float pt = scoreT->GetLeaf("lepPt")->GetValue(0);
//        float met = scoreT->GetLeaf("missinget")->GetValue(0);
//        lepPt.push_back(pt);
//        missET.push_back(met);
//      }
//    }
//  }
//  else cout << option.Data() << endl;

  if( !dosyst and syst_ext.length() < 5 and !option.Contains("Run201") ) cout << sample.c_str() << " " << "No external systematic option!" << endl;


  //cout << "SlaveBegin" << endl;
  for( int ich=0; ich < 3; ich++ ){
    for( int syst = 0; syst != syst_num; ++syst ){
      if( syst > 0 and !dosyst ) continue;

//      h_MVA_b2[ich][syst] = new TH1D(Form("h_DNN_b2_Ch%i%s",ich,syst_name[syst]), "Final MVA b2", 40, -1, 1);
//      h_MVA_b2[ich][syst]->SetXTitle("DNN Score");
//      h_MVA_b2[ich][syst]->Sumw2();
//      fOutput->Add(h_MVA_b2[ich][syst]);
//
//      h_MVA_b3[ich][syst] = new TH1D(Form("h_DNN_b3_Ch%i%s",ich,syst_name[syst]), "Final MVA b3", 40, -1, 1);
//      h_MVA_b3[ich][syst]->SetXTitle("DNN Score");
//      h_MVA_b3[ich][syst]->Sumw2();
//      fOutput->Add(h_MVA_b3[ich][syst]);
//
//      h_MVA_b4[ich][syst] = new TH1D(Form("h_DNN_b4_Ch%i%s",ich,syst_name[syst]), "Final MVA b4", 40, -1, 1);
//      h_MVA_b4[ich][syst]->SetXTitle("DNN Score");
//      h_MVA_b4[ich][syst]->Sumw2();
//      fOutput->Add(h_MVA_b4[ich][syst]);

      h_MVA_j3b2[ich][syst] = new TH1D(Form("h_DNN_j3b2_Ch%i%s",ich,syst_name[syst]), "Final MVA j3b2", 40, -1, 1);
      h_MVA_j3b2[ich][syst]->SetXTitle("MVA Score");
      h_MVA_j3b2[ich][syst]->Sumw2();
      fOutput->Add(h_MVA_j3b2[ich][syst]);

      h_MVA_j3b3[ich][syst] = new TH1D(Form("h_DNN_j3b3_Ch%i%s",ich,syst_name[syst]), "Final MVA j3b3", 40, -1, 1);
      h_MVA_j3b3[ich][syst]->SetXTitle("MVA Score");
      h_MVA_j3b3[ich][syst]->Sumw2();
      fOutput->Add(h_MVA_j3b3[ich][syst]);

      h_MVA_j4b2[ich][syst] = new TH1D(Form("h_DNN_j4b2_Ch%i%s",ich,syst_name[syst]), "Final MVA j4b2", 40, -1, 1);
      h_MVA_j4b2[ich][syst]->SetXTitle("MVA Score");
      h_MVA_j4b2[ich][syst]->Sumw2();
      fOutput->Add(h_MVA_j4b2[ich][syst]);

      h_MVA_j4b3[ich][syst] = new TH1D(Form("h_DNN_j4b3_Ch%i%s",ich,syst_name[syst]), "Final MVA j4b3", 40, -1, 1);
      h_MVA_j4b3[ich][syst]->SetXTitle("MVA Score");
      h_MVA_j4b3[ich][syst]->Sumw2();
      fOutput->Add(h_MVA_j4b3[ich][syst]);

      h_MVA_j4b4[ich][syst] = new TH1D(Form("h_DNN_j4b4_Ch%i%s",ich,syst_name[syst]), "Final MVA j4b4", 40, -1, 1);
      h_MVA_j4b4[ich][syst]->SetXTitle("MVA Score");
      h_MVA_j4b4[ich][syst]->Sumw2();
      fOutput->Add(h_MVA_j4b4[ich][syst]);
    }

    for(int ijet=0; ijet<3; ijet++){
      bSFInfo[ich][ijet] = new TH1D(Form("bSFInfo_Ch%i_J%i",ich,ijet), "bSF info for normalization", 18, 0, 18);
      bSFInfo[ich][ijet]->SetXTitle("bSF Sum of weight");
      bSFInfo[ich][ijet]->Sumw2();
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(1,"Raw");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(2,"Nominal");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(3,"lfup");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(4,"lfdown");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(5,"hfup");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(6,"hfdown");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(7,"hfstat1up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(8,"hfstat1down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(9,"hfstat2up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(10,"hfstat2down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(11,"lfstat1up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(12,"lfstat1down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(13,"lfstat2up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(14,"lfstat2down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(15,"cferr1up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(16,"cferr1down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(17,"cferr2up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(18,"cferr2down");
      fOutput->Add(bSFInfo[ich][ijet]);
    }
  }
} 

Bool_t MyAnalysis::Process(Long64_t entry)
{
  fReader.SetEntry(entry);
  TString option = GetOption();
  string sample = option.Data();

  int era = 0;
  TString current_file_name = MyAnalysis::fReader.GetTree()->GetCurrentFile()->GetName();
  if     ( current_file_name.Contains("V9") ) era = 2017;
  else if( current_file_name.Contains("V10") ) era = 2018;

  int mode = 999; 
  mode = *channel;

  if( mode > 2 ) return kTRUE;

  float wrongPVrate = 1;
  if( era == 2017 ){
    if( !option.Contains("Run2017") ){
      if     ( option.Contains("DYJets10to50") ) wrongPVrate = 1.02921;
      else if( option.Contains("QCDEM15to20") ) wrongPVrate = 1.01333;
      else if( option.Contains("QCDEM20to30") ) wrongPVrate = 1.01227;
      else if( option.Contains("QCDEM300toInf") ) wrongPVrate = 1.01194;
      else if( option.Contains("QCDEM50to80") ) wrongPVrate = 1.02226;
      else if( option.Contains("QCDMu120to170") ) wrongPVrate = 1.01289;
      else if( option.Contains("QCDMu170to300") ) wrongPVrate = 1.01181;
      else if( option.Contains("QCDMu20to30") ) wrongPVrate = 1.0253;
      else if( option.Contains("QCDMu30to50") ) wrongPVrate = 1.02105;
      else if( option.Contains("QCDMu470to600") ) wrongPVrate = 1.0141;
      else if( option.Contains("QCDMu50to80") ) wrongPVrate = 1.01149;
      else if( option.Contains("QCDMu80to120") ) wrongPVrate = 1.01278;
      else if( option.Contains("TTLLpowhegttbbhdampup") ) wrongPVrate = 1.01807;
      else if( option.Contains("TTLLpowhegttcchdampup") ) wrongPVrate = 1.01978;
      else if( option.Contains("TTLLpowhegttlfhdampup") ) wrongPVrate = 1.01938;
      else if( option.Contains("TTZToLLNuNu") ) wrongPVrate = 1.02425;
      else if( option.Contains("TTpowhegttbbTuneCP5down") ) wrongPVrate = 1.02715;
      else if( option.Contains("TTpowhegttbbhdampdown") ) wrongPVrate = 1.02717;
      else if( option.Contains("TTpowhegttccTuneCP5down") ) wrongPVrate = 1.0273;
      else if( option.Contains("TTpowhegttcchdampdown") ) wrongPVrate = 1.02746;
      else if( option.Contains("TTpowhegttlfTuneCP5down") ) wrongPVrate = 1.02742;
      else if( option.Contains("TTpowhegttlfhdampdown") ) wrongPVrate = 1.02774;
      else if( option.Contains("W3JetsToLNu") ) wrongPVrate = 1.02348;
      else if( option.Contains("WW") ) wrongPVrate = 1.0295;
      else if( option.Contains("WZ") ) wrongPVrate = 1.02298;
      else if( option.Contains("ZZ") ) wrongPVrate = 1.01508;
      else   wrongPVrate = 1.0;
    }
    if( wrongPVrate > 1.01 ){
      if( *TruePV <=0 ) return kTRUE;
    }
  }
  float relIso = *lepton_relIso; 

  //Object selection
  int njets = 0;
  int nbjets_m = 0; 

  TLorentzVector p4met;
  float met = *MET;
  float met_phi = *MET_phi;
  float apt = TMath::Abs(met);
  float met_x = apt*TMath::Cos(met_phi);
  float met_y = apt*TMath::Sin(met_phi);
  //p4met.SetPxPyPzE( met_x, met_y, 0, met);

  TLorentzVector lepton;
  lepton.SetPtEtaPhiE(*lepton_pt, *lepton_eta, *lepton_phi, *lepton_e);
  //lepton = lepton*lepton_scale[0];

  //Event selection 
  bool passmuon = (mode == 0) && (lepton.Pt() > 30) && (abs(lepton.Eta()) <= 2.4);
  bool passelectron = (mode == 1) && (lepton.Pt() > 30) && (abs(lepton.Eta()) <= 2.4);

  //Single Lepton only
  if( option.Contains("SingleMuon") ){
    if( !passmuon ) return kTRUE;//RDMu
    if( passelectron) return kTRUE;//RDMu
  }
  else if( option.Contains("SingleElectron") ){
    if( !passelectron ) return kTRUE;//RDelec
    if( passmuon ) return kTRUE;//RDelec
  }
  else{
    if( !passmuon && !passelectron ) return kTRUE;
  }
  //if( passmuon || passelectron ){

  float bWP_M;
  if      ( era == 2017 ) bWP_M = 0.4941;
  else if ( era == 2018 ) bWP_M = 0.4184;
  else                    bWP_M = 0.0;

  int jetIdx[4];
  TLorentzVector jetP4s[4];

  for (unsigned int iJet = 0; iJet < jet_pt.GetSize() ; ++iJet) {

    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt[iJet], jet_eta[iJet], jet_phi[iJet], jet_e[iJet]);

    if( !option.Contains("Run201") ){
      if     ( syst_ext == "jecup" )   jet = jet * jet_JER_Nom[iJet] * jet_JES_Up[iJet];
      else if( syst_ext == "jecdown" ) jet = jet * jet_JER_Nom[iJet] * jet_JES_Down[iJet];
      else if( syst_ext == "jerup" )   jet = jet * jet_JER_Up[iJet];
      else if( syst_ext == "jerdown" ) jet = jet * jet_JER_Down[iJet];
      else                             jet = jet * jet_JER_Nom[iJet];
    }

    if( jet.Pt() > 30 && abs(jet.Eta())<=2.4){
      njets++;
      if( jet_deepCSV[iJet] > bWP_M ) nbjets_m++;
    }
  }

  if( !option.Contains("Run201") ){
    if( syst_ext == "jecup" ){
      met_x = MET_unc_x[0]; met_y = MET_unc_y[0];
    }
    else if( syst_ext == "jecdown" ){
      met_x = MET_unc_x[1]; met_y = MET_unc_y[1];
    }
    else if( syst_ext == "jerup" ){
      met_x = MET_unc_x[2]; met_y = MET_unc_y[2];
    }
    else if( syst_ext == "jerdown" ){
      met_x = MET_unc_x[3]; met_y = MET_unc_y[3];
    }
  }
  p4met.SetPxPyPzE(met_x, met_y, 0, sqrt(met_x*met_x + met_y*met_y));
  met = p4met.Pt();
  met_phi = p4met.Phi();

  //Selection Option
  float transverseM = transverseMass(lepton, p4met);
  float lepDphi = lepton.DeltaPhi(p4met);
  //bool isQCD = transverseM < 10 && met < 10 && lepDphi < 1;
  //bool makeIso = true;
  //bool isIso = *lepton_isIso;
  //if( makeIso && !isIso ) return kTRUE;
  //if( !makeIso && isIso ) return kTRUE;

  //Event selection is done here!
  if( njets < 3 ) return kTRUE;
  //if( nbjets_m < 2 or nbjets_m > 4 ) return kTRUE; commented out to compute sumW of bSF

  //Hard-coded, FIXME
  float tmp_score = -1;
  vector<double>::iterator iter;
  int evtIdx = 0;

  if( njets == 3 and nbjets_m == 2 ){
    if( !lepPt_j3b2.empty() ){
      for( iter = lepPt_j3b2.begin(); iter != lepPt_j3b2.end(); iter++ ){
        if( *iter == static_cast<float>(lepton.Pt()) ){
          int tmpIdx = distance(lepPt_j3b2.begin(), iter);
          if( missET_j3b2[tmpIdx] == met ) evtIdx = tmpIdx;
          else continue;
        }
      }
      dupCheck_j3b2.push_back(evtIdx);
      //cout << evtIdx << endl;
      //if( evtIdx == 0 ) cout << lepton.Pt() << " " << met << endl;
      scoreT_j3b2->GetEntry(evtIdx);
      tmp_score = scoreT_j3b2->GetLeaf("MLScore")->GetValue(0);
    }
  }
  else if( njets == 3 and nbjets_m == 3 ){
    if( !lepPt_j3b3.empty() ){
      for( iter = lepPt_j3b3.begin(); iter != lepPt_j3b3.end(); iter++ ){
        if( *iter == static_cast<float>(lepton.Pt()) ){
          int tmpIdx = distance(lepPt_j3b3.begin(), iter);
          if( missET_j3b3[tmpIdx] == met ) evtIdx = tmpIdx;
          else continue;
        }
      }
      dupCheck_j3b3.push_back(evtIdx);
      //cout << evtIdx << endl;
      //if( evtIdx == 0 ) cout << lepton.Pt() << " " << met << endl;
      scoreT_j3b3->GetEntry(evtIdx);
      tmp_score = scoreT_j3b3->GetLeaf("MLScore")->GetValue(0);
    }
  }
  else if( njets >= 4 and nbjets_m == 2 ){
    if( !lepPt_j4b2.empty() ){
      for( iter = lepPt_j4b2.begin(); iter != lepPt_j4b2.end(); iter++ ){
        if( *iter == static_cast<float>(lepton.Pt()) ){
          int tmpIdx = distance(lepPt_j4b2.begin(), iter);
          if( missET_j4b2[tmpIdx] == met ) evtIdx = tmpIdx;
          else continue;
        }
      }
      dupCheck_j4b2.push_back(evtIdx);
      //cout << evtIdx << endl;
      //if( evtIdx == 0 ) cout << lepton.Pt() << " " << met << endl;
      scoreT_j4b2->GetEntry(evtIdx);
      tmp_score = scoreT_j4b2->GetLeaf("MLScore")->GetValue(0);
    }
  }
  else if( njets >= 4 and nbjets_m == 3 ){
    if( !lepPt_j4b3.empty() ){
      for( iter = lepPt_j4b3.begin(); iter != lepPt_j4b3.end(); iter++ ){
        if( *iter == static_cast<float>(lepton.Pt()) ){
          int tmpIdx = distance(lepPt_j4b3.begin(), iter);
          if( missET_j4b3[tmpIdx] == met ) evtIdx = tmpIdx;
          else continue;
        }
      }
      dupCheck_j4b3.push_back(evtIdx);
      //cout << evtIdx << endl;
      //if( evtIdx == 0 ) cout << lepton.Pt() << " " << met << endl;
      scoreT_j4b3->GetEntry(evtIdx);
      tmp_score = scoreT_j4b3->GetLeaf("MLScore")->GetValue(0);
    }
  }
  else if( njets >= 4 and nbjets_m == 4 ){
    if( !lepPt_j4b4.empty() ){
      for( iter = lepPt_j4b4.begin(); iter != lepPt_j4b4.end(); iter++ ){
        if( *iter == static_cast<float>(lepton.Pt()) ){
          int tmpIdx = distance(lepPt_j4b4.begin(), iter);
          if( missET_j4b4[tmpIdx] == met ) evtIdx = tmpIdx;
          else continue;
        }
      }
      dupCheck_j4b4.push_back(evtIdx);
      //cout << evtIdx << endl;
      //if( evtIdx == 0 ) cout << lepton.Pt() << " " << met << endl;
      scoreT_j4b4->GetEntry(evtIdx);
      tmp_score = scoreT_j4b4->GetLeaf("MLScore")->GetValue(0);
    }
  }

//  float tmp_score = -1;
//  vector<double>::iterator iter;
//  int evtIdx = 0;
//  if( !lepPt.empty() ){
//    for( iter = lepPt.begin(); iter != lepPt.end(); iter++ ){
//      if( *iter == static_cast<float>(lepton.Pt()) ){
//        int tmpIdx = distance(lepPt.begin(), iter);
//        if( missET[tmpIdx] == met ) evtIdx = tmpIdx;
//        else continue;
//      }
//    }
//    dupCheck.push_back(evtIdx);
//    //cout << evtIdx << endl;
//    //if( evtIdx == 0 ) cout << lepton.Pt() << " " << met << endl;
//
//    scoreT->GetEntry(evtIdx);
//    tmp_score = scoreT->GetLeaf("MLScore")->GetValue(0);
//  }


  /////Fill histograms
  //int modeArray[2] = {mode, 2};
  int modeArray[1] = {mode};
  int jetmode = 0; //for b SF normalization, 0 is useless in mva hist
  if     ( njets == 3 ) jetmode = 1;
  else if( njets >= 4 ) jetmode = 2;

  for( int MODE : modeArray ){
    for( int syst = 0; syst != syst_num; ++syst ){
      if( syst > 0 and !dosyst ) continue;

      float EventWeight = 1;
      //Multiply syst. to event weight
      if( !option.Contains("Run201") ){
        EventWeight *= wrongPVrate;
        EventWeight *= *genweight;
        if( era == 2017 ){
          //PrefireWeight
          if     ( isPartOf("prefireup", std::string(syst_name[syst])) )   EventWeight *= prefireweight[1];
          else if( isPartOf("prefiredown", std::string(syst_name[syst])) ) EventWeight *= prefireweight[2];
          else   EventWeight *= prefireweight[0];
        }
        //if( option.Contains("owheg") ) EventWeight *= *topptweight; //for now, apply only for ttbar
        //PUWight
        if     ( isPartOf("puup", std::string(syst_name[syst])) )   EventWeight *= PUWeight[1];
        else if( isPartOf("pudown", std::string(syst_name[syst])) ) EventWeight *= PUWeight[2];
        else   EventWeight *= PUWeight[0];
        //leptonSF
        if( passmuon ){
          //mu ID: 0, mu Iso: 1, mu Trg: 2
          //if     ( isPartOf("__muidup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[1];
          //else if( isPartOf("__muiddown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[2];
          //else   EventWeight *= lepton_SF[0];
          //if     ( isPartOf("__muisoup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[4];
          //else if( isPartOf("__muisodown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[5];
          //else   EventWeight *= lepton_SF[3];
          //if     ( isPartOf("__mutrgup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[7];
          //else if( isPartOf("__mutrgdown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[8];
          //else   EventWeight *= lepton_SF[6];
          if     ( isPartOf("__muidup", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[0] + sqrt(pow(lepton_SF[1] - lepton_SF[0], 2) + pow(0.005, 2));
          else if( isPartOf("__muiddown", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[0] - sqrt(pow(lepton_SF[2] - lepton_SF[0], 2) + pow(0.005, 2));
          else   EventWeight *= lepton_SF[0];
          if     ( isPartOf("__muisoup", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[3] + sqrt(pow(lepton_SF[4] - lepton_SF[3], 2) + pow(0.005, 2));
          else if( isPartOf("__muisodown", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[3] - sqrt(pow(lepton_SF[5] - lepton_SF[3], 2) + pow(0.005, 2));
          else   EventWeight *= lepton_SF[3];
          if     ( isPartOf("__mutrgup", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[6] + sqrt(pow(lepton_SF[7] - lepton_SF[6], 2) + pow(0.005, 2));
          else if( isPartOf("__mutrgdown", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[6] - sqrt(pow(lepton_SF[8] - lepton_SF[6], 2) + pow(0.005, 2));
          else   EventWeight *= lepton_SF[6];
          if     ( isPartOf("__elidup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__eliddown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__elrecoup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__elrecodown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__elzvtxup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__elzvtxdown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__eltrgup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__eltrgdown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
        }
        else if( passelectron ){
          //elec ID: 0, elec Reco: 1, elec Zvtx: 2
          if     ( isPartOf("__muidup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__muiddown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__muisoup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__muisodown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__mutrgup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__mutrgdown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__elidup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[1];
          else if( isPartOf("__eliddown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[2];
          else   EventWeight *= lepton_SF[0];
          if     ( isPartOf("__elrecoup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[4];
          else if( isPartOf("__elrecodown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[5];
          else   EventWeight *= lepton_SF[3];
          if     ( isPartOf("__elzvtxup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[7];
          else if( isPartOf("__elzvtxdown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[8];
          else   EventWeight *= lepton_SF[6];
          if     ( isPartOf("__eltrgup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[10];
          else if( isPartOf("__eltrgdown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[11];
          else   EventWeight *= lepton_SF[9];
        }
        //ME&PS
        //[0] = muF up , [1] = muF down, [2] = muR up, [3] = muR up && muF up, [4] = muR down, [5] = muF down && muF down
        if( option.Contains("TTpowheg") or option.Contains("TTLL") or option.Contains("TTHad") ){
          //Scale weight
          if     ( isPartOf("scale0", std::string(syst_name[syst])) ) EventWeight *= scaleweight[0];
          else if( isPartOf("scale1", std::string(syst_name[syst])) ) EventWeight *= scaleweight[1];
          else if( isPartOf("scale2", std::string(syst_name[syst])) ) EventWeight *= scaleweight[2];
          else if( isPartOf("scale3", std::string(syst_name[syst])) ) EventWeight *= scaleweight[3];
          else if( isPartOf("scale4", std::string(syst_name[syst])) ) EventWeight *= scaleweight[4];
          else if( isPartOf("scale5", std::string(syst_name[syst])) ) EventWeight *= scaleweight[5];
          else   EventWeight *= 1;
          //PS weight
          if     ( isPartOf("ps0", std::string(syst_name[syst])) ) EventWeight *= psweight[0];//isr down
          else if( isPartOf("ps1", std::string(syst_name[syst])) ) EventWeight *= psweight[1];//fsr down
          else if( isPartOf("ps2", std::string(syst_name[syst])) ) EventWeight *= psweight[2];//isr up
          else if( isPartOf("ps3", std::string(syst_name[syst])) ) EventWeight *= psweight[3];//fsr up
          else   EventWeight *= 1;
          //PDF uncdertainty
          if( isPartOf("pdf", std::string(syst_name[syst])) ){
            string tmp_name = string(syst_name[syst]);
            int pdf_unc_idx = std::stoi(tmp_name.erase(0,5));
            if( pdf_unc_idx > 103 or pdf_unc_idx < 0 ){
              cout << "Wrong pdf unc index!" << endl;
              continue;
            }
            EventWeight *= pdfweight[pdf_unc_idx];
          }
        }
        else EventWeight *= 1;
        //Deep CSV shape
        float bSF = 1.0;
        if     ( isPartOf("lfup",        std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[3];
          bSF = jet_SF_deepCSV_30[3];
          bSFInfo[MODE][jetmode]->Fill(2.5, EventWeight*bSF);
        }
        else if( isPartOf("lfdown",      std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[4];
          bSF = jet_SF_deepCSV_30[4];
          bSFInfo[MODE][jetmode]->Fill(3.5, EventWeight*bSF);
        }
        else if( isPartOf("hfup",        std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[5];
          bSF = jet_SF_deepCSV_30[5];
          bSFInfo[MODE][jetmode]->Fill(4.5, EventWeight*bSF);
        }
        else if( isPartOf("hfdown",      std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[6];
          bSF = jet_SF_deepCSV_30[6];
          bSFInfo[MODE][jetmode]->Fill(5.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat1up",   std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[7];
          bSF = jet_SF_deepCSV_30[7];
          bSFInfo[MODE][jetmode]->Fill(6.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat1down", std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[8];
          bSF = jet_SF_deepCSV_30[8];
          bSFInfo[MODE][jetmode]->Fill(7.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat2up",   std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[9];
          bSF = jet_SF_deepCSV_30[9];
          bSFInfo[MODE][jetmode]->Fill(8.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat2down", std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[10];
          bSF = jet_SF_deepCSV_30[10];
          bSFInfo[MODE][jetmode]->Fill(9.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat1up",   std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[11];
          bSF = jet_SF_deepCSV_30[11];
          bSFInfo[MODE][jetmode]->Fill(10.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat1down", std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[12];
          bSF = jet_SF_deepCSV_30[12];
          bSFInfo[MODE][jetmode]->Fill(11.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat2up",   std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[13];
          bSF = jet_SF_deepCSV_30[13];
          bSFInfo[MODE][jetmode]->Fill(12.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat2down", std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[14];
          bSF = jet_SF_deepCSV_30[14];
          bSFInfo[MODE][jetmode]->Fill(13.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr1up",    std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[15];
          bSF = jet_SF_deepCSV_30[15];
          bSFInfo[MODE][jetmode]->Fill(14.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr1down",  std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[16];
          bSF = jet_SF_deepCSV_30[16];
          bSFInfo[MODE][jetmode]->Fill(15.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr2up",    std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]+jet_SF_deepCSV_30[17];
          bSF = jet_SF_deepCSV_30[17];
          bSFInfo[MODE][jetmode]->Fill(16.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr2down",  std::string(syst_name[syst])) ){
          //bSF = jet_SF_deepCSV_30[0]-jet_SF_deepCSV_30[18];
          bSF = jet_SF_deepCSV_30[18];
          bSFInfo[MODE][jetmode]->Fill(17.5, EventWeight*bSF);
        }
        else{
          bSF = jet_SF_deepCSV_30[0];
          if( !isPartOf("__", std::string(syst_name[syst])) ){
            bSFInfo[MODE][jetmode]->Fill(0.5, EventWeight);
            bSFInfo[MODE][jetmode]->Fill(1.5, EventWeight*bSF);
          }
        }
        EventWeight *= bSF;
      }

      if( nbjets_m < 2 or nbjets_m > 4 ) continue;
      if     ( njets == 3 and nbjets_m == 2 ) h_MVA_j3b2[MODE][syst]->Fill(tmp_score,EventWeight);
      else if( njets == 3 and nbjets_m == 3 ) h_MVA_j3b3[MODE][syst]->Fill(tmp_score,EventWeight);
      else if( njets >= 4 and nbjets_m == 2 ) h_MVA_j4b2[MODE][syst]->Fill(tmp_score,EventWeight);
      else if( njets >= 4 and nbjets_m == 3 ) h_MVA_j4b3[MODE][syst]->Fill(tmp_score,EventWeight);
      else if( njets >= 4 and nbjets_m == 4 ) h_MVA_j4b4[MODE][syst]->Fill(tmp_score,EventWeight);
      else cout << "Wrong jet multiplicity" << endl;

    }//syst loop
  }//mode loop : e or mu + emu
  evtNum++;
  //cout << evtNum << '\r';

  return kTRUE;
}

void MyAnalysis::SlaveTerminate()
{
  TString option = GetOption();
}
   

void MyAnalysis::Terminate()
{
  TString option = GetOption();
  string sample = option.Data();
  string era =  sample.substr(sample.find_first_of("-")+1,4);
  string train_scheme = sample.substr(sample.find_first_of("-")+5,string::npos);
  sample.erase(sample.find_first_of("-"),string::npos);
  string sig_ch = train_scheme.substr(0,3);
  string ver_j3b2 = train_scheme.substr(train_scheme.find_first_of("_")+1,2);
  string ver_j3b3 = train_scheme.substr(train_scheme.find_first_of("_")+3,2);
  string ver_j4b2 = train_scheme.substr(train_scheme.find_first_of("_")+5,2);
  string ver_j4b3 = train_scheme.substr(train_scheme.find_first_of("_")+7,2);
  string ver_j4b4 = train_scheme.substr(train_scheme.find_first_of("_")+9,2);

  dosyst = true;
  if( option.Contains("Run201") ) dosyst = false;

  syst_ext = "";
  if( train_scheme.find("jec") != string::npos or train_scheme.find("jer") != string::npos
    or train_scheme.find("TuneCP5") != string::npos or train_scheme.find("hdamp") != string::npos ){
    if     ( train_scheme.find("jecup") != string::npos )       syst_ext = "__jecup";
    else if( train_scheme.find("jecdown") != string::npos )     syst_ext = "__jecdown";
    else if( train_scheme.find("jerup") != string::npos )       syst_ext = "__jerup";
    else if( train_scheme.find("jerdown") != string::npos )     syst_ext = "__jerdown";
    else if( train_scheme.find("TuneCP5up") != string::npos )   syst_ext = "__TuneCP5up";
    else if( train_scheme.find("TuneCP5down") != string::npos ) syst_ext = "__TuneCP5down";
    else if( train_scheme.find("hdampup") != string::npos )     syst_ext = "__hdampup";
    else if( train_scheme.find("hdampdown") != string::npos )   syst_ext = "__hdampdown";
  }
  string syst_str = "-" + train_scheme.substr(train_scheme.find_last_of("_")+1,string::npos );
  if( syst_ext == "" ) syst_str.erase(0,1);
  train_scheme.erase( train_scheme.find_last_of("_"),string::npos );

//  const char* score_file = "";
//  score_file = Form("scores/%s/%s%s/score_finalMVA_%s.root", era.c_str(), train_scheme.c_str(), syst_str.c_str(), sample.c_str());
//  string file_tmp_path = score_file;
//  ifstream file_tmp(file_tmp_path);
//
//  if(file_tmp.is_open()){
//    //cout << lepcount << endl;
//    auto it = unique( dupCheck.begin(), dupCheck.end() );
//    if( it != dupCheck.end() ) cout << string(option.Data()) + string(": Duplicate(s)") << endl;
//  }
//  else cout << "file " + file_tmp_path + " not opened!" << endl;
  out = TFile::Open(Form("./histos/temp/hist_%s%s.root", sample.c_str(), syst_ext.c_str()),"RECREATE");

  TList * l = GetOutputList();
  TIter next(l);
  TObject *object = 0;
  while( ( object = next()) ){
    const char * name = object->GetName();
    std::string str(name);

    if(str.find("Ch2") != std::string::npos ){
      std::string strMuon = boost::replace_all_copy(str, "Ch2", "Ch0");
      std::string strElec = boost::replace_all_copy(str, "Ch2", "Ch1");
      TList *tmp = new TList;
      if( object->InheritsFrom(TH1D::Class()) ){
        auto htmp = dynamic_cast<TH1D *>(object);
        tmp->Add((TH1D *)fOutput->FindObject(strMuon.c_str()));
        tmp->Add((TH1D *)fOutput->FindObject(strElec.c_str()));
        htmp->Merge(tmp);
      }
    }

    if (str.find("h_") !=std::string::npos or str.find("Info") !=std::string::npos){
      object->Write();
    }
  }

  out->Write();
  out->Close();
}

float MyAnalysis::transverseMass( const TLorentzVector & lepton, const TLorentzVector & met){

  TLorentzVector leptonT(lepton.Px(),lepton.Py(),0.,lepton.E()*TMath::Sin(lepton.Theta()));
  TLorentzVector metT(met.Px(), met.Py(), 0, met.E());

  TLorentzVector sumT=leptonT+metT;
  float out = sumT.M();

  return out;

}

bool MyAnalysis::isPartOf(const std::string& word, const std::string& sentence) {
    return sentence.find(word)    // this returns the index of the first instance
                                  // word
           != std::string::npos;  // which will take this value if it's not found
}
