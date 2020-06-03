#include "CbmTrdTimeCorrel.h"

#include "CbmSpadicRawMessage.h"
#include "CbmHistManager.h"
#include "CbmBeamDefaults.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdAddress.h"

#include <FairLogger.h>

#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMultiGraph.h>
#include <TNtuple.h>
#include <TString.h>
#include <TMath.h>

#include <cmath>
#include <map>
#include <vector>
#include <iostream>
#include <stdexcept>

using std::vector;
using std::string;

// ----              -------------------------------------------------------
CbmTrdTimeCorrel::CbmTrdTimeCorrel()
  : FairTask("CbmTrdTimeCorrel"),
    fMessageBuffer(),
    fLinearHitBuffer(),
    fSpadics(0),
    fRewriteSpadicName(true),
    fBaseline(NrOfActiveSyscores*NrOfHalfSpadics*16,0),
    fBaselineMon (
	new TCanvas (TString ("Canvas_Baselinemonitor").Data (),
		     TString ("Baselinemonitor").Data (), 1600, 900)),
    fRawSpadic(NULL),
    fRawMessages(NULL),
    fDigis(NULL),
    fClusters(NULL),
    fiRawMessage(0),
    fiDigi(0),
    fiCluster(0),
    fHM(new CbmHistManager()),
    fNrTimeSlices(0),
    fRun(0),
    fLastMessageTime (NrOfActiveSyscores*NrOfSpadics*32,0),
    fEpochMarkerArray(NrOfActiveSyscores*NrOfHalfSpadics,0),
    fFirstEpochMarker(NrOfActiveSyscores*NrOfHalfSpadics,true),
    EpochRegressTriggered(NrOfActiveSyscores*NrOfHalfSpadics,false),
    EpochRegressOffset(NrOfActiveSyscores*NrOfHalfSpadics,0),
    EpochRegressCounter(NrOfActiveSyscores*NrOfHalfSpadics,0),
    timestampOffsets(),
    fGraph(true),
    fOutputCloneArrays(false),
    fClusterBuffer()
 {
   gStyle->SetNumberContours(99);
    {
      TString CalibrationFilename =
          FairRootManager::Instance ()->GetOutFile ()->GetName ();
      Int_t DefaultBaselineFrankfurt = -190;
      Int_t DefaultBaselineMuenster = -220;
      CalibrationFilename.ReplaceAll (".root", "_Calibration.root");
      TFile* Calibration = new TFile (CalibrationFilename.Data (), "READ");
      if (Calibration == nullptr)
        {
          LOG(info) << "No calibration file found, continuing with default baseline.";
          for (Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore)
            {
              for (Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic)
                {
                  for (Int_t channel = 0; channel < 32; ++channel)
                    {
                      fBaseline[syscore * NrOfHalfSpadics * 16 + spadic * 32 + channel] =
                          ((spadic / 2) == 0 ?
                              DefaultBaselineFrankfurt : DefaultBaselineMuenster);
                    }
                }
            }
        }
      else{ Calibration->cd();
          LOG(info) << "Calibration file found at " <<CalibrationFilename<< " reading baseline.";
          for (Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore)
            {
              for (Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic)
                {
                  for (Int_t channel = 0; channel < 32; ++channel)
                    {
                	  string spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic).Data();
                      TString fitname = "Fit_Baseline_for_"+spadicName
                          + "_Channel_" + std::to_string (channel);
                      TFitResult* CurrentFit = dynamic_cast<TFitResult*>(Calibration->Get(fitname.Data()));
                      if(CurrentFit==nullptr){
                          LOG(info) << "No baseline fit found for Syscore "<< syscore << " Spadic "<<spadic << " Channel "<<channel;
                          fBaseline[syscore * NrOfHalfSpadics * 16 + spadic * 16 + channel] =
                                                    ((spadic / 2) == 0 ?
                                                        DefaultBaselineFrankfurt : DefaultBaselineMuenster);
                      }
                      else {
                          fBaseline[syscore * NrOfHalfSpadics * 16 + spadic * 32 + channel] = CurrentFit->GetParams()[1]-0*CurrentFit->GetParams()[2];
                      }
                    }
                }
            }
          FairRootManager::Instance ()->GetOutFile ()->cd();
          Calibration->Close();
        }
    }
 };

