// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadic20OnlineMonitor                       -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmTSUnpackSpadic20OnlineMonitor.h"

#include "CbmSpadicRawMessage.h"

#include "TimesliceReader.hpp"
#include "Timeslice.hpp"

#include "Message.hpp"

#include "FairLogger.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include "CbmHistManager.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMultiGraph.h"
#include "TMath.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "THttpServer.h"

#include <iostream>
#include <map>
#include <vector>



CbmTSUnpackSpadic20OnlineMonitor::CbmTSUnpackSpadic20OnlineMonitor(Bool_t highPerformance)
  : CbmTSUnpack(),
    fSpadicRaw(new TClonesArray("CbmSpadicRawMessage", 10)),
    fHighPerformance(highPerformance),
    fEpochMarkerArray(),
    fPreviousEpochMarkerArray(),
    fSuperEpochArray(),
    fEpochMarker(0),
    fSuperEpoch(0),
    fNrExtraneousSamples{0},
    fHM(new CbmHistManager()),
    fcB(NULL),
    fcM(NULL),
    fcH(NULL),
    fcL(NULL),
    fcE(NULL),
    fcO(NULL),
    fcS(NULL),
    fcI(NULL),
    fcTS(NULL),
    fcF(NULL),
    fcSp(NULL),
    fcPS{NULL},
    fcMS{NULL},
    fBaseline{NULL},
    fmaxADCmaxTimeBin{NULL},
    fHit{NULL},
    fLost{NULL},
    fEpoch{NULL},
    fOutOfSync{NULL},
    fStrange{NULL},
    fInfo{NULL},
    fHitTimeA{NULL},
    fHitTimeB{NULL},
    fHitFrequency{NULL},
    fSpectrum{NULL},
    fPulseShape{NULL},
    fMessageStatistic{NULL},
    fHitSync{NULL},
    fTSGraph{NULL},
    fLastSuperEpochA{0},
    fLastSuperEpochB{0},
    fLastFullTime{{{0}}},
    fMessageTypes{"Epoch",
	  "Epoch out of synch",
	  "Hit",
	  "Hit aborted",
	  "Info",
	  "Overflow",
	  "Strange"},
    fTriggerTypes{ "Global trigger",
	  "Self triggered",
	  "Neighbor triggered",
	  "Self and neighbor triggered"},
    fStopTypes{"Normal end of message",
	  "Channel buffer full",
	  "Ordering FIFO full",
	  "Multi hit",
	  "Multi hit and channel buffer full",
	  "Multi hit and ordering FIFO full"},
    fInfoTypes{"Channel disabled during message building",
	  "Next grant timeout",
	  "Next request timeout",
	  "New grant but channel empty",
	  "Corruption in message builder",
	  "Empty word",
	  "Epoch out of sync"},
    fMaxADC{NULL}
{
  for (Int_t i=0; i < NrOfSyscores; ++i) {
    for (Int_t j=0; j < NrOfHalfSpadics; ++j) {
      fEpochMarkerArray[i][j] = 0;
      fPreviousEpochMarkerArray[i][j] =0;
      fSuperEpochArray[i][j] = 0;
    }
  }

  InitHistos();
  InitCanvas();
}

CbmTSUnpackSpadic20OnlineMonitor::~CbmTSUnpackSpadic20OnlineMonitor()
{
	LOG(info) << "Number of extraneous Samples "<< fNrExtraneousSamples;
}

Bool_t CbmTSUnpackSpadic20OnlineMonitor::Init()
{
	LOG(info) << "Initializing";

	FairRootManager* ioman = FairRootManager::Instance();
	if (ioman == NULL) {
	  LOG(fatal) << "No FairRootManager instance";
	}
	ioman->Register("SpadicRawMessage", "spadic raw data", fSpadicRaw, kTRUE);
	return kTRUE;
}


Int_t  startTime = 0; // should be ULong_t as soon as used with a GetFullTime lateron
Bool_t first = true; // for the "Sync" histo : is it the very first entry into this histo?
Bool_t syncHistFinished = false; // declare explicitly, if the "Sync" histo is finished.
Int_t  maxTime = 0; // should be ULong_t as soon as used with a GetFullTime lateron

