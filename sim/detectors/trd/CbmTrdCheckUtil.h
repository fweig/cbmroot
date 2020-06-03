#ifndef CBMTRDCHECKUTIL_H
#define CBMTRDCHECKUTIL_H

#include "CbmTrdDigi.h"
#include<map>
#include<vector>
#include<string>

#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TProfile3D.h>
#include <TFile.h>
#include <FairRootManager.h>



class CbmTrdCheckUtil : public TObject
{


 public:

  /** @brief default Constructor with messages
   **/
  CbmTrdCheckUtil();

  CbmTrdCheckUtil(std::string readfile);

  /** @brief Constructor with messages and preset reconstruction mode
   **/
  CbmTrdCheckUtil(Double_t cal,Double_t tau,Int_t mode);

    /** @brief Constructor with messages and selection mask
   **/
  CbmTrdCheckUtil(Double_t cal,Double_t tau,std::vector<Int_t> mask);
  
  /** @brief Destructor **/
  virtual ~CbmTrdCheckUtil() {;}    

  //  CbmTrdDigi* MakeDigi(CbmSpadicRawMessage22* raw);
  CbmTrdDigi* MakeDigi(std::vector<Int_t> samples, Int_t channel,Int_t module,Int_t layer,ULong64_t time);

  Bool_t fSet=false;                       // Boolean for module initialisation in simulation
  std::map<TString, TH1D *>                 f1D;
  std::map<TString, TH2D *>                 f2D;
  std::map<TString, TH3D *>                 f3D;
  std::map<TString, TProfile *>                 fProfile1D;
  std::map<TString, TProfile2D *>               fProfile2D;
  std::map<TString, TProfile3D *>               fProfile3D;
  
  void SetSetter(Bool_t set)                {fSet=set;}
  void DumpPlots(){    
    TDirectory * oldir = gDirectory;
    TFile* outFile = FairRootManager::Instance()->GetOutFile();
    if (outFile != NULL) {
      outFile->cd();
      if(!f1D.empty())      for(auto const& x : f1D)   if(x.second) x.second->Write(x.first);
      if(!f2D.empty())      for(auto const& x : f2D)   if(x.second) x.second->Write(x.first);
      if(!f3D.empty())      for(auto const& x : f3D)   if(x.second) x.second->Write(x.first); 
      if(!fProfile1D.empty())     for(auto const& x : fProfile1D)   if(x.second) x.second->Write(x.first);
      if(!fProfile2D.empty())     for(auto const& x : fProfile2D)   if(x.second) x.second->Write(x.first);
      if(!fProfile3D.empty())     for(auto const& x : fProfile3D)   if(x.second) x.second->Write(x.first); 
    }
    gDirectory->cd( oldir->GetPath() );
  }
  
  void CreateHist(std::string name,Int_t xbins,Double_t xlow,Double_t xhigh,Int_t ybins=0,Double_t ylow=1.,Double_t yhigh=1.);
  void CreateHist3D(std::string name,Int_t xbins,Double_t xlow,Double_t xhigh,Int_t ybins,Double_t ylow,Double_t yhigh,Int_t zbins,Double_t zlow,Double_t zhigh);
  void CreateProfile(std::string name,Int_t xbins,Double_t xlow,Double_t xhigh,Int_t ybins=0,Double_t ylow=1.,Double_t yhigh=1.);
  void CreateProfile3D(std::string name,Int_t xbins,Double_t xlow,Double_t xhigh,Int_t ybins,Double_t ylow,Double_t yhigh,Int_t zbins,Double_t zlow,Double_t zhigh);
  void Fill(std::string name,Double_t x, Double_t y=9999.);
  //  void FillProfile(std::string name,Double_t x,Double_t y=9999.);
  void FillProfile(std::string name,Double_t x,Double_t y,Double_t z=9999.);
  void FillProfile3D(std::string name,Double_t x,Double_t y,Double_t z,Double_t w=1.);
  void Fill(std::string name,Double_t x, Double_t y,Double_t z);
  void Fill3D(std::string name,Double_t x, Double_t y,Double_t z);
  void FillW(std::string name,Double_t x, Double_t w);
  Double_t GetCont2D(std::string name,Double_t x, Double_t y) {if(!f2D[name]) return 0.;return f2D[name]->GetBinContent(x,y);}
  Bool_t GetSetter()                {return fSet;}
  
 private:
  CbmTrdCheckUtil(const CbmTrdCheckUtil&);
  CbmTrdCheckUtil operator=(const CbmTrdCheckUtil&);

  
  ClassDef(CbmTrdCheckUtil, 1);  
};

#endif
