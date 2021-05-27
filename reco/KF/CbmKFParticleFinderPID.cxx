//-----------------------------------------------------------
//-----------------------------------------------------------

// Cbm Headers ----------------------
#include "CbmKFParticleFinderPID.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMuchTrack.h"
#include "CbmRichRing.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"

#include "FairRunAna.h"

//ROOT headers
#include "TClonesArray.h"

//c++ and std headers
#include <iostream>
#include <vector>
using std::vector;

double vecMedian(const vector<double>& vec)
{
  double median          = 0.;
  vector<double> vecCopy = vec;
  sort(vecCopy.begin(), vecCopy.end());
  int size = vecCopy.size();
  if (size % 2 == 0) median = (vecCopy[size / 2 - 1] + vecCopy[size / 2]) / 2;
  else
    median = vecCopy[size / 2];
  return median;
}

CbmKFParticleFinderPID::CbmKFParticleFinderPID(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fStsTrackBranchName("StsTrack")
  , fGlobalTrackBranchName("GlobalTrack")
  , fStsHitBranchName("StsHit")
  , fStsClusterBranchName("StsCluster")
  , fStsDigiBranchName("StsDigi")
  , fTofBranchName("TofHit")
  , fMCTracksBranchName("MCTrack")
  , fTrackMatchBranchName("StsTrackMatch")
  , fTrdBranchName("TrdTrack")
  , fTrdHitBranchName("TrdHit")
  , fRichBranchName("RichRing")
  , fMuchTrackBranchName("MuchTrack")
  , fTrackArray(0)
  , fGlobalTrackArray(0)
  , fStsHitArray(0)
  , fStsClusterArray(0)
  , fDigiManager(nullptr)
  , fTofHitArray(0)
  , fMCTrackArray(0)
  , fTrackMatchArray(0)
  , fTrdTrackArray(0)
  , fTrdHitArray(0)
  , fRichRingArray(0)
  , fMuchTrackArray(0)
  , fMCTracks(0)
  , fPIDMode(0)
  , fSisMode(1)
  , fTrdPIDMode(0)
  , fRichPIDMode(0)
  , fMuchMode(0)
  , fUseSTSdEdX(kFALSE)
  , fUseTRDdEdX(kFALSE)
  , fTimeSliceMode(0)
  , fPID(0)
{
  //MuCh cuts
  fMuchCutsInt[0]   = 7;     // N sts hits
  fMuchCutsInt[1]   = 14;    // N MuCh hits for LMVM
  fMuchCutsInt[2]   = 17;    // N MuCh hits for J/Psi
  fMuchCutsFloat[0] = 1.e6;  // STS  Chi2/NDF for muons
  fMuchCutsFloat[1] = 1.5;   // MuSh Chi2/NDF for muons
}

CbmKFParticleFinderPID::~CbmKFParticleFinderPID() {}

InitStatus CbmKFParticleFinderPID::Init()
{
  //Get ROOT Manager
  FairRootManager* ioman = FairRootManager::Instance();

  if (ioman == 0) {
    Error("CbmKFParticleFinderPID::Init", "RootManager not instantiated!");
    return kERROR;
  }


  //check the mode
  fTimeSliceMode = 0;
  if (ioman->CheckBranch("CbmEvent")) {
    fTimeSliceMode = 1;
    LOG(info) << GetName() << ": Running in the timeslice mode.";
  }
  else
    LOG(info) << GetName() << ": Running in the event by event mode.";

  // Get reconstructed events

  if (fPIDMode == 1) {
    FairRootManager* fManger = FairRootManager::Instance();
    if (fManger == 0) {
      Fatal("CbmKFParticleFinder::Init", "fManger is not found!");
      return kERROR;
    }


    CbmMCDataManager* mcManager = 0;

    if (fTimeSliceMode) mcManager = (CbmMCDataManager*) fManger->GetObject("MCDataManager");

    if (fTimeSliceMode)
      if (mcManager == 0) {
        Fatal("CbmKFParticleFinderPID::Init", "MC Data Manager is not found!");
        return kERROR;
      }

    if (fTimeSliceMode) {
      fMCTracks = mcManager->InitBranch("MCTrack");
      if (fMCTracks == 0) {
        Fatal("CbmKFParticleFinderPID::Init", "MC track array not found!");
        return kERROR;
      }
    }
    else {
      fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
      if (fMCTrackArray == 0) {
        Fatal("CbmKFParticleFinderPID::Init", "MC track array not found!");
        return kERROR;
      }
    }

    //Track match
    fTrackMatchArray = (TClonesArray*) ioman->GetObject(fTrackMatchBranchName);
    if (fTrackMatchArray == 0) {
      Error("CbmKFParticleFinderPID::Init", "track match array not found!");
      return kERROR;
    }
  }

  // Get sts tracks
  fTrackArray = (TClonesArray*) ioman->GetObject(fStsTrackBranchName);
  if (fTrackArray == 0) {
    Error("CbmKFParticleFinderPID::Init", "track-array not found!");
    return kERROR;
  }

  if (fPIDMode == 2) {
    // Get global tracks
    fGlobalTrackArray = (TClonesArray*) ioman->GetObject(fGlobalTrackBranchName);
    if (fGlobalTrackArray == 0) {
      Error("CbmKFParticleFinderPID::Init", "global track array not found!");
      return kERROR;
    }

    // Get STS hit
    fStsHitArray = (TClonesArray*) ioman->GetObject(fStsHitBranchName);
    if (fStsHitArray == 0) {
      Error("CbmKFParticleFinderPID::Init", "STS hit array not found!");
      return kERROR;
    }

    // Get sts clusters
    fStsClusterArray = (TClonesArray*) ioman->GetObject(fStsClusterBranchName);
    if (fStsClusterArray == 0) {
      Error("CbmKFParticleFinderPID::Init", "STS cluster array not found!");
      return kERROR;
    }


    // --- Digi Manager
    fDigiManager = CbmDigiManager::Instance();
    fDigiManager->Init();

    // --- Check input array (StsDigis)
    if (!fDigiManager->IsPresent(ECbmModuleId::kSts)) LOG(fatal) << GetName() << ": No StsDigi branch in input!";

    // Get ToF hits
    fTofHitArray = (TClonesArray*) ioman->GetObject(fTofBranchName);
    if (fTofHitArray == 0) {
      Error("CbmKFParticleFinderPID::Init", "TOF track-array not found!");
      //return kERROR;
    }

    if (fTrdPIDMode > 0) {
      fTrdTrackArray = (TClonesArray*) ioman->GetObject(fTrdBranchName);
      if (fTrdTrackArray == 0) {
        Error("CbmKFParticleFinderPID::Init", "TRD track-array not found!");
        //return kERROR;
      }
    }

    fTrdHitArray = (TClonesArray*) ioman->GetObject(fTrdHitBranchName);
    if (fTrdHitArray == 0) { Error("CbmKFParticleFinderPID::Init", "TRD hit array not found!"); }

    if (fRichPIDMode > 0) {
      fRichRingArray = (TClonesArray*) ioman->GetObject(fRichBranchName);
      if (fRichRingArray == 0) {
        Error("CbmKFParticleFinderPID::Init", "Rich ring array not found!");
        //return kERROR;
      }
    }

    if (fMuchMode > 0) {
      fMuchTrackArray = (TClonesArray*) ioman->GetObject(fMuchTrackBranchName);
      if (fMuchTrackArray == 0) {
        Error("CbmKFParticleFinderPID::Init", "Much track-array not found!");
        return kERROR;
      }
    }
  }

  return kSUCCESS;
}

void CbmKFParticleFinderPID::Exec(Option_t* /*opt*/)
{
  fPID.clear();

  Int_t nTracks = fTrackArray->GetEntriesFast();
  fPID.resize(nTracks, -1);

  if (fPIDMode == 1) SetMCPID();
  if (fPIDMode == 2) SetRecoPID();
}

void CbmKFParticleFinderPID::Finish() {}

void CbmKFParticleFinderPID::SetMCPID()
{
  Int_t nTracks   = fTrackArray->GetEntriesFast();
  Int_t nMCTracks = 0;
  if (!fTimeSliceMode) nMCTracks = fMCTrackArray->GetEntriesFast();

  for (int iTr = 0; iTr < nTracks; iTr++) {
    fPID[iTr] = -2;

    CbmTrackMatchNew* stsTrackMatch = (CbmTrackMatchNew*) fTrackMatchArray->At(iTr);
    if (stsTrackMatch->GetNofLinks() == 0) continue;
    Float_t bestWeight  = 0.f;
    Float_t totalWeight = 0.f;
    Int_t mcTrackId     = -1;
    Int_t iFile         = -1;
    Int_t iEvent        = -1;

    for (int iLink = 0; iLink < stsTrackMatch->GetNofLinks(); iLink++) {
      totalWeight += stsTrackMatch->GetLink(iLink).GetWeight();
      if (stsTrackMatch->GetLink(iLink).GetWeight() > bestWeight) {
        bestWeight = stsTrackMatch->GetLink(iLink).GetWeight();
        mcTrackId  = stsTrackMatch->GetLink(iLink).GetIndex();
        if (fTimeSliceMode) {
          iFile  = stsTrackMatch->GetLink(iLink).GetFile();
          iEvent = stsTrackMatch->GetLink(iLink).GetEntry();
        }
      }
    }
    if (bestWeight / totalWeight < 0.7) continue;

    if ((!fTimeSliceMode) && (mcTrackId >= nMCTracks || mcTrackId < 0)) continue;
    //     if(mcTrackId >= nMCTracks || mcTrackId < 0)
    //     {
    //       LOG(info) << "Sts Matching is wrong!    StsTrackId = " << mcTrackId << " N mc tracks = " << nMCTracks;
    //       continue;
    //     }


    //     LOG(info) <<mcTrackId<<" mcTrackId "<<fMCTrackArray->GetEntriesFast();

    CbmMCTrack* cbmMCTrack = 0;

    if (fTimeSliceMode) cbmMCTrack = dynamic_cast<CbmMCTrack*>(fMCTracks->Get(iFile, iEvent, mcTrackId));
    else
      cbmMCTrack = (CbmMCTrack*) fMCTrackArray->At(mcTrackId);


    if (!(TMath::Abs(cbmMCTrack->GetPdgCode()) == 11 || TMath::Abs(cbmMCTrack->GetPdgCode()) == 13
          || TMath::Abs(cbmMCTrack->GetPdgCode()) == 211 || TMath::Abs(cbmMCTrack->GetPdgCode()) == 321
          || TMath::Abs(cbmMCTrack->GetPdgCode()) == 2212 || TMath::Abs(cbmMCTrack->GetPdgCode()) == 1000010020
          || TMath::Abs(cbmMCTrack->GetPdgCode()) == 1000010030 || TMath::Abs(cbmMCTrack->GetPdgCode()) == 1000020030
          || TMath::Abs(cbmMCTrack->GetPdgCode()) == 1000020040))
      fPID[iTr] = -1;
    else
      fPID[iTr] = cbmMCTrack->GetPdgCode();
  }
}

void CbmKFParticleFinderPID::SetRecoPID()
{
  const Double_t m2TOF[7] = {0.885, 0.245, 0.019479835, 0., 3.49, 7.83, 1.95};

  Double_t sP[7][5];
  if (fSisMode == 0)  //SIS-100
  {
    Double_t sPLocal[7][5] = {{0.056908, -0.0470572, 0.0216465, -0.0021016, 8.50396e-05},
                              {0.00943075, -0.00635429, 0.00998695, -0.00111527, 7.77811e-05},
                              {0.00176298, 0.00367263, 0.00308013, 0.000844013, -0.00010423},
                              {0.00218401, 0.00152391, 0.00895357, -0.000533423, 3.70326e-05},
                              {0.261491, -0.103121, 0.0247587, -0.00123286, 2.61731e-05},
                              {0.657274, -0.22355, 0.0430177, -0.0026822, 7.34146e-05},
                              {0.116525, -0.045522, 0.0151319, -0.000495545, 4.43144e-06}};
    for (Int_t iSp = 0; iSp < 7; iSp++)
      for (Int_t jSp = 0; jSp < 5; jSp++)
        sP[iSp][jSp] = sPLocal[iSp][jSp];
  }

  if (fSisMode == 1)  //SIS-300
  {
    Double_t sPLocal[7][5] = {{0.0337428, -0.013939, 0.00567602, -0.000202229, 4.07531e-06},
                              {0.00717827, -0.00257353, 0.00389851, -9.83097e-05, 1.33011e-06},
                              {0.001348, 0.00220126, 0.0023619, 7.35395e-05, -4.06706e-06},
                              {0.00142972, 0.00308919, 0.00326995, 6.91715e-05, -2.44194e-06},
                              {0.261491, -0.103121, 0.0247587, -0.00123286, 2.61731e-05},  //TODO tune for SIS300
                              {0.657274, -0.22355, 0.0430177, -0.0026822, 7.34146e-05},
                              {0.116525, -0.045522, 0.0151319, -0.000495545, 4.43144e-06}};
    for (Int_t iSp = 0; iSp < 7; iSp++)
      for (Int_t jSp = 0; jSp < 5; jSp++)
        sP[iSp][jSp] = sPLocal[iSp][jSp];
  }

  const Int_t PdgHypo[9] = {2212, 321, 211, -11, 1000010029, 1000010030, 1000020030, -13, -19};

  for (Int_t igt = 0; igt < fGlobalTrackArray->GetEntriesFast(); igt++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTrackArray->At(igt));

    Int_t stsTrackIndex = globalTrack->GetStsTrackIndex();
    if (stsTrackIndex < 0) continue;

    Bool_t isElectronTRD  = 0;
    Bool_t isElectronRICH = 0;
    Bool_t isElectron     = 0;


    CbmStsTrack* cbmStsTrack = (CbmStsTrack*) fTrackArray->At(stsTrackIndex);

    const FairTrackParam* stsPar = cbmStsTrack->GetParamFirst();
    TVector3 mom;
    stsPar->Momentum(mom);

    Double_t p  = mom.Mag();
    Double_t pt = mom.Perp();
    Double_t pz = sqrt(p * p - pt * pt);
    Int_t q     = stsPar->GetQp() > 0 ? 1 : -1;

    if (fRichPIDMode == 1) {
      if (fRichRingArray) {
        Int_t richIndex = globalTrack->GetRichRingIndex();
        if (richIndex > -1) {
          CbmRichRing* richRing = (CbmRichRing*) fRichRingArray->At(richIndex);
          if (richRing) {
            Double_t axisA = richRing->GetAaxis();
            Double_t axisB = richRing->GetBaxis();
            Double_t dist  = 0;  // richRing->GetDistance();

            Double_t fMeanA    = 4.95;
            Double_t fMeanB    = 4.54;
            Double_t fRmsA     = 0.30;
            Double_t fRmsB     = 0.22;
            Double_t fRmsCoeff = 3.5;
            Double_t fDistCut  = 1.;


            //            if(fElIdAnn->DoSelect(richRing, p) > -0.5) isElectronRICH = 1;
            if (p < 5.) {
              if (fabs(axisA - fMeanA) < fRmsCoeff * fRmsA && fabs(axisB - fMeanB) < fRmsCoeff * fRmsB
                  && dist < fDistCut)
                isElectronRICH = 1;
            }
            else {
              ///3 sigma
              // Double_t polAaxis = 5.80008 - 4.10118 / (momentum - 3.67402);
              // Double_t polBaxis = 5.58839 - 4.75980 / (momentum - 3.31648);
              // Double_t polRaxis = 5.87252 - 7.64641/(momentum - 1.62255);
              ///2 sigma
              Double_t polAaxis = 5.64791 - 4.24077 / (p - 3.65494);
              Double_t polBaxis = 5.41106 - 4.49902 / (p - 3.52450);
              //Double_t polRaxis = 5.66516 - 6.62229/(momentum - 2.25304);
              if (axisA < (fMeanA + fRmsCoeff * fRmsA) && axisA > polAaxis && axisB < (fMeanB + fRmsCoeff * fRmsB)
                  && axisB > polBaxis && dist < fDistCut)
                isElectronRICH = 1;
            }
          }
        }
      }
    }

    if (fTrdPIDMode == 1) {
      if (fTrdTrackArray) {
        Int_t trdIndex = globalTrack->GetTrdTrackIndex();
        if (trdIndex > -1) {
          CbmTrdTrack* trdTrack = (CbmTrdTrack*) fTrdTrackArray->At(trdIndex);
          if (trdTrack) {
            if (trdTrack->GetPidWkn() > 0.635) isElectronTRD = 1;
          }
        }
      }
    }

    if (fTrdPIDMode == 2) {
      if (fTrdTrackArray) {
        Int_t trdIndex = globalTrack->GetTrdTrackIndex();
        if (trdIndex > -1) {
          CbmTrdTrack* trdTrack = (CbmTrdTrack*) fTrdTrackArray->At(trdIndex);
          if (trdTrack) {
            if (trdTrack->GetPidANN() > 0.9) isElectronTRD = 1;
          }
        }
      }
    }

    // dEdX in TRD
    double dEdXTRD = 1e6;  // in case if TRD is not used
    if (fTrdTrackArray) {
      Int_t trdIndex = globalTrack->GetTrdTrackIndex();
      if (trdIndex > -1) {
        CbmTrdTrack* trdTrack = (CbmTrdTrack*) fTrdTrackArray->At(trdIndex);
        if (trdTrack) {
          Double_t eloss = 0.;
          for (Int_t iTRD = 0; iTRD < trdTrack->GetNofHits(); iTRD++) {
            Int_t TRDindex    = trdTrack->GetHitIndex(iTRD);
            CbmTrdHit* trdHit = (CbmTrdHit*) fTrdHitArray->At(TRDindex);
            eloss += trdHit->GetELoss();
          }
          if (trdTrack->GetNofHits() > 0.) dEdXTRD = 1e6 * pz / p * eloss / trdTrack->GetNofHits();
        }
      }
    }

    //STS dE/dX
    vector<double> dEdxAllveto;
    int nClustersWveto = cbmStsTrack->GetNofStsHits() + cbmStsTrack->GetNofStsHits();  //assume all clusters with veto
    double dr          = 1.;
    for (int iHit = 0; iHit < cbmStsTrack->GetNofStsHits(); ++iHit) {
      bool frontVeto = kFALSE, backVeto = kFALSE;
      CbmStsHit* stsHit = (CbmStsHit*) fStsHitArray->At(cbmStsTrack->GetStsHitIndex(iHit));

      double x, y, z, xNext, yNext, zNext;
      x = stsHit->GetX();
      y = stsHit->GetY();
      z = stsHit->GetZ();

      if (iHit != cbmStsTrack->GetNofStsHits() - 1) {
        CbmStsHit* stsHitNext = (CbmStsHit*) fStsHitArray->At(cbmStsTrack->GetStsHitIndex(iHit + 1));
        xNext                 = stsHitNext->GetX();
        yNext                 = stsHitNext->GetY();
        zNext                 = stsHitNext->GetZ();
        dr                    = sqrt((xNext - x) * (xNext - x) + (yNext - y) * (yNext - y) + (zNext - z) * (zNext - z))
             / (zNext - z);  // if *300um, you get real reconstructed dr
      }                      // else dr stay previous

      CbmStsCluster* frontCluster = (CbmStsCluster*) fStsClusterArray->At(stsHit->GetFrontClusterId());
      CbmStsCluster* backCluster  = (CbmStsCluster*) fStsClusterArray->At(stsHit->GetBackClusterId());

      if (!frontCluster || !backCluster) {
        LOG(info) << "CbmKFParticleFinderPID: no front or back cluster";
        continue;
      }

      //check if at least one digi in a cluster has overflow --- charge is registered in the last ADC channel #31
      for (int iDigi = 0; iDigi < frontCluster->GetNofDigis(); ++iDigi) {
        if (31 == (fDigiManager->Get<CbmStsDigi>(frontCluster->GetDigi(iDigi))->GetCharge())) frontVeto = kTRUE;
      }
      for (int iDigi = 0; iDigi < backCluster->GetNofDigis(); ++iDigi) {
        if (31 == (fDigiManager->Get<CbmStsDigi>(backCluster->GetDigi(iDigi))->GetCharge())) backVeto = kTRUE;
      }

      if (!frontVeto) dEdxAllveto.push_back((frontCluster->GetCharge()) / dr);
      if (!backVeto) dEdxAllveto.push_back((backCluster->GetCharge()) / dr);

      if (0 == frontVeto) nClustersWveto--;
      if (0 == backVeto) nClustersWveto--;
    }
    float dEdXSTS = 1.e6;
    if (dEdxAllveto.size() != 0) dEdXSTS = vecMedian(dEdxAllveto);


    int isMuon = 0;
    if (fMuchTrackArray && fMuchMode > 0) {
      Int_t muchIndex = globalTrack->GetMuchTrackIndex();
      if (muchIndex > -1) {
        CbmMuchTrack* muchTrack = (CbmMuchTrack*) fMuchTrackArray->At(muchIndex);
        if (muchTrack) {
          if ((cbmStsTrack->GetChiSq() / cbmStsTrack->GetNDF()) < fMuchCutsFloat[0]
              && (muchTrack->GetChiSq() / muchTrack->GetNDF()) < fMuchCutsFloat[1]
              && cbmStsTrack->GetNofHits() >= fMuchCutsInt[0]) {
            if (muchTrack->GetNofHits() >= fMuchCutsInt[1]) isMuon = 2;
            if (muchTrack->GetNofHits() >= fMuchCutsInt[2]) isMuon = 1;
          }
        }
      }
    }

    if (p > 1.5) {
      if (isElectronRICH && isElectronTRD) isElectron = 1;
      if (fRichPIDMode == 0 && isElectronTRD) isElectron = 1;
      if (fTrdPIDMode == 0 && isElectronRICH) isElectron = 1;
    }
    else if (isElectronRICH)
      isElectron = 1;

    if (fTofHitArray && isMuon == 0) {
      Double_t l = globalTrack->GetLength();  // l is calculated by global tracking
      if (fSisMode == 0)                      //SIS-100
        if (!((l > 500.) && (l < 900.))) continue;
      if (fSisMode == 1)  //SIS 300
        if (!((l > 700.) && (l < 1500.))) continue;

      Double_t time;
      Int_t tofHitIndex = globalTrack->GetTofHitIndex();
      if (tofHitIndex >= 0) {
        const CbmTofHit* tofHit = static_cast<const CbmTofHit*>(fTofHitArray->At(tofHitIndex));
        if (!tofHit) continue;
        time = tofHit->GetTime();
      }
      else
        continue;

      if (fSisMode == 0)  //SIS-100
        if (!((time > 16.) && (time < 42.))) continue;
      if (fSisMode == 1)  //SIS 300
        if (!((time > 26.) && (time < 52.))) continue;

      Double_t m2 = p * p * (1. / ((l / time / 29.9792458) * (l / time / 29.9792458)) - 1.);

      Double_t sigma[7];
      Double_t dm2[7];

      for (int iSigma = 0; iSigma < 7; iSigma++) {
        sigma[iSigma] = sP[iSigma][0] + sP[iSigma][1] * p + sP[iSigma][2] * p * p + sP[iSigma][3] * p * p * p
                        + sP[iSigma][4] * p * p * p * p;
        dm2[iSigma] = fabs(m2 - m2TOF[iSigma]) / sigma[iSigma];
      }

      if (isElectron) {
        if (dm2[3] > 3.) isElectron = 0;
      }

      int iPdg        = 2;
      Double_t dm2min = dm2[2];

      if (!isElectron && isMuon == 0) {
        //         if(p>12.) continue;

        for (int jPDG = 0; jPDG < 7; jPDG++) {
          if (jPDG == 3) continue;
          if (dm2[jPDG] < dm2min) {
            iPdg   = jPDG;
            dm2min = dm2[jPDG];
          }
        }

        if (dm2min > 2) iPdg = -1;

        Double_t dEdXTRDthresholdProton = 10.;
        if (iPdg == 6) {
          if (fUseTRDdEdX && (dEdXTRD < dEdXTRDthresholdProton)) iPdg = 0;
          if (fUseSTSdEdX && (dEdXSTS < 7.5e4)) iPdg = 0;
        }

        if (iPdg > -1) fPID[stsTrackIndex] = q * PdgHypo[iPdg];
      }
    }

    if (isElectron) fPID[stsTrackIndex] = q * PdgHypo[3];

    if (isMuon == 1) fPID[stsTrackIndex] = q * PdgHypo[7];
    if (isMuon == 2) fPID[stsTrackIndex] = q * PdgHypo[8];
  }
}

ClassImp(CbmKFParticleFinderPID);
