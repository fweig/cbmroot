/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann, Florian Uhlig [committer] */

/** @file CbmTofCalibrator.cxx
 ** @author nh
 ** @date 28.02.2020
 **
 **/

// CBMroot classes and includes
#include "CbmTofCalibrator.h"

#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmTofAddress.h"  // in cbmdata/tof
#include "CbmTofCell.h"     // in tof/TofData
#include "CbmTofClusterizersDef.h"
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"
#include "CbmTofTracklet.h"

// FAIR classes and includes
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

// ROOT Classes and includes
#include <TClonesArray.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TFitResult.h>
#include <TGeoManager.h>
#include <TH1.h>
#include <TH2.h>
#include <TMinuit.h>
#include <TProfile.h>
#include <TROOT.h>

CbmTofCalibrator::CbmTofCalibrator()
  : fDigiMan(NULL)
  , fTofClusterizer(NULL)
  , fTofFindTracks(NULL)
  , fTrackletTools(NULL)
  , fDigiPar(NULL)
  , fDigiBdfPar(NULL)
  , fTofDigiMatchColl(NULL)
  , fhCalR0(NULL)
  , fhCalDX0(NULL)
  , fhCalDY0(NULL)
  , fhCalTot()
  , fhCalPosition()
  , fhCalPos()
  , fhCalTOff()
  , fhCalTofOff()
  , fhCalWalk()
  , fhCorPos()
  , fhCorTOff()
  , fhCorTot()
  , fhCorTotOff()
  , fhCorSvel()
  , fhCorWalk()
  , fDetIdIndexMap()
  , fhDoubletDt()
  , fhDoubletDd()
  , fhDoubletV()
{
}

CbmTofCalibrator::~CbmTofCalibrator() {}

InitStatus CbmTofCalibrator::Init()
{

  FairRootManager* fManager = FairRootManager::Instance();
  // Get access to TofCalDigis
  fTofCalDigiVec = fManager->InitObjectAs<std::vector<CbmTofDigi> const*>("TofCalDigi");
  //dynamic_cast<std::vector<CbmTofDigi> const*>(fManager->GetObject("TofCalDigi"));
  if (NULL == fTofCalDigiVec) LOG(warning) << "No access to TofCalDigis!";

  // check for availability of digis
  fDigiMan = CbmDigiManager::Instance();
  if (NULL == fDigiMan) {
    LOG(warning) << "No CbmDigiManager";
    return kFATAL;
  }
  fDigiMan->Init();

  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) { LOG(warn) << "CbmTofCalibrator: No digi input!"; }

  // check for access to current calibration file
  fTofClusterizer = CbmTofEventClusterizer::Instance();
  if (NULL == fTofClusterizer) { LOG(warn) << "CbmTofCalibrator: no access to active calibration"; }
  else {
    TString CalParFileName = fTofClusterizer->GetCalParFileName();
    LOG(info) << "Current calibration file: " << CalParFileName;
  }
  fTofFindTracks = CbmTofFindTracks::Instance();
  if (NULL == fTofFindTracks) { LOG(warn) << "CbmTofCalibrator: no access to tof tracker "; }

  fEvtHeader = (FairEventHeader*) fManager->GetObject("EventHeader.");
  if (NULL == fEvtHeader) {
    LOG(warning) << "CbmTofCalibrator::RegisterInputs => Could not get EvtHeader Object";
    return kFATAL;
  }
  // Get Access to MatchCollection
  fTofDigiMatchColl = (TClonesArray*) fManager->GetObject("TofHitCalDigiMatch");
  if (NULL == fTofDigiMatchColl) {
    LOG(warn) << "No branch TofHitCalDigiMatch found, try TofHitDigiMatch";
    fTofDigiMatchColl = (TClonesArray*) fManager->GetObject("TofHitDigiMatch");
  }

  if (NULL == fTofDigiMatchColl) {
    LOG(error) << "CbmTofCalibrator: no access to DigiMatch ";
    return kFATAL;
  }
  LOG(info) << Form("Got DigiMatchColl %s at %p", fTofDigiMatchColl->GetName(), fTofDigiMatchColl);

  // Get Parameters
  if (!InitParameters()) { LOG(error) << "CbmTofCalibrator: No parameters!"; }
  // generate deviation histograms
  if (!CreateCalHist()) {
    LOG(error) << "CbmTofCalibrator: No histograms!";
    return kFATAL;
  }

  fTrackletTools = new CbmTofTrackletTools();  // initialize tools

  LOG(info) << "TofCalibrator initialized successfully";
  return kSUCCESS;
}

Bool_t CbmTofCalibrator::InitParameters()
{
  LOG(info) << "InitParameters: get par pointers from RTDB";
  // Get Base Container
  FairRun* ana        = FairRun::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  if (NULL == fDigiPar) return kFALSE;

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
  if (NULL == fDigiBdfPar) return kFALSE;

  return kTRUE;
}

