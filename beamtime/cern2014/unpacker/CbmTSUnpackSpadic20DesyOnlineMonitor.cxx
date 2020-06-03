// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadic20OnlineMonitor                       -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmTSUnpackSpadic20DesyOnlineMonitor.h"

#include "CbmSpadicRawMessage.h"

#include "TimesliceReader20.hpp"
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



CbmTSUnpackSpadic20DesyOnlineMonitor::CbmTSUnpackSpadic20DesyOnlineMonitor(Bool_t highPerformance)
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
    fcSy{NULL},
    fcNS{nullptr},
    fcTH{nullptr},
    fcAS(NULL),
    fcA{NULL},
    fcES{nullptr},
    fcHC{nullptr},
    fBaseline{NULL},
    fmaxADCmaxTimeBin{NULL},
    fHit{NULL},
    fHitChannel{NULL},
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
    fNrSamples{NULL},
    fTimeBetweenHits{NULL},
    fADCmaxSum{NULL},
    fTSGraph{NULL},
    fLastSuperEpochA{0},
    fLastSuperEpochB{0},
    fLastFullTime{{{0}}},
    fHitsTrd0{{}},
    fHitsTrd1{{}},
    fHitsTrd1n{{}},
    fHitsTrd2{{}},
    fLastTimeGlobal{0},
    fThisTime{0},
    fAlignmentCounter0{0},
    fAlignmentCounter1{0},
    fAlignmentCompareCounter{0},
    fAlignmentCompareCounter1{0},
    fLastTimes{},
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
    fClusterSwitch{},
    fMaxSum{},
    fLastTrigger{},
    fLastChannel{},
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
   fAlignments{"0_1",
	"1_2",
	"2_3"},
    fChambers{"0",
	"1",
	"2",
	"3"},
    //    fAFCKs{24465,0x1889,43522,43523}, // Insert AFCK Adresses!!!!!!!!! 6281 22465
    fAFCKs{43520,43521,43522,43523}, // Insert AFCK Adresses!!!!!!!!! 6281 22465
    fSource{-1},
    fLastSource{-1},
    fFillCounter{0},
    fSyncChambers{NULL},
    fEventSelection{NULL},
    fAlignment{NULL},
    fMaxADC{NULL}
{
//   for (Int_t i=0; i < NrOfAfcks; ++i) {
//     for (Int_t j=0; j < NrOfHalfSpadics; ++j) {
//       fEpochMarkerArray[i][j] = 0;
//       fPreviousEpochMarkerArray[i][j] =0;
//       fSuperEpochArray[i][j] = 0;
//     }
//  }
//  }
  fLastTimeGlobal=0;
  fThisTime=0;
  fSource=-1;
  fLastSource=-1;
  fAlignmentCounter0=0;
  fAlignmentCounter1=0;
  fAlignmentCompareCounter=0;
  fFillCounter=0;
  InitHistos();
  InitCanvas();
}

CbmTSUnpackSpadic20DesyOnlineMonitor::~CbmTSUnpackSpadic20DesyOnlineMonitor()
{
	LOG(info) << "Number of extraneous Samples "<< fNrExtraneousSamples;
}

Bool_t CbmTSUnpackSpadic20DesyOnlineMonitor::Init()
{
	LOG(info) << "Initializing";

	FairRootManager* ioman = FairRootManager::Instance();
	if (ioman == NULL) {
	  LOG(fatal) << "No FairRootManager instance";
	}
	ioman->Register("SpadicRawMessage", "spadic raw data", fSpadicRaw, kTRUE);
	return kTRUE;
}