// ----              -------------------------------------------------------
CbmTrdTimeCorrel::~CbmTrdTimeCorrel()
{
  LOG(debug) << "Destructor of CbmTrdTimeCorrel";
  fRawMessages->Clear();
  fDigis->Delete();
  fClusters->Delete();
}
// ----              -------------------------------------------------------
void CbmTrdTimeCorrel::SetParContainers()
{
 LOG(debug) << "SetParContainers of CbmTrdTimeCorrel";
}
// ----              -------------------------------------------------------
InitStatus CbmTrdTimeCorrel::Init()
{
  LOG(info) << "Initilization of CbmTrdTimeCorrel";
  FairRootManager* ioman = FairRootManager::Instance();
  fRawSpadic = static_cast<TClonesArray*>(ioman->GetObject("SpadicRawMessage"));
  if ( !fRawSpadic ) {
    LOG(fatal) << "No InputDataLevelName array!\n CbmTrdTimeCorrel will be inactive";
    return kERROR;
  }

  fRawMessages = new TClonesArray("CbmSpadicRawMessage");
  ioman->Register("TrdRawMessage", "TRD Raw Messages", fRawMessages, kTRUE);

  fDigis = new TClonesArray("CbmTrdDigi");
  ioman->Register("TrdDigi", "TRD Digis", fDigis, kTRUE);

  fClusters = new TClonesArray("CbmTrdCluster");
  ioman->Register("TrdCluster", "TRD Clusters", fClusters, kTRUE);

  CreateHistograms();
  return kSUCCESS;
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmTrdTimeCorrel::ReInit()
{
  LOG(debug) << "Initilization of CbmTrdTimeCorrel";
  return kSUCCESS;
}
// ---- Exec  -------------------------------------------------------
void CbmTrdTimeCorrel::Exec(Option_t*)
{
  const Int_t maxNrColumns = 16; //max number of channels on a pad plane per asic and row

  // Analysis based on single SPADIC data streams can be done here!!!

  if(fNrTimeSlices==0){
    if(fHM->G1("TsCounter")->GetN()!=0){
      LOG(info) << "Expected empty TsCounter before first TimeSlice, but found " << fHM->G1("TsCounter")->GetN() << " entries.";
    }
  }
  
  TString spadicName = "";
  TString histName="";
  TString title="";
  


  LOG(info) << "CbmTrdTimeCorrel: Number of current TimeSlice:   " << fNrTimeSlices;
  LOG(info) << "Digis in TClonesArray:                           " << fDigis->GetEntriesFast();
  LOG(info) << "Clusters in TClonesArray:                        " << fClusters->GetEntriesFast();
  LOG(info) << "rawMessages in TS:                               " << fRawSpadic->GetEntriesFast();
  fDigis->Delete();
  fDigis = new TClonesArray("CbmTrdDigi");
  fClusters->Delete();
  fClusters = new TClonesArray("CbmTrdCluster");
  Int_t nSpadicMessagesFull = fRawSpadic->GetEntriesFast(); //SPADIC messages per TimeSlice
  Int_t nSpadicMessages[NrOfActiveSyscores*NrOfSpadics]; //SPADIC messages per TimeSlice for single SPADICS
  //SPADIC message types per TimeSlice for single SPADICS:
  Int_t nSpadicMessagesHit[NrOfActiveSyscores*NrOfSpadics]={};
  Int_t nSpadicMessagesHitAborted[NrOfActiveSyscores*NrOfSpadics]={};
  Int_t nSpadicMessagesOverflow[NrOfActiveSyscores*NrOfSpadics]={};
  Int_t nSpadicMessagesInfo[NrOfActiveSyscores*NrOfSpadics]={};
  Int_t nSpadicMessagesEpoch[NrOfActiveSyscores*NrOfSpadics]={};
  Int_t nSpadicMessagesLost[NrOfActiveSyscores*NrOfSpadics]={};
  Int_t nSpadicMessagesStrange[NrOfActiveSyscores*NrOfSpadics]={};
  Int_t lostMessages(0);// this variable should be reset to 0 for every SPADIC message
  // Getting message type bools from Spadic raw message
  Bool_t isHit = false;
  Bool_t isHitAborted = false;
  Bool_t isOverflow = false;
  Bool_t isInfo = false;
  Bool_t isEpoch = false;
  Bool_t isEpochOutOfSynch = false;
  Bool_t isStrange = false;
  
  // Initialise message coordinates to -1 to recognise unset variables
  Int_t eqID(-1), sourceA(-1), triggerType(-1), stopType(-1), infoType(-1), groupId(-1), sysID(-1), spaID(-1);
  // chID in ASIC. Take care, neighboured numbers are not neccessarily neighboured on the connected TRD cathode pad plane. Resorted lateron!
  // padID are sorted chIDs in the order as on the pad plane
  Int_t chID(-1), padID(-1), columnID(-1), rowID(-1), combiID(-1);
  ULong_t time = 0;
  // Time stamp and epoch are counted in the Spadic
  Int_t timeStamp(0), epoch(0), superEpoch(0);
  LOG(info) << "nSpadicMessages: " << nSpadicMessagesFull;

  Int_t hitCounter[NrOfActiveSyscores*NrOfSpadics]={};
  CbmSpadicRawMessage* raw = NULL;

  // Do the message type counting per timeslice here to make the full numbers available early in the following analysis loop
  for (Int_t iSpadicMessage=0; iSpadicMessage < nSpadicMessagesFull; ++iSpadicMessage){
    raw = static_cast<CbmSpadicRawMessage*>(fRawSpadic->At(iSpadicMessage));
    lostMessages = 0; // reset lost-counter for a new message
    sourceA = raw->GetSourceAddress();
    spaID = GetSpadicID(sourceA);
    eqID = raw->GetEquipmentID();
    sysID = GetSyscoreID(eqID);
    spadicName = GetSpadicName(eqID,sourceA);
    chID = raw->GetChannelID();
    if(chID > -1 && chID < 16 && spaID%2==1) chID+=16; // eqID ?
    isHit = raw->GetHit();
    isHitAborted = raw->GetHitAborted();
    isOverflow = raw->GetOverFlow();
    isInfo = raw->GetInfo();
    isEpoch = raw->GetEpoch();
    isEpochOutOfSynch = raw->GetEpochOutOfSynch();
    isStrange = raw->GetStrange();

    fHM->H2("SourceAvsEquipIDall")->Fill(eqID,sourceA);
    if(isEpoch) fHM->H2("SourceAvsEquipIDepoch")->Fill(eqID,sourceA);
    if(isHit && chID >= 0 && chID <= 15) fHM->H2("SourceAvsEquipIDhit")->Fill(eqID,(sourceA + GetChannelOnPadPlane(chID)*0.0625 + 0.03125)); // For hit messages, the originating padID is displayed within the sourceA axis
    
    // Count total messages per ASIC and message-types per ASIC.
    {
      nSpadicMessages[NrOfSpadics*sysID+spaID]++;
      if(isHit) nSpadicMessagesHit[NrOfSpadics*sysID+spaID]++;
      else if(isHitAborted) nSpadicMessagesHitAborted[NrOfSpadics*sysID+spaID]++;
      else if(isOverflow) {
	nSpadicMessagesOverflow[NrOfSpadics*sysID+spaID]++;
	if (lostMessages > 0) nSpadicMessagesLost[NrOfSpadics*sysID+spaID] += lostMessages; //lostMessages might be -1 for hits or epochs, therefore one has to ensure that it is > 0
      }
      else if(isInfo) nSpadicMessagesInfo[NrOfSpadics*sysID+spaID]++;
      else if(isEpoch) nSpadicMessagesEpoch[NrOfSpadics*sysID+spaID]++;
      else if(isStrange) nSpadicMessagesStrange[NrOfSpadics*sysID+spaID]++;
    }
    
    // Currently only expecting Spadic0 and Spadic1. Logging others, if appearing.
  }
  {
	  TString path = FairRootManager::Instance()->GetOutFile()->GetName();
	  TString RunNumber = path(TRegexp("[0-9]+_"));
	Int_t Cap = 1000000;
  if(RunNumber.Contains("111_")) Cap=11400;
  if(RunNumber.Contains("97_")) Cap=2000;
  if (fNrTimeSlices>Cap){
	  fNrTimeSlices++;
	  return;
	  }

  }
  static Long64_t lastHitTimestamp[NrOfActiveSyscores*NrOfSpadics*32];
  if (fNrTimeSlices==0){
	for(size_t i=0;i<NrOfActiveSyscores*NrOfSpadics*32;i++){
		lastHitTimestamp[i]=0;
	}
  }

  // Starting to loop over all Spadic messages in unpacked TimeSlice, Analysis loop
  for (Int_t iSpadicMessage=0; iSpadicMessage < nSpadicMessagesFull; ++iSpadicMessage) {
    //std::cout << "  " << iSpadicMessage << std::endl;
    raw = static_cast<CbmSpadicRawMessage*>(fRawSpadic->At(iSpadicMessage));
    lostMessages = 0; // reset lost-counter for a new message
    chID = -1; // reset to notice if set to a new ID
    padID = -1;
    isHit = raw->GetHit();
    isHitAborted = raw->GetHitAborted();
    isOverflow = raw->GetOverFlow();
    isInfo = raw->GetInfo();
    isEpoch = raw->GetEpoch();
    isStrange = raw->GetStrange();
    isEpochOutOfSynch = raw->GetEpochOutOfSynch();
    // Seriously guys, a message can only be of one type.
    if(Int_t(isHit+isInfo+isEpoch+isHitAborted+isOverflow+isStrange+isEpochOutOfSynch)!=1) LOG(error) << "SpadicMessage " << iSpadicMessage << " is classified from CbmSpadicRawMessage to be: HIT " << (Int_t)isHit << " / INFO " << (Int_t)isInfo << " / EPOCH " << (Int_t)isEpoch << " / HITaborted " << (Int_t)isHitAborted << " / OVERFLOW " << (Int_t)isOverflow << " / STRANGE " << (Int_t)isStrange;

    // Get SysCore & Spadic propertys
    eqID = raw->GetEquipmentID();
    sourceA = raw->GetSourceAddress();

    groupId=raw->GetGroupId();
    chID = raw->GetChannelID();
    sysID = GetSyscoreID(eqID);
    spaID = GetSpadicID(sourceA);
    if(chID > -1 && chID < 16 && spaID%2==1) chID+=16; // eqID ?
    padID = GetChannelOnPadPlane(chID);// Remapping from ASIC to pad-plane channel numbers.

    columnID = GetColumnID(raw);
    rowID = GetRowID(raw);
    combiID = rowID * (maxNrColumns + 1) + columnID;// Is needed to cluster messages within one detector layer. combiID provides a linear representation of  a 2dim coordinate system (row, column) with an additional column. By using continues combiIDs one avoids to build continues clusters between to rows.


    //    Int_t nrSamples=raw->GetNrSamples();

    lostMessages = raw->GetBufferOverflowCount();


    if (!isOverflow && lostMessages!=0) LOG(error) << "SpadicMessage " << iSpadicMessage << " is HIT " << (Int_t)isHit << " / INFO " << (Int_t)isInfo << " / EPOCH " << (Int_t)isEpoch << " / HITaborted " << (Int_t)isHitAborted << " / STRANGE " << (Int_t)isStrange << " but claims to have lost " << lostMessages << " messages";
    time = raw->GetFullTime();
    timeStamp = raw->GetTime();
    
    epoch = raw->GetEpochMarker();// is copied to each SpadicRawMessage by the unpacker not only epoch messages
    superEpoch = raw->GetSuperEpoch();// is copied to each SpadicRawMessage by the unpacker not only epoch message

    stopType = raw->GetStopType();
    TString stopName = GetStopName(stopType);
    infoType=raw->GetInfoType();
    triggerType = raw->GetTriggerType();

    if(isHit) hitCounter[sysID*NrOfHalfSpadics+spaID]++;
    if(isHit||isHitAborted){
    	TString TempName = "Hitfrequency_for_"+GetSpadicName(sysID, spaID, kProcessedData,kFullSpadic);
    	string HistName=TempName.Data();
    	if(fNrTimeSlices>0)fHM->G1(HistName)->SetPoint(fHM->G1(HistName)->GetN(),static_cast<Double_t>(time)/15000000.0,15000000/std::fabs((static_cast<Double_t>(lastHitTimestamp[spaID*16+raw->GetChannelID()])-static_cast<Double_t>(time))));
//    	fHM->H2(HistName+"_linear")->Fill(fNrTimeSlices,17600000/std::fabs((static_cast<Double_t>(lastHitTimestamp[spaID*16+raw->GetChannelID()])-static_cast<Double_t>(time))));
    	lastHitTimestamp[sysID*NrOfHalfSpadics*16+spaID*16+raw->GetChannelID()]=time;
    }

    if (raw->GetChannelID()>100) LOG(error) << "SpadicMessage with strange chID: " << iSpadicMessage << " sourceA: " << sourceA << " chID: " << raw->GetChannelID() << " groupID: " << groupId << " spaID: " << spaID << " stopType: " << stopType << " infoType: " << infoType << " triggerType: " << triggerType << " isHit: " << isHit << " isInfo: " << isInfo << " isEpoch: " << isEpoch << " Lost Messages: " << lostMessages;
    if (raw->GetChannelID()>32 && !isInfo && !isStrange) LOG(fatal) << "SpadicMessage: " << iSpadicMessage << " sourceA: " << sourceA << " chID: " << raw->GetChannelID() << " groupID: " << groupId << " spaID: " << spaID << " stopType: " << stopType << " infoType: " << infoType << " triggerType: " << triggerType << " isHit: " << isHit << " isInfo: " << isInfo << " isEpoch: " << isEpoch << " Lost Messages: " << lostMessages;


    if (infoType > 6) {
      LOG(error) << " InfoType " << infoType << "is larger 6, set to 7!";
      infoType = 7;
    }
    
    spadicName = GetSpadicName(eqID,sourceA,kRawData,kFullSpadic);
    //printf("spadicName:%s:\n",spadicName.Data());
    if ( isOverflow )
      if (spadicName != "")
	fHM->H2((spadicName + TString("_LostCount_vs_PadID")).Data())->Fill(lostMessages,padID);

    // add raw message to map sorted by timestamps, syscore and spadic
    if(spadicName == ""){
      LOG(error) << "eqID:" << eqID << " sourceA:" << sourceA << " spadicName:" << spadicName;
      //continue;
    } else {
      //if (false)
  
      if (!isStrange && !isEpoch && !isEpochOutOfSynch){

	if (!isInfo) {
	  std::map<Int_t, CbmSpadicRawMessage*>::iterator MessageBufferIt = fMessageBuffer[TString(spadicName)][time].find(combiID);
	  if (MessageBufferIt == fMessageBuffer[TString(spadicName)][time].end()){ 
	    /*
	      if there is no message found for spadicName at this time and the same padID (which should never be the case) the 
	      processed message is added to the map. This avoids per definition the use of overlapping microslices. It is up to you to 
	      define the time for spacial and time clusterization (after each timeSliceContainer or ad the end of the file. This will 
	      be mainly a question of available RAM. It might be a good idea to clusterize after each timeSliceContainer, write all 
	      rawMessages inside of the buffer to a TClonesArray or TTree as well as all found CbmTrdClusters to a separate TClonesArray 
	      or TTree. Afterwards one should erase the buffer partially (leaving a rest of messages at the end of the buffer to be able 
	      to cluster messages at the beginning of the next TimeSliceContainer.
	    */
	    //fMessageBuffer[TString(spadicName)][time][combiID] = raw;
	    /*
	      In order to not interfere with the clean up of the TClonesArray, we copy here the information and delete the stored objects in the clean up of the map
	    */
	    fMessageBuffer[TString(spadicName)][time][combiID] = new CbmSpadicRawMessage(raw->GetEquipmentID(), raw->GetSourceAddress(), raw->GetChannelID(),
											 raw->GetEpochMarker(), raw->GetTime(), 
											 raw->GetSuperEpoch(), raw->GetTriggerType(),
											 raw->GetInfoType(), raw->GetStopType(), 
											 raw->GetGroupId(), raw->GetBufferOverflowCount(), 
											 raw->GetNrSamples(), raw->GetSamples(),
											 raw->GetHit(), raw->GetInfo(), raw->GetEpoch(), raw->GetEpochOutOfSynch(), raw->GetHitAborted(), raw->GetOverFlow(), raw->GetStrange());

	    /*
	      A new TClonesArray without MS overlaps is created for offline analysis
	    *//*
		new ((*fRawMessages)[fiRawMessage])CbmSpadicRawMessage(raw->GetEquipmentID(), raw->GetSourceAddress(), raw->GetChannelID(),
		raw->GetEpochMarker(), raw->GetTime(), 
		raw->GetSuperEpoch(), raw->GetTriggerType(),
		raw->GetInfoType(), raw->GetStopType(), 
		raw->GetGroupId(), raw->GetBufferOverflowCount(), 
		raw->GetNrSamples(), raw->GetSamples(),
		raw->GetHit(), raw->GetInfo(), raw->GetEpoch(), raw->GetEpochOutOfSynch(), raw->GetHitAborted(), raw->GetOverFlow(), raw->GetStrange());
	      */
	    fiRawMessage++;
	    if (isHit&&(triggerType==1||triggerType==3))fHM->H2(TString("Map_of_Hits_for_"+GetSpadicName(eqID,sourceA,kRawData,kFullSpadic)).Data())->Fill(padID%16,static_cast<Int_t>(padID/16));
	    if (isHit&&(triggerType==1||triggerType==3))fHM->H2(TString("Map_of_Charge_for_"+GetSpadicName(eqID,sourceA,kRawData,kFullSpadic)).Data())->Fill(padID%16,static_cast<Int_t>(padID/16),GetMaxADC(*raw,false,&fBaseline));
	    if (isHit) FillBaselineHistogram(raw);
	    if(fActivateClusterizer)
	      if (isHit || isHitAborted || isOverflow)
	        if(fNrTimeSlices!=0){
	          /*
	          CbmSpadicRawMessage* tempPtr = new CbmSpadicRawMessage(*raw);
	          */
	          fLinearHitBuffer.push_back(raw);
	          string Histname =  "Signalshape_for_" + string(spadicName) + "Channel"
						+ std::to_string(chID);
	          if(triggerType==1&&(stopType==0)) FillSignalShape(*raw,Histname);
			  TString graphName = "TSTime_"+GetSpadicName(eqID,sourceA,kRawData,kHalfSpadic);
			  fHM->G1(graphName.Data())->SetPoint(fHM->G1(graphName.Data())->GetN(),fNrTimeSlices+1,raw->GetFullTime()/1.5E7);
	        }
	    if(fDrawSignalDebugHistograms){
	    	if (isHit||isHitAborted){
	    		Int_t maxADC= GetMaxADC(*raw,false,&fBaseline);
	    		Int_t TriggerType = raw->GetTriggerType();
	    		if(!(TriggerType<0 ||TriggerType>3)){
	    			string TriggerName;
	    			{
	    				TString triggerTypes[4] = {
	    						"Global trigger", "Self triggered",
								"Neighbor triggered",
								"Self and neighbor triggered" };
	    				TriggerName = triggerTypes[TriggerType];
	    			}
	    			Int_t StopType = raw->GetStopType();
	    			Int_t AvgBaseline=GetAvgBaseline(*raw);
	    			string Histname = "MaxADC_vs_NrSamples_for_"
	    					+string(GetSpadicName(sysID, spaID, kProcessedData,kFullSpadic).Data())+"_"+TriggerName;
	    			string Hittype;
	    			fHM->H2(Histname)->Fill(raw->GetNrSamples(),maxADC);
	    			if (StopType==0) {
	    				Hittype = "_Hits";
	    			}else {
	    				Hittype = "_MultiHits";
	    			}
	    			Histname = "MaxADC_vs_avgBaseline_for_"
	    					+string(GetSpadicName(sysID, spaID, kProcessedData,kFullSpadic).Data())+Hittype+"_"+TriggerName;
	    			if(raw->GetSamples()[0] < maxADC) {
		    			fHM->H2(Histname)->Fill(AvgBaseline,maxADC);
	    			}else{
	    				Histname = "Exp_Decay" +Histname;
	    				fHM->H2(Histname)->Fill(raw->GetSamples()[raw->GetNrSamples()-1]/*AvgBaseline*/,maxADC);
	    			}
	    			Histname = "avgBaseline_for_"
	    			+string(GetSpadicName(sysID, spaID, kProcessedData,kFullSpadic).Data())+"_"+TriggerName;
	    			if(raw->GetStopType()==0)fHM->H1(Histname)->Fill(AvgBaseline);
	    			Histname = "avgBaseline_vs_NrSamples_for_"
	    					+string(GetSpadicName(sysID, spaID, kProcessedData,kFullSpadic).Data())+"_"+TriggerName;
	    			fHM->H2(Histname)->Fill(raw->GetNrSamples(),raw->GetSamples()[raw->GetNrSamples()-1]/*AvgBaseline*/);
	    			Histname = "First_Sample_vs_last_Sample_for_"
	    					+string(GetSpadicName(sysID, spaID, kProcessedData,kFullSpadic).Data())+Hittype+"_"+TriggerName;
	    			if(raw->GetSamples()[0] < maxADC) {
		    			fHM->H2(Histname)->SetBinContent(fHM->H2(Histname)->FindBin(0),0);
		    			fHM->H2(Histname)->Fill(raw->GetSamples()[raw->GetNrSamples()-1],raw->GetSamples()[0]);
	    			}else{
	    				Histname = "Exp_Decay" +Histname;
		    			fHM->H2(Histname)->SetBinContent(fHM->H2(Histname)->FindBin(0),0);
		    			fHM->H2(Histname)->Fill(raw->GetSamples()[raw->GetNrSamples()-1],raw->GetSamples()[0]);
	    			}
	    		}
	    	}
	    }

	  } else {  
	    
	    LOG(debug) << "Found Message already in fMessageBuffer at :" << TString(spadicName).Data() << ":, time:" << time << ", padID:" << padID << ". Potential overlapping MS container!";

	    if (fMessageBuffer[TString(spadicName)][time][combiID] != NULL)
	      fHM->H2("DoubleMessage_MessageType")->Fill(GetMessageType(fMessageBuffer[TString(spadicName)][time][combiID]),GetMessageType(raw));
	    else 	  
	      fHM->H2("DoubleMessage_MessageType")->Fill(-1,GetMessageType(raw));
	    /*
	      LOG(info) << "------------------------------------";
	      raw->PrintMessage();
	      LOG(info) << "<raw---------------------------->";
	      fMessageBuffer[TString(spadicName)][time][combiID]->PrintMessage();
	      LOG(info) << ">buffer-------------------------<";
	      LOG(info) << "------------------------------------";
	    */
	  }
	}
      }
      if (isInfo){
	if(chID < 32)fHM->H2("InfoType_vs_Channel")->Fill(padID,infoType);
	else fHM->H2("InfoType_vs_Channel")->Fill(33,infoType); // chIDs greater than 32 are quite strange and will be put into the last bin
      }
      if (isEpoch){   // fill epoch messages in an additional row
	if(chID < 32)fHM->H2("InfoType_vs_Channel")->Fill(padID,9);
	else fHM->H2("InfoType_vs_Channel")->Fill(33,9);
      }
      if (isOverflow){   // fill overflow messages in an additional row
	if(chID < 32)fHM->H2("InfoType_vs_Channel")->Fill(padID,10);
	else fHM->H2("InfoType_vs_Channel")->Fill(33,10);
      }
      if (isEpoch) {
	if (!fFirstEpochMarker[sysID*NrOfHalfSpadics+spaID]) {
	  fHM->H1(TString("Delta_Epoch_hist_for_"+GetSpadicName(eqID, sourceA, kRawData,kHalfSpadic)).Data())->Fill(epoch - fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID]);
	  fHM->H2(TString("Hitrate_vs_Delta_Epoch_hist_for_"+GetSpadicName(eqID, sourceA, kRawData,kHalfSpadic)).Data())->Fill(epoch - fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID], nSpadicMessagesHit[sysID*NrOfHalfSpadics+spaID]);
	  // Book keep here the trending of Epoch Counters after a regress has been detected
	  if ( EpochRegressTriggered[sysID*NrOfHalfSpadics+spaID] ) { // Fill subsequent epoch messages in the histo, if the mechanism is triggered
	    EpochRegressCounter[sysID*NrOfHalfSpadics+spaID]++; // Count how many epochs are put in the trending histo already
	    LOG(info) << "EPOCH REGRESS ongoing (step " << EpochRegressCounter[sysID*NrOfHalfSpadics+spaID] << ") for spaID " << spaID << " from " << fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID] << " to " << epoch << " (timeslice " << fNrTimeSlices << ", spadicmessage " << iSpadicMessage << ")";
	    fHM->H2(TString("EpochTrend_postregress_hist_for_"+GetSpadicName(sysID, spaID, kProcessedData,kHalfSpadic)).Data())->Fill(EpochRegressCounter[sysID*NrOfHalfSpadics+spaID],epoch - EpochRegressOffset[sysID*NrOfHalfSpadics+spaID]);
	    if ( EpochRegressCounter[sysID*NrOfHalfSpadics+spaID] >= 150 ) { // After this condition is fulfilled, enough epochs have been recorded -- reset trigger and counter
	      EpochRegressTriggered[sysID*NrOfHalfSpadics+spaID] = false;
	      EpochRegressCounter[sysID*NrOfHalfSpadics+spaID] = 0;
	    }
	  }
	  if (epoch - fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID] <= 0  && !EpochRegressTriggered[sysID*NrOfHalfSpadics+spaID]) { // Trigger the recording if a regress is detected and the recording is not running up to now. The natural overflow of epoch (regress of 4095) is excluded
	    fHM->G1(TString("DeltaEpoch_vs_timeslice_for_"+GetSpadicName(eqID, sourceA, kRawData,kHalfSpadic)).Data())->SetPoint(fHM->G1(TString("DeltaEpoch_vs_timeslice_for_"+GetSpadicName(eqID, sourceA, kRawData,kHalfSpadic)).Data())->GetN(),fNrTimeSlices,epoch - fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID]); // Fill TGraph with every regress of Epoch, also the normal overflows by 4095
	    if (epoch - fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID] > -4000) { // but trigger the recording only for irregular regress
	      LOG(info) << "EPOCH REGRESS triggered for sysId "<< sysID <<" spaID " << spaID << " from " << fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID] << " to " << epoch << " (timeslice " << fNrTimeSlices << ", spadicmessage " << iSpadicMessage << ")";
	      EpochRegressTriggered[sysID*NrOfHalfSpadics+spaID] = true;
	      EpochRegressOffset[sysID*NrOfHalfSpadics+spaID] = epoch;
	    }
	  }
	}
     	fEpochMarkerArray[sysID*NrOfHalfSpadics+spaID] = epoch;
	fFirstEpochMarker[sysID*NrOfHalfSpadics+spaID] = false;
      }
      //if(spadicName!="") {

      //
      //  DEBUG PLOTS
      //
      
      if(isHit && chID >= 0 && chID < 32) {
    	  histName = GetSpadicName(eqID,sourceA,kRawData,kHalfSpadic) + "_Time_vs_TimeSlice";
    	  fHM->H2(histName.Data())->Fill(fNrTimeSlices,time); //timeStamp(0), epoch(0), superEpoch(0);
      }

      if(isEpoch && chID >= 0 && chID < 32) {
    	  histName = GetSpadicName(eqID,sourceA,kRawData,kHalfSpadic) + "_Epoch_vs_TimeSlice";
    	  fHM->H2(histName.Data())->Fill(fNrTimeSlices,epoch); //timeStamp(0), epoch(0), superEpoch(0);
      }
      
      // These are pure debuging histos to ensure that the unpacker is running without errors (C.B)
      fHM->H2("TriggerType_vs_InfoType")->Fill(raw->GetTriggerType(), raw->GetInfoType());
      fHM->H2("TriggerType_vs_StopType")->Fill(raw->GetTriggerType(), raw->GetStopType());
      fHM->H2("InfoType_vs_StopType")->Fill(raw->GetInfoType(), raw->GetStopType());
      fHM->H2("NrSamples_vs_StopType")->Fill(raw->GetNrSamples(), raw->GetStopType());
      fHM->H2("NrSamples_vs_InfoType")->Fill(raw->GetNrSamples(), raw->GetInfoType());
      fHM->H2("NrSamples_vs_TriggerType")->Fill(raw->GetNrSamples(), raw->GetTriggerType());

      //-------------------
      
      /*Extended Message Debugging:
	-Check for deformed Messages
	-Check for empty Messages claiming to be normally stopped
	->search for BufferOverflows
      */
      if(false){
	if(stopType == 0 && (chID <-1 || chID >32)) LOG(fatal) << "SpadicMessage: " << iSpadicMessage << " sourceA: " << sourceA << " chID: " << chID << " groupID: " << groupId << " spaID: " << spaID << " stopType: " << stopType << " infoType: " << infoType << " triggerType: " << triggerType << " isHit: " << isHit << " is Info: " << isInfo;
	if(stopType == 0 && raw->GetNrSamples()==0 && iSpadicMessage < nSpadicMessagesFull){
	  for ( Int_t i=iSpadicMessage;i<nSpadicMessagesFull;i++){
	    if ((static_cast<CbmSpadicRawMessage*>(fRawSpadic->At(i)))->GetOverFlow()==true && GetSpadicID((static_cast<CbmSpadicRawMessage*>(fRawSpadic->At(i)))->GetSourceAddress())==spaID)
	      {
		lostMessages = (static_cast<CbmSpadicRawMessage*>(fRawSpadic->At(i)))->GetBufferOverflowCount();
		break;
	      }
	    //LOG(info) << i << "Info not found ";
	  }
	  LOG(info) << "SpadicMessages: " << nSpadicMessagesFull << " Lost Messages ";
	  LOG(error) << "SpadicMessage: " << iSpadicMessage << " sourceA: " << sourceA << " chID: " << raw->GetChannelID() << " groupID: " << groupId << " spaID: " << spaID << " stopType: " << stopType << " infoType: " << infoType << " triggerType: " << triggerType << " isHit: " << isHit << " is Info: " << isInfo << " Lost Messages: " << lostMessages;
	}
      }


      //Fill trigger-type histogram
      fHM->H1("Trigger")->Fill(spadicName,1);
      fHM->H1("MessageCount")->Fill(TString(spadicName+"_"+stopName),1);
      if(stopType==-1) fHM->H1("MessageCount")->Fill(TString(spadicName+"_"+stopName+" n-fold"),1); // replace weight 1 with number of lost messages
    }
    //std::cout << "   tuut" << std::endl;
  }
  // complicated loop over sorted map of timestamps, manually delete all elements in the nested maps
  // commented out, since obviuously the following outer clear command destructs all contained elements recursively
  // The objects correlated to the pointers stored in this map are deleted after each run of Exec. Therefore it is leading to seg.fa. if one tryies to delete the pointers here twice
  /*
    for(std::map<TString, std::map<ULong_t, std::vector<CbmSpadicRawMessage*> > >::iterator it = timeBuffer.begin() ; it != timeBuffer.end(); it++){
    // complicated loop over sorted map of raw messages
    for (std::map<ULong_t, std::vector<CbmSpadicRawMessage*> > ::iterator it2 = it->second.begin() ; it2 != it->second.end(); it2++) {
    //      LOG(info) <<  "ClusterSize:" << Int_t(it2->second.size());
    //for (std::vector<CbmSpadicRawMessage*>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
    for(Int_t i = 0; i < Int_t(it2->second.size()); i++){
    //delete it2->second[i];//it3->second;
    // here: looping through the vector
    }
    //      it2->second.clear();
    }
    //    it->second.clear();
    }
  */

  // Fill number of spadic-messages in tscounter-graph. Use TimeSlices (slices in processing time) here instead of physical full-time on the x-axis.
  // Length of one timeslice: m * n * 8 ns, with e.g. n=1250 length of microslice and m=100 microslices in one timeslice at SPS2015
  if (fGraph) {
	  std::vector<Int_t> spadicList(NrOfActiveHalfSpadics);
	  std::iota(spadicList.begin(),spadicList.end(),0);
	  fHM->G1("TsCounter")->SetPoint(fHM->G1("TsCounter")->GetN(),fNrTimeSlices+1,nSpadicMessagesFull);
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(auto spadic : spadicList) {
			  string SpadicName = GetSpadicName(syscore, spadic, kDirectOutput,kHalfSpadic).Data();
			  string graphName = "TsCounterHit_"+SpadicName;
			  fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,nSpadicMessagesHit[NrOfSpadics*syscore+spadic]);
 			  graphName = "TsCounterHitAborted_"+SpadicName;
			  fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,nSpadicMessagesHitAborted[NrOfSpadics*syscore+spadic]);
			  graphName = "TsCounterOverflow_"+SpadicName;
			  fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,nSpadicMessagesOverflow[NrOfSpadics*syscore+spadic]);
			  graphName = "TsLost_"+SpadicName;
			  if(nSpadicMessagesOverflow[NrOfSpadics*syscore+spadic] > 0) fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,(Double_t(nSpadicMessagesLost[NrOfSpadics*syscore+spadic])/Double_t(nSpadicMessagesOverflow[NrOfSpadics*syscore+spadic])));
			  else fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,0);
			  graphName = "TsCounterInfo_"+SpadicName;
			  fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,nSpadicMessagesInfo[NrOfSpadics*syscore+spadic]);
			  graphName = "TsCounterEpoch_"+SpadicName;
			  fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,nSpadicMessagesEpoch[NrOfSpadics*syscore+spadic]);
			  graphName = "TsCounterStrange_"+SpadicName;
			  fHM->G1(graphName)->SetPoint(fHM->G1(graphName)->GetN(),fNrTimeSlices+1,nSpadicMessagesStrange[NrOfSpadics*syscore+spadic]);
			  if(nSpadicMessages[NrOfSpadics*syscore+spadic] > 0) fHM->G1("TsStrangeness_"+SpadicName)->SetPoint(fHM->G1("TsStrangeness_"+SpadicName)->GetN(),fNrTimeSlices+1,(Double_t(nSpadicMessagesStrange[NrOfSpadics*syscore+spadic])/Double_t(nSpadicMessages[NrOfSpadics*syscore+spadic])));
			  else fHM->G1("TsStrangeness_"+SpadicName)->SetPoint(fHM->G1("TsStrangeness_"+SpadicName)->GetN(),fNrTimeSlices+1,0);
		  }
	  }
  }
  // Catch empty TimeSlices.
  if(fNrTimeSlices==0){
    if(fHM->G1("TsCounter")->GetN()==0){
      LOG(info) << "Expected entries in TsCounter after finishinig first TimeSlice, but found none.";
    }
  }
  fNrTimeSlices++;

  if(fNrTimeSlices % 1 ==0)
    {
      if (fActivateClusterizer){
	if (fNrTimeSlices!=0)ClusterizerTime();
	//ClusterizerSpace();
      }
      CleanUpBuffers();
    }
  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
		  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
		  histName = "Baseline_for_"+spadicName;
		  TString TemphistName = "Temp_"+histName;
		  fHM->H2(histName.Data())->Add(fHM->H2(TemphistName.Data()));
		  TString Graph = "Baselinetrend_"+spadicName;
		  fHM->G1(Graph.Data())->SetPoint(fHM->G1(Graph.Data())->GetN(),fNrTimeSlices+1,fHM->H2(TemphistName.Data())->GetMean(1));
		  fHM->H2(TemphistName.Data())->Reset();
		  if(fHM->H2(TemphistName.Data())->GetEntries()!=0) LOG(fatal)<<"Reset Failed for Temp_"+histName;
	  }
  }
  for(int i=1;i<=12;i++) {
      fBaselineMon->cd(i)->Modified();
  }
  fBaselineMon->Update();

}
// ---- Finish  -------------------------------------------------------
void CbmTrdTimeCorrel::Finish()
{
  ClusterizerTime();
  fRawMessages->Clear();
  fDigis->Clear();
  fClusters->Clear();
  FitBaseline();
  //FitPRF();
  TString path = FairRootManager::Instance()->GetOutFile()->GetName();
  TString runName= path(TRegexp("[0-9]+_"));
  if(fRun!=0) runName=Form(" (Run %d)",fRun);

  fHM->H2("SourceAvsEquipIDall")->GetXaxis()->SetTitle("eqID");
  fHM->H2("SourceAvsEquipIDall")->GetYaxis()->SetTitle("sourceA");
  fHM->H2("SourceAvsEquipIDepoch")->GetXaxis()->SetTitle("eqID");
  fHM->H2("SourceAvsEquipIDepoch")->GetYaxis()->SetTitle("sourceA");
  fHM->H2("SourceAvsEquipIDhit")->GetXaxis()->SetTitle("eqID");
  fHM->H2("SourceAvsEquipIDhit")->GetYaxis()->SetTitle("sourceA/padID");

  
  
  //Perform uniform relabeling of Axis
  ReLabelAxis(fHM->H1("InfoType_vs_Channel")->GetYaxis(),"infoType",true,true);

  // use this to produce nice single plots
  /*
    TCanvas *cnice = new TCanvas("cnice","cnice",800,400); 
    cnice->cd();
    fHM->G1("TsCounterHit1")->Draw("AL");
    fHM->G1("TsCounterHit1")->SetLineColor(kBlack);
    fHM->G1("TsCounterHit1")->GetXaxis()->SetTitle("timeslice");
    fHM->G1("TsCounterHit1")->GetXaxis()->SetRangeUser(0,2166);
    fHM->G1("TsCounterHit1")->GetYaxis()->SetTitle("SPADIC1 hit messages");
  */
  //Buffer (map) or multi SPADIC data streams based analyis have to be done here!!

  /*
  std::vector<TH2*> TH2vector = fHM->H2Vector(".*");
  for (std::vector<TH2*>::iterator it = TH2vector.begin() ; it != TH2vector.end(); ++it){
    LOG(info) << (*it)->GetTitle();
    (*it)->SetContour(99);
    }

	std::vector<TCanvas*> c;
	for (Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		for (Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			string spadicName = GetSpadicName(syscore, spadic, kDirectOutput,
					kFullSpadic).Data();
			c.push_back(
					new TCanvas(TString("Canvas_Signalshapes_for_" + spadicName).Data(),
							TString("Signalshapes_for_" + spadicName).Data(), 1920, 1080));
			c.back()->Divide(8, 4);
			if (spadicName != "") {
				for (Int_t Channel = 0; Channel < 32; Channel++) {
					c.back()->cd(Channel + 1)->SetLogz();
					string HistName = "Signalshape_for_" + spadicName
							+ "Channel" + std::to_string(Channel);
					fHM->H2(HistName)->Draw("colz2");
				}
			}

		}
	}
	TDirectory *Canvasdir = FairRootManager::Instance ()->GetOutFile ()->mkdir (
	    "Signalshapes");
	Canvasdir->cd ();
	for (auto C : c)
	  {
	    C->Write ();
	  }
	FairRootManager::Instance ()->GetOutFile ()->cd ();
	Canvasdir = FairRootManager::Instance ()->GetOutFile ()->mkdir ("Baselines");
	Canvasdir->cd ();
	fBaselineMon->Write ();
	FairRootManager::Instance ()->GetOutFile ()->cd ();
  */
{	  vector<TH2*> HitFrequencies = fHM->H2Vector("Clusterfrequency.*");
	for (auto h : HitFrequencies){
		h->Sumw2();
		TH1* P= h->ProjectionY();
		P->Scale(1.0/P->GetEntries(),"width");
		TString HistName = TString(h->GetTitle())+"_Spectrum";
		P->SetNameTitle(HistName.Data(),HistName.Data());
		fHM->Add(HistName.Data(),P);
		h->GetXaxis()->SetNameTitle("Time t/s","Time t/s");
		h->GetYaxis()->SetNameTitle("Clusterfrequency f/Hz","Clusterfrequency f/Hz");
	}
}
{	  vector<TGraph*> HitFrequencies = fHM->G1Vector(".*frequency.*");
	for (auto h : HitFrequencies){
		Int_t NrEntries=h->GetN();
		auto Y=h->GetY();
		for(Int_t i = 0; i<NrEntries;i++){
			if(TMath::IsNaN(Y[i])||Y[i]==TMath::Infinity()){
				h->RemovePoint(i);
				i=0;
				NrEntries=h->GetN();
			}
		}
		h->GetXaxis()->SetNameTitle("Time t/s","Time t/s");
		h->GetYaxis()->SetNameTitle("Hitfrequency f/Hz","Hitfrequency f/Hz");
	}
}
  if(fBatchAssessment){
	  vector<TH2*> PadResponses = fHM->H2Vector("Pad_Response.*");
	  vector<TH2*> Heatmap = fHM->H2Vector(".*Heatmap.*");
	  vector<TH2*> Baselines = fHM->H2Vector(".*Baseline.*");
	  TCanvas *c6 = new TCanvas("BatchAssesment",""+runName,1600,900);
	  path = FairRootManager::Instance()->GetOutFile()->GetName();
	  TString RunNumber = path(TRegexp("[0-9]+_"));
	  path = "/home/p_munk01/Pictures/BatchAssesment/";
	  for (auto h : PadResponses){
		  TString Title = h->GetTitle();
		  if(!(Title.Contains("_3")||Title.Contains("_4"))) continue;
		  c6->Clear();
		  c6->SetLogz();
		  h->SetContour(99);
		  h->Draw("colz");
		  c6->Update();
		  c6->SaveAs(path+RunNumber+Title+".png");
	  }
	  for (auto h : Heatmap){
		  TString Title = h->GetTitle();
		  if(!(Title.Contains("Cluster(3")||Title.Contains("Cluster(4"))) continue;
		  c6->Clear();
		  c6->SetLogz();
		  h->SetContour(99);
		  h->Draw("colz");
		  c6->Update();
		  c6->SaveAs(path+RunNumber+h->GetTitle()+".png");
	  }
	  for (auto h : Baselines){
		  TString Title = h->GetTitle();
		  c6->Clear();
		  c6->SetLogz();
		  h->SetContour(99);
		  h->Draw("colz");
		  c6->Update();
		  c6->SaveAs(path+RunNumber+h->GetTitle()+".png");
	  }
  }

  LOG(debug) << "Finish of CbmTrdTimeCorrel";
  LOG(info) << "Write histo list to " << FairRootManager::Instance()->GetOutFile()->GetName();
  FairRootManager::Instance()->GetOutFile()->cd();
  fHM->WriteToFile();
  //if(false)
  //delete c1;
  TString outfile = FairRootManager::Instance()->GetOutFile()->GetName();
      outfile.ReplaceAll(".root", "_Calibration.root");
    LOG(info) << "Baselines extracted and stored at "<< outfile << " If this is your first time running this analysis, please rerun this analysis.";
}
// ---- FinishEvent  -------------------------------------------------------
void CbmTrdTimeCorrel::FinishEvent()
{
  LOG(debug) << "FinishEvent of CbmTrdTimeCorrel";
  
  LOG(info) << "Digis in TClonesArray:                           " << fDigis->GetEntriesFast();
  LOG(info) << "Clusters in TClonesArray:                        " << fClusters->GetEntriesFast();
  fClusters->Delete();
  fClusters = new TClonesArray("CbmTrdCluster");
  fDigis->Delete();
  fDigis = new TClonesArray("CbmTrdDigi");  
}
// -------------------------------------------------------------------------
void CbmTrdTimeCorrel::ClusterizerTime()
{
  static Int_t Clusterrun =0;
  auto CompareSpadicMessages=
      [&](CbmSpadicRawMessage* a,CbmSpadicRawMessage* b)
	  {
	  	  Int_t SyscoreIDA = GetSyscoreID(a->GetEquipmentID());
	  	  Int_t SyscoreIDB = GetSyscoreID(b->GetEquipmentID());
		  Int_t SpadicIDA = GetSpadicID(a->GetSourceAddress());
		  Int_t SpadicIDB = GetSpadicID(b->GetSourceAddress());
		  Int_t ChIDA = a->GetChannelID() + ((SpadicIDA %2 == 1)? 16 : 0);
		  Int_t ChIDB = b->GetChannelID() + ((SpadicIDB %2 == 1)? 16 : 0);
		  Int_t PadA = GetChannelOnPadPlane(ChIDA);
		  Int_t PadB = GetChannelOnPadPlane(ChIDB);
		  Int_t rowA = (SpadicIDA/2)*2+PadA/16;
		  Int_t rowB = (SpadicIDB/2)*2+PadB/16;
		  if(a->GetFullTime() == b->GetFullTime())
			  if(SyscoreIDA==SyscoreIDB)
				  if (SpadicIDA == SpadicIDB)
					  if(rowA==rowB)
						  if(PadA==PadB)
							  return true;
		  return false;
      };
  auto CompareSpadicMessagesSmaller=
      [&](CbmSpadicRawMessage* a,CbmSpadicRawMessage* b)
      {
  	    Int_t SyscoreIDA = GetSyscoreID(a->GetEquipmentID());
  	  	Int_t SyscoreIDB = GetSyscoreID(b->GetEquipmentID());
		Int_t SpadicIDA = GetSpadicID(a->GetSourceAddress());
		Int_t SpadicIDB = GetSpadicID(b->GetSourceAddress());
        Int_t ChIDA = a->GetChannelID() + ((SpadicIDA %2 == 1)? 16 : 0);
        Int_t ChIDB = b->GetChannelID() + ((SpadicIDB %2 == 1)? 16 : 0);
        Int_t PadA = GetChannelOnPadPlane(ChIDA);
        Int_t PadB = GetChannelOnPadPlane(ChIDB);
        Int_t rowA = (SpadicIDA/2)*4+PadA/16;
        Int_t rowB = (SpadicIDB/2)*4+PadB/16;
        if(a->GetFullTime() < b->GetFullTime())
        	if(SyscoreIDA<SyscoreIDB){
        		if(rowA<rowB)
        			return true;
        		else if(rowA == rowB)
        			if(PadA<PadB)
        				return true;
        	}
        return false;
      };
  /*
  auto TimeStampSort=
      [&](CbmSpadicRawMessage* a,CbmSpadicRawMessage* b)
      {
		if(a->GetFullTime() < b->GetFullTime())
			return true;
		return false;
      };
*/
    const Int_t clusterWindow = 0; // size of time window in which two hits are called "correlated", unit is timestamps
  std::sort(fLinearHitBuffer.begin(),fLinearHitBuffer.end(),CompareSpadicMessagesSmaller);
  std::unique(fLinearHitBuffer.begin(),fLinearHitBuffer.end(),CompareSpadicMessages);
  std::multimap<ULong_t, CbmSpadicRawMessage*> TempHitMap,TempOverflowMap;
  for (auto x : fLinearHitBuffer){
      long unsigned int MessageTime = static_cast <long unsigned int>(x->GetFullTime());
      (x->GetOverFlow()) ? TempOverflowMap.insert(std::make_pair(MessageTime,x)): TempHitMap.insert(std::make_pair(MessageTime,x));
  }
  UInt_t LastTimestamp = 0;
  for (auto it=TempHitMap.begin(); it != TempHitMap.end(); ++it){
      auto range = std::make_pair(TempHitMap.lower_bound(it->first), TempHitMap.upper_bound(it->first + clusterWindow));
      auto rangeCopy=range;
      if(TempHitMap.size()==0 || (range.first == TempHitMap.end())) continue;
      for (;range.first != range.second; ++(range.first)){
	  if(it->second == nullptr || range.first->second == nullptr) continue;
	    if(range.first->second->GetTriggerType() == 1 || range.first->second->GetTriggerType() ==3) { // exclude purely neighbour triggered hits
		if(it->second->GetTriggerType() == 1 || it->second->GetTriggerType() == 3) { // exclude purely neighbour triggered hits for the comparator side too
		    Int_t ChID1 = it->second->GetChannelID();
		    Int_t ChID2 = range.first->second->GetChannelID();
		    Int_t SpaID1 = GetSpadicID(it->second->GetSourceAddress());
		    Int_t SpaID2 = GetSpadicID(range.first->second->GetSourceAddress());
		    Int_t SysID1 = GetSyscoreID(it->second->GetEquipmentID());
		    Int_t SysID2 = GetSyscoreID(range.first->second->GetEquipmentID());
		    ChID1 += (SpaID1 %2 == 1)? 16 : 0; // Remap the channel IDs of each second Half-Spadic to 16...31
		    ChID2 += (SpaID2 %2 == 1)? 16 : 0;
		    // special for SPS2015 data: the chamber with SpaID 2 and 3 was turned by 180 degree with respect to the first one. thus, turn the pad plane number here
		    //Int_t SpaPad1 = (Int_t)(SpaID1/2) * 32 + ((SpaID1>1) ? 31-GetChannelOnPadPlane(ChID1) : GetChannelOnPadPlane(ChID1));
		    //Int_t SpaPad2 = (Int_t)(SpaID2/2) * 32 + ((SpaID2>1) ? 31-GetChannelOnPadPlane(ChID2) : GetChannelOnPadPlane(ChID2));
		    Int_t SpaPad1 = (Int_t)(SpaID1/2) * 32 + (GetChannelOnPadPlane(ChID1));
		    Int_t SpaPad2 = (Int_t)(SpaID2/2) * 32 + (GetChannelOnPadPlane(ChID2));
		    if (it!=range.first) { //Exclude Correlations of messages with themselves
			//TODO:Implement Clusters and fill them
			    std::pair<TString,TString> Corrmap=std::make_pair(GetSpadicName(SysID1, SpaID1, kProcessedData, kFullSpadic),GetSpadicName(SysID2, SpaID2, kProcessedData, kFullSpadic));
			    std::cout<< Corrmap.first << " " << Corrmap.second << std::endl;
			    if(std::count(fCorrmaps.begin(),fCorrmaps.end(),Corrmap)!=0){
			  	  TString histName="Correlation_Map_"+Corrmap.first+"_"+Corrmap.second;
			    	fHM->H2(histName.Data())->Fill(GetChannelOnPadPlane(ChID1)-GetChannelOnPadPlane(ChID2),static_cast<Long_t>(range.first->second->GetFullTime())-static_cast<Long_t>(it->second->GetFullTime()));
			    }
			if(fDebugMode){
			    fHM->H2("Hit_Coincidences")->Fill(SpaPad1,SpaPad2);
			    // Fill for map of correlations following: require origin from variant TRD chambers to cut on physical correlations between two chambers, which is adressed for the moment just by requiring different SpaID/2
			    // Furthermore, physical correlations coming from the target require a positve direction from Chamber 1 to Chamber 2, which means ascending SpaIDs
			    fHM->H2("Correlation_Map")->Fill(SpaPad2-SpaPad1-32,(static_cast<Long_t>(range.first->second->GetFullTime())-static_cast<Long_t>(it->second->GetFullTime())));
			}else{
			    if (((Int_t)(SpaID1/2) - (Int_t)(SpaID2/2)) != 0) fHM->H2("Hit_Coincidences")->Fill(SpaPad1,SpaPad2);
			    // Fill for map of correlations following: require origin from variant TRD chambers to cut on physical correlations between two chambers, which is adressed for the moment just by requiring different SpaID/2
			    // Furthermore, physical correlations coming from the target require a positve direction from Chamber 1 to Chamber 2, which means ascending SpaIDs
			    if (((Int_t)(SpaID2/2) - (Int_t)(SpaID1/2)) > 0) fHM->H2("Correlation_Map")->Fill(SpaPad2-SpaPad1-32,(static_cast<Long_t>(range.first->second->GetFullTime())-static_cast<Long_t>(it->second->GetFullTime())));
			}
		    }
		}
	    }
      }
      if(LastTimestamp>=it->first-clusterWindow) continue;
      else LastTimestamp = it->first;
/*
 * Important Notice:
 * If fActivate2DClusterizer is set to true,
 * Messages are sorted byChIDs instead of PadID's,
 * also turns of row detection.
 */
      auto SortSpadicMessageRange =
    		  [&](CbmSpadicRawMessage a,CbmSpadicRawMessage b)
			  {	if (fActivate2DClusterizer) {
				  Int_t ChIDA = a.GetChannelID() + ((GetSpadicID(a.GetSourceAddress()) %2 == 1)? 16 : 0);
				  Int_t ChIDB = b.GetChannelID() + ((GetSpadicID(b.GetSourceAddress()) %2 == 1)? 16 : 0);
				  Int_t SpaIDA = (GetSpadicID(a.GetSourceAddress())/2);
				  Int_t SpaIDB = (GetSpadicID(b.GetSourceAddress())/2);
				  if(SpaIDA<SpaIDB)
					  return true;
				  else if(SpaIDA == SpaIDB)
					  if(ChIDA<ChIDB)
						  return true;
				  return false;
			  } else {
				  Int_t ChIDA = a.GetChannelID() + ((GetSpadicID(a.GetSourceAddress()) %2 == 1)? 16 : 0);
				  Int_t ChIDB = b.GetChannelID() + ((GetSpadicID(b.GetSourceAddress()) %2 == 1)? 16 : 0);
				  Int_t PadA = GetChannelOnPadPlane(ChIDA);
				  Int_t PadB = GetChannelOnPadPlane(ChIDB);
				  Int_t rowA = (GetSpadicID(a.GetSourceAddress())/2)*2+PadA/16;
				  Int_t rowB = (GetSpadicID(b.GetSourceAddress())/2)*2+PadB/16;
				  if(rowA<rowB)
					  return true;
				  else if(rowA == rowB)
					  if(PadA<PadB)
						  return true;
				  return false;
			  }
          };
      Bool_t OutsideCluster = true;
      std::vector<CbmSpadicRawMessage> linearClusterBuffer;
      std::vector<Int_t> HitChs,HitSpadic;
      for (; rangeCopy.first != rangeCopy.second; ++(rangeCopy.first)) //Copy Messages from the previously selected range for easier access
        {
          linearClusterBuffer.push_back (*rangeCopy.first->second);
          Int_t Trigger = (*rangeCopy.first->second).GetTriggerType();
          if(Trigger==1||Trigger==3) {
        	  HitChs.push_back((*rangeCopy.first->second).GetChannelID()+(GetSpadicID((*rangeCopy.first->second).GetSourceAddress())%2)*16);
        	  HitSpadic.push_back(GetSpadicID((*rangeCopy.first->second).GetSourceAddress()));
          }
		}
		for (UInt_t i = 0; i < HitChs.size(); i++) {
			for (auto x : linearClusterBuffer) {
				Int_t SpadicID = GetSpadicID(x.GetSourceAddress());
				Int_t Channel = x.GetChannelID()
						+ (GetSpadicID(x.GetSourceAddress()) % 2) * 16;
				if ((HitSpadic.at(i) / 2) == (SpadicID / 2)) {
					Int_t Trigger = x.GetTriggerType();
					if (std::abs(Channel - HitChs.at(i)) < 3)
						if (Trigger == 2) {
							string histname =
									"Trigger_Bug_Investigation_"
											+ string(GetSpadicName(x.GetEquipmentID(),x.GetSourceAddress(),kRawData,kFullSpadic).Data());
							fHM->H2(histname)->Fill(HitChs.at(i), Channel);
						}
				}
			}
		}
      Clusterrun++;
      std::sort (linearClusterBuffer.begin (), linearClusterBuffer.end (),SortSpadicMessageRange);//Sort the Hitmessages by their position of origin on the padplane
      Cluster BuildingCluster (&fBaseline,clusterWindow,fClusterThreshhold); //Create a new Cluster
      Int_t lastRow = (GetSpadicID(linearClusterBuffer.begin()->GetSourceAddress())/2)*32+GetChannelOnPadPlane(linearClusterBuffer.begin()->GetChannelID() + ((GetSpadicID(linearClusterBuffer.begin()->GetSourceAddress()) %2 == 1)? 16 : 0))/16;
      Int_t lastPad = GetChannelOnPadPlane(linearClusterBuffer.begin()->GetChannelID() + ((GetSpadicID(linearClusterBuffer.begin()->GetSourceAddress()) %2 == 1)? 16 : 0));
      auto SatteliteDebug =
    		  [&](Cluster currentCluster){
    	  if(/*currentCluster.size() != 2&&*/!currentCluster.Get2DStatus()) return 0;
    	  return 0;
    	  Int_t Detector = currentCluster.GetSpadic()/2;
          string Detectorname = (Detector == 0 ? "Frankfurt" : "Muenster");
    		  //std::cout << "Fragmented Cluster Found on Pad: " <<currentCluster.GetHorizontalPosition() << std::endl;
    		  //std::cout << "Satelites on ChID|SpaID|Timestamp|HitType|StopType" << std::endl;
    		  CbmSpadicRawMessage MainMassage;
    		  auto Messages= currentCluster.GetEntries();
    		  for (auto tempMessage=Messages.first;tempMessage!= Messages.second;tempMessage++){
    			  if (tempMessage->GetTriggerType()!=2) {
    				  MainMassage = *tempMessage;
    				  break;
    			  }
    		  }
    		  for (auto x: linearClusterBuffer ){
    		  std::cout << x.GetChannelID() << " " << GetSpadicID(x.GetSourceAddress()) <<" " << x.GetFullTime() << " " <<x.GetTriggerType() << " " << x.GetStopType() << std::endl;

    	  }
    		  std::cout << "All Satellites printed " << std::endl;
    		  std::cout << "________________________________________________________"<< std::endl;
    	  return 0;
      };
      for (auto currentMessage = linearClusterBuffer.begin(); currentMessage != linearClusterBuffer.end(); currentMessage++) //Loop over all Messages inside the Hitwindow
        {
    	  Int_t currentPad = GetChannelOnPadPlane(currentMessage->GetChannelID() + ((GetSpadicID(currentMessage->GetSourceAddress()) %2 == 1)? 16 : 0));
    	  if(BuildingCluster.size()>16) std::cout << "Cluster of Size " << BuildingCluster.size() << " with current Pad " << currentPad << " " << lastPad <<std::endl;
          if (currentPad-lastPad >1 ){
        	  if(!fActivate2DClusterizer) OutsideCluster=true;
        	  else if(currentPad-lastPad >3)OutsideCluster=true;
        	  //std::cout << "Gap in Cluster found " << std::endl;
          }
          Int_t currentRow = (GetSpadicID(currentMessage->GetSourceAddress())/2)*32+currentPad/16;
          if((!fActivate2DClusterizer&&(BuildingCluster.size () > currentPad || lastRow != currentRow||lastPad >= currentPad)))//Check if the current message is not from the same row/spadic
        	  {
              if (BuildingCluster.size () != 0)//Store away nonempty Cluster
                {
            	  SatteliteDebug(BuildingCluster);
                  fClusterBuffer.push_back (BuildingCluster);
                  Cluster tempCluster (&fBaseline,clusterWindow,fClusterThreshhold);
                  BuildingCluster = tempCluster;
                }
              //std::cout << " Rowchange " << lastRow << " " << currentRow << " " << std::endl;
              OutsideCluster = true; //New Row, so we start outside any Cluster
              lastRow = currentRow;
            } else if(fActivate2DClusterizer){
            	if(currentRow%32==lastRow%32){
            		lastRow=currentRow;
            	}
            	else {
            		if (BuildingCluster.size () != 0)//Store away nonempty Cluster
            				{
            			SatteliteDebug(BuildingCluster);
            			fClusterBuffer.push_back (BuildingCluster);
            			Cluster tempCluster (&fBaseline,clusterWindow,fClusterThreshhold);
            			BuildingCluster = tempCluster;
            				}
            		//std::cout << " Rowchange " << lastRow << " " << currentRow << " " << std::endl;
            		OutsideCluster = true; //New Row, so we start outside any Cluster
            		lastRow = currentRow;
            	}
            }

          if (currentMessage->GetTriggerType () == 2)//Check for neighbour triggered Messages
            {
              if (OutsideCluster)
                {//Beginning of a Cluster
                  if (BuildingCluster.size () != 0) //Store away the previous Cluster, if not already done
                    {
                	  SatteliteDebug(BuildingCluster);
                      fClusterBuffer.push_back (BuildingCluster);
                      Cluster tempCluster (&fBaseline,clusterWindow,fClusterThreshhold);
                      BuildingCluster = tempCluster;
                    }
                  BuildingCluster.AddEntry (*currentMessage);
                  OutsideCluster = false;
                }
              else
                {//Test if currentMessage is Part of next Cluster
            	  /*
                  auto nextMessage = currentMessage;
                  if(currentMessage!=linearClusterBuffer.end()) nextMessage++;
                  if (currentMessage != linearClusterBuffer.begin())
                    if(nextMessage != linearClusterBuffer.end())
                      if(nextMessage->GetTriggerType () == 1||nextMessage->GetTriggerType () == 3)
                        {
                          if(GetChannelOnPadPlane(nextMessage->GetChannelID() + ((GetSpadicID(nextMessage->GetSourceAddress()) %2 == 1)? 16 : 0))-
                              currentPad >1)
                            {
                        	  std::cout << " Black Magic Code > 2 has triggered "<< std::endl;

                              BuildingCluster.AddEntry (*currentMessage);
                              if (BuildingCluster.size () != 0) //Store away the completed Cluster
                                {
                            	  SatteliteDebug(BuildingCluster);
                                  fClusterBuffer.push_back (BuildingCluster);
                                  Cluster tempCluster (fBaseline,clusterWindow,fClusterThreshhold);
                                  BuildingCluster = tempCluster;
                                }
                            }
                          else if(GetChannelOnPadPlane(nextMessage->GetChannelID() + ((GetSpadicID(nextMessage->GetSourceAddress()) %2 == 1)? 16 : 0))-
                              currentPad <=1)
                            {
                        	  std::cout << " Black Magic Code <=1 has triggered "<< std::endl;
                              if (BuildingCluster.size () != 0) //Store away the completed Cluster
                                {
                            	  SatteliteDebug(BuildingCluster);
                                  fClusterBuffer.push_back (BuildingCluster);
                                  Cluster tempCluster (fBaseline,clusterWindow,fClusterThreshhold);
                                  BuildingCluster = tempCluster;
                                }
                              currentMessage--;
                            }
                          OutsideCluster = true;
                          std::cout << "EVIL GOTO" << std::endl;
                          goto SchleifenendeClusterizer;
                        }*/
                  //End of a Cluster, marked by second neighbour triggered message
                  BuildingCluster.AddEntry (*currentMessage);
                  OutsideCluster = true;
                  if (BuildingCluster.size () != 0) //Store away the completed Cluster
                    {
                	  SatteliteDebug(BuildingCluster);
                      fClusterBuffer.push_back (BuildingCluster);
                      Cluster tempCluster (&fBaseline,clusterWindow,fClusterThreshhold);
                      BuildingCluster = tempCluster;
                    }
                }
            }
          else if ((currentMessage->GetTriggerType () == 3 || currentMessage->GetTriggerType () == 1)){
              if (OutsideCluster)
                {//incomplete Cluster without a starting neighbour triggered Hitmessage
                  if (BuildingCluster.size () != 0)//Should not occur, but stores away previous Cluster
                    {
                	  SatteliteDebug(BuildingCluster);
                      fClusterBuffer.push_back (BuildingCluster);
                      Cluster tempCluster (&fBaseline,clusterWindow,fClusterThreshhold);
                      BuildingCluster = tempCluster;
                    }//Start the Cluster
                  BuildingCluster.AddEntry (*currentMessage);
                  OutsideCluster = false;//we are now inside a cluster
                }
              else //Fill the Cluster
                {
                  BuildingCluster.AddEntry (*currentMessage);
                }
          }
	  //          SchleifenendeClusterizer:
          lastPad = currentPad;
        }
      //std::cout << " CLusterizer loop finished" << std::endl;
      if (BuildingCluster.size () != 0) //Store away the last Cluster
        {
    	  SatteliteDebug(BuildingCluster);
          fClusterBuffer.push_back (BuildingCluster);
          Cluster tempCluster (&fBaseline,clusterWindow,120);
          BuildingCluster = tempCluster;
        }

      //std::sort(rangeCopy.first,rangeCopy.second,CompareSpadicMessagesSmaller);
  }
  if(fClusterBuffer.size()>0)
  {
	  TH2* CoincidenceHistogram = fHM->H2("Cluster_Coincidences");
	  for (unsigned int i =0; i< fClusterBuffer.size()-1;i++){
		  Float_t CurrentPosition=fClusterBuffer.at(i).GetHorizontalPosition();
		  if (fClusterBuffer.at(i).size()<3 ||fClusterBuffer.at(i).size()>4) continue;
		  Int_t CurrentDetector = fClusterBuffer.at(i).GetSpadic()/2;
		  //if(CurrentPosition>16.0) CurrentPosition -= 16.0;
		  Long_t CurrentTime = fClusterBuffer.at(i).GetFulltime();
		  for(unsigned int j=1;j<10&&i+j<fClusterBuffer.size();j++){
			  if(fClusterBuffer.at(i+j).GetSpadic()/2==CurrentDetector) continue;
			  if (fClusterBuffer.at(i+j).size()<3 ||fClusterBuffer.at(i+j).size()>4) continue;
			  Float_t NextPosition=fClusterBuffer.at(i+j).GetHorizontalPosition();
			  //if(NextPosition>16.0) NextPosition -= 16.0;
			  Long_t NextTime = fClusterBuffer.at(i+j).GetFulltime();
			  CoincidenceHistogram->Fill(NextPosition - CurrentPosition,NextTime -CurrentTime);
		  }
	  }
  }
  static Long64_t lastClusterTimestamp[NrOfActiveSyscores*NrOfSpadics];
  if (fNrTimeSlices==1){
	for(Int_t i=0;i<NrOfActiveSyscores*NrOfSpadics;i++){
		lastClusterTimestamp[i]=0;
	}
  }

  for (auto x: fClusterBuffer){
      Float_t Position = x.GetHorizontalPosition();
      if (Position < 0.5 || Position > 30.5 || //exclude Clusters outside Padplane
          (Position > 14.5 && Position < 16.5) || //exclude Clusters between rows on Padplane
          (Position > 6.5 && Position < 8.5) || //exclude Clusters on gap between Halfchip A and B
          (Position > 22.5 && Position < 24.5)|| //exclude Clusters on gap between Halfchip A and B
          (Position > 26.5 && Position < 29.5)) //exclude Clusters around defective channel 6
        {
          fHM->H1 (
              "Masked_Clustersize_for_"
        		  + string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data()))->Fill (
              static_cast<Int_t> (x.size ()));
          continue;
        }
      fHM->H1("Clustersize_for_"+string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data()))->Fill(static_cast<Int_t>(x.size ()));
      fHM->H2("Cluster("+std::to_string(static_cast<Int_t>(x.size()))+")_Heatmap_for_"+string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data()))->Fill((x.GetHorizontalPosition()<16.0 ? x.GetHorizontalPosition() : x.GetHorizontalPosition()-16.0),1-static_cast<Int_t>(x.GetRow()));
      if (fDrawClustertypes){
    	  string histname = "Clustertypes_for_"+string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data());
    	  fHM->H2(histname)->Fill(x.size(),x.Type());
      }
      if(x.Type() > 0 && x.size()>=3 && x.Type() <6){
    	  string histname = "Cut5_Pad_Response_"+ string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data())
    			  + "_for_Clusters_of_Size_"+std::to_string(static_cast<Int_t>(x.size()));
    	  if(x.Type()==5) x.FillChargeDistribution(fHM->H2(histname));
    	  continue;
      }
      if(x.fMaxStopType < 1 && x.size() >=3)
      {
          string histname = "Charge_Spectrum_for_"+string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data());
          fHM->H1(histname)->Fill(x.GetTotalCharge(),1/*,x.GetHorizontalPosition()*/);
          string histname2 = "ChargeIntegral_Spectrum_for_"+string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data());
          fHM->H1(histname2)->Fill(x.GetTotalIntegralCharge(),1/*,x.GetHorizontalPosition()*/);

      }
      //if(x.size()==3&&(x.GetTotalCharge()> 250)) continue;
      if(fDrawPadResponse){
    	  string histname = "Pad_Response_"+ string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data()) + "_for_Clusters_of_Size_"+std::to_string(static_cast<Int_t>(x.size()));
    	  x.FillChargeDistribution(fHM->H2(histname),fHM->H2("Central_"+histname));
      }
        int spaID=x.GetSpadic();
	Long64_t time=x.GetFulltime();
        string HistName = "Clusterfrequency_for_"+string(GetSpadicName(0, x.GetSpadic (), kProcessedData,kFullSpadic).Data());
    	fHM->H2(HistName)->Fill(static_cast<Double_t>(time)/15000000.0,15000000/std::fabs((static_cast<Double_t>(lastClusterTimestamp[(spaID%2)*32])-static_cast<Double_t>(time))));
    	lastClusterTimestamp[(spaID%2)*32]=time;
  }
}
// -------------------------------------------------------------------------
void CbmTrdTimeCorrel::ClusterizerSpace()
{
  LOG(info) <<  "ClusterizerSpace";
  Int_t mapDigiCounter = 0;
  CbmSpadicRawMessage* raw = NULL;
  //CbmTrdCluster* cluster = NULL;
  Int_t  layerId(0), moduleId(0), sectorId(0), rowId(0), columnId(0), clusterSize(0), address(0);
  ULong_t lastClusterTime = 0;
  ULong_t fullTime = 0;
  TString SysSpaID = "";
  std::vector<Int_t> digiIndices;
  for (std::map<TString, std::map<ULong_t, std::map<Int_t, CbmSpadicRawMessage*> > >::iterator SpaSysIt = fMessageBuffer.begin() ; SpaSysIt != fMessageBuffer.end(); SpaSysIt++){
    SysSpaID = SpaSysIt->first;
    for (std::map<ULong_t, std::map<Int_t, CbmSpadicRawMessage*> > ::iterator timeIt = (SpaSysIt->second).begin() ; timeIt != (SpaSysIt->second).end(); timeIt++){
      LOG(debug) <<  "ClusterSize:" << Int_t((timeIt->second).size());
      clusterSize = Int_t((timeIt->second).size());
      if ( SysSpaID != "")
	fHM->H1((SysSpaID + TString("_CluterSize")).Data())->Fill(clusterSize);

      fullTime = timeIt->first;
      lastClusterTime = fullTime;

      Int_t lastCombiID = -1;
      for (std::map<Int_t, CbmSpadicRawMessage*> ::iterator combiIt = (timeIt->second).begin(); combiIt != (timeIt->second).end(); combiIt++){
	mapDigiCounter++;
	if (combiIt->second == NULL) {
	  LOG(debug) <<  "Entrie in fMessageBuffer == NULL. SysSpaID:" << SysSpaID << " fullTime:" << fullTime << " combiID:" << combiIt->first;
	  continue;
	}
	raw = combiIt->second;
	
	layerId = GetLayerID(raw);
	moduleId = GetModuleID(raw);
	sectorId = GetSectorID(raw);
	rowId = GetRowID(raw);
	columnId = GetColumnID(raw);
	LOG(debug) <<  " layerId:" << layerId  << " moduleId:" << moduleId << " sectorId:" << sectorId << " rowId:" << rowId << " columnId:" << columnId;
	// BaseLineCorrection==
	Float_t Baseline = 0.;
	
	if (raw->GetStopType() == 0){
	  /*
	    for (Int_t bin = 1; bin < 4; bin++){
	    Baseline += raw->GetSamples()[raw->GetNrSamples()-bin];
	    }
	    Baseline /= 3.;
	  */
	} else {
	  // Use average baseline estimated for full message length
	}
	const Int_t nSamples = raw->GetNrSamples();
	//Float_t Samples[nSamples] = {0.};
	Float_t* Samples = new Float_t[nSamples];
	for (Int_t iBin = 0; iBin < raw->GetNrSamples(); iBin++){
	  Samples[iBin] = raw->GetSamples()[iBin] - Baseline;
	}
	address = CbmTrdAddress::GetAddress(layerId,moduleId,sectorId,rowId,columnId);
	if (fOutputCloneArrays){
	  new ((*fDigis)[fiDigi]) CbmTrdDigi(address,
					     raw->GetFullTime(),//57,14 ns per timestamp
					     raw->GetTriggerType(), raw->GetInfoType(), raw->GetStopType(),
					     raw->GetNrSamples(), Samples);
	}
	delete[] Samples;
	if (combiIt->first - lastCombiID != 1 && digiIndices.size() > 0){
	  address = CbmTrdAddress::GetAddress(layerId,moduleId,sectorId,rowId,columnId);
	  if (fOutputCloneArrays){
	    //new ((*fClusters)[fiCluster]) CbmTrdCluster(digiIndices,address);
	  }
	  digiIndices.clear();
	  fiCluster++;
	}
	digiIndices.push_back(fiDigi);
	lastCombiID = combiIt->first;
	fiDigi++;
      }
      if (digiIndices.size() > 0){
	address = CbmTrdAddress::GetAddress(layerId,moduleId,sectorId,rowId,columnId);
	if (fOutputCloneArrays){
	  //new ((*fClusters)[fiCluster]) CbmTrdCluster(digiIndices,address);
	}	
	digiIndices.clear();      
	fiCluster++;
      }
    }
  }
  //LOG(info) << "CbmTrdTimeCorrel::ClusterizerSpace Digis:" << mapDigiCounter;
  //CleanUpBuffers();
}
// -------------------------------------------------------------------------
void CbmTrdTimeCorrel::CleanUpBuffers()
{  
  LOG(info) <<  "CleanUpBuffers";
  //LOG(info) <<  "fMessageBuffer.size() " << fMessageBuffer.size();
  for (std::map<TString, std::map<ULong_t, std::map<Int_t, CbmSpadicRawMessage*> > >::iterator SpaSysIt = fMessageBuffer.begin(); SpaSysIt != fMessageBuffer.end(); SpaSysIt++){
    //LOG(info) <<  "  (SpaSysIt->second).size() " << (SpaSysIt->second).size();
    for (std::map<ULong_t, std::map<Int_t, CbmSpadicRawMessage*> >::iterator TimeIt = SpaSysIt->second.begin(); TimeIt != SpaSysIt->second.end(); TimeIt++){
      //LOG(info) <<  "    (TimeIt->second).size() " << (TimeIt->second).size();
      for (std::map<Int_t, CbmSpadicRawMessage*> ::iterator CombiIt = TimeIt->second.begin(); CombiIt != TimeIt->second.end(); CombiIt++){
        //LOG(info) <<  "      (CombiIt->second).size() " << (CombiIt->second).size();
        if(CombiIt->second != NULL){
          delete CombiIt->second;
        }
      }
      TimeIt->second.clear();
    }
    SpaSysIt->second.clear();
  }
  fMessageBuffer.clear();  
  /*for (auto ptr :fLinearHitBuffer)
    delete ptr;*/
  fLinearHitBuffer.clear();
  fClusterBuffer.clear ();
  //fDigis->Clear("C");
  //fClusters->Clear("C");
}
// -------------------------------------------------------------------------
Int_t CbmTrdTimeCorrel::GetMessageType(CbmSpadicRawMessage* raw)
{
  Int_t messageType = -1;

  if (raw->GetEpoch())
    messageType = 0;
  else if (raw->GetEpochOutOfSynch())
    messageType = 1;
  else if (raw->GetHit())
    messageType = 2;
  else if (raw->GetHitAborted())
    messageType = 3;
  else if (raw->GetOverFlow())
    messageType = 5;
  else if (raw->GetInfo())
    messageType = 4;
  else if (raw->GetStrange())
    messageType = 6;

  return messageType;
}
// ----              -------------------------------------------------------
void CbmTrdTimeCorrel::ReLabelAxis(TAxis* axis, TString type, Bool_t underflow, Bool_t overflow)
{
  Int_t startBin(1), stopBin(axis->GetNbins());
  if (underflow){
    axis->SetBinLabel(1,"underflow -1");
    startBin++;
  }
  if (overflow){
    axis->SetBinLabel(axis->GetNbins(),"Type out of array");
    stopBin--;
  }
  TString messageTypes[7] = {"Epoch",
			     "Epoch out of synch",
			     "Hit",
			     "Hit aborted",
			     "Info",
			     "Overflow",
			     "Strange"};
  TString triggerTypes[4] = { "Global trigger",
			      "Self triggered",
			      "Neighbor triggered",
			      "Self and neighbor triggered"};
  TString stopTypes[6] = {"Normal end of message", 
			  "Channel buffer full", 
			  "Ordering FIFO full", 
			  "Multi hit", 
			  "Multi hit and channel buffer full", 
			  "Multi hit and ordering FIFO full"};
  TString infoTypes[7] = {"Channel disabled during message building", 
			  "Next grant timeout", 
			  "Next request timeout", 
			  "New grant but channel empty", 
			  "Corruption in message builder", 
			  "Empty word", 
			  "Epoch out of sync"};
  if (type == "infoType") {
    if (stopBin - startBin == 7){
      for (Int_t iBin = startBin; iBin < stopBin; iBin++)
	axis->SetBinLabel(iBin,infoTypes[iBin-startBin]);
    } else {
      LOG(error) << "  axis could not be labeled with " << type << ". Wrong number of bins in histogram.";
    }
  } else if (type == "triggerType") {
    if (stopBin - startBin == 4){
      for (Int_t iBin = startBin; iBin < stopBin; iBin++)
	axis->SetBinLabel(iBin,triggerTypes[iBin-startBin]); 
    } else {
      LOG(error) << "  axis could not be labeled with " << type << ". Wrong number of bins in histogram.";
    }
  } else if (type == "stopType") {
    if (stopBin - startBin == 6){
      for (Int_t iBin = startBin; iBin < stopBin; iBin++)
	axis->SetBinLabel(iBin,stopTypes[iBin-startBin]);
    } else {
      LOG(error) << "  axis could not be labeled with " << type << ". Wrong number of bins in histogram.";
    }
  } else if (type == "messageType") {
    if (stopBin - startBin == 7){
      for (Int_t iBin = startBin; iBin < stopBin; iBin++)
	axis->SetBinLabel(iBin,messageTypes[iBin-startBin]);
    } else {
      LOG(error) << "  axis could not be labeled with " << type << ". Wrong number of bins in histogram.";
    }
  } else {
    LOG(error) << type << " not known ";
  }

}
// ----              -------------------------------------------------------
void CbmTrdTimeCorrel::CreateHistograms()
{    
  
  TString spadicName = "";
  TString runName="";
  if(fRun!=0) runName=Form(" (Run %d)",fRun);
  TString histName="";
  TString title="";

  /*
  TString channelName[32] = { "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
			      "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", 
			      "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", 
			      "30", "31"};
  */
  TString triggerTypes[4] = { "Global trigger",
			      "Self triggered",
			      "Neighbor triggered",
			      "Self and neighbor triggered"};
  TString stopTypes[6] = {"Normal end of message", 
			  "Channel buffer full", 
			  "Ordering FIFO full", 
			  "Multi hit", 
			  "Multi hit and channel buffer full", 
			  "Multi hit and ordering FIFO full"};
  /*
  TString infoTypes[8] = {"Channel disabled during message building", 
			  "Next grant timeout", 
			  "Next request timeout", 
			  "New grant but channel empty", 
			  "Corruption in message builder", 
			  "Empty word", 
			  "Epoch out of sync", 
			  "infoType out of array"}; //not official type, just to monitor overflows
  */
  
  fSpadics = NrOfActiveSyscores*NrOfActiveSpadics;
  
  
  
  Int_t n = 0;
  
  fHM->Add("Trigger", new TH1F("Trigger", "Trigger", fSpadics,0,fSpadics));
  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
    for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
      spadicName = GetSpadicName(syscore, spadic, kDirectOutput);
      if(spadicName != "") {
        n++;
        fHM->H1("Trigger")->GetXaxis()->SetBinLabel(n,spadicName);
      }
    }
  }

  fHM->Add("MessageCount", new TH1D("MessageCount","MessageCount",fSpadics*8,0,fSpadics*8));
  n = 0;
  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
    for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
      spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
      if(spadicName != "") {
    	  fHM->Add((spadicName + TString("_LostCount_vs_PadID")).Data(), new TH2D((spadicName + TString("_LostCount_vs_PadID")).Data(),(spadicName + TString("_LostCount_vs_PadID")).Data(),257,-0.5,256.5,32,-0.5,31.5));
    	  fHM->H2((spadicName + TString("_LostCount_vs_PadID")).Data())->GetXaxis()->SetTitle("LostMessages");
    	  fHM->H2((spadicName + TString("_LostCount_vs_PadID")).Data())->GetYaxis()->SetTitle("PadID");
    	  fHM->Add((spadicName + TString("_CluterSize")).Data(), new TH1D((spadicName + TString("_ClusterSize")).Data(),(spadicName + TString("_ClusterSize")).Data(),32,-0.5,31.5));
        for(Int_t stopType = 0; stopType < 6; ++stopType) {
          n++;
          fHM->H1("MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_"+stopTypes[stopType]));
        }
        n++;
        fHM->H1("MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_Info or epoch mess"));
        n++;
        fHM->H1("MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_Info or epoch mess n-fold"));
      }
    }
  }

  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
    for(Int_t spadic = 0; spadic < NrOfActiveHalfSpadics; ++spadic) {
      spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kHalfSpadic);
      if(spadicName != "") {
	  histName = spadicName + "_Time_vs_TimeSlice";
	  title = histName + runName + ";TimeSlice;Time";
	  fHM->Add(histName.Data(), new TH2F(histName, title, 2001, -0.5, 2000.5, 9000, 0, 90000000000));
      }
    }
  }

  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveHalfSpadics; ++spadic) {
	      spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kHalfSpadic);
		  if(spadicName != "") {
			  histName = spadicName + "_Epoch_vs_TimeSlice";
			  title = histName + runName + ";TimeSlice;Epoch";
			  fHM->Add(histName.Data(), new TH2F(histName, title, 2001, -0.5, 2000.5, 4097, -0.5, 4096.5));
		  }
	  }
  }

  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveHalfSpadics; ++spadic) {
	      spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kHalfSpadic);
		  if(spadicName != "") {
			  histName = "Hitrate_vs_Delta_Epoch_hist_for_"+spadicName;
			  title = histName + runName + ";DeltaEpoch;HitMessages per TimeSlice";
			  fHM->Add(histName.Data(), new TH2F(histName, title, 8301, -4150.5, 4150.5, 1000, 0, 10E5));
		  }
	  }
  }

  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveHalfSpadics; ++spadic) {
	      spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kHalfSpadic);
		  histName = "EpochTrend_postregress_hist_for_"+spadicName;
		  title = histName + runName;
		  fHM->Add(histName.Data(), new TH2I(histName, title, 150,0.5,150.5,201,-50.5,150.5));
	  }
  }

  if(true){
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
			  histName = "Trigger_Bug_Investigation_"+spadicName;
			  title = histName + runName;
			  fHM->Create2<TH2I>(histName.Data(), title.Data(), 32,-0.5,31.5, 32,-0.5,31.5);
			  fHM->H2(histName.Data())->GetXaxis()->SetNameTitle("Ch Self triggered","Ch Self triggered");
			  fHM->H2(histName.Data())->GetYaxis()->SetNameTitle("Ch Neighbour triggered","Ch Neighbour triggered");
		  }
	  }
  }


  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveHalfSpadics; ++spadic) {
	      spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kHalfSpadic);
		  if(spadicName != "") {
			  histName = "Delta_Epoch_hist_for_"+spadicName;
			  title = histName + runName;
			  fHM->Add(histName.Data(), new TH1I(histName, title, 8301,-4150.5,4150.5));
		  }
	  }
  }
  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveHalfSpadics; ++spadic) {
	      spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kHalfSpadic);
		  if(spadicName != "") {
			  histName = "DeltaEpoch_vs_timeslice_for_"+spadicName;
			  title = histName + runName;
			  fHM->Add(histName.Data(), new TGraph());
			  fHM->G1(histName.Data())->SetNameTitle(title,title);
		  }
	  }
  }
  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
		  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
          if(spadicName != "") {
              histName = "Map_of_Hits_for_"+spadicName;
              title = histName + runName;
              fHM->Create2<TH2I>(histName.Data(), title.Data(), 16,-0.5,15.5,2,-0.5,1.5);
              histName = "Map_of_Charge_for_"+spadicName;
              title = histName + runName;
			  fHM->Create2<TH2I>(histName.Data(), title.Data(), 16,-0.5,15.5,2,-0.5,1.5);
          }
      }
  }

  // On the x axis, there is a typical eqID hardcoded so far. 57344 is equal 0xE000.
  fHM->Add("SourceAvsEquipIDall", new TH2I("SourceAvsEquipIDall", "SourceAddress vs Equipment ID ALL MESS", 20,57334.5,57354.5,6,-0.5,5.5));
  fHM->Add("SourceAvsEquipIDepoch", new TH2I("SourceAvsEquipIDepoch", "SourceAddress vs Equipment ID EPOCH MESS", 20,57334.5,57354.5,6,-0.5,5.5));
  fHM->Add("SourceAvsEquipIDhit", new TH2F("SourceAvsEquipIDhit", "SourceAddress vs Equipment ID HIT MESS", 20,57334.5,57354.5,96,-0.5,5.5));

  if(fActivateClusterizer)
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
			  if(spadicName != "") {
				  histName = "Clustersize_for_"+spadicName;
				  title = histName + runName;
				  fHM->Create1<TH1I>(histName.Data(), title.Data(), 16,-0.5,15.5);
				  histName = "Masked_"+histName;
				  title = histName + runName;
				  fHM->Create1<TH1I>(histName.Data(), title.Data(), 16,-0.5,15.5);
				  for (Int_t clusterSize=1; clusterSize<=16;clusterSize++){
					  histName = "Cluster("+std::to_string(clusterSize)+")_Heatmap_for_"+spadicName;
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(), 76,-0.1,15.1,2,-0.5,1.5);
				  }
			  }
		  }
	  }
  if(fActivateClusterizer)
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
  			  const Int_t nBins=70;
  			  auto BinBoarders = [/*&nBins*/] (Int_t reBinHigh=32){
  				  const Double_t MaxFreq = (17500000.0);
				  //  				  const Double_t slope=MaxFreq/nBins;
  				  //const Int_t iThreshhold=std::ceil((MaxFreq/Threshold-0.5)/static_cast<Double_t>(reBinHigh));
  				  //LOG(fatal) << "iThresshold = " << iThreshhold;
  				  Double_t* Result=new Double_t[nBins+1];
  				  for(Int_t i=0;i<nBins;i++){
  					  size_t j=nBins-i;
  					  //Result[j]=TMath::Power(TMath::E(),i*slope+TMath::Log(2));
  					  Double_t LowEdge = MaxFreq/(reBinHigh*(i*i)+0.5);
  					  Result[j]=LowEdge;
  				  }/*
  				  for(Int_t i=0;i<nBins-iThreshhold;i++) {
  					  size_t j=nBins-iThreshhold-i;
  					  Double_t LowEdge = MaxFreq/(reBinHigh*(iThreshhold)+0.5+RebinLow*(i));
  					  Result[j]=LowEdge;
  				  }*/
  				  Result[0]=0.0;
				  std::sort(Result,&Result[nBins]);
  				  return Result;
  			  };
  			  Double_t* freqbins = (BinBoarders());
  			  //spadicName = RewriteSpadicName(Form("SysCore%01d_Spadic%01d", syscore, spadic));
  			  histName = "Hitfrequency_for_"+spadicName;
  			  title = histName + runName;
  			  //TH2* tempHist= new TH2D(histName.Data(), title.Data(),40000,0,400,nBins,freqbins);
  			  //delete[] freqbins;
  			  fHM->Add(histName.Data(),new TGraph());
  			  fHM->G1(histName.Data())->GetXaxis()->SetNameTitle("Time t/s","Time t/s");
  			  fHM->G1(histName.Data())->SetNameTitle(histName.Data(),histName.Data());
  			  fHM->G1(histName.Data())->GetYaxis()->SetNameTitle("Hitfrequency f/Hz","Hitfrequency f/Hz");
  			  histName = "Clusterfrequency_for_"+spadicName;
  			  title = histName + runName;
  			  TH2* tempHist= new TH2D(histName.Data(), title.Data(),40000,0,4000,nBins,freqbins);
  			  fHM->Add(histName.Data(),tempHist);
  			  fHM->H2(histName.Data())->GetXaxis()->SetNameTitle("Time t/s","Time t/s");
  			  fHM->H2(histName.Data())->GetYaxis()->SetNameTitle("Clusterfrequency f/Hz","Clusterfrequency f/Hz");
  			  /*
  			   * fHM->Create2<TH2I>(histName.Data(), title.Data(),2000,-0.5,1999.5,5000,0,17600000.0);
  			   * fHM->H2(histName.Data())->GetXaxis()->SetTitle("Full Time");
  			   * fHM->H2(histName.Data())->GetYaxis()->SetTitle("Hitfrequency f/Hz");
  			   * histName = "Masked_"+histName;
  			   * title = histName + runName;
  			   * fHM->Create1<TH1I>(histName.Data(), title.Data(), 16,-0.5,15.5);*/
  		  }
	  }
  if(fActivateClusterizer)
	  if(fDrawPadResponse)
		  for(Int_t Size=1 ;Size <= 16; Size++)
		  {
			  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
				  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
					  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
					  histName = "Pad_Response_"+ spadicName + "_for_Clusters_of_Size_"+std::to_string(Size);
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(), 801,-(4+0.05)*7.125,(4+0.05)*7.125,101,-0.005,1.005);
					  //fHM->Create3<TH3I>(histName.Data(), title.Data(), 801,-(4+0.05)*7.125,(4+0.05)*7.125,101,-0.005,1.005,100,0,1000);
					  fHM->H2(histName.Data())->GetXaxis()->SetTitle("Displacement frac{d}{mm}");
					  fHM->H2(histName.Data())->GetYaxis()->SetTitle("Chargeratio frac{Q_{i}}{#sum_{k}^{ } Q_{k}}");
					  //fHM->H3(histName.Data())->GetZaxis()->SetTitle("TotalCharge");
					  histName = "Cut5_" +histName;
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(), 801,-(4+0.05)*7.125,(4+0.05)*7.125,101,-0.005,1.005);
					  //fHM->Create3<TH3I>(histName.Data(), title.Data(), 801,-(4+0.05)*7.125,(4+0.05)*7.125,101,-0.005,1.005,100,0,1000);
					  fHM->H2(histName.Data())->GetXaxis()->SetTitle("Displacement frac{d}{mm}");
					  fHM->H2(histName.Data())->GetYaxis()->SetTitle("Chargeratio frac{Q_{i}}{#sum_{k}^{ } Q_{k}}");
					  //fHM->H3(histName.Data())->GetZaxis()->SetTitle("TotalCharge");
					  histName = "Central_Pad_Response_"+ spadicName + "_for_Clusters_of_Size_"+std::to_string(Size);
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(), 801,-(4+0.05)*7.125,(4+0.05)*7.125,101,-0.005,1.005);
					  //fHM->Create3<TH3I>(histName.Data(), title.Data(), 801,-(4+0.05)*7.125,(4+0.05)*7.125,101,-0.005,1.005,100,0,1000);
					  fHM->H2(histName.Data())->GetXaxis()->SetTitle("Displacement frac{d}{mm}");
					  fHM->H2(histName.Data())->GetYaxis()->SetTitle("Chargeratio frac{Q_{i}}{#sum_{k}^{ } Q_{k}}");
				  }
			  }
		  }

  if(fActivateClusterizer){
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
              TString histname = "Charge_Spectrum_for_"+spadicName;
              title = histname + runName;
              fHM->Create1<TH1D>(histname.Data(), title.Data(),2001,-0.5,2000.5/*,33,-0.5,32.5*/);
              fHM->H1(histname.Data())->GetXaxis()->SetTitle("Cluster Charge in ADC Units");
              //fHM->H2(histname.Data())->GetYaxis()->SetTitle("ChID");
              fHM->H1(histname.Data())->GetYaxis()->SetTitle("N^{o}");
              // Integral Charge
              TString histname2 = "ChargeIntegral_Spectrum_for_"+spadicName;
              title = histname2 + runName;
              fHM->Create1<TH1D>(histname2.Data(), title.Data(),6001,-0.5,6000.5/*,33,-0.5,32.5*/);
              fHM->H1(histname2.Data())->GetXaxis()->SetTitle("Cluster Charge in ADC Units");
              fHM->H1(histname2.Data())->GetYaxis()->SetTitle("N^{o}");
          }
      }
  }

  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
	  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
		  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
		  histName = "Baseline_for_"+spadicName;
		  title = histName + runName;
		  fHM->Create2<TH2I>(histName.Data(), title.Data(),512,-256.5,255.5,33,-0.5,32.5);
		  histName = "Temp_"+histName;
		  title = histName + runName;
		  fHM->Create2<TH2I>(histName.Data(), title.Data(),512,-256.5,255.5,33,-0.5,32.5);
	  }
  }
  //Prototype for Online Baseline Monitoring
	fBaselineMon->Divide(4,3);
	for (Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore)
	  {
	    for (Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic)
	      {
		spadicName = GetSpadicName (syscore, spadic, kDirectOutput,
						   kFullSpadic).Data ();
		TString Histname = "Baseline_for_" + spadicName;
		fBaselineMon->cd (syscore + spadic * 4 + 1)->SetLogz();;
		fHM->H2 (Histname.Data ())->Draw ("colz2");
	      }
	  }

  //Cluster Coincidences
  {
	  histName="Cluster_Coincidences";
	  title = histName + runName;
	  fHM->Create2<TH2I>(histName.Data(), title.Data(),461,-32.5,32.5,100,0,100);
  }

  if (fDrawClustertypes){
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
			  histName = "Clustertypes_for_"+spadicName;
			  title = histName + runName;
			  fHM->Create2<TH2I>(histName.Data(), title.Data(),17,-0.5,16.5,6,-0.5,5.5);
			  fHM->H2(histName.Data())->GetXaxis()->SetNameTitle("Clustersize in Pads","Clustersize in Pads");
			  fHM->H2(histName.Data())->GetYaxis()->SetNameTitle("Clustertype","Clustertype");
		  }
	  }
  }
  if (fDrawSignalDebugHistograms) {
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic);
			  for(Int_t TriggerType =0;TriggerType<4;TriggerType++){
				  histName = "MaxADC_vs_NrSamples_for_"+spadicName+"_"+triggerTypes[TriggerType];
				  title = histName + runName;
				  fHM->Create2<TH2I>(histName.Data(), title.Data(),33,-0.5,32.5,512,-256.5,255.5);
				  fHM->H2(histName.Data())->GetXaxis()->SetTitle("NrSamples");
				  fHM->H2(histName.Data())->GetYaxis()->SetTitle("MaxADC");
				  histName = "avgBaseline_vs_NrSamples_for_"+spadicName+"_"+triggerTypes[TriggerType];;
				  title = histName + runName;
				  fHM->Create2<TH2I>(histName.Data(), title.Data(),33,-0.5,32.5,512,-256.5,255.5);
				  fHM->H2(histName.Data())->GetXaxis()->SetTitle("NrSamples");
				  fHM->H2(histName.Data())->GetYaxis()->SetTitle("averagedBaseline");
				  vector<string> Hittypes = {"_Hits","_MultiHits"};
				  for (string Hittype : Hittypes){
					  histName = "MaxADC_vs_avgBaseline_for_"+spadicName+Hittype+"_"+triggerTypes[TriggerType];
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(),512,-256.5,255.5,512,-256.5,255.5);
					  fHM->H2(histName.Data())->GetXaxis()->SetTitle("averagedBaseline");
					  fHM->H2(histName.Data())->GetYaxis()->SetTitle("MaxADC");
					  histName = "Exp_Decay" +histName;
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(),512,-256.5,255.5,512,-256.5,255.5);
					  fHM->H2(histName.Data())->GetXaxis()->SetTitle("averagedBaseline");
					  fHM->H2(histName.Data())->GetYaxis()->SetTitle("MaxADC");
					  histName = "First_Sample_vs_last_Sample_for_"+spadicName+Hittype+"_"+triggerTypes[TriggerType];
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(),512,-256.5,255.5,512,-256.5,255.5);
					  fHM->H2(histName.Data())->GetXaxis()->SetTitle("Letztes Sample");
					  fHM->H2(histName.Data())->GetYaxis()->SetTitle("Erstes Sample");
					  histName = "Exp_Decay" +histName;
					  title = histName + runName;
					  fHM->Create2<TH2I>(histName.Data(), title.Data(),512,-256.5,255.5,512,-256.5,255.5);
					  fHM->H2(histName.Data())->GetXaxis()->SetTitle("Letztes Sample");
					  fHM->H2(histName.Data())->GetYaxis()->SetTitle("Erstes Sample");


				  }
				  histName = "avgBaseline_for_"+spadicName+"_"+triggerTypes[TriggerType];
				  title = histName + runName;
				  fHM->Create1<TH1I>(histName.Data(), title.Data(),512,-256.5,255.5);
				  fHM->H1(histName.Data())->GetXaxis()->SetTitle("averagedBaseline");
			  }
		  }
	  }
  }
	{
		for (Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
			for (Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
				spadicName = GetSpadicName(syscore, spadic, kDirectOutput,
						kFullSpadic);
				for (Int_t Channel = 0; Channel < 32; Channel++) {
					histName = "Signalshape_for_" + spadicName + "Channel"
							+ std::to_string(Channel);
					title = histName + runName;
					fHM->Create2<TH2I>(histName.Data(), title.Data(), 33, -0.5,
							32.5, 512, -256.5, 255.5);
				}
			}
		}
	}

  fHM->Add("TsCounter", new TGraph());
  fHM->G1("TsCounter")->SetNameTitle("TsCounter","TsCounter");
	//TODO: Refactor TGraphs
  {
	  vector<string> graphnames={
			  "Baselinetrend_",
			  "TsCounterHit_",
			  "TsCounterHitAborted_",
			  "TsCounterOverflow_",
			  "TsLost_",
			  "TsCounterInfo_",
			  "TsCounterEpoch_",
			  "TsCounterStrange_",
			  "TsStrangeness_",
			  "TSTime_"
	  };
	  std::vector<Int_t> spadicList(NrOfActiveHalfSpadics);
	  std::iota(spadicList.begin(),spadicList.end(),0);
	  for (const string& graphName : graphnames){
		  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
				  for(auto spadic : spadicList) {
				  kSpadicType SpadicType;
				  if(graphName=="Baselinetrend_"){
					  SpadicType=kFullSpadic;
					  if(spadic>=NrOfActiveSpadics)continue;
				  }else{
					  SpadicType= kHalfSpadic;
				  }
				  spadicName = GetSpadicName(syscore, spadic, kDirectOutput,SpadicType);
				  TGraph* currentGraph = new TGraph();
				  fHM->Add(graphName+spadicName.Data(),currentGraph);
				  currentGraph->SetNameTitle((graphName+spadicName).Data(),(graphName+spadicName).Data());
				  currentGraph->GetXaxis()->SetTitle("Timeslice");
				  if(graphName=="Baselinetrend_")currentGraph->GetYaxis()->SetTitle("Baseline");
			  }
		  }
	  }
  }
  fHM->Add("DoubleMessage_MessageType", new TH2I("DoubleMessage_MessageType","DoubleMessage_MessageType",8,-1.5,6.5,8,-1.5,6.5));
  fHM->H2("DoubleMessage_MessageType")->GetYaxis()->SetTitle("MessageType TClonesArray");
  fHM->H2("DoubleMessage_MessageType")->GetXaxis()->SetTitle("MessageType Buffer Map");

  fHM->Add("TriggerType_vs_InfoType", new TH2I("TriggerType_vs_InfoType","TriggerType_vs_InfoType",5,-1.5,3.5,9,-1.5,7.5));
  fHM->H2("TriggerType_vs_InfoType")->GetYaxis()->SetTitle("InfoType");
  fHM->H2("TriggerType_vs_InfoType")->GetXaxis()->SetTitle("TriggerType");

  fHM->Add("TriggerType_vs_StopType", new TH2I("TriggerType_vs_StopType","TriggerType_vs_StopType",5,-1.5,3.5,7,-1.5,5.5));
  fHM->H2("TriggerType_vs_StopType")->GetYaxis()->SetTitle("StopType");
  fHM->H2("TriggerType_vs_StopType")->GetXaxis()->SetTitle("TriggerType");

  fHM->Add("InfoType_vs_StopType", new TH2I("InfoType_vs_StopType","InfoType_vs_StopType",9,-1.5,7.5,7,-1.5,5.5));
  fHM->H2("InfoType_vs_StopType")->GetYaxis()->SetTitle("StopType");
  fHM->H2("InfoType_vs_StopType")->GetXaxis()->SetTitle("InfoType");

  fHM->Add("NrSamples_vs_StopType", new TH2I("NrSamples_vs_StopType","NrSamples_vs_StopType",34,-1.5,32.5,7,-1.5,5.5));
  fHM->H2("NrSamples_vs_StopType")->GetXaxis()->SetTitle("Nr. of Samples per Message");
  fHM->H2("NrSamples_vs_StopType")->GetYaxis()->SetTitle("StopType");

  fHM->Add("NrSamples_vs_TriggerType", new TH2I("NrSamples_vs_TriggerType","NrSamples_vs_TriggerType",34,-1.5,32.5,5,-1.5,3.5));
  fHM->H2("NrSamples_vs_TriggerType")->GetXaxis()->SetTitle("Nr. of Samples per Message");
  fHM->H2("NrSamples_vs_TriggerType")->GetYaxis()->SetTitle("TriggerType");

  fHM->Add("NrSamples_vs_InfoType", new TH2I("NrSamples_vs_InfoType","NrSamples_vs_InfoType",34,-1.5,32.5,9,-1.5,7.5));
  fHM->H2("NrSamples_vs_InfoType")->GetXaxis()->SetTitle("Nr. of Samples per Message");
  fHM->H2("NrSamples_vs_InfoType")->GetYaxis()->SetTitle("InfoType");

  fHM->Add("InfoType_vs_Channel", new TH2I("InfoType_vs_Channel","InfoType_vs_Channel",35,-1.5,33.5,10,0.5,10.5));
  fHM->H2("InfoType_vs_Channel")->GetXaxis()->SetTitle("Channel");
  fHM->H2("InfoType_vs_Channel")->GetYaxis()->SetTitle("Info or Type");

  fHM->Add("Hit_Coincidences", new TH2I("Hit_Coincidences","Hit_Coincidences",64,-0.5,63.5,64,-0.5,63.5));
  fHM->H2("Hit_Coincidences")->GetXaxis()->SetTitle("SpadicChannel");
  fHM->H2("Hit_Coincidences")->GetYaxis()->SetTitle("SpadicChannel");

  fHM->Add("Correlation_Map", new TH2I("Correlation_Map","Correlation_Map",63,-31.5,31.5,21,-0.5,20.5));
  fHM->H2("Correlation_Map")->GetXaxis()->SetTitle("#Delta Pad");
  fHM->H2("Correlation_Map")->GetYaxis()->SetTitle("#Delta Time (1 timestamp = 57 ns)");


  for (auto i : fCorrmaps){
	  histName="Correlation_Map_"+i.first+"_"+i.second;
	  fHM->Add(histName.Data(), new TH2I(histName.Data(),histName.Data(),63,-31.5,31.5,200,-0.5,20000.5));
	  fHM->H2(histName.Data())->GetXaxis()->SetTitle("#Delta Pad");
	  fHM->H2(histName.Data())->GetYaxis()->SetTitle("#Delta Time (1 timestamp = 57 ns)");
  }
}