Bool_t CbmTofCalibrator::CreateCalHist()
{
  TDirectory* oldir = gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT->cd();                     // <= To prevent histos from being sucked in by the param file of the TRootManager !

  // detector related distributions
  Int_t iNbDet = fDigiBdfPar->GetNbDet();
  LOG(info) << "Define Calibrator histos for " << iNbDet << " detectors "
            << "in directory " << gDirectory->GetName() << ", old " << oldir->GetName();

  fhCalR0  = new TH1D("hCalR0", "Tracklet distance to nominal vertex; R_0 [cm]", 100, 0., 0.5);
  fhCalDX0 = new TH1D("hCalDX0", "Tracklet distance to nominal vertex; #DeltaX_0 [cm]", 100, -1.5, 1.5);
  fhCalDY0 = new TH1D("hCalDY0", "Tracklet distance to nominal vertex; #DeltaY_0 [cm]", 100, -1.5, 1.5);

  fhCalTot.resize(iNbDet);
  fhCalPosition.resize(iNbDet);
  fhCalPos.resize(iNbDet);
  fhCalTOff.resize(iNbDet);
  fhCalTofOff.resize(iNbDet);
  fhCalWalk.resize(iNbDet);

  fDetIdIndexMap.clear();

  for (Int_t iDetIndx = 0; iDetIndx < iNbDet; iDetIndx++) {
    Int_t iUniqueId           = fDigiBdfPar->GetDetUId(iDetIndx);
    fDetIdIndexMap[iUniqueId] = iDetIndx;

    Int_t iSmType            = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSmId              = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpcId             = CbmTofAddress::GetRpcId(iUniqueId);
    Int_t iUCellId           = CbmTofAddress::GetUniqueAddress(iSmId, iRpcId, 0, 0, iSmType);
    CbmTofCell* fChannelInfo = fDigiPar->GetCell(iUCellId);
    if (NULL == fChannelInfo) {
      LOG(warning) << "No DigiPar for Det " << Form("0x%08x", iUCellId);
      continue;
    }

    Double_t TotMax    = 20.;
    fhCalTot[iDetIndx] = new TH2F(
      Form("cal_SmT%01d_sm%03d_rpc%03d_Tot", iSmType, iSmId, iRpcId),
      Form("Clu Tot of Rpc #%03d in Sm %03d of type %d; Strip []; Tot [a.u.]", iRpcId, iSmId, iSmType),
      2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, 2 * fDigiBdfPar->GetNbChan(iSmType, iRpcId), 100, 0., TotMax);

    Double_t YDMAX = TMath::Max(2., fChannelInfo->GetSizey()) * 1.;
    fhCalPosition[iDetIndx] =
      new TH2F(Form("cal_SmT%01d_sm%03d_rpc%03d_Position", iSmType, iSmId, iRpcId),
               Form("Clu position of Rpc #%03d in Sm %03d of type %d; Strip []; ypos [cm]", iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 100, -YDMAX, YDMAX);

    YDMAX = 5;
    fhCalPos[iDetIndx] =
      new TH2F(Form("cal_SmT%01d_sm%03d_rpc%03d_Pos", iSmType, iSmId, iRpcId),
               Form("Clu pos deviation of Rpc #%03d in Sm %03d of type %d; "
                    "Strip []; ypos [cm]",
                    iRpcId, iSmId, iSmType),
               fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -YDMAX, YDMAX);

    Double_t TSumMax = 2.;
    //if(iSmType == 5) TSumMax *= 2.; // enlarge for diamond / beamcounter
    fhCalTOff[iDetIndx] = new TH2F(Form("cal_SmT%01d_sm%03d_rpc%03d_TOff", iSmType, iSmId, iRpcId),
                                   Form("Clu T0 deviation of Rpc #%03d in Sm %03d of type %d; "
                                        "Strip []; TOff [ns]",
                                        iRpcId, iSmId, iSmType),
                                   fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0, fDigiBdfPar->GetNbChan(iSmType, iRpcId),
                                   99, -TSumMax, TSumMax);
    TSumMax               = 50.;
    fhCalTofOff[iDetIndx] = new TH2F(Form("cal_SmT%01d_sm%03d_rpc%03d_TofOff", iSmType, iSmId, iRpcId),
                                     Form("Clu T0 deviation of Rpc #%03d in Sm %03d of type %d; "
                                          "Strip []; TofOff [ns]",
                                          iRpcId, iSmId, iSmType),
                                     fDigiBdfPar->GetNbChan(iSmType, iRpcId), 0,
                                     fDigiBdfPar->GetNbChan(iSmType, iRpcId), 99, -TSumMax, TSumMax);

    TSumMax = 0.8;
    fhCalWalk[iDetIndx].resize(fDigiBdfPar->GetNbChan(iSmType, iRpcId));
    for (Int_t iCh = 0; iCh < fDigiBdfPar->GetNbChan(iSmType, iRpcId); iCh++) {
      fhCalWalk[iDetIndx][iCh].resize(2);
      for (Int_t iSide = 0; iSide < 2; iSide++) {
        fhCalWalk[iDetIndx][iCh][iSide] =
          new TH2D(Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk", iSmType, iSmId, iRpcId, iCh, iSide),
                   Form("Walk in SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk; Tot "
                        "[a.u.];  #DeltaT [ns]",
                        iSmType, iSmId, iRpcId, iCh, iSide),
                   nbClWalkBinX, 0., TotMax, nbClWalkBinY, -TSumMax, TSumMax);
      }
    }
  }

  return kTRUE;
}

static Int_t NevtH = 0;
void CbmTofCalibrator::FillCalHist(CbmTofHit* pRef, Int_t /*iOpt*/, CbmEvent* /*tEvent*/)
{
  if (NULL == fTofCalDigiVec) LOG(fatal) << "No access to TofCalDigis!";

  NevtH++;
  double dTAv = 0.;
  if (pRef != NULL) dTAv = pRef->GetTime();
  else {
    if (-1 < fTofClusterizer->GetBeamAddr()) {  // look for beam counter
      if (NevtH == 1) LOG(info) << Form("FillCalHist: look for beam counter 0x%08x", fTofClusterizer->GetBeamAddr());
      Int_t iHit = 0;
      for (; iHit < (int) fTofClusterizer->GetNbHits(); iHit++) {
        CbmTofHit* pHit = fTofClusterizer->GetHitPointer(iHit);
        if (NULL == pHit) continue;
        if (pHit->GetAddress() == fTofClusterizer->GetBeamAddr()) {
          pRef = pHit;
          dTAv = pRef->GetTime();
          if (NevtH == 1) LOG(info) << "FillCalHist: found beam counter";
          break;
        }
      }
      if (iHit == fTofClusterizer->GetNbHits()) return;  // beam counter not present
    }
    else {
      double dNAv = 0.;
      for (Int_t iHit = 0; iHit < (int) fTofClusterizer->GetNbHits(); iHit++) {
        LOG(debug) << "GetHitPointer for " << iHit;
        CbmTofHit* pHit = fTofClusterizer->GetHitPointer(iHit);
        if (NULL == pHit) continue;
        dTAv += pHit->GetTime();
        dNAv++;
      }
      if (dNAv == 0) return;
      dTAv /= dNAv;
    }
  }

  for (Int_t iHit = 0; iHit < (int) fTofClusterizer->GetNbHits(); iHit++) {
    CbmTofHit* pHit = fTofClusterizer->GetHitPointer(iHit);
    if (NULL == pHit) continue;

    Int_t iDetId  = (pHit->GetAddress() & DetMask);
    Int_t iSmType = CbmTofAddress::GetSmType(iDetId);
    Int_t iSm     = CbmTofAddress::GetSmId(iDetId);
    Int_t iRpc    = CbmTofAddress::GetRpcId(iDetId);

    std::map<UInt_t, UInt_t>::iterator it = fDetIdIndexMap.find(iDetId);
    if (it == fDetIdIndexMap.end()) {
      LOG(info) << "detector TSR " << iSmType << iSm << iRpc << " not found in detector map";
      continue;  // continue for invalid detector index
    }
    Int_t iDetIndx = it->second;  //fDetIdIndexMap[iDetId];

    Int_t iChId = pHit->GetAddress();
    Int_t iCh   = CbmTofAddress::GetChannelId(iChId);

    // fill CalDigi Tots
    CbmMatch* digiMatch = fTofClusterizer->GetMatchPointer(iHit);
    for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink += 2) {  // loop over digis
      CbmLink L0      = digiMatch->GetLink(iLink);                         //vDigish.at(ivDigInd);
      UInt_t iDigInd0 = L0.GetIndex();
      UInt_t iDigInd1 = (digiMatch->GetLink(iLink + 1)).GetIndex();  //vDigish.at(ivDigInd+1);
      //LOG(debug1)<<" " << iDigInd0<<", "<<iDigInd1;
      if (iDigInd0 < fTofCalDigiVec->size() && iDigInd1 < fTofCalDigiVec->size()) {
        const CbmTofDigi* pDig0 = &(fTofCalDigiVec->at(iDigInd0));
        const CbmTofDigi* pDig1 = &(fTofCalDigiVec->at(iDigInd1));
        if ((Int_t) pDig0->GetType() != iSmType) {
          LOG(error) << Form(" Wrong Digi SmType %d - %d for Tofhit %d in iDetIndx "
                             "%d, Ch %d with %d strips at Indx %d, %d",
                             iSmType, (Int_t) pDig0->GetType(), iHit, iDetIndx, iCh,
                             fDigiBdfPar->GetNbChan(iSmType, iRpc), iDigInd0, iDigInd1);

          for (Int_t iL = 0; iL < digiMatch->GetNofLinks(); iL++) {  // loop over digis
            int idx               = (digiMatch->GetLink(iL)).GetIndex();
            const CbmTofDigi* pDx = &(fTofCalDigiVec->at(idx));
            LOG(info) << Form(" Digi %d, idx %d, TSRCS %d%d%d%02d%d ", iL, idx, (int) pDx->GetType(),
                              (int) pDx->GetSm(), (int) pDx->GetRpc(), (int) pDx->GetChannel(), (int) pDx->GetSide());
          }

          for (Int_t idx = 0; idx < (int) fTofCalDigiVec->size(); idx++) {  // loop over digis
            const CbmTofDigi* pDx = &(fTofCalDigiVec->at(idx));
            LOG(info) << Form(" AllDigi %d, TSRCS %d%d%d%02d%d ", idx, (int) pDx->GetType(), (int) pDx->GetSm(),
                              (int) pDx->GetRpc(), (int) pDx->GetChannel(), (int) pDx->GetSide());
          }

          LOG(fatal) << " fhRpcCluTot:  Digi 0 " << iDigInd0 << ": Ch " << pDig0->GetChannel() << ", Side "
                     << pDig0->GetSide() << ", StripSide " << (Double_t) iCh * 2. + pDig0->GetSide() << " Digi 1 "
                     << iDigInd1 << ": Ch " << pDig1->GetChannel() << ", Side " << pDig1->GetSide() << ", StripSide "
                     << (Double_t) iCh * 2. + pDig1->GetSide() << ", Tot0 " << pDig0->GetTot() << ", Tot1 "
                     << pDig1->GetTot();
        }  // end of type mismatch condition
        fhCalTot[iDetIndx]->Fill(pDig0->GetChannel() * 2. + pDig0->GetSide(), pDig0->GetTot());
        fhCalTot[iDetIndx]->Fill(pDig1->GetChannel() * 2. + pDig1->GetSide(), pDig1->GetTot());
      }
      else {
        LOG(warn) << "Invalid CalDigi index " << iDigInd0 << ",  " << iDigInd1 << " for size "
                  << fTofCalDigiVec->size();
      }
    }

    CbmTofCell* fChannelInfo = fDigiPar->GetCell(iChId);
    if (NULL == fChannelInfo) {
      LOG(error) << "Invalid Channel Pointer for ChId " << Form(" 0x%08x ", iChId) << ", Ch " << iCh;
      continue;
    }
    gGeoManager->FindNode(fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
    Double_t hitpos[3];
    hitpos[0] = pHit->GetX();
    hitpos[1] = pHit->GetY();
    hitpos[2] = pHit->GetZ();
    Double_t hlocal_p[3];
    //TGeoNode* cNode=
    gGeoManager->GetCurrentNode();
    gGeoManager->MasterToLocal(hitpos, hlocal_p);
    /*
    LOG(info)<<Form(" Ev %d: TSRC %d%d%d%2d, y %6.2f, tof %6.2f ",
    		   NevtH,iSmType,iSm,iRpc,iCh,hlocal_p[1],pHit->GetTime() - pRef->GetTime());
    */
    fhCalPosition[iDetIndx]->Fill((Double_t) iCh, hlocal_p[1]);  // transformed into LRF
    /*
    if(pHit != pRef )
      fhCalTofOff[iDetIndx]->Fill((Double_t) iCh,
                              pHit->GetTime() - pRef->GetTime());  // ignore any dependence
                              */
    fhCalTofOff[iDetIndx]->Fill((Double_t) iCh, pHit->GetTime() - dTAv);
  }

  return;
}

static Int_t NevtT = 0;
void CbmTofCalibrator::FillCalHist(CbmTofTracklet* pTrk, Int_t iOpt, CbmEvent* tEvent)
{
  NevtT++;
  if (NULL == fTofCalDigiVec) LOG(fatal) << "No access to TofCalDigis!";
  // fill deviation histograms on walk level
  if (pTrk->GetTt() < 0) return;  // take tracks with positive velocity only
  if (fbBeam && !pTrk->ContainsAddr(CbmTofAddress::GetUniqueAddress(0, 0, 0, 0, 5)))
    return;  // request beam counter hit for calibration

  if (fbBeam && fdR0Lim > 0.)  // consider only tracks originating from nominal interaction point
  {
    fhCalR0->Fill(pTrk->GetR0());
    if (pTrk->GetR0() > fdR0Lim) return;
  }
  fhCalDX0->Fill(pTrk->GetFitX(0.));
  fhCalDY0->Fill(pTrk->GetFitY(0.));
  if (iOpt > 70) HstDoublets(pTrk);  // Fill Doublets histograms

  //double dEvtStart=fEvtHeader->GetEventTime();
  for (Int_t iHit = 0; iHit < pTrk->GetNofHits(); iHit++) {
    CbmTofHit* pHit = pTrk->GetTofHitPointer(iHit);
    Int_t iDetId    = (pHit->GetAddress() & DetMask);
    Int_t iSmType   = CbmTofAddress::GetSmType(iDetId);
    Int_t iSm       = CbmTofAddress::GetSmId(iDetId);
    Int_t iRpc      = CbmTofAddress::GetRpcId(iDetId);

    std::map<UInt_t, UInt_t>::iterator it = fDetIdIndexMap.find(iDetId);
    if (it == fDetIdIndexMap.end()) continue;  // continue for invalid detector index
    Int_t iDetIndx = it->second;               //fDetIdIndexMap[iDetId];

    Int_t iChId              = pHit->GetAddress();
    Int_t iCh                = CbmTofAddress::GetChannelId(iChId);
    CbmTofCell* fChannelInfo = fDigiPar->GetCell(iChId);
    if (NULL == fChannelInfo) {
      LOG(error) << "Invalid Channel Pointer for ChId " << Form(" 0x%08x ", iChId) << ", Ch " << iCh;
      continue;
    }
    gGeoManager->FindNode(fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
    Double_t hitpos[3];
    hitpos[0] = pHit->GetX();
    hitpos[1] = pHit->GetY();
    hitpos[2] = pHit->GetZ();
    Double_t hlocal_p[3];
    //TGeoNode* cNode=
    gGeoManager->GetCurrentNode();
    gGeoManager->MasterToLocal(hitpos, hlocal_p);
    hitpos[0] = pTrk->GetFitX(pHit->GetZ());
    hitpos[1] = pTrk->GetFitY(pHit->GetZ());
    Double_t hlocal_f[3];
    gGeoManager->MasterToLocal(hitpos, hlocal_f);
    fhCalPosition[iDetIndx]->Fill((Double_t) iCh,
                                  hlocal_p[1]);                           // transformed into LRF
    fhCalPos[iDetIndx]->Fill((Double_t) iCh, hlocal_p[1] - hlocal_f[1]);  // transformed into LRF
    fhCalTOff[iDetIndx]->Fill((Double_t) iCh,
                              pHit->GetTime() - pTrk->GetFitT(pHit->GetZ()));  // residuals transformed into LRF
    //fhCalTOff[iDetIndx]->Fill((Double_t)iCh,fTrackletTools->GetTdif(pTrk, iDetId, pHit));

    Int_t iEA  = pTrk->GetTofHitIndex(iHit);
    Int_t iTSA = fTofFindTracks->GetTofHitIndex(iEA);

    if (iTSA > fTofDigiMatchColl->GetEntriesFast()) {
      LOG(error) << " Inconsistent DigiMatches for Hitind " << iTSA
                 << ", TClonesArraySize: " << fTofDigiMatchColl->GetEntriesFast();
    }
    CbmMatch* digiMatch = (CbmMatch*) fTofDigiMatchColl->At(iTSA);

    Double_t hlocal_d[3];
    for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink += 2) {  // loop over digis
      CbmLink L0     = digiMatch->GetLink(iLink);
      Int_t iDigInd0 = L0.GetIndex();
      Int_t iDigInd1 = (digiMatch->GetLink(iLink + 1)).GetIndex();

      const CbmTofDigi* tDigi0 = NULL;
      const CbmTofDigi* tDigi1 = NULL;
      if (tEvent != NULL) {  //disable
        LOG(debug) << "Locate MatchDigiInd " << iDigInd0 << " and " << iDigInd1 << " in CalDigiVec of size "
                   << fTofCalDigiVec->size();
        //		  <<" in current event not implemented";
        //continue;
        tDigi0 = &(fTofCalDigiVec->at(iDigInd0));
        tDigi1 = &(fTofCalDigiVec->at(iDigInd1));
      }
      else {  // event wise entries (TS mode)
        LOG(debug) << "TS mode: locate MatchDigiInd " << iDigInd0 << " and " << iDigInd1 << " in CalDigiVec of size "
                   << fTofCalDigiVec->size();
        if (fair::Logger::Logging(fair::Severity::debug2)) {
          for (UInt_t iDigi = 0; iDigi < fTofCalDigiVec->size(); iDigi++) {
            tDigi0 = &(fTofCalDigiVec->at(iDigi));
            LOG(debug) << Form("#%u: TSRCS %d%d%d%2d%d", iDigi, (Int_t) tDigi0->GetType(), (Int_t) tDigi0->GetSm(),
                               (Int_t) tDigi0->GetRpc(), (Int_t) tDigi0->GetChannel(), (Int_t) tDigi0->GetSide());
          }
        }
        tDigi0 = &(fTofCalDigiVec->at(iDigInd0));
        tDigi1 = &(fTofCalDigiVec->at(iDigInd1));
        //tDigi0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
        //tDigi1 = fDigiMan->Get<CbmTofDigi>(iDigInd1);
      }

      Int_t iCh0   = tDigi0->GetChannel();
      Int_t iSide0 = tDigi0->GetSide();

      LOG(debug) << "Fill Walk for Hit Ind " << iEA << ", " << iTSA
                 << Form(", TSRC %d%d%d%2d, DigiInd %2d, %2d", iSmType, iSm, iRpc, iCh, iDigInd0, iDigInd1)
                 << Form(", TSRCS %d%d%d%2d%d %d%d%d%2d%d", (Int_t) tDigi0->GetType(), (Int_t) tDigi0->GetSm(),
                         (Int_t) tDigi0->GetRpc(), (Int_t) tDigi0->GetChannel(), (Int_t) tDigi0->GetSide(),
                         (Int_t) tDigi1->GetType(), (Int_t) tDigi1->GetSm(), (Int_t) tDigi1->GetRpc(),
                         (Int_t) tDigi1->GetChannel(), (Int_t) tDigi1->GetSide());

      if (iDetIndx > (Int_t) fhCalWalk.size()) {
        LOG(error) << "Invalid DetIndx " << iDetIndx;
        continue;
      }
      if (iCh0 > (Int_t) fhCalWalk[iDetIndx].size()) {
        LOG(error) << "Invalid Ch0 " << iCh0;
        continue;
      }
      if (iSide0 > (Int_t) fhCalWalk[iDetIndx][iCh0].size()) {
        LOG(error) << "Invalid Side0 " << iSide0;
        continue;
      }

      Int_t iCh1   = tDigi1->GetChannel();
      Int_t iSide1 = tDigi1->GetSide();
      if (iCh1 > (Int_t) fhCalWalk[iDetIndx].size()) {
        LOG(error) << "Invalid Ch1 " << iCh1;
        continue;
      }
      if (iSide1 > (Int_t) fhCalWalk[iDetIndx][iCh1].size()) {
        LOG(error) << "Invalid Side1 " << iSide1;
        continue;
      }

      if (iCh0 != iCh1 || iSide0 == iSide1) {
        LOG(fatal) << "Invalid digi pair for TSR " << iSmType << iSm << iRpc
                   << Form(" Ch  %2d %2d side %d %d", iCh0, iCh1, iSide0, iSide1) << " in event " << NevtT;
        continue;
      }

      hlocal_d[1] =
        -0.5 * ((1. - 2. * tDigi0->GetSide()) * tDigi0->GetTime() + (1. - 2. * tDigi1->GetSide()) * tDigi1->GetTime())
        * fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc);

      if (TMath::Abs(hlocal_d[1] - hlocal_p[1]) > 10.) {
        LOG(warn) << "CMPY for TSRC " << iSmType << iSm << iRpc << iCh0 << ": " << hlocal_f[1] << ", " << hlocal_p[1]
                  << ", " << hlocal_d[1] << ", TOT: " << tDigi0->GetTot() << " " << tDigi1->GetTot();
      }
      Int_t iWalkMode = (iOpt - iOpt % 100) / 100;
      switch (iWalkMode) {
        case 1:
          fhCalWalk[iDetIndx][iCh0][iSide0]->Fill(
            tDigi0->GetTot(),
            tDigi0->GetTime() + (1. - 2. * tDigi0->GetSide()) * hlocal_d[1] / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)
              - pTrk->GetFitT(pHit->GetZ())  //-fTrackletTools->GetTexpected(pTrk, iDetId, pHit)
              + fTofFindTracks->GetTOff(iDetId)
              + 2. * (1. - 2. * tDigi0->GetSide()) * (hlocal_d[1] - hlocal_f[1])
                  / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc));
          /*
        LOG(info)<<"TSRCS "<<iSmType<<iSm<<iRpc<<iCh<<iSide0<<Form(": digi0 %f, ex %f, prop %f, Off %f, res %f",
                            tDigi0->GetTime(),
                            fTrackletTools->GetTexpected(pTrk, iDetId, pHit) ,
                            fTofFindTracks->GetTOff(iDetId),
                            (1.-2.*tDigi0->GetSide())*hlocal_f[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc),
                            tDigi0->GetTime()-fTrackletTools->GetTexpected(pTrk, iDetId, pHit)
                            -(1.-2.*tDigi0->GetSide())*hlocal_f[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
        */

          fhCalWalk[iDetIndx][iCh1][iSide1]->Fill(
            tDigi1->GetTot(),
            tDigi1->GetTime() + (1. - 2. * tDigi1->GetSide()) * hlocal_d[1] / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc)
              - pTrk->GetFitT(pHit->GetZ())  //-fTrackletTools->GetTexpected(pTrk, iDetId, pHit)
              + fTofFindTracks->GetTOff(iDetId)
              + 2. * (1. - 2. * tDigi1->GetSide()) * (hlocal_d[1] - hlocal_f[1])
                  / fDigiBdfPar->GetSigVel(iSmType, iSm, iRpc));
          break;

        case 0: {
          Double_t dDeltaT = 0.5 * (tDigi0->GetTime() + tDigi1->GetTime()) + fTofFindTracks->GetTOff(iDetId)
                             - pTrk->GetFitT(pHit->GetZ());
          fhCalWalk[iDetIndx][iCh0][iSide0]->Fill(tDigi0->GetTot(), dDeltaT);
          fhCalWalk[iDetIndx][iCh1][iSide1]->Fill(tDigi1->GetTot(), dDeltaT);
          if (iSmType == 5 || iSmType == 8) {  // symmetrize for Pad counters
            fhCalWalk[iDetIndx][iCh0][iSide0]->Fill(tDigi1->GetTot(), dDeltaT);
            fhCalWalk[iDetIndx][iCh1][iSide1]->Fill(tDigi0->GetTot(), dDeltaT);
          }
        } break;
      }
    }
  }
}