Bool_t CbmTSUnpackSpadic20OnlineMonitor::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  // compare to: https://github.com/spadic/spadic10-software/blob/master/lib/message/message.h
  // or fles/spadic/message/wrap/cpp/message.cpp
  LOG(debug) << "Unpacking Spadic Data";
  Int_t link = 0;
  //TH2I* h = NULL;
  TString cName;

  LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
             << "microslices.";

  std::cout << "Timeslice contains " << ts.num_microslices(component) << std::endl;


  //printf("Component: %i Num MS: %i \n", component, ts.num_microslices(component));

   // Loop over microslices
  //for (size_t m = 0; m < ts.num_microslices(component); ++m){
  for (size_t m {0}; m < ts.num_microslices(component); m++){

    spadic::MessageReader r;
    Int_t address = 0; // Fake the addr for now
    Int_t addr = 0; // Fake!
    Int_t counter=0;
    std::vector<uint16_t> buf0;
    std::vector<uint16_t> buf1;
    //buf0.reserve(20000);
    //buf1.reserve(20000);

    // Get the microslice
    auto mc = ts.get_microslice(component, m);
    auto mv = reinterpret_cast<const fles::MicrosliceView&>(mc);
    //auto mv_p = reinterpret_cast<const uint16_t *>(mv.content());        // Pointer to the data // (VF) not used
    //auto mv_s = mv.desc().size * sizeof(*mv.content()) / sizeof(*mv_p);  // Size (VF) unused

    //for(auto p=0; p<mv_s; p++){
    //  printf("D: %x", mv_p[p]);
    //}
    //continue;

    //const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( mv_p );

    // ----------------
    // Get the microslice
    auto msDescriptor =  ts.descriptor(component, m);   // mc
    //Int_t fEquipmentId = msDescriptor.eq_id; (VF) unused
    //auto msContent = reinterpret_cast<const uint16_t*>(ts.content(component, m));
    const uint8_t* msContent = ts.content(component, m);
    auto s = msDescriptor.size * sizeof(*msContent) / sizeof(*msContent);
    const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent );

    for (size_t uIdx = 0; uIdx < s; uIdx ++){
    //for (auto uIdx = 0; uIdx < mv_s; uIdx ++){

      uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
      //uint64_t ulData = static_cast<uint64_t>( mv_p[uIdx] );

      //auto ulData = mv_p[uIdx];
      //printf("%04x ", ulData);

      uint16_t afck_id = (ulData & 0xffff000000000000) >> 48;
      // Last 2bytes of mac addr of the afck, hard coded now for sps2016
      //if(afck_id != 0x5b9d) //IRI AFCK
      //if(afck_id != 0x187f)  // IKF 217
      if(afck_id != 0x5f91)  // LEMO
	continue;

      //uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
      uint8_t  downlink_header = (ulData & 0xf00000) >> 20;
      uint8_t  downlink_nr     = (ulData & 0xf0000)  >> 16;
      uint16_t payload         =  ulData & 0xffff;

      if(downlink_header != 0xa)
	std::cout << "Corruption in downlink header found" << std::endl;

      //printf("Payload: 0x%04X\n", payload);

      // Put the data in a buffer
      if(downlink_nr == 0)
	buf0.push_back(payload);
      else if(downlink_nr == 1)
	buf1.push_back(payload);
    }

    // This is not nice... quick n' dirty
    //std::move(buf1.begin(), buf1.end(), std::back_inserter(buf0));

    r.add_buffer(buf0.data() , buf0.size());
    r.add_buffer(buf1.data() , buf1.size());
    link = ts.descriptor(component, 0).eq_id;

    //} // End microslice

  Bool_t isInfo(false), isHit(false), isEpoch(false), isEpochOutOfSync(false), isOverflow(false), isHitAborted(false), isStrange(false);

  while(auto message = r.get_message()) {

    if(message->is_hit()){
      auto& sample = message->samples();

      printf("Group: %i Ch: %i Ts: %i Samples: %lu\n", message->group_id(), message->channel_id(), message->timestamp(), sample.size());
      //printf("Ch: %i Ts: %i hit type: %i Stop type: %i Samples: %i Trace: \n", message->channel_id(), message->timestamp(),  message->hit_type(), message->stop_type(), sample.size());
      //for(int i = 0; i < sample.size(); i++)
      //     printf(" %02i,", sample[i]);
      //printf("\n");


      // Fill some nice histos
      //Int_t triggerType =  static_cast<Int_t>(message->hit_type());
      //Int_t stopType = static_cast<Int_t>(message->stop_type());
      Int_t time = message->timestamp();
      //Int_t infoType = -1;
      Int_t channel = message->channel_id();
      Int_t groupId = message->group_id();
      //Int_t bufferOverflowCounter = 0;
      Int_t samples = message->samples().size();
      //Int_t padRow(-1);
      //Int_t padChannel = GetChannelOnPadPlane(channel,groupId);
      Int_t counter1=0;
      Int_t maxADC(-256)/*, maxTB(-1) (VF) unused*/;
      Int_t* sample_values = new Int_t[samples];
      for (auto x : message->samples()) {
	sample_values[counter1] = x;
	//if (sample_values[2]>-150) {
	//if (!fHighPerformance)
	//fPulseShape[ ( GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address))*32+channel+16*(groupId%2) ]->Fill(counter1,x);
	fPulseShape[channel+16*(groupId%2)]->Fill(counter1,x);
	//  fMaxADC[channel]->Fill(counter1,x);
	//}
	if (x > maxADC){
	  maxADC = x;
	  // maxTB = counter1;  (VF) not used
	}
	++counter1;
      }

      // Nasty look on messages from all available channels here.
      // Only filled within one epoch. Therefore: regress of time will declare the histo as finished.
      if(time < maxTime){
	syncHistFinished = true;
      }

      // Here it is: its only processed and filled as long as not declared as finished.
      if(!syncHistFinished){
	maxTime = time; // lets bookkeep the highest time value we've seen so far
      	if(first){      // just in case we are using ULong_t FullTime for the time info,
	                // its worth to store what was the time of the very first message in this histo.
	  startTime = time;
	}
      	fHitSync[0]->Fill(time, groupId*16+channel); // when startTime should be used, its x=time-startTime here
      }

      fMaxADC[channel]->Fill(maxADC);
      cName.Form("SysCore_%i_Spadic_%i",0,0);
  	  //h = (TH2I*)fHM->H2(TString("Baseline_"+cName).Data());
  	  //h->Fill(sample_values[0],groupId*16+channel);
  	  fBaseline[0]->Fill(sample_values[0],groupId*16+channel);


      // ----
    }

    else if ( message->is_buffer_overflow() ){
    	LOG(debug) <<  counter << " This is a buffer overflow message";
    	isOverflow = true;
    	//GetEpochInfo(link, addr);
    	//Int_t triggerType = -1;
    	//Int_t infoType = -1;
    	//Int_t stopType = -1;
    	Int_t groupId = message->group_id();
    	Int_t channel = message->channel_id();
    	//Int_t time = message->timestamp();
    	Int_t bufferOverflowCounter = static_cast<Int_t>(message->buffer_overflow_count());
    	Int_t samples = 1;
    	Int_t* sample_values = new Int_t[samples];
    	sample_values[0] = -256;
      /*
    	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
    	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
    			      fSuperEpoch, triggerType, infoType, stopType, groupId,
    			      bufferOverflowCounter, samples, sample_values,
    			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);
                */
    	delete[] sample_values;
    	fMessageStatistic[GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address)]->Fill(fMessageTypes[5].Data(),bufferOverflowCounter);
    	channel = GetChannelOnPadPlane(channel,groupId);
    	if (channel > 15){
    	  channel-= 16;
    	  groupId = 1;
    	} else {
    	  groupId = 0;
    	}
    	fLost[GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address)]->Fill(channel,groupId,bufferOverflowCounter);
    }

    else if ( message->is_info() ){
    	LOG(debug) <<  counter << " This is a info message";
    	isInfo = true;
    	//GetEpochInfo(link, addr);

    	Int_t triggerType = -1;
    	Int_t infoType = static_cast<Int_t>(message->info_type());
    	Int_t stopType = -1;
    	Int_t groupId = message->group_id();
    	Int_t channel = message->channel_id();
    	Int_t time = message->timestamp();
    	Int_t bufferOverflowCounter = 0;//mp->buffer_overflow_count();// should be now obsolete
    	Int_t samples = 1;
    	Int_t* sample_values = new Int_t[samples];
    	sample_values[0] = -256;
    	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
    	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
    			      fSuperEpoch, triggerType, infoType, stopType, groupId,
    			      bufferOverflowCounter, samples, sample_values,
    			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);
    	delete[] sample_values;
    	fMessageStatistic[GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address)]->Fill(fInfoTypes[infoType].Data(),1);
    	channel = GetChannelOnPadPlane(channel,groupId);
    	if (channel > 15){
    	  channel-= 16;
    	  groupId = 1;
    	} else {
    	  groupId = 0;
    	}
    	if (!fHighPerformance)fInfo[GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address)]->Fill(channel,groupId,1);
    }


    else if ( message->is_hit_aborted()) {
    	LOG(debug) <<  counter << " This is a hit message was aborted";
    	isHitAborted = true;
    	GetEpochInfo(link, addr);
    	Int_t triggerType = -1;
    	Int_t stopType = -1;
    	Int_t time = message->timestamp();
    	Int_t infoType = -1;
    	Int_t groupId = -1;//mp->group_id();//???
    	Int_t bufferOverflowCounter = 0;
    	Int_t samples = 1;
    	Int_t* sample_values = NULL;
    	Int_t channel = message->channel_id();
    	infoType = static_cast<Int_t>(message->info_type());// should here be stoptype instead???
    	sample_values = new Int_t[samples];
    	sample_values[0] = -256;
    	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
    	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
    			      fSuperEpoch, triggerType, infoType, stopType, groupId,
    			      bufferOverflowCounter, samples, sample_values,
    			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);
    	//++counter;
    	delete[] sample_values;
    	fMessageStatistic[GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address)]->Fill(fMessageTypes[3].Data(),1);
    }

    else {
    	isStrange = true;
    	//GetEpochInfo(link, addr);
    	Int_t triggerType = -1;
    	Int_t stopType = -1;
    	Int_t time = -1;//mp->timestamp();
    	Int_t infoType = -1;
    	Int_t groupId = -1;//mp->group_id();
    	Int_t bufferOverflowCounter = 0;
    	Int_t samples = 1;
    	Int_t* sample_values = NULL;
    	Int_t channel = -1;//mp->channel_id();
    	sample_values = new Int_t[samples];
    	sample_values[0] = -256;
    	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
    	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
    			      fSuperEpoch, triggerType, infoType, stopType, groupId,
    			      bufferOverflowCounter, samples, sample_values,
    			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);
    	//++counter;
    	delete[] sample_values;
    	fMessageStatistic[GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address)]->Fill(fMessageTypes[6].Data(),1);
    	//fStrange[GetSyscoreID(link) * NrOfSpadics + GetSpadicID(address)]->Fill(channel,groupId);
    	LOG(debug) <<  counter << " This message type is not hit, info, epoch or overflow and will not be stored in the TClonesArray";
    	LOG(debug) << " valide:" << message->is_valid() << " epoch marker:" << fEpochMarker << " super epoch marker:" << fSuperEpoch << " time:" << time << " link:" << link << " address:" << address;
    	LOG(debug) << "Channel ID:" << message->channel_id();
    	//if ( mp->is_hit_aborted() )
    	//LOG(info) << "hit is aborted";
    	//if ( mp->is_hit() )
    	LOG(debug) << "GroupID:" << message->group_id() << "hit: triggerType:" << static_cast<Int_t>(message->hit_type()) << " stopType:" << static_cast<Int_t>(message->stop_type()) << " Nr.of samples:" << message->samples().size();

    	//if ()
      std::stringstream ss;
    	for (auto x : message->samples()) {
    	  ss << " " << x;
    	}
    	LOG(debug) << ss.str();
    	//if (mp->is_info())
    	LOG(debug) << "InfoType:" << static_cast<Int_t>(message->info_type());
    	//if ()
    	LOG(debug) << "Nr. of overflows:" << static_cast<Int_t>(message->buffer_overflow_count());
    	//print_message(mp);
    }
  }

 } // End microslice

  return kTRUE;
}

