/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceMcbmUnpack.cxx
 *
 * @since 2020-05-04
 * @author P.-A. Loizeau
 */

#include "CbmDeviceMcbmUnpack.h"

#include "CbmFlesCanvasTools.h"
#include "CbmMQDefs.h"
#include "CbmMcbm2018UnpackerAlgoMuch.h"
#include "CbmMcbm2018UnpackerAlgoPsd.h"
#include "CbmMcbm2018UnpackerAlgoRich.h"
#include "CbmMcbm2018UnpackerAlgoSts.h"
#include "CbmMcbm2018UnpackerAlgoTof.h"
#include "CbmMcbm2018UnpackerAlgoTrdR.h"

#include "StorableTimeslice.hpp"
#include "TimesliceMetaData.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"

#include "BoostSerializer.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "RootSerializer.h"
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

Bool_t bMcbm2018MonitorTaskT0ResetHistos = kFALSE;

CbmDeviceMcbmUnpack::CbmDeviceMcbmUnpack()
{
  fUnpAlgoSts  = new CbmMcbm2018UnpackerAlgoSts();
  fUnpAlgoMuch = new CbmMcbm2018UnpackerAlgoMuch();
  fUnpAlgoTrd  = new CbmMcbm2018UnpackerAlgoTrdR();
  fUnpAlgoTof  = new CbmMcbm2018UnpackerAlgoTof();
  fUnpAlgoRich = new CbmMcbm2018UnpackerAlgoRich();
  fUnpAlgoPsd  = new CbmMcbm2018UnpackerAlgoPsd();
}

void CbmDeviceMcbmUnpack::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDeviceMcbmUnpack.";
  fbIgnoreOverlapMs       = fConfig->GetValue<bool>("IgnOverMs");
  fvsSetTimeOffs          = fConfig->GetValue<std::vector<std::string>>("SetTrigWin");
  fsChannelNameDataInput  = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameDataOutput = fConfig->GetValue<std::string>("TsNameOut");
  /// TODO: option to set fuDigiMaskedIdT0 !!!!
  fsAllowedChannels[0] = fsChannelNameDataInput;

  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  //logger::SetLogLevel("INFO");

  int noChannel = fChannels.size();
  LOG(info) << "Number of defined channels: " << noChannel;
  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (std::string::npos != entry.first.find(fsChannelNameDataInput)) {
      if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
      OnData(entry.first, &CbmDeviceMcbmUnpack::HandleData);
    }  // if( entry.first.find( "ts" )
  }    // for( auto const &entry : fChannels )
  InitContainers();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceMcbmUnpack::IsChannelNameAllowed(std::string channelName)
{
  for (auto const& entry : fsAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const vector<std::string>::const_iterator pos =
        std::find(fsAllowedChannels.begin(), fsAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos - fsAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName << " found in list of allowed channel names at position " << idx;
      return true;
    }  // if (pos1!=std::string::npos)
  }    // for(auto const &entry : fsAllowedChannels)
  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceMcbmUnpack::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmDeviceMcbmUnpack.";

  if (kFALSE == InitParameters(fUnpAlgoSts->GetParList())) return kFALSE;
  if (kFALSE == InitParameters(fUnpAlgoMuch->GetParList())) return kFALSE;
  if (kFALSE == InitParameters(fUnpAlgoTrd->GetParList())) return kFALSE;
  if (kFALSE == InitParameters(fUnpAlgoTof->GetParList())) return kFALSE;
  if (kFALSE == InitParameters(fUnpAlgoRich->GetParList())) return kFALSE;
  if (kFALSE == InitParameters(fUnpAlgoPsd->GetParList())) return kFALSE;

  /// Need to add accessors for all options
  fUnpAlgoSts->SetIgnoreOverlapMs(fbIgnoreOverlapMs);
  fUnpAlgoMuch->SetIgnoreOverlapMs(fbIgnoreOverlapMs);
  fUnpAlgoTrd->SetIgnoreOverlapMs(fbIgnoreOverlapMs);
  fUnpAlgoTof->SetIgnoreOverlapMs(fbIgnoreOverlapMs);
  fUnpAlgoRich->SetIgnoreOverlapMs(fbIgnoreOverlapMs);
  fUnpAlgoPsd->SetIgnoreOverlapMs(fbIgnoreOverlapMs);

  /// Load time offsets
  for (std::vector<std::string>::iterator itStrOffs = fvsSetTimeOffs.begin(); itStrOffs != fvsSetTimeOffs.end();
       ++itStrOffs) {
    size_t charPosDel = (*itStrOffs).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info) << "CbmDeviceMcbmUnpack::InitContainers => "
                << "Trying to set trigger window with invalid option pattern, ignored! "
                << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found " << (*itStrOffs) << " )";
    }  // if( std::string::npos == charPosDel )

    /// Detector Enum Tag
    std::string sSelDet = (*itStrOffs).substr(0, charPosDel);
    /// Min number
    charPosDel++;
    Double_t dOffset = std::stod((*itStrOffs).substr(charPosDel));

    if ("kSTS" == sSelDet) { fUnpAlgoSts->SetTimeOffsetNs(dOffset); }  // if( "kSTS"  == sSelDet )
    else if ("kMUCH" == sSelDet) {
      fUnpAlgoMuch->SetTimeOffsetNs(dOffset);
    }  // else if( "kMUCH" == sSelDet )
    else if ("kTRD" == sSelDet) {
      fUnpAlgoTrd->SetTimeOffsetNs(dOffset);
    }  // else if( "kTRD"  == sSelDet  )
    else if ("kTOF" == sSelDet) {
      fUnpAlgoTof->SetTimeOffsetNs(dOffset);
    }  // else if( "kTOF"  == sSelDet )
    else if ("kRICH" == sSelDet) {
      fUnpAlgoRich->SetTimeOffsetNs(dOffset);
    }  // else if( "kRICH" == sSelDet )
    else if ("kPSD" == sSelDet) {
      fUnpAlgoPsd->SetTimeOffsetNs(dOffset);
    }  // else if( "kPSD"  == sSelDet )
    else {
      LOG(info) << "CbmDeviceMcbmUnpack::InitContainers => Trying to set time "
                   "offset for unsupported detector, ignored! "
                << (sSelDet);
      continue;
    }  // else of detector enum detection
  }  // for( std::vector< std::string >::iterator itStrAdd = fvsAddDet.begin(); itStrAdd != fvsAddDet.end(); ++itStrAdd )


  /// Starting from first run on Tuesday 28/04/2020, STS uses bin sorter FW
  fUnpAlgoSts->SetBinningFwFlag(kTRUE);
  /// Starting from first run on Monday 04/05/2020, MUCH uses bin sorter FW
  fUnpAlgoMuch->SetBinningFwFlag(kTRUE);

  Bool_t initOK = fUnpAlgoSts->InitContainers();
  initOK &= fUnpAlgoMuch->InitContainers();
  initOK &= fUnpAlgoTrd->InitContainers();
  initOK &= fUnpAlgoTof->InitContainers();
  initOK &= fUnpAlgoRich->InitContainers();
  initOK &= fUnpAlgoPsd->InitContainers();

  /// Special case for TRD vector initialization
  /// Get address of vector from algo in a kind of loopback ^^'
  initOK &= fUnpAlgoTrd->SetDigiOutputPointer(&(fUnpAlgoTrd->GetVector()));

  //   Bool_t initOK = fMonitorAlgo->ReInitContainers();

  return initOK;
}

