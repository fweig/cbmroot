// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadic20                     -----
// -----                    Created 07.11.2014 by P. Munkes                -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
#include "CbmTSUnpackSpadic20.h"

#include "CbmSpadicRawMessage.h"

#include "TimesliceReader20.hpp"

#include "FairLogger.h"
#include "FairRootManager.h"

#include "TClonesArray.h"
#include <TFile.h>

#include "CbmHistManager.h"

#include <iostream>
#include <array>
#include <algorithm>

CbmTSUnpackSpadic20::CbmTSUnpackSpadic20 () :
  CbmTSUnpack (),
  fHm (new CbmHistManager),
  fSpadicRaw (new TClonesArray ("CbmSpadicRawMessage", 10)),
  fEpochMarkerArray (),
  fPreviousEpochMarkerArray (),
  fSuperEpochArray (),
  fEpochMarker (0),
  fSuperEpoch (0),
  fNrExtraneousSamples { 0 },
  multihit_buffer()
{}

CbmTSUnpackSpadic20::~CbmTSUnpackSpadic20 ()
{
  LOG(info) << "Number of extraneous Samples " << fNrExtraneousSamples;
}

Bool_t
CbmTSUnpackSpadic20::Init ()
{
  LOG(info) << "Initializing";

  FairRootManager* ioman = FairRootManager::Instance ();
  if (ioman == NULL)
    {
      LOG(fatal) << "No FairRootManager instance";
    }
  ioman->Register ("SpadicRawMessage", "spadic raw data", fSpadicRaw, kFALSE);

  CbmTSUnpackSpadic20::CreateHistograms();
  return kTRUE;
}

void
CbmTSUnpackSpadic20::CreateHistograms()
{

}

