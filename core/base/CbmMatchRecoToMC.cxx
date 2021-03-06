/*
 * CbmMatchRecoToMC.cxx
 *
 *  Created on: Oct 17, 2013
 *      Author: andrey
 */
#include "CbmMatchRecoToMC.h"

#include "CbmCluster.h"        // for CbmCluster
#include "CbmDigiManager.h"    // for CbmDigiManager
#include "CbmHit.h"            // for CbmHit, kMUCHPIXELHIT
#include "CbmLink.h"           // for CbmLink
#include "CbmMCDataArray.h"    // for CbmMCDataArray
#include "CbmMCDataManager.h"  // for CbmMCDataManager
#include "CbmMCTrack.h"        // for CbmMCTrack
#include "CbmMatch.h"          // for CbmMatch
#include "CbmPixelHit.h"       // for CbmPixelHit
#include "CbmRichDigi.h"       // for CbmRichDigi
#include "CbmRichHit.h"        // for CbmRichHit
#include "CbmRichPoint.h"      // for CbmRichPoint
#include "CbmRichRing.h"       // for CbmRichRing
#include "CbmStsAddress.h"     // for GetSystemId
#include "CbmStsHit.h"         // for CbmStsHit
#include "CbmStsTrack.h"       // for CbmStsTrack
#include "CbmTofDigi.h"        // for CbmTofDigi
#include "CbmTrack.h"          // for CbmTrack
#include "CbmTrackMatchNew.h"  // for CbmTrackMatchNew

#include <FairLogger.h>       // for LOG, Logger
#include <FairMCPoint.h>      // for FairMCPoint
#include <FairRootManager.h>  // for FairRootManager
#include <FairTask.h>         // for FairTask, InitStatus

#include <TClonesArray.h>  // for TClonesArray

#include <algorithm>                               // for find
#include <boost/exception/exception.hpp>           // for clone_impl
#include <boost/type_index/type_index_facade.hpp>  // for operator==

using std::pair;
using std::vector;

Int_t CbmMatchRecoToMC::fEventNumber = 0;

CbmMatchRecoToMC::CbmMatchRecoToMC() : FairTask("MatchRecoToMC") {}

CbmMatchRecoToMC::~CbmMatchRecoToMC() {

  if (fStsClusterMatches != nullptr) {
    fStsClusterMatches->Delete();
    delete fStsClusterMatches;
  }
  if (fStsHitMatches != nullptr) {
    fStsHitMatches->Delete();
    delete fStsHitMatches;
  }
  if (fStsTrackMatches) {
    fStsTrackMatches->Delete();
    delete fStsTrackMatches;
  }

  if (fRichTrackMatches) {
    fRichTrackMatches->Delete();
    delete fRichTrackMatches;
  }

  if (fTrdClusterMatches != nullptr) {
    fTrdClusterMatches->Delete();
    delete fTrdClusterMatches;
  }
  if (fTrdHitMatches != nullptr) {
    fTrdHitMatches->Delete();
    delete fTrdHitMatches;
  }
  if (fTrdTrackMatches) {
    fTrdTrackMatches->Delete();
    delete fTrdTrackMatches;
  }

  if (fMuchClusterMatches != nullptr) {
    fMuchClusterMatches->Delete();
    delete fMuchClusterMatches;
  }
  if (fMuchPixelHitMatches != nullptr) {
    fMuchPixelHitMatches->Delete();
    delete fMuchPixelHitMatches;
  }
  if (fMuchTrackMatches) {
    fMuchTrackMatches->Delete();
    delete fMuchTrackMatches;
  }

  if (fMvdClusterMatches != nullptr) {
    fMvdClusterMatches->Delete();
    delete fMvdClusterMatches;
  }
  if (fMvdHitMatches != nullptr) {
    fMvdHitMatches->Delete();
    delete fMvdHitMatches;
  }

  if (fTofHitMatches != nullptr) {
    fTofHitMatches->Delete();
    delete fTofHitMatches;
  }
}

InitStatus CbmMatchRecoToMC::Init() {
  ReadAndCreateDataBranches();

  return kSUCCESS;
}

