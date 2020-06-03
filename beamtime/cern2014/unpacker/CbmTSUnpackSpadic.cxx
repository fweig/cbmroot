// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadic                       -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmTSUnpackSpadic.h"

#include "CbmSpadicRawMessage.h"

#include "TimesliceReader.hpp"

#include "FairLogger.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include <iostream>

CbmTSUnpackSpadic::CbmTSUnpackSpadic()
  : CbmTSUnpack(),
    fSpadicRaw(new TClonesArray("CbmSpadicRawMessage", 10)),
    fEpochMarkerArray(),
	fPreviousEpochMarkerArray(),
    fSuperEpochArray(),
    fEpochMarker(0),
    fSuperEpoch(0),
    fNrExtraneousSamples{0}
{
  for (Int_t i=0; i < NrOfSyscores; ++i) {
    for (Int_t j=0; j < NrOfHalfSpadics; ++j) {
      fEpochMarkerArray[i][j] = 0;
	  fPreviousEpochMarkerArray[i][j] =0;
      fSuperEpochArray[i][j] = 0;
    }
  }
}

CbmTSUnpackSpadic::~CbmTSUnpackSpadic()
{
  LOG(info) << "Number of extraneous Samples "<< fNrExtraneousSamples;
}

Bool_t CbmTSUnpackSpadic::Init()
{
  LOG(info) << "Initializing";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }
  ioman->Register("SpadicRawMessage", "spadic raw data", fSpadicRaw, kFALSE);

  return kTRUE;
}

