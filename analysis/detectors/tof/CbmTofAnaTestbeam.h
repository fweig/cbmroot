/* Copyright (C) 2014-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Norbert Herrmann [committer], Christian Simon */

// ------------------------------------------------------------------
// -----                     CbmTofTests                        -----
// -----              Created 29/08/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef _CBMTOFANATESTBEAM_H_
#define _CBMTOFANATESTBEAM_H_ 1

#include "CbmTofFindTracks.h"
#include "CbmTofTestBeamClusterizer.h"

class CbmTofGeoHandler;
class CbmTofCell;
// Geometry
class CbmTofGeoHandler;
class CbmTofDetectorId;
class CbmTofCell;

class CbmTofDigiPar;
class CbmTofDigiBdfPar;
class TMbsMappingTofPar;

//class CbmTofDigi;
class CbmTofHit;
class CbmTofTracklet;

class CbmMCDataArray;
class CbmMCDataObject;
class CbmMCEventList;

#include "CbmTofAddress.h"  // in cbmdata/tof

#include "FairTask.h"

class TClonesArray;
class TH1;
class TH2;
class TString;
class TTrbHeader;
class TGeoNode;
class TGeoPhysicalNode;
class TEfficiency;

#include "TTimeStamp.h"

class CbmTofAnaTestbeam : public FairTask {
  friend class CbmTofFindTracks;
  friend class CbmTofTestbeamClusterizer;

public:
  CbmTofAnaTestbeam();
  CbmTofAnaTestbeam(const char* name, Int_t verbose = 1);
  virtual ~CbmTofAnaTestbeam();

  virtual InitStatus Init();
  virtual void Exec(Option_t* option);
  virtual void ExecEvent(Option_t* option);
  virtual void Finish();

  /**
        ** @brief Inherited from FairTask.
        **/
  virtual void SetParContainers();

  Bool_t RegisterInputs();

  inline Double_t GetDXMean() const { return fdDXMean; }
  inline Double_t GetDYMean() const { return fdDYMean; }
  inline Double_t GetDTMean() const { return fdDTMean; }
  inline Double_t GetDXWidth() const { return fdDXWidth; }
  inline Double_t GetDYWidth() const { return fdDYWidth; }
  inline Double_t GetDTWidth() const { return fdDTWidth; }
  inline Double_t GetMul0Max() const { return fdMul0Max; }
  inline Double_t GetMul4Max() const { return fdMul4Max; }
  inline Double_t GetCh4Sel() const { return fdCh4Sel; }
  inline Double_t GetDCh4Sel() const { return fdDCh4Sel; }
  inline Double_t GetPosY4Sel() const { return fdPosY4Sel; }
  inline Double_t GetPosY4SelOff() const { return fdPosY4SelOff; }
  inline Double_t GetChS2Sel() const { return fdChS2Sel; }
  inline Double_t GetDChS2Sel() const { return fdDChS2Sel; }
  inline Double_t GetPosYS2Sel() const { return fdPosYS2Sel; }
  inline Double_t GetPosYS2SelOff() const { return fdPosYS2SelOff; }
  inline Double_t GetSel2TOff() const { return fdSel2TOff; }
  inline Double_t GetMulDMax() const { return fdMulDMax; }
  inline Double_t GetDTDia() const { return fdDTDia; }
  inline Double_t GetDTD4MAX() const { return fdDTD4MAX; }
  inline Double_t GetHitDistMin() const { return fdHitDistMin; }
  inline Double_t GetTOffD4() const { return fdTOffD4; }
  inline Double_t GetTShift() const { return fdTShift; }

  inline Int_t GetCorMode() const { return fiCorMode; }
  inline Int_t GetDut() const { return fiDut; }
  inline Int_t GetDutRpc() const { return fiDutRpc; }
  inline Int_t GetMrpcRef() const { return fiMrpcRef; }
  inline Int_t GetMrpcSel2() const { return fiMrpcSel2; }
  inline Int_t GetMrpcSel3() const { return fiMrpcSel3; }
  inline Int_t GetPlaSelect() const { return fiPlaSelect; }
  inline Int_t GetBeamRefSmType() const { return fiBeamRefSmType; }
  inline Int_t GetBeamRefSmId() const { return fiBeamRefSmId; }
  inline Int_t GetBeamRefRpc() const { return fiBeamRefRpc; }

