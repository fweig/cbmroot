/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau */

/**
 * CbmDeviceMcbmMonitorPulser.cxx
 *
 * @since 2020-05-04
 * @author P.-A Loizeau
 */

#include "CbmDeviceMcbmMonitorPulser.h"

#include "CbmMQDefs.h"

#include "TimesliceMetaData.h"

//#include "CbmMcbm2018MonitorAlgoTof.h"
#include "CbmFlesCanvasTools.h"

#include "StorableTimeslice.hpp"

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

//Bool_t bMcbm2018MonitorTaskTofResetHistos = kFALSE;

CbmDeviceMcbmMonitorPulser::CbmDeviceMcbmMonitorPulser()
//   : fMonitorAlgo{ new CbmMcbm2018MonitorAlgoTof() }
{
}

void CbmDeviceMcbmMonitorPulser::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmMqStarHistoServer.";

  fbDebugMonitorMode = fConfig->GetValue<bool>("DebugMoni");
  fuHistoryHistoSize = fConfig->GetValue<uint32_t>("HistEvoSz");
  fuMinTotPulser     = fConfig->GetValue<uint32_t>("PulsTotMin");
  fuMaxTotPulser     = fConfig->GetValue<uint32_t>("PulsTotMax");

  fuPublishFreqTs           = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime          = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime          = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameDataInput    = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameHistosInput  = fConfig->GetValue<std::string>("ChNameIn");
  fsChannelNameHistosConfig = fConfig->GetValue<std::string>("ChNameHistCfg");
  fsChannelNameCanvasConfig = fConfig->GetValue<std::string>("ChNameCanvCfg");
  fsAllowedChannels[0]      = fsChannelNameDataInput;

  LOG(info) << "Histograms publication frequency in TS:    " << fuPublishFreqTs;
  LOG(info) << "Histograms publication min. interval in s: " << fdMinPublishTime;
  LOG(info) << "Histograms publication max. interval in s: " << fdMaxPublishTime;

  /// Set the Monitor Algo in Absolute time scale
  //   fMonitorAlgo->UseAbsoluteTime();

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
      OnData(entry.first, &CbmDeviceMcbmMonitorPulser::HandleData);
    }  // if( std::string::npos != entry.first.find( fsChannelNameDataInput ) )
  }    // for( auto const &entry : fChannels )
  InitContainers();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceMcbmMonitorPulser::IsChannelNameAllowed(std::string channelName)
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

