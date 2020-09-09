// -------------------------------------------------------------------------
// -----                CbmPsdHitProducer source file             -----
// -----                  Created 15/05/12  by     Alla & SELIM               -----
// -------------------------------------------------------------------------
#include <fstream>
#include <iostream>
#include <map>

#include "TClonesArray.h"
#include "TMath.h"
#include <TFile.h>

#include "FairRootManager.h"

#include "CbmDigiManager.h"
#include "CbmPsdDigi.h"
#include "CbmPsdHit.h"
#include "CbmPsdHitProducer.h"

#include <Logger.h>

using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
CbmPsdHitProducer::CbmPsdHitProducer()
  : FairTask("Ideal Psd Hit Producer", 1)
  , fNHits(0)
  , fHitArray(NULL)
  , fDigiMan(nullptr)
  , fXi()
  , fYi()
  , fhModXNewEn(NULL) {
  //  Reset();
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmPsdHitProducer::~CbmPsdHitProducer() {
  if (fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }
}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
InitStatus CbmPsdHitProducer::Init() {

  //ifstream fxypos("psd_geo_xy.txt");
  //for (Int_t ii=1; ii<50; ii++) { //SELIM: 49 modules, including central & corner modules (rejected in analysis/flow/eventPlane.cxx)
  //    fxypos>>fXi[ii]>>fYi[ii];
  //    cout<<ii<<" "<<fXi[ii]<<" "<<fYi[ii]<<endl;
  //}
  //fxypos.close();

  fhModXNewEn = new TH1F("hModXNewEn", "X distr, En", 300, -150., 150.);
  fhModXNewEn->Print();

  // Get RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    LOG(fatal)
      << "-W- CbmPsdHitProducer::Init: RootManager not instantised!";  //FLORIAN & SELIM
    return kFATAL;
  }

  // --- Initialise digi manager
  CbmDigiManager* digiMan = CbmDigiManager::Instance();
  digiMan->Init();
  // --- Check input branch (PsdDigi). If not present, set task inactive.
  if (!digiMan->IsPresent(ECbmModuleId::kPsd)) {
    LOG(error) << GetName() << ": No PsdDigi input array present; "
               << "task will be inactive.";
    return kERROR;
  }


  // Create and register output array
  fHitArray = new TClonesArray("CbmPsdHit", 1000);
  ioman->Register(
    "PsdHit", "PSD", fHitArray, IsOutputBranchPersistent("PsdHit"));

  fHitArray->Dump();
  cout << "-I- CbmPsdHitProducer: Intialisation successfull " << kSUCCESS
       << endl;
  return kSUCCESS;
}


// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmPsdHitProducer::Exec(Option_t* /*opt*/) {

  cout << " CbmPsdHitProducer::Exec(Option_t* /*opt*/) " << endl;
  fhModXNewEn->Print();

  // Reset output array
  Reset();

  // Declare some variables
  const CbmPsdDigi* dig = NULL;

  /*
  Double_t edep[NPsdMod];//marina
  for (Int_t imod=0; imod<NPsdMod; imod++)  { edep[imod]=0.; }//marina
*/

  std::map<int, Double_t> edepmap;

  // Loop over PsdDigits
  Int_t nDigi = fDigiMan->GetNofDigis(ECbmModuleId::kPsd);
  cout << " nDigits " << nDigi << endl;

  for (Int_t idig = 0; idig < nDigi; idig++) {
    dig = fDigiMan->Get<CbmPsdDigi>(idig);
    if (!dig) continue;
    Int_t mod = dig->GetModuleID();
    //    Int_t sec = dig->GetSectionID();
    Double_t eDep = (Double_t) dig->GetEdep();
    //edep[mod-1] += (Double_t) dig->GetEdep();                     //DEBUG: SELIM

    auto insert_result = edepmap.insert(std::make_pair(mod, eDep));
    if (!insert_result.second) {  // entry was here before
      (*insert_result.first).second += eDep;
    }

  }  // Loop over MCPoints

  fNHits = 0;
  for (auto edep_entry : edepmap) {
    int modID     = edep_entry.first;
    Double_t eDep = edep_entry.second;
    new ((*fHitArray)[fNHits]) CbmPsdHit(modID, eDep);
    fNHits++;
  }

  /*
  for (Int_t imod=0; imod<NPsdMod; imod++) //marina
  {
    if (edep[imod]>0.)
    {
      new ((*fHitArray)[fNHits]) CbmPsdHit(imod+1, edep[imod]);
      fNHits++;
      //cout<<"MARINA CbmPsdHitProducer " <<fNHits <<" " <<imod+1 <<" " <<edep[imod] <<endl;
      //fhModXNewEn->Fill(fXi[imod],TMath::Sqrt(edep[imod]) );
      //cout<<"CbmPsdHitProducer "<<fNHits<<" "<<imod<<" "<<edep[imod]<<endl;
    }
  }
  */

  // Event summary
  cout << "-I- CbmPsdHitProducer: " << fNHits << " CbmPsdHits created." << endl;
}
// -------------------------------------------------------------------------
void CbmPsdHitProducer::Finish() {
  cout << " CbmPsdHitProducer::Finish() " << endl;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* outfile = new TFile("EdepHistos.root", "RECREATE");
  outfile->cd();
  fhModXNewEn->Write();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  //outfile->Close();                     //SELIM
}

// -----   Private method Reset   ------------------------------------------
void CbmPsdHitProducer::Reset() {
  fNHits = 0;
  if (fHitArray) fHitArray->Delete();
}


ClassImp(CbmPsdHitProducer)
