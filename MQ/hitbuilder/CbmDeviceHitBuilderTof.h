/* Copyright (C) 2018-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

/**
 * CbmDeviceHitBuilderTof.h
 *
 * @since 2018-05-31
 * @author N. Herrmann
 */

#ifndef CBMDEVICEHITBUILDERTOF_H_
#define CBMDEVICEHITBUILDERTOF_H_

#include "CbmMqTMessage.h"
#include "CbmTofAddress.h"     // in cbmdata/tof
#include "CbmTofGeoHandler.h"  // in tof/TofTools

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include "FairEventHeader.h"
#include "FairMQDevice.h"

#include "Rtypes.h"
#include "TFile.h"
#include "TGeoManager.h"
#include "TMessage.h"
#include "TTree.h"

#include <map>
#include <vector>

class CbmHistManager;
// Relevant TOF classes
class CbmTofDigi;
class CbmTofHit;
class CbmMatch;
class CbmEvent;
class CbmVertex;
// Geometry
class CbmTofGeoHandler;
class CbmTofDetectorId;
class CbmTofDigiPar;
class CbmTofDigiBdfPar;
class CbmTofCell;

// ROOT Classes and includes
class TClonesArray;
class TF1;
class TH1;
class TH2;
class TProfile;
class TString;

// C++ Classes and includes
#include <list>
#include <map>
#include <vector>

class CbmDeviceHitBuilderTof : public FairMQDevice {
public:
  CbmDeviceHitBuilderTof();
  virtual ~CbmDeviceHitBuilderTof();

protected:
  virtual void InitTask();
  //bool HandleData(FairMQMessagePtr&, int);
  bool HandleData(FairMQParts&, int);
  bool HandleMessage(FairMQMessagePtr&, int);

  virtual void fit_ybox(const char* hname);                    // Fit
  virtual void fit_ybox(TH1* h, Double_t dy);                  // Fit
  virtual void fit_ybox(TH1* h, Double_t dy, Double_t* fpar);  // Fit
  virtual void CheckLHMemory();                                // Check consistency of stored last hits
  virtual void CleanLHMemory();                                // Cleanup
  virtual Bool_t AddNextChan(Int_t iSmType, Int_t iSm, Int_t iRpc, Int_t iLastChan, Double_t dLastPosX,
                             Double_t dLastPosY, Double_t dLastTime,
                             Double_t dLastTot);  // needed for time based data
  virtual void LH_store(Int_t iSmType, Int_t iSm, Int_t iRpc, Int_t iChm, CbmTofHit* pHit);

private:
  // Variables used for histo filling

  Bool_t IsChannelNameAllowed(std::string channelName);

  Bool_t InitWorkspace();
  Bool_t InitContainers();
  Bool_t LoadGeometry();
  Bool_t InitRootOutput();

  Bool_t ReInitContainers();
  void CreateHistograms();
  void WriteHistograms();

  Bool_t MonitorPulser();
  Bool_t ApplyPulserCorrection();
  Bool_t InitCalibParameter();
  Bool_t BuildClusters();
  Bool_t InspectRawDigis();
  Bool_t CalibRawDigis();
  Bool_t FillDigiStor();
  Bool_t BuildHits();
  Bool_t MergeClusters();
  Bool_t FillHistos();
  Bool_t SendHits();
  Bool_t SendAll();

  uint64_t fNumMessages;
  std::vector<std::string> fAllowedChannels = {"tofcomponent", "parameters", "tofdigis", "tofhits", "syscmd"};

  TGeoManager* fGeoMan;
  // ToF geometry variables
  CbmTofGeoHandler* fGeoHandler;
  CbmTofDetectorId* fTofId;
  CbmTofDigiPar* fDigiPar;
  CbmTofCell* fChannelInfo;
  CbmTofDigiBdfPar* fDigiBdfPar;
  // Input variables
  Int_t fiNDigiIn;
  std::vector<CbmTofDigi> fvDigiIn;
  std::vector<int> fEventHeader;
  FairEventHeader* fEvtHeader;