Bool_t CbmDeviceMcbmUnpack::InitParameters(TList* fParCList)
{
  for (int iparC = 0; iparC < fParCList->GetEntries(); iparC++) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);
    std::string paramName {tempObj->GetName()};
    // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
    // Should only be used for small data because of the cost of an additional copy

    // Her must come the proper Runid
    std::string message = paramName + ",111";
    LOG(info) << "Requesting parameter container " << paramName << ", sending message: " << message;

    FairMQMessagePtr req(NewSimpleMessage(message));
    FairMQMessagePtr rep(NewMessage());

    FairParGenericSet* newObj = nullptr;

    if (Send(req, "parameters") > 0) {
      if (Receive(rep, "parameters") >= 0) {
        if (0 != rep->GetSize()) {
          CbmMqTMessage tmsg(rep->GetData(), rep->GetSize());
          newObj = static_cast<FairParGenericSet*>(tmsg.ReadObject(tmsg.GetClass()));
          LOG(info) << "Received unpack parameter from the server:";
          newObj->print();
        }  // if( 0 !=  rep->GetSize() )
        else {
          LOG(error) << "Received empty reply. Parameter not available";
          return kFALSE;
        }  // else of if( 0 !=  rep->GetSize() )
      }    // if( Receive( rep, "parameters" ) >= 0)
    }      // if( Send(req, "parameters") > 0 )
    fParCList->AddAt(newObj, iparC);
    delete tempObj;
  }  // for( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ )

  return kTRUE;
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceMcbmUnpack::HandleData(FairMQMessagePtr& msg, int /*index*/)
{
  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with size " << msg->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  /// Create an empty TS and fill it with the incoming message
  fles::StorableTimeslice ts {0};
  inputArchive >> ts;

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsOverSizeInNs = fdMsSizeInNs * (fuNbOverMsPerTs);
    fdTsFullSizeInNs = fdTsCoreSizeInNs + fdTsOverSizeInNs;
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";
  }  // if( -1.0 == fdTsCoreSizeInNs )

  fTsMetaData = new TimesliceMetaData(ts.descriptor(0, 0).idx, fdTsCoreSizeInNs, fdTsOverSizeInNs, ts.index());

  /// Process the Timeslice
  DoUnpack(ts, 0);

  /// Send digi vectors to ouput
  if (!SendUnpData()) return false;

  delete fTsMetaData;

  /// Clear the digis vectors in case it was filled
  fUnpAlgoSts->ClearVector();
  fUnpAlgoMuch->ClearVector();
  fUnpAlgoTrd->ClearVector();
  fUnpAlgoTof->ClearVector();
  fUnpAlgoRich->ClearVector();
  fUnpAlgoPsd->ClearVector();

  /// Clear the digis vectors in case it was filled
  fUnpAlgoSts->ClearErrorVector();
  fUnpAlgoMuch->ClearErrorVector();
  fUnpAlgoTrd->ClearErrorVector();
  fUnpAlgoTof->ClearErrorVector();
  fUnpAlgoRich->ClearErrorVector();
  fUnpAlgoPsd->ClearErrorVector();

  return true;
}

