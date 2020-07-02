/**
 * CbmDeviceMcbmEventSink.cxx
 *
 * @since 2020-05-24
 * @author P.-A. Loizeau
 */

#include "CbmDeviceMcbmEventSink.h"


/// CBM headers
#include "CbmMQDefs.h"

#include "CbmEvent.h"
#include "TimesliceMetaData.h"
#include "CbmFlesCanvasTools.h"

/// FAIRROOT headers
#include "FairRunOnline.h"
#include "FairRootManager.h"
#include "FairRootFileSink.h"
#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig
#include "FairParGenericSet.h"
#include "RootSerializer.h"
#include "BoostSerializer.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "TNamed.h"
#include "TList.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include <boost/serialization/utility.hpp>
#include <boost/archive/binary_iarchive.hpp>

/// C/C++ headers
#include <string>
#include <iomanip>
#include <array>
#include <thread> // this_thread::sleep_for

#include <stdexcept>
struct InitTaskError : std::runtime_error { using std::runtime_error::runtime_error; };

using namespace std;

//Bool_t bMcbm2018MonitorTaskT0ResetHistos = kFALSE;

CbmDeviceMcbmEventSink::CbmDeviceMcbmEventSink()
{
}

void CbmDeviceMcbmEventSink::InitTask()
try
{
   /// Read options from executable
   LOG(info) << "Init options for CbmDeviceMcbmEventSink.";

   fsOutputFileName          = fConfig->GetValue< std::string >( "OutFileName" );

   fsChannelNameDataInput    = fConfig->GetValue< std::string >( "EvtNameIn" );
   fsAllowedChannels[ 0 ] = fsChannelNameDataInput;

   fbFillHistos              = fConfig->GetValue< bool >( "FillHistos" );
   fsChannelNameHistosInput  = fConfig->GetValue< std::string >( "ChNameIn" );
   fsChannelNameHistosConfig = fConfig->GetValue< std::string >( "ChNameHistCfg" );
   fsChannelNameCanvasConfig = fConfig->GetValue< std::string >( "ChNameCanvCfg" );
   fuPublishFreqTs           = fConfig->GetValue< uint32_t >( "PubFreqTs" );
   fdMinPublishTime          = fConfig->GetValue< double_t >( "PubTimeMin" );
   fdMaxPublishTime          = fConfig->GetValue< double_t >( "PubTimeMax" );

   /// Associate the MissedTs Channel to the corresponding handler
   OnData( fsChannelNameMissedTs, &CbmDeviceMcbmEventSink::HandleMissTsData );

   /// Associate the command Channel to the corresponding handler
   OnData( fsChannelNameCommands, &CbmDeviceMcbmEventSink::HandleCommand );

   /// Associate the Event + Unp data Channel to the corresponding handler
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
   for( auto const &entry : fChannels )
   {
      LOG(info) << "Channel name: " << entry.first;
      if( std::string::npos != entry.first.find( fsChannelNameDataInput ) )
      {
         if( !IsChannelNameAllowed( entry.first ) )
            throw InitTaskError( "Channel name does not match." );
         OnData( entry.first, &CbmDeviceMcbmEventSink::HandleData );
      } // if( entry.first.find( "ts" )
   } // for( auto const &entry : fChannels )

//   InitContainers();

   /// Create input vectors
   fvDigiT0   = new std::vector< CbmTofDigi >();
   fvDigiSts  = new std::vector< CbmStsDigi >();
   fvDigiMuch = new std::vector< CbmMuchBeamTimeDigi >();
   fvDigiTrd  = new std::vector< CbmTrdDigi >();
   fvDigiTof  = new std::vector< CbmTofDigi >();
   fvDigiRich = new std::vector< CbmRichDigi >();
   fvDigiPsd  = new std::vector< CbmPsdDigi >();

   /// Prepare storage TClonesArrays
      /// TS MetaData storage
   fTimeSliceMetaDataArray = new TClonesArray("TimesliceMetaData", 1);
   if( NULL == fTimeSliceMetaDataArray )
   {
      throw InitTaskError( "Failed creating the TS meta data TClonesarray " );
   } // if( NULL == fTimeSliceMetaDataArray )
      /// Events storage
   /// TODO: remove TObject from CbmEvent and switch to vectors!
   fEventsArray = new TClonesArray( "CbmEvent", 500 );
   if( NULL == fEventsArray )
   {
      throw InitTaskError( "Failed creating the Events TClonesarray " );
   } // if( NULL == fEventsArray )

   /// Prepare root output
   if( "" != fsOutputFileName )
   {
      fpRun = new FairRunOnline();
      fpFairRootMgr = FairRootManager::Instance();
      fpFairRootMgr->SetSink( new FairRootFileSink( fsOutputFileName ) );
      if( nullptr == fpFairRootMgr->GetOutFile() )
      {
         throw InitTaskError( "Could not open root file" );
      } // if( nullptr == fpFairRootMgr->GetOutFile() )
   } // if( "" != fsOutputFileName )
      else
      {
         throw InitTaskError( "Empty output filename!" );
      } // else of if( "" != fsOutputFileName )

   LOG(info) << "Init Root Output to " << fsOutputFileName;

   fpFairRootMgr->InitSink();
//      fEvtHeader = new FairEventHeader();
//      fEvtHeader->SetRunId(iRunId);
//      rootMgr->Register("EventHeader.", "Event", fEvtHeader, kTRUE);
//      rootMgr->FillEventHeader(fEvtHeader);

   /// Register all input data members with the FairRoot manager
      /// TS MetaData
   fpFairRootMgr->Register( "TimesliceMetaData", "TS Meta Data", fTimeSliceMetaDataArray, kTRUE);
      /// Digis storage
   fpFairRootMgr->RegisterAny( "T0Digi",           fvDigiT0,   kTRUE);
   fpFairRootMgr->RegisterAny( "StsDigi",          fvDigiSts,  kTRUE);
   fpFairRootMgr->RegisterAny( "MuchBeamTimeDigi", fvDigiMuch, kTRUE);
   fpFairRootMgr->RegisterAny( "TrdDigi",          fvDigiTrd,  kTRUE);
   fpFairRootMgr->RegisterAny( "TofDigi",          fvDigiTof,  kTRUE);
   fpFairRootMgr->RegisterAny( "RichDigi",         fvDigiRich, kTRUE);
   fpFairRootMgr->RegisterAny( "PsdDigi",          fvDigiPsd,  kTRUE);
      /// CbmEvent
   fpFairRootMgr->Register( "CbmEvent", "Cbm Event", fEventsArray, kTRUE);
/*
   TTree* outTree =new TTree(FairRootManager::GetTreeName(), "/cbmout", 99);
   LOG(info) << "define Tree " << outTree->GetName();

   fpFairRootMgr->GetSink()->SetOutTree(outTree);
*/
   fpFairRootMgr->WriteFolder();

   LOG(info) << "Initialized outTree with rootMgr at " << fpFairRootMgr;

   /// Histograms management
   if( kTRUE == fbFillHistos )
   {
/*
         /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
      std::vector< std::pair< TNamed *, std::string > > vHistos = fpAlgo->GetHistoVector();
         /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
      std::vector< std::pair< TCanvas *, std::string > > vCanvases = fpAlgo->GetCanvasVector();

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
            throw InitTaskError( "Problem sending histo config" );
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
            throw InitTaskError( "Problem sending canvas config" );
         } // if( Send( messageCan, fsChannelNameCanvasConfig ) < 0 )

         LOG(info) << "Config string of Canvas  " << psCanvConfig.first.data()
                   << " is " << psCanvConfig.second.data() ;
      } //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )
*/
   } // if( kTRUE == fbFillHistos )

} catch (InitTaskError& e) {
   LOG(error) << e.what();
   // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
   cbm::mq::ChangeState( this, cbm::mq::Transition::ErrorFound );
}