Bool_t CbmDeviceMcbmMonitorPulser::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmDeviceMcbmMonitorPulser.";
  Bool_t initOK = kTRUE;
  /*
   fParCList = fMonitorAlgo->GetParList();

   for( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ ) {
      FairParGenericSet* tempObj = (FairParGenericSet*)( fParCList->At( iparC ) );
      fParCList->Remove( tempObj );
      std::string paramName{ tempObj->GetName() };
      // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
      // Should only be used for small data because of the cost of an additional copy

      // Her must come the proper Runid
      std::string message = paramName + ",111";
      LOG(info) << "Requesting parameter container " << paramName << ", sending message: " << message;

      FairMQMessagePtr req( NewSimpleMessage(message) );
      FairMQMessagePtr rep( NewMessage() );

      FairParGenericSet* newObj = nullptr;

      if ( Send(req, "parameters") > 0 ) {
         if ( Receive( rep, "parameters" ) >= 0) {
            if ( rep->GetSize() != 0 ) {
               CbmMQTMessage tmsg( rep->GetData(), rep->GetSize() );
               newObj = static_cast< FairParGenericSet* >( tmsg.ReadObject( tmsg.GetClass() ) );
               LOG( info ) << "Received unpack parameter from the server:";
               newObj->print();
            } else {
               LOG( error ) << "Received empty reply. Parameter not available";
            } // if (rep->GetSize() != 0)
         } // if (Receive(rep, "parameters") >= 0)
      } // if (Send(req, "parameters") > 0)
      fParCList->AddAt( newObj, iparC );
      delete tempObj;
   } // for ( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ )

   /// Need to add accessors for all options
   fMonitorAlgo->SetIgnoreOverlapMs( fbIgnoreOverlapMs );
   fMonitorAlgo->SetDebugMonitorMode( fbDebugMonitorMode );
   fMonitorAlgo->SetIgnoreCriticalErrors( fbIgnoreCriticalErrors );
   fMonitorAlgo->SetHistoryHistoSize( fuHistoryHistoSize );
   fMonitorAlgo->SetPulserTotLimits( fuMinTotPulser, fuMaxTotPulser );

   Bool_t initOK = fMonitorAlgo->InitContainers();
*/
  //   Bool_t initOK = fMonitorAlgo->ReInitContainers();

  //   CreateHistos();
  /*
   /// Histos creation and obtain pointer on them
      /// Trigger histo creation on all associated algos
   initOK &= fMonitorAlgo->CreateHistograms();

      /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
   std::vector< std::pair< TNamed *, std::string > > vHistos = fMonitorAlgo->GetHistoVector();
      /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
   std::vector< std::pair< TCanvas *, std::string > > vCanvases = fMonitorAlgo->GetCanvasVector();

   /// Add pointers to each histo in the histo array
   /// Create histo config vector
   /// ===> Use an std::vector< std::pair< std::string, std::string > > with < Histo name, Folder >
   ///      and send it through a separate channel using the BoostSerializer
   for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )
   {
//         LOG(info) << "Registering  " << vHistos[ uHisto ].first->GetName()
//                   << " in " << vHistos[ uHisto ].second.data()
//                   ;
      fArrayHisto.Add( vHistos[ uHisto ].first );
      std::pair< std::string, std::string > psHistoConfig( vHistos[ uHisto ].first->GetName(),
                                                           vHistos[ uHisto ].second );
      fvpsHistosFolder.push_back( psHistoConfig );

      /// Serialize the vector of histo config into a single MQ message
      FairMQMessagePtr messageHist( NewMessage() );
      Serialize< BoostSerializer < std::pair< std::string, std::string > > >( *messageHist, psHistoConfig );

      /// Send message to the common histogram config messages queue
      if( Send( messageHist, fsChannelNameHistosConfig ) < 0 )
      {
         LOG(error) << "Problem sending histo config";
         return false;
      } // if( Send( messageHist, fsChannelNameHistosConfig ) < 0 )

      LOG(info) << "Config of hist  " << psHistoConfig.first.data()
                << " in folder " << psHistoConfig.second.data() ;
   } // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

   /// Create canvas config vector
   /// ===> Use an std::vector< std::pair< std::string, std::string > > with < Canvas name, config >
   ///      and send it through a separate channel using the BoostSerializer
   for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )
   {
//         LOG(info) << "Registering  " << vCanvases[ uCanv ].first->GetName()
//                   << " in " << vCanvases[ uCanv ].second.data();
      std::string sCanvName = (vCanvases[ uCanv ].first)->GetName();
      std::string sCanvConf = GenerateCanvasConfigString( vCanvases[ uCanv ].first );

      std::pair< std::string, std::string > psCanvConfig( sCanvName, sCanvConf );

      fvpsCanvasConfig.push_back( psCanvConfig );

      /// Serialize the vector of canvas config into a single MQ message
      FairMQMessagePtr messageCan( NewMessage() );
      Serialize< BoostSerializer < std::pair< std::string, std::string > > >( *messageCan, psCanvConfig );

      /// Send message to the common canvas config messages queue
      if( Send( messageCan, fsChannelNameCanvasConfig ) < 0 )
      {
         LOG(error) << "Problem sending canvas config";
         return false;
      } // if( Send( messageCan, fsChannelNameCanvasConfig ) < 0 )

      LOG(info) << "Config string of Canvas  " << psCanvConfig.first.data()
                << " is " << psCanvConfig.second.data() ;
   } //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )
*/
  return initOK;
}


// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceMcbmMonitorPulser::HandleData(FairMQParts& parts, int /*index*/)
{
  fulNumMessages++;

  LOG(debug) << "Received message " << fulNumMessages << " with " << parts.Size() << " parts"
             << ", size0: " << parts.At(0)->GetSize();

  uint32_t uPartIdx = 0;

  /// TODO: code order of vectors in the TS MetaData!!
  /*
  std::string msgStrTsMeta( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                            ( parts.At( uPartIdx ) )->GetSize() );
  std::istringstream issTsMeta(msgStrTsMeta);
  boost::archive::binary_iarchive inputArchiveTsMeta(issTsMeta);
  inputArchiveTsMeta >> (*fTsMetaData);
  ++uPartIdx;
*/
  Deserialize<RootSerializer>(*parts.At(uPartIdx), fTsMetaData);
  ++uPartIdx;

  std::string msgStrT0(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issT0(msgStrT0);
  boost::archive::binary_iarchive inputArchiveT0(issT0);
  inputArchiveT0 >> fvDigiT0;
  ++uPartIdx;

  std::string msgStrSts(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issSts(msgStrSts);
  boost::archive::binary_iarchive inputArchiveSts(issSts);
  inputArchiveSts >> fvDigiSts;
  ++uPartIdx;

  std::string msgStrMuch(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issMuch(msgStrMuch);
  boost::archive::binary_iarchive inputArchiveMuch(issMuch);
  inputArchiveMuch >> fvDigiMuch;
  ++uPartIdx;

  std::string msgStrTrd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTrd(msgStrTrd);
  boost::archive::binary_iarchive inputArchiveTrd(issTrd);
  inputArchiveTrd >> fvDigiTrd;
  ++uPartIdx;

  std::string msgStrTof(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTof(msgStrTof);
  boost::archive::binary_iarchive inputArchiveTof(issTof);
  inputArchiveTof >> fvDigiTof;
  ++uPartIdx;

  std::string msgStrRich(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issRich(msgStrRich);
  boost::archive::binary_iarchive inputArchiveRich(issRich);
  inputArchiveRich >> fvDigiRich;
  ++uPartIdx;

  std::string msgStrPsd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issPsd(msgStrPsd);
  boost::archive::binary_iarchive inputArchivePsd(issPsd);
  inputArchivePsd >> fvDigiPsd;
  ++uPartIdx;

  /// Process data in Algo

  /// Clear vectors
  delete fTsMetaData;
  fvDigiT0.clear();
  fvDigiSts.clear();
  fvDigiMuch.clear();
  fvDigiTrd.clear();
  fvDigiTof.clear();
  fvDigiRich.clear();
  fvDigiPsd.clear();

  /*
   LOG(debug) << "Received message number "<<  fulNumMessages
              << " with size " << msg->GetSize();

   if( 0 == fulNumMessages % 10000 )
      LOG(info) << "Received " << fulNumMessages << " messages";

   std::string msgStr( static_cast<char*>( msg->GetData() ), msg->GetSize() );
   std::istringstream iss( msgStr );
   boost::archive::binary_iarchive inputArchive( iss );

   /// Create an empty TS and fill it with the incoming message
   fles::StorableTimeslice component{ 0 };
   inputArchive >> component;

   /// Process the Timeslice
   DoUnpack(component, 0);

   /// Send histograms each 100 time slices. Should be each ~1s
   /// Use also runtime checker to trigger sending after M s if
   /// processing too slow or delay sending if processing too fast
   std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
   std::chrono::duration<double_t> elapsedSeconds = currentTime - fLastPublishTime;
   if( ( fdMaxPublishTime < elapsedSeconds.count() ) ||
       ( 0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count() ) )
   {
      SendHistograms();
      fLastPublishTime = std::chrono::system_clock::now();
   } // if( ( fdMaxPublishTime < elapsedSeconds.count() ) || ( 0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count() ) )
*/
  return true;
}

bool CbmDeviceMcbmMonitorPulser::SendHistograms()
{
  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr message(NewMessage());
  Serialize<RootSerializer>(*message, &fArrayHisto);

  // test code to check if deserialization works
  /*
  TObject* tempObject = nullptr;
  Deserialize<RootDeserializer>(*message, tempObject);

  if (TString(tempObject->ClassName()).EqualTo("TObjArray")) {
   TObjArray* arrayHisto = static_cast<TObjArray*>(tempObject);
   LOG(info) << "Array contains " << arrayHisto->GetEntriesFast()
             << " entries";
    for (Int_t i = 0; i < arrayHisto->GetEntriesFast(); i++) {
      TObject* obj = arrayHisto->At(i);
      LOG(info) << obj->GetName();
      TH1* histogram = static_cast<TH1*>(obj);
      LOG(info) << histogram->GetNbinsX();
    }
  }
*/

  /// Send message to the common histogram messages queue
  if (Send(message, fsChannelNameHistosInput) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }  // if( Send( message, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  //   fMonitorAlgo->ResetHistograms( kFALSE );

  return true;
}


CbmDeviceMcbmMonitorPulser::~CbmDeviceMcbmMonitorPulser() {}

void CbmDeviceMcbmMonitorPulser::Finish() {}