Bool_t CbmTSUnpackSpadic20DesyOnlineMonitor::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  // compare to: https://github.com/spadic/spadic10-software/blob/master/lib/message/message.h
  // or fles/spadic/message/wrap/cpp/message.cpp
  LOG(debug) << "Unpacking Spadic Data";
  Int_t link = 0;
  // TH2I* h = NULL;  (VF) not used
  TString cName;

  LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
             << "microslices.";

  //  std::cout << "Timeslice contains " << ts.num_microslices(component) << std::endl; //only turned off for debugging!!

  //printf("Component: %i Num MS: %i \n", component, ts.num_microslices(component));

  // Timeslice reader for spadic 2.0
  //spadic::TimesliceReader20 tsReader;
  //tsReader.add_component(ts, component);
  /*
  for (auto addr : tsReader.sources()) {
    std::cout << "---- reader " << addr << " ----" << std::endl;
    while (auto mp = tsReader.get_message(addr)) {
      //print_message(*mp);
      auto& s = mp->samples();
      if ( mp->is_info() )
	      std::cout << " This is a info message" << std::endl;
      printf("Group: %i Ch: %i Ts: %i Samples: %i Trace: ", mp->group_id(), mp->channel_id(), mp->timestamp(),  s.size());
      for(int i = 0; i < s.size(); i++)
           printf(" %02i,", s[i]);
      printf("\n");
      // Process the Spadic 2.0 message ;)
    }
  }
  */

  //Int_t link = ts.descriptor(component, 0).eq_id;
  //printf("Link: %x\n", link);
  //Int_t address = addr;

  Int_t address = 0; // Fake the addr for now
    //Int_t addr = 0; // Fake!
    Int_t counter=0;
    /*
   // Loop over microslices
  //for (size_t m = 0; m < ts.num_microslices(component); ++m){
  for (size_t m {0}; m < ts.num_microslices(component); m++){

    //spadic::MessageReader r;

    //std::vector<uint16_t> buf0;
    //std::vector<uint16_t> buf1;
    //    std::vector<uint16_t> buf2;
    //    std::vector<uint16_t> buf3;
    //buf0.reserve(20000);
    //buf1.reserve(20000);

    // Get the microslice
    //auto mc = ts.get_microslice(component, m);
    //auto mv = reinterpret_cast<const fles::MicrosliceView&>(mc);
    //auto mv_p = reinterpret_cast<const uint16_t *>(mv.content());        // Pointer to the data
    //auto mv_s = mv.desc().size * sizeof(*mv.content()) / sizeof(*mv_p);  // Size

    //printf("Component: %x\n", component);

    for(auto p=0; p<mv_s; p++){
      printf("%04x ", mv_p[p]);
    }
    printf("\n");

    //continue;

    //const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( mv_p );

    // ----------------
    // Get the microslice

    auto msDescriptor =  ts.descriptor(component, m);   // mc
    Int_t fEquipmentId = msDescriptor.eq_id;
    //auto msContent = reinterpret_cast<const uint16_t*>(ts.content(component, m));
    const uint8_t* msContent = ts.content(component, m);
    auto s = msDescriptor.size * sizeof(*msContent) / sizeof(*msContent);
    const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent );

    for (auto uIdx = 0; uIdx < s; uIdx ++){
    //for (auto uIdx = 0; uIdx < mv_s; uIdx ++){
      uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
      //uint64_t ulData = static_cast<uint64_t>( mv_p[uIdx] );
      //auto ulData = mv_p[uIdx];
      //printf("%016x ", ulData);
      uint16_t afck_id = (ulData & 0xffff000000000000) >> 48;
      // Last 2bytes of mac addr of the afck, hard coded now for sps2016
      //if(afck_id != 0x5b9d) //IRI AFCK
      //if(afck_id != 0x187f)  // IKF 217
      //if(afck_id != 0x5f91 && afck_id != 1889){ // if not from AFCK 0 or AFCK 1


      if(afck_id != 0x5f91){ // if not from AFCK 0
	//continue;
      }

      //uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
      uint8_t  downlink_header = (ulData & 0xf00000) >> 20;
      uint8_t  downlink_nr     = (ulData & 0xf0000)  >> 16;
      uint16_t payload         =  ulData & 0xffff;
      //if(downlink_header != 0xa)
      //      std::cout << "Corruption in downlink header found" << std::endl;

      //printf("%04x : %04X\n", afck_id, payload);

      // Put the data in a buffer
      //if(downlink_nr == 0)
      //      buf0.push_back(payload);
      //else if(downlink_nr == 1)
      //      buf1.push_back(payload);
     }
    }
*/
    // This is not nice... quick n' dirty
    //std::move(buf1.begin(), buf1.end(), std::back_inserter(buf0));
    //r.add_buffer(buf0.data() , buf0.size());
    //r.add_buffer(buf1.data() , buf1.size());
    //    r.add_buffer(buf2.data() , buf2.size());
    //    r.add_buffer(buf3.data() , buf3.size());


  spadic::TimesliceReader20 tsReader;
  tsReader.add_component(ts, component);
  for (auto addr : tsReader.sources()) {
    //    std::cout << "---- reader " << std::hex << addr << " ----comp " << std::hex << component<< std::endl;

    while (auto m = tsReader.get_message(addr)) {
      //  Int_t SpadicAddress = tsReader.get_link(addr)/(256*8);
      //    if((int)tsReader.get_link(addr)== 1024){
       //	printf("Double Hit noticed! tsReader.get_link(addr): %i \n", tsReader.get_link(addr));
      //	continue;
      // }
      Int_t groupId = (((int)tsReader.get_link(addr))%2048)/256;
      Int_t SpadicAddress = tsReader.get_link(addr)/2048;
      Int_t HalfSpadicAddress = 2*SpadicAddress + groupId;
      Int_t AfckAddress = ts.descriptor(component,0).eq_id;


	//groupId = 0;
      /*   if ( (SpadicAddress) == 0x0c00 ){
	std::cout << "False addr" <<std::endl;
	continue;
	}*/
      //      Int_t AfckAddress = addr - tsReader.get_link(addr)/256;
	// Int_t AfckAddress = 9000;
      //std::cout << addr << " " <<tsReader.get_link(addr) << " " <<std::endl;
      /*      if ((AfckAddress == 6289) or (AfckAddress == 6290)){
	  AfckAddress = 6281;
	}else{ AfckAddress = 0;
	}
      */
       Int_t AfckID = -1;
       for(int i = 0; i < 4; i++){
	if (AfckAddress == fAFCKs[i]){
	  AfckID = i;
	  break;
	}
       }

       //    AfckID = 0;

        link = ts.descriptor(component, 0).eq_id;
	//	printf("Addr: %x, AfckID: %i, Spadic Address: %i, Group ID: %i, tsReader.get_link(addr): %x, Eq Id: %x", addr, AfckID, SpadicAddress, groupId, tsReader.get_link(addr),ts.descriptor(component, 0).eq_id);
	//	  printf("\n");

    // @FIXME

    // std::cout << std::hex << link << std::endl;
    //} // End microslice

  Bool_t isInfo(false), isHit(false), isEpoch(false), isEpochOutOfSync(false), isOverflow(false), isHitAborted(false), isStrange(false);

  //while(auto m = r.get_message()) {


    if(m->is_hit()){
      // auto& s = m->samples(); (VF) not used
      GetEpochInfo(HalfSpadicAddress, AfckID);

      // Fill some nice histos
      fSource = AfckID * NrOfSpadics + (Int_t)(SpadicAddress);
      Int_t triggerType =  static_cast<Int_t>(m->hit_type());
      Int_t stopType = static_cast<Int_t>(m->stop_type());
      Int_t time = m->timestamp();
      fThisTime= ((fEpochMarker << 12) + time);
      // printf("Timestamp: %i, Epoch: %i, fThisTime: %i \n", time, fEpochMarker, fThisTime);
      Int_t infoType = -1;
      Int_t channel = m->channel_id();
      //   Int_t groupId = m->group_id();
      //std::cout << "GROUP ID "<< groupId <<std::endl;
      Int_t bufferOverflowCounter = 0;
      Int_t samples = m->samples().size();
      Int_t padRow(-1);
      Int_t padChannel = GetChannelOnPadPlane(channel,groupId);
      Int_t counter1=0;
      Int_t maxADC(-256), maxTB(-1);
      // Int_t maxADCsum(-256);  (VF) not used
      // Int_t adc(-256);  (VF) not used
      // Int_t base(-256);  (VF) not used
      // ULong_t timediff= fThisTime-fLastTimes[fSource];  (VF) not used
      // Bool_t fillCluster=kFALSE;  (VF) not used
      Int_t* sample_values = new Int_t[samples];
      // Int_t baseline=-1;   (VF) not used
      // printf("Afck Address: %i Source : %i \n", AfckAddress, fSource);
      // if(samples == 0){
      // 	continue;}
      if((triggerType == 1) or (triggerType == 0)){
      for (auto x : m->samples()) {
	sample_values[counter1] = static_cast<int>(x);
	//if (!fHighPerformance)
	fPulseShape[(fSource)*32+channel+16*(groupId%2) ]->Fill(counter1,x);
	if(counter1==0)	fBaseline[fSource]->Fill(x,groupId*16+channel);
	//fPulseShape[channel+16*(groupId%2)]->Fill(counter1,x);
	//}
	if (x > maxADC){
	  maxADC = x;
	  maxTB = counter1;
	}
	++counter1;
      }
      //      fMaxADC[(fSource)*32+channel+16*(groupId%2)]->Fill(maxADC);
       }
       new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
       	CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
       			    fSuperEpoch, triggerType, infoType, stopType, groupId,
       			    bufferOverflowCounter, samples, sample_values,
       			    isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);
       static ULong_t firstTimeStamp=0;
       if(firstTimeStamp==0){
	 //	 firstTimeStamp=raw->GetFullTime();
       }


      //    printf("Source: %i, AFCK Address: %i, AFCK Id: %i, SPADIC Address: %i Ch: %i Ts: %i hit type: %i Stop type: %i Samples: %i Trace:   ADCmax: %i    Trigger: %i    Base: %i \n", fSource,  AfckAddress, AfckID, SpadicAddress, m->channel_id(), m->timestamp(),  m->hit_type(), m->stop_type(), s.size(), maxADCsum, triggerType, base);
      //  printf("Channel: %i, Group ID: %i \n", channel, groupId);


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
      	fHitSync[fSource]->Fill(time, groupId*16+channel); // when startTime should be used, its x=time-startTime here
      }



      //MaxADC vs Max TimeBin Histo
      fmaxADCmaxTimeBin[fSource]->Fill(maxTB,maxADC);


       //Nr of Samples vs Stop Type Histo
      fNrSamples[fSource]->Fill(samples,stopType);

      // Sync of Chambers Histo
      fSyncChambers[0]->Fill(fThisTime-fLastTimeGlobal,fSource); // not sure if this gives us the right Number of the Chamber
      fLastTimeGlobal=fThisTime;

      //Time between Hits Histo
           fTimeBetweenHits[fSource]->Fill(fThisTime-fLastTimes[fSource]);


      //Information for the next message
      fLastTimes[fSource]=fThisTime;
      fLastTrigger[fSource]=triggerType;
      fLastChannel[fSource]=channel;


      //Hitmap Histo
      if (padChannel > 15){
	padChannel -= 16;
	padRow = 1;
      } else {
	padRow = 0;
      }
      fHit[fSource]->Fill(padChannel,padRow);
      fHitChannel[fSource]->Fill(channel,groupId);
      // printf("fSource: %i, Group ID: %i \n", fSource, groupId);
      //Max ADC Spectrum
      fMaxADC[channel+16*(groupId%2)]->Fill(maxADC);
      cName.Form("SysCore_%i_Spadic_%i",0,0);
  	  //h = (TH2I*)fHM->H2(TString("Baseline_"+cName).Data());
  	  //h->Fill(sample_values[0],groupId*16+channel);
      /*
      if(sample_values[0] !=0)
	fBaseline[fSource]->Fill( m->samples()[0],groupId*16+channel);
      */

      //   Alignment Histo 0_1             marker1
      if((triggerType == 1) or (triggerType == 3)){
	if((fSource == 0) or (fSource == 3)){
	   if (fLastSource == fSource){
	    if (fSource == 0){ // Source 0 should correspond to TRD0
	      fHitsTrd0[fAlignmentCounter0][0] = fThisTime;
	      //	    printf("This Time: %i \n", fThisTime);
	      fHitsTrd0[fAlignmentCounter0][1] = padChannel + 16 * padRow; //  channel+16*(groupId%2);
	      //	printf("Hit bei fSource = 0, Time: %i, AlignmentCounter: %i, Pad: %i, Channel: %i \n", fHitsTrd0[fAlignmentCounter0][0] ,fAlignmentCounter0, padChannel, channel+16*(groupId%2));
	      fAlignmentCounter0++;
	    }  else if (fSource == 3){ // Source 3 should correspond to TRD1
	      fHitsTrd1[fAlignmentCounter0][0] = fThisTime;
	      //	  printf("This Time: %i \n", fThisTime);
	      fHitsTrd1[fAlignmentCounter0][1] =padChannel + 16 * padRow; // channel+16*(groupId%2);
	      //	printf("Hit bei fSource = 3, Time: %i, AlignmentCounter: %i, Pad: %i Channel: %i \n", fHitsTrd1[fAlignmentCounter0][0],fAlignmentCounter0, padChannel, channel+16*(groupId%2));
	      fAlignmentCounter0++;
	    }
	  } else {
	    // Now both Alignment Arrays have to be compared
	    fAlignmentCompareCounter=0;
	    if(fLastSource == 0){
	      // printf("Last Source was 0, starting comparison! \n");
	      // printf("Alignment Counter: %i   fSource:  %i    fLastSource:   %i, AlignmentCompareCounter: %i \n", fAlignmentCounter0,  fSource, fLastSource, fAlignmentCompareCounter);
	      // for(int i=0; i < fAlignmentCounter0; i++){
	      // 	printf("fHitsTrd0: %i, Channel: %i, fHitsTrd1: %i, Channel: %i \n", fHitsTrd0[i][0], fHitsTrd0[i][1], fHitsTrd1[i][0], fHitsTrd1[i][1]);
	      // }
	      for(ULong_t i=0; i < fAlignmentCounter0; i++){
		//	  printf(" fHitsTrd0: %i, fHitsTrd1: %i, AlignmentCompareCounter: %i, i: %i \n",fHitsTrd0[i][0],fHitsTrd1[fAlignmentCompareCounter][0], fAlignmentCompareCounter, i);
		while(fHitsTrd0[i][0] > fHitsTrd1[fAlignmentCompareCounter][0]){
		  fAlignmentCompareCounter++;
		  if(fAlignmentCompareCounter <10){
		    //     printf("INCREASING AlignmentCompareCounter");
		  }
		}
		if (fHitsTrd0[i][0] == fHitsTrd1[fAlignmentCompareCounter][0]){
		  fAlignment[0]->Fill(fHitsTrd0[i][1],fHitsTrd1[fAlignmentCompareCounter][1]);
		  // printf("Coincidence at Channels: %i and %i! \n",fHitsTrd0[i][1],fHitsTrd1[fAlignmentCompareCounter][1]);
		  // printf("At Time: %i = %i \n", fHitsTrd0[i][0], fHitsTrd1[fAlignmentCompareCounter][0]);
		}
	      }
	      for(ULong_t i = 0; i <= fAlignmentCounter0; i++){
		fHitsTrd1[i][0]=0;
	      }
	      fHitsTrd0[0][0] = fThisTime;
	      fHitsTrd0[0][1] =padChannel + 16 * padRow; // channel+16*(groupId%2);
	      //fAlignmentCounter0++;
	    }
	    if(fLastSource == 3){
	      // printf("Last Source was 3, starting comparison! \n");
	      // printf("Alignment Counter: %i   fSource:  %i    fLastSource:   %i, AlignmentCompareCounter: %i \n", fAlignmentCounter0,  fSource, fLastSource, fAlignmentCompareCounter);
	      //  for(int i=0; i < fAlignmentCounter0; i++){
	      // printf("fHitsTrd0: %i, Channel: %i, fHitsTrd1: %i, Channel: %i \n", fHitsTrd0[i][0], fHitsTrd0[i][1], fHitsTrd1[i][0], fHitsTrd1[i][1]);
	      //  }
	      for(ULong_t i=0; i < fAlignmentCounter0; i++){
		//	  printf(" fHitsTrd0: %i, fHitsTrd1: %i, AlignmentCompareCounter: %i, i: %i \n",fHitsTrd0[fAlignmentCompareCounter][0],fHitsTrd1[i][0],  fAlignmentCompareCounter, i);
		while(fHitsTrd1[i][0] > fHitsTrd0[fAlignmentCompareCounter][0]){
		  fAlignmentCompareCounter++;
		  if(fAlignmentCompareCounter <10){
		    //      printf("INCREASING AlignmentCompareCounter");
		  }
		}
		if (fHitsTrd1[i][0] == fHitsTrd0[fAlignmentCompareCounter][0]){
		  fAlignment[0]->Fill(fHitsTrd0[fAlignmentCompareCounter][1],fHitsTrd1[i][1]);
		  //    printf(" fHitsTrd0: %i, fHitsTrd1: %i \n",fHitsTrd0[fAlignmentCompareCounter][0],fHitsTrd1[i][0]);
		  // printf("Coincidence at Channels: %i and %i! \n",fHitsTrd0[fAlignmentCompareCounter][1],fHitsTrd1[i][1]);
		  // printf("At Time: %i = %i \n", fHitsTrd0[fAlignmentCompareCounter][0], fHitsTrd1[i][0]);
		}
	      }
	      for(ULong_t i = 0; i <= fAlignmentCounter0; i++){
		fHitsTrd0[i][0]=0;
	      }
	      fHitsTrd1[0][0] = fThisTime;
	      fHitsTrd1[0][1] = padChannel + 16 * padRow; // channel+16*(groupId%2);
	      // fAlignmentCounter0++;
	    }
	    fLastSource = fSource;
	    fAlignmentCounter0 = 1;
	  }
	}
      }





      //Alignment Histo 1_2

      // if((triggerType == 1) or (triggerType == 3)){
      // 	if((fSource == 3) or (fSource == 6)){
      // 	   if (fLastSource == fSource){
      // 	    if (fSource == 3){ // Source 3 should correspond to TRD1
      // 	      fHitsTrd1n[fAlignmentCounter1][0] = fThisTime;
      // 	      fHitsTrd1n[fAlignmentCounter1][1] = padChannel + 16 * padRow; //  channel+16*(groupId%2);
      // 	      //	printf("Hit bei fSource = 0, Time: %i, AlignmentCounter: %i, Pad: %i, Channel: %i \n", fHitsTrd0[fAlignmentCounter1][0] ,fAlignmentCounter1, padChannel, channel+16*(groupId%2));
      // 	      fAlignmentCounter1++;
      // 	    }  else if (fSource == 6){ // Source 3 should correspond to TRD1
      // 	      fHitsTrd2[fAlignmentCounter1][0] = fThisTime;
      // 	      fHitsTrd2[fAlignmentCounter1][1] =padChannel + 16 * padRow; // channel+16*(groupId%2);
      // 	      //	printf("Hit bei fSource = 3, Time: %i, AlignmentCounter: %i, Pad: %i Channel: %i \n", fHitsTrd1[fAlignmentCounter1][0],fAlignmentCounter1, padChannel, channel+16*(groupId%2));
      // 	      fAlignmentCounter1++;
      // 	    }
      // 	  } else {
      // 	    // Now both Alignment Arrays have to be compared
      // 	    fAlignmentCompareCounter=0;
      // 	    if(fLastSource == 3){
      // 	      // printf("Last Source was 0, starting comparison! \n");
      // 	      // printf("Alignment Counter: %i   fSource:  %i    fLastSource:   %i, AlignmentCompareCounter: %i \n", fAlignmentCounter1,  fSource, fLastSource, fAlignmentCompareCounter);
      // 	      // for(int i=0; i < fAlignmentCounter1; i++){
      // 	      // 	printf("fHitsTrd0: %i, Channel: %i, fHitsTrd1: %i, Channel: %i \n", fHitsTrd0[i][0], fHitsTrd0[i][1], fHitsTrd1[i][0], fHitsTrd1[i][1]);
      // 	      // }
      // 	      for(int i=0; i < fAlignmentCounter1; i++){
      // 		//	  printf(" fHitsTrd0: %i, fHitsTrd1: %i, AlignmentCompareCounter: %i, i: %i \n",fHitsTrd0[i][0],fHitsTrd1[fAlignmentCompareCounter][0], fAlignmentCompareCounter, i);
      // 		while(fHitsTrd1n[i][0] > fHitsTrd2[fAlignmentCompareCounter][0]){
      // 		  fAlignmentCompareCounter++;
      // 		  if(fAlignmentCompareCounter <10){
      // 		    //     printf("INCREASING AlignmentCompareCounter");
      // 		  }
      // 		}
      // 		if (fHitsTrd1n[i][0] == fHitsTrd2[fAlignmentCompareCounter][0]){
      // 		  fAlignment[1]->Fill(fHitsTrd1n[i][1],fHitsTrd2[fAlignmentCompareCounter][1]);
      // 		  // printf("Coincidence at Channels: %i and %i! \n",fHitsTrd0[i][1],fHitsTrd1[fAlignmentCompareCounter][1]);
      // 		  // printf("At Time: %i = %i \n", fHitsTrd0[i][0], fHitsTrd1[fAlignmentCompareCounter][0]);
      // 		}
      // 	      }
      // 	      for(int i = 0; i <= fAlignmentCounter1; i++){
      // 		fHitsTrd2[i][0]=0;
      // 	      }
      // 	      fHitsTrd1n[0][0] = fThisTime;
      // 	      fHitsTrd1n[0][1] =padChannel + 16 * padRow; // channel+16*(groupId%2);
      // 	      //fAlignmentCounter1++;
      // 	    }
      // 	    if(fLastSource == 6){
      // 	      // printf("Last Source was 3, starting comparison! \n");
      // 	      // printf("Alignment Counter: %i   fSource:  %i    fLastSource:   %i, AlignmentCompareCounter: %i \n", fAlignmentCounter1,  fSource, fLastSource, fAlignmentCompareCounter);
      // 	      //  for(int i=0; i < fAlignmentCounter1; i++){
      // 	      // printf("fHitsTrd0: %i, Channel: %i, fHitsTrd1: %i, Channel: %i \n", fHitsTrd0[i][0], fHitsTrd0[i][1], fHitsTrd1[i][0], fHitsTrd1[i][1]);
      // 	      //  }
      // 	      for(int i=0; i < fAlignmentCounter1; i++){
      // 		//	  printf(" fHitsTrd0: %i, fHitsTrd1: %i, AlignmentCompareCounter: %i, i: %i \n",fHitsTrd0[fAlignmentCompareCounter][0],fHitsTrd1[i][0],  fAlignmentCompareCounter, i);
      // 		while(fHitsTrd2[i][0] > fHitsTrd1n[fAlignmentCompareCounter][0]){
      // 		  fAlignmentCompareCounter++;
      // 		  if(fAlignmentCompareCounter <10){
      // 		    //      printf("INCREASING AlignmentCompareCounter");
      // 		  }
      // 		}
      // 		if (fHitsTrd2[i][0] == fHitsTrd1n[fAlignmentCompareCounter][0]){
      // 		  fAlignment[1]->Fill(fHitsTrd1n[fAlignmentCompareCounter][1],fHitsTrd2[i][1]);
      // 		  //    printf(" fHitsTrd0: %i, fHitsTrd1: %i \n",fHitsTrd0[fAlignmentCompareCounter][0],fHitsTrd1[i][0]);
      // 		  // printf("Coincidence at Channels: %i and %i! \n",fHitsTrd0[fAlignmentCompareCounter][1],fHitsTrd1[i][1]);
      // 		  // printf("At Time: %i = %i \n", fHitsTrd0[fAlignmentCompareCounter][0], fHitsTrd1[i][0]);
      // 		}
      // 	      }
      // 	      for(int i = 0; i <= fAlignmentCounter1; i++){
      // 		fHitsTrd1n[i][0]=0;
      // 	      }
      // 	      fHitsTrd2[0][0] = fThisTime;
      // 	      fHitsTrd2[0][1] = padChannel + 16 * padRow; // channel+16*(groupId%2);
      // 	      // fAlignmentCounter1++;
      // 	    }
      // 	    fLastSource = fSource;
      // 	    fAlignmentCounter1 = 1;
      // 	  }
      // 	}
      // }


    }
    else if ( m->is_buffer_overflow() ){
    	LOG(debug) <<  counter << " This is a buffer overflow message";
    	isOverflow = true;
    	//GetEpochInfo(link, addr);
    	Int_t triggerType = -1;
    	Int_t infoType = -1;
    	Int_t stopType = -1;
	//	Int_t groupId = m->group_id();
    	Int_t channel = m->channel_id();
    	Int_t time = m->timestamp();
    	Int_t bufferOverflowCounter = static_cast<Int_t>(m->buffer_overflow_count());
    	Int_t samples = 1;
    	Int_t* sample_values = new Int_t[samples];
    	sample_values[0] = -256;

    	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
    	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
    			      fSuperEpoch, triggerType, infoType, stopType, groupId,
    			      bufferOverflowCounter, samples, sample_values,
    			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);

    	delete[] sample_values;
    	fMessageStatistic[fSource]->Fill(fMessageTypes[5].Data(),bufferOverflowCounter);
    	channel = GetChannelOnPadPlane(channel,groupId);
    	if (channel > 15){
    	  channel-= 16;
    	  groupId = 1;
    	} else {
    	  groupId = 0;
    	}
    	fLost[fSource]->Fill(channel,groupId,bufferOverflowCounter);
	    	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
    	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
    			      fSuperEpoch, triggerType, infoType, stopType, groupId,
    			      bufferOverflowCounter, samples, sample_values,
    			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);

    }

    else if ( m->is_info() ){
    	LOG(debug) <<  counter << " This is a info message";
    	isInfo = true;
    	//GetEpochInfo(link, addr);

    	Int_t triggerType = -1;
    	Int_t infoType = static_cast<Int_t>(m->info_type());
    	Int_t stopType = -1;
	//	Int_t groupId = m->group_id();
    	Int_t channel = m->channel_id();
    	Int_t time = m->timestamp();
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
    	fMessageStatistic[fSource]->Fill(fInfoTypes[infoType].Data(),1);
    	channel = GetChannelOnPadPlane(channel,groupId);
    	if (channel > 15){
    	  channel-= 16;
    	  groupId = 1;
    	} else {
    	  groupId = 0;
    	}
       fInfo[fSource]->Fill(channel,groupId,1);
    }


    else if ( m->is_hit_aborted()) {
    	LOG(debug) <<  counter << " This is a hit message was aborted";
    	isHitAborted = true;
	//    	GetEpochInfo(link, addr);
    	Int_t triggerType = -1;
    	Int_t stopType = -1;
    	Int_t time = m->timestamp();
    	Int_t infoType = -1;
	//	Int_t groupId = -1;//mp->group_id();//???
    	Int_t bufferOverflowCounter = 0;
    	Int_t samples = 1;
    	Int_t* sample_values = NULL;
    	Int_t channel = m->channel_id();
    	infoType = static_cast<Int_t>(m->info_type());// should here be stoptype instead???
    	sample_values = new Int_t[samples];
    	sample_values[0] = -256;
    	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
    	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
    			      fSuperEpoch, triggerType, infoType, stopType, groupId,
    			      bufferOverflowCounter, samples, sample_values,
    			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);
    	//++counter;
    	delete[] sample_values;
    	fMessageStatistic[fSource]->Fill(fMessageTypes[3].Data(),1);
    }

     else if ( m->is_epoch_marker() ) {
	LOG(debug) <<  counter << " This is an Epoch Marker";
	isEpoch = true;
	FillEpochInfo(HalfSpadicAddress, AfckID, m->epoch_count());
	GetEpochInfo(HalfSpadicAddress, AfckID);
	Int_t triggerType = -1;
	Int_t infoType = -1;
	Int_t stopType = -1;
	//Int_t groupId = mp->group_id();
	Int_t channel = m->channel_id();
	Int_t time = -1;
	Int_t bufferOverflowCounter = 0;
	Int_t samples = 1;
	Int_t* sample_values = new Int_t[samples];
	sample_values[0] = -256;
	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
			      fSuperEpoch, triggerType, infoType, stopType, groupId,
			      bufferOverflowCounter, samples, sample_values,
			      isHit, isInfo, isEpoch, isEpochOutOfSync, isHitAborted, isOverflow, isStrange);
	delete[] sample_values;
	fMessageStatistic[AfckID * NrOfSpadics + SpadicAddress]->Fill(fMessageTypes[0].Data(),1);
	if (!fHighPerformance)fEpoch[AfckID * NrOfSpadics + SpadicAddress]->Fill(channel,groupId,1);
      }

    else {
    	isStrange = true;
    	//GetEpochInfo(link, addr);
    	Int_t triggerType = -1;
    	Int_t stopType = -1;
    	Int_t time = -1;//mp->timestamp();
    	Int_t infoType = -1;
	// 	Int_t groupId = -1;//mp->group_id();
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
	//	fMessageStatistic[fSource]->Fill(fMessageTypes[6].Data(),1);
    	//fStrange[fSource]->Fill(channel,groupId);
    	LOG(debug) <<  counter << " This message type is not hit, info, epoch or overflow and will not be stored in the TClonesArray";
    	LOG(debug) << " valide:" << m->is_valid() << " epoch marker:" << fEpochMarker << " super epoch marker:" << fSuperEpoch << " time:" << time << " link:" << link << " address:" << address;
    	LOG(debug) << "Channel ID:" << m->channel_id();
    	//if ( mp->is_hit_aborted() )
    	//LOG(info) << "hit is aborted";
    	//if ( mp->is_hit() )
    	LOG(debug) << "GroupID:" << m->group_id() << "hit: triggerType:" << static_cast<Int_t>(m->hit_type()) << " stopType:" << static_cast<Int_t>(m->stop_type()) << " Nr.of samples:" << m->samples().size();

    	//if ()
      std::stringstream ss;
    	for (auto x : m->samples()) {
    	  ss << " " << x;
    	}
    	LOG(debug) << ss.str();
    	//if (mp->is_info())
    	LOG(debug) << "InfoType:" << static_cast<Int_t>(m->info_type());
    	//if ()
    	LOG(debug) << "Nr. of overflows:" << static_cast<Int_t>(m->buffer_overflow_count());
    	//print_message(mp);
    }
  } // while()

 } // tsReader

  return kTRUE;
}

