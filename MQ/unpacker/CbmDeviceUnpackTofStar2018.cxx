/**
 * CbmDeviceUnpackTofStar2018.cxx
 *
 * @since 2018-04-24
 * @author F. Uhlig
 */

#include "CbmDeviceUnpackTofStar2018.h"

#include "CbmTofUnpackPar.h"
//#include "CbmHistManager.h"
#include "CbmTbDaqBuffer.h"
#include "CbmTofDigiExp.h"

#include "StorableTimeslice.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig

#include "TH1.h"
#include "TH2.h"

#include <boost/archive/binary_iarchive.hpp>

#include <string>
#include <iomanip>

#include <stdexcept>
struct InitTaskError : std::runtime_error { using std::runtime_error::runtime_error; };

using namespace std;

static Int_t iMess=0;
const  Int_t DetMask = 0x0001FFFF;

CbmDeviceUnpackTofStar2018::CbmDeviceUnpackTofStar2018()
  : fNumMessages(0)
  , fuMsAcceptsPercent(100)
  , fuOverlapMsNb(0)
  , fuMinNbGdpb( 0 )
  , fuCurrNbGdpb( 0 )
  , fuNrOfGdpbs(0)
  , fuNrOfFebsPerGdpb(0)
  , fuNrOfGet4PerFeb(0)
  , fuNrOfChannelsPerGet4(0)
  , fuNrOfChannelsPerFeet(0)
  , fuNrOfGet4(0)
  , fuNrOfGet4PerGdpb(0)
  , fuNrOfChannelsPerGdpb(0)
  , fMsgCounter(11,0) // length of enum MessageTypes initialized with 0
  , fGdpbIdIndexMap()
  , fuGdpbId(0)
  , fuGdpbNr(0)
  , fuGet4Id(0)
  , fuGet4Nr(0)
    //  , fHM(new CbmHistManager())
  , fvulCurrentEpoch()
  , fvbFirstEpochSeen()
  , fNofEpochs(0)
  , fulCurrentEpochTime(0.)
  , fEquipmentId(0)
  , fdMsIndex(0.)
  , fdTShiftRef(0.)
  , fbEpochSuppModeOn( kFALSE )
  , fbGet4M24b( kFALSE )
  , fbGet4v20( kFALSE )
  , fbMergedEpochsOn( kFALSE )
  , fDigi(nullptr)
  , fUnpackPar(nullptr)
  , fdRefTime(0.)
  , fdLastDigiTime(0.)
  , fdFirstDigiTimeDif(0.)
  , fdEvTime0(0.)
  , fhRawTDigEvT0( nullptr )
  , fhRawTDigRef0( nullptr )
  , fhRawTDigRef( nullptr )
  , fhRawTRefDig0( nullptr )
  , fhRawTRefDig1( nullptr )
  , fhRawDigiLastDigi( nullptr )
  , fhRawTotCh()
  , fhChCount()
  , fvbChanThere()
  , fhChanCoinc()
  , fhDetChanCoinc(nullptr)
  , fvmEpSupprBuffer()
  , fBuffer(CbmTbDaqBuffer::Instance())
  , fulGdpbTsMsb(0.)	
  , fulGdpbTsLsb(0.)	
  , fulStarTsMsb(0.)	
  , fulStarTsMid(0.)	
  , fulGdpbTsFullLast(0.)
  , fulStarTsFullLast(0.)
  , fuStarTokenLast(0)
  , fuStarDaqCmdLast(0)	
  , fuStarTrigCmdLast(0)
{
}

void CbmDeviceUnpackTofStar2018::InitTask()
try
{
   // Get the information about created channels from the device
    // Check if the defined channels from the topology (by name)
    // are in the list of channels which are possible/allowed
    // for the device
    // The idea is to check at initilization if the devices are
    // properly connected. For the time beeing this is done with a
    // nameing convention. It is not avoided that someone sends other
    // data on this channel.
    int noChannel = fChannels.size();
    LOG(info) << "Number of defined input channels: " << noChannel;
    for(auto const &entry : fChannels) {
      LOG(info) << "Channel name: " << entry.first;
      if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
      OnData(entry.first, &CbmDeviceUnpackTofStar2018::HandleData);
    }
    InitContainers();
} catch (InitTaskError& e) {
 LOG(error) << e.what();
 ChangeState(ERROR_FOUND);
}