// ----              -------------------------------------------------------
 Int_t CbmTrdTimeCorrel::GetModuleID(CbmSpadicRawMessage* raw)
  {
    if (raw == NULL){
      LOG(error) << "CbmTrdRawBeamProfile::GetModuleID: CbmSpadicRawMessage == NULL";
      return -1;
    } else {
      return GetLayerID(raw);
    }
  }
 // ----              -------------------------------------------------------
void CbmTrdTimeCorrel::FitBaseline() {
	if (!fCalculateBaseline) {
		return;
	}
	TString outfile = FairRootManager::Instance()->GetOutFile()->GetName();
	outfile.ReplaceAll(".root", "_Calibration.root");
	TFile CalibrationFile(outfile, "RECREATE");
	CalibrationFile.cd();
	vector<TFitResult> FitResults;
	//TODO: Refine fits based on previous iterations
	//DONE: Check for Rootfile of fits and import them
	  for(Int_t syscore = 0; syscore < NrOfActiveSyscores; ++syscore) {
		  for(Int_t spadic = 0; spadic < NrOfActiveSpadics; ++spadic) {
			  string spadicName = GetSpadicName(syscore, spadic, kDirectOutput,kFullSpadic).Data();
			  string histName = "Baseline_for_" + spadicName;
			  TH2* BaselineHistogram2D = fHM->H2(histName);
			  for (Int_t ChannelID = 0; ChannelID < 32; ChannelID++) {
				  TH1D* BaselineProjection = BaselineHistogram2D->ProjectionX("_px",
						  ChannelID + 1, ChannelID + 1);
				  if (BaselineProjection->GetEntries() < 50)
					  continue;
				  TFitResultPtr Baselinefit = BaselineProjection->Fit("gaus", "MQS", "",
						  -253, 253);
				  TString fitname = "Fit_Baseline_for_"
						  + spadicName + "_Channel_"
						  + std::to_string (ChannelID);
				  Baselinefit->SetName(fitname);
				  FitResults.push_back(*Baselinefit);
				  //Baselinefit->Print("V ");
			  }
		  }
	  }
	  for (auto x : FitResults) {
		x.Write();
	}
	FairRootManager::Instance()->GetOutFile()->cd();
	CalibrationFile.Close();
}

