#include <CbmSpadicTriggerComp.h>
#include "CbmSpadicRawMessage.h"
#include "CbmBeamDefaults.h"
#include "CbmTrdDigi.h"
#include "CbmTrdAddress.h"

#include "FairLogger.h"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include "TString.h"
#include "TStyle.h"

#include <cmath>
#include <map>
#include <vector>
#include <deque>

ClassImp(CbmSpadicTriggerComp)

// ---- Default constructor -------------------------------------------
CbmSpadicTriggerComp::CbmSpadicTriggerComp () :
    FairTask ("CbmSpadicTriggerComp"), fRawSpadic (nullptr), fProcSpadic (
    nullptr), fMessageCounter (0), fProcessedMessages (0), fNrTimeSlices (0)
{
  LOG(debug) << "Default Constructor of CbmSpadicTriggerComp";
}

// ---- Destructor ----------------------------------------------------
CbmSpadicTriggerComp::~CbmSpadicTriggerComp ()
{
  LOG(debug) << "Destructor of CbmSpadicTriggerComp";
}

// ----  Initialisation  ----------------------------------------------
void CbmSpadicTriggerComp::SetParContainers ()
{
  LOG(debug) << "SetParContainers of CbmSpadicTriggerComp";
  // Load all necessary parameter containers from the runtime data base
  /*
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   <CbmSpadicTriggerCompDataMember> = (<ClassPointer>*)
   (rtdb->getContainer("<ContainerName>"));
   */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmSpadicTriggerComp::Init ()
{
  LOG(debug) << "Initilization of CbmSpadicTriggerComp";

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance ();

  // Get a pointer to the previous already existing data level
  fRawSpadic =
      static_cast<TClonesArray*> (ioman->GetObject ("SpadicRawMessage"));
  if (!fRawSpadic)
    {
      LOG(fatal)
		    << "No InputDataLevelName array!\n CbmSpadicTriggerComp will be inactive";
      return kERROR;
    }
  fProcSpadic = new TClonesArray ("CbmSpadicRawMessage");
  ioman->Register ("SpadicProcessedMessage", "TRD Processed Messages",
		   fProcSpadic, kTRUE);

  return kSUCCESS;
}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmSpadicTriggerComp::ReInit ()
{
  LOG(debug) << "Initilization of CbmSpadicTriggerComp";
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmSpadicTriggerComp::Exec (Option_t*)
{
  fProcSpadic->Clear ();
  LOG(info) << "CbmSpadicTriggerComp: Number of current TimeSlice:"
	       << fNrTimeSlices++;
  LOG(info) << "CbmSpadicTriggerComp: rawMessages in TS:          "
	       << fRawSpadic->GetEntriesFast ();
  LOG(debug) << "CbmSpadicTriggerComp: Begin sorting CbmSpadicMessage*";
  Int_t nSpadicMessages = fRawSpadic->GetEntriesFast (); //SPADIC messages per TimeSlice
  CbmSpadicRawMessage* raw = nullptr;
  std::deque<CbmSpadicRawMessage*> processedMessages;
  std::deque<CbmSpadicRawMessage*> selfCreatedMessages;
  std::map<Int_t,std::vector<CbmSpadicRawMessage*> > channelSortedMessages;
  for (Int_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      raw = static_cast<CbmSpadicRawMessage*> (fRawSpadic->At (iSpadicMessage));
      if (raw == nullptr)
	{
	  LOG(debug)
			<< "CbmSpadicTriggerComp: found nullptr in fRawSpadic in Timeslice: "
			<< fNrTimeSlices;
	  continue;
	}
      Int_t spadicID = GetSpadicID(raw->GetSourceAddress());
      Int_t sysID = 0;//to be filled with appropriate function.
      Int_t chID = raw->GetChannelID();
      Int_t linearizedChID=sysID*NrOfHalfSpadics*16+spadicID*16+chID;
      if(spadicID == -1|| chID < 0 ||chID>15){
	  processedMessages.push_back(raw);
	  LOG(debug) << "CbmSpadicTriggerComp: found unprocessable CbmSpadicMessage*";
	  continue;
      }
      if(raw->GetHit() == false && raw->GetHitAborted() == false){
	  processedMessages.push_back(raw);
	  LOG(debug) << "CbmSpadicTriggerComp: found CbmSpadicMessage* not to be processed";
	  continue;
      }
/*      if(raw->GetStopType()==0){
	  processedMessages.push_back(raw);
	  continue;
      }*/
      channelSortedMessages[linearizedChID].push_back(raw);
    }
  LOG(info) << "CbmSpadicTriggerComp: starting naive rejoin";
  Int_t samples[64];
  for(Int_t i =0;i<64;i++)
    samples[i]=-255;
  Int_t samplesCount = 0;
  for (auto &ent : channelSortedMessages)
    {
      LOG(debug) << "CbmSpadicTriggerComp: processing linear ChID: "
		    << ent.first;
      LOG(debug) << "CbmSpadicTriggerComp: linear ChID Size: "
		    << ent.second.size();
      CbmSpadicRawMessage* firstMessage = nullptr;
      for (UInt_t i =0; i<ent.second.size();i++){
	  CbmSpadicRawMessage* currentMessage = ent.second.at(i);
	  if(currentMessage == nullptr){
	      continue;
	  }
	  if (firstMessage == nullptr)
	    {
	      if(currentMessage->GetStopType()==0){
		processedMessages.push_back(currentMessage);
		continue;
	    }
	      firstMessage = currentMessage;
	    }
	  //Naive implementation: try to fill 32 Samples, discarding true multihits
	  Int_t NrSamples = currentMessage->GetNrSamples ();
	  Int_t* MessageSamples = currentMessage->GetSamples ();
	  for (Int_t j = 0; (j < NrSamples || j + samplesCount < 64); j++)
	    {
	      samples[j + samplesCount] = *(MessageSamples + j);
	    }
	  samplesCount += NrSamples;
	  if (samplesCount >= 32)
	    {
	      //create temporary CbmSpadicRawMessage to hold the newly constructed Message
	      CbmSpadicRawMessage* tempMessage = new CbmSpadicRawMessage (
		  firstMessage->GetEquipmentID (),
		  firstMessage->GetSourceAddress (),
		  firstMessage->GetChannelID (),
		  firstMessage->GetEpochMarker (), firstMessage->GetTime (),
		  firstMessage->GetSuperEpoch (),
		  firstMessage->GetTriggerType (), firstMessage->GetInfoType (),
		  0, firstMessage->GetGroupId (),
		  firstMessage->GetBufferOverflowCount (), 32, samples, true,
		  false, false, false, false, false, false);
	      processedMessages.push_back (tempMessage);
	      selfCreatedMessages.push_back (tempMessage);
	      firstMessage = nullptr;
	      for (Int_t j = 0; j < 64; j++)
		samples[j] = -255;
	      samplesCount = 0;
	    }
      }
  }
  auto CompareSpadicMessages=
      [&](CbmSpadicRawMessage* a,CbmSpadicRawMessage* b)
	  {
	  if(a->GetFullTime() < b->GetFullTime())
	    return true;
	  else
	    return false;
      };
  std::sort(processedMessages.begin(),processedMessages.end(),CompareSpadicMessages);
  for (UInt_t i =0; i<processedMessages.size();i++){
      CbmSpadicRawMessage* currentMessage = processedMessages.at (i);
      new ((*fProcSpadic)[fProcSpadic->GetEntriesFast ()]) CbmSpadicRawMessage (
	  currentMessage->GetEquipmentID (),
	  currentMessage->GetSourceAddress (), currentMessage->GetChannelID (),
	  currentMessage->GetEpochMarker (), currentMessage->GetTime (),
	  currentMessage->GetSuperEpoch (), currentMessage->GetTriggerType (),
	  currentMessage->GetInfoType (), currentMessage->GetStopType (),
	  currentMessage->GetGroupId (),
	  currentMessage->GetBufferOverflowCount (),
	  currentMessage->GetNrSamples (), currentMessage->GetSamples (),
	  currentMessage->GetHit (), currentMessage->GetInfo (),
	  currentMessage->GetEpoch (), currentMessage->GetEpochOutOfSynch (),
	  currentMessage->GetHitAborted (), currentMessage->GetOverFlow (),
	  currentMessage->GetStrange ());
    }
  LOG(info) << "CbmSpadicTriggerComp: Created Messages in TS:     "
	       << selfCreatedMessages.size ();
  for (auto ptr : selfCreatedMessages){
      delete ptr;
  }
  LOG(info) << "CbmSpadicTriggerComp: processedMessages in TS:    "
	       << fProcSpadic->GetEntriesFast ();
}

// ---- FinishEvent----------------------------------------------------
/*void CbmSpadicTriggerComp::FinishEvent ()
{
  LOG(debug) << "FinishEvent of CbmSpadicTriggerComp";
  fProcSpadic->Clear ();
}*/

Int_t CbmSpadicTriggerComp::GetSpadicID(Int_t sourceA)
{
  //TString spadic="";
  Int_t SpaId = -1;
  switch (sourceA) {
  case (SpadicBaseAddress+0):  // first spadic
    //spadic="Spadic0";
    SpaId = 0;
    break;
  case (SpadicBaseAddress+1):  // first spadic
    //spadic="Spadic0";
    SpaId = 1;
    break;
  case (SpadicBaseAddress+2):  // second spadic
    //spadic="Spadic1";
    SpaId = 2;
    break;
  case (SpadicBaseAddress+3):  // second spadic
    //spadic="Spadic1";
    SpaId = 3;
    break;
  case (SpadicBaseAddress+4):  // third spadic
    //spadic="Spadic2";
    SpaId = 4;
    break;
  case (SpadicBaseAddress+5):  // third spadic
    //spadic="Spadic2";
    SpaId = 5;
    break;
  default:
    LOG(error) << "Source Address " << sourceA << " not known.";
    break;
  }
  return SpaId;
}
// ---- Finish --------------------------------------------------------
void CbmSpadicTriggerComp::Finish ()
{
  LOG(debug) << "Finish of CbmSpadicTriggerComp";
  // Update Histos and Canvases
}/*
void CbmSpadicTriggerComp::FinishTask ()
{
  LOG(debug) << "Finish of CbmSpadicTriggerComp";
  // Update Histos and Canvases
}*/