bool CbmDeviceMcbmEventSink::IsChannelNameAllowed(std::string channelName)
{
   for( auto const &entry : fsAllowedChannels ) {
      std::size_t pos1 = channelName.find(entry);
      if( pos1 != std::string::npos ) {
         const vector< std::string >::const_iterator pos =
            std::find( fsAllowedChannels.begin(), fsAllowedChannels.end(), entry );
         const vector< std::string >::size_type idx = pos - fsAllowedChannels.begin();
         LOG(info) << "Found " << entry << " in " << channelName;
         LOG(info) << "Channel name " << channelName
                 << " found in list of allowed channel names at position " << idx;
         return true;
      } // if (pos1!=std::string::npos)
   } // for(auto const &entry : fsAllowedChannels)
   LOG(info) << "Channel name " << channelName
            << " not found in list of allowed channel names.";
   LOG(error) << "Stop device.";
   return false;
}
/*
Bool_t CbmDeviceMcbmEventSink::InitContainers()
{
   LOG(info) << "Init parameter containers for CbmDeviceMcbmEventSink.";

   if( kFALSE == InitParameters( fpAlgo ->GetParList() ) )
      return kFALSE;

   /// Need to add accessors for all options
   fpAlgo ->SetIgnoreOverlapMs( fbIgnoreOverlapMs );

   Bool_t initOK = fpAlgo->InitContainers();

//   Bool_t initOK = fMonitorAlgo->ReInitContainers();

  return initOK;
}

Bool_t CbmDeviceMcbmEventSink::InitParameters( TList* fParCList )
{
   for( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ )
   {
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

      if( Send(req, "parameters") > 0 )
      {
         if( Receive( rep, "parameters" ) >= 0)
         {
            if( 0 !=  rep->GetSize() )
            {
               CbmMQTMessage tmsg( rep->GetData(), rep->GetSize() );
               newObj = static_cast< FairParGenericSet* >( tmsg.ReadObject( tmsg.GetClass() ) );
               LOG( info ) << "Received unpack parameter from the server:";
               newObj->print();
            } // if( 0 !=  rep->GetSize() )
               else
               {
                  LOG( error ) << "Received empty reply. Parameter not available";
                  return kFALSE;
               } // else of if( 0 !=  rep->GetSize() )
         } // if( Receive( rep, "parameters" ) >= 0)
      } // if( Send(req, "parameters") > 0 )
      fParCList->AddAt( newObj, iparC );
      delete tempObj;
   } // for( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ )

   return kTRUE;
}
*/
//--------------------------------------------------------------------//
// handler is called whenever a message arrives on fsChannelNameMissedTs, with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceMcbmEventSink::HandleMissTsData( FairMQMessagePtr& msg, int /*index*/ )
{
   std::vector< uint64_t > vIndices;
   std::string msgStrMissTs( static_cast< char * >( msg->GetData() ),
                          msg->GetSize() );
   std::istringstream issMissTs( msgStrMissTs );
   boost::archive::binary_iarchive inputArchiveMissTs( issMissTs );
   inputArchiveMissTs >> vIndices;

   fvulMissedTsIndices.insert( fvulMissedTsIndices.end(), vIndices.begin(), vIndices.end() );

   /// Check TS queue and process it if needed (in case it filled a hole!)
   CheckTsQueues();

   return true;
}
//--------------------------------------------------------------------//
// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceMcbmEventSink::HandleData(FairMQParts& parts, int /*index*/)
{
   fulNumMessages++;
   LOG(debug) << "Received message number "<<  fulNumMessages << " with "
	           << parts.Size() << " parts" << ", size0: " << parts.At(0)->GetSize();

   if( 0 == fulNumMessages % 10000 )
      LOG(info) << "Received " << fulNumMessages << " messages";

   /// Extract unpacked data from input message
   uint32_t uPartIdx = 0;
      /// TS metadata
      /// TODO: code order of vectors in the TS MetaData!!
/*
  std::string msgStrTsMeta( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                            ( parts.At( uPartIdx ) )->GetSize() );
  std::istringstream issTsMeta(msgStrTsMeta);
  boost::archive::binary_iarchive inputArchiveTsMeta(issTsMeta);
  inputArchiveTsMeta >> (*fTsMetaData);
  ++uPartIdx;
*/
   Deserialize< RootSerializer >( *parts.At( uPartIdx ), fTsMetaData );
   LOG(debug) << "TS metadata extracted";

   /// FIXME: Need to check if TS arrived in order (probably not!!!) + buffer!!!
   if( fuPrevTsIndex + 1 == fTsMetaData->GetIndex() ||
       ( 0 == fuPrevTsIndex && 0 == fulTsCounter && 0 == fTsMetaData->GetIndex() ) )
   {
      LOG(debug) << "TS direct to dump";
      /// Fill all storage variables registers for data output
      PrepareTreeEntry( parts );
      /// Trigger FairRoot manager to dump Tree entry
      DumpTreeEntry();
      /// Update counters
      fuPrevTsIndex = fTsMetaData->GetIndex();
      fulTsCounter++;
   } // if( fuPrevTsIndex + 1 == fTsMetaData->GetIndex() ||  ( 0 == fuPrevTsIndex && 0 == fulTsCounter ) )
      else
      {
         LOG(debug) << "TS direct to storage";
         /// If not consecutive to last TS sent,
         fmFullTsStorage.emplace_hint( fmFullTsStorage.end(),
                                       std::pair< uint64_t, CbmUnpackedTimeslice >( fTsMetaData->GetIndex(),
                                                                           std::move( CbmUnpackedTimeslice( parts ) ) ) );
      } // else of if( fuPrevTsIndex + 1 == fTsMetaData->GetIndex() ||  ( 0 == fuPrevTsIndex && 0 == fulTsCounter && 0 == fTsMetaData->GetIndex() )
   LOG(debug) << "TS metadata checked";

   /// Clear metadata => crashes, maybe not needed as due to move the pointer is invalidated?
//   delete fTsMetaData;

   /// Check TS queue and process it if needed (in case it filled a hole!)
   CheckTsQueues();
   LOG(debug) << "TS queues checked";

   /// Histograms management
   if( kTRUE == fbFillHistos )
   {
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
   } // if( kTRUE == fbFillHistos )

   return true;
}
//--------------------------------------------------------------------//
bool CbmDeviceMcbmEventSink::HandleCommand( FairMQMessagePtr& msg, int /*index*/ )
{
/*
   std::string sCommand( static_cast< char * >( msg->GetData() ),
                          msg->GetSize() );
*/
   std::string sCommand;
   std::string msgStrCmd( static_cast< char * >( msg->GetData() ),
                          msg->GetSize() );
   std::istringstream issCmd( msgStrCmd );
   boost::archive::binary_iarchive inputArchiveCmd( issCmd );
   inputArchiveCmd >> sCommand;

   std::string sCmdTag = sCommand;
   size_t charPosDel = sCommand.find( ' ' );
   if( std::string::npos != charPosDel )
   {
      sCmdTag = sCommand.substr( 0, charPosDel );
   } // if( std::string::npos != charPosDel )

   if( "EOF" == sCmdTag )
   {
      fbReceivedEof = true;

      /// Extract the last TS index and global full TS count
      if( std::string::npos == charPosDel )
      {
         LOG( fatal ) << "CbmDeviceMcbmEventSink::HandleCommand => "
                     << "Incomplete EOF command received: "
                     << sCommand;
         return false;
      } // if( std::string::npos == charPosDel )
         /// Last TS index
      charPosDel++;
      std::string sNext = sCommand.substr( charPosDel );
      charPosDel = sNext.find( ' ' );

      if( std::string::npos == charPosDel )
      {
         LOG( fatal ) << "CbmDeviceMcbmEventSink::HandleCommand => "
                     << "Incomplete EOF command received: "
                     << sCommand;
         return false;
      } // if( std::string::npos == charPosDel )
      fuLastTsIndex = std::stoul( sNext.substr( 0, charPosDel ) );
         /// Total TS count
      charPosDel++;
      fuTotalTsCount = std::stoul( sNext.substr( charPosDel ) );

      LOG( info ) << "CbmDeviceMcbmEventSink::HandleCommand => "
                   << "Received EOF command with final TS index "
                   << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
      /// End of data: clean save of data + close file + send last state of histos if enabled
      if( fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount )
      {
         LOG( info ) << "CbmDeviceMcbmEventSink::HandleCommand => "
                      << "Found final TS index "
                      << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
         Finish();
      } // if( fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount )
   } // if( "EOF" == sCmdTag )
   else if( "STOP" == sCmdTag )
   {
      /// TODO: different treatment in case of "BAD" ending compared to EOF?
      /// Source failure: clean save of received data + close file + send last state of histos if enabled
      Finish();
   } // else if( "STOP" == sCmdTag )
      else
      {
         LOG(warning) << "Unknown command received: " << sCmdTag << " => will be ignored!";
      } // else if command not recognized

   return true;
}
//--------------------------------------------------------------------//
void CbmDeviceMcbmEventSink::CheckTsQueues()
{
   bool bHoleFoundInBothQueues = false;

   std::map< uint64_t, CbmUnpackedTimeslice >::iterator itFullTs = fmFullTsStorage.begin();
   std::vector< uint64_t >::iterator           itMissTs = fvulMissedTsIndices.begin();

   while( !bHoleFoundInBothQueues )
   {
      /// Check if the first TS in the full TS queue is the next one
      if( fmFullTsStorage.end() != itFullTs && fuPrevTsIndex + 1 == (*itFullTs).first )
      {
         /// Fill all storage variables registers for data output
         PrepareTreeEntry( (*itFullTs).second );
         /// Trigger FairRoot manager to dump Tree entry
         DumpTreeEntry();

         /// Update counters
         fuPrevTsIndex = (*itFullTs).first;
         fulTsCounter++;

         /// Increment iterator
         ++itFullTs;
         continue;
      } // if( fmFullTsStorage.end() != itFullTs && fuPrevTsIndex + 1 == (*itFullTs).first() )
      /// Check if the first TS in the missed TS queue is the next one
      if( fvulMissedTsIndices.end() != itMissTs && fuPrevTsIndex + 1 == (*itMissTs ) )
      {
         /// Prepare entry with only dummy TS metadata and empty storage variables
         new( (*fTimeSliceMetaDataArray)[ fTimeSliceMetaDataArray->GetEntriesFast()
                                          ] ) TimesliceMetaData( 0, 0, 0, (*itMissTs ) ) ;

         /// Trigger FairRoot manager to dump Tree entry
         DumpTreeEntry();

         /// Update counters
         fuPrevTsIndex = (*itMissTs);
         fulMissedTsCounter++;

         /// Increment iterator
         ++itMissTs;
         continue;
      } // if( fvulMissedTsIndices.end() != itMissTs && fuPrevTsIndex + 1 == (*itMissTs ) )

      /// Should be reached only if both queues at the end or hole found in both
      bHoleFoundInBothQueues = true;
   } // while( !bHoleFoundInBothQueues )

   /// Delete the processed entries
   fmFullTsStorage.erase(     fmFullTsStorage.begin(),     itFullTs );
   fvulMissedTsIndices.erase( fvulMissedTsIndices.begin(), itMissTs );

   /// End of data: clean save of data + close file + send last state of histos if enabled
   if( fbReceivedEof && fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount )
   {
      LOG( info ) << "CbmDeviceMcbmEventSink::CheckTsQueues => "
                   << "Found final TS index "
                   << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
      Finish();
   } // if( fbReceivedEof && fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount )
}
//--------------------------------------------------------------------//
void CbmDeviceMcbmEventSink::PrepareTreeEntry( CbmUnpackedTimeslice unpTs )
{
   /// FIXME: poor man solution with lots of data copy until we undertsnad how to properly deal
   /// with FairMq messages ownership and memory managment

   /// FIXME: Not sure if this is the proper way to insert the data
   new( (*fTimeSliceMetaDataArray)[ fTimeSliceMetaDataArray->GetEntriesFast()
                                    ] ) TimesliceMetaData( std::move( unpTs.fTsMetaData ) ) ;

/*
   /// Explicit copy version: safe but slow
      /// T0
   fvDigiT0->insert( fvDigiT0->end(), unpTs.fvDigiT0.begin(), unpTs.fvDigiT0.end() );
      /// STS
   fvDigiSts->insert( fvDigiSts->end(), unpTs.fvDigiSts.begin(), unpTs.fvDigiSts.end() );
      /// MUCH
   fvDigiMuch->insert( fvDigiMuch->end(), unpTs.fvDigiMuch.begin(), unpTs.fvDigiMuch.end() );
      /// TRD
   fvDigiTrd->insert( fvDigiTrd->end(), unpTs.fvDigiTrd.begin(), unpTs.fvDigiTrd.end() );
      /// T0F
   fvDigiTof->insert( fvDigiTof->end(), unpTs.fvDigiTof.begin(), unpTs.fvDigiTof.end() );
      /// RICH
   fvDigiRich->insert( fvDigiRich->end(), unpTs.fvDigiRich.begin(), unpTs.fvDigiRich.end() );
      /// PSD
   fvDigiPsd->insert( fvDigiPsd->end(), unpTs.fvDigiPsd.begin(), unpTs.fvDigiPsd.end() );
*/
   /// move version: safe but slow
      /// T0
   (*fvDigiT0) = std::move( unpTs.fvDigiT0 );
      /// STS
   (*fvDigiSts) = std::move( unpTs.fvDigiSts );
      /// MUCH
   (*fvDigiMuch) = std::move( unpTs.fvDigiMuch );
      /// TRD
   (*fvDigiTrd) = std::move( unpTs.fvDigiTrd );
      /// T0F
   (*fvDigiTof) = std::move( unpTs.fvDigiTof );
      /// RICH
   (*fvDigiRich) = std::move( unpTs.fvDigiRich );
      /// PSD
   (*fvDigiPsd) = std::move( unpTs.fvDigiPsd );

   /// Extract CbmEvent TClonesArray from input message
   fEventsArray->AbsorbObjects( &( unpTs.fEventsArray ) );
}
void CbmDeviceMcbmEventSink::DumpTreeEntry()
{
   // Unpacked digis + CbmEvent output to root file
/*
 * NH style
//      fpFairRootMgr->FillEventHeader(fEvtHeader);
//      LOG(info) << "Fill WriteOutBuffer with FairRootManager at " << fpFairRootMgr;
//      fpOutRootFile->cd();
      fpFairRootMgr->Fill();
      fpFairRootMgr->StoreWriteoutBufferData( fpFairRootMgr->GetEventTime() );
      //fpFairRootMgr->StoreAllWriteoutBufferData();
      fpFairRootMgr->DeleteOldWriteoutBufferData();
*/
   /// FairRunOnline style
   fpFairRootMgr->StoreWriteoutBufferData( fpFairRootMgr->GetEventTime() );
   fpFairRootMgr->Fill();
   fpFairRootMgr->DeleteOldWriteoutBufferData();

   /// Clear metadata array
   fTimeSliceMetaDataArray->Clear();

   /// Clear vectors
   fvDigiT0->clear();
   fvDigiSts->clear();
   fvDigiMuch->clear();
   fvDigiTrd->clear();
   fvDigiTof->clear();
   fvDigiRich->clear();
   fvDigiPsd->clear();

   /// Clear event array
//   fEventsArray->Delete();
   fEventsArray->Clear( "C" );
//   fEventsArray->Clear();
}