void CbmTSUnpackSpadic20OnlineMonitor::print_message(const spadic::Message& m)
{
  printf("PrintMsg\n");
  LOG(info) << "v: " << (m.is_valid() ? "o" : "x");
  LOG(debug) << " / gid: " << static_cast<int>(m.group_id());
  LOG(debug) << " / chid: " << static_cast<int>(m.channel_id());
  if ( m.is_hit() ) {
    LOG(debug) << " / ts: " << m.timestamp();
    LOG(debug) << " / samples (" << m.samples().size() << "):";
    for (auto x : m.samples()) {
      LOG(debug) << " " << x;
    }
    LOG(debug);
  } else {
    if ( m.is_epoch_marker() ) {
      LOG(debug) << " This is an Epoch Marker";
    } else if ( m.is_epoch_out_of_sync() ) {
      LOG(info) << " This is an out of sync Epoch Marker";
    } else {
      LOG(info) << " This is not known";
    }
  }
}


void CbmTSUnpackSpadic20OnlineMonitor::FillEpochInfo(Int_t link, Int_t addr, Int_t epoch_count)
{

  auto it=groupToExpMap.find(link);
  if (it == groupToExpMap.end()) {
    LOG(fatal) << "Could not find an entry for equipment ID" <<
      std::hex << link << std::dec;
  } else {
    /* Check for repeated Epoch Messages, as the repeated Microslices
       are not captured by the CbmTsUnpacker. This is to ensure the
       linearity of the GetFullTime() method.
    */
    // dirty workaround in the following line: only sufficiently big backsteps of Epoch will trigger an upcount of SuperEpoch. this avoids a high sensitivity of the SuperEpoch counting method to overlaps of some epochs, before the overlaps are understood better
    if ( epoch_count < (fEpochMarkerArray[it->second][addr] - 3000) )
      {
	if(SuppressMultipliedEpochMessages)
	  if ( epoch_count != fPreviousEpochMarkerArray[it->second][addr] ){
	    fSuperEpochArray[it->second][addr]++;
	  } else {
	    LOG(error)<< "Multiply repeated Epoch Messages at Super Epoch "
		      << fSuperEpoch << " Epoch "
		      << epoch_count << " for Syscore"
		      << it->second << "_Spadic"
		      << addr;
	  }
	else fSuperEpochArray[it->second][addr]++;

	LOG(debug) << "Overflow of EpochCounter for Syscore"
		   << it->second << "_Spadic"
		   << addr;
      } else if ((epoch_count - fEpochMarkerArray[it->second][addr]) !=1 ) {
      LOG(info) << "Missed epoch counter for Syscore"
		<< it->second << "_Spadic"
		<< addr;
    } else if (epoch_count == fEpochMarkerArray[it->second][addr]){
      LOG(error) << "Identical Epoch Counters for Syscore"
		 << it->second << "_Spadic"
		 << addr;
    }
    fPreviousEpochMarkerArray[it->second][addr] = fEpochMarkerArray[it->second][addr];
    fEpochMarkerArray[it->second][addr] = epoch_count;
  }

}