  inline void SetDXMean(Double_t val) { fdDXMean = val; }
  inline void SetDYMean(Double_t val) { fdDYMean = val; }
  inline void SetDTMean(Double_t val) { fdDTMean = val; }
  inline void SetDXWidth(Double_t val) { fdDXWidth = val; }
  inline void SetDYWidth(Double_t val) { fdDYWidth = val; }
  inline void SetDTWidth(Double_t val) { fdDTWidth = val; }
  inline void SetMul0Max(Double_t val) { fdMul0Max = val; }
  inline void SetMul4Max(Double_t val) { fdMul4Max = val; }
  inline void SetCh4Sel(Double_t val) { fdCh4Sel = val; }
  inline void SetDCh4Sel(Double_t val) { fdDCh4Sel = val; }
  inline void SetPosY4Sel(Double_t val) { fdPosY4Sel = val; }
  inline void SetPosY4SelOff(Double_t val) { fdPosY4SelOff = val; }
  inline void SetChS2Sel(Double_t val) { fdChS2Sel = val; }
  inline void SetDChS2Sel(Double_t val) { fdDChS2Sel = val; }
  inline void SetPosYS2Sel(Double_t val) { fdPosYS2Sel = val; }
  inline void SetPosYS2SelOff(Double_t val) { fdPosYS2SelOff = val; }
  inline void SetSel2TOff(Double_t val) { fdSel2TOff = val; }
  inline void SetSpillDuration(Double_t val) { fdSpillDuration = val; }
  inline void SetSpillBreak(Double_t val) { fdSpillBreak = val; }
  inline void SetStartSpillTime(Double_t val) { fdStartSpillTime = val; }
  inline void SetMulDMax(Double_t val) { fdMulDMax = val; }
  inline void SetDTDia(Double_t val) { fdDTDia = val; }
  inline void SetDTD4MAX(Double_t val) { fdDTD4MAX = val; }
  inline void SetHitDistMin(Double_t val) { fdHitDistMin = val; }
  inline void SetHitDistAv(Double_t val) { fdHitDistAv = val; }
  inline void SetTOffD4(Double_t val) { fdTOffD4 = val; }
  inline void SetTShift(Double_t val) { fdTShift = val; }
  inline void SetChi2Lim(Double_t val) { fdChi2Lim = val; }
  inline void SetChi2Lim2(Double_t val) { fdChi2Lim2 = val; }

  inline void SetCorMode(Int_t ival) { fiCorMode = ival; }
  inline void SetDutAddr(Int_t ival) { fiDutAddr = ival; }
  inline void SetMrpcRefAddr(Int_t ival) { fiMrpcRefAddr = ival; }
  inline void SetMrpcSel2Addr(Int_t ival) { fiMrpcSel2Addr = ival; }
  inline void SetMrpcSel3Addr(Int_t ival) { fiMrpcSel3Addr = ival; }
  inline void SetBeamRefAddr(Int_t ival) { fiBeamRefAddr = ival; }

  inline void SetDut(Int_t ival)
  {
    fiDut     = ival;
    fiDutAddr = CbmTofAddress::GetUniqueAddress(fiDutSm, fiDutRpc, 0, 0, fiDut);
  }
  inline void SetDutSm(Int_t ival)
  {
    fiDutSm   = ival;
    fiDutAddr = CbmTofAddress::GetUniqueAddress(fiDutSm, fiDutRpc, 0, 0, fiDut);
  }
  inline void SetDutRpc(Int_t ival)
  {
    fiDutRpc  = ival;
    fiDutAddr = CbmTofAddress::GetUniqueAddress(fiDutSm, fiDutRpc, 0, 0, fiDut);
  }

  inline void SetMrpcRef(Int_t ival)
  {
    fiMrpcRef     = ival;
    fiMrpcRefAddr = CbmTofAddress::GetUniqueAddress(fiMrpcRefSm, fiMrpcRefRpc, 0, 0, fiMrpcRef);
  }
  inline void SetMrpcRefSm(Int_t ival)
  {
    fiMrpcRefSm   = ival;
    fiMrpcRefAddr = CbmTofAddress::GetUniqueAddress(fiMrpcRefSm, fiMrpcRefRpc, 0, 0, fiMrpcRef);
  }
  inline void SetMrpcRefRpc(Int_t ival)
  {
    fiMrpcRefRpc  = ival;
    fiMrpcRefAddr = CbmTofAddress::GetUniqueAddress(fiMrpcRefSm, fiMrpcRefRpc, 0, 0, fiMrpcRef);
  }

  inline void SetMrpcSel2(Int_t ival)
  {
    fiMrpcSel2     = ival;
    fiMrpcSel2Addr = CbmTofAddress::GetUniqueAddress(fiMrpcSel2Sm, fiMrpcSel2Rpc, 0, 0, fiMrpcSel2);
  }
  inline void SetMrpcSel2Sm(Int_t ival)
  {
    fiMrpcSel2Sm   = ival;
    fiMrpcSel2Addr = CbmTofAddress::GetUniqueAddress(fiMrpcSel2Sm, fiMrpcSel2Rpc, 0, 0, fiMrpcSel2);
  }
  inline void SetMrpcSel2Rpc(Int_t ival)
  {
    fiMrpcSel2Rpc  = ival;
    fiMrpcSel2Addr = CbmTofAddress::GetUniqueAddress(fiMrpcSel2Sm, fiMrpcSel2Rpc, 0, 0, fiMrpcSel2);
  }

