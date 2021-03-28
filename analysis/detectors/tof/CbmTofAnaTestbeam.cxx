// ------------------------------------------------------------------
// -----                     CbmTofAnaTestbeam                        -----
// -----              Created 12/04/2014 by nh                 -----
// ------------------------------------------------------------------

#include "CbmTofAnaTestbeam.h"

// TOF Classes and includes
#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmTofAddress.h"          // in cbmdata/tof
#include "CbmTofCell.h"             // in tof/TofData
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDigi.h"             // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"              // in cbmdata/tof
#include "CbmTofPoint.h"
#include "CbmTofTracklet.h"
#include "CbmTofTrackletTools.h"
#include "CbmTrackMatchNew.h"
#include "CbmVertex.h"

#include "CbmDigiManager.h"
#include "CbmTofTestBeamClusterizer.h"
#include "CbmTofTrackFinderNN.h"

#include "TTrbHeader.h"
//#include "TMbsMappingTofPar.h"

#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCDataObject.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"

// CBMroot classes and includes

// FAIR classes and includes
#include "FairLogger.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ROOT Classes and includes
#include "Riostream.h"
#include "TClonesArray.h"
#include "TDirectory.h"
#include "TEfficiency.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TGeoManager.h"
#include "TGeoPhysicalNode.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMCProcess.h"
#include "TMath.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"

// C++ STL
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using std::cout;
using std::endl;
using std::vector;

const Int_t DetMask   = 0x001FFFFF;  // for v21a geometries
const Double_t DTDMAX = 6.;          // diamond inspection range in ns

Double_t dTDia;
Double_t dDTD4Min                 = 1.E8;
static Double_t StartAnalysisTime = 0.;
static Double_t dTLEvt            = 0.;

Int_t iNspills = 0;

static Double_t fdMemoryTime = 1.E12;  // memory time in ns

CbmDigiManager* fDigiMan = NULL;

static std::vector<TH2D*> fhLHTime;
static std::vector<std::vector<CbmTofHit*>> fvLHit;
static std::map<UInt_t, UInt_t> fDetIdMap;

struct cmp_str {
  bool operator()(char const* a, char const* b) const {
    return std::strcmp(a, b) < 0;
  }
};

std::map<Int_t, const char*> giPdgMap {// Gauge bosons
                                       {22, "#gamma"},
                                       // leptons
                                       {11, "e^{-}"},
                                       {-11, "e^{+}"},
                                       {13, "#mu^{-}"},
                                       {-13, "#mu^{+}"},
                                       // mesons
                                       {111, "#pi^{0}"},
                                       {211, "#pi^{+}"},
                                       {-211, "#pi^{-}"},
                                       {221, "#eta"},
                                       {130, "#Kappa^{0}_{L}"},
                                       {310, "#Kappa^{0}_{S}"},
                                       {321, "#Kappa^{+}"},
                                       {-321, "#Kappa^{-}"},
                                       // baryons
                                       {2112, "n"},
                                       {-2112, "#bar{n}"},
                                       {2212, "p"},
                                       {-2212, "#bar{p}"},
                                       {3112, "#Sigma^{-}"},
                                       {-3112, "#Sigma^{+}"},
                                       {3122, "#Lambda"},
                                       {-3122, "#bar{#Lambda}"},
                                       {3212, "#Sigma^{0}"},
                                       {-3212, "#bar{#Sigma^{0}}"},
                                       {3222, "#Sigma^{+}"},
                                       {-3222, "#Sigma^{-}"},
                                       {3312, "#Xi^{-}"},
                                       {-3312, "#Xi^{+}"},
                                       {3322, "#Xi^{0}"},
                                       {-3322, "#bar{#Xi^{0}}"},
                                       {3334, "#Omega^{-}"},
                                       {-3334, "#Omega^{+}"},
                                       // fragments
                                       {1000010020, "d"},
                                       {1000010030, "t"},
                                       {1000020040, "#alpha"},
                                       // dummy
                                       {0, "Geant"}};

std::map<const char*, const char*, cmp_str> gcMaterialMap {
  {"RPCgas", "gas"},
  {"RPCgas_noact", "gas"},
  {"RPCglass", "glass"},
  {"air", "air"},
  {"aluminium", "box"},
  {"carbon", "pcb"},
  {"targetMaterial", "target"},
  {"silicon", "diamond"}};

std::map<const char*, const char*, cmp_str> gcProcessMap {
  {"Compton scattering", "Compton"},
  {"Decay", "Decay"},
  {"Delta ray", "Delta"},
  {"Hadronic interaction", "Hadronic"},
  {"Lepton pair production", "Pair"},
  {"Photoelectric effect", "Photo"},
  {"Positron annihilation", "Annil"},
  {"Primary particle emission", "Primary"},
  {"Bremstrahlung", "Brems"}};

void GetPdgName(Int_t iPdgCode, const char*& cPdgName);
void GetMaterialName(const char* cMaterial, const char*& cMaterialName);
void GetProcessName(const char* cProcess, const char*& cProcessName);

//___________________________________________________________________
//
// CbmTofAnaTestbeam
//
// Task for analysis of Testbeam data
//
// ------------------------------------------------------------------
CbmTofAnaTestbeam::CbmTofAnaTestbeam()
  : CbmTofAnaTestbeam("TestbeamAnalysis", 0) {
  cout << "CbmTofTests: Task started " << endl;
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmTofAnaTestbeam::CbmTofAnaTestbeam(const char* name, Int_t verbose)
  : FairTask(name, verbose)
  , fEvents(0)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fChannelInfo(NULL)
  , fChannelInfoRef(NULL)
  , fChannelInfoDut(NULL)
  , fChannelInfoSel2(NULL)
  , fMbsMappingPar(NULL)
  , iNbSmTot(0)
  , fvTypeSmOffs()
  , iNbRpcTot(0)
  , fvSmRpcOffs()
  , iNbChTot(0)
  , fvRpcChOffs()
  , fDigiPar(NULL)
  , fDigiBdfPar(NULL)
  , fTofDigisColl(NULL)
  , fTofHitsColl(NULL)
  , fTofDigiMatchColl(NULL)
  , fTofTrackColl(NULL)
  , fEventsColl(NULL)
  , fTofDigisCollIn(NULL)
  , fTofHitsCollIn(NULL)
  , fTofDigiMatchCollIn(NULL)
  , fTofTrackCollIn(NULL)
  , fTrbHeader(NULL)
  , fdDXMean(0.)
  , fdDYMean(0.)
  , fdDTMean(0.)
  , fdDXWidth(0.)
  , fdDYWidth(0.)
  , fdDTWidth(0.)
  , fhTriggerPattern(NULL)
  , fhTriggerType(NULL)
  , fhTimeInSpill(NULL)
  , fhRate_all(NULL)
  , fhTIS_all(NULL)
  , fhTIS_sel(NULL)
  , fhTIS_sel1(NULL)
  , fhTIS_sel2(NULL)
  , fhTIR_all(NULL)
  , fhTIR_sel(NULL)
  , fhTIR_sel1(NULL)
  , fhTIR_sel2(NULL)
  , fhTIS_Nhit(NULL)
  , fhTIS_Ntrk(NULL)
  , fhDTLH_all(NULL)
  , fhDTLH_sel(NULL)
  , fhDTLH_sel1(NULL)
  , fhDTLH_sel2(NULL)
  , fhDTLH_DStrip(NULL)
  , fhDT2(NULL)
  , fhXX2(NULL)
  , fhYY2(NULL)
  , fhNMatch04(NULL)
  , fhXX04(NULL)
  , fhYY04(NULL)
  , fhXY04(NULL)
  , fhYX04(NULL)
  , fhTT04(NULL)
  , fhDutDXDYDT(NULL)
  , fhRefDXDYDT(NULL)
  , fhChi04(NULL)
  , fhChiSel24(NULL)
  , fhDXSel24(NULL)
  , fhDYSel24(NULL)
  , fhDTSel24(NULL)
  , fhDXDY04(NULL)
  , fhDXDT04(NULL)
  , fhDYDT04(NULL)
  , fhTofSel24(NULL)
  , fhNMatch04sel(NULL)
  , fhChi04best(NULL)
  , fhDigiMul0best(NULL)
  , fhDigiMul4best(NULL)
  , fhDXDY04best(NULL)
  , fhDXDT04best(NULL)
  , fhDYDT04best(NULL)
  , fhChiDT04best(NULL)
  , fhDT24DT04best(NULL)
  , fhDTD4DT04best(NULL)
  , fhX0DT04best(NULL)
  , fhY0DT04best(NULL)
  , fhNMatchD4sel(NULL)
  , fhChi04D4best(NULL)
  , fhTofD4best(NULL)
  , fhVelD4best(NULL)
  , fhDigiMul0D4best(NULL)
  , fhDigiMul4D4best(NULL)
  , fhCluSize04D4best(NULL)
  , fhCluMul04D4best(NULL)
  , fhStrMul04D4best(NULL)
  , fhCluMulTSig0D4best(NULL)
  , fhCluMulTSig4D4best(NULL)
  , fhCluMulTrel0D4best(NULL)
  , fhCluMulTrel4D4best(NULL)
  , fhCluSizeTrel0D4best(NULL)
  , fhCluSizeTrel4D4best(NULL)
  , fhDXDY04D4best(NULL)
  , fhDXDT04D4best(NULL)
  , fhDYDT04D4best(NULL)
  , fhDistDT04D4best(NULL)
  , fhTexpDT04D4best(NULL)
  , fhCluSize0DT04D4best(NULL)
  , fhCluSize4DT04D4best(NULL)
  , fhTot0DT04D4best(NULL)
  , fhTot4DT04D4best(NULL)
  , fhCluSizeSigT0D4best(NULL)
  , fhCluSizeSigT4D4best(NULL)
  , fhChiDT04D4best(NULL)
  , fhDT24DT04D4best(NULL)
  , fhDTD4DT04D4best(NULL)
  , fhX0DT04D4best(NULL)
  , fhY0DT04D4best(NULL)
  , fhTISDT04D4best(NULL)
  , fhDTMul4D4best(NULL)
  , fhDTX4D4best(NULL)
  , fhDTY4D4best(NULL)
  , fhDXX4D4best(NULL)
  , fhDXY4D4best(NULL)
  , fhDYX4D4best(NULL)
  , fhDYY4D4best(NULL)
  , fhDTMul0D4best(NULL)
  , fhDTX0D4best(NULL)
  , fhDTY0D4best(NULL)
  , fhDXX0D4best(NULL)
  , fhDXY0D4best(NULL)
  , fhDYX0D4best(NULL)
  , fhDYY0D4best(NULL)
  , fhChi04D4sbest(NULL)
  , fhTofD4sbest(NULL)
  , fhVelD4sbest(NULL)
  , fhDigiMul0D4sbest(NULL)
  , fhDigiMul4D4sbest(NULL)
  , fhCluMul04D4sbest(NULL)
  , fhDXDY04D4sbest(NULL)
  , fhDXDT04D4sbest(NULL)
  , fhDYDT04D4sbest(NULL)
  , fhDistDT04D4sbest(NULL)
  , fhTexpDT04D4sbest(NULL)
  , fhCluSize0DT04D4sbest(NULL)
  , fhCluSize4DT04D4sbest(NULL)
  , fhTot0DT04D4sbest(NULL)
  , fhTot4DT04D4sbest(NULL)
  , fhChiDT04D4sbest(NULL)
  , fhDT24DT04D4sbest(NULL)
  , fhDTD4DT04D4sbest(NULL)
  , fhX0DT04D4sbest(NULL)
  , fhY0DT04D4sbest(NULL)
  , fhDTMul4D4sbest(NULL)
  , fhDTX4D4sbest(NULL)
  , fhDTY4D4sbest(NULL)
  , fhDXX4D4sbest(NULL)
  , fhDXY4D4sbest(NULL)
  , fhDYX4D4sbest(NULL)
  , fhDYY4D4sbest(NULL)
  , fhDTMul0D4sbest(NULL)
  , fhDTX0D4sbest(NULL)
  , fhDTY0D4sbest(NULL)
  , fhDXX0D4sbest(NULL)
  , fhDXY0D4sbest(NULL)
  , fhDYX0D4sbest(NULL)
  , fhDYY0D4sbest(NULL)
  , fhNMatch24(NULL)
  , fhNMatch24sel(NULL)
  , fhDT24sel(NULL)
  , fhChi24(NULL)
  , fhXY24(NULL)
  , fhDXDY24(NULL)
  , fhDXDT24(NULL)
  , fhDYDT24(NULL)
  , fhXY0D4best(NULL)
  , fhXY4D4best(NULL)
  , fhXX04D4best(NULL)
  , fhYY04D4best(NULL)
  , fhXYSel2D4best(NULL)
  , fhXY0D4sel(NULL)
  , fhXY4D4sel(NULL)
  , fhXYSel2D4sel(NULL)
  , fhDTD4sel(NULL)
  , fhTofD4sel(NULL)
  , fhBRefMul(NULL)
  , fhDTD4(NULL)
  , fhXYPos()
  , fhEtaPhi(NULL)
  , fhDT04DX0_1(NULL)
  , fhDT04DY0_1(NULL)
  , fhDT04DT0_1(NULL)
  , fhDT04DX4_1(NULL)
  , fhDT04DY4_1(NULL)
  , fhDT04DT4_1(NULL)
  , fhDT04DX0_2(NULL)
  , fhDT04DY0_2(NULL)
  , fhDT04DT0_2(NULL)
  , fhDT04DX4_2(NULL)
  , fhDT04DY4_2(NULL)
  , fhDT04DT4_2(NULL)
  , fhDutPullX(NULL)
  , fhDutPullXB(NULL)
  , fhDutPullY(NULL)
  , fhDutPullYB(NULL)
  , fhDutPullZ(NULL)
  , fhDutPullT(NULL)
  , fhDutPullTB(NULL)
  , fhDutChi_Found(NULL)
  , fhDutChi_Missed(NULL)
  , fhDutChi_Match(NULL)
  , fhDutXY_Found(NULL)
  , fhDutXY_Missed(NULL)
  , fhDutDTLH_Found(NULL)
  , fhDutDTLH_Missed(NULL)
  , fhDutMul_Found(NULL)
  , fhDutMul_Missed(NULL)
  , fhDutTIS_Found(NULL)
  , fhDutTIS_Missed(NULL)
  , fhDutTIR_Found(NULL)
  , fhDutTIR_Missed(NULL)
  , fhDutVel_Found(NULL)
  , fhDutVel_Missed(NULL)
  , fhDutDTLH_CluSize(NULL)
  , fhDutDTLH_Tot(NULL)
  , fhDutDTLH_Mul(NULL)
  , fhDutDTLH_TIS(NULL)
  , fhDutDTLH_Missed_TIS(NULL)
  , fhDutDTLH_DDH_Found(NULL)
  , fhDutDTLH_DD_Found(NULL)
  , fhDutDTLH_DD_Missed(NULL)
  , fhDutXYDX(NULL)
  , fhDutXYDY(NULL)
  , fhDutXYDT(NULL)
  , fhNMergedMCEvents(NULL)
  , fhAccTrackMul(NULL)
  , fhAccRefTrackMul(NULL)
  , fhAccPrimTrackMul(NULL)
  , fhAccTrackPointMul(NULL)
  , fhAccRefTrackPointMul(NULL)
  , fhAccRndmTrackPointMul(NULL)
  , fhAccRefTrackAcceptance(NULL)
  , fhAccRefTrackAcceptanceEfficiency(NULL)
  , fhAccRefTrackAcceptancePurity(NULL)
  , fhAccRefTrackMulCentrality(NULL)
  , fhAccRefTracksProcSpec(NULL)
  , fhSelMCTrackEfficiency(NULL)
  , fhSelMCTrackMatchEfficiency(NULL)
  , fhSelMCTrackMatchPurity(NULL)
  , fhSelMCTrackDutHitMatchNNMul(NULL)
  , fhSelMCTrackDutHitMatchAccNNMul(NULL)
  , fhSelEfficiency(NULL)
  , fhSelPurity(NULL)
  , fhSelRefTrackShare(NULL)
  , fhSelRefTrackProcSpec(NULL)
  , fhSelMatchEfficiency(NULL)
  , fhSelMatchPurity(NULL)
  , fhResX04HitExp(NULL)
  , fhResX04ExpMC(NULL)
  , fhResX04HitMC(NULL)
  , fhResY04HitExp(NULL)
  , fhResY04ExpMC(NULL)
  , fhResY04HitMC(NULL)
  , fhResT04HitExp(NULL)
  , fhResT04ExpMC(NULL)
  , fhResT04HitMC(NULL)
  , fhNTracksPerMRefHit(NULL)
  , fhNTracksPerSel2Hit(NULL)
  , fhNTracksPerDutHit(NULL)
  , fhNTracksPerSelMRefHit(NULL)
  , fhNTracksPerSelSel2Hit(NULL)
  , fhNTracksPerSelDutHit(NULL)
  , fhSelTrklEfficiency(NULL)
  , fhSelTrklPurity(NULL)
  , fhSelTrklRefTrackShare(NULL)
  , fhSelTrklRefTrackProcSpec(NULL)
  , fhSelTrklMatchEfficiency(NULL)
  , fhSelTrklMatchPurity(NULL)
  , fhDutResX_Hit_Trk(NULL)
  , fhDutResX_Trk_MC(NULL)
  , fhDutResX_Hit_MC(NULL)
  , fhDutResY_Hit_Trk(NULL)
  , fhDutResY_Trk_MC(NULL)
  , fhDutResY_Hit_MC(NULL)
  , fhDutResT_Hit_Trk(NULL)
  , fhDutResT_Trk_MC(NULL)
  , fhDutResT_Hit_MC(NULL)
  , fhSelHitTupleEfficiencyTIS(NULL)
  , fhSelTrklEfficiencyTIS(NULL)
  , fhSelMCTrackEfficiencyTIS(NULL)
  , fhSelHitTupleMatchEfficiencyTIS(NULL)
  , fhSelTrklMatchEfficiencyTIS(NULL)
  , fhSelMCTrackMatchEfficiencyTIS(NULL)
  , fhSelHitTupleResidualTTIS(NULL)
  , fhSelTrklResidualTTIS(NULL)
  , fhSelMCTrackResidualTTIS(NULL)
  , fhSelHitTupleDutCluSizeTIS(NULL)
  , fhSelTrklDutCluSizeTIS(NULL)
  , fhSelMCTrackDutCluSizeTIS(NULL)
  , fhPVResTAll(NULL)
  , fhPVResXAll(NULL)
  , fhPVResYAll(NULL)
  , fhPVResTRef(NULL)
  , fhPVResXRef(NULL)
  , fhPVResYRef(NULL)
  , fhAccRefTrackResT(NULL)
  , fhAccRefTrackResX(NULL)
  , fhAccRefTrackResY(NULL)
  , fhAccRefTrackResTx(NULL)
  , fhAccRefTrackResTy(NULL)
  , fhAccRefTrackResV(NULL)
  , fhAccRefTrackResN(NULL)
  , fhAccRefTrackShare(NULL)
  , fhRecRefTrackEfficiency(NULL)
  , fhRecRndmTrackEfficiency(NULL)
  , fhRecRefTrackGhostShare(NULL)
  , fhRecRefTrackCloneShare(NULL)
  , fhRecRndmTrackGhostShare(NULL)
  , fhRecRndmTrackCloneShare(NULL)
  , fhDomTracksProcSpec()
  , fhDomTracksProcMat()
  , fhRndmTracksProcSpec()
  , fhRndmTracksProcMat()
  , fhCounterAccTrackMul()
  , fhCounterAccRefTrackMul()
  , fhCounterAccRndmTrackMul()
  , fhCounterAccDomTrackMul()
  , fhCounterRecRefTrackEfficiencyPassed()
  , fhCounterRecRefTrackEfficiencyTotal()
  , fhCounterRecRefTrackPurityPassed()
  , fhCounterRefTrackMulHitMul()
  , fhCounterRefTrackLocalXY()
  , fhCounterRefTrackMulCell()
  , fhCounterHitMulCell()
  , fhSelTrklFitRedChiSq(NULL)
  , fhSelTrklDutHitMatchNNMul(NULL)
  , fhSelTrklDutHitMatchAccNNMul(NULL)
  , fhSelHitTupleDutHitMatchMul(NULL)
  , fhSelHitTupleDutHitMatchAccMul(NULL)
  , fhSelTypeNNChiSq(NULL)
  , fhSelTypeNNResidualT(NULL)
  , fhSelTypeNNResidualX(NULL)
  , fhSelTypeNNResidualY(NULL)
  , fhSelTypeAccNNChiSq(NULL)
  , fhSelTypeAccNNResidualT(NULL)
  , fhSelTypeAccNNResidualX(NULL)
  , fhSelTypeAccNNResidualY(NULL)
  , fhGoodSelTypeNNPureChiSq(NULL)
  , fhGoodSelTypeNNAllChiSq(NULL)
  , fhTrklNofHitsRate(NULL)
  , fhTrklDetHitRate(NULL)
  , fhTrklNofHitsRateInSpill(NULL)
  , fhTrklDetHitRateInSpill(NULL)
  , fStart()
  , fStop()
  , fCalParFileName("")
  , fCalOutFileName("./tofAnaTestBeam.hst.root")
  , fCalParFile(NULL)
  , fhDTD4DT04D4Off(NULL)
  , fhDTX4D4Off(NULL)
  , fhDTY4D4Off(NULL)
  , fhDTTexpD4Off(NULL)
  , fhCluSize0DT04D4Off(NULL)
  , fhCluSize4DT04D4Off(NULL)
  , fhTot0DT04D4Off(NULL)
  , fhTot4DT04D4Off(NULL)
  , fhSelTypeNNResidualT_Width(NULL)
  , fhSelTypeNNResidualX_Width(NULL)
  , fhSelTypeNNResidualY_Width(NULL)
  , fhSelHitTupleResidualXYT_Width(NULL)
  , fdMulDMax(0.)
  , fdSpillDuration(20.)
  , fdSpillBreak(0.9)
  , fdStartSpillTime(-100)
  , fdDTDia(0.)
  , fdDTD4MAX(0.)
  , fdMul0Max(0.)
  , fdMul4Max(0.)
  , fdCh4Sel(0.)
  , fdDCh4Sel(0.)
  , fdPosY4Sel(0.)
  , fdPosY4SelOff(0.)
  , fdChS2Sel(0.)
  , fdDChS2Sel(0.)
  , fdPosYS2Sel(0.)
  , fdPosYS2SelOff(0.)
  , fdSel2TOff(0.)
  , fdHitDistMin(0.)
  , fdHitDistAv(1.)
  , fdTOffD4(0.)
  , fdTShift(0.)
  , fdChi2Lim(0.)
  , fdChi2Lim2(0.)
  , fdDutX(0.)
  , fdDutDX(100.)
  , fdDutY(0.)
  , fdDutDY(100.)
  , fiCorMode(0)
  , fiDutAddr(0)
  , fiMrpcRefAddr(0)
  , fiMrpcSel2Addr(0)
  , fiMrpcSel3Addr(0)
  , fiBeamRefAddr(0)
  , fiDut(0)
  , fiDutSm(0)
  , fiDutRpc(0)
  , fiMrpcRef(0)
  , fiMrpcRefSm(0)
  , fiMrpcRefRpc(0)
  , fiMrpcSel2(-1)
  , fiMrpcSel2Sm(0)
  , fiMrpcSel2Rpc(0)
  , fiMrpcSel3(0)
  , fiMrpcSel3Sm(0)
  , fiMrpcSel3Rpc(0)
  , fiPlaSelect(0)
  , fiBeamRefSmType(0)
  , fiBeamRefSmId(0)
  , fiBeamRefRpc(0)
  , fiDutNch(0)
  , fiReqTrg(-1)
  , fChi2LimFit(100.)
  , fR0LimFit(0.)
  , fSIGLIM(3.)
  , fSIGT(100.)
  , fSIGX(1.)
  , fSIGY(1.)
  , fEnableMatchPosScaling(kTRUE)
  , fFindTracks(NULL)
  , fClusterizer(NULL)
  , fTrackletTools(NULL)
  , fbMonteCarloComparison(kFALSE)
  , fbPointsInInputFile(kFALSE)
  , fbTracksInInputFile(kFALSE)
  , fMCEventHeader(NULL)
  , fMCEventList(NULL)
  , fAccTracks(NULL)
  , fTofPointsTB(NULL)
  , fTofPoints(NULL)
  , fMCTracks(NULL)
  , fTofHitPointMatches(NULL)
  , fTofHitAccTrackMatches(NULL)
  , fTofTrackletAccTrackMatches(NULL)
  , fTofAccTrackTrackletMatches(NULL)
  , fTofAccTrackPointMatches(NULL)
  , fCurrentNodePath()
  , fCurrentModuleNodePath()
  , fiCurrentModuleType(-1)
  , fiCurrentModuleIndex(-1)
  , fiCurrentCounterIndex(-1)
  , fCounterModuleNodes()
  , fiNAccRefTracks(0)
  , fdGhostTrackHitQuota(0.7)
  , fbDelayMCPoints(kFALSE)
  , fbAttachDutHitToTracklet(kFALSE)
  , fbBestSelTrackletOnly(kFALSE)
  , fbUseSigCalib(kFALSE)
  , fiAnaMode(0)
  , fdMCSIGLIM(3.)
  , fdMCSIGT(100.)
  , fdMCSIGX(1.)
  , fdMCSIGY(1.)
  , fiMinMCRefTrackPoints(3)
  , fiMaxMCRefTracks(1)

{}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmTofAnaTestbeam::~CbmTofAnaTestbeam() {
  // Destructor
}
// ------------------------------------------------------------------
/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmTofAnaTestbeam::Init() {
  if (kFALSE == RegisterInputs()) return kFATAL;

  //   fTofId = new ( CbmTofDetectorId )CbmTofDetectorId_v14a();
  if (kFALSE == InitParameters()) return kFATAL;

  if (kFALSE == LoadGeometry()) return kFATAL;

  if (kFALSE == LoadCalParameter()) return kFATAL;

  if (kFALSE == CreateHistos()) return kFATAL;

  if (fEnableMatchPosScaling)
    LOG(info) << Form("Position Scaling for Matching enabled ");
  else
    LOG(info) << Form("Position Scaling for Matching not enabled ");

  fFindTracks    = CbmTofFindTracks::Instance();
  fClusterizer   = CbmTofTestBeamClusterizer::Instance();
  fTrackletTools = new CbmTofTrackletTools();  // initialize tools

  if (NULL == fFindTracks) {
    //fdTShift   += fChannelInfoDut->GetZ()/30.;  // in ns
    //if ( NULL != fChannelInfoSel2 ) fdSel2TOff += fChannelInfoSel2->GetZ()/30.;
    LOG(WARNING) << Form(
      "no FindTracks instance found, use TShift = %8.3f, Sel2Toff = %8.3f",
      fdTShift,
      fdSel2TOff);
  } else {  // reinitialize Offsets
    fdTShift += -fFindTracks->GetTOff(fiMrpcRefAddr)
                + fFindTracks->GetTOff(fiBeamRefAddr);
    fdSel2TOff += -fFindTracks->GetTOff(fiMrpcRefAddr)
                  + fFindTracks->GetTOff(fiMrpcSel2Addr);
    LOG(info) << Form(
      "Set time offsets D: %7.1f, 4: %7.1f, D4: %7.1f, Sel24:  %7.1f",
      fFindTracks->GetTOff(fiBeamRefAddr),
      fFindTracks->GetTOff(fiMrpcRefAddr),
      fdTShift,
      fdSel2TOff);
  }
  return kSUCCESS;
}

void CbmTofAnaTestbeam::SetParContainers() {
  LOG(info) << " CbmTofAnaTestbeam => Get the digi parameters for tof";
  return;
  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
}

Bool_t CbmTofAnaTestbeam::LoadGeometry() {
  LOG(info) << "Geometry starting for  " << fDigiPar->GetNrOfModules()
            << " geometrically known detector cells, looking for type "
            << fiDut;
  fiDutNch          = 0;
  Double_t dDutXmin = 1.E300;
  Double_t dDutXmax = -1.E300;
  fChannelInfoDut   = NULL;
  for (Int_t icell = 0; icell < fDigiPar->GetNrOfModules(); ++icell) {

    Int_t cellId =
      fDigiPar->GetCellId(icell);  // cellId is assigned in CbmTofCreateDigiPar
    fChannelInfo = fDigiPar->GetCell(cellId);
    /*
     Int_t smodule  = fGeoHandler->GetSMType(cellId);   // FIXME  - wrong inline functions!!!
     Int_t smtype   = fGeoHandler->GetSModule(cellId);  // FIXME
     Int_t module  = fGeoHandler->GetCounter(cellId);
     */
    Int_t smtype  = CbmTofAddress::GetSmType(cellId);
    Int_t smodule = CbmTofAddress::GetSmId(cellId);
    Int_t module  = CbmTofAddress::GetRpcId(cellId);
    /*
     LOG(debug3) <<Form(" Id 0x%08x ",cellId)
		<< "  got cell " << smtype << ", " << smodule << ", " << module
		<< ", x-size "<< fChannelInfo->GetSizex()
		<< ", y-size "<< fChannelInfo->GetSizey()
		;
     */
    if (smtype == fiDut && smodule == fiDutSm && module == fiDutRpc) {
      fiDutNch++;
      if (fChannelInfo->GetX() > dDutXmax) dDutXmax = fChannelInfo->GetX();
      if (fChannelInfo->GetX() < dDutXmin) dDutXmin = fChannelInfo->GetX();
      if (fiDutNch == 1) {
        fChannelInfoDut = fChannelInfo;
        LOG(info) << Form(" DutId 0x%08x ", cellId) << "  got cell " << smtype
                  << ", " << smodule << ", " << module << ", x-size "
                  << fChannelInfoDut->GetSizex() << ", y-size "
                  << fChannelInfoDut->GetSizey() << ", # " << fiDutNch
                  << ", Xmin " << dDutXmin << ", Xmax " << dDutXmax;
      }
    }
  }
  if (fiDutNch > 0) {
    Double_t dDutDx = (dDutXmax - dDutXmin) / (fiDutNch - 1);
    LOG(info) << "CbmTofAnaTestbeam::LoadGeometry Dut = " << fiDut << " with "
              << fiDutNch << " channels in x- direction from " << dDutXmin
              << " to " << dDutXmax << ", dx = " << dDutDx;

  } else {
    LOG(error) << "Dut inconsistent " << fiDut << ", " << fiDutNch;
    return kFALSE;
  }

  if (!FindModuleNodes()) {
    LOG(error) << "Could not retrieve module nodes from TGeoManager!";
    return kFALSE;
  }

  return kTRUE;
}

void CbmTofAnaTestbeam::Exec(Option_t* opt) {
  if (!fEventsColl) {
    ExecEvent(opt);
  } else {
    for (Int_t iEvent = 0; iEvent < fEventsColl->GetEntriesFast(); iEvent++) {
      CbmEvent* tEvent = dynamic_cast<CbmEvent*>(fEventsColl->At(iEvent));
      LOG(debug) << "Process event " << iEvent << " with "
                 << tEvent->GetNofData(ECbmDataType::kTofHit) << " hits from "
                 << tEvent->GetNofData(ECbmDataType::kTofDigi) << ", "
                 << tEvent->GetNofData(ECbmDataType::kTofCalDigi) << " digis ";

      if (fTofDigisColl) fTofDigisColl->Clear("C");
      //if(fTofDigisColl)     fTofDigisColl->Delete();
      if (fTofHitsColl) fTofHitsColl->Clear("C");
      if (fTofDigiMatchColl) fTofDigiMatchColl->Clear("C");
      if (fTofTrackColl) fTofTrackColl->Clear("C");

      Int_t iNbDigi = 0;
      /*
      assert ( fTofDigisColl );
      for (Int_t iDigi = 0; iDigi < tEvent->GetNofData(ECbmDataType::kTofCalDigi); iDigi++)
      {
        Int_t iDigiIndex = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofCalDigi, iDigi));
        const CbmTofDigi* tDigi = fDigiMan->Get<CbmTofDigi>(iDigiIndex);
//        CbmTofDigi* tDigi = dynamic_cast<CbmTofDigi*>(fTofDigisCollIn->At(iDigiIndex));
        assert (tDigi);
        //LOG(INFO) << "Copy TofDigi " << iDigi << " from " <<  iDigiIndex << " to " << iNbDigi;
        //new((*fTofDigisColl)[iNbDigi++]) CbmTofDigi(*tDigi); // does not work for tDigi, since no TObject
      }
      */
      Int_t iNbHits = 0;
      for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kTofHit);
           iHit++) {
        Int_t iHitIndex =
          static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofHit, iHit));
        CbmTofHit* tHit =
          dynamic_cast<CbmTofHit*>(fTofHitsCollIn->At(iHitIndex));
        new ((*fTofHitsColl)[iNbHits]) CbmTofHit(*tHit);

        CbmMatch* tMatch =
          dynamic_cast<CbmMatch*>(fTofDigiMatchCollIn->At(iHitIndex));
        new ((*fTofDigiMatchColl)[iNbHits]) CbmMatch(*tMatch);

        iNbHits++;
      }

      Int_t iNbTrks = 0;
      for (Int_t iTrk = 0; iTrk < tEvent->GetNofData(ECbmDataType::kTofTrack);
           iTrk++) {
        Int_t iTrkIndex =
          static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofTrack, iTrk));
        CbmTofTracklet* tTrk =
          dynamic_cast<CbmTofTracklet*>(fTofTrackCollIn->At(iTrkIndex));
        new ((*fTofTrackColl)[iNbTrks++]) CbmTofTracklet(*tTrk);
      }

      ExecEvent(opt);

      if (iNbDigi)
        fTofDigisColl
          ->Delete();  //Clear("C"); // Clear causes memory leak, FIXME
      if (iNbHits) fTofHitsColl->Clear("C");
      if (iNbHits) fTofDigiMatchColl->Delete();  //Clear("C");
      if (iNbTrks) fTofTrackColl->Delete();      //Clear("C");
    }
  }
}


void CbmTofAnaTestbeam::ExecEvent(Option_t* /*option*/) {
  // Task execution
  if (NULL != fFindTracks && fdMemoryTime == 0.) {
    if (!fFindTracks->InspectEvent()) return;

    LOG(debug) << "=> New event with " << fFindTracks->GetNStationsFired()
               << " fired stations for " << fFindTracks->GetMinNofHits()
               << " requested hits";
  }
  fStart.Set();
  FillHistos();
  fStop.Set();


  //   if( 0 < fEvents )
  if (0 == (fEvents % 100000) && 0 < fEvents) {
    LOG(info) << "CbmTofAnaTestbeam::Exec : "
              << "event " << fEvents << " in " << iNspills
              << " spills processed.";
  }
  fEvents += 1;
}

void CbmTofAnaTestbeam::Finish() {
  // Normalisations
  LOG(info) << "Finish up with " << fEvents << " analyzed events in "
            << iNspills << " spills ";

  if (fbMonteCarloComparison) {
    TH1* tCurrentHistogram(NULL);

    if (fhAccRefTracksProcSpec->GetEntries()) {
      fhAccRefTracksProcSpec->LabelsDeflate("X");
      fhAccRefTracksProcSpec->LabelsDeflate("Y");
      fhAccRefTracksProcSpec->LabelsOption("<d", "X");
      fhAccRefTracksProcSpec->LabelsOption("<v", "Y");
      fhAccRefTracksProcSpec->Scale(1. / fhAccRefTracksProcSpec->GetEntries());
    }

    if (fhSelRefTrackProcSpec->GetEntries()) {
      fhSelRefTrackProcSpec->LabelsDeflate("X");
      fhSelRefTrackProcSpec->LabelsDeflate("Y");
      fhSelRefTrackProcSpec->LabelsOption("<d", "X");
      fhSelRefTrackProcSpec->LabelsOption("<v", "Y");
      fhSelRefTrackProcSpec->Scale(1. / fhSelRefTrackProcSpec->GetEntries());
    }

    if (fbAttachDutHitToTracklet) {
      if (fhSelTrklRefTrackProcSpec->GetEntries()) {
        fhSelTrklRefTrackProcSpec->LabelsDeflate("X");
        fhSelTrklRefTrackProcSpec->LabelsDeflate("Y");
        fhSelTrklRefTrackProcSpec->LabelsOption("<d", "X");
        fhSelTrklRefTrackProcSpec->LabelsOption("<v", "Y");
        fhSelTrklRefTrackProcSpec->Scale(
          1. / fhSelTrklRefTrackProcSpec->GetEntries());
      }
    }

    for (auto const& CounterModuleNode : fCounterModuleNodes) {
      auto const& CounterID = CounterModuleNode.first;

      tCurrentHistogram = fhDomTracksProcSpec.at(CounterID);
      if (tCurrentHistogram->GetEntries()) {
        tCurrentHistogram->LabelsDeflate("X");
        tCurrentHistogram->LabelsDeflate("Y");
        tCurrentHistogram->LabelsOption("<d", "X");
        tCurrentHistogram->LabelsOption("<v", "Y");
        tCurrentHistogram->Scale(1. / tCurrentHistogram->GetEntries());
      }

      tCurrentHistogram = fhDomTracksProcMat.at(CounterID);
      if (tCurrentHistogram->GetEntries()) {
        tCurrentHistogram->LabelsDeflate("X");
        tCurrentHistogram->LabelsDeflate("Y");
        tCurrentHistogram->LabelsOption("<d", "X");
        tCurrentHistogram->LabelsOption("<v", "Y");
        tCurrentHistogram->Scale(1. / tCurrentHistogram->GetEntries());
      }

      tCurrentHistogram = fhRndmTracksProcSpec.at(CounterID);
      if (tCurrentHistogram->GetEntries()) {
        tCurrentHistogram->LabelsDeflate("X");
        tCurrentHistogram->LabelsDeflate("Y");
        tCurrentHistogram->LabelsOption("<d", "X");
        tCurrentHistogram->LabelsOption("<v", "Y");
        tCurrentHistogram->Scale(1. / tCurrentHistogram->GetEntries());
      }

      tCurrentHistogram = fhRndmTracksProcMat.at(CounterID);
      if (tCurrentHistogram->GetEntries()) {
        tCurrentHistogram->LabelsDeflate("X");
        tCurrentHistogram->LabelsDeflate("Y");
        tCurrentHistogram->LabelsOption("<d", "X");
        tCurrentHistogram->LabelsOption("<v", "Y");
        tCurrentHistogram->Scale(1. / tCurrentHistogram->GetEntries());
      }
    }
  }


  WriteHistos();
  // Prevent them from being sucked in by the CbmHadronAnalysis WriteHistograms method
  DeleteHistos();
}

/************************************************************************************/
// Functions common for all clusters approximations
Bool_t CbmTofAnaTestbeam::RegisterInputs() {
  FairRootManager* fManager = FairRootManager::Instance();

  fEventsColl = dynamic_cast<TClonesArray*>(fManager->GetObject("Event"));
  if (NULL == fEventsColl)
    fEventsColl = dynamic_cast<TClonesArray*>(fManager->GetObject("CbmEvent"));

  if (NULL == fEventsColl)
    LOG(info) << "CbmEvent not found in input file, assume eventwise input";

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(error) << GetName() << ": No digi input!";
    return kFALSE;
  }


  if (!fEventsColl) {

    if (NULL == fTofDigisColl)
      fTofDigisColl = (TClonesArray*) fManager->GetObject("CbmTofDigi");

    if (NULL == fTofDigisColl)
      fTofDigisColl = (TClonesArray*) fManager->GetObject("TofCalDigi");

    if (NULL == fTofDigisColl) {
      LOG(WARNING)
        << "CbmTofAnaTestbeam::RegisterInputs => Could not get the TofDigi "
           "TClonesArray!!! ... continuing with incomplete input ";
      // return kFALSE;
    }  // if( NULL == fTofDigisColl)

    fTofHitsColl = (TClonesArray*) fManager->GetObject("TofHit");
    if (NULL == fTofHitsColl) {
      LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not get the "
                    "TofHit TClonesArray!!!";
      return kFALSE;
    }  // if( NULL == fTofHitsColl)

    fTofDigiMatchColl = (TClonesArray*) fManager->GetObject("TofHitCalDigiMatch");
    if (NULL == fTofDigiMatchColl) {
      LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not get the "
                    "Match TClonesArray!!!";
      return kFALSE;
    }  // if( NULL == fTofDigiMatchColl)


    fTofTrackColl = (TClonesArray*) fManager->GetObject("TofTracks");
    if (NULL == fTofTrackColl) {
      LOG(info) << "CbmTofAnaTestbeam::RegisterInputs => Could not get the "
                   "TofTracklet TClonesArray!!!";
      //      return kFALSE;
    }  // if( NULL == fTofHitsColl)

    fTrbHeader = (TTrbHeader*) fManager->GetObject("TofTrbHeader.");
    if (NULL == fTrbHeader) {
      LOG(info) << "CbmTofAnaTestbeam::RegisterInputs => Could not get the "
                   "TofTrbHeader Object";
    }

    fDigiMan = CbmDigiManager::Instance();
    fDigiMan->Init();
    if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
      LOG(error) << GetName() << ": No input digis!";
      //return kFALSE;
    } else
      LOG(info) << "DigiManager has Tof Digis";

    if (fbMonteCarloComparison) {
      // The input data arrays correspond to a timeslice.
      fMCEventList =
        dynamic_cast<CbmMCEventList*>(fManager->GetObject("MCEventList."));
      if (!fMCEventList) {
        // The input data arrays correspond to a reconstructed event.
        fMCEventList =
          dynamic_cast<CbmMCEventList*>(fManager->GetObject("EventList."));

        // TODO: For the time being, the following check prevents the MC comparison code
        //       from being executed when ordinary event-by-event input is being processed.
        //       Without a MC event list at hand (as is the case in event-based mode) it is
        //       not straightforward to find the position of the MC event header in the
        //       input chain.
        if (!fMCEventList) {
          LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not "
                        "retrieve branch \"(MC)EventList.\" for MC comparison.";
          return kFALSE;
        }
      }

      CbmMCDataManager* tMCManager =
        dynamic_cast<CbmMCDataManager*>(fManager->GetObject("MCDataManager"));
      if (!tMCManager) {
        LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not retrieve "
                      "CbmMCDataManager for MC comparison.";
        return kFALSE;
      }

      fMCEventHeader = tMCManager->GetObject("MCEventHeader.");
      if (!fMCEventHeader) {
        LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not retrieve "
                      "branch \"MCEventHeader.\" for MC comparison.";
        return kFALSE;
      }

      fTofPointsTB =
        dynamic_cast<TClonesArray*>(fManager->GetObject("TofPointTB"));
      if (fTofPointsTB) {
        fbPointsInInputFile = kTRUE;
      } else {
        fTofPoints = tMCManager->InitBranch("TofPoint");
        if (!fTofPoints) {
          LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not "
                        "retrieve branch \"TofPoint\" for MC comparison.";
          return kFALSE;
        }
      }

      fTofHitPointMatches =
        dynamic_cast<TClonesArray*>(fManager->GetObject("TofHitMatch"));
      if (!fTofHitPointMatches) {
        LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not retrieve "
                      "branch \"TofHitMatch\" for MC comparison.";
        return kFALSE;
      }

      fAccTracks =
        dynamic_cast<TClonesArray*>(fManager->GetObject("TofAccMCTracks"));
      if (fAccTracks) {
        fbTracksInInputFile = kTRUE;

        fTofAccTrackPointMatches = dynamic_cast<TClonesArray*>(
          fManager->GetObject("TofAccMCTrackPointMatch"));
        if (!fTofAccTrackPointMatches) {
          LOG(error)
            << "CbmTofAnaTestbeam::RegisterInputs => Could not retrieve branch "
               "\"TofAccMCTrackPointMatch\" for MC comparison.";
          return kFALSE;
        }

        fTofHitAccTrackMatches =
          dynamic_cast<TClonesArray*>(fManager->GetObject("TofHitTrackMatch"));
        if (!fTofHitAccTrackMatches) {
          LOG(error)
            << "CbmTofAnaTestbeam::RegisterInputs => Could not retrieve branch "
               "\"TofHitTrackMatch\" for MC comparison.";
          return kFALSE;
        }

        if (fTofTrackColl) {
          fTofTrackletAccTrackMatches =
            dynamic_cast<TClonesArray*>(fManager->GetObject("TofTrackMatch"));
          if (!fTofTrackletAccTrackMatches) {
            LOG(error)
              << "CbmTofAnaTestbeam::RegisterInputs => Could not retrieve "
                 "branch \"TofTrackMatch\" for MC comparison.";
            return kFALSE;
          }

          fTofAccTrackTrackletMatches = dynamic_cast<TClonesArray*>(
            fManager->GetObject("TofAccMCTrackMatch"));
          if (!fTofAccTrackTrackletMatches) {
            LOG(error)
              << "CbmTofAnaTestbeam::RegisterInputs => Could not retrieve "
                 "branch \"TofAccMCTrackMatch\" for MC comparison.";
            return kFALSE;
          }
        }
      } else {
        fMCTracks = tMCManager->InitBranch("MCTrack");
        if (!fMCTracks) {
          LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not "
                        "retrieve branch \"MCTrack\" for MC comparison.";
          return kFALSE;
        }
      }
    }

  } else {
    if (NULL == fTofDigisCollIn)
      fTofDigisCollIn = (TClonesArray*) fManager->GetObject("TofCalDigi");
    /*
     if( NULL == fTofDigisCollIn)
       fTofDigisCollIn   = (TClonesArray *) fManager->GetObject("CbmTofDigi");

     if( NULL == fTofDigisCollIn)
       fTofDigisCollIn = (TClonesArray *) fManager->GetObject("CbmTofDigi");

     if( NULL == fTofDigisCollIn)
       fTofDigisCollIn = (TClonesArray *) fManager->GetObject("TofDigiExp");

     if( NULL == fTofDigisCollIn)
       fTofDigisCollIn = (TClonesArray *) fManager->GetObject("TofDigi");
     */
    if (NULL == fTofDigisCollIn) {
      LOG(WARNING)
        << "CbmTofAnaTestbeam::RegisterInputs => Could not get the TofDigi "
           "TClonesArray!!! ... continuing with incomplete input ";
      //return kFALSE;
    }  // if( NULL == fTofDigisColl)

    fTofHitsCollIn = (TClonesArray*) fManager->GetObject("TofHit");
    if (NULL == fTofHitsCollIn) {
      LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not get the "
                    "TofHit TClonesArray!!!";
      return kFALSE;
    }  // if( NULL == fTofHitsColl)

    fTofDigiMatchCollIn = (TClonesArray*) fManager->GetObject("TofDigiMatch");
    if (NULL == fTofDigiMatchCollIn) {
      fTofDigiMatchCollIn = (TClonesArray*) fManager->GetObject("TofHitCalDigiMatch");
      if (NULL == fTofDigiMatchCollIn) {
        LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => Could not get the "
                      "Match TClonesArray!!!";
      }
    }  // if( NULL == fTofDigiMatchColl)

    fTofTrackCollIn = (TClonesArray*) fManager->GetObject("TofTracks");
    if (NULL == fTofTrackCollIn) {
      LOG(info) << "CbmTofAnaTestbeam::RegisterInputs => Could not get the "
                   "TofTracklet TClonesArray!!!";
      //      return kFALSE;
    }  // if( NULL == fTofHitsColl)

    fDigiMan = CbmDigiManager::Instance();
    fDigiMan->Init();
    if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
      LOG(error) << GetName() << ": No input digis!";
      return kFALSE;
    } else
      LOG(info) << "DigiManager has Tof Digis";

    // Create work Arrays
    fTofDigisColl     = new TClonesArray("CbmTofDigi", 100);
    fTofDigiMatchColl = new TClonesArray("CbmMatch", 100);
    fTofHitsColl      = new TClonesArray("CbmTofHit", 100);
    fTofTrackColl     = new TClonesArray("CbmTofTracklet", 100);

    if (fbMonteCarloComparison) {
      LOG(error) << "CbmTofAnaTestbeam::RegisterInputs => MC comparison not "
                    "compatible with 'CbmEvent' processing!!!";
      fbMonteCarloComparison = kFALSE;
    }
  }
  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofAnaTestbeam::InitParameters() {

  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  LOG(info) << "CbmTofAnaTestbeam::InitParameters - Geometry, Mapping, ... ";
  // Get Base Container
  FairRun* ana        = FairRun::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  Int_t iGeoVersion = fGeoHandler->Init(isSimulation);
  if (k14a > iGeoVersion) {}

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  if (0 == fDigiPar) {
    LOG(error) << "CbmTofAnaTestbeam::InitParameters => Could not obtain the "
                  "CbmTofDigiPar ";
    return kFALSE;
  }

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
  if (0 == fDigiBdfPar) {
    LOG(error) << "CbmTofAnaTestbeam::InitParameters => Could not obtain the "
                  "CbmTofDigiBdfPar ";
    return kFALSE;
  }

  // Mapping parameter
  /*
   fMbsMappingPar = (TMbsMappingTofPar*) (rtdb->getContainer("TMbsMappingTofPar"));
   if( 0 == fMbsMappingPar )
   {
      LOG(error)<<"CbmTofAnaTestBeam::InitParameters => Could not obtain the TMbsMappingTofPar ";
      return kFALSE;
   }
   */
  rtdb->initContainers(ana->GetRunId());

  LOG(info) << "CbmTofAnaTestbeam::InitParameter: currently "
            << fDigiPar->GetNrOfModules() << " digi cells ";

  // set defaults for backward compatibility to sep14
  if (0 == fiBeamRefAddr) {
    LOG(info)
      << "CbmTofAnaTestbeam::InitParameter: set beam ref to default (sep14) ";
    fiBeamRefSmType = 5;
    fiBeamRefSmId   = 1;
    fiBeamRefAddr =
      CbmTofAddress::GetUniqueAddress(fiBeamRefSmId, 0, 0, 0, fiBeamRefSmType);
  }
  LOG(info) << Form("CbmTofAnaTestbeam::InitParameter BeamRef = %d, %d, 0x%08x",
                    fiBeamRefSmType,
                    fiBeamRefSmId,
                    fiBeamRefAddr);

  if (0 == fdDTD4MAX) fdDTD4MAX = DTDMAX;

  if (0. == fdChi2Lim) fdChi2Lim = 10.;
  if (0. == fdChi2Lim2) fdChi2Lim2 = fdChi2Lim;
  LOG(info) << "CbmTofAnaTestbeam::InitParameter: Chi2 limits initialized to "
            << fdChi2Lim << " and " << fdChi2Lim2;
  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofAnaTestbeam::LoadCalParameter() {
  if (fCalParFileName.IsNull()) return kTRUE;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fCalParFile = new TFile(fCalParFileName, "");
  if (NULL == fCalParFile) {
    LOG(error) << "CbmTofAnaTestBeam::LoadCalParameter: "
               << "file " << fCalParFileName << " does not exist  ";
    return kTRUE;
  }

  LOG(info) << "CbmTofAnaTestBeam::LoadCalParameter: "
            << "read from file " << fCalParFileName;

  TProfile* fhtmp =
    (TProfile*) gDirectory->FindObjectAny(Form("hDTD4DT04D4best_pfx_px"));
  if (NULL == fhtmp) {
    if (!fbUseSigCalib) {
      fdChi2Lim  = fdChi2Lim * 100.;
      fdChi2Lim2 = fdChi2Lim2 * 100.;
    }
    LOG(info) << "Histo hDTD4DT04D4best_pfx_px not found => Chi2Lim = "
              << fdChi2Lim;
  }

  TProfile* fhtmpx =
    (TProfile*) gDirectory->FindObjectAny(Form("hDTX4D4best_pfx_px"));
  if (NULL == fhtmpx) {
    LOG(info) << " Histo " << Form("hDTX4D4best_pfx_px") << " not found. ";
  }

  TProfile* fhtmpy =
    (TProfile*) gDirectory->FindObjectAny(Form("hDTY4D4best_pfx_px"));
  if (NULL == fhtmpy) {
    LOG(info) << " Histo " << Form("hDTY4D4best_pfx_px") << " not found. ";
  }

  TProfile* fhtmpt =
    (TProfile*) gDirectory->FindObjectAny(Form("hTexpDT04D4best_pfx_px"));
  if (NULL == fhtmpt) {
    LOG(info) << " Histo " << Form("hTexpDT04D4best_pfx_px") << " not found. ";
  }

  TProfile* fhtmpcs0 =
    (TProfile*) gDirectory->FindObjectAny(Form("hCluSize0DT04D4best_pfx_px"));
  if (NULL == fhtmpcs0) {
    LOG(info) << " Histo " << Form("hCluSize0DT04D4best_pfx_px")
              << " not found. ";
  }

  TProfile* fhtmpcs4 =
    (TProfile*) gDirectory->FindObjectAny(Form("hCluSize4DT04D4best_pfx_px"));
  if (NULL == fhtmpcs4) {
    LOG(info) << " Histo " << Form("hCluSize4DT04D4best_pfx_px")
              << " not found. ";
  }

  TProfile* fhtmptot0 =
    (TProfile*) gDirectory->FindObjectAny(Form("hTot0DT04D4best_pfx_px"));
  if (NULL == fhtmptot0) {
    LOG(info) << " Histo " << Form("hTot0DT04D4best_pfx_px") << " not found. ";
  }

  TProfile* fhtmptot4 =
    (TProfile*) gDirectory->FindObjectAny(Form("hTot4DT04D4best_pfx_px"));
  if (NULL == fhtmptot4) {
    LOG(info) << " Histo " << Form("hTot4DT04D4best_pfx_px") << " not found. ";
  }

  TH1D* fhtmpstnnrt =
    (TH1D*) gDirectory->FindObjectAny(Form("hSelTypeNNResidualT_Width"));
  TH1D* fhtmpstnnrx =
    (TH1D*) gDirectory->FindObjectAny(Form("hSelTypeNNResidualX_Width"));
  TH1D* fhtmpstnnry =
    (TH1D*) gDirectory->FindObjectAny(Form("hSelTypeNNResidualY_Width"));
  TH1D* fhtmphtrxyt =
    (TH1D*) gDirectory->FindObjectAny(Form("hSelHitTupleResidualXYT_Width"));

  TH2D* fh2tmp = (TH2D*) gDirectory->FindObjectAny(Form("hDistDT04D4best"));
  if (NULL != fh2tmp) fdHitDistAv = fh2tmp->GetMean(1);
  if (fdHitDistAv <= 0.) fdHitDistAv = 1.;
  LOG(info) << "CbmTofAnaTestBeam::LoadCalParameter: average distance Dut - "
               "Ref initialized to "
            << Form("%5.1f", fdHitDistAv) << " cm ";

  gROOT->cd();
  if (NULL != fhtmp) fhDTD4DT04D4Off = (TH1D*) fhtmp->Clone();
  if (NULL != fhtmpx) fhDTX4D4Off = (TH1D*) fhtmpx->Clone();
  if (NULL != fhtmpy) fhDTY4D4Off = (TH1D*) fhtmpy->Clone();
  if (NULL != fhtmpt) fhDTTexpD4Off = (TH1D*) fhtmpt->Clone();
  if (NULL != fhtmpcs0) fhCluSize0DT04D4Off = (TH1D*) fhtmpcs0->Clone();
  if (NULL != fhtmpcs4) fhCluSize4DT04D4Off = (TH1D*) fhtmpcs4->Clone();
  if (NULL != fhtmptot0) fhTot0DT04D4Off = (TH1D*) fhtmptot0->Clone();
  if (NULL != fhtmptot4) fhTot4DT04D4Off = (TH1D*) fhtmptot4->Clone();


  if (NULL == fhtmpstnnrt) {
    LOG(info) << " Histo " << Form("hSelTypeNNResidualT_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) {
      fhSelTypeNNResidualT_Width = (TH1D*) fhtmpstnnrt->Clone();
    }
  }

  if (NULL == fhSelTypeNNResidualT_Width) {
    fhSelTypeNNResidualT_Width =
      new TH1F(Form("hSelTypeNNResidualT_Width"),
               Form("Sel-DUT ResiT Width vs SelType ; SelType ; RMS(T) (ns)"),
               3,
               0,
               3);

    fhSelTypeNNResidualT_Width->SetBinContent(1, fdDTWidth);
    fhSelTypeNNResidualT_Width->SetBinContent(2, fSIGT);
    fhSelTypeNNResidualT_Width->SetBinContent(3, fdMCSIGT);
  }

  if (NULL == fhtmpstnnrx) {
    LOG(info) << " Histo " << Form("hSelTypeNNResidualX_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) {
      fhSelTypeNNResidualX_Width = (TH1D*) fhtmpstnnrx->Clone();
    }
  }

  if (NULL == fhSelTypeNNResidualX_Width) {
    fhSelTypeNNResidualX_Width =
      new TH1F(Form("hSelTypeNNResidualX_Width"),
               Form("Sel-DUT ResiX Width vs SelType ; SelType ; RMS(X) (cm)"),
               3,
               0,
               3);

    fhSelTypeNNResidualX_Width->SetBinContent(1, fdDXWidth);
    fhSelTypeNNResidualX_Width->SetBinContent(2, fSIGX);
    fhSelTypeNNResidualX_Width->SetBinContent(3, fdMCSIGX);
  }

  if (NULL == fhtmpstnnry) {
    LOG(info) << " Histo " << Form("hSelTypeNNResidualY_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) {
      fhSelTypeNNResidualY_Width = (TH1D*) fhtmpstnnry->Clone();
    }
  }

  if (NULL == fhSelTypeNNResidualY_Width) {
    fhSelTypeNNResidualY_Width =
      new TH1F(Form("hSelTypeNNResidualY_Width"),
               Form("Sel-DUT ResiY Width vs SelType ; SelType ; RMS(Y) (cm)"),
               3,
               0,
               3);

    fhSelTypeNNResidualY_Width->SetBinContent(1, fdDYWidth);
    fhSelTypeNNResidualY_Width->SetBinContent(2, fSIGY);
    fhSelTypeNNResidualY_Width->SetBinContent(3, fdMCSIGY);
  }

  if (NULL == fhtmphtrxyt) {
    LOG(info) << " Histo " << Form("hSelHitTupleResidualXYT_Width")
              << " not found. ";
  } else {
    if (fbUseSigCalib) {
      fhSelHitTupleResidualXYT_Width = (TH1D*) fhtmphtrxyt->Clone();
    }
  }

  if (NULL == fhSelHitTupleResidualXYT_Width) {
    fhSelHitTupleResidualXYT_Width = new TH1F(
      Form("hSelHitTupleResidualXYT_Width"),
      Form("Sel hit tuple Resi XYT Width; X/Y/T [] ; RMS(X/Y/T) (cm/cm/ns)"),
      3,
      0,
      3);

    fhSelHitTupleResidualXYT_Width->SetBinContent(1, fdDXWidth);
    fhSelHitTupleResidualXYT_Width->SetBinContent(2, fdDYWidth);
    fhSelHitTupleResidualXYT_Width->SetBinContent(3, fdDTWidth);
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  fCalParFile->Close();
  //    fhDTD4DT04D4Off->Draw();

  if (fdDCh4Sel == 0.) fdDCh4Sel = 1000.;  // open default window
  if (fdPosY4Sel == 0.) fdPosY4Sel = 10.;  // open default window

  return kTRUE;
}
/************************************************************************************/
// ------------------------------------------------------------------
Bool_t CbmTofAnaTestbeam::CreateHistos() {
  // Create histogramms
  Double_t XDMAX = 300.;
  Double_t YDMAX = 300.;
  Double_t TDMAX = 1000.;
  Double_t DTMAX = 1.;
  Double_t DXMAX = 10.;
  Double_t DYMAX = 10.;
  /*   Double_t XMAX=100.;*/
  /*   Double_t YMAX=100.;*/

  TDirectory* oldir =
    gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT
    ->cd();  // <= To prevent histos from being sucked in by the param file of the TRootManager !

  // define histos here
  Double_t TISmax   = 11.;   //fdSpillDuration;
  Double_t TISnbins = 110.;  //50.;
  fhRate_all =
    new TH1F("hRate_all", "Event Rate; Rate (Hz); t (sec)", 1000, 0, 100);
  fhTriggerPattern =
    new TH1I("tof_trb_trigger_pattern", "CTS trigger pattern", 16, 0, 16);
  fhTriggerType =
    new TH1I("tof_trb_trigger_types", "CTS trigger types", 16, 0, 16);
  fhTimeInSpill =
    new TH1I("tof_trb_time_in_spill", "Time in Spill", TISnbins, 0, TISmax);
  fhTIS_all =
    new TH1F("TIS_all", "Time in Spill (all); t (sec)", TISnbins, 0, TISmax);
  fhTIS_sel =
    new TH1F("TIS_sel", "Time in Spill (sel); t (sec)", TISnbins, 0, TISmax);
  fhTIS_sel1 =
    new TH1F("TIS_sel1", "Time in Spill (sel1); t (sec)", TISnbins, 0, TISmax);
  fhTIS_sel2 =
    new TH1F("TIS_sel2", "Time in Spill (sel2); t (sec)", TISnbins, 0, TISmax);


  Double_t TIRmax   = 1800.;  // Run duration
  Double_t TIRnbins = 18000.;
  fhTIR_all =
    new TH1F("TIR_all", "Time in Run (all); t (sec)", TIRnbins, 0, TIRmax);
  fhTIR_sel =
    new TH1F("TIR_sel", "Time in Run (sel); t (sec)", TIRnbins, 0, TIRmax);
  fhTIR_sel1 =
    new TH1F("TIR_sel1", "Time in Run (sel1); t (sec)", TIRnbins, 0, TIRmax);
  fhTIR_sel2 =
    new TH1F("TIR_sel2", "Time in Run (sel2); t (sec)", TIRnbins, 0, TIRmax);

  Double_t TISmax2   = 11.;
  Double_t TISnbins2 = 110.;
  fhTIS_Nhit         = new TH2F("TIS_Nhit",
                        "Time in Spill (Nhit); t (sec); N_{hit}",
                        TISnbins2,
                        0,
                        TISmax2,
                        25,
                        0,
                        50);
  fhTIS_Ntrk         = new TH2F("TIS_Ntrk",
                        "Time in Spill (Ntrk); t (sec); N_{trk}",
                        TISnbins2,
                        0,
                        TISmax2,
                        10,
                        0,
                        10);

  fhDTLH_all =
    new TH1F("hDTLH_all",
             "Time to last hit in Dut(all); log( #DeltaT (ns)); counts",
             100.,
             0.,
             12.);
  fhDTLH_sel =
    new TH1F("hDTLH_sel",
             "Time to last hit in Dut(sel); log( #DeltaT (ns)); counts",
             100.,
             0.,
             12.);
  fhDTLH_sel1 =
    new TH1F("hDTLH_sel1",
             "Time to last hit in Dut(sel1); log( #DeltaT (ns)); counts",
             100.,
             0.,
             12.);
  fhDTLH_sel2 =
    new TH1F("hDTLH_sel2",
             "Time to last hit in Dut(sel2); log( #DeltaT (ns)); counts",
             100.,
             0.,
             12.);
  fhDTLH_DStrip = new TH2F("hDTLH_DStrip",
                           "Time to last hit strip matching Dut(sel1); strip ; "
                           "Channel-PredictedChannel",
                           32,
                           0,
                           31,
                           7.,
                           -3.5,
                           3.5);

  fhBRefMul = new TH1F(Form("hBRefMul"),
                       Form("Multiplicity in Beam Reference counter ; Mul ()"),
                       50,
                       0.,
                       50.);
  fhDTD4    = new TH1F(
    Form("hDTD4"), Form("reference time ; #Delta tD4 (ns)"), 101, -100., 100.);

  Int_t iNbDet = fDigiBdfPar->GetNbDet();  //fMbsMappingPar->GetNbMappedDet();
  fhXYPos.resize(iNbDet);
  for (Int_t iDet = 0; iDet < iNbDet; iDet++) {
    fhXYPos[iDet] =
      new TH2F(Form("hXY_SmT%d", iDet),
               Form("XY Position correlation of Det# %d; X[cm]; Y [cm]", iDet),
               100,
               -YDMAX / 2.,
               YDMAX / 2.,
               100,
               -YDMAX,
               0.);
  }

  fhEtaPhi = new TH2F(Form("hEtaPhi"),
                      Form("Reconstructed Tof Hits; #phi (degree) ; #eta"),
                      200,
                      -20.,
                      20.,
                      200,
                      1.,
                      1.7);

  fhXX2 = new TH2F(Form("hXX2"),
                   Form("Plastic position correlation; XPla0; XPla1"),
                   100,
                   -XDMAX,
                   XDMAX,
                   100,
                   -XDMAX,
                   XDMAX);
  fhYY2 = new TH2F(Form("hYY2"),
                   Form("Plastic position correlation; YPla0; YPla1"),
                   100,
                   -YDMAX,
                   YDMAX,
                   100,
                   -YDMAX,
                   YDMAX);
  fhDT2 = new TH1F(Form("hDT2"),
                   Form("Plastic time difference; TPla0 - TPla1 (ns)"),
                   100,
                   -1.,
                   1.);

  for (Int_t iDet = 0; iDet < 2; iDet++) {
    fhXX02[iDet] = new TH2F(Form("hXX02_%d", iDet),
                            Form("X Position correlation; X0; X2"),
                            100,
                            -YDMAX,
                            YDMAX,
                            100,
                            -YDMAX,
                            YDMAX);
    fhYY02[iDet] = new TH2F(Form("hYY02_%d", iDet),
                            Form("Y Position correlation; Y0; Y2"),
                            100,
                            -YDMAX,
                            YDMAX,
                            100,
                            -YDMAX,
                            YDMAX);
  }

  const Double_t HTMAX = 100.;
  const Double_t HYMAX = 40.;
  fhDutDXDYDT =
    new TH3F(Form("hDutDXDYDT"),
             Form("Hits distances; #DeltaX [cm]; #DeltaY [cm];  #DeltaT [ns]"),
             101,
             -HYMAX,
             HYMAX,
             101,
             -HYMAX,
             HYMAX,
             101,
             -HTMAX,
             HTMAX);
  fhRefDXDYDT =
    new TH3F(Form("hRefDXDYDT"),
             Form("Hits distances; #DeltaX [cm]; #DeltaY [cm];  #DeltaT [ns]"),
             101,
             -HYMAX,
             HYMAX,
             101,
             -HYMAX,
             HYMAX,
             101,
             -HTMAX,
             HTMAX);

  fhXX04   = new TH2F(Form("hXX04"),
                    Form("X Position correlation; X0 [cm]; X4 [cm]"),
                    500,
                    -YDMAX,
                    YDMAX,
                    500,
                    -YDMAX,
                    YDMAX);
  fhYY04   = new TH2F(Form("hYY04"),
                    Form("Y Position correlation; Y0 [cm]; Y4 [cm]"),
                    500,
                    -YDMAX,
                    YDMAX,
                    500,
                    -YDMAX,
                    YDMAX);
  fhXY04   = new TH2F(Form("hXY04"),
                    Form("X Position correlation; X0 [cm]; Y4 [cm]"),
                    500,
                    -YDMAX,
                    YDMAX,
                    500,
                    -YDMAX,
                    YDMAX);
  fhYX04   = new TH2F(Form("hYX04"),
                    Form("Y Position correlation; Y0 [cm]; X4 [cm]"),
                    500,
                    -YDMAX,
                    YDMAX,
                    500,
                    -YDMAX,
                    YDMAX);
  fhTT04   = new TH2F(Form("hTT04"),
                    Form("Time  correlation; T0 [ns]; T0 - T4 [ns]"),
                    100,
                    -TDMAX,
                    TDMAX,
                    100,
                    -1.,
                    1.);
  fhDXDY04 = new TH2F(Form("hDXDY04"),
                      Form("position correlation; #Delta x [cm]; #DeltaY [cm]"),
                      100,
                      -DXMAX,
                      DXMAX,
                      100,
                      -DYMAX,
                      DYMAX);
  fhDXDT04 =
    new TH2F(Form("hDXDT04"),
             Form("Time - position correlation; #Delta x [cm]; #DeltaT [ns]"),
             100,
             -DXMAX,
             DXMAX,
             100,
             -DTMAX,
             DTMAX);
  fhDYDT04 =
    new TH2F(Form("hDYDT04"),
             Form("Time - position correlation; #Delta y [cm]; #DeltaT [ns]"),
             100,
             -DYMAX,
             DYMAX,
             100,
             -DTMAX,
             DTMAX);
  fhChi04 = new TH1F(
    Form("hChi04"), Form("Matching Chi2; #chi; Nhits"), 100, 0., fdChi2Lim);
  fhChiSel24     = new TH1F(Form("hChiSel24"),
                        Form("Matching Chi2S24; #chi; Nhits"),
                        100,
                        0.,
                        fdChi2Lim2);
  fhDXSel24      = new TH1F(Form("hDXSel24"),
                       Form("Matching Sel24; #Delta x [cm]; Nhits"),
                       100,
                       -10.,
                       10.);
  fhDYSel24      = new TH1F(Form("hDYSel24"),
                       Form("Matching Sel24; #Delta y [cm]; Nhits"),
                       100,
                       -10.,
                       10.);
  fhDTSel24      = new TH1F(Form("hDTSel24"),
                       Form("Matching Sel24; #Delta t_{cor} [ns]; Nhits"),
                       100,
                       -1.,
                       1.);
  fhTofSel24     = new TH1F(Form("hTofSel24"),
                        Form("Matching Sel24; #Delta t [ns]; Nhits"),
                        100,
                        -10.,
                        10.);
  Int_t iNbinXY  = 34;
  fhXY0D4best    = new TH2F(Form("hXY0D4best"),
                         Form("local position 0;  x [cm]; y [cm]"),
                         iNbinXY,
                         -17.,
                         17.,
                         iNbinXY,
                         -17.,
                         17.);
  fhXY4D4best    = new TH2F(Form("hXY4D4best"),
                         Form("local position 4;  x [cm]; y [cm]"),
                         iNbinXY,
                         -17.,
                         17.,
                         iNbinXY,
                         -17.,
                         17.);
  fhXX04D4best   = new TH2F(Form("hXX04D4best"),
                          Form("local x position 0-4; x0 [cm]; x4 [cm]"),
                          iNbinXY,
                          -17.,
                          17.,
                          iNbinXY,
                          -17.,
                          17.);
  fhYY04D4best   = new TH2F(Form("hYY04D4best"),
                          Form("local y position 0-4; y0 [cm]; y4 [cm]"),
                          iNbinXY,
                          -17.,
                          17.,
                          iNbinXY,
                          -17.,
                          17.);
  fhXYSel2D4best = new TH2F(Form("hXYSel2D4best"),
                            Form("local position Sel2;  x [cm]; y [cm]"),
                            iNbinXY,
                            -17.,
                            17.,
                            iNbinXY,
                            -17.,
                            17.);
  fhXY0D4sel     = new TH2F(Form("hXY0D4sel"),
                        Form("predicted position 0;  x [cm]; y [cm]"),
                        iNbinXY,
                        -17.,
                        17.,
                        iNbinXY,
                        -17.,
                        17.);
  fhXY4D4sel     = new TH2F(Form("hXY4D4sel"),
                        Form("selected position 4;  x [cm]; y [cm]"),
                        iNbinXY,
                        -17.,
                        17.,
                        iNbinXY,
                        -17.,
                        17.);
  fhXYSel2D4sel  = new TH2F(Form("hXYSel2D4sel"),
                           Form("selected position Sel2;  x [cm]; y [cm]"),
                           iNbinXY,
                           -17.,
                           17.,
                           iNbinXY,
                           -17.,
                           17.);

  fhDXDY04best =
    new TH2F(Form("hDXDY04best"),
             Form("position correlation; #Delta x [cm]; #DeltaY [cm]"),
             100,
             -DXMAX,
             DXMAX,
             100,
             -DYMAX,
             DYMAX);
  fhDXDT04best =
    new TH2F(Form("hDXDT04best"),
             Form("time - position correlation; #Delta x [cm]; #DeltaT [ns]"),
             100,
             -DXMAX,
             DXMAX,
             100,
             -DTMAX,
             DTMAX);
  fhDYDT04best =
    new TH2F(Form("hDYDT04best"),
             Form("time - position correlation; #Delta y [cm]; #DeltaT [ns]"),
             100,
             -DYMAX,
             DYMAX,
             100,
             -DTMAX,
             DTMAX);

  fhX0DT04best =
    new TH2F(Form("hX0DT04best"),
             Form("time - position correlation; #Delta x [cm]; #DeltaT [ns]"),
             100,
             -50.,
             50.,
             100,
             -DTMAX,
             DTMAX);
  fhY0DT04best =
    new TH2F(Form("hY0DT04best"),
             Form("time - position correlation; #Delta y [cm]; #DeltaT [ns]"),
             100,
             -50.,
             50.,
             100,
             -DTMAX,
             DTMAX);

  fhChi04best = new TH1F(
    Form("hChi04best"), Form("matching chi2; #chi; Nhits"), 100, 0., fdChi2Lim);
  fhDigiMul0best = new TH1F(Form("hDigiMul0best"),
                            Form("Number of digis in cluster; N_{digi}; "),
                            40,
                            0.,
                            40.);
  fhDigiMul4best = new TH1F(Form("hDigiMul4best"),
                            Form("Number of digis in cluster; N_{digi}; "),
                            40,
                            0.,
                            40.);

  fhChiDT04best = new TH2F(Form("hChiDT04best"),
                           Form("Time - Chi correlation; #chi; #DeltaT [ns]"),
                           100,
                           0.,
                           100.,
                           100,
                           -DTMAX,
                           DTMAX);

  fhDXDY04D4best =
    new TH2F(Form("hDXDY04D4best"),
             Form("position correlation; #Delta x [cm]; #DeltaY [cm]"),
             100,
             -DXMAX,
             DXMAX,
             100,
             -DYMAX,
             DYMAX);
  fhDXDT04D4best =
    new TH2F(Form("hDXDT04D4best"),
             Form("time - position correlation; #Delta x [cm]; #DeltaT [ns]"),
             100,
             -DXMAX,
             DXMAX,
             100,
             -DTMAX,
             DTMAX);
  fhDYDT04D4best =
    new TH2F(Form("hDYDT04D4best"),
             Form("time - position correlation; #Delta y [cm]; #DeltaT [ns]"),
             100,
             -DYMAX,
             DYMAX,
             100,
             -DTMAX,
             DTMAX);
  fhDistDT04D4best =
    new TH2F(Form("hDistDT04D4best"),
             Form("time - distance correlation; Dist [cm]; #DeltaT [ns]"),
             100,
             fdHitDistMin,
             fdHitDistMin + 30.,
             100,
             -DTMAX,
             DTMAX);
  fhTexpDT04D4best = new TH2F(
    Form("hTexpDT04D4best"),
    Form("measured - expected time - correlation; Texp [ns]; #DeltaT [ns]"),
    100,
    0.,
    1.,
    100,
    -DTMAX,
    DTMAX);
  fhCluSize0DT04D4best =
    new TH2F(Form("hCluSize0DT04D4best"),
             Form("time - CluSize correlation; M_{strips} ; #DeltaT [ns]"),
             20,
             0.5,
             20.5,
             100,
             -DTMAX,
             DTMAX);
  fhCluSize4DT04D4best =
    new TH2F(Form("hCluSize4DT04D4best"),
             Form("time - CluSize correlation; M_{strips} ; #DeltaT [ns]"),
             20,
             0.5,
             20.5,
             100,
             -DTMAX,
             DTMAX);
  fhCluSizeSigT0D4best = new TH2F(
    Form("hCluSizeSigT0D4best"),
    Form("time spread - CluSize correlation; M_{strips} ; #sigma_{T} [ns]"),
    20,
    0.5,
    20.5,
    100,
    0.,
    DTMAX / 5.);
  fhCluSizeSigT4D4best = new TH2F(
    Form("hCluSizeSigT4D4best"),
    Form("time spread - CluSize correlation; M_{strips} ; #sigma_{T} [ns]"),
    20,
    0.5,
    20.5,
    100,
    0.,
    DTMAX / 5.);
  fhTot0DT04D4best =
    new TH2F(Form("hTot0DT04D4best"),
             Form("time - Tot correlation; ln TOT0 ; #DeltaT [ns]"),
             100,
             -2.5,
             3.0,
             100,
             -DTMAX,
             DTMAX);
  fhTot4DT04D4best =
    new TH2F(Form("hTot4DT04D4best"),
             Form("time - Tot correlation; ln TOT4 ; #DeltaT [ns]"),
             100,
             -2.5,
             3.0,
             100,
             -DTMAX,
             DTMAX);

  fhX0DT04D4best =
    new TH2F(Form("hX0DT04D4best"),
             Form("time - position correlation; #Delta x [cm]; #DeltaT [ns]"),
             100,
             -50.,
             50.,
             100,
             -DTMAX,
             DTMAX);
  fhY0DT04D4best =
    new TH2F(Form("hY0DT04D4best"),
             Form("time - position correlation; #Delta y [cm]; #DeltaT [ns]"),
             100,
             -50.,
             50.,
             100,
             -DTMAX,
             DTMAX);

  fhTISDT04D4best =
    new TH2F(Form("hTISDT04D4best"),
             Form("time - TIS; time in spill  [s]; #DeltaT [ns]"),
             TISnbins,
             0.,
             TISmax,
             100,
             -DTMAX,
             DTMAX);

  fhChi04D4best = new TH1F(Form("hChi04D4best"),
                           Form("matching chi2; #chi; Nhits"),
                           100,
                           0.,
                           fdChi2Lim);
  fhTofD4best =
    new TH1F(Form("hTofD4best"), Form("tof D4; t [ns]; Counts"), 100, 0., 50.);
  fhVelD4best = new TH1F(
    Form("hVelD4best"), Form("vel D4; v [cm/ns]; Counts"), 100, 0., 50.);

  fhDigiMul0D4best =
    new TH2F(Form("hDigiMul0D4best"),
             Form("Number of digis in cluster; Mul0; N_{strips}; "),
             40,
             0.,
             40.,
             40,
             0.,
             40.);
  fhDigiMul4D4best =
    new TH2F(Form("hDigiMul4D4best"),
             Form("Number of digis in cluster; Mul4; N_{strips}; "),
             40,
             0.,
             40.,
             40,
             0.,
             40.);
  fhCluSize04D4best =
    new TH2F(Form("hCluSize04D4best"),
             Form("cluster multiplicity ; CluSize0; CluSize4"),
             40,
             0.,
             40.,
             40,
             0.,
             40.);
  fhCluMul04D4best = new TH2F(Form("hCluMul04D4best"),
                              Form("cluster size correlation ; Mul0; Mul4"),
                              40,
                              0.,
                              40.,
                              20,
                              0.,
                              20.);
  fhStrMul04D4best = new TH2F(Form("hStrMul04D4best"),
                              Form("strip multiplicity ; StrMul0; StrMul4"),
                              50,
                              0.,
                              50.,
                              50,
                              0.,
                              50.);
  fhCluMulTSig0D4best =
    new TH2F(Form("hCluMulTSig0D4best"),
             Form("time spread - cluster multiplicity ; Mul0; #sigma_{T} (ns)"),
             50,
             0.,
             50.,
             100,
             0.,
             200.);
  fhCluMulTSig4D4best =
    new TH2F(Form("hCluMulTSig4D4best"),
             Form("time spread - cluster multiplicity ; Mul4; #sigma_{T} (ns)"),
             50,
             0.,
             50.,
             100,
             0.,
             200.);
  fhCluMulTrel0D4best =
    new TH2F(Form("hCluMulTrel0D4best"),
             Form("arrrival time - cluster multiplicity ; Mul0; T_{rel} (ns)"),
             50,
             0.,
             50.,
             99,
             -10.,
             10.);
  fhCluMulTrel4D4best =
    new TH2F(Form("hCluMulTrel4D4best"),
             Form("arrival time - cluster multiplicity ; Mul4; T_{rel} (ns)"),
             50,
             0.,
             50.,
             99,
             -10.,
             10.);
  fhCluSizeTrel0D4best =
    new TH2F(Form("hCluSizeTrel0D4best"),
             Form("arrrival time - cluster size ; CluSize0; T_{rel} (ns)"),
             30,
             0.,
             30.,
             99,
             -10.,
             10.);
  fhCluSizeTrel4D4best =
    new TH2F(Form("hCluSizeTrel4D4best"),
             Form("arrival time - cluster size ; CluSize4; T_{rel} (ns)"),
             30,
             0.,
             30.,
             99,
             -10.,
             10.);

  fhChiDT04D4best =
    new TH2F(Form("hChiDT04D4best"),
             Form("Time - position correlation; #chi; #DeltaT [ns]"),
             100,
             0.,
             100.,
             100,
             -DTMAX,
             DTMAX);
  Double_t dtscal = 5.;
  if (fdChi2Lim > 100.) dtscal *= 2.;
  fhDTD4DT04D4best = new TH2F(
    Form("hDTD4DT04D4best"),
    Form("Time - velocity correlation; #DeltaTD4 [ns]; #DeltaT04 [ns]"),
    100,
    -DTMAX * 6.,
    DTMAX * 6.,
    500,
    -DTMAX * dtscal,
    DTMAX * dtscal);

  /*     Double_t dXMAX=30.;*/
  /*     Double_t dYMAX=20.;*/
  fhDTMul4D4best = new TH2F(
    Form("hDTMul4D4best"),
    Form("MRef Time - Multiplicity correlation; Mul4 ; #DeltaT04 [ns]"),
    20,
    0.,
    20.,
    100,
    -DTMAX,
    DTMAX);
  fhDTX4D4best =
    new TH2F(Form("hDTX4D4best"),
             Form("MRef Time - position correlation; X4 [cm]; #DeltaT04 [ns]"),
             50,
             -17.,
             17.,
             100,
             -DTMAX,
             DTMAX);
  fhDTY4D4best =
    new TH2F(Form("hDTY4D4best"),
             Form("MRef Time - position correlation; Y4 [cm]; #DeltaT04 [ns]"),
             50,
             -15.,
             15.,
             100,
             -DTMAX,
             DTMAX);
  fhDXX4D4best =
    new TH2F(Form("hDXX4D4best"),
             Form("MRef DX - position correlation; X4 [cm]; #DeltaX04 [cm]"),
             50,
             -17.,
             17.,
             50,
             -10.,
             10.);
  fhDXY4D4best =
    new TH2F(Form("hDXY4D4best"),
             Form("MRef DX - position correlation; Y4 [cm]; #DeltaX04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -10.,
             10.);
  fhDYX4D4best =
    new TH2F(Form("hDYX4D4best"),
             Form("MRef DY - position correlation; X4 [cm]; #DeltaY04 [cm]"),
             50,
             -17.,
             17.,
             50,
             -10.,
             10.);
  fhDYY4D4best =
    new TH2F(Form("hDYY4D4best"),
             Form("MRef DY - position correlation; Y4 [cm]; #DeltaY04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -10.,
             10.);

  fhDTMul0D4best =
    new TH2F(Form("hDTMul0D4best"),
             Form("Dut Time - Multiplicity correlation; Mul0 ; #DeltaT04 [ns]"),
             40,
             0.,
             40.,
             100,
             -DTMAX,
             DTMAX);
  fhDTX0D4best =
    new TH2F(Form("hDTX0D4best"),
             Form("Dut Time - position correlation; X0 [cm]; #DeltaT04 [ns]"),
             50,
             -17.,
             17.,
             100,
             -DTMAX,
             DTMAX);
  fhDTY0D4best =
    new TH2F(Form("hDTY0D4best"),
             Form("Dut Time - position correlation; Y0 [cm]; #DeltaT04 [ns]"),
             50,
             -15.,
             15.,
             100,
             -DTMAX,
             DTMAX);
  fhDXX0D4best =
    new TH2F(Form("hDXX0D4best"),
             Form("Dut DX - position correlation; X0 [cm]; #DeltaX04 [cm]"),
             50,
             -17.,
             17.,
             50,
             -15.,
             15.);
  fhDXY0D4best =
    new TH2F(Form("hDXY0D4best"),
             Form("Dut DX - position correlation; Y0 [cm]; #DeltaX04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -15.,
             15.);
  fhDYX0D4best =
    new TH2F(Form("hDYX0D4best"),
             Form("Dut DY - position correlation; X0 [cm]; #DeltaY04 [cm]"),
             50,
             -17.,
             17.,
             50,
             -15.,
             15.);
  fhDYY0D4best =
    new TH2F(Form("hDYY0D4best"),
             Form("Dut DY - position correlation; Y0 [cm]; #DeltaY04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -15.,
             15.);


  fhX0DT04D4sbest =
    new TH2F(Form("hX0DT04D4sbest"),
             Form("time - position correlation; #Delta x [cm]; #DeltaT [ns]"),
             100,
             -50.,
             50.,
             100,
             -DTMAX,
             DTMAX);
  fhY0DT04D4sbest =
    new TH2F(Form("hY0DT04D4sbest"),
             Form("time - position correlation; #Delta y [cm]; #DeltaT [ns]"),
             100,
             -50.,
             50.,
             100,
             -DTMAX,
             DTMAX);

  fhDXDY04D4sbest =
    new TH2F(Form("hDXDY04D4sbest"),
             Form("position correlation; #Delta x [cm]; #DeltaY [cm]"),
             100,
             -DXMAX,
             DXMAX,
             100,
             -DYMAX,
             DYMAX);
  fhDXDT04D4sbest =
    new TH2F(Form("hDXDT04D4sbest"),
             Form("time - position correlation; #Delta x [cm]; #DeltaT [ns]"),
             100,
             -DXMAX,
             DXMAX,
             100,
             -DTMAX,
             DTMAX);
  fhDYDT04D4sbest =
    new TH2F(Form("hDYDT04D4sbest"),
             Form("time - position correlation; #Delta y [cm]; #DeltaT [ns]"),
             100,
             -DYMAX,
             DYMAX,
             100,
             -DTMAX,
             DTMAX);
  fhDistDT04D4sbest =
    new TH2F(Form("hDistDT04D4sbest"),
             Form("time - distance correlation; Dist [cm]; #DeltaT [ns]"),
             100,
             fdHitDistMin,
             fdHitDistMin + 30.,
             100,
             -DTMAX,
             DTMAX);
  fhTexpDT04D4sbest = new TH2F(
    Form("hTexpDT04D4sbest"),
    Form("measured - expected time - correlation; Texp [ns]; #DeltaT [ns]"),
    100,
    0.,
    6.,
    100,
    -DTMAX,
    DTMAX);
  fhCluSize0DT04D4sbest =
    new TH2F(Form("hCluSize0DT04D4sbest"),
             Form("time - CluSize correlation; N_{strips} ; #DeltaT [ns]"),
             20,
             0.5,
             20.5,
             100,
             -DTMAX,
             DTMAX);
  fhCluSize4DT04D4sbest =
    new TH2F(Form("hCluSize4DT04D4sbest"),
             Form("time - CluSize correlation; N_{strips} ; #DeltaT [ns]"),
             20,
             0.5,
             20.5,
             100,
             -DTMAX,
             DTMAX);
  fhTot0DT04D4sbest =
    new TH2F(Form("hTot0DT04D4sbest"),
             Form("time - Tot correlation; ln TOT0 ; #DeltaT [ns]"),
             100,
             6.5,
             9.5,
             100,
             -DTMAX,
             DTMAX);
  fhTot4DT04D4sbest =
    new TH2F(Form("hTot4DT04D4sbest"),
             Form("time - Tot correlation; ln TOT4 ; #DeltaT [ns]"),
             100,
             6.5,
             9.5,
             100,
             -DTMAX,
             DTMAX);

  fhChi04D4sbest = new TH1F(Form("hChi04D4sbest"),
                            Form("matching chi2; #chi; Nhits"),
                            100,
                            0.,
                            fdChi2Lim);
  fhTofD4sbest =
    new TH1F(Form("hTofD4sbest"), Form("tof D4; t [ns]; Counts"), 100, 0., 50.);
  fhVelD4sbest = new TH1F(
    Form("hVelD4sbest"), Form("vel D4; v [cm/ns]; Counts"), 100, 0., 50.);

  fhDigiMul0D4sbest = new TH1F(Form("hDigiMul0D4sbest"),
                               Form("Number of digis in cluster; N_{digi}; "),
                               20,
                               0.,
                               20.);
  fhDigiMul4D4sbest = new TH1F(Form("hDigiMul4D4sbest"),
                               Form("Number of digis in cluster; N_{digi}; "),
                               20,
                               0.,
                               20.);
  fhCluMul04D4sbest = new TH2F(Form("hCluMul04D4sbest"),
                               Form("cluster multiplicity ; Mul0; Mul4"),
                               10,
                               0.,
                               10.,
                               10,
                               0.,
                               10.);

  fhChiDT04D4sbest =
    new TH2F(Form("hChiDT04D4sbest"),
             Form("Time - position correlation; #chi; #DeltaT [ns]"),
             100,
             0.,
             100.,
             100,
             -DTMAX,
             DTMAX);

  fhDTD4DT04D4sbest = new TH2F(
    Form("hDTD4DT04D4sbest"),
    Form("Time - velocity correlation; #DeltaTD4 [ns]; #DeltaT04 [ns]"),
    100,
    -DTMAX * 6.,
    DTMAX * 6.,
    100,
    -DTMAX,
    DTMAX);

  fhDTMul4D4sbest =
    new TH2F(Form("hDTMul4D4sbest"),
             Form("Time - Multiplicity correlation; Mul4 ; #DeltaT04 [ns]"),
             10,
             0.,
             10.,
             100,
             -DTMAX,
             DTMAX);
  fhDTX4D4sbest =
    new TH2F(Form("hDTX4D4sbest"),
             Form("Time - position correlation; X4 [cm]; #DeltaT04 [ns]"),
             50,
             -15.,
             15.,
             100,
             -DTMAX,
             DTMAX);
  fhDTY4D4sbest =
    new TH2F(Form("hDTY4D4sbest"),
             Form("Time - position correlation; Y4 [cm]; #DeltaT04 [ns]"),
             50,
             -15.,
             15.,
             100,
             -DTMAX,
             DTMAX);
  fhDXX4D4sbest =
    new TH2F(Form("hDXX4D4sbest"),
             Form("DX - position correlation; X4 [cm]; #DeltaX04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -10.,
             10.);
  fhDXY4D4sbest =
    new TH2F(Form("hDXY4D4sbest"),
             Form("DX - position correlation; Y4 [cm]; #DeltaX04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -10.,
             10.);
  fhDYX4D4sbest =
    new TH2F(Form("hDYX4D4sbest"),
             Form("DY - position correlation; X4 [cm]; #DeltaY04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -10.,
             10.);
  fhDYY4D4sbest =
    new TH2F(Form("hDYY4D4sbest"),
             Form("DY - position correlation; Y4 [cm]; #DeltaY04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -10.,
             10.);

  fhDTMul0D4sbest =
    new TH2F(Form("hDTMul0D4sbest"),
             Form("Time - Multiplicity correlation; Mul0 ; #DeltaT04 [ns]"),
             10,
             0.,
             10.,
             100,
             -DTMAX,
             DTMAX);
  fhDTX0D4sbest =
    new TH2F(Form("hDTX0D4sbest"),
             Form("Time - position correlation; X0 [cm]; #DeltaT04 [ns]"),
             50,
             -17.,
             17.,
             100,
             -DTMAX,
             DTMAX);
  fhDTY0D4sbest =
    new TH2F(Form("hDTY0D4sbest"),
             Form("Time - position correlation; Y0 [cm]; #DeltaT04 [ns]"),
             50,
             -15.,
             15.,
             100,
             -DTMAX,
             DTMAX);
  fhDXX0D4sbest =
    new TH2F(Form("hDXX0D4sbest"),
             Form("DX - position correlation; X0 [cm]; #DeltaX04 [cm]"),
             50,
             -17.,
             17.,
             50,
             -15.,
             15.);
  fhDXY0D4sbest =
    new TH2F(Form("hDXY0D4sbest"),
             Form("DX - position correlation; Y0 [cm]; #DeltaX04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -15.,
             15.);
  fhDYX0D4sbest =
    new TH2F(Form("hDYX0D4sbest"),
             Form("DY - position correlation; X0 [cm]; #DeltaY04 [cm]"),
             50,
             -17.,
             17.,
             50,
             -15.,
             15.);
  fhDYY0D4sbest =
    new TH2F(Form("hDYY0D4sbest"),
             Form("DY - position correlation; Y0 [cm]; #DeltaY04 [cm]"),
             50,
             -15.,
             15.,
             50,
             -15.,
             15.);

  fhNMatch04 = new TH1F(Form("hNMatch04"),
                        Form("Number of Matched Hit pairs 0-4; NMatched"),
                        50,
                        0.,
                        50.);

  fhNMatch04sel = new TH1F(Form("hNMatch04sel"),
                           Form("Number of Matched Hit pairs 0-4; NMatched"),
                           50,
                           0.,
                           50.);

  fhNMatchD4sel = new TH1F(Form("hNMatchD4sel"),
                           Form("Number of Matched Hit pairs 0-4; NMatched"),
                           50,
                           0.,
                           50.);

  fhDTD4sel = new TH1F(Form("hDTD4sel"),
                       Form("Time difference BRef - MrpcRef; #DeltaTD4 (ns)"),
                       100,
                       -DTDMAX,
                       DTDMAX);

  fhTofD4sel = new TH1F(Form("hTofD4sel"),
                        Form("Time difference BRef - MrpcRef; #DeltaTD4 (ns)"),
                        100,
                        -DTDMAX,
                        DTDMAX);

  fhDT04DX0_1 =
    new TH2F(Form("hDT04DX0_1"),
             Form("Time - position correlation; #DeltaX0 [cm]; #DeltaT04 [ns]"),
             50,
             -25.,
             25.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DY0_1 =
    new TH2F(Form("hDT04DY0_1"),
             Form("Time - position correlation; #DeltaY0 [cm]; #DeltaT04 [ns]"),
             50,
             -25.,
             25.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DT0_1 =
    new TH2F(Form("hDT04DT0_1"),
             Form("Time - time correlation; #DeltaT0 [ns]; #DeltaT04 [ns]"),
             50,
             -5.,
             5.,
             100,
             -DTMAX,
             DTMAX);

  fhDT04DX4_1 =
    new TH2F(Form("hDT04DX4_1"),
             Form("Time - position correlation; #DeltaX4 [cm]; #DeltaT04 [ns]"),
             50,
             -10.,
             10.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DY4_1 =
    new TH2F(Form("hDT04DY4_1"),
             Form("Time - position correlation; #DeltaY4 [cm]; #DeltaT04 [ns]"),
             50,
             -15.,
             15.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DT4_1 =
    new TH2F(Form("hDT04DT4_1"),
             Form("Time - time correlation; #DeltaT4 [ns]; #DeltaT04 [ns]"),
             50,
             -5.,
             5.,
             100,
             -DTMAX,
             DTMAX);

  fhDT04DX0_2 =
    new TH2F(Form("hDT04DX0_2"),
             Form("Time - position correlation; #DeltaX0 [cm]; #DeltaT04 [ns]"),
             50,
             -25.,
             25.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DY0_2 =
    new TH2F(Form("hDT04DY0_2"),
             Form("Time - position correlation; #DeltaY0 [cm]; #DeltaT04 [ns]"),
             50,
             -25.,
             25.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DT0_2 =
    new TH2F(Form("hDT04DT0_2"),
             Form("Time - time correlation; #DeltaT0 [ns]; #DeltaT04 [ns]"),
             50,
             -5.,
             5.,
             100,
             -DTMAX,
             DTMAX);

  fhDT04DX4_2 =
    new TH2F(Form("hDT04DX4_2"),
             Form("Time - position correlation; #DeltaX4 [cm]; #DeltaT04 [ns]"),
             50,
             -10.,
             10.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DY4_2 =
    new TH2F(Form("hDT04DY4_2"),
             Form("Time - position correlation; #DeltaY4 [cm]; #DeltaT04 [ns]"),
             50,
             -15.,
             15.,
             100,
             -DTMAX,
             DTMAX);
  fhDT04DT4_2 =
    new TH2F(Form("hDT04DT4_2"),
             Form("Time - time correlation; #DeltaT4 [ns]; #DeltaT04 [ns]"),
             50,
             -5.,
             5.,
             100,
             -DTMAX,
             DTMAX);

  // Dut histos
  Int_t iDutId = fiDut * 100 + fiDutSm * 10 + fiDutRpc;
  fhDutPullX   = new TH1F(Form("hDutPullX_Sm_%03d", iDutId),
                        Form("hDutPullX_Sm_%03d;  #DeltaX", iDutId),
                        100,
                        -10.,
                        10.);
  fhDutPullXB  = new TH1F(Form("hDutPullXB_Sm_%03d", iDutId),
                         Form("hDutPullXB_Sm_%03d;  #DeltaX", iDutId),
                         100,
                         -10.,
                         10.);
  fhDutPullY   = new TH1F(Form("hDutPullY_Sm_%03d", iDutId),
                        Form("hDutPullY_Sm_%03d;  #DeltaY", iDutId),
                        100,
                        -10.,
                        10.);
  fhDutPullYB  = new TH1F(Form("hDutPullYB_Sm_%03d", iDutId),
                         Form("hDutPullYB_Sm_%03d;  #DeltaY", iDutId),
                         100,
                         -10.,
                         10.);
  fhDutPullZ   = new TH1F(Form("hDutPullZ_Sm_%03d", iDutId),
                        Form("hDutPullZ_Sm_%03d;  #DeltaZ", iDutId),
                        100,
                        -200.,
                        200.);
  fhDutPullT   = new TH1F(Form("hDutPullT_Sm_%03d", iDutId),
                        Form("hDutPullT_Sm_%03d;  #DeltaT", iDutId),
                        100,
                        -0.5,
                        0.5);
  fhDutPullTB  = new TH1F(Form("hDutPullTB_Sm_%03d", iDutId),
                         Form("hDutPullTB_Sm_%03d;  #DeltaT", iDutId),
                         150,
                         -0.75,
                         0.75);

  fhDutChi_Found  = new TH1F(Form("hDutChi_Found_%03d", iDutId),
                            Form("hDutChi_Found_%03d;  #chi", iDutId),
                            50,
                            0.,
                            10.);
  fhDutChi_Missed = new TH1F(Form("hDutChi_Missed_%03d", iDutId),
                             Form("hDutChi_Missed_%03d;  #chi", iDutId),
                             50,
                             0.,
                             10.);

  fhDutChi_Match = new TH1F(Form("hDutChi_Match_%03d", iDutId),
                            Form("hDutChi_Match_%03d;  #chi", iDutId),
                            50,
                            0.,
                            10.);
  Double_t XSIZ  = 20.;
  Double_t DTSIZ = 0.5;
  Int_t Nbins    = 40.;
  fhDutXY_Found  = new TH2F(Form("hDutXY_Found_%03d", iDutId),
                           Form("hDutXY_Found_%03d;  x(cm); y (cm)", iDutId),
                           Nbins,
                           -XSIZ,
                           XSIZ,
                           Nbins,
                           -XSIZ,
                           XSIZ);
  fhDutXY_Missed = new TH2F(Form("hDutXY_Missed_%03d", iDutId),
                            Form("hDutXY_Missed_%03d;  x(cm); y (cm)", iDutId),
                            Nbins,
                            -XSIZ,
                            XSIZ,
                            Nbins,
                            -XSIZ,
                            XSIZ);

  fhDutDTLH_Found = new TH1F(Form("hDutDTLH_Found_%03d", iDutId),
                             Form("hDutDTLH_Found_%03d;  log(#DeltaT)", iDutId),
                             50,
                             0.,
                             12.);
  fhDutDTLH_Missed =
    new TH1F(Form("hDutDTLH_Missed_%03d", iDutId),
             Form("hDutDTLH_Missed_%03d;  log(#DeltaT)", iDutId),
             50,
             0.,
             12.);
  fhDutMul_Found =
    new TH1F(Form("hDutMul_Found_%03d", iDutId),
             Form("hDutMul_Found_%03d; Hit Multiplicity", iDutId),
             32,
             0.,
             32.);
  fhDutMul_Missed =
    new TH1F(Form("hDutMul_Missed_%03d", iDutId),
             Form("hDutMul_Missed_%03d; Hit Multiplicity", iDutId),
             32,
             0.,
             32.);
  fhDutTIS_Found =
    new TH2F(Form("hDutTIS_Found_%03d", iDutId),
             Form("hDutTIS_Found_%03d; Time in spill (s)", iDutId),
             TISnbins,
             0,
             TISmax,
             9,
             0,
             9);
  fhDutTIS_Missed =
    new TH2F(Form("hDutTIS_Missed_%03d", iDutId),
             Form("hDutTIS_Missed_%03d; Time in spill (s)", iDutId),
             TISnbins,
             0,
             TISmax,
             9,
             0,
             9);
  fhDutTIR_Found =
    new TH2F(Form("hDutTIR_Found_%03d", iDutId),
             Form("hDutTIR_Found_%03d; Time in spill (s)", iDutId),
             TIRnbins,
             0,
             TIRmax,
             9,
             0,
             9);
  fhDutTIR_Missed =
    new TH2F(Form("hDutTIR_Missed_%03d", iDutId),
             Form("hDutTIR_Missed_%03d; Time in spill (s)", iDutId),
             TIRnbins,
             0,
             TIRmax,
             9,
             0,
             9);
  fhDutVel_Found =
    new TH1F(Form("hDutVel_Found_%03d", iDutId),
             Form("hDutVel_Found_%03d; velocity (cm/ns)", iDutId),
             50,
             0.,
             50.);
  fhDutVel_Missed =
    new TH1F(Form("hDutVel_Missed_%03d", iDutId),
             Form("hDutVel_Missed_%03d; velocity (cm/ns)", iDutId),
             50,
             0.,
             50.);
  fhDutDTLH_CluSize =
    new TH2F(Form("hDutDTLH_CluSize_%03d", iDutId),
             Form("hDutDTLH_CluSize_%03d;  log(#DeltaT); CluSize", iDutId),
             50,
             0.,
             12.,
             10,
             1.,
             11.);
  fhDutDTLH_Tot =
    new TH2F(Form("hDutDTLH_Tot_%03d", iDutId),
             Form("hDutDTLH_Tot_%03d;  log(#DeltaT); Tot", iDutId),
             50,
             0.,
             12.,
             50,
             0.,
             20.);
  fhDutDTLH_Mul =
    new TH2F(Form("hDutDTLH_Mul_%03d", iDutId),
             Form("hDutDTLH_Mul_%03d;  log(#DeltaT); Mul", iDutId),
             50,
             0.,
             12.,
             30,
             0.,
             30.);
  fhDutDTLH_TIS =
    new TH2F(Form("hDutDTLH_TIS_%03d", iDutId),
             Form("hDutDTLH_TIS_%03d;  log(#DeltaT); TIS (s)", iDutId),
             50,
             0.,
             12.,
             TISnbins,
             0,
             TISmax);
  fhDutDTLH_Missed_TIS =
    new TH2F(Form("hDutDTLH_Missed_TIS_%03d", iDutId),
             Form("hDutDTLH_Missed_TIS_%03d;  log(#DeltaT); TIS (s)", iDutId),
             50,
             0.,
             12.,
             TISnbins,
             0,
             TISmax);
  fhDutDTLH_DDH_Found = new TH2F(
    Form("hDutDTLH_DDH_Found_%03d", iDutId),
    Form("hDutDTLH_DDH_Found_%03d;  log(#DeltaT); distance to LH (cm)", iDutId),
    50,
    0.,
    12.,
    40,
    0.,
    4.);
  fhDutDTLH_DD_Found = new TH2F(
    Form("hDutDTLH_DD_Found_%03d", iDutId),
    Form("hDutDTLH_DD_Found_%03d;  log(#DeltaT); distance to LH (cm)", iDutId),
    50,
    0.,
    12.,
    40,
    0.,
    4.);
  fhDutDTLH_DD_Missed = new TH2F(
    Form("hDutDTLH_DD_Missed_%03d", iDutId),
    Form("hDutDTLH_DD_Missed_%03d;  log(#DeltaT); distance to LH (cm)", iDutId),
    50,
    0.,
    12.,
    40,
    0.,
    4.);
  fhDutXYDX =
    new TH3F(Form("hDutXYDX_%03d", iDutId),
             Form("hDutXYDT_%03d;  x(cm); y (cm); #Deltax (cm)", iDutId),
             Nbins,
             -XSIZ,
             XSIZ,
             Nbins,
             -XSIZ,
             XSIZ,
             Nbins,
             -2.,
             2.);
  fhDutXYDY =
    new TH3F(Form("hDutXYDY_%03d", iDutId),
             Form("hDutXYDT_%03d;  x(cm); y (cm); #Deltay (cm)", iDutId),
             Nbins,
             -XSIZ,
             XSIZ,
             Nbins,
             -XSIZ,
             XSIZ,
             Nbins,
             -2.,
             2.);
  fhDutXYDT =
    new TH3F(Form("hDutXYDT_%03d", iDutId),
             Form("hDutXYDT_%03d;  x(cm); y (cm); #Deltat (ns)", iDutId),
             Nbins,
             -XSIZ,
             XSIZ,
             Nbins,
             -XSIZ,
             XSIZ,
             Nbins,
             -DTSIZ,
             DTSIZ);


  fhSelTypeNNChiSq =
    new TH2F("hSelTypeNNChiSq",
             "ST NN #chi^{2}; sel type ST []; #chi^{2}_{3} []; ",
             3,
             0,
             3,
             1000,
             0.,
             100.);

  fhSelTypeNNResidualT =
    new TH2F("hSelTypeNNResidualT",
             "ST NN ResiT vs. ST; sel type ST [];  T_{DUT} - T_{ST} [ns]",
             3,
             0,
             3,
             3001,
             -1.5,
             1.5);

  fhSelTypeNNResidualX =
    new TH2F("hSelTypeNNResidualX",
             "ST NN ResiX vs. ST; sel type ST [];  X_{DUT} - X_{ST} [cm]",
             3,
             0,
             3,
             801,
             -4.,
             4.);

  fhSelTypeNNResidualY =
    new TH2F("hSelTypeNNResidualY",
             "ST NN ResiY vs. ST; sel type ST [];  Y_{DUT} - Y_{ST} [cm]",
             3,
             0,
             3,
             3001,
             -15.,
             15.);

  fhSelTypeAccNNChiSq =
    new TH2F("hSelTypeAccNNChiSq",
             "ST acc NN #chi^{2}; sel type ST []; #chi^{2}_{3} []; ",
             3,
             0,
             3,
             1000,
             0.,
             100.);

  fhSelTypeAccNNResidualT =
    new TH2F("hSelTypeAccNNResidualT",
             "ST acc NN ResiT vs. ST; sel type ST [];  T_{DUT} - T_{ST} [ns]",
             3,
             0,
             3,
             3001,
             -1.5,
             1.5);

  fhSelTypeAccNNResidualX =
    new TH2F("hSelTypeAccNNResidualX",
             "ST acc NN ResiX vs. ST; sel type ST [];  X_{DUT} - X_{ST} [cm]",
             3,
             0,
             3,
             801,
             -4.,
             4.);

  fhSelTypeAccNNResidualY =
    new TH2F("hSelTypeAccNNResidualY",
             "ST acc NN ResiY vs. ST; sel type ST [];  Y_{DUT} - Y_{ST} [cm]",
             3,
             0,
             3,
             3001,
             -15.,
             15.);


  fhSelEfficiency = new TEfficiency(
    "hSelEfficiency", "; acc ref MUL []; Sel efficiency []", 50, 0., 50.);

  fhSelHitTupleEfficiencyTIS =
    new TEfficiency("hSelHitTupleEfficiencyTIS",
                    "; time in spill [s]; Sel efficiency []",
                    TISnbins,
                    0.,
                    TISmax);

  fhSelMatchEfficiency =
    new TEfficiency("hSelMatchEfficiency",
                    "; acc ref MUL []; Sel match efficiency []",
                    50,
                    0.,
                    50.);

  fhSelHitTupleMatchEfficiencyTIS =
    new TEfficiency("hSelHitTupleMatchEfficiencyTIS",
                    "; time in spill [s]; Sel match efficiency []",
                    TISnbins,
                    0.,
                    TISmax);

  fhSelHitTupleResidualTTIS =
    new TH2F("hSelHitTupleResidualTTIS",
             "; time in spill [s]; T_{hit} - T_{sel} [ns]",
             TISnbins,
             0.,
             TISmax,
             301,
             -1.5,
             1.5);

  fhSelHitTupleDutCluSizeTIS =
    new TH2F("hSelHitTupleDutCluSizeTIS",
             "; time in spill [s]; cluster size [cells]",
             TISnbins,
             0.,
             TISmax,
             20,
             0.5,
             20.5);


  fhSelTrklEfficiency = new TEfficiency(
    "hSelTrklEfficiency", "; acc ref MUL []; Sel efficiency []", 50, 0., 50.);

  fhSelTrklEfficiencyTIS =
    new TEfficiency("hSelTrklEfficiencyTIS",
                    "; time in spill [s]; Sel efficiency []",
                    TISnbins,
                    0.,
                    TISmax);

  fhSelTrklMatchEfficiency =
    new TEfficiency("hSelTrklMatchEfficiency",
                    "; acc ref MUL []; Sel match efficiency []",
                    50,
                    0.,
                    50.);

  fhSelTrklMatchEfficiencyTIS =
    new TEfficiency("hSelTrklMatchEfficiencyTIS",
                    "; time in spill [s]; Sel match efficiency []",
                    TISnbins,
                    0.,
                    TISmax);

  fhSelTrklResidualTTIS =
    new TH2F("hSelTrklResidualTTIS",
             "; time in spill [s]; T_{hit} - T_{sel} [ns]",
             TISnbins,
             0.,
             TISmax,
             301,
             -1.5,
             1.5);

  fhSelTrklDutCluSizeTIS = new TH2F("hSelTrklDutCluSizeTIS",
                                    "; time in spill [s]; cluster size [cells]",
                                    TISnbins,
                                    0.,
                                    TISmax,
                                    20,
                                    0.5,
                                    20.5);


  if (fbMonteCarloComparison) {
    Int_t iNCounters(fCounterModuleNodes.size());


    fhNMergedMCEvents = new TH1F(
      "hNMergedMCEvents", "Merged MC events; events []; ", 10, -0.5, 9.5);

    fhAccTrackMul =
      new TH1F("hAccTrackMul", "Acc track mul; MUL []; ", 150, -0.5, 149.5);
    fhAccRefTrackMul = new TH1F(
      "hAccRefTrackMul", "Acc ref track mul; MUL []; ", 50, -0.5, 49.5);
    fhAccPrimTrackMul = new TH1F(
      "hAccPrimTrackMul", "Acc prim track mul; MUL []; ", 60, -0.5, 59.5);
    fhAccTrackPointMul     = new TH1F("hAccTrackPointMul",
                                  "Acc track points; points []; ",
                                  iNCounters,
                                  -0.5,
                                  iNCounters - 0.5);
    fhAccRefTrackPointMul  = new TH1F("hAccRefTrackPointMul",
                                     "Acc ref track points; points []; ",
                                     iNCounters,
                                     -0.5,
                                     iNCounters - 0.5);
    fhAccRndmTrackPointMul = new TH1F("hAccRndmTrackPointMul",
                                      "Acc rndm track points; points []; ",
                                      iNCounters,
                                      -0.5,
                                      iNCounters - 0.5);


    fhAccRefTrackAcceptance =
      new TH2F("hAccRefTrackAcceptance",
               "Ref track acceptance; y_{lab} []; p_{t}/m [1/c]",
               42,
               -1.,
               6.,
               30,
               0.,
               2.5);

    fhAccRefTrackAcceptanceEfficiency = new TEfficiency(
      "hAccRefTrackAcceptanceEfficiency",
      "Ref track acceptance efficiency; y_{lab} []; p_{t}/m [1/c]",
      42,
      -1.,
      6.,
      30,
      0.,
      2.5);

    fhAccRefTrackAcceptancePurity =
      new TEfficiency("hAccRefTrackAcceptancePurity",
                      "Ref track acceptance purity; y_{lab} []; p_{t}/m [1/c]",
                      42,
                      -1.,
                      6.,
                      30,
                      0.,
                      2.5);

    fhAccRefTrackMulCentrality =
      new TH2F("hAccRefTrackMulCentrality",
               "Acc ref track mul vs. centrality; b [fm]; MUL []",
               20,
               0.,
               20.,
               50,
               -0.5,
               49.5);

    fhAccRefTracksProcSpec = new TH2F(
      "hAccRefTracksProcSpec", "ref track proc/spec; ; ", 5, 0., 5., 5, 0., 5.);
    fhAccRefTracksProcSpec->SetCanExtend(TH1::kAllAxes);
    fhAccRefTracksProcSpec->SetStats(0);


    fhSelMCTrackEfficiency =
      new TEfficiency("hSelMCTrackEfficiency",
                      "; acc ref MUL []; Sel efficiency []",
                      50,
                      0.,
                      50.);

    fhSelMCTrackMatchEfficiency =
      new TEfficiency("hSelMCTrackMatchEfficiency",
                      "; acc ref MUL []; Sel match efficiency []",
                      50,
                      0.,
                      50.);

    fhSelMCTrackMatchPurity =
      new TEfficiency("hSelMCTrackMatchPurity",
                      "; acc ref MUL []; Sel match purity []",
                      50,
                      0.,
                      50.);

    fhSelMCTrackDutHitMatchNNMul = new TH1F("hSelMCTrackDutHitMatchNNMul",
                                            "Sel track-hit match mul; MUL []; ",
                                            30,
                                            0.5,
                                            30.5);

    fhSelMCTrackDutHitMatchAccNNMul =
      new TH1F("hSelMCTrackDutHitMatchAccNNMul",
               "Sel track-hit match acc mul; MUL []; ",
               30,
               0.5,
               30.5);


    fhSelPurity = new TEfficiency(
      "hSelPurity", "; acc ref MUL []; Sel purity []", 50, 0., 50.);

    fhSelRefTrackShare =
      new TEfficiency("hSelRefTrackShare",
                      "; acc ref MUL []; Sel ref track share []",
                      50,
                      0.5,
                      50.5);

    fhSelRefTrackProcSpec = new TH2F("hSelRefTrackProcSpec",
                                     "ST 0 ref track proc/spec; ; ",
                                     5,
                                     0.,
                                     5.,
                                     5,
                                     0.,
                                     5.);
    fhSelRefTrackProcSpec->SetCanExtend(TH1::kAllAxes);
    fhSelRefTrackProcSpec->SetStats(0);

    fhSelMatchPurity = new TEfficiency(
      "hSelMatchPurity", "; acc ref MUL []; Sel match purity []", 50, 0., 50.);


    fhResX04HitExp =
      new TH2F("hResX04HitExp",
               "DUT hit-exp residual X; acc ref MUL []; X_{hit} - X_{exp} [cm]",
               50,
               0.,
               50.,
               801,
               -4.,
               4.);

    fhResX04ExpMC =
      new TH2F("hResX04ExpMC",
               "DUT exp-MC residual X; acc ref MUL []; X_{exp} - X_{MC} [cm]",
               50,
               0.,
               50.,
               801,
               -4.,
               4.);

    fhResX04HitMC =
      new TH2F("hResX04HitMC",
               "DUT hit-MC residual X; acc ref MUL []; X_{hit} - X_{MC} [cm]",
               50,
               0.,
               50.,
               801,
               -4.,
               4.);

    fhResY04HitExp =
      new TH2F("hResY04HitExp",
               "DUT hit-exp residual Y; acc ref MUL []; Y_{hit} - Y_{exp} [cm]",
               50,
               0.,
               50.,
               3001,
               -15.,
               15.);

    fhResY04ExpMC =
      new TH2F("hResY04ExpMC",
               "DUT exp-MC residual Y; acc ref MUL []; Y_{exp} - Y_{MC} [cm]",
               50,
               0.,
               50.,
               3001,
               -15.,
               15.);

    fhResY04HitMC =
      new TH2F("hResY04HitMC",
               "DUT hit-MC residual Y; acc ref MUL []; Y_{hit} - Y_{MC} [cm]",
               50,
               0.,
               50.,
               3001,
               -15.,
               15.);

    fhResT04HitExp =
      new TH2F("hResT04HitExp",
               "DUT hit-exp residual T; acc ref MUL []; T_{hit} - T_{exp} [ns]",
               50,
               0.,
               50.,
               3001,
               -1.5,
               1.5);

    fhResT04ExpMC =
      new TH2F("hResT04ExpMC",
               "DUT exp-MC residual T; acc ref MUL []; T_{exp} - T_{MC} [ns]",
               50,
               0.,
               50.,
               3001,
               -1.5,
               1.5);

    fhResT04HitMC =
      new TH2F("hResT04HitMC",
               "DUT hit-MC residual T; acc ref MUL []; T_{hit} - T_{MC} [ns]",
               50,
               0.,
               50.,
               3001,
               -1.5,
               1.5);

    fhNTracksPerMRefHit = new TH1F("hNTracksPerMRefHit",
                                   "MRef tracks/hit; #tracks/hit []; ",
                                   20,
                                   -0.5,
                                   19.5);

    fhNTracksPerSel2Hit = new TH1F("hNTracksPerSel2Hit",
                                   "Sel2 tracks/hit; #tracks/hit []; ",
                                   20,
                                   -0.5,
                                   19.5);

    fhNTracksPerDutHit = new TH1F(
      "hNTracksPerDutHit", "DUT tracks/hit; #tracks/hit []; ", 20, -0.5, 19.5);

    fhNTracksPerSelMRefHit = new TH1F("hNTracksPerSelMRefHit",
                                      "MRef sel tracks/hit; #tracks/hit []; ",
                                      20,
                                      -0.5,
                                      19.5);

    fhNTracksPerSelSel2Hit = new TH1F("hNTracksPerSelSel2Hit",
                                      "Sel2 sel tracks/hit; #tracks/hit []; ",
                                      20,
                                      -0.5,
                                      19.5);

    fhNTracksPerSelDutHit = new TH1F("hNTracksPerSelDutHit",
                                     "DUT sel tracks/hit; #tracks/hit []; ",
                                     20,
                                     -0.5,
                                     19.5);


    fhSelTrklPurity = new TEfficiency(
      "hSelTrklPurity", "; acc ref MUL []; Sel purity []", 50, 0., 50.);

    fhSelTrklRefTrackShare =
      new TEfficiency("hSelTrklRefTrackShare",
                      "; acc ref MUL []; Sel ref track share []",
                      50,
                      0.5,
                      50.5);

    fhSelTrklRefTrackProcSpec = new TH2F("hSelTrklRefTrackProcSpec",
                                         "ST 1 ref track proc/spec; ; ",
                                         5,
                                         0.,
                                         5.,
                                         5,
                                         0.,
                                         5.);
    fhSelTrklRefTrackProcSpec->SetCanExtend(TH1::kAllAxes);
    fhSelTrklRefTrackProcSpec->SetStats(0);


    fhSelTrklMatchPurity =
      new TEfficiency("hSelTrklMatchPurity",
                      "; acc ref MUL []; Sel match purity []",
                      50,
                      0.,
                      50.);

    fhDutResX_Hit_Trk = new TH2F(
      "hDutResX_Hit_Trk",
      "DUT hit-trklt residual X; acc ref MUL []; X_{hit} - X_{trklt} [cm]",
      50,
      0.,
      50.,
      801,
      -4.,
      4.);

    fhDutResX_Trk_MC = new TH2F(
      "hDutResX_Trk_MC",
      "DUT trklt-MC residual X; acc ref MUL []; X_{trklt} - X_{MC} [cm]",
      50,
      0.,
      50.,
      801,
      -4.,
      4.);

    fhDutResX_Hit_MC =
      new TH2F("hDutResX_Hit_MC",
               "DUT hit-MC residual X; acc ref MUL []; X_{hit} - X_{MC} [cm]",
               50,
               0.,
               50.,
               801,
               -4.,
               4.);

    fhDutResY_Hit_Trk = new TH2F(
      "hDutResY_Hit_Trk",
      "DUT hit-trklt residual Y; acc ref MUL []; Y_{hit} - Y_{trklt} [cm]",
      50,
      0.,
      50.,
      3001,
      -15.,
      15.);

    fhDutResY_Trk_MC = new TH2F(
      "hDutResY_Trk_MC",
      "DUT trklt-MC residual Y; acc ref MUL []; Y_{trklt} - Y_{MC} [cm]",
      50,
      0.,
      50.,
      3001,
      -15.,
      15.);

    fhDutResY_Hit_MC =
      new TH2F("hDutResY_Hit_MC",
               "DUT hit-MC residual Y; acc ref MUL []; Y_{hit} - Y_{MC} [cm]",
               50,
               0.,
               50.,
               3001,
               -15.,
               15.);

    fhDutResT_Hit_Trk = new TH2F(
      "hDutResT_Hit_Trk",
      "DUT hit-trklt residual T; acc ref MUL []; T_{hit} - T_{trklt} [ns]",
      50,
      0.,
      50.,
      3001,
      -1.5,
      1.5);

    fhDutResT_Trk_MC = new TH2F(
      "hDutResT_Trk_MC",
      "DUT trklt-MC residual T; acc ref MUL []; T_{trklt} - T_{MC} [ns]",
      50,
      0.,
      50.,
      3001,
      -1.5,
      1.5);

    fhDutResT_Hit_MC =
      new TH2F("hDutResT_Hit_MC",
               "DUT hit-MC residual T; acc ref MUL []; T_{hit} - T_{MC} [ns]",
               50,
               0.,
               50.,
               3001,
               -1.5,
               1.5);


    fhSelMCTrackEfficiencyTIS =
      new TEfficiency("hSelMCTrackEfficiencyTIS",
                      "; time in spill [s]; Sel efficiency []",
                      TISnbins,
                      0.,
                      TISmax);

    fhSelMCTrackMatchEfficiencyTIS =
      new TEfficiency("hSelMCTrackMatchEfficiencyTIS",
                      "; time in spill [s]; Sel match efficiency []",
                      TISnbins,
                      0.,
                      TISmax);

    fhSelMCTrackResidualTTIS =
      new TH2F("hSelMCTrackResidualTTIS",
               "; time in spill [s]; T_{hit} - T_{sel} [ns]",
               TISnbins,
               0.,
               TISmax,
               301,
               -1.5,
               1.5);

    fhSelMCTrackDutCluSizeTIS =
      new TH2F("hSelMCTrackDutCluSizeTIS",
               "; time in spill [s]; cluster size [cells]",
               TISnbins,
               0.,
               TISmax,
               20,
               0.5,
               20.5);


    fhPVResTAll = new TH2F(
      "hPVResTAll",
      "PV all reco-MC residual T; acc ref MUL []; T_{reco} - T_{MC} [ns]",
      50,
      0.,
      50.,
      3001,
      -1.5,
      1.5);

    fhPVResXAll = new TH2F(
      "hPVResXAll",
      "PV all reco-MC residual X; acc ref MUL []; X_{reco} - X_{MC} [cm]",
      50,
      0.,
      50.,
      801,
      -4.,
      4.);

    fhPVResYAll = new TH2F(
      "hPVResYAll",
      "PV all reco-MC residual Y; acc ref MUL []; Y_{reco} - Y_{MC} [cm]",
      50,
      0.,
      50.,
      2001,
      -10.,
      10.);

    fhPVResTRef = new TH2F(
      "hPVResTRef",
      "PV ref reco-MC residual T; acc ref MUL []; T_{reco} - T_{MC} [ns]",
      50,
      0.,
      50.,
      3001,
      -1.5,
      1.5);

    fhPVResXRef = new TH2F(
      "hPVResXRef",
      "PV ref reco-MC residual X; acc ref MUL []; X_{reco} - X_{MC} [cm]",
      50,
      0.,
      50.,
      801,
      -4.,
      4.);

    fhPVResYRef = new TH2F(
      "hPVResYRef",
      "PV ref reco-MC residual Y; acc ref MUL []; Y_{reco} - Y_{MC} [cm]",
      50,
      0.,
      50.,
      2001,
      -10.,
      10.);


    fhAccRefTrackResT = new TH2F("hAccRefTrackResT",
                                 "ref track-tracklet residual T; acc ref MUL "
                                 "[]; T0_{trklt} - T_{trk} [ns]",
                                 50,
                                 0.,
                                 50.,
                                 3001,
                                 -1.5,
                                 1.5);

    fhAccRefTrackResX = new TH2F("hAccRefTrackResX",
                                 "ref track-tracklet residual X; acc ref MUL "
                                 "[]; X0_{trklt} - X_{trk} [cm]",
                                 50,
                                 0.,
                                 50.,
                                 801,
                                 -4.,
                                 4.);

    fhAccRefTrackResY = new TH2F("hAccRefTrackResY",
                                 "ref track-tracklet residual Y; acc ref MUL "
                                 "[]; Y0_{trklt} - Y_{trk} [cm]",
                                 50,
                                 0.,
                                 50.,
                                 2001,
                                 -10.,
                                 10.);

    fhAccRefTrackResTx = new TH2F("hAccRefTrackResTx",
                                  "ref track-tracklet residual Tx; acc ref MUL "
                                  "[]; Tx0_{trklt} - Tx_{trk} []",
                                  50,
                                  0.,
                                  50.,
                                  201,
                                  -1.,
                                  1.);

    fhAccRefTrackResTy = new TH2F("hAccRefTrackResTy",
                                  "ref track-tracklet residual Ty; acc ref MUL "
                                  "[]; Ty0_{trklt} - Ty_{trk} []",
                                  50,
                                  0.,
                                  50.,
                                  201,
                                  -1.,
                                  1.);

    fhAccRefTrackResV = new TH2F("hAccRefTrackResV",
                                 "ref track-tracklet residual V; acc ref MUL "
                                 "[]; V0_{trklt} - V_{trk} [cm/ns]",
                                 50,
                                 0.,
                                 50.,
                                 101,
                                 -5.,
                                 5.);

    fhAccRefTrackResN = new TH2F(
      "hAccRefTrackResN",
      "ref track-tracklet residual N; acc ref MUL []; N0_{trklt} - N_{trk} []",
      50,
      0.,
      50.,
      11,
      -5.5,
      5.5);


    fhAccRefTrackShare = new TEfficiency(
      "hAccRefTrackShare", "; acc ref MUL []; acc ref share []", 50, 0., 50.);

    fhRecRefTrackEfficiency =
      new TEfficiency("hRecRefTrackEfficiency",
                      "; acc ref MUL []; acc ref reco eff []",
                      50,
                      0.,
                      50.);

    fhRecRndmTrackEfficiency =
      new TEfficiency("hRecRndmTrackEfficiency",
                      "; acc ref MUL []; acc rndm reco eff []",
                      50,
                      0.,
                      50.);

    fhRecRefTrackGhostShare =
      new TEfficiency("hRecRefTrackGhostShare",
                      "; acc ref MUL []; acc ref ghost share []",
                      50,
                      0.,
                      50.);

    fhRecRefTrackCloneShare =
      new TEfficiency("hRecRefTrackCloneShare",
                      "; acc ref MUL []; acc ref clone share []",
                      50,
                      0.,
                      50.);

    fhRecRndmTrackGhostShare =
      new TEfficiency("hRecRndmTrackGhostShare",
                      "; acc ref MUL []; acc rndm ghost share []",
                      50,
                      0.,
                      50.);

    fhRecRndmTrackCloneShare =
      new TEfficiency("hRecRndmTrackCloneShare",
                      "; acc ref MUL []; acc rndm clone share []",
                      50,
                      0.,
                      50.);

    TH1* tCurrentHistogram(NULL);

    for (auto const& CounterModuleNode : fCounterModuleNodes) {
      auto const& CounterID = CounterModuleNode.first;

      Int_t iModuleType    = std::get<0>(CounterID);
      Int_t iModuleIndex   = std::get<1>(CounterID);
      Int_t iCounterIndex  = std::get<2>(CounterID);
      Int_t iNCounterCells = fDigiBdfPar->GetNbChan(iModuleType, iCounterIndex);


      tCurrentHistogram = new TH2F(Form("hDomTracksProcSpec_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d dom proc/spec; ; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   5,
                                   0.,
                                   5.,
                                   5,
                                   0.,
                                   5.);
      tCurrentHistogram->SetCanExtend(TH1::kAllAxes);
      tCurrentHistogram->SetStats(0);

      fhDomTracksProcSpec.insert(
        std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));


      tCurrentHistogram = new TH2F(Form("hDomTracksProcMat_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d dom proc/mat; ; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   5,
                                   0.,
                                   5.,
                                   5,
                                   0.,
                                   5.);
      tCurrentHistogram->SetCanExtend(TH1::kAllAxes);
      tCurrentHistogram->SetStats(0);

      fhDomTracksProcMat.insert(
        std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));


      tCurrentHistogram = new TH2F(Form("hRndmTracksProcSpec_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d rndm proc/spec; ; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   5,
                                   0.,
                                   5.,
                                   5,
                                   0.,
                                   5.);
      tCurrentHistogram->SetCanExtend(TH1::kAllAxes);
      tCurrentHistogram->SetStats(0);

      fhRndmTracksProcSpec.insert(
        std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));


      tCurrentHistogram = new TH2F(Form("hRndmTracksProcMat_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d rndm proc/mat; ; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   5,
                                   0.,
                                   5.,
                                   5,
                                   0.,
                                   5.);
      tCurrentHistogram->SetCanExtend(TH1::kAllAxes);
      tCurrentHistogram->SetStats(0);

      fhRndmTracksProcMat.insert(
        std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));


      tCurrentHistogram = new TH1F(Form("hCounterAccTrackMul_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d acc mul; MUL []; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   100,
                                   -0.5,
                                   99.5);

      fhCounterAccTrackMul.insert(std::make_pair(CounterID, tCurrentHistogram));


      tCurrentHistogram = new TH1F(Form("hCounterAccRefTrackMul_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d acc ref mul; MUL []; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   50,
                                   -0.5,
                                   49.5);

      fhCounterAccRefTrackMul.insert(
        std::make_pair(CounterID, tCurrentHistogram));


      tCurrentHistogram = new TH1F(Form("hCounterAccRndmTrackMul_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d acc rndm mul; MUL []; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   50,
                                   -0.5,
                                   49.5);

      fhCounterAccRndmTrackMul.insert(
        std::make_pair(CounterID, tCurrentHistogram));


      tCurrentHistogram = new TH1F(Form("hCounterAccDomTrackMul_%d_%d_%d",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   Form("%d-%d-%d acc dom mul; MUL []; ",
                                        iModuleType,
                                        iModuleIndex,
                                        iCounterIndex),
                                   50,
                                   -0.5,
                                   49.5);

      fhCounterAccDomTrackMul.insert(
        std::make_pair(CounterID, tCurrentHistogram));


      tCurrentHistogram = new TH1F(
        Form("hCounterRecRefTrackEfficiencyPassed_%d_%d_%d",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        Form("%d-%d-%d acc ref reco eff; acc ref MUL []; reco efficiency []",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        50,
        0.,
        50.);

      fhCounterRecRefTrackEfficiencyPassed.insert(
        std::make_pair(CounterID, tCurrentHistogram));


      tCurrentHistogram = new TH1F(
        Form("hCounterRecRefTrackEfficiencyTotal_%d_%d_%d",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        Form("%d-%d-%d acc ref reco eff; acc ref MUL []; reco efficiency []",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        50,
        0.,
        50.);

      fhCounterRecRefTrackEfficiencyTotal.insert(
        std::make_pair(CounterID, tCurrentHistogram));


      tCurrentHistogram = new TH1F(
        Form("hCounterRecRefTrackPurityPassed_%d_%d_%d",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        Form("%d-%d-%d acc ref reco pure; acc ref MUL []; reco purity []",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        50,
        0.,
        50.);

      fhCounterRecRefTrackPurityPassed.insert(
        std::make_pair(CounterID, tCurrentHistogram));


      tCurrentHistogram = new TH2F(
        Form("hCounterRefTrackMulHitMul_%d_%d_%d",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        Form("%d-%d-%d acc ref mul/hit mul; acc ref MUL []; hit MUL []",
             iModuleType,
             iModuleIndex,
             iCounterIndex),
        50,
        -0.5,
        49.5,
        30,
        -0.5,
        29.5);

      fhCounterRefTrackMulHitMul.insert(
        std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));


      tCurrentHistogram =
        new TH2F(Form("hCounterRefTrackLocalXY_%d_%d_%d",
                      iModuleType,
                      iModuleIndex,
                      iCounterIndex),
                 Form("%d-%d-%d acc ref track position; X [cm]; Y [cm]",
                      iModuleType,
                      iModuleIndex,
                      iCounterIndex),
                 2 * iNbinXY + 1,
                 -17.,
                 17.,
                 2 * iNbinXY + 1,
                 -17.,
                 17.);

      fhCounterRefTrackLocalXY.insert(
        std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));


      tCurrentHistogram =
        new TH2F(Form("hCounterRefTrackMulCell_%d_%d_%d",
                      iModuleType,
                      iModuleIndex,
                      iCounterIndex),
                 Form("%d-%d-%d cell acc ref mul; strip []; ref track MUL []",
                      iModuleType,
                      iModuleIndex,
                      iCounterIndex),
                 iNCounterCells,
                 0,
                 iNCounterCells,
                 10,
                 -0.5,
                 9.5);

      fhCounterRefTrackMulCell.insert(
        std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));
    }


    fhGoodSelTypeNNPureChiSq =
      new TH2F("hGoodSelTypeNNPureChiSq",
               "gST pure NN #chi^{2}; sel type ST []; #chi^{2}_{3} []; ",
               3,
               0,
               3,
               1000,
               0.,
               100.);

    fhGoodSelTypeNNAllChiSq =
      new TH2F("hGoodSelTypeNNAllChiSq",
               "gST all NN #chi^{2}; sel type ST []; #chi^{2}_{3} []; ",
               3,
               0,
               3,
               1000,
               0.,
               100.);
  }


  for (auto const& CounterModuleNode : fCounterModuleNodes) {
    auto const& CounterID = CounterModuleNode.first;

    Int_t iModuleType    = std::get<0>(CounterID);
    Int_t iModuleIndex   = std::get<1>(CounterID);
    Int_t iCounterIndex  = std::get<2>(CounterID);
    Int_t iNCounterCells = fDigiBdfPar->GetNbChan(iModuleType, iCounterIndex);

    TH1* tCurrentHistogram =
      new TH2F(Form("hCounterHitMulCell_%d_%d_%d",
                    iModuleType,
                    iModuleIndex,
                    iCounterIndex),
               Form("%d-%d-%d cell hit mul; strip []; hit MUL []",
                    iModuleType,
                    iModuleIndex,
                    iCounterIndex),
               iNCounterCells,
               0,
               iNCounterCells,
               10,
               -0.5,
               9.5);

    fhCounterHitMulCell.insert(
      std::make_pair(CounterID, dynamic_cast<TH2*>(tCurrentHistogram)));
  }


  fhSelTrklFitRedChiSq      = new TH1F("hSelTrklFitRedChiSq",
                                  "Sel trkl 3D fit #chi^{2}/4; #chi^{2}/4 []; ",
                                  1000,
                                  0.,
                                  10.);
  fhSelTrklDutHitMatchNNMul = new TH1F("hSelTrklDutHitMatchNNMul",
                                       "Sel trkl-hit match mul; MUL []; ",
                                       30,
                                       0.5,
                                       30.5);
  fhSelTrklDutHitMatchAccNNMul =
    new TH1F("hSelTrklDutHitMatchAccNNMul",
             "Sel trkl-hit match acc mul; MUL []; ",
             30,
             0.5,
             30.5);

  fhSelHitTupleDutHitMatchMul = new TH1F("hSelHitTupleDutHitMatchMul",
                                         "Sel hit-hit match mul; MUL []; ",
                                         30,
                                         0.5,
                                         30.5);
  fhSelHitTupleDutHitMatchAccMul =
    new TH1F("hSelHitTupleDutHitMatchAccMul",
             "Sel hit-hit match acc mul; MUL []; ",
             30,
             0.5,
             30.5);

  // rate histos
  Double_t TRange    = 600.;  //in seconds
  Double_t NStations = 10.;
  if (NULL != fFindTracks) NStations = fFindTracks->GetNStations();

  fhTrklNofHitsRate = new TH2F(Form("hTrklNofHitsRate"),
                               Form("hTrklNofHitsRate;  Time (s); NofHits "),
                               (Int_t) TRange,
                               0.,
                               TRange,
                               10,
                               1.,
                               11.);
  fhTrklDetHitRate  = new TH2F(Form("hTrklDetHitRate"),
                              Form("hTrklDetHitRate;  Time (s); DetIndx "),
                              (Int_t) TRange,
                              0.,
                              TRange,
                              NStations,
                              0.,
                              NStations);

  // spill histos
  Double_t TRangeSpill = 10.;  //in seconds
  fhTrklNofHitsRateInSpill =
    new TH2F(Form("hTrklNofHitsRateInSpill"),
             Form("hTrklNofHitsRateInSpill;  Time (s); NofHits "),
             (Int_t) TRangeSpill * 10,
             0.,
             TRangeSpill,
             10,
             1.,
             11.);
  fhTrklDetHitRateInSpill =
    new TH2F(Form("hTrklDetHitRateInSpill"),
             Form("hTrklDetHitRateInSpill;  Time (s); DetIndx "),
             (Int_t) TRangeSpill * 10,
             0.,
             TRangeSpill,
             NStations,
             0.,
             NStations);

  if (fdMemoryTime > 0.) {  // book histograms for memory effects
    fhLHTime.resize(iNbDet);
    fvLHit.resize(iNbDet);
    Int_t iNbins = 32;  // corresponds to 2x2 cm^2 regions
    for (Int_t iDet = 0; iDet < iNbDet; iDet++) {
      fhLHTime[iDet] = new TH2D(Form("hLHTime_Det%d", iDet),
                                Form("hLHTime_Det%d ;  x (cm); y (cm) ", iDet),
                                iNbins,
                                -16.,
                                16.,
                                iNbins,
                                -16.,
                                16.);
      fvLHit[iDet].resize(iNbins * iNbins);
      for (size_t iBin = 0; iBin < fvLHit[iDet].size(); iBin++)
        fvLHit[iDet][iBin] = NULL;
    }
  }

  gDirectory->cd(
    oldir
      ->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!

  return kTRUE;
}

// ------------------------------------------------------------------
Bool_t CbmTofAnaTestbeam::FillHistos() {
  std::set<Int_t> DutHitSet;

  for (Int_t iHitInd = 0; iHitInd < fTofHitsColl->GetEntriesFast(); iHitInd++) {
    CbmTofHit* pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
    Int_t iDetId    = (pHit->GetAddress() & DetMask);

    if (fiDutAddr == iDetId) { DutHitSet.emplace(iHitInd); }
  }


  if (fbMonteCarloComparison) {
    if (fbTracksInInputFile) {
      fiNAccRefTracks = 0;

      TGeoNode* tNode(NULL);
      TGeoMedium* tMedium(NULL);
      TGeoMaterial* tMaterial(NULL);

      const char* cMaterialName;

      fhAccTrackMul->Fill(fAccTracks->GetEntriesFast());

      // Determine the number of reference tracks in the reconstructed event
      // as a function of which several QA observables are studied. A reference
      // track must originate from the target (not necessarily from the primary
      // vertex) and intersect the active planes of at least 'fiMinMCRefTrackPoints'
      // different counters.
      Int_t iNAccPrimTracks(0);

      for (Int_t iTrack = 0; iTrack < fAccTracks->GetEntriesFast(); iTrack++) {
        CbmMCTrack* tAccTrack =
          dynamic_cast<CbmMCTrack*>(fAccTracks->At(iTrack));
        CbmMatch* tAccTrackPointMatch =
          dynamic_cast<CbmMatch*>(fTofAccTrackPointMatches->At(iTrack));

        tNode     = gGeoManager->FindNode(tAccTrack->GetStartX(),
                                      tAccTrack->GetStartY(),
                                      tAccTrack->GetStartZ());
        tMedium   = tNode->GetMedium();
        tMaterial = tMedium->GetMaterial();

        GetMaterialName(tMaterial->GetName(), cMaterialName);

        if (0 == std::strcmp("target", cMaterialName)) {
          iNAccPrimTracks++;

          if (fiMinMCRefTrackPoints <= tAccTrackPointMatch->GetNofLinks()) {
            fiNAccRefTracks++;
          }
        }
      }

      fhAccRefTrackMul->Fill(fiNAccRefTracks);
      fhAccPrimTrackMul->Fill(iNAccPrimTracks);
    }
  }


  // Constants, TODO => put as parameter !!!

  // Declare variables outside the loop
  CbmTofHit* pHit;
  CbmTofHit* pLHit;
  CbmTofHit* pHit1;
  CbmTofHit* pHit2;
  CbmTofHit* pHit3;
  CbmTofHit* pHit4;
  CbmTofHit* pHitRef  = NULL;  // May be used uninitialized later, to check!
  CbmTofHit* pHitSel2 = NULL;
  CbmTofHit* pDia;
  CbmTofCell* fChannelInfo1;
  CbmTofCell* fChannelInfo2;
  CbmTofCell* fChannelInfo3;
  CbmTofCell* fChannelInfo4;

  Int_t iBRefHitInd(-1);
  Int_t iMRefHitInd(-1);
  Int_t iSel2HitInd(-1);
  CbmTrackMatchNew tSelHitTupleTrackMatch;
  Bool_t bGoodTrackSel(kFALSE);

  // Trb System
  if (NULL != fTrbHeader) {
    if (fiReqTrg > -1)
      if (!fTrbHeader->TriggerFired(fiReqTrg)) return kFALSE;
    UInt_t uTriggerPattern = fTrbHeader->GetTriggerPattern();
    for (UInt_t uChannel = 0; uChannel < 16; uChannel++) {
      if (uTriggerPattern & (0x1 << uChannel)) {
        fhTriggerPattern->Fill(uChannel);
      }
    }
    fhTriggerType->Fill(fTrbHeader->GetTriggerType());
    fhTimeInSpill->Fill(fTrbHeader->GetTimeInSpill());
  }


  /*   Int_t iNbTofDigis;*/
  Int_t iNbTofHits, iNbTofTracks;

  //   iNbTofDigis   = fTofDigisColl->GetEntriesFast();
  iNbTofHits = fTofHitsColl->GetEntriesFast();

  /*
   LOG(info)<<Form("CbmTofAnaTestbeam::FillHistos for %d digis and %d Tof hits",iNbTofDigis,iNbTofHits)
	    ;
   */
  // Digis info
  /*
   Double_t dTotalDataSize = 0;
   if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
   {
      CbmTofDigiExp *pDigi;
      for( Int_t iDigInd = 0; iDigInd < iNbTofDigis; iDigInd++ )
      {
         pDigi = (CbmTofDigiExp*) fTofDigisColl->At( iDigInd );

         Int_t iSmType = pDigi->GetType();
         Int_t iSm     = pDigi->GetSm();
         Int_t iRpc    = pDigi->GetRpc();
         Int_t iCh     = pDigi->GetChannel();

      } // for( Int_t iDigInd = 0; iDigInd < iNbTofDigis; iDigInd++ )
   } // if( kTRUE == fDigiBdfPar->UseExpandedDigi() )
   */
  // Hits info
  Int_t iNbMatchedHits = 0;
  Int_t iNbMaxMatch    = 500;
  //   Double_t Zref=300.;
  /*   Double_t Chi2MatchMin=1.E8;*/
  Double_t Chi2List[iNbMaxMatch];
  CbmTofHit* pChi2Hit1[iNbMaxMatch];
  CbmTofHit* pChi2Hit2[iNbMaxMatch];
  Chi2List[0]  = 1.E8;
  pChi2Hit1[0] = NULL;
  pChi2Hit2[0] = NULL;
  pDia         = NULL;
  Int_t iNSel  = 1;
  Bool_t BSel[iNSel];
  for (Int_t iSel = 0; iSel < iNSel; iSel++) {
    BSel[iSel] = kFALSE;
  }
  Double_t dDTD4    = 0.;
  dTDia             = 1.E300;
  dDTD4Min          = 1.E300;
  Double_t dMul0    = 0.;
  Double_t dMul4    = 0.;
  Double_t dMulS2   = 0.;
  Double_t dStrMul0 = 0.;
  Double_t dStrMul4 = 0.;
  Double_t dMulD    = 0.;
  Double_t dM4Max   = 1;  // modify
  if (fdMul4Max > 0) dM4Max = fdMul4Max;
  Double_t dM0Max = 100;  // modify
  if (fdMul0Max > 0) dM0Max = fdMul0Max;
  Double_t dMDMax = 1;
  if (fdMulDMax > 0) dMDMax = fdMulDMax;
  Double_t hitpos1[3], hitpos2[3], hitpos3[3], hitpos4[3];
  Double_t hitpos1_local[3], hitpos2_local[3], hitpos3_local[3],
    hitpos4_local[3];
  std::vector<CbmTofHit*> vDiaHit;
  Double_t DDiaAvLim = 0.2;  // average width for fastest diamond hits in ns
  Double_t dMulDAv   = 0;
  Double_t dTAv      = 0.;
  Double_t dMAv      = 0.;
  fDetIdMap.clear();


  if (fiBeamRefAddr
      == 0x00055056)  // determine reference time from earliest hits (25%)
  {
    //fTofHitsColl->Sort(); //not sortable!
    Int_t iBeamRefMul = iNbTofHits / 4;
    if (iBeamRefMul > 0) {
      pHit         = (CbmTofHit*) fTofHitsColl->At(iNbTofHits - 1);
      Int_t iDetId = (pHit->GetAddress() & DetMask);
      if (iDetId == fiBeamRefAddr) {
        dMulD       = (Double_t) iBeamRefMul;
        pDia        = pHit;
        iBRefHitInd = iNbTofHits - 1;
        dTDia       = pDia->GetTime();
        dTAv        = dTDia;
        dMAv        = 1.;
      } else {
        dTDia = 0.;
        Double_t vdTimes[iBeamRefMul];
        Int_t iLast    = 0;
        vdTimes[iLast] = 1.E300;
        for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
          pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
          if (NULL == pHit) continue;
          iDetId = (pHit->GetAddress() & DetMask);
          if (iDetId != fiDutAddr) {
            if (pHit->GetTime() < vdTimes[iLast]) {  // insert or replace
              if (iLast < iBeamRefMul) { iLast++; }
              Int_t iT;
              for (iT = 0; iT < iLast; iT++)
                if (pHit->GetTime() < vdTimes[iT]) break;
              for (Int_t iL = iLast; iL > iT; iL--)
                vdTimes[iL] = vdTimes[iL - 1];
              vdTimes[iT] = pHit->GetTime();
            }
          }
        }
        /*
	   LOG(info)<<Form("Sorted %d hits of %d (%d): ", iLast, iBeamRefMul, iNbTofHits);
	   for(Int_t iT=0; iT<iLast; iT++)  LOG(info)<<Form(" %7.2f",vdTimes[iT]);
	   LOG(info);
	 */
        for (Int_t iHit = 0; iHit < iLast; iHit++) {
          dTDia += vdTimes[iHit];
        }
        dTDia /= (Double_t) iLast;
        dMulD                 = (Double_t) iLast;
        Double_t hitPos[3]    = {3 * 0.};
        Double_t hitPosErr[3] = {3 * 1.};
        pDia                  = new CbmTofHit(fiBeamRefAddr,
                             hitPos,
                             hitPosErr,  //local detector coordinates
                             iNbTofHits,  // this number is used as reference!!
                             dTDia,
                             0,  // number of linked digis =  2*CluSize
                             10);  //channel -> Tot
        new ((*fTofHitsColl)[iNbTofHits])
          CbmTofHit(*pDia);  // insert fake hit into TClonesarray
        dTAv = dTDia;
        dMAv = 1.;
        pDia->Delete();
        pDia        = (CbmTofHit*) fTofHitsColl->At(iNbTofHits);
        iBRefHitInd = iNbTofHits;
        iNbTofHits++;
      }
    }
  } else
    // find diamond reference (BRef)
    for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
      pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
      if (NULL == pHit) continue;
      Int_t iDetId = (pHit->GetAddress() & DetMask);
      dTAv += pHit->GetTime();
      dMAv += 1.;

      std::map<UInt_t, UInt_t>::iterator it = fDetIdMap.find(iDetId);
      if (it == fDetIdMap.end()) fDetIdMap[iDetId] = fDetIdMap.size();

      /*
      Int_t iChId = pHit->GetAddress();
      fChannelInfo = fDigiPar->GetCell( iChId );
      Int_t iSmType=CbmTofAddress::GetSmType( iDetId );

      if(NULL == fChannelInfo){
        LOG(debug) << Form("CbmTofAnaTestbeam::FillHistos: NULL Channel Pointer for ChId 0x%08x ",iChId)
		   ;
	continue;
      }
      */
      /*
      LOG(debug) << Form(" BRef 0x%08x == 0x%08x ? ",iDetId,fiBeamRefAddr)
		 ;
      */
      if (iDetId
          == fiBeamRefAddr) {  // diamond hit (or other reference counter)
        dMulD++;
        vDiaHit.resize(dMulD);
        vDiaHit[dMulD - 1] = pHit;
        if (pHit->GetTime() < dTDia) {
          dTDia       = pHit->GetTime();
          pDia        = pHit;
          iBRefHitInd = iHitInd;
        }
      }
      if (dMulD > 0) {  // average fastest channels
        dMulDAv = 1;
        for (Int_t iDiaHit = 0; iDiaHit < dMulD; iDiaHit++) {
          if (vDiaHit[iDiaHit] != pDia) {  //additional hit found
            if (TMath::Abs(vDiaHit[iDiaHit]->GetTime() - dTDia) < DDiaAvLim) {
              dTDia =
                (dTDia * dMulDAv + vDiaHit[iDiaHit]->GetTime()) / (dMulDAv + 1);
              dMulDAv++;
            }
          }
        }
      }
    }  // reaction reference search loop end;
  fhBRefMul->Fill(dMulD);
  LOG(debug)
    << Form(
         "Diamond mul %3.0f, mulAv %3.0f, time: %6.2e", dMulD, dMulDAv, dTDia)
    << Form(", inspect Dut 0x%08x, Ref 0x%08x, Sel2  0x%08x, Sel3  0x%08x ",
            fiDutAddr,
            fiMrpcRefAddr,
            fiMrpcSel2Addr,
            fiMrpcSel3Addr);
  dTAv /= dMAv;  //use any hit

  if (iNbTofHits > 5) {  // FIXME hard wired constant in code
    if (StartAnalysisTime == 0.) {
      StartAnalysisTime = dTAv;
      dTLEvt            = dTAv;
      LOG(info) << "StartAnalysisTime from TAv set to " << StartAnalysisTime
                << " ns. ";
    }  // process counter hits, fill Chi2List, check selector
  }

  vector<CbmTofHit*> vDutHit;
  vector<CbmTofHit*> vRefHit;

  iNbTofHits = fTofHitsColl->GetEntriesFast();
  for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
    pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
    if (NULL == pHit) continue;
    Int_t iDetId = (pHit->GetAddress() & DetMask);
    Int_t iChId  = pHit->GetAddress();
    fChannelInfo = fDigiPar->GetCell(iChId);
    // Int_t iSmType = CbmTofAddress::GetSmType( iDetId );  (VF) not used
    Int_t iDetInd = fDigiBdfPar->GetDetInd(
      iDetId);  //fMbsMappingPar->GetMappedDetInd( pHit->GetAddress());
    LOG(debug) << Form("process %d.(%d) Tof hit 0x%08x, Ind %d, x = %6.1f, y = "
                       "%6.1f, z=%6.1f, t=%10.1f",
                       iHitInd,
                       iNbTofHits,
                       iChId,
                       iDetInd,
                       pHit->GetX(),
                       pHit->GetY(),
                       pHit->GetZ(),
                       pHit->GetTime());

    if (NULL == fChannelInfo) {
      LOG(debug) << "NULL Channel Pointer for ChId " << Form(" 0x%08x ", iChId);
      if (iDetId != fiBeamRefAddr)
        continue;  // exception for software reference
    }

    if (static_cast<UInt_t>(iDetInd) < fhXYPos.size())
      fhXYPos[iDetInd]->Fill(pHit->GetX(), pHit->GetY());

    Double_t dPhi =
      TMath::ATan2(pHit->GetY(), pHit->GetX()) * 180.0 / TMath::Pi();
    Double_t dTheta = TMath::ATan(
      TMath::Sqrt(pHit->GetX() * pHit->GetX() + pHit->GetY() * pHit->GetY())
      / pHit->GetZ());
    Double_t dEta = -TMath::Log(TMath::Tan(dTheta * 0.5));
    //LOG(info) << "x,y,z->phi: "<< dPhi <<", "<<dTheta<<", "<<dEta;
    dPhi += 90.;  // FIXME, just for STAR2018
    dPhi *= -1.;
    fhEtaPhi->Fill(dPhi, dEta);

    LOG(debug2) << "process iDetId  " << Form(" 0x%08x ", iDetId)
                << Form(", Muls %4.0f, %4.0f, %4.0f", dMulD, dMul0, dMul4);


    if (fiDutAddr == iDetId) {  // Process Dut
      dMul0++;
      vDutHit.push_back(pHit);

      CbmMatch* digiMatch =
        (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit));
      dStrMul0 += digiMatch->GetNofLinks() / 2.;
      //Double_t xPos1=Zref/pHit->GetZ()*pHit->GetX();
      //Double_t yPos1=Zref/pHit->GetZ()*pHit->GetY();
      Double_t xPos1  = pHit->GetX();
      Double_t yPos1  = pHit->GetY();
      Double_t zPos1  = pHit->GetZ();
      Double_t tof1   = pHit->GetTime();
      Double_t dzscal = 1.;

      for (Int_t iHitInd2 = 0; iHitInd2 < iNbTofHits; iHitInd2++) {
        if (iHitInd2 != iHitInd) {
          pHit2 = (CbmTofHit*) fTofHitsColl->At(iHitInd2);
          if (NULL == pHit2) continue;

          Int_t iDetId2 = (pHit2->GetAddress() & DetMask);
          Int_t iChId2  = pHit2->GetAddress();
          fChannelInfo2 = fDigiPar->GetCell(iChId2);

          if (fiMrpcRefAddr == iDetId2
              /*
	            fiMrpcRef  == CbmTofAddress::GetSmType( iDetId2 )
	       && fiMrpcRefSm  == CbmTofAddress::GetSmId( iDetId2 )
	       && fiMrpcRefRpc == CbmTofAddress::GetRpcId( iDetId2 )
	     */
          ) {  // Dut - MrpcRef

            if (fEnableMatchPosScaling) dzscal = zPos1 / pHit2->GetZ();

            Double_t xPos2 = dzscal * pHit2->GetX();
            Double_t yPos2 = dzscal * pHit2->GetY();
            Double_t tof2  = pHit2->GetTime();
            Double_t dTcor = 0.;
            if (fhDTD4DT04D4Off != NULL)
              dTcor = (Double_t) fhDTD4DT04D4Off->GetBinContent(
                fhDTD4DT04D4Off->FindBin(dTDia - tof2 - fdTShift));

            Double_t Chi2Match =
              TMath::Power((xPos1 - xPos2 - fdDXMean) / GetSigX(0), 2.)
              + TMath::Power((yPos1 - yPos2 - fdDYMean) / GetSigY(0), 2.)
              + TMath::Power((tof1 - tof2 - dTcor - fdDTMean) / GetSigT(0), 2.);
            if (Chi2Match > 1.E8) continue;
            Chi2Match /= 3;

            LOG(debug2) << " Chi2 "
                        << Form(" %f %f %f %f %f %f ",
                                fdDXMean,
                                GetSigX(0),
                                fdDYMean,
                                GetSigY(0),
                                fdDTMean,
                                GetSigT(0))
                        << Form(" -> %f ", Chi2Match);
            LOG(debug2) << " Chi2 "
                        << Form(" %f %f %f %f %f %f ",
                                xPos1,
                                xPos2,
                                yPos1,
                                yPos2,
                                tof1,
                                tof2)
                        << Form(" -> %f ", Chi2Match);

            iNbMatchedHits++;  // count Dut - Ref matches
            if (iNbMatchedHits == iNbMaxMatch)
              iNbMatchedHits = iNbMaxMatch - 1;  //prevent array overflow
            LOG(debug) << Form("match %d (%f):  %2d. - %2d. Tof hit 0x%08x "
                               "with 0x%08x, DeltaT = %f ns",
                               iNbMatchedHits,
                               Chi2Match,
                               iHitInd,
                               iHitInd2,
                               iChId2,
                               iChId,
                               tof1 - tof2 - dTcor - fdDTMean);

            fhDXDY04->Fill(xPos1 - xPos2, yPos1 - yPos2);
            fhDXDT04->Fill(xPos1 - xPos2, tof1 - tof2 - dTcor - fdDTMean);
            fhDYDT04->Fill(yPos1 - yPos2, tof1 - tof2 - dTcor - fdDTMean);
            fhChi04->Fill(Chi2Match);

            for (Int_t iM = 0; iM < iNbMatchedHits; iM++) {
              if (Chi2Match < Chi2List[iM]) {

                LOG(debug) << Form(" fill Chi2 %3d (%3d), %12.1f, %12.1f: "
                                   "HitInd %3d, %3d, Ids 0x%08x, 0x%08x",
                                   iM,
                                   iNbMatchedHits,
                                   Chi2Match,
                                   Chi2List[iM],
                                   iHitInd,
                                   iHitInd2,
                                   iChId,
                                   iChId2);

                for (Int_t iMM = iNbMatchedHits; iMM >= iM; iMM--) {
                  Chi2List[iMM]  = Chi2List[iMM - 1];
                  pChi2Hit1[iMM] = pChi2Hit1[iMM - 1];
                  pChi2Hit2[iMM] = pChi2Hit2[iMM - 1];
                }

                Chi2List[iM]             = Chi2Match;
                pChi2Hit1[iM]            = pHit;
                pChi2Hit2[iM]            = pHit2;
                Chi2List[iNbMatchedHits] = 1.E8;
                if (iM > 0) {
                  if (Chi2Match == Chi2List[iM - 1]) {
                    LOG(debug) << Form("same Chi2?: M %d, Mul0 %3.0f, HitInd "
                                       "%d, %d, Ids 0x%p, 0x%p - 0x%p, 0x%p",
                                       iM,
                                       dMul0,
                                       iHitInd,
                                       iHitInd2,
                                       pHit,
                                       pHit2,
                                       pChi2Hit1[iM - 1],
                                       pChi2Hit2[iM - 1]);
                    LOG(debug) << pHit->ToString();
                    LOG(debug) << pChi2Hit1[iM - 1]->ToString();
                    LOG(debug) << pHit2->ToString();
                  }
                }
                break;
              }
            }
          }                                              //fiMrpcRef condition end
          if (2 == CbmTofAddress::GetSmType(iDetId2)) {  // Plastic
            Int_t iSm = CbmTofAddress::GetSmId(iDetId2);
            if (iSm < 2) {
              fhXX02[iSm]->Fill(pHit->GetX(), dzscal * pHit2->GetX());
              fhYY02[iSm]->Fill(pHit->GetY(), dzscal * pHit2->GetY());
            }
          }
        }  //iHit2 != iHit condition end
      }    // iHit2 loop end
    }      // fiDut condition end

    if (fiMrpcRefAddr == iDetId) {
      dMul4++;
      vRefHit.push_back(pHit);
      CbmMatch* digiMatch =
        (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit));
      dStrMul4 += digiMatch->GetNofLinks() / 2.;
    }

    if (fiMrpcSel2Addr == iDetId) { dMulS2++; }

    if (fiBeamRefAddr == iDetId) {  // process beam Ref hit
      LOG(debug2) << "Process BeamRef Hit with " << pHit << ", " << pDia;
      if (pHit != pDia) continue;
      if (fdDTDia > 0.) {
        Double_t dDDia = 0.;
        for (Int_t iHitInd1 = 0; iHitInd1 < iNbTofHits; iHitInd1++)
          if (iHitInd1 != iHitInd) {
            pHit1 = (CbmTofHit*) fTofHitsColl->At(iHitInd1);
            if (pHit1 == NULL) continue;
            Int_t iDetId1 = (pHit1->GetAddress() & DetMask);
            Int_t iChId1  = pHit1->GetAddress();
            fChannelInfo1 = fDigiPar->GetCell(iChId1);
            pHit1         = (CbmTofHit*) fTofHitsColl->At(iHitInd1);
            if (fiBeamRefSmType == CbmTofAddress::GetSmType(iDetId1)
                && fiBeamRefSmId != CbmTofAddress::GetSmId(iDetId1)
                && TMath::Abs(pHit1->GetTime() - dTDia)
                     < fdDTDia) {  // second diamond fired
              dDDia = pHit1->GetTime() - dTDia;
            }
          }
        LOG(debug) << Form("DDia %f", dDDia);
        if (dDDia == 0.) continue;
      }  // 2 diamond condition end

      Double_t zPos1 = pHit->GetZ();
      for (Int_t iHitInd2 = 0; iHitInd2 < iNbTofHits; iHitInd2++)
        if (iHitInd2 != iHitInd) {
          pHit2 = (CbmTofHit*) fTofHitsColl->At(iHitInd2);
          if (pHit2 == NULL) continue;

          Int_t iDetId2 = (pHit2->GetAddress() & DetMask);
          Int_t iChId2  = pHit2->GetAddress();
          fChannelInfo2 = fDigiPar->GetCell(iChId2);
          if (NULL == fChannelInfo2) {
            LOG(debug) << "Invalid Channel Pointer for ChId2 "
                       << Form(" 0x%08x ", iChId2);
            continue;
          }
          LOG(debug2) << Form("TDia %f, THit %f", dTDia, pHit2->GetTime());

          if (fiMrpcRefAddr == iDetId2  // Beam - Ref coincidence
          ) {                           // Reference RPC hit
            dDTD4 = pHit2->GetTime() - dTDia + fdTShift;
            fhDTD4->Fill(dDTD4);

            LOG(debug) << Form("dDTD4 %8.4e, min: %8.4e", dDTD4, dDTD4Min);

            if (TMath::Abs(dDTD4) < fdDTD4MAX
                &&  // single selection scheme, selector 0
                TMath::Abs(CbmTofAddress::GetChannelId(iChId2) - fdCh4Sel)
                  < fdDCh4Sel) {
              LOG(debug1) << Form("Valid Mrpc hit 0x%08x", iChId2);
              /*TGeoNode *fNode=*/  // prepare global->local trafo
              gGeoManager->FindNode(fChannelInfo2->GetX(),
                                    fChannelInfo2->GetY(),
                                    fChannelInfo2->GetZ());
              Double_t hitpos[3], hitpos_local[3];
              hitpos[0] = pHit2->GetX();
              hitpos[1] = pHit2->GetY();
              hitpos[2] = pHit2->GetZ();
              /*TGeoNode* cNode=*/gGeoManager->GetCurrentNode();
              gGeoManager->MasterToLocal(hitpos, hitpos_local);
              if (TMath::Abs(hitpos_local[1] - fdPosY4SelOff)
                    < fdPosY4Sel * fChannelInfo2->GetSizey()
                  && TMath::Abs(dDTD4) < TMath::Abs(dDTD4Min)) {
                BSel[0]         = kFALSE;  // invalidate previous matches
                dDTD4Min        = dDTD4;
                pHitRef         = pHit2;
                iMRefHitInd     = iHitInd2;
                fChannelInfoRef = fChannelInfo2;
                LOG(debug1) << Form("accept Mrpc, look for Sel2 %d, 0x%08x",
                                    fiMrpcSel2,
                                    fiMrpcSel2Addr);
                if (fiMrpcSel2
                    < 0) {  // assume Mrpctype to be initialized to -1
                  BSel[0] = kTRUE;
                } else {  // request presence of coincident fiMrpcSel2 hit!
                  Double_t dzscal = 1.;
                  Double_t xPos2  = pHit2->GetX();
                  Double_t yPos2  = pHit2->GetY();
                  Double_t zPos2  = pHit2->GetZ();
                  Double_t tof2   = pHit2->GetTime();
                  Double_t dTcor  = 0.;
                  Double_t xPos3B = 0.;
                  Double_t yPos3B = 0.;
                  Double_t tof3B  = 0.;
                  //	 if(fhDTD4DT04D4Off != NULL)
                  //  dTcor=(Double_t)fhDTD4DT04D4Off->GetBinContent(fhDTD4DT04D4Off->FindBin(dTDia-tof2));

                  Double_t Chi2Max = fdChi2Lim2;
                  pHitSel2         = NULL;

                  for (Int_t iHitInd3 = 0; iHitInd3 < iNbTofHits; iHitInd3++) {
                    LOG(debug2) << Form("inspect %d. Sel2, Ind %d, Ind2 %d ",
                                        iHitInd3,
                                        iHitInd,
                                        iHitInd2);

                    //if(iHitInd3 != iHitInd && iHitInd3 != iHitInd2)
                    if (iHitInd3 != iHitInd2) {
                      pHit3 = (CbmTofHit*) fTofHitsColl->At(iHitInd3);
                      if (pHit3 == NULL) continue;
                      Int_t iDetId3 = (pHit3->GetAddress() & DetMask);
                      Int_t iChId3  = pHit3->GetAddress();
                      fChannelInfo3 = fDigiPar->GetCell(iChId3);
                      if (NULL == fChannelInfo3) {
                        LOG(debug) << "Invalid Channel Pointer for ChId3 "
                                   << Form(" 0x%08x ", iChId3);
                        continue;
                      }
                      LOG(debug2)
                        << Form("inspect %d. Sel2 0x%08x", iHitInd3, iDetId3);

                      if (
                        fiMrpcSel2Addr
                        == iDetId3) {  //CbmTofAddress::GetSmType( iDetId3 )){   // Sel2 RPC hit
                        LOG(debug1)
                          << Form("found Sel2 0x%08x, z-info: %7.1f, %7.1f",
                                  fiMrpcSel2Addr,
                                  dzscal,
                                  zPos1);
                        if (TMath::Abs(CbmTofAddress::GetChannelId(iChId3)
                                       - fdChS2Sel)
                            < fdDChS2Sel) {
                          /*TGeoNode *fNode3= */  // prepare global->local trafo
                          gGeoManager->FindNode(fChannelInfo3->GetX(),
                                                fChannelInfo3->GetY(),
                                                fChannelInfo3->GetZ());
                          hitpos3[0] = pHit3->GetX();
                          hitpos3[1] = pHit3->GetY();
                          hitpos3[2] = pHit3->GetZ();
                          /*TGeoNode* cNode3=*/gGeoManager->GetCurrentNode();
                          gGeoManager->MasterToLocal(hitpos3, hitpos3_local);
                          if (TMath::Abs(hitpos3_local[1] - fdPosYS2SelOff)
                              < fdPosYS2Sel * fChannelInfo3->GetSizey()) {
                            if (fEnableMatchPosScaling)
                              dzscal = zPos2 / pHit3->GetZ();
                            Double_t xPos3 = dzscal * pHit3->GetX();
                            Double_t yPos3 = dzscal * pHit3->GetY();
                            Double_t tof3  = pHit3->GetTime();

                            Double_t Chi2Match =
                              TMath::Power((xPos3 - xPos2) / GetSHTSigX(), 2.)
                              + TMath::Power((yPos3 - yPos2) / GetSHTSigY(), 2.)
                              + TMath::Power((tof3 - tof2 - dTcor - fdSel2TOff)
                                               / GetSHTSigT(),
                                             2.);

                            Chi2Match /= 3;
                            LOG(debug1)
                              << Form("valid Sel2 0x%08x with Chi2 %7.1f, "
                                      "%7.1f, %7.1f, %7.1f, %7.1f, %7.1f",
                                      fiMrpcSel2Addr,
                                      Chi2Match,
                                      Chi2Max,
                                      dzscal,
                                      xPos3,
                                      yPos3,
                                      tof3);

                            if (Chi2Match < Chi2Max) {
                              xPos3B           = xPos3;
                              yPos3B           = yPos3;
                              tof3B            = tof3;
                              Chi2Max          = Chi2Match;
                              BSel[0]          = kTRUE;
                              pHitSel2         = pHit3;
                              iSel2HitInd      = iHitInd3;
                              fChannelInfoSel2 = fChannelInfo3;
                              LOG(debug) << Form(
                                "better Sel2 0x%08x with Chi2 %7.1f, x %7.1f, "
                                "Deltax %7.1f, y %7.1f, Deltay %7.1f",
                                fiMrpcSel2Addr,
                                Chi2Max,
                                xPos3B,
                                xPos3B - xPos2,
                                yPos3B,
                                yPos3B - yPos2);
                            }
                          }
                        }
                      }
                    }
                  }  // loop over third hit end


                  if (BSel[0]) {
                    fhChiSel24->Fill(Chi2Max);
                    fhDXSel24->Fill(xPos3B - xPos2);
                    fhDYSel24->Fill(yPos3B - yPos2);
                    fhDTSel24->Fill(tof3B - tof2 - dTcor - fdSel2TOff);
                    fhTofSel24->Fill(tof3B - tof2 - fdSel2TOff);
                  }
                }
              }
            }
          }
        }  //LOG(info)<<" TDia="<<dTDia;
    }      // diamond condition end
  }        // for( Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++)


  /*
   if( dMulD>0 && dMul4>0 && dMul0>0 && dTDia - fdStartSpillTime > SpillDuration*1.E9 ) {
     Double_t dDTSpill=dTDia-fdStartSpillTime;
     fdStartSpillTime=dTDia;
   */
  if (fdSpillBreak == 0.) fdStartSpillTime = dTAv;

  Double_t dDTSpill = dTAv - fdStartSpillTime;
  if (fDetIdMap.size() > 3 && dMulD > 0) {  // FIXME - hardwired constants
    Double_t dDTLEvt = dTAv - dTLEvt;
    dTLEvt           = dTAv;
    if (dDTLEvt > fdSpillBreak * 1.E9 && dDTSpill > fdSpillDuration * 1.E9) {
      fdStartSpillTime = dTAv;
      iNspills++;
      LOG(info) << "StartSpillTime for " << iNspills
                << Form(". spill set to %f ns after %7.4f s, at event %d with "
                        "MulD %2.0f, MulDet %d, DTLE %7.4f s ",
                        fdStartSpillTime,
                        dDTSpill / 1.E9,
                        fEvents,
                        dMulD,
                        (Int_t) fDetIdMap.size(),
                        dDTLEvt / 1.E9);
    }
  }
  Double_t dTIR = (dTAv - StartAnalysisTime) / 1.E9;
  fhRate_all->Fill(dTIR, 1. / fhRate_all->GetBinWidth(1));
  if (fdStartSpillTime < 0) {
    LOG(debug) << "SpillStartTime not available, abort Anatestbeam ";
    return kFALSE;
  }
  //   if(dMul4>dM4Max || dMulD>dMDMax || dMul0>dM0Max)
  if (dMul0 > dM0Max || dMul4 > dM4Max || dMulD > dMDMax || dMulS2 > dM4Max) {
    BSel[0] = kFALSE;
    LOG(debug) << Form("<D> Muls %4.0f, %4.0f, %4.0f,  %4.0f, Matches %d",
                       dMulD,
                       dMul0,
                       dMul4,
                       dMulS2,
                       iNbMatchedHits);
  }

  // Determine average event quantities

  Double_t dDutTMean  = 0.;
  Double_t dDutTMean2 = 0.;
  Int_t iNDutHits     = vDutHit.size();
  for (Int_t i = 0; i < iNDutHits; i++) {  // loop over Dut Hits
    dDutTMean += vDutHit[i]->GetTime();
    dDutTMean2 += vDutHit[i]->GetTime() * vDutHit[i]->GetTime();
  }
  dDutTMean /= (Double_t) vDutHit.size();
  dDutTMean2 /= (Double_t) vDutHit.size();
  Double_t dDutTSig = TMath::Sqrt(dDutTMean2 - dDutTMean * dDutTMean);

  Double_t dRefTMean  = 0.;
  Double_t dRefTMean2 = 0.;
  Int_t iNRefHits     = vRefHit.size();
  for (Int_t i = 0; i < iNRefHits; i++) {  // loop over Ref Hits
    dRefTMean += vRefHit[i]->GetTime();
    dRefTMean2 += vRefHit[i]->GetTime() * vRefHit[i]->GetTime();
  }
  dRefTMean /= (Double_t) vRefHit.size();
  dRefTMean2 /= (Double_t) vRefHit.size();
  Double_t dRefTSig = TMath::Sqrt(dRefTMean2 - dRefTMean * dRefTMean);

  //  histogram distances
  for (Int_t i = 0; i < iNDutHits - 1; i++) {    // loop over Dut Hits
    for (Int_t j = i + 1; j < iNDutHits; j++) {  // loop over Dut Hits
      fhDutDXDYDT->Fill(vDutHit[j]->GetX() - vDutHit[i]->GetX(),
                        vDutHit[j]->GetY() - vDutHit[i]->GetY(),
                        vDutHit[j]->GetTime() - vDutHit[i]->GetTime());
    }
  }
  for (Int_t i = 0; i < iNRefHits - 1; i++) {    // loop over Ref Hits
    for (Int_t j = i + 1; j < iNRefHits; j++) {  // loop over Ref Hits
      fhRefDXDYDT->Fill(vRefHit[j]->GetX() - vRefHit[i]->GetX(),
                        vRefHit[j]->GetY() - vRefHit[i]->GetY(),
                        vRefHit[j]->GetTime() - vRefHit[i]->GetTime());
    }
  }

  //  normalisation distributions
  fhNMatch04->Fill(iNbMatchedHits);
  if (fTrbHeader != NULL)
    fhTIS_all->Fill(fTrbHeader->GetTimeInSpill());
  else
    fhTIS_all->Fill((dTAv - fdStartSpillTime) / 1.E9);

  fhTIR_all->Fill(dTIR);

  LOG(debug) << Form(
    " FoundMatches: %d with first chi2s = %12.1f, %12.1f, %12.1f, %12.1f",
    iNbMatchedHits,
    Chi2List[0],
    Chi2List[1],
    Chi2List[2],
    Chi2List[3])
             << Form(", Muls %4.0f, %4.0f, %4.0f", dMulD, dMul0, dMul4);


  if (BSel[0]) {
    fhSelEfficiency->Fill(kTRUE, fiNAccRefTracks);
    fhSelHitTupleEfficiencyTIS->Fill(kTRUE, (dTAv - fdStartSpillTime) / 1.E9);
  } else {
    fhSelEfficiency->Fill(kFALSE, fiNAccRefTracks);
    fhSelHitTupleEfficiencyTIS->Fill(kFALSE, (dTAv - fdStartSpillTime) / 1.E9);
  }


  // selector 0 distributions
  if (BSel[0]) {
    LOG(debug) << Form(" Found valid selector ");
    fhNMatchD4sel->Fill(iNbMatchedHits);  // use as normalisation

    if (fTrbHeader != NULL)
      fhTIS_sel->Fill(fTrbHeader->GetTimeInSpill());
    else
      fhTIS_sel->Fill((dTAv - fdStartSpillTime) / 1.E9);
    fhTIR_sel->Fill(dTIR);

    fhTofD4sel->Fill(pHitRef->GetTime() - dTDia);  //  general normalisation
    fhDTD4sel->Fill(dDTD4Min);                     //  general normalisation

    Int_t iDutCh                  = 0;
    Double_t dTimeSinceLastDutHit = 0.;

    if (fChannelInfoDut != NULL) {
      // Project into Dut reference frame
      /*TGeoNode *fNodeDut=*/  // prepare global->local trafo
      gGeoManager->FindNode(fChannelInfoDut->GetX(),
                            fChannelInfoDut->GetY(),
                            fChannelInfoDut->GetZ());

      hitpos1[0] = fChannelInfoDut->GetZ() / pHitRef->GetZ() * pHitRef->GetX();
      hitpos1[1] = fChannelInfoDut->GetZ() / pHitRef->GetZ() * pHitRef->GetY();
      hitpos1[2] = fChannelInfoDut->GetZ();

      /*TGeoNode* cNodeDut=*/gGeoManager->GetCurrentNode();
      gGeoManager->MasterToLocal(hitpos1, hitpos1_local);
      //hitpos1_local[0] -= fiDutNch/2 * fChannelInfoDut->GetSizex();
      fhXY0D4sel->Fill(hitpos1_local[0], hitpos1_local[1]);

      iDutCh = TMath::Floor(hitpos1_local[0] / fChannelInfoDut->GetSizex())
               + fiDutNch / 2;  // FIXME: needs proper calculation
      if (iDutCh < 0 || iDutCh > fiDutNch - 1) {
        LOG(debug) << "Predicted ch " << iDutCh
                   << " from x = " << hitpos1_local[0]
                   << ", Sizex =" << fChannelInfoDut->GetSizex();
        if (iDutCh < 0) iDutCh = 0;
        if (iDutCh > fiDutNch - 1) iDutCh = fiDutNch - 1;
      }
      if (NULL != fClusterizer)
        if (fClusterizer->fdMemoryTime > 0) {
          for (Int_t iDutChtest = iDutCh; iDutChtest < iDutCh + 1;
               iDutChtest++) {  // test possible strips
            if (iDutChtest < 0) iDutChtest = 0;
            if (iDutChtest > fiDutNch - 1) iDutChtest = fiDutNch - 1;
            if (fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutChtest]
                  .size()
                > 1) {
              std::list<CbmTofHit*>::iterator itL =
                fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutChtest]
                  .end();
              itL--;
              //find out, whether hit was added for current event
              for (Int_t i = 0; i < iNDutHits; i++) {  // loop over Dut Hits
                LOG(debug1)
                  << "Inspect " << i << " for address  match "
                  << (*itL)->GetAddress() << " - " << vDutHit[i]->GetAddress();
                if ((*itL)->GetAddress() == vDutHit[i]->GetAddress()) {
                  LOG(debug)
                    << "iDutCh hit was added to DTLH from Mul " << iNDutHits;
                  itL--;
                  break;  //
                }
              }
              // std::list<CbmTofHit *>::iterator itL=fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutChtest].begin();
              dTimeSinceLastDutHit = TMath::Max(
                0., TMath::Log10(pHitRef->GetTime() - (*itL)->GetTime()));
              break;  //leave for-loop
            } else
              dTimeSinceLastDutHit =
                9.9;  //generate entry in last bin of histogram
          }

          LOG(debug) << Form(
            " DTLH for iDutCh %02d Mul %03d: %6.1f from size %lu, ref %12.0f ",
            iDutCh,
            iNDutHits,
            dTimeSinceLastDutHit,
            //fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutCh-1].size(),
            fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutCh].size(),
            //fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutCh+1].size(),
            //	(*fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutCh-1].begin())->GetTime(),
            //	(*fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutCh].begin())->GetTime(),
            //(*(fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutCh].end())--)->GetTime(),
            //(*fClusterizer->fvLastHits[fiDut][fiDutSm][fiDutRpc][iDutCh+1].begin())->GetTime(),
            pHitRef->GetTime());
          fhDTLH_sel->Fill(dTimeSinceLastDutHit);
        }
    }

    // Monitor selected Reference Hit position
    /*TGeoNode *fNodeRef=*/  // prepare global->local trafo
    gGeoManager->FindNode(fChannelInfoRef->GetX(),
                          fChannelInfoRef->GetY(),
                          fChannelInfoRef->GetZ());

    hitpos2[0] = pHitRef->GetX();
    hitpos2[1] = pHitRef->GetY();
    hitpos2[2] = pHitRef->GetZ();
    /*TGeoNode* cNodeRef=*/gGeoManager->GetCurrentNode();
    gGeoManager->MasterToLocal(hitpos2, hitpos2_local);
    fhXY4D4sel->Fill(hitpos2_local[0], hitpos2_local[1]);

    if (NULL != pHitSel2) {
      /*Int_t iDetId3 = (pHitSel2->GetAddress() & DetMask);*/
      Int_t iChId3  = pHitSel2->GetAddress();
      fChannelInfo3 = fDigiPar->GetCell(iChId3);
      /*TGeoNode *fNode3=*/  // prepare global->local trafo
      gGeoManager->FindNode(
        fChannelInfo3->GetX(), fChannelInfo3->GetY(), fChannelInfo3->GetZ());
      hitpos3[0] = pHitSel2->GetX();
      hitpos3[1] = pHitSel2->GetY();
      hitpos3[2] = pHitSel2->GetZ();
      /*TGeoNode* cNode3=*/gGeoManager->GetCurrentNode();
      gGeoManager->MasterToLocal(hitpos3, hitpos3_local);
      fhXYSel2D4sel->Fill(hitpos3_local[0], hitpos3_local[1]);

      if (fTrbHeader != NULL)
        fhTIS_sel2->Fill(fTrbHeader->GetTimeInSpill());
      else
        fhTIS_sel2->Fill((dTAv - fdStartSpillTime) / 1.E9);
      fhTIR_sel2->Fill(dTIR);

      if (NULL != fClusterizer)
        if (fClusterizer->fdMemoryTime > 0) {
          fhDTLH_sel2->Fill(dTimeSinceLastDutHit);
        }
    }

    if (fbMonteCarloComparison) {
      if (fbTracksInInputFile) {
        Int_t iMatchedRealTrackLink(-1);
        std::set<Int_t> RealTrackLinkSet;

        // A Sel2 counter was defined. Assessing purity for a selector hit tuple
        // does not make too much sense if the MRef counter is the only reference
        // apart from a BRef (start) counter.
        if (pHitSel2) {
          Int_t iNSelHitTupleHits(3);

          CbmMatch* tMRefHitTrackMatch =
            dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iMRefHitInd));
          CbmMatch* tSel2HitTrackMatch =
            dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iSel2HitInd));
          CbmMatch* tBRefHitTrackMatch =
            dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iBRefHitInd));

          for (Int_t iMRefHitTrackLink = 0;
               iMRefHitTrackLink < tMRefHitTrackMatch->GetNofLinks();
               iMRefHitTrackLink++) {
            tSelHitTupleTrackMatch.AddLink(
              tMRefHitTrackMatch->GetLink(iMRefHitTrackLink));
          }

          for (Int_t iSel2HitTrackLink = 0;
               iSel2HitTrackLink < tSel2HitTrackMatch->GetNofLinks();
               iSel2HitTrackLink++) {
            tSelHitTupleTrackMatch.AddLink(
              tSel2HitTrackMatch->GetLink(iSel2HitTrackLink));
          }

          for (Int_t iBRefHitTrackLink = 0;
               iBRefHitTrackLink < tBRefHitTrackMatch->GetNofLinks();
               iBRefHitTrackLink++) {
            tSelHitTupleTrackMatch.AddLink(
              tBRefHitTrackMatch->GetLink(iBRefHitTrackLink));
          }

          const CbmLink& tSelHitTupleMatchedTrackLink =
            tSelHitTupleTrackMatch.GetMatchedLink();
          Double_t dSelHitTupleMaxTrackLinkWeight =
            tSelHitTupleMatchedTrackLink.GetWeight();

          for (Int_t iSelHitTupleTrackLink = 0;
               iSelHitTupleTrackLink < tSelHitTupleTrackMatch.GetNofLinks();
               iSelHitTupleTrackLink++) {
            const CbmLink& tSelHitTupleTrackLink =
              tSelHitTupleTrackMatch.GetLink(iSelHitTupleTrackLink);

            if (dSelHitTupleMaxTrackLinkWeight
                  == tSelHitTupleTrackLink.GetWeight()
                && -1 < tSelHitTupleTrackLink.GetIndex()) {
              iMatchedRealTrackLink = tSelHitTupleTrackLink.GetIndex();
              RealTrackLinkSet.emplace(tSelHitTupleTrackLink.GetIndex());
            }
          }

          Int_t iNTrueTupleHits =
            static_cast<Int_t>(dSelHitTupleMaxTrackLinkWeight);

          // The beam particle track link (-5, -5, -5) cannot add up to the
          // weight of track links found in the MRef and Sel2 counters. For
          // consistency, it should be counted as a true sel tuple hit, though.
          if (5 == fiBeamRefSmType) { iNTrueTupleHits++; }

          tSelHitTupleTrackMatch.SetNofTrueHits(iNTrueTupleHits);
          tSelHitTupleTrackMatch.SetNofWrongHits(iNSelHitTupleHits
                                                 - iNTrueTupleHits);

          // For a pure sel hit tuple, request a particle track match different
          // from a beam or a dark point.
          // The case that a noise link and a particle link have the same weight
          // and the noise link is - by chance - the matched one is handled here:
          // The noise link is ignored while assessing purity if and only if a
          // particle link with the same weight exists.
          if (-1 < iMatchedRealTrackLink) {
            if (1. == tSelHitTupleTrackMatch.GetTrueOverAllHitsRatio()) {
              fhSelPurity->Fill(kTRUE, fiNAccRefTracks);
              bGoodTrackSel = kTRUE;
            } else {
              fhSelPurity->Fill(kFALSE, fiNAccRefTracks);
            }
          } else {
            fhSelPurity->Fill(kFALSE, fiNAccRefTracks);
          }
        }


        if (bGoodTrackSel) {
          TGeoNode* tNode(NULL);
          TGeoMedium* tMedium(NULL);
          TGeoMaterial* tMaterial(NULL);

          const char* cMaterialName;

          const char* cSelRefTrackPdgName;
          const char* cSelRefTrackProcessName;

          Bool_t bSelRefTrack(kFALSE);

          for (auto const& iTrack : RealTrackLinkSet) {
            CbmMCTrack* tTrack =
              dynamic_cast<CbmMCTrack*>(fAccTracks->At(iTrack));
            CbmMatch* tTrackPointMatch =
              dynamic_cast<CbmMatch*>(fTofAccTrackPointMatches->At(iTrack));

            tNode = gGeoManager->FindNode(
              tTrack->GetStartX(), tTrack->GetStartY(), tTrack->GetStartZ());
            tMedium   = tNode->GetMedium();
            tMaterial = tMedium->GetMaterial();

            GetMaterialName(tMaterial->GetName(), cMaterialName);

            if (0 == std::strcmp("target", cMaterialName)
                && fiMinMCRefTrackPoints <= tTrackPointMatch->GetNofLinks()) {
              bSelRefTrack = kTRUE;

              GetPdgName(tTrack->GetPdgCode(), cSelRefTrackPdgName);
              GetProcessName(TMCProcessName[tTrack->GetGeantProcessId()],
                             cSelRefTrackProcessName);
            }
          }

          if (bSelRefTrack) {
            fhSelRefTrackShare->Fill(kTRUE, fiNAccRefTracks);
            fhSelRefTrackProcSpec->Fill(
              cSelRefTrackProcessName, cSelRefTrackPdgName, 1.);
          } else {
            fhSelRefTrackShare->Fill(kFALSE, fiNAccRefTracks);
          }
        }
      }
    }


    if (iNbMatchedHits > 0) {
      // best match
      LOG(debug) << Form("best match D4 (%d): 0x%p, 0x%p in ch 0x%08x, 0x%08x: "
                         "%12.1f < %12.1f ?",
                         iNbMatchedHits,
                         pChi2Hit1[0],
                         pChi2Hit2[0],
                         pChi2Hit1[0]->GetAddress(),
                         pChi2Hit2[0]->GetAddress(),
                         Chi2List[0],
                         fdChi2Lim);

      if (NULL != pHitSel2) {
        fhXYSel2D4best->Fill(hitpos3_local[0], hitpos3_local[1]);
      }

      pHit1     = pChi2Hit1[0];  //Dut
      pHit2     = pChi2Hit2[0];  //MRpcRef
      Int_t iM0 = 0;
      if (pHit2 != pHitRef) {
        LOG(debug) << " selector hit does not match reference hit for best "
                      "match, chi2best "
                   << Chi2List[0]
                   << Form(", ref found in Addr 0x%08x ",
                           pHitRef->GetAddress());
        for (iM0 = 1; iM0 < iNbMatchedHits; iM0++) {
          if (pHitRef == pChi2Hit2[iM0]) {
            LOG(debug) << " found reference hit for best match, chi2new "
                       << Chi2List[iM0];
            pHit1 = pChi2Hit1[iM0];
            pHit2 = pChi2Hit2[iM0];
            break;
          }
        }
        if (iM0 == iNbMatchedHits) {
          LOG(debug) << Form("no valid match in %d hits for Addr 0x%08x found ",
                             iNbMatchedHits,
                             pHitRef->GetAddress());
          if (gLogger->IsLogNeeded(fair::Severity::debug1))
            LOG(fatal) << "Check for consistency!";
          return 0;
        }
      }

      // 2019-01-02 chs
      // Calibration code was moved here to make the corrected time difference
      // 'dToD' between the DUT and MRef counters available also for the
      // (else) case that the selected DUT-MRef hit pair does not meet the
      // 'fdChi2Lim' criterion for an efficient match.
      Int_t iDetId1 = (pHit1->GetAddress() & DetMask);
      Int_t iChId1  = pHit1->GetAddress();
      fChannelInfo1 = fDigiPar->GetCell(iChId1);

      Int_t iDetId2 = (pHit2->GetAddress() & DetMask);
      Int_t iChId2  = pHit2->GetAddress();
      fChannelInfo2 = fDigiPar->GetCell(iChId2);

      Double_t xPos1  = pHit1->GetX();
      Double_t yPos1  = pHit1->GetY();
      Double_t zPos1  = pHit1->GetZ();
      Double_t tof1   = pHit1->GetTime();
      Double_t dzscal = 1.;
      if (fEnableMatchPosScaling) dzscal = zPos1 / pHit2->GetZ();

      Double_t xPos2 = dzscal * pHit2->GetX();
      Double_t yPos2 = dzscal * pHit2->GetY();
      Double_t tof2  = pHit2->GetTime();

      Double_t dDist =
        TMath::Sqrt(TMath::Power(pHit1->GetX() - pHit2->GetX(), 2)
                    + TMath::Power(pHit1->GetY() - pHit2->GetY(), 2)
                    + TMath::Power(pHit1->GetZ() - pHit2->GetZ(), 2));

      CbmMatch* digiMatch1 =
        (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit1));
      Double_t dCluSize0 = digiMatch1->GetNofLinks() / 2.;

      CbmMatch* digiMatch2 =
        (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit2));
      Double_t dCluSize4 = digiMatch2->GetNofLinks() / 2.;

      // check for dependence in counter reference frame
      /*TGeoNode *fNode=*/  // prepare global->local trafo
      gGeoManager->FindNode(
        fChannelInfo2->GetX(), fChannelInfo2->GetY(), fChannelInfo2->GetZ());

      hitpos2[0] = pHit2->GetX();
      hitpos2[1] = pHit2->GetY();
      hitpos2[2] = pHit2->GetZ();

      /*     TGeoNode* cNode=*/gGeoManager
        ->GetCurrentNode();  // -> Comment to remove warning because set but never used
      // if(0) cNode->Print();
      gGeoManager->MasterToLocal(hitpos2, hitpos2_local);

      Double_t dTofD4  = fdTOffD4 + dDTD4Min;
      Double_t dInvVel = dTofD4 / pHitRef->GetR();  // in ns/cm
      Double_t dDTexp  = dDist * dInvVel;
      Double_t dTMin   = fdHitDistAv / 30.;  // in ns
      CbmMatch* digiMatch0 =
        (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit1));
      Double_t dTot0 = 0.;

      if (fDigiMan->IsPresent(ECbmModuleId::kTof))
        for (Int_t iLink = 0; iLink < digiMatch0->GetNofLinks();
             iLink++) {  // loop over digis
          CbmLink L0              = digiMatch0->GetLink(iLink);
          Int_t iDigInd0          = L0.GetIndex();
          const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
          dTot0 += pDig0->GetTot();
          LOG(debug1) << Form(" dTot of hit 0x%08x: digind %d add %f -> sum %f",
                              iDetId1,
                              iDigInd0,
                              pDig0->GetTot(),
                              dTot0);
        }
      dTot0 /= digiMatch0->GetNofLinks();  // average time over threshold

      CbmMatch* digiMatch4 =
        (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit2));
      Double_t dTot4 = 0.;
      //     if(NULL != fTofDigisColl)
      if (fDigiMan->IsPresent(ECbmModuleId::kTof))
        for (Int_t iLink = 0; iLink < digiMatch4->GetNofLinks();
             iLink++) {  // loop over digis
          CbmLink L0              = digiMatch4->GetLink(iLink);
          Int_t iDigInd0          = L0.GetIndex();
          const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
          //  CbmTofDigi *pDig0 = (CbmTofDigi*) (fTofDigisColl->At(iDigInd0));
          dTot4 += pDig0->GetTot();
          LOG(debug1) << Form(" dTot of hit 0x%08x: digind %d add %f -> sum %f",
                              iDetId1,
                              iDigInd0,
                              pDig0->GetTot(),
                              dTot4);
        }
      dTot4 /= digiMatch4->GetNofLinks();  // average time over threshold

      Double_t dTcor = 0.;
      if (fhDTD4DT04D4Off != NULL)
        dTcor += (Double_t) fhDTD4DT04D4Off->GetBinContent(
          fhDTD4DT04D4Off->FindBin(-dDTD4Min));
      if (fhDTX4D4Off != NULL)
        dTcor += (Double_t) fhDTX4D4Off->GetBinContent(
          fhDTX4D4Off->FindBin(hitpos2_local[0]));
      if (fhDTY4D4Off != NULL)
        dTcor += (Double_t) fhDTY4D4Off->GetBinContent(
          fhDTY4D4Off->FindBin(hitpos2_local[1]));
      if (fhDTTexpD4Off != NULL)
        dTcor += (Double_t) fhDTTexpD4Off->GetBinContent(
          fhDTTexpD4Off->FindBin(dDTexp - dTMin));
      if (fhCluSize0DT04D4Off != NULL)
        dTcor += (Double_t) fhCluSize0DT04D4Off->GetBinContent(
          fhCluSize0DT04D4Off->FindBin(dCluSize0));
      if (fhCluSize4DT04D4Off != NULL)
        dTcor += (Double_t) fhCluSize4DT04D4Off->GetBinContent(
          fhCluSize4DT04D4Off->FindBin(dCluSize4));
      if (fhTot0DT04D4Off != NULL)
        dTcor += (Double_t) fhTot0DT04D4Off->GetBinContent(
          fhTot0DT04D4Off->FindBin(TMath::Log(dTot0)));
      if (fhTot4DT04D4Off != NULL)
        dTcor += (Double_t) fhTot4DT04D4Off->GetBinContent(
          fhTot4DT04D4Off->FindBin(TMath::Log(dTot4)));

      //    dTcor *= dDist/fdHitDistAv;
      Double_t dToD = (tof1 - tof2 - dTcor - fdDTMean);  //*fdHitDistAv/dDist;
      //     LOG(info) << "dTcor for "<<-dDTD4<<" from "<<fhDTD4DT04D4Off<<": "<<dTcor;


      Bool_t bGoodSelDutMatch(kFALSE);

      if (fbMonteCarloComparison) {
        Int_t iDutHitInd = fTofHitsColl->IndexOf(pHit1);

        if (fbTracksInInputFile) {
          if (bGoodTrackSel) {
            CbmMatch* tDutHitTrackMatch =
              dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iDutHitInd));

            Double_t dSelHitTupleMaxTrackLinkWeight =
              (tSelHitTupleTrackMatch.GetMatchedLink()).GetWeight();


            for (Int_t iSelHitTupleTrackLink = 0;
                 iSelHitTupleTrackLink < tSelHitTupleTrackMatch.GetNofLinks();
                 iSelHitTupleTrackLink++) {
              const CbmLink& tSelHitTupleTrackLink =
                tSelHitTupleTrackMatch.GetLink(iSelHitTupleTrackLink);

              if (dSelHitTupleMaxTrackLinkWeight
                  == tSelHitTupleTrackLink.GetWeight()) {
                for (Int_t iDutHitTrackLink = 0;
                     iDutHitTrackLink < tDutHitTrackMatch->GetNofLinks();
                     iDutHitTrackLink++) {
                  const CbmLink& tDutHitTrackLink =
                    tDutHitTrackMatch->GetLink(iDutHitTrackLink);

                  if (tSelHitTupleTrackLink == tDutHitTrackLink) {
                    bGoodSelDutMatch = kTRUE;
                    break;
                  }
                }
              }

              if (bGoodSelDutMatch) { break; }
            }

            if (bGoodSelDutMatch) {
              fhGoodSelTypeNNPureChiSq->Fill(0., 3. * Chi2List[iM0]);
            }

            fhGoodSelTypeNNAllChiSq->Fill(0., 3. * Chi2List[iM0]);
          }
        }
      }


      if (Chi2List[iM0] < fdChi2Lim) {

        fhSelMatchEfficiency->Fill(kTRUE, fiNAccRefTracks);
        fhSelHitTupleMatchEfficiencyTIS->Fill(kTRUE,
                                              (dTAv - fdStartSpillTime) / 1.E9);

        if (fTrbHeader != NULL)
          fhTIS_sel1->Fill(fTrbHeader->GetTimeInSpill());
        else
          fhTIS_sel1->Fill((dTAv - fdStartSpillTime) / 1.E9);
        fhTIR_sel1->Fill(dTIR);

        if (NULL != fClusterizer)
          if (fClusterizer->fdMemoryTime > 0) {
            Int_t iDut = CbmTofAddress::GetSmType(pHit1->GetAddress());
            if (fiDut != iDut)
              LOG(fatal) << "inconsistent Dut - type " << iDut
                         << " != " << fiDut;

            Int_t iCh = CbmTofAddress::GetChannelId(pHit1->GetAddress());
            if (iDutCh != iCh)
              LOG(debug) << "Inconsistent Dut strip #: " << iCh
                         << " != " << iDutCh;
            fhDTLH_DStrip->Fill((Double_t) iCh, (Double_t)(iCh - iDutCh));
            fhDTLH_sel1->Fill(dTimeSinceLastDutHit);
          }

        fhXX04->Fill(xPos1, xPos2);
        fhYY04->Fill(yPos1, yPos2);
        fhXY04->Fill(xPos1, yPos2);
        fhYX04->Fill(yPos1, xPos2);
        fhTT04->Fill(tof1, tof1 - tof2);


        fhChi04D4best->Fill(Chi2List[iM0]);


        fhDigiMul0D4best->Fill(dMul0, dCluSize0);
        fhDigiMul4D4best->Fill(dMul4, dCluSize4);

        fhCluSize04D4best->Fill(dCluSize0, dCluSize4);

        fhCluMul04D4best->Fill(dMul0, dMul4);
        fhStrMul04D4best->Fill(dStrMul0, dStrMul4);

        fhCluMulTSig0D4best->Fill(dMul0, dDutTSig);
        fhCluMulTSig4D4best->Fill(dMul4, dRefTSig);

        fhCluMulTrel0D4best->Fill(dMul0, pHit1->GetTime() - dDutTMean);
        fhCluMulTrel4D4best->Fill(dMul4, pHit2->GetTime() - dRefTMean);

        fhCluSizeTrel0D4best->Fill(dCluSize0, pHit1->GetTime() - dDutTMean);
        fhCluSizeTrel4D4best->Fill(dCluSize4, pHit2->GetTime() - dRefTMean);


        fhTofD4best->Fill(dTofD4);
        if (dInvVel > 0.) fhVelD4best->Fill(1. / dInvVel);

        fhChiDT04D4best->Fill(Chi2List[iM0], dToD);
        fhDTD4DT04D4best->Fill(-dDTD4Min, dToD);
        fhDTMul4D4best->Fill(dMul4, dToD);

        fhXY4D4best->Fill(hitpos2_local[0], hitpos2_local[1]);
        fhDTX4D4best->Fill(hitpos2_local[0], dToD);
        fhDTY4D4best->Fill(hitpos2_local[1], dToD);
        fhDXX4D4best->Fill(hitpos2_local[0], xPos1 - xPos2);
        fhDXY4D4best->Fill(hitpos2_local[1], xPos1 - xPos2);
        fhDYX4D4best->Fill(hitpos2_local[0], yPos1 - yPos2);
        fhDYY4D4best->Fill(hitpos2_local[1], yPos1 - yPos2);

        fhCluSize4DT04D4best->Fill(digiMatch2->GetNofLinks() / 2., dToD);

        Double_t dTot = 0.;
        if (fDigiMan->IsPresent(ECbmModuleId::kTof))
          for (Int_t iLink = 0; iLink < digiMatch2->GetNofLinks();
               iLink++) {  // loop over digis
            CbmLink L0              = digiMatch2->GetLink(iLink);
            Int_t iDigInd0          = L0.GetIndex();
            const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
            dTot += pDig0->GetTot();
            LOG(debug) << Form(
              " dTot of hit 0x%08x: digind %d add %f -> sum %f",
              iDetId2,
              iDigInd0,
              pDig0->GetTot(),
              dTot);
          }

        dTot /= digiMatch2->GetNofLinks();  // average time over threshold
        fhTot4DT04D4best->Fill(TMath::Log(dTot), dToD);

        fhCluSize0DT04D4best->Fill(digiMatch0->GetNofLinks() / 2., dToD);

        dTot = 0.;
        if (fDigiMan->IsPresent(ECbmModuleId::kTof))
          for (Int_t iLink = 0; iLink < digiMatch0->GetNofLinks();
               iLink++) {  // loop over digis
            CbmLink L0              = digiMatch0->GetLink(iLink);
            Int_t iDigInd0          = L0.GetIndex();
            const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
            dTot += pDig0->GetTot();
            LOG(debug1) << Form(
              " dTot of hit 0x%08x: digind %d add %f -> sum %f",
              iDetId1,
              iDigInd0,
              pDig0->GetTot(),
              dTot);
          }
        dTot /= digiMatch0->GetNofLinks();  // average time over threshold
        fhTot0DT04D4best->Fill(TMath::Log(dTot), dToD);

        fhCluSizeSigT0D4best->Fill(digiMatch1->GetNofLinks() / 2.,
                                   pHit1->GetTimeError());
        fhCluSizeSigT4D4best->Fill(digiMatch2->GetNofLinks() / 2.,
                                   pHit2->GetTimeError());

        fhDTMul0D4best->Fill(dMul0, dToD);

        // check for dependence in counter reference frame
        /*TGeoNode *fNode1=*/  // prepare global->local trafo
        gGeoManager->FindNode(
          fChannelInfo1->GetX(), fChannelInfo1->GetY(), fChannelInfo1->GetZ());

        hitpos1[0] = pHit1->GetX();
        hitpos1[1] = pHit1->GetY();
        hitpos1[2] = pHit1->GetZ();

        /*TGeoNode* cNode1=*/gGeoManager->GetCurrentNode();
        gGeoManager->MasterToLocal(hitpos1, hitpos1_local);

        fhXY0D4best->Fill(hitpos1_local[0], hitpos1_local[1]);
        fhXX04D4best->Fill(hitpos1_local[0], hitpos2_local[0]);
        fhYY04D4best->Fill(hitpos1_local[1], hitpos2_local[1]);

        fhDTX0D4best->Fill(hitpos1_local[0], dToD);
        fhDTY0D4best->Fill(hitpos1_local[1], dToD);
        fhDXX0D4best->Fill(hitpos1_local[0], xPos1 - xPos2);
        fhDXY0D4best->Fill(hitpos1_local[1], xPos1 - xPos2);
        fhDYX0D4best->Fill(hitpos1_local[0], yPos1 - yPos2);
        fhDYY0D4best->Fill(hitpos1_local[1], yPos1 - yPos2);

        fhDXDY04D4best->Fill(xPos1 - xPos2, yPos1 - yPos2);
        fhDXDT04D4best->Fill(xPos1 - xPos2, dToD);
        fhDYDT04D4best->Fill(yPos1 - yPos2, dToD);
        fhDistDT04D4best->Fill(dDist, dToD);
        fhTexpDT04D4best->Fill(dDTexp - dTMin, dToD);
        fhX0DT04D4best->Fill(hitpos1_local[0], dToD);
        fhY0DT04D4best->Fill(hitpos1_local[1], dToD);
        if (fTrbHeader != NULL)
          fhTISDT04D4best->Fill(fTrbHeader->GetTimeInSpill(), dToD);
        else
          fhTISDT04D4best->Fill((dTAv - fdStartSpillTime) / 1.E9, dToD);

        if (fbMonteCarloComparison) {
          Int_t iDutHitInd = fTofHitsColl->IndexOf(pHit1);

          if (fbTracksInInputFile) {
            if (bGoodTrackSel) {
              if (bGoodSelDutMatch) {
                fhSelMatchPurity->Fill(kTRUE, fiNAccRefTracks);
              } else {
                fhSelMatchPurity->Fill(kFALSE, fiNAccRefTracks);
              }
            }
            // uncomment to account for impure selectors in the matching purity calculation
            /*
           else
           {
             fhSelMatchPurity->Fill(kFALSE, fiNAccRefTracks);
           }
*/

            fhNTracksPerSelMRefHit->Fill(
              dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iMRefHitInd))
                ->GetNofLinks());
            if (pHitSel2) {
              fhNTracksPerSelSel2Hit->Fill(
                dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iSel2HitInd))
                  ->GetNofLinks());
            }
            fhNTracksPerSelDutHit->Fill(
              dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iDutHitInd))
                ->GetNofLinks());
          }


          CbmMatch* tDutHitPointMatch =
            dynamic_cast<CbmMatch*>(fTofHitPointMatches->At(iDutHitInd));
          const CbmLink& tDutHitPointLink = tDutHitPointMatch->GetMatchedLink();
          CbmTofPoint* tDutHitMatchedPoint(NULL);

          Int_t iFileIndex  = tDutHitPointLink.GetFile();
          Int_t iEventIndex = tDutHitPointLink.GetEntry();
          Int_t iArrayIndex = tDutHitPointLink.GetIndex();

          if (-1 < iFileIndex && -1 < iEventIndex && -1 < iArrayIndex) {
            Double_t dMCEventStartTime(0.);

            if (fbPointsInInputFile) {
              tDutHitMatchedPoint =
                dynamic_cast<CbmTofPoint*>(fTofPointsTB->At(iArrayIndex));
            } else {
              tDutHitMatchedPoint = dynamic_cast<CbmTofPoint*>(
                fTofPoints->Get(iFileIndex, iEventIndex, iArrayIndex));

              dMCEventStartTime =
                fMCEventList->GetEventTime(iEventIndex, iFileIndex);
            }

            // The extrapolation of a hit position in the DUT plane from the
            // position of the reference hit in the MRef plane following the
            // intercept theorem (fEnableMatchPosScaling) is based on
            // two assumptions:
            // 1) The track which both DUT and MRef hits are supposed to be
            //    associated with is a 3D original straight line.
            // 2) The global X and Y coordinates of hits in both planes have
            //    identical signs.

            fhResX04HitExp->Fill(fiNAccRefTracks, xPos1 - xPos2 - fdDXMean);
            fhResX04ExpMC->Fill(fiNAccRefTracks,
                                xPos2 - tDutHitMatchedPoint->GetX() + fdDXMean);
            fhResX04HitMC->Fill(fiNAccRefTracks,
                                xPos1 - tDutHitMatchedPoint->GetX());

            fhResY04HitExp->Fill(fiNAccRefTracks, yPos1 - yPos2 - fdDYMean);
            fhResY04ExpMC->Fill(fiNAccRefTracks,
                                yPos2 - tDutHitMatchedPoint->GetY() + fdDYMean);
            fhResY04HitMC->Fill(fiNAccRefTracks,
                                yPos1 - tDutHitMatchedPoint->GetY());

            fhResT04HitExp->Fill(fiNAccRefTracks,
                                 dToD);  // center-adjustable with 'fdDTMean'
            Double_t dSign(1.);
            if (pHit1->GetZ() < pHit2->GetZ()) { dSign = -1.; }
            Double_t dDutMCPointDelay(0.);
            Double_t dMRefMCPointDelay(0.);
            if (fbDelayMCPoints) {
              dDutMCPointDelay =
                0.5
                * (fChannelInfo1->GetSizey() >= fChannelInfo1->GetSizex()
                     ? fChannelInfo1->GetSizey()
                     : fChannelInfo1->GetSizex())
                / fDigiBdfPar->GetSigVel(fiDut, fiDutSm, fiDutRpc);
              dMRefMCPointDelay =
                0.5
                * (fChannelInfo2->GetSizey() >= fChannelInfo2->GetSizex()
                     ? fChannelInfo2->GetSizey()
                     : fChannelInfo2->GetSizex())
                / fDigiBdfPar->GetSigVel(fiMrpcRef, fiMrpcRefSm, fiMrpcRefRpc);
            }
            fhResT04ExpMC->Fill(
              fiNAccRefTracks,
              tof2 + dSign * dDTexp - tDutHitMatchedPoint->GetTime()
                - dMCEventStartTime
                - dMRefMCPointDelay);  // center-adjustable with 'fdTOffD4'
            fhResT04HitMC->Fill(fiNAccRefTracks,
                                tof1 - tDutHitMatchedPoint->GetTime()
                                  - dMCEventStartTime
                                  - dDutMCPointDelay);  // TODO: how to adjust?
          }
        }


        if (iNbMatchedHits > 1) {
          LOG(debug) << Form(
            " Matches>1: %d with first chi2s = %12.1f, %12.1f, %12.1f, %12.1f",
            iNbMatchedHits,
            Chi2List[0],
            Chi2List[1],
            Chi2List[2],
            Chi2List[3]);

          for (Int_t iM = 0; iM < iNbMatchedHits; iM++) {
            LOG(debug) << Form(
              " Match: %d (%d) with ids = 0x%08x, 0x%08x - 0x%08x, 0x%08x",
              iM,
              iNbMatchedHits,
              pChi2Hit1[iM]->GetAddress(),
              pHit1->GetAddress(),
              pChi2Hit2[iM]->GetAddress(),
              pHit2->GetAddress());
            if (pChi2Hit1[iM] != pHit1 && pChi2Hit2[iM] != pHit2) {

              LOG(debug) << Form(" second best match D4 at %d (%d): chi2 %f ",
                                 iM,
                                 iNbMatchedHits,
                                 Chi2List[iM]);

              if (Chi2List[iM] > 1.E3) break;  // FIXME hardwired limit !

              pHit3 = pChi2Hit1[iM];
              pHit4 = pChi2Hit2[iM];

              /*Int_t iDetId3 = (pHit1->GetAddress() & DetMask);*/
              Int_t iChId3  = pHit1->GetAddress();
              fChannelInfo3 = fDigiPar->GetCell(iChId3);

              /*Int_t iDetId4 = (pHit4->GetAddress() & DetMask);*/
              Int_t iChId4  = pHit4->GetAddress();
              fChannelInfo4 = fDigiPar->GetCell(iChId4);

              // check for dependence in counter reference frame
              /*TGeoNode *fNode4= */  // prepare global->local trafo
              gGeoManager->FindNode(fChannelInfo4->GetX(),
                                    fChannelInfo4->GetY(),
                                    fChannelInfo4->GetZ());

              hitpos4[0] = pChi2Hit2[iM]->GetX();
              hitpos4[1] = pChi2Hit2[iM]->GetY();
              hitpos4[2] = pChi2Hit2[iM]->GetZ();

              /*	 cNode=*/gGeoManager
                ->GetCurrentNode();  // -> Comment to remove warning because set but never used
              gGeoManager->MasterToLocal(hitpos4, hitpos4_local);

              if (TMath::Abs(hitpos4_local[1])
                  > fdPosY4Sel * fChannelInfo4->GetSizey())
                continue;

              fhChi04D4sbest->Fill(Chi2List[iM]);
              if (fEnableMatchPosScaling)
                dzscal = zPos1 / pChi2Hit1[iM]->GetZ();
              Double_t xPos3 = dzscal * pChi2Hit1[iM]->GetX();
              Double_t yPos3 = dzscal * pChi2Hit1[iM]->GetY();
              Double_t tof3  = pChi2Hit1[iM]->GetTime();

              if (fEnableMatchPosScaling)
                dzscal = zPos1 / pChi2Hit2[iM]->GetZ();
              Double_t xPos4 = dzscal * pChi2Hit2[iM]->GetX();
              Double_t yPos4 = dzscal * pChi2Hit2[iM]->GetY();
              Double_t tof4  = pChi2Hit2[iM]->GetTime();

              Double_t dDist34 =
                TMath::Sqrt(TMath::Power(pHit3->GetX() - pHit4->GetX(), 2)
                            + TMath::Power(pHit3->GetY() - pHit4->GetY(), 2)
                            + TMath::Power(pHit3->GetZ() - pHit4->GetZ(), 2));

              CbmMatch* digiMatch3 =
                (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit3));
              fhDigiMul0D4sbest->Fill(digiMatch3->GetNofLinks() / 2.);
              digiMatch4 =
                (CbmMatch*) fTofDigiMatchColl->At(fTofHitsColl->IndexOf(pHit4));
              fhDigiMul4D4sbest->Fill(digiMatch4->GetNofLinks() / 2.);

              fhCluMul04D4sbest->Fill(dMul0, dMul4);

              Double_t dTofD44  = fdTOffD4 + pHit4->GetTime() - dTDia;
              Double_t dInvVel4 = dTofD44 / pHitRef->GetR();  // in ns/cm
              Double_t dDTexp4  = dDist34 * dInvVel4;
              Double_t dTcor4   = 0.;
              if (fhDTD4DT04D4Off != NULL)
                dTcor4 = (Double_t) fhDTD4DT04D4Off->GetBinContent(
                  fhDTD4DT04D4Off->FindBin(dTDia - pHit4->GetTime()));
              if (fhDTX4D4Off != NULL)
                dTcor4 += (Double_t) fhDTX4D4Off->GetBinContent(
                  fhDTX4D4Off->FindBin(hitpos4_local[0]));
              if (fhDTY4D4Off != NULL)
                dTcor4 += (Double_t) fhDTY4D4Off->GetBinContent(
                  fhDTY4D4Off->FindBin(hitpos4_local[1]));
              if (fhDTTexpD4Off != NULL)
                dTcor4 += (Double_t) fhDTTexpD4Off->GetBinContent(
                  fhDTTexpD4Off->FindBin(dDTexp4));

              Double_t dToD34 = (tof3 - tof4 - dTcor4 - fdDTMean);

              fhTofD4sbest->Fill(dTofD44);
              if (dInvVel4 > 0.) fhVelD4sbest->Fill(1000. / dInvVel4);

              fhChiDT04D4sbest->Fill(Chi2List[iM], dToD34);

              fhDTD4DT04D4sbest->Fill(dTDia - pHit4->GetTime(), dToD34);
              fhDTMul4D4sbest->Fill(dMul4, dToD34);

              fhDTX4D4sbest->Fill(hitpos4_local[0], dToD34);
              fhDTY4D4sbest->Fill(hitpos4_local[1], dToD34);
              fhDXX4D4sbest->Fill(hitpos4_local[0], xPos3 - xPos4);
              fhDXY4D4sbest->Fill(hitpos4_local[1], xPos3 - xPos4);
              fhDYX4D4sbest->Fill(hitpos4_local[0], yPos3 - yPos4);
              fhDYY4D4sbest->Fill(hitpos4_local[1], yPos3 - yPos4);

              fhCluSize4DT04D4sbest->Fill(digiMatch4->GetNofLinks() / 2.,
                                          dToD34);

              dTot4 = 0.;
              if (fDigiMan->IsPresent(ECbmModuleId::kTof))
                for (Int_t iLink = 0; iLink < digiMatch4->GetNofLinks();
                     iLink++) {  // loop over digis
                  CbmLink L0              = digiMatch4->GetLink(iLink);
                  Int_t iDigInd0          = L0.GetIndex();
                  const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
                  dTot4 += pDig0->GetTot();
                  LOG(debug)
                    << Form(" dTot4 of hit 0x%08x: digind %d add %f -> sum %f",
                            iDetId2,
                            iDigInd0,
                            pDig0->GetTot(),
                            dTot4);
                }

              dTot4 /=
                digiMatch4->GetNofLinks();  // average time over threshold
              fhTot4DT04D4sbest->Fill(TMath::Log(dTot4), dToD34);

              fhCluSize0DT04D4sbest->Fill(digiMatch3->GetNofLinks() / 2.,
                                          dToD34);

              Double_t dTot3 = 0.;
              if (fDigiMan->IsPresent(ECbmModuleId::kTof))
                for (Int_t iLink = 0; iLink < digiMatch3->GetNofLinks();
                     iLink++) {  // loop over digis
                  CbmLink L0              = digiMatch3->GetLink(iLink);
                  Int_t iDigInd0          = L0.GetIndex();
                  const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
                  dTot3 += pDig0->GetTot();
                  LOG(debug)
                    << Form(" dTot of hit 0x%08x: digind %d add %f -> sum %f",
                            iDetId1,
                            iDigInd0,
                            pDig0->GetTot(),
                            dTot3);
                }
              dTot3 /=
                digiMatch3->GetNofLinks();  // average time over threshold
              fhTot0DT04D4sbest->Fill(TMath::Log(dTot3), dToD34);

              fhDTMul0D4sbest->Fill(dMul0, dToD34);

              // check for dependence in counter reference frame
              /*TGeoNode *fNode3=*/  // prepare global->local trafo
              gGeoManager->FindNode(fChannelInfo3->GetX(),
                                    fChannelInfo3->GetY(),
                                    fChannelInfo3->GetZ());

              hitpos3[0] = pChi2Hit1[iM]->GetX();
              hitpos3[1] = pChi2Hit1[iM]->GetY();
              hitpos3[2] = pChi2Hit1[iM]->GetZ();

              /*TGeoNode* cNode3=*/gGeoManager->GetCurrentNode();
              gGeoManager->MasterToLocal(hitpos3, hitpos3_local);

              fhDTX0D4sbest->Fill(hitpos3_local[0], dToD34);
              fhDTY0D4sbest->Fill(hitpos3_local[1], tof4 - tof4 - dTcor4);
              fhDXX0D4sbest->Fill(hitpos3_local[0], xPos3 - xPos4);
              fhDXY0D4sbest->Fill(hitpos3_local[1], xPos3 - xPos4);
              fhDYX0D4sbest->Fill(hitpos3_local[0], yPos3 - yPos4);
              fhDYY0D4sbest->Fill(hitpos3_local[1], yPos3 - yPos4);

              fhDXDY04D4sbest->Fill(xPos3 - xPos4, yPos3 - yPos4);
              fhDXDT04D4sbest->Fill(xPos3 - xPos4, dToD34);
              fhDYDT04D4sbest->Fill(yPos3 - yPos4, dToD34);
              fhDistDT04D4sbest->Fill(dDist34, dToD34);
              fhTexpDT04D4sbest->Fill(dDTexp4, dToD34);
              fhX0DT04D4sbest->Fill(hitpos3_local[0], dToD34);
              fhY0DT04D4sbest->Fill(hitpos3_local[1], dToD34);

              fhDT04DX0_2->Fill(hitpos1_local[0] - hitpos3_local[0], dToD34);
              fhDT04DY0_2->Fill(hitpos1_local[1] - hitpos3_local[1], dToD34);
              fhDT04DT0_2->Fill(tof1 - tof3, dToD34);

              fhDT04DX4_2->Fill(hitpos2_local[0] - hitpos4_local[0], dToD34);
              fhDT04DY4_2->Fill(hitpos2_local[1] - hitpos4_local[1], dToD34);
              fhDT04DT4_2->Fill(tof2 - tof4, dToD34);

              fhDT04DX0_1->Fill(hitpos1_local[0] - hitpos3_local[0], dToD);
              fhDT04DY0_1->Fill(hitpos1_local[1] - hitpos3_local[1], dToD);
              fhDT04DT0_1->Fill(tof1 - tof3, dToD);

              fhDT04DX4_1->Fill(hitpos2_local[0] - hitpos4_local[0], dToD);
              fhDT04DY4_1->Fill(hitpos2_local[1] - hitpos4_local[1], dToD);
              fhDT04DT4_1->Fill(tof2 - tof4, dToD);

              break;
            }
          }
        }

        fhSelTypeAccNNChiSq->Fill(0., 3. * Chi2List[iM0]);
        fhSelTypeAccNNResidualT->Fill(0., dToD);
        fhSelTypeAccNNResidualX->Fill(0., xPos1 - xPos2 - fdDXMean);
        fhSelTypeAccNNResidualY->Fill(0., yPos1 - yPos2 - fdDYMean);

        fhSelTypeNNChiSq->Fill(0., 3. * Chi2List[iM0]);
        fhSelTypeNNResidualT->Fill(0., dToD);
        fhSelTypeNNResidualX->Fill(0., xPos1 - xPos2 - fdDXMean);
        fhSelTypeNNResidualY->Fill(0., yPos1 - yPos2 - fdDYMean);

        fhSelHitTupleResidualTTIS->Fill((dTAv - fdStartSpillTime) / 1.E9, dToD);
        fhSelHitTupleDutCluSizeTIS->Fill((dTAv - fdStartSpillTime) / 1.E9,
                                         dCluSize0);

      }  // fdChi2Lim end
      else {
        fhSelMatchEfficiency->Fill(kFALSE, fiNAccRefTracks);
        fhSelHitTupleMatchEfficiencyTIS->Fill(kFALSE,
                                              (dTAv - fdStartSpillTime) / 1.E9);

        fhSelTypeNNChiSq->Fill(0., 3. * Chi2List[iM0]);
        fhSelTypeNNResidualT->Fill(0., dToD);
        fhSelTypeNNResidualX->Fill(0., xPos1 - xPos2 - fdDXMean);
        fhSelTypeNNResidualY->Fill(0., yPos1 - yPos2 - fdDYMean);
      }


      Int_t iSelHitTupleDutHitMatchMul(0);
      Int_t iSelHitTupleDutHitMatchAccMul(0);

      for (Int_t iMatch = 0; iMatch < iNbMatchedHits; iMatch++) {
        if (pHitRef == pChi2Hit2[iMatch]) {
          iSelHitTupleDutHitMatchMul++;

          if (fdChi2Lim > Chi2List[iMatch]) { iSelHitTupleDutHitMatchAccMul++; }
        }
      }

      if (iSelHitTupleDutHitMatchMul) {
        fhSelHitTupleDutHitMatchMul->Fill(iSelHitTupleDutHitMatchMul);
      }

      if (iSelHitTupleDutHitMatchAccMul) {
        fhSelHitTupleDutHitMatchAccMul->Fill(iSelHitTupleDutHitMatchAccMul);
      }

    }  // end of if(iNbMatchedHits>0)
    else {
      fhSelMatchEfficiency->Fill(kFALSE, fiNAccRefTracks);
      fhSelHitTupleMatchEfficiencyTIS->Fill(kFALSE,
                                            (dTAv - fdStartSpillTime) / 1.E9);
    }
  }  // BSel[0] condition end

  // Tracklet based analysis
  Double_t hitpos[3], hitpos_local[3];

  // DDDDDD for verifying storage concept
  /*
     if(fFindTracks != NULL && fdMemoryTime > 0.) {
       // everything else done -> update hit memory to latest hits
       for( Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++)
       {
	 pHit = (CbmTofHit*) fTofHitsColl->At( iHitInd );
	 if(NULL == pHit) continue;
	 Int_t iDetId = (pHit->GetAddress() & DetMask);
	 Int_t iDet=fFindTracks->fMapRpcIdParInd[iDetId];
	 fChannelInfo = fDigiPar->GetCell(pHit->GetAddress());
	 gGeoManager->FindNode(fChannelInfo->GetX(),fChannelInfo->GetY(),fChannelInfo->GetZ());
	 hitpos[0]=pHit->GetX();
	 hitpos[1]=pHit->GetY();
	 hitpos[2]=pHit->GetZ();
	 gGeoManager->MasterToLocal(hitpos, hitpos_local);
	 Int_t iBin = fhLHTime[iDet]->FindBin( hitpos_local[0], hitpos_local[1] );
         fhLHTime[iDet]->SetBinContent(iBin,pHit->GetTime()-2.);
	 LOG(debug) << Form("Store hit 0x%08x at x %6.3f, y %6.3f, bin %d, time %f in det Id 0x%08x, #%d from x %6.3f, y %6.3f",
			    pHit->GetAddress(),hitpos_local[0],hitpos_local[1], iBin,
	                    pHit->GetTime(), iDetId, iDet, hitpos[0],hitpos[1] )
		    ;
       }
     } //(fdMemoryTime > 0.) end
     */

  fhTIS_Nhit->Fill((dTAv - fdStartSpillTime) / 1.E9, (Double_t) iNbTofHits);
  if (NULL != fTofTrackColl && NULL != fFindTracks) {
    Bool_t bSelTrackletFound(kFALSE);
    Int_t iBestTrklFitIndex(-1);
    Double_t dBestTrklFitRedChiSq(1.E300);

    iNbTofTracks = fTofTrackColl->GetEntriesFast();
    fhTIS_Ntrk->Fill((dTAv - fdStartSpillTime) / 1.E9, (Double_t) iNbTofTracks);

    Int_t NStations = fFindTracks->GetNStations();
    LOG(debug) << Form("Tracklet analysis of %d tracklets from %d stations",
                       iNbTofTracks,
                       NStations);

    //     if(dMul4<=dM4Max && dMulD<=dMDMax &&dMulS2<=dM4Max)
    if (dMul0 <= dM0Max && dMul4 <= dM4Max && dMulD <= dMDMax
        && dMulS2 <= dM4Max)
      if (iNbTofTracks > 0) {  // Tracklet Analysis
                               // prepare Dut Hit List

        Int_t iChIdDut =
          CbmTofAddress::GetUniqueAddress(fiDutSm, fiDutRpc, 0, 0, fiDut);
        fChannelInfo = fDigiPar->GetCell(iChIdDut);
        /*TGeoNode *fNode=*/  // prepare global->local trafo
        gGeoManager->FindNode(
          fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
        /*TGeoNode* cNode=*/gGeoManager->GetCurrentNode();
        Double_t dDutzPos = fChannelInfo->GetZ();

        LOG(debug) << Form("Tracklet analysis of %d tracks and %d Dut Hits ",
                           iNbTofTracks,
                           (int) vDutHit.size());

        std::vector<std::map<Double_t, Int_t>>
          vTrkMap;  //contains the tracks for a given hit
        std::vector<std::map<Double_t, Int_t>>
          vHitMap;  //contains the hits for a given track

        vTrkMap.resize(vDutHit.size());
        vHitMap.resize(iNbTofTracks);

        std::multimap<Double_t, std::pair<Int_t, Int_t>>
          RedChiSqTrackletDutHitPair;
        std::map<Int_t, std::pair<Int_t, Double_t>>
          TrackletMatchedDutHitRedChiSq;

        for (Int_t iTrk = 0; iTrk < iNbTofTracks; iTrk++) {
          CbmTofTracklet* pTrk = (CbmTofTracklet*) fTofTrackColl->At(iTrk);
          if (NULL == pTrk) continue;

          if (dBestTrklFitRedChiSq > pTrk->GetChiSq()) {
            dBestTrklFitRedChiSq = pTrk->GetChiSq();
            iBestTrklFitIndex    = iTrk;
          }
        }

        for (Int_t iTrk = 0; iTrk < iNbTofTracks;
             iTrk++) {  // loop over all Tracklets
          CbmTofTracklet* pTrk = (CbmTofTracklet*) fTofTrackColl->At(iTrk);
          if (NULL == pTrk) continue;
          fhTrklNofHitsRate->Fill((pTrk->GetTime() - StartAnalysisTime) / 1.E9,
                                  pTrk->GetNofHits());  // Monitor tracklet size
          for (Int_t iTH = 0; iTH < pTrk->GetNofHits();
               iTH++) {  // Loop over Tracklet hits
            fhTrklDetHitRate->Fill(
              (pTrk->GetTime() - StartAnalysisTime) / 1.E9,  // Station hit rate
              fFindTracks->GetStationOfAddr(
                pTrk->GetTofHitPointer(iTH)->GetAddress() & DetMask));
          }

          fhTrklNofHitsRateInSpill->Fill(
            (pTrk->GetTime() - fdStartSpillTime) / 1.E9,
            pTrk->GetNofHits());  // Monitor tracklet size
          for (Int_t iTH = 0; iTH < pTrk->GetNofHits();
               iTH++) {  // Loop over Tracklet hits
            fhTrklDetHitRateInSpill->Fill(
              (pTrk->GetTime() - fdStartSpillTime) / 1.E9,  // Station hit rate
              fFindTracks->GetStationOfAddr(
                pTrk->GetTofHitPointer(iTH)->GetAddress() & DetMask));
          }

          /*
	 if (pTrk->GetNofHits() < NStations - 1) continue;

	 // Calculate positions and time in Dut plane
	 Double_t dXex=pTrk->GetFitX(dDutzPos);
	 Double_t dYex=pTrk->GetFitY(dDutzPos);
	 Double_t dR=TMath::Sqrt(dXex*dXex + dYex*dYex + dDutzPos*dDutzPos);
	 Double_t dTex=pTrk->GetFitT(dR);

	 for (UInt_t i=0; i<vDutHit.size();i++){ // loop over Dut Hits
	  Double_t dChi = TMath::Sqrt(TMath::Power(TMath::Abs(dTex-vDutHit[i]->GetTime())/fSIGT,2)
				     +TMath::Power(TMath::Abs(dXex-vDutHit[i]->GetX())/fSIGX,2)
	   			     +TMath::Power(TMath::Abs(dYex-vDutHit[i]->GetY())/fSIGY,2))/3;
	  LOG(debug1)<<Form(" Inspect track %d, hit %d Chi %6.2f,%6.2f, T  %6.2f,%6.2f ",
			    iTrk,i,dChi,fSIGLIM,dTex,vDutHit[i]->GetTime())
		    ;

	  if(dChi < fSIGLIM) {       // acceptable match
            if(vHitMap[iTrk].size()>0) {
	      Int_t iCnt=0;
	      for ( std::map<Double_t,Int_t>::iterator it=vHitMap[iTrk].begin(); it!=vHitMap[iTrk].end(); it++){
		iCnt++;
		LOG(debug)<<Form(" HitMap[%d]: cnt %d, check %d, %6.2f > %6.2f ?",iTrk,iCnt, it->second,it->first,dChi)
			  ;
                if(it->first > dChi) {
		  vHitMap[iTrk].insert(--it,std::pair<Double_t, Int_t>(dChi,i));
		  LOG(debug)<<Form(" HitMap[%d]: ins at %d:  %d, %6.2f ",iTrk,iCnt,it->second,it->first);
		  break;
		}
	      }
	    }
	    else{
	      vHitMap[iTrk].insert(std::pair<Double_t, Int_t>(dChi,i));
	      LOG(debug)<<Form(" HitMap[%d]: start %d, %6.2f ",iTrk,i,dChi);
	    }

            if(vTrkMap[i].size()>0) {
	      for ( std::map<Double_t,Int_t>::iterator it=vTrkMap[i].begin(); it!=vTrkMap[i].end(); it++){
               if(it->first > dChi) {
		  vTrkMap[i].insert(--it,std::pair<Double_t, Int_t>(dChi,iTrk));
		  break;
		}
	      }
	    }
	    else{
	      vTrkMap[i].insert(std::pair<Double_t, Int_t>(dChi,iTrk));
	    }

	  }  // end of Chi condition
	  if(vTrkMap[i].size()>0)
	  LOG(debug1)<<Form(" TrkMap[%d]: best %d, %6.4f ",i,vTrkMap[i].begin()->second,vTrkMap[i].begin()->first)
		    ;
	 }
	 if(vHitMap[iTrk].size()>0)
	 LOG(debug)<<Form(" HitMap[%d]: best %d, %6.4f ",iTrk,vHitMap[iTrk].begin()->second,vHitMap[iTrk].begin()->first)
		   ;
       } // tracklet loop end

       // inspect assignment results
       Int_t iCheck = 1;
       while(iCheck-- > 0)
       for(Int_t iHit=0; static_cast<UInt_t>(iHit)<vDutHit.size(); iHit++) {
	 if(vTrkMap[iHit].size()>0){
	   Int_t iTrk=vTrkMap[iHit].begin()->second;     // hit was assigned best to track iTrk
	   if(vHitMap[iTrk].begin()->second == iHit) {   // unique/consistent assignment
	     LOG(debug)<<Form(" Hit %d -> HitMap[%d]: uni %d, %6.4f ",
			      iHit,iTrk,vHitMap[iTrk].begin()->second,vHitMap[iTrk].begin()->first)
		       ;
	     // remove all other assignments of this hit and this track
	     for ( std::map<Double_t,Int_t>::iterator it=vTrkMap[iHit].begin()++; it != vTrkMap[iHit].end(); it++){
	       Int_t iTrk1=it->second;
	       if(iTrk != iTrk1)
	       for ( std::map<Double_t,Int_t>::iterator it1=vHitMap[iTrk1].begin()++; it1 != vHitMap[iTrk1].end(); it1++){
		 if(it1->second == iHit) {
		   vHitMap[iTrk1].erase(it1);
  	           LOG(debug1)<<Form("    Erase hit %d from  HitMap[%d]",iHit,iTrk1);
		   break;
		 }
	       }
	     }
	     for ( std::map<Double_t,Int_t>::iterator it=vHitMap[iTrk].begin()++; it != vHitMap[iTrk].end(); it++){
	       Int_t iHit1=it->second;
	       if(iHit != iHit1)
	       for ( std::map<Double_t,Int_t>::iterator it1=vTrkMap[iHit1].begin()++; it1 != vTrkMap[iHit1].end(); it1++){
		 if(it1->second == iTrk) {
		   vTrkMap[iHit1].erase(it1);
  	           LOG(debug1)<<Form("    Erase trk %d from  TrkMap[%d]",iHit1,iTrk);
		   break;
		 }
	       }
	     }
	   }else{      // mismatch, other track fits even better
	     LOG(debug)<<Form(" Hit %d -> HitMap[%d]: mis %d, %6.4f < %6.4f ",iHit,iTrk,
			      vHitMap[iTrk].begin()->second,vHitMap[iTrk].begin()->first,vTrkMap[iHit].begin()->first)
		       ;
	   }
	   LOG(debug)<<Form(" Hit %d -> TrkMap.size:  %d  ",iHit,(int)vTrkMap[iHit].size())
		     ;

	 }
       */

          if (fChi2LimFit < pTrk->GetChiSq()) { continue; }

          if (fbBestSelTrackletOnly) {
            if (iTrk != iBestTrklFitIndex) { continue; }
          }


          if (NStations == pTrk->GetNofHits()) {
            if (fbAttachDutHitToTracklet) {
              if (pTrk->ContainsAddr(fiDutAddr)) {
                LOG(fatal) << "DUT hit found already attached to a tracklet in "
                              "'AttachDutHitToTracklet' mode!";
              }

              Int_t iSelTrklDutHitMatchMul(0);
              Int_t iSelTrklDutHitMatchAccMul(0);

              Double_t dXex = pTrk->GetFitX(dDutzPos);
              Double_t dYex = pTrk->GetFitY(dDutzPos);
              Double_t dTex = pTrk->GetFitT(dDutzPos);

              for (auto const& iHit : DutHitSet) {
                CbmTofHit* tHit =
                  dynamic_cast<CbmTofHit*>(fTofHitsColl->At(iHit));

                Double_t dRedChiSq =
                  (TMath::Power(TMath::Abs(dTex - tHit->GetTime()) / GetSigT(1),
                                2)
                   + TMath::Power(TMath::Abs(dXex - tHit->GetX()) / GetSigX(1),
                                  2)
                   + TMath::Power(TMath::Abs(dYex - tHit->GetY()) / GetSigY(1),
                                  2))
                  / 3.;

                RedChiSqTrackletDutHitPair.emplace(dRedChiSq,
                                                   std::make_pair(iTrk, iHit));


                iSelTrklDutHitMatchMul++;

                if (dRedChiSq < fSIGLIM) { iSelTrklDutHitMatchAccMul++; }
              }


              if (iSelTrklDutHitMatchMul) {
                fhSelTrklDutHitMatchNNMul->Fill(iSelTrklDutHitMatchMul);
              }

              if (iSelTrklDutHitMatchAccMul) {
                fhSelTrklDutHitMatchAccNNMul->Fill(iSelTrklDutHitMatchAccMul);
              }
            }

            // A selector tracklet is considered to be found in a given reconstructed
            // event if the event satisfies the specified hit multiplicity criteria
            // and at least one reconstructed tracklet comprises hits from
            // 'CbmTofFindTracks::fNTofStations' counters.
            bSelTrackletFound = kTRUE;
          }
        }


        if (fbAttachDutHitToTracklet) {
          // DUT hits are (if at all) AMBIGUOUSLY matched with selector tracklets
          // (possibly multiple tracklets -> one hit) according to a minimum reduced
          // chi-square criterion. As the chi-square keys of the map looped over
          // below are sorted in ascending order, all other map elements containing
          // the corresponding tracklet index can be eliminated. The best match
          // (i.e. the lowest chi-square value) has been identified for the tracklet
          // by the current map element.
          for (auto itChi2Map = RedChiSqTrackletDutHitPair.cbegin();
               itChi2Map != RedChiSqTrackletDutHitPair.cend();) {
            Double_t dRedChiSq  = itChi2Map->first;
            Int_t iUsedTracklet = itChi2Map->second.first;
            Int_t iUsedHit      = itChi2Map->second.second;

            TrackletMatchedDutHitRedChiSq.emplace(
              iUsedTracklet, std::make_pair(iUsedHit, dRedChiSq));

            Bool_t bFoundNextUniqueMatch(kFALSE);

            for (auto itSubMap = ++itChi2Map;
                 itSubMap != RedChiSqTrackletDutHitPair.cend();) {
              Int_t iTracklet = itSubMap->second.first;
              // Int_t iHit = itSubMap->second.second;   (VF) not used

              // Uncomment the rear part of the following line of code to obtain
              // UNIQUE hit-to-tracklet matching.
              if (iUsedTracklet == iTracklet)  // || iUsedHit == iHit)
              {
                itSubMap = RedChiSqTrackletDutHitPair.erase(itSubMap);
              } else {
                if (!bFoundNextUniqueMatch) {
                  itChi2Map             = itSubMap;
                  bFoundNextUniqueMatch = kTRUE;
                }

                ++itSubMap;
              }
            }

            if (!bFoundNextUniqueMatch) {
              itChi2Map = RedChiSqTrackletDutHitPair.cend();
            }
          }
        }


        // fill tracklet histos
        Double_t dTiS =
          (dTAv - fdStartSpillTime) / 1.E9;  // Time in Spill of current event
        for (Int_t iTrk = 0; iTrk < iNbTofTracks;
             iTrk++) {  // loop over all Tracklets
          CbmTofTracklet* pTrk = (CbmTofTracklet*) fTofTrackColl->At(iTrk);
          if (NULL == pTrk) continue;
          if (pTrk->GetNofHits() < NStations - 1)
            continue;  // pick only full & full - 1 tracklets

          if (fChi2LimFit < pTrk->GetChiSq()) { continue; }

          if (fbBestSelTrackletOnly) {
            if (iTrk != iBestTrklFitIndex) { continue; }
          }

          pLHit            = NULL;
          Double_t dDelTLH = 11.;
          Double_t dTLH    = 0.;
          hitpos[0]        = pTrk->GetFitX(dDutzPos);
          hitpos[1]        = pTrk->GetFitY(dDutzPos);
          hitpos[2]        = dDutzPos;
          gGeoManager->FindNode(fChannelInfoDut->GetX(),
                                fChannelInfoDut->GetY(),
                                fChannelInfoDut->GetZ());
          gGeoManager->MasterToLocal(hitpos, hitpos_local);

          // select limited Dut area
          if (TMath::Abs(hitpos_local[0] - fdDutX) > fdDutDX
              || TMath::Abs(hitpos_local[1] - fdDutY) > fdDutDY)
            continue;

          pLHit      = NULL;
          Int_t iDet = fFindTracks->fMapRpcIdParInd[fiDutAddr];
          Double_t LHpos[3], LHpos_local[3];
          Int_t iBinA = 0;
          if (fhLHTime.size() > static_cast<size_t>(iDet)) {
            Int_t iBin =
              fhLHTime[iDet]->FindBin(hitpos_local[0], hitpos_local[1]);
            iBinA = iBin - 1;  // index in pointer array

            if (iBin <= 0
                || iBin > fhLHTime[iDet]->GetNbinsX()
                            * fhLHTime[iDet]->GetNbinsY()) {
              LOG(debug) << "Invalid bin number for reading fhLHTime, det "
                         << iDet << ": " << iBin << ", " << dTLH;
              dTLH = 0.;
            } else
              pLHit = fvLHit[iDet][iBinA];  // pointer to Last Hit object

            if (NULL != pLHit) {
              dTLH = fhLHTime[iDet]->GetBinContent(iBin);
              if (pLHit->GetTime() != dTLH)
                LOG(fatal) << " LHTime mismatch for Det " << iDet << ", Bin "
                           << iBin
                           << Form(": %f - %f = %f",
                                   pLHit->GetTime(),
                                   dTLH,
                                   pLHit->GetTime() - dTLH);
              if (0) {
                // Check whether neighbouring bins contain later hit
                Int_t iNbinsX     = (Int_t) fhLHTime[iDet]->GetNbinsX();
                CbmTofHit* pLLHit = pLHit;
                Double_t dTLLH    = dTLH;
                Int_t iRow        = iBin / iNbinsX + 1;
                Int_t iCol        = iBin % iNbinsX;

                if (iCol > 1) {  // check left side
                  if (fhLHTime[iDet]->GetBinContent(iBin - 1) > dTLLH) {
                    pLLHit = fvLHit[iDet][iBinA - 1];
                    dTLLH  = pLLHit->GetTime();
                  }
                  if (iRow > 1) {  // lower neighbbour
                    for (Int_t iBLL = iBin - iNbinsX - 1;
                         iBLL < iBin - iNbinsX + 1;
                         iBLL++) {
                      if (fhLHTime[iDet]->GetBinContent(iBLL) > dTLLH) {
                        pLLHit = fvLHit[iDet][iBLL - 1];
                        dTLLH  = pLLHit->GetTime();
                      }
                    }
                  }
                  if (iRow < iNbinsX - 1) {  //upper neighbour
                    for (Int_t iBLL = iBin + iNbinsX - 1;
                         iBLL < iBin + iNbinsX + 1;
                         iBLL++) {
                      if (fhLHTime[iDet]->GetBinContent(iBLL) > dTLLH) {
                        pLLHit = fvLHit[iDet][iBLL - 1];
                        dTLLH  = pLLHit->GetTime();
                      }
                    }
                  }
                }
                if (iCol < iNbinsX - 1) {  // check right side
                  if (fhLHTime[iDet]->GetBinContent(iBin + 1) > dTLLH) {
                    pLLHit = fvLHit[iDet][iBin];
                    dTLLH  = pLLHit->GetTime();
                  }
                  if (iRow > 1) {  // lower neighbbour
                    for (Int_t iBLL = iBin - iNbinsX + 1;
                         iBLL < iBin - iNbinsX + 2;
                         iBLL++) {
                      if (fhLHTime[iDet]->GetBinContent(iBLL) > dTLLH) {
                        pLLHit = fvLHit[iDet][iBLL - 1];
                        dTLLH  = pLLHit->GetTime();
                      }
                    }
                  }
                  if (iRow < iNbinsX - 1) {  //upper neighbour
                    for (Int_t iBLL = iBin + iNbinsX + 1;
                         iBLL < iBin + iNbinsX + 2;
                         iBLL++) {
                      if (fhLHTime[iDet]->GetBinContent(iBLL) > dTLLH) {
                        pLLHit = fvLHit[iDet][iBLL - 1];
                        dTLLH  = pLLHit->GetTime();
                      }
                    }
                  }
                }
                pLHit = pLLHit;
                dTLH  = dTLLH;
              }  // enable/disable neigbor inspection
              // TODO: Why was the fitted tracklet time at the origin (and not at the DUT plane) used here before?
              //	   if( dTLH > 0) dDelTLH=TMath::Log10( pTrk->GetTime()-dTLH);
              if (dTLH > 0)
                dDelTLH = TMath::Log10(pTrk->GetFitT(dDutzPos) - dTLH);
              /*
	       LOG(info)<< "Got LHTime NbinsX: "<<iNbinsX
	       << ", Bin "<<iBin<<", Row "<<iRow<<", Col "<<iCol
	       << ": TLH "<<dTLH<<", "<<dTLLH
	       ;
	     */
              LHpos[0] = pLHit->GetX();
              LHpos[1] = pLHit->GetY();
              LHpos[2] = pLHit->GetZ();
              gGeoManager->MasterToLocal(LHpos, LHpos_local);

              if (0)
                LOG(info) << Form(
                  " expect hit at %f in %d. spill got TLH = %f, DelT %f for "
                  "Det %d at x %6.2f, y %6.2f, bin %d from x %6.2f, y %6.2f",
                  pTrk->GetTime(),
                  iNspills,
                  dTLH,
                  dDelTLH,
                  iDet,
                  hitpos_local[0],
                  hitpos_local[1],
                  iBin,
                  LHpos_local[0],
                  LHpos_local[1]);
            }
            if (0)
              LOG(info) << Form(
                " Trkl with %d hits expect Dut hit at %f in %d. spill after "
                "TLH = %f, LogDelT %f for Det %d in Bin %d at x %6.2f, y %6.2f",
                pTrk->GetNofHits(),
                pTrk->GetTime(),
                iNspills,
                dTLH,
                dDelTLH,
                iDet,
                iBinA,
                hitpos_local[0],
                hitpos_local[1]);
          }

          Bool_t bGoodSelTracklet(kFALSE);
          Bool_t bGoodSelTrackletDutMatch(kFALSE);

          auto itDutHitMatch = TrackletMatchedDutHitRedChiSq.find(iTrk);

          Double_t dVel = 0.;
          Double_t dTt  = fTrackletTools->FitTt(pTrk, fiDutAddr);
          if (dTt > 0.) dVel = 1. / dTt;

          if (NStations == pTrk->GetNofHits()) {
            if (fbAttachDutHitToTracklet) {
              if (fbMonteCarloComparison) {
                if (fbTracksInInputFile) {
                  Int_t iMatchedRealTrackLink(-1);
                  std::set<Int_t> RealTrackLinkSet;

                  CbmTrackMatchNew* tTrackletAccTrackMatch =
                    dynamic_cast<CbmTrackMatchNew*>(
                      fTofTrackletAccTrackMatches->At(iTrk));
                  const CbmLink& tLink =
                    tTrackletAccTrackMatch->GetMatchedLink();
                  Double_t dSelTrackletMaxTrackLinkWeight = tLink.GetWeight();

                  for (Int_t iSelTrackletTrackLink = 0;
                       iSelTrackletTrackLink
                       < tTrackletAccTrackMatch->GetNofLinks();
                       iSelTrackletTrackLink++) {
                    const CbmLink& tSelTrackletTrackLink =
                      tTrackletAccTrackMatch->GetLink(iSelTrackletTrackLink);

                    if (dSelTrackletMaxTrackLinkWeight
                          == tSelTrackletTrackLink.GetWeight()
                        && -1 < tSelTrackletTrackLink.GetIndex()) {
                      iMatchedRealTrackLink = tSelTrackletTrackLink.GetIndex();
                      RealTrackLinkSet.emplace(
                        tSelTrackletTrackLink.GetIndex());
                    }
                  }

                  // For a pure sel tracklet, request a particle track match different
                  // from a beam or a dark point.
                  // The case that a noise link and a particle link have the same weight
                  // and the noise link is - by chance - the matched one is handled here:
                  // The noise link is ignored while assessing purity if and only if a
                  // particle link with the same weight exists.
                  if (-1 < iMatchedRealTrackLink) {
                    if (1.
                        == tTrackletAccTrackMatch->GetTrueOverAllHitsRatio()) {
                      fhSelTrklPurity->Fill(kTRUE, fiNAccRefTracks);
                      bGoodSelTracklet = kTRUE;
                    } else {
                      fhSelTrklPurity->Fill(kFALSE, fiNAccRefTracks);
                    }
                  } else {
                    fhSelTrklPurity->Fill(kFALSE, fiNAccRefTracks);
                  }

                  if (bGoodSelTracklet) {
                    TGeoNode* tNode(NULL);
                    TGeoMedium* tMedium(NULL);
                    TGeoMaterial* tMaterial(NULL);

                    const char* cMaterialName;

                    const char* cSelRefTrackPdgName;
                    const char* cSelRefTrackProcessName;

                    Bool_t bSelRefTrack(kFALSE);

                    for (auto const& iTrack : RealTrackLinkSet) {
                      CbmMCTrack* tTrack =
                        dynamic_cast<CbmMCTrack*>(fAccTracks->At(iTrack));
                      CbmMatch* tTrackPointMatch = dynamic_cast<CbmMatch*>(
                        fTofAccTrackPointMatches->At(iTrack));

                      tNode     = gGeoManager->FindNode(tTrack->GetStartX(),
                                                    tTrack->GetStartY(),
                                                    tTrack->GetStartZ());
                      tMedium   = tNode->GetMedium();
                      tMaterial = tMedium->GetMaterial();

                      GetMaterialName(tMaterial->GetName(), cMaterialName);

                      if (0 == std::strcmp("target", cMaterialName)
                          && fiMinMCRefTrackPoints
                               <= tTrackPointMatch->GetNofLinks()) {
                        bSelRefTrack = kTRUE;

                        GetPdgName(tTrack->GetPdgCode(), cSelRefTrackPdgName);
                        GetProcessName(
                          TMCProcessName[tTrack->GetGeantProcessId()],
                          cSelRefTrackProcessName);
                      }
                    }

                    if (bSelRefTrack) {
                      fhSelTrklRefTrackShare->Fill(kTRUE, fiNAccRefTracks);
                      fhSelTrklRefTrackProcSpec->Fill(
                        cSelRefTrackProcessName, cSelRefTrackPdgName, 1.);
                    } else {
                      fhSelTrklRefTrackShare->Fill(kFALSE, fiNAccRefTracks);
                    }


                    if (itDutHitMatch != TrackletMatchedDutHitRedChiSq.end()) {
                      Int_t iDutHitIndex = itDutHitMatch->second.first;

                      CbmMatch* tDutHitTrackMatch = dynamic_cast<CbmMatch*>(
                        fTofHitAccTrackMatches->At(iDutHitIndex));


                      for (Int_t iSelTrackletTrackLink = 0;
                           iSelTrackletTrackLink
                           < tTrackletAccTrackMatch->GetNofLinks();
                           iSelTrackletTrackLink++) {
                        const CbmLink& tSelTrackletTrackLink =
                          tTrackletAccTrackMatch->GetLink(
                            iSelTrackletTrackLink);

                        if (dSelTrackletMaxTrackLinkWeight
                            == tSelTrackletTrackLink.GetWeight()) {
                          for (Int_t iDutHitTrackLink = 0;
                               iDutHitTrackLink
                               < tDutHitTrackMatch->GetNofLinks();
                               iDutHitTrackLink++) {
                            const CbmLink& tDutHitTrackLink =
                              tDutHitTrackMatch->GetLink(iDutHitTrackLink);

                            if (tSelTrackletTrackLink == tDutHitTrackLink) {
                              bGoodSelTrackletDutMatch = kTRUE;
                              break;
                            }
                          }
                        }

                        if (bGoodSelTrackletDutMatch) { break; }
                      }

                      if (bGoodSelTrackletDutMatch) {
                        fhGoodSelTypeNNPureChiSq->Fill(
                          1, 3. * itDutHitMatch->second.second);
                      }

                      fhGoodSelTypeNNAllChiSq->Fill(
                        1, 3. * itDutHitMatch->second.second);
                    }
                  }
                }
              }
            }
          }


          // matched hit found
          if ((!fbAttachDutHitToTracklet && pTrk->GetNofHits() == NStations
               && pTrk->ContainsAddr(fiDutAddr))
              || (fbAttachDutHitToTracklet && pTrk->GetNofHits() == NStations
                  && itDutHitMatch != TrackletMatchedDutHitRedChiSq.end()
                  && itDutHitMatch->second.second < fSIGLIM)) {
            LOG(debug) << Form(
              " Event %d : process complete Trkl %d, HMul %d, iDet %d ",
              fEvents,
              iTrk,
              pTrk->GetNofHits(),
              fFindTracks->fMapRpcIdParInd[fiDutAddr]);

            fhSelTrklFitRedChiSq->Fill(pTrk->GetChiSq());

            Int_t iDutHitIndex(-1);
            if (fbAttachDutHitToTracklet) {
              iDutHitIndex = itDutHitMatch->second.first;
              //       pHit = vDutHit[vHitMap[iTrk].begin()->second];
              pHit = dynamic_cast<CbmTofHit*>(fTofHitsColl->At(iDutHitIndex));
            } else {
              iDutHitIndex =
                pTrk->GetTofHitIndex(pTrk->HitIndexOfAddr(fiDutAddr));
              pHit = pTrk->HitPointerOfAddr(fiDutAddr);
            }

            if (NULL == pHit)
              LOG(WARNING) << "Dut not found in full length track";

            //fChannelInfo = fDigiPar->GetCell(pHit->GetAddress());
            //gGeoManager->FindNode(fChannelInfo->GetX(),fChannelInfo->GetY(),fChannelInfo->GetZ());
            /*
	   gGeoManager->FindNode(fChannelInfoDut->GetX(),fChannelInfoDut->GetY(),fChannelInfoDut->GetZ());
	   hitpos[0]=pHit->GetX();
	   hitpos[1]=pHit->GetY();
	   hitpos[2]=pHit->GetZ();
	   gGeoManager->MasterToLocal(hitpos, hitpos_local);
	   */

            if (
              fR0LimFit
              > 0.)  // consider only tracks originating from interaction point
              if (pTrk->GetR0() > fR0LimFit) continue;

            Double_t dDX =
              pHit->GetX()
              - pTrk->GetFitX(
                pHit->GetZ());  // - tPar->GetX() - tPar->GetTx()*dDZ;
            Double_t dDXB = pTrk->GetXdif(
              fiDutAddr, pHit);  // ignore pHit in calc of reference
            Double_t dDY =
              pHit->GetY()
              - pTrk->GetFitY(pHit->GetZ());  // - tPar->GetTy()*dDZ;
            Double_t dDYB = pTrk->GetYdif(
              fiDutAddr, pHit);  // ignore pHit in calc of reference
            Double_t dDT =
              pHit->GetTime()
              - pTrk->GetFitT(
                pHit->GetZ());  // pTrk->GetTdif(fStationType[iSt]);
            Double_t dDTB = pTrk->GetTdif(
              fiDutAddr, pHit);  // ignore pHit in calc of reference

            fhDutPullX->Fill(dDX);
            fhDutPullXB->Fill(dDXB);
            fhDutPullY->Fill(dDY);
            fhDutPullYB->Fill(dDYB);
            fhDutPullT->Fill(dDT);
            fhDutPullTB->Fill(dDTB);
            fhDutChi_Found->Fill(pTrk->GetChiSq());

            Int_t iGet4   = -1;
            Double_t dTot = 0.;
            CbmMatch* digiMatch =
              (CbmMatch*) fTofDigiMatchColl->At(iDutHitIndex);
            if (NULL != digiMatch)
              if (fDigiMan->IsPresent(ECbmModuleId::kTof))
                for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks();
                     iLink++) {  // loop over digis
                  CbmLink L0              = digiMatch->GetLink(iLink);
                  Int_t iDigInd0          = L0.GetIndex();
                  const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
                  if (iGet4 == -1) {
                    iGet4 = Int_t(pDig0->GetChannel()) % 8 + 1;
                    dTot  = pDig0->GetTot();
                  } else {
                    if (iGet4 > 0) {
                      if (pDig0->GetTot() > dTot) {
                        iGet4 = Int_t(pDig0->GetChannel()) % 8 + 1;
                        dTot  = pDig0->GetTot();
                      }
                      //if ( Int_t(pDig0->GetChannel())%8 + 1 != iGet4 ) iGet4=0;
                    }
                  }
                }
              else
                LOG(error) << "no Tof Digis";
            else
              LOG(error) << "no Tof Digi Match";

            if (fbAttachDutHitToTracklet) {
              //	   fhDutChi_Match->Fill(vHitMap[iTrk].begin()->first);
              fhDutChi_Match->Fill(itDutHitMatch->second.second);
            }
            fhDutXY_Found->Fill(hitpos_local[0], hitpos_local[1]);
            fhDutDTLH_Found->Fill(dDelTLH);
            fhDutMul_Found->Fill(dMul4);
            fhDutTIS_Found->Fill(dTiS, iGet4);
            fhDutTIR_Found->Fill(dTIR, iGet4);
            fhDutVel_Found->Fill(dVel);

            fhDutXYDX->Fill(hitpos_local[0], hitpos_local[1], dDX);
            fhDutXYDY->Fill(hitpos_local[0], hitpos_local[1], dDY);
            fhDutXYDT->Fill(hitpos_local[0], hitpos_local[1], dDTB);

            fhSelTrklMatchEfficiency->Fill(kTRUE, fiNAccRefTracks);
            fhSelTrklMatchEfficiencyTIS->Fill(kTRUE,
                                              (dTAv - fdStartSpillTime) / 1.E9);

            if (fbMonteCarloComparison) {
              if (fbAttachDutHitToTracklet) {
                if (fbTracksInInputFile) {
                  if (bGoodSelTracklet) {
                    if (bGoodSelTrackletDutMatch) {
                      fhSelTrklMatchPurity->Fill(kTRUE, fiNAccRefTracks);
                    } else {
                      fhSelTrklMatchPurity->Fill(kFALSE, fiNAccRefTracks);
                    }
                  }
                  // uncomment to account for impure selectors in the matching purity calculation
                  /*
           else
           {
             fhSelTrklMatchPurity->Fill(kFALSE, fiNAccRefTracks);
           }
*/
                }
              }


              if (!fbAttachDutHitToTracklet) {
                CbmTofHit* tDutHitPointer =
                  dynamic_cast<CbmTofHit*>(fTofHitsColl->At(iDutHitIndex));
                Double_t dDutHitChi = pTrk->GetMatChi2(fiDutAddr);

                pTrk->RemoveTofHitIndex(-1, fiDutAddr, NULL, 0.);
                CbmTofTrackFinderNN::Line3Dfit(pTrk);
                pTrk->SetTime(pTrk->UpdateT0());
                pTrk->AddTofHitIndex(
                  iDutHitIndex, fiDutAddr, tDutHitPointer, dDutHitChi);
                pHit = pTrk->HitPointerOfAddr(fiDutAddr);
              }

              CbmMatch* tDutHitPointMatch =
                dynamic_cast<CbmMatch*>(fTofHitPointMatches->At(iDutHitIndex));
              const CbmLink& tDutHitPointLink =
                tDutHitPointMatch->GetMatchedLink();
              CbmTofPoint* tDutHitMatchedPoint(NULL);

              Int_t iFileIndex  = tDutHitPointLink.GetFile();
              Int_t iEventIndex = tDutHitPointLink.GetEntry();
              Int_t iArrayIndex = tDutHitPointLink.GetIndex();

              if (-1 < iFileIndex && -1 < iEventIndex && -1 < iArrayIndex) {
                Double_t dMCEventStartTime(0.);

                if (fbPointsInInputFile) {
                  tDutHitMatchedPoint =
                    dynamic_cast<CbmTofPoint*>(fTofPointsTB->At(iArrayIndex));
                } else {
                  tDutHitMatchedPoint = dynamic_cast<CbmTofPoint*>(
                    fTofPoints->Get(iFileIndex, iEventIndex, iArrayIndex));

                  dMCEventStartTime =
                    fMCEventList->GetEventTime(iEventIndex, iFileIndex);
                }

                //         fhDutResX_Hit_Trk->Fill(fiNAccRefTracks, pTrk->GetXdif(fiDutAddr, pHit));
                fhDutResX_Hit_Trk->Fill(
                  fiNAccRefTracks, pHit->GetX() - pTrk->GetFitX(pHit->GetZ()));
                fhDutResX_Trk_MC->Fill(fiNAccRefTracks,
                                       pTrk->GetFitX(pHit->GetZ())
                                         - tDutHitMatchedPoint->GetX());
                fhDutResX_Hit_MC->Fill(
                  fiNAccRefTracks, pHit->GetX() - tDutHitMatchedPoint->GetX());

                //         fhDutResY_Hit_Trk->Fill(fiNAccRefTracks, pTrk->GetYdif(fiDutAddr, pHit));
                fhDutResY_Hit_Trk->Fill(
                  fiNAccRefTracks, pHit->GetY() - pTrk->GetFitY(pHit->GetZ()));
                fhDutResY_Trk_MC->Fill(fiNAccRefTracks,
                                       pTrk->GetFitY(pHit->GetZ())
                                         - tDutHitMatchedPoint->GetY());
                fhDutResY_Hit_MC->Fill(
                  fiNAccRefTracks, pHit->GetY() - tDutHitMatchedPoint->GetY());

                //         fhDutResT_Hit_Trk->Fill(fiNAccRefTracks, pTrk->GetTdif(fiDutAddr, pHit));
                fhDutResT_Hit_Trk->Fill(fiNAccRefTracks,
                                        pHit->GetTime()
                                          - pTrk->GetFitT(pHit->GetZ()));
                fhDutResT_Trk_MC->Fill(fiNAccRefTracks,
                                       pTrk->GetFitT(pHit->GetZ())
                                         - tDutHitMatchedPoint->GetTime()
                                         - dMCEventStartTime);
                fhDutResT_Hit_MC->Fill(fiNAccRefTracks,
                                       pHit->GetTime()
                                         - tDutHitMatchedPoint->GetTime()
                                         - dMCEventStartTime);
              }

              if (!fbAttachDutHitToTracklet) {
                CbmTofTrackFinderNN::Line3Dfit(pTrk);
                pTrk->SetTime(pTrk->UpdateT0());
              }
            }

            if (fbAttachDutHitToTracklet) {
              fhSelTypeAccNNChiSq->Fill(1, 3. * itDutHitMatch->second.second);
              fhSelTypeAccNNResidualT->Fill(
                1, pHit->GetTime() - pTrk->GetFitT(pHit->GetZ()));
              fhSelTypeAccNNResidualX->Fill(
                1, pHit->GetX() - pTrk->GetFitX(pHit->GetZ()));
              fhSelTypeAccNNResidualY->Fill(
                1, pHit->GetY() - pTrk->GetFitY(pHit->GetZ()));

              fhSelTypeNNChiSq->Fill(1, 3. * itDutHitMatch->second.second);
              fhSelTypeNNResidualT->Fill(
                1, pHit->GetTime() - pTrk->GetFitT(pHit->GetZ()));
              fhSelTypeNNResidualX->Fill(
                1, pHit->GetX() - pTrk->GetFitX(pHit->GetZ()));
              fhSelTypeNNResidualY->Fill(
                1, pHit->GetY() - pTrk->GetFitY(pHit->GetZ()));

              CbmMatch* tDutHitDigiMatch =
                dynamic_cast<CbmMatch*>(fTofDigiMatchColl->At(iDutHitIndex));

              fhSelTrklResidualTTIS->Fill((dTAv - fdStartSpillTime) / 1.E9,
                                          pHit->GetTime()
                                            - pTrk->GetFitT(pHit->GetZ()));
              fhSelTrklDutCluSizeTIS->Fill((dTAv - fdStartSpillTime) / 1.E9,
                                           tDutHitDigiMatch->GetNofLinks() / 2);
            }

            if (NULL != pLHit) {

              CbmMatch* digiMatch0 =
                (CbmMatch*) fTofDigiMatchColl->At(iDutHitIndex);
              Double_t dTot0 = 0.;

              if (fDigiMan->IsPresent(ECbmModuleId::kTof)
                  && NULL != digiMatch0) {
                for (Int_t iLink = 0; iLink < digiMatch0->GetNofLinks();
                     iLink++) {  // loop over digis
                  CbmLink L0              = digiMatch0->GetLink(iLink);
                  Int_t iDigInd0          = L0.GetIndex();
                  const CbmTofDigi* pDig0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
                  dTot0 += pDig0->GetTot();
                }
                dTot0 /=
                  digiMatch0->GetNofLinks();  // average time over threshold
              }
              fhDutDTLH_CluSize->Fill(dDelTLH, digiMatch0->GetNofLinks() / 2.);
              fhDutDTLH_Tot->Fill(dDelTLH, dTot0);
              fhDutDTLH_Mul->Fill(dDelTLH, (Double_t) vDutHit.size());
              fhDutDTLH_TIS->Fill(dDelTLH, dTiS);
              Double_t dDDH = TMath::Sqrt(
                TMath::Power(hitpos_local[0] - LHpos_local[0], 2.)
                + TMath::Power(hitpos_local[1] - LHpos_local[1], 2.));
              fhDutDTLH_DDH_Found->Fill(dDelTLH, dDDH);

              hitpos[0] = pTrk->GetFitX(dDutzPos);
              hitpos[1] = pTrk->GetFitY(dDutzPos);
              hitpos[2] = dDutzPos;
              gGeoManager->MasterToLocal(hitpos, hitpos_local);
              Double_t dDD = TMath::Sqrt(
                TMath::Power(hitpos_local[0] - LHpos_local[0], 2.)
                + TMath::Power(hitpos_local[1] - LHpos_local[1], 2.));
              fhDutDTLH_DD_Found->Fill(dDelTLH, dDD);

              /*
	       if( pHit->GetTime()- dTLH > 10.) // debugging check
	       LOG(info)<< Form("invalid time distance for event %d, hit address 0x%08x",
	       fEvents, pHit->GetAddress())
	       ;
	     */
            }
          }

          if (fbAttachDutHitToTracklet && pTrk->GetNofHits() == NStations
              && itDutHitMatch != TrackletMatchedDutHitRedChiSq.end()
              && !(itDutHitMatch->second.second < fSIGLIM)) {
            pHit = dynamic_cast<CbmTofHit*>(
              fTofHitsColl->At(itDutHitMatch->second.first));

            fhSelTypeNNChiSq->Fill(1, 3. * itDutHitMatch->second.second);
            fhSelTypeNNResidualT->Fill(
              1, pHit->GetTime() - pTrk->GetFitT(pHit->GetZ()));
            fhSelTypeNNResidualX->Fill(
              1, pHit->GetX() - pTrk->GetFitX(pHit->GetZ()));
            fhSelTypeNNResidualY->Fill(
              1, pHit->GetY() - pTrk->GetFitY(pHit->GetZ()));

            fhSelTrklMatchEfficiency->Fill(kFALSE, fiNAccRefTracks);
            fhSelTrklMatchEfficiencyTIS->Fill(kFALSE,
                                              (dTAv - fdStartSpillTime) / 1.E9);
          }

          // no match for this track
          if ((!fbAttachDutHitToTracklet && pTrk->GetNofHits() == NStations - 1
               && !pTrk->ContainsAddr(fiDutAddr))
              || (fbAttachDutHitToTracklet && pTrk->GetNofHits() == NStations
                  && itDutHitMatch == TrackletMatchedDutHitRedChiSq.end())) {
            if (0)
              LOG(info) << Form(
                " Missed hit at %f, TLH = %f, LogDelT %f for Det %d at x "
                "%6.2f, y %6.2f, bin %d from x %6.2f, y %6.2f",
                pTrk->GetTime(),
                dTLH,
                dDelTLH,
                iDet,
                hitpos_local[0],
                hitpos_local[1],
                iBinA,
                hitpos[0],
                hitpos[1]);

            if (
              fR0LimFit
              > 0.)  // consider only tracks originating from interaction point
              if (pTrk->GetR0() > fR0LimFit) continue;

            fhSelTrklFitRedChiSq->Fill(pTrk->GetChiSq());
            fhDutChi_Missed->Fill(pTrk->GetChiSq());
            fhDutXY_Missed->Fill(hitpos_local[0], hitpos_local[1]);
            fhDutMul_Missed->Fill(dMul4);
            Int_t iGet4 = ((Int_t) hitpos_local[0] + 16) % 8 + 1;
            fhDutTIS_Missed->Fill(dTiS, iGet4);
            fhDutTIR_Missed->Fill(dTIR, iGet4);
            fhDutVel_Missed->Fill(dVel);
            fhDutDTLH_Missed->Fill(dDelTLH);
            fhDutDTLH_Missed_TIS->Fill(dDelTLH, dTiS);
            if (NULL != pLHit) {
              LHpos[0] = pLHit->GetX();
              LHpos[1] = pLHit->GetY();
              LHpos[2] = pLHit->GetZ();
              gGeoManager->MasterToLocal(LHpos, LHpos_local);
              Double_t dDD = TMath::Sqrt(
                TMath::Power(hitpos_local[0] - LHpos_local[0], 2.)
                + TMath::Power(hitpos_local[1] - LHpos_local[1], 2.));
              fhDutDTLH_DD_Missed->Fill(dDelTLH, dDD);
            }

            fhSelTrklMatchEfficiency->Fill(kFALSE, fiNAccRefTracks);
            fhSelTrklMatchEfficiencyTIS->Fill(kFALSE,
                                              (dTAv - fdStartSpillTime) / 1.E9);
          }

        }  // end of loop over all tracklets

        if (fbAttachDutHitToTracklet) {
          // Eliminate all tracklet - DUT hit matches which do not meet the
          // specified 'fSIGLIM' criterion. TODO: LEGACY code
          for (auto itChi2Map = TrackletMatchedDutHitRedChiSq.cbegin();
               itChi2Map != TrackletMatchedDutHitRedChiSq.cend();) {
            Double_t dRedChiSq = itChi2Map->second.second;

            if (dRedChiSq < fSIGLIM) {
              ++itChi2Map;
            } else {
              itChi2Map = TrackletMatchedDutHitRedChiSq.erase(itChi2Map);
            }
          }
        }

      }  //(iNbTofTracks>0) end


    /*  this is the proper place LHTime filling, move for testing purpose*/
    if (fFindTracks != NULL && fdMemoryTime > 0.) {
      // everything else done -> update hit memory to latest hits
      for (Int_t iHitInd = 0; iHitInd < iNbTofHits; iHitInd++) {
        pHit = (CbmTofHit*) fTofHitsColl->At(iHitInd);
        if (NULL == pHit) continue;
        Int_t iDetId = (pHit->GetAddress() & DetMask);
        if (iDetId != fiDutAddr) continue;  // store only Dut Hits

        Int_t iDet = fFindTracks->fMapRpcIdParInd[iDetId];
        //fChannelInfo = fDigiPar->GetCell(pHit->GetAddress());
        //gGeoManager->FindNode(fChannelInfo->GetX(),fChannelInfo->GetY(),fChannelInfo->GetZ());
        gGeoManager->FindNode(fChannelInfoDut->GetX(),
                              fChannelInfoDut->GetY(),
                              fChannelInfoDut->GetZ());
        hitpos[0] = pHit->GetX();
        hitpos[1] = pHit->GetY();
        hitpos[2] = pHit->GetZ();
        gGeoManager->MasterToLocal(hitpos, hitpos_local);
        if (fhLHTime.size() > 0) {
          if (static_cast<size_t>(iDet) >= fhLHTime.size()) {
            LOG(WARNING) << " LHdeb:  invalid iDet index " << iDet << " of "
                         << fhLHTime.size();
            fFindTracks->PrintMapRpcIdParInd();
            continue;
          }
          Int_t iBin =
            fhLHTime[iDet]->FindBin(hitpos_local[0], hitpos_local[1]);
          Int_t iBinA = iBin - 1;  // index in pointer array
          if (0)
            LOG(info) << Form(
              "Insert Dut hit for Det %d, Bin %d, x %f, y %f, poi ",
              iDet,
              iBinA,
              hitpos_local[0],
              hitpos_local[1])
                      << fhLHTime[iDet];

          if (iBin <= 0
              || iBin > fhLHTime[iDet]->GetNbinsX()
                          * fhLHTime[iDet]->GetNbinsY()) {
            LOG(debug) << "Invalid bin number for fhLHTime, det " << iDet
                       << ": " << iBin;
          } else {
            fhLHTime[iDet]->SetBinContent(iBin, pHit->GetTime());
            /*
	       LOG(info)<<"LHit check for "<<iDet<<", "<<iBin
	       ;
	     */
            if (NULL != fvLHit[iDet][iBinA]) {
              /*
		 LOG(info)<<"LHit "<<fvLHit[iDet][iBinA]->GetName()<<" exists for "<<iDet<<", "<<iBin
		      <<" at "<< fvLHit[iDet][iBinA] <<" -> delete "
		      ;
	       */
              fvLHit[iDet][iBinA]->Delete();
              //delete fvLHit[iDet][iBinA];             // delete outdated CbmTofHit
            }
            fvLHit[iDet][iBinA] = new CbmTofHit(*pHit);  // put onto heap

            if (0)
              LOG(info) << Form("Store hit 0x%08x at x %6.3f, y %6.3f, bin %d, "
                                "time %f in det Id 0x%08x, #%d",
                                fvLHit[iDet][iBinA]->GetAddress(),
                                hitpos_local[0],
                                hitpos_local[1],
                                iBinA,
                                pHit->GetTime(),
                                iDetId,
                                iDet);
          }
        }
      }
    }  //(fdMemoryTime > 0.) end


    if (bSelTrackletFound) {
      fhSelTrklEfficiency->Fill(kTRUE, fiNAccRefTracks);
      fhSelTrklEfficiencyTIS->Fill(kTRUE, (dTAv - fdStartSpillTime) / 1.E9);
    } else {
      fhSelTrklEfficiency->Fill(kFALSE, fiNAccRefTracks);
      fhSelTrklEfficiencyTIS->Fill(kFALSE, (dTAv - fdStartSpillTime) / 1.E9);
    }

  }  // (NULL!=fTofTrackColl && NULL != fFindTracks) end


  std::map<std::tuple<Int_t, Int_t, Int_t>, std::map<Int_t, Int_t>>
    CounterNCellHits;

  for (Int_t iHit = 0; iHit < fTofHitsColl->GetEntriesFast(); iHit++) {
    CbmTofHit* tHit     = dynamic_cast<CbmTofHit*>(fTofHitsColl->At(iHit));
    Int_t iHitAddress   = tHit->GetAddress();
    Int_t iModuleType   = CbmTofAddress::GetSmType(iHitAddress);
    Int_t iModuleIndex  = CbmTofAddress::GetSmId(iHitAddress);
    Int_t iCounterIndex = CbmTofAddress::GetRpcId(iHitAddress);
    Int_t iCellIndex    = CbmTofAddress::GetChannelId(iHitAddress);

    CounterNCellHits[std::make_tuple(iModuleType, iModuleIndex, iCounterIndex)]
                    [iCellIndex]++;

    if (fbMonteCarloComparison) {
      if (fbTracksInInputFile) {
        CbmMatch* tHitTrackMatch =
          dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iHit));

        if (fiMrpcRefAddr == (iHitAddress & DetMask)) {
          fhNTracksPerMRefHit->Fill(tHitTrackMatch->GetNofLinks());
        } else if (fiMrpcSel2Addr == (iHitAddress & DetMask)) {
          fhNTracksPerSel2Hit->Fill(tHitTrackMatch->GetNofLinks());
        } else if (fiDutAddr == (iHitAddress & DetMask)) {
          fhNTracksPerDutHit->Fill(tHitTrackMatch->GetNofLinks());
        }
      }
    }
  }


  // MC track selector analysis
  if (fbMonteCarloComparison) {
    if (fbTracksInInputFile) {
      //       if(fiMaxMCRefTracks >= fiNAccRefTracks)
      if (dMul0 <= dM0Max && dMul4 <= dM4Max && dMulD <= dMDMax
          && dMulS2 <= dM4Max) {
        std::vector<Int_t> RefTrackSet;

        TGeoNode* tNode(NULL);
        TGeoMedium* tMedium(NULL);
        TGeoMaterial* tMaterial(NULL);

        const char* cMaterialName;

        std::multimap<Double_t, std::pair<Int_t, Int_t>>
          RedChiSqTrackDutHitPair;
        std::map<Int_t, std::pair<Int_t, Double_t>> TrackMatchedDutHitRedChiSq;
        Int_t iSelMCTrack(-1);
        Int_t iSelMCTrackDutHitMatchMul(0);
        Int_t iSelMCTrackDutHitMatchAccMul(0);

        for (Int_t iTrack = 0; iTrack < fAccTracks->GetEntriesFast();
             iTrack++) {
          CbmMCTrack* tAccTrack =
            dynamic_cast<CbmMCTrack*>(fAccTracks->At(iTrack));
          CbmMatch* tAccTrackPointMatch =
            dynamic_cast<CbmMatch*>(fTofAccTrackPointMatches->At(iTrack));

          tNode     = gGeoManager->FindNode(tAccTrack->GetStartX(),
                                        tAccTrack->GetStartY(),
                                        tAccTrack->GetStartZ());
          tMedium   = tNode->GetMedium();
          tMaterial = tMedium->GetMaterial();

          GetMaterialName(tMaterial->GetName(), cMaterialName);

          if (0 == std::strcmp("target", cMaterialName)
              && fiMinMCRefTrackPoints <= tAccTrackPointMatch->GetNofLinks()) {
            // Scan for a MC track intersection with the DUT counter plane
            for (Int_t iPoint = 0; iPoint < tAccTrackPointMatch->GetNofLinks();
                 iPoint++) {
              const CbmLink& tLink = tAccTrackPointMatch->GetLink(iPoint);

              CbmTofPoint* tPoint(NULL);
              Int_t iFileIndex  = tLink.GetFile();
              Int_t iEventIndex = tLink.GetEntry();
              Int_t iArrayIndex = tLink.GetIndex();

              Double_t dMCEventStartTime(0.);

              if (fbPointsInInputFile) {
                tPoint =
                  dynamic_cast<CbmTofPoint*>(fTofPointsTB->At(iArrayIndex));
              } else {
                tPoint = dynamic_cast<CbmTofPoint*>(
                  fTofPoints->Get(iFileIndex, iEventIndex, iArrayIndex));

                dMCEventStartTime =
                  fMCEventList->GetEventTime(iEventIndex, iFileIndex);
              }

              Int_t iModuleType =
                fGeoHandler->GetSMType(tPoint->GetDetectorID());
              Int_t iModuleIndex =
                fGeoHandler->GetSModule(tPoint->GetDetectorID());
              Int_t iCounterIndex =
                fGeoHandler->GetCounter(tPoint->GetDetectorID());

              if (fiDut == iModuleType && fiDutSm == iModuleIndex
                  && fiDutRpc == iCounterIndex) {
                for (auto const& iHit : DutHitSet) {
                  CbmTofHit* tHit =
                    dynamic_cast<CbmTofHit*>(fTofHitsColl->At(iHit));

                  Double_t dRedChiSq =
                    (TMath::Power(TMath::Abs(tPoint->GetTime()
                                             + dMCEventStartTime
                                             - tHit->GetTime())
                                    / GetSigT(2),
                                  2)
                     + TMath::Power(TMath::Abs(tPoint->GetX() - tHit->GetX())
                                      / GetSigX(2),
                                    2)
                     + TMath::Power(TMath::Abs(tPoint->GetY() - tHit->GetY())
                                      / GetSigY(2),
                                    2))
                    / 3.;

                  RedChiSqTrackDutHitPair.emplace(dRedChiSq,
                                                  std::make_pair(iTrack, iHit));
                }

                RefTrackSet.push_back(iTrack);

                break;
              }
            }
          }
        }


        // Randomly choose a selector MC track from the set of available reference
        // MC tracks in the event.
        if (RefTrackSet.size()) {
          iSelMCTrack = RefTrackSet.at(gRandom->Integer(RefTrackSet.size()));
        }


        for (auto const& Chi2MapElement : RedChiSqTrackDutHitPair) {
          Double_t dRedChiSq = Chi2MapElement.first;
          Int_t iTrackIndex  = Chi2MapElement.second.first;

          if (iTrackIndex == iSelMCTrack) {
            iSelMCTrackDutHitMatchMul++;

            if (dRedChiSq < fdMCSIGLIM) { iSelMCTrackDutHitMatchAccMul++; }
          }
        }

        if (iSelMCTrackDutHitMatchMul) {
          fhSelMCTrackDutHitMatchNNMul->Fill(iSelMCTrackDutHitMatchMul);
        }

        if (iSelMCTrackDutHitMatchAccMul) {
          fhSelMCTrackDutHitMatchAccNNMul->Fill(iSelMCTrackDutHitMatchAccMul);
        }


        // DUT hits are (if at all) AMBIGUOUSLY matched with selector MC tracks
        // (possibly multiple tracks -> one hit) according to a minimum reduced
        // chi-square criterion. As the chi-square keys of the map looped over
        // below are sorted in ascending order, all other map elements containing
        // the corresponding MC track index can be eliminated. The best match
        // (i.e. the lowest chi-square value) has been identified for the track
        // by the current map element.
        for (auto itChi2Map = RedChiSqTrackDutHitPair.cbegin();
             itChi2Map != RedChiSqTrackDutHitPair.cend();) {
          Double_t dRedChiSq = itChi2Map->first;
          Int_t iUsedTrack   = itChi2Map->second.first;
          Int_t iUsedHit     = itChi2Map->second.second;

          TrackMatchedDutHitRedChiSq.emplace(
            iUsedTrack, std::make_pair(iUsedHit, dRedChiSq));

          Bool_t bFoundNextUniqueMatch(kFALSE);

          for (auto itSubMap = ++itChi2Map;
               itSubMap != RedChiSqTrackDutHitPair.cend();) {
            Int_t iTrack = itSubMap->second.first;
            //Int_t iHit = itSubMap->second.second;  (VF) not used

            // Uncomment the rear part of the following line of code to obtain
            // UNIQUE hit-to-track matching.
            if (iUsedTrack == iTrack)  // || iUsedHit == iHit)
            {
              itSubMap = RedChiSqTrackDutHitPair.erase(itSubMap);
            } else {
              if (!bFoundNextUniqueMatch) {
                itChi2Map             = itSubMap;
                bFoundNextUniqueMatch = kTRUE;
              }

              ++itSubMap;
            }
          }

          if (!bFoundNextUniqueMatch) {
            itChi2Map = RedChiSqTrackDutHitPair.cend();
          }
        }


        if (-1 < iSelMCTrack) {
          auto itDutHitMatch = TrackMatchedDutHitRedChiSq.find(iSelMCTrack);

          if (itDutHitMatch != TrackMatchedDutHitRedChiSq.end()) {
            Double_t dRedChiSq = itDutHitMatch->second.second;
            Int_t iDutHit      = itDutHitMatch->second.first;

            // CbmMCTrack* tTrack = dynamic_cast<CbmMCTrack*>(fAccTracks->At(iSelMCTrack));   (VF) not used
            CbmMatch* tTrackPointMatch = dynamic_cast<CbmMatch*>(
              fTofAccTrackPointMatches->At(iSelMCTrack));
            CbmTofHit* tHit =
              dynamic_cast<CbmTofHit*>(fTofHitsColl->At(iDutHit));
            CbmTofPoint* tPoint(NULL);

            Double_t dMCEventStartTime(0.);

            // Scan for the MC track intersection with the DUT counter plane
            for (Int_t iPoint = 0; iPoint < tTrackPointMatch->GetNofLinks();
                 iPoint++) {
              const CbmLink& tLink = tTrackPointMatch->GetLink(iPoint);

              Int_t iFileIndex  = tLink.GetFile();
              Int_t iEventIndex = tLink.GetEntry();
              Int_t iArrayIndex = tLink.GetIndex();

              if (fbPointsInInputFile) {
                tPoint =
                  dynamic_cast<CbmTofPoint*>(fTofPointsTB->At(iArrayIndex));
              } else {
                tPoint = dynamic_cast<CbmTofPoint*>(
                  fTofPoints->Get(iFileIndex, iEventIndex, iArrayIndex));

                dMCEventStartTime =
                  fMCEventList->GetEventTime(iEventIndex, iFileIndex);
              }

              Int_t iModuleType =
                fGeoHandler->GetSMType(tPoint->GetDetectorID());
              Int_t iModuleIndex =
                fGeoHandler->GetSModule(tPoint->GetDetectorID());
              Int_t iCounterIndex =
                fGeoHandler->GetCounter(tPoint->GetDetectorID());

              if (fiDut == iModuleType && fiDutSm == iModuleIndex
                  && fiDutRpc == iCounterIndex) {
                break;
              }
            }


            Bool_t bGoodSelDutMatch(kFALSE);
            CbmMatch* tDutHitTrackMatch =
              dynamic_cast<CbmMatch*>(fTofHitAccTrackMatches->At(iDutHit));

            for (Int_t iDutHitTrackLink = 0;
                 iDutHitTrackLink < tDutHitTrackMatch->GetNofLinks();
                 iDutHitTrackLink++) {
              const CbmLink& tDutHitTrackLink =
                tDutHitTrackMatch->GetLink(iDutHitTrackLink);

              if (tDutHitTrackLink.GetIndex() == iSelMCTrack) {
                bGoodSelDutMatch = kTRUE;
                break;
              }
            }

            if (bGoodSelDutMatch) {
              fhGoodSelTypeNNPureChiSq->Fill(2, 3. * dRedChiSq);
            }

            fhGoodSelTypeNNAllChiSq->Fill(2, 3. * dRedChiSq);


            if (dRedChiSq < fdMCSIGLIM) {
              fhSelTypeAccNNChiSq->Fill(2, 3. * dRedChiSq);
              fhSelTypeAccNNResidualT->Fill(
                2, tHit->GetTime() - tPoint->GetTime() - dMCEventStartTime);
              fhSelTypeAccNNResidualX->Fill(2, tHit->GetX() - tPoint->GetX());
              fhSelTypeAccNNResidualY->Fill(2, tHit->GetY() - tPoint->GetY());

              fhSelTypeNNChiSq->Fill(2, 3. * dRedChiSq);
              fhSelTypeNNResidualT->Fill(
                2, tHit->GetTime() - tPoint->GetTime() - dMCEventStartTime);
              fhSelTypeNNResidualX->Fill(2, tHit->GetX() - tPoint->GetX());
              fhSelTypeNNResidualY->Fill(2, tHit->GetY() - tPoint->GetY());

              fhSelMCTrackMatchEfficiency->Fill(kTRUE, fiNAccRefTracks);
              fhSelMCTrackMatchEfficiencyTIS->Fill(
                kTRUE, (dTAv - fdStartSpillTime) / 1.E9);

              if (bGoodSelDutMatch) {
                fhSelMCTrackMatchPurity->Fill(kTRUE, fiNAccRefTracks);
              } else {
                fhSelMCTrackMatchPurity->Fill(kFALSE, fiNAccRefTracks);
              }

              CbmMatch* tDutHitDigiMatch =
                dynamic_cast<CbmMatch*>(fTofDigiMatchColl->At(iDutHit));

              fhSelMCTrackResidualTTIS->Fill((dTAv - fdStartSpillTime) / 1.E9,
                                             tHit->GetTime() - tPoint->GetTime()
                                               - dMCEventStartTime);
              fhSelMCTrackDutCluSizeTIS->Fill((dTAv - fdStartSpillTime) / 1.E9,
                                              tDutHitDigiMatch->GetNofLinks()
                                                / 2);
            } else {
              fhSelTypeNNChiSq->Fill(2, 3. * dRedChiSq);
              fhSelTypeNNResidualT->Fill(
                2, tHit->GetTime() - tPoint->GetTime() - dMCEventStartTime);
              fhSelTypeNNResidualX->Fill(2, tHit->GetX() - tPoint->GetX());
              fhSelTypeNNResidualY->Fill(2, tHit->GetY() - tPoint->GetY());

              fhSelMCTrackMatchEfficiency->Fill(kFALSE, fiNAccRefTracks);
              fhSelMCTrackMatchEfficiencyTIS->Fill(
                kFALSE, (dTAv - fdStartSpillTime) / 1.E9);
            }
          } else {
            fhSelMCTrackMatchEfficiency->Fill(kFALSE, fiNAccRefTracks);
            fhSelMCTrackMatchEfficiencyTIS->Fill(
              kFALSE, (dTAv - fdStartSpillTime) / 1.E9);
          }


          // Eliminate all MC track - DUT hit matches which do not meet the
          // specified 'fdMCSIGLIM' criterion. TODO: LEGACY code
          for (auto itChi2Map = TrackMatchedDutHitRedChiSq.cbegin();
               itChi2Map != TrackMatchedDutHitRedChiSq.cend();) {
            Double_t dRedChiSq = itChi2Map->second.second;

            if (dRedChiSq < fdMCSIGLIM) {
              ++itChi2Map;
            } else {
              itChi2Map = TrackMatchedDutHitRedChiSq.erase(itChi2Map);
            }
          }


          fhSelMCTrackEfficiency->Fill(kTRUE, fiNAccRefTracks);
          fhSelMCTrackEfficiencyTIS->Fill(kTRUE,
                                          (dTAv - fdStartSpillTime) / 1.E9);
        } else {
          fhSelMCTrackEfficiency->Fill(kFALSE, fiNAccRefTracks);
          fhSelMCTrackEfficiencyTIS->Fill(kFALSE,
                                          (dTAv - fdStartSpillTime) / 1.E9);
        }

      } else {
        fhSelMCTrackEfficiency->Fill(kFALSE, fiNAccRefTracks);
        fhSelMCTrackEfficiencyTIS->Fill(kFALSE,
                                        (dTAv - fdStartSpillTime) / 1.E9);
      }
    }
  }


  if (fbMonteCarloComparison) {
    // Retrieve the original MC event header which provides detailed information
    // about the collision parameters in case no event mixing occurred during
    // event reconstruction.
    FairMCEventHeader* tMCEventHeader(NULL);
    Double_t dMCEventStartTime(0.);

    if (1 == fMCEventList->GetNofEvents()) {
      Int_t iFileID  = fMCEventList->GetFileIdByIndex(0);
      Int_t iEventID = fMCEventList->GetEventIdByIndex(0);

      dMCEventStartTime = fMCEventList->GetEventTimeByIndex(0);
      tMCEventHeader    = dynamic_cast<FairMCEventHeader*>(
        fMCEventHeader->Get(iFileID, iEventID));

      fhAccRefTrackMulCentrality->Fill(tMCEventHeader->GetB(), fiNAccRefTracks);
    }

    fhNMergedMCEvents->Fill(fMCEventList->GetNofEvents());

    if (tMCEventHeader && fFindTracks && fFindTracks->InspectEvent()
        && fFindTracks->GetVertexT()) {
      // collision vertex reconstruction QA
      fhPVResTAll->Fill(fiNAccRefTracks,
                        fFindTracks->GetVertexT() - tMCEventHeader->GetT()
                          - dMCEventStartTime);
      fhPVResXAll->Fill(fiNAccRefTracks,
                        fFindTracks->GetVertexX() - tMCEventHeader->GetX());
      fhPVResYAll->Fill(fiNAccRefTracks,
                        fFindTracks->GetVertexY() - tMCEventHeader->GetY());
    }


    if (fbTracksInInputFile) {
      TGeoNode* tNode(NULL);
      TGeoMedium* tMedium(NULL);
      TGeoMaterial* tMaterial(NULL);

      const char* cPdgName;
      const char* cMaterialName;
      const char* cProcessName;

      Double_t dLocalTrackStart[3]  = {0., 0., 0.};
      Double_t dGlobalTrackStart[3] = {0., 0., 0.};
      Double_t dLocalPoint[3]       = {0., 0., 0.};
      Double_t dGlobalPoint[3]      = {0., 0., 0.};

      std::map<std::tuple<Int_t, Int_t, Int_t>, Int_t> CounterNAccTracks;
      std::map<std::tuple<Int_t, Int_t, Int_t>, Int_t> CounterNAccRefTracks;
      std::map<std::tuple<Int_t, Int_t, Int_t>, Int_t> CounterNAccRndmTracks;
      std::map<std::tuple<Int_t, Int_t, Int_t>, Int_t> CounterNAccDomTracks;
      std::map<std::tuple<Int_t, Int_t, Int_t>, Int_t> CounterNRecRefTracks;
      std::map<std::tuple<Int_t, Int_t, Int_t>, Int_t> CounterNPureRefTracks;

      std::map<std::tuple<Int_t, Int_t, Int_t>, std::map<Int_t, Int_t>>
        CounterNCellAccRefTracks;


      Double_t dRefPVRecoT(0.);
      Double_t dRefPVRecoX(0.);
      Double_t dRefPVRecoY(0.);
      Int_t iRefPVRecoN(0);

      for (Int_t iTrack = 0; iTrack < fAccTracks->GetEntriesFast(); iTrack++) {
        Bool_t bIsAccRefTrack(kFALSE);

        CbmMCTrack* tAccTrack =
          dynamic_cast<CbmMCTrack*>(fAccTracks->At(iTrack));
        CbmMatch* tAccTrackPointMatch =
          dynamic_cast<CbmMatch*>(fTofAccTrackPointMatches->At(iTrack));
        CbmMatch* tAccTrackTrackletMatch(NULL);
        if (fTofTrackColl) {
          tAccTrackTrackletMatch =
            dynamic_cast<CbmMatch*>(fTofAccTrackTrackletMatches->At(iTrack));
        }

        dGlobalTrackStart[0] = tAccTrack->GetStartX();
        dGlobalTrackStart[1] = tAccTrack->GetStartY();
        dGlobalTrackStart[2] = tAccTrack->GetStartZ();

        tNode = gGeoManager->FindNode(
          dGlobalTrackStart[0], dGlobalTrackStart[1], dGlobalTrackStart[2]);
        tMedium   = tNode->GetMedium();
        tMaterial = tMedium->GetMaterial();

        GetPdgName(tAccTrack->GetPdgCode(), cPdgName);
        GetMaterialName(tMaterial->GetName(), cMaterialName);
        GetProcessName(TMCProcessName[tAccTrack->GetGeantProcessId()],
                       cProcessName);

        fhAccTrackPointMul->Fill(tAccTrackPointMatch->GetNofLinks());
        // XXX: tAccTrack->GetNPoints(ECbmModuleId::kTof)

        if (0 == std::strcmp("target", cMaterialName)
            && fiMinMCRefTrackPoints <= tAccTrackPointMatch->GetNofLinks()) {
          bIsAccRefTrack = kTRUE;

          fhAccRefTrackShare->Fill(kTRUE, fiNAccRefTracks);

          if (tAccTrack->GetMass()) {
            fhAccRefTrackAcceptance->Fill(tAccTrack->GetRapidity(),
                                          tAccTrack->GetPt()
                                            / tAccTrack->GetMass());
          }

          fhAccRefTracksProcSpec->Fill(cProcessName, cPdgName, 1.);

          if (fTofTrackColl) {
            if (0 < tAccTrackTrackletMatch->GetNofLinks()) {
              fhRecRefTrackEfficiency->Fill(kTRUE, fiNAccRefTracks);

              if (tAccTrack->GetMass()) {
                fhAccRefTrackAcceptanceEfficiency->Fill(
                  kTRUE,
                  tAccTrack->GetRapidity(),
                  tAccTrack->GetPt() / tAccTrack->GetMass());
              }

              CbmTofTracklet* tTracklet =
                dynamic_cast<CbmTofTracklet*>(fTofTrackColl->At(
                  (tAccTrackTrackletMatch->GetMatchedLink()).GetIndex()));

              // The properties of 'CbmTofTrackletParam' (fX, fY, fTx, fTy) describe
              // the position and orientation of the tracklet at fZ = 0.
              // The MC reference track, however, can - according to its definition -
              // originate from the entire target volume, i.e. have an origin in Z
              // different from 0. As the track production vertex is just a random
              // point on its straight-line trajectory, we are free to choose any
              // other location along the track/tracklet for a point-to-point
              // comparison between the two 3D straight lines. Since methods
              // for tracklet parameter extrapolation along Z are available in
              // 'CbmTofTracklet', values are calculated at 'CbmMCTrack::fStartZ'
              // for tracklet-track comparison.
              fhAccRefTrackResT->Fill(fiNAccRefTracks,
                                      tTracklet->GetFitT(tAccTrack->GetStartZ())
                                        - tAccTrack->GetStartT());
              fhAccRefTrackResX->Fill(fiNAccRefTracks,
                                      tTracklet->GetFitX(tAccTrack->GetStartZ())
                                        - tAccTrack->GetStartX());
              fhAccRefTrackResY->Fill(fiNAccRefTracks,
                                      tTracklet->GetFitY(tAccTrack->GetStartZ())
                                        - tAccTrack->GetStartY());
              fhAccRefTrackResTx->Fill(fiNAccRefTracks,
                                       tTracklet->GetTrackTx()
                                         - tAccTrack->GetPx()
                                             / tAccTrack->GetPz());
              fhAccRefTrackResTy->Fill(fiNAccRefTracks,
                                       tTracklet->GetTrackTy()
                                         - tAccTrack->GetPy()
                                             / tAccTrack->GetPz());
              fhAccRefTrackResV->Fill(fiNAccRefTracks,
                                      1. / tTracklet->GetTt()
                                        - tAccTrack->GetP()
                                            / tAccTrack->GetEnergy()
                                            * TMath::Ccgs() * 1.e-9);

              // To compare the number of hits assigned to the tracklet with the
              // number of points created by the MC reference track the number
              // of diamond hits which have no point equivalent in the MC track
              // needs to be subtracted from the number of tracklet hits.
              Int_t iNTrackletHits = tTracklet->GetNofHits();

              for (Int_t iHit = 0; iHit < tTracklet->GetNofHits(); iHit++) {
                if (5
                    == CbmTofAddress::GetSmType(
                      (tTracklet->GetTofHitPointer(iHit))->GetAddress())) {
                  iNTrackletHits--;
                }
              }

              fhAccRefTrackResN->Fill(fiNAccRefTracks,
                                      iNTrackletHits
                                        - tAccTrackPointMatch->GetNofLinks());

              Int_t iRefPVRecoW = tTracklet->GetNofHits();

              if (fFindTracks) {
                if (iRefPVRecoW >= fFindTracks->GetMinNofHits()) {
                  dRefPVRecoT += iRefPVRecoW * tTracklet->GetFitT(0.);
                  dRefPVRecoX += iRefPVRecoW * tTracklet->GetFitX(0.);
                  dRefPVRecoY += iRefPVRecoW * tTracklet->GetFitY(0.);
                  iRefPVRecoN += iRefPVRecoW;
                }
              }
            } else {
              fhRecRefTrackEfficiency->Fill(kFALSE, fiNAccRefTracks);

              if (tAccTrack->GetMass()) {
                fhAccRefTrackAcceptanceEfficiency->Fill(
                  kFALSE,
                  tAccTrack->GetRapidity(),
                  tAccTrack->GetPt() / tAccTrack->GetMass());
              }
            }
          }

          fhAccRefTrackPointMul->Fill(tAccTrackPointMatch->GetNofLinks());
        } else {
          fhAccRefTrackShare->Fill(kFALSE, fiNAccRefTracks);

          if (fTofTrackColl) {
            if (0 < tAccTrackTrackletMatch->GetNofLinks()) {
              fhRecRndmTrackEfficiency->Fill(kTRUE, fiNAccRefTracks);
            } else {
              fhRecRndmTrackEfficiency->Fill(kFALSE, fiNAccRefTracks);
            }
          }

          fhAccRndmTrackPointMul->Fill(tAccTrackPointMatch->GetNofLinks());
        }

        // Having a maximum of 1 MC point per track per counter this point loop is
        // effectively a loop over counters which the MC track crossed.
        for (Int_t iPoint = 0; iPoint < tAccTrackPointMatch->GetNofLinks();
             iPoint++) {
          const CbmLink& tLink = tAccTrackPointMatch->GetLink(iPoint);

          CbmTofPoint* tPoint(NULL);
          Int_t iFileIndex  = tLink.GetFile();
          Int_t iEventIndex = tLink.GetEntry();
          Int_t iArrayIndex = tLink.GetIndex();

          if (fbPointsInInputFile) {
            tPoint = dynamic_cast<CbmTofPoint*>(fTofPointsTB->At(iArrayIndex));
          } else {
            tPoint = dynamic_cast<CbmTofPoint*>(
              fTofPoints->Get(iFileIndex, iEventIndex, iArrayIndex));
          }

          Int_t iModuleType  = fGeoHandler->GetSMType(tPoint->GetDetectorID());
          Int_t iModuleIndex = fGeoHandler->GetSModule(tPoint->GetDetectorID());
          Int_t iCounterIndex =
            fGeoHandler->GetCounter(tPoint->GetDetectorID());

          auto CounterID =
            std::make_tuple(iModuleType, iModuleIndex, iCounterIndex);

          CounterNAccTracks[CounterID]++;

          // A MC reference track passed through the current counter.
          if (bIsAccRefTrack) {
            CounterNAccRefTracks[CounterID]++;

            fChannelInfo = fDigiPar->GetCell(tPoint->GetDetectorID());
            gGeoManager->FindNode(
              fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
            dGlobalPoint[0] = tPoint->GetX();
            dGlobalPoint[1] = tPoint->GetY();
            dGlobalPoint[2] = tPoint->GetZ();
            gGeoManager->MasterToLocal(dGlobalPoint, dLocalPoint);

            fhCounterRefTrackLocalXY[CounterID]->Fill(dLocalPoint[0],
                                                      dLocalPoint[1]);

            Int_t iNCounterCells =
              fDigiBdfPar->GetNbChan(iModuleType, iCounterIndex);

            Int_t iCellIndex(-1);
            if (fChannelInfo->GetSizey() >= fChannelInfo->GetSizex()) {
              iCellIndex = static_cast<Int_t>(
                dLocalPoint[0] / fChannelInfo->GetSizex()
                + static_cast<Double_t>(iNCounterCells) / 2.);
            } else {
              iCellIndex = static_cast<Int_t>(
                dLocalPoint[1] / fChannelInfo->GetSizey()
                + static_cast<Double_t>(iNCounterCells) / 2.);
            }

            CounterNCellAccRefTracks[CounterID][iCellIndex]++;

            if (fTofTrackColl) {
              // A tracklet was constructed for the MC reference track.
              if (0 < tAccTrackTrackletMatch->GetNofLinks()) {
                Int_t iCounterAddress = CbmTofAddress::GetUniqueAddress(
                  iModuleIndex, iCounterIndex, 0, 0, iModuleType);

                CbmTofTracklet* tTracklet =
                  dynamic_cast<CbmTofTracklet*>(fTofTrackColl->At(
                    (tAccTrackTrackletMatch->GetMatchedLink()).GetIndex()));

                Int_t iHitIndex = tTracklet->HitIndexOfAddr(iCounterAddress);

                // A hit on the current counter was assigned to the tracklet.
                // As there is a point created by the MC reference track on this
                // counter, the counter is efficient with respect to the MC
                // reference track because it provided a hit to the matched
                // tracklet where it should provide one.
                if (-1 < iHitIndex) {
                  CounterNRecRefTracks[CounterID]++;

                  iHitIndex = tTracklet->GetTofHitIndex(iHitIndex);
                  CbmMatch* tHitTrackMatch = dynamic_cast<CbmMatch*>(
                    fTofHitAccTrackMatches->At(iHitIndex));

                  // Although the counter detected a hit that is attributed
                  // to the tracklet matching the MC reference track best it
                  // is not granted that this hit actually is the true one, i.e.
                  // contains a link to the MC reference track. If it contains
                  // a corresponding link, the counter "purely" detected the
                  // track.
                  for (Int_t iHitTrackLink = 0;
                       iHitTrackLink < tHitTrackMatch->GetNofLinks();
                       iHitTrackLink++) {
                    if (iTrack
                        == tHitTrackMatch->GetLink(iHitTrackLink).GetIndex()) {
                      CounterNPureRefTracks[CounterID]++;
                    }
                  }
                }
              }
            }
          }

          TGeoPhysicalNode* tModuleNode = fCounterModuleNodes.at(CounterID);

          tModuleNode->GetMatrix()->MasterToLocal(dGlobalTrackStart,
                                                  dLocalTrackStart);

          if (tModuleNode->GetVolume()->Contains(dLocalTrackStart)) {
            fhDomTracksProcSpec.at(CounterID)->Fill(cProcessName, cPdgName, 1.);
            fhDomTracksProcMat.at(CounterID)->Fill(
              cProcessName, cMaterialName, 1.);

            CounterNAccDomTracks[CounterID]++;
          } else {
            if (!bIsAccRefTrack) {
              fhRndmTracksProcSpec.at(CounterID)->Fill(
                cProcessName, cPdgName, 1.);
              fhRndmTracksProcMat.at(CounterID)->Fill(
                cProcessName, cMaterialName, 1.);

              CounterNAccRndmTracks[CounterID]++;
            }
          }
        }
      }

      if (tMCEventHeader && fFindTracks) {
        if (0 < iRefPVRecoN) {
          dRefPVRecoT /= iRefPVRecoN;
          dRefPVRecoX /= iRefPVRecoN;
          dRefPVRecoY /= iRefPVRecoN;

          fhPVResTRef->Fill(fiNAccRefTracks,
                            dRefPVRecoT - tMCEventHeader->GetT()
                              - dMCEventStartTime);
          fhPVResXRef->Fill(fiNAccRefTracks,
                            dRefPVRecoX - tMCEventHeader->GetX());
          fhPVResYRef->Fill(fiNAccRefTracks,
                            dRefPVRecoY - tMCEventHeader->GetY());
        }
      }


      if (fTofTrackColl) {
        std::set<CbmLink> tMatchedAccTracks;

        for (Int_t iTracklet = 0; iTracklet < fTofTrackColl->GetEntriesFast();
             iTracklet++) {
          CbmTrackMatchNew* tTrackletAccTrackMatch =
            dynamic_cast<CbmTrackMatchNew*>(
              fTofTrackletAccTrackMatches->At(iTracklet));
          const CbmLink& tLink = tTrackletAccTrackMatch->GetMatchedLink();

          // Ignore tracklets which have been matched to a beam or to a dark point
          // for which no MC track exists.
          // TODO: dedicated QA for such cases
          if (-1 < tLink.GetFile() && -1 < tLink.GetEntry()
              && -1 < tLink.GetIndex()) {
            CbmMCTrack* tAccTrack =
              dynamic_cast<CbmMCTrack*>(fAccTracks->At(tLink.GetIndex()));
            CbmMatch* tAccTrackPointMatch = dynamic_cast<CbmMatch*>(
              fTofAccTrackPointMatches->At(tLink.GetIndex()));

            dGlobalTrackStart[0] = tAccTrack->GetStartX();
            dGlobalTrackStart[1] = tAccTrack->GetStartY();
            dGlobalTrackStart[2] = tAccTrack->GetStartZ();

            tNode = gGeoManager->FindNode(
              dGlobalTrackStart[0], dGlobalTrackStart[1], dGlobalTrackStart[2]);
            tMedium   = tNode->GetMedium();
            tMaterial = tMedium->GetMaterial();

            GetMaterialName(tMaterial->GetName(), cMaterialName);

            // TODO: There could be another tracklet-to-track link with the same weight
            //       as the matched one that hypothetically - in contrast to the
            //       matched link - originated from the target. This case is
            //       not covered here (cf. the filling of the "SelRefTrackShare"
            //       efficiency histograms above).
            if (0 == std::strcmp("target", cMaterialName)
                && fiMinMCRefTrackPoints
                     <= tAccTrackPointMatch->GetNofLinks()) {
              if (fdGhostTrackHitQuota
                  > tTrackletAccTrackMatch->GetTrueOverAllHitsRatio()) {
                fhRecRefTrackGhostShare->Fill(kTRUE, fiNAccRefTracks);

                if (tAccTrack->GetMass()) {
                  fhAccRefTrackAcceptancePurity->Fill(kFALSE,
                                                      tAccTrack->GetRapidity(),
                                                      tAccTrack->GetPt()
                                                        / tAccTrack->GetMass());
                }
              } else {
                fhRecRefTrackGhostShare->Fill(kFALSE, fiNAccRefTracks);

                if (tAccTrack->GetMass()) {
                  fhAccRefTrackAcceptancePurity->Fill(kTRUE,
                                                      tAccTrack->GetRapidity(),
                                                      tAccTrack->GetPt()
                                                        / tAccTrack->GetMass());
                }
              }

              // Another tracklet has been matched to the MC track the current
              // tracklet has been matched to as well.
              if (kFALSE
                  == (tMatchedAccTracks.emplace(1.,
                                                tLink.GetIndex(),
                                                tLink.GetEntry(),
                                                tLink.GetFile()))
                       .second) {
                fhRecRefTrackCloneShare->Fill(kTRUE, fiNAccRefTracks);
              } else {
                fhRecRefTrackCloneShare->Fill(kFALSE, fiNAccRefTracks);
              }
            } else {
              if (fdGhostTrackHitQuota
                  > tTrackletAccTrackMatch->GetTrueOverAllHitsRatio()) {
                fhRecRndmTrackGhostShare->Fill(kTRUE, fiNAccRefTracks);
              } else {
                fhRecRndmTrackGhostShare->Fill(kFALSE, fiNAccRefTracks);
              }

              // Another tracklet has been matched to the MC track the current
              // tracklet has been matched to as well.
              if (kFALSE
                  == (tMatchedAccTracks.emplace(1.,
                                                tLink.GetIndex(),
                                                tLink.GetEntry(),
                                                tLink.GetFile()))
                       .second) {
                fhRecRndmTrackCloneShare->Fill(kTRUE, fiNAccRefTracks);
              } else {
                fhRecRndmTrackCloneShare->Fill(kFALSE, fiNAccRefTracks);
              }
            }
          }
        }
      }


      for (auto const& CounterModuleNode : fCounterModuleNodes) {
        auto const& CounterID = CounterModuleNode.first;
        Int_t iModuleType     = std::get<0>(CounterID);
        // Int_t iModuleIndex = std::get<1>(CounterID);   (VF) not used
        Int_t iCounterIndex = std::get<2>(CounterID);
        Int_t iNCounterCells =
          fDigiBdfPar->GetNbChan(iModuleType, iCounterIndex);

        fhCounterAccTrackMul.at(CounterID)->Fill(CounterNAccTracks[CounterID]);
        fhCounterAccRefTrackMul.at(CounterID)->Fill(
          CounterNAccRefTracks[CounterID]);
        fhCounterAccRndmTrackMul.at(CounterID)->Fill(
          CounterNAccRndmTracks[CounterID]);
        fhCounterAccDomTrackMul.at(CounterID)->Fill(
          CounterNAccDomTracks[CounterID]);

        if (fTofTrackColl) {
          fhCounterRecRefTrackEfficiencyPassed.at(CounterID)->Fill(
            CounterNAccRefTracks[CounterID], CounterNRecRefTracks[CounterID]);
          fhCounterRecRefTrackEfficiencyTotal.at(CounterID)->Fill(
            CounterNAccRefTracks[CounterID], CounterNAccRefTracks[CounterID]);
          fhCounterRecRefTrackPurityPassed.at(CounterID)->Fill(
            CounterNAccRefTracks[CounterID], CounterNPureRefTracks[CounterID]);
        }

        Int_t iCounterHitMul(0);
        Int_t iCounterRefTrackMul(0);

        for (Int_t iCell = 0; iCell < iNCounterCells; iCell++) {
          fhCounterRefTrackMulCell.at(CounterID)->Fill(
            iCell, CounterNCellAccRefTracks[CounterID][iCell]);
          iCounterRefTrackMul += CounterNCellAccRefTracks[CounterID][iCell];
          iCounterHitMul += CounterNCellHits[CounterID][iCell];
        }

        fhCounterRefTrackMulHitMul.at(CounterID)->Fill(iCounterRefTrackMul,
                                                       iCounterHitMul);
      }
    }
  }


  for (auto const& CounterModuleNode : fCounterModuleNodes) {
    auto const& CounterID = CounterModuleNode.first;
    Int_t iModuleType     = std::get<0>(CounterID);
    // Int_t iModuleIndex = std::get<1>(CounterID);   (VF) not used
    Int_t iCounterIndex  = std::get<2>(CounterID);
    Int_t iNCounterCells = fDigiBdfPar->GetNbChan(iModuleType, iCounterIndex);

    for (Int_t iCell = 0; iCell < iNCounterCells; iCell++) {
      fhCounterHitMulCell.at(CounterID)->Fill(
        iCell, CounterNCellHits[CounterID][iCell]);
    }
  }

  return kTRUE;
}  //FillHistos end
// ------------------------------------------------------------------

Bool_t CbmTofAnaTestbeam::WriteHistos() {

  LOG(info) << "Write ./tofAnaTestBeam.hst.root, mode = " << fiCorMode;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  // Write histogramms to the file
  TFile* fHist      = new TFile(fCalOutFileName, "RECREATE");
  fHist->cd();

  switch (fiCorMode) {
    case 0:
    case 1: {
      TProfile* htmp = fhDTD4DT04D4best->ProfileX();
      TH1D* htmp1D   = htmp->ProjectionX();

      //     htmp1D->Draw();

      if (fhDTD4DT04D4Off != NULL) {
        // fhDTD4DT04D4Off->Draw("same");
        // fhDTD4DT04D4Off->Write();
        //LOG(info)<<"Update hDTD4DT04D4best";
        Double_t nx = htmp1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmp1D->GetBinContent(ix) + fhDTD4DT04D4Off->GetBinContent(ix);
          // Double_t dVal=fhDTD4DT04D4Off->GetBinContent(ix);
          LOG(debug2) << "Update hDTD4DT04D4best " << ix << ": "
                      << htmp1D->GetBinContent(ix) << " + "
                      << fhDTD4DT04D4Off->GetBinContent(ix) << " -> " << dVal;
          htmp1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhDTD4DT04D4Off not found ";
      }
      //   fhDTD4DT04D4best->Write();
      htmp1D->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;
    case 2: {
      TProfile* htmpx = fhDTX4D4best->ProfileX();
      TH1D* htmpx1D   = htmpx->ProjectionX();
      if (fhDTX4D4Off != NULL) {
        Double_t nx = htmpx1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmpx1D->GetBinContent(ix) + fhDTX4D4Off->GetBinContent(ix);
          LOG(debug1) << "Update hDTX4D4best " << ix << ": "
                      << htmpx1D->GetBinContent(ix) << " + "
                      << fhDTX4D4Off->GetBinContent(ix) << " -> " << dVal;
          htmpx1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhDTX4D4Off not found ";
      }
      htmpx1D->Write();
      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    case 3: {
      TProfile* htmpx = fhDTY4D4best->ProfileX();
      TH1D* htmpx1D   = htmpx->ProjectionX();
      if (fhDTY4D4Off != NULL) {
        Double_t nx = htmpx1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmpx1D->GetBinContent(ix) + fhDTY4D4Off->GetBinContent(ix);
          LOG(debug1) << "Update hDTY4D4best " << ix << ": "
                      << htmpx1D->GetBinContent(ix) << " + "
                      << fhDTY4D4Off->GetBinContent(ix) << " -> " << dVal;
          htmpx1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhDTY4D4Off not found ";
      }
      htmpx1D->Write();
      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    case 4: {
      TProfile* htmpx = fhTexpDT04D4best->ProfileX();
      TH1D* htmpx1D   = htmpx->ProjectionX();
      if (fhDTTexpD4Off != NULL) {
        Double_t nx = htmpx1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmpx1D->GetBinContent(ix) + fhDTTexpD4Off->GetBinContent(ix);
          LOG(debug1) << "Update hDTTexpD4best " << ix << ": "
                      << htmpx1D->GetBinContent(ix) << " + "
                      << fhDTTexpD4Off->GetBinContent(ix) << " -> " << dVal;
          htmpx1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhDTTexpD4Off not found ";
      }
      htmpx1D->Write();
      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    case 5: {
      TProfile* htmpx = fhCluSize0DT04D4best->ProfileX();
      TH1D* htmpx1D   = htmpx->ProjectionX();
      if (fhCluSize0DT04D4Off != NULL) {
        Double_t nx = htmpx1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmpx1D->GetBinContent(ix) + fhCluSize0DT04D4Off->GetBinContent(ix);
          LOG(debug1) << "Update hCluSize0DT04D4best " << ix << ": "
                      << htmpx1D->GetBinContent(ix) << " + "
                      << fhCluSize0DT04D4Off->GetBinContent(ix) << " -> "
                      << dVal;
          htmpx1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhCluSize0DT04D4Off not found ";
      }
      htmpx1D->Write();
      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    case 6: {
      TProfile* htmpx = fhCluSize4DT04D4best->ProfileX();
      TH1D* htmpx1D   = htmpx->ProjectionX();
      if (fhCluSize4DT04D4Off != NULL) {
        Double_t nx = htmpx1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmpx1D->GetBinContent(ix) + fhCluSize4DT04D4Off->GetBinContent(ix);
          LOG(debug1) << "Update hCluSize4DT04D4best " << ix << ": "
                      << htmpx1D->GetBinContent(ix) << " + "
                      << fhCluSize4DT04D4Off->GetBinContent(ix) << " -> "
                      << dVal;
          htmpx1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhCluSize4DT04D4Off not found ";
      }
      htmpx1D->Write();
      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    case 7: {
      TProfile* htmpx = fhTot0DT04D4best->ProfileX();
      TH1D* htmpx1D   = htmpx->ProjectionX();
      if (fhTot0DT04D4Off != NULL) {
        Double_t nx = htmpx1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmpx1D->GetBinContent(ix) + fhTot0DT04D4Off->GetBinContent(ix);
          LOG(debug1) << "Update hTot0DT04D4best " << ix << ": "
                      << htmpx1D->GetBinContent(ix) << " + "
                      << fhTot0DT04D4Off->GetBinContent(ix) << " -> " << dVal;
          htmpx1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhTot0DT04D4Off not found ";
      }
      htmpx1D->Write();
      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    case 8: {
      TProfile* htmpx = fhTot4DT04D4best->ProfileX();
      TH1D* htmpx1D   = htmpx->ProjectionX();
      if (fhTot4DT04D4Off != NULL) {
        Double_t nx = htmpx1D->GetNbinsX();
        for (Int_t ix = 1; ix <= nx; ix++) {
          Double_t dVal =
            htmpx1D->GetBinContent(ix) + fhTot4DT04D4Off->GetBinContent(ix);
          LOG(debug1) << "Update hTot4DT04D4best " << ix << ": "
                      << htmpx1D->GetBinContent(ix) << " + "
                      << fhTot4DT04D4Off->GetBinContent(ix) << " -> " << dVal;
          htmpx1D->SetBinContent(ix, dVal);
        }
      } else {
        LOG(WARNING) << "Histo fhTot4DT04D4Off not found ";
      }
      htmpx1D->Write();
      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
    } break;

    case 9: {
      for (Int_t iSelType = 0; iSelType < 3; iSelType++) {
        TH1D* hResidualT =
          fhSelTypeNNResidualT->ProjectionY("_py", iSelType + 1, iSelType + 1);
        TH1D* hResidualX =
          fhSelTypeNNResidualX->ProjectionY("_py", iSelType + 1, iSelType + 1);
        TH1D* hResidualY =
          fhSelTypeNNResidualY->ProjectionY("_py", iSelType + 1, iSelType + 1);

        if (hResidualT->GetEntries() > 100.) {
          Double_t dRMS = TMath::Abs(hResidualT->GetRMS());
          /*
	         TFitResultPtr tFitResult = hResidualT->Fit("gaus", "QS");
	         dRMS = tFitResult->Parameter(2);
*/
          fhSelTypeNNResidualT_Width->SetBinContent(iSelType + 1, dRMS);
        }

        if (hResidualX->GetEntries() > 100.) {
          Double_t dRMS = TMath::Abs(hResidualX->GetRMS());
          /*
	         TFitResultPtr tFitResult = hResidualX->Fit("gaus", "QS");
	         dRMS = tFitResult->Parameter(2);
*/
          fhSelTypeNNResidualX_Width->SetBinContent(iSelType + 1, dRMS);
        }

        if (hResidualY->GetEntries() > 100.) {
          Double_t dRMS = TMath::Abs(hResidualY->GetRMS());
          /*
	         TFitResultPtr tFitResult = hResidualY->Fit("gaus", "QS");
	         dRMS = tFitResult->Parameter(2);
*/
          fhSelTypeNNResidualY_Width->SetBinContent(iSelType + 1, dRMS);
        }
      }


      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    case 10: {
      if (fhDXSel24->GetEntries() > 100.) {
        Double_t dRMS = TMath::Abs(fhDXSel24->GetRMS());

        fhSelHitTupleResidualXYT_Width->SetBinContent(1, dRMS);
      }

      if (fhDYSel24->GetEntries() > 100.) {
        Double_t dRMS = TMath::Abs(fhDYSel24->GetRMS());

        fhSelHitTupleResidualXYT_Width->SetBinContent(2, dRMS);
      }

      if (fhDTSel24->GetEntries() > 100.) {
        Double_t dRMS = TMath::Abs(fhDTSel24->GetRMS());

        fhSelHitTupleResidualXYT_Width->SetBinContent(3, dRMS);
      }


      if (fhDTD4DT04D4Off != NULL) fhDTD4DT04D4Off->Write();
      if (fhDTX4D4Off != NULL) fhDTX4D4Off->Write();
      if (fhDTY4D4Off != NULL) fhDTY4D4Off->Write();
      if (fhDTTexpD4Off != NULL) fhDTTexpD4Off->Write();
      if (fhCluSize0DT04D4Off != NULL) fhCluSize0DT04D4Off->Write();
      if (fhCluSize4DT04D4Off != NULL) fhCluSize4DT04D4Off->Write();
      if (fhTot0DT04D4Off != NULL) fhTot0DT04D4Off->Write();
      if (fhTot4DT04D4Off != NULL) fhTot4DT04D4Off->Write();
    } break;

    default:;
  }

  fhDistDT04D4best->Write();

  if (fhSelHitTupleResidualXYT_Width != NULL)
    fhSelHitTupleResidualXYT_Width->Write();
  if (fhSelTypeNNResidualT_Width != NULL) fhSelTypeNNResidualT_Width->Write();
  if (fhSelTypeNNResidualX_Width != NULL) fhSelTypeNNResidualX_Width->Write();
  if (fhSelTypeNNResidualY_Width != NULL) fhSelTypeNNResidualY_Width->Write();

  //   fHist->Write();
  if (0) {
    fhXX2->Write();
    fhYY2->Write();
    for (Int_t iDet = 0; iDet < 2; iDet++) {
      fhXX02[iDet]->Write();
      fhYY02[iDet]->Write();
    }
    fhXX04->Write();
    fhYY04->Write();
    fhXY04->Write();
    fhYX04->Write();
  }
  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  fHist->Close();

  return kTRUE;
}

Bool_t CbmTofAnaTestbeam::DeleteHistos() {
  // Test class performance

  // Mapping

  return kTRUE;
}


Double_t CbmTofAnaTestbeam::GetSigT(Int_t iSelType) {
  return (Double_t) fhSelTypeNNResidualT_Width->GetBinContent(iSelType + 1);
}


Double_t CbmTofAnaTestbeam::GetSigX(Int_t iSelType) {
  return (Double_t) fhSelTypeNNResidualX_Width->GetBinContent(iSelType + 1);
}


Double_t CbmTofAnaTestbeam::GetSigY(Int_t iSelType) {
  return (Double_t) fhSelTypeNNResidualY_Width->GetBinContent(iSelType + 1);
}


Double_t CbmTofAnaTestbeam::GetSHTSigX() {
  return (Double_t) fhSelHitTupleResidualXYT_Width->GetBinContent(1);
}


Double_t CbmTofAnaTestbeam::GetSHTSigY() {
  return (Double_t) fhSelHitTupleResidualXYT_Width->GetBinContent(2);
}


Double_t CbmTofAnaTestbeam::GetSHTSigT() {
  return (Double_t) fhSelHitTupleResidualXYT_Width->GetBinContent(3);
}


Bool_t CbmTofAnaTestbeam::FindModuleNodes() {
  Bool_t bFoundTofNode(kFALSE);
  TGeoNode* tTofNode(NULL);

  fCurrentNodePath = "/" + (TString) gGeoManager->GetTopNode()->GetName();

  TObjArray* tDaughterNodes = gGeoManager->GetTopNode()->GetNodes();
  for (Int_t iNode = 0; iNode < tDaughterNodes->GetEntriesFast(); iNode++) {
    TGeoNode* tDaughterNode =
      dynamic_cast<TGeoNode*>(tDaughterNodes->At(iNode));
    if (TString(tDaughterNode->GetName())
          .Contains("tof", TString::kIgnoreCase)) {
      bFoundTofNode = kTRUE;
      tTofNode      = tDaughterNode;
      fCurrentNodePath += "/" + (TString) tDaughterNode->GetName();
      break;
    }
  }

  if (!bFoundTofNode) {
    LOG(error) << "Could not retrieve 'tof' node from TGeoManager.";
    return kFALSE;
  }

  ExpandNode(tTofNode);

  return kTRUE;
}


void CbmTofAnaTestbeam::ExpandNode(TGeoNode* tMotherNode) {
  TObjArray* tDaughterNodes = tMotherNode->GetNodes();

  for (Int_t iNode = 0; iNode < tDaughterNodes->GetEntriesFast(); iNode++) {
    TGeoNode* tDaughterNode =
      dynamic_cast<TGeoNode*>(tDaughterNodes->At(iNode));
    fCurrentNodePath += "/" + (TString) tDaughterNode->GetName();

    // Extract the current module type and module index from the module node
    if (TString(tDaughterNode->GetName()).Contains("module")) {
      fiCurrentModuleType   = -1;
      fiCurrentModuleIndex  = -1;
      fiCurrentCounterIndex = -1;

      boost::regex rgx(".*_(\\d+)_.*");
      boost::cmatch match;
      if (boost::regex_search(tDaughterNode->GetName(), match, rgx)) {
        fiCurrentModuleType = boost::lexical_cast<Int_t>(match[1]);
      }

      fiCurrentModuleIndex = tDaughterNode->GetNumber();

      fCurrentModuleNodePath = fCurrentNodePath;
    }
    // Extract the current counter index from the counter node
    else if (TString(tDaughterNode->GetName()).Contains("counter")) {
      fiCurrentCounterIndex = tDaughterNode->GetNumber();

      fCounterModuleNodes[std::make_tuple(
        fiCurrentModuleType, fiCurrentModuleIndex, fiCurrentCounterIndex)] =
        new TGeoPhysicalNode(fCurrentModuleNodePath.Data());
    }

    // Expand nodes recursively
    if (0 < tDaughterNode->GetNdaughters()) { ExpandNode(tDaughterNode); }

    // Remove a node's name from the current node path upon completing a recursion step
    fCurrentNodePath.ReplaceAll("/" + (TString) tDaughterNode->GetName(), "");
  }
}


void GetPdgName(Int_t iPdgCode, const char*& cPdgName) {
  auto itPdg = giPdgMap.find(iPdgCode);
  if (itPdg != giPdgMap.end()) {
    cPdgName = itPdg->second;
  } else {
    cPdgName = "XXX";
    LOG(error) << "unknown PDG code: " << iPdgCode;
  }
}

void GetMaterialName(const char* cMaterial, const char*& cMaterialName) {
  auto itMaterial = gcMaterialMap.find(cMaterial);
  if (itMaterial != gcMaterialMap.end()) {
    cMaterialName = itMaterial->second;
  } else {
    cMaterialName = "XXX";
    LOG(error) << "unknown material: " << cMaterial;
  }
}

void GetProcessName(const char* cProcess, const char*& cProcessName) {
  auto itProcess = gcProcessMap.find(cProcess);
  if (itProcess != gcProcessMap.end()) {
    cProcessName = itProcess->second;
  } else {
    cProcessName = "XXX";
    LOG(error) << "unknown process: " << cProcess;
  }
}


ClassImp(CbmTofAnaTestbeam);
