/** @file CbmTofHitFinderQa.cxx
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 27/08/2015
 **/

#include "CbmTofHitFinderQa.h"

// TOF Classes and includes
#include "CbmTofAddress.h"          // in cbmdata/tof
#include "CbmTofCell.h"             // in tof/TofData
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDigi.h"             // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"              // in cbmdata/tof
#include "CbmTofPoint.h"            // in cbmdata/tof

// CBMroot classes and includes
#include "CbmMCTrack.h"
#include "CbmMatch.h"

// FAIR classes and includes
#include "FairLogger.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ROOT Classes and includes
#include "Riostream.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"

using std::cout;
using std::endl;

//___________________________________________________________________
// Constants definitions: Particles list
const Int_t kiNbPart                = 13;
const TString ksPartTag[kiNbPart]   = {"others",
                                     "ep",
                                     "em",
                                     "pip",
                                     "pim",
                                     "kp",
                                     "km",
                                     "p",
                                     "pbar",
                                     "d",
                                     "t",
                                     "he",
                                     "a"};
const Int_t kiPartPdgCode[kiNbPart] = {0,
                                       11,
                                       -11,
                                       211,
                                       -211,
                                       321,
                                       -321,
                                       2212,
                                       -2212,
                                       1000010020,
                                       1000010030,
                                       1000020030,
                                       1000020040};
const TString ksPartName[kiNbPart]  = {"any other part.",
                                      "e+",
                                      "e-",
                                      "#pi+",
                                      "#pi-",
                                      "k+",
                                      "k-",
                                      "p",
                                      "anti-p",
                                      "d",
                                      "t",
                                      "he",
                                      "#alpha"};
const Int_t kiMinNbStsPntAcc =
  3;  // Number of STS Pnt for Trk to be reconstructable
//___________________________________________________________________