  inline void SetMrpcSel3(Int_t ival)
  {
    fiMrpcSel3     = ival;
    fiMrpcSel3Addr = CbmTofAddress::GetUniqueAddress(fiMrpcSel3Sm, fiMrpcSel3Rpc, 0, 0, fiMrpcSel3);
  }
  inline void SetMrpcSel3Sm(Int_t ival)
  {
    fiMrpcSel3Sm   = ival;
    fiMrpcSel3Addr = CbmTofAddress::GetUniqueAddress(fiMrpcSel3Sm, fiMrpcSel3Rpc, 0, 0, fiMrpcSel3);
  }
  inline void SetMrpcSel3Rpc(Int_t ival)
  {
    fiMrpcSel3Rpc  = ival;
    fiMrpcSel3Addr = CbmTofAddress::GetUniqueAddress(fiMrpcSel3Sm, fiMrpcSel3Rpc, 0, 0, fiMrpcSel3);
  }

  inline void SetPlaSelect(Int_t ival) { fiPlaSelect = ival; }
  inline void SetBeamRefSmType(Int_t ival)
  {
    fiBeamRefSmType = ival;
    fiBeamRefAddr   = CbmTofAddress::GetUniqueAddress(fiBeamRefSmId, fiBeamRefRpc, 0, 0, fiBeamRefSmType);
  }
  inline void SetBeamRefSmId(Int_t ival)
  {
    fiBeamRefSmId = ival;
    fiBeamRefAddr = CbmTofAddress::GetUniqueAddress(fiBeamRefSmId, fiBeamRefRpc, 0, 0, fiBeamRefSmType);
  }
  inline void SetBeamRefRpc(Int_t ival)
  {
    fiBeamRefRpc  = ival;
    fiBeamRefAddr = CbmTofAddress::GetUniqueAddress(fiBeamRefSmId, fiBeamRefRpc, 0, 0, fiBeamRefSmType);
  }

  inline void SetReqTrg(Int_t ival) { fiReqTrg = ival; }

  inline void SetCalParFileName(TString CalParFileName) { fCalParFileName = CalParFileName; }
  inline void SetCalOutFileName(TString CalOutFileName) { fCalOutFileName = CalOutFileName; }

  inline void SetChi2LimFit(Double_t val) { fChi2LimFit = val; }
  inline void SetR0LimFit(Double_t val) { fR0LimFit = val; }
  inline void SetSIGLIM(Double_t val) { fSIGLIM = val; }
  inline void SetSIGT(Double_t val) { fSIGT = val; }
  inline void SetSIGX(Double_t val) { fSIGX = val; }
  inline void SetSIGY(Double_t val) { fSIGY = val; }

  inline void SetEnableMatchPosScaling(Bool_t bval) { fEnableMatchPosScaling = bval; }

  inline Double_t GetChi2LimFit() { return fChi2LimFit; }
  inline Double_t GetR0LimFit() { return fR0LimFit; }
  inline Double_t GetSIGLIM() { return fSIGLIM; }
  inline Double_t GetSIGT() { return fSIGT; }
  inline Double_t GetSIGX() { return fSIGX; }
  inline Double_t GetSIGY() { return fSIGY; }

  inline void SetDutX(Double_t val) { fdDutX = val; }
  inline void SetDutDX(Double_t val) { fdDutDX = val; }
  inline void SetDutY(Double_t val) { fdDutY = val; }
  inline void SetDutDY(Double_t val) { fdDutDY = val; }

  inline void SetMonteCarloComparison(Bool_t bval) { fbMonteCarloComparison = bval; }
  inline void SetGhostTrackHitQuota(Double_t val) { fdGhostTrackHitQuota = val; }
  inline void SetDelayMCPoints(Bool_t bval) { fbDelayMCPoints = bval; }
  inline void SetAttachDutHitToTracklet(Bool_t bval) { fbAttachDutHitToTracklet = bval; }
  inline void SetBestSelTrackletOnly(Bool_t bval) { fbBestSelTrackletOnly = bval; }
  inline void SetUseSigCalib(Bool_t bval) { fbUseSigCalib = bval; }
  inline void SetAnaMode(Int_t ival) { fiAnaMode = ival; }

  inline void SetMCSIGLIM(Double_t val) { fdMCSIGLIM = val; }
  inline void SetMCSIGT(Double_t val) { fdMCSIGT = val; }
  inline void SetMCSIGX(Double_t val) { fdMCSIGX = val; }
  inline void SetMCSIGY(Double_t val) { fdMCSIGY = val; }

  inline Double_t GetMCSIGLIM() { return fdMCSIGLIM; }
  inline Double_t GetMCSIGT() { return fdMCSIGT; }
  inline Double_t GetMCSIGX() { return fdMCSIGX; }
  inline Double_t GetMCSIGY() { return fdMCSIGY; }

  Double_t GetSigT(Int_t iSelType);
  Double_t GetSigX(Int_t iSelType);
  Double_t GetSigY(Int_t iSelType);