Bool_t CbmTofCalibrator::UpdateCalHist(Int_t iOpt)
{
  // get current calibration histos
  TString CalFileName = fTofClusterizer->GetCalParFileName();
  LOG(info) << "CbmTofCalibrator:: update histos from "
            << "file " << CalFileName << " with option " << iOpt;
  int iOpt0 = iOpt % 10;
  int iOpt1 = (iOpt - iOpt0) / 10;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* fCalParFile = new TFile(CalFileName, "update");
  if (NULL == fCalParFile) {
    LOG(warn) << "Could not open TofClusterizer calibration file " << CalFileName;
    if (iOpt0 == 9) {  //modify reference file name
      int iCalMode          = CalFileName.Index("tofClust") - 3;
      CalFileName(iCalMode) = '3';
      LOG(info) << "Modified CalFileName = " << CalFileName;
      fCalParFile = new TFile(CalFileName, "update");
      if (NULL == fCalParFile) LOG(fatal) << "Could not open TofClusterizer calibration file " << CalFileName;
    }
  }
  assert(fCalParFile);
  ReadHist(fCalParFile);
  if (kTRUE) {                     // all calibration modes
    const Double_t MINCTS = 100.;  //FIXME, numerical constant in code
    // modify calibration histograms
    // check for beam counter
    Double_t dBeamTOff = 0.;
    for (Int_t iDetIndx = 0; iDetIndx < fDigiBdfPar->GetNbDet(); iDetIndx++) {
      Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
      Int_t iSmType   = CbmTofAddress::GetSmType(iUniqueId);
      if (5 == iSmType) {
        if (fhCalTOff[iDetIndx]->GetEntries() > MINCTS) {
          TH1* hBy = (TH1*) fhCalTOff[iDetIndx]->ProjectionY();
          // Fit gaussian around peak value
          Double_t dFMean    = hBy->GetBinCenter(hBy->GetMaximumBin());
          Double_t dFLim     = 0.5;  // CAUTION, fixed numeric value
          Double_t dBinSize  = hBy->GetBinWidth(1);
          dFLim              = TMath::Max(dFLim, 5. * dBinSize);
          TFitResultPtr fRes = hBy->Fit("gaus", "SQM", "", dFMean - dFLim, dFMean + dFLim);
          dBeamTOff          = fRes->Parameter(1);  //overwrite mean
          LOG(info) << "Found beam counter with average TOff = " << dBeamTOff;
        }
        else {
          LOG(info) << "Beam counter has too few entries: " << fhCalTOff[iDetIndx]->GetEntries();
        }
        break;
      }
    }
    if (dBeamTOff == 0.) LOG(warn) << "No beam counter found";

    for (Int_t iDetIndx = 0; iDetIndx < fDigiBdfPar->GetNbDet(); iDetIndx++) {
      Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
      // Int_t iSmAddr   = iUniqueId & DetMask;
      Int_t iSmType = CbmTofAddress::GetSmType(iUniqueId);
      Int_t iSm     = CbmTofAddress::GetSmId(iUniqueId);
      Int_t iRpc    = CbmTofAddress::GetRpcId(iUniqueId);
      if (NULL == fhCorTOff[iDetIndx]) {
        LOG(warn) << "hCorTOff for TSR " << iSmType << iSm << iRpc << " not available";
        continue;
      }

      switch (iOpt0) {
        case 0:  // none
          break;
        case 1:  // update channel mean
        {
          LOG(info) << "Update Offsets for TSR " << iSmType << iSm << iRpc;

          TProfile* hpP = fhCalPos[iDetIndx]->ProfileX();
          TProfile* hpT = fhCalTOff[iDetIndx]->ProfileX();
          TH1* hCalP    = fhCalPos[iDetIndx]->ProjectionX();
          TH1* hCalT    = fhCalTOff[iDetIndx]->ProjectionX();
          //fhCorPos[iDetIndx]->Add((TH1 *)hpP,-1.);
          //fhCorTOff[iDetIndx]->Add((TH1*)hpT,-1.);
          double dTOffMeanShift = 0.;
          double dNCh           = 0.;
          for (Int_t iBin = 0; iBin < fhCorTOff[iDetIndx]->GetNbinsX(); iBin++) {
            Double_t dDt   = hpT->GetBinContent(iBin + 1);
            Double_t dCorT = fhCorTOff[iDetIndx]->GetBinContent(iBin + 1);
            Double_t dCtsT = hCalT->GetBinContent(iBin + 1);
            Double_t dCtsP = hCalP->GetBinContent(iBin + 1);
            Double_t dDp   = hpP->GetBinContent(iBin + 1);
            Double_t dCorP = fhCorPos[iDetIndx]->GetBinContent(iBin + 1);
            LOG(info) << "Cts check for " << fhCalTOff[iDetIndx]->GetName() << ", bin " << iBin << ": " << dCtsT << ", "
                      << dCtsP << ", " << MINCTS;
            if (dCtsT > MINCTS && dCtsP > MINCTS) {
              // Fit Gaussian around peak
              TH1* hpPy =
                (TH1*) fhCalPos[iDetIndx]->ProjectionY(Form("PosPy_%d_%d", iDetIndx, iBin), iBin + 1, iBin + 1);
              Double_t dFMean    = hpPy->GetBinCenter(hpPy->GetMaximumBin());
              Double_t dFLim     = 0.5;  // CAUTION, fixed numeric value
              Double_t dBinSize  = hpPy->GetBinWidth(1);
              dFLim              = TMath::Max(dFLim, 5. * dBinSize);
              TFitResultPtr fRes = hpPy->Fit("gaus", "SQM", "", dFMean - dFLim, dFMean + dFLim);
              if (gMinuit->fCstatu.Contains("OK") || gMinuit->fCstatu.Contains("CONVERGED")) {
                dDp = fRes->Parameter(1);  //overwrite mean
              }
              TH1* hpTy =
                (TH1*) fhCalTOff[iDetIndx]->ProjectionY(Form("TOffPy_%d_%d", iDetIndx, iBin), iBin + 1, iBin + 1);
              dFMean   = hpTy->GetBinCenter(hpTy->GetMaximumBin());
              dFLim    = 0.5;  // CAUTION, fixed numeric value
              dBinSize = hpTy->GetBinWidth(1);
              dFLim    = TMath::Max(dFLim, 5. * dBinSize);
              fRes     = hpTy->Fit("gaus", "SQM", "", dFMean - dFLim, dFMean + dFLim);
              if (gMinuit->fCstatu.Contains("OK") || gMinuit->fCstatu.Contains("CONVERGED")) {
                dDt = fRes->Parameter(1);  //overwrite mean
              }
              // Double_t dDpRes = fRes->Parameter(2);
              if (iSmType == 5) {  // do not shift beam counter in time
                fhCorTOff[iDetIndx]->SetBinContent(iBin + 1, dCorT + dDt - dBeamTOff);
                dTOffMeanShift += dDt - dBeamTOff;
              }
              else {
                fhCorTOff[iDetIndx]->SetBinContent(iBin + 1, dCorT + dDt + dBeamTOff);
                dTOffMeanShift += dDt + dBeamTOff;
              }
              dNCh += 1.;
              if (iOpt1 > 0) {  // active  y-position correction
                fhCorPos[iDetIndx]->SetBinContent(iBin + 1, dCorP + dDp);
              }
              if (iDetIndx > -1) {
                LOG(info) << Form("Update %s: bin %02d, Cts: %d, Old %6.3f, dev %6.3f, beam %6.3f, new %6.3f",
                                  fhCorTOff[iDetIndx]->GetName(), iBin, (Int_t) dCtsT, dCorT, dDt, dBeamTOff,
                                  dCorT + dDt + dBeamTOff);
              }
            }
          }
          if (dNCh > 0) dTOffMeanShift /= dNCh;
          LOG(info) << "Apply dTOffMeanShift " << dTOffMeanShift << " to " << fhCorTOff[iDetIndx]->GetName();
          for (Int_t iBin = 0; iBin < fhCorTOff[iDetIndx]->GetNbinsX(); iBin++) {  //preserve mean offset
            fhCorTOff[iDetIndx]->SetBinContent(iBin + 1, fhCorTOff[iDetIndx]->GetBinContent(iBin + 1) - dTOffMeanShift);
          }
        } break;
        case 2: {  // update individual channel walks
          LOG(info) << "Update Walks for TSR " << iSmType << iSm << iRpc;
          if (iSmType == 5) continue;  // no walk correction for beam counter up to now
          const Double_t MinCounts = 10.;
          Int_t iNbCh              = fDigiBdfPar->GetNbChan(iSmType, iRpc);
          for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
            for (Int_t iSide = 0; iSide < 2; iSide++) {
              //LOG(info) << "Get walk histo pointer for TSRCS " << iSmType<<iSm<<iRpc<<iCh<<iSide;
              TProfile* hpW = fhCalWalk[iDetIndx][iCh][iSide]->ProfileX();     // mean deviation
              TH1* hCW      = fhCalWalk[iDetIndx][iCh][iSide]->ProjectionX();  // contributing counts
              if (hCW->GetEntries() == 0) {
                LOG(info) << "No entries in " << hCW->GetName();
                continue;
              }
              Double_t dCorT = 0;
              for (Int_t iBin = 0; iBin < fhCorWalk[iDetIndx][iCh][iSide]->GetNbinsX(); iBin++) {
                Double_t dCts  = hCW->GetBinContent(iBin + 1);
                Double_t dWOff = fhCorWalk[iDetIndx][iCh][iSide]->GetBinContent(iBin + 1);  // current value
                if (iBin > 0 && dCts == 0)
                  fhCorWalk[iDetIndx][iCh][iSide]->SetBinContent(iBin + 1,
                                                                 fhCorWalk[iDetIndx][iCh][iSide]->GetBinContent(iBin));
                if (dCts > MinCounts) { dCorT = hpW->GetBinContent(iBin + 1); }
                fhCorWalk[iDetIndx][iCh][iSide]->SetBinContent(iBin + 1, dWOff + dCorT);  //set new value
              }
              // determine effective/count rate weighted mean
              Double_t dMean   = 0;
              Double_t dCtsAll = 0.;
              for (Int_t iBin = 0; iBin < fhCorWalk[iDetIndx][iCh][iSide]->GetNbinsX(); iBin++) {
                Double_t dCts  = hCW->GetBinContent(iBin + 1);
                Double_t dWOff = fhCorWalk[iDetIndx][iCh][iSide]->GetBinContent(iBin + 1);
                if (dCts > MinCounts) {
                  dCtsAll += dCts;
                  dMean += dCts * dWOff;
                }
              }
              if (dCtsAll > 0.) dMean /= dCtsAll;

              LOG(info) << "Mean shift for TSRCS " << iSmType << iSm << iRpc << iCh << iSide << ": " << dMean;

              // keep mean value at 0
              for (Int_t iBin = 0; iBin < fhCorWalk[iDetIndx][iCh][iSide]->GetNbinsX(); iBin++) {
                Double_t dWOff = fhCorWalk[iDetIndx][iCh][iSide]->GetBinContent(iBin + 1);  // current value
                fhCorWalk[iDetIndx][iCh][iSide]->SetBinContent(iBin + 1, dWOff - dMean);    //set new value
                if (iCh == 0 && iSmType == 8 && iBin == 10)                                 // debugging
                  LOG(info) << "New Walk for " << fhCorWalk[iDetIndx][iCh][iSide]->GetName() << " bin " << iBin << ": "
                            << fhCorWalk[iDetIndx][iCh][iSide]->GetBinContent(iBin + 1) << ", Mean " << dMean
                            << ", WOff " << dWOff;
              }
            }
          }
        } break;

        case 9:  // update channel means on cluster level
        {
          // position, do Edge fit by default
          //LOG(info) << "Update Offsets for TSR " << iSmType << iSm << iRpc;

          //TProfile* hpP = fhCalPosition[iDetIndx]->ProfileX();
          TH1* hCalP = fhCalPosition[iDetIndx]->ProjectionX();

          for (Int_t iBin = 0; iBin < fhCorPos[iDetIndx]->GetNbinsX(); iBin++) {
            Double_t dCorP = fhCorPos[iDetIndx]->GetBinContent(iBin + 1);
            //Double_t dDp   = hpP->GetBinContent(iBin + 1);
            Double_t dCtsP = hCalP->GetBinContent(iBin + 1);
            if (dCtsP > MINCTS) {
              TH1* hPos_py = fhCalPosition[iDetIndx]->ProjectionY(
                Form("%s_py_%d", fhCalPosition[iDetIndx]->GetName(), iBin), iBin + 1, iBin + 1);
              double dYShift           = hPos_py->GetMean();
              int iChId                = CbmTofAddress::GetUniqueAddress(iSm, iRpc, 0, 0, iSmType);
              CbmTofCell* fChannelInfo = fDigiPar->GetCell(iChId);
              if (NULL == fChannelInfo) LOG(fatal) << Form("invalid ChannelInfo for 0x%08x", iChId);

              double dThr = 10.;
              double* dRes =
                fTofClusterizer->find_yedges((const char*) (hPos_py->GetName()), dThr, fChannelInfo->GetSizey());
              /*
          	  LOG(info) << Form("EdgeY for %s, TSR %d%d%d: DY %5.2f, Len %5.2f, Size %5.2f ",
          	                     hPos_py->GetName(), iSmType, iSm, iRpc, dRes[1], dRes[0], fChannelInfo->GetSizey());
              */
              if (TMath::Abs(dRes[0] - fChannelInfo->GetSizey()) / fChannelInfo->GetSizey() < 0.1) {
                dYShift = dRes[1];
              }
              else {
                dYShift = hPos_py->GetMean();
              }
              // apply correction
              //LOG(info)<<Form("UpdateCalPos TSRC %d%d%d%2d: %6.2f -> %6.2f ",iSmType,iSm,iRpc,iBin,dCorP,dCorP+dYShift);
              fhCorPos[iDetIndx]->SetBinContent(iBin + 1, dCorP + dYShift);
            }
          }

          //TofOff
          TProfile* hpT = fhCalTofOff[iDetIndx]->ProfileX();
          TH1* hCalT    = fhCalTofOff[iDetIndx]->ProjectionX();
          for (Int_t iBin = 0; iBin < fhCorTOff[iDetIndx]->GetNbinsX(); iBin++) {
            //Double_t dDt   = hpT->GetBinContent(iBin + 1);
            Double_t dCorT = fhCorTOff[iDetIndx]->GetBinContent(iBin + 1);
            Double_t dCtsT = hCalT->GetBinContent(iBin + 1);
            if (dCtsT > MINCTS) {
              double dTmean = hpT->GetBinContent(iBin + 1);
              TH1* hTy      = (TH1*) fhCalTofOff[iDetIndx]->ProjectionY(
                Form("%s_py%d", fhCalTofOff[iDetIndx]->GetName(), iBin), iBin + 1, iBin + 1);
              Double_t dNPeak = hTy->GetBinContent(hTy->GetMaximumBin());
              if (dNPeak > MINCTS * 0.1) {
                Double_t dFMean    = hTy->GetBinCenter(hTy->GetMaximumBin());
                Double_t dFLim     = 2.0;  // CAUTION, fixed numeric value
                Double_t dBinSize  = hTy->GetBinWidth(1);
                dFLim              = TMath::Max(dFLim, 10. * dBinSize);
                TFitResultPtr fRes = hTy->Fit("gaus", "SQM", "", dFMean - dFLim, dFMean + dFLim);
                //if (fRes == 0 )
                if (gMinuit->fCstatu.Contains("OK") || gMinuit->fCstatu.Contains("CONVERGED")) {
                  //if (TMath::Abs(TMean - fRes->Parameter(1)) > 1.)
                  LOG(info) << "CalTOff  "
                            << Form("TSRC %d%d%d%d, %s gaus %8.2f %8.2f %8.2f for "
                                    "TM %8.2f, TBeam %6.2f",
                                    iSmType, iSm, iRpc, iBin, hTy->GetName(), fRes->Parameter(0), fRes->Parameter(1),
                                    fRes->Parameter(2), dTmean, dBeamTOff);
                  dTmean = fRes->Parameter(1);  //overwrite mean
                }
              }
              LOG(info) << Form("UpdateCalTOff TSRC %d%d%d%2d, cts %d: %6.2f -> %6.2f, %6.2f ", iSmType, iSm, iRpc,
                                iBin, (int) dCtsT, dCorT, dCorT + dTmean, dCorT + hpT->GetBinContent(iBin + 1));
              fhCorTOff[iDetIndx]->SetBinContent(iBin + 1, dCorT + dTmean);
            }
          }

          //Tot
          double dCalTotMean = fTofClusterizer->GetTotMean();  // Target value
          for (Int_t iBin = 0; iBin < fhCorTot[iDetIndx]->GetNbinsX(); iBin++) {
            int ib          = iBin + 1;
            TH1* hbin       = fhCalTot[iDetIndx]->ProjectionY(Form("bin%d", ib), ib, ib);
            double dTotMean = hbin->GetMean();
            // Do gaus fit around maximum
            Double_t dCtsTot = hbin->GetEntries();
            if (kFALSE && dCtsTot > MINCTS) {
              double dFMean = hbin->GetBinCenter(hbin->GetMaximumBin());
              double dFLim  = dFMean;  // CAUTION,
              //LOG(info)<<Form("FitTot TSRC %d%d%d%2d:  Mean %6.2f, Width %6.2f ",iSmType,iSm,iRpc,iBin,dFMean,dFLim);
              if (dFMean > 2.) {
                TFitResultPtr fRes = hbin->Fit("gaus", "SQM", "", dFMean - dFLim, dFMean + dFLim);
                if (gMinuit->fCstatu.Contains("OK") || gMinuit->fCstatu.Contains("CONVERGED")) {
                  dTotMean = fRes->Parameter(1);  //overwrite mean
                }
              }
            }
            if (dTotMean > 0.) {  // prevent zero-divide
              double dCorTot = fhCorTot[iDetIndx]->GetBinContent(ib);
              /*
          	  LOG(info)<<Form("UpdateCalTot TSRC %d%d%d%2d: %6.2f, %6.2f, %6.2f -> %6.2f ",iSmType,iSm,iRpc,iBin,
          			  dCorTot,dCalTotMean,dTotMean,dCorTot*dTotMean/dCalTotMean);
          			  */
              fhCorTot[iDetIndx]->SetBinContent(ib, dCorTot * dTotMean / dCalTotMean);
            }
          }

        } break;

        default: LOG(warning) << "No valid calibration mode " << iOpt0;
      }  //switch( iOpt) end
    }
  }
  else {
    // currently not needed
  }
  TString fFile = fCalParFile->GetName();
  if (!fFile.Contains("/")) {
    TFile* fCalParFileNew = new TFile(Form("New_%s", fCalParFile->GetName()), "RECREATE");
    WriteHist(fCalParFileNew);
    fCalParFileNew->Close();
  }
  fCalParFile->Close();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  //gDirectory = oldDir;
  gDirectory->cd(oldDir->GetPath());

  return kTRUE;
}

