/* Copyright (C) 2018-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Norbert Herrmann [committer] */

/**
 * CbmDeviceUnpackTofMcbm2018.h
 *
 * @since 2018-04-25
 * @author F. Uhlig
 */

// TODO: (VF, 190914) Many unused private members were commented out.
// The class has to be revised.

#ifndef CBMDEVICEUNPACKTOFMCBM2018_H_
#define CBMDEVICEUNPACKTOFMCBM2018_H_

#include "CbmMcbm2018TofPar.h"
#include "CbmMqTMessage.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include "FairMQDevice.h"

#include "Rtypes.h"

#include <map>
#include <vector>

#include "gDpbMessv100.h"

class CbmMcbm2018UnpackerAlgoTof;
class CbmMcbm2018TofPar;
class CbmTbDaqBuffer;
class CbmHistManager;
class CbmTofDigi;
class TH1;
class TH2;

class CbmDeviceUnpackTofMcbm2018 : public FairMQDevice {
public:
  CbmDeviceUnpackTofMcbm2018();
  virtual ~CbmDeviceUnpackTofMcbm2018();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);
  bool HandleParts(FairMQParts&, int);
  bool HandleMessage(FairMQMessagePtr&, int);
  virtual void BuildTint(int);
  virtual bool SendDigis(std::vector<CbmTofDigi*>, int);