void CbmMatchRecoToMC::Exec(Option_t* /*opt*/) {
  if (fStsClusterMatches != nullptr) fStsClusterMatches->Delete();
  if (fStsHitMatches != nullptr) fStsHitMatches->Delete();
  if (fStsTrackMatches != nullptr) fStsTrackMatches->Delete();
  if (fRichTrackMatches != nullptr) fRichTrackMatches->Delete();
  if (fTrdClusterMatches != nullptr) fTrdClusterMatches->Delete();
  if (fTrdHitMatches != nullptr) fTrdHitMatches->Delete();
  if (fTrdTrackMatches != nullptr) fTrdTrackMatches->Delete();
  if (fMuchClusterMatches != nullptr) fMuchClusterMatches->Delete();
  if (fMuchPixelHitMatches != nullptr) fMuchPixelHitMatches->Delete();
  if (fMuchTrackMatches != nullptr) fMuchTrackMatches->Delete();
  if (fMvdHitMatches != nullptr) fMvdHitMatches->Delete();
  if (fMvdClusterMatches != nullptr) fMvdClusterMatches->Delete();
  if (fTofHitMatches != nullptr) fTofHitMatches->Delete();


  // -----   MVD   -----
  // MVD: digi->hit
  if (fMvdHits && fMvdHitMatches && !fMvdCluster) {
    MatchHitsMvd(fMvdHits, fMvdHitMatches);
  } else {
    // MVD: digi->cluster
    MatchClusters(ECbmModuleId::kMvd, fMvdCluster, fMvdClusterMatches);
    // MVD: cluster->hit
    MatchHits(fMvdClusterMatches, fMvdHits, fMvdHitMatches);
  }

  // -----   STS   -----
  // STS: digi->cluster
  MatchClusters(ECbmModuleId::kSts, fStsClusters, fStsClusterMatches);
  // STS: cluster->hit
  MatchHitsSts(fStsClusterMatches, fStsHits, fStsHitMatches);
  // STS: hit->track
  MatchStsTracks(fMvdHitMatches,
                 fStsHitMatches,
                 fMvdPoints,
                 fStsPoints,
                 fStsTracks,
                 fStsTrackMatches);

  // -----   MUCH  -----
  // MUCH: digi->cluster
  MatchClusters(ECbmModuleId::kMuch, fMuchClusters, fMuchClusterMatches);
  // MUCH: cluster->hit
  MatchHits(fMuchClusterMatches, fMuchPixelHits, fMuchPixelHitMatches);
  // MUCH: hit->track
  MatchTracks(
    fMuchPixelHitMatches, fMuchPoints, fMuchTracks, fMuchTrackMatches);


  //RICH
  if (fRichHits && fRichMcPoints && fRichRings && fRichTrackMatches) {
    MatchRichRings(
      fRichRings, fRichHits, fRichMcPoints, fMCTracks, fRichTrackMatches);
  }

  // TRD
  if (fTrdClusters && fTrdHits) {  // MC->digi->cluster->hit->track
    MatchClusters(ECbmModuleId::kTrd, fTrdClusters, fTrdClusterMatches);
    MatchHits(fTrdClusterMatches, fTrdHits, fTrdHitMatches);
    MatchTracks(fTrdHitMatches, fTrdPoints, fTrdTracks, fTrdTrackMatches);
  } else if (fTrdHits) {  // MC->hit->track
    MatchHitsToPoints(fTrdPoints, fTrdHits, fTrdHitMatches);
    MatchTracks(fTrdHitMatches, fTrdPoints, fTrdTracks, fTrdTrackMatches);
  }

  // TOF: (Digi->MC)+(Hit->Digi)=>(Hit->MC)
  MatchHitsTof(fTofHitDigiMatches, fTofHits, fTofHitMatches);

  //static Int_t eventNo = 0;
  LOG(info) << "CbmMatchRecoToMC::Exec eventNo=" << fEventNumber++;
}

void CbmMatchRecoToMC::Finish() {}