  // Output variables
  // TClonesArray* fTofCalDigisColl;      // Calibrated TOF Digis
  std::vector<CbmTofDigi>* fTofCalDigiVec = nullptr;  // Calibrated TOF Digis
  TClonesArray* fTofHitsColl;                         // TOF hits
  TClonesArray* fTofDigiMatchColl;                    // TOF Digi Links
  TClonesArray* fTofHitsCollOut;                      // TOF hits
  TClonesArray* fTofDigiMatchCollOut;                 // TOF Digi Links
  Int_t fiNbHits;                                     // Index of the CbmTofHit TClonesArray

  // Constants or setting parameters
  Int_t fiNevtBuild;
  Int_t fiMsgCnt;

  Double_t fdTOTMax;
  Double_t fdTOTMin;
  Double_t fdTTotMean;

  Double_t fdMaxTimeDist;   // Isn't this just a local variable? Why make it global and preset?!?
  Double_t fdMaxSpaceDist;  // Isn't this just a local variable? Why make it global and preset?!?

  Double_t fdEvent;
  Int_t fiMaxEvent;
  Int_t fiRunId;

  Int_t fiOutputTreeEntry;
  Int_t fiFileIndex;

  // Intermediate storage variables
  std::vector<std::vector<std::vector<std::vector<CbmTofDigi*>>>> fStorDigi;  //[nbType][nbSm*nbRpc][nbCh][nDigis]
  std::vector<std::vector<std::vector<std::vector<Int_t>>>> fStorDigiInd;     //[nbType][nbSm*nbRpc][nbCh][nDigis]
  std::vector<Int_t> vDigiIndRef;

  std::vector<std::vector<std::vector<Int_t>>> fviClusterMul;   //[nbType][nbSm][nbRpc]
  std::vector<std::vector<std::vector<Int_t>>> fviClusterSize;  //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Int_t>>> fviTrkMul;       //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdX;         //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdY;         //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdDifX;      //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdDifY;      //[nbType][nbRpc][nClusters]
  std::vector<std::vector<std::vector<Double_t>>> fvdDifCh;     //[nbType][nbRpc][nClusters]

  // Intermediate calibration variables
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPDelTof;   //[nSMT][nRpc][nbClDelTofBinX][nbSel]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPTOff;     //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPTotGain;  //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPTotOff;   //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<std::vector<Double_t>>>>>
    fvCPWalk;  //[nSMT][nRpc][nCh][nbSide][nbWalkBins]
  std::vector<std::vector<std::vector<std::vector<std::list<CbmTofHit*>>>>> fvLastHits;  //[nSMT[nSm][nRpc][nCh][NHits]
  std::vector<Int_t> fvDeadStrips;                                                       //[nbDet]

  std::vector<std::vector<Double_t>> fvPulserOffset;  //[nbDet][nbSide]
  const Int_t NPulserTimes = 10;
  std::vector<std::vector<std::list<Double_t>>> fvPulserTimes;  //[nbDet][nbSide][NPulserTimes]


  // histograms
  TH1* fhEvDetMul;
  TH1* fhPulMul;
  TH2* fhPulserTimesRaw;
  std::vector<TProfile*> fhPulserTimeRawEvo;
  TH2* fhPulserTimesCor;
  TH2* fhDigiTimesRaw;
  TH2* fhDigiTimesCor;

  std::vector<TH2*> fhRpcDigiTot;                            //[nbDet]
  std::vector<TH2*> fhRpcDigiCor;                            //[nbDet]
  std::vector<TH1*> fhRpcCluMul;                             //[nbDet]
  std::vector<TH1*> fhRpcCluRate;                            //[nbDet]
  std::vector<TH2*> fhRpcCluPosition;                        //[nbDet]
  std::vector<TH2*> fhRpcCluDelPos;                          //[nbDet]
  std::vector<TH2*> fhRpcCluDelMatPos;                       //[nbDet]
  std::vector<TH2*> fhRpcCluTOff;                            //[nbDet]
  std::vector<TH2*> fhRpcCluDelTOff;                         //[nbDet]
  std::vector<TH2*> fhRpcCluDelMatTOff;                      //[nbDet]
  std::vector<TH2*> fhRpcCluTrms;                            //[nbDet]
  std::vector<TH2*> fhRpcCluTot;                             //[nbDet]
  std::vector<TH2*> fhRpcCluSize;                            //[nbDet]
  std::vector<TH2*> fhRpcCluAvWalk;                          //[nbDet]
  std::vector<TH2*> fhRpcCluAvLnWalk;                        //[nbDet]
  std::vector<std::vector<std::vector<TH2*>>> fhRpcCluWalk;  // [nbDet][nbCh][nSide]
  std::vector<TH2*> fhSmCluPosition;                         //[nbSmTypes]
  std::vector<TH2*> fhSmCluTOff;
  std::vector<TProfile*> fhSmCluSvel;
  std::vector<std::vector<TProfile*>> fhSmCluFpar;
  std::vector<TH1*> fhRpcDTLastHits;          //[nbDet]
  std::vector<TH1*> fhRpcDTLastHits_Tot;      //[nbDet]
  std::vector<TH1*> fhRpcDTLastHits_CluSize;  //[nbDet]