  Double_t GetSHTSigX();
  Double_t GetSHTSigY();
  Double_t GetSHTSigT();

  inline void SetMinMCRefTrackPoints(Int_t val) { fiMinMCRefTrackPoints = val; }
  inline void SetMaxMCRefTracks(Int_t val) { fiMaxMCRefTracks = val; }

private:
  Bool_t LoadGeometry();
  Bool_t CreateHistos();
  Bool_t FillHistos();
  Bool_t WriteHistos();
  Bool_t DeleteHistos();

  Bool_t FindModuleNodes();
  void ExpandNode(TGeoNode* tMotherNode);

  /**
       ** @brief Initialize other parameters not included in parameter classes.
       **/
  Bool_t InitParameters();
  /**
      ** @brief Load the geometry: for now just resizing the Digis temporary vectors
      **/
  Bool_t LoadCalParameter();

  CbmTofAnaTestbeam(const CbmTofAnaTestbeam&);
  CbmTofAnaTestbeam operator=(const CbmTofAnaTestbeam&);

  Int_t fEvents;  // Number of processed events

  // Geometry infos
  CbmTofGeoHandler* fGeoHandler;
  CbmTofDetectorId* fTofId;
  CbmTofCell* fChannelInfo;
  CbmTofCell* fChannelInfoRef;
  CbmTofCell* fChannelInfoDut;
  CbmTofCell* fChannelInfoSel2;

  TMbsMappingTofPar* fMbsMappingPar;

  Int_t iNbSmTot;
  std::vector<Int_t> fvTypeSmOffs;  // Offset in SM index for first SM of each SM type
  Int_t iNbRpcTot;
  std::vector<std::vector<Int_t>> fvSmRpcOffs;  // Offset in RPC index for first RPC of each SM
  Int_t iNbChTot;
  std::vector<std::vector<std::vector<Int_t>>> fvRpcChOffs;  // Offset in channel index for first channel of each RPC

  // Parameters
  CbmTofDigiPar* fDigiPar;
  CbmTofDigiBdfPar* fDigiBdfPar;

  TClonesArray* fTofDigisColl;      // TOF Digis
  TClonesArray* fTofHitsColl;       // TOF hits
  TClonesArray* fTofDigiMatchColl;  // TOF DigiMatches
  TClonesArray* fTofTrackColl;      // TOF Tracks

  TClonesArray* fEventsColl;          // CBMEvents (time based)
  TClonesArray* fTofDigisCollIn;      // TOF Digis
  TClonesArray* fTofHitsCollIn;       // TOF hits
  TClonesArray* fTofDigiMatchCollIn;  // TOF DigiMatches
  TClonesArray* fTofTrackCollIn;      // TOF Tracks

  TTrbHeader* fTrbHeader;

  Double_t fdDXMean;
  Double_t fdDYMean;
  Double_t fdDTMean;
  Double_t fdDXWidth;
  Double_t fdDYWidth;
  Double_t fdDTWidth;

  // Histograms

  TH1* fhTriggerPattern;
  TH1* fhTriggerType;
  TH1* fhTimeInSpill;
  TH1* fhRate_all;
  TH1* fhTIS_all;
  TH1* fhTIS_sel;
  TH1* fhTIS_sel1;
  TH1* fhTIS_sel2;
  TH1* fhTIR_all;
  TH1* fhTIR_sel;
  TH1* fhTIR_sel1;
  TH1* fhTIR_sel2;
  TH2* fhTIS_Nhit;
  TH2* fhTIS_Ntrk;
  TH1* fhDTLH_all;
  TH1* fhDTLH_sel;
  TH1* fhDTLH_sel1;
  TH1* fhDTLH_sel2;
  TH2* fhDTLH_DStrip;
  TH1* fhDT2;
  TH2* fhXX2;
  TH2* fhYY2;
  TH1* fhNMatch04;
  TH2* fhXX04;
  TH2* fhYY04;
  TH2* fhXY04;
  TH2* fhYX04;
  TH2* fhTT04;

  TH3* fhDutDXDYDT;
  TH3* fhRefDXDYDT;

  TH1* fhChi04;
  TH1* fhChiSel24;
  TH1* fhDXSel24;
  TH1* fhDYSel24;
  TH1* fhDTSel24;
  TH2* fhDXDY04;
  TH2* fhDXDT04;
  TH2* fhDYDT04;
  TH1* fhTofSel24;

  TH1* fhNMatch04sel;
  TH1* fhChi04best;
  TH1* fhDigiMul0best;
  TH1* fhDigiMul4best;
  TH2* fhDXDY04best;
  TH2* fhDXDT04best;
  TH2* fhDYDT04best;
  TH2* fhChiDT04best;
  TH2* fhDT24DT04best;
  TH2* fhDTD4DT04best;
  TH2* fhX0DT04best;
  TH2* fhY0DT04best;