private:
  uint64_t fNumMessages;
  uint64_t fiSelectComponents;
  uint64_t fNumTint;
  std::vector<int> fEventHeader;
  uint64_t fiReqMode;
  uint64_t fiReqTint;
  uint64_t fiReqBeam;
  std::vector<Int_t> fiReqDigiAddr;
  Int_t fiPulserMode;
  uint64_t fiPulMulMin;
  uint64_t fiPulTotMin;
  uint64_t fiPulTotMax;

  std::vector<std::string> fAllowedChannels             = {"tofcomponent", "parameters", "tofdigis", "syscmd"};
  std::vector<std::vector<std::string>> fChannelsToSend = {{}, {}, {}};

  size_t fuTotalMsNb;   /** Total nb of MS per link in timeslice **/
  size_t fuOverlapMsNb; /** Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
  size_t fuCoreMs;      /** Number of non overlap MS at beginning of TS **/
  Double_t fdMsSizeInNs;
  Double_t fdTsCoreSizeInNs;
  UInt_t fuMinNbGdpb;
  UInt_t fuNrOfGdpbs;            // Total number of GDPBs in the system
  UInt_t fuNrOfFeePerGdpb;       // Number of FEEs per GDPB
  UInt_t fuNrOfGet4PerFee;       // Number of GET4s per FEE
  UInt_t fuNrOfChannelsPerGet4;  // Number of channels in each GET4

  UInt_t fuNrOfChannelsPerFee;   // Number of channels in each FEET
  UInt_t fuNrOfGet4;             // Total number of Get4 chips in the system
  UInt_t fuNrOfGet4PerGdpb;      // Number of GET4s per GDPB
  UInt_t fuNrOfChannelsPerGdpb;  // Number of channels per GDPB

  const UInt_t kuNbFeePerGbtx  = 5;
  const UInt_t kuNbGbtxPerGdpb = 6;

  UInt_t fuGdpbId;  // Id (hex number) of the GDPB for current message
  UInt_t fuGdpbNr;  // running number (0 to fNrOfGdpbs) of the GDPB for current message
  UInt_t fuGet4Id;  // running number (0 to fNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr;  // running number (0 to fNrOfGet4) of the Get4 chip in the system for current message

  std::vector<int> fMsgCounter;
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

  //   CbmHistManager* fHM;  ///< Histogram manager

  /** Current epoch marker for each GDPB and GET4
     * (first epoch in the stream initializes the map item)
     * pointer points to an array of size fNrOfGdpbs * fNrOfGet4PerGdpb
     * The correct array index is calculated using the function
     * GetArrayIndex(gdpbId, get4Id)
     **/
  std::vector<ULong64_t> fvulCurrentEpoch;  //!
  std::vector<Bool_t> fvbFirstEpochSeen;    //!

  Int_t fNofEpochs;              /** Current epoch marker for each ROC **/
  ULong64_t fulCurrentEpochTime; /** Time stamp of current epoch **/

  //Double_t fdMsIndex;
  Double_t fdToffTof;
  UInt_t fiAddrRef;

  //UInt_t     fuDiamondDpbIdx;
  //Bool_t fbEpochSuppModeOn;
  //Bool_t fbGet4M24b;
  //Bool_t fbGet4v20;
  //Bool_t fbMergedEpochsOn;

  CbmMcbm2018TofPar* fUnpackPar;  //!

  // Variables used for histo filling
  Double_t fdLastDigiTime;
  Double_t fdFirstDigiTimeDif;
  //Double_t fdEvTime0;
  TH1* fhRawTDigEvT0;
  TH1* fhRawTDigRef0;
  TH1* fhRawTDigRef;
  TH1* fhRawTRefDig0;
  TH1* fhRawTRefDig1;
  TH1* fhRawDigiLastDigi;
  std::vector<TH2*> fhRawTotCh;
  std::vector<TH1*> fhChCount;
  std::vector<Bool_t> fvbChanThere;
  std::vector<TH2*> fhChanCoinc;
  //Bool_t fbDetChanThere[64]; // FIXME
  TH2* fhDetChanCoinc;

  inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id) { return gdpbId * fuNrOfGet4PerGdpb + get4Id; }

  ///* PADI channel to GET4 channel mapping and reverse *///
  std::vector<UInt_t> fvuPadiToGet4;
  std::vector<UInt_t> fvuGet4ToPadi;

  ///* GET4 to eLink mapping and reverse *///
  static const UInt_t kuNbGet4PerGbtx = 5 * 8;  /// 5 FEE with 8 GET4 each
  std::vector<UInt_t> fvuElinkToGet4;
  std::vector<UInt_t> fvuGet4ToElink;
  inline UInt_t ConvertElinkToGet4(UInt_t uElinkIdx)
  {
    return fvuElinkToGet4[uElinkIdx % kuNbGet4PerGbtx] + kuNbGet4PerGbtx * (uElinkIdx / kuNbGet4PerGbtx);
  }
  inline UInt_t ConvertGet4ToElink(UInt_t uGet4Idx)
  {
    return fvuGet4ToElink[uGet4Idx % kuNbGet4PerGbtx] + kuNbGet4PerGbtx * (uGet4Idx / kuNbGet4PerGbtx);
  }
  std::vector<Int_t> fviRpcType;
  std::vector<Int_t> fviModuleId;
  std::vector<Int_t> fviNrOfRpc;
  std::vector<Int_t> fviRpcSide;
  std::vector<Int_t> fviRpcChUId;

  CbmTbDaqBuffer* fBuffer;

  bool CheckTimeslice(const fles::Timeslice& ts);
  void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);
  bool IsChannelNameAllowed(std::string channelName);

  void SetParContainers();
  Bool_t InitContainers();
  Bool_t ReInitContainers();
  void CreateHistograms();
  void AddReqDigiAddr(int);

  Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

  /// Temp until we change from CbmMcbmUnpack to something else
  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void SetNbMsInTs(size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/) {};

  /// Algo settings setters
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE);
  void SetTimeOffsetNs(Double_t dOffsetIn = 0.0);
  void SetDiamondDpbIdx(UInt_t uIdx = 2);

  /// Processing algo
  CbmMcbm2018UnpackerAlgoTof* fUnpackerAlgo;
  /// Control flags
  // Bool_t fbMonitorMode;  //! Switch ON the filling of a minimal set of histograms
  // Bool_t fbDebugMonitorMode; //! Switch ON the filling of a additional set of histograms
  // Bool_t fbSeparateArrayT0; //! If ON, T0 digis are saved in separate TClonesArray
  // Bool_t fbWriteOutput; //! If ON the output TClonesArray of digi is written to disk

  CbmDeviceUnpackTofMcbm2018(const CbmDeviceUnpackTofMcbm2018&) = delete;
  CbmDeviceUnpackTofMcbm2018 operator=(const CbmDeviceUnpackTofMcbm2018&) = delete;
};

#endif /* CBMDEVICEUNPACKTOFMCBM2018_H_ */