void CbmTSUnpackSpadic20OnlineMonitor::GetEpochInfo(Int_t link, Int_t addr)
{
  printf("Get epoch info\n");
  auto it=groupToExpMap.find(link);
  if (it == groupToExpMap.end()) {
    LOG(fatal) << "Could not find an entry for equipment ID" <<
      std::hex << link << std::dec;
  } else {
    fEpochMarker = fEpochMarkerArray[it->second][addr];
    fSuperEpoch = fSuperEpochArray[it->second][addr];
  }

}

Int_t CbmTSUnpackSpadic20OnlineMonitor::GetChannelOnPadPlane(Int_t SpadicChannel, Int_t groupId)
{
  if(SpadicChannel > 31 || groupId > 1) LOG(debug) << "CbmTSUnpackSpadic20OnlineMonitor::                     ChId " << SpadicChannel << "  GroupId: " << groupId;
  SpadicChannel = groupId * 16 + SpadicChannel;
  Int_t channelMapping[32] = {31,15,30,14,29,13,28,12,27,11,26,10,25, 9,24, 8,
			      23, 7,22, 6,21, 5,20, 4,19, 3,18, 2,17, 1,16, 0};
  if (SpadicChannel < 0 || SpadicChannel > 31){
    if (SpadicChannel !=-1) LOG(debug) << "CbmTSUnpackSpadic20OnlineMonitor::GetChannelOnPadPlane ChId " << SpadicChannel << "  GroupId: " << groupId;
    return -1;
  } else {
    return channelMapping[SpadicChannel];
  }
}