// ----              -------------------------------------------------------
void CbmTrdTimeCorrel::FitPRF() {
	TString outfile = FairRootManager::Instance()->GetOutFile()->GetName();
	FairRootManager::Instance()->GetOutFile()->cd();
	vector<TFitResult> FitResults;
	auto PRFLambda =[&](Double_t Displacement, Double_t * Parameters)
        {
          Double_t d = Displacement;
          Double_t K3 = Parameters[0];
          Double_t A = Parameters[2];
          Double_t SqrtK3 = sqrt(K3);
          Double_t W=Parameters[3];
          Double_t h = Parameters[1];

          return std::fabs(
              - A / (2. * atan(SqrtK3)) * (
                  std::atan(SqrtK3 * std::tanh(TMath::Pi() * (-2. + SqrtK3 ) * (W + 2.* d) / (8.* h) )) +
                  std::atan(SqrtK3 * std::tanh(TMath::Pi() * (-2. + SqrtK3 ) * (W - 2.* d) / (8.* h) ))
              )
          );
        };
	TF1* PRF =
	    new TF1 (
	        "PRF",[&PRFLambda](double*x, double *p){ return PRFLambda(x[0],p);}
	        ,-10.0, 10.0, 4);
	//TODO: Check for Rootfile of fits
	auto MPVProfile = [&](TH2* PRFHist,Int_t size=3){
	  TString ProfileName= "Profile_";
	      ProfileName +=PRFHist->GetName();
          TProfile* PRFProfile =  new TProfile(ProfileName.Data(),ProfileName.Data(), 1001,-(size+0.05)*7.125,(size+0.05)*7.125);
          TF1* MPVFkt = new TF1("MPVFkt","gaus",0,1);
          MPVFkt->SetParameters(100,0.5,0.125);
          MPVFkt->SetParLimits(1,0,1);
          MPVFkt->SetParLimits(2,0,1);
	  if (PRFHist->GetEntries() < 50) return PRFProfile;
	  TH1D* TempProfile=nullptr;
          for (Int_t BinX=1;BinX<=1001;BinX++){
              TempProfile=PRFHist->ProjectionY("_px",BinX,BinX);
              //std::cout <<BinX << " " << TempProfile->GetEntries()<< std::endl;
              if (TempProfile->GetEntries() < 1)continue;
              TFitResultPtr MPVFit = TempProfile->Fit(MPVFkt, "MQS","",0,1.0);
              if(MPVFit->GetParams()==nullptr) continue;
              Double_t Mean=MPVFit->Parameter(1);
              Double_t Sigma=(MPVFit->Parameter(2));
              //std::cout <<BinX << " " << Mean << " " << Sigma << std::endl;
              PRFProfile->Fill(PRFProfile->GetBinCenter(BinX),Mean,TempProfile->GetEntries());
              PRFProfile->SetBinError((BinX),Sigma);
          }
          delete TempProfile;
          delete MPVFkt;
          return PRFProfile;
	};
	for (Int_t size=3;size<=4;size++)
	for (Int_t Detector = 0; Detector <= NrOfSpadics / 2; Detector++) {
		string Detectorname = (Detector == 0 ? "Frankfurt" : "Muenster");
		string histName = "Pad_Response_" + Detectorname
				+ "_for_Clusters_of_Size_" + std::to_string(size);
		TH2* PRFHist = fHM->H2(histName);
		if (PRFHist->GetEntries() < 50)
			continue;
		//TString ProfileName ="Profile_"+histName;
                TCanvas *c1 = new TCanvas("c1","",1600,600);
                TH1* PRFProfile = MPVProfile(PRFHist);
		PRFProfile->Draw("E1");
		PRFProfile->Write();
	        PRF->SetParameters(0.1,3.5,1,7.125);
	        PRF->SetParName(0,"K3");
                //PRF->FixParameter(0,0.92);
	        //PRF->SetParLimits(0,0.78,0.92);
	        PRF->SetParName(1,"h");
	        PRF->FixParameter(1,3.5);
	        PRF->SetParName(2,"A");
	        PRF->FixParameter(2,1.0);
	        //PRF->SetParLimits(2,0.99999,1.00001);
	        PRF->SetParName(3,"W");
	        PRF->FixParameter(3,7.125);
		PRF->Draw();
                c1->Update();
                TString fitname = "Fit_PRF_" + histName;
                c1->SaveAs("pics/Pre_"+fitname+".pdf");
                c1->SaveAs("pics/Pre_"+fitname+".png");
                c1->Clear();
                PRFProfile->Draw("E2");
		TFitResultPtr PRFFit = PRFProfile->Fit(PRF, "S", "", -10.0, 10.0);
		PRFFit->SetName(fitname);
		FitResults.push_back(*PRFFit);
		PRFFit->Print("V");
		c1->Update();
		c1->SaveAs("pics/"+fitname+".pdf");
                c1->SaveAs("pics/"+fitname+".png");
                delete c1;
		std::cout << std::endl;
		std::cout << "Fits " << Detectorname << " " << FitResults.size()
				<< std::endl;
	}
        outfile.ReplaceAll(".root", "_Calibration.root");
        TFile CalibrationFile(outfile, "UPDATE");
        CalibrationFile.cd();
	for (auto x : FitResults) {
		x.Write();
	}
	FairRootManager::Instance()->GetOutFile()->cd();
	CalibrationFile.Close();
}