Bool_t
CbmTSUnpackSpadic20::DoUnpack (const fles::Timeslice& ts, size_t component)
{
  // compare to: https://github.com/spadic/spadic10-software/blob/master/lib/message/message.h
  // or fles/spadic/message/wrap/cpp/message.cpp
  LOG(debug) << "Unpacking Spadic Data";

  spadic::TimesliceReader20 r;
  Int_t counter = 0;

  r.add_component (ts, component);

  std::array<Int_t,32> sample_values;
  sample_values.fill(-256);
  for (uint16_t addr : r.sources ())
    {
      //    auto spadicLink=(addr);
      int address=2*(r.get_link(addr)/0x100)/8+(r.get_link(addr)/0x100)%8;
      //LOG(info) <<std::hex << "---- reader " << addr << " ----" << r.get_link(addr) << "--------------" << ts.descriptor(component,0).eq_id<<std::dec;

      //LOG(info) << "---- reader " << std::hex << addr;

      while (std::unique_ptr<spadic::Message> mp = r.get_message (addr))
	{
	  if (gLogger->IsLogNeeded (fair::Severity::debug))
	    {
	      print_message (*mp);
	    }
	  counter++;
	  Int_t link = ts.descriptor(component, 0).eq_id-0xaa00;
	  //std::cout << std::hex << address << std::dec <<std::endl;

	  Bool_t isInfo (false), isHit (false), isEpoch (false),
	    isEpochOutOfSynch (false), isOverflow (false),
	    isHitAborted (false), isStrange (false);
	  Int_t triggerType = -1;
	  Int_t infoType = -1;
	  Int_t stopType = -1;
	  Int_t groupId = mp->group_id ();
	  Int_t channel = mp->channel_id ();
	  Int_t time = -1;
	  Int_t bufferOverflowCounter = 0;
	  Int_t samples = 0;
	  if (mp->is_epoch_marker ())
	    {
	      LOG(debug) << counter << " This is an Epoch Marker";
	      isEpoch = true;
	      FillEpochInfo (link, address, mp->epoch_count ());
	      GetEpochInfo (link, address);
	      time=0;
	    }
	  else if (mp->is_buffer_overflow ())
	    {
	      LOG(debug) << counter << " This is a buffer overflow message";
	      isOverflow = true;
	      GetEpochInfo (link, address);
	      time = mp->timestamp ();
	      bufferOverflowCounter =
		static_cast<Int_t> (mp->buffer_overflow_count ());
	    }
	  else if (mp->is_info ())
	    {
	      LOG(debug) << counter << " This is a info message";
	      isInfo = true;
	      GetEpochInfo (link, address);
	      infoType = static_cast<Int_t> (mp->info_type ());
	      time = mp->timestamp ();
	      bufferOverflowCounter = 0; //mp->buffer_overflow_count();// should be now obsolete
	    }
	  else if (mp->is_hit ())
	    {
	      TString HistName = "";
	      n_hit++;
	      LOG(debug) << counter << " This is a hit message";
	      isHit = true;
	      GetEpochInfo (link, address);
	      triggerType = static_cast<Int_t> (mp->hit_type ());
	      stopType = static_cast<Int_t> (mp->stop_type ());
	      time = mp->timestamp ();
	      samples = mp->samples ().size ();
	      if (samples > 32)
		{
		  fNrExtraneousSamples++;
		  samples = 32; //Suppress extraneous Samples, which cannot (!) occur in Raw Data Stream.
		}
	      auto temp_sample_array = mp->samples();
	      std::copy(temp_sample_array.begin(),temp_sample_array.begin()+samples,sample_values.begin());

	      //Changes by J.Beckhoff
	      ULong_t fullTime = ( ( static_cast<ULong_t>(fSuperEpoch) << 24) |
		      ( static_cast<ULong_t>(fEpochMarker) << 12) |
		      ( time & 0xfff )
		      );
	      hit_object currHit{
		triggerType,
		  stopType,
		  samples,
		  true,  //b_complete
		  false, //b_multiFlag
		  time,
		  fullTime,
		  {0}
		  };
	      currHit.samples.fill(-256);
	      if(samples == 0) //hit messages with 0 samples are discarded here
		{
		  currHit.b_complete = false;
		  n_zeroSample++;
		  continue;
		}
	      std::copy(temp_sample_array.begin(),temp_sample_array.begin()+samples,currHit.samples.begin());

	      if(currHit.fullTime - multihit_buffer[std::make_pair(link, addr)][channel].fullTime < 32)//successor
		{
		  n_succ++;
		  currHit = correct_successor(link, addr, channel, currHit);
		}
	      if(currHit.stopType >= 3) //predecessor
		{
		  n_pre++;
		  currHit = correct_predecessor(link, addr, channel, currHit);
		}
	      if(currHit.b_complete == true) //hit is complete and ready for digi creation
		{
		  if(currHit.b_multiFlag == true) // flag corrected multimessages with StopType = 10
		    {
		      stopType = 10;
		      n_finalMultiMessage++;
		    }
		  triggerType = currHit.triggerType;
		  time = currHit.time;
		  samples = currHit.nrSamples;
		  std::copy(currHit.samples.begin(),currHit.samples.begin() + currHit.nrSamples,sample_values.begin());
		  n_final_hit++;
		}
	      else
		{
		  continue;
		}
	      //end changes J.Beckhoff
	    }
	  else if (mp->is_hit_aborted ())
	    {
	      LOG(debug) << counter << " This is a hit message was aborted";
	      isHitAborted = true;
	      GetEpochInfo (link, address);
	      samples = 0;
	      infoType = static_cast<Int_t> (mp->info_type ()); // should here be stoptype instead???
	    }
	  else
	    {
	      isStrange = true;
	      GetEpochInfo(link, address);
	      LOG(debug) << counter
			 << " This message type is not hit, info, epoch or overflow and will not be stored in the TClonesArray";
	      LOG(debug) << " valide:" << mp->is_valid () << " epoch marker:"
			 << fEpochMarker << " super epoch marker:"
			 << fSuperEpoch << " time:" << time << " link:"
			 << link << " address:" << address;
	      LOG(debug) << "Channel ID:" << mp->channel_id ();
	      LOG(debug) << "GroupID:" << mp->group_id () << "hit: triggerType:"
			 << static_cast<Int_t> (mp->hit_type ())
			 << " stopType:"
			 << static_cast<Int_t> (mp->stop_type ())
			 << " Nr.of samples:" << mp->samples ().size ();
        std::stringstream ss;
        for (auto x : mp->samples ())
		{
		  ss << " " << x;
		}
	      LOG(debug) << ss.str();
	      //if (mp->is_info())
	      LOG(debug) << "InfoType:" << static_cast<Int_t> (mp->info_type ());
	      //if ()
	      LOG(debug) << "Nr. of overflows:"
			 << static_cast<Int_t> (mp->buffer_overflow_count ());
	    }
	  Int_t* sample_ptr=sample_values.data();
	  new( (*fSpadicRaw)[fSpadicRaw->GetEntriesFast()] )
	    CbmSpadicRawMessage(link, address, channel, fEpochMarker, time,
				fSuperEpoch, triggerType, infoType, stopType, groupId,
				bufferOverflowCounter, samples, sample_ptr,
				isHit, isInfo, isEpoch, isEpochOutOfSynch, isHitAborted,
				isOverflow, isStrange);
	  sample_values.fill(-256);
	}
    }
  return kTRUE;
}

