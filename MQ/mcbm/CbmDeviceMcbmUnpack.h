/** @file CbmDeviceMcbmUnpack.h
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

/**
 * CbmDeviceMcbmUnpack.h
 *
 * @since 2020-05-04
 * @author P.-A. Loizeau
 */

#ifndef CBMDEVICEMCBMUNPACK_H_
#define CBMDEVICEMCBMUNPACK_H_

#include "Timeslice.hpp"

#include "FairMQDevice.h"

#include "Rtypes.h"
#include "TMessage.h"
#include "TObjArray.h"

#include <map>
#include <vector>

class TList;
class CbmMcbm2018UnpackerAlgoSts;
class CbmMcbm2018UnpackerAlgoMuch;
class CbmMcbm2018UnpackerAlgoTrdR;
class CbmMcbm2018UnpackerAlgoTof;
class CbmMcbm2018UnpackerAlgoRich;
class CbmMcbm2018UnpackerAlgoPsd;
class TimesliceMetaData;

class CbmDeviceMcbmUnpack : public FairMQDevice {
public:
  CbmDeviceMcbmUnpack();
  virtual ~CbmDeviceMcbmUnpack();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);
  bool HandleCommand(FairMQMessagePtr&, int);

private:
  /// Constants
  static const uint16_t kusSysIdSts  = 0x10;
  static const uint16_t kusSysIdMuch = 0x50;
  static const uint16_t kusSysIdTrd  = 0x40;
  static const uint16_t kusSysIdTof  = 0x60;
  static const uint16_t kusSysIdT0   = 0x90;
  static const uint16_t kusSysIdRich = 0x30;
  static const uint16_t kusSysIdPsd  = 0x80;

  /// Control flags
  Bool_t fbIgnoreOverlapMs       = false;  //! Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice
  Bool_t fbComponentsAddedToList = kFALSE;

  /// User settings parameters
  std::string fsChannelNameDataInput  = "fullts";
  std::string fsChannelNameDataOutput = "unpts_0";
  std::string fsChannelNameCommands   = "commands";
  UInt_t fuDigiMaskedIdT0             = 0x00005006;
  UInt_t fuDigiMaskId                 = 0x0001FFFF;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Parameters management
  //      TList* fParCList = nullptr;
  Bool_t InitParameters(TList* fParCList);

  /// Statistics & first TS rejection
  uint64_t fulNumMessages = 0;
  uint64_t fulTsCounter   = 0;

  /// Processing algos
  CbmMcbm2018UnpackerAlgoSts* fUnpAlgoSts   = nullptr;
  CbmMcbm2018UnpackerAlgoMuch* fUnpAlgoMuch = nullptr;
  CbmMcbm2018UnpackerAlgoTrdR* fUnpAlgoTrd  = nullptr;
  CbmMcbm2018UnpackerAlgoTof* fUnpAlgoTof   = nullptr;
  CbmMcbm2018UnpackerAlgoRich* fUnpAlgoRich = nullptr;
  CbmMcbm2018UnpackerAlgoPsd* fUnpAlgoPsd   = nullptr;

  /// Time offsets
  std::vector<std::string> fvsSetTimeOffs = {};

  /// TS MetaData storage
  size_t fuNbCoreMsPerTs    = 0;        //!
  size_t fuNbOverMsPerTs    = 0;        //!
  Double_t fdMsSizeInNs     = 1280000;  //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs = -1.0;     //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsOverSizeInNs = -1.0;     //! Total size of the overlap MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs = -1.0;     //! Total size of all MS in a TS, [nanoseconds]
  TimesliceMetaData* fTsMetaData;

  bool IsChannelNameAllowed(std::string channelName);
  Bool_t InitContainers();
  Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  void Finish();
  bool SendUnpData();
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage {
public:
  CbmMQTMessage(void* buf, Int_t len) : TMessage(buf, len) { ResetBit(kIsOwner); }
};


#endif /* CBMDEVICEMCBMUNPACK_H_ */