Int_t CbmTSUnpackSpadic20OnlineMonitor::GetSpadicID(Int_t address)
{
  //TString spadic="";
  Int_t SpaId = -1;
  switch (address) {
  case (SpadicBaseAddress+0):  // first spadic
    //spadic="Spadic0";
    SpaId = 0;
    break;
  case (SpadicBaseAddress+1):  // first spadic
    //spadic="Spadic0";
    SpaId = 0;
    break;
  case (SpadicBaseAddress+2):  // second spadic
    //spadic="Spadic1";
    SpaId = 1;
    break;
  case (SpadicBaseAddress+3):  // second spadic
    //spadic="Spadic1";
    SpaId = 1;
    break;
  case (SpadicBaseAddress+4):  // third spadic
    //spadic="Spadic2";
    SpaId = 2;
    break;
  case (SpadicBaseAddress+5):  // third spadic
    //spadic="Spadic2";
    SpaId = 2;
    break;
  default:
    LOG(error) << "Source Address " << address << " not known.";
    break;
  }
  return SpaId;
}

Int_t CbmTSUnpackSpadic20OnlineMonitor::GetSyscoreID(Int_t /*link*/)
{
  /*
  Int_t SyscoreID=link-BaseEquipmentID;
  if((SyscoreID<0||SyscoreID>3)){
    LOG(debug) << "EqID " << link << " not known.";
    SyscoreID=-1;
  }
  return SyscoreID;
  */
  return 0;   // FIXME!!!!!! Defaults to AFCK 0!!
}
// ----              -------------------------------------------------------


inline TString CbmTSUnpackSpadic20OnlineMonitor::GetSpadicName(Int_t link,Int_t address){

  TString spadicName="";
  Int_t SpadicID=0;
  spadicName="SysCore_"+std::to_string(GetSyscoreID(link))+"_";
  SpadicID=GetSpadicID(address);

  //SpadicID/=2;

  spadicName += "Spadic_";

  spadicName+=std::to_string(SpadicID);

  return spadicName;
}