void CbmTSUnpackSpadic20DesyOnlineMonitor::print_message(const spadic::Message& m)
{
  printf("PrintMsg\n");
  LOG(info) << "v: " << (m.is_valid() ? "o" : "x");
  LOG(debug) << " / gid: " << static_cast<int>(m.group_id());
  LOG(debug) << " / chid: " << static_cast<int>(m.channel_id());
  if ( m.is_hit() ) {
    LOG(debug) << " / ts: " << m.timestamp();
    LOG(debug) << " / samples (" << m.samples().size() << "):";
    std::stringstream ss;
    for (auto x : m.samples()) {
      ss << " " << x;
    }
    LOG(debug) << ss.str();
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


void CbmTSUnpackSpadic20DesyOnlineMonitor::FillEpochInfo(Int_t HalfSpadicAddress, Int_t AfckID, Int_t epoch_count)
{


  if ( epoch_count < (fEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)] - 3000) )
      {
	if(SuppressMultipliedEpochMessages)
	  if ( epoch_count != fPreviousEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)] ){
	    fSuperEpochArray[std::make_pair(HalfSpadicAddress,AfckID)]++;
	  } else {
	    LOG(error)<< "Multiply repeated Epoch Messages at Super Epoch "
		      << fSuperEpoch << " Epoch "
		      << epoch_count << " for AFCK"
		      << AfckID << "_Half_Spadic"
		      << HalfSpadicAddress;
	  }
	else fSuperEpochArray[std::make_pair(HalfSpadicAddress,AfckID)]++;

	LOG(debug) << "Overflow of EpochCounter for Syscore"
		   << AfckID << "Half_Spadic"
		   << HalfSpadicAddress;
      } else if ((epoch_count - fEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)]) !=1 ) {
     // LOG(info) << "Missed epoch counter for Syscore"
     // 		<< AfckID << "_Spadic"
     // 		<< HalfSpadicAddress;
    } else if (epoch_count == fEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)]){
      LOG(error) << "Identical Epoch Counters for Syscore"
		 << AfckID << "_Spadic"
		 << HalfSpadicAddress;
    }
    fPreviousEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)] = fEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)];
    fEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)] = epoch_count;


}

