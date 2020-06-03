/**
 *  CbmMQTsaMultiSampler.cpp
 *
 * @since 2017-11-17
 * @author F. Uhlig
 */


#include "CbmMQTsaMultiSampler.h"

#include "CbmMQDefs.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig

#include "TimesliceSubscriber.hpp"
#include "TimesliceMultiSubscriber.hpp"
#include "TimesliceInputArchive.hpp"
#include "TimesliceMultiInputArchive.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace filesys = boost::filesystem;

#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <ctime>
#include <thread> // this_thread::sleep_for
#include <chrono>
#include <algorithm>
#include <string>

using namespace std;

#include <stdexcept>

struct InitTaskError : std::runtime_error { using std::runtime_error::runtime_error; };

CbmMQTsaMultiSampler::CbmMQTsaMultiSampler()
    : FairMQDevice()
    , fMaxTimeslices(0)
    , fFileName("")
    , fDirName("")
    , fInputFileList()
    , fFileCounter(0)
    , fHost("")
    , fPort(0)
    , fHighWaterMark(1)
    , fTSNumber(0)
    , fTSCounter(0)
    , fMessageCounter(0)
    , fSource(nullptr)
    , fTime()
{
}

void CbmMQTsaMultiSampler::InitTask()
try
{
    // Get the values from the command line options (via fConfig)
    fFileName = fConfig->GetValue<string>("filename");
    fDirName = fConfig->GetValue<string>("dirname");
    fHost = fConfig->GetValue<string>("flib-host");
    fPort = fConfig->GetValue<uint64_t>("flib-port");
    fHighWaterMark = fConfig->GetValue<uint64_t>("high-water-mark");
    fMaxTimeslices = fConfig->GetValue<uint64_t>("max-timeslices");
    fbNoSplitTs        = fConfig->GetValue<bool>("no-split-ts");
    fbSendTsPerSysId   = fConfig->GetValue<bool>("send-ts-per-sysid");
    fbSendTsPerChannel = fConfig->GetValue<bool>("send-ts-per-channel");

    if( fbNoSplitTs )
    {
      if( fbSendTsPerSysId )
      {
        if( fbSendTsPerChannel )
        {
          LOG(warning) << "Both no-split-ts, send-ts-per-sysid and send-ts-per-channel options used => "
                       << " second and third one will be ignored!!!!";
        } // if( fbSendTsPerSysId )
          else LOG(warning) << "Both no-split-ts and send-ts-per-sysid options used => "
                     << " second one will be ignored!!!!";
      } // if( fbSendTsPerSysId )
      else if( fbSendTsPerChannel )
      {
        LOG(warning) << "Both no-split-ts and send-ts-per-channel options used => "
                     << " second one will be ignored!!!!";
      } // else if( fbSendTsPerSysId ) of if( fbSendTsPerSysId )
    } // if( fbNoSplitTs )
    else if( fbSendTsPerSysId && fbSendTsPerChannel )
    {
        LOG(warning) << "Both send-ts-per-sysid and send-ts-per-channel options used => "
                     << " second one will be ignored!!!!";
    } // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs )

    std::vector< std::string > vSysIdChanPairs =  fConfig->GetValue< std::vector< std::string > >("sysid-chan");
    for( uint32_t uPair = 0; uPair < vSysIdChanPairs.size(); ++uPair )
    {
      const size_t sep = vSysIdChanPairs[ uPair ].find(':');
      if( string::npos == sep || 0 == sep || vSysIdChanPairs[ uPair ].size() == sep )
      {
        LOG(info) << vSysIdChanPairs[ uPair ];
        throw InitTaskError("Provided pair of SysId + Channel name is missing a : or an argument.");
      } // if( string::npos == sep || 0 == sep || vSysIdChanPairs[ uPair ].size() == sep )

      /// Extract SysId
      std::string sSysId =  vSysIdChanPairs[ uPair ].substr( 0, sep );
      const size_t hexPos = sSysId.find("0x");
      int iSysId;
      if( string::npos == hexPos )
        iSysId = std::stoi( sSysId  );
        else iSysId = std::stoi( sSysId.substr( hexPos + 2  ), nullptr, 16 );

      /// Extract Channel name
      std::string sChannelName = vSysIdChanPairs[ uPair ].substr( sep + 1 );

      /// Look if SysId is already defined
      const vector< int >::const_iterator pos = std::find( fSysId.begin(), fSysId.end(), iSysId );
      if( fSysId.end() != pos )
      {
        /// SysId already there, redefine the corresponding channel name
        const vector<std::string>::size_type idx = pos - fSysId.begin();
        fAllowedChannels[ idx ] = sChannelName;
      } // if( fSysId.end() != pos )
        else
        {
          /// SysId unknown yet, add both SysId and channe name at end of respective vectors
          fSysId.push_back( iSysId );
          fAllowedChannels.push_back( sChannelName );
        } // else of if( fSysId.end() != pos )

      LOG(info) << vSysIdChanPairs[ uPair ] << " " << iSysId << " " << sChannelName;
    } // for( uint32_t uPair = 0; uPair < vSysIdChanPairs.size(); ++uPair )

    if (0 == fMaxTimeslices) fMaxTimeslices = UINT_MAX;

    // Check which input is defined
    // Posibilities
    // filename && ! dirname : single file
    // filename with wildcards && diranme : all files with filename regex in the directory
    // host && port : connect to the flim server

    bool isGoodInputCombi{false};
    if ( 0 != fFileName.size() &&  0 == fDirName.size() && 0 == fHost.size() && 0 == fPort ) {
      isGoodInputCombi=true;
      fInputFileList.push_back(fFileName);
    } else if ( 0 != fFileName.size() &&  0 != fDirName.size() && 0 == fHost.size() && 0 == fPort ) {
      isGoodInputCombi=true;
      fInputFileList.push_back(fFileName);
    } else if ( 0 == fFileName.size() &&  0 == fDirName.size() && 0 != fHost.size() && 0 != fPort) {
      isGoodInputCombi=true;
      LOG(info) << "Host: " << fHost;
      LOG(info) << "Port: " << fPort;
    } else if ( 0 == fFileName.size() &&  0 == fDirName.size() && 0 != fHost.size() && 0 == fPort) {
      isGoodInputCombi=true;
      LOG(info) << "Host string: " << fHost;
    } else {
      isGoodInputCombi=false;
    }


    if (!isGoodInputCombi) {
      throw InitTaskError("Wrong combination of inputs. Either file or wildcard file + directory or host + port are allowed combination.");
    }


    LOG(info) << "MaxTimeslices: " << fMaxTimeslices;

    // Get the information about created channels from the device
    // Check if the defined channels from the topology (by name)
    // are in the list of channels which are possible/allowed
    // for the device
    // The idea is to check at initilization if the devices are
    // properly connected. For the time beeing this is done with a
    // nameing convention. It is not avoided that someone sends other
    // data on this channel.
    int noChannel = fChannels.size();
    LOG(info) << "Number of defined output channels: " << noChannel;
    for(auto const &entry : fChannels) {
      LOG(info) << "Channel name: " << entry.first;
      if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
    }

    for(auto const& value: fComponentsToSend) {
      LOG(info) << "Value : " << value;
      if (value > 1) {
        throw InitTaskError("Sending same data to more than one output channel not implemented yet.");
      }
    }


  if ( 0 == fFileName.size() && 0 != fHost.size() && 0 != fPort ) {
    // Don't add the protocol since this is done now in the TimesliceMultiSubscriber
    //std::string connector = "tcp://" + fHost + ":" + std::to_string(fPort);
    std::string connector = fHost + ":" + std::to_string(fPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fSource = new fles::TimesliceMultiSubscriber(connector);
  } else if ( 0 == fFileName.size() && 0 != fHost.size() ) {
    std::string connector = fHost;
    LOG(info) << "Open TSPublisher with host string: " << connector;
    fSource = new fles::TimesliceMultiSubscriber(connector, fHighWaterMark);
  } else {
    // Create a ";" separated string with all file names
    std::string fileList{""};
    for(const auto obj: fInputFileList) {
      std::string fileName = obj;
      fileList += fileName;
      fileList += ";";
    }
    fileList.pop_back(); // Remove the last ;
    LOG(info) << "Input File String: " << fileList;
    fSource = new fles::TimesliceMultiInputArchive(fileList, fDirName);
    if ( !fSource) {
      throw InitTaskError("Could open files from file list.");
    }
  }

  LOG( info ) << "High-Water Mark: " << fHighWaterMark;
  LOG( info ) << "Max. Timeslices: " << fMaxTimeslices;
  if( fbNoSplitTs )
  {
    LOG(info) << "Sending TS copies in no-split mode";
  } // if( fbNoSplitTs )
  else if( fbSendTsPerSysId )
  {
    LOG(info) << "Sending components in separate TS per SysId";
  } // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs
  else if( fbSendTsPerChannel )
  {
    LOG(info) << "Sending components in separate TS per channel";
  } // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs )


  fTime = std::chrono::steady_clock::now();
} catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmMQTsaMultiSampler::IsChannelNameAllowed(std::string channelName)
{
  bool bFoundMatch = false;
//  for(auto const &entry : fAllowedChannels) {
  for( uint32_t idx = 0; idx < fAllowedChannels.size(); ++idx )
  {
    auto const &entry = fAllowedChannels[ idx ];
    LOG(info) << "Looking for name " << channelName << " in " << entry;
    std::size_t pos1 = channelName.find(entry);
    if (pos1!=std::string::npos) {
/*
      const vector<std::string>::const_iterator pos =
         std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos-fAllowedChannels.begin();
*/
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName
              << " found in list of allowed channel names at position " << idx
              << " (SysId 0x" << std::hex << fSysId[ idx ] << std::dec << ")";
      fComponentsToSend[idx]++;
      fChannelsToSend[idx].push_back(channelName);

      /// If sending per channel, do not stop the loop as we allow more than 1 comp type per channel
      if( fbSendTsPerChannel )
        bFoundMatch = true;
        else return true;
    } // if (pos1!=std::string::npos)
  }
  /// If sending per channel, do not stop the loop but still check if at least 1 match found
  if( fbSendTsPerChannel && bFoundMatch )
    return true;

  LOG(info) << "Channel name " << channelName
            << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

bool CbmMQTsaMultiSampler::ConditionalRun()
{


  auto timeslice = fSource->get();
  if (timeslice) {
    if (fTSCounter < fMaxTimeslices) {
      fTSCounter++;
      if (fTSCounter % 10000 == 0)  {
        LOG(info) << "Analyse Event " << fTSCounter;
      }

      const fles::Timeslice& ts = *timeslice;
//      auto tsIndex = ts.index();


      LOG(debug) << "Found " << ts.num_components()
                << " different components in timeslice";


//      CheckTimeslice(ts);

      if( fbNoSplitTs )
      {
        /// This is a special case for the TOF + T0
        /// => Inefficient as copy the TS as many times as need!
        if( !CreateAndSendFullTs( ts ) )
          return false;
      } // if( fbNoSplitTs )
      else if( fbSendTsPerSysId )
      {
        /// This assumes that the order of the components does NOT change after the first TS
        /// That should be the case as the component index correspond to a physical link idx
        if( !CreateAndCombineComponentsPerSysId( ts ) )
          return false;
      } // else if( fbSendTsPerSysId ) of  if( fbNoSplitTs )
      else if( fbSendTsPerChannel )
      {
        /// This assumes that the order of the components does NOT change after the first TS
        /// That should be the case as the component index correspond to a physical link idx
        if( !CreateAndCombineComponentsPerChannel( ts ) )
          return false;
      } // else if( fbSendTsPerChannel ) of  if( fbSendTsPerSysId )
        else
        {
          for (unsigned int nrComp = 0; nrComp < ts.num_components(); ++nrComp) {
            if( !CreateAndSendComponent(ts, nrComp) )
              return false;
          }
        } // else of if( fbSendTsPerSysId )
      return true;
    } else {
      CalcRuntime();
      return false;
    }
  } else {
    CalcRuntime();
    return false;
  }

}

bool CbmMQTsaMultiSampler::CreateAndSendComponent(const fles::Timeslice& ts, int nrComp)
{

  // Check if component has to be send. If the corresponding channel
  // is connected create the new timeslice and send it to the
  // correct channel

  LOG(debug) << "SysID: " << static_cast<int>(ts.descriptor(nrComp,0).sys_id);
  const vector<int>::const_iterator pos =
     std::find(fSysId.begin(), fSysId.end(), static_cast<int>(ts.descriptor(nrComp,0).sys_id));
  if (pos != fSysId.end() ) {
    const vector<std::string>::size_type idx = pos-fSysId.begin();
    if (fComponentsToSend[idx]>0) {
      LOG(debug) << "Create timeslice component for link " << nrComp;

      fles::StorableTimeslice component{static_cast<uint32_t>(ts.num_core_microslices()), ts.index()};
      component.append_component(ts.num_microslices(0));

      for (size_t m = 0; m < ts.num_microslices(nrComp); ++m) {
        component.append_microslice( 0, m, ts.descriptor(nrComp, m), ts.content(nrComp, m) );
      }
/*
      LOG(info) << "Number of core microslices before: " << ts.num_core_microslices();
      LOG(info) << "Number of core microslices after : " << component.num_core_microslices();
      LOG(info) << "Number of microslices: " << component.num_microslices(0);
*/
      if ( ! SendData(component, idx) ) return false;
      return true;
    }
  }
  return true;
}

bool CbmMQTsaMultiSampler::CreateAndCombineComponentsPerSysId( const fles::Timeslice& ts )
{
  /// First build the list of components for each SysId if it was not already done
  if( false == fbListCompPerSysIdReady )
  {
    for( uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx )
    {
      uint16_t usSysId = ts.descriptor( uCompIdx, 0 ).sys_id;

      const vector<int>::const_iterator pos = std::find( fSysId.begin(), fSysId.end(), usSysId );
      if( fSysId.end() != pos )
      {
        const vector<std::string>::size_type idx = pos - fSysId.begin();

        fvvCompPerSysId[ idx ].push_back( uCompIdx );
      } // if( fSysId.end() != pos )
    } // for( uint32_t uNrComp = 0; uNrComp < ts.num_components(); ++uNrComp )

    for( uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx )
    {
      std::stringstream ss;
      ss << "Found " << std::setw( 2 ) << fvvCompPerSysId[ uSysIdx ].size()
         << " components for SysId 0x"
         << std::hex << std::setw( 2 ) << fSysId[ uSysIdx ] << std::dec << " :";

      for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uSysIdx ].size(); ++uComp )
      {
        ss << " " << std::setw( 3 ) << fvvCompPerSysId[ uSysIdx ][ uComp ];
      } // for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uSysIdx ].size(); ++uComp )

      LOG(info) << ss.str();
    } // for( uint32_t uSysId = 0; uSysId < fSysId.size(); ++uSysId )

    fbListCompPerSysIdReady = true;
  } // if( false == fbListCompPerSysIdReady )

  /// Then loop on all possible SysId and send TS with their respective components if needed
  for( uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx )
  {
    if( 0 < fComponentsToSend[ uSysIdx ] )
    {
      LOG(debug) << "Create timeslice with components for SysId "
                 << std::hex << fSysId[ uSysIdx ] << std::dec;

      if( 0 < fvvCompPerSysId[ uSysIdx ].size() )
      {
        fles::StorableTimeslice component{ static_cast<uint32_t>( ts.num_core_microslices() ), ts.index() };

        for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uSysIdx ].size(); ++uComp )
        {
          uint32_t uNumMsInComp = ts.num_microslices( fvvCompPerSysId[ uSysIdx ][ uComp ] );
          component.append_component( uNumMsInComp );

          LOG(debug) << "Add components to TS for SysId "
                    << std::hex << fSysId[ uSysIdx ] << std::dec
                    << " TS " << ts.index()
                    << " Comp " << fvvCompPerSysId[ uSysIdx ][ uComp ];

          for( size_t m = 0; m < uNumMsInComp; ++m )
          {
            component.append_microslice( uComp, m,
                                         ts.descriptor( fvvCompPerSysId[ uSysIdx ][ uComp ], m ),
                                         ts.content(    fvvCompPerSysId[ uSysIdx ][ uComp ], m ) );
          } // for( size_t m = 0; m < uNumMsInComp; ++m )
        } // for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uSysIdx ].size(); ++uComp )

        LOG(debug) << "Prepared timeslice for SysId "
                   << std::hex << fSysId[ uSysIdx ] << std::dec
                   << " with " << component.num_components() << " components";

        if( !SendData( component, uSysIdx ) )
          return false;
      } // if( 0 < fvvCompPerSysId[ uSysIdx ].size() )
    } // if( 0 < fComponentsToSend[ uSysIdx ] )
  } // for( uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx )

  return true;
}

