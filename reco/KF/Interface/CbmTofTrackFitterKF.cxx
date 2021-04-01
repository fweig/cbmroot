// -----------------------------------------------------------------------
// -----                  CbmTofTrackFitterKF                        -----
// -----             Created 29/11/05  by D. Kresan                  -----
// -----------------------------------------------------------------------
#include "CbmTofTrackFitterKF.h"

#include "CbmKFTofHit.h"
#include "CbmKFTrack.h"
#include "CbmTofHit.h"
#include "CbmTofTracklet.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

//________________________________________________________________________
//
// CbmTofTrackFitterKF
//
// Concrete implementation of the TOF track fitter based on Kalman filter
//

// -----------------------------------------------------------------------
CbmTofTrackFitterKF::CbmTofTrackFitterKF()
  : fArrayTofHit(0), fVerbose(1), fPid(211), fKfTrack(0) {
  fKfTrack = new CbmKFTrack();
}
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
CbmTofTrackFitterKF::CbmTofTrackFitterKF(Int_t verbose, Int_t pid)
  : fArrayTofHit(0), fVerbose(verbose), fPid(pid), fKfTrack(0) {
  // Standard constructor
  fArrayTofHit = NULL;
  fVerbose     = verbose;
  fPid         = pid;
  fKfTrack     = new CbmKFTrack();
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
CbmTofTrackFitterKF::~CbmTofTrackFitterKF() {
  // Destructor
  delete fKfTrack;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
void CbmTofTrackFitterKF::Init() {
  // Initialisation

  // Get the pointer to FairRootManager
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (NULL == rootMgr) {
    cout << "-E- CbmTofTrackFitterKF::Init : "
         << " ROOT manager is not instantiated!" << endl;
    return;
  }

  // Activate data branches
  fArrayTofHit = (TClonesArray*) rootMgr->GetObject("TofHit");
  if (NULL == fArrayTofHit) {
    cout << "-E- CbmTofTrackFitterKF::Init : "
         << " no TOF hit array!" << endl;
    return;
  }
  //    cout << Form("<D> TrackFitterKF::Init: ArrayTofHit at 0x%08x ",fArrayTofHit) <<endl;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
Int_t CbmTofTrackFitterKF::DoFit(CbmTofTracklet* pTrack) {
  LOG(debug1) << "CbmTofTrackFitterKF::DoFit starting ";
  // Implementation of the fitting algorithm
  if (NULL == fArrayTofHit) this->Init();

  if (NULL == fArrayTofHit) {
    LOG(error) << "CbmTofTrackFitterKF::DoFit No fArrayTofHit ";
    return 1;
  }
  // Declare variables outside the loop
  CbmTofHit* pHit     = NULL;
  CbmKFTofHit* pKFHit = NULL;
  Int_t hitIndex      = 0;
  Int_t materialIndex = 0;

  // Loop over TOF hits of the track
  for (Int_t iHit = 0; iHit < pTrack->GetNofHits(); iHit++) {
    // Get current hit index
    hitIndex = pTrack->GetHitIndex(iHit);
    //Get the pointer to the CbmTofHit
    pHit = (CbmTofHit*) fArrayTofHit->At(hitIndex);
    if (NULL == pHit) {
      cout << "-E- CbmTofTrackFitterKF::DoFit : "
           << " empty TOF hit : " << hitIndex << ", " << pHit << endl;
      return 1;
    }

    // Create CbmKFTofHit
    pKFHit = new CbmKFTofHit();
    pKFHit->Create(pHit);
    materialIndex = pKFHit->MaterialIndex;
    // Add to the KFTrack
    fKfTrack->fHits.push_back(pKFHit);

    LOG(debug) << "   TOF hit : (" << pHit->GetX() << ", " << pHit->GetY()
               << ", " << pHit->GetZ() << ") "
               << " is added to track. matidx=" << materialIndex;

  }                              // Loop over TOF hits end
  Double_t qpini         = 0.1;  // FIXME: avoid constants in code
  fKfTrack->GetRefChi2() = 0.;
  fKfTrack->GetRefNDF()  = 0;
  FairTrackParam* pParam = (FairTrackParam*) (pTrack->GetParamLast());
  pParam->SetQp(qpini);
  fKfTrack->SetTrackParam(
    *(const_cast<FairTrackParam*>(pTrack->GetParamLast())));
  fKfTrack->SetPID(fPid);
  /*
    fKfTrack->Fit(0);  // what does this do???, up/downstream flag 
    fKfTrack->Fit(1);
    */
  fKfTrack->Fit(0);
  // Store parameters at first layer
  fKfTrack->GetTrackParam(
    *(const_cast<FairTrackParam*>(pTrack->GetParamFirst())));
  if (fVerbose > 2) { pTrack->GetParamFirst()->Print(); }
  fKfTrack->Fit(1);
  // Store parameters at last layer
  fKfTrack->GetTrackParam(
    *(const_cast<FairTrackParam*>(pTrack->GetParamLast())));
  if (fVerbose > 2) { pTrack->GetParamLast()->Print(); }

  // Store chi2 of fit
  pTrack->SetChiSq(fKfTrack->GetRefChi2());
  pTrack->SetNDF(fKfTrack->GetRefNDF());

  // Delete CbmKFTofHit objects
  vector<CbmKFHit*>::iterator it;
  for (it = fKfTrack->fHits.begin(); it != fKfTrack->fHits.end(); it++) {
    pKFHit = (CbmKFTofHit*) (*it);
    delete pKFHit;
  }
  fKfTrack->fHits.clear();

  LOG(debug) << "CbmTofTrackFitterKF::DoFit: TOF track fitted. chi2/ndf = "
             << pTrack->GetChiSq() / pTrack->GetNDF();
  if (fVerbose > 2) { cout << endl << endl; }

  return 0;
}
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
Int_t CbmTofTrackFitterKF::DoFit(CbmTofTrack* /*pTrack*/) { return 0; }

void CbmTofTrackFitterKF::Extrapolate(const FairTrackParam* track,
                                      Double_t z,
                                      FairTrackParam* e_track) {
  if (!track) return;
  CbmKFTrack T;
  T.SetTrackParam(*track);
  T.Extrapolate(z);
  if (e_track) T.GetTrackParam(*e_track);
}

/*
void CbmTofTrackFitterKF::Extrapolate( CbmTofTracklet* track, Double_t z, FairTrackParam* e_track )
{
  if( !track ) return;
  CbmKFTrack T;
  T.SetPID( track->GetPidHypo() );
  const FairTrackParam *fpar = track->GetParamFirst(), *lpar = track->GetParamLast();
  
  if( z<=fpar->GetZ() ){ // extrapolate first parameters
    T.SetTrackParam( *fpar );
    T.Extrapolate( z );
  }else   if( z<fpar->GetZ()+0.1 ){ // extrapolate first parameters
    T.SetTrackParam( *fpar );
    T.Propagate( z );
  }else if( lpar->GetZ()<=z ){ // extrapolate last parameters
    T.SetTrackParam( *lpar );
    T.Extrapolate( z );
  }else if( lpar->GetZ()-0.1<z ){ // extrapolate last parameters
    T.SetTrackParam( *lpar );
    T.Propagate( z );
  }else { // refit with smoother
    SetKFHits( T, track);
    T.SetTrackParam( *fpar );
    T.Smooth( z );
  }
  if( e_track ) T.GetTrackParam( *e_track );
}
*/

void CbmTofTrackFitterKF::SetKFHits(CbmKFTrack& T, CbmTofTracklet* /*track*/) {

  T.fHits.clear();
  /*
  Int_t NTofHits   = ( fArrayTofHit ) ?track->GetNofHits()   :0;

  fHits.resize( NTofHits );
 
  if( NTofHits>0 && fArrayTofHit ){  
    for (Int_t i=0; i<NTofHits;i++){
      Int_t j = track->GetHitIndex(i);
      fHits[i].Create( reinterpret_cast<CbmTofHit*>(fArrayTofHit->At(j)) );
      T.fHits.push_back(&(fHits[i]));
    }
  }
  */
}

ClassImp(CbmTofTrackFitterKF);