  TH1* fhNMatchD4sel;
  TH1* fhChi04D4best;
  TH1* fhTofD4best;
  TH1* fhVelD4best;
  TH2* fhDigiMul0D4best;
  TH2* fhDigiMul4D4best;
  TH2* fhCluSize04D4best;
  TH2* fhCluMul04D4best;
  TH2* fhStrMul04D4best;
  TH2* fhCluMulTSig0D4best;
  TH2* fhCluMulTSig4D4best;
  TH2* fhCluMulTrel0D4best;
  TH2* fhCluMulTrel4D4best;
  TH2* fhCluSizeTrel0D4best;
  TH2* fhCluSizeTrel4D4best;
  TH2* fhDXDY04D4best;
  TH2* fhDXDT04D4best;
  TH2* fhDYDT04D4best;
  TH2* fhDistDT04D4best;
  TH2* fhTexpDT04D4best;
  TH2* fhCluSize0DT04D4best;
  TH2* fhCluSize4DT04D4best;
  TH2* fhTot0DT04D4best;
  TH2* fhTot4DT04D4best;
  TH2* fhCluSizeSigT0D4best;
  TH2* fhCluSizeSigT4D4best;
  TH2* fhChiDT04D4best;
  TH2* fhDT24DT04D4best;
  TH2* fhDTD4DT04D4best;
  TH2* fhX0DT04D4best;
  TH2* fhY0DT04D4best;
  TH2* fhTISDT04D4best;
  TH2* fhDTMul4D4best;
  TH2* fhDTX4D4best;
  TH2* fhDTY4D4best;
  TH2* fhDXX4D4best;
  TH2* fhDXY4D4best;
  TH2* fhDYX4D4best;
  TH2* fhDYY4D4best;
  TH2* fhDTMul0D4best;
  TH2* fhDTX0D4best;
  TH2* fhDTY0D4best;
  TH2* fhDXX0D4best;
  TH2* fhDXY0D4best;
  TH2* fhDYX0D4best;
  TH2* fhDYY0D4best;

  TH1* fhChi04D4sbest;
  TH1* fhTofD4sbest;
  TH1* fhVelD4sbest;
  TH1* fhDigiMul0D4sbest;
  TH1* fhDigiMul4D4sbest;
  TH2* fhCluMul04D4sbest;
  TH2* fhDXDY04D4sbest;
  TH2* fhDXDT04D4sbest;
  TH2* fhDYDT04D4sbest;
  TH2* fhDistDT04D4sbest;
  TH2* fhTexpDT04D4sbest;
  TH2* fhCluSize0DT04D4sbest;
  TH2* fhCluSize4DT04D4sbest;
  TH2* fhTot0DT04D4sbest;
  TH2* fhTot4DT04D4sbest;
  TH2* fhChiDT04D4sbest;
  TH2* fhDT24DT04D4sbest;
  TH2* fhDTD4DT04D4sbest;
  TH2* fhX0DT04D4sbest;
  TH2* fhY0DT04D4sbest;
  TH2* fhDTMul4D4sbest;
  TH2* fhDTX4D4sbest;
  TH2* fhDTY4D4sbest;
  TH2* fhDXX4D4sbest;
  TH2* fhDXY4D4sbest;
  TH2* fhDYX4D4sbest;
  TH2* fhDYY4D4sbest;
  TH2* fhDTMul0D4sbest;
  TH2* fhDTX0D4sbest;
  TH2* fhDTY0D4sbest;
  TH2* fhDXX0D4sbest;
  TH2* fhDXY0D4sbest;
  TH2* fhDYX0D4sbest;
  TH2* fhDYY0D4sbest;

  TH1* fhNMatch24;
  TH1* fhNMatch24sel;
  TH1* fhDT24sel;
  TH1* fhChi24;
  TH2* fhXY24;
  TH2* fhDXDY24;
  TH2* fhDXDT24;
  TH2* fhDYDT24;

  TH2* fhXY0D4best;
  TH2* fhXY4D4best;
  TH2* fhXX04D4best;
  TH2* fhYY04D4best;
  TH2* fhXYSel2D4best;
  TH2* fhXY0D4sel;
  TH2* fhXY4D4sel;
  TH2* fhXYSel2D4sel;

  TH1* fhDTD4sel;
  TH1* fhTofD4sel;
  TH1* fhBRefMul;
  TH1* fhDTD4;

  TH2* fhXX02[2];
  TH2* fhYY02[2];

  std::vector<TH2*> fhXYPos;  //[nbDet]
  TH2* fhEtaPhi;

  TH2 *fhDT04DX0_1, *fhDT04DY0_1, *fhDT04DT0_1;
  TH2 *fhDT04DX4_1, *fhDT04DY4_1, *fhDT04DT4_1;
  TH2 *fhDT04DX0_2, *fhDT04DY0_2, *fhDT04DT0_2;
  TH2 *fhDT04DX4_2, *fhDT04DY4_2, *fhDT04DT4_2;