bool CbmDeviceUnpackTofStar2018::IsChannelNameAllowed(std::string channelName)
{

  for(auto const &entry : fAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1!=std::string::npos) {
      const vector<std::string>::const_iterator pos =
         std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos-fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName
              << " found in list of allowed channel names at position " << idx;
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName
            << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceUnpackTofStar2018::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmDeviceUnpackTofStar2018.";
  
  // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
  // Should only be used for small data because of the cost of an additional copy
  std::string message{"CbmTofUnpackPar,111"};
  LOG(info) << "Requesting parameter container CbmTofUnpackPar, sending message: " << message;
  
  FairMQMessagePtr req(NewSimpleMessage("CbmTofUnpackPar,111"));
  FairMQMessagePtr rep(NewMessage());
  
  if (Send(req, "parameters") > 0)
    {
      if (Receive(rep, "parameters") >= 0)
        {
	  if (rep->GetSize() != 0)
            {
	      CbmMQTMessage tmsg(rep->GetData(), rep->GetSize());
	      fUnpackPar = static_cast<CbmTofUnpackPar*>(tmsg.ReadObject(tmsg.GetClass()));
	      LOG(info) << "Received parameter from the server:";
	      fUnpackPar->print();
            }
	  else
            {
	      LOG(error) << "Received empty reply. Parameter not available";
            }
        }
    }
  
  
  Bool_t initOK = ReInitContainers();
  
  CreateHistograms();
  
  fvulCurrentEpoch.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
  fvbFirstEpochSeen.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
  fvbChanThere.resize( fUnpackPar->GetNumberOfChannels(), kFALSE );
  for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
    {
      for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
	{
	  fvulCurrentEpoch[GetArrayIndex(i, j)] = 0;
	  fvbFirstEpochSeen[GetArrayIndex(i, j)] = kFALSE;
	} // for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
    } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
  
  return initOK;
}

Bool_t CbmDeviceUnpackTofStar2018::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for CbmDeviceUnpackTofStar2018.";
  
  fuNrOfGdpbs = fUnpackPar->GetNrOfRocs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;
  fuMinNbGdpb = fuNrOfGdpbs;
  
  fuNrOfFebsPerGdpb = fUnpackPar->GetNrOfFebsPerGdpb();
  LOG(info) << "Nr. of FEBS per Tof GDPB: " << fuNrOfFebsPerGdpb;
  
  fuNrOfGet4PerFeb = fUnpackPar->GetNrOfGet4PerFeb();
  LOG(info) << "Nr. of GET4 per Tof FEB: " << fuNrOfGet4PerFeb;
  
  fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
  LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;
  
  fuNrOfChannelsPerFeet = fuNrOfGet4PerFeb * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per FEET: " << fuNrOfChannelsPerFeet;
  
  fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFebsPerGdpb * fuNrOfGet4PerFeb;
  LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;
  
  fuNrOfGet4PerGdpb = fuNrOfFebsPerGdpb * fuNrOfGet4PerFeb;
  LOG(info) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;
  
  fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;
  
  fGdpbIdIndexMap.clear();
  for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
    {
      fGdpbIdIndexMap[fUnpackPar->GetRocId(i)] = i;
      LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex << fUnpackPar->GetRocId(i)
		<< std::dec;
    } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
  UInt_t uNrOfChannels = fUnpackPar->GetNumberOfChannels();
  LOG(info) << "Nr. of mapped Tof channels: " << uNrOfChannels;
  for( UInt_t i = 0; i < uNrOfChannels; ++i)
    {
      if (i % 8 == 0)
	LOG(info);
      LOG(info) << Form(" 0x%08x", fUnpackPar->GetChannelToDetUIdMap(i) );
    } // for( UInt_t i = 0; i < uNrOfChannels; ++i)
  LOG(info);
  
  LOG(info) << "Plot Channel Rate => "
	    << (fUnpackPar->IsChannelRateEnabled() ? "ON" : "OFF");
  
  if( fbEpochSuppModeOn )
    fvmEpSupprBuffer.resize( fuNrOfGet4 );
  
  return kTRUE;
}