bool CbmDeviceMcbmUnpack::SendUnpData()
{

  /// Prepare serialized versions of the TS Meta
  /*
   std::stringstream ossTsMeta;
   boost::archive::binary_oarchive oaTsMeta(ossTsMeta);
   oaTsMeta << *(fTsMetaData);
   std::string* strMsgTsMetaE = new std::string(ossTsMeta.str());
*/
  FairMQMessagePtr messTsMeta(NewMessage());
  //  Serialize<RootSerializer>(*messTsMeta, fTsMetaData);
  RootSerializer().Serialize(*messTsMeta, fTsMetaData);

  std::stringstream ossSts;
  boost::archive::binary_oarchive oaSts(ossSts);
  oaSts << fUnpAlgoSts->GetVector();
  std::string* strMsgSts = new std::string(ossSts.str());

  std::stringstream ossMuch;
  boost::archive::binary_oarchive oaMuch(ossMuch);
  oaMuch << fUnpAlgoMuch->GetVector();
  std::string* strMsgMuch = new std::string(ossMuch.str());

  std::stringstream ossTrd;
  boost::archive::binary_oarchive oaTrd(ossTrd);
  oaTrd << fUnpAlgoTrd->GetVector();
  std::string* strMsgTrd = new std::string(ossTrd.str());

  /// Split TOF vector in TOF and T0
  std::vector<CbmTofDigi>& vDigiTofT0 = fUnpAlgoTof->GetVector();
  std::vector<CbmTofDigi> vDigiTof    = {};
  std::vector<CbmTofDigi> vDigiT0     = {};

  for (auto digi : vDigiTofT0) {
    if (fuDigiMaskedIdT0 == (digi.GetAddress() & fuDigiMaskId)) {
      /// Insert data in T0 output container
      vDigiT0.emplace_back(digi);
    }  // if( fuDigiMaskedIdT0 == ( digi.GetAddress() & fuDigiMaskId ) )
    else {
      /// Insert data in TOF output container
      vDigiTof.emplace_back(digi);
    }  // else of if( fuDigiMaskedIdT0 == ( digi.GetAddress() & fuDigiMaskId ) )
  }    // for( auto digi: vDigi )

  std::stringstream ossTof;
  boost::archive::binary_oarchive oaTof(ossTof);
  oaTof << vDigiTof;
  std::string* strMsgTof = new std::string(ossTof.str());

  std::stringstream ossT0;
  boost::archive::binary_oarchive oaT0(ossT0);
  oaT0 << vDigiT0;
  std::string* strMsgT0 = new std::string(ossT0.str());

  std::stringstream ossRich;
  boost::archive::binary_oarchive oaRich(ossRich);
  oaRich << fUnpAlgoRich->GetVector();
  std::string* strMsgRich = new std::string(ossRich.str());

  std::stringstream ossPsd;
  boost::archive::binary_oarchive oaPsd(ossPsd);
  oaPsd << fUnpAlgoPsd->GetVector();
  std::string* strMsgPsd = new std::string(ossPsd.str());

  FairMQParts parts;

  parts.AddPart(std::move(messTsMeta));
  /*
   parts.AddPart( NewMessage( const_cast< char * >( strMsgTsMetaE->c_str() ), // data
                              strMsgTsMetaE->length(), // size
                              []( void* , void* object ){ delete static_cast< std::string * >( object ); },
                              strMsgTsMetaE
                            )
                ); // object that manages the data
*/

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgT0->c_str()),  // data
    strMsgT0->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgT0));  // object that manages the data

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgSts->c_str()),  // data
    strMsgSts->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgSts));  // object that manages the data

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgMuch->c_str()),  // data
    strMsgMuch->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgMuch));  // object that manages the data

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgTrd->c_str()),  // data
    strMsgTrd->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgTrd));  // object that manages the data

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgTof->c_str()),  // data
    strMsgTof->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgTof));  // object that manages the data

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgRich->c_str()),  // data
    strMsgRich->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgRich));  // object that manages the data

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgPsd->c_str()),  // data
    strMsgPsd->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgPsd));  // object that manages the data

  if (Send(parts, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }

  return true;
}