void CbmMatchRecoToMC::ReadAndCreateDataBranches() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) {
    LOG(fatal)
      << "CbmMatchRecoToMC::ReadAndCreateDataBranches() NULL FairRootManager.";
  }

  CbmMCDataManager* mcManager =
    (CbmMCDataManager*) ioman->GetObject("MCDataManager");

  if (nullptr == mcManager) { LOG(fatal) << "CbmMatchRecoToMC::ReadAndCreateDataBranches() NULL MCDataManager."; }

  fMCTracks = mcManager->InitBranch("MCTrack");

  //fMCTracksArray= (TClonesArray*) ioman->GetObject("MCTrack");

  // --- Digi Manager
  fDigiManager = CbmDigiManager::Instance();
  fDigiManager->Init();


  // Register output branches only if they don't exist
  // This should solve the problem with BranchName_1, BranchName_2
  // when having two instances of CbmMatchRecoToMC in the same macro

  // STS
  fStsPoints = mcManager->InitBranch("StsPoint");

  fStsClusters = static_cast<TClonesArray*>(ioman->GetObject("StsCluster"));
  if (nullptr != fStsClusters) {
    fStsClusterMatches =
      static_cast<TClonesArray*>(ioman->GetObject("StsClusterMatch"));
    if (nullptr == fStsClusterMatches) {
      fStsClusterMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("StsClusterMatch",
                      "STS",
                      fStsClusterMatches,
                      IsOutputBranchPersistent("StsClusterMatch"));
    }
  }

  fStsHits = static_cast<TClonesArray*>(ioman->GetObject("StsHit"));
  if (nullptr != fStsHits) {
    fStsHitMatches =
      static_cast<TClonesArray*>(ioman->GetObject("StsHitMatch"));
    if (nullptr == fStsHitMatches) {
      fStsHitMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("StsHitMatch",
                      "STS",
                      fStsHitMatches,
                      IsOutputBranchPersistent("StsHitMatch"));
    }
  }

  fStsTracks = static_cast<TClonesArray*>(ioman->GetObject("StsTrack"));
  if (nullptr != fStsTracks) {
    fStsTrackMatches =
      static_cast<TClonesArray*>(ioman->GetObject("StsTrackMatch"));
    if (nullptr == fStsTrackMatches) {
      fStsTrackMatches = new TClonesArray("CbmTrackMatchNew", 100);
      ioman->Register("StsTrackMatch",
                      "STS",
                      fStsTrackMatches,
                      IsOutputBranchPersistent("StsTrackMatch"));
    }
  }

  //RICH
  fRichMcPoints = mcManager->InitBranch("RichPoint");
  fRichHits     = static_cast<TClonesArray*>(ioman->GetObject("RichHit"));

  fRichRings = static_cast<TClonesArray*>(ioman->GetObject("RichRing"));
  if (nullptr != fRichRings) {
    fRichTrackMatches =
      static_cast<TClonesArray*>(ioman->GetObject("RichRingMatch"));
    if (nullptr == fRichTrackMatches) {
      fRichTrackMatches = new TClonesArray("CbmTrackMatchNew", 100);
      ioman->Register("RichRingMatch",
                      "RICH",
                      fRichTrackMatches,
                      IsOutputBranchPersistent("RichRingMatch"));
    }
  }

  // TRD
  fTrdPoints   = mcManager->InitBranch("TrdPoint");
  fTrdClusters = static_cast<TClonesArray*>(ioman->GetObject("TrdCluster"));
  if (nullptr != fTrdClusters) {
    fTrdClusterMatches =
      static_cast<TClonesArray*>(ioman->GetObject("TrdClusterMatch"));
    if (nullptr == fTrdClusterMatches) {
      fTrdClusterMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("TrdClusterMatch",
                      "TRD",
                      fTrdClusterMatches,
                      IsOutputBranchPersistent("TrdClusterMatch"));
    }
  }

  fTrdHits = static_cast<TClonesArray*>(ioman->GetObject("TrdHit"));
  if (nullptr != fTrdHits) {
    fTrdHitMatches =
      static_cast<TClonesArray*>(ioman->GetObject("TrdHitMatch"));
    if (nullptr == fTrdHitMatches) {
      fTrdHitMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("TrdHitMatch",
                      "TRD",
                      fTrdHitMatches,
                      IsOutputBranchPersistent("TrdHitMatch"));
    }
  }


  fTrdTracks = static_cast<TClonesArray*>(ioman->GetObject("TrdTrack"));
  if (nullptr != fTrdTracks) {
    fTrdTrackMatches =
      static_cast<TClonesArray*>(ioman->GetObject("TrdTrackMatch"));
    if (nullptr == fTrdTrackMatches) {
      fTrdTrackMatches = new TClonesArray("CbmTrackMatchNew", 100);
      ioman->Register("TrdTrackMatch",
                      "TRD",
                      fTrdTrackMatches,
                      IsOutputBranchPersistent("TrdTrackMatch"));
    }
  }

  // MUCH
  fMuchPoints = mcManager->InitBranch("MuchPoint");

  fMuchTracks   = (TClonesArray*) ioman->GetObject("MuchTrack");
  fMuchClusters = static_cast<TClonesArray*>(ioman->GetObject("MuchCluster"));
  if (nullptr != fMuchClusters) {
    fMuchClusterMatches =
      static_cast<TClonesArray*>(ioman->GetObject("MuchClusterMatch"));
    if (nullptr == fMuchClusterMatches) {
      fMuchClusterMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("MuchClusterMatch",
                      "MUCH",
                      fMuchClusterMatches,
                      IsOutputBranchPersistent("MuchClusterMatch"));
    }
  }

  fMuchPixelHits = static_cast<TClonesArray*>(ioman->GetObject("MuchPixelHit"));
  if (nullptr != fMuchPixelHits) {
    fMuchPixelHitMatches =
      static_cast<TClonesArray*>(ioman->GetObject("MuchPixelHitMatch"));
    if (nullptr == fMuchPixelHitMatches) {
      fMuchPixelHitMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("MuchPixelHitMatch",
                      "MUCH",
                      fMuchPixelHitMatches,
                      IsOutputBranchPersistent("MuchPixelHitMatch"));
    }
  }

  fMuchTracks = static_cast<TClonesArray*>(ioman->GetObject("MuchTrack"));
  if (nullptr != fMuchTracks) {
    fMuchTrackMatches =
      static_cast<TClonesArray*>(ioman->GetObject("MuchTrackMatch"));
    if (nullptr == fMuchTrackMatches) {
      fMuchTrackMatches = new TClonesArray("CbmTrackMatchNew", 100);
      ioman->Register("MuchTrackMatch",
                      "MUCH",
                      fMuchTrackMatches,
                      IsOutputBranchPersistent("MuchTrackMatch"));
    }
  }

  // MVD
  fMvdPoints = mcManager->InitBranch("MvdPoint");

  fMvdCluster = static_cast<TClonesArray*>(ioman->GetObject("MvdCluster"));
  if (nullptr != fMvdCluster) {
    fMvdClusterMatches =
      static_cast<TClonesArray*>(ioman->GetObject("MvdClusterMatch"));
    if (nullptr == fMvdClusterMatches) {
      fMvdClusterMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("MvdClusterMatch",
                      "MVD",
                      fMvdClusterMatches,
                      IsOutputBranchPersistent("MvdClusterMatch"));
    }
  }

  fMvdHits = static_cast<TClonesArray*>(ioman->GetObject("MvdHit"));
  if (nullptr != fMvdHits) {
    fMvdHitMatches =
      static_cast<TClonesArray*>(ioman->GetObject("MvdHitMatch"));
    if (nullptr == fMvdHitMatches) {
      fMvdHitMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("MvdHitMatch",
                      "MVD",
                      fMvdHitMatches,
                      IsOutputBranchPersistent("MvdHitMatch"));
    }
  }

  fIsMvdActive = (fMvdPoints || fMvdCluster || fMvdHits);

  // Currently in the time-based mode MVD is present but not reconstructed
  // TODO: remove the check once the reconstruction works
  if (fIsMvdActive && !fMvdHits) {
    LOG(warning) << "CbmMatchRecoToMC: MVD hits are missing, MVD will not be "
                    "included to the STS track match";
    fIsMvdActive = false;
  }


  // TOF
  fTofPoints         = mcManager->InitBranch("TofPoint");
  fTofHitDigiMatches = (TClonesArray*) ioman->GetObject("TofHitDigiMatch");

  fTofHits = static_cast<TClonesArray*>(ioman->GetObject("TofHit"));
  if (nullptr != fTofHits) {
    fTofHitMatches =
      static_cast<TClonesArray*>(ioman->GetObject("TofHitMatch"));
    if (nullptr == fTofHitMatches) {
      fTofHitMatches = new TClonesArray("CbmMatch", 100);
      ioman->Register("TofHitMatch",
                      "TOF",
                      fTofHitMatches,
                      IsOutputBranchPersistent("TofHitMatch"));
    }
  }
}