void CbmDeviceUnpackTofStar2018::CreateHistograms()
{
  LOG(info) << "create Histos for " << fuNrOfGdpbs <<" gDPBs ";
  
  fhRawTDigEvT0 = new TH1F( Form("Raw_TDig-EvT0"),
			    Form("Raw digi time difference to 1st digi ; time [ns]; cts"),
			    500, 0, 100.);
  //   fHM->Add( Form("Raw_TDig-EvT0"), fhRawTDigEvT0);
  
  fhRawTDigRef0 = new TH1F( Form("Raw_TDig-Ref0"),
			    Form("Raw digi time difference to Ref ; time [ns]; cts"),
			    6000, -10000, 50000);
  //   fHM->Add( Form("Raw_TDig-Ref0"), fhRawTDigRef0);
  
  fhRawTDigRef = new TH1F( Form("Raw_TDig-Ref"),
			   Form("Raw digi time difference to Ref ; time [ns]; cts"),
			   6000, -1000, 5000);
  //   fHM->Add( Form("Raw_TDig-Ref"), fhRawTDigRef);
  
  fhRawTRefDig0 = new TH1F( Form("Raw_TRef-Dig0"),
			    Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
			    9999, -50000, 50000);
  //   fHM->Add( Form("Raw_TRef-Dig0"), fhRawTRefDig0);
  
  fhRawTRefDig1 = new TH1F( Form("Raw_TRef-Dig1"),
			    Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
			    9999, -5000, 5000);
  //   fHM->Add( Form("Raw_TRef-Dig1"), fhRawTRefDig1);
  
  fhRawDigiLastDigi = new TH1F( Form("Raw_Digi-LastDigi"),
				Form("Raw Digi time difference to last digi  ; time [ns]; cts"),
				9999, -5000, 5000);
  //                                 9999, -5000000, 5000000);
  //   fHM->Add( Form("Raw_Digi-LastDigi"), fhRawDigiLastDigi);
  
  fhRawTotCh.resize( fuNrOfGdpbs );
  fhChCount.resize( fuNrOfGdpbs );
  fhChanCoinc.resize( fuNrOfGdpbs * fuNrOfFebsPerGdpb / 2 );
  for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
    {
      fhRawTotCh[ uGdpb ] = new TH2F( Form("Raw_Tot_gDPB_%02u", uGdpb),
                                      Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb),
                                      fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb,
                                      256, 0., 256. );
      //      fHM->Add( Form("Raw_Tot_gDPB_%02u", uGdpb), fhRawTotCh[ uGdpb ]);
      
      fhChCount[ uGdpb ] = new TH1I( Form("ChCount_gDPB_%02u", uGdpb),
                                     Form("Channel counts gDPB %02u; channel; Hits", uGdpb),
                                     fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );
      //      fHM->Add( Form("ChCount_gDPB_%02u", uGdpb), fhChCount[ uGdpb ]);
      /*
	for( UInt_t uLeftFeb = uGdpb*fuNrOfFebsPerGdpb / 2;
	uLeftFeb < (uGdpb + 1 )*fuNrOfFebsPerGdpb / 2;
	++uLeftFeb )
	{
	fhChanCoinc[ uLeftFeb ] = new TH2F( Form("fhChanCoinc_%02u", uLeftFeb),
	Form("Channels Coincidence %02; Left; Right", uLeftFeb),
	fuNrOfChannelsPerFeet, 0., fuNrOfChannelsPerFeet,
	fuNrOfChannelsPerFeet, 0., fuNrOfChannelsPerFeet );
	} // for( UInt_t uLeftFeb = 0; uLeftFeb < fuNrOfFebsPerGdpb / 2; uLeftFeb ++ )
      */
      fhChanCoinc[ uGdpb ] = new TH2F( Form("fhChanCoinc_%02u", uGdpb),
				       Form("Channels Coincidence %02u; Left; Right", uGdpb),
				       fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb,
				       fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );
    } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
  fhDetChanCoinc = new TH2F( "fhDetChanCoinc",
			     "Det Channels Coincidence; Left; Right",
			     32, 0., 32,
			     32, 0., 32 );
}


// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceUnpackTofStar2018::HandleData(FairMQMessagePtr& msg, int /*index*/)
{
// Don't do anything with the data
// Maybe add an message counter which counts the incomming messages and add
// an output
  fNumMessages++;
  LOG(info) << "Received message number "<<  fNumMessages 
            << " with size " << msg->GetSize(); 

  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  fles::StorableTimeslice component{0};
  inputArchive >> component;

  CheckTimeslice(component);
  DoUnpack(component, 0);

  return true;
}

Bool_t CbmDeviceUnpackTofStar2018::DoUnpack(const fles::Timeslice& ts, size_t component)
{
   LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
              << " microslices of component " << component;

   // Loop over microslices
   Int_t iMessageType = -111;
   size_t numCompMsInTs = ts.num_microslices(component);
   for (size_t m = 0; m < numCompMsInTs; ++m)
   {
      // Jump some microslices if needed
//      if( fuMsAcceptsPercent < m)
//         continue;

      // Ignore overlap ms if number defined by user
      if( numCompMsInTs - fuOverlapMsNb <= m )
         continue;

      constexpr uint32_t kuBytesPerMessage = 8;

      auto msDescriptor = ts.descriptor(component, m);
      fEquipmentId = msDescriptor.eq_id;
      fdMsIndex = static_cast<double>(msDescriptor.idx);
      const uint8_t* msContent = reinterpret_cast< const uint8_t* >( ts.content(component, m) );

      uint32_t size = msDescriptor.size;
      if( 0 < size )
         LOG(debug) << "Microslice "<< m <<": " << fdMsIndex
                     << " has size: " << size;

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
         LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
         LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (size - (size % kuBytesPerMessage) )
                            / kuBytesPerMessage;

      // Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent );
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
      {
         // Fill message
         uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
         ngdpb::Message mess( ulData );

	 /*
         if( gLogger->IsLogNeeded(fair::mq::logger::DEBUG) )
         {
            mess.printDataCout();
         } // if( gLogger->IsLogNeeded(fair::mq::logger::DEBUG) )
	 */

         // Increment counter for different message types
         iMessageType = mess.getMessageType();
         fMsgCounter[ iMessageType ]++;

         fuGdpbId = mess.getRocNumber();
         fuGdpbNr = fGdpbIdIndexMap[ fuGdpbId ];

         fuGet4Id = mess.getGdpbGenChipId();
         fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

         if( fuNrOfGet4PerGdpb <= fuGet4Id &&
             ngdpb::MSG_STAR_TRI != iMessageType &&
             ( get4v1x::kuChipIdMergedEpoch != fuGet4Id ||
	       kFALSE == fbMergedEpochsOn ) )
	   {
            LOG(warn) << "Message with Get4 ID too high: " << fuGet4Id
                         << " VS " << fuNrOfGet4PerGdpb << " set in parameters, message ignored";
	    continue;
	   }

         switch( mess.getMessageType() )
         {
            case ngdpb::MSG_HIT:
            case ngdpb::MSG_EPOCH:
            case ngdpb::MSG_GET4:
            {
	      if( fbGet4M24b )
		FillHitInfo(mess);
	      else PrintGenInfo(mess);
	      break;
            } // case old non tof messages
            case ngdpb::MSG_EPOCH2:
            {
               if( get4v1x::kuChipIdMergedEpoch == fuGet4Id &&
		   kTRUE == fbMergedEpochsOn )
               {
                  for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )
                  {
		    fuGet4Id = uGet4Index;
		    fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
		    ngdpb::Message tmpMess( mess );
		    tmpMess.setGdpbGenChipId( uGet4Index );
//                     fHistGet4MessType->Fill(uGet4Index, ngdpb::GET4_32B_EPOCH);
                     FillEpochInfo( tmpMess );
                  } // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGetIndex ++ )
               } // if this epoch message is a merged one valiud for all chips
               else
               {
//                  fHistGet4MessType->Fill( fuGet4Nr, ngdpb::GET4_32B_EPOCH );
                  FillEpochInfo(mess);
               } // if single chip epoch message
               break;
            } // case ngdpb::MSG_EPOCH2:
            case ngdpb::MSG_GET4_32B:
            {
	      if( fbEpochSuppModeOn )
	      {
		fvmEpSupprBuffer[ fuGet4Nr ].push_back( mess );
		LOG(debug) << "Add 32B message from Gdpb " << fuGdpbNr << " to EpSupprBuffer of Get4 " 
                           << fuGet4Nr << " size " << fvmEpSupprBuffer[ fuGet4Nr ].size();
	      }
	      else FillHitInfo(mess);
              break;

            } // case ngdpb::MSG_GET4_32B:
            case ngdpb::MSG_GET4_SLC:
            {
               PrintSlcInfo(mess);
               break;
            } // case ngdpb::MSG_GET4_SLC:
            case ngdpb::MSG_GET4_SYS:
            {
               if(100 > iMess++)
                  PrintSysInfo(mess);
               break;
            } // case ngdpb::MSG_GET4_SYS:
            case ngdpb::MSG_STAR_TRI:
            {
               FillStarTrigInfo(mess);
               break;
            } // case ngdpb::MSG_STAR_TRI:
            default:
            {
               if(100 > iMess++)
                  LOG(error) << "Message ("<<iMess<<") type " << std::hex << std::setw(2)
                             << static_cast< uint16_t >( mess.getMessageType() )
                             << " not included in Get4 unpacker.";
               if(100 == iMess)
                  LOG(error) << "Stop reporting MSG errors... ";
            } // default:
         } // switch( mess.getMessageType() )

      } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

   } // for (size_t m = 0; m < numCompMsInTs; ++m)


  return kTRUE;
}