void CbmTSUnpackSpadic20DesyOnlineMonitor::GetEpochInfo(Int_t HalfSpadicAddress, Int_t AfckID)
{
  //  printf("Get epoch info\n");

    fEpochMarker = fEpochMarkerArray[std::make_pair(HalfSpadicAddress,AfckID)];
    fSuperEpoch = fSuperEpochArray[std::make_pair(HalfSpadicAddress,AfckID)];



}

Int_t CbmTSUnpackSpadic20DesyOnlineMonitor::GetChannelOnPadPlane(Int_t SpadicChannel, Int_t groupId)
{
  if(SpadicChannel > 15 || groupId > 1) LOG(debug) << "CbmTSUnpackSpadic20DesyOnlineMonitor::                     ChId " << SpadicChannel << "  GroupId: " << groupId;
  SpadicChannel = groupId * 16 + SpadicChannel;
  Int_t channelMapping[32] = {31,15,30,14,29,13,28,12,27,11,26,10,25, 9,24, 8,
			      23, 7,22, 6,21, 5,20, 4,19, 3,18, 2,17, 1,16, 0};
  if (SpadicChannel < 0 || SpadicChannel > 31){
    if (SpadicChannel !=-1) LOG(debug) << "CbmTSUnpackSpadic20DesyOnlineMonitor::GetChannelOnPadPlane ChId " << SpadicChannel << "  GroupId: " << groupId;
    return -1;
  } else {
    return channelMapping[SpadicChannel];
  }
}