void CbmMatchRecoToMC::MatchClusters(const TClonesArray* digiMatches,
                                     const TClonesArray* clusters,
                                     TClonesArray* clusterMatches) {
  if (!(digiMatches && clusters && clusterMatches)) return;
  Int_t nofClusters = clusters->GetEntriesFast();
  for (Int_t iCluster = 0; iCluster < nofClusters; iCluster++) {
    CbmCluster* cluster    = static_cast<CbmCluster*>(clusters->At(iCluster));
    CbmMatch* clusterMatch = new ((*clusterMatches)[iCluster]) CbmMatch();
    Int_t nofDigis         = cluster->GetNofDigis();
    for (Int_t iDigi = 0; iDigi < nofDigis; iDigi++) {
      const CbmMatch* digiMatch =
        static_cast<const CbmMatch*>(digiMatches->At(cluster->GetDigi(iDigi)));
      clusterMatch->AddLinks(*digiMatch);
    }  //# digis in cluster
  }    //# clusters
}

void CbmMatchRecoToMC::MatchClusters(ECbmModuleId systemId,
                                     const TClonesArray* clusters,
                                     TClonesArray* clusterMatches) {
  if (!fDigiManager->IsMatchPresent(systemId)) return;
  if (!(clusters && clusterMatches)) return;
  Int_t nofClusters = clusters->GetEntriesFast();
  for (Int_t iCluster = 0; iCluster < nofClusters; iCluster++) {
    CbmCluster* cluster    = static_cast<CbmCluster*>(clusters->At(iCluster));
    CbmMatch* clusterMatch = new ((*clusterMatches)[iCluster]) CbmMatch();
    Int_t nofDigis         = cluster->GetNofDigis();
    for (Int_t iDigi = 0; iDigi < nofDigis; iDigi++) {
      Int_t digiIndex           = cluster->GetDigi(iDigi);
      const CbmMatch* digiMatch = fDigiManager->GetMatch(systemId, digiIndex);
      clusterMatch->AddLinks(*digiMatch);
    }  //# digis in cluster
  }    //# clusters
}

void CbmMatchRecoToMC::MatchHits(const TClonesArray* matches,
                                 const TClonesArray* hits,
                                 TClonesArray* hitMatches) {
  if (!(matches && hits && hitMatches)) return;
  Int_t nofHits = hits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofHits; iHit++) {
    CbmHit* hit        = static_cast<CbmHit*>(hits->At(iHit));
    CbmMatch* hitMatch = new ((*hitMatches)[iHit]) CbmMatch();
    const CbmMatch* clusterMatch =
      static_cast<const CbmMatch*>(matches->At(hit->GetRefId()));
    hitMatch->AddLinks(*clusterMatch);
  }
}

void CbmMatchRecoToMC::MatchHitsSts(const TClonesArray* cluMatches,
                                    const TClonesArray* hits,
                                    TClonesArray* hitMatches) {
  if (!(cluMatches && hits && hitMatches)) return;
  Int_t nofHits = hits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofHits; iHit++) {
    CbmStsHit* hit     = static_cast<CbmStsHit*>(hits->At(iHit));
    CbmMatch* hitMatch = new ((*hitMatches)[iHit]) CbmMatch();
    const CbmMatch* frontClusterMatch =
      static_cast<const CbmMatch*>(cluMatches->At(hit->GetFrontClusterId()));
    const CbmMatch* backClusterMatch =
      static_cast<const CbmMatch*>(cluMatches->At(hit->GetBackClusterId()));
    hitMatch->AddLinks(*frontClusterMatch);
    hitMatch->AddLinks(*backClusterMatch);
  }
}

