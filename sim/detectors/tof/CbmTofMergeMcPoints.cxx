/* Copyright (C) 2016-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/*
 * CbmTofMergeMcPoints.cxx
 *
 *  Created on: Jan 08, 2016
 *      Author: P.-A. Loizeau
 */

#include "CbmTofMergeMcPoints.h"

// TOF Classes and includes
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofPoint.h"            // in cbmdata/tof

// CBMroot classes and includes
#include "CbmMCTrack.h"
#include "CbmMatch.h"

// FAIR classes and includes
#include "FairRootManager.h"
#include <Logger.h>

// ROOT Classes and includes
#include "TClonesArray.h"

#include "Riostream.h"

CbmTofMergeMcPoints::CbmTofMergeMcPoints()
  : FairTask()
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fMcTracksColl(NULL)
  , fTofPointsColl(NULL)
  , fTofPntTrkMap()
  , fRealTofPoints(NULL)
  , fTofRealPntMatches(NULL)
{
}

CbmTofMergeMcPoints::~CbmTofMergeMcPoints()
{
  if (fRealTofPoints != NULL) {
    fRealTofPoints->Delete();
    delete fRealTofPoints;
  }

  if (fTofRealPntMatches != NULL) {
    fTofRealPntMatches->Delete();
    delete fTofRealPntMatches;
  }
}

InitStatus CbmTofMergeMcPoints::Init()
{
  ReadAndCreateDataBranches();

  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  Int_t iGeoVersion   = fGeoHandler->Init(isSimulation);
  LOG(info) << "CbmTofMergeMcPoints::Init with GeoVersion " << iGeoVersion;

  if (k12b > iGeoVersion) {
    LOG(error) << "CbmTofMergeMcPoints::Init => Only compatible with "
                  "geometries after v12b !!!";
    return kFATAL;
  }  // if( k12b > iGeoVersion )

  fTofId = fGeoHandler->GetDetIdPointer();

  if (NULL != fTofId) LOG(info) << "CbmTofMergeMcPoints::Init with GeoVersion " << fGeoHandler->GetGeoVersion();
  else {
    switch (iGeoVersion) {
      case k12b: fTofId = new CbmTofDetectorId_v12b(); break;
      case k14a: fTofId = new CbmTofDetectorId_v14a(); break;
      default: LOG(error) << "CbmTofMergeMcPoints::Init => Invalid geometry!!!" << iGeoVersion; return kFATAL;
    }  // switch(iGeoVersion)
  }    // else of if(NULL != fTofId)

  return kSUCCESS;
}

void CbmTofMergeMcPoints::Exec(Option_t* /*opt*/)
{
  if (fRealTofPoints != NULL) fRealTofPoints->Delete();
  if (fTofRealPntMatches != NULL) fTofRealPntMatches->Delete();
  ;
  // TOF: (MC)=>(Realistic MC) & (MC->RealisticMC)
  MergeRealisticTofPoints(fMcTracksColl, fTofPointsColl, fRealTofPoints, fTofRealPntMatches);

  static Int_t eventNo = 0;
  LOG(info) << "CbmTofMergeMcPoints::Exec eventNo=" << eventNo++;
}

void CbmTofMergeMcPoints::Finish() {}

void CbmTofMergeMcPoints::ReadAndCreateDataBranches()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman)
    LOG(fatal) << "CbmTofMergeMcPoints::ReadAndCreateDataBranches() NULL "
                  "FairRootManager.";

  // TOF
  fMcTracksColl = (TClonesArray*) ioman->GetObject("MCTrack");
  if (NULL == fMcTracksColl) {
    LOG(fatal) << "CbmTofMergeMcPoints::ReadAndCreateDataBranches => Could not "
                  "get the MCTrack TClonesArray!!!";
  }  // if( NULL == fMcTracksColl)

  fTofPointsColl = (TClonesArray*) ioman->GetObject("TofPoint");
  if (NULL == fTofPointsColl) {
    LOG(fatal) << "CbmTofMergeMcPoints::ReadAndCreateDataBranches => Could not "
                  "get the TofPoint TClonesArray!!!";
  }  // if( NULL == fTofPointsColl)

  if (NULL != fTofPointsColl) {
    fRealTofPoints = new TClonesArray("CbmTofPoint", 100);
    ioman->Register("RealisticTofPoint", "TOF", fRealTofPoints, IsOutputBranchPersistent("RealisticTofPoint"));
    fTofRealPntMatches = new TClonesArray("CbmMatch", 100);
    ioman->Register("TofRealPntMatch", "TOF", fTofRealPntMatches, IsOutputBranchPersistent("TofRealPntMatch"));
  }
}