void CbmTofCalibrator::ReadHist(TFile* fHist)
{
  LOG(info) << "Read Cor histos from file " << fHist->GetName();
  if (0 == fhCorPos.size()) {
    Int_t iNbDet = fDigiBdfPar->GetNbDet();
    //LOG(info) << "resize histo vector for " << iNbDet << " detectors ";
    fhCorPos.resize(iNbDet);
    fhCorTOff.resize(iNbDet);
    fhCorTot.resize(iNbDet);
    fhCorTotOff.resize(iNbDet);
    fhCorSvel.resize(iNbDet);
    fhCorWalk.resize(iNbDet);
  }

  for (Int_t iDetIndx = 0; iDetIndx < fDigiBdfPar->GetNbDet(); iDetIndx++) {
    Int_t iUniqueId = fDigiBdfPar->GetDetUId(iDetIndx);
    //Int_t iSmAddr   = iUniqueId & DetMask;
    Int_t iSmType = CbmTofAddress::GetSmType(iUniqueId);
    Int_t iSm     = CbmTofAddress::GetSmId(iUniqueId);
    Int_t iRpc    = CbmTofAddress::GetRpcId(iUniqueId);
    //LOG(info) << "Get histo pointer for TSR " << iSmType<<iSm<<iRpc;

    fhCorPos[iDetIndx] =
      (TH1*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Pos_pfx", iSmType, iSm, iRpc));
    if (NULL == fhCorPos[iDetIndx]) {
      LOG(error) << "hCorPos not found for TSR " << iSmType << iSm << iRpc;
      continue;
    }
    fhCorTOff[iDetIndx] =
      (TH1*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_TOff_pfx", iSmType, iSm, iRpc));
    fhCorTot[iDetIndx] =
      (TH1*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Mean", iSmType, iSm, iRpc));
    fhCorTotOff[iDetIndx] =
      (TH1*) gDirectory->FindObjectAny(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Off", iSmType, iSm, iRpc));

    Int_t iNbCh = fDigiBdfPar->GetNbChan(iSmType, iRpc);
    fhCorWalk[iDetIndx].resize(iNbCh);
    for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
      fhCorWalk[iDetIndx][iCh].resize(2);
      for (Int_t iSide = 0; iSide < 2; iSide++) {
        //LOG(info) << "Get walk histo pointer for TSRCS " << iSmType<<iSm<<iRpc<<iCh<<iSide;
        fhCorWalk[iDetIndx][iCh][iSide] = (TH1*) gDirectory->FindObjectAny(
          Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_Walk_px", iSmType, iSm, iRpc, iCh, iSide));
        if (NULL == fhCorWalk[iDetIndx][iCh][iSide]) {
          LOG(warn) << "No Walk histo for TSRCS " << iSmType << iSm << iRpc << iCh << iSide;
          continue;
        }

        if (iSmType == 8 && iSide == 1) {  //pad special treatment
          LOG(warn) << "Overwrite pad counter walk for TSRCS " << iSmType << iSm << iRpc << iCh << iSide;
          TH1* hTmp = (TH1*) gDirectory->FindObjectAny(
            Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_Walk_px", iSmType, iSm, iRpc, iCh, 1 - iSide));
          for (Int_t iBin = 0; iBin < fhCorWalk[iDetIndx][iCh][iSide]->GetNbinsX(); iBin++)
            fhCorWalk[iDetIndx][iCh][iSide]->SetBinContent(iBin + 1, hTmp->GetBinContent(iBin + 1));
        }
      }
    }
  }
}

void CbmTofCalibrator::WriteHist(TFile* fHist)
{
  LOG(info) << "Write Cor histos to file " << fHist->GetName();
  TDirectory* oldir = gDirectory;
  fHist->cd();
  for (Int_t iDetIndx = 0; iDetIndx < fDigiBdfPar->GetNbDet(); iDetIndx++) {
    if (NULL == fhCorPos[iDetIndx]) continue;
    fhCorPos[iDetIndx]->Write();
    fhCorTOff[iDetIndx]->Write();
    fhCorTot[iDetIndx]->Write();
    fhCorTotOff[iDetIndx]->Write();

    Int_t iNbCh = (Int_t) fhCorWalk[iDetIndx].size();
    for (Int_t iCh = 0; iCh < iNbCh; iCh++) {
      for (Int_t iSide = 0; iSide < 2; iSide++) {
        fhCorWalk[iDetIndx][iCh][iSide]->Write();
      }
    }
  }
  oldir->cd();
}

void CbmTofCalibrator::HstDoublets(CbmTofTracklet* pTrk)
{
  for (Int_t iHit = 0; iHit < pTrk->GetNofHits() - 1; iHit++) {
    for (Int_t iHit1 = 0; iHit1 < pTrk->GetNofHits(); iHit1++) {
      if (iHit == iHit1) continue;
      CbmTofHit* pHit0 = pTrk->GetTofHitPointer(iHit);
      CbmTofHit* pHit1 = pTrk->GetTofHitPointer(iHit1);
      //auto iHind0=pTrk->GetTofHitIndex(iHit);
      //auto iHind1=pTrk->GetTofHitIndex(iHit+1);
      int iDind0 = fDetIdIndexMap[pTrk->GetTofDetIndex(iHit)];
      int iDind1 = fDetIdIndexMap[pTrk->GetTofDetIndex(iHit1)];
      if (pHit1->GetZ() < pHit0->GetZ()) {
        continue;
        //iHind0=pTrk->GetTofHitIndex(iHit+1);
        //iHind1=pTrk->GetTofHitIndex(iHit);
        /*
	    iDind0=fDetIdIndexMap[ pTrk->GetTofDetIndex(iHit1) ]; // numbering according to BDF
        iDind1=fDetIdIndexMap[ pTrk->GetTofDetIndex(iHit) ];
	    pHit0=pTrk->GetTofHitPointer(iHit1);
	    pHit1=pTrk->GetTofHitPointer(iHit);
	    */
      }
      int iHst = iDind0 * 100 + iDind1;
      if (NULL == fhDoubletDt[iHst]) {
        // create histograms
        TDirectory* oldir =
          gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
        gROOT->cd();   // <= To prevent histos from being sucked in by the param file of the TRootManager !
        TString hNameDt = Form("hDoubletDt_%02d%02d", iDind0, iDind1);
        LOG(info) << Form("Book histo %lu %s, Addrs 0x%08x, 0x%08x ", fhDoubletDt.size(), hNameDt.Data(),
                          pTrk->GetTofDetIndex(iHit), pTrk->GetTofDetIndex(iHit + 1));
        TH1D* pHstDt      = new TH1D(hNameDt, Form("%s; #Delta t (ns); cts", hNameDt.Data()), 100, -5., 5.);
        fhDoubletDt[iHst] = pHstDt;
        TString hNameDd   = Form("hDoubletDd_%02d%02d", iDind0, iDind1);
        TH1D* pHstDd      = new TH1D(hNameDd, Form("%s; #Delta D (cm); cts", hNameDd.Data()), 200, 0., 200.);
        fhDoubletDd[iHst] = pHstDd;
        TString hNameV    = Form("hDoubletV_%02d%02d", iDind0, iDind1);
        TH1D* pHstV       = new TH1D(hNameV, Form("%s; v (cm/ns); cts", hNameV.Data()), 100, 0., 100.);
        fhDoubletV[iHst]  = pHstV;
        oldir->cd();
      }
      // Fill Histograms
      double dDt = pHit1->GetTime() - pHit0->GetTime();
      double dDd = pTrk->Dist3D(pHit1, pHit0);
      fhDoubletDt[iHst]->Fill(dDt);
      fhDoubletDd[iHst]->Fill(dDd);
      fhDoubletV[iHst]->Fill(dDd / dDt);
    }
  }
}


ClassImp(CbmTofCalibrator)