  TH1* fhDutPullX;
  TH1* fhDutPullXB;
  TH1* fhDutPullY;
  TH1* fhDutPullYB;
  TH1* fhDutPullZ;
  TH1* fhDutPullT;
  TH1* fhDutPullTB;
  TH1* fhDutChi_Found;
  TH1* fhDutChi_Missed;
  TH1* fhDutChi_Match;
  TH2* fhDutXY_Found;
  TH2* fhDutXY_Missed;
  TH1* fhDutDTLH_Found;
  TH1* fhDutDTLH_Missed;
  TH1* fhDutMul_Found;
  TH1* fhDutMul_Missed;
  TH2* fhDutTIS_Found;
  TH2* fhDutTIS_Missed;
  TH2* fhDutTIR_Found;
  TH2* fhDutTIR_Missed;
  TH1* fhDutVel_Found;
  TH1* fhDutVel_Missed;
  TH2* fhDutDTLH_CluSize;
  TH2* fhDutDTLH_Tot;
  TH2* fhDutDTLH_Mul;
  TH2* fhDutDTLH_TIS;
  TH2* fhDutDTLH_Missed_TIS;
  TH2* fhDutDTLH_DDH_Found;
  TH2* fhDutDTLH_DD_Found;
  TH2* fhDutDTLH_DD_Missed;
  TH3* fhDutXYDX;
  TH3* fhDutXYDY;
  TH3* fhDutXYDT;


  TH1* fhNMergedMCEvents;
  TH1* fhAccTrackMul;
  TH1* fhAccRefTrackMul;
  TH1* fhAccPrimTrackMul;
  TH1* fhAccTrackPointMul;
  TH1* fhAccRefTrackPointMul;
  TH1* fhAccRndmTrackPointMul;

  TH2* fhAccRefTrackAcceptance;
  TEfficiency* fhAccRefTrackAcceptanceEfficiency;
  TEfficiency* fhAccRefTrackAcceptancePurity;
  TH2* fhAccRefTrackMulCentrality;
  TH2* fhAccRefTracksProcSpec;

  TEfficiency* fhSelMCTrackEfficiency;
  TEfficiency* fhSelMCTrackMatchEfficiency;
  TEfficiency* fhSelMCTrackMatchPurity;
  TH1* fhSelMCTrackDutHitMatchNNMul;
  TH1* fhSelMCTrackDutHitMatchAccNNMul;

  TEfficiency* fhSelEfficiency;
  TEfficiency* fhSelPurity;
  TEfficiency* fhSelRefTrackShare;
  TH2* fhSelRefTrackProcSpec;
  TEfficiency* fhSelMatchEfficiency;
  TEfficiency* fhSelMatchPurity;
  TH2* fhResX04HitExp;
  TH2* fhResX04ExpMC;
  TH2* fhResX04HitMC;
  TH2* fhResY04HitExp;
  TH2* fhResY04ExpMC;
  TH2* fhResY04HitMC;
  TH2* fhResT04HitExp;
  TH2* fhResT04ExpMC;
  TH2* fhResT04HitMC;
  TH1* fhNTracksPerMRefHit;
  TH1* fhNTracksPerSel2Hit;
  TH1* fhNTracksPerDutHit;
  TH1* fhNTracksPerSelMRefHit;
  TH1* fhNTracksPerSelSel2Hit;
  TH1* fhNTracksPerSelDutHit;

  TEfficiency* fhSelTrklEfficiency;
  TEfficiency* fhSelTrklPurity;
  TEfficiency* fhSelTrklRefTrackShare;
  TH2* fhSelTrklRefTrackProcSpec;
  TEfficiency* fhSelTrklMatchEfficiency;
  TEfficiency* fhSelTrklMatchPurity;
  TH2* fhDutResX_Hit_Trk;
  TH2* fhDutResX_Trk_MC;
  TH2* fhDutResX_Hit_MC;
  TH2* fhDutResY_Hit_Trk;
  TH2* fhDutResY_Trk_MC;
  TH2* fhDutResY_Hit_MC;
  TH2* fhDutResT_Hit_Trk;
  TH2* fhDutResT_Trk_MC;
  TH2* fhDutResT_Hit_MC;

  TEfficiency* fhSelHitTupleEfficiencyTIS;
  TEfficiency* fhSelTrklEfficiencyTIS;
  TEfficiency* fhSelMCTrackEfficiencyTIS;

  TEfficiency* fhSelHitTupleMatchEfficiencyTIS;
  TEfficiency* fhSelTrklMatchEfficiencyTIS;
  TEfficiency* fhSelMCTrackMatchEfficiencyTIS;

  TH2* fhSelHitTupleResidualTTIS;
  TH2* fhSelTrklResidualTTIS;
  TH2* fhSelMCTrackResidualTTIS;

  TH2* fhSelHitTupleDutCluSizeTIS;
  TH2* fhSelTrklDutCluSizeTIS;
  TH2* fhSelMCTrackDutCluSizeTIS;