// ----              -------------------------------------------------------
Int_t CbmTrdTimeCorrel::GetSpadicID(Int_t sourceA)
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
// ----              -------------------------------------------------------
Int_t CbmTrdTimeCorrel::GetSyscoreID(Int_t eqID)
{
	Int_t SyscoreID=eqID-BaseEquipmentID;
	if((SyscoreID<0||SyscoreID>NrOfActiveSyscores)){
	    LOG(error) << "EqID " << eqID << " not known.";
	    SyscoreID=-1;
	}
	return SyscoreID;
}
// ----              -------------------------------------------------------
TString CbmTrdTimeCorrel::GetStopName(Int_t stopType)
{
  TString stopName="";
  switch (stopType) {
  case (-1):
    stopName="Info or epoch mess";
    break;
  case (0):
    stopName="Normal end of message";
    break;
  case (1):
    stopName="Channel buffer full";
    break;
  case (2):
    stopName="Ordering FIFO full";
    break;
  case (3):
    stopName="Multi hit";
    break;
  case (4):
    stopName="Multi hit and channel buffer full";
    break;
  case (5):
    stopName="Multi hit and ordering FIFO full";
    break;
  default:
    LOG(error) << "stopType " << stopType << " not known.";
    break;
  }
  return stopName;
}

TString CbmTrdTimeCorrel::RewriteSpadicName(TString spadicName) 
{
  if(spadicName=="SysCore_0_Spadic_0") {
    if(fRewriteSpadicName) spadicName="Frankfurt";
  }else if(spadicName=="SysCore_0_Spadic_1"){
    if(fRewriteSpadicName) spadicName="Muenster";
  }else{
    spadicName="";
  }
  
  return spadicName;
}