//changes by J.Beckhoff
CbmTSUnpackSpadic20::hit_object CbmTSUnpackSpadic20::correct_successor(Int_t link, Int_t addr, Int_t channel, hit_object currHit)
{
  auto oldHit=multihit_buffer[std::make_pair(link, addr)][channel];
  Int_t samples_pre = oldHit.nrSamples;
  Int_t samples_succ = currHit.nrSamples;
  if(samples_pre < 6)//multimessage, if predecessor with less than 6 samples
    {
      int sum_Samples = std::min((samples_pre + samples_succ), 32);
      for(int i = sum_Samples-1; i >= 0; i--) //combine pre + succ samples
	{
	  if(i >= samples_pre)//predecessor samples
	    {
	      currHit.samples[i] = currHit.samples[i-samples_pre];
	    }
	  else //successor samples
	    {
	      currHit.samples[i] = oldHit.samples[i];
	    }
	}
      //set hit variables
      currHit.nrSamples = sum_Samples;
      currHit.triggerType = oldHit.triggerType; //time and triggerType of predecessor
      currHit.time = oldHit.time;  //time and triggerType of predecessor
      currHit.fullTime = oldHit.fullTime;  //time and triggerType of predecessor
      currHit.b_complete = true;
      currHit.b_multiFlag = true;
      n_multimessage_succ++;
    }
  else // multihit, handled to the digitizer
    {
      currHit.b_complete = true;
      currHit.b_multiFlag = false;
      n_multihit_succ++;
    }
  multihit_buffer[std::make_pair(link,addr)].erase(channel); //delete buffer entry
  return currHit;
}

CbmTSUnpackSpadic20::hit_object CbmTSUnpackSpadic20::correct_predecessor(Int_t link, Int_t addr, Int_t channel, hit_object currHit)
{
  if(currHit.nrSamples < 6) //multimessage, if less than 6 samples
    {
      currHit.b_complete = false;
      n_multimessage_pre++;
    }
  else //multihit, handled to the digitizer
    {
      currHit.b_complete = true;
      n_multihit_pre++;
    }
  currHit.b_multiFlag = false;
  //make buffer entry
  multihit_buffer[std::make_pair(link,addr)].erase(channel);
  multihit_buffer[std::make_pair(link,addr)].insert(std::make_pair(channel, currHit));
  return currHit;
}
//end changes by J.Beckhoff

void
CbmTSUnpackSpadic20::print_message (const spadic::Message& m)
{
  LOG(info) << "v: " << (m.is_valid () ? "o" : "x");
  LOG(debug) << " / gid: " << static_cast<int> (m.group_id ());
  LOG(debug) << " / chid: " << static_cast<int> (m.channel_id ());
  if (m.is_hit ())
    {
      LOG(debug) << " / ts: " << m.timestamp ();
      LOG(debug) << " / samples (" << m.samples ().size () << "):";
      std::stringstream ss;
      for (auto x : m.samples ())
	{
	  ss << " " << x;
	}
      LOG(debug) << ss.str();
    }
  else
    {
      if (m.is_epoch_marker ())
	{
	  LOG(debug) << " This is an Epoch Marker";
	}
      else if (m.is_epoch_out_of_sync ())
	{
	  LOG(info) << " This is an out of sync Epoch Marker";
	}
      else
	{
	  LOG(info) << " This is not known";
	}
    }
}