bool CbmMQTsaMultiSampler::CreateAndCombineComponentsPerChannel( const fles::Timeslice& ts )
{

  /// First build the list of components for each channel name if it was not already done
  if( false == fbListCompPerChannelReady )
  {
    /// First add each channel enabled for sending to the list of channels we will use
    for( uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx )
    {
      if( 0 < fComponentsToSend[ uSysIdx ] )
      {
        for( uint32_t uChan = 0; uChan < fChannelsToSend[ uSysIdx ].size(); ++ uChan )
        {
          const vector<std::string>::const_iterator pos = std::find( fvChannelsToSend.begin(), fvChannelsToSend.end(),
                                                                     fChannelsToSend[ uSysIdx ][ uChan ] );
          if( fvChannelsToSend.end() == pos )
          {
            fvChannelsToSend.push_back( fChannelsToSend[ uSysIdx ][ uChan ] );
            fvvCompPerChannel.push_back( std::vector< uint32_t >() );
          }
        } // for( uChan = 0; uChan < fChannelsToSend[ uSysIdx ].size(); ++ uChan )
      } // if( 0 < fComponentsToSend[ uSysIdx ] )
    } // for( uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx )

    /// Now resize the vector in which we will store fo each sending channel the list of components
    fvvCompPerChannel.resize( fvChannelsToSend.size() );

    /// Check for each component if its system is enabled and if the name of its channel(s) is in the list
    /// If yes, add it to the vector of the corresponding channel
    for( uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx )
    {
      uint16_t usSysId = ts.descriptor( uCompIdx, 0 ).sys_id;

      const vector<int>::const_iterator pos = std::find( fSysId.begin(), fSysId.end(), usSysId );
      if( fSysId.end() != pos )
      {
        const vector<std::string>::size_type idxSys = pos - fSysId.begin();

        if( 0 < fComponentsToSend[ idxSys ] )
        {
          for( uint32_t uChan = 0; uChan < fChannelsToSend[ idxSys ].size(); ++ uChan )
          {
            const vector<std::string>::const_iterator posCh = std::find( fvChannelsToSend.begin(), fvChannelsToSend.end(),
                                                                         fChannelsToSend[ idxSys ][ uChan ] );
            if( fvChannelsToSend.end() != posCh )
            {
              const vector<std::string>::size_type idxChan = posCh - fvChannelsToSend.begin();
              fvvCompPerChannel[ idxChan ].push_back( uCompIdx );
            } // if( fvChannelsToSend.end() != posCh )
          } // for( uChan = 0; uChan < fChannelsToSend[ idxSys ].size(); ++ uChan )
        } // if( 0 < fComponentsToSend[ uSysIdx ] )
      } // if( fSysId.end() != pos )
    } // for( uint32_t uNrComp = 0; uNrComp < ts.num_components(); ++uNrComp )

    for( uint32_t uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx )
    {
      std::stringstream ss;
      ss << "Found " << std::setw( 2 ) << fvvCompPerChannel[ uChanIdx ].size()
         << " components for channel " << fvChannelsToSend[ uChanIdx ] << " :";

      for( uint32_t uComp = 0; uComp < fvvCompPerChannel[ uChanIdx ].size(); ++uComp )
      {
        ss << " " << std::setw( 3 ) << fvvCompPerChannel[ uChanIdx ][ uComp ];
      } // for( uint32_t uComp = 0; uComp < fvvCompPerChannel[ uChanIdx ].size(); ++uComp )

      LOG(info) << ss.str();
    } // for( uint32_t uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx )

    fbListCompPerChannelReady = true;
  } // if( false == fbListCompPerSysIdReady )

  /// Loop on channels
    /// Loop on possible SysId and check channels

  /// Then loop on all possible channels and send TS with their respective components if needed
  for( uint32_t uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx )
  {
    LOG(debug) << "Create timeslice with components for channel "
               << fvChannelsToSend[ uChanIdx ];

    if( 0 < fvvCompPerChannel[ uChanIdx ].size() )
    {
      fles::StorableTimeslice component{ static_cast<uint32_t>( ts.num_core_microslices() ), ts.index() };

      for( uint32_t uComp = 0; uComp < fvvCompPerChannel[ uChanIdx ].size(); ++uComp )
      {
        uint32_t uNumMsInComp = ts.num_microslices( fvvCompPerChannel[ uChanIdx ][ uComp ] );
        component.append_component( uNumMsInComp );

        LOG(debug) << "Add components to TS for SysId "
                  << std::hex
                  << static_cast< uint16_t >( ts.descriptor( fvvCompPerChannel[ uChanIdx ][ uComp ], 0).sys_id )
                  << std::dec
                  << " TS " << ts.index()
                  << " Comp " << fvvCompPerChannel[ uChanIdx ][ uComp ];

        for( size_t m = 0; m < uNumMsInComp; ++m )
        {
          component.append_microslice( uComp, m,
                                       ts.descriptor( fvvCompPerChannel[ uChanIdx ][ uComp ], m ),
                                       ts.content(    fvvCompPerChannel[ uChanIdx ][ uComp ], m ) );
        } // for( size_t m = 0; m < uNumMsInComp; ++m )
      } // for( uint32_t uComp = 0; uComp < fvvCompPerChannel[ uChanIdx ].size(); ++uComp )

      LOG(debug) << "Prepared timeslice for channel "
                 << fvChannelsToSend[ uChanIdx ]
                 << " with " << component.num_components() << " components";

      if( !SendData( component, fvChannelsToSend[ uChanIdx ] ) )
        return false;
    } // if( 0 < fvvCompPerSysId[ uSysIdx ].size() )
  } // for( uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx )

  return true;
}

