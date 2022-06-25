/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau[committer], Dominik Smith */

#include "CbmDevTrigger.h"

/// CBM headers
#include "CbmDigiTimeslice.h"
#include "CbmMQDefs.h"

/// FAIRROOT headers
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"

#include "BoostSerializer.h"

#include "RootSerializer.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "TimesliceMetaData.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

/// C/C++ headers
#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

CbmDevTrigger::CbmDevTrigger() {}

void CbmDevTrigger::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDevTrigger.";

  // Trigger algorithm params
  const std::string sTriggerDet = fConfig->GetValue<std::string>("TriggerDet");
  fTriggerWindow                = fConfig->GetValue<double>("TriggerWin");
  fMinNumDigis                  = fConfig->GetValue<int32_t>("TriggerMinDigis");
  fDeadTime                     = fConfig->GetValue<double>("TriggerDeadTime");

  fChannelNameDataInput  = fConfig->GetValue<std::string>("TsNameIn");
  fChannelNameDataOutput = fConfig->GetValue<std::string>("TriggerNameOut");

  OnData(fChannelNameDataInput, &CbmDevTrigger::HandleData);

  /// Extract refdet
  fTriggerDet = GetDetectorId(sTriggerDet);

  if (ECbmModuleId::kNotExist == fTriggerDet) { throw InitTaskError("CbmDevTrigger: Trigger detector not set."); }
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

ECbmModuleId CbmDevTrigger::GetDetectorId(std::string detName)
{
  /// FIXME: Disable clang formatting for now as it corrupts all alignment
  /* clang-format off */
  ECbmModuleId detId = ("T0"   == detName ? ECbmModuleId::kT0
                       : ("Sts"  == detName ? ECbmModuleId::kSts
                       : ("Much" == detName ? ECbmModuleId::kMuch
                       : ("Trd"  == detName ? ECbmModuleId::kTrd
                       : ("Tof"  == detName ? ECbmModuleId::kTof
                       : ("Rich" == detName ? ECbmModuleId::kRich
                       : ("Psd"  == detName ? ECbmModuleId::kPsd
                                             : ECbmModuleId::kNotExist)))))));
  return detId; 
  /// FIXME: Re-enable clang formatting after formatted lines
  /* clang-format on */
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDevTrigger::HandleData(FairMQParts& parts, int /*index*/)
{
  fNumMessages++;
  LOG(info) << "Received message number " << fNumMessages << " with " << parts.Size() << " parts"
            << ", size0: " << parts.At(0)->GetSize();

  if (0 == fNumMessages % 10000) LOG(info) << "Received " << fNumMessages << " messages";

  /// Extract unpacked data from input message
  CbmDigiTimeslice ts;

  std::string msgStrTS(static_cast<char*>(parts.At(0)->GetData()), (parts.At(0))->GetSize());
  std::istringstream issTS(msgStrTS);
  boost::archive::binary_iarchive inputArchiveTS(issTS);
  inputArchiveTS >> ts;

  LOG(debug) << "T0 Vector size: " << ts.fData.fT0.fDigis.size();
  LOG(debug) << "STS Vector size: " << ts.fData.fSts.fDigis.size();
  LOG(debug) << "MUCH Vector size: " << ts.fData.fMuch.fDigis.size();
  LOG(debug) << "TRD Vector size: " << ts.fData.fTrd.fDigis.size();
  LOG(debug) << "TOF Vector size: " << ts.fData.fTof.fDigis.size();
  LOG(debug) << "RICH Vector size: " << ts.fData.fRich.fDigis.size();
  LOG(debug) << "PSD Vector size: " << ts.fData.fPsd.fDigis.size();

  const std::vector<double> triggers = GetTriggerTimes(ts);
  LOG(debug) << "triggers: " << triggers.size();

  /// Send output message
  if (!SendTriggers(triggers, parts)) { return false; }

  return true;
}

std::vector<double> CbmDevTrigger::GetTriggerTimes(const CbmDigiTimeslice& ts)
{
  std::vector<double> vDigiTimes;
  switch (fTriggerDet) {
    case ECbmModuleId::kMuch: {
      vDigiTimes = GetDigiTimes(ts.fData.fMuch.fDigis);
      break;
    }
    case ECbmModuleId::kSts: {
      vDigiTimes = GetDigiTimes(ts.fData.fSts.fDigis);
      break;
    }
    case ECbmModuleId::kTof: {
      vDigiTimes = GetDigiTimes(ts.fData.fTof.fDigis);
      break;
    }
    case ECbmModuleId::kTrd: {
      vDigiTimes = GetDigiTimes(ts.fData.fTrd.fDigis);
      break;
    }
    case ECbmModuleId::kRich: {
      vDigiTimes = GetDigiTimes(ts.fData.fRich.fDigis);
      break;
    }
    case ECbmModuleId::kPsd: {
      vDigiTimes = GetDigiTimes(ts.fData.fPsd.fDigis);
      break;
    }
    case ECbmModuleId::kT0: {
      vDigiTimes = GetDigiTimes(ts.fData.fT0.fDigis);
      break;
    }
    default: LOG(fatal) << "CbmDevTrigger::GetTriggerTimes(): Reading digis from unknown detector type!";
  }
  LOG(debug) << "CbmDevTrigger::GetTriggerTimes(): Building triggers from " << vDigiTimes.size() << " digis.";
  return fTriggerAlgo(vDigiTimes, fTriggerWindow, fMinNumDigis, fDeadTime);
}

bool CbmDevTrigger::SendTriggers(const std::vector<double>& vTriggers, FairMQParts& partsIn)
{
  LOG(debug) << "Vector size: " << vTriggers.size();

  FairMQParts partsOut;
  partsOut.AddPart(std::move(partsIn.At(0)));  // DigiTimeslice
  partsOut.AddPart(std::move(partsIn.At(1)));  // TsMetaData

  // Prepare trigger vector.
  std::stringstream ossTrig;
  boost::archive::binary_oarchive oaTrig(ossTrig);
  oaTrig << vTriggers;
  std::string* strMsgTrig = new std::string(ossTrig.str());

  partsOut.AddPart(NewMessage(
    const_cast<char*>(strMsgTrig->c_str()),  // data
    strMsgTrig->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgTrig));  // object that manages the data

  if (Send(partsOut, fChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fChannelNameDataOutput;
    return false;
  }
  return true;
}