// ----              -------------------------------------------------------
  Int_t CbmTrdTimeCorrel::GetChannelOnPadPlane(Int_t SpadicChannel)
  {
    Int_t channelMapping[32] = {31,15,30,14,29,13,28,12,27,11,26,10,25, 9,24, 8,
				23, 7,22, 6,21, 5,20, 4,19, 3,18, 2,17, 1,16, 0};
    if (SpadicChannel < 0 || SpadicChannel > 31){
      if (SpadicChannel !=-1) LOG(error) << "CbmTrdTimeCorrel::GetChannelOnPadPlane ChId " << SpadicChannel;
      return -1;
    } else {
      return channelMapping[SpadicChannel];
    }
  }

//---------------------------------------------------------------------------
  Int_t  CbmTrdTimeCorrel::GetSectorID(CbmSpadicRawMessage*)// To be used to create CbmTrdDigis
  {
    return 0;
  }
//---------------------------------------------------------------------------
Int_t  CbmTrdTimeCorrel::GetRowID(CbmSpadicRawMessage* raw)// To be used to create CbmTrdDigis
{
  Int_t sourceA = raw->GetSourceAddress();
  Int_t chID = raw->GetChannelID();
  Int_t spaID = GetSpadicID(sourceA);
  if(chID > -1 && chID < 16 && spaID%2==1) chID+=16; // eqID ?
  Int_t padID = GetChannelOnPadPlane(chID);// Remapping from ASIC to pad-plane channel numbers.
  if (padID > 16) 
    return 1;
  else
    return 0;
}
//---------------------------------------------------------------------------
Int_t  CbmTrdTimeCorrel::GetLayerID(CbmSpadicRawMessage* raw)// To be used to create CbmTrdDigis
{
  TString name = GetSpadicName(raw->GetEquipmentID(), raw->GetSourceAddress());
  if (name == "Frankfurt")
    return 1;
  else if (name == "Muenster")
    return 2;
  else
    return -1;
}
//---------------------------------------------------------------------------
Int_t  CbmTrdTimeCorrel::GetColumnID(CbmSpadicRawMessage* raw)// To be used to create CbmTrdDigis
{
  Int_t sourceA = raw->GetSourceAddress();
  Int_t chID = raw->GetChannelID();
  Int_t spaID = GetSpadicID(sourceA);
  if(chID > -1 && chID < 16 && spaID%2==1) chID+=16; // eqID ?
 
  Int_t columnId = GetChannelOnPadPlane(chID);
  if (columnId >= 16) 
    columnId -= 16;
  return columnId;
}
//---------------------------------------------------------------------------
std::map<Int_t, std::map<Int_t,std::map<ULong_t, Long_t> > > CbmTrdTimeCorrel::CalculateTimestampOffsets(const EpochMap &epochBuffer)
{
  //Calculate time offsets between various Spadics
  std::map<Int_t, std::map<Int_t,std::map<ULong_t, Long_t > > > epochOffsets;
  //loop over all SpaIDs in the epochBuffer for base timestamps
  for (auto baseSpaIDIt = epochBuffer.begin() ; baseSpaIDIt != epochBuffer.end(); ++baseSpaIDIt)
    //loop over all SpaIDs in the epochBuffer for comparing timestamps
    for (auto compSpaIDIt = epochBuffer.begin() ; compSpaIDIt != epochBuffer.end(); ++compSpaIDIt){
      //Retrieve SpadicID's
      Int_t baseSource = baseSpaIDIt->second.begin()->second->GetSourceAddress();
      Int_t baseSpaID = GetSpadicID(baseSource);
      Int_t compSource = compSpaIDIt->second.begin()->second->GetSourceAddress();
      Int_t compSpaID = GetSpadicID(compSource);
      //loop over all EpochMessages in the epochBuffer for comparing timestamps
      auto baseTimestampIt = baseSpaIDIt->second.begin();
      auto compTimestampIt = compSpaIDIt->second.begin();
      for ((baseTimestampIt = baseSpaIDIt->second.begin(),compTimestampIt = compSpaIDIt->second.begin());baseTimestampIt != baseSpaIDIt->second.end(); ++baseTimestampIt, ++compTimestampIt)
	{
	  //If there are no more messages in the comparing buffer set the difference to the last known value
	  if(compTimestampIt == compSpaIDIt->second.end())
	    {
	      for (auto innerIt = baseTimestampIt; innerIt != baseSpaIDIt->second.end(); ++innerIt)
		{
		  innerIt--;
		  ULong_t previousTimestamp = (innerIt++)->second->GetFullTime();
		  ULong_t baseTimestamp = innerIt->second->GetFullTime();
		  epochOffsets[baseSpaID][compSpaID][baseTimestamp] = epochOffsets[baseSpaID][compSpaID][previousTimestamp];
		}
	      break;
	    } else{
	    ULong_t baseTimestamp = baseTimestampIt->second->GetFullTime();
	    ULong_t compTimestamp = compTimestampIt->second->GetFullTime();
	    epochOffsets[baseSpaID][compSpaID][baseTimestamp]=static_cast<Long_t>(compTimestamp-baseTimestamp);
	  }
	}
    }
  return epochOffsets;
}