Int_t CbmTSUnpackSpadic20DesyOnlineMonitor::GetSpadicID(Int_t address)
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

//Int_t CbmTSUnpackSpadic20DesyOnlineMonitor::GetAfckID(Int_t AfckAddress) // Fix me!!
//{
//
//  /*  Int_t AfckID = -1;
//  for(int i = 0; i < 4; i++)
//	if (AfckAddress == fAFCKs[i]){
//	  AfckID = i;
//	}
//  return AfckID;
//  Int_t SyscoreID=link-BaseEquipmentID;
//  if((SyscoreID<0||SyscoreID>3)){
//    LOG(debug) << "EqID " << link << " not known.";
//    SyscoreID=-1;
//  }
//  return SyscoreID;
//  */
//}
// ----              -------------------------------------------------------


inline TString CbmTSUnpackSpadic20DesyOnlineMonitor::GetSpadicName(Int_t /*link*/,
                                                                   Int_t /*address*/){

  TString spadicName="";
  Int_t SpadicID=0;
  Int_t SpadicAddress = 0;
  Int_t AfckID = 0;
  spadicName="Afck_"+std::to_string(AfckID)+"_";
  SpadicID=SpadicAddress;

  //SpadicID/=2;

  spadicName += "Spadic_";

  spadicName+=std::to_string(SpadicID);

  return spadicName;
}