bool CbmMQTsaMultiSampler::CreateAndSendFullTs( const fles::Timeslice& ts )
{
  /// Send full TS to all enabled channels
  for( uint32_t uChanIdx = 0; uChanIdx < fChannelsToSend.size(); ++uChanIdx )
  {
    if( 0 < fComponentsToSend[ uChanIdx ] )
    {
      LOG(debug) << "Copy timeslice component for channel " << fChannelsToSend[ uChanIdx ][ 0 ];

      fles::StorableTimeslice fullTs{ ts };
      if ( ! SendData( fullTs, uChanIdx ) )
        return false;
    } // if( 0 < fComponentsToSend[ uChanIdx ] )
  } // for( uint32_t uChanIdx = 0; uChanIdx < fChannelsToSend.size(); ++uChanIdx )
  return true;
}

bool CbmMQTsaMultiSampler::SendData(const fles::StorableTimeslice& component, int idx)
{
  // serialize the timeslice and create the message
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << component;
  std::string* strMsg = new std::string(oss.str());

  FairMQMessagePtr msg(NewMessage(const_cast<char*>(strMsg->c_str()), // data
                                                    strMsg->length(), // size
                                                    [](void* /*data*/, void* object){ delete static_cast<std::string*>(object); },
                                                    strMsg)); // object that manages the data

  // TODO: Implement sending same data to more than one channel
  // Need to create new message (copy message??)
  if (fComponentsToSend[idx]>1) {
    LOG(info) << "Need to copy FairMessage";
  }

  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).

  LOG(debug) << "Send data to channel " << fChannelsToSend[idx][0];
  if (Send(msg, fChannelsToSend[idx][0]) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }

  fMessageCounter++;
  LOG(debug) << "Send message " << fMessageCounter << " with a size of "
            << msg->GetSize();

  return true;
}

