/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Dominik Smith [committer] */

/**
 * CbmDevUnpack.cxx
 *
 * @since 2020-05-04
 * @author P.-A. Loizeau
 */

#include "CbmDevUnpack.h"

#include "CbmDigiTimeslice.h"
#include "CbmMQDefs.h"

#include "StorableTimeslice.hpp"
#include "TimesliceMetaData.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"

#include "TStopwatch.h"

#include "BoostSerializer.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>

#include "RootSerializer.h"
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;
using cbm::algo::UnpackStsElinkPar;
using cbm::algo::UnpackStsPar;

CbmDevUnpack::CbmDevUnpack() {}

void CbmDevUnpack::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDevUnpack.";
  fsChannelNameDataInput  = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameDataOutput = fConfig->GetValue<std::string>("TsNameOut");
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

Bool_t CbmDevUnpack::InitAlgos()
{
  /// Event header object
  fCbmTsEventHeader = new CbmTsEventHeader();

  // --- Common parameters for all components
  uint32_t numChansPerAsic   = 128;  // R/O channels per ASIC
  uint32_t numAsicsPerModule = 16;   // Number of ASICs per module

  // Create one algorithm per component and configure it with parameters
  auto equipIds = fStsConfig.GetEquipmentIds();
  for (auto& equip : equipIds) {
    std::unique_ptr<UnpackStsPar> par(new UnpackStsPar());
    par->fNumChansPerAsic   = numChansPerAsic;
    par->fNumAsicsPerModule = numAsicsPerModule;
    const size_t numElinks  = fStsConfig.GetNumElinks(equip);
    for (size_t elink = 0; elink < numElinks; elink++) {
      UnpackStsElinkPar elinkPar;
      auto mapEntry        = fStsConfig.Map(equip, elink);
      elinkPar.fAddress    = mapEntry.first;   // Module address for this elink
      elinkPar.fAsicNr     = mapEntry.second;  // ASIC number within module
      elinkPar.fTimeOffset = 0.;
      elinkPar.fAdcOffset  = 0.;
      elinkPar.fAdcGain    = 0.;
      // TODO: Add parameters for time and ADC calibration
      par->fElinkParams.push_back(elinkPar);
    }
    fAlgoSts[equip].SetParams(std::move(par));
    LOG(info) << "--- Configured equipment " << equip << " with " << numElinks << " elinks";
  }  //# equipments

  LOG(info) << "--- Configured " << fAlgoSts.size() << " unpacker algorithms for STS.";
  LOG(debug) << "Readout map:" << fStsConfig.PrintReadoutMap();
  LOG(info) << "==================================================";
  std::cout << std::endl;

  return true;
}


// Method called by run loop and requesting new data from the TS source whenever
bool CbmDevUnpack::ConditionalRun()
{
  /// First request a new TS (full one)
  std::string message = "full";
  LOG(debug) << "Requesting new TS by sending message: full" << message;
  FairMQMessagePtr req(NewSimpleMessage(message));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, fsChannelNameDataInput) <= 0) {
    LOG(error) << "Failed to send the request! message was " << message;
    return false;
  }
  else if (Receive(rep, fsChannelNameDataInput) < 0) {
    LOG(error) << "Failed to receive a reply to the request! message was " << message;
    return false;
  }
  else if (rep->GetSize() == 0) {
    LOG(error) << "Received empty reply. Something went wrong with the timeslice generation! message was " << message;
    return false;
  }

  /// Message received, do Algo related Initialization steps if needed
  if (0 == fNumMessages) { InitAlgos(); }

  fNumMessages++;
  LOG(debug) << "Received message number " << fNumMessages << " with size " << rep->GetSize();

  if (0 == fNumMessages % 10000) LOG(info) << "Received " << fNumMessages << " messages";

  std::string msgStr(static_cast<char*>(rep->GetData()), rep->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  /// Create an empty TS and fill it with the incoming message
  fles::StorableTimeslice ts {0};
  inputArchive >> ts;

  /// Extract the TS parameters from header (by definition stable over time)
  const size_t NbCoreMsPerTs  = ts.num_core_microslices();
  const size_t NbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
  const double MsSizeInNs     = (ts.descriptor(0, NbCoreMsPerTs).idx - ts.descriptor(0, 0).idx) / NbCoreMsPerTs;
  const double TsCoreSizeInNs = MsSizeInNs * (NbCoreMsPerTs);
  const double TsOverSizeInNs = MsSizeInNs * (NbOverMsPerTs);
  const double TsFullSizeInNs = TsCoreSizeInNs + TsOverSizeInNs;
  const TimesliceMetaData TsMetaData(ts.start_time(), TsCoreSizeInNs, TsOverSizeInNs, ts.index());

  if (0 == fNumTs) {
    LOG(info) << "Timeslice parameters: each TS has " << NbCoreMsPerTs << " Core MS and " << NbOverMsPerTs
              << " Overlap MS, for a MS duration of " << MsSizeInNs << " ns, a core duration of " << TsCoreSizeInNs
              << " ns and a full duration of " << TsFullSizeInNs << " ns";
  }

  /// Process the timeslice
  CbmDigiTimeslice digiTs = DoUnpack(ts);

  LOG(debug) << "Unpack: Sending TS index " << ts.index();
  /// Send digi vectors to ouput
  if (!SendData(digiTs, TsMetaData)) return false;
  LOG(debug) << "Unpack: Sent TS index " << ts.index();

  // Reset the event header for a new timeslice
  fCbmTsEventHeader->Reset();

  return true;
}