void CbmMatchRecoToMC::MatchHitsMvd(const TClonesArray* hits,
                                    TClonesArray* hitMatches) {
  if (!(hits && hitMatches)) return;
  Int_t nofHits = hits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofHits; iHit++) {
    CbmPixelHit* hit   = static_cast<CbmPixelHit*>(hits->At(iHit));
    CbmMatch* hitMatch = new ((*hitMatches)[iHit]) CbmMatch();
    const CbmMatch* digiMatch =
      fDigiManager->GetMatch(ECbmModuleId::kMvd, hit->GetRefId());
    hitMatch->AddLinks(*digiMatch);
  }
}

void CbmMatchRecoToMC::MatchHitsTof(const TClonesArray* HitDigiMatches,
                                    const TClonesArray* hits,
                                    TClonesArray* hitMatches) {
  if (!(HitDigiMatches && hits && hitMatches)) return;

  Int_t iNbTofDigis = fDigiManager->GetNofDigis(ECbmModuleId::kTof);
  Int_t nofHits     = hits->GetEntriesFast();
  const CbmTofDigi* pTofDigi;
  const CbmMatch* pMatchDigiPnt;

  for (Int_t iHit = 0; iHit < nofHits; iHit++) {
    CbmMatch* hitDigiMatch = static_cast<CbmMatch*>(HitDigiMatches->At(iHit));
    CbmMatch* hitMatch     = new ((*hitMatches)[iHit]) CbmMatch();

    Int_t iNbDigisHit = hitDigiMatch->GetNofLinks();
    for (Int_t iDigi = 0; iDigi < iNbDigisHit; iDigi++) {
      CbmLink lDigi  = hitDigiMatch->GetLink(iDigi);
      Int_t iDigiIdx = lDigi.GetIndex();

      if (iNbTofDigis <= iDigiIdx) {
        LOG(error)
          << "CbmTofHitFinderQa::FillHistos => Digi index from Hit #" << iHit
          << " is bigger than nb entries in Digis arrays => ignore it!!!";
        continue;
      }  // if( iNbTofDigis <= iDigiIdx )

      pTofDigi      = fDigiManager->Get<CbmTofDigi>(iDigiIdx);
      pMatchDigiPnt = fDigiManager->GetMatch(ECbmModuleId::kTof, iDigiIdx);
      Int_t iNbPointsDigi = pMatchDigiPnt->GetNofLinks();
      CbmLink lTruePoint =
        pMatchDigiPnt->GetMatchedLink();  // Point generating the Digi
      Int_t iTruePointIdx = lTruePoint.GetIndex();
      for (Int_t iPoint = 0; iPoint < iNbPointsDigi; iPoint++) {
        CbmLink lPoint  = pMatchDigiPnt->GetLink(iPoint);
        Int_t iPointIdx = lPoint.GetIndex();

        if (iPointIdx == iTruePointIdx)
          hitMatch->AddLink(
            CbmLink(pTofDigi->GetTot(),
                    iPointIdx,
                    lPoint.GetEntry(),
                    lPoint.GetFile()));  // Point generating the Digi
        else
          hitMatch->AddLink(CbmLink(
            0,
            iPointIdx,
            lPoint.GetEntry(),
            lPoint.GetFile()));  // Point whose Digi was hidden by True one
      }  // for( Int_t iPoint = 0; iPoint < iNbPointsDigi; iPoint ++)
    }    // for (Int_t iDigi = 0; iDigi < iNbDigisHit; iDigi++)
  }      // for (Int_t iHit = 0; iHit < nofHits; iHit++)
}

void CbmMatchRecoToMC::MatchHitsToPoints(CbmMCDataArray* points,
                                         const TClonesArray* hits,
                                         TClonesArray* hitMatches) {
  if (!(hits && hitMatches)) return;
  Int_t nofHits = hits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofHits; iHit++) {
    CbmHit* hit              = static_cast<CbmHit*>(hits->At(iHit));
    CbmMatch* hitMatch       = new ((*hitMatches)[iHit]) CbmMatch();
    const FairMCPoint* point = static_cast<const FairMCPoint*>(
      points->Get(0, fEventNumber, hit->GetRefId()));
    hitMatch->AddLink(
      CbmLink(point->GetEnergyLoss(), hit->GetRefId(), fEventNumber));
  }
}