CbmTrdTimeCorrel::Cluster::Cluster(std::vector<Int_t> * BaselineArray,Int_t initWindowsize, Int_t ChargeThreshhold = 50) :
    TObject (),
    fMaxStopType(0),    
    fEntries (),
    fParametersCalculated(false),
    fPreCalculatedBaseline(true),
    fSyscore(0),
    fSpadic (0),
    fRow(0),
    fType (0),
    fTotalCharge (0),
    fTotalIntegralCharge (0),
    fMaxADC(-256),
    fMaxCharge(-256),
    fWindowsize(initWindowsize),
    fClusterChargeThreshhold(ChargeThreshhold),     
    fBaseline(BaselineArray),
    fFullTime(),
    fHorizontalPosition (0)
{};

CbmTrdTimeCorrel::Cluster::~Cluster(){};

Bool_t CbmTrdTimeCorrel::Cluster::AddEntry(CbmSpadicRawMessage NewEntry){
  auto CompareSpadicMessages=
      [&](CbmSpadicRawMessage a,CbmSpadicRawMessage b)
      {
        Int_t ChIDA = a.GetChannelID() + ((CbmTrdTimeCorrel::GetSpadicID(a.GetSourceAddress()) %2 == 1)? 16 : 0);
        Int_t ChIDB = b.GetChannelID() + ((CbmTrdTimeCorrel::GetSpadicID(b.GetSourceAddress()) %2 == 1)? 16 : 0);
        Int_t PadA = CbmTrdTimeCorrel::GetChannelOnPadPlane(ChIDA);
        Int_t PadB = CbmTrdTimeCorrel::GetChannelOnPadPlane(ChIDB);
        Int_t rowA = (CbmTrdTimeCorrel::GetSpadicID(a.GetSourceAddress())/2)+PadA/16;
        Int_t rowB = (CbmTrdTimeCorrel::GetSpadicID(b.GetSourceAddress())/2)+PadB/16;
        if(a.GetFullTime() == b.GetFullTime())
          if(rowA==rowB)
            if(PadA==PadB)
              return true;
        return false;
      };
  auto CompareSpadicMessagesSmaller=
      [&](CbmSpadicRawMessage a,CbmSpadicRawMessage b)
      {
        Int_t ChIDA = a.GetChannelID() + ((CbmTrdTimeCorrel::GetSpadicID(a.GetSourceAddress()) %2 == 1)? 16 : 0);
        Int_t ChIDB = b.GetChannelID() + ((CbmTrdTimeCorrel::GetSpadicID(b.GetSourceAddress()) %2 == 1)? 16 : 0);
        Int_t PadA = CbmTrdTimeCorrel::GetChannelOnPadPlane(ChIDA);
        Int_t PadB = CbmTrdTimeCorrel::GetChannelOnPadPlane(ChIDB);
        Int_t rowA = (CbmTrdTimeCorrel::GetSpadicID(a.GetSourceAddress())/2)+PadA/16;
        Int_t rowB = (CbmTrdTimeCorrel::GetSpadicID(b.GetSourceAddress())/2)+PadB/16;
        if(rowA<rowB)
          return true;
        else if(rowA==rowB)
          if(PadA<PadB)
            return true;
        return false;
      };

  fParametersCalculated = false;
  fEntries.push_back(NewEntry);
  std::sort(fEntries.begin(),fEntries.end(),CompareSpadicMessagesSmaller);
  std::unique(fEntries.begin(),fEntries.end(),CompareSpadicMessages);
  fSpadic = CbmTrdTimeCorrel::GetSpadicID(fEntries.begin()->GetSourceAddress());
  fRow = GetHorizontalMessagePosition(*(fEntries.begin()))/16;
//GetChannelOnPadPlane(fEntries.begin()->GetChannelID() + ((GetSpadicID(fEntries.begin()->GetSourceAddress()) %2 == 1)? 16 : 0))/16;
  BeforeLoop:
  if (fIs2D) {
	  for (auto it = fEntries.begin(); it != fEntries.end(); it++) {
		  if (((CbmTrdTimeCorrel::GetSpadicID(it->GetSourceAddress()) / 2) != fSpadic / 2)) {
			  fEntries.erase(it);
			  goto BeforeLoop;
		  }
	  }
  } else {
	  for (auto it = fEntries.begin(); it != fEntries.end(); it++) {
		  if (((CbmTrdTimeCorrel::GetSpadicID(it->GetSourceAddress()) / 2) != fSpadic / 2)
				  || GetHorizontalMessagePosition(*it) / 16 != fRow) {
			  fEntries.erase(it);
			  goto BeforeLoop;
		  }
	  }
  }//End of goto scope
  return true;
}

Int_t CbmTrdTimeCorrel::Cluster::GetTotalCharge() {
	if (fParametersCalculated) {
		return fTotalCharge;
	} else {
		CalculateParameters();
		return fTotalCharge;
	}
}

 Int_t CbmTrdTimeCorrel::Cluster::GetTotalIntegralCharge() {  //For Integral Charge
	if (fParametersCalculated) {
		return fTotalIntegralCharge;
	} else {
		CalculateParameters();
		return fTotalIntegralCharge;
	}
}

Bool_t CbmTrdTimeCorrel::Cluster::FillChargeDistribution(TH2* ChargeMap,TH2* CentralMap)
{
  if (!fParametersCalculated)
    CalculateParameters();
  //if(fMaxStopType>0)return false;
  for (auto currentMessage : fEntries )
    {
      Int_t Charge = GetMaxADC(currentMessage);
      Float_t ChargeRatio =  static_cast<Float_t>(Charge)/static_cast<Float_t>(GetTotalCharge());
      Float_t Displacement = static_cast<Float_t>(GetHorizontalMessagePosition(currentMessage));
      Displacement -= GetHorizontalPosition();
      Displacement *= 7.125;
      if(ChargeMap!=nullptr)ChargeMap->Fill(Displacement,ChargeRatio/*,fTotalCharge*/);
      if(CentralMap!=nullptr && currentMessage.GetTriggerType()!=2)CentralMap->Fill(Displacement,ChargeRatio/*,fTotalCharge*/);
  }
  return true;
}