  TH2* fhPVResTAll;
  TH2* fhPVResXAll;
  TH2* fhPVResYAll;
  TH2* fhPVResTRef;
  TH2* fhPVResXRef;
  TH2* fhPVResYRef;

  TH2* fhAccRefTrackResT;
  TH2* fhAccRefTrackResX;
  TH2* fhAccRefTrackResY;
  TH2* fhAccRefTrackResTx;
  TH2* fhAccRefTrackResTy;
  TH2* fhAccRefTrackResV;
  TH2* fhAccRefTrackResN;

  TEfficiency* fhAccRefTrackShare;
  TEfficiency* fhRecRefTrackEfficiency;
  TEfficiency* fhRecRndmTrackEfficiency;
  TEfficiency* fhRecRefTrackGhostShare;
  TEfficiency* fhRecRefTrackCloneShare;
  TEfficiency* fhRecRndmTrackGhostShare;
  TEfficiency* fhRecRndmTrackCloneShare;

  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhDomTracksProcSpec;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhDomTracksProcMat;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhRndmTracksProcSpec;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhRndmTracksProcMat;

  std::map<std::tuple<Int_t, Int_t, Int_t>, TH1*> fhCounterAccTrackMul;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH1*> fhCounterAccRefTrackMul;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH1*> fhCounterAccRndmTrackMul;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH1*> fhCounterAccDomTrackMul;

  std::map<std::tuple<Int_t, Int_t, Int_t>, TH1*> fhCounterRecRefTrackEfficiencyPassed;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH1*> fhCounterRecRefTrackEfficiencyTotal;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH1*> fhCounterRecRefTrackPurityPassed;

  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhCounterRefTrackMulHitMul;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhCounterRefTrackLocalXY;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhCounterRefTrackMulCell;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TH2*> fhCounterHitMulCell;

  TH1* fhSelTrklFitRedChiSq;
  TH1* fhSelTrklDutHitMatchNNMul;
  TH1* fhSelTrklDutHitMatchAccNNMul;

  TH1* fhSelHitTupleDutHitMatchMul;
  TH1* fhSelHitTupleDutHitMatchAccMul;

  TH2* fhSelTypeNNChiSq;
  TH2* fhSelTypeNNResidualT;
  TH2* fhSelTypeNNResidualX;
  TH2* fhSelTypeNNResidualY;

  TH2* fhSelTypeAccNNChiSq;
  TH2* fhSelTypeAccNNResidualT;
  TH2* fhSelTypeAccNNResidualX;
  TH2* fhSelTypeAccNNResidualY;

  TH2* fhGoodSelTypeNNPureChiSq;
  TH2* fhGoodSelTypeNNAllChiSq;

  // Test class performance

  // Rates and data rates

  TH2* fhTrklNofHitsRate;
  TH2* fhTrklDetHitRate;
  TH2* fhTrklNofHitsRateInSpill;
  TH2* fhTrklDetHitRateInSpill;

  // Performances check

  // Control
  TTimeStamp fStart;
  TTimeStamp fStop;

  TString fCalParFileName;  // name of the file name with Calibration Parameters
  TString fCalOutFileName;
  TFile* fCalParFile;                   // pointer to Calibration Parameter file
  TH1* fhDTD4DT04D4Off;                 // 'calibration' histo
  TH1* fhDTX4D4Off;                     // 'calibration' histo
  TH1* fhDTY4D4Off;                     // 'calibration' histo
  TH1* fhDTTexpD4Off;                   // 'calibration' histo
  TH1* fhCluSize0DT04D4Off;             // 'calibration' histo
  TH1* fhCluSize4DT04D4Off;             // 'calibration' histo
  TH1* fhTot0DT04D4Off;                 // 'calibration' histo
  TH1* fhTot4DT04D4Off;                 // 'calibration' histo
  TH1* fhSelTypeNNResidualT_Width;      // 'calibration' histo
  TH1* fhSelTypeNNResidualX_Width;      // 'calibration' histo
  TH1* fhSelTypeNNResidualY_Width;      // 'calibration' histo
  TH1* fhSelHitTupleResidualXYT_Width;  // 'calibration' histo
  Double_t fdMulDMax;                   // max multiplicity in Diamond counter
  Double_t fdSpillDuration;             // min. spill length in sec
  Double_t fdSpillBreak;                // min. spill break in sec
  Double_t fdStartSpillTime;            // Start of Spill, if negative find begin of first spill
  Double_t fdDTDia;                     // max time difference between diamonds
  Double_t fdDTD4MAX;                   // max time difference between reference & diamond
  Double_t fdMul0Max;                   // max multiplicity in Dut
  Double_t fdMul4Max;                   // max multiplicity in HD/Buc Ref counter
  Double_t fdCh4Sel;                    // Center of selected strip numbers
  Double_t fdDCh4Sel;                   // Width  of selected strip numbers
  Double_t fdPosY4Sel;                  // Fraction of Y-Size
  Double_t fdPosY4SelOff;               // Offset of selection interval in cm
  Double_t fdChS2Sel;                   // Center of selected strip numbers
  Double_t fdDChS2Sel;                  // Width  of selected strip numbers
  Double_t fdPosYS2Sel;                 // Fraction of Y-Size
  Double_t fdPosYS2SelOff;              // Offset of selection interval in cm
  Double_t fdSel2TOff;                  // Offset for matching Sel2 time