//--------------------------------------------------------------------//
bool CbmDeviceMcbmEventSink::SendHistograms()
{
   /// Serialize the array of histos into a single MQ message
   FairMQMessagePtr message( NewMessage() );
   Serialize<RootSerializer>( *message, &fArrayHisto );

   /// Send message to the common histogram messages queue
   if( Send( message, fsChannelNameHistosInput ) < 0 )
   {
      LOG(error) << "Problem sending data";
      return false;
   } // if( Send( message, fsChannelNameHistosInput ) < 0 )

   /// Reset the histograms after sending them (but do not reset the time)
//   fpAlgo->ResetHistograms( kFALSE );

   return true;
}

//--------------------------------------------------------------------//
CbmDeviceMcbmEventSink::~CbmDeviceMcbmEventSink()
{
   /// FIXME: Add pointers check before delete

   /// Close things properly if not alredy done
   if( !fbFinishDone )
      Finish();

   /// Clear metadata
   fTimeSliceMetaDataArray->Clear();
   delete fTimeSliceMetaDataArray;
   delete fTsMetaData;

   /// Clear vectors
   fvDigiT0->clear();
   fvDigiSts->clear();
   fvDigiMuch->clear();
   fvDigiTrd->clear();
   fvDigiTof->clear();
   fvDigiRich->clear();
   fvDigiPsd->clear();

   /// Clear events TClonesArray
   fEventsArray->Clear();
   delete fEventsArray;

   delete fpRun;
}