inline Int_t CbmTrdTimeCorrel::Cluster::GetMaxADC(CbmSpadicRawMessage& message,Bool_t SubtractBaseline)
{
  return CbmTrdTimeCorrel::GetMaxADC(message,SubtractBaseline,fBaseline);
}

Int_t CbmTrdTimeCorrel::GetMaxADC(CbmSpadicRawMessage& message,Bool_t SubtractBaseline,std::vector<Int_t>* fBaseline)
{
  Int_t maxADC=-255;
  Int_t previousADC=-255;
  Int_t NrSamples = message.GetNrSamples ();
  Bool_t validHit=(NrSamples==32);
  //  Int_t Spadic = GetSpadicID(message.GetSourceAddress());
  Int_t Syscore = GetSyscoreID(message.GetEquipmentID());
  if(fBaseline==nullptr) SubtractBaseline=false;
  for (Int_t i = 0 ; i < NrSamples ; i++){
      Int_t currentADC = *(message.GetSamples() + i);
      if(currentADC > maxADC) maxADC=currentADC;
      if((currentADC > previousADC) && ((currentADC - previousADC)>10)) validHit = true;
      previousADC = currentADC;
  }
  //Int_t Baseline=GetAvgBaseline(message);
  return (SubtractBaseline ?  maxADC-(*fBaseline)[Syscore*NrOfHalfSpadics * 16+16*GetSpadicID(message.GetSourceAddress())+message.GetChannelID()] : maxADC);
}

Int_t CbmTrdTimeCorrel::Cluster::GetMessageChargeIntegral(CbmSpadicRawMessage& message)
{
  Int_t maxADC=-255;
  Int_t integral=0;
  Bool_t validHit=false;
  Int_t previousADC=-255;
  Int_t NrSamples = message.GetNrSamples ();
  Int_t Baseline=0;
 if(!fPreCalculatedBaseline){
 for (Int_t i = NrSamples -3 ; i < NrSamples ; i++)
   Baseline += *(message.GetSamples() + i);
 Baseline = Baseline/3;
 }
 else{
	 Baseline = (*fBaseline)[0*NrOfActiveHalfSpadics*16+16*GetSpadicID(message.GetSourceAddress())+message.GetChannelID()];
 }
 for (Int_t i = 0 ; i < NrSamples ; i++){
   Int_t currentADC = *(message.GetSamples() + i) - Baseline; // correct each tb with the channels Baseline
     integral += currentADC;
     if(currentADC > maxADC) maxADC=currentADC;
     if((currentADC > previousADC) && ((currentADC - previousADC)>10)) validHit = true;
 }

 return (integral);
};


Int_t CbmTrdTimeCorrel::GetAvgBaseline(CbmSpadicRawMessage& message,Int_t n){
	  Int_t NrSamples = message.GetNrSamples ();
	  if(NrSamples==0||n==0) return -255;
	  if(NrSamples<n) n = NrSamples;
	  Int_t Baseline=0;
	  for (Int_t i = NrSamples -n ; i < NrSamples ; i++)
	    Baseline += *(message.GetSamples() + i);
	  Baseline /= n;
	  return Baseline;
};


void CbmTrdTimeCorrel::FillBaselineHistogram(CbmSpadicRawMessage* message){
	string histName;
	string spadicName = GetSpadicName(message->GetEquipmentID(), message->GetSourceAddress(),kRawData,kFullSpadic).Data();
	histName = "Temp_Baseline_for_"+spadicName;
	Int_t StopType = message->GetStopType();
	if (StopType > 0) return;
	//Int_t * Samples=message->GetSamples();
	//if (GetMaxADC(*message,false)>Samples[0]) return;
	TH2* Histogram = fHM->H2(histName);
	Int_t ChID =  message->GetChannelID() + ((GetSpadicID(message->GetSourceAddress()) %2 == 1)? 16 : 0);
	//	Int_t NrSamples = message->GetNrSamples ();
	Histogram->Fill(GetAvgBaseline(*message),ChID);

};

void CbmTrdTimeCorrel::FillSignalShape(CbmSpadicRawMessage& message,string Hist){
	string histName=Hist;
	if(Hist=="") return;
	Int_t NrSamples = message.GetNrSamples();
	Int_t * Samples = message.GetSamples();
	TH2* Histogram = nullptr;
	Histogram = fHM->H2(histName);
	for (Int_t i = 0 ; i < NrSamples ; i++){
		Histogram->Fill(i,*(Samples+ i));
	}
}

std::pair<std::vector<CbmSpadicRawMessage>::iterator,std::vector<CbmSpadicRawMessage>::iterator> CbmTrdTimeCorrel::Cluster::GetEntries()
{
  return std::make_pair(fEntries.begin(),fEntries.end());
}

Int_t CbmTrdTimeCorrel::Cluster::GetSpadic(){
  if(fParametersCalculated) return fSpadic;
  else {
      CalculateParameters();
      return fSpadic;
  }
}

Int_t CbmTrdTimeCorrel::Cluster::GetRow(){
  if(fParametersCalculated) return (static_cast<Int_t>(fHorizontalPosition)/16);
  else {
      CalculateParameters();
      return (static_cast<Int_t>(fHorizontalPosition)/16);
  }
}

Bool_t CbmTrdTimeCorrel::Cluster::Get2DStatus(){
	if(fParametersCalculated) return fIs2D;
	else {
		CalculateParameters();
		return fIs2D;
	}
}

ULong_t CbmTrdTimeCorrel::Cluster::GetFulltime(){
	if(fParametersCalculated) return fFullTime;
	else {
		CalculateParameters();
		return fFullTime;
	}
}


Float_t CbmTrdTimeCorrel::Cluster::GetHorizontalPosition(){
  if(fParametersCalculated) return fHorizontalPosition;
  else {
      CalculateParameters();
      return fHorizontalPosition;
  }
}

Int_t CbmTrdTimeCorrel::Cluster::GetHorizontalMessagePosition(CbmSpadicRawMessage& Message){
  Int_t Position= CbmTrdTimeCorrel::GetChannelOnPadPlane(Message.GetChannelID()+((GetSpadicID(Message.GetSourceAddress())%2 == 1) ? 16 : 0 ));
  return Position;
}

Int_t CbmTrdTimeCorrel::Cluster::Type(){
	/* Get Information about the status of the Cluster
	 * Type 0 means that the cluster is complete (no gaps and capping TriggerType 2 messages)
	 * Type 1 means that the cluster is incomplete (either with gaps or without capping TriggerType 2 messages)
	 * Type 2 means that the cluster is hitless (no selftriggered HitMessages)
	 * Type 3 means that the Cluster is invalid (see Veto())
	 */
  if(fParametersCalculated) return fType;
  else {
      CalculateParameters();
      return fType;
  }
}

void CbmTrdTimeCorrel::Cluster::Veto() {
	//Abort Conditions
	if (fType > 0 || size() <= 2)
		return;
	//secondly veto based on total charge
	if (fTotalCharge <= 0) {
		//std::cout << " Veto based on Threshold" << std::endl;
		fType = 3;
		return;
	}
	if (true||size()<3|| size()>4){
		if (fMaxADC >= 0) {
			//std::cout << " Veto based on Threshold" << std::endl;
			fType = 5;
			return;
		}
	} else if (size() == 3) {
		if ((GetMaxADC(fEntries.at(0)) > GetMaxADC(fEntries.at(1)))
				|| (GetMaxADC(fEntries.at(2)) > GetMaxADC(fEntries.at(1)))) {
			fType = 5;
			return;
		}
	} else if (size() == 4) {
		if (((GetMaxADC(fEntries.at(0)) > GetMaxADC(fEntries.at(1)))
				|| (GetMaxADC(fEntries.at(3)) > GetMaxADC(fEntries.at(1))))
			|| ((GetMaxADC(fEntries.at(0)) > GetMaxADC(fEntries.at(2)))
				|| (GetMaxADC(fEntries.at(3)) > GetMaxADC(fEntries.at(2))))) {
			fType = 5;
			return;
		}
	}

	return;

	//Lastly Veto based on Charge Distribution.
	Float_t VetoThreshhold = 100.0 / (size() - 0.50);

	for (auto x : fEntries) {
		Float_t ChargeRatio = 100.0 * static_cast<Float_t>(GetMaxADC(x))
				/ fTotalCharge;
		/*
		Float_t Displacement =
				static_cast<Float_t>(GetHorizontalMessagePosition(x))
						- fHorizontalPosition;
		*/
		if(x.GetTriggerType()==1||x.GetTriggerType()==3){
			if(ChargeRatio<VetoThreshhold)
			{
				fType=6;
			}

		}
		/*
		if(x.GetTriggerType()==2)
        if(x.GetTriggerType()==1||x.GetTriggerType()==3){
			if (ChargeRatio<VetoThreshhold) fType=3;
		}
		continue;
		if (std::abs(Displacement) > (size() / 2.0)) {
			//std::cout << " Veto based on width" << std::endl;
			fType = 3;
			return;
		} else if ((x.GetTriggerType() ==1)||(x.GetTriggerType()==3))
		if (std::abs(Displacement)
				<= static_cast<Float_t>((size() - 2.0) / 2.0)) {
			if ((ChargeRatio < VetoThreshhold)) {
				//std::cout << "Veto based on Distribution " << ChargeRatio << " " << VetoThreshhold << " " <<Displacement<< " " <<std::abs(Displacement)<< " " <<  static_cast<Float_t>((size()-2.0)/2.0) <<  std::endl;
				fType = 3;
				return;
			}
		} else if (std::abs(Displacement)
				>= static_cast<Float_t>((size() - 2.0) / 2.0)) {
			if (ChargeRatio >= VetoThreshhold) {
				//std::cout << "Veto based on Distribution " << ChargeRatio << " " << VetoThreshhold << " " <<Displacement<< " " <<std::abs(Displacement)<< " " <<  static_cast<Float_t>((size()-2.0)/2.0) <<  std::endl;
				fType = 3;
				return;
			}
		}*/
	}
}

std::pair<Int_t,Float_t> CbmTrdTimeCorrel::Cluster::GetPosition(){
  Int_t Detector = GetSpadic()/2;
  Float_t HorPos = GetHorizontalPosition();
  return std::make_pair(Detector,HorPos);
}

Size_t CbmTrdTimeCorrel::Cluster::size(){
  return fEntries.size();
}

void CbmTrdTimeCorrel::Cluster::CalculateParameters(){
  fSpadic = CbmTrdTimeCorrel::GetSpadicID(fEntries.begin()->GetSourceAddress());//GetChannelOnPadPlane(fEntries.begin()->GetChannelID() + ((GetSpadicID(fEntries.begin()->GetSourceAddress()) %2 == 1)? 16 : 0))/16;
  fSyscore = CbmTrdTimeCorrel::GetSyscoreID(fEntries.begin()->GetEquipmentID());
  fType = 0;
  fTotalCharge = 0;
  fTotalIntegralCharge = 0;
  fHorizontalPosition = 0;
  fFullTime = fEntries.begin()->GetFullTime();
  fIs2D=false;
  std::vector<Int_t> unweightedPosSum;
  std::vector<Int_t> Charges;
  std::vector<Int_t> ChargesIntegral;
  Int_t NumberOfTypeTwoMessages=0;
  Int_t NumberOfHits=0;
  Int_t LastPad= GetHorizontalMessagePosition(*fEntries.begin())-1;
  Int_t maxADC = -255;
  fMaxADC=maxADC;
  fMaxStopType=0;
  for(auto message : fEntries){
	  Int_t CurrentPad = GetHorizontalMessagePosition(message);
      if (LastPad+1 != CurrentPad&&fType<3)
	{
    	  //std::cout << LastPad << " " << CurrentPad << std:: endl;
	  fType=1;
	}
	  LastPad = CurrentPad;
	  Int_t Charge = GetMaxADC(message); // here the calculation of the Charge takes place
	  Int_t ChargeIntegral = GetMessageChargeIntegral(message);
      maxADC = GetMaxADC(message,false);
      if(maxADC > fMaxADC) fMaxADC=maxADC;
      if(message.GetStopType()>fMaxStopType)fMaxStopType=message.GetStopType();
      if(Charge < 0){
    	  fType = 4;
      }
      ChargesIntegral.push_back(ChargeIntegral);//charge Integral
      fTotalIntegralCharge += ChargeIntegral;
      Charges.push_back(Charge);// chargemax of each message in a cluster
      fTotalCharge += Charge;
      unweightedPosSum.push_back(CurrentPad);
      if(message.GetTriggerType()==2) NumberOfTypeTwoMessages++;
      if(message.GetTriggerType()==1||message.GetTriggerType()==3) NumberOfHits++;
  }
  // if(!(fEntries.size()==4)) fTotalCharge = -42;
  //std::cout << fEntries.size() << "   " << fTotalCharge << std::endl;

  if(NumberOfTypeTwoMessages!=2) fType=1;
  if(NumberOfHits==0) fType=2;
  const Float_t PadWidth = 1.0;//7.0/7.125;
  //  const Float_t sigma = 0.646432;
  if (size()<3||size()>4||fType > 0/*||(size()==2&&!(fType==1||fType==2))*/){
	  for (UInt_t i=0;i<fEntries.size();i++){
		  Double_t Weight = static_cast<Double_t>(Charges.at(i))/static_cast<Double_t>(fTotalCharge);
		  fHorizontalPosition += static_cast<Float_t>(Weight* static_cast<Double_t>(unweightedPosSum.at(i)));
	  }
  }/*else if(size()==2){//For the special Case of size 2 Clusters reconstruction formulas for both Energy and displacement are given in [Bergmann2014]
	  if(fType==1){
		  //First identify if the cluster is left sided or right sided
		  Bool_t IsLeftsided=false; //Leftsided is shorthand for the following Trigger Type composition: 2;(1|3).
		  IsLeftsided = (fEntries.at(0).GetTriggerType()==2 &&
				  	 (fEntries.at(1).GetTriggerType()==1||fEntries.at(1).GetTriggerType()==3));
		  Bool_t IsRightsided=false; //Leftsided is shorthand for the following Trigger Type composition: 2;(1|3).
		  IsRightsided = ((fEntries.at(0).GetTriggerType()==1||fEntries.at(0).GetTriggerType()==3) &&
				  	 fEntries.at(1).GetTriggerType()==2);
		  if (IsLeftsided) {
				Float_t Displacement = (sigma * sigma) / PadWidth
						* log( static_cast<Double_t>(Charges.at(1))
										/ Charges.at(0)) - PadWidth / 2;
				fHorizontalPosition = Displacement + GetHorizontalMessagePosition(fEntries.at(1));

		  } else if(IsRightsided){
				Float_t Displacement = (sigma * sigma) / PadWidth
						* log( static_cast<Double_t>(Charges.at(1))
										/ Charges.at(0)) + PadWidth / 2;
				fHorizontalPosition = Displacement + GetHorizontalMessagePosition(fEntries.at(0));
		  }
	  }else if(fType==2){
		  Float_t a = -0.19;
			Float_t Displacement = a
					* log(static_cast<Double_t>(Charges.at(0)) / Charges.at(1))
					+ PadWidth / 2;
			fHorizontalPosition = Displacement + (GetHorizontalMessagePosition(fEntries.at(0))+ GetHorizontalMessagePosition(fEntries.at(1)))/2;
	  }
  }*/
  else if(size()==3){/*
		fHorizontalPosition = PadWidth / 2.0
				* log(static_cast<Double_t>(Charges.at(2)) / static_cast<Double_t>(Charges.at(0)))
				/ log(static_cast<Double_t>(Charges.at(1)*Charges.at(1))
								/ static_cast<Double_t>(Charges.at(0)
								* Charges.at(2)));
	  fHorizontalPosition += GetHorizontalMessagePosition(fEntries.at(1));
	  //Float_t NormalizationFactor = 1.0/(Charges.at(0)*Charges.at(0)+Charges.at(1)*Charges.at(1));
	  //Float_t LeftDisplacement=
	   * */
	  Double_t a3= TMath::Pi()*PadWidth/TMath::ACosH(0.5*(sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(0)))+sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(2)))));
	  fHorizontalPosition=a3/TMath::Pi()*TMath::ATanH((sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(0)))-sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(2))))/(2.0*TMath::SinH(TMath::Pi()*PadWidth/a3)));
	  fHorizontalPosition += GetHorizontalMessagePosition(fEntries.at(1));
  }
  else if (size()==4){/*
    fHorizontalPosition = PadWidth * 2.0
				* log(static_cast<Double_t>(Charges.at(2)) / static_cast<Double_t>(Charges.at(1)))
				/ (log(static_cast<Double_t>(Charges.at(1))
								/ static_cast<Double_t>(Charges.at(3)))
				+ log(static_cast<Double_t>(Charges.at(2))
								/ static_cast<Double_t>(Charges.at(0))));
	  fHorizontalPosition += static_cast<Double_t>(GetHorizontalMessagePosition(fEntries.at(1))
				+GetHorizontalMessagePosition(fEntries.at(2)))/2.0;*/
	  Double_t a3= TMath::Pi()*PadWidth/TMath::ACosH(0.5*(sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(0)))+sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(2)))));
	  Double_t EstimateLeft  = a3/TMath::Pi()*TMath::ATanH((sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(0)))-sqrt(static_cast<Double_t>(Charges.at(1))/static_cast<Double_t>(Charges.at(2))))/(2.0*TMath::SinH(TMath::Pi()*PadWidth/a3)));
	  	  	   a3= TMath::Pi()*PadWidth/TMath::ACosH(0.5*(sqrt(static_cast<Double_t>(Charges.at(2))/static_cast<Double_t>(Charges.at(1)))+sqrt(static_cast<Double_t>(Charges.at(2))/static_cast<Double_t>(Charges.at(3)))));
	  Double_t EstimateRight = a3/TMath::Pi()*TMath::ATanH((sqrt(static_cast<Double_t>(Charges.at(2))/static_cast<Double_t>(Charges.at(1)))-sqrt(static_cast<Double_t>(Charges.at(2))/static_cast<Double_t>(Charges.at(3))))/(2.0*TMath::SinH(TMath::Pi()*PadWidth/a3)));
	  EstimateRight +=1.0;
	  fHorizontalPosition=0.5*(EstimateLeft+EstimateRight);
	  fHorizontalPosition += GetHorizontalMessagePosition(fEntries.at(1));

  }

  //fHorizontalPosition=fHorizontalPosition/(size())+Offset;
  Veto();
  fParametersCalculated =true;
}