CbmDeviceMcbmUnpack::~CbmDeviceMcbmUnpack()
{
  if (nullptr != fUnpAlgoSts) delete fUnpAlgoSts;
  if (nullptr != fUnpAlgoMuch) delete fUnpAlgoMuch;
  if (nullptr != fUnpAlgoTrd) delete fUnpAlgoTrd;
  if (nullptr != fUnpAlgoTof) delete fUnpAlgoTof;
  if (nullptr != fUnpAlgoRich) delete fUnpAlgoRich;
  if (nullptr != fUnpAlgoPsd) delete fUnpAlgoPsd;
}


Bool_t CbmDeviceMcbmUnpack::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  fulTsCounter++;

  if (kFALSE == fbComponentsAddedToList) {
    for (uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx) {
      switch (ts.descriptor(uCompIdx, 0).sys_id) {
        case kusSysIdSts: {
          fUnpAlgoSts->AddMsComponentToList(uCompIdx, kusSysIdSts);
          break;
        }  // case kusSysIdSts
        case kusSysIdMuch: {
          fUnpAlgoMuch->AddMsComponentToList(uCompIdx, kusSysIdMuch);
          break;
        }  // case kusSysIdMuch
        case kusSysIdTrd: {
          fUnpAlgoTrd->AddMsComponentToList(uCompIdx, kusSysIdTrd);
          break;
        }  // case kusSysIdTrd
        case kusSysIdTof: {
          fUnpAlgoTof->AddMsComponentToList(uCompIdx, kusSysIdTof);
          break;
        }  // case kusSysIdTof
        case kusSysIdT0: {
          fUnpAlgoTof->AddMsComponentToList(uCompIdx, kusSysIdT0);
          break;
        }  // case kusSysIdT0
        case kusSysIdRich: {
          fUnpAlgoRich->AddMsComponentToList(uCompIdx, kusSysIdRich);
          break;
        }  // case kusSysIdRich
        case kusSysIdPsd: {
          fUnpAlgoPsd->AddMsComponentToList(uCompIdx, kusSysIdPsd);
          break;
        }  // case kusSysIdPsd
        default: break;
      }  // switch( ts.descriptor( uCompIdx, 0 ).sys_id )
    }    // for( uint32_t uComp = 0; uComp < ts.num_components(); ++uComp )
    fbComponentsAddedToList = kTRUE;
  }  // if( kFALSE == fbComponentsAddedToList )

  if (kFALSE == fUnpAlgoSts->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in STS unpacker algorithm class";
    return kFALSE;
  }  // if( kFALSE == fUnpAlgoSts->ProcessTs( ts ) )

  if (kFALSE == fUnpAlgoMuch->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in MUCH unpacker algorithm class";
    return kFALSE;
  }  // if( kFALSE == fUnpAlgoMuch->ProcessTs( ts ) )

  if (kFALSE == fUnpAlgoTrd->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in TRD unpacker algorithm class";
    return kFALSE;
  }  // if( kFALSE == fUnpAlgoTrd->ProcessTs( ts ) )

  if (kFALSE == fUnpAlgoTof->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in TOF unpacker algorithm class";
    return kFALSE;
  }  // if( kFALSE == fUnpAlgoTof->ProcessTs( ts ) )

  if (kFALSE == fUnpAlgoRich->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in RICH unpacker algorithm class";
    return kFALSE;
  }  // if( kFALSE == fUnpAlgoRich->ProcessTs( ts ) )

  if (kFALSE == fUnpAlgoPsd->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in PSD unpacker algorithm class";
    return kFALSE;
  }  // if( kFALSE == fUnpAlgoPsd->ProcessTs( ts ) )


  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << " time slices";

  return kTRUE;
}

void CbmDeviceMcbmUnpack::Finish() {}
