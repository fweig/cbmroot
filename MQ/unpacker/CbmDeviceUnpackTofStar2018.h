/* Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/**
 * CbmDeviceUnpackTofStar2018.h
 *
 * @since 2018-04-25
 * @author F. Uhlig
 */

#ifndef CBMDEVICEUNPACKTOFSTAR2018_H_
#define CBMDEVICEUNPACKTOFSTAR2018_H_

#include "CbmMqTMessage.h"

#include "MicrosliceDescriptor.hpp"
#include "Timeslice.hpp"

#include "FairMQDevice.h"

#include "Rtypes.h"
#include "TMessage.h"

#include <map>
#include <vector>

#include "rocMess_wGet4v1.h"

class CbmTofUnpackPar;
class CbmTbDaqBuffer;
class CbmHistManager;
class CbmTofDigiExp;
class TH1;
class TH2;

class CbmDeviceUnpackTofStar2018 : public FairMQDevice {
public:
  CbmDeviceUnpackTofStar2018();
  virtual ~CbmDeviceUnpackTofStar2018();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);

private:
  uint64_t fNumMessages;

  std::vector<std::string> fAllowedChannels = {"tofcomponent", "parameters"};


  size_t fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
  size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
  UInt_t fuMinNbGdpb;
  UInt_t fuCurrNbGdpb;

  UInt_t fuNrOfGdpbs;            // Total number of GDPBs in the system
  UInt_t fuNrOfFebsPerGdpb;      // Number of FEBs per GDPB
  UInt_t fuNrOfGet4PerFeb;       // Number of GET4s per FEB
  UInt_t fuNrOfChannelsPerGet4;  // Number of channels in each GET4

  UInt_t fuNrOfChannelsPerFeet;  // Number of channels in each FEET
  UInt_t fuNrOfGet4;             // Total number of Get4 chips in the system
  UInt_t fuNrOfGet4PerGdpb;      // Number of GET4s per GDPB
  UInt_t fuNrOfChannelsPerGdpb;  // Number of channels per GDPB

  std::vector<int> fMsgCounter;
  std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

  UInt_t fuGdpbId;  // Id (hex number) of the GDPB for current message
  UInt_t fuGdpbNr;  // running number (0 to fNrOfGdpbs) of the GDPB for current message
  UInt_t fuGet4Id;  // running number (0 to fNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
  UInt_t fuGet4Nr;  // running number (0 to fNrOfGet4) of the Get4 chip in the system for current message

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

  Int_t fEquipmentId;
  Double_t fdMsIndex;
  Double_t fdTShiftRef;

  Bool_t fbEpochSuppModeOn;
  Bool_t fbGet4M24b;
  Bool_t fbGet4v20;
  Bool_t fbMergedEpochsOn;

  CbmTofDigiExp* fDigi;

  CbmTofUnpackPar* fUnpackPar;  //!

  // Variables used for histo filling
  Double_t fdRefTime;
  Double_t fdLastDigiTime;
  Double_t fdFirstDigiTimeDif;
  Double_t fdEvTime0;
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
  Bool_t fbDetChanThere[64];
  TH2* fhDetChanCoinc;

  std::vector<std::vector<ngdpb::Message>> fvmEpSupprBuffer;

  CbmTbDaqBuffer* fBuffer;


  ULong64_t fulGdpbTsMsb;
  ULong64_t fulGdpbTsLsb;
  ULong64_t fulStarTsMsb;
  ULong64_t fulStarTsMid;
  ULong64_t fulGdpbTsFullLast;
  ULong64_t fulStarTsFullLast;
  UInt_t fuStarTokenLast;
  UInt_t fuStarDaqCmdLast;
  UInt_t fuStarTrigCmdLast;

  bool CheckTimeslice(const fles::Timeslice& ts);
  void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);
  bool IsChannelNameAllowed(std::string channelName);

  Bool_t InitContainers();

  Bool_t ReInitContainers();
  void CreateHistograms();

  void FillHitInfo(ngdpb::Message);
  void FillStarTrigInfo(ngdpb::Message);
  void FillEpochInfo(ngdpb::Message);
  void PrintSlcInfo(ngdpb::Message);
  void PrintSysInfo(ngdpb::Message);
  void PrintGenInfo(ngdpb::Message);

  Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);


  inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id) { return gdpbId * fuNrOfGet4PerGdpb + get4Id; }
};

#endif /* CBMDEVICEUNPACKTOFSTAR2018_H_ */