void CbmDeviceUnpackTofStar2018::FillHitInfo( ngdpb::Message mess )
{
   // --- Get absolute time, GET4 ID and channel number
   UInt_t uGet4Id     = mess.getGdpbGenChipId();
   UInt_t uChannel    = mess.getGdpbHitChanId();
   UInt_t uTot        = mess.getGdpbHit32Tot();
//   UInt_t uFts        = mess.getGdpbHitFineTs();

   ULong_t  ulCurEpochGdpbGet4 = fvulCurrentEpoch[ fuGet4Nr ];

   if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
   {
      // In Ep. Suppr. Mode, receive following epoch instead of previous
      if( 0 < ulCurEpochGdpbGet4 )
         ulCurEpochGdpbGet4 --;
         else ulCurEpochGdpbGet4 = get4v1x::kuEpochCounterSz; // Catch epoch cycle!

//      ULong_t  ulhitTime;
      Double_t dHitTime;
      if( fbGet4v20 )
      {
//	ulhitTime = mess.getMsgG4v2FullTime(  ulCurEpochGdpbGet4 );
	dHitTime = mess.getMsgG4v2FullTimeD( ulCurEpochGdpbGet4 );

      // In 32b mode the coarse counter is already computed back to 112 FTS bins
      // => need to hide its contribution from the Finetime
      // => FTS = Fullt TS modulo 112
//	if( !fbGet4M24b )
//	  uFts = mess.getGdpbHitFullTs() % 112;
      } // if( fbGet4v20 )
      else
      {
//         ulhitTime  = mess.getMsgFullTime(ulCurEpochGdpbGet4);
         dHitTime   = mess.getMsgFullTimeD(ulCurEpochGdpbGet4);
      } // else of if( fbGet4v20 )

      Double_t dHitTot   = uTot;     // in bins


//      UInt_t uFebIdx     = (uGet4Id / fuNrOfGet4PerFeb);
//      UInt_t uFullFebIdx = (fuGdpbNr * fuNrOfFebsPerGdpb) + uFebIdx;

      UInt_t uChanInGdpb = uGet4Id * fuNrOfChannelsPerGet4 + uChannel;
      Int_t uChanInSyst = fuGdpbNr * fuNrOfChannelsPerGdpb + uChanInGdpb;
      if( fUnpackPar->GetNumberOfChannels() < uChanInSyst )
      {
         LOG(error) << "Invalid mapping index " << uChanInSyst
                    << " VS " << fUnpackPar->GetNumberOfChannels()
                    <<", from " << fuGdpbNr
                    <<", " << uGet4Id
                    <<", " << uChannel;
         return;
      } // if( fUnpackPar->GetNumberOfChannels() < uChanUId )

      fvbChanThere[ uChanInSyst ] = kTRUE;

      UInt_t uChanUId = fUnpackPar->GetChannelToDetUIdMap( uChanInSyst );
      if( 0 == uChanUId )
         return;   // Hit not mapped to digi

      /*
      if( (uChanUId & DetMask) == 0x00001006 )
      {
         UInt_t uDetChan = (uChanUId & 0xFF000000) >> 24;
         if( (uChanUId & 0x00800000) == 0x00800000 )
            uDetChan += 32;

         fbDetChanThere[ uDetChan ] = kTRUE;
      } // if( (uChanUId & DetMask) == 0x00001006 )
      */
      //      fhRawDigiLastDigi->Fill( dHitTime - fdLastDigiTime );

      if( (uChanUId & DetMask) != 0x00005006 )  dHitTime += fdTShiftRef;
      fdLastDigiTime = dHitTime;

      LOG(debug) << Form("Insert 0x%08x digi with time ", uChanUId ) << dHitTime << Form(", Tot %4.0f",dHitTot)
                 << " into buffer with " << fBuffer->GetSize() << " data from "
                 << Form("%11.1f to %11.1f ", fBuffer->GetTimeFirst(), fBuffer->GetTimeLast())
                 << " at epoch " << ulCurEpochGdpbGet4;

      fDigi = new CbmTofDigiExp(uChanUId, dHitTime, dHitTot);

      fBuffer->InsertData<CbmTofDigiExp>(fDigi);

      // Histograms filling
      fhRawTotCh[ fuGdpbNr ]->Fill( uChanInGdpb, dHitTot);
      fhChCount[ fuGdpbNr ] ->Fill( uChanInGdpb );

   } // if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
}