void CbmTSUnpackSpadic20DesyOnlineMonitor::InitHistos()
{

  #ifdef USE_HTTP_SERVER
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  #endif

  //cout << "InitHistos" << endl;
  TString histName;

  //  fHM->Add("DeltaT",new TH1I ("DeltaT","DeltaT",4001,-2000.5,2000.5));
  //  fDeltaT[0]=(TH1I*)fHM->H1("DeltaT");

  for (Int_t iLink = 0; iLink < NrOfAfcks; iLink++){
    for (Int_t iAddress = 0; iAddress < NrOfSpadics; iAddress++){

    histName.Form("AFCK_%i_Spadic_%i",iLink,iAddress);
    //histName.Form("AFCK_0_Spadic_0");

    fHM->Add(TString("Time_between_Hits"+histName).Data(),new TH1D (TString("Time_between_Hits"+histName).Data(),TString("Time_between_Hits"+histName).Data(),8092,-0.5,8091.5));
    fTimeBetweenHits[(iLink)*(NrOfSpadics)+iAddress]=(TH1D*)fHM->H1(TString("Time_between_Hits"+histName).Data());
    fTimeBetweenHits[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("Delta_Time");

    fHM->Add(TString("Cluster_ADCmax_sum"+histName).Data(),new TH1I (TString("Cluster_ADCmax_sum"+histName).Data(),TString("Cluster_ADCmax_sum"+histName).Data(),1000,0.,1000.));
    fADCmaxSum[(iLink)*(NrOfSpadics)+iAddress]=(TH1I*)fHM->H1(TString("Cluster_ADCmax_sum"+histName).Data());
    fADCmaxSum[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("ADCmax_Sum");


    fHM->Add(TString("Nr_Samples_vs_Stoptype"+histName).Data(),new TH2I (TString("Nr_Samples_vs_Stoptype"+histName).Data(),TString("Nr_Samples_vs_Stoptype"+histName).Data(),33,-0.5,32.5,6,-0.5,5.5));
    fNrSamples[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Nr_Samples_vs_Stoptype"+histName).Data());
    fNrSamples[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("Nr_Of_Samples");
    fNrSamples[(iLink)*(NrOfSpadics)+iAddress]->GetYaxis()->SetTitle("Stop_Type");

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
    fHM->Add(TString("Hit_Channel"+histName).Data(),new TH2I (TString("Hit_Channel"+histName).Data(),TString("Hit_Channel"+histName).Data(),16,-0.5,15.5,2,-0.5,1.5));
    fHitChannel[(iLink)*(NrOfSpadics)+iAddress]=(TH2I*)fHM->H2(TString("Hit_Channel"+histName).Data());
    fHitChannel[(iLink)*(NrOfSpadics)+iAddress]->GetXaxis()->SetTitle("Channel ID");
    fHitChannel[(iLink)*(NrOfSpadics)+iAddress]->GetYaxis()->SetTitle("Group ID");
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
      // const Double_t maxFreq = (175000000.0);  (VF) not used
      auto BinBoarders = [/*&nBins*/] (Int_t /*reBinHigh*/=32)
      	{
      	  const Double_t MaxFreq = (175000000.0);
//      	  const Double_t slope=10;//MaxFreq/nBins;  (VF) not used
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
      	fMaxADC[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]=(TH1D*)fHM->H1(TString("MaxADC_"+chName+histName).Data());
      	fMaxADC[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->GetXaxis()->SetTitle("ADC");
      	fMaxADC[((iLink)*(NrOfSpadics)+iAddress)*32+iCh]->GetYaxis()->SetTitle("counts");
      }

       //Alignment Histos
	for (Int_t iHist =0; iHist <3; iHist++){
	  TString align;
	  align.Form("%s", fAlignments[iHist].Data());
	    fHM->Add(TString("Alignment_"+align).Data(),new TH2I (TString("Alignment_"+align).Data(),TString("Alignment_"+align).Data(),32,-0.5,31.5,32,-0.5,31.5));
	  fAlignment[iHist]=(TH2I*)fHM->H2(TString("Alignment_"+align).Data());
	  fAlignment[iHist]->GetXaxis()->SetTitle(TString("Channels_Chamber_"+fChambers[iHist]).Data());
	  fAlignment[iHist]->GetYaxis()->SetTitle(TString("Channels_Chamber_"+fChambers[iHist+1]).Data());

	  }


      //Global Histo: Sync
       fHM->Add("SyncChambers",new TH2I ("SyncChambers","SyncChambers",4096,-0.5,4095.5,4,-0.5,3.5));
      	fSyncChambers[0]=(TH2I*)fHM->H2("SyncChambers");
      	fSyncChambers[0]->GetXaxis()->SetTitle("Time_between_Hits");
      	fSyncChambers[0]->GetYaxis()->SetTitle("Chamber");

	//Global Histo: Event Selection
	fHM->Add("Event_Selection",new TH1I ("Event_Selection","Event_Selection",5,-0.5,4.5));
	fEventSelection[0]=(TH1I*)fHM->H1("Event_Selection");
	fEventSelection[0]->GetXaxis()->SetTitle("Chambers_Hit");


    }
  }
  fHM->Create2<TH2I>("TimeCorrel","TimeCorrel",4,-0.5,3.5,5000,-0.5,20000);
  fHM->H2("TimeCorrel")->GetXaxis()->SetTitle("SpadicId");
  fHM->H2("TimeCorrel")->GetXaxis()->SetTitle("#Delta t");

}




void CbmTSUnpackSpadic20DesyOnlineMonitor::InitCanvas()
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
  fcHC= new TCanvas(TString("HitMapChannel").Data(),TString("HitMapChannel").Data(),1600,1200);
  fcHC->Divide(3,4);
  fcL= new TCanvas(TString("LostMap").Data(),TString("LostMap").Data(),1600,1200);
  fcL->Divide(3,4);
  fcTS= new TCanvas(TString("TSGraph").Data(),TString("TSGraph").Data(),1600,1200);
  fcTS->Divide(3,4);
  fcMS= new TCanvas(TString("MessageStatistic").Data(),TString("MessageStatistic").Data(),1600,1200);
  fcMS->Divide(3,4);
  fcSy= new TCanvas(TString("SyncChambers").Data(),TString("SyncChambers").Data(),1600,1200);
  fcSy->Divide(1,1);
  fcNS= new TCanvas(TString("Nr_Samples_vs_Stoptype").Data(),TString("Nr_Samples_vs_Stoptype").Data(),1600,1200);
  fcNS->Divide(3,4);
  fcTH= new TCanvas(TString("Time_between_Hits").Data(),TString("Time_between_Hits").Data(),1600,1200);
  fcTH->Divide(3,4);
  fcAS= new TCanvas(TString("Cluster_ADCmax_sum").Data(),TString("Cluster_ADCmax_sum").Data(),1600,1200);
  fcAS->Divide(3,4);
  fcA= new TCanvas(TString("Alignment").Data(),TString("Alignment").Data(),1600,1200);
  fcA->Divide(2,2);
  fcES= new TCanvas(TString("Event_Selection").Data(),TString("Event_Selection").Data(),1600,1200);
  fcES->Divide(1,1);


  for (Int_t iLink = 0; iLink < NrOfAfcks; iLink++){
    for (Int_t iAddress = 0; iAddress < NrOfSpadics; iAddress++){
      cName.Form("AFCK_%i_Spadic_%i",iLink,iAddress);
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
	//	fDeltaT[0]->Draw("");
      }
      fcB->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz();
      fBaseline[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      // cName.Form("AFCK_%i_Spadic_%i",iLink,iAddress);
      fcM->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz();
      fmaxADCmaxTimeBin[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcH->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(1);
      fHit[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcHC->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(1);
      fHitChannel[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcL->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz(0);
      fLost[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcAS->cd((iLink)*(NrOfSpadics)+iAddress+1);
      fADCmaxSum[(iLink)*(NrOfSpadics)+iAddress]->Draw("hist");
      fcMS->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogy(1);
      fMessageStatistic[(iLink)*(NrOfSpadics)+iAddress]->Draw("");
      fcTS->cd((iLink)*(NrOfSpadics)+iAddress+1);
      fHitTimeA[(iLink)*(NrOfSpadics)+iAddress]->GetYaxis()->SetRangeUser(0,1000);
      fHitTimeA[(iLink)*(NrOfSpadics)+iAddress]->Draw("");
      fHitTimeB[(iLink)*(NrOfSpadics)+iAddress]->Draw("same");
      //fTSGraph[(iLink)*(NrOfSpadics)+iAddress]->Draw("ALP");
      fHitSync[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      //      fDeltaT[0]->Draw("");
      fcNS->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogz();
      fNrSamples[(iLink)*(NrOfSpadics)+iAddress]->Draw("colz");
      fcTH->cd((iLink)*(NrOfSpadics)+iAddress+1)->SetLogy();
      fTimeBetweenHits[(iLink)*(NrOfSpadics)+iAddress]->Draw("");
       for (Int_t iHist =0; iHist <3; iHist++){
      	fcA->cd(iHist+1)->SetLogz(1);
      	fAlignment[iHist]->Draw("colz");
      	  }
      fcES->cd(1)->SetLogy(0);
      fEventSelection[0]->Draw("");
      fcSy->cd(1)->SetLogz(1);
      fSyncChambers[0]->Draw("");
    }
  }
}



void CbmTSUnpackSpadic20DesyOnlineMonitor::UpdateCanvas()
{
  /*
    //cout << "UpdateCanvas" << endl;
    TH2I* h = NULL;
    TString cName;
    for (Int_t iLink = 0; iLink < NrOfAfcks; iLink++){
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

void CbmTSUnpackSpadic20DesyOnlineMonitor::Reset()
{
  fSpadicRaw->Clear();
}
void CbmTSUnpackSpadic20DesyOnlineMonitor::FinishEvent()
{
  //UpdateCanvas();
}

void CbmTSUnpackSpadic20DesyOnlineMonitor::Finish()
{
  for(auto p : fSuperEpochArray){
    int supEpCnt= p.second;
    int halfSpadicAddress=p.first.first;
    int afckId=p.first.second;
   LOG(debug) << "There have been " << supEpCnt
		 << " SuperEpochs for AFCK" << afckId << "_Spadic"
		 << halfSpadicAddress << " in this file";
  }
  UpdateCanvas();
}


  /*
    void CbmTSUnpackSpadic20DesyOnlineMonitor::Register()
    {
    }
  */


  ClassImp(CbmTSUnpackSpadic20DesyOnlineMonitor)