  std::vector<std::vector<TH1*>> fhTRpcCluMul;                             //[nbDet][nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCluPosition;                        //[nbDet][nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCluTOff;                            //[nbDet] [nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCluTot;                             // [nbDet][nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCluSize;                            // [nbDet][nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCluAvWalk;                          // [nbDet][nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCluDelTof;                          // [nbDet][nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCludXdY;                            // [nbDet][nbSel]
  std::vector<std::vector<std::vector<std::vector<TH2*>>>> fhTRpcCluWalk;  // [nbDet][nbSel][nbCh][nSide]

  std::vector<std::vector<TH2*>> fhTSmCluPosition;  //[nbSmTypes][nbSel]
  std::vector<std::vector<TH2*>> fhTSmCluTOff;      //[nbSmTypes][nbSel]
  std::vector<std::vector<TH2*>> fhTSmCluTRun;      //[nbSmTypes][nbSel]
  std::vector<std::vector<TH2*>> fhTRpcCluTOffDTLastHits;
  std::vector<std::vector<TH2*>> fhTRpcCluTotDTLastHits;
  std::vector<std::vector<TH2*>> fhTRpcCluSizeDTLastHits;
  std::vector<std::vector<TH2*>> fhTRpcCluMemMulDTLastHits;

  std::vector<TH1*> fhSeldT;  //[nbSel]

  // Calibration control variables
  Double_t dTRef;
  Int_t fCalMode;
  Double_t fdCaldXdYMax;
  Int_t fiCluMulMax;
  Int_t fTRefMode;
  Int_t fTRefHits;
  Int_t fDutId;
  Int_t fDutSm;
  Int_t fDutRpc;
  Int_t fDutAddr;
  Int_t fSelId;
  Int_t fSelSm;
  Int_t fSelRpc;
  Int_t fSelAddr;
  Int_t fiBeamRefType;
  Int_t fiBeamRefSm;
  Int_t fiBeamRefDet;
  Int_t fiBeamRefAddr;
  Int_t fiBeamRefMulMax;
  Int_t fiBeamAddRefMul;
  Int_t fSel2Id;
  Int_t fSel2Sm;
  Int_t fSel2Rpc;
  Int_t fSel2Addr;
  Int_t fiMode;
  Int_t fiPulserMode;
  uint64_t fiPulMulMin;
  uint64_t fiPulDetRef;
  uint64_t fiPulTotMin;
  uint64_t fiPulTotMax;

  std::map<UInt_t, UInt_t> fDetIdIndexMap;
  std::vector<Int_t> fviDetId;

  Double_t fPosYMaxScal;
  Double_t fTRefDifMax;
  Double_t fTotMax;
  Double_t fTotMin;
  Double_t fTotMean;
  Double_t fdDelTofMax;
  Double_t fMaxTimeDist;
  Double_t fdChannelDeadtime;
  Double_t fdMemoryTime;

  Bool_t fEnableMatchPosScaling;
  Bool_t fbPs2Ns;  // convert input raw digis from ps to ns

  TString fCalParFileName;   // name of the file name with Calibration Parameters
  TString fOutHstFileName;   // name of the histogram output file name with Calibration Parameters
  TString fOutRootFileName;  // name of the output file name with Digis & Hits
  TFile* fCalParFile;        // pointer to Calibration Parameter file
  TFile* fOutRootFile;       // pointer to root output file
};

#endif /* CBMDEVICEHITBUILDERTOF_H_ */