void CbmMatchRecoToMC::MatchTracks(const TClonesArray* hitMatches,
                                   CbmMCDataArray* points,
                                   const TClonesArray* tracks,
                                   TClonesArray* trackMatches) {
  if (!(hitMatches && points && tracks && trackMatches)) return;

  Bool_t editMode = (trackMatches->GetEntriesFast() != 0);

  Int_t nofTracks = tracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofTracks; iTrack++) {
    const CbmTrack* track = static_cast<const CbmTrack*>(tracks->At(iTrack));
    CbmTrackMatchNew* trackMatch =
      (editMode) ? static_cast<CbmTrackMatchNew*>(trackMatches->At(iTrack))
                 : new ((*trackMatches)[iTrack]) CbmTrackMatchNew();
    Int_t nofHits = track->GetNofHits();
    for (Int_t iHit = 0; iHit < nofHits; iHit++) {
      if ((track->GetHitType(iHit) == kMUCHPIXELHIT)
          || (track->GetHitType(iHit) == kMUCHSTRAWHIT
              && hitMatches == fMuchPixelHitMatches))
        continue;  //AZ
      const CbmMatch* hitMatch =
        static_cast<CbmMatch*>(hitMatches->At(track->GetHitIndex(iHit)));
      Int_t nofLinks = hitMatch->GetNofLinks();
      for (Int_t iLink = 0; iLink < nofLinks; iLink++) {
        const FairMCPoint* point = static_cast<const FairMCPoint*>(
          points->Get(hitMatch->GetLink(iLink)));
        if (nullptr == point) continue;
        ////fix low energy cut case on STS
        if (CbmStsAddress::GetSystemId(point->GetDetectorID())
            == ECbmModuleId::kSts) {
          Int_t mcTrackId = point->GetTrackID();
          CbmMCTrack* mcTrack =
            (CbmMCTrack*) fMCTracks->Get(hitMatch->GetLink(iLink).GetFile(),
                                         hitMatch->GetLink(iLink).GetEntry(),
                                         mcTrackId);
          if (mcTrack->GetNPoints(ECbmModuleId::kSts) < 2) continue;
        }
        ////
        trackMatch->AddLink(CbmLink(
          1., point->GetTrackID(), hitMatch->GetLink(iLink).GetEntry()));
      }
    }
    if (!trackMatch->GetNofLinks()) continue;
    // Calculate number of true and wrong hits
    Int_t trueCounter  = trackMatch->GetNofTrueHits();
    Int_t wrongCounter = trackMatch->GetNofWrongHits();
    for (Int_t iHit = 0; iHit < nofHits; iHit++) {
      if ((track->GetHitType(iHit) == kMUCHPIXELHIT)
          || (track->GetHitType(iHit) == kMUCHSTRAWHIT
              && hitMatches == fMuchPixelHitMatches))
        continue;  //AZ
      const CbmMatch* hitMatch =
        static_cast<CbmMatch*>(hitMatches->At(track->GetHitIndex(iHit)));
      Int_t nofLinks = hitMatch->GetNofLinks();
      Bool_t hasTrue = false;
      for (Int_t iLink = 0; iLink < nofLinks; iLink++) {
        const FairMCPoint* point = static_cast<const FairMCPoint*>(
          points->Get(hitMatch->GetLink(iLink)));
        if (nullptr == point) continue;
        if (
          /*point->GetEventID() == trackMatch->GetMatchedLink().GetEntry() && */
          point->GetTrackID() == trackMatch->GetMatchedLink().GetIndex()) {
          hasTrue = true;
          break;
        }
      }
      if (hasTrue)
        trueCounter++;
      else
        wrongCounter++;
    }
    trackMatch->SetNofTrueHits(trueCounter);
    trackMatch->SetNofWrongHits(wrongCounter);
    // LOG(debug) << iTrack << " "; track->Print(); LOG(debug) << " " << trackMatch->ToString();
  }
}