Bool_t CbmTSUnpackSpadic::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  // compare to: https://github.com/spadic/spadic10-software/blob/master/lib/message/message.h
  // or fles/spadic/message/wrap/cpp/message.cpp
  LOG(debug) << "Unpacking Spadic Data";

  spadic::TimesliceReader r;
  Int_t counter=0;

  r.add_component(ts, component);

  for (auto addr : r.sources()) {
    LOG(debug) << "---- reader " << addr << " ----";
    while (auto mp = r.get_message(addr)) {
      if(gLogger->IsLogNeeded( fair::Severity::debug )) {
	print_message(*mp);
      }
      counter++;
      Int_t link = ts.descriptor(component, 0).eq_id;
      Int_t address = addr;
      /*
	if ( mp->is_valid() ){
	LOG(debug) << counter << " This message is valid";
	} else {
	LOG(error) << counter <<  " This message is not valid";
	continue;
	}
      */
      Bool_t isInfo(false), isHit(false), isEpoch(false), isEpochOutOfSynch(false), isOverflow(false), isHitAborted(false), isStrange(false);
      if ( mp->is_epoch_out_of_sync() ){
	isEpochOutOfSynch = true;
        FillEpochInfo(link, addr, mp->epoch_count());
	GetEpochInfo(link, addr);
	Int_t triggerType = -1;
        Int_t infoType = -1;
        Int_t stopType = -1;
	Int_t groupId = mp->group_id();
	Int_t channel = mp->channel_id();
	Int_t time = -1;
	Int_t bufferOverflowCounter = 0;
	Int_t samples = 1;
	Int_t* sample_values = new Int_t[samples];
	sample_values[0] = -256;
	LOG(debug) <<  counter << " This is an out of sync Epoch Marker";
	LOG(debug) << "   TimeStamp: " << mp->timestamp();
	LOG(debug) << "   Channel:   " << mp->channel_id();
	LOG(debug) << "   Group:     " << mp->group_id();
	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
			      fSuperEpoch, triggerType, infoType, stopType, groupId,
			      bufferOverflowCounter, samples, sample_values,
			      isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted, isOverflow, isStrange);
	delete[] sample_values;
      }
      else if ( mp->is_epoch_marker() ) {
	LOG(debug) <<  counter << " This is an Epoch Marker";
	isEpoch = true;
        FillEpochInfo(link, addr, mp->epoch_count());
	GetEpochInfo(link, addr);
	Int_t triggerType = -1;
        Int_t infoType = -1;
        Int_t stopType = -1;
	Int_t groupId = mp->group_id();
	Int_t channel = mp->channel_id();
	Int_t time = -1;
	Int_t bufferOverflowCounter = 0;
	Int_t samples = 1;
	Int_t* sample_values = new Int_t[samples];
	sample_values[0] = -256;
	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
			      fSuperEpoch, triggerType, infoType, stopType, groupId,
			      bufferOverflowCounter, samples, sample_values,
			      isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted, isOverflow, isStrange);
	delete[] sample_values;
      }
      else if ( mp->is_buffer_overflow() ){
	LOG(debug) <<  counter << " This is a buffer overflow message";
	isOverflow = true;
	GetEpochInfo(link, addr);
	Int_t triggerType = -1;
        Int_t infoType = -1;
        Int_t stopType = -1;
	Int_t groupId = mp->group_id();
	Int_t channel = mp->channel_id();
	Int_t time = mp->timestamp();
        Int_t bufferOverflowCounter = static_cast<Int_t>(mp->buffer_overflow_count());
	Int_t samples = 1;
	Int_t* sample_values = new Int_t[samples];
	sample_values[0] = -256;
	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
			      fSuperEpoch, triggerType, infoType, stopType, groupId,
			      bufferOverflowCounter, samples, sample_values,
			      isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted, isOverflow, isStrange);
	delete[] sample_values;
      }
      else if ( mp->is_info() ){
	LOG(debug) <<  counter << " This is a info message";
	isInfo = true;
	GetEpochInfo(link, addr);

        Int_t triggerType = -1;
        Int_t infoType = static_cast<Int_t>(mp->info_type());
        Int_t stopType = -1;
	Int_t groupId = mp->group_id();
	Int_t channel = mp->channel_id();
	Int_t time = mp->timestamp();
        Int_t bufferOverflowCounter = 0;//mp->buffer_overflow_count();// should be now obsolete
	Int_t samples = 1;
	Int_t* sample_values = new Int_t[samples];
	sample_values[0] = -256;
	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
			      fSuperEpoch, triggerType, infoType, stopType, groupId,
			      bufferOverflowCounter, samples, sample_values,
			      isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted, isOverflow, isStrange);
	delete[] sample_values;
      }
      else if ( mp->is_hit() ) {
	LOG(debug) <<  counter << " This is a hit message";
	isHit = true;
	GetEpochInfo(link, addr);
	Int_t triggerType =  static_cast<Int_t>(mp->hit_type());
	Int_t stopType = static_cast<Int_t>(mp->stop_type());
	Int_t time = mp->timestamp();
        Int_t infoType = -1;
	Int_t groupId = mp->group_id();
	Int_t bufferOverflowCounter = 0;
	Int_t samples = mp->samples().size();
	if(samples>32) {
	    fNrExtraneousSamples++;
	    samples=32; //Suppress extraneous Samples, which cannot (!) occur in Raw Data Stream.
	}
	Int_t* sample_values =  new Int_t[samples];
	Int_t channel = mp->channel_id();
	Int_t counter1=0;
	for (auto x : mp->samples()) {
	  sample_values[counter1] = x;
	  ++counter1;
	}
	//}
	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
			      fSuperEpoch, triggerType, infoType, stopType, groupId,
			      bufferOverflowCounter, samples, sample_values,
			      isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted, isOverflow, isStrange);
	//++counter;
	delete[] sample_values;
      }
      else if ( mp->is_hit_aborted()) {
	LOG(debug) <<  counter << " This is a hit message was aborted";
	isHitAborted = true;
	GetEpochInfo(link, addr);
	Int_t triggerType = -1;
	Int_t stopType = -1;
	Int_t time = mp->timestamp();
        Int_t infoType = -1;
	Int_t groupId = -1;//mp->group_id();//???
	Int_t bufferOverflowCounter = 0;
	Int_t samples = 1;
	Int_t* sample_values = NULL;
	Int_t channel = mp->channel_id();
	infoType = static_cast<Int_t>(mp->info_type());// should here be stoptype instead???
	sample_values = new Int_t[samples];
	sample_values[0] = -256;
	new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	  CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
			      fSuperEpoch, triggerType, infoType, stopType, groupId,
			      bufferOverflowCounter, samples, sample_values,
			      isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted, isOverflow, isStrange);
	//++counter;
	delete[] sample_values;

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
			      isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted, isOverflow, isStrange);
	//++counter;
	delete[] sample_values;

	LOG(debug) <<  counter << " This message type is not hit, info, epoch or overflow and will not be stored in the TClonesArray";
	LOG(debug) << " valide:" << mp->is_valid() << " epoch marker:" << fEpochMarker << " super epoch marker:" << fSuperEpoch << " time:" << time << " link:" << link << " address:" << address;
	LOG(debug) << "Channel ID:" << mp->channel_id();
	//if ( mp->is_hit_aborted() )
	//LOG(info) << "hit is aborted";
	//if ( mp->is_hit() )
	LOG(debug) << "GroupID:" << mp->group_id() << "hit: triggerType:" << static_cast<Int_t>(mp->hit_type()) << " stopType:" << static_cast<Int_t>(mp->stop_type()) << " Nr.of samples:" << mp->samples().size();

	//if ()
	for (auto x : mp->samples()) {
	  LOG(debug) << " " << x;
	}
	LOG(debug);
	//if (mp->is_info())
	LOG(debug) << "InfoType:" << static_cast<Int_t>(mp->info_type());
	//if ()
	LOG(debug) << "Nr. of overflows:" << static_cast<Int_t>(mp->buffer_overflow_count());
	//print_message(mp);
      }
    }
  }
  return kTRUE;
}

  void CbmTSUnpackSpadic::print_message(const spadic::Message& m)
  {
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

void CbmTSUnpackSpadic::FillEpochInfo(Int_t link, Int_t addr, Int_t epoch_count)
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

  void CbmTSUnpackSpadic::GetEpochInfo(Int_t link, Int_t addr)
  {
    auto it=groupToExpMap.find(link);
    if (it == groupToExpMap.end()) {
      LOG(fatal) << "Could not find an entry for equipment ID" <<
	std::hex << link << std::dec;
    } else {
      fEpochMarker = fEpochMarkerArray[it->second][addr];
      fSuperEpoch = fSuperEpochArray[it->second][addr];
    }

  }

  void CbmTSUnpackSpadic::Reset()
  {
    fSpadicRaw->Clear();
  }

  void CbmTSUnpackSpadic::Finish()
  {
    for (Int_t i=0; i < NrOfSyscores; ++i) {
      for (Int_t j=0; j < NrOfHalfSpadics; ++j) {
	LOG(debug) << "There have been " << fSuperEpochArray[i][j]
		   << " SuperEpochs for Syscore" << i << "_Spadic"
		   << j << " in this file";
      }
    }

  }


  /*
    void CbmTSUnpackSpadic::Register()
    {
    }
  */


  ClassImp(CbmTSUnpackSpadic)