void CbmTSUnpackSpadic20OnlineMonitor::InitHistos()
{

  #ifdef USE_HTTP_SERVER
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  #endif

  //cout << "InitHistos" << endl;
  TString histName;


  for (Int_t iLink = 0; iLink < NrOfSyscores; iLink++){
    for (Int_t iAddress = 0; iAddress < NrOfSpadics; iAddress++){

    //histName.Form("SysCore_%i_Spadic_%i",iLink,iAddress);
    histName.Form("AFCK_0_Spadic_0");

    fHM->Add(TString("Baseline_"+histName).Data(),new TH2I (TString("Baseline_"+histName).Data(),TString("Baseline_"+histName).Data(), 128,-256.5,255.5,32,-0.5,31.5));
    fBaseline[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Baseline_"+histName).Data());
    fBaseline[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("ADC");
    fBaseline[(iLink)*(NrOfSpadics)+iAddress]->GetYaxis()->SetTitle("Channel ID");
    fHM->Add(TString("maxADC_vs_maxTimeBin_"+histName).Data(),new TH2I (TString("maxADC_vs_maxTimeBin_"+histName).Data(),TString("maxADC_vs_maxTimeBin_"+histName).Data(),32,-0.5,31.5, 128,-256.5,255.5));

    fmaxADCmaxTimeBin[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("maxADC_vs_maxTimeBin_"+histName).Data());
    fmaxADCmaxTimeBin[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("max. Time Bin");
    fmaxADCmaxTimeBin[(iLink)*(NrOfSpadics)+iAddress]->GetYaxis()->SetTitle("max ADC");
    fHM->Add(TString("Hit_"+histName).Data(),new TH2I (TString("Hit_"+histName).Data(),TString("Hit_"+histName).Data(),16,-0.5,15.5,2,-0.5,1.5));
    fHit[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Hit_"+histName).Data());
    fHM->Add(TString("Lost_"+histName).Data(),new TH2I (TString("Lost_"+histName).Data(),TString("Lost_"+histName).Data(),16,-0.5,15.5,2,-0.5,1.5));
    fLost[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Lost_"+histName).Data());
    fHM->Add(TString("Epoch_"+histName).Data(),new TH2I (TString("Epoch_"+histName).Data(),TString("Epoch_"+histName).Data(),16,-0.5,15.5,2,-0.5,1.5));
    fEpoch[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Epoch_"+histName).Data());
    fHM->Add(TString("OutOfSync_"+histName).Data(),new TH2I (TString("OutOfSync_"+histName).Data(),TString("OutOfSync_"+histName).Data(),16,-0.5,15.5,2,-0.5,1.5));
    fOutOfSync[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("OutOfSync_"+histName).Data());
    fHM->Add(TString("Strange_"+histName).Data(),new TH2I (TString("Strange_"+histName).Data(),TString("Strange_"+histName).Data(),16,-0.5,15.5,2,-0.5,1.5));
    fStrange[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Strange_"+histName).Data());
    fHM->Add(TString("Info_"+histName).Data(),new TH2I (TString("Info_"+histName).Data(),TString("Info_"+histName).Data(),16,-0.5,15.5,2,-0.5,1.5));
    fInfo[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Info_"+histName).Data());
    fTSGraph[(iLink)*(NrOfSpadics)+iAddress] = new TGraph();

    fHM->Add(TString("Sync_"+histName).Data(),new TH2I (TString("Sync_"+histName).Data(),TString("Sync_"+histName).Data(), 4096,-0.5,4095.5,32,-0.5,31.5));
    fHitSync[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Sync_"+histName).Data());

    fHM->Add(TString("HitTimeA_"+histName).Data(),new TH1I (TString("HitTimeA_"+histName).Data(),TString("HitTimeA_"+histName).Data(),300,-299.5,0.5));
    fHitTimeA[(iLink)*(NrOfSpadics)+iAddress]=(TH1I*)fHM->H1(TString("HitTimeA_"+histName).Data());
    fHitTimeA[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("SuperEpoch count");
    fHM->Add(TString("HitTimeB_"+histName).Data(),new TH1I (TString("HitTimeB_"+histName).Data(),TString("HitTimeB_"+histName).Data(),300,-299.5,0.5));
    fHitTimeB[(iLink)*(NrOfSpadics)+iAddress]=(TH1I*)fHM->H1(TString("HitTimeB_"+histName).Data());
    fHitTimeB[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("SuperEpoch count");
    fHitTimeB[(iLink)*(NrOfSpadics)+iAddress]->SetLineColor(2);
    fHM->Add(TString("MessageStatistic_"+histName).Data(),new TH1I (TString("MessageStatistic_"+histName).Data(),TString("MessageStatistic_"+histName).Data(),24,-0.5,23.5));
    fMessageStatistic[(iLink)*(NrOfSpadics)+iAddress]=(TH1I*)fHM->H1(TString("MessageStatistic_"+histName).Data());
    for (Int_t iM = 0; iM < 7; iM++){
    	fMessageStatistic[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetBinLabel(iM+1,fMessageTypes[iM]);
    }
    for (Int_t iT = 0; iT < 4; iT++){
    	fMessageStatistic[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetBinLabel(7+1+iT,fTriggerTypes[iT]);
    }
    for (Int_t iS = 0; iS < 6; iS++){
    	fMessageStatistic[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetBinLabel(7+4+1+iS,fStopTypes[iS]);
    }
    for (Int_t iI = 0; iI < 7; iI++){
    	fMessageStatistic[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetBinLabel(7+4+6+1+iI,fInfoTypes[iI]);
    }

      // const Int_t nBins=70;
      // auto BinBoarders = [&nBins] (Int_t reBinHigh=32){
      // //auto BinBoarders = [&nBins] (Int_t reBinLow=32){
      // const Double_t Threshold = 100.;
      // const Double_t MaxFreq = (17500000.0);
      // const Double_t slope=MaxFreq/nBins;
      // const Int_t iThreshhold=std::ceil((MaxFreq/Threshold-0.5)/static_cast<Double_t>(reBinHigh));
      // LOG(fatal) << "iThresshold = " << iThreshhold;
      // Double_t* Result=new Double_t[nBins+1];
      //
      // for(Int_t i=0;i<nBins;i++){
      //  size_t j=nBins-i;
      //  //Result[j]=TMath::Power(TMath::E(),i*slope+TMath::Log(2));
      //  //printf("%d\n",Result[j]);
      //  Double_t LowEdge = MaxFreq/(reBinHigh*(i*i)+0.5);
      //  Result[j]=LowEdge;
      // }/*
      //   for(Int_t i=0;i<nBins-iThreshhold;i++) {
      //   size_t j=nBins-iThreshhold-i;
      //   Double_t LowEdge = MaxFreq/(reBinHigh*(iThreshhold)+0.5+RebinLow*(i));
      //   Result[j]=LowEdge;
      //   }*/
      // Result[0]=0.0;
      // std::sort(Result,&Result[nBins]);
      // return Result;
      // };
      // Double_t* freqbins = (BinBoarders());

      const Int_t nBins = 234;
      // const Double_t maxFreq = (175000000.0); (VF) unused
      auto BinBoarders = [/*&nBins*/] (Int_t /*reBinHigh*/=32)
      	{
      	  const Double_t MaxFreq = (175000000.0);
      	  // const Double_t slope=10;//MaxFreq/nBins; (VF) unused
      	  Double_t* Result=new Double_t[nBins+1];
      	  for(Int_t i=0;i<nBins;i++)
      	    {
      	      size_t j=nBins-i;
      	      Double_t LowEdge = MaxFreq/(TMath::Power(TMath::E(),(0.1*Int_t(i))));
      	      //printf("LowEdge: %e\n",LowEdge);
      	      Result[j]=LowEdge;
      	      //Result[j]=TMath::Power(TMath::E(),i*slope+TMath::Log(2));
      	    }
      	  Result[0]=0.1;
      	  std::sort(Result,&Result[nBins]);
      	  return Result;
      	};
      Double_t* freqbins = (BinBoarders());
      fHM->Add(TString("HitFrequency_"+histName).Data(),new TH1I (TString("HitFrequency_"+histName).Data(),TString("HitFrequency_"+histName).Data(),nBins,freqbins/*100000,1,10000000*/));
      fHitFrequency[(iLink)*(NrOfSpadics)+iAddress]=(TH1I*)fHM->H1(TString("HitFrequency_"+histName).Data());
      fHitFrequency[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("Trigger frequency (Hz)");
      fHitFrequency[(iLink)*(NrOfSpadics)+iAddress]->GetYaxis()->SetTitle("Channel ID");
      fHM->Add(TString("Spectrum_"+histName).Data(),new TH1I (TString("Spectrum_"+histName).Data(),TString("Spectrum_"+histName).Data(),513,-256.5,256.5));
      fSpectrum[(iLink)*(NrOfSpadics)+iAddress]=(TH1I*)fHM->H1(TString("Spectrum_"+histName).Data());
      fSpectrum[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("max. ADC (ADC)");



      for (Int_t iCh = 0; iCh < 32; iCh++){
      	TString chName;
      	chName.Form("Ch%02i_",iCh);
        // FIXME!!!
      	fHM->Add(TString("PulseShape_"+chName+histName).Data(),new TH2I (TString("PulseShape_"+chName+histName).Data(),TString("PulseShape_"+chName+histName).Data(),32,-0.5,31.5, 128,-256.5,255.5));
      	fPulseShape[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]=(TH2I*)fHM->H2(TString("PulseShape_"+chName+histName).Data());
      	fPulseShape[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->GetXaxis()->SetTitle("Time Bin");
      	fPulseShape[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->GetYaxis()->SetTitle("ADC");
      	fPulseShape[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->GetZaxis()->SetTitle("counts");
      }

      for (Int_t iCh = 0; iCh < 32; iCh++){
      	TString chName;
      	chName.Form("Ch%02i_",iCh);
        // FIXME!!!
      	fHM->Add(TString("MaxADC_"+chName+histName).Data(),new TH1D (TString("MaxADC_"+chName+histName).Data(),TString("MaxADC_"+chName+histName).Data(),256,-256.5,255.5));
      	fMaxADC[iCh]=(TH1D*)fHM->H1(TString("MaxADC_"+chName+histName).Data());
      	fMaxADC[iCh]->GetXaxis()->SetTitle("ADC");
      	fMaxADC[iCh]->GetYaxis()->SetTitle("counts");
      }

    }
  }
}




void CbmTSUnpackSpadic20OnlineMonitor::InitCanvas()
{
  TString cName;
  if (!fHighPerformance){
    fcE= new TCanvas(TString("EpochMap").Data(),TString("EpochMap").Data(),1600,1200);
    fcE->Divide(3,4);
    fcO= new TCanvas(TString("OutOfSyncMap").Data(),TString("OutOfSyncMap").Data(),1600,1200);
    fcO->Divide(3,4);
    fcS= new TCanvas(TString("StrangeMap").Data(),TString("StrangeMap").Data(),1600,1200);
    fcS->Divide(3,4);
    fcI= new TCanvas(TString("InfoMap").Data(),TString("InfoMap").Data(),1600,1200);
    fcI->Divide(3,4);
    fcF= new TCanvas(TString("HitFrequency").Data(),TString("HitFrequency").Data(),1600,1200);
    fcF->Divide(3,4);
    fcSp= new TCanvas(TString("HitSelfTriggersSpectrum").Data(),TString("HitSelfTriggersSpectrum").Data(),1600,1200);
    fcSp->Divide(3,4);
  }
  fcB = new TCanvas(TString("Baseline").Data(),TString("Baseline").Data(),1600,1200);
  fcB->Divide(3,4);
  fcM/*[(iLink)*(NrOfSpadics)+iAddress]*/ = new TCanvas(TString("maxADC_vs_maxTimeBin").Data(),TString("maxADC_vs_maxTimeBin").Data(),1600,1200);
  fcM->Divide(3,4);
  fcH= new TCanvas(TString("HitMap").Data(),TString("HitMap").Data(),1600,1200);
  fcH->Divide(3,4);
  fcL= new TCanvas(TString("LostMap").Data(),TString("LostMap").Data(),1600,1200);
  fcL->Divide(3,4);
  fcTS= new TCanvas(TString("TSGraph").Data(),TString("TSGraph").Data(),1600,1200);
  fcTS->Divide(3,4);
  fcMS= new TCanvas(TString("MessageStatistic").Data(),TString("MessageStatistic").Data(),1600,1200);
  fcMS->Divide(3,4);

  for (Int_t iLink = 0; iLink < NrOfSyscores; iLink++){
    for (Int_t iAddress = 0; iAddress < NrOfSpadics; iAddress++){
      cName.Form("SysCore_%i_Spadic_%i",iLink,iAddress);
      if (!fHighPerformance){
      	fcPS[(iLink)*(NrOfSpadics)+iAddress] = new TCanvas(TString("PulseShapes_"+cName).Data(),TString("PulseShapes_"+cName).Data(),1600,1200);
      	fcPS[(iLink)*(NrOfSpadics)+iAddress]->Divide(4,8);
	      for(Int_t iCh = 0; iCh < 32; iCh++){
      	  fcPS[(iLink)*(NrOfSpadics)+iAddress]->cd(iCh+1);
      	  //fPulseShape[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->Draw("colz");
          fPulseShape[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->Draw("col");
      	}
        for(Int_t iCh = 0; iCh < 32; iCh++){
      	  fcPS[(iLink)*(NrOfSpadics)+iAddress]->cd(iCh+1);
          fMaxADC[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->Draw("");
      	}
      	fcE->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(0);
      	fEpoch[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      	fcO->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(0);
      	fOutOfSync[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      	fcS->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(0);
      	fStrange[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      	fcI->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogx(0);
      	fInfo[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      	fcF->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogx(1);
      	fcF->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogy(1);
      	fcF->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(0);
      	fHitFrequency[(iLink)*(NrOfSpadics)+iAddress]->Draw(/*"colz"*/);
      	fcSp->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogy(1);
      	fSpectrum[(iLink)*(NrOfSpadics)+iAddress]->Draw();
	fHitSync[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      }
      fcB->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz();
      fBaseline[(iLink)*(NrOfSpadics)+iAddress]->Draw("col");
      cName.Form("AFCK_%i_Spadic_%i",iLink,iAddress);
      fcM->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz();
      fmaxADCmaxTimeBin[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcH->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(0);
      fHit[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcL->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(0);
      fLost[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcMS->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogy(1);
      fMessageStatistic[(iLink)*(NrOfSpadics)+iAddress]->Draw("");
      fcTS->cd((iLink)*(NrOfSpadics)+iAddress+1);
      fHitTimeA[(iLink)*(NrOfSpadics)+iAddress]->GetYaxis()->SetRangeUser(0,1000);
      fHitTimeA[(iLink)*(NrOfSpadics)+iAddress]->Draw("");
      fHitTimeB[(iLink)*(NrOfSpadics)+iAddress]->Draw("same");
      //fTSGraph[(iLink)*(NrOfSpadics)+iAddress]->Draw("ALP");
      fHitSync[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
    }
  }

}



void CbmTSUnpackSpadic20OnlineMonitor::UpdateCanvas()
{
  /*
    //cout << "UpdateCanvas" << endl;
    TH2I* h = NULL;
    TString cName;
    for (Int_t iLink = 0; iLink < NrOfSyscores; iLink++){
      for (Int_t iAddress = 0; iAddress < NrOfSpadics; iAddress++){
        cName.Form("AFCK_%i_Spadic_%i",iLink,iAddress);
        fcB->cd((iLink)*(NrOfSpadics)+iAddress+1);
        fBaseline[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
        h = (TH2I*)fHM->H2(TString("Baseline_"+cName).Data());
        h->Draw("colz");
        fcB->cd((iLink)*(NrOfSpadics)+iAddress+1)->Update();
        cName.Form("AFCK_%i_Spadic_%i",iLink,iAddress);
        fcM->cd((iLink)*(NrOfSpadics)+iAddress+1);
        fmaxADCmaxTimeBin[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
        h = (TH2I*)fHM->H2(TString("maxADC_vs_maxTimeBin_"+cName).Data());
        h->Draw("colz");
        fcM->cd((iLink)*(NrOfSpadics)+iAddress+1)->Update();
      }
    }
    */
}

void CbmTSUnpackSpadic20OnlineMonitor::Reset()
{
  fSpadicRaw->Clear();
}
void CbmTSUnpackSpadic20OnlineMonitor::FinishEvent()
{
  //UpdateCanvas();
}

void CbmTSUnpackSpadic20OnlineMonitor::Finish()
{
  for (Int_t i=0; i < NrOfSyscores; ++i) {
    for (Int_t j=0; j < NrOfHalfSpadics; ++j) {
      LOG(debug) << "There have been " << fSuperEpochArray[i][j]
		 << " SuperEpochs for Syscore" << i << "_Spadic"
		 << j << " in this file";
    }
  }
  UpdateCanvas();
}


  /*
    void CbmTSUnpackSpadic20OnlineMonitor::Register()
    {
    }
  */


  ClassImp(CbmTSUnpackSpadic20OnlineMonitor)