void CbmMatchRecoToMC::MatchStsTracks(const TClonesArray* mvdHitMatches,
                                      const TClonesArray* stsHitMatches,
                                      CbmMCDataArray* mvdPoints,
                                      CbmMCDataArray* stsPoints,
                                      const TClonesArray* tracks,
                                      TClonesArray* trackMatches) {
  if (!tracks) { return; }

  if (!(stsHitMatches && stsPoints && tracks && trackMatches)) {
    LOG(error) << "CbmMatchRecoToMC:  missing the necessary STS info for the "
                  "STS track match";
    return;
  }

  if (fIsMvdActive && !(mvdHitMatches && mvdPoints)) {
    LOG(error) << "CbmMatchRecoToMC:  missing the necessary MVD info for the "
                  "STS track match";
    return;
  }

  // make sure that the Mvd hits are not present in the reconstruced tracks
  // when we assume that the Mvd reconstruction was disabled
  if (!fIsMvdActive) {
    for (Int_t iTrack = 0; iTrack < trackMatches->GetEntriesFast(); iTrack++) {
      const CbmStsTrack* track = static_cast<const CbmStsTrack*>(tracks->At(iTrack));
      if (track->GetNofMvdHits() > 0) {
        LOG(error) << "CbmMatchRecoToMC: Sts track contains Mvd hits, but "
                      "there is no MVD data available";
        return;
      }
    }
  }

  Bool_t editMode = (trackMatches->GetEntriesFast() != 0);

  Int_t nofTracks = tracks->GetEntriesFast();

  for (Int_t iTrack = 0; iTrack < nofTracks; iTrack++) {
    const CbmStsTrack* track =
      static_cast<const CbmStsTrack*>(tracks->At(iTrack));
    CbmTrackMatchNew* trackMatch =
      (editMode) ? static_cast<CbmTrackMatchNew*>(trackMatches->At(iTrack))
                 : new ((*trackMatches)[iTrack]) CbmTrackMatchNew();

    Int_t nofStsHits = track->GetNofStsHits();
    for (Int_t iHit = 0; iHit < nofStsHits; iHit++) {
      const CbmMatch* hitMatch =
        static_cast<CbmMatch*>(stsHitMatches->At(track->GetHitIndex(iHit)));
      Int_t nofLinks = hitMatch->GetNofLinks();
      for (Int_t iLink = 0; iLink < nofLinks; iLink++) {
        const CbmLink& link = hitMatch->GetLink(iLink);
        const FairMCPoint* point =
          static_cast<const FairMCPoint*>(stsPoints->Get(link));
        assert(point);
        /// fix low energy cut case on STS
        if (CbmStsAddress::GetSystemId(point->GetDetectorID())
            == ECbmModuleId::kSts) {
          Int_t mcTrackId     = point->GetTrackID();
          CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->Get(
            link.GetFile(), link.GetEntry(), mcTrackId);
          assert(mcTrack);
          if (mcTrack->GetNPoints(ECbmModuleId::kSts) < 2) { continue; }
        }
        ///
        trackMatch->AddLink(
          CbmLink(1., point->GetTrackID(), link.GetEntry(), link.GetFile()));
      }
    }

    // Include MVD hits in STS matching if MVD is active
    Int_t nofMvdHits = track->GetNofMvdHits();
    for (Int_t iHit = 0; iHit < nofMvdHits; iHit++) {
      const CbmMatch* hitMatch = static_cast<CbmMatch*>(mvdHitMatches->At(track->GetMvdHitIndex(iHit)));
      Int_t nofLinks           = hitMatch->GetNofLinks();
      for (Int_t iLink = 0; iLink < nofLinks; iLink++) {
        const CbmLink& link      = hitMatch->GetLink(iLink);
        const FairMCPoint* point = static_cast<const FairMCPoint*>(mvdPoints->Get(link));
        assert(point);
        trackMatch->AddLink(CbmLink(1., point->GetTrackID(), link.GetEntry(), link.GetFile()));
      }
    }

    if (!trackMatch->GetNofLinks()) { continue; }

    // Calculate number of true and wrong hits
    Int_t trueCounter  = trackMatch->GetNofTrueHits();
    Int_t wrongCounter = trackMatch->GetNofWrongHits();
    for (Int_t iHit = 0; iHit < nofStsHits; iHit++) {
      const CbmMatch* hitMatch =
        static_cast<CbmMatch*>(stsHitMatches->At(track->GetHitIndex(iHit)));
      Int_t nofLinks = hitMatch->GetNofLinks();
      Bool_t hasTrue = false;
      for (Int_t iLink = 0; iLink < nofLinks; iLink++) {
        const FairMCPoint* point = static_cast<const FairMCPoint*>(
          stsPoints->Get(hitMatch->GetLink(iLink)));
        if (nullptr == point) { continue; }
        if (point->GetTrackID() == trackMatch->GetMatchedLink().GetIndex()) {
          hasTrue = true;
          break;
        }
      }
      if (hasTrue) { trueCounter++; }
      else {
        wrongCounter++;
      }
    }

    // Include MVD hits in STS matching if MVD is active
    for (Int_t iHit = 0; iHit < nofMvdHits; iHit++) {
      const CbmMatch* hitMatch = static_cast<CbmMatch*>(mvdHitMatches->At(track->GetMvdHitIndex(iHit)));
      Int_t nofLinks           = hitMatch->GetNofLinks();
      Bool_t hasTrue           = false;
      for (Int_t iLink = 0; iLink < nofLinks; iLink++) {
        const FairMCPoint* point = static_cast<const FairMCPoint*>(mvdPoints->Get(hitMatch->GetLink(iLink)));
        if (nullptr == point) { continue; }
        if (
          /*point->GetEventID() == trackMatch->GetMatchedLink().GetEntry() && */
          point->GetTrackID() == trackMatch->GetMatchedLink().GetIndex()) {
          hasTrue = true;
          break;
        }
      }
      if (hasTrue) { trueCounter++; }
      else {
        wrongCounter++;
      }
    }

    trackMatch->SetNofTrueHits(trueCounter);
    trackMatch->SetNofWrongHits(wrongCounter);
    // LOG(debug) << iTrack << " "; track->Print(); LOG(debug) << " " << trackMatch->ToString();
  }
}


void CbmMatchRecoToMC::MatchRichRings(const TClonesArray* richRings,
                                      const TClonesArray* richHits,
                                      // const TClonesArray* richDigis,
                                      // const TClonesArray* richDigiMatches,
                                      CbmMCDataArray* richMcPoints,
                                      CbmMCDataArray* mcTracks,
                                      TClonesArray* ringMatches) {
  // Loop over RichRings
  Int_t nRings = richRings->GetEntriesFast();
  for (Int_t iRing = 0; iRing < nRings; iRing++) {
    const CbmRichRing* ring =
      static_cast<const CbmRichRing*>(richRings->At(iRing));
    if (nullptr == ring) continue;

    CbmTrackMatchNew* ringMatch =
      new ((*ringMatches)[iRing]) CbmTrackMatchNew();

    Int_t nofHits = ring->GetNofHits();
    for (Int_t iHit = 0; iHit < nofHits; iHit++) {
      const CbmRichHit* hit =
        static_cast<const CbmRichHit*>(richHits->At(ring->GetHit(iHit)));
      if (nullptr == hit) continue;

      vector<pair<Int_t, Int_t>> motherIds = GetMcTrackMotherIdsForRichHit(
        fDigiManager, hit, richMcPoints, mcTracks, fEventNumber);
      for (UInt_t i = 0; i < motherIds.size(); i++) {
        ringMatch->AddLink(1., motherIds[i].second, motherIds[i].first);
      }
    }

    if (ringMatch->GetNofLinks() != 0) {

      Int_t bestTrackId      = ringMatch->GetMatchedLink().GetIndex();
      Int_t bestTrackEventId = ringMatch->GetMatchedLink().GetEntry();

      Int_t trueCounter  = 0;
      Int_t wrongCounter = 0;
      for (Int_t iHit = 0; iHit < nofHits; iHit++) {
        const CbmRichHit* hit =
          static_cast<const CbmRichHit*>(richHits->At(ring->GetHit(iHit)));
        if (nullptr == hit) continue;
        vector<pair<Int_t, Int_t>> motherIds = GetMcTrackMotherIdsForRichHit(
          fDigiManager, hit, richMcPoints, mcTracks, fEventNumber);
        if (std::find(motherIds.begin(),
                      motherIds.end(),
                      std::make_pair(bestTrackEventId, bestTrackId))
            != motherIds.end()) {
          trueCounter++;
        } else {
          wrongCounter++;
        }
      }

      ringMatch->SetNofTrueHits(trueCounter);
      ringMatch->SetNofWrongHits(wrongCounter);
    } else {
      ringMatch->SetNofTrueHits(0);
      ringMatch->SetNofWrongHits(0);
    }

  }  // Ring loop
}