void CbmTofMergeMcPoints::MergeRealisticTofPoints(const TClonesArray* tracks, const TClonesArray* points,
                                                  TClonesArray* realisticPoints, TClonesArray* pointsMatches)
{
  if (!(points && realisticPoints && pointsMatches)) return;

  Int_t iNbTracks    = tracks->GetEntriesFast();
  Int_t iNbTofPoints = points->GetEntriesFast();

  CbmMCTrack* pMcTrk = NULL;
  CbmTofPoint* pPnt  = NULL;
  Int_t iTrackId     = 0;
  Int_t iDetId       = 0;
  Int_t iModType     = 0;
  Int_t iModule      = 0;
  Int_t iCounter     = 0;

  // Initial loop to fill one pair per track with TOF points in the map
  for (Int_t iTrk = 0; iTrk < iNbTracks; iTrk++) {
    pMcTrk = (CbmMCTrack*) tracks->At(iTrk);

    if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof))
      fTofPntTrkMap.insert(std::pair<Int_t, std::vector<Int_t>>(iTrk, std::vector<Int_t>()));
  }  // for (Int_t iTrk = 0; iTrk < iNbTofPoints; iTrk++)

  // Prepare the vector to keep track of which mean TOF Point comes
  // from each TofPoint
  // Maybe more efficient to use std::list instead of std::vector as
  // removinge elements inside the array later?
  std::vector<Int_t> vMeanIdPnts(iNbTofPoints, -1);

  // Initial loop to fill the vectors and order the points by tracks
  for (Int_t iPnt = 0; iPnt < iNbTofPoints; iPnt++) {
    pPnt = static_cast<CbmTofPoint*>(points->At(iPnt));

    iTrackId = pPnt->GetTrackID();

    fTofPntTrkMap[iTrackId].push_back(iPnt);
  }  // for (Int_t iPnt = 0; iPnt < iNbTofPoints; iPnt++)

  // Second loop to generate the mean MC point per (track, det) pair
  // and fill them in the output arrays
  Int_t iMeanMcPointId = 0;
  Int_t iMeanChannel   = 0;  // mean channel index
  Double_t dMeanPosX   = 0;
  Double_t dMeanPosY   = 0;
  Double_t dMeanPosZ   = 0;
  Double_t dMeanTime   = 0;
  Double_t dMeanMomX   = 0;
  Double_t dMeanMomY   = 0;
  Double_t dMeanMomZ   = 0;
  Double_t dMeanLen    = 0;
  Double_t dTotELoss   = 0;
  Int_t iNbPntInMean   = 0;
  Int_t iMeanModType   = -1;
  Int_t iMeanModule    = -1;
  Int_t iMeanCounter   = -1;
  for (std::map<Int_t, std::vector<Int_t>>::iterator it = fTofPntTrkMap.begin(); it != fTofPntTrkMap.end(); ++it) {
    //      std::vector< Int_t > vTofPnt = it->second;

    // Each pair associate a track ID with the list of id for its corresponding TofPoint
    while (0 < (it->second).size()) {
      // A - Start storing the info for the mean MC point with the first
      //     point
      //     Keep track of current mean MC Point index
      Int_t iPntIdxList = (it->second).size() - 1;
      pPnt              = static_cast<CbmTofPoint*>(points->At((it->second)[iPntIdxList]));
      iDetId            = pPnt->GetDetectorID();

      // First store the info identifying the counter
      iMeanModType = fGeoHandler->GetSMType(iDetId);
      iMeanModule  = fGeoHandler->GetSModule(iDetId);
      iMeanCounter = fGeoHandler->GetCounter(iDetId);
      // Then store the MC Points information
      iNbPntInMean = 1;
      iMeanChannel = fGeoHandler->GetCell(iDetId);  // mean channel index
      dMeanPosX    = pPnt->GetX();
      dMeanPosY    = pPnt->GetY();
      dMeanPosZ    = pPnt->GetZ();
      dMeanTime    = pPnt->GetTime();
      dMeanMomX    = pPnt->GetPx();
      dMeanMomY    = pPnt->GetPy();
      dMeanMomZ    = pPnt->GetPz();
      dMeanLen     = pPnt->GetLength();
      dTotELoss    = pPnt->GetEnergyLoss();
      // Keep track of mean Point index match
      vMeanIdPnts[(it->second)[iPntIdxList]] = iMeanMcPointId;
      // Get rid of the point index in the list
      (it->second).pop_back();

      // B - Scan the remaining points to find the ones belonging to
      //     the same RPC counter
      //     Fill in parallel the vector keeping track of the match
      iPntIdxList = (it->second).size() - 1;
      while (0 <= iPntIdxList && 0 < (it->second).size()) {
        pPnt = static_cast<CbmTofPoint*>(points->At((it->second)[iPntIdxList]));

        // First access the info identifying the counter
        iDetId   = pPnt->GetDetectorID();
        iModType = fGeoHandler->GetSMType(iDetId);
        iModule  = fGeoHandler->GetSModule(iDetId);
        iCounter = fGeoHandler->GetCounter(iDetId);

        if ((iMeanModType == iModType) && (iMeanModule == iModule) && (iMeanCounter == iCounter)) {
          // Then store the MC Points information if counter match
          iNbPntInMean++;
          iMeanChannel += fGeoHandler->GetCell(iDetId);  // mean channel index
          dMeanPosX += pPnt->GetX();
          dMeanPosY += pPnt->GetY();
          dMeanPosZ += pPnt->GetZ();
          dMeanTime += pPnt->GetTime();
          dMeanMomX += pPnt->GetPx();
          dMeanMomY += pPnt->GetPy();
          dMeanMomZ += pPnt->GetPz();
          dMeanLen += pPnt->GetLength();
          dTotELoss += pPnt->GetEnergyLoss();

          // Keep track of mean Point index match
          vMeanIdPnts[(it->second)[iPntIdxList]] = iMeanMcPointId;

          // Get rid of the point index in the list
          (it->second).erase((it->second).begin() + iPntIdxList);
          iPntIdxList = (it->second).size() - 1;
        }  // if same sounter as first point
        else {
          iPntIdxList--;  // just got to the next point in the list
        }                 // if different conter as first point
      }                   // while( 0 <= iPntIdxList && 0 < (it->second).size() )

      // C - Create new mean MC Point
      // First do the mean
      iMeanChannel /= iNbPntInMean;  // mean channel index
      dMeanPosX /= iNbPntInMean;
      dMeanPosY /= iNbPntInMean;
      dMeanPosZ /= iNbPntInMean;
      dMeanTime /= iNbPntInMean;
      dMeanMomX /= iNbPntInMean;
      dMeanMomY /= iNbPntInMean;
      dMeanMomZ /= iNbPntInMean;
      dMeanLen /= iNbPntInMean;
      // Store the new mean MC Point in the output TClonesArray
      CbmTofDetectorInfo detInfo(ECbmModuleId::kTof, iMeanModType, iMeanModule, iMeanCounter, 0, iMeanChannel);
      TVector3 meanPos(dMeanPosX, dMeanPosY, dMeanPosZ);
      TVector3 meanMom(dMeanMomX, dMeanMomY, dMeanMomZ);
      //         CbmTofPoint* pMeanPoint =
      new ((*realisticPoints)[iMeanMcPointId])
        CbmTofPoint(it->first, fTofId->SetDetectorInfo(detInfo), meanPos, meanMom, dMeanTime, dMeanLen, dTotELoss);
      // Update the index for the next mean Point
      iMeanMcPointId++;

    }  // while( 0 < (it->second).size() )
  }    // for (Int_t iPnt = 0; iPnt < iNbTofPoints; iPnt++)

  // For each input MC Point, create a CbmMatch object to keep track
  // of the corresponding Mean MC TOF point and store it in the ouptut
  // TClonesArray
  for (Int_t iPnt = 0; iPnt < iNbTofPoints; iPnt++) {

    CbmMatch* pntMatch = new ((*pointsMatches)[iPnt]) CbmMatch();
    // Add link storing the ID of the corresponding mean MC point
    pntMatch->AddLink(CbmLink(1.0, vMeanIdPnts[iPnt]));
  }  // for (Int_t iPnt = 0; iPnt < iNbTofPoints; iPnt++)
}
