/**
 * \file CbmLitFitQa.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmLitFitQa.h"

#include "../mc/CbmLitMCTrackCreator.h"
#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmLitFitQaReport.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMvdHit.h"
#include "CbmStsAddress.h"
#include "CbmStsHit.h"
#include "CbmStsSetup.h"
#include "CbmStsTrack.h"
#include "CbmTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdAddress.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include <TFile.h>

#include <boost/assign/list_of.hpp>

#include <vector>
using std::make_pair;
using std::pair;
using std::vector;

CbmLitFitQa::CbmLitFitQa()
  : fIsFixedBounds(true)
  , fMvdMinNofHits(0)
  , fStsMinNofHits(0)
  , fTrdMinNofHits(0)
  , fMuchMinNofHits(0)
  , fOutputDir("./test/")
  , fHM(NULL)
  , fPRangeMin(0.)
  , fPRangeMax(10.)
  , fPRangeBins(20)
  , fGlobalTracks(NULL)
  , fStsTracks(NULL)
  , fStsTrackMatches(NULL)
  , fStsHits(NULL)
  , fMvdHits(NULL)
  , fTrdTracks(NULL)
  , fTrdTrackMatches(NULL)
  , fTrdHits(NULL)
  , fMuchTracks(NULL)
  , fMuchTrackMatches(NULL)
  , fMuchPixelHits(NULL)
  , fMuchStripHits(NULL)
  , fMCTracks(NULL)
  , fEvents(NULL)
  , fQuota(0.7)
  , fPrimVertex(NULL)
  , fKFFitter()
  , fMCTrackCreator(NULL)
  , fDet()
{
}

CbmLitFitQa::~CbmLitFitQa()
{
  if (fHM) delete fHM;
}

InitStatus CbmLitFitQa::Init()
{
  fHM = new CbmHistManager();
  fDet.DetermineSetup();
  CreateHistograms();
  ReadDataBranches();
  fMCTrackCreator = CbmLitMCTrackCreator::Instance();
  fKFFitter.Init();
  return kSUCCESS;
}

void CbmLitFitQa::Exec(Option_t* opt)
{
  static Int_t nofEvents = 0;
  nofEvents++;
  std::cout << "CbmLitFitQa::Exec: event=" << nofEvents << std::endl;
  fMCTrackCreator->Create(nofEvents - 1);
  ProcessGlobalTracks();
  ProcessTrackParamsAtVertex();
  ProcessTrackMomentumAtVertex();
}

void CbmLitFitQa::Finish()
{
  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());

  fHM->ShrinkEmptyBinsH1ByPattern("htf_.+_WrongCov_.+");
  CbmSimulationReport* report = new CbmLitFitQaReport();
  report->Create(fHM, fOutputDir);
  delete report;
}

void CbmLitFitQa::ReadDataBranches()
{
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman != NULL);

  fGlobalTracks     = (TClonesArray*) ioman->GetObject("GlobalTrack");
  fStsTracks        = (TClonesArray*) ioman->GetObject("StsTrack");
  fStsTrackMatches  = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  fStsHits          = (TClonesArray*) ioman->GetObject("StsHit");
  fMvdHits          = (TClonesArray*) ioman->GetObject("MvdHit");
  fMuchTracks       = (TClonesArray*) ioman->GetObject("MuchTrack");
  fMuchTrackMatches = (TClonesArray*) ioman->GetObject("MuchTrackMatch");
  fMuchPixelHits    = (TClonesArray*) ioman->GetObject("MuchPixelHit");
  fMuchStripHits    = (TClonesArray*) ioman->GetObject("MuchStripHit");
  fTrdTracks        = (TClonesArray*) ioman->GetObject("TrdTrack");
  fTrdTrackMatches  = (TClonesArray*) ioman->GetObject("TrdTrackMatch");
  fTrdHits          = (TClonesArray*) ioman->GetObject("TrdHit");

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");

  if (0 == mcManager) LOG(fatal) << "CbmMatchRecoToMC::ReadAndCreateDataBranches() NULL MCDataManager.";

  fMCTracks = mcManager->InitBranch("MCTrack");
  fEvents   = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  //    fPrimVertex = (CbmVertex*) ioman->GetObject("PrimaryVertex");
  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(ioman->GetObject("PrimaryVertex")); }
  if (nullptr == fPrimVertex) {
    //   LOG(fatal) << "No primary vertex";
  }
}

void CbmLitFitQa::ProcessGlobalTracks()
{
  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));
    ProcessStsTrack(globalTrack->GetStsTrackIndex());
    ProcessTrdTrack(globalTrack->GetTrdTrackIndex());
    ProcessMuchTrack(globalTrack->GetMuchTrackIndex());
  }
}

void CbmLitFitQa::ProcessStsTrack(Int_t trackId)
{
  if (NULL == fStsTracks || NULL == fStsTrackMatches || trackId < 0) return;

  CbmTrackMatchNew* match = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(trackId));
  if (match->GetNofLinks() < 1) return;
  Int_t mcId      = match->GetMatchedLink().GetIndex();
  Int_t mcEventId = match->GetMatchedLink().GetEntry();
  if (mcId < 0) return;  // Ghost or fake track

  // Check correctness of reconstructed track
  if (match->GetTrueOverAllHitsRatio() < fQuota) return;

  CbmStsTrack* track = static_cast<CbmStsTrack*>(fStsTracks->At(trackId));
  Int_t nofStsHits   = track->GetNofStsHits();
  Int_t nofMvdHits   = track->GetNofMvdHits();
  if (nofStsHits < 1) return;  // No hits in STS
  if (nofMvdHits < fMvdMinNofHits || nofStsHits + nofMvdHits < fStsMinNofHits)
    return;  // cut on number of hits in track

  const CbmLitMCTrack& mcTrack = fMCTrackCreator->GetTrack(mcEventId, mcId);

  const FairTrackParam* firstParam = track->GetParamFirst();
  const FairTrackParam* lastParam  = track->GetParamLast();

  FillTrackParamHistogramm("htp_Sts_FirstParam", firstParam);
  FillTrackParamHistogramm("htp_Sts_LastParam", lastParam);

  // Fill histograms for first track parameters
  if (nofMvdHits > 0) {  // first track parameters in MVD
    const CbmMvdHit* firstHit = static_cast<const CbmMvdHit*>(fMvdHits->At(track->GetMvdHitIndex(0)));
    Int_t firstStation        = firstHit->GetStationNr() - 1;  // to start with 0
    if (mcTrack.GetNofPointsAtStation(ECbmModuleId::kMvd, firstStation) > 0) {
      const CbmLitMCPoint& firstPoint = mcTrack.GetPointAtStation(ECbmModuleId::kMvd, firstStation, 0);
      FillResidualsAndPulls(firstParam, &firstPoint, "htf_Sts_FirstParam_", nofMvdHits + nofStsHits,
                            ECbmModuleId::kMvd);
    }
  }
  else {  // first track parameters in STS
    const CbmStsHit* firstHit = static_cast<const CbmStsHit*>(fStsHits->At(track->GetHitIndex(0)));
    Int_t firstStation        = CbmStsSetup::Instance()->GetStationNumber(firstHit->GetAddress())
                         - 1;  //firstHit->GetStationNr() - 1; // to start with 0
    if (mcTrack.GetNofPointsAtStation(ECbmModuleId::kSts, firstStation) > 0) {
      const CbmLitMCPoint& firstPoint = mcTrack.GetPointAtStation(ECbmModuleId::kSts, firstStation, 0);
      FillResidualsAndPulls(firstParam, &firstPoint, "htf_Sts_FirstParam_", nofMvdHits + nofStsHits,
                            ECbmModuleId::kSts);
    }
  }

  // Fill histograms for last track parameters
  const CbmStsHit* lastHit = static_cast<const CbmStsHit*>(fStsHits->At(track->GetHitIndex(nofStsHits - 1)));
  Int_t lastStation        = CbmStsSetup::Instance()->GetStationNumber(lastHit->GetAddress())
                      - 1;  //lastHit->GetStationNr() - 1; // to start with 0
  if (mcTrack.GetNofPointsAtStation(ECbmModuleId::kSts, lastStation) > 0) {
    const CbmLitMCPoint& lastPoint = mcTrack.GetPointAtStation(ECbmModuleId::kSts, lastStation, 0);
    FillResidualsAndPulls(lastParam, &lastPoint, "htf_Sts_LastParam_", nofMvdHits + nofStsHits, ECbmModuleId::kSts);
  }
}

void CbmLitFitQa::ProcessTrdTrack(Int_t trackId)
{
  if (NULL == fTrdTracks || NULL == fTrdTrackMatches || trackId < 0) return;

  CbmTrackMatchNew* match = static_cast<CbmTrackMatchNew*>(fTrdTrackMatches->At(trackId));
  Int_t mcId              = match->GetMatchedLink().GetIndex();
  Int_t mcEntryId         = match->GetMatchedLink().GetEntry();
  if (mcId < 0) return;  // Ghost or fake track

  // Check correctness of reconstructed track
  if (match->GetTrueOverAllHitsRatio() < fQuota) return;

  CbmTrack* track = static_cast<CbmTrack*>(fTrdTracks->At(trackId));
  Int_t nofHits   = track->GetNofHits();
  if (nofHits < 1) return;               // No hits
  if (nofHits < fTrdMinNofHits) return;  // cut on number of hits in track

  const CbmLitMCTrack& mcTrack = fMCTrackCreator->GetTrack(mcEntryId, mcId);

  const FairTrackParam* firstParam = track->GetParamFirst();
  const FairTrackParam* lastParam  = track->GetParamLast();

  FillTrackParamHistogramm("htp_Trd_FirstParam", firstParam);
  FillTrackParamHistogramm("htp_Trd_LastParam", lastParam);

  // Fill histograms for first track parameters
  const CbmHit* firstHit = static_cast<const CbmHit*>(fTrdHits->At(track->GetHitIndex(0)));
  //Int_t firstStation = 10 * CbmTrdAddress::GetStationNr(firstHit->GetAddress()) + CbmTrdAddress::GetLayerNr(firstHit->GetAddress());
  Int_t firstStation = CbmTrdAddress::GetLayerId(firstHit->GetAddress());
  if (mcTrack.GetNofPointsAtStation(ECbmModuleId::kTrd, firstStation) > 0) {
    const CbmLitMCPoint& firstPoint = mcTrack.GetPointAtStation(ECbmModuleId::kTrd, firstStation, 0);
    FillResidualsAndPulls(firstParam, &firstPoint, "htf_Trd_FirstParam_", nofHits, ECbmModuleId::kTrd);
  }

  // Fill histograms for last track parameters
  const CbmHit* lastHit = static_cast<const CbmHit*>(fTrdHits->At(track->GetHitIndex(nofHits - 1)));
  //Int_t lastStation = 10 * CbmTrdAddress::GetStationNr(lastHit->GetAddress()) + CbmTrdAddress::GetLayerNr(lastHit->GetAddress());
  Int_t lastStation = CbmTrdAddress::GetLayerId(lastHit->GetAddress());
  if (mcTrack.GetNofPointsAtStation(ECbmModuleId::kTrd, lastStation) > 0) {
    const CbmLitMCPoint& lastPoint = mcTrack.GetPointAtStation(ECbmModuleId::kTrd, lastStation, 0);
    FillResidualsAndPulls(lastParam, &lastPoint, "htf_Trd_LastParam_", nofHits, ECbmModuleId::kTrd);
  }
}

void CbmLitFitQa::ProcessMuchTrack(Int_t trackId)
{
  if (NULL == fMuchTracks || NULL == fMuchTrackMatches || trackId < 0) return;

  const CbmTrackMatchNew* match = static_cast<const CbmTrackMatchNew*>(fMuchTrackMatches->At(trackId));
  Int_t mcId                    = match->GetMatchedLink().GetIndex();
  Int_t mcEventId               = match->GetMatchedLink().GetEntry();
  if (mcId < 0) return;  // Ghost or fake track

  // Check correctness of reconstructed track
  if (match->GetTrueOverAllHitsRatio() < fQuota) return;

  CbmTrack* track = static_cast<CbmTrack*>(fMuchTracks->At(trackId));
  Int_t nofHits   = track->GetNofHits();
  if (nofHits < 1) return;                // No hits
  if (nofHits < fMuchMinNofHits) return;  // cut on number of hits in track

  const CbmLitMCTrack& mcTrack = fMCTrackCreator->GetTrack(mcEventId, mcId);

  const FairTrackParam* firstParam = track->GetParamFirst();
  const FairTrackParam* lastParam  = track->GetParamLast();

  FillTrackParamHistogramm("htp_Much_FirstParam", firstParam);
  FillTrackParamHistogramm("htp_Much_LastParam", lastParam);

  // Fill histograms for first track parameters
  const CbmHit* firstHit = static_cast<const CbmHit*>(fMuchPixelHits->At(track->GetHitIndex(0)));
  //   Int_t firstStation = firstHit->GetPlaneId();
  Int_t firstStation = 100 * CbmMuchGeoScheme::GetStationIndex(firstHit->GetAddress())
                       + 10 * CbmMuchGeoScheme::GetLayerIndex(firstHit->GetAddress())
                       + CbmMuchGeoScheme::GetLayerSideIndex(firstHit->GetAddress());
  if (mcTrack.GetNofPointsAtStation(ECbmModuleId::kMuch, firstStation) > 0) {
    const CbmLitMCPoint& firstPoint = mcTrack.GetPointAtStation(ECbmModuleId::kMuch, firstStation, 0);
    FillResidualsAndPulls(firstParam, &firstPoint, "htf_Much_FirstParam_", nofHits, ECbmModuleId::kMuch);
  }

  // Fill histograms for last track parameters
  const CbmHit* lastHit = static_cast<const CbmHit*>(fMuchPixelHits->At(track->GetHitIndex(nofHits - 1)));
  //   Int_t lastStation = lastHit->GetPlaneId();
  Int_t lastStation = 100 * CbmMuchGeoScheme::GetStationIndex(lastHit->GetAddress())
                      + 10 * CbmMuchGeoScheme::GetLayerIndex(lastHit->GetAddress())
                      + CbmMuchGeoScheme::GetLayerSideIndex(lastHit->GetAddress());
  if (mcTrack.GetNofPointsAtStation(ECbmModuleId::kMuch, lastStation) > 0) {
    const CbmLitMCPoint& lastPoint = mcTrack.GetPointAtStation(ECbmModuleId::kMuch, lastStation, 0);
    FillResidualsAndPulls(lastParam, &lastPoint, "htf_Much_LastParam_", nofHits, ECbmModuleId::kMuch);
  }
}

void CbmLitFitQa::FillResidualsAndPulls(const FairTrackParam* par, const CbmLitMCPoint* mcPoint, const string& histName,
                                        Float_t wrongPar, ECbmModuleId detId)
{
  // Residuals
  Double_t resX = 0., resY = 0., resTx = 0., resTy = 0., resQp = 0.;
  if (detId == ECbmModuleId::kMvd) {  // Use MC average for MVD
    resX  = par->GetX() - mcPoint->GetX();
    resY  = par->GetY() - mcPoint->GetY();
    resTx = par->GetTx() - mcPoint->GetTx();
    resTy = par->GetTy() - mcPoint->GetTy();
    resQp = par->GetQp() - mcPoint->GetQp();
  }
  else if (detId == ECbmModuleId::kSts) {  // Use MC average for STS
    resX  = par->GetX() - mcPoint->GetX();
    resY  = par->GetY() - mcPoint->GetY();
    resTx = par->GetTx() - mcPoint->GetTx();
    resTy = par->GetTy() - mcPoint->GetTy();
    resQp = par->GetQp() - mcPoint->GetQp();
  }
  else if (detId == ECbmModuleId::kTrd) {  // Use MC out for TRD
    resX  = par->GetX() - mcPoint->GetXOut();
    resY  = par->GetY() - mcPoint->GetYOut();
    resTx = par->GetTx() - mcPoint->GetTxOut();
    resTy = par->GetTy() - mcPoint->GetTyOut();
    resQp = par->GetQp() - mcPoint->GetQpOut();
  }
  else if (detId == ECbmModuleId::kMuch) {  // Use MC average for MUCH
    resX  = par->GetX() - mcPoint->GetX();
    resY  = par->GetY() - mcPoint->GetY();
    resTx = par->GetTx() - mcPoint->GetTx();
    resTy = par->GetTy() - mcPoint->GetTy();
    resQp = par->GetQp() - mcPoint->GetQp();
  }
  Double_t mcP = mcPoint->GetP();

  fHM->H2(histName + "Res_X")->Fill(mcP, resX);
  fHM->H2(histName + "Res_Y")->Fill(mcP, resY);
  fHM->H2(histName + "Res_Tx")->Fill(mcP, resTx);
  fHM->H2(histName + "Res_Ty")->Fill(mcP, resTy);
  fHM->H2(histName + "Res_Qp")->Fill(mcP, resQp);

  // Pulls
  Double_t C[15];
  par->CovMatrix(C);

  Double_t sigmaX  = (C[0] > 0.) ? std::sqrt(C[0]) : -1.;
  Double_t sigmaY  = (C[5] > 0.) ? std::sqrt(C[5]) : -1.;
  Double_t sigmaTx = (C[9] > 0.) ? std::sqrt(C[9]) : -1.;
  Double_t sigmaTy = (C[12] > 0.) ? std::sqrt(C[12]) : -1.;
  Double_t sigmaQp = (C[14] > 0.) ? std::sqrt(C[14]) : -1.;
  if (sigmaX < 0) fHM->H2(histName + "WrongCov_X")->Fill(mcP, wrongPar);
  else
    fHM->H2(histName + "Pull_X")->Fill(mcP, resX / sigmaX);
  if (sigmaY < 0) fHM->H2(histName + "WrongCov_Y")->Fill(mcP, wrongPar);
  else
    fHM->H2(histName + "Pull_Y")->Fill(mcP, resY / sigmaY);
  if (sigmaTx < 0) fHM->H2(histName + "WrongCov_Tx")->Fill(mcP, wrongPar);
  else
    fHM->H2(histName + "Pull_Tx")->Fill(mcP, resTx / sigmaTx);
  if (sigmaTy < 0) fHM->H2(histName + "WrongCov_Ty")->Fill(mcP, wrongPar);
  else
    fHM->H2(histName + "Pull_Ty")->Fill(mcP, resTy / sigmaTy);
  if (sigmaQp < 0) fHM->H2(histName + "WrongCov_Qp")->Fill(mcP, wrongPar);
  else
    fHM->H2(histName + "Pull_Qp")->Fill(mcP, resQp / sigmaQp);
}

void CbmLitFitQa::FillTrackParamHistogramm(const string& histName, const FairTrackParam* par)
{
  fHM->H1(histName + "_X")->Fill(par->GetX());
  fHM->H1(histName + "_Y")->Fill(par->GetY());
  fHM->H1(histName + "_Z")->Fill(par->GetZ());
  fHM->H1(histName + "_Tx")->Fill(par->GetTx());
  fHM->H1(histName + "_Ty")->Fill(par->GetTy());
  fHM->H1(histName + "_Qp")->Fill(par->GetQp());
  Double_t p = (par->GetQp() != 0) ? std::fabs(1. / par->GetQp()) : 0.;
  fHM->H1(histName + "_p")->Fill(p);
  TVector3 mom;
  par->Momentum(mom);
  Double_t pt = std::sqrt(mom.X() * mom.X() + mom.Y() * mom.Y());
  fHM->H1(histName + "_pt")->Fill(pt);
}

void CbmLitFitQa::ProcessTrackParamsAtVertex()
{
  if (fEvents) {
    Int_t nofEvents = fEvents->GetEntriesFast();

    for (int i = 0; i < nofEvents; ++i)
      ProcessTrackParamsAtVertex(static_cast<CbmEvent*>(fEvents->At(i)));
  }
  else
    ProcessTrackParamsAtVertex(0);
}

void CbmLitFitQa::ProcessTrackParamsAtVertex(CbmEvent* event)
{
  Int_t nofTracks = event ? event->GetNofData(ECbmDataType::kStsTrack) : fStsTracks->GetEntriesFast();

  for (Int_t i = 0; i < nofTracks; ++i) {
    Int_t iTrack       = event ? event->GetIndex(ECbmDataType::kStsTrack, i) : i;
    CbmStsTrack* track = static_cast<CbmStsTrack*>(fStsTracks->At(iTrack));

    CbmTrackMatchNew* match = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(iTrack));
    if (match->GetNofLinks() < 1) continue;
    Int_t mcId      = match->GetMatchedLink().GetIndex();
    Int_t mcEventId = match->GetMatchedLink().GetEntry();
    if (mcId < 0) continue;  // Ghost or fake track

    const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, mcEventId, mcId));
    if (mcTrack->GetMotherId() != -1) continue;  // only for primaries
    //if (mcTrack->GetP() < 1.) continue; // only for momentum large 1 GeV/c

    // Check correctness of reconstructed track
    if (match->GetTrueOverAllHitsRatio() < fQuota) continue;

    CbmVertex* primVertex = event ? event->GetVertex() : fPrimVertex;
    fKFFitter.DoFit(track, 211);
    Double_t chiPrimary = fKFFitter.GetChiToVertex(track, primVertex);
    fHM->H1("htf_ChiPrimary")->Fill(chiPrimary);

    FairTrackParam vtxTrack;
    fKFFitter.FitToVertex(track, primVertex, &vtxTrack);

    TVector3 momMC, momRec;
    mcTrack->GetMomentum(momMC);
    vtxTrack.Momentum(momRec);

    Double_t dpp = 100. * (momMC.Mag() - momRec.Mag()) / momMC.Mag();
    fHM->H1("htf_MomRes_Mom")->Fill(momMC.Mag(), dpp);
  }
}

void CbmLitFitQa::ProcessTrackMomentumAtVertex()
{
  Int_t nofTracks = fGlobalTracks->GetEntriesFast();

  for (int i = 0; i < nofTracks; ++i) {
    CbmGlobalTrack* globalTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(i));
    Int_t stsId                 = globalTrack->GetStsTrackIndex();
    CbmStsTrack* stsTrack       = static_cast<CbmStsTrack*>(fStsTracks->At(stsId));
    CbmTrackMatchNew* match     = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(stsId));

    if (match->GetNofLinks() < 1) continue;

    Int_t mcId      = match->GetMatchedLink().GetIndex();
    Int_t mcEventId = match->GetMatchedLink().GetEntry();

    if (mcId < 0) continue;  // Ghost or fake track

    const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, mcEventId, mcId));

    if (mcTrack->GetMotherId() != -1) continue;  // only for primaries

    // Check correctness of reconstructed track
    if (match->GetTrueOverAllHitsRatio() < fQuota) continue;

    const CbmTrackParam* vtxParam = globalTrack->GetParamVertex();
    TVector3 momMC;
    mcTrack->GetMomentum(momMC);

    fHM->H1("htp_PrimaryVertexResidualPx")->Fill(vtxParam->GetPx() - momMC.x());
    fHM->H1("htp_PrimaryVertexResidualPy")->Fill(vtxParam->GetPy() - momMC.y());
    fHM->H1("htp_PrimaryVertexResidualPz")->Fill(vtxParam->GetPz() - momMC.z());

    fHM->H1("htp_PrimaryVertexPullPx")->Fill((vtxParam->GetPx() - momMC.x()) / vtxParam->GetDpx());
    fHM->H1("htp_PrimaryVertexPullPy")->Fill((vtxParam->GetPy() - momMC.y()) / vtxParam->GetDpy());
    fHM->H1("htp_PrimaryVertexPullPz")->Fill((vtxParam->GetPz() - momMC.z()) / vtxParam->GetDpz());

    fHM->H1("htp_PrimaryVertexResidualTx")->Fill(vtxParam->GetTx() - mcTrack->GetPx() / mcTrack->GetPz());
    fHM->H1("htp_PrimaryVertexResidualTy")->Fill(vtxParam->GetTy() - mcTrack->GetPy() / mcTrack->GetPz());
    fHM->H1("htp_PrimaryVertexResidualQp")->Fill(vtxParam->GetQp() - 1 / mcTrack->GetP());

    Double_t cov[15];
    vtxParam->CovMatrix(cov);
    fHM->H1("htp_PrimaryVertexPullTx")
      ->Fill((vtxParam->GetTx() - mcTrack->GetPx() / mcTrack->GetPz()) / TMath::Sqrt(cov[9]));
    fHM->H1("htp_PrimaryVertexPullTy")
      ->Fill((vtxParam->GetTy() - mcTrack->GetPy() / mcTrack->GetPz()) / TMath::Sqrt(cov[12]));
    fHM->H1("htp_PrimaryVertexPullQp")->Fill((vtxParam->GetQp() - 1 / mcTrack->GetP()) / TMath::Sqrt(cov[14]));
  }
}

void CbmLitFitQa::CreateHistograms()
{
  CreateResidualAndPullHistograms(ECbmModuleId::kSts, "Sts");
  CreateTrackParamHistograms(ECbmModuleId::kSts, "Sts");

  CreateResidualAndPullHistograms(ECbmModuleId::kTrd, "Trd");
  CreateTrackParamHistograms(ECbmModuleId::kTrd, "Trd");

  CreateResidualAndPullHistograms(ECbmModuleId::kMuch, "Much");
  CreateTrackParamHistograms(ECbmModuleId::kMuch, "Much");

  // Momentum resolution vs momwntum
  fHM->Add("htf_MomRes_Mom", new TH2F("htf_MomRes_Mom", "htf_MomRes_Mom;P [GeV/c];dP/P [%]", fPRangeBins, fPRangeMin,
                                      fPRangeMax, 100, -3., 3.));
  fHM->Add("htf_ChiPrimary", new TH1F("htf_ChiPrimary", "htf_ChiPrimary;#chi_{primary}", 100, 0, 10));
  fHM->Add("htp_PrimaryVertexResidualPx",
           new TH1F("htp_PrimaryVertexResidualPx", "htp_PrimaryVertexResidualPx;[cm]", 200, -1., 1.));
  fHM->Add("htp_PrimaryVertexResidualPy",
           new TH1F("htp_PrimaryVertexResidualPy", "htp_PrimaryVertexResidualPy;[cm]", 200, -1., 1.));
  fHM->Add("htp_PrimaryVertexResidualPz",
           new TH1F("htp_PrimaryVertexResidualPz", "htp_PrimaryVertexResidualPz;[cm]", 200, -1., 1.));
  fHM->Add("htp_PrimaryVertexPullPx",
           new TH1F("htp_PrimaryVertexPullPx", "htp_PrimaryVertexPullPx;[cm]", 200, -5., 5.));
  fHM->Add("htp_PrimaryVertexPullPy",
           new TH1F("htp_PrimaryVertexPullPy", "htp_PrimaryVertexPullPy;[cm]", 200, -5., 5.));
  fHM->Add("htp_PrimaryVertexPullPz",
           new TH1F("htp_PrimaryVertexPullPz", "htp_PrimaryVertexPullPz;[cm]", 200, -5., 5.));

  fHM->Add("htp_PrimaryVertexResidualTx",
           new TH1F("htp_PrimaryVertexResidualTx", "htp_PrimaryVertexResidualTx;[cm]", 200, -1., 1.));
  fHM->Add("htp_PrimaryVertexResidualTy",
           new TH1F("htp_PrimaryVertexResidualTy", "htp_PrimaryVertexResidualTy;[cm]", 200, -1., 1.));
  fHM->Add("htp_PrimaryVertexResidualQp",
           new TH1F("htp_PrimaryVertexResidualQ[", "htp_PrimaryVertexResidualQp;[cm]", 200, -1., 1.));
  fHM->Add("htp_PrimaryVertexPullTx",
           new TH1F("htp_PrimaryVertexPullTx", "htp_PrimaryVertexPullTx;[cm]", 200, -5., 5.));
  fHM->Add("htp_PrimaryVertexPullTy",
           new TH1F("htp_PrimaryVertexPullTy", "htp_PrimaryVertexPullTy;[cm]", 200, -5., 5.));
  fHM->Add("htp_PrimaryVertexPullQp",
           new TH1F("htp_PrimaryVertexPullQp", "htp_PrimaryVertexPullQp;[cm]", 200, -5., 5.));
}

void CbmLitFitQa::CreateResidualAndPullHistograms(ECbmModuleId detId, const string& detName)
{
  if (!fDet.GetDet(detId)) return;

  // Parameter names of the state vector (x, y, tx, ty, q/p)
  string parameterNames[] = {"X", "Y", "Tx", "Ty", "Qp"};

  // Axis titles for residual, pull and wrong covariance histograms
  string xTitles[] = {
    "Residual X [cm]", "Residual Y [cm]", "Residual Tx",    "Residual Ty",    "Residual q/p [(GeV/c)^{-1}]",
    "Pull X",          "Pull Y",          "Pull Tx",        "Pull Ty",        "Pull q/p",
    "Number of hits",  "Number of hits",  "Number of hits", "Number of hits", "Number of hits"};

  // Category names: Residual, Pull, Wrong Covariance
  string catNames[] = {"Res", "Pull", "WrongCov"};

  vector<Int_t> bins(15, 200);
  vector<pair<Float_t, Float_t>> bounds;
  if (fIsFixedBounds) {
    vector<pair<Float_t, Float_t>> tmp = boost::assign::list_of(make_pair(-1., 1.))  // X residual
      (make_pair(-1., 1.))                                                           // Y residual
      (make_pair(-.01, .01))                                                         // Tx residual
      (make_pair(-.01, .01))                                                         // Ty residual
      (make_pair(-.1, .1))                                                           // Qp residual
      (make_pair(-5., 5.))                                                           // X pull
      (make_pair(-5., 5.))                                                           // Y pull
      (make_pair(-5., 5.))                                                           // Tx pull
      (make_pair(-5., 5.))                                                           // Ty pull
      (make_pair(-7., 7.))                                                           // Qp pull
      (make_pair(0., 200.))                                                          // X wrong covariance
      (make_pair(0., 200.))                                                          // Y wrong covariance
      (make_pair(0., 200.))                                                          // Tx wrong covariance
      (make_pair(0., 200.))                                                          // Ty wrong covariance
      (make_pair(0., 200.));                                                         // Qp wrong covariance
    bounds = tmp;
  }
  else {
    bounds.assign(15, make_pair(0., 0.));
  }

  Double_t momentumMin     = 0.;
  Double_t momentumMax     = 10.;
  Int_t nofMomentumBins    = 20;
  string momentumAxisTitle = "P [GeV/c]";

  // [0] - for the first track parameter, [1] - for the last track parameter
  for (Int_t i = 0; i < 2; i++) {
    string trackParamName = (i == 0) ? "FirstParam" : "LastParam";
    // [0] - "Res", [1] - "Pull", [2] - "WrongCov"
    for (Int_t iCat = 0; iCat < 3; iCat++) {
      for (Int_t iPar = 0; iPar < 5; iPar++) {
        string histName = "htf_" + detName + "_" + trackParamName + "_" + catNames[iCat] + "_" + parameterNames[iPar];
        Int_t histId    = iCat * 5 + iPar;
        fHM->Add(histName,
                 new TH2F(histName.c_str(),
                          string(histName + ";" + momentumAxisTitle + +";" + xTitles[histId] + ";Counter").c_str(),
                          nofMomentumBins, momentumMin, momentumMax, bins[histId], bounds[histId].first,
                          bounds[histId].second));
      }
    }
  }
}

void CbmLitFitQa::CreateTrackParamHistograms(ECbmModuleId detId, const string& detName)
{
  if (!fDet.GetDet(detId)) return;

  // Parameter names of the state vector (x, y, tx, ty, q/p)
  string parameterNames[] = {"X", "Y", "Z", "Tx", "Ty", "Qp", "p", "pt"};

  // Axis titles for state vector Components
  string xTitles[] = {"X [cm]",           "Y [cm]",       "Z [cm]", "Tx", "Ty", "q/p [(GeV/c)^{-1}]",
                      "Momentum [GeV/c]", "P_{t} [GeV/c]"};

  vector<Int_t> bins(8, 200);
  vector<pair<Float_t, Float_t>> bounds;
  if (fIsFixedBounds) {
    vector<pair<Float_t, Float_t>> tmp = boost::assign::list_of(make_pair(-500., 500.))  // X
      (make_pair(-500., 500.))                                                           // Y
      (make_pair(-500., 500.))                                                           // Z
      (make_pair(-1., 1.))                                                               // Tx
      (make_pair(-1., 1.))                                                               // Ty
      (make_pair(-2., 2.))                                                               // Qp
      (make_pair(0., 25.))                                                               // Momentum
      (make_pair(0., 5.));                                                               // Pt
    bounds = tmp;
  }
  else {
    bounds.assign(8, make_pair(0., 0.));
  }

  // [0] - for the first track parameter, [1] - for the last track parameter
  for (Int_t i = 0; i < 2; i++) {
    string trackParamName = (i == 0) ? "FirstParam" : "LastParam";
    for (Int_t iPar = 0; iPar < 8; iPar++) {
      string histName = "htp_" + detName + "_" + trackParamName + "_" + parameterNames[iPar];
      fHM->Add(histName, new TH1F(histName.c_str(), string(histName + ";" + xTitles[iPar] + ";Counter").c_str(),
                                  bins[iPar], bounds[iPar].first, bounds[iPar].second));
    }
  }
}
ClassImp(CbmLitFitQa)