bool CbmMQTsaMultiSampler::SendData(const fles::StorableTimeslice& component, std::string sChannel )
{
  // serialize the timeslice and create the message
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << component;
  std::string* strMsg = new std::string(oss.str());

  FairMQMessagePtr msg(NewMessage(const_cast<char*>(strMsg->c_str()), // data
                                                    strMsg->length(), // size
                                                    [](void* /*data*/, void* object){ delete static_cast<std::string*>(object); },
                                                    strMsg)); // object that manages the data

  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).
  LOG(debug) << "Send data to channel " << sChannel;
  if (Send(msg, sChannel) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }

  fMessageCounter++;
  LOG(debug) << "Send message " << fMessageCounter << " with a size of "
            << msg->GetSize();

  return true;
}


CbmMQTsaMultiSampler::~ CbmMQTsaMultiSampler()
{
}

void CbmMQTsaMultiSampler::CalcRuntime()
{
  std::chrono::duration<double> run_time =
     std::chrono::steady_clock::now() - fTime;

  LOG(info) << "Runtime: " << run_time.count();
  LOG(info) << "No more input data";
}



void CbmMQTsaMultiSampler::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
{
  LOG(info) << "Header ID: Ox" << std::hex << static_cast<int>(mdsc.hdr_id)
            << std::dec;
  LOG(info) << "Header version: Ox" << std::hex << static_cast<int>(mdsc.hdr_ver)
            << std::dec;
  LOG(info) << "Equipement ID: " << mdsc.eq_id;
  LOG(info) << "Flags: " << mdsc.flags;
  LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(mdsc.sys_id)
            << std::dec;
  LOG(info) << "Sys version: Ox" << std::hex << static_cast<int>(mdsc.sys_ver)
            << std::dec;
  LOG(info) << "Microslice Idx: " << mdsc.idx;
  LOG(info) << "Checksum: " << mdsc.crc;
  LOG(info) << "Size: " << mdsc.size;
  LOG(info) << "Offset: " << mdsc.offset;
}

bool CbmMQTsaMultiSampler::CheckTimeslice(const fles::Timeslice& ts)
{
  if ( 0 == ts.num_components() ) {
    LOG(error) << "No Component in TS " << ts.index();
    return 1;
  }
  LOG(info) << "Found " << ts.num_components()
            << " different components in timeslice";

  for (size_t c = 0; c < ts.num_components(); ++c) {
    LOG(info) << "Found " << ts.num_microslices(c)
              << " microslices in component " << c;
    LOG(info) << "Component " << c << " has a size of "
              << ts.size_component(c) << " bytes";
    LOG(info) << "Component " << c << " has the system id 0x"
              << std::hex << static_cast<int>(ts.descriptor(c,0).sys_id)
              << std::dec;
    LOG(info) << "Component " << c << " has the system id 0x"
              << static_cast<int>(ts.descriptor(c,0).sys_id);

/*
    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
*/
  }

  return true;
}