//___________________________________________________________________
//
// CbmTofHitFinderQa
//
// Task for QA of TOF event based hit finders and digitizers
//
// ------------------------------------------------------------------
CbmTofHitFinderQa::CbmTofHitFinderQa()
  : FairTask("TofHitFinderQa")
  , fEvents(0)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fChannelInfo(NULL)
  , iNbSmTot(0)
  , fvTypeSmOffs()
  , iNbRpcTot(0)
  , fvSmRpcOffs()
  , fiNbChTot(0)
  , fvRpcChOffs()
  , fDigiPar(NULL)
  , fDigiBdfPar(NULL)
  , fMCEventHeader(NULL)
  , fTofPointsColl(NULL)
  , fMcTracksColl(NULL)
  , fTofDigisColl(NULL)
  , fTofDigiMatchPointsColl(NULL)
  , fTofHitsColl(NULL)
  , fTofDigiMatchColl(NULL)
  , fTofHitMatchColl(NULL)
  , fbHitProducerSource(kFALSE)
  , fRealTofPointsColl(NULL)
  , fRealTofMatchColl(NULL)
  , fbRealPointAvail(kFALSE)
  , fbNormHistGenMode(kFALSE)
  , fsHistoInNormCartFilename("")
  , fsHistoInNormAngFilename("")
  , fsHistoInNormSphFilename("")
  , fsHistoOutFilename("./tofQa.hst.root")
  , fdWallPosZ(1000)
  , fuNbEventsForHitsNbPlots(30000)
  , fhNbHitsPerEvent(NULL)
  , fhNbHitsSingPntPerEvent(NULL)
  , fhNbHitsMultPntPerEvent(NULL)
  , fhNbHitsSingTrkPerEvent(NULL)
  , fhNbHitsMultTrkPerEvent(NULL)
  , fhNbDigisInHit(NULL)
  , fhNbDigisInHitMapXY(NULL)
  , fvhTrackAllStartZCent()
  ,  // Beam pipe check
  fvhTrackSecStartZCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZ()
  ,  // Beam pipe check
  fvhTrackAllStartYZ()
  ,  // Beam pipe check
  fvhTofPntAllAngCent()
  ,  // Beam pipe check
  fhTrackMapXY(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapXZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapYZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapAng(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapSph(NULL)
  ,  // Only when creating normalization histos
  fhPointMapXY(NULL)
  , fhPointMapXZ(NULL)
  , fhPointMapYZ(NULL)
  , fhPointMapAng(NULL)
  , fhPointMapSph(NULL)
  , fhRealPointMapXY(NULL)
  , fhRealPointMapXZ(NULL)
  , fhRealPointMapYZ(NULL)
  , fhRealPointMapAng(NULL)
  , fhRealPointMapSph(NULL)
  , fhDigiMapXY(NULL)
  , fhDigiMapXZ(NULL)
  , fhDigiMapYZ(NULL)
  , fhDigiMapAng(NULL)
  , fhDigiMapSph(NULL)
  , fhHitMapXY(NULL)
  , fhHitMapXZ(NULL)
  , fhHitMapYZ(NULL)
  , fhHitMapAng(NULL)
  , fhHitMapSph(NULL)
  , fhLeftRightDigiMatch(NULL)
  , fhNbPointsInHit(NULL)
  , fhNbTracksInHit(NULL)
  , fhHitMapSingPntXY(NULL)
  , fhHitMapSingPntXZ(NULL)
  , fhHitMapSingPntYZ(NULL)
  , fhHitMapSingPntAng(NULL)
  , fhHitMapSingPntSph(NULL)
  , fhHitMapMultPntXY(NULL)
  , fhHitMapMultPntXZ(NULL)
  , fhHitMapMultPntYZ(NULL)
  , fhHitMapMultPntAng(NULL)
  , fhHitMapMultPntSph(NULL)
  , fhHitMapSingTrkXY(NULL)
  , fhHitMapSingTrkXZ(NULL)
  , fhHitMapSingTrkYZ(NULL)
  , fhHitMapSingTrkAng(NULL)
  , fhHitMapSingTrkSph(NULL)
  , fhHitMapMultTrkXY(NULL)
  , fhHitMapMultTrkXZ(NULL)
  , fhHitMapMultTrkYZ(NULL)
  , fhHitMapMultTrkAng(NULL)
  , fhHitMapMultTrkSph(NULL)
  , fhSinglePointHitDeltaX(NULL)
  , fhSinglePointHitDeltaY(NULL)
  , fhSinglePointHitDeltaZ(NULL)
  , fhSinglePointHitDeltaR(NULL)
  , fhSinglePointHitDeltaT(NULL)
  , fhSinglePointHitPullX(NULL)
  , fhSinglePointHitPullY(NULL)
  , fhSinglePointHitPullZ(NULL)
  , fhSinglePointHitPullR(NULL)
  , fhSinglePointHitPullT(NULL)
  , fhCltSzSinglePointHitPullX(NULL)
  , fhCltSzSinglePointHitPullY(NULL)
  , fhCltSzSinglePointHitPullZ(NULL)
  , fhCltSzSinglePointHitPullR(NULL)
  , fhCltSzSinglePointHitPullT(NULL)
  , fhMultiPntHitClosestDeltaX(NULL)
  , fhMultiPntHitClosestDeltaY(NULL)
  , fhMultiPntHitClosestDeltaZ(NULL)
  , fhMultiPntHitClosestDeltaR(NULL)
  , fhMultiPntHitClosestDeltaT(NULL)
  , fhMultiPntHitClosestPullX(NULL)
  , fhMultiPntHitClosestPullY(NULL)
  , fhMultiPntHitClosestPullZ(NULL)
  , fhMultiPntHitClosestPullR(NULL)
  , fhMultiPntHitClosestPullT(NULL)
  , fhMultiPntHitFurthestDeltaX(NULL)
  , fhMultiPntHitFurthestDeltaY(NULL)
  , fhMultiPntHitFurthestDeltaZ(NULL)
  , fhMultiPntHitFurthestDeltaR(NULL)
  , fhMultiPntHitFurthestDeltaT(NULL)
  , fhMultiPntHitFurthestPullX(NULL)
  , fhMultiPntHitFurthestPullY(NULL)
  , fhMultiPntHitFurthestPullZ(NULL)
  , fhMultiPntHitFurthestPullR(NULL)
  , fhMultiPntHitFurthestPullT(NULL)
  , fhMultiPntHitMeanDeltaX(NULL)
  , fhMultiPntHitMeanDeltaY(NULL)
  , fhMultiPntHitMeanDeltaZ(NULL)
  , fhMultiPntHitMeanDeltaR(NULL)
  , fhMultiPntHitMeanDeltaT(NULL)
  , fhMultiPntHitMeanPullX(NULL)
  , fhMultiPntHitMeanPullY(NULL)
  , fhMultiPntHitMeanPullZ(NULL)
  , fhMultiPntHitMeanPullR(NULL)
  , fhMultiPntHitMeanPullT(NULL)
  , fhMultiPntHitBestDeltaX(NULL)
  , fhMultiPntHitBestDeltaY(NULL)
  , fhMultiPntHitBestDeltaZ(NULL)
  , fhMultiPntHitBestDeltaR(NULL)
  , fhMultiPntHitBestDeltaT(NULL)
  , fhMultiPntHitBestPullX(NULL)
  , fhMultiPntHitBestPullY(NULL)
  , fhMultiPntHitBestPullZ(NULL)
  , fhMultiPntHitBestPullR(NULL)
  , fhMultiPntHitBestPullT(NULL)
  , fhSingleTrackHitDeltaX(NULL)
  , fhSingleTrackHitDeltaY(NULL)
  , fhSingleTrackHitDeltaZ(NULL)
  , fhSingleTrackHitDeltaR(NULL)
  , fhSingleTrackHitDeltaT(NULL)
  , fhSingleTrackHitPullX(NULL)
  , fhSingleTrackHitPullY(NULL)
  , fhSingleTrackHitPullZ(NULL)
  , fhSingleTrackHitPullR(NULL)
  , fhSingleTrackHitPullT(NULL)
  , fhSingTrkMultiPntHitDeltaX(NULL)
  , fhSingTrkMultiPntHitDeltaY(NULL)
  , fhSingTrkMultiPntHitDeltaZ(NULL)
  , fhSingTrkMultiPntHitDeltaR(NULL)
  , fhSingTrkMultiPntHitDeltaT(NULL)
  , fhSingTrkMultiPntHitPullX(NULL)
  , fhSingTrkMultiPntHitPullY(NULL)
  , fhSingTrkMultiPntHitPullZ(NULL)
  , fhSingTrkMultiPntHitPullR(NULL)
  , fhSingTrkMultiPntHitPullT(NULL)
  , fhMultiTrkHitClosestDeltaX(NULL)
  , fhMultiTrkHitClosestDeltaY(NULL)
  , fhMultiTrkHitClosestDeltaZ(NULL)
  , fhMultiTrkHitClosestDeltaR(NULL)
  , fhMultiTrkHitClosestDeltaT(NULL)
  , fhMultiTrkHitClosestPullX(NULL)
  , fhMultiTrkHitClosestPullY(NULL)
  , fhMultiTrkHitClosestPullZ(NULL)
  , fhMultiTrkHitClosestPullR(NULL)
  , fhMultiTrkHitClosestPullT(NULL)
  , fhMultiTrkHitFurthestDeltaX(NULL)
  , fhMultiTrkHitFurthestDeltaY(NULL)
  , fhMultiTrkHitFurthestDeltaZ(NULL)
  , fhMultiTrkHitFurthestDeltaR(NULL)
  , fhMultiTrkHitFurthestDeltaT(NULL)
  , fhMultiTrkHitFurthestPullX(NULL)
  , fhMultiTrkHitFurthestPullY(NULL)
  , fhMultiTrkHitFurthestPullZ(NULL)
  , fhMultiTrkHitFurthestPullR(NULL)
  , fhMultiTrkHitFurthestPullT(NULL)
  , fhMultiTrkHitMeanDeltaX(NULL)
  , fhMultiTrkHitMeanDeltaY(NULL)
  , fhMultiTrkHitMeanDeltaZ(NULL)
  , fhMultiTrkHitMeanDeltaR(NULL)
  , fhMultiTrkHitMeanDeltaT(NULL)
  , fhMultiTrkHitMeanPullX(NULL)
  , fhMultiTrkHitMeanPullY(NULL)
  , fhMultiTrkHitMeanPullZ(NULL)
  , fhMultiTrkHitMeanPullR(NULL)
  , fhMultiTrkHitMeanPullT(NULL)
  , fhMultiTrkHitBestDeltaX(NULL)
  , fhMultiTrkHitBestDeltaY(NULL)
  , fhMultiTrkHitBestDeltaZ(NULL)
  , fhMultiTrkHitBestDeltaR(NULL)
  , fhMultiTrkHitBestDeltaT(NULL)
  , fhMultiTrkHitBestPullX(NULL)
  , fhMultiTrkHitBestPullY(NULL)
  , fhMultiTrkHitBestPullZ(NULL)
  , fhMultiTrkHitBestPullR(NULL)
  , fhMultiTrkHitBestPullT(NULL)
  , fvhPtmRapGenTrk()
  , fvhPtmRapStsPnt()
  , fvhPtmRapTofPnt()
  , fvhPtmRapTofHit()
  , fvhPtmRapTofHitSinglePnt()
  , fvhPtmRapTofHitSingleTrk()
  , fvhPtmRapSecGenTrk()
  , fvhPtmRapSecStsPnt()
  , fvhPtmRapSecTofPnt()
  , fvhPtmRapSecTofHit()
  , fvhPtmRapSecTofHitSinglePnt()
  , fvhPtmRapSecTofHitSingleTrk()
  , fvhPlabGenTrk()
  , fvhPlabStsPnt()
  , fvhPlabTofPnt()
  , fvhPlabTofHit()
  , fvhPlabTofHitSinglePnt()
  , fvhPlabTofHitSingleTrk()
  , fvhPlabSecGenTrk()
  , fvhPlabSecStsPnt()
  , fvhPlabSecTofPnt()
  , fvhPlabSecTofHit()
  , fvhPlabSecTofHitSinglePnt()
  , fvhPlabSecTofHitSingleTrk()
  , fvhPtmRapGenTrkTofPnt()
  , fvhPtmRapGenTrkTofHit()
  , fvhPlabGenTrkTofPnt()
  , fvhPlabGenTrkTofHit()
  , fvhPlabStsTrkTofPnt()
  , fvhPlabStsTrkTofHit()
  , fvhPtmRapSecGenTrkTofPnt()
  , fvhPtmRapSecGenTrkTofHit()
  , fvhPlabSecGenTrkTofPnt()
  , fvhPlabSecGenTrkTofHit()
  , fvhPlabSecStsTrkTofPnt()
  , fvhPlabSecStsTrkTofHit()
  , fvulIdxTracksWithPnt()
  , fvulIdxTracksWithHit()
  , fhIntegratedHitPntEff(NULL)
  , fvulIdxPrimTracksWithPnt()
  , fvulIdxPrimTracksWithHit()
  , fhIntegratedHitPntEffPrim(NULL)
  , fvulIdxSecTracksWithPnt()
  , fvulIdxSecTracksWithHit()
  , fhIntegratedHitPntEffSec(NULL)
  , fvulIdxHiddenTracksWithHit()
  , fhIntegratedHiddenHitPntLoss(NULL)
  , fvulIdxHiddenPrimTracksWithHit()
  , fhIntegratedHiddenHitPntLossPrim(NULL)
  , fvulIdxHiddenSecTracksWithHit()
  , fhIntegratedHiddenHitPntLossSec(NULL)
  , fvulIdxTracksWithPntGaps()
  , fvulIdxTracksWithHitGaps()
  , fhIntegratedHitPntEffGaps(NULL)
  , fvulIdxPrimTracksWithPntGaps()
  , fvulIdxPrimTracksWithHitGaps()
  , fhIntegratedHitPntEffPrimGaps(NULL)
  , fvulIdxSecTracksWithPntGaps()
  , fvulIdxSecTracksWithHitGaps()
  , fhIntegratedHitPntEffSecGaps(NULL)
  , fhMcTrkStartPrimSingTrk(NULL)
  , fhMcTrkStartSecSingTrk(NULL)
  , fhMcTrkStartPrimMultiTrk(NULL)
  , fhMcTrkStartSecMultiTrk(NULL)
  , fhPointMatchWeight(NULL) {
  cout << "CbmTofHitFinderQa: Task started " << endl;
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmTofHitFinderQa::CbmTofHitFinderQa(const char* name, Int_t verbose)
  : FairTask(name, verbose)
  , fEvents(0)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fChannelInfo(NULL)
  , iNbSmTot(0)
  , fvTypeSmOffs()
  , iNbRpcTot(0)
  , fvSmRpcOffs()
  , fiNbChTot(0)
  , fvRpcChOffs()
  , fDigiPar(NULL)
  , fDigiBdfPar(NULL)
  , fMCEventHeader(NULL)
  , fTofPointsColl(NULL)
  , fMcTracksColl(NULL)
  , fTofDigisColl(NULL)
  , fTofDigiMatchPointsColl(NULL)
  , fTofHitsColl(NULL)
  , fTofDigiMatchColl(NULL)
  , fTofHitMatchColl(NULL)
  , fbHitProducerSource(kFALSE)
  , fRealTofPointsColl(NULL)
  , fRealTofMatchColl(NULL)
  , fbRealPointAvail(kFALSE)
  , fbNormHistGenMode(kFALSE)
  , fsHistoInNormCartFilename("")
  , fsHistoInNormAngFilename("")
  , fsHistoInNormSphFilename("")
  , fsHistoOutFilename("./tofQa.hst.root")
  , fdWallPosZ(1000)
  , fuNbEventsForHitsNbPlots(30000)
  , fhNbHitsPerEvent(NULL)
  , fhNbHitsSingPntPerEvent(NULL)
  , fhNbHitsMultPntPerEvent(NULL)
  , fhNbHitsSingTrkPerEvent(NULL)
  , fhNbHitsMultTrkPerEvent(NULL)
  , fhNbDigisInHit(NULL)
  , fhNbDigisInHitMapXY(NULL)
  , fvhTrackAllStartZCent()
  ,  // Beam pipe check
  fvhTrackSecStartZCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZ()
  ,  // Beam pipe check
  fvhTrackAllStartYZ()
  ,  // Beam pipe check
  fvhTofPntAllAngCent()
  ,  // Beam pipe check
  fhTrackMapXY(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapXZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapYZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapAng(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapSph(NULL)
  ,  // Only when creating normalization histos
  fhPointMapXY(NULL)
  , fhPointMapXZ(NULL)
  , fhPointMapYZ(NULL)
  , fhPointMapAng(NULL)
  , fhPointMapSph(NULL)
  , fhRealPointMapXY(NULL)
  , fhRealPointMapXZ(NULL)
  , fhRealPointMapYZ(NULL)
  , fhRealPointMapAng(NULL)
  , fhRealPointMapSph(NULL)
  , fhDigiMapXY(NULL)
  , fhDigiMapXZ(NULL)
  , fhDigiMapYZ(NULL)
  , fhDigiMapAng(NULL)
  , fhDigiMapSph(NULL)
  , fhHitMapXY(NULL)
  , fhHitMapXZ(NULL)
  , fhHitMapYZ(NULL)
  , fhHitMapAng(NULL)
  , fhHitMapSph(NULL)
  , fhLeftRightDigiMatch(NULL)
  , fhNbPointsInHit(NULL)
  , fhNbTracksInHit(NULL)
  , fhHitMapSingPntXY(NULL)
  , fhHitMapSingPntXZ(NULL)
  , fhHitMapSingPntYZ(NULL)
  , fhHitMapSingPntAng(NULL)
  , fhHitMapSingPntSph(NULL)
  , fhHitMapMultPntXY(NULL)
  , fhHitMapMultPntXZ(NULL)
  , fhHitMapMultPntYZ(NULL)
  , fhHitMapMultPntAng(NULL)
  , fhHitMapMultPntSph(NULL)
  , fhHitMapSingTrkXY(NULL)
  , fhHitMapSingTrkXZ(NULL)
  , fhHitMapSingTrkYZ(NULL)
  , fhHitMapSingTrkAng(NULL)
  , fhHitMapSingTrkSph(NULL)
  , fhHitMapMultTrkXY(NULL)
  , fhHitMapMultTrkXZ(NULL)
  , fhHitMapMultTrkYZ(NULL)
  , fhHitMapMultTrkAng(NULL)
  , fhHitMapMultTrkSph(NULL)
  , fhSinglePointHitDeltaX(NULL)
  , fhSinglePointHitDeltaY(NULL)
  , fhSinglePointHitDeltaZ(NULL)
  , fhSinglePointHitDeltaR(NULL)
  , fhSinglePointHitDeltaT(NULL)
  , fhSinglePointHitPullX(NULL)
  , fhSinglePointHitPullY(NULL)
  , fhSinglePointHitPullZ(NULL)
  , fhSinglePointHitPullR(NULL)
  , fhSinglePointHitPullT(NULL)
  , fhCltSzSinglePointHitPullX(NULL)
  , fhCltSzSinglePointHitPullY(NULL)
  , fhCltSzSinglePointHitPullZ(NULL)
  , fhCltSzSinglePointHitPullR(NULL)
  , fhCltSzSinglePointHitPullT(NULL)
  , fhMultiPntHitClosestDeltaX(NULL)
  , fhMultiPntHitClosestDeltaY(NULL)
  , fhMultiPntHitClosestDeltaZ(NULL)
  , fhMultiPntHitClosestDeltaR(NULL)
  , fhMultiPntHitClosestDeltaT(NULL)
  , fhMultiPntHitClosestPullX(NULL)
  , fhMultiPntHitClosestPullY(NULL)
  , fhMultiPntHitClosestPullZ(NULL)
  , fhMultiPntHitClosestPullR(NULL)
  , fhMultiPntHitClosestPullT(NULL)
  , fhMultiPntHitFurthestDeltaX(NULL)
  , fhMultiPntHitFurthestDeltaY(NULL)
  , fhMultiPntHitFurthestDeltaZ(NULL)
  , fhMultiPntHitFurthestDeltaR(NULL)
  , fhMultiPntHitFurthestDeltaT(NULL)
  , fhMultiPntHitFurthestPullX(NULL)
  , fhMultiPntHitFurthestPullY(NULL)
  , fhMultiPntHitFurthestPullZ(NULL)
  , fhMultiPntHitFurthestPullR(NULL)
  , fhMultiPntHitFurthestPullT(NULL)
  , fhMultiPntHitMeanDeltaX(NULL)
  , fhMultiPntHitMeanDeltaY(NULL)
  , fhMultiPntHitMeanDeltaZ(NULL)
  , fhMultiPntHitMeanDeltaR(NULL)
  , fhMultiPntHitMeanDeltaT(NULL)
  , fhMultiPntHitMeanPullX(NULL)
  , fhMultiPntHitMeanPullY(NULL)
  , fhMultiPntHitMeanPullZ(NULL)
  , fhMultiPntHitMeanPullR(NULL)
  , fhMultiPntHitMeanPullT(NULL)
  , fhMultiPntHitBestDeltaX(NULL)
  , fhMultiPntHitBestDeltaY(NULL)
  , fhMultiPntHitBestDeltaZ(NULL)
  , fhMultiPntHitBestDeltaR(NULL)
  , fhMultiPntHitBestDeltaT(NULL)
  , fhMultiPntHitBestPullX(NULL)
  , fhMultiPntHitBestPullY(NULL)
  , fhMultiPntHitBestPullZ(NULL)
  , fhMultiPntHitBestPullR(NULL)
  , fhMultiPntHitBestPullT(NULL)
  , fhSingleTrackHitDeltaX(NULL)
  , fhSingleTrackHitDeltaY(NULL)
  , fhSingleTrackHitDeltaZ(NULL)
  , fhSingleTrackHitDeltaR(NULL)
  , fhSingleTrackHitDeltaT(NULL)
  , fhSingleTrackHitPullX(NULL)
  , fhSingleTrackHitPullY(NULL)
  , fhSingleTrackHitPullZ(NULL)
  , fhSingleTrackHitPullR(NULL)
  , fhSingleTrackHitPullT(NULL)
  , fhSingTrkMultiPntHitDeltaX(NULL)
  , fhSingTrkMultiPntHitDeltaY(NULL)
  , fhSingTrkMultiPntHitDeltaZ(NULL)
  , fhSingTrkMultiPntHitDeltaR(NULL)
  , fhSingTrkMultiPntHitDeltaT(NULL)
  , fhSingTrkMultiPntHitPullX(NULL)
  , fhSingTrkMultiPntHitPullY(NULL)
  , fhSingTrkMultiPntHitPullZ(NULL)
  , fhSingTrkMultiPntHitPullR(NULL)
  , fhSingTrkMultiPntHitPullT(NULL)
  , fhMultiTrkHitClosestDeltaX(NULL)
  , fhMultiTrkHitClosestDeltaY(NULL)
  , fhMultiTrkHitClosestDeltaZ(NULL)
  , fhMultiTrkHitClosestDeltaR(NULL)
  , fhMultiTrkHitClosestDeltaT(NULL)
  , fhMultiTrkHitClosestPullX(NULL)
  , fhMultiTrkHitClosestPullY(NULL)
  , fhMultiTrkHitClosestPullZ(NULL)
  , fhMultiTrkHitClosestPullR(NULL)
  , fhMultiTrkHitClosestPullT(NULL)
  , fhMultiTrkHitFurthestDeltaX(NULL)
  , fhMultiTrkHitFurthestDeltaY(NULL)
  , fhMultiTrkHitFurthestDeltaZ(NULL)
  , fhMultiTrkHitFurthestDeltaR(NULL)
  , fhMultiTrkHitFurthestDeltaT(NULL)
  , fhMultiTrkHitFurthestPullX(NULL)
  , fhMultiTrkHitFurthestPullY(NULL)
  , fhMultiTrkHitFurthestPullZ(NULL)
  , fhMultiTrkHitFurthestPullR(NULL)
  , fhMultiTrkHitFurthestPullT(NULL)
  , fhMultiTrkHitMeanDeltaX(NULL)
  , fhMultiTrkHitMeanDeltaY(NULL)
  , fhMultiTrkHitMeanDeltaZ(NULL)
  , fhMultiTrkHitMeanDeltaR(NULL)
  , fhMultiTrkHitMeanDeltaT(NULL)
  , fhMultiTrkHitMeanPullX(NULL)
  , fhMultiTrkHitMeanPullY(NULL)
  , fhMultiTrkHitMeanPullZ(NULL)
  , fhMultiTrkHitMeanPullR(NULL)
  , fhMultiTrkHitMeanPullT(NULL)
  , fhMultiTrkHitBestDeltaX(NULL)
  , fhMultiTrkHitBestDeltaY(NULL)
  , fhMultiTrkHitBestDeltaZ(NULL)
  , fhMultiTrkHitBestDeltaR(NULL)
  , fhMultiTrkHitBestDeltaT(NULL)
  , fhMultiTrkHitBestPullX(NULL)
  , fhMultiTrkHitBestPullY(NULL)
  , fhMultiTrkHitBestPullZ(NULL)
  , fhMultiTrkHitBestPullR(NULL)
  , fhMultiTrkHitBestPullT(NULL)
  , fvhPtmRapGenTrk()
  , fvhPtmRapStsPnt()
  , fvhPtmRapTofPnt()
  , fvhPtmRapTofHit()
  , fvhPtmRapTofHitSinglePnt()
  , fvhPtmRapTofHitSingleTrk()
  , fvhPtmRapSecGenTrk()
  , fvhPtmRapSecStsPnt()
  , fvhPtmRapSecTofPnt()
  , fvhPtmRapSecTofHit()
  , fvhPtmRapSecTofHitSinglePnt()
  , fvhPtmRapSecTofHitSingleTrk()
  , fvhPlabGenTrk()
  , fvhPlabStsPnt()
  , fvhPlabTofPnt()
  , fvhPlabTofHit()
  , fvhPlabTofHitSinglePnt()
  , fvhPlabTofHitSingleTrk()
  , fvhPlabSecGenTrk()
  , fvhPlabSecStsPnt()
  , fvhPlabSecTofPnt()
  , fvhPlabSecTofHit()
  , fvhPlabSecTofHitSinglePnt()
  , fvhPlabSecTofHitSingleTrk()
  , fvhPtmRapGenTrkTofPnt()
  , fvhPtmRapGenTrkTofHit()
  , fvhPlabGenTrkTofPnt()
  , fvhPlabGenTrkTofHit()
  , fvhPlabStsTrkTofPnt()
  , fvhPlabStsTrkTofHit()
  , fvhPtmRapSecGenTrkTofPnt()
  , fvhPtmRapSecGenTrkTofHit()
  , fvhPlabSecGenTrkTofPnt()
  , fvhPlabSecGenTrkTofHit()
  , fvhPlabSecStsTrkTofPnt()
  , fvhPlabSecStsTrkTofHit()
  , fvulIdxTracksWithPnt()
  , fvulIdxTracksWithHit()
  , fhIntegratedHitPntEff(NULL)
  , fvulIdxPrimTracksWithPnt()
  , fvulIdxPrimTracksWithHit()
  , fhIntegratedHitPntEffPrim(NULL)
  , fvulIdxSecTracksWithPnt()
  , fvulIdxSecTracksWithHit()
  , fhIntegratedHitPntEffSec(NULL)
  , fvulIdxHiddenTracksWithHit()
  , fhIntegratedHiddenHitPntLoss(NULL)
  , fvulIdxHiddenPrimTracksWithHit()
  , fhIntegratedHiddenHitPntLossPrim(NULL)
  , fvulIdxHiddenSecTracksWithHit()
  , fhIntegratedHiddenHitPntLossSec(NULL)
  , fvulIdxTracksWithPntGaps()
  , fvulIdxTracksWithHitGaps()
  , fhIntegratedHitPntEffGaps(NULL)
  , fvulIdxPrimTracksWithPntGaps()
  , fvulIdxPrimTracksWithHitGaps()
  , fhIntegratedHitPntEffPrimGaps(NULL)
  , fvulIdxSecTracksWithPntGaps()
  , fvulIdxSecTracksWithHitGaps()
  , fhIntegratedHitPntEffSecGaps(NULL)
  , fhMcTrkStartPrimSingTrk(NULL)
  , fhMcTrkStartSecSingTrk(NULL)
  , fhMcTrkStartPrimMultiTrk(NULL)
  , fhMcTrkStartSecMultiTrk(NULL)
  , fhPointMatchWeight(NULL) {}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmTofHitFinderQa::~CbmTofHitFinderQa() {
  // Destructor
}
// ------------------------------------------------------------------
/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmTofHitFinderQa::Init() {
  if (kFALSE == RegisterInputs()) return kFATAL;

  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  Int_t iGeoVersion   = fGeoHandler->Init(isSimulation);
  LOG(info) << "CbmTofHitFinderQa::Init with GeoVersion " << iGeoVersion;

  if (k12b > iGeoVersion) {
    LOG(error) << "CbmTofHitFinderQa::Init => Only compatible with geometries "
                  "after v12b !!!";
    return kFATAL;
  }  // if( k12b > iGeoVersion )

  if (NULL != fTofId)
    LOG(info) << "CbmTofHitFinderQa::Init with GeoVersion "
              << fGeoHandler->GetGeoVersion();
  else {
    switch (iGeoVersion) {
      case k12b: fTofId = new CbmTofDetectorId_v12b(); break;
      case k14a: fTofId = new CbmTofDetectorId_v14a(); break;
      default:
        LOG(error) << "CbmTofHitFinderQa::Init => Invalid geometry!!!"
                   << iGeoVersion;
        return kFATAL;
    }  // switch(iGeoVersion)
  }    // else of if(NULL != fTofId)

  if (kFALSE == LoadGeometry()) return kFATAL;

  if (kFALSE == CreateHistos()) return kFATAL;

  return kSUCCESS;
}

void CbmTofHitFinderQa::SetParContainers() {
  LOG(info) << " CbmTofHitFinderQa => Get the digi parameters for tof";

  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
}

void CbmTofHitFinderQa::Exec(Option_t* /*option*/) {
  // Task execution

  LOG(debug) << " CbmTofHitFinderQa => New event";

  FillHistos();

  if (0 == (fEvents % 100) && 0 < fEvents) {
    cout << "-I- CbmTofHitFinderQa::Exec : "
         << "event " << fEvents << " processed." << endl;
  }
  fEvents += 1;
}

void CbmTofHitFinderQa::Finish() {
  // Normalisations
  cout << "CbmTofHitFinderQa::Finish up with " << fEvents << " analyzed events "
       << endl;

  if (kFALSE == fbNormHistGenMode)
    NormalizeMapHistos();
  else
    NormalizeNormHistos();

  WriteHistos();
  // Prevent them from being sucked in by the CbmHadronAnalysis WriteHistograms method
  DeleteHistos();
}

/************************************************************************************/
// Functions common for all clusters approximations
Bool_t CbmTofHitFinderQa::RegisterInputs() {
  FairRootManager* fManager = FairRootManager::Instance();

  fMCEventHeader = (FairMCEventHeader*) fManager->GetObject("MCEventHeader.");
  if (NULL == fMCEventHeader) {
    LOG(error) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                  "MCEventHeader object!!!";
    return kFALSE;
  }

  fTofPointsColl = (TClonesArray*) fManager->GetObject("TofPoint");
  if (NULL == fTofPointsColl) {
    LOG(error) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                  "TofPoint TClonesArray!!!";
    return kFALSE;
  }  // if( NULL == fTofPointsColl)

  fMcTracksColl = (TClonesArray*) fManager->GetObject("MCTrack");
  if (NULL == fMcTracksColl) {
    LOG(error) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                  "MCTrack TClonesArray!!!";
    return kFALSE;
  }  // if( NULL == fMcTracksColl)

  fTofDigisColl = (TClonesArray*) fManager->GetObject("TofDigi");
  if (NULL == fTofDigisColl) {
    LOG(WARNING) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                    "TofDigi TClonesArray!!!";
    LOG(WARNING) << "                                  => Assuming that the "
                    "CbmTofHitProducerNew is used!!!";
    //     return kFALSE;
  }  // if( NULL == fTofDigisColl)
  fTofDigiMatchPointsColl =
    (TClonesArray*) fManager->GetObject("TofDigiMatchPoints");
  if (NULL == fTofDigiMatchPointsColl) {
    LOG(WARNING) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                    "TofDigiMatchPoints TClonesArray!!!";
    LOG(WARNING) << "                                  => Assuming that the "
                    "CbmTofHitProducerNew is used!!!";
    //      return kFALSE;
  }  // if( NULL == fTofDigiMatchPointsColl)

  fTofHitsColl = (TClonesArray*) fManager->GetObject("TofHit");
  if (NULL == fTofHitsColl) {
    LOG(error) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                  "TofHit TClonesArray!!!";
    return kFALSE;
  }  // if( NULL == fTofHitsColl)
  fTofDigiMatchColl = (TClonesArray*) fManager->GetObject("TofDigiMatch");
  if (NULL == fTofDigiMatchColl) {
    LOG(WARNING) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                    "TofDigiMatch TClonesArray!!!";
    LOG(WARNING) << "                                  => Assuming that the "
                    "CbmTofHitProducerNew is used!!!";
    //     return kFALSE;
  }  // if( NULL == fTofDigiMatchColl)

  fTofHitMatchColl = (TClonesArray*) fManager->GetObject("TofHitMatch");
  if (NULL == fTofHitMatchColl) {
    LOG(error) << "CbmTofHitFinderQa::RegisterInputs => Could not get the "
                  "TofHitMatch TClonesArray!!!";
    return kFALSE;
  }  // if( NULL == fTofDigiMatchPointsColl)

  if (NULL == fTofDigisColl && NULL == fTofDigiMatchPointsColl
      && NULL == fTofDigiMatchColl)
    fbHitProducerSource = kTRUE;

  if ((kFALSE == fbHitProducerSource)
      && (NULL == fTofDigisColl || NULL == fTofDigiMatchPointsColl
          || NULL == fTofDigiMatchColl)) {
    LOG(error) << "CbmTofHitFinderQa::RegisterInputs => fTofDigisColl or "
                  "fTofDigiMatchPointsColl or fTofDigiMatchColl present while "
                  "the other is missing";
    LOG(error) << "                                  => Cannot be the result "
                  "of CbmTofHitProducerNew use!!!";
    return kFALSE;
  }  // if only one of fTofDigisColl and fTofDigiMatchColl is missing

  fRealTofPointsColl = (TClonesArray*) fManager->GetObject("RealisticTofPoint");
  fRealTofMatchColl  = (TClonesArray*) fManager->GetObject("TofRealPntMatch");
  if (NULL != fRealTofPointsColl && NULL != fRealTofMatchColl) {
    fbRealPointAvail = kTRUE;
    LOG(info)
      << "CbmTofHitFinderQa::RegisterInputs => Both fRealTofPointsColl & "
         "fRealTofMatchColl there, realistic mean TOF MC point used for QA";
  }  // if( NULL != fRealTofPointsColl && NULL != fRealTofMatchColl )

  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofHitFinderQa::LoadGeometry() {
  /*
     Type 0: 5 RPC/SM,  24 SM, 32 ch/RPC =>  3840 ch          , 120 RPC         ,
     Type 1: 5 RPC/SM, 142 SM, 32 ch/RPC => 22720 ch => 26560 , 710 RPC =>  830 , => 166
     Type 3: 3 RPC/SM,  50 SM, 56 ch/RPC =>  8400 ch => 34960 , 150 RPC =>  980 , => 216
     Type 4: 5 RPC/SM,   8 SM, 96 ch/RPC =>  3840 ch => 38800 ,  40 RPC => 1020 , => 224
     Type 5: 5 RPC/SM,   8 SM, 96 ch/RPC =>  3840 ch => 42640 ,  40 RPC => 1060 , => 232
     Type 6: 2 RPC/SM,  10 SM, 96 ch/RPC =>  1920 ch => 44560 ,  20 RPC => 1080 , => 242
   */

  // Count the total number of channels and
  // generate an array with the global channel index of the first channe in each RPC
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
  fvTypeSmOffs.resize(iNbSmTypes);
  fvSmRpcOffs.resize(iNbSmTypes);
  fvRpcChOffs.resize(iNbSmTypes);
  iNbSmTot  = 0;
  iNbRpcTot = 0;
  fiNbChTot = 0;
  for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
    Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);

    fvTypeSmOffs[iSmType] = iNbSmTot;
    iNbSmTot += iNbSm;

    fvSmRpcOffs[iSmType].resize(iNbSm);
    fvRpcChOffs[iSmType].resize(iNbSm);

    for (Int_t iSm = 0; iSm < iNbSm; iSm++) {
      fvSmRpcOffs[iSmType][iSm] = iNbRpcTot;
      iNbRpcTot += iNbRpc;

      fvRpcChOffs[iSmType][iSm].resize(iNbRpc);
      for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
        fvRpcChOffs[iSmType][iSm][iRpc] = fiNbChTot;
        fiNbChTot += fDigiBdfPar->GetNbChan(iSmType, iRpc);
      }  // for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
    }    // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
  }      // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )

  return kTRUE;
}
/************************************************************************************/
// ------------------------------------------------------------------
Bool_t CbmTofHitFinderQa::SetWallPosZ(Double_t dWallPosCm) {
  fdWallPosZ = dWallPosCm;
  LOG(info) << "CbmTofHitFinderQa::SetWallPosZ => Change histograms center on "
               "Z axis to "
            << dWallPosCm << " cm";
  return kTRUE;
}
Bool_t CbmTofHitFinderQa::CreateHistos() {
  // Create histogramms

  TDirectory* oldir =
    gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT
    ->cd();  // <= To prevent histos from being sucked in by the param file of the TRootManager !
             /*
   Double_t ymin=-1.;
   Double_t ymax=4.;
   Double_t ptmmax=2.5;
   Int_t    ptm_nbx=30;
   Int_t    ptm_nby=30;

   Double_t v1_nbx=20.;
   Double_t v1_nby=20.;
   Double_t yvmax=1.3;
*/
  // xy - hit densities and rates
  Int_t nbinx     = 1500;
  Int_t nbiny     = 1000;
  Int_t nbinz     = 1500;
  Double_t xrange = 750.;
  Double_t yrange = 500.;
  Double_t zmin   = fdWallPosZ - 50.;
  Double_t zmax   = fdWallPosZ + 150.;

  // angular densities for overlap check
  Int_t iNbBinThetaX  = 1200;
  Double_t dThetaXMin = -60.0;
  Double_t dThetaXMax = 60.0;
  Int_t iNbBinThetaY  = 900;
  Double_t dThetaYMin = -45.0;
  Double_t dThetaYMax = 45.0;

  Int_t iNbBinTheta  = 180;
  Double_t dThetaMin = 0;
  Double_t dThetaMax = TMath::Pi() * 90 / 180;
  Int_t iNbBinPhi    = 180;
  Double_t dPhiMin   = -TMath::Pi();
  Double_t dPhiMax   = TMath::Pi();

  // Range hit deviation from MC
  Int_t iNbBinsDeltaPos    = 600;
  Double_t dDeltaPosRange  = 30;  // cm
  Int_t iNbBinsDeltaTime   = 1000;
  Double_t dDeltaTimeRange = 1000;  // ps
  Int_t iNbBinsMulti       = 18;
  Double_t iMinMulti       = 2 - 0.5;
  Double_t iMaxMulti       = 19 + 0.5;
  // Range pulls from MC
  Int_t iNbBinsPullPos   = 500;
  Double_t dPullPosRange = 5;

  // Range Mapping
  Int_t iNbBinsStartZ  = 1250;
  Double_t dMinStartZ  = -50.0;
  Double_t dMaxStartZ  = 1200.0;
  Int_t iNbBinsStartXY = 1200;
  Double_t dMinStartXY = -600.0;
  Double_t dMaxStartXY = 600.0;
  Int_t iNbBinsCentr   = 16;
  Double_t dNbMinCentr = 0.0;
  Double_t dNbMaxCentr = 16.0;

  // Nb Hits per event for first N events
  fhNbHitsPerEvent =
    new TH1I("TofTests_NbHitsPerEvent",
             "Number of Hits per event; EventsIdx []; # [Hits]",
             fuNbEventsForHitsNbPlots,
             -0.5,
             fuNbEventsForHitsNbPlots - 0.5);
  fhNbHitsSingPntPerEvent =
    new TH1I("TofTests_NbHitsSingPntPerEvent",
             "Number of Single Point Hits per event; EventsIdx []; # [Hits]",
             fuNbEventsForHitsNbPlots,
             -0.5,
             fuNbEventsForHitsNbPlots - 0.5);
  fhNbHitsMultPntPerEvent =
    new TH1I("TofTests_NbHitsMultPntPerEvent",
             "Number of Multi Point Hits per event; EventsIdx []; # [Hits]",
             fuNbEventsForHitsNbPlots,
             -0.5,
             fuNbEventsForHitsNbPlots - 0.5);
  fhNbHitsSingTrkPerEvent =
    new TH1I("TofTests_NbHitsSingTrkPerEvent",
             "Number of Single Track Hits per event; EventsIdx []; # [Hits]",
             fuNbEventsForHitsNbPlots,
             -0.5,
             fuNbEventsForHitsNbPlots - 0.5);
  fhNbHitsMultTrkPerEvent =
    new TH1I("TofTests_NbHitsMultTrkPerEvent",
             "Number of Multi Track Hits per event; EventsIdx []; # [Hits]",
             fuNbEventsForHitsNbPlots,
             -0.5,
             fuNbEventsForHitsNbPlots - 0.5);
  // Nb different TOF digis in Hit
  fhNbDigisInHit      = new TH1I("TofTests_NbDigisInHit",
                            "Number of Digis per Hit; # [Digis]",
                            100,
                            -0.5,
                            100 - 0.5);
  fhNbDigisInHitMapXY = new TProfile2D(
    "TofTests_NbDigisInHitMapXY",
    "Number of Digis per Hit vs Position; X[cm]; Y[cm]; Mean [Digis/Hit]",
    nbinx,
    -xrange,
    xrange,
    nbiny,
    -yrange,
    yrange);

  // Mapping
  // Dependence of Track origin on centrality
  fvhTrackAllStartZCent.resize(kiNbPart);
  fvhTrackSecStartZCent.resize(kiNbPart);
  fvhTrackAllStartXZCent.resize(kiNbPart);
  fvhTrackAllStartXZ.resize(kiNbPart);
  fvhTrackAllStartYZ.resize(kiNbPart);
  fvhTofPntAllAngCent.resize(kiNbPart);
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Track origin for tracks reaching TOF
    fvhTrackAllStartZCent[iPartIdx] = new TH2D(
      Form("TofTests_TrackAllStartZCent_%s", ksPartTag[iPartIdx].Data()),
      Form("Centrality vs Start Z distribution for MC tracks w/ TOF Pnt, %s, "
           "all tracks; Start Z [cm]; B [fm]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsStartZ,
      dMinStartZ,
      dMaxStartZ,
      iNbBinsCentr,
      dNbMinCentr,
      dNbMaxCentr);
    fvhTrackSecStartZCent[iPartIdx] = new TH2D(
      Form("TofTests_TrackSecStartZCent_%s", ksPartTag[iPartIdx].Data()),
      Form("Centrality vs Start Z distribution for MC tracks w/ TOF Pnt, %s, "
           "secondary tracks; Start Z [cm]; B [fm]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsStartZ,
      dMinStartZ,
      dMaxStartZ,
      iNbBinsCentr,
      dNbMinCentr,
      dNbMaxCentr);
    if (2 == iPartIdx)  // 3D plot only for e-
      fvhTrackAllStartXZCent[iPartIdx] = new TH3D(
        Form("TofTests_TrackAllStartXZCent_%s", ksPartTag[iPartIdx].Data()),
        Form("Centrality vs Start Z distribution for MC tracks w/ TOF Pnt, %s, "
             "all tracks; Start X [cm]; Start Z [cm]; B [fm];",
             ksPartName[iPartIdx].Data()),
        iNbBinsStartXY / 2,
        dMinStartXY,
        dMaxStartXY,
        iNbBinsStartZ / 2,
        dMinStartZ,
        dMaxStartZ,
        iNbBinsCentr,
        dNbMinCentr,
        dNbMaxCentr);
    fvhTrackAllStartXZ[iPartIdx] =
      new TH2D(Form("TofTests_TrackAllStartXZ_%s", ksPartTag[iPartIdx].Data()),
               Form("Start X vs Z distribution for MC tracks w/ TOF Pnt, %s, "
                    "all tracks; Start Z [cm]; Start X [cm]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsStartZ / 2,
               dMinStartZ,
               dMaxStartZ,
               iNbBinsStartXY,
               dMinStartXY,
               dMaxStartXY);
    fvhTrackAllStartYZ[iPartIdx] =
      new TH2D(Form("TofTests_TrackAllStartYZ_%s", ksPartTag[iPartIdx].Data()),
               Form("Start Y vs Z distribution for MC tracks w/ TOF Pnt, %s, "
                    "all tracks; Start Z [cm]; Start Y [cm]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsStartZ / 2,
               dMinStartZ,
               dMaxStartZ,
               iNbBinsStartXY,
               dMinStartXY,
               dMaxStartXY);
    fvhTofPntAllAngCent[iPartIdx] =
      new TH3D(Form("TofTests_TofPntAllAngCent_%s", ksPartTag[iPartIdx].Data()),
               Form("Centrality vs Angular position of TOF Pnt, %s, all "
                    "tracks; #theta_{x}[Deg.]; #theta_{y}[Deg.]; B [fm];",
                    ksPartName[iPartIdx].Data()),
               iNbBinThetaX / 2,
               dThetaXMin,
               dThetaXMax,
               iNbBinThetaY / 2,
               dThetaYMin,
               dThetaYMax,
               iNbBinsCentr,
               dNbMinCentr,
               dNbMaxCentr);
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
     // tracks: Only when creating normalization histos
  if (kTRUE == fbNormHistGenMode) {
    fhTrackMapXY  = new TH2D("TofTests_TracksMapXY",
                            "Position of the MC Tracks assuming along Z axis; "
                            "X[cm]; Y[cm]; # [Tracks]",
                            nbinx,
                            -xrange,
                            xrange,
                            nbiny,
                            -yrange,
                            yrange);
    fhTrackMapXZ  = new TH2D("TofTests_TracksMapXZ",
                            "Position of the MC Tracks assuming along Z axis; "
                            "X[cm]; Z[cm]; # [Tracks]",
                            nbinx,
                            -xrange,
                            xrange,
                            nbinz,
                            zmin,
                            zmax);
    fhTrackMapYZ  = new TH2D("TofTests_TracksMapYZ",
                            "Position of the MC Tracks assuming along Z axis; "
                            "Y[cm]; Z[cm]; # [Tracks]",
                            nbiny,
                            -yrange,
                            yrange,
                            nbinz,
                            zmin,
                            zmax);
    fhTrackMapAng = new TH2D("TofTests_TracksMapAng",
                             "Position of the MC Tracks assuming from origin; "
                             "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                             iNbBinThetaX,
                             dThetaXMin,
                             dThetaXMax,
                             iNbBinThetaY,
                             dThetaYMin,
                             dThetaYMax);
    fhTrackMapSph = new TH2D("TofTests_TracksMapSph",
                             "Position of the MC Tracks assuming from origin; "
                             "#theta[rad.]; #phi[rad.]; # [Tracks]",
                             iNbBinTheta,
                             dThetaMin,
                             dThetaMax,
                             iNbBinPhi,
                             dPhiMin,
                             dPhiMax);
  }  // if( kTRUE == fbNormHistGenMode )
     // points
  fhPointMapXY =
    new TH2D("TofTests_PointsMapXY",
             "Position of the Tof Points; X[cm]; Y[cm]; # [Points]",
             nbinx,
             -xrange,
             xrange,
             nbiny,
             -yrange,
             yrange);
  fhPointMapXZ =
    new TH2D("TofTests_PointsMapXZ",
             "Position of the Tof Points; X[cm]; Z[cm]; # [Points]",
             nbinx,
             -xrange,
             xrange,
             nbinz,
             zmin,
             zmax);
  fhPointMapYZ =
    new TH2D("TofTests_PointsMapYZ",
             "Position of the Tof Points; Y[cm]; Z[cm]; # [Points]",
             nbiny,
             -yrange,
             yrange,
             nbinz,
             zmin,
             zmax);
  fhPointMapAng = new TH2D("TofTests_PointsMapAng",
                           "Position of the Tof Points; #theta_{x}[Deg.]; "
                           "#theta_{y}[Deg.]; # [Points]",
                           iNbBinThetaX,
                           dThetaXMin,
                           dThetaXMax,
                           iNbBinThetaY,
                           dThetaYMin,
                           dThetaYMax);
  fhPointMapSph =
    new TH2D("TofTests_PointsMapSph",
             "Position of the Tof Points; #theta[rad.]; #phi[rad.]; # [Points]",
             iNbBinTheta,
             dThetaMin,
             dThetaMax,
             iNbBinPhi,
             dPhiMin,
             dPhiMax);
  // real (mean over all gaps) points
  fhRealPointMapXY = new TH2D(
    "TofTests_RealPointsMapXY",
    "Position of the Tof Points (mean o/ gaps); X[cm]; Y[cm]; # [Points]",
    nbinx,
    -xrange,
    xrange,
    nbiny,
    -yrange,
    yrange);
  fhRealPointMapXZ = new TH2D(
    "TofTests_RealPointsMapXZ",
    "Position of the Tof Points (mean o/ gaps); X[cm]; Z[cm]; # [Points]",
    nbinx,
    -xrange,
    xrange,
    nbinz,
    zmin,
    zmax);
  fhRealPointMapYZ = new TH2D(
    "TofTests_RealPointsMapYZ",
    "Position of the Tof Points (mean o/ gaps); Y[cm]; Z[cm]; # [Points]",
    nbiny,
    -yrange,
    yrange,
    nbinz,
    zmin,
    zmax);
  fhRealPointMapAng = new TH2D("TofTests_RealPointsMapAng",
                               "Position of the Tof Points (mean o/ gaps); "
                               "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Points]",
                               iNbBinThetaX,
                               dThetaXMin,
                               dThetaXMax,
                               iNbBinThetaY,
                               dThetaYMin,
                               dThetaYMax);
  fhRealPointMapSph = new TH2D("TofTests_RealPointsMapSph",
                               "Position of the Tof Points (mean o/ gaps); "
                               "#theta[rad.]; #phi[rad.]; # [Points]",
                               iNbBinTheta,
                               dThetaMin,
                               dThetaMax,
                               iNbBinPhi,
                               dPhiMin,
                               dPhiMax);
  // Digis
  fhDigiMapXY  = new TH2D("TofTests_DigisMapXY",
                         "Position of the Tof Digis; X[cm]; Y[cm]; # [Digi]",
                         nbinx,
                         -xrange,
                         xrange,
                         nbiny,
                         -yrange,
                         yrange);
  fhDigiMapXZ  = new TH2D("TofTests_DigisMapXZ",
                         "Position of the Tof Digis; X[cm]; Z[cm]; # [Digi]",
                         nbinx,
                         -xrange,
                         xrange,
                         nbinz,
                         zmin,
                         zmax);
  fhDigiMapYZ  = new TH2D("TofTests_DigisMapYZ",
                         "Position of the Tof Digis; Y[cm]; Z[cm]; # [Digi]",
                         nbiny,
                         -yrange,
                         yrange,
                         nbinz,
                         zmin,
                         zmax);
  fhDigiMapAng = new TH2D(
    "TofTests_DigisMapAng",
    "Position of the Tof Digis; #theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Digi]",
    iNbBinThetaX,
    dThetaXMin,
    dThetaXMax,
    iNbBinThetaY,
    dThetaYMin,
    dThetaYMax);
  fhDigiMapSph =
    new TH2D("TofTests_DigisMapSph",
             "Position of the Tof Digis; #theta[rad.]; #phi[rad.]; # [Points]",
             iNbBinTheta,
             dThetaMin,
             dThetaMax,
             iNbBinPhi,
             dPhiMin,
             dPhiMax);

  // Hits
  fhHitMapXY  = new TH2D("TofTests_HitsMapXY",
                        "Position of the Tof Hits; X[cm]; Y[cm]; # [Hits]",
                        nbinx,
                        -xrange,
                        xrange,
                        nbiny,
                        -yrange,
                        yrange);
  fhHitMapXZ  = new TH2D("TofTests_HitsMapXZ",
                        "Position of the Tof Hits; X[cm]; Z[cm]; # [Hits]",
                        nbinx,
                        -xrange,
                        xrange,
                        nbinz,
                        zmin,
                        zmax);
  fhHitMapYZ  = new TH2D("TofTests_HitsMapYZ",
                        "Position of the Tof Hits; Y[cm]; Z[cm]; # [Hits]",
                        nbiny,
                        -yrange,
                        yrange,
                        nbinz,
                        zmin,
                        zmax);
  fhHitMapAng = new TH2D(
    "TofTests_HitsMapAng",
    "Position of the Tof Hits; #theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Hits]",
    iNbBinThetaX,
    dThetaXMin,
    dThetaXMax,
    iNbBinThetaY,
    dThetaYMin,
    dThetaYMax);
  fhHitMapSph =
    new TH2D("TofTests_HitsMapSph",
             "Position of the Tof Hits; #theta[rad.]; #phi[rad.]; # [Points]",
             iNbBinTheta,
             dThetaMin,
             dThetaMax,
             iNbBinPhi,
             dPhiMin,
             dPhiMax);

  // L/R digis missmatch
  if (0 == fiNbChTot && kTRUE == fbHitProducerSource)
    fiNbChTot =
      1;  // Should only happen when the HitProducer is used, set to 1 to avoid histogram warning

  fhLeftRightDigiMatch = new TH2D("TofTests_DigiMatchLR",
                                  "Matching between the left an right digi for "
                                  "strips added to hit; Global Strip Index []",
                                  fiNbChTot,
                                  0,
                                  fiNbChTot,
                                  3,
                                  -0.5,
                                  2.5);
  fhLeftRightDigiMatch->GetYaxis()->SetBinLabel(1, "Same MC Point");
  fhLeftRightDigiMatch->GetYaxis()->SetBinLabel(2, "Same MC Track");
  fhLeftRightDigiMatch->GetYaxis()->SetBinLabel(3, "Diff MC Track");

  // Nb different MC Points and Tracks in Hit
  fhNbPointsInHit =
    new TH1D("TofTests_NbPointsInHit",
             "Number of different MC Points in Hit; # [MC Points]",
             100,
             -0.5,
             99.5);
  fhNbTracksInHit =
    new TH1D("TofTests_NbTracksInHit",
             "Number of different MC Tracks in Hit; # [MC Tracks]",
             100,
             -0.5,
             99.5);

  // Mapping of position for hits coming from a single MC Point
  fhHitMapSingPntXY = new TH2D(
    "TofTests_HitsMapSingPntXY",
    "Position of the Tof Hits from a single MC Point; X[cm]; Y[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbiny,
    -yrange,
    yrange);
  fhHitMapSingPntXZ = new TH2D(
    "TofTests_HitsMapSingPntXZ",
    "Position of the Tof Hits from a single MC Point; X[cm]; Z[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapSingPntYZ = new TH2D(
    "TofTests_HitsMapSingPntYZ",
    "Position of the Tof Hits from a single MC Point; Y[cm]; Z[cm]; # [Hits]",
    nbiny,
    -yrange,
    yrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapSingPntAng =
    new TH2D("TofTests_HitsMapSingPntAng",
             "Position of the Tof Hits from a single MC Point; "
             "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Hits]",
             iNbBinThetaX,
             dThetaXMin,
             dThetaXMax,
             iNbBinThetaY,
             dThetaYMin,
             dThetaYMax);
  fhHitMapSingPntSph = new TH2D("TofTests_HitsMapSingPntSph",
                                "Position of the Tof Hits from a single MC "
                                "Point; #theta[rad.]; #phi[rad.]; # [Points]",
                                iNbBinTheta,
                                dThetaMin,
                                dThetaMax,
                                iNbBinPhi,
                                dPhiMin,
                                dPhiMax);
  // Mapping of position for hits coming from multiple MC Points
  fhHitMapMultPntXY = new TH2D(
    "TofTests_HitsMapMultPntXY",
    "Position of the Tof Hits from multiple MC Points; X[cm]; Y[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbiny,
    -yrange,
    yrange);
  fhHitMapMultPntXZ = new TH2D(
    "TofTests_HitsMapMultPntXZ",
    "Position of the Tof Hits from multiple MC Points; X[cm]; Z[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapMultPntYZ = new TH2D(
    "TofTests_HitsMapMultPntYZ",
    "Position of the Tof Hits from multiple MC Points; Y[cm]; Z[cm]; # [Hits]",
    nbiny,
    -yrange,
    yrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapMultPntAng =
    new TH2D("TofTests_HitsMapMultPntAng",
             "Position of the Tof Hits from multiple MC Points; "
             "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Hits]",
             iNbBinThetaX,
             dThetaXMin,
             dThetaXMax,
             iNbBinThetaY,
             dThetaYMin,
             dThetaYMax);
  fhHitMapMultPntSph = new TH2D("TofTests_HitsMapMultPntSph",
                                "Position of the Tof Hits from multiple MC "
                                "Points; #theta[rad.]; #phi[rad.]; # [Points]",
                                iNbBinTheta,
                                dThetaMin,
                                dThetaMax,
                                iNbBinPhi,
                                dPhiMin,
                                dPhiMax);
  // Mapping of position for hits coming from a single MC Track
  fhHitMapSingTrkXY = new TH2D(
    "TofTests_HitsMapSingTrkXY",
    "Position of the Tof Hits from a single MC Track; X[cm]; Y[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbiny,
    -yrange,
    yrange);
  fhHitMapSingTrkXZ = new TH2D(
    "TofTests_HitsMapSingTrkXZ",
    "Position of the Tof Hits from a single MC Track; X[cm]; Z[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapSingTrkYZ = new TH2D(
    "TofTests_HitsMapSingTrkYZ",
    "Position of the Tof Hits from a single MC Track; Y[cm]; Z[cm]; # [Hits]",
    nbiny,
    -yrange,
    yrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapSingTrkAng =
    new TH2D("TofTests_HitsMapSingTrkAng",
             "Position of the Tof Hits from a single MC Track; "
             "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Hits]",
             iNbBinThetaX,
             dThetaXMin,
             dThetaXMax,
             iNbBinThetaY,
             dThetaYMin,
             dThetaYMax);
  fhHitMapSingTrkSph = new TH2D("TofTests_HitsMapSingTrkSph",
                                "Position of the Tof Hits from a single MC "
                                "Track; #theta[rad.]; #phi[rad.]; # [Points]",
                                iNbBinTheta,
                                dThetaMin,
                                dThetaMax,
                                iNbBinPhi,
                                dPhiMin,
                                dPhiMax);
  // Mapping of position for hits coming from multiple MC Tracks
  fhHitMapMultTrkXY = new TH2D(
    "TofTests_HitsMapMultTrkXY",
    "Position of the Tof Hits from multiple MC Tracks; X[cm]; Y[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbiny,
    -yrange,
    yrange);
  fhHitMapMultTrkXZ = new TH2D(
    "TofTests_HitsMapMultTrkXZ",
    "Position of the Tof Hits from multiple MC Tracks; X[cm]; Z[cm]; # [Hits]",
    nbinx,
    -xrange,
    xrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapMultTrkYZ = new TH2D(
    "TofTests_HitsMapMultTrkYZ",
    "Position of the Tof Hits from multiple MC Tracks; Y[cm]; Z[cm]; # [Hits]",
    nbiny,
    -yrange,
    yrange,
    nbinz,
    zmin,
    zmax);
  fhHitMapMultTrkAng =
    new TH2D("TofTests_HitsMapMultTrkAng",
             "Position of the Tof Hits from multiple MC Tracks; "
             "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Hits]",
             iNbBinThetaX,
             dThetaXMin,
             dThetaXMax,
             iNbBinThetaY,
             dThetaYMin,
             dThetaYMax);
  fhHitMapMultTrkSph = new TH2D("TofTests_HitsMapMultTrkSph",
                                "Position of the Tof Hits from multiple MC "
                                "Tracks; #theta[rad.]; #phi[rad.]; # [Points]",
                                iNbBinTheta,
                                dThetaMin,
                                dThetaMax,
                                iNbBinPhi,
                                dPhiMin,
                                dPhiMax);


  // Hit Quality for Hits coming from a single MC Point
  fhSinglePointHitDeltaX =
    new TH1D("TofTests_SinglePointHitDeltaX",
             "Quality of the Tof Hits position on X axis, for hit coming from "
             "a single MC Point; X(Hit) - X(Point) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSinglePointHitDeltaY =
    new TH1D("TofTests_SinglePointHitDeltaY",
             "Quality of the Tof Hits position on Y axis, for hit coming from "
             "a single MC Point; Y(Hit) - Y(Point) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSinglePointHitDeltaZ =
    new TH1D("TofTests_SinglePointHitDeltaZ",
             "Quality of the Tof Hits position on Z axis, for hit coming from "
             "a single MC Point; Z(Hit) - Z(Point) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSinglePointHitDeltaR =
    new TH1D("TofTests_SinglePointHitDeltaR",
             "Quality of the Tof Hits position, for hit coming from a single "
             "MC Point; R(Hit -> Point) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSinglePointHitDeltaT =
    new TH1D("TofTests_SinglePointHitDeltaT",
             "Quality of the Tof Hits Time, for hit coming from a single MC "
             "Point; T(hit) - T(Point) [ps]; # [Hits]",
             iNbBinsDeltaTime,
             -dDeltaTimeRange,
             dDeltaTimeRange);
  fhSinglePointHitPullX =
    new TH1D("TofTests_SinglePointHitPullX",
             "Quality of the Tof Hits position error on X axis, for hit coming "
             "from a single MC Point; Pull X(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSinglePointHitPullY =
    new TH1D("TofTests_SinglePointHitPullY",
             "Quality of the Tof Hits position error on Y axis, for hit coming "
             "from a single MC Point; Pull Y(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSinglePointHitPullZ =
    new TH1D("TofTests_SinglePointHitPullZ",
             "Quality of the Tof Hits position error on Z axis, for hit coming "
             "from a single MC Point; Pull Z(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSinglePointHitPullR =
    new TH1D("TofTests_SinglePointHitPullR",
             "Quality of the Tof Hits position error, for hit coming from a "
             "single MC Point; Pull R(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSinglePointHitPullT =
    new TH1D("TofTests_SinglePointHitPullT",
             "Quality of the Tof Hits time error, for hit coming from a single "
             "MC Point; Pull T(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhCltSzSinglePointHitPullX =
    new TH2D("TofTests_CltSzSinglePointHitPullX",
             "Quality of the Tof Hits position error on X axis, for hit coming "
             "from a single MC Point; Pull X(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             32,
             1,
             33);
  fhCltSzSinglePointHitPullY =
    new TH2D("TofTests_CltSzSinglePointHitPullY",
             "Quality of the Tof Hits position error on Y axis, for hit coming "
             "from a single MC Point; Pull Y(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             32,
             1,
             33);
  fhCltSzSinglePointHitPullZ =
    new TH2D("TofTests_CltSzSinglePointHitPullZ",
             "Quality of the Tof Hits position error on Z axis, for hit coming "
             "from a single MC Point; Pull Z(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             32,
             1,
             33);
  fhCltSzSinglePointHitPullR =
    new TH2D("TofTests_CltSzSinglePointHitPullR",
             "Quality of the Tof Hits position error, for hit coming from a "
             "single MC Point; Pull R(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             32,
             1,
             33);
  fhCltSzSinglePointHitPullT =
    new TH2D("TofTests_CltSzSinglePointHitPullT",
             "Quality of the Tof Hits time error, for hit coming from a single "
             "MC Point; Pull T(Hit -> Point) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             32,
             1,
             33);

  // Hit Quality for Hits coming from a multiple MC Points
  // To Point closest to Hit
  fhMultiPntHitClosestDeltaX =
    new TH2D("TofTests_MultiPntHitClosestDeltaX",
             "Quality of the Tof Hits position on X axis relative to closest "
             "Point, for hit coming from multiple MC Point; X(Hit) - X(Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestDeltaY =
    new TH2D("TofTests_MultiPntHitClosestDeltaY",
             "Quality of the Tof Hits position on Y axis relative to closest "
             "Point, for hit coming from multiple MC Point; Y(Hit) - Y(Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestDeltaZ =
    new TH2D("TofTests_MultiPntHitClosestDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to closest "
             "Point, for hit coming from multiple MC Point; Z(Hit) - Z(Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestDeltaR =
    new TH2D("TofTests_MultiPntHitClosestDeltaR",
             "Quality of the Tof Hits position relative to closest Point, for "
             "hit coming from multiple MC Point; R(Hit -> Point) [cm]; # "
             "[Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestDeltaT = new TH2D(
    "TofTests_MultiPntHitClosestDeltaT",
    "Quality of the Tof Hits Time relative to closest Point, for hit coming "
    "from multiple MC Point; T(hit) - T(Point) [ps]; # [Hits]; Multi [Pnt]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiPntHitClosestPullX =
    new TH2D("TofTests_MultiPntHitClosestPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "closest Point, for hit coming from multiple MC Point; Pull X(Hit "
             "-> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestPullY =
    new TH2D("TofTests_MultiPntHitClosestPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "closest Point, for hit coming from multiple MC Point; Pull Y(Hit "
             "-> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestPullZ =
    new TH2D("TofTests_MultiPntHitClosestPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "closest Point, for hit coming from multiple MC Point; Pull Z(Hit "
             "-> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestPullR =
    new TH2D("TofTests_MultiPntHitClosestPullR",
             "Quality of the Tof Hits position error relative to closest "
             "Point, for hit coming from multiple MC Point; Pull R(Hit -> "
             "Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitClosestPullT =
    new TH2D("TofTests_MultiPntHitClosestPullT",
             "Quality of the Tof Hits time error relative to closest Point, "
             "for hit coming from multiple MC Point; Pull T(Hit -> Point) []; "
             "# [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  // To Point furthest from Hit
  fhMultiPntHitFurthestDeltaX =
    new TH2D("TofTests_MultiPntHitFurthestDeltaX",
             "Quality of the Tof Hits position on X axis relative to furthest "
             "Point, for hit coming from multiple MC Point; X(Hit) - X(Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestDeltaY =
    new TH2D("TofTests_MultiPntHitFurthestDeltaY",
             "Quality of the Tof Hits position on Y axis relative to furthest "
             "Point, for hit coming from multiple MC Point; Y(Hit) - Y(Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestDeltaZ =
    new TH2D("TofTests_MultiPntHitFurthestDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to furthest "
             "Point, for hit coming from multiple MC Point; Z(Hit) - Z(Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestDeltaR =
    new TH2D("TofTests_MultiPntHitFurthestDeltaR",
             "Quality of the Tof Hits position relative to furthest Point, for "
             "hit coming from multiple MC Point; R(Hit -> Point) [cm]; # "
             "[Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestDeltaT = new TH2D(
    "TofTests_MultiPntHitFurthestDeltaT",
    "Quality of the Tof Hits Time relative to furthest Point, for hit coming "
    "from multiple MC Point; T(hit) - T(Point) [ps]; # [Hits]; Multi [Pnt]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiPntHitFurthestPullX =
    new TH2D("TofTests_MultiPntHitFurthestPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "furthest Point, for hit coming from multiple MC Point; Pull "
             "X(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestPullY =
    new TH2D("TofTests_MultiPntHitFurthestPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "furthest Point, for hit coming from multiple MC Point; Pull "
             "Y(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestPullZ =
    new TH2D("TofTests_MultiPntHitFurthestPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "furthest Point, for hit coming from multiple MC Point; Pull "
             "Z(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestPullR =
    new TH2D("TofTests_MultiPntHitFurthestPullR",
             "Quality of the Tof Hits position error relative to furthest "
             "Point, for hit coming from multiple MC Point; Pull R(Hit -> "
             "Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitFurthestPullT =
    new TH2D("TofTests_MultiPntHitFurthestPullT",
             "Quality of the Tof Hits time error relative to furthest Point, "
             "for hit coming from multiple MC Point; Pull T(Hit -> Point) []; "
             "# [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  // To mean Point position
  fhMultiPntHitMeanDeltaX =
    new TH2D("TofTests_MultiPntHitMeanDeltaX",
             "Quality of the Tof Hits position on X axis relative to mean "
             "Point position, for hit coming from multiple MC Point; X(Hit) - "
             "X(Point) [cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanDeltaY =
    new TH2D("TofTests_MultiPntHitMeanDeltaY",
             "Quality of the Tof Hits position on Y axis relative to mean "
             "Point position, for hit coming from multiple MC Point; Y(Hit) - "
             "Y(Point) [cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanDeltaZ =
    new TH2D("TofTests_MultiPntHitMeanDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to mean "
             "Point position, for hit coming from multiple MC Point; Z(Hit) - "
             "Z(Point) [cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanDeltaR =
    new TH2D("TofTests_MultiPntHitMeanDeltaR",
             "Quality of the Tof Hits position relative to mean Point "
             "position, for hit coming from multiple MC Point; R(Hit -> Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanDeltaT = new TH2D(
    "TofTests_MultiPntHitMeanDeltaT",
    "Quality of the Tof Hits Time relative to mean Point time, for hit coming "
    "from multiple MC Point; T(hit) - T(Point) [ps]; # [Hits]; Multi [Pnt]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiPntHitMeanPullX =
    new TH2D("TofTests_MultiPntHitMeanPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "mean Point position, for hit coming from multiple MC Point; Pull "
             "X(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanPullY =
    new TH2D("TofTests_MultiPntHitMeanPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "mean Point position, for hit coming from multiple MC Point; Pull "
             "Y(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanPullZ =
    new TH2D("TofTests_MultiPntHitMeanPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "mean Point position, for hit coming from multiple MC Point; Pull "
             "Z(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanPullR =
    new TH2D("TofTests_MultiPntHitMeanPullR",
             "Quality of the Tof Hits position error relative to mean Point "
             "position, for hit coming from multiple MC Point; Pull R(Hit -> "
             "Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitMeanPullT =
    new TH2D("TofTests_MultiPntHitMeanPullT",
             "Quality of the Tof Hits time error relative to mean Point time, "
             "for hit coming from multiple MC Point; Pull T(Hit -> Point) []; "
             "# [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  // To best Point (highest TOT contribution
  fhMultiPntHitBestDeltaX =
    new TH2D("TofTests_MultiPntHitBestDeltaX",
             "Quality of the Tof Hits position on X axis relative to best "
             "Point position, for hit coming from multiple MC Point; X(Hit) - "
             "X(Point) [cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestDeltaY =
    new TH2D("TofTests_MultiPntHitBestDeltaY",
             "Quality of the Tof Hits position on Y axis relative to best "
             "Point position, for hit coming from multiple MC Point; Y(Hit) - "
             "Y(Point) [cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestDeltaZ =
    new TH2D("TofTests_MultiPntHitBestDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to best "
             "Point position, for hit coming from multiple MC Point; Z(Hit) - "
             "Z(Point) [cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestDeltaR =
    new TH2D("TofTests_MultiPntHitBestDeltaR",
             "Quality of the Tof Hits position relative to best Point "
             "position, for hit coming from multiple MC Point; R(Hit -> Point) "
             "[cm]; # [Hits]; Multi [Pnt]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestDeltaT = new TH2D(
    "TofTests_MultiPntHitBestDeltaT",
    "Quality of the Tof Hits Time relative to best Point time, for hit coming "
    "from multiple MC Point; T(hit) - T(Point) [ps]; # [Hits]; Multi [Pnt]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiPntHitBestPullX =
    new TH2D("TofTests_MultiPntHitBestPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "best Point position, for hit coming from multiple MC Point; Pull "
             "X(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestPullY =
    new TH2D("TofTests_MultiPntHitBestPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "best Point position, for hit coming from multiple MC Point; Pull "
             "Y(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestPullZ =
    new TH2D("TofTests_MultiPntHitBestPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "best Point position, for hit coming from multiple MC Point; Pull "
             "Z(Hit -> Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestPullR =
    new TH2D("TofTests_MultiPntHitBestPullR",
             "Quality of the Tof Hits position error relative to best Point "
             "position, for hit coming from multiple MC Point; Pull R(Hit -> "
             "Point) []; # [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiPntHitBestPullT =
    new TH2D("TofTests_MultiPntHitBestPullT",
             "Quality of the Tof Hits time error relative to best Point time, "
             "for hit coming from multiple MC Point; Pull T(Hit -> Point) []; "
             "# [Hits]; Multi [Pnt]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);

  // Hit Quality for Hits coming from a single MC Track
  fhSingleTrackHitDeltaX =
    new TH1D("TofTests_SingleTrackHitDeltaX",
             "Quality of the Tof Hits position on X axis, for hit coming from "
             "a single MC Track; X(Hit) - X(Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingleTrackHitDeltaY =
    new TH1D("TofTests_SingleTrackHitDeltaY",
             "Quality of the Tof Hits position on Y axis, for hit coming from "
             "a single MC Track; Y(Hit) - X(Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingleTrackHitDeltaZ =
    new TH1D("TofTests_SingleTrackHitDeltaZ",
             "Quality of the Tof Hits position on Z axis, for hit coming from "
             "a single MC Track; Z(Hit) - X(Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingleTrackHitDeltaR =
    new TH1D("TofTests_SingleTrackHitDeltaR",
             "Quality of the Tof Hits position, for hit coming from a single "
             "MC Track; R(Hit -> Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingleTrackHitDeltaT =
    new TH1D("TofTests_SingleTrackHitDeltaT",
             "Quality of the Tof Hits Time, for hit coming from a single MC "
             "Track; T(hit) - T(Track) [ps]; # [Hits]",
             iNbBinsDeltaTime,
             -dDeltaTimeRange,
             dDeltaTimeRange);
  fhSingleTrackHitPullX =
    new TH1D("TofTests_SingleTrackHitPullX",
             "Quality of the Tof Hits position error on X axis, for hit coming "
             "from a single MC Track; Pull X(Hit -> Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingleTrackHitPullY =
    new TH1D("TofTests_SingleTrackHitPullY",
             "Quality of the Tof Hits position error on Y axis, for hit coming "
             "from a single MC Track; Y(Hit) - X(Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingleTrackHitPullZ =
    new TH1D("TofTests_SingleTrackHitPullZ",
             "Quality of the Tof Hits position error on Z axis, for hit coming "
             "from a single MC Track; Z(Hit) - X(Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingleTrackHitPullR =
    new TH1D("TofTests_SingleTrackHitPullR",
             "Quality of the Tof Hits position error, for hit coming from a "
             "single MC Track; Pull R(Hit -> Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingleTrackHitPullT =
    new TH1D("TofTests_SingleTrackHitPullT",
             "Quality of the Tof Hits time error, for hit coming from a single "
             "MC Track; Pull T(Hit -> Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);

  // Hit Quality for Hits coming from a single MC Track but multiple points
  fhSingTrkMultiPntHitDeltaX =
    new TH1D("TofTests_SingTrkMultiPntHitDeltaX",
             "Quality of the Tof Hits position on X axis, for hit coming from "
             "a single MC Track; X(Hit) - X(Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingTrkMultiPntHitDeltaY =
    new TH1D("TofTests_SingTrkMultiPntHitDeltaY",
             "Quality of the Tof Hits position on Y axis, for hit coming from "
             "a single MC Track; Y(Hit) - X(Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingTrkMultiPntHitDeltaZ =
    new TH1D("TofTests_SingTrkMultiPntHitDeltaZ",
             "Quality of the Tof Hits position on Z axis, for hit coming from "
             "a single MC Track; Z(Hit) - X(Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingTrkMultiPntHitDeltaR =
    new TH1D("TofTests_SingTrkMultiPntHitDeltaR",
             "Quality of the Tof Hits position, for hit coming from a single "
             "MC Track; R(Hit -> Track) [cm]; # [Hits]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange);
  fhSingTrkMultiPntHitDeltaT =
    new TH1D("TofTests_SingTrkMultiPntHitDeltaT",
             "Quality of the Tof Hits Time, for hit coming from a single MC "
             "Track; T(hit) - T(Track) [ps]; # [Hits]",
             iNbBinsDeltaTime,
             -dDeltaTimeRange,
             dDeltaTimeRange);
  fhSingTrkMultiPntHitPullX =
    new TH1D("TofTests_SingTrkMultiPntHitPullX",
             "Quality of the Tof Hits position error on X axis, for hit coming "
             "from a single MC Track; Pull X(Hit -> Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingTrkMultiPntHitPullY =
    new TH1D("TofTests_SingTrkMultiPntHitPullY",
             "Quality of the Tof Hits position error on Y axis, for hit coming "
             "from a single MC Track; Y(Hit) - X(Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingTrkMultiPntHitPullZ =
    new TH1D("TofTests_SingTrkMultiPntHitPullZ",
             "Quality of the Tof Hits position error on Z axis, for hit coming "
             "from a single MC Track; Z(Hit) - X(Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingTrkMultiPntHitPullR =
    new TH1D("TofTests_SingTrkMultiPntHitPullR",
             "Quality of the Tof Hits position error, for hit coming from a "
             "single MC Track; Pull R(Hit -> Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);
  fhSingTrkMultiPntHitPullT =
    new TH1D("TofTests_SingTrkMultiPntHitPullT",
             "Quality of the Tof Hits time error, for hit coming from a single "
             "MC Track; Pull T(Hit -> Track) []; # [Hits]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange);

  // Hit Quality for Hits coming from a multiple MC Tracks
  // To Point closest to Hit
  fhMultiTrkHitClosestDeltaX =
    new TH2D("TofTests_MultiTrkHitClosestDeltaX",
             "Quality of the Tof Hits position on X axis relative to closest "
             "Track, for hit coming from multiple MC Track; X(Hit) - X(Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestDeltaY =
    new TH2D("TofTests_MultiTrkHitClosestDeltaY",
             "Quality of the Tof Hits position on Y axis relative to closest "
             "Track, for hit coming from multiple MC Track; Y(Hit) - Y(Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestDeltaZ =
    new TH2D("TofTests_MultiTrkHitClosestDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to closest "
             "Track, for hit coming from multiple MC Track; Z(Hit) - Z(Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestDeltaR =
    new TH2D("TofTests_MultiTrkHitClosestDeltaR",
             "Quality of the Tof Hits position relative to closest Track, for "
             "hit coming from multiple MC Track; R(Hit -> Track) [cm]; # "
             "[Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestDeltaT = new TH2D(
    "TofTests_MultiTrkHitClosestDeltaT",
    "Quality of the Tof Hits Time relative to closest Track, for hit coming "
    "from multiple MC Track; T(hit) - T(Track) [ps]; # [Hits]; Multi [Trk]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiTrkHitClosestPullX =
    new TH2D("TofTests_MultiTrkHitClosestPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "closest Track, for hit coming from multiple MC Track; Pull X(Hit "
             "-> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestPullY =
    new TH2D("TofTests_MultiTrkHitClosestPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "closest Track, for hit coming from multiple MC Track; Pull Y(Hit "
             "-> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestPullZ =
    new TH2D("TofTests_MultiTrkHitClosestPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "closest Track, for hit coming from multiple MC Track; Pull Z(Hit "
             "-> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestPullR =
    new TH2D("TofTests_MultiTrkHitClosestPullR",
             "Quality of the Tof Hits position error relative to closest "
             "Track, for hit coming from multiple MC Track; Pull R(Hit -> "
             "Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitClosestPullT =
    new TH2D("TofTests_MultiTrkHitClosestPullT",
             "Quality of the Tof Hits time error relative to closest Track, "
             "for hit coming from multiple MC Track; Pull T(Hit -> Track) []; "
             "# [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  // To Track furthest from Hit
  fhMultiTrkHitFurthestDeltaX =
    new TH2D("TofTests_MultiTrkHitFurthestDeltaX",
             "Quality of the Tof Hits position on X axis relative to furthest "
             "Track, for hit coming from multiple MC Track; X(Hit) - X(Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestDeltaY =
    new TH2D("TofTests_MultiTrkHitFurthestDeltaY",
             "Quality of the Tof Hits position on Y axis relative to furthest "
             "Track, for hit coming from multiple MC Track; Y(Hit) - Y(Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestDeltaZ =
    new TH2D("TofTests_MultiTrkHitFurthestDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to furthest "
             "Track, for hit coming from multiple MC Track; Z(Hit) - Z(Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestDeltaR =
    new TH2D("TofTests_MultiTrkHitFurthestDeltaR",
             "Quality of the Tof Hits position relative to furthest Track, for "
             "hit coming from multiple MC Track; R(Hit -> Track) [cm]; # "
             "[Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestDeltaT = new TH2D(
    "TofTests_MultiTrkHitFurthestDeltaT",
    "Quality of the Tof Hits Time relative to furthest Track, for hit coming "
    "from multiple MC Track; T(hit) - T(Track) [ps]; # [Hits]; Multi [Trk]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiTrkHitFurthestPullX =
    new TH2D("TofTests_MultiTrkHitFurthestPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "furthest Track, for hit coming from multiple MC Track; Pull "
             "X(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestPullY =
    new TH2D("TofTests_MultiTrkHitFurthestPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "furthest Track, for hit coming from multiple MC Track; Pull "
             "Y(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestPullZ =
    new TH2D("TofTests_MultiTrkHitFurthestPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "furthest Track, for hit coming from multiple MC Track; Pull "
             "Z(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestPullR =
    new TH2D("TofTests_MultiTrkHitFurthestPullR",
             "Quality of the Tof Hits position error relative to furthest "
             "Track, for hit coming from multiple MC Track; Pull R(Hit -> "
             "Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitFurthestPullT =
    new TH2D("TofTests_MultiTrkHitFurthestPullT",
             "Quality of the Tof Hits time error relative to furthest Track, "
             "for hit coming from multiple MC Track; Pull T(Hit -> Track) []; "
             "# [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  // To mean Track position
  fhMultiTrkHitMeanDeltaX =
    new TH2D("TofTests_MultiTrkHitMeanDeltaX",
             "Quality of the Tof Hits position on X axis relative to mean "
             "Track position, for hit coming from multiple MC Track; X(Hit) - "
             "X(Track) [cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanDeltaY =
    new TH2D("TofTests_MultiTrkHitMeanDeltaY",
             "Quality of the Tof Hits position on Y axis relative to mean "
             "Track position, for hit coming from multiple MC Track; Y(Hit) - "
             "Y(Track) [cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanDeltaZ =
    new TH2D("TofTests_MultiTrkHitMeanDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to mean "
             "Track position, for hit coming from multiple MC Track; Z(Hit) - "
             "Z(Track) [cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanDeltaR =
    new TH2D("TofTests_MultiTrkHitMeanDeltaR",
             "Quality of the Tof Hits position relative to mean Track "
             "position, for hit coming from multiple MC Track; R(Hit -> Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanDeltaT = new TH2D(
    "TofTests_MultiTrkHitMeanDeltaT",
    "Quality of the Tof Hits Time relative to mean Track time, for hit coming "
    "from multiple MC Track; T(hit) - T(Track) [ps]; # [Hits]; Multi [Trk]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiTrkHitMeanPullX =
    new TH2D("TofTests_MultiTrkHitMeanPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "mean Track position, for hit coming from multiple MC Track; Pull "
             "X(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanPullY =
    new TH2D("TofTests_MultiTrkHitMeanPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "mean Track position, for hit coming from multiple MC Track; Pull "
             "Y(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanPullZ =
    new TH2D("TofTests_MultiTrkHitMeanPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "mean Track position, for hit coming from multiple MC Track; Pull "
             "Z(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanPullR =
    new TH2D("TofTests_MultiTrkHitMeanPullR",
             "Quality of the Tof Hits position error relative to mean Track "
             "position, for hit coming from multiple MC Track; Pull R(Hit -> "
             "Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitMeanPullT =
    new TH2D("TofTests_MultiTrkHitMeanPullT",
             "Quality of the Tof Hits time error relative to mean Track "
             "position, for hit coming from multiple MC Track; Pull T(Hit -> "
             "Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  // To best Track (Highest TOT contribution)
  fhMultiTrkHitBestDeltaX =
    new TH2D("TofTests_MultiTrkHitBestDeltaX",
             "Quality of the Tof Hits position on X axis relative to best "
             "Track position, for hit coming from multiple MC Track; X(Hit) - "
             "X(Track) [cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestDeltaY =
    new TH2D("TofTests_MultiTrkHitBestDeltaY",
             "Quality of the Tof Hits position on Y axis relative to best "
             "Track position, for hit coming from multiple MC Track; Y(Hit) - "
             "Y(Track) [cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestDeltaZ =
    new TH2D("TofTests_MultiTrkHitBestDeltaZ",
             "Quality of the Tof Hits position on Z axis relative to best "
             "Track position, for hit coming from multiple MC Track; Z(Hit) - "
             "Z(Track) [cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestDeltaR =
    new TH2D("TofTests_MultiTrkHitBestDeltaR",
             "Quality of the Tof Hits position relative to best Track "
             "position, for hit coming from multiple MC Track; R(Hit -> Track) "
             "[cm]; # [Hits]; Multi [Trk]",
             iNbBinsDeltaPos,
             -dDeltaPosRange,
             dDeltaPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestDeltaT = new TH2D(
    "TofTests_MultiTrkHitBestDeltaT",
    "Quality of the Tof Hits Time relative to best Track time, for hit coming "
    "from multiple MC Track; T(hit) - T(Track) [ps]; # [Hits]; Multi [Trk]",
    iNbBinsDeltaTime,
    -dDeltaTimeRange,
    dDeltaTimeRange,
    iNbBinsMulti,
    iMinMulti,
    iMaxMulti);
  fhMultiTrkHitBestPullX =
    new TH2D("TofTests_MultiTrkHitBestPullX",
             "Quality of the Tof Hits position error on X axis relative to "
             "best Track position, for hit coming from multiple MC Track; Pull "
             "X(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestPullY =
    new TH2D("TofTests_MultiTrkHitBestPullY",
             "Quality of the Tof Hits position error on Y axis relative to "
             "best Track position, for hit coming from multiple MC Track; Pull "
             "Y(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestPullZ =
    new TH2D("TofTests_MultiTrkHitBestPullZ",
             "Quality of the Tof Hits position error on Z axis relative to "
             "best Track position, for hit coming from multiple MC Track; Pull "
             "Z(Hit -> Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestPullR =
    new TH2D("TofTests_MultiTrkHitBestPullR",
             "Quality of the Tof Hits position error relative to best Track "
             "position, for hit coming from multiple MC Track; Pull R(Hit -> "
             "Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);
  fhMultiTrkHitBestPullT =
    new TH2D("TofTests_MultiTrkHitBestPullT",
             "Quality of the Tof Hits time error relative to best Track "
             "position, for hit coming from multiple MC Track; Pull T(Hit -> "
             "Track) []; # [Hits]; Multi [Trk]",
             iNbBinsPullPos,
             -dPullPosRange,
             dPullPosRange,
             iNbBinsMulti,
             iMinMulti,
             iMaxMulti);

  // Physics coord mapping, 1 per particle type
  // Phase space
  Int_t iNbBinsY    = 30;
  Double_t dMinY    = -1.;
  Double_t dMaxY    = 4.;
  Int_t iNbBNinsPtm = 30;
  Double_t dMinPtm  = 0.0;
  Double_t dMaxPtm  = 2.5;
  fvhPtmRapGenTrk.resize(kiNbPart);
  fvhPtmRapStsPnt.resize(kiNbPart);
  fvhPtmRapTofPnt.resize(kiNbPart);
  fvhPtmRapTofHit.resize(kiNbPart);
  fvhPtmRapTofHitSinglePnt.resize(kiNbPart);
  fvhPtmRapTofHitSingleTrk.resize(kiNbPart);
  fvhPtmRapSecGenTrk.resize(kiNbPart);
  fvhPtmRapSecStsPnt.resize(kiNbPart);
  fvhPtmRapSecTofPnt.resize(kiNbPart);
  fvhPtmRapSecTofHit.resize(kiNbPart);
  fvhPtmRapSecTofHitSinglePnt.resize(kiNbPart);
  fvhPtmRapSecTofHitSingleTrk.resize(kiNbPart);
  // PLab
  Int_t iNbBinsPlab = 100;
  Double_t dMinPlab = 0.0;
  Double_t dMaxPlab = 10.0;
  fvhPlabGenTrk.resize(kiNbPart);
  fvhPlabStsPnt.resize(kiNbPart);
  fvhPlabTofPnt.resize(kiNbPart);
  fvhPlabTofHit.resize(kiNbPart);
  fvhPlabTofHitSinglePnt.resize(kiNbPart);
  fvhPlabTofHitSingleTrk.resize(kiNbPart);
  fvhPlabSecGenTrk.resize(kiNbPart);
  fvhPlabSecStsPnt.resize(kiNbPart);
  fvhPlabSecTofPnt.resize(kiNbPart);
  fvhPlabSecTofHit.resize(kiNbPart);
  fvhPlabSecTofHitSinglePnt.resize(kiNbPart);
  fvhPlabSecTofHitSingleTrk.resize(kiNbPart);
  // MC Tracks losses
  fvhPtmRapGenTrkTofPnt.resize(kiNbPart);
  fvhPtmRapGenTrkTofHit.resize(kiNbPart);
  fvhPlabGenTrkTofPnt.resize(kiNbPart);
  fvhPlabGenTrkTofHit.resize(kiNbPart);
  fvhPlabStsTrkTofPnt.resize(kiNbPart);
  fvhPlabStsTrkTofHit.resize(kiNbPart);
  fvhPtmRapSecGenTrkTofPnt.resize(kiNbPart);
  fvhPtmRapSecGenTrkTofHit.resize(kiNbPart);
  fvhPlabSecGenTrkTofPnt.resize(kiNbPart);
  fvhPlabSecGenTrkTofHit.resize(kiNbPart);
  fvhPlabSecStsTrkTofPnt.resize(kiNbPart);
  fvhPlabSecStsTrkTofHit.resize(kiNbPart);
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Phase space
    fvhPtmRapGenTrk[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapGenTrk_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution for MC tracks, %s, primary "
                    "tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapStsPnt[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapStsPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution from MC Track with STS points, "
                    "%s, primary tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapTofPnt[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapTofPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution from MC Track for TOF points, "
                    "%s, primary tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapTofHit[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapTofHit_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution from MC Track for TOF Hits, %s, "
                    "primary tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapTofHitSinglePnt[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapTofHitSinglePnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution from MC Track for TOF Hits from a single "
           "TOF Point, %s, primary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);
    fvhPtmRapTofHitSingleTrk[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapTofHitSingleTrk_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution from MC Track for TOF Hits from a single "
           "MC track, %s, primary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);
    // PLab
    fvhPlabGenTrk[iPartIdx] =
      new TH1D(Form("TofTests_PlabGenTrk_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution for MC tracks, %s, primary tracks; "
                    "P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabStsPnt[iPartIdx] =
      new TH1D(Form("TofTests_PlabStsPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track with STS points, %s, "
                    "primary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabTofPnt[iPartIdx] =
      new TH1D(Form("TofTests_PlabTofPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track for TOF points, %s, "
                    "primary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabTofHit[iPartIdx] =
      new TH1D(Form("TofTests_PlabTofHit_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track for TOF Hits, %s, "
                    "primary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabTofHitSinglePnt[iPartIdx] = new TH1D(
      Form("TofTests_PlabTofHitSinglePnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution from MC Track for TOF Hits from a single TOF "
           "Point, %s, primary tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);
    fvhPlabTofHitSingleTrk[iPartIdx] = new TH1D(
      Form("TofTests_PlabTofHitSingleTrk_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution from MC Track for TOF Hits from a single MC "
           "tracks, %s, primary tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);


    // MC Tracks losses
    fvhPtmRapGenTrkTofPnt[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution for MC tracks with TOF Point(s), %s, "
           "primary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);
    fvhPtmRapGenTrkTofHit[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapGenTrkTofHit_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution for MC tracks with TOF Hit(s), %s, "
           "primary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);

    fvhPlabGenTrkTofPnt[iPartIdx] =
      new TH1D(Form("TofTests_PlabGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution for MC tracks with TOF Point(s), %s, "
                    "primary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabGenTrkTofHit[iPartIdx] =
      new TH1D(Form("TofTests_PlabGenTrkTofHit_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track with TOF Hit(s), %s, "
                    "primary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabStsTrkTofPnt[iPartIdx] =
      new TH1D(Form("TofTests_PlabStsTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution for MC tracks with STS and TOF "
                    "Point(s), %s, primary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabStsTrkTofHit[iPartIdx] =
      new TH1D(Form("TofTests_PlabStsTrkTofHit_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track with STS points and "
                    "TOF Hit(s), %s, primary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);

    // Secondary tracks
    // Phase space
    fvhPtmRapSecGenTrk[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapSecGenTrk_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution for MC tracks, %s, secondary "
                    "tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapSecStsPnt[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapSecStsPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution from MC Track with STS points, "
                    "%s, secondary tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapSecTofPnt[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapSecTofPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution from MC Track for TOF points, "
                    "%s, secondary tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapSecTofHit[iPartIdx] =
      new TH2D(Form("TofTests_PtmRapSecTofHit_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{t}/M vs y distribution from MC Track for TOF Hits, %s, "
                    "secondary tracks; y; P_{t}/M; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsY,
               dMinY,
               dMaxY,
               iNbBNinsPtm,
               dMinPtm,
               dMaxPtm);
    fvhPtmRapSecTofHitSinglePnt[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapSecTofHitSinglePnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution from MC Track for TOF Hits from a single "
           "TOF Point, %s, secondary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);
    fvhPtmRapSecTofHitSingleTrk[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapSecTofHitSingleTrk_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution from MC Track for TOF Hits from a single "
           "MC track, %s, secondary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);
    // PLab
    fvhPlabSecGenTrk[iPartIdx] =
      new TH1D(Form("TofTests_PlabSecGenTrk_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution for MC tracks, %s, secondary tracks; "
                    "P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabSecStsPnt[iPartIdx] =
      new TH1D(Form("TofTests_PlabSecStsPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track with STS points, %s, "
                    "secondary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabSecTofPnt[iPartIdx] =
      new TH1D(Form("TofTests_PlabSecTofPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track for TOF points, %s, "
                    "secondary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabSecTofHit[iPartIdx] =
      new TH1D(Form("TofTests_PlabSecTofHit_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution from MC Track for TOF Hits, %s, "
                    "secondary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab,
               dMinPlab,
               dMaxPlab);
    fvhPlabSecTofHitSinglePnt[iPartIdx] = new TH1D(
      Form("TofTests_PlabSecTofHitSinglePnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution from MC Track for TOF Hits from a single TOF "
           "Point, %s, secondary tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);
    fvhPlabSecTofHitSingleTrk[iPartIdx] = new TH1D(
      Form("TofTests_PlabSecTofHitSingleTrk_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution from MC Track for TOF Hits from a single MC "
           "tracks, %s, secondary tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);


    // MC Tracks losses
    fvhPtmRapSecGenTrkTofPnt[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapSecGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution for MC tracks with TOF Point(s), %s, "
           "secondary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);
    fvhPtmRapSecGenTrkTofHit[iPartIdx] = new TH2D(
      Form("TofTests_PtmRapSecGenTrkTofHit_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{t}/M vs y distribution for MC tracks with TOF Hit(s), %s, "
           "secondary tracks; y; P_{t}/M; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsY,
      dMinY,
      dMaxY,
      iNbBNinsPtm,
      dMinPtm,
      dMaxPtm);

    fvhPlabSecGenTrkTofPnt[iPartIdx] = new TH1D(
      Form("TofTests_PlabSecGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution for MC tracks with TOF Point(s), %s, "
           "secondary tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);
    fvhPlabSecGenTrkTofHit[iPartIdx] = new TH1D(
      Form("TofTests_PlabSecGenTrkTofHit_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution from MC Track with TOF Hit(s), %s, secondary "
           "tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);

    fvhPlabSecStsTrkTofPnt[iPartIdx] = new TH1D(
      Form("TofTests_PlabSecStsTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution for MC tracks with STS and TOF Point(s), %s, "
           "secondary tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);
    fvhPlabSecStsTrkTofHit[iPartIdx] = new TH1D(
      Form("TofTests_PlabSecStsTrkTofHit_%s", ksPartTag[iPartIdx].Data()),
      Form("P_{lab} distribution from MC Track with STS points and TOF Hit(s), "
           "%s, secondary tracks; P_{lab} [GeV/c]; # []",
           ksPartName[iPartIdx].Data()),
      iNbBinsPlab,
      dMinPlab,
      dMaxPlab);
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

  // Integrated TofHit Efficiency
  fhIntegratedHitPntEff =
    new TH1D("TofTests_IntegratedHitPntEff",
             "Efficiency of TOF hit generation, all tracks; Eff = Nb_{Trk w/ "
             "Hit}/Nb_{Trk w/ Pnt} [\%]; # [Events]",
             202,
             -0.5,
             100.5);
  fhIntegratedHitPntEffPrim =
    new TH1D("TofTests_IntegratedHitPntEffPrim",
             "Efficiency of TOF hit generation, primary tracks; Eff = Nb_{Trk "
             "w/ Hit}/Nb_{Trk w/ Pnt} [\%]; # [Events]",
             202,
             -0.5,
             100.5);
  fhIntegratedHitPntEffSec =
    new TH1D("TofTests_IntegratedHitPntEffSec",
             "Efficiency of TOF hit generation, secondary tracks; Eff = "
             "Nb_{Trk w/ Hit}/Nb_{Trk w/ Pnt} [\%]; # [Events]",
             202,
             -0.5,
             100.5);


  // Integrated TofHit Efficiency: Tracks firing channel but not going to Digi/Hit
  fhIntegratedHiddenHitPntLoss =
    new TH1D("TofTests_IntegratedHiddenHitPntLoss",
             "Losses of TOF hit generation due to multiplicity, all tracks; "
             "Loss = Nb_{Trk w/ Hidden Hit}/Nb_{Trk w/ Pnt} [\%]; # [Events]",
             202,
             -0.5,
             100.5);
  fhIntegratedHiddenHitPntLossPrim = new TH1D(
    "TofTests_IntegratedHiddenHitPntLossPrim",
    "Efficiency of TOF hit generation due to multiplicity, primary tracks; "
    "Loss = Nb_{Trk w/ Hidden Hit}/Nb_{Trk w/ Pnt} [\%]; # [Events]",
    202,
    -0.5,
    100.5);
  fhIntegratedHiddenHitPntLossSec = new TH1D(
    "TofTests_IntegratedHiddenHitPntLossSec",
    "Efficiency of TOF hit generation due to multiplicity, secondary tracks; "
    "Loss = Nb_{Trk w/ Hidden Hit}/Nb_{Trk w/ Pnt} [\%]; # [Events]",
    202,
    -0.5,
    100.5);

  // Efficiency dependence on nb crossed gaps
  fvulIdxTracksWithPntGaps.resize(fuMaxCrossedGaps);
  fvulIdxTracksWithHitGaps.resize(fuMaxCrossedGaps);
  fvulIdxPrimTracksWithPntGaps.resize(fuMaxCrossedGaps);
  fvulIdxPrimTracksWithHitGaps.resize(fuMaxCrossedGaps);
  fvulIdxSecTracksWithPntGaps.resize(fuMaxCrossedGaps);
  fvulIdxSecTracksWithHitGaps.resize(fuMaxCrossedGaps);
  fhIntegratedHitPntEffGaps = new TH2D(
    "TofTests_IntegratedHitPntEffGaps",
    "Efficiency of TOF hit generation VS gaps crossed, all tracks; Eff = "
    "Nb_{Trk w/ Hit}/Nb_{Trk w/ Pnt} [\%]; Nb TofPoint (gaps) []; # [Events]",
    202,
    -0.5,
    100.5,
    fuMaxCrossedGaps,
    0.5,
    fuMaxCrossedGaps + 0.5);
  fhIntegratedHitPntEffPrimGaps = new TH2D(
    "TofTests_IntegratedHitPntEffPrimGaps",
    "Efficiency of TOF hit generation VS gaps crossed, primary tracks; Eff = "
    "Nb_{Trk w/ Hit}/Nb_{Trk w/ Pnt} [\%]; Nb TofPoint (gaps) []; # [Events]",
    202,
    -0.5,
    100.5,
    fuMaxCrossedGaps,
    0.5,
    fuMaxCrossedGaps + 0.5);
  fhIntegratedHitPntEffSecGaps = new TH2D(
    "TofTests_IntegratedHitPntEffSecGaps",
    "Efficiency of TOF hit generation VS gaps crossed, secondary tracks; Eff = "
    "Nb_{Trk w/ Hit}/Nb_{Trk w/ Pnt} [\%]; Nb TofPoint (gaps) []; # [Events]",
    202,
    -0.5,
    100.5,
    fuMaxCrossedGaps,
    0.5,
    fuMaxCrossedGaps + 0.5);

  Float_t fMaxCbmLen = 1500;
  fhMcTrkStartPrimSingTrk =
    new TH2D("TofTests_McTrkStartPrimSingTrk",
             "Origin in Z of the MC primary tracks in hits with a single "
             "track; MC trk Start Z [cm]; ; # [Events]",
             fMaxCbmLen + 1,
             -1,
             fMaxCbmLen,
             kiNbPart,
             -0.5,
             kiNbPart - 0.5);
  fhMcTrkStartSecSingTrk =
    new TH2D("TofTests_McTrkStartSecSingTrk",
             "Origin in Z of the MC secondary tracks in hits with a single "
             "track; MC trk Start Z [cm]; ; # [Events]",
             fMaxCbmLen + 1,
             -1,
             fMaxCbmLen,
             kiNbPart,
             -0.5,
             kiNbPart - 0.5);
  fhMcTrkStartPrimMultiTrk =
    new TH2D("TofTests_McTrkStartPrimMultiTrk",
             "Origin in Z of the MC primary tracks in hits with multiple "
             "tracks; MC trk Start Z [cm]; ; # [Events]",
             fMaxCbmLen + 1,
             -1,
             fMaxCbmLen,
             kiNbPart,
             -0.5,
             kiNbPart - 0.5);
  fhMcTrkStartSecMultiTrk =
    new TH2D("TofTests_McTrkStartSecMultiTrk",
             "Origin in Z of the MC secondary tracks in hits with multiple "
             "tracks; MC trk Start Z [cm]; ; # [Events]",
             fMaxCbmLen + 1,
             -1,
             fMaxCbmLen,
             kiNbPart,
             -0.5,
             kiNbPart - 0.5);
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    fhMcTrkStartPrimSingTrk->GetYaxis()->SetBinLabel(1 + iPartIdx,
                                                     ksPartTag[iPartIdx]);
    fhMcTrkStartSecSingTrk->GetYaxis()->SetBinLabel(1 + iPartIdx,
                                                    ksPartTag[iPartIdx]);
    fhMcTrkStartPrimMultiTrk->GetYaxis()->SetBinLabel(1 + iPartIdx,
                                                      ksPartTag[iPartIdx]);
    fhMcTrkStartSecMultiTrk->GetYaxis()->SetBinLabel(1 + iPartIdx,
                                                     ksPartTag[iPartIdx]);
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

  fhPointMatchWeight =
    new TH1I("TofTests_PointMatchWeight",
             "Weigth of TofPoints contributing to Hits; w [Prop. or ps]; #",
             1000,
             -0.05,
             100 - 0.05);

  gDirectory->cd(
    oldir
      ->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!

  return kTRUE;
}

// ------------------------------------------------------------------
Bool_t CbmTofHitFinderQa::FillHistos() {
  // Declare variables outside the loop
  CbmMCTrack* pMcTrk;
  CbmTofPoint* pTofPoint;
  CbmMatch* pMatchDigiPnt;
  CbmMatch* pMatchDigiPntB;
  CbmTofHit* pTofHit;
  CbmMatch* pMatchHitDigi;
  CbmMatch* pMatchHitPnt;

  Int_t iNbTracks, iNbTofPts, iNbTofRealPts, iNbTofDigisMatch, iNbTofHits,
    iNbTofHitsMatch;
  Int_t iNbTofDigis = 0;

  iNbTracks = fMcTracksColl->GetEntriesFast();
  iNbTofPts = fTofPointsColl->GetEntriesFast();
  if (kTRUE == fbRealPointAvail)
    iNbTofRealPts = fRealTofPointsColl->GetEntriesFast();
  else
    iNbTofRealPts = 0;
  iNbTofHits = fTofHitsColl->GetEntriesFast();
  if (kFALSE == fbHitProducerSource) {
    iNbTofDigis      = fTofDigisColl->GetEntriesFast();
    iNbTofDigisMatch = fTofDigiMatchPointsColl->GetEntriesFast();
    iNbTofHitsMatch  = fTofDigiMatchColl->GetEntriesFast();
    if (iNbTofDigis != iNbTofDigisMatch)
      LOG(fatal)
        << "CbmTofHitFinderQa::FillHistos => Nb entries in TofDigiMatchPoints "
           "TClonesArray doe not match nb entries in TofDigi!!!";
    if (iNbTofHits != iNbTofHitsMatch)
      LOG(fatal)
        << "CbmTofHitFinderQa::FillHistos => Nb entries in TofDigiMatch "
           "TClonesArray doe not match nb entries in TofHit!!! "
        << iNbTofHits << " VS " << iNbTofHitsMatch
        << " (Prev step: " << iNbTofDigis << " VS " << iNbTofDigisMatch << " )";
  }  // if( kFALSE == fbHitProducerSource )

  fvulIdxTracksWithPnt.clear();
  fvulIdxTracksWithHit.clear();
  fvulIdxPrimTracksWithPnt.clear();
  fvulIdxPrimTracksWithHit.clear();
  fvulIdxSecTracksWithPnt.clear();
  fvulIdxSecTracksWithHit.clear();
  fvulIdxHiddenTracksWithHit.clear();
  fvulIdxHiddenPrimTracksWithHit.clear();
  fvulIdxHiddenSecTracksWithHit.clear();
  for (UInt_t uNbGaps = 0; uNbGaps < fuMaxCrossedGaps; uNbGaps++) {
    fvulIdxTracksWithPntGaps[uNbGaps].clear();
    fvulIdxTracksWithHitGaps[uNbGaps].clear();
    fvulIdxPrimTracksWithPntGaps[uNbGaps].clear();
    fvulIdxPrimTracksWithHitGaps[uNbGaps].clear();
    fvulIdxSecTracksWithPntGaps[uNbGaps].clear();
    fvulIdxSecTracksWithHitGaps[uNbGaps].clear();
  }  // for( UInt_t uNbGaps = 0; uNbGaps < fuMaxCrossedGaps; uNbGaps++)

  // Tracks Info
  Int_t iNbTofTracks     = 0;
  Int_t iNbTofTracksPrim = 0;
  std::vector<Bool_t> vbTrackHasHit(iNbTracks, kFALSE);
  // Are MC tracks reconstructable in STS?
  //   std::vector< Bool_t > vbTrackStsRecOk( iNbTracks kFALSE);
  for (Int_t iTrkInd = 0; iTrkInd < iNbTracks; iTrkInd++) {
    pMcTrk = (CbmMCTrack*) fMcTracksColl->At(iTrkInd);

    // Is track reconstructable in STS
    /*
      UInt_t uNbStsPnts = pMcTrk->GetNPoints(ECbmModuleId::kSts);

         // True criterium is whether enough STS stations are crossed
         // but if already less STS points, can escape looping
      if( kiMinNbStsPntAcc <= uNbStsPnts )
      {
         std::vector< UInt_t > vStsStationsId();
         for( UInt_t uStsPntIdx = 0; uStsPntIdx < uNbStsPnts; uStsPntIdx++)
         {
            UInt_t uStation = CbmStsAddress::GetElementId(
               (dynamic_cast<CbmStsPoint*>fStsPointsColl->At( uStsPntIdx ))->GetDetectorID(), 1);
            if( kiMinNbStsPntAcc <= vStsStationsId.size() )
            {
               vbTrackStsRecOk[iTrkInd] = kTRUE;
               break;
            }
         } // for( UInt_t uStsPntIdx = 0; uStsPntIdx < uNbStsPnts; uStsPntIdx++)
      } // if( kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts) )
       */

    if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
      iNbTofTracks++;
      // Keep track of MC tracks with at least one TOF Point
      fvulIdxTracksWithPnt.push_back(iTrkInd);

      UInt_t uNbTofPnt = pMcTrk->GetNPoints(ECbmModuleId::kTof) - 1;
      if (uNbTofPnt < fuMaxCrossedGaps)
        fvulIdxTracksWithPntGaps[uNbTofPnt].push_back(iTrkInd);

      if (-1 == pMcTrk->GetMotherId()) {
        iNbTofTracksPrim++;
        fvulIdxPrimTracksWithPnt.push_back(iTrkInd);
        if (uNbTofPnt < fuMaxCrossedGaps)
          fvulIdxPrimTracksWithPntGaps[uNbTofPnt].push_back(iTrkInd);
      }  // if( -1 == pMcTrk->GetMotherId() )
      else {
        fvulIdxSecTracksWithPnt.push_back(iTrkInd);

        if (uNbTofPnt < fuMaxCrossedGaps)
          fvulIdxSecTracksWithPntGaps[uNbTofPnt].push_back(iTrkInd);
      }  // else of if( -1 == pMcTrk->GetMotherId() )
    }    // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kTof) )

    // tracks mapping: Only when creating normalization histos
    // Assume only TOF in setup, no field (only straight tracks)
    // and all tracks reach TOF (protons)
    if (kTRUE == fbNormHistGenMode) {
      // XYZ mapping: assume tracks along Z axis
      if (pMcTrk->GetPz() == pMcTrk->GetP()) {
        fhTrackMapXY->Fill(pMcTrk->GetStartX(), pMcTrk->GetStartY());
        //            fhTrackMapXZ->Fill( pMcTrk->GetStartX(), fdWallPosZ ); // Not sure how to get Z here
        //            fhTrackMapYZ->Fill( pMcTrk->GetStartY(), fdWallPosZ ); // Not sure how to get Z here
      }  // if( pMcTrk->GetPz() == pMcTrk->GetP() )

      // Angular mapping: assume tracks all coming from origin and not necess. along Z axis
      if (0 != pMcTrk->GetPz()
          && ((0 != pMcTrk->GetPx()) || (0 != pMcTrk->GetPy())))
        fhTrackMapAng->Fill(
          TMath::ATan2(pMcTrk->GetPx(), pMcTrk->GetPz()) * 180.0 / TMath::Pi(),
          TMath::ATan2(pMcTrk->GetPy(), pMcTrk->GetPz()) * 180.0 / TMath::Pi());

      // Spherical mapping: assume tracks all coming from origin and not necess. along Z axis
      if (0 != pMcTrk->GetPz() && 0 != pMcTrk->GetPx())
        fhTrackMapSph->Fill(TMath::ATan2(pMcTrk->GetPt(), pMcTrk->GetPz()),
                            TMath::ATan2(pMcTrk->GetPy(), pMcTrk->GetPx()));
    }  // if( kTRUE == fbNormHistGenMode )

    // Physics coord mapping, 1 per particle type
    Int_t iPdgCode = pMcTrk->GetPdgCode();
    Int_t iPartIdx = -1;
    for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
      if (kiPartPdgCode[iPart] == iPdgCode) {
        iPartIdx = iPart;
        break;
      }  // if( kiPartPdgCode[iPart] == iPdgCode )
    if (-1 == iPartIdx) iPartIdx = 0;

    // Dependence of Track origin on centrality or position
    if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
      fvhTrackAllStartZCent[iPartIdx]->Fill(pMcTrk->GetStartZ(),
                                            fMCEventHeader->GetB());
      if (2 == iPartIdx)  // 3D plot only for e-
        fvhTrackAllStartXZCent[iPartIdx]->Fill(
          pMcTrk->GetStartX(), pMcTrk->GetStartZ(), fMCEventHeader->GetB());
      fvhTrackAllStartXZ[iPartIdx]->Fill(pMcTrk->GetStartZ(),
                                         pMcTrk->GetStartX());
      fvhTrackAllStartYZ[iPartIdx]->Fill(pMcTrk->GetStartZ(),
                                         pMcTrk->GetStartY());
    }

    if (-1 == pMcTrk->GetMotherId()) {
      // primary track
      // Phase space
      fvhPtmRapGenTrk[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                      pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabGenTrk[iPartIdx]->Fill(pMcTrk->GetP());
      // Do the same for tracks within STS acceptance
      if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts)) {
        fvhPtmRapStsPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                        pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabStsPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kSts) )
      // Do the same for tracks within STS acceptance
      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
        fvhPtmRapGenTrkTofPnt[iPartIdx]->Fill(
          pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabGenTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());

        if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts))
          fvhPlabStsTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(kTof) )
    }    // if( -1 == pMcTrk->GetMotherId() )
    else {
      // secondary track
      // Dependence of Track origin on centrality
      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof))
        fvhTrackSecStartZCent[iPartIdx]->Fill(pMcTrk->GetStartZ(),
                                              fMCEventHeader->GetB());

      // Phase space
      fvhPtmRapSecGenTrk[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                         pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabSecGenTrk[iPartIdx]->Fill(pMcTrk->GetP());
      // Do the same for tracks within STS acceptance
      if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts)) {
        fvhPtmRapSecStsPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                           pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabSecStsPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kSts) )
      // Do the same for tracks within STS acceptance
      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
        fvhPtmRapSecGenTrkTofPnt[iPartIdx]->Fill(
          pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabSecGenTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());

        if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts))
          fvhPlabSecStsTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kTof) )
    }    // else of if( -1 == pMcTrk->GetMotherId() )
  }      // for(Int_t iTrkInd = 0; iTrkInd < nMcTracks; iTrkInd++)

  // Loop over Points and map them?
  for (Int_t iPntInd = 0; iPntInd < iNbTofPts; iPntInd++) {
    // Get a pointer to the TOF point
    pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iPntInd);
    // Get a pointer to the corresponding MC Track
    pMcTrk = (CbmMCTrack*) fMcTracksColl->At(pTofPoint->GetTrackID());

    // Obtain position
    TVector3 vPntPos;
    pTofPoint->Position(vPntPos);

    Double_t dX = vPntPos.X();
    Double_t dY = vPntPos.Y();
    Double_t dZ = vPntPos.Z();

    // tracks mapping: Only when creating normalization histos
    // Assume only TOF in setup, no field (only straight tracks)
    // and all tracks reach TOF (protons)
    if (kTRUE == fbNormHistGenMode) {
      // XYZ mapping: assume tracks along Z axis
      if (pMcTrk->GetPz() == pMcTrk->GetP() && pMcTrk->GetStartX() == dX
          && pMcTrk->GetStartY() == dY) {
        fhTrackMapXZ->Fill(dX, dZ);  // only way to get Z here?
        fhTrackMapYZ->Fill(dY, dZ);  // only way to get Z here?
      }                              // if( pMcTrk->GetPz() == pMcTrk->GetP() )
    }                                // if( kTRUE == fbNormHistGenMode )

    fhPointMapXY->Fill(dX, dY);
    fhPointMapXZ->Fill(dX, dZ);
    fhPointMapYZ->Fill(dY, dZ);

    Double_t dThetaX = TMath::ATan2(dX, dZ) * 180.0 / TMath::Pi();
    Double_t dThetaY = TMath::ATan2(dY, dZ) * 180.0 / TMath::Pi();
    fhPointMapAng->Fill(dThetaX, dThetaY);

    Double_t dTheta =
      TMath::ATan2(TMath::Sqrt(dX * dX + dY * dY), dZ);  // *180.0/TMath::Pi();
    Double_t dPhi = TMath::ATan2(dY, dX);                // *180.0/TMath::Pi();
    fhPointMapSph->Fill(dTheta, dPhi);

    // Physics coord mapping, 1 per particle type
    Int_t iPdgCode = pMcTrk->GetPdgCode();
    Int_t iPartIdx = -1;
    for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
      if (kiPartPdgCode[iPart] == iPdgCode) {
        iPartIdx = iPart;
        break;
      }  // if( kiPartPdgCode[iPart] == iPdgCode )
    if (-1 == iPartIdx) iPartIdx = 0;

    // Beam pipe check
    fvhTofPntAllAngCent[iPartIdx]->Fill(
      dThetaX, dThetaY, fMCEventHeader->GetB());

    if (-1 == pMcTrk->GetMotherId()) {
      // primary track
      // Phase space
      fvhPtmRapTofPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                      pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
    }  // if( -1 == pMcTrk->GetMotherId() )
    else {
      // secondary track
      // Phase space
      fvhPtmRapSecTofPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                         pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabSecTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
    }  // else of if( -1 == pMcTrk->GetMotherId() )
  }    // for (Int_t iPntInd = 0; iPntInd < nTofPoint; iPntInd++ )

  // Loop over Real Points and map them, only in case of protons
  if (kTRUE == fbRealPointAvail)
    for (Int_t iPntInd = 0; iPntInd < iNbTofRealPts; iPntInd++) {
      // Get a pointer to the TOF point
      pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iPntInd);
      // Get a pointer to the corresponding MC Track
      pMcTrk = (CbmMCTrack*) fMcTracksColl->At(pTofPoint->GetTrackID());

      // Physics coord mapping, 1 per particle type
      //      Int_t iPdgCode = pMcTrk->GetPdgCode();
      //      if( 2212 == iPdgCode ) // Protons cut, comment to get all
      {
        // Obtain position
        TVector3 vPntPos;
        pTofPoint->Position(vPntPos);

        Double_t dX = vPntPos.X();
        Double_t dY = vPntPos.Y();
        Double_t dZ = vPntPos.Z();

        fhRealPointMapXY->Fill(dX, dY);
        fhRealPointMapXZ->Fill(dX, dZ);
        fhRealPointMapYZ->Fill(dY, dZ);

        Double_t dThetaX = TMath::ATan2(dX, dZ) * 180.0 / TMath::Pi();
        Double_t dThetaY = TMath::ATan2(dY, dZ) * 180.0 / TMath::Pi();
        fhRealPointMapAng->Fill(dThetaX, dThetaY);

        Double_t dTheta = TMath::ATan2(TMath::Sqrt(dX * dX + dY * dY),
                                       dZ);      // *180.0/TMath::Pi();
        Double_t dPhi   = TMath::ATan2(dY, dX);  // *180.0/TMath::Pi();
        fhRealPointMapSph->Fill(dTheta, dPhi);
      }  // if( 2212 == iPdgCode )
    }    // for (Int_t iPntInd = 0; iPntInd < iNbTofRealPts; iPntInd++ )

  // Loop over Digis and map them?
  if (kFALSE == fbHitProducerSource) {
    if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
      CbmTofDigi* pTofDigi;
      for (Int_t iDigInd = 0; iDigInd < iNbTofDigis; iDigInd++) {
        pTofDigi = (CbmTofDigi*) fTofDigisColl->At(iDigInd);

        Int_t iSmType = pTofDigi->GetType();
        Int_t iSm     = pTofDigi->GetSm();
        Int_t iRpc    = pTofDigi->GetRpc();
        Int_t iCh     = pTofDigi->GetChannel();
        // First Get X/Y position info
        if (fGeoHandler->GetGeoVersion() < k14a)
          iCh = iCh + 1;  //FIXME: Due to change in tofGeoHandler
        CbmTofDetectorInfo xDetInfo(
          ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
        Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
        fChannelInfo = fDigiPar->GetCell(iChId);

        Double_t dX = fChannelInfo->GetX();
        Double_t dY = fChannelInfo->GetY();
        Double_t dZ = fChannelInfo->GetZ();

        fhDigiMapXY->Fill(dX, dY);
        fhDigiMapXZ->Fill(dX, dZ);
        fhDigiMapYZ->Fill(dY, dZ);

        Double_t dThetaX = TMath::ATan2(dX, dZ) * 180.0 / TMath::Pi();
        Double_t dThetaY = TMath::ATan2(dY, dZ) * 180.0 / TMath::Pi();
        fhDigiMapAng->Fill(dThetaX, dThetaY);

        Double_t dTheta = TMath::ATan2(TMath::Sqrt(dX * dX + dY * dY),
                                       dZ);      // *180.0/TMath::Pi();
        Double_t dPhi   = TMath::ATan2(dY, dX);  // *180.0/TMath::Pi();
        fhDigiMapSph->Fill(dTheta, dPhi);
      }  // for( Int_t iDigInd = 0; iDigInd < iNbTofDigis; iDigInd++ )
    }    // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
  }      // if( kFALSE == fbHitProducerSource )

  // Loop Over Hits
  Int_t iNbTofHitsSingPnt = 0;
  Int_t iNbTofHitsMultPnt = 0;
  Int_t iNbTofHitsSingTrk = 0;
  Int_t iNbTofHitsMultTrk = 0;
  for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
    std::vector<Int_t> vTofPointsId;
    std::vector<Int_t> vTofTracksId;
    std::vector<Double_t> vTofTracksWeight;
    std::vector<Int_t> vTofTracksFirstPntId;
    Double_t dPntMeanPosX   = 0;
    Double_t dPntMeanPosY   = 0;
    Double_t dPntMeanPosZ   = 0;
    Double_t dPntMeanTime   = 0;
    Double_t dTrkMeanPosX   = 0;
    Double_t dTrkMeanPosY   = 0;
    Double_t dTrkMeanPosZ   = 0;
    Double_t dTrkMeanTime   = 0;
    Int_t iClosestPntIdx    = -1;
    Double_t dClosestPntDr  = 1e18;
    Int_t iFurthestPntIdx   = -1;
    Double_t dFurthestPntDr = -1;
    Int_t iClosestTrkIdx    = -1;
    Double_t dClosestTrkDr  = 1e18;
    Int_t iFurthestTrkIdx   = -1;
    Double_t dFurthestTrkDr = -1;

    pTofHit         = (CbmTofHit*) fTofHitsColl->At(iHitInd);
    pMatchHitPnt    = (CbmMatch*) fTofHitMatchColl->At(iHitInd);
    Int_t iNbPntHit = pMatchHitPnt->GetNofLinks();

    Double_t dX    = pTofHit->GetX();
    Double_t dY    = pTofHit->GetY();
    Double_t dZ    = pTofHit->GetZ();
    Double_t dErrX = pTofHit->GetDx();
    Double_t dErrY = pTofHit->GetDy();
    Double_t dErrZ = pTofHit->GetDz();
    Double_t dErrT = pTofHit->GetTimeError() * 1000.0;
    //      Double_t dErrR = TMath::Sqrt( dErrX*dErrX + dErrY*dErrY + dErrZ*dErrZ );
    Double_t dErrR = TMath::Sqrt(dErrX * dErrX + dErrY * dErrY);

    Int_t iClusterSize = pTofHit->GetClusterSize();
    /*
      LOG(info) << "Hit errors = X "
                << dErrX << " Y " << dErrY << " Z " << dErrZ
                << " T " << dErrT
                << " R " << dErrR
                ;
*/
    fhHitMapXY->Fill(dX, dY);
    fhHitMapXZ->Fill(dX, dZ);
    fhHitMapYZ->Fill(dY, dZ);

    Double_t dThetaX = TMath::ATan2(dX, dZ) * 180.0 / TMath::Pi();
    Double_t dThetaY = TMath::ATan2(dY, dZ) * 180.0 / TMath::Pi();
    fhHitMapAng->Fill(dThetaX, dThetaY);

    Double_t dTheta =
      TMath::ATan2(TMath::Sqrt(dX * dX + dY * dY), dZ);  // *180.0/TMath::Pi();
    Double_t dPhi = TMath::ATan2(dY, dX);                // *180.0/TMath::Pi();
    fhHitMapSph->Fill(dTheta, dPhi);

    if (kFALSE == fbNormHistGenMode) {
      // Tests using the Digis as data
      if (kFALSE == fbHitProducerSource) {
        pMatchHitDigi     = (CbmMatch*) fTofDigiMatchColl->At(iHitInd);
        Int_t iNbDigisHit = pMatchHitDigi->GetNofLinks();
        if (0 != iNbDigisHit % 2)
          LOG(fatal)
            << "CbmTofHitFinderQa::FillHistos => Nb of digis matching Hit #"
            << iHitInd << " in event #" << fEvents
            << " is not a multiple of 2 => should not happen as both ends of "
               "strp required!!!";

        // Nb different TOF digis in Hit
        fhNbDigisInHit->Fill(iNbDigisHit);
        fhNbDigisInHitMapXY->Fill(dX, dY, iNbDigisHit);

        // Loop over Digis inside Hit
        if (kTRUE == fDigiBdfPar->UseExpandedDigi()) {
          CbmTofDigi* pTofDigi;
          for (Int_t iDigi = 0; iDigi < iNbDigisHit; iDigi++) {
            CbmLink lDigi  = pMatchHitDigi->GetLink(iDigi);
            Int_t iDigiIdx = lDigi.GetIndex();

            if (iNbTofDigis <= iDigiIdx) {
              LOG(error)
                << "CbmTofHitFinderQa::FillHistos => Digi index from Hit #"
                << iHitInd << " in event #" << fEvents
                << " is bigger than nb entries in Digis arrays => ignore it!!!";
              continue;
            }  // if( iNbTofDigis <= iDigiIdx )

            pTofDigi      = (CbmTofDigi*) fTofDigisColl->At(iDigiIdx);
            pMatchDigiPnt = (CbmMatch*) fTofDigiMatchPointsColl->At(iDigiIdx);

            CbmLink lPt  = pMatchDigiPnt->GetMatchedLink();
            Int_t iPtIdx = lPt.GetIndex();
            Int_t iTrkId =
              ((CbmTofPoint*) fTofPointsColl->At(iPtIdx))->GetTrackID();

            Int_t iSmType     = pTofDigi->GetType();
            Int_t iSm         = pTofDigi->GetSm();
            Int_t iRpc        = pTofDigi->GetRpc();
            Int_t iCh         = pTofDigi->GetChannel();
            Int_t iGlobalChan = iCh + fvRpcChOffs[iSmType][iSm][iRpc];

            // Check Left-Right MC missmatch (digis match always stored by pairs)
            if (0 == iDigi % 2) {
              // Get Info about the other end of the strip
              pMatchDigiPntB =
                (CbmMatch*) fTofDigiMatchPointsColl->At(iDigiIdx + 1);

              CbmLink lPtB  = pMatchDigiPntB->GetMatchedLink();
              Int_t iPtIdxB = lPtB.GetIndex();

              // Check Left-Right missmatch for MC Point
              if (iPtIdx != iPtIdxB) {
                // Check Left-Right missmatch for MC Track
                if (iTrkId
                    != ((CbmTofPoint*) fTofPointsColl->At(iPtIdxB))
                         ->GetTrackID()) {
                  fhLeftRightDigiMatch->Fill(iGlobalChan, 2);
                }  // if( iTrkId != ((CbmTofPoint*) fTofPointsColl->At(iPtIdxB))->GetTrackID() )
                else
                  fhLeftRightDigiMatch->Fill(iGlobalChan, 1);
              }  // if( iPtIdx != iPtIdxB )
              else
                fhLeftRightDigiMatch->Fill(iGlobalChan, 0);

            }  // if( 0 == iDigi%2 )
          }    // for( Int_t iDigiIdx = 0; iDigiIdx < iNbDigisHit; iDigiIdx++)
        }      // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
      }        // if( kFALSE == fbHitProducerSource )

      // Tests independent of the Digis
      for (Int_t iPnt = 0; iPnt < iNbPntHit; iPnt++) {
        CbmLink lPnt = pMatchHitPnt->GetLink(iPnt);
        Int_t iPtIdx = lPnt.GetIndex();

        if (iNbTofPts <= iPtIdx) {
          LOG(error)
            << "CbmTofHitFinderQa::FillHistos => Pnt index from Hit #"
            << iHitInd << " in event #" << fEvents
            << " is bigger than nb entries in TofPoints arrays => ignore it!!!";
          continue;
        }  // if( iNbTofDigis <= iDigiIdx )

        fhPointMatchWeight->Fill(lPnt.GetWeight());

        pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iPtIdx);
        if (kTRUE == fbRealPointAvail) {
          // Always only one mean MC Point Index per MC TofPoint
          // Weight always 1.0 so just read the index
          Int_t iRealPntIdx =
            (((CbmMatch*) fRealTofMatchColl->At(iPtIdx))->GetLink(0))
              .GetIndex();
          pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iRealPntIdx);
          iPtIdx    = iRealPntIdx;
        }  // if( kTRUE == fbRealPointAvail )

        Int_t iTrkId = pTofPoint->GetTrackID();

        // MC Track losses
        if (kFALSE == vbTrackHasHit[iTrkId]) vbTrackHasHit[iTrkId] = kTRUE;

        // Count Nb different MC Points in Hit
        Bool_t bPointFound = kFALSE;
        for (UInt_t uPrevPtIdx = 0; uPrevPtIdx < vTofPointsId.size();
             uPrevPtIdx++)
          if (iPtIdx == vTofPointsId[uPrevPtIdx]) {
            bPointFound = kTRUE;
            break;
          }  // if( iPtIdx == vTofPointsId[uPrevPtIdx] )
        if (kFALSE == bPointFound) {
          vTofPointsId.push_back(iPtIdx);

          // Obtain Point position
          //               pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iPtIdx);
          TVector3 vPntPos;
          pTofPoint->Position(vPntPos);

          // Compute mean MC Points position (X, Y, Z, T)
          dPntMeanPosX += vPntPos.X();
          dPntMeanPosY += vPntPos.Y();
          dPntMeanPosZ += vPntPos.Z();
          dPntMeanTime += pTofPoint->GetTime();

          // Check if closest MC Point to Hit position
          Double_t dPntDeltaR =
            TMath::Sqrt((dX - vPntPos.X()) * (dX - vPntPos.X())
                        + (dY - vPntPos.Y()) * (dY - vPntPos.Y()));
          //                   + (dZ - vPntPos.Z())*(dZ - vPntPos.Z()) );
          if (dPntDeltaR < dClosestPntDr) {
            iClosestPntIdx = iPtIdx;
            dClosestPntDr  = dPntDeltaR;
          }  // if( dPntDeltaR < dClosestPntDr )
          // Check if furthest MC Point to Hit position
          if (dFurthestPntDr < dPntDeltaR) {
            iFurthestPntIdx = iPtIdx;
            dFurthestPntDr  = dPntDeltaR;
          }  // if( dFurthestPntDr < dPntDeltaR )
        }    // if( kFALSE == bPointFound )

        // Count Nb different MC Tracks in Hit
        // Build a list of Track weigths
        Bool_t bTrackFound = kFALSE;
        for (UInt_t uPrevTrkIdx = 0; uPrevTrkIdx < vTofTracksId.size();
             uPrevTrkIdx++)
          if (iTrkId == vTofTracksId[uPrevTrkIdx]) {
            bTrackFound = kTRUE;
            vTofTracksWeight[uPrevTrkIdx] += lPnt.GetWeight();
            break;
          }  // if( iTrkId == vTofPointsId[uPrevTrkIdx] )
        if (kFALSE == bTrackFound) {
          vTofTracksId.push_back(iTrkId);
          vTofTracksWeight.push_back(lPnt.GetWeight());
          vTofTracksFirstPntId.push_back(iPtIdx);

          // Obtain Point position (Consider 1st Pnt of each Trk is approximate coord)
          //               pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iPtIdx);
          TVector3 vPntPos;
          pTofPoint->Position(vPntPos);

          // Compute mean MC Tracks position (X, Y, Z, T)
          dTrkMeanPosX += vPntPos.X();
          dTrkMeanPosY += vPntPos.Y();
          dTrkMeanPosZ += vPntPos.Z();
          dTrkMeanTime += pTofPoint->GetTime();

          // Check if closest MC track to Hit position
          Double_t dTrkDeltaR =
            TMath::Sqrt((dX - vPntPos.X()) * (dX - vPntPos.X())
                        + (dY - vPntPos.Y()) * (dY - vPntPos.Y()));
          //                   + (dZ - vPntPos.Z())*(dZ - vPntPos.Z()) );
          if (dTrkDeltaR < dClosestTrkDr) {
            iClosestTrkIdx = iPtIdx;
            dClosestTrkDr  = dTrkDeltaR;
          }  // if( dTrkDeltaR < dClosestTrkDr )
          // Check if furthest MC track to Hit position
          if (dFurthestTrkDr < dTrkDeltaR) {
            iFurthestTrkIdx = iPtIdx;
            dFurthestTrkDr  = dTrkDeltaR;
          }  // if( dFurthestTrkDr < dTrkDeltaR )
        }    // if( kFALSE == bTrackFound )
      }      // for( Int_t iPnt = 0; iPnt < iNbPntHit; iPnt++)

      // Count Nb different MC Points in Hit
      UInt_t uNbPointsInHit = vTofPointsId.size();
      fhNbPointsInHit->Fill(uNbPointsInHit);
      // Count Nb different MC Tracks in Hit
      UInt_t uNbTracksInHit = vTofTracksId.size();
      fhNbTracksInHit->Fill(uNbTracksInHit);

      // Keep track of MC tracks part of at least one TOF Hit
      UInt_t uHitTrk;
      for (UInt_t uTrkInHit = 0; uTrkInHit < vTofTracksId.size(); uTrkInHit++) {
        for (uHitTrk = 0; uHitTrk < fvulIdxTracksWithHit.size(); uHitTrk++)
          if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
              == fvulIdxTracksWithHit[uHitTrk])
            break;
        if (fvulIdxTracksWithHit.size() == uHitTrk)
          fvulIdxTracksWithHit.push_back(vTofTracksId[uTrkInHit]);

        // Get a pointer to the corresponding MC Track
        pMcTrk = (CbmMCTrack*) fMcTracksColl->At(vTofTracksId[uTrkInHit]);

        if (-1 == pMcTrk->GetMotherId()) {
          for (uHitTrk = 0; uHitTrk < fvulIdxPrimTracksWithHit.size();
               uHitTrk++)
            if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                == fvulIdxPrimTracksWithHit[uHitTrk])
              break;
          if (fvulIdxPrimTracksWithHit.size() == uHitTrk)
            fvulIdxPrimTracksWithHit.push_back(vTofTracksId[uTrkInHit]);
        }  // if( -1 == pMcTrk->GetMotherId() )
        else {
          for (uHitTrk = 0; uHitTrk < fvulIdxSecTracksWithHit.size(); uHitTrk++)
            if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                == fvulIdxSecTracksWithHit[uHitTrk])
              break;
          if (fvulIdxSecTracksWithHit.size() == uHitTrk)
            fvulIdxSecTracksWithHit.push_back(vTofTracksId[uTrkInHit]);
        }  // else of if( -1 == pMcTrk->GetMotherId() )

        // Check as function of Nb gaps crossed
        UInt_t uNbTofPnt = pMcTrk->GetNPoints(ECbmModuleId::kTof) - 1;
        if (uNbTofPnt < fuMaxCrossedGaps) {
          for (uHitTrk = 0;
               uHitTrk < fvulIdxTracksWithHitGaps[uNbTofPnt].size();
               uHitTrk++)
            if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                == fvulIdxTracksWithHitGaps[uNbTofPnt][uHitTrk])
              break;
          if (fvulIdxTracksWithHitGaps[uNbTofPnt].size() == uHitTrk)
            fvulIdxTracksWithHitGaps[uNbTofPnt].push_back(
              vTofTracksId[uTrkInHit]);

          if (-1 == pMcTrk->GetMotherId()) {
            for (uHitTrk = 0;
                 uHitTrk < fvulIdxPrimTracksWithHitGaps[uNbTofPnt].size();
                 uHitTrk++)
              if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                  == fvulIdxPrimTracksWithHitGaps[uNbTofPnt][uHitTrk])
                break;
            if (fvulIdxPrimTracksWithHitGaps[uNbTofPnt].size() == uHitTrk)
              fvulIdxPrimTracksWithHitGaps[uNbTofPnt].push_back(
                vTofTracksId[uTrkInHit]);
          }  // if( -1 == pMcTrk->GetMotherId() )
          else {
            for (uHitTrk = 0;
                 uHitTrk < fvulIdxSecTracksWithHitGaps[uNbTofPnt].size();
                 uHitTrk++)
              if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                  == fvulIdxSecTracksWithHitGaps[uNbTofPnt][uHitTrk])
                break;
            if (fvulIdxSecTracksWithHitGaps[uNbTofPnt].size() == uHitTrk)
              fvulIdxSecTracksWithHitGaps[uNbTofPnt].push_back(
                vTofTracksId[uTrkInHit]);
          }  // else of if( -1 == pMcTrk->GetMotherId() )
        }  // if( static_cast<UInt_t>(pMcTrk->GetNPoints(ECbmModuleId::kTof)) < fuMaxCrossedGaps + 1 )

        if (0 == vTofTracksWeight[uTrkInHit]) {
          // Tracks generating only signal hidden under signal from other tracks!

          for (uHitTrk = 0; uHitTrk < fvulIdxHiddenTracksWithHit.size();
               uHitTrk++)
            if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                == fvulIdxHiddenTracksWithHit[uHitTrk])
              break;
          if (fvulIdxHiddenTracksWithHit.size() == uHitTrk)
            fvulIdxHiddenTracksWithHit.push_back(vTofTracksId[uTrkInHit]);

          // Get a pointer to the corresponding MC Track
          pMcTrk = (CbmMCTrack*) fMcTracksColl->At(vTofTracksId[uTrkInHit]);

          if (-1 == pMcTrk->GetMotherId()) {
            for (uHitTrk = 0; uHitTrk < fvulIdxHiddenPrimTracksWithHit.size();
                 uHitTrk++)
              if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                  == fvulIdxHiddenPrimTracksWithHit[uHitTrk])
                break;
            if (fvulIdxHiddenPrimTracksWithHit.size() == uHitTrk)
              fvulIdxHiddenPrimTracksWithHit.push_back(vTofTracksId[uTrkInHit]);
          }  // if( -1 == pMcTrk->GetMotherId() )
          else {
            for (uHitTrk = 0; uHitTrk < fvulIdxHiddenSecTracksWithHit.size();
                 uHitTrk++)
              if (static_cast<ULong64_t>(vTofTracksId[uTrkInHit])
                  == fvulIdxHiddenSecTracksWithHit[uHitTrk])
                break;
            if (fvulIdxHiddenSecTracksWithHit.size() == uHitTrk)
              fvulIdxHiddenSecTracksWithHit.push_back(vTofTracksId[uTrkInHit]);
          }  // else of if( -1 == pMcTrk->GetMotherId() )
        }    // if( 0 == vTofTracksWeight[uTrkInHit] )
      }  // for( UInt_t uTrkInHit = 0; uTrkInHit < vTofTracksId.size(); uTrkInHit ++)

      // Check Hit Quality for Hits coming from a single MC Point
      if (1 == uNbPointsInHit) {
        iNbTofHitsSingPnt++;

        fhHitMapSingPntXY->Fill(dX, dY);
        fhHitMapSingPntXZ->Fill(dX, dZ);
        fhHitMapSingPntYZ->Fill(dY, dZ);
        fhHitMapSingPntAng->Fill(dThetaX, dThetaY);
        fhHitMapSingPntSph->Fill(dTheta, dPhi);

        //            pTofPoint = (CbmTofPoint*) fTofPointsColl->At(vTofPointsId[0]);
        if (kTRUE == fbRealPointAvail)
          pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(vTofPointsId[0]);
        else
          pTofPoint = (CbmTofPoint*) fTofPointsColl->At(vTofPointsId[0]);

        // Obtain Point position
        TVector3 vPntPos;
        pTofPoint->Position(vPntPos);

        Double_t dDeltaX = dX - vPntPos.X();
        Double_t dDeltaY = dY - vPntPos.Y();
        Double_t dDeltaZ = dZ - vPntPos.Z();
        Double_t dDeltaR = TMath::Sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY);
        Double_t dDeltaT = 1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

        fhSinglePointHitDeltaX->Fill(dDeltaX);
        fhSinglePointHitDeltaY->Fill(dDeltaY);
        fhSinglePointHitDeltaZ->Fill(dDeltaZ);
        fhSinglePointHitDeltaR->Fill(dDeltaR);
        fhSinglePointHitDeltaT->Fill(dDeltaT);
        fhSinglePointHitPullX->Fill(dDeltaX / dErrX);
        fhSinglePointHitPullY->Fill(dDeltaY / dErrY);
        fhSinglePointHitPullZ->Fill(
          dDeltaZ / dErrZ);  // Offset(s) bec. hit assigned to middle of det.
        fhSinglePointHitPullR->Fill(dDeltaR / dErrR);
        fhSinglePointHitPullT->Fill(dDeltaT / dErrT);

        fhCltSzSinglePointHitPullX->Fill(dDeltaX / dErrX, iClusterSize);
        fhCltSzSinglePointHitPullY->Fill(dDeltaY / dErrY, iClusterSize);
        fhCltSzSinglePointHitPullZ->Fill(
          dDeltaZ / dErrZ,
          iClusterSize);  // Offset(s) bec. hit assigned to middle of det.
        fhCltSzSinglePointHitPullR->Fill(dDeltaR / dErrR, iClusterSize);
        fhCltSzSinglePointHitPullT->Fill(dDeltaT / dErrT, iClusterSize);
      }  // if( 1 == vTofPointsId.size() )
         // For Hits with multiple MC Points
      else {
        iNbTofHitsMultPnt++;

        fhHitMapMultPntXY->Fill(dX, dY);
        fhHitMapMultPntXZ->Fill(dX, dZ);
        fhHitMapMultPntYZ->Fill(dY, dZ);
        fhHitMapMultPntAng->Fill(dThetaX, dThetaY);
        fhHitMapMultPntSph->Fill(dTheta, dPhi);

        // Check Hit Quality relative to mean of all MC Points
        dPntMeanPosX /= vTofPointsId.size();
        dPntMeanPosY /= vTofPointsId.size();
        dPntMeanPosZ /= vTofPointsId.size();
        dPntMeanTime /= vTofPointsId.size();

        Double_t dDeltaMeanX = dX - dPntMeanPosX;
        Double_t dDeltaMeanY = dY - dPntMeanPosY;
        Double_t dDeltaMeanZ = dZ - dPntMeanPosZ;
        Double_t dDeltaMeanR =
          TMath::Sqrt(dDeltaMeanX * dDeltaMeanX + dDeltaMeanY * dDeltaMeanY);
        Double_t dDeltaMeanT = 1000.0 * (pTofHit->GetTime() - dPntMeanTime);

        fhMultiPntHitMeanDeltaX->Fill(dDeltaMeanX, uNbPointsInHit);
        fhMultiPntHitMeanDeltaY->Fill(dDeltaMeanY, uNbPointsInHit);
        fhMultiPntHitMeanDeltaZ->Fill(dDeltaMeanZ, uNbPointsInHit);
        fhMultiPntHitMeanDeltaR->Fill(dDeltaMeanR, uNbPointsInHit);
        fhMultiPntHitMeanDeltaT->Fill(dDeltaMeanT, uNbPointsInHit);
        fhMultiPntHitMeanPullX->Fill(dDeltaMeanX / dErrX, uNbPointsInHit);
        fhMultiPntHitMeanPullY->Fill(dDeltaMeanY / dErrY, uNbPointsInHit);
        fhMultiPntHitMeanPullZ->Fill(
          dDeltaMeanZ / dErrZ,
          uNbPointsInHit);  // Offset(s) bec. hit assigned to middle of det.
        fhMultiPntHitMeanPullR->Fill(dDeltaMeanR / dErrR, uNbPointsInHit);
        fhMultiPntHitMeanPullT->Fill(dDeltaMeanT / dErrT, uNbPointsInHit);

        // Check Hit Quality relative to closest MC Point
        //               pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iClosestPntIdx);
        if (kTRUE == fbRealPointAvail)
          pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iClosestPntIdx);
        else
          pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iClosestPntIdx);

        // Obtain Point position
        TVector3 vPntPosClo;
        pTofPoint->Position(vPntPosClo);

        Double_t dDeltaCloX = dX - vPntPosClo.X();
        Double_t dDeltaCloY = dY - vPntPosClo.Y();
        Double_t dDeltaCloZ = dZ - vPntPosClo.Z();
        Double_t dDeltaCloR =
          TMath::Sqrt(dDeltaCloX * dDeltaCloX + dDeltaCloY * dDeltaCloY);
        Double_t dDeltaCloT =
          1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

        fhMultiPntHitClosestDeltaX->Fill(dDeltaCloX, uNbPointsInHit);
        fhMultiPntHitClosestDeltaY->Fill(dDeltaCloY, uNbPointsInHit);
        fhMultiPntHitClosestDeltaZ->Fill(dDeltaCloZ, uNbPointsInHit);
        fhMultiPntHitClosestDeltaR->Fill(dDeltaCloR, uNbPointsInHit);
        fhMultiPntHitClosestDeltaT->Fill(dDeltaCloT, uNbPointsInHit);
        fhMultiPntHitClosestPullX->Fill(dDeltaCloX / dErrX, uNbPointsInHit);
        fhMultiPntHitClosestPullY->Fill(dDeltaCloY / dErrY, uNbPointsInHit);
        fhMultiPntHitClosestPullZ->Fill(
          dDeltaCloZ / dErrZ,
          uNbPointsInHit);  // Offset(s) bec. hit assigned to middle of det.
        fhMultiPntHitClosestPullR->Fill(dDeltaCloR / dErrR, uNbPointsInHit);
        fhMultiPntHitClosestPullT->Fill(dDeltaCloT / dErrT, uNbPointsInHit);

        // Check Hit Quality relative to furthest MC Point
        //               pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iFurthestPntIdx);
        if (kTRUE == fbRealPointAvail)
          pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iFurthestPntIdx);
        else
          pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iFurthestPntIdx);

        // Obtain Point position
        TVector3 vPntPosFar;
        pTofPoint->Position(vPntPosFar);

        Double_t dDeltaFarX = dX - vPntPosFar.X();
        Double_t dDeltaFarY = dY - vPntPosFar.Y();
        Double_t dDeltaFarZ = dZ - vPntPosFar.Z();
        Double_t dDeltaFarR =
          TMath::Sqrt(dDeltaFarX * dDeltaFarX + dDeltaFarY * dDeltaFarY);
        Double_t dDeltaFarT =
          1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

        fhMultiPntHitFurthestDeltaX->Fill(dDeltaFarX, uNbPointsInHit);
        fhMultiPntHitFurthestDeltaY->Fill(dDeltaFarY, uNbPointsInHit);
        fhMultiPntHitFurthestDeltaZ->Fill(dDeltaFarZ, uNbPointsInHit);
        fhMultiPntHitFurthestDeltaR->Fill(dDeltaFarR, uNbPointsInHit);
        fhMultiPntHitFurthestDeltaT->Fill(dDeltaFarT, uNbPointsInHit);
        fhMultiPntHitFurthestPullX->Fill(dDeltaFarX / dErrX, uNbPointsInHit);
        fhMultiPntHitFurthestPullY->Fill(dDeltaFarY / dErrY, uNbPointsInHit);
        fhMultiPntHitFurthestPullZ->Fill(
          dDeltaFarZ / dErrZ,
          uNbPointsInHit);  // Offset(s) bec. hit assigned to middle of det.
        fhMultiPntHitFurthestPullR->Fill(dDeltaFarR / dErrR, uNbPointsInHit);
        fhMultiPntHitFurthestPullT->Fill(dDeltaFarT / dErrT, uNbPointsInHit);

        // Check Quality to best Point match (highest sum of TOT)
        // Do not fill it in Hit producer case as both strip ends receive a same weigth of 1/2
        if (kFALSE == fbHitProducerSource) {
          CbmLink lPnt = pMatchHitPnt->GetMatchedLink();
          Int_t iPtIdx = lPnt.GetIndex();
          pTofPoint    = (CbmTofPoint*) fTofPointsColl->At(iPtIdx);
          if (kTRUE == fbRealPointAvail) {
            // Always only one mean MC Point Index per MC TofPoint
            // Weight always 1.0 so just read the index
            Int_t iRealPntIdx =
              (((CbmMatch*) fRealTofMatchColl->At(iPtIdx))->GetLink(0))
                .GetIndex();
            pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iRealPntIdx);
            iPtIdx    = iRealPntIdx;
          }  // if( kTRUE == fbRealPointAvail )

          // Obtain Point position
          TVector3 vPntPosBest;
          pTofPoint->Position(vPntPosBest);

          Double_t dDeltaBestX = dX - vPntPosBest.X();
          Double_t dDeltaBestY = dY - vPntPosBest.Y();
          Double_t dDeltaBestZ = dZ - vPntPosBest.Z();
          Double_t dDeltaBestR =
            TMath::Sqrt(dDeltaBestX * dDeltaBestX + dDeltaBestY * dDeltaBestY);
          Double_t dDeltaBestT =
            1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

          fhMultiPntHitBestDeltaX->Fill(dDeltaBestX, uNbPointsInHit);
          fhMultiPntHitBestDeltaY->Fill(dDeltaBestY, uNbPointsInHit);
          fhMultiPntHitBestDeltaZ->Fill(dDeltaBestZ, uNbPointsInHit);
          fhMultiPntHitBestDeltaR->Fill(dDeltaBestR, uNbPointsInHit);
          fhMultiPntHitBestDeltaT->Fill(dDeltaBestT, uNbPointsInHit);
          fhMultiPntHitBestPullX->Fill(dDeltaBestX / dErrX, uNbPointsInHit);
          fhMultiPntHitBestPullY->Fill(dDeltaBestY / dErrY, uNbPointsInHit);
          fhMultiPntHitBestPullZ->Fill(
            dDeltaBestZ / dErrZ,
            uNbPointsInHit);  // Offset(s) bec. hit assigned to middle of det.
          fhMultiPntHitBestPullR->Fill(dDeltaBestR / dErrR, uNbPointsInHit);
          fhMultiPntHitBestPullT->Fill(dDeltaBestT / dErrT, uNbPointsInHit);
        }  // if( kFALSE == fbHitProducerSource )
      }    // else of if( 1 == vTofPointsId.size() )

      // Check Hit Quality for Hits coming from a single MC Track
      if (1 == uNbTracksInHit) {
        iNbTofHitsSingTrk++;

        fhHitMapSingTrkXY->Fill(dX, dY);
        fhHitMapSingTrkXZ->Fill(dX, dZ);
        fhHitMapSingTrkYZ->Fill(dY, dZ);
        fhHitMapSingTrkAng->Fill(dThetaX, dThetaY);
        fhHitMapSingTrkSph->Fill(dTheta, dPhi);

        // => If all points come from the same track, any of them should give
        //    the approximate position and time of the track at the detector level
        //            pTofPoint = (CbmTofPoint*) fTofPointsColl->At(vTofPointsId[0]);
        if (kTRUE == fbRealPointAvail)
          pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(vTofPointsId[0]);
        else
          pTofPoint = (CbmTofPoint*) fTofPointsColl->At(vTofPointsId[0]);

        // Obtain Point position
        TVector3 vPntPos;
        pTofPoint->Position(vPntPos);

        Double_t dDeltaX = dX - vPntPos.X();
        Double_t dDeltaY = dY - vPntPos.Y();
        Double_t dDeltaZ = dZ - vPntPos.Z();
        Double_t dDeltaR = TMath::Sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY);
        Double_t dDeltaT = 1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

        fhSingleTrackHitDeltaX->Fill(dDeltaX);
        fhSingleTrackHitDeltaY->Fill(dDeltaY);
        fhSingleTrackHitDeltaZ->Fill(dDeltaZ);
        fhSingleTrackHitDeltaR->Fill(dDeltaR);
        fhSingleTrackHitDeltaT->Fill(dDeltaT);
        fhSingleTrackHitPullX->Fill(dDeltaX / dErrX);
        fhSingleTrackHitPullY->Fill(dDeltaY / dErrY);
        fhSingleTrackHitPullZ->Fill(
          dDeltaZ / dErrZ);  // Offset(s) bec. hit assigned to middle of det.
        fhSingleTrackHitPullR->Fill(dDeltaR / dErrR);
        fhSingleTrackHitPullT->Fill(dDeltaT / dErrT);

        if (1 < uNbPointsInHit) {
          fhSingTrkMultiPntHitDeltaX->Fill(dDeltaX);
          fhSingTrkMultiPntHitDeltaY->Fill(dDeltaY);
          fhSingTrkMultiPntHitDeltaZ->Fill(dDeltaZ);
          fhSingTrkMultiPntHitDeltaR->Fill(dDeltaR);
          fhSingTrkMultiPntHitDeltaT->Fill(
            1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime()));
          fhSingTrkMultiPntHitPullX->Fill(dDeltaX / dErrX);
          fhSingTrkMultiPntHitPullY->Fill(dDeltaY / dErrY);
          fhSingTrkMultiPntHitPullZ->Fill(
            dDeltaZ / dErrZ);  // Offset(s) bec. hit assigned to middle of det.
          fhSingTrkMultiPntHitPullR->Fill(dDeltaR / dErrR);
          fhSingTrkMultiPntHitPullT->Fill(dDeltaT / dErrT);
        }  // if( 1 < uNbPointsInHit )

        // Get the MC track and fill its length up to the TOF point in the proper histo
        pMcTrk = (CbmMCTrack*) fMcTracksColl->At(pTofPoint->GetTrackID());

        Int_t iPdgCode = pMcTrk->GetPdgCode();
        Int_t iPartIdx = -1;
        for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
          if (kiPartPdgCode[iPart] == iPdgCode) {
            iPartIdx = iPart;
            break;
          }  // if( kiPartPdgCode[iPart] == iPdgCode )
        if (-1 == iPartIdx) iPartIdx = 0;
        if (-1 == pMcTrk->GetMotherId())
          fhMcTrkStartPrimSingTrk->Fill(pMcTrk->GetStartZ(), iPartIdx);
        else
          fhMcTrkStartSecSingTrk->Fill(pMcTrk->GetStartZ(), iPartIdx);
      }  // if( 1 == vTofTracksId.size() )
         // For Hits with multiple MC Tracks
      else {
        iNbTofHitsMultTrk++;

        fhHitMapMultTrkXY->Fill(dX, dY);
        fhHitMapMultTrkXZ->Fill(dX, dZ);
        fhHitMapMultTrkYZ->Fill(dY, dZ);
        fhHitMapMultTrkAng->Fill(dThetaX, dThetaY);
        fhHitMapMultTrkSph->Fill(dTheta, dPhi);

        // Check Hit Quality relative to mean of all MC Tracks
        dTrkMeanPosX /= vTofTracksId.size();
        dTrkMeanPosY /= vTofTracksId.size();
        dTrkMeanPosZ /= vTofTracksId.size();
        dTrkMeanTime /= vTofTracksId.size();

        Double_t dDeltaMeanX = dX - dTrkMeanPosX;
        Double_t dDeltaMeanY = dY - dTrkMeanPosY;
        Double_t dDeltaMeanZ = dZ - dTrkMeanPosZ;
        Double_t dDeltaMeanR =
          TMath::Sqrt(dDeltaMeanX * dDeltaMeanX + dDeltaMeanY * dDeltaMeanY);
        Double_t dDeltaMeanT = 1000.0 * (pTofHit->GetTime() - dTrkMeanTime);

        fhMultiTrkHitMeanDeltaX->Fill(dDeltaMeanX, uNbTracksInHit);
        fhMultiTrkHitMeanDeltaY->Fill(dDeltaMeanY, uNbTracksInHit);
        fhMultiTrkHitMeanDeltaZ->Fill(dDeltaMeanZ, uNbTracksInHit);
        fhMultiTrkHitMeanDeltaR->Fill(dDeltaMeanR, uNbTracksInHit);
        fhMultiTrkHitMeanDeltaT->Fill(dDeltaMeanT, uNbTracksInHit);
        fhMultiTrkHitMeanPullX->Fill(dDeltaMeanX / dErrX, uNbTracksInHit);
        fhMultiTrkHitMeanPullY->Fill(dDeltaMeanY / dErrY, uNbTracksInHit);
        fhMultiTrkHitMeanPullZ->Fill(
          dDeltaMeanZ / dErrZ,
          uNbTracksInHit);  // Offset(s) bec. hit assigned to middle of det.
        fhMultiTrkHitMeanPullR->Fill(dDeltaMeanR / dErrR, uNbTracksInHit);
        fhMultiTrkHitMeanPullT->Fill(dDeltaMeanT / dErrT, uNbTracksInHit);

        // Check Hit Quality relative to closest MC Tracks
        // Obtain Point position (Consider 1st Pnt of each Trk is approximate coord)
        //               pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iClosestTrkIdx);
        if (kTRUE == fbRealPointAvail)
          pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iClosestTrkIdx);
        else
          pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iClosestTrkIdx);

        TVector3 vPntPosClo;
        pTofPoint->Position(vPntPosClo);

        Double_t dDeltaCloX = dX - vPntPosClo.X();
        Double_t dDeltaCloY = dY - vPntPosClo.Y();
        Double_t dDeltaCloZ = dZ - vPntPosClo.Z();
        Double_t dDeltaCloR =
          TMath::Sqrt(dDeltaCloX * dDeltaCloX + dDeltaCloY * dDeltaCloY);
        Double_t dDeltaCloT =
          1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

        fhMultiTrkHitClosestDeltaX->Fill(dDeltaCloX, uNbTracksInHit);
        fhMultiTrkHitClosestDeltaY->Fill(dDeltaCloY, uNbTracksInHit);
        fhMultiTrkHitClosestDeltaZ->Fill(dDeltaCloZ, uNbTracksInHit);
        fhMultiTrkHitClosestDeltaR->Fill(dDeltaCloR, uNbTracksInHit);
        fhMultiTrkHitClosestDeltaT->Fill(dDeltaCloT, uNbTracksInHit);
        fhMultiTrkHitClosestPullX->Fill(dDeltaCloX / dErrX, uNbTracksInHit);
        fhMultiTrkHitClosestPullY->Fill(dDeltaCloY / dErrY, uNbTracksInHit);
        fhMultiTrkHitClosestPullZ->Fill(
          dDeltaCloZ / dErrZ,
          uNbTracksInHit);  // Offset(s) bec. hit assigned to middle of det.
        fhMultiTrkHitClosestPullR->Fill(dDeltaCloR / dErrR, uNbTracksInHit);
        fhMultiTrkHitClosestPullT->Fill(dDeltaCloT / dErrT, uNbTracksInHit);

        // Check Hit Quality relative to furthest MC Tracks
        // Obtain Point position (Consider 1st Pnt of each Trk is approximate coord)
        //               pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iFurthestTrkIdx);
        if (kTRUE == fbRealPointAvail)
          pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iFurthestTrkIdx);
        else
          pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iFurthestTrkIdx);

        TVector3 vPntPosFar;
        pTofPoint->Position(vPntPosFar);

        Double_t dDeltaFarX = dX - vPntPosFar.X();
        Double_t dDeltaFarY = dY - vPntPosFar.Y();
        Double_t dDeltaFarZ = dZ - vPntPosFar.Z();
        Double_t dDeltaFarR =
          TMath::Sqrt(dDeltaFarX * dDeltaFarX + dDeltaFarY * dDeltaFarY);
        Double_t dDeltaFarT =
          1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

        fhMultiTrkHitFurthestDeltaX->Fill(dDeltaFarX, uNbTracksInHit);
        fhMultiTrkHitFurthestDeltaY->Fill(dDeltaFarY, uNbTracksInHit);
        fhMultiTrkHitFurthestDeltaZ->Fill(dDeltaFarZ, uNbTracksInHit);
        fhMultiTrkHitFurthestDeltaR->Fill(dDeltaFarR, uNbTracksInHit);
        fhMultiTrkHitFurthestDeltaT->Fill(dDeltaFarT, uNbTracksInHit);
        fhMultiTrkHitFurthestPullX->Fill(dDeltaFarX / dErrX, uNbTracksInHit);
        fhMultiTrkHitFurthestPullY->Fill(dDeltaFarY / dErrY, uNbTracksInHit);
        fhMultiTrkHitFurthestPullZ->Fill(
          dDeltaFarZ / dErrZ,
          uNbTracksInHit);  // Offset(s) bec. hit assigned to middle of det.
        fhMultiTrkHitFurthestPullR->Fill(dDeltaFarR / dErrR, uNbTracksInHit);
        fhMultiTrkHitFurthestPullT->Fill(dDeltaFarT / dErrT, uNbTracksInHit);

        // Check Quality to best Track match (highest sum of TOT)
        // Do not fill it in Hit producer case as both strip ends receive a same weigth of 1/2
        if (kFALSE == fbHitProducerSource) {
          // Loop on tracks to find the one with the highest weight
          UInt_t uBestTrackIdx      = 0;
          Double_t dBestTrackWeight = -1.0;
          for (UInt_t uTrkIdx = 0; uTrkIdx < vTofTracksId.size(); uTrkIdx++)
            if (dBestTrackWeight < vTofTracksWeight[uTrkIdx]) {
              uBestTrackIdx    = uTrkIdx;
              dBestTrackWeight = vTofTracksWeight[uTrkIdx];
            }  // if( dBestTrackWeight < vTofTracksWeight[uTrkIdx] )

          // Obtain Point position (Consider 1st Pnt of each Trk is approximate coord)
          //                  pTofPoint = (CbmTofPoint*) fTofPointsColl->At( vTofTracksFirstPntId[uBestTrackIdx] );
          if (kTRUE == fbRealPointAvail)
            pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(
              vTofTracksFirstPntId[uBestTrackIdx]);
          else
            pTofPoint = (CbmTofPoint*) fTofPointsColl->At(
              vTofTracksFirstPntId[uBestTrackIdx]);

          TVector3 vPntPosBest;
          pTofPoint->Position(vPntPosBest);

          Double_t dDeltaBestX = dX - vPntPosBest.X();
          Double_t dDeltaBestY = dY - vPntPosBest.Y();
          Double_t dDeltaBestZ = dZ - vPntPosBest.Z();
          Double_t dDeltaBestR =
            TMath::Sqrt(dDeltaBestX * dDeltaBestX + dDeltaBestY * dDeltaBestY);
          Double_t dDeltaBestT =
            1000.0 * (pTofHit->GetTime() - pTofPoint->GetTime());

          fhMultiTrkHitBestDeltaX->Fill(dDeltaBestX, uNbTracksInHit);
          fhMultiTrkHitBestDeltaY->Fill(dDeltaBestY, uNbTracksInHit);
          fhMultiTrkHitBestDeltaZ->Fill(dDeltaBestZ, uNbTracksInHit);
          fhMultiTrkHitBestDeltaR->Fill(dDeltaBestR, uNbTracksInHit);
          fhMultiTrkHitBestDeltaT->Fill(dDeltaBestT, uNbTracksInHit);
          fhMultiTrkHitBestPullX->Fill(dDeltaBestX / dErrX, uNbTracksInHit);
          fhMultiTrkHitBestPullY->Fill(dDeltaBestY / dErrY, uNbTracksInHit);
          fhMultiTrkHitBestPullZ->Fill(
            dDeltaBestZ / dErrZ,
            uNbTracksInHit);  // Offset(s) bec. hit assigned to middle of det.
          fhMultiTrkHitBestPullR->Fill(dDeltaBestR / dErrR, uNbTracksInHit);
          fhMultiTrkHitBestPullT->Fill(dDeltaBestT / dErrT, uNbTracksInHit);
        }  // if( kFALSE == fbHitProducerSource )

        // Get each  MC track and fill its length up to the 1at TOF point in the proper histo
        for (UInt_t uTrkIdx = 0; uTrkIdx < vTofTracksId.size(); uTrkIdx++) {
          //                  pTofPoint = (CbmTofPoint*) fTofPointsColl->At( vTofTracksFirstPntId[uTrkIdx] );
          if (kTRUE == fbRealPointAvail)
            pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(
              vTofTracksFirstPntId[uTrkIdx]);
          else
            pTofPoint =
              (CbmTofPoint*) fTofPointsColl->At(vTofTracksFirstPntId[uTrkIdx]);

          pMcTrk = (CbmMCTrack*) fMcTracksColl->At(pTofPoint->GetTrackID());

          Int_t iPdgCode = pMcTrk->GetPdgCode();
          Int_t iPartIdx = -1;
          for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
            if (kiPartPdgCode[iPart] == iPdgCode) {
              iPartIdx = iPart;
              break;
            }  // if( kiPartPdgCode[iPart] == iPdgCode )
          if (-1 == iPartIdx) iPartIdx = 0;
          if (-1 == pMcTrk->GetMotherId())
            fhMcTrkStartPrimMultiTrk->Fill(pMcTrk->GetStartZ(), iPartIdx);
          else
            fhMcTrkStartSecMultiTrk->Fill(pMcTrk->GetStartZ(), iPartIdx);
        }  // for( UInt_t uTrkIdx = 0; uTrkIdx < vTofTracksId.size(); uTrkIdx++)
      }    // else of if( 1 == vTofTracksId.size() )

      // Physics coord mapping, 1 per particle type
      /*
            // Do as in CbmHadronAna: Take the MC Point of the first Digi matched with the hit
         pMatchHitDigi = (CbmMatch*) fTofDigiMatchColl->At( iHitInd );
            // Get index of first digi
         CbmLink lDigi    = pMatchHitDigi->GetLink(0);
         Int_t   iDigiIdx = lDigi.GetIndex();
         if( iNbTofDigis <= iDigiIdx )
         {
            LOG(error)<<"CbmTofHitFinderQa::FillHistos => Digi index from Hit #"
            <<iHitInd<<" in event #"<<fEvents
            <<" is bigger than nb entries in Digis arrays => ignore it!!!";
            continue;
         } // if( iNbTofDigis <= iDigiIdx )
            // Get pointer on match of first digi
         pMatchDigiPnt = (CbmMatch*) fTofDigiMatchPointsColl->At( iDigiIdx );
            // Get index of tof point corresponding to the first digi
         CbmLink lPt    = pMatchDigiPnt->GetMatchedLink();
*/
      // Get point with the best match (highest weight), in HitProd this returns the left Pnt
      CbmLink lPt  = pMatchHitPnt->GetMatchedLink();
      Int_t iPtIdx = lPt.GetIndex();
      // Get index of corresponding MC track
      Int_t iTrkId = ((CbmTofPoint*) fTofPointsColl->At(iPtIdx))->GetTrackID();
      if (kTRUE == fbRealPointAvail) {
        // Always only one mean MC Point Index per MC TofPoint
        // Weight always 1.0 so just read the index
        Int_t iRealPntIdx =
          (((CbmMatch*) fRealTofMatchColl->At(iPtIdx))->GetLink(0)).GetIndex();
        iTrkId =
          ((CbmTofPoint*) fRealTofPointsColl->At(iRealPntIdx))->GetTrackID();
        iPtIdx = iRealPntIdx;
      }  // if( kTRUE == fbRealPointAvail )

      // Get a pointer to the corresponding MC Track
      pMcTrk = (CbmMCTrack*) fMcTracksColl->At(iTrkId);

      Int_t iPdgCode = pMcTrk->GetPdgCode();
      Int_t iPartIdx = -1;
      for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
        if (kiPartPdgCode[iPart] == iPdgCode) {
          iPartIdx = iPart;
          break;
        }  // if( kiPartPdgCode[iPart] == iPdgCode )
      if (-1 == iPartIdx) iPartIdx = 0;
      if (-1 == pMcTrk->GetMotherId()) {
        // primary track
        // Phase space
        fvhPtmRapTofHit[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                        pMcTrk->GetPt() / pMcTrk->GetMass());
        // PLab
        fvhPlabTofHit[iPartIdx]->Fill(pMcTrk->GetP());
        if (1 == vTofPointsId.size()) {
          fvhPtmRapTofHitSinglePnt[iPartIdx]->Fill(
            pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
          fvhPlabTofHitSinglePnt[iPartIdx]->Fill(pMcTrk->GetP());
        }  // if( 1 == vTofPointsId.size() )
        if (1 == vTofTracksId.size()) {
          fvhPtmRapTofHitSingleTrk[iPartIdx]->Fill(
            pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
          fvhPlabTofHitSingleTrk[iPartIdx]->Fill(pMcTrk->GetP());
        }  // if( 1 == vTofTracksId.size() )
      }    // if( -1 == pMcTrk->GetMotherId() )
      else {
        // secondary track
        // Phase space
        fvhPtmRapSecTofHit[iPartIdx]->Fill(pMcTrk->GetRapidity(),
                                           pMcTrk->GetPt() / pMcTrk->GetMass());
        // PLab
        fvhPlabSecTofHit[iPartIdx]->Fill(pMcTrk->GetP());
        if (1 == vTofPointsId.size()) {
          fvhPtmRapSecTofHitSinglePnt[iPartIdx]->Fill(
            pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
          fvhPlabSecTofHitSinglePnt[iPartIdx]->Fill(pMcTrk->GetP());
        }  // if( 1 == vTofPointsId.size() )
        if (1 == vTofTracksId.size()) {
          fvhPtmRapSecTofHitSingleTrk[iPartIdx]->Fill(
            pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
          fvhPlabSecTofHitSingleTrk[iPartIdx]->Fill(pMcTrk->GetP());
        }  // if( 1 == vTofTracksId.size() )
      }    // else of if( -1 == pMcTrk->GetMotherId() )
      // clear storage of point and track IDs
      vTofPointsId.clear();
      vTofTracksId.clear();
    }  // if( kFALSE == fbNormHistGenMode )
  }    // for( Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++ )

  // Nb Hits per event for first N events
  fhNbHitsPerEvent->Fill(fEvents, iNbTofHits);
  fhNbHitsSingPntPerEvent->Fill(fEvents, iNbTofHitsSingPnt);
  fhNbHitsMultPntPerEvent->Fill(fEvents, iNbTofHitsMultPnt);
  fhNbHitsSingTrkPerEvent->Fill(fEvents, iNbTofHitsSingTrk);
  fhNbHitsMultTrkPerEvent->Fill(fEvents, iNbTofHitsMultTrk);

  // MC Tracks losses
  for (Int_t iTrkInd = 0; iTrkInd < iNbTracks; iTrkInd++) {
    pMcTrk = (CbmMCTrack*) fMcTracksColl->At(iTrkInd);

    if (kTRUE == vbTrackHasHit[iTrkInd]) {
      // Physics coord mapping, 1 per particle type
      Int_t iPdgCode = pMcTrk->GetPdgCode();
      Int_t iPartIdx = -1;
      for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
        if (kiPartPdgCode[iPart] == iPdgCode) {
          iPartIdx = iPart;
          break;
        }  // if( kiPartPdgCode[iPart] == iPdgCode )
      if (-1 == iPartIdx) iPartIdx = 0;
      if (-1 == pMcTrk->GetMotherId()) {
        // primary track
        fvhPtmRapGenTrkTofHit[iPartIdx]->Fill(
          pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabGenTrkTofHit[iPartIdx]->Fill(pMcTrk->GetP());

        if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts))
          fvhPlabStsTrkTofHit[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( -1 == pMcTrk->GetMotherId() )
      else {
        // secondary track
        fvhPtmRapSecGenTrkTofHit[iPartIdx]->Fill(
          pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabSecGenTrkTofHit[iPartIdx]->Fill(pMcTrk->GetP());

        if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts))
          fvhPlabSecStsTrkTofHit[iPartIdx]->Fill(pMcTrk->GetP());
      }  // else of if( -1 == pMcTrk->GetMotherId() )
    }    // if( kTRUE == vbTrackHasHit[iTrkId] )
  }      // for(Int_t iTrkInd = 0; iTrkInd < nMcTracks; iTrkInd++)
  vbTrackHasHit.clear();

  // Integrated TofHit Efficiency
  if (0 < fvulIdxTracksWithPnt.size())
    fhIntegratedHitPntEff->Fill(
      100.0 * (static_cast<Double_t>(fvulIdxTracksWithHit.size()))
      / (static_cast<Double_t>(fvulIdxTracksWithPnt.size())));
  if (0 < fvulIdxPrimTracksWithPnt.size())
    fhIntegratedHitPntEffPrim->Fill(
      100.0 * (static_cast<Double_t>(fvulIdxPrimTracksWithHit.size()))
      / (static_cast<Double_t>(fvulIdxPrimTracksWithPnt.size())));
  if (0 < fvulIdxSecTracksWithPnt.size())
    fhIntegratedHitPntEffSec->Fill(
      100.0 * (static_cast<Double_t>(fvulIdxSecTracksWithHit.size()))
      / (static_cast<Double_t>(fvulIdxSecTracksWithPnt.size())));

  // Integrated TofHit Efficiency: Tracks firing channel but not going to Digi/Hit
  if (0 < fvulIdxTracksWithPnt.size())
    fhIntegratedHiddenHitPntLoss->Fill(
      100.0 * (static_cast<Double_t>(fvulIdxHiddenTracksWithHit.size()))
      / (static_cast<Double_t>(fvulIdxTracksWithPnt.size())));
  if (0 < fvulIdxPrimTracksWithPnt.size())
    fhIntegratedHiddenHitPntLossPrim->Fill(
      100.0 * (static_cast<Double_t>(fvulIdxHiddenPrimTracksWithHit.size()))
      / (static_cast<Double_t>(fvulIdxPrimTracksWithPnt.size())));
  if (0 < fvulIdxSecTracksWithPnt.size())
    fhIntegratedHiddenHitPntLossSec->Fill(
      100.0 * (static_cast<Double_t>(fvulIdxHiddenSecTracksWithHit.size()))
      / (static_cast<Double_t>(fvulIdxSecTracksWithPnt.size())));

  LOG(debug2) << "CbmTofHitFinderQa::FillHistos => nb prim trk w/ pnt: "
              << fvulIdxPrimTracksWithPnt.size()
              << " nb prim trk w/ hit:  " << fvulIdxPrimTracksWithHit.size()
              << " ";
  // Efficiency dependence on nb crossed gaps
  TString sHead =
    "CbmTofHitFinderQa::FillHistos =>                N pnt:       ";
  TString sPnt =
    "CbmTofHitFinderQa::FillHistos => nb prim trk w/ N pnt:       ";
  TString sHit =
    "CbmTofHitFinderQa::FillHistos => nb prim trk w/ N pnt & hit: ";
  for (UInt_t uNbGaps = 0; uNbGaps < fuMaxCrossedGaps; uNbGaps++) {
    sHead += Form("%4u ", uNbGaps);
    sPnt += Form("%4lu ", fvulIdxPrimTracksWithPntGaps[uNbGaps].size());
    sHit += Form("%4lu ", fvulIdxPrimTracksWithHitGaps[uNbGaps].size());

    if (0 < fvulIdxTracksWithPntGaps[uNbGaps].size())
      fhIntegratedHitPntEffGaps->Fill(
        100.0
          * (static_cast<Double_t>(fvulIdxTracksWithHitGaps[uNbGaps].size()))
          / (static_cast<Double_t>(fvulIdxTracksWithPntGaps[uNbGaps].size())),
        uNbGaps + 1);
    if (0 < fvulIdxPrimTracksWithPntGaps[uNbGaps].size())
      fhIntegratedHitPntEffPrimGaps->Fill(
        100.0
          * (static_cast<Double_t>(
            fvulIdxPrimTracksWithHitGaps[uNbGaps].size()))
          / (static_cast<Double_t>(
            fvulIdxPrimTracksWithPntGaps[uNbGaps].size())),
        uNbGaps + 1);
    if (0 < fvulIdxSecTracksWithPntGaps[uNbGaps].size())
      fhIntegratedHitPntEffSecGaps->Fill(
        100.0
          * (static_cast<Double_t>(fvulIdxSecTracksWithHitGaps[uNbGaps].size()))
          / (static_cast<Double_t>(
            fvulIdxSecTracksWithPntGaps[uNbGaps].size())),
        uNbGaps + 1);
  }  // for( UInt_t uNbGaps = 0; uNbGaps < fuMaxCrossedGaps; uNbGaps++)
  LOG(debug2) << sHead;
  LOG(debug2) << sPnt;
  LOG(debug2) << sHit;

  return kTRUE;
}
// ------------------------------------------------------------------

Bool_t CbmTofHitFinderQa::SetHistoFileNameCartCoordNorm(TString sFilenameIn) {
  fsHistoInNormCartFilename = sFilenameIn;
  return kTRUE;
}
Bool_t CbmTofHitFinderQa::SetHistoFileNameAngCoordNorm(TString sFilenameIn) {
  fsHistoInNormAngFilename = sFilenameIn;
  return kTRUE;
}
Bool_t CbmTofHitFinderQa::SetHistoFileNameSphCoordNorm(TString sFilenameIn) {
  fsHistoInNormSphFilename = sFilenameIn;
  return kTRUE;
}
Bool_t CbmTofHitFinderQa::NormalizeMapHistos() {
  // TIP: Normalization files can (should?) be generated using the FairBoxGenerator
  //      to get uniform distributions with the right density in the desired coords.
  // Normalization of Cartesian coordinates maps
  if ("" != fsHistoInNormCartFilename) {
    // Open file and set folders properly to avoid creating objects in uncontrolled places
    TDirectory* oldir = gDirectory;
    TFile* fHistNorm  = new TFile(fsHistoInNormCartFilename, "READ");
    if (NULL == fHistNorm) {
      LOG(error)
        << "CbmTofHitFinderQa::NormalizeMapHistos => Could not open file"
        << " with Normalization histos of Cartesian coordinates maps with name "
        << fsHistoInNormCartFilename << " => Stopping normalization here!";
      return kFALSE;
    }  // if( NULL == fHistNorm )
    gROOT->cd();

    // Prepare pointers to access normalization histos
    TH2* fhPointMapNormXY = NULL;
    TH2* fhPointMapNormXZ = NULL;
    TH2* fhPointMapNormYZ = NULL;
    TH2* fhDigiMapNormXY  = NULL;
    TH2* fhDigiMapNormXZ  = NULL;
    TH2* fhDigiMapNormYZ  = NULL;
    TH2* fhHitMapNormXY   = NULL;
    TH2* fhHitMapNormXZ   = NULL;
    TH2* fhHitMapNormYZ   = NULL;

    // Get pointers for norm histos from file if they exist
    fhPointMapNormXY =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_PointsMapXY"));
    fhPointMapNormXZ =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_PointsMapXZ"));
    fhPointMapNormYZ =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_PointsMapYZ"));

    fhDigiMapNormXY =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_DigisMapXY"));
    fhDigiMapNormXZ =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_DigisMapXZ"));
    fhDigiMapNormYZ =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_DigisMapYZ"));

    fhHitMapNormXY =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_HitsMapXY"));
    fhHitMapNormXZ =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_HitsMapXZ"));
    fhHitMapNormYZ =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_HitsMapYZ"));
    LOG(info) << "CbmTofHitFinderQa::NormalizeMapHistos => XY norm histos: "
              << fhPointMapNormXY << " " << fhDigiMapNormXY << " "
              << fhHitMapNormXY << " ";
    LOG(info) << "CbmTofHitFinderQa::NormalizeMapHistos => XZ norm histos: "
              << fhPointMapNormXZ << " " << fhDigiMapNormXZ << " "
              << fhHitMapNormXZ << " ";
    LOG(info) << "CbmTofHitFinderQa::NormalizeMapHistos => YZ norm histos: "
              << fhPointMapNormYZ << " " << fhDigiMapNormYZ << " "
              << fhHitMapNormYZ << " ";

    // If norm histo found, normalize corresponding map
    if (NULL != fhPointMapNormXY) {
      fhPointMapXY->Divide(fhPointMapNormXY);
    }  // if( NULL != fhPointMapNormXY )
    if (NULL != fhPointMapNormXZ) {
      fhPointMapXZ->Divide(fhPointMapNormXZ);
    }  // if( NULL != fhPointMapNormXZ )
    if (NULL != fhPointMapNormYZ) {
      fhPointMapYZ->Divide(fhPointMapNormYZ);
    }  // if( NULL != fhPointMapNormYZ )
    if (NULL != fhDigiMapNormXY) {
      fhDigiMapXY->Divide(fhDigiMapNormXY);
    }  // if( NULL != fhDigiMapNormXY  )
    if (NULL != fhDigiMapNormXZ) {
      fhDigiMapXZ->Divide(fhDigiMapNormXZ);
    }  // if( NULL != fhDigiMapNormXZ  )
    if (NULL != fhDigiMapNormYZ) {
      fhDigiMapYZ->Divide(fhDigiMapNormYZ);
    }  // if( NULL != fhDigiMapNormYZ  )
    if (NULL != fhHitMapNormXY) {
      fhHitMapXY->Divide(fhHitMapNormXY);
      fhHitMapSingPntXY->Divide(fhHitMapNormXY);
      fhHitMapMultPntXY->Divide(fhHitMapNormXY);
      fhHitMapSingTrkXY->Divide(fhHitMapNormXY);
      fhHitMapMultTrkXY->Divide(fhHitMapNormXY);
    }  // if( NULL != fhHitMapNormXY   )
    if (NULL != fhHitMapNormXZ) {
      fhHitMapXZ->Divide(fhHitMapNormXZ);
      fhHitMapSingPntXZ->Divide(fhHitMapNormXZ);
      fhHitMapMultPntXZ->Divide(fhHitMapNormXZ);
      fhHitMapSingTrkXZ->Divide(fhHitMapNormXZ);
      fhHitMapMultTrkXZ->Divide(fhHitMapNormXZ);
    }  // if( NULL != fhHitMapNormXZ   )
    if (NULL != fhHitMapNormYZ) {
      fhHitMapYZ->Divide(fhHitMapNormYZ);
      fhHitMapSingPntYZ->Divide(fhHitMapNormYZ);
      fhHitMapMultPntYZ->Divide(fhHitMapNormYZ);
      fhHitMapSingTrkYZ->Divide(fhHitMapNormYZ);
      fhHitMapMultTrkYZ->Divide(fhHitMapNormYZ);
    }  // if( NULL != fhHitMapNormYZ   )

    // Go back to original folder, whereever it is
    gDirectory->cd(oldir->GetPath());

    fHistNorm->Close();
  }  // if( "" != fsHistoInNormCartFilename )
  else
    LOG(info)
      << "CbmTofHitFinderQa::NormalizeMapHistos => Name of input file for"
      << " Normalization of Cartesian coordinates maps not given"
      << " => no normalization for these maps!";
  // Normalization of Angular   coordinates maps
  if ("" != fsHistoInNormAngFilename) {
    // Open file and set folders properly to avoid creating objects in uncontrolled places
    TDirectory* oldir = gDirectory;
    TFile* fHistNorm  = new TFile(fsHistoInNormAngFilename, "READ");
    if (NULL == fHistNorm) {
      LOG(error)
        << "CbmTofHitFinderQa::NormalizeMapHistos => Could not open file"
        << " with Normalization histos of Angular   coordinates maps with name "
        << fsHistoInNormAngFilename << " => Stopping normalization here!";
      return kFALSE;
    }  // if( NULL == fHistNorm )
    gROOT->cd();

    // Prepare pointers to access normalization histos
    TH2* fhPointMapNormAng = NULL;
    TH2* fhDigiMapNormAng  = NULL;
    TH2* fhHitMapNormAng   = NULL;

    // Get pointers for norm histos from file if they exist
    fhPointMapNormAng =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_PointsMapAng"));
    fhDigiMapNormAng =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_DigisMapAng"));
    fhHitMapNormAng =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_HitsMapAng"));
    LOG(info)
      << "CbmTofHitFinderQa::NormalizeMapHistos => Angular norm histos: "
      << fhPointMapNormAng << " " << fhDigiMapNormAng << " " << fhHitMapNormAng
      << " ";

    // If norm histo found, normalize corresponding map
    if (NULL != fhPointMapNormAng) fhPointMapAng->Divide(fhPointMapNormAng);
    if (NULL != fhDigiMapNormAng) fhDigiMapAng->Divide(fhDigiMapNormAng);
    if (NULL != fhHitMapNormAng) {
      fhHitMapAng->Divide(fhHitMapNormAng);
      fhHitMapSingPntAng->Divide(fhHitMapNormAng);
      fhHitMapMultPntAng->Divide(fhHitMapNormAng);
      fhHitMapSingTrkAng->Divide(fhHitMapNormAng);
      fhHitMapMultTrkAng->Divide(fhHitMapNormAng);
    }

    // Go back to original folder, whereever it is
    gDirectory->cd(oldir->GetPath());

    fHistNorm->Close();
  }  // if( "" != fsHistoInNormAngFilename )
  else
    LOG(info)
      << "CbmTofHitFinderQa::NormalizeMapHistos => Name of input file for"
      << " Normalization of Angular   coordinates maps not given"
      << " => no normalization for these maps!";
  // Normalization of Spherical coordinates maps
  if ("" != fsHistoInNormSphFilename) {
    // Open file and set folders properly to avoid creating objects in uncontrolled places
    TDirectory* oldir = gDirectory;
    TFile* fHistNorm  = new TFile(fsHistoInNormSphFilename, "READ");
    if (NULL == fHistNorm) {
      LOG(error)
        << "CbmTofHitFinderQa::NormalizeMapHistos => Could not open file"
        << " with Normalization histos of Spherical coordinates maps with name "
        << fsHistoInNormSphFilename << " => Stopping normalization here!";
      return kFALSE;
    }  // if( NULL == fHistNorm )
    gROOT->cd();

    // Prepare pointers to access normalization histos
    TH2* fhPointMapNormSph = NULL;
    TH2* fhDigiMapNormSph  = NULL;
    TH2* fhHitMapNormSph   = NULL;

    // Get pointers for norm histos from file if they exist
    fhPointMapNormSph =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_PointsMapSph"));
    fhDigiMapNormSph =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_DigisMapSph"));
    fhHitMapNormSph =
      static_cast<TH2*>(fHistNorm->FindObjectAny("TofTests_HitsMapSph"));
    LOG(info)
      << "CbmTofHitFinderQa::NormalizeMapHistos => Spherical norm histos: "
      << fhPointMapNormSph << " " << fhDigiMapNormSph << " " << fhHitMapNormSph
      << " ";

    // If norm histo found, normalize corresponding map
    if (NULL != fhPointMapNormSph) fhPointMapSph->Divide(fhPointMapNormSph);
    if (NULL != fhDigiMapNormSph) fhDigiMapSph->Divide(fhDigiMapNormSph);
    if (NULL != fhHitMapNormSph) {
      fhHitMapSph->Divide(fhHitMapNormSph);
      fhHitMapSingPntSph->Divide(fhHitMapNormSph);
      fhHitMapMultPntSph->Divide(fhHitMapNormSph);
      fhHitMapSingTrkSph->Divide(fhHitMapNormSph);
      fhHitMapMultTrkSph->Divide(fhHitMapNormSph);
    }

    // Go back to original folder, whereever it is
    gDirectory->cd(oldir->GetPath());

    fHistNorm->Close();
  }  // if( "" != fsHistoInNormSphFilename )
  else
    LOG(info)
      << "CbmTofHitFinderQa::NormalizeMapHistos => Name of input file for"
      << " Normalization of Spherical coordinates maps not given"
      << " => no normalization for these maps!";
  return kTRUE;
}
Bool_t CbmTofHitFinderQa::NormalizeNormHistos() {
  if (0 < fhTrackMapXY->GetEntries()) {
    fhPointMapXY->Divide(fhTrackMapXY);
    fhDigiMapXY->Divide(fhTrackMapXY);
    fhHitMapXY->Divide(fhTrackMapXY);
  }  // if( 0 < fhTrackMapXY->GetEntries())
  if (0 < fhTrackMapXZ->GetEntries()) {
    fhPointMapXZ->Divide(fhTrackMapXZ);
    fhDigiMapXZ->Divide(fhTrackMapXZ);
    fhHitMapXZ->Divide(fhTrackMapXZ);
  }  // if( 0 < fhTrackMapXZ->GetEntries())
  if (0 < fhTrackMapYZ->GetEntries()) {
    fhPointMapYZ->Divide(fhTrackMapYZ);
    fhDigiMapYZ->Divide(fhTrackMapYZ);
    fhHitMapYZ->Divide(fhTrackMapYZ);
  }  // if( 0 < fhTrackMapYZ->GetEntries())
  if (0 < fhTrackMapAng->GetEntries()) {
    fhPointMapAng->Divide(fhTrackMapAng);
    fhDigiMapAng->Divide(fhTrackMapAng);
    fhHitMapAng->Divide(fhTrackMapAng);
  }  // if( 0 < fhTrackMapAng->GetEntries())
  if (0 < fhTrackMapSph->GetEntries()) {
    fhPointMapSph->Divide(fhTrackMapSph);
    fhDigiMapSph->Divide(fhTrackMapSph);
    fhHitMapSph->Divide(fhTrackMapSph);
  }  // if( 0 < fhTrackMapSph->GetEntries())

  return kTRUE;
}
// ------------------------------------------------------------------

Bool_t CbmTofHitFinderQa::SetHistoFileName(TString sFilenameIn) {
  fsHistoOutFilename = sFilenameIn;
  return kTRUE;
}
Bool_t CbmTofHitFinderQa::WriteHistos() {
  // TODO: add sub-folders ?

  // Write histogramms to the file
  TDirectory* oldir = gDirectory;
  TFile* fHist      = new TFile(fsHistoOutFilename, "RECREATE");
  fHist->cd();

  // Nb Hits per event for first N events
  fhNbHitsPerEvent->Write();
  fhNbHitsSingPntPerEvent->Write();
  fhNbHitsMultPntPerEvent->Write();
  fhNbHitsSingTrkPerEvent->Write();
  fhNbHitsMultTrkPerEvent->Write();
  // Nb different TOF digis in Hit
  fhNbDigisInHit->Write();
  fhNbDigisInHitMapXY->Write();

  // Mapping
  TDirectory* cdGeomMap = fHist->mkdir("GeomMap");
  cdGeomMap->cd();  // make the "GeomMap" directory the current directory
  if (kTRUE == fbNormHistGenMode) {
    fhTrackMapXY->Write();
    fhTrackMapXZ->Write();
    fhTrackMapYZ->Write();
    fhTrackMapAng->Write();
    fhTrackMapSph->Write();
  }  // if( kTRUE == fbNormHistGenMode )
  fhPointMapXY->Write();
  fhPointMapXZ->Write();
  fhPointMapYZ->Write();
  fhPointMapAng->Write();
  fhPointMapSph->Write();
  fhRealPointMapXY->Write();
  fhRealPointMapXZ->Write();
  fhRealPointMapYZ->Write();
  fhRealPointMapAng->Write();
  fhRealPointMapSph->Write();
  fhDigiMapXY->Write();
  fhDigiMapXZ->Write();
  fhDigiMapYZ->Write();
  fhDigiMapAng->Write();
  fhDigiMapSph->Write();
  fhHitMapXY->Write();
  fhHitMapXZ->Write();
  fhHitMapYZ->Write();
  fhHitMapAng->Write();
  fhHitMapSph->Write();

  if (kFALSE == fbNormHistGenMode) {
    fHist->cd();  // make the file root the current directory
                  // L/R digis missmatch
    fhLeftRightDigiMatch->Write();
    // Nb different MC Points and Tracks in Hit
    fhNbPointsInHit->Write();
    fhNbTracksInHit->Write();

    TDirectory* cdStartZ = fHist->mkdir("StartZ");
    cdStartZ->cd();  // make the "MixMap" directory the current directory
                     // Dependence of Track origin on centrality
    for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
      fvhTrackAllStartZCent[iPartIdx]->Write();
      fvhTrackSecStartZCent[iPartIdx]->Write();
      if (2 == iPartIdx)  // 3D plot only for e-
        fvhTrackAllStartXZCent[iPartIdx]->Write();
      fvhTrackAllStartXZ[iPartIdx]->Write();
      fvhTrackAllStartYZ[iPartIdx]->Write();
      fvhTofPntAllAngCent[iPartIdx]->Write();
    }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

    TDirectory* cdMixMap = fHist->mkdir("MixMap");
    cdMixMap->cd();  // make the "MixMap" directory the current directory
      // Mapping of position for hits coming from a single MC Point
    fhHitMapSingPntXY->Write();
    fhHitMapSingPntXZ->Write();
    fhHitMapSingPntYZ->Write();
    fhHitMapSingPntAng->Write();
    fhHitMapSingPntSph->Write();
    // Mapping of position for hits coming from multiple MC Points
    fhHitMapMultPntXY->Write();
    fhHitMapMultPntXZ->Write();
    fhHitMapMultPntYZ->Write();
    fhHitMapMultPntAng->Write();
    fhHitMapMultPntSph->Write();
    // Mapping of position for hits coming from a single MC Track
    fhHitMapSingTrkXY->Write();
    fhHitMapSingTrkXZ->Write();
    fhHitMapSingTrkYZ->Write();
    fhHitMapSingTrkAng->Write();
    fhHitMapSingTrkSph->Write();
    // Mapping of position for hits coming from multiple MC Tracks
    fhHitMapMultTrkXY->Write();
    fhHitMapMultTrkXZ->Write();
    fhHitMapMultTrkYZ->Write();
    fhHitMapMultTrkAng->Write();
    fhHitMapMultTrkSph->Write();

    TDirectory* cdHitQual = fHist->mkdir("HitQual");
    cdHitQual->cd();  // make the "HitQual" directory the current directory
                      // Hit Quality for Hits coming from a single MC Point
    fhSinglePointHitDeltaX->Write();
    fhSinglePointHitDeltaY->Write();
    fhSinglePointHitDeltaZ->Write();
    fhSinglePointHitDeltaR->Write();
    fhSinglePointHitDeltaT->Write();
    fhSinglePointHitPullX->Write();
    fhSinglePointHitPullY->Write();
    fhSinglePointHitPullZ->Write();
    fhSinglePointHitPullR->Write();
    fhSinglePointHitPullT->Write();

    fhCltSzSinglePointHitPullX->Write();
    fhCltSzSinglePointHitPullY->Write();
    fhCltSzSinglePointHitPullZ->Write();
    fhCltSzSinglePointHitPullR->Write();
    fhCltSzSinglePointHitPullT->Write();
    // Hit Quality for Hits coming from a multiple MC Points
    // To Point closest to Hit
    fhMultiPntHitClosestDeltaX->Write();
    fhMultiPntHitClosestDeltaY->Write();
    fhMultiPntHitClosestDeltaZ->Write();
    fhMultiPntHitClosestDeltaR->Write();
    fhMultiPntHitClosestDeltaT->Write();
    fhMultiPntHitClosestPullX->Write();
    fhMultiPntHitClosestPullY->Write();
    fhMultiPntHitClosestPullZ->Write();
    fhMultiPntHitClosestPullR->Write();
    fhMultiPntHitClosestPullT->Write();
    // To Point furthest from Hit
    fhMultiPntHitFurthestDeltaX->Write();
    fhMultiPntHitFurthestDeltaY->Write();
    fhMultiPntHitFurthestDeltaZ->Write();
    fhMultiPntHitFurthestDeltaR->Write();
    fhMultiPntHitFurthestDeltaT->Write();
    fhMultiPntHitFurthestPullX->Write();
    fhMultiPntHitFurthestPullY->Write();
    fhMultiPntHitFurthestPullZ->Write();
    fhMultiPntHitFurthestPullR->Write();
    fhMultiPntHitFurthestPullT->Write();
    // To mean Point position
    fhMultiPntHitMeanDeltaX->Write();
    fhMultiPntHitMeanDeltaY->Write();
    fhMultiPntHitMeanDeltaZ->Write();
    fhMultiPntHitMeanDeltaR->Write();
    fhMultiPntHitMeanDeltaT->Write();
    fhMultiPntHitMeanPullX->Write();
    fhMultiPntHitMeanPullY->Write();
    fhMultiPntHitMeanPullZ->Write();
    fhMultiPntHitMeanPullR->Write();
    fhMultiPntHitMeanPullT->Write();
    // To best Point position
    fhMultiPntHitBestDeltaX->Write();
    fhMultiPntHitBestDeltaY->Write();
    fhMultiPntHitBestDeltaZ->Write();
    fhMultiPntHitBestDeltaR->Write();
    fhMultiPntHitBestDeltaT->Write();
    fhMultiPntHitBestPullX->Write();
    fhMultiPntHitBestPullY->Write();
    fhMultiPntHitBestPullZ->Write();
    fhMultiPntHitBestPullR->Write();
    fhMultiPntHitBestPullT->Write();
    // Hit Quality for Hits coming from a single MC Track
    fhSingleTrackHitDeltaX->Write();
    fhSingleTrackHitDeltaY->Write();
    fhSingleTrackHitDeltaZ->Write();
    fhSingleTrackHitDeltaR->Write();
    fhSingleTrackHitDeltaT->Write();
    fhSingleTrackHitPullX->Write();
    fhSingleTrackHitPullY->Write();
    fhSingleTrackHitPullZ->Write();
    fhSingleTrackHitPullR->Write();
    fhSingleTrackHitPullT->Write();
    // Hit Quality for Hits coming from a single MC Track but multiple points
    fhSingTrkMultiPntHitDeltaX->Write();
    fhSingTrkMultiPntHitDeltaY->Write();
    fhSingTrkMultiPntHitDeltaZ->Write();
    fhSingTrkMultiPntHitDeltaR->Write();
    fhSingTrkMultiPntHitDeltaT->Write();
    fhSingTrkMultiPntHitPullX->Write();
    fhSingTrkMultiPntHitPullY->Write();
    fhSingTrkMultiPntHitPullZ->Write();
    fhSingTrkMultiPntHitPullR->Write();
    fhSingTrkMultiPntHitPullT->Write();
    // Hit Quality for Hits coming from a multiple MC Points
    // To Track closest to Hit
    fhMultiTrkHitClosestDeltaX->Write();
    fhMultiTrkHitClosestDeltaY->Write();
    fhMultiTrkHitClosestDeltaZ->Write();
    fhMultiTrkHitClosestDeltaR->Write();
    fhMultiTrkHitClosestDeltaT->Write();
    fhMultiTrkHitClosestPullX->Write();
    fhMultiTrkHitClosestPullY->Write();
    fhMultiTrkHitClosestPullZ->Write();
    fhMultiTrkHitClosestPullR->Write();
    fhMultiTrkHitClosestPullT->Write();
    // To Track furthest from Hit
    fhMultiTrkHitFurthestDeltaX->Write();
    fhMultiTrkHitFurthestDeltaY->Write();
    fhMultiTrkHitFurthestDeltaZ->Write();
    fhMultiTrkHitFurthestDeltaR->Write();
    fhMultiTrkHitFurthestDeltaT->Write();
    fhMultiTrkHitFurthestPullX->Write();
    fhMultiTrkHitFurthestPullY->Write();
    fhMultiTrkHitFurthestPullZ->Write();
    fhMultiTrkHitFurthestPullR->Write();
    fhMultiTrkHitFurthestPullT->Write();
    // To mean Track position
    fhMultiTrkHitMeanDeltaX->Write();
    fhMultiTrkHitMeanDeltaY->Write();
    fhMultiTrkHitMeanDeltaZ->Write();
    fhMultiTrkHitMeanDeltaR->Write();
    fhMultiTrkHitMeanDeltaT->Write();
    fhMultiTrkHitMeanPullX->Write();
    fhMultiTrkHitMeanPullY->Write();
    fhMultiTrkHitMeanPullZ->Write();
    fhMultiTrkHitMeanPullR->Write();
    fhMultiTrkHitMeanPullT->Write();
    // To best Track position
    fhMultiTrkHitBestDeltaX->Write();
    fhMultiTrkHitBestDeltaY->Write();
    fhMultiTrkHitBestDeltaZ->Write();
    fhMultiTrkHitBestDeltaR->Write();
    fhMultiTrkHitBestDeltaT->Write();
    fhMultiTrkHitBestPullX->Write();
    fhMultiTrkHitBestPullY->Write();
    fhMultiTrkHitBestPullZ->Write();
    fhMultiTrkHitBestPullR->Write();
    fhMultiTrkHitBestPullT->Write();

    TDirectory* cdPhysMap = fHist->mkdir("PhysMap");
    cdPhysMap->cd();  // make the "PhysMap" directory the current directory
                      // Physics coord mapping, 1 per particle type
    for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
      // Phase space
      fvhPtmRapGenTrk[iPartIdx]->Write();
      fvhPtmRapStsPnt[iPartIdx]->Write();
      fvhPtmRapTofPnt[iPartIdx]->Write();
      fvhPtmRapTofHit[iPartIdx]->Write();
      fvhPtmRapTofHitSinglePnt[iPartIdx]->Write();
      fvhPtmRapTofHitSingleTrk[iPartIdx]->Write();
      // PLab
      fvhPlabGenTrk[iPartIdx]->Write();
      fvhPlabStsPnt[iPartIdx]->Write();
      fvhPlabTofPnt[iPartIdx]->Write();
      fvhPlabTofHit[iPartIdx]->Write();
      fvhPlabTofHitSinglePnt[iPartIdx]->Write();
      fvhPlabTofHitSingleTrk[iPartIdx]->Write();
      // MC Tracks losses
      fvhPtmRapGenTrkTofPnt[iPartIdx]->Write();
      fvhPtmRapGenTrkTofHit[iPartIdx]->Write();
      fvhPlabGenTrkTofPnt[iPartIdx]->Write();
      fvhPlabGenTrkTofHit[iPartIdx]->Write();
      fvhPlabStsTrkTofPnt[iPartIdx]->Write();
      fvhPlabStsTrkTofHit[iPartIdx]->Write();
    }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

    TDirectory* cdPhysMapSec = fHist->mkdir("PhysMapSec");
    cdPhysMapSec->cd();  // make the "PhysMap" directory the current directory
                         // Physics coord mapping, 1 per particle type
    for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
      // Phase space
      fvhPtmRapSecGenTrk[iPartIdx]->Write();
      fvhPtmRapSecStsPnt[iPartIdx]->Write();
      fvhPtmRapSecTofPnt[iPartIdx]->Write();
      fvhPtmRapSecTofHit[iPartIdx]->Write();
      fvhPtmRapSecTofHitSinglePnt[iPartIdx]->Write();
      fvhPtmRapSecTofHitSingleTrk[iPartIdx]->Write();
      // PLab
      fvhPlabSecGenTrk[iPartIdx]->Write();
      fvhPlabSecStsPnt[iPartIdx]->Write();
      fvhPlabSecTofPnt[iPartIdx]->Write();
      fvhPlabSecTofHit[iPartIdx]->Write();
      fvhPlabSecTofHitSinglePnt[iPartIdx]->Write();
      fvhPlabSecTofHitSingleTrk[iPartIdx]->Write();
      // MC Tracks losses
      fvhPtmRapSecGenTrkTofPnt[iPartIdx]->Write();
      fvhPtmRapSecGenTrkTofHit[iPartIdx]->Write();
      fvhPlabSecGenTrkTofPnt[iPartIdx]->Write();
      fvhPlabSecGenTrkTofHit[iPartIdx]->Write();
      fvhPlabSecStsTrkTofPnt[iPartIdx]->Write();
      fvhPlabSecStsTrkTofHit[iPartIdx]->Write();
    }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

    fHist->cd();
    fhIntegratedHitPntEff->Write();
    fhIntegratedHitPntEffPrim->Write();
    fhIntegratedHitPntEffSec->Write();
    fhIntegratedHiddenHitPntLoss->Write();
    fhIntegratedHiddenHitPntLossPrim->Write();
    fhIntegratedHiddenHitPntLossSec->Write();
    fhIntegratedHitPntEffGaps->Write();
    fhIntegratedHitPntEffPrimGaps->Write();
    fhIntegratedHitPntEffSecGaps->Write();

    fhMcTrkStartPrimSingTrk->Write();
    fhMcTrkStartSecSingTrk->Write();
    fhMcTrkStartPrimMultiTrk->Write();
    fhMcTrkStartSecMultiTrk->Write();
  }  // if( kFALSE == fbNormHistGenMode )

  fHist->cd();  // make the file root the current directory
  fhPointMatchWeight->Write();

  gDirectory->cd(oldir->GetPath());

  fHist->Close();

  return kTRUE;
}
Bool_t CbmTofHitFinderQa::DeleteHistos() {
  // Nb Hits per event for first N events
  delete fhNbHitsPerEvent;
  delete fhNbHitsSingPntPerEvent;
  delete fhNbHitsMultPntPerEvent;
  delete fhNbHitsSingTrkPerEvent;
  delete fhNbHitsMultTrkPerEvent;
  // Nb different TOF digis in Hit
  delete fhNbDigisInHit;
  delete fhNbDigisInHitMapXY;

  // Mapping
  // Physics coord mapping, 1 per particle type
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    delete fvhTrackAllStartZCent[iPartIdx];
    delete fvhTrackSecStartZCent[iPartIdx];
    if (2 == iPartIdx)  // 3D plot only for e-
      delete fvhTrackAllStartXZCent[iPartIdx];
    delete fvhTrackAllStartXZ[iPartIdx];
    delete fvhTrackAllStartYZ[iPartIdx];
    delete fvhTofPntAllAngCent[iPartIdx];
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
  fvhTrackAllStartZCent.clear();
  fvhTrackSecStartZCent.clear();
  fvhTrackAllStartXZCent.clear();
  fvhTrackAllStartXZ.clear();
  fvhTrackAllStartYZ.clear();
  fvhTofPntAllAngCent.clear();
  if (kTRUE == fbNormHistGenMode) {
    delete fhTrackMapXY;
    delete fhTrackMapXZ;
    delete fhTrackMapYZ;
    delete fhTrackMapAng;
    delete fhTrackMapSph;
  }  // if( kTRUE == fbNormHistGenMode )
  delete fhPointMapXY;
  delete fhPointMapXZ;
  delete fhPointMapYZ;
  delete fhPointMapAng;
  delete fhPointMapSph;
  delete fhRealPointMapXY;
  delete fhRealPointMapXZ;
  delete fhRealPointMapYZ;
  delete fhRealPointMapAng;
  delete fhRealPointMapSph;
  delete fhDigiMapXY;
  delete fhDigiMapXZ;
  delete fhDigiMapYZ;
  delete fhDigiMapAng;
  delete fhDigiMapSph;
  delete fhHitMapXY;
  delete fhHitMapXZ;
  delete fhHitMapYZ;
  delete fhHitMapAng;
  delete fhHitMapSph;
  // L/R digis missmatch
  delete fhLeftRightDigiMatch;
  // Nb different MC Points and Tracks in Hit
  delete fhNbPointsInHit;
  delete fhNbTracksInHit;
  // Mapping of position for hits coming from a single MC Point
  delete fhHitMapSingPntXY;
  delete fhHitMapSingPntXZ;
  delete fhHitMapSingPntYZ;
  delete fhHitMapSingPntAng;
  delete fhHitMapSingPntSph;
  // Mapping of position for hits coming from multiple MC Points
  delete fhHitMapMultPntXY;
  delete fhHitMapMultPntXZ;
  delete fhHitMapMultPntYZ;
  delete fhHitMapMultPntAng;
  delete fhHitMapMultPntSph;
  // Mapping of position for hits coming from a single MC Track
  delete fhHitMapSingTrkXY;
  delete fhHitMapSingTrkXZ;
  delete fhHitMapSingTrkYZ;
  delete fhHitMapSingTrkAng;
  delete fhHitMapSingTrkSph;
  // Mapping of position for hits coming from multiple MC Tracks
  delete fhHitMapMultTrkXY;
  delete fhHitMapMultTrkXZ;
  delete fhHitMapMultTrkYZ;
  delete fhHitMapMultTrkAng;
  delete fhHitMapMultTrkSph;
  // Hit Quality for Hits coming from a single MC Point
  delete fhSinglePointHitDeltaX;
  delete fhSinglePointHitDeltaY;
  delete fhSinglePointHitDeltaZ;
  delete fhSinglePointHitDeltaR;
  delete fhSinglePointHitDeltaT;
  delete fhSinglePointHitPullX;
  delete fhSinglePointHitPullY;
  delete fhSinglePointHitPullZ;
  delete fhSinglePointHitPullR;
  delete fhSinglePointHitPullT;

  delete fhCltSzSinglePointHitPullX;
  delete fhCltSzSinglePointHitPullY;
  delete fhCltSzSinglePointHitPullZ;
  delete fhCltSzSinglePointHitPullR;
  delete fhCltSzSinglePointHitPullT;
  // Hit Quality for Hits coming from a multiple MC Points
  // To Point closest to Hit
  delete fhMultiPntHitClosestDeltaX;
  delete fhMultiPntHitClosestDeltaY;
  delete fhMultiPntHitClosestDeltaZ;
  delete fhMultiPntHitClosestDeltaR;
  delete fhMultiPntHitClosestDeltaT;
  delete fhMultiPntHitClosestPullX;
  delete fhMultiPntHitClosestPullY;
  delete fhMultiPntHitClosestPullZ;
  delete fhMultiPntHitClosestPullR;
  delete fhMultiPntHitClosestPullT;
  // To Point furthest from Hit
  delete fhMultiPntHitFurthestDeltaX;
  delete fhMultiPntHitFurthestDeltaY;
  delete fhMultiPntHitFurthestDeltaZ;
  delete fhMultiPntHitFurthestDeltaR;
  delete fhMultiPntHitFurthestDeltaT;
  delete fhMultiPntHitFurthestPullX;
  delete fhMultiPntHitFurthestPullY;
  delete fhMultiPntHitFurthestPullZ;
  delete fhMultiPntHitFurthestPullR;
  delete fhMultiPntHitFurthestPullT;
  // To mean Point position
  delete fhMultiPntHitMeanDeltaX;
  delete fhMultiPntHitMeanDeltaY;
  delete fhMultiPntHitMeanDeltaZ;
  delete fhMultiPntHitMeanDeltaR;
  delete fhMultiPntHitMeanDeltaT;
  delete fhMultiPntHitMeanPullX;
  delete fhMultiPntHitMeanPullY;
  delete fhMultiPntHitMeanPullZ;
  delete fhMultiPntHitMeanPullR;
  delete fhMultiPntHitMeanPullT;
  // To best Point position
  delete fhMultiPntHitBestDeltaX;
  delete fhMultiPntHitBestDeltaY;
  delete fhMultiPntHitBestDeltaZ;
  delete fhMultiPntHitBestDeltaR;
  delete fhMultiPntHitBestDeltaT;
  delete fhMultiPntHitBestPullX;
  delete fhMultiPntHitBestPullY;
  delete fhMultiPntHitBestPullZ;
  delete fhMultiPntHitBestPullR;
  delete fhMultiPntHitBestPullT;
  // Hit Quality for Hits coming from a single MC Track
  delete fhSingleTrackHitDeltaX;
  delete fhSingleTrackHitDeltaY;
  delete fhSingleTrackHitDeltaZ;
  delete fhSingleTrackHitDeltaR;
  delete fhSingleTrackHitDeltaT;
  delete fhSingleTrackHitPullX;
  delete fhSingleTrackHitPullY;
  delete fhSingleTrackHitPullZ;
  delete fhSingleTrackHitPullR;
  delete fhSingleTrackHitPullT;
  // Hit Quality for Hits coming from a single MC Track but multiple points
  delete fhSingTrkMultiPntHitDeltaX;
  delete fhSingTrkMultiPntHitDeltaY;
  delete fhSingTrkMultiPntHitDeltaZ;
  delete fhSingTrkMultiPntHitDeltaR;
  delete fhSingTrkMultiPntHitDeltaT;
  delete fhSingTrkMultiPntHitPullX;
  delete fhSingTrkMultiPntHitPullY;
  delete fhSingTrkMultiPntHitPullZ;
  delete fhSingTrkMultiPntHitPullR;
  delete fhSingTrkMultiPntHitPullT;

  // Hit Quality for Hits coming from a multiple MC Points
  // To Track closest to Hit
  delete fhMultiTrkHitClosestDeltaX;
  delete fhMultiTrkHitClosestDeltaY;
  delete fhMultiTrkHitClosestDeltaZ;
  delete fhMultiTrkHitClosestDeltaR;
  delete fhMultiTrkHitClosestDeltaT;
  delete fhMultiTrkHitClosestPullX;
  delete fhMultiTrkHitClosestPullY;
  delete fhMultiTrkHitClosestPullZ;
  delete fhMultiTrkHitClosestPullR;
  delete fhMultiTrkHitClosestPullT;
  // To Track furthest from Hit
  delete fhMultiTrkHitFurthestDeltaX;
  delete fhMultiTrkHitFurthestDeltaY;
  delete fhMultiTrkHitFurthestDeltaZ;
  delete fhMultiTrkHitFurthestDeltaR;
  delete fhMultiTrkHitFurthestDeltaT;
  delete fhMultiTrkHitFurthestPullX;
  delete fhMultiTrkHitFurthestPullY;
  delete fhMultiTrkHitFurthestPullZ;
  delete fhMultiTrkHitFurthestPullR;
  delete fhMultiTrkHitFurthestPullT;
  // To mean Track position
  delete fhMultiTrkHitMeanDeltaX;
  delete fhMultiTrkHitMeanDeltaY;
  delete fhMultiTrkHitMeanDeltaZ;
  delete fhMultiTrkHitMeanDeltaR;
  delete fhMultiTrkHitMeanDeltaT;
  delete fhMultiTrkHitMeanPullX;
  delete fhMultiTrkHitMeanPullY;
  delete fhMultiTrkHitMeanPullZ;
  delete fhMultiTrkHitMeanPullR;
  delete fhMultiTrkHitMeanPullT;
  // To best Track position
  delete fhMultiTrkHitBestDeltaX;
  delete fhMultiTrkHitBestDeltaY;
  delete fhMultiTrkHitBestDeltaZ;
  delete fhMultiTrkHitBestDeltaR;
  delete fhMultiTrkHitBestDeltaT;
  delete fhMultiTrkHitBestPullX;
  delete fhMultiTrkHitBestPullY;
  delete fhMultiTrkHitBestPullZ;
  delete fhMultiTrkHitBestPullR;
  delete fhMultiTrkHitBestPullT;

  // Physics coord mapping, 1 per particle type
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Phase space
    delete fvhPtmRapGenTrk[iPartIdx];
    delete fvhPtmRapStsPnt[iPartIdx];
    delete fvhPtmRapTofPnt[iPartIdx];
    delete fvhPtmRapTofHit[iPartIdx];
    delete fvhPtmRapTofHitSinglePnt[iPartIdx];
    delete fvhPtmRapTofHitSingleTrk[iPartIdx];
    // PLab
    delete fvhPlabGenTrk[iPartIdx];
    delete fvhPlabStsPnt[iPartIdx];
    delete fvhPlabTofPnt[iPartIdx];
    delete fvhPlabTofHit[iPartIdx];
    delete fvhPlabTofHitSinglePnt[iPartIdx];
    delete fvhPlabTofHitSingleTrk[iPartIdx];
    // MC Tracks losses
    delete fvhPtmRapGenTrkTofPnt[iPartIdx];
    delete fvhPtmRapGenTrkTofHit[iPartIdx];
    delete fvhPlabGenTrkTofPnt[iPartIdx];
    delete fvhPlabGenTrkTofHit[iPartIdx];
    delete fvhPlabStsTrkTofPnt[iPartIdx];
    delete fvhPlabStsTrkTofHit[iPartIdx];

    // Secondary tracks
    // Phase space
    delete fvhPtmRapSecGenTrk[iPartIdx];
    delete fvhPtmRapSecStsPnt[iPartIdx];
    delete fvhPtmRapSecTofPnt[iPartIdx];
    delete fvhPtmRapSecTofHit[iPartIdx];
    delete fvhPtmRapSecTofHitSinglePnt[iPartIdx];
    delete fvhPtmRapSecTofHitSingleTrk[iPartIdx];
    // PLab
    delete fvhPlabSecGenTrk[iPartIdx];
    delete fvhPlabSecStsPnt[iPartIdx];
    delete fvhPlabSecTofPnt[iPartIdx];
    delete fvhPlabSecTofHit[iPartIdx];
    delete fvhPlabSecTofHitSinglePnt[iPartIdx];
    delete fvhPlabSecTofHitSingleTrk[iPartIdx];
    // MC Tracks losses
    delete fvhPtmRapSecGenTrkTofPnt[iPartIdx];
    delete fvhPtmRapSecGenTrkTofHit[iPartIdx];
    delete fvhPlabSecGenTrkTofPnt[iPartIdx];
    delete fvhPlabSecGenTrkTofHit[iPartIdx];
    delete fvhPlabSecStsTrkTofPnt[iPartIdx];
    delete fvhPlabSecStsTrkTofHit[iPartIdx];
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
     // Phase space
  fvhPtmRapGenTrk.clear();
  fvhPtmRapStsPnt.clear();
  fvhPtmRapTofPnt.clear();
  fvhPtmRapTofHit.clear();
  fvhPtmRapTofHitSinglePnt.clear();
  fvhPtmRapTofHitSingleTrk.clear();
  // PLab
  fvhPlabGenTrk.clear();
  fvhPlabStsPnt.clear();
  fvhPlabTofPnt.clear();
  fvhPlabTofHit.clear();
  fvhPlabTofHitSinglePnt.clear();
  fvhPlabTofHitSingleTrk.clear();
  // MC Tracks losses
  fvhPtmRapGenTrkTofPnt.clear();
  fvhPtmRapGenTrkTofHit.clear();
  fvhPlabGenTrkTofPnt.clear();
  fvhPlabGenTrkTofHit.clear();
  fvhPlabStsTrkTofPnt.clear();
  fvhPlabStsTrkTofHit.clear();

  // Secondary tracks
  // Phase space
  fvhPtmRapSecGenTrk.clear();
  fvhPtmRapSecStsPnt.clear();
  fvhPtmRapSecTofPnt.clear();
  fvhPtmRapSecTofHit.clear();
  fvhPtmRapSecTofHitSinglePnt.clear();
  fvhPtmRapSecTofHitSingleTrk.clear();
  // PLab
  fvhPlabSecGenTrk.clear();
  fvhPlabSecStsPnt.clear();
  fvhPlabSecTofPnt.clear();
  fvhPlabSecTofHit.clear();
  fvhPlabSecTofHitSinglePnt.clear();
  fvhPlabSecTofHitSingleTrk.clear();
  // MC Tracks losses
  fvhPtmRapSecGenTrkTofPnt.clear();
  fvhPtmRapSecGenTrkTofHit.clear();
  fvhPlabSecGenTrkTofPnt.clear();
  fvhPlabSecGenTrkTofHit.clear();
  fvhPlabSecStsTrkTofPnt.clear();
  fvhPlabSecStsTrkTofHit.clear();

  delete fhIntegratedHitPntEff;
  delete fhIntegratedHitPntEffPrim;
  delete fhIntegratedHitPntEffSec;

  delete fhIntegratedHiddenHitPntLoss;
  delete fhIntegratedHiddenHitPntLossPrim;
  delete fhIntegratedHiddenHitPntLossSec;

  delete fhIntegratedHitPntEffGaps;
  delete fhIntegratedHitPntEffPrimGaps;
  delete fhIntegratedHitPntEffSecGaps;

  delete fhMcTrkStartPrimSingTrk;
  delete fhMcTrkStartSecSingTrk;
  delete fhMcTrkStartPrimMultiTrk;
  delete fhMcTrkStartSecMultiTrk;

  delete fhPointMatchWeight;

  return kTRUE;
}


ClassImp(CbmTofHitFinderQa);