void
CbmTSUnpackSpadic20::FillEpochInfo (Int_t link, Int_t addr, Int_t epoch_count)
{


  /* Check for repeated Epoch Messages, as the repeated Microslices
     are not captured by the CbmTsUnpacker. This is to ensure the
     linearity of the GetFullTime() method.
  */
  // dirty workaround in the following line: only sufficiently big backsteps of Epoch will trigger an upcount of SuperEpoch. this avoids a high sensitivity of the SuperEpoch counting method to overlaps of some epochs, before the overlaps are understood better
  if (epoch_count < (fEpochMarkerArray[std::pair<int,int>(link,addr)] - 3000))
    {
      if (SuppressMultipliedEpochMessages)
	if (epoch_count != fPreviousEpochMarkerArray[std::pair<int,int>(link,addr)])
	  {
	    fSuperEpochArray[std::pair<int,int>(link,addr)]++;
	  }
	else
	  {
	    LOG(error) << "Multiply repeated Epoch Messages at Super Epoch "
		       << fSuperEpoch << " Epoch " << epoch_count
		       << " for AFCK" << link << "_Spadic" << addr;
	  }
      else
	fSuperEpochArray[std::pair<int,int>(link,addr)]++;

      LOG(debug) << "Overflow of EpochCounter for Afck" << link
		 << "_Spadic" << addr;
    }
  else if ((epoch_count - fEpochMarkerArray[std::pair<int,int>(link,addr)]) != 1)
    {
      LOG(info) <<std::hex<< "Missed epoch counter for Afck" << link
		<< "_Spadic" << addr <<std::dec;
    }
  else if (epoch_count == fEpochMarkerArray[std::pair<int,int>(link,addr)])
    {
      LOG(error) << "Identical Epoch Counters for Afck" << link
		 << "_Spadic" << addr;
    }
  fPreviousEpochMarkerArray[std::pair<int,int>(link,addr)] =
    fEpochMarkerArray[std::pair<int,int>(link,addr)];
  fEpochMarkerArray[std::pair<int,int>(link,addr)] = epoch_count;


}

void
CbmTSUnpackSpadic20::GetEpochInfo (Int_t link, Int_t addr)
{
  fEpochMarker = fEpochMarkerArray[std::pair<int,int>(link,addr)];
  fSuperEpoch = fSuperEpochArray[std::pair<int,int>(link,addr)];
}

void
CbmTSUnpackSpadic20::Reset ()
{
  fSpadicRaw->Clear ();
}

void
CbmTSUnpackSpadic20::Finish ()
{
  LOG(debug) << n_hit << " initial hits" << "\n"
	     << n_pre << " Predecessor, " << n_succ << " Successor" << "\n"
	     << "Multihits: " << n_multihit_pre << " Predecessor, " << n_multihit_succ << " Successor" << "\n"
	     << "Multimessages: " << n_multimessage_pre << " Predecessor, " << n_multimessage_succ << " Successor" << "\n"
	     << "Zero Sample Hits: " << n_zeroSample << "\n"
	     << "Final Multimessages: " << n_finalMultiMessage << "\n"
	     << n_final_hit << " final hits";
  for (auto it = fSuperEpochArray.begin (); it != fSuperEpochArray.end (); it++)
    {
      LOG(debug) << "There have been " << it->second << " SuperEpochs for Afck"
		 << it->first.first << "_Spadic" << it->first.second
		 << " in this file";

    }

  gDirectory->mkdir (this->GetName ());
  gDirectory->Cd (this->GetName ());
  fHm->WriteToFile ();
  FairRootManager::Instance ()->GetOutFile ()->cd ();

}

/*
  void CbmTSUnpackSpadic20::Register()
  {
  }
*/

ClassImp(CbmTSUnpackSpadic20)

