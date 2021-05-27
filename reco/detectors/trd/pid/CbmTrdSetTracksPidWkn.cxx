
// -------------------------------------------------------------------------
// -----                    CbmTrdSetTracksPidWkn source file          -----
// -----                  Created 13/02/07 by F.Uhlig                  -----
//------           Last modification 01/07/18 by O.Derenovskaya
// -------------------------------------------------------------------------
#include "CbmTrdSetTracksPidWkn.h"

#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"

#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TMath.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
CbmTrdSetTracksPidWkn::CbmTrdSetTracksPidWkn() : CbmTrdSetTracksPidWkn("TrdSetTracksPidWkn", "") {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTrdSetTracksPidWkn::CbmTrdSetTracksPidWkn(const char* name, const char*)
  : FairTask(name)
  , fnSet(0)
  , fdegWkn(0)
  , fNofTracks(0)
  , fk1(0)
  , fwkn0(0)
  , fEmp(0)
  , fXi(0)
  , fWmin(0)
  , fWmax(0)
  , fDiff(0)
  , fSISType("sis100")
  , fTrackArray(NULL)
  , fTrdHitArray(NULL)


{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTrdSetTracksPidWkn::~CbmTrdSetTracksPidWkn() {}
// -------------------------------------------------------------------------


// -----  SetParContainers -------------------------------------------------
void CbmTrdSetTracksPidWkn::SetParContainers() {}
// -------------------------------------------------------------------------


// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmTrdSetTracksPidWkn::Init()
{

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- CbmTrdSetTracksPidWkn::Init: "
         << "RootManager not instantised!" << endl;
    return kFATAL;
  }

  // Get TrdTrack array
  fTrackArray = (TClonesArray*) ioman->GetObject("TrdTrack");
  if (!fTrackArray) {
    cout << "-E- CbmTrdSetTracksPidWkn::Init: No TrdTrack array!" << endl;
    return kERROR;
  }

  // Get TrdHit array
  fTrdHitArray = (TClonesArray*) ioman->GetObject("TrdHit");
  if (!fTrdHitArray) {
    cout << "-E- CbmTrdSetTracksPidWkn::Init: No TrdHit array!" << endl;
    return kERROR;
  }

  SetParameters();

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmTrdSetTracksPidWkn::Exec(Option_t*)
{

  if (!fTrackArray) return;

  Int_t nTracks = fTrackArray->GetEntriesFast();

  Double_t result_wkn;
  Int_t NHits;

  fNofTracks = 0;


  std::vector<float> eLossVectorTmp;
  std::vector<float> eLossVector;

  for (Int_t iTrack = 0; iTrack < nTracks; iTrack++) {
    eLossVectorTmp.clear();
    eLossVector.clear();
    CbmTrdTrack* pTrack = (CbmTrdTrack*) fTrackArray->At(iTrack);
    NHits               = pTrack->GetNofHits();

    // Set total energy loss member in CbmTrdTack object if not already done
    assert(pTrack);
    if (pTrack->GetELoss() < 0.) {
      double_t sumEloss = 0.;
      for (Int_t iHit = 0; iHit < NHits; iHit++) {
        CbmTrdHit* hit = (CbmTrdHit*) fTrdHitArray->At(pTrack->GetHitIndex(iHit));
        assert(hit);
        sumEloss += hit->GetELoss();
      }
      pTrack->SetELoss(sumEloss);
    }  //? Track energy loss not set

    // Up to now only for tracks with twelve hits the Wkn can be calculated
    // This should be changed in the future.
    if (NHits < fnSet) {
      fNofTracks++;
      pTrack->SetPidWkn(-2.);
      continue;
    }

    for (Int_t iTRD = 0; iTRD < NHits; iTRD++) {
      Int_t TRDindex    = pTrack->GetHitIndex(iTRD);
      CbmTrdHit* trdHit = (CbmTrdHit*) fTrdHitArray->At(TRDindex);
      eLossVectorTmp.push_back((trdHit->GetELoss()) * 1000000);
    }

    // calculate the lambda parameter for each TRD layer
    for (unsigned int jVec = 0; jVec < eLossVectorTmp.size(); jVec++)
      eLossVectorTmp[jVec] = (eLossVectorTmp[jVec] - fEmp) / fXi - 0.225;

    sort(eLossVectorTmp.begin(), eLossVectorTmp.end());

    for (unsigned int jVec = 0; jVec < eLossVectorTmp.size(); jVec++) {
      eLossVectorTmp[jVec] = TMath::LandauI(eLossVectorTmp[jVec]);
    }

    for (int iHit = 0; iHit < fnSet; iHit++)
      eLossVector.push_back(eLossVectorTmp[NHits - fnSet + iHit]);

    // calculate the Wkn and add the information to the TrdTrack

    Double_t S = 0, ty = 0, ti = 0;

    for (Int_t i = 0; i < fnSet; i++) {
      ty = eLossVector[i];
      ti = i;
      S += pow((ti - 1) / fnSet - ty, fk1) - pow(ti / fnSet - ty, fk1);
    }
    result_wkn = -fwkn0 * S;
    //	cout<<result_wkn<<endl;
    pTrack->SetPidWkn(result_wkn);
  }
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmTrdSetTracksPidWkn::Finish() {}
// -------------------------------------------------------------------------

void CbmTrdSetTracksPidWkn::SetParameters()
{
  if (fSISType == "sis300") {
    fnSet   = 5;  //  number of the layers with TR
    fdegWkn = 4;  //  statistics degree
    fEmp    = 1.06;
    fXi     = 0.57;
  }

  if (fSISType == "sis100") {
    fnSet   = 3;  //  number of the layers with TR
    fdegWkn = 2;  //  statistics degree
    fEmp    = 3.5;
    fXi     = 5.0;
  }


  fk1   = fdegWkn + 1;
  fwkn0 = pow(fnSet, 0.5 * fdegWkn) / fk1;
  fWmin = 1 / (pow(2, fdegWkn) * pow(fnSet, fdegWkn / 2) * (fdegWkn + 1));
  fWmax = pow(fnSet, fdegWkn / 2) / (fdegWkn + 1);
  fDiff = fWmax - fWmin;
}

ClassImp(CbmTrdSetTracksPidWkn)