void CbmDeviceUnpackTofStar2018::FillEpochInfo( ngdpb::Message mess )
{
   ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();

   //   LOG(debug) << "Get4Nr "<<fuGet4Nr<< " in epoch "<<ulEpochNr;

   fvulCurrentEpoch[ fuGet4Nr ] = ulEpochNr;

   if( kFALSE == fvbFirstEpochSeen[ fuGet4Nr ] )
      fvbFirstEpochSeen[ fuGet4Nr ] = kTRUE;

   fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);
   fNofEpochs++;

   /// In Ep. Suppr. Mode, receive following epoch instead of previous
   /// Re-align the epoch number of the message in case it will be used later:
   /// We received the epoch after the data instead of the one before!
   if( 0 < ulEpochNr )
      mess.setEpoch2Number( ulEpochNr - 1 );
   else mess.setEpoch2Number( get4v1x::kuEpochCounterSz );

   Int_t iBufferSize = fvmEpSupprBuffer[ fuGet4Nr ].size();
   if( 0 < iBufferSize )
   {
     LOG(debug) << "Now processing "<<iBufferSize<<" stored messages for get4 " 
                << fuGet4Nr << " with epoch number "
                << (fvulCurrentEpoch[ fuGet4Nr ] - 1);

      for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
      {
         FillHitInfo( fvmEpSupprBuffer[ fuGet4Nr ][ iMsgIdx ] );
      } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
/*
      for( UInt_t uLeftFeb = fuGdpbNr * fuNrOfFebsPerGdpb / 2;
           uLeftFeb < (fuGdpbNr + 1) * fuNrOfFebsPerGdpb / 2;
           ++uLeftFeb )
      {
         for( UInt_t uChanA = 2*uLeftFeb * fuNrOfChannelsPerFeet;
              uChanA < (2*uLeftFeb + 1) * fuNrOfChannelsPerFeet;
              ++uChanA )
         {
            if( kTRUE == fvbChanThere[ uChanA ] )
            {
               for( UInt_t uChanB = (2*uLeftFeb + 1) * fuNrOfChannelsPerFeet;
                    uChanB < (2*uLeftFeb + 2) * fuNrOfChannelsPerFeet;
                    ++uChanB )
               {
                  if( kTRUE == fvbChanThere[ uChanB ] )
                  {
                     fhChanCoinc[ uLeftFeb  ]->Fill( uChanA, uChanB );
                  }
               }
            }
         }
      } // for( UInt_t uLeftFeb = 0; uLeftFeb < fuNrOfFebsPerGdpb / 2; ++uLeftFeb )
*/
      fvmEpSupprBuffer[fuGet4Nr].clear();
   } // if( 0 < fvmEpSupprBuffer[fGet4Nr] )
}