void CbmDeviceMcbmEventSink::Finish()
{
   // Clean closure of output to root file
   fpFairRootMgr->Write();
//   fpFairRootMgr->GetSource()->Close();
   fpFairRootMgr->CloseSink();
   LOG(info) << "File closed after saving "
             << ( fulTsCounter + fulMissedTsCounter ) << " TS ("
             << fulTsCounter << " full ones and "
             << fulMissedTsCounter << " missed/empty ones)";

   if( kTRUE == fbFillHistos )
   {
      SendHistograms();
      fLastPublishTime = std::chrono::system_clock::now();
   } // if( kTRUE == fbFillHistos )

   ChangeState( fair::mq::Transition::Stop );
   std::this_thread::sleep_for( std::chrono::milliseconds( 3000 ) );
   ChangeState( fair::mq::Transition::End );

   fbFinishDone = kTRUE;
}

CbmUnpackedTimeslice::CbmUnpackedTimeslice( FairMQParts & parts ) :
   fEventsArray( "CbmEvent", 500 )
{
   /// Extract unpacked data from input message
   uint32_t uPartIdx = 0;
      /// TS metadata
      /// TODO: code order of vectors in the TS MetaData!!
/*
  std::string msgStrTsMeta( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                            ( parts.At( uPartIdx ) )->GetSize() );
  std::istringstream issTsMeta(msgStrTsMeta);
  boost::archive::binary_iarchive inputArchiveTsMeta(issTsMeta);
  inputArchiveTsMeta >> (*fTsMetaData);
  ++uPartIdx;
*/
   TObject* tempObjectMeta = nullptr;
   RootSerializer().Deserialize( *parts.At( uPartIdx ), tempObjectMeta );
   ++uPartIdx;

   if( TString( tempObjectMeta->ClassName() ).EqualTo( "TimesliceMetaData") )
   {
      fTsMetaData = *( static_cast< TimesliceMetaData * >( tempObjectMeta ) );
   } // if( TString( tempObject->ClassName() ).EqualTo( "TClonesArray") )

      /// T0
   std::string msgStrT0( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                          ( parts.At( uPartIdx ) )->GetSize() );
   std::istringstream issT0( msgStrT0 );
   boost::archive::binary_iarchive inputArchiveT0( issT0 );
   inputArchiveT0 >> fvDigiT0;
   ++uPartIdx;

      /// STS
   std::string msgStrSts( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                          ( parts.At( uPartIdx ) )->GetSize() );
   std::istringstream issSts( msgStrSts );
   boost::archive::binary_iarchive inputArchiveSts( issSts );
   inputArchiveSts >> fvDigiSts;
   ++uPartIdx;

      /// MUCH
   std::string msgStrMuch( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                          ( parts.At( uPartIdx ) )->GetSize() );
   std::istringstream issMuch( msgStrMuch );
   boost::archive::binary_iarchive inputArchiveMuch( issMuch );
   inputArchiveMuch >> fvDigiMuch;
   ++uPartIdx;

      /// TRD
   std::string msgStrTrd( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                          ( parts.At( uPartIdx ) )->GetSize() );
   std::istringstream issTrd( msgStrTrd );
   boost::archive::binary_iarchive inputArchiveTrd( issTrd );
   inputArchiveTrd >> fvDigiTrd;
   ++uPartIdx;

      /// T0F
   std::string msgStrTof( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                          ( parts.At( uPartIdx ) )->GetSize() );
   std::istringstream issTof( msgStrTof );
   boost::archive::binary_iarchive inputArchiveTof( issTof );
   inputArchiveTof >> fvDigiTof;
   ++uPartIdx;

      /// RICH
   std::string msgStrRich( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                           ( parts.At( uPartIdx ) )->GetSize() );
   std::istringstream issRich( msgStrRich );
   boost::archive::binary_iarchive inputArchiveRich( issRich );
   inputArchiveRich >> fvDigiRich;
   ++uPartIdx;

      /// PSD
   std::string msgStrPsd( static_cast< char * >( parts.At( uPartIdx )->GetData() ),
                          ( parts.At( uPartIdx ) )->GetSize() );
   std::istringstream issPsd( msgStrPsd );
   boost::archive::binary_iarchive inputArchivePsd( issPsd );
   inputArchivePsd >> fvDigiPsd;
   ++uPartIdx;

   /// Extract CbmEvent TClonesArray from input message
   TObject* tempObject = nullptr;
   RootSerializer().Deserialize( *parts.At( uPartIdx ), tempObject);
   ++uPartIdx;

   if( TString( tempObject->ClassName() ).EqualTo( "TClonesArray") )
   {
      TClonesArray* arrayEventsIn = static_cast< TClonesArray * >( tempObject );

      /// Copy data in registered TClonesArray (by taking ownership!)
      fEventsArray.AbsorbObjects( arrayEventsIn );
   } // if( TString( tempObject->ClassName() ).EqualTo( "TClonesArray") )
}