vector<pair<Int_t, Int_t>>
CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(CbmDigiManager* digiMan,
                                                const CbmRichHit* hit,
                                                CbmMCDataArray* richPoints,
                                                CbmMCDataArray* mcTracks,
                                                Int_t /*eventNumber*/) {
  vector<pair<Int_t, Int_t>> result;
  if (nullptr == hit) return result;
  Int_t digiIndex = hit->GetRefId();
  if (digiIndex < 0) return result;
  const CbmRichDigi* digi = digiMan->Get<CbmRichDigi>(digiIndex);
  if (nullptr == digi) return result;
  const CbmMatch* digiMatch = digiMan->GetMatch(ECbmModuleId::kRich, digiIndex);
  if (digiMatch == nullptr) return result;

  vector<CbmLink> links = digiMatch->GetLinks();
  for (UInt_t i = 0; i < links.size(); i++) {
    Int_t pointId = links[i].GetIndex();
    Int_t eventId = links[i].GetEntry();
    if (pointId < 0) continue;  // noise hit
    const CbmRichPoint* pMCpt =
      static_cast<const CbmRichPoint*>(richPoints->Get(0, eventId, pointId));

    if (nullptr == pMCpt) continue;
    Int_t mcTrackIndex = pMCpt->GetTrackID();
    if (mcTrackIndex < 0) continue;
    //TODO: Currently we support only legacy mode of CbmMCDataArray
    const CbmMCTrack* mcTrack =
      static_cast<const CbmMCTrack*>(mcTracks->Get(0, eventId, mcTrackIndex));
    // CbmMCTrack* pMCtr = (CbmMCTrack*) mcTracks->At(mcTrackIndex);
    if (nullptr == mcTrack) continue;
    if (mcTrack->GetPdgCode() != 50000050)
      continue;  // select only Cherenkov photons
    Int_t motherId = mcTrack->GetMotherId();
    // several photons can have same mother track
    // count only unique motherIds
    pair<Int_t, Int_t> val = std::make_pair(eventId, motherId);
    if (std::find(result.begin(), result.end(), val) == result.end()) {
      result.push_back(val);
    }
  }

  return result;
}

vector<Int_t>
CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(CbmDigiManager* digiMan,
                                                const CbmRichHit* hit,
                                                const TClonesArray* richPoints,
                                                const TClonesArray* mcTracks) {
  vector<Int_t> result;
  if (nullptr == hit) return result;
  Int_t digiIndex = hit->GetRefId();
  if (digiIndex < 0) return result;
  const CbmRichDigi* digi = digiMan->Get<CbmRichDigi>(digiIndex);
  if (nullptr == digi) return result;
  const CbmMatch* digiMatch = digiMan->GetMatch(ECbmModuleId::kRich, digiIndex);
  if (digiMatch == nullptr) return result;

  vector<CbmLink> links = digiMatch->GetLinks();
  for (UInt_t i = 0; i < links.size(); i++) {
    Int_t pointId = links[i].GetIndex();
    if (pointId < 0) continue;  // noise hit

    const CbmRichPoint* pMCpt =
      static_cast<const CbmRichPoint*>(richPoints->At(pointId));
    if (nullptr == pMCpt) continue;
    Int_t mcTrackIndex = pMCpt->GetTrackID();
    if (mcTrackIndex < 0) continue;
    //TODO: Currently we support only legacy mode of CbmMCDataArray
    const CbmMCTrack* mcTrack =
      static_cast<const CbmMCTrack*>(mcTracks->At(mcTrackIndex));
    // CbmMCTrack* pMCtr = (CbmMCTrack*) mcTracks->At(mcTrackIndex);
    if (nullptr == mcTrack) continue;
    if (mcTrack->GetPdgCode() != 50000050)
      continue;  // select only Cherenkov photons
    Int_t motherId = mcTrack->GetMotherId();
    // several photons can have same mother track
    // count only unique motherIds
    if (std::find(result.begin(), result.end(), motherId) == result.end()) {
      result.push_back(motherId);
    }
  }

  return result;
}

ClassImp(CbmMatchRecoToMC);