void CbmDeviceUnpackTofStar2018::PrintSlcInfo(ngdpb::Message /*mess*/)
{
   /// Nothing to do, maybe later use it to trakc parameter changes like treshold?
/*
  if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
     LOG(info) << "GET4 Slow Control message, epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
                << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                << ", Chan = " << std::setw(1) << mess.getGdpbSlcChan()
                << ", Edge = " << std::setw(1) << mess.getGdpbSlcEdge()
                << ", Type = " << std::setw(1) << mess.getGdpbSlcType()
                << ", Data = " << std::hex << std::setw(6) << mess.getGdpbSlcData() << std::dec
                << ", Type = " << mess.getGdpbSlcCrc();
*/
}

void CbmDeviceUnpackTofStar2018::PrintGenInfo(ngdpb::Message mess)
{
   Int_t mType        = mess.getMessageType();
   Int_t rocId          = mess.getRocNumber();
   Int_t get4Id     = mess.getGdpbGenChipId();
   Int_t channel    = mess.getGdpbHitChanId();
   uint64_t            uData = mess.getData();
   if(100 > iMess++)
      LOG(info) << "Get4 MSG type "<<mType<<" from rocId "<<rocId<<", getId "<<get4Id
                << ", (hit channel) "<<channel<<Form(" hex data %0lx ",uData);
}

void CbmDeviceUnpackTofStar2018::PrintSysInfo(ngdpb::Message mess)
{
   LOG(info) << "GET4 System message,       epoch " << (fvulCurrentEpoch[ fuGet4Nr])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << fuGdpbId << std::dec;

   switch( mess.getGdpbSysSubType() )
   {
      case ngdpb::SYSMSG_GET4_EVENT:
      {
         LOG(info) << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                   << ", Chan = " << std::setw(1) << mess.getGdpbSysErrChanId()
                   << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                   << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused()
                   << ", Data = " << std::hex << std::setw(2) << mess.getGdpbSysErrData() << std::dec
                   << " -- GET4 V1 Error Event";
         break;
      } //
      case ngdpb::SYSMSG_CLOSYSYNC_ERROR:
         LOG(info) << "Closy synchronization error";
         break;
      case ngdpb::SYSMSG_TS156_SYNC:
         LOG(info) << "156.25MHz timestamp reset";
         break;
      case ngdpb::SYSMSG_GDPB_UNKWN:
         LOG(info) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                   << mess.getGdpbSysUnkwData() << std::dec;
         break;
   } // switch( getGdpbSysSubType() )
}