  Double_t fdHitDistMin;  // minimal spatial distance of correlated hits
  Double_t fdHitDistAv;   // average spatial distance of correlated hits
  Double_t fdTOffD4;      // Offset to calibrate TD4 to time of flight
  Double_t fdTShift;      // general time shift of all hits
  Double_t fdChi2Lim;     // Chi2 Limit to accept selector coincidence
  Double_t fdChi2Lim2;    // Chi2 Limit for Mref - Sel2 to accept coincidence

  Double_t fdDutX;
  Double_t fdDutDX;
  Double_t fdDutY;
  Double_t fdDutDY;

  Int_t fiCorMode;        // Correction histogram generation mode
  Int_t fiDutAddr;        // Device under test Address
  Int_t fiMrpcRefAddr;    // Reference Mrpc Address
  Int_t fiMrpcSel2Addr;   // Coincident Mrpc 2 Addr
  Int_t fiMrpcSel3Addr;   // Coincident Mrpc 3 Addr
  Int_t fiBeamRefAddr;    // Beam reference counter
  Int_t fiDut;            // Device under test type
  Int_t fiDutSm;          // requested (super)module of Device under test
  Int_t fiDutRpc;         // requested Rpc of Device under test
  Int_t fiMrpcRef;        // Reference Mrpc Type
  Int_t fiMrpcRefSm;      // Reference Mrpc Sm
  Int_t fiMrpcRefRpc;     // Reference Mrpc Rpc
  Int_t fiMrpcSel2;       // Coincident Mrpc 2 type
  Int_t fiMrpcSel2Sm;     // Coincident Mrpc 2 module
  Int_t fiMrpcSel2Rpc;    // Coincident Mrpc 2 Rpc
  Int_t fiMrpcSel3;       // Coincident Mrpc 3 Type
  Int_t fiMrpcSel3Sm;     // Coincident Mrpc 3 module
  Int_t fiMrpcSel3Rpc;    // Coincident Mrpc 3 Rpc
  Int_t fiPlaSelect;      // Select plastics: 0 - P2, 2 - Buc2013
  Int_t fiBeamRefSmType;  // Beam reference counter type
  Int_t fiBeamRefSmId;    // Beam reference module
  Int_t fiBeamRefRpc;     // Beam reference Rpc
  Int_t fiDutNch;         // Number of cells in Device under test
  Int_t fiReqTrg;         // Requested Trigger Pattern

  Double_t fChi2LimFit;
  Double_t fR0LimFit;
  Double_t fSIGLIM;
  Double_t fSIGT;
  Double_t fSIGX;
  Double_t fSIGY;

  Bool_t fEnableMatchPosScaling;

  CbmTofFindTracks* fFindTracks;            // Pointer to Task
  CbmTofTestBeamClusterizer* fClusterizer;  // Pointer to Task
  CbmTofTrackletTools* fTrackletTools;      // Pointer to Class

  Bool_t fbMonteCarloComparison;
  Bool_t fbPointsInInputFile;
  Bool_t fbTracksInInputFile;
  CbmMCDataObject* fMCEventHeader;
  CbmMCEventList* fMCEventList;
  TClonesArray* fAccTracks;
  TClonesArray* fTofPointsTB;
  CbmMCDataArray* fTofPoints;
  CbmMCDataArray* fMCTracks;
  TClonesArray* fTofHitPointMatches;
  TClonesArray* fTofHitAccTrackMatches;
  TClonesArray* fTofTrackletAccTrackMatches;
  TClonesArray* fTofAccTrackTrackletMatches;
  TClonesArray* fTofAccTrackPointMatches;

  TString fCurrentNodePath;
  TString fCurrentModuleNodePath;
  Int_t fiCurrentModuleType;
  Int_t fiCurrentModuleIndex;
  Int_t fiCurrentCounterIndex;
  std::map<std::tuple<Int_t, Int_t, Int_t>, TGeoPhysicalNode*> fCounterModuleNodes;

  Int_t fiNAccRefTracks;
  Double_t fdGhostTrackHitQuota;
  Bool_t fbDelayMCPoints;
  Bool_t fbAttachDutHitToTracklet;
  Bool_t fbBestSelTrackletOnly;
  Bool_t fbUseSigCalib;
  Int_t fiAnaMode;

  Double_t fdMCSIGLIM;
  Double_t fdMCSIGT;
  Double_t fdMCSIGX;
  Double_t fdMCSIGY;

  Int_t fiMinMCRefTrackPoints;
  Int_t fiMaxMCRefTracks;

  ClassDef(CbmTofAnaTestbeam, 1);
};

#endif