bool CbmDevUnpack::SendData(const CbmDigiTimeslice& timeslice, const TimesliceMetaData& TsMetaData)
{
  FairMQParts parts;

  /// Prepare serialized versions of the TS Event header
  FairMQMessagePtr messTsHeader(NewMessage());
  RootSerializer().Serialize(*messTsHeader, fCbmTsEventHeader);
  parts.AddPart(std::move(messTsHeader));

  /// Prepare serialized version of Digi Timeslice
  std::stringstream ossTS;
  boost::archive::binary_oarchive oaTS(ossTS);
  oaTS << timeslice;

  std::string* strMsgTS = new std::string(ossTS.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgTS->c_str()),  // data
    strMsgTS->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgTS));  // object that manages the data

  /// Prepare serialized versions of the TS Meta
  /// FIXME: only for TS duration and overlap, should be sent to parameter service instead as stable values in run
  ///        Index and start time are already included in the TsHeader object!
  FairMQMessagePtr messTsMeta(NewMessage());
  RootSerializer().Serialize(*messTsMeta, &TsMetaData);
  parts.AddPart(std::move(messTsMeta));

  if (Send(parts, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }

  return true;
}

CbmDevUnpack::~CbmDevUnpack()
{
  if (fCbmTsEventHeader) { delete fCbmTsEventHeader; }
}

CbmDigiTimeslice CbmDevUnpack::DoUnpack(const fles::Timeslice& timeslice)
{
  fNumTs++;

  fCbmTsEventHeader->SetTsIndex(timeslice.index());
  fCbmTsEventHeader->SetTsStartTime(timeslice.start_time());

  // Output digi timeslice
  CbmDigiTimeslice digiTs;

  // --- Timer
  TStopwatch timer;
  TStopwatch compTimer;
  timer.Start();

  // --- Timeslice properties
  const uint64_t tsIndex = timeslice.index();
  const uint64_t tsTime  = timeslice.start_time();
  const uint64_t numComp = timeslice.num_components();
  uint64_t numCompUsed   = 0;

  // --- Counters
  size_t numMs    = 0;
  size_t numBytes = 0;
  size_t numDigis = 0;

  // ---  Component loop
  for (uint64_t comp = 0; comp < numComp; comp++) {

    auto systemId = static_cast<fles::SubsystemIdentifier>(timeslice.descriptor(comp, 0).sys_id);
    if (systemId == fles::SubsystemIdentifier::STS) {
      const uint16_t equipmentId = timeslice.descriptor(comp, 0).eq_id;
      const auto algoIt          = fAlgoSts.find(equipmentId);
      assert(algoIt != fAlgoSts.end());

      // The current algorithm works for the STS data format version 0x20 used in 2021.
      // Other versions are not yet supported.
      // In the future, different data formats will be supported by instantiating different
      // algorithms depending on the version.
      assert(timeslice.descriptor(comp, 0).sys_ver == 0x20);

      // --- Component log
      size_t numBytesInComp = 0;
      size_t numDigisInComp = 0;
      compTimer.Start();

      // --- Microslice loop
      uint64_t numMsInComp = timeslice.num_microslices(comp);
      for (uint64_t mslice = 0; mslice < numMsInComp; mslice++) {
        const auto msDescriptor = timeslice.descriptor(comp, mslice);
        const auto msContent    = timeslice.content(comp, mslice);
        numBytesInComp += msDescriptor.size;
        auto result = (algoIt->second)(msContent, msDescriptor, tsTime);
        LOG(debug1) << "CbmDevUnpack::DoUnpack(): Component " << comp << ", microslice " << mslice << ", digis "
                    << result.first.size() << ", errors " << result.second.fNumNonHitOrTsbMessage << " | "
                    << result.second.fNumErrElinkOutOfRange << " | " << result.second.fNumErrInvalidFirstMessage
                    << " | " << result.second.fNumErrInvalidMsSize << " | " << result.second.fNumErrTimestampOverflow
                    << " | ";
        const auto it = digiTs.fData.fSts.fDigis.end();
        digiTs.fData.fSts.fDigis.insert(it, result.first.begin(), result.first.end());
        numDigisInComp += result.first.size();
      }

      compTimer.Stop();
      LOG(debug) << "CbmDevUnpack::DoUnpack(): Component " << comp << ", microslices " << numMsInComp << " input size "
                 << numBytesInComp << " bytes, "
                 << ", digis " << numDigisInComp << ", CPU time " << compTimer.CpuTime() * 1000. << " ms";
      numCompUsed++;
      numBytes += numBytesInComp;
      numDigis += numDigisInComp;
      numMs += numMsInComp;
    }

  }  //# component

  // --- Add Sts Digis to header
  fCbmTsEventHeader->AddNDigisSts(numDigis);

  // --- Sorting of output digis. Is required by both digi trigger and event builder.
  std::sort(digiTs.fData.fSts.fDigis.begin(), digiTs.fData.fSts.fDigis.end(),
            [](CbmStsDigi digi1, CbmStsDigi digi2) { return digi1.GetTime() < digi2.GetTime(); });

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(15) << left << "CbmDevUnpack::DoUnpackGetName(): [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << " (index " << tsIndex << ")";
  logOut << ", components " << numCompUsed << " / " << numComp << ", microslices " << numMs;
  logOut << ", input rate " << double(numBytes) / timer.RealTime() / 1.e6 << " MB/s";
  logOut << ", digis " << numDigis;
  LOG(debug) << logOut.str();

  if (0 == fNumTs % 10000) LOG(info) << "Processed " << fNumTs << " time slices";

  return digiTs;
}