void CbmDeviceUnpackTofStar2018::FillStarTrigInfo(ngdpb::Message mess)
{
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  //mess.printDataCout();

  switch( iMsgIndex )
  {
      case 0:
         fulGdpbTsMsb = mess.getGdpbTsMsbStarA();
         break;
      case 1:
         fulGdpbTsLsb = mess.getGdpbTsLsbStarB();
         fulStarTsMsb = mess.getStarTsMsbStarB();
         break;
      case 2:
         fulStarTsMid = mess.getStarTsMidStarC();
         break;
      case 3:
      {
         ULong64_t ulNewGdpbTsFull = ( fulGdpbTsMsb << 24 )
                           + ( fulGdpbTsLsb       );
         ULong64_t ulNewStarTsFull = ( fulStarTsMsb << 48 )
                           + ( fulStarTsMid <<  8 )
                           + mess.getStarTsLsbStarD();
         UInt_t uNewToken  = mess.getStarTokenStarD();
         UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();
         if( ( uNewToken == fuStarTokenLast ) && ( ulNewGdpbTsFull == fulGdpbTsFullLast ) &&
             ( ulNewStarTsFull == fulStarTsFullLast ) && ( uNewDaqCmd == fuStarDaqCmdLast ) &&
             ( uNewTrigCmd == fuStarTrigCmdLast ) )
         {
            LOG(debug) << "Possible error: identical STAR tokens found twice in a row => ignore 2nd! "
                         << Form("token = %5u ", fuStarTokenLast )
                         << Form("gDPB ts  = %12llu ", fulGdpbTsFullLast )
                         << Form("STAR ts = %12llu ", fulStarTsFullLast )
                         << Form("DAQ cmd = %2u ", fuStarDaqCmdLast )
                         << Form("TRG cmd = %2u ", fuStarTrigCmdLast );
            return;
         } // if exactly same message repeated
/*
         if( (uNewToken != fuStarTokenLast + 1) &&
             0 < fulGdpbTsFullLast && 0 < fulStarTsFullLast &&
             ( 4095 != fuStarTokenLast || 1 != uNewToken)  )
            LOG(warn) << "Possible error: STAR token did not increase by exactly 1! "
                         << Form("old = %5u vs new = %5u ", fuStarTokenLast,   uNewToken)
                         << Form("old = %12llu vs new = %12llu ", fulGdpbTsFullLast, ulNewGdpbTsFull)
                         << Form("old = %12llu vs new = %12llu ", fulStarTsFullLast, ulNewStarTsFull)
                         << Form("old = %2u vs new = %2u ", fuStarDaqCmdLast,  uNewDaqCmd)
                         << Form("old = %2u vs new = %2u ", fuStarTrigCmdLast, uNewTrigCmd);
*/
         fulGdpbTsFullLast = ulNewGdpbTsFull;
         fulStarTsFullLast = ulNewStarTsFull;
         fuStarTokenLast   = uNewToken;
         fuStarDaqCmdLast  = uNewDaqCmd;
         fuStarTrigCmdLast = uNewTrigCmd;

         Double_t dTot = 1.;
         Double_t dTime = fulGdpbTsFullLast * 6.25;
         if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )
         {
            fdFirstDigiTimeDif = dTime - fdLastDigiTime;
            LOG(info) << "Reference fake digi time shift initialized to " << fdFirstDigiTimeDif
		      << ", default: " << fdTShiftRef;
         } // if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )

	 //         dTime -= fdFirstDigiTimeDif;
         // dTime += fdTShiftRef;

         LOG(debug) << "Insert fake digi with time " << dTime << ", Tot " << dTot;
         fhRawTRefDig0->Fill( dTime - fdLastDigiTime);
         fhRawTRefDig1->Fill( dTime - fdLastDigiTime);

         fDigi = new CbmTofDigiExp(0x00005006, dTime, dTot); // fake start counter signal
         fBuffer->InsertData<CbmTofDigiExp>(fDigi);
         break;
	  } // case 3
      default:
         LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  } // switch( iMsgIndex )
}

CbmDeviceUnpackTofStar2018::~CbmDeviceUnpackTofStar2018()
{
}




void CbmDeviceUnpackTofStar2018::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
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

bool CbmDeviceUnpackTofStar2018::CheckTimeslice(const fles::Timeslice& ts)
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
    LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(ts.descriptor(0,0).sys_id)
            << std::dec;

/*
    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
*/
  }

  return true;
}
