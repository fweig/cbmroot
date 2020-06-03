#include "CbmTrdTestBeamAnalysis.h"
#include <iostream>
#include "CbmSpadicRawMessage.h"
#include "CbmHistManager.h"
#include "CbmBeamDefaults.h"

#include "FairLogger.h"

#include "TDirectory.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TNtuple.h"
#include <TFile.h>

#include "TString.h"
#include "TMath.h"
#include <cmath>
#include <map>
#include <vector>
// ---- Default constructor -------------------------------------------
CbmTrdTestBeamAnalysis::CbmTrdTestBeamAnalysis()
  : FairTask("CbmTrdTestBeamAnalysis"),
    fTimeBins(32),
    fRawSpadic(NULL),
    fHM(new CbmHistManager()),
    fSysCore(-1),
    fSpadic(-1),
    fTimeCounter(0),
    fBaseMethod(kLastTimeBins),
    fBaselineBins(1),
    fBaseline(),
    fNoiseCounter(0),
  fNT(0x0)
{
  for (Int_t i=0; i<32; ++i) {
    fBaseline[i] = 0L; 
  }
  LOG(debug) << "Default Constructor of CbmTrdTestBeamAnalysis";
}

// ---- Destructor ----------------------------------------------------
CbmTrdTestBeamAnalysis::~CbmTrdTestBeamAnalysis()
{
  if(fNT) delete fNT;
  LOG(debug) << "Destructor of CbmTrdTestBeamAnalysis";
}

// ----  Initialisation  ----------------------------------------------
void CbmTrdTestBeamAnalysis::SetParContainers()
{
  LOG(debug) << "SetParContainers of CbmTrdTestBeamAnalysis";
  // Load all necessary parameter containers from the runtime data base
  /*
    FairRunAna* ana = FairRunAna::Instance();
    FairRuntimeDb* rtdb=ana->GetRuntimeDb();

    <CbmTrdTestBeamAnalysisDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmTrdTestBeamAnalysis::Init()
{
  LOG(debug) << "Initilization of CbmTrdTestBeamAnalysis";

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get a pointer to the previous already existing data level
  fRawSpadic = static_cast<TClonesArray*>(ioman->GetObject("SpadicRawMessage"));
  if ( ! fRawSpadic ) {
    LOG(fatal) << "No InputDataLevelName array!\n CbmTrdTestBeamAnalysis will be inactive";
    return kERROR;
  }

  // Do whatever else is needed at the initilization stage
  // Create histograms to be filled
  // initialize variables
  CreateHistograms();

  fNT = new TNtuple("ntuple","tuple","timeCount:channelID:triggerType");
  
  return kSUCCESS;
}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmTrdTestBeamAnalysis::ReInit()
{
  LOG(debug) << "Initilization of CbmTrdTestBeamAnalysis";
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmTrdTestBeamAnalysis::Exec(Option_t* /*option*/)
{
  //Int_t channelMapping[32] = {1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,
  //			      0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};

  // <ASIC ID "Syscore%d_Spadic%d"<Time, SpadicMessage> >
  std::map<TString, std::map<ULong_t, std::vector<CbmSpadicRawMessage*> > > timeBuffer;


  Int_t entries = fRawSpadic->GetEntriesFast();
  Int_t sumTrigger[3][6] = {{0}};
  //  LOG(info) << "******";
  if (entries > 0)
    LOG(info) << "Entries: " << entries;

  // Find info about hitType, stopType and infoType in cbmroot/fles/spadic/message/constants/..
  TString triggerTypes[4] = {"Global trigger",
			     "Self triggered",
			     "Neighbor triggered",
			     "Self and neighbor triggered"};
  TString stopTypes[6] = {"Normal end of message", 
			  "Channel buffer full", 
			  "Ordering FIFO full", 
			  "Multi hit", 
			  "Multi hit and channel buffer full", 
			  "Multi hit and ordering FIFO full"};
  TString infoTypes[8] = {"Channel disabled during message building", 
			  "Next grant timeout", 
			  "Next request timeout", 
			  "New grant but channel empty", 
			  "Corruption in message builder", 
			  "Empty word", 
			  "Epoch out of sync", 
			  "infoType out of array"};

  ULong_t lastSpadicTime[3][6] = {{0}}; //[sys][spa]
  ULong_t firstSpadicTime[3][6] = {{static_cast<ULong_t>(9e+99)}}; //[sys][spa]
  Int_t clusterSize[3][6] = {{1}};
  //  Double_t centreOfGravity =0.;

  // loop over all entries
  for (Int_t i=0; i < entries; ++i) {
    CbmSpadicRawMessage* raw = static_cast<CbmSpadicRawMessage*>(fRawSpadic->At(i));
    Int_t eqID = raw->GetEquipmentID();
    Int_t sourceA = raw->GetSourceAddress();
    Int_t chID = raw->GetChannelID();
    //    Int_t nrSamples=raw->GetNrSamples();
    Int_t triggerType=raw->GetTriggerType();
    Int_t stopType=raw->GetStopType();
    Int_t infoType=raw->GetInfoType();
    if (infoType > 6) infoType = 7;
    Int_t groupId=raw->GetGroupId();
    ULong_t time = raw->GetFullTime();

    // get syscore, spadic and channel
    TString syscore = GetSysCore(eqID);
    Int_t sysID     = GetSysCoreID(eqID);
    TString spadic  = GetSpadic(sourceA);
    Int_t spaID     = GetSpadicID(sourceA);
    if(spaID%2) chID+=16;
    TString channelId=Form("_Ch%02d", chID);

    // use global slection of syscore and/or spadic
    if(fSysCore>-1 && sysID!=fSysCore)                      continue;
    if(fSpadic>-1  && spaID!=fSpadic && spaID !=fSpadic+1)  continue;

    // add raw message to map sorted by timestamps, syscore and spadic
    timeBuffer[TString(syscore+spadic)][time].push_back(raw);

    if (time > lastSpadicTime[sysID][spaID]){
      // ok, next hit
      //LOG(debug) << "ClusterSize: " << clusterSize[sysID][spaID];
      fHM->H1(TString("ClusterSize_" + syscore + spadic).Data())->Fill(clusterSize[sysID][2*spaID]);
      fHM->H1(TString("ClusterSize_" + syscore + spadic).Data())->Fill(clusterSize[sysID][2*spaID+1]);
      clusterSize[sysID][spaID] = 1;
    } else if (time == lastSpadicTime[sysID][spaID]) { // Clusterizer
      // possible FNR trigger
      if (stopType == 0){ // normal ending
	//clusterSize = 1;
	if (triggerType == 0) { // gobal dlm trigger

	} else if (triggerType == 1) { //Self triggered
	  // central pad candidate
	  clusterSize[sysID][spaID]+=1;
	} else if (triggerType == 2) { //Neighbor triggered
	  // outer pad candidate
	  clusterSize[sysID][spaID]+=1;
	} else { //Self and neighbor triggered
	  // central pad candidate
	  clusterSize[sysID][spaID]+=1;
	}
      } else {
	// multihit or currupted
      }
    } else {
      LOG(error) << "SPADIC " << sysID << spaID << " event time " << time << " < last time " << lastSpadicTime[sysID][spaID];
      //Finish();
      //return;
    }

    fHM->H1(TString("TriggerTypes_" + syscore + spadic).Data())->Fill(triggerTypes[triggerType],1);
    fHM->H1(TString("StopTypes_" + syscore + spadic).Data())->Fill(stopTypes[stopType],1);
    if (infoType <= 7)
      fHM->H1(TString("InfoTypes_" + syscore + spadic).Data())->Fill(infoTypes[infoType],1);
    fHM->H1(TString("GroupId_" + syscore + spadic).Data())->Fill(groupId);
    //sumTrigger[sysID][spaID]++;
    if (stopType > 0){ //corrupt or multi message
      TString histName = "ErrorCounter_" + syscore + spadic;
      fHM->H1(histName.Data())->Fill(chID);
    }
    else  {
      //only normal message end
      sumTrigger[sysID][spaID]++;

      TString histName = "CountRate_" + syscore + spadic;
      fHM->H1(histName.Data())->Fill(chID);

      // histName = "Trigger_Heatmap_" + syscore + spadic;
      // if (chID%2 == 0) {
      // fHM->H2(histName.Data())->Fill(chID/2,0);
      // } else {
      // fHM->H2(histName.Data())->Fill((chID-1)/2,1);
      // }
    }
    lastSpadicTime[sysID][spaID] = time;
    if(time<firstSpadicTime[sysID][spaID]) firstSpadicTime[sysID][spaID] = time;
  } //entries pedestal


  for (Int_t sy = 0; sy < 2; sy++){
    for (Int_t sp = 0; sp < 2; sp++){

      // use global selection of syscore and/or spadic
      if(fSysCore>-1 && sy  !=fSysCore) continue;
      if(fSpadic>-1  && sp  !=fSpadic && sp  !=fSpadic+1)  continue;

      TString histName = "TriggerCounter_SysCore" + std::to_string(sy) + "_Spadic" + std::to_string(sp);
      for (Int_t timeSlice = 1; timeSlice <= fHM->H1(histName.Data())->GetNbinsX(); timeSlice++){
	fHM->H1(histName.Data())->SetBinContent(timeSlice,fHM->H1(histName.Data())->GetBinContent(timeSlice+1)); // shift all bin one to the left
      }
      fHM->H1(histName.Data())->SetBinContent(fHM->H1(histName.Data())->GetNbinsX(),sumTrigger[sy][2*sp] + sumTrigger[sy][2*sp+1]);// set only the spa sys combi to new value
      fHM->H1("TriggerSum")->Fill(TString("SysCore" + std::to_string(sy) + "_Spadic" + std::to_string(sp)),sumTrigger[sy][2*sp] + sumTrigger[sy][2*sp+1]);
    }
  }

  //// Clusterisation
  TString histName;
  // complicated loop over sorted map of timestamps
  for(std::map<TString, std::map<ULong_t, std::vector<CbmSpadicRawMessage*> > >::iterator it = timeBuffer.begin() ; it != timeBuffer.end(); it++){

    // complicated loop over sorted map of raw messages
    for (std::map<ULong_t, std::vector<CbmSpadicRawMessage*> > ::iterator it2 = it->second.begin() ; it2 != it->second.end(); it2++) {
      //LOG(info) <<  "ClusterSize:" << Int_t(it2->second.size());

      Int_t Clustersize=Int_t(it2->second.size());
      //      Int_t Integral_cluster[32]; // TODO: needed?
      //      Double_t clsAdcInt_Max=0; //TODO: needed?
      //      Int_t Integral_sum=0; //TODO: obsololete
      Int_t clsType=0; // cluster trigger type identification

      // buffer for the whole clusters
      Int_t clsChID[15];        // channel ID
      //Int_t clsTriggerInfo[15]; // trigger info
      Double_t clsAdcInt[15];   // adc integral
      Double_t clsAdcAmpl[15];  // max amplitude
      Double_t clsStopType[15]; // stop type
      Bool_t   clsNoise[15];    // noisy channel
      for(Int_t n=0;n<15;n++){
	clsChID[n]=0;
	//clsTriggerInfo[n]=0;
	clsAdcInt[n]=0;
	clsAdcAmpl[n]=0;
	clsStopType[n]=0;
	clsNoise[n]=kTRUE;
      }
      //      for(Int_t i=0;i<nrSamples;i++) Integral_cluster[i]=0;


      // loop over all pads in the cluster
      for (Int_t iPad = 0; iPad < Clustersize; iPad++){
	CbmSpadicRawMessage* raw = static_cast<CbmSpadicRawMessage*>(it2->second.at(iPad));
	Int_t eqID = raw->GetEquipmentID();
	Int_t sourceA = raw->GetSourceAddress();
	Int_t chID = raw->GetChannelID();
	Int_t nrSamples=raw->GetNrSamples();
	Int_t triggerType=raw->GetTriggerType();
	Int_t stopType=raw->GetStopType();
	Int_t infoType=raw->GetInfoType();
	if (infoType > 6) infoType = 7;
	//	Int_t groupId=raw->GetGroupId();
	//	ULong_t time = raw->GetFullTime();

	// get syscore, spadic and channel
	TString syscore = GetSysCore(eqID);
	//	Int_t sysID     = GetSysCoreID(eqID);
	TString spadic  = GetSpadic(sourceA);
	Int_t spaID     = GetSpadicID(sourceA);
	if(spaID%2) chID+=16;
	TString channelId=Form("_Ch%02d", chID);

	// fill cluster histograms
	if(!iPad) {
	  histName = "Clustersize_" + syscore + spadic;
	  fHM->H1(histName.Data())->Fill(Clustersize,1.);
	}

	// continue only with normal ending messages
	//	if(stopType) continue; //TODO:what about clustersize

	///// Noise events
	Int_t hlfSamples   = TMath::CeilNint(nrSamples/2);
	Int_t adcTolerance = 2;
	Int_t max1[45]={0};
	Int_t max2[45]={0};
	for(Int_t i=0;i<45;i++) { max1[i]=0; max2[i]=0;}
	for(Int_t bin=0; bin < hlfSamples;bin++){
	  max1[bin]= raw->GetSamples()[bin];
	  max2[bin]= raw->GetSamples()[bin+hlfSamples];
	}
	Bool_t isNoise=(TMath::MaxElement(hlfSamples,max1)-TMath::MaxElement(hlfSamples,max2))<adcTolerance;
	if(isNoise && !stopType) {
	  //	LOG(info) << "Noisy channel found ";
	  fNoiseCounter++;
	  Int_t max     =-300;
	  Int_t min     =-100;
	  Float_t middle=0;
	  Int_t minbin  =0;
	  Int_t maxbin  =0;

	  // find max and min adc values in noise events
	  for(Int_t i=0;i<10;i++){
	    Int_t adc=raw->GetSamples()[nrSamples-1-i];
	    if(adc>max){
	      max    = adc;
	      maxbin = nrSamples-1-i;
	    }
	    //	    adc=raw->GetSamples()[i];
	    if(adc<min){
	      min    = adc;
	      minbin = i;
	    }
	  }
	  // calculate mean noise adc value
	  for(Int_t bin=minbin;bin<=maxbin;bin++)   middle+=raw->GetSamples()[bin];
	  middle /= (maxbin-minbin);

	  // noise amplitude
	  Int_t ampl = max-min;

	  // fill noise histograms
	  histName = "Noise_AvgBaseline_" + syscore + spadic;
	  fHM->P1(histName.Data())->Fill(chID,middle);

	  histName = "Running_Baseline_" + syscore + spadic;
	  fHM->H2(histName.Data())->Fill(fTimeCounter,middle,chID);

	  histName = "Running_AvgBaseline_" + syscore + spadic;
	  fHM->P2(histName.Data())->Fill(fTimeCounter,chID,middle);

	  histName = "Noise_Shape_" + syscore + spadic;
	  for(Int_t bin = 0; bin < nrSamples; bin++) {
	    Int_t adc=raw->GetSamples()[bin];
	    fHM->H2(histName.Data())->Fill(bin,adc);
	    fHM->H2((histName+channelId).Data())->Fill(bin,adc);
	  }

	  histName = "Noise_AvgAmplitude_" + syscore + spadic;
	  fHM->P1(histName.Data())->Fill(chID,ampl);
	}

	// fill noise counter
	histName = "NoiseCounter_" + syscore + spadic;
	fHM->H1(histName.Data())->Fill(isNoise);

	histName = "NoiseCounter_chID_" + syscore + spadic;
	fHM->H2(histName.Data())->Fill(chID,isNoise);

	histName = "NoiseCounter_TimeCounter_"+ syscore + spadic;
	fHM->H1(histName.Data())->SetBinContent(fTimeCounter,fNoiseCounter);


	// buffer channel/pad information for cluster contributor
	clsChID[iPad]        = chID;
	//clsTriggerInfo[iPad] = triggerType;
	clsStopType[iPad]    = stopType;

	// clustertypes TODO: needed?
	switch(triggerType) {
	case 0: clsType-=1000; break; // global
	case 1: clsType+=1;    break; // self
	case 2: clsType+=10;   break; // neighbour
	case 3: clsType+=100;  break; // self+neighbour
	default: Printf("trigger type %d is not defined for channel %d",triggerType,chID);
	}

	// calculate baseline based on method
	switch(fBaseMethod) {
	case kFirstTimeBin:  fBaseline[chID] = raw->GetSamples()[0];           break;
	case kLastTimeBins:
	  {
	    Float_t base=0.;
	    // sum up last time bins and normalize
	    for(Int_t i=0;i<fBaselineBins;i++) base+=raw->GetSamples()[nrSamples-1-i];
	    fBaseline[chID] = static_cast<Int_t>(base/fBaselineBins);
	    break;
	  }
	case kNoise:
	  {
	    histName = "Running_AvgBaseline_" +syscore + spadic;
	    Int_t bin = fHM->P2(histName.Data())->FindBin(fTimeCounter,chID);
	    fBaseline[chID]=fHM->P2(histName.Data())->GetBinContent(bin);
	    break;
	  }
	}

	// fill baseline histograms
	histName = "BaseLine_" + syscore + spadic;
	fHM->H2(histName.Data())->Fill(chID,fBaseline[chID]);

	histName = "AvgBaseline_" + syscore + spadic;
	fHM->P1(histName.Data())->Fill(chID,fBaseline[chID]);

	// cleaned single channel signals
	// these signals could belong to incomplete clusters
	if( !isNoise && fBaseline[chID]!=0 && !stopType) {

	  Int_t maxAdc=0;
	  Int_t maxAdc_self=0;
	  Int_t maxAdc_neighbour=0;
	  Float_t intAdc = 0;
	  Float_t intAdc_self = 0;
	  Float_t intAdc_neighbour = 0;

	  // loop over all timebins
	  for (Int_t bin = 0; bin < nrSamples; bin++) {

	    // TODO: check if really needed
	    if(raw->GetSamples()[0]>=-100 || raw->GetSamples()[1]>=-100) continue;

	    // apply baseline correction
	    Int_t adc = raw->GetSamples()[bin] - fBaseline[chID];

	    // summations
	    intAdc                                   += adc;
	    if(triggerType==1)      intAdc_self      += adc;
	    else if(triggerType==2) intAdc_neighbour += adc;
	    //	    Integral_cluster[bin]                    += adc;

	    // find maximum adc values
	    if(adc>maxAdc)	                       maxAdc           = adc;
	    if(triggerType==1 && adc>maxAdc_self)      maxAdc_self      = adc;
	    if(triggerType==2 && adc>maxAdc_neighbour) maxAdc_neighbour = adc;

	    // signal shape histograms
	    histName = "Signal_Shape_" + syscore + spadic + channelId;
	    fHM->H2(histName.Data())->Fill(bin,adc);

	    histName = "AvgSignal_Shape_" + syscore + spadic + channelId;
	    fHM->P1(histName.Data())->Fill(bin,adc);

	    if(triggerType==1) {
	      histName = "Signal_Shape_self_" + syscore + spadic + channelId;
	      fHM->H2(histName.Data())->Fill(bin,adc);

	      histName = "AvgSignal_Shape_self_" + syscore + spadic + channelId;
	      fHM->P1(histName.Data())->Fill(bin,adc);
	    }
	    else if(triggerType==2){
	      histName = "Signal_Shape_neighbour_" + syscore + spadic + channelId;
	      fHM->H2(histName.Data())->Fill(bin,adc);

	      histName = "AvgSignal_Shape_neighbour_" + syscore + spadic + channelId;
	      fHM->P1(histName.Data())->Fill(bin,adc);
	    }

	  }//end time bin loop

	  // fill max adc values
	  histName = "AvgAdcMax_" + syscore + spadic;
	  fHM->P1(histName.Data())->Fill(chID, maxAdc);

	  histName = "AvgAdcMax_self_" + syscore + spadic;
	  fHM->P1(histName.Data())->Fill(chID, maxAdc_self);

	  histName = "AvgAdcMax_neighbour_" + syscore + spadic;
	  fHM->P1(histName.Data())->Fill(chID, maxAdc_neighbour);

	  // fill integrated spectra
	  histName = "AdcInt_" + syscore + spadic;
	  fHM->H1(histName.Data())->Fill(intAdc);

	  histName = "AvgAdcInt_" + syscore + spadic;
	  fHM->P1(histName.Data())->Fill(chID, intAdc);

	  histName = "AvgAdcInt_self_" + syscore + spadic;
	  if(triggerType==1) fHM->P1(histName.Data())->Fill(chID, intAdc_self);

	  histName = "AvgAdcInt_neighbour_" + syscore + spadic;
	  if(triggerType==2) fHM->P1(histName.Data())->Fill(chID, intAdc_neighbour);

	  // integrated spectra - channel by channel
	  histName = "AdcInt_" + syscore + spadic+ channelId;
	  fHM->H1(histName.Data())->Fill(intAdc,1.);

	  histName = "AdcInt_self_" + syscore + spadic+ channelId;
	  if(triggerType==1) fHM->H1(histName.Data())->Fill(intAdc_self,1.);

	  histName = "AdcInt_neighbour_" + syscore + spadic+ channelId;
	  if(triggerType==2)fHM->H1(histName.Data())->Fill(intAdc_neighbour,1.);

	  // fill buffer
	  clsAdcInt[iPad]  = intAdc;
	  clsAdcAmpl[iPad] = maxAdc;
	  clsNoise[iPad]   = kFALSE;
	}//end good signal selection


	// 'good' cluster selection
	// enter only when last channel is reached
	// make use of buffered values
	if(iPad==Clustersize-1) {

	  // fill cluster types (based on trigger types of its pads)
	  histName="Cluster_Types_" + syscore + spadic;
	  if(     clsType==21)   fHM->H1(histName.Data())->Fill(2);
	  else if(clsType<4)     fHM->H1(histName.Data())->Fill(1);
	  else if(clsType==220)  fHM->H1(histName.Data())->Fill(3);
	  else if(!(clsType%10)) fHM->H1(histName.Data())->Fill(0);
	  else                   fHM->H1(histName.Data())->Fill(4);

	  // only use full size clusters size==3||4
	  if( Clustersize>=3 && Clustersize<=4) {
	    // remove full cluster if a stop type is != 'normal ending'
	    if(!clsStopType[0] && !clsStopType[1] && !clsStopType[2] && !clsStopType[iPad]) {
	      // remove full cluster if a noisy pad is found
	      if(!clsNoise[0] && !clsNoise[1] && !clsNoise[2] && !clsNoise[iPad]) {
		// remove full cluster if channel 0 has fired
		if(clsChID[0]!=0 && clsChID[1]!=0 && clsChID[2]!=0 && clsChID[iPad]!=0) {
		  // remove full cluster if negative adcInt for single pad
		  if(clsAdcInt[0]>0 && clsAdcInt[1]>0 && clsAdcInt[2]>0 && clsAdcInt[iPad]>0) {


		    // cluster charge
		    Int_t clsAdcInt_sum=0;
		    for(Int_t i=0;i<Clustersize;i++) clsAdcInt_sum += clsAdcInt[i];
		    histName = "Cluster_Charge_" + syscore + spadic;
		    fHM->H1(histName.Data())->Fill(clsAdcInt_sum);

		    // cluster max amplitude
		    Int_t clsAdcAmpl_max=0;
		    for(Int_t i=0;i<Clustersize;i++) clsAdcAmpl_max += clsAdcAmpl[i];

		    // Pad Response Function - PRF
		    // TODO: add sorting, currently we assume padIdx=1 = self triggered pad
		    Double_t w = 6.75; // pad width
		    if(Clustersize==3) {
		      for(Int_t i=0;i<Clustersize;i++) {

			// PRF using ADC integrals
			// shift by one pad width in case of neighbours
			Double_t d = (w/2) * (TMath::Log(clsAdcInt[2]/clsAdcInt[0]) /
					      TMath::Log(clsAdcInt[1]*clsAdcInt[1] / (clsAdcInt[2]*clsAdcInt[0])) ) - (w*(i-1));
			Double_t q = clsAdcInt[i] / clsAdcInt_sum;

			// fill pad response histograms
			histName = "PRF_AdcInt_"+ syscore + spadic;
			fHM->H2(histName.Data())->Fill(d,q);

			histName = "AvgPRF_AdcInt_"+ syscore + spadic;
			fHM->P1(histName.Data())->Fill(d,q);

			// PRF using ADC maxima
			d = (w/2) * (TMath::Log(clsAdcAmpl[2]/clsAdcAmpl[0]) /
				     TMath::Log(clsAdcAmpl[1]*clsAdcAmpl[1] / (clsAdcAmpl[2]*clsAdcAmpl[0])) ) - (w*(i-1));
			q = clsAdcAmpl[i] / clsAdcAmpl_max;

			// fill pad response histograms
			histName = "PRF_AdcAmpl_"+ syscore + spadic;
			fHM->H2(histName.Data())->Fill(d,q);

			histName = "AvgPRF_AdcAmpl_"+ syscore + spadic;
			fHM->P1(histName.Data())->Fill(d,q);

		      }
		    } //cluster size

		  } // adc integral negative 
		} // bad channel removal
	      } // noise pad check
	    } //stop type check
	  } // cluster size check
	} // last pad of cluster

	  //	if(iPad==Clustersize-1 && fBaseline[chID]!=0 && chID!=0 && chID!=30){
	  //	  if((cluster_histo==0) && (clsType==21 || clsType==220)){
	  //	    for(Int_t k=0;k< Clustersize;k++){
	  //	      histName = "Clusterization_";
	  // time count: channelID : triggertype
	  //	      fNT->Fill(fTimeCounter,clsChID[k],clsTriggerInfo[k]);
	  //	    }

      } //end cluster loop

	// increment time counter
      fTimeCounter++;

    } //end timestamp loop
  } //end Syscore/spadic loop
}
// ---- Finish --------------------------------------------------------
void CbmTrdTestBeamAnalysis::Finish()
{
  LOG(debug) << "Finish of CbmTrdTestBeamAnalysis";

  TString histName = "Clusterization";
  TString title = histName + ";Time;Channel";
  //Printf("total number of time counts %d",fTimeCounter);
  fHM->Add(histName.Data(), new TH2F(histName, title,fTimeCounter,0,fTimeCounter,fTimeBins,0,fTimeBins));

  LOG(info) << " Write histo list to " << FairRootManager::Instance()->GetOutFile()->GetName();
  FairRootManager::Instance()->GetOutFile()->cd();

  Float_t time=0.;
  Float_t channel=0.;
  Float_t trigger=0.;
  fNT->SetBranchAddress("timeCount",&time);
  fNT->SetBranchAddress("channelID",&channel);
  fNT->SetBranchAddress("triggerType",&trigger);
  for (Int_t i=0;i<Int_t(fNT->GetEntries());i++) {
    fNT->GetEntry(i);
    //    Printf("idx %d:  time %.0f channel %.0f trigger %.0f ",i,time,channel,trigger);
    fHM->H2(histName.Data())->Fill(time, channel, trigger);
  }

  fHM->WriteToFile();
  //    fHM->Write("",TObject::kSingleKey);
}

void CbmTrdTestBeamAnalysis::CreateHistograms()
{
  // Create histograms for 3 Syscores with maximum 3 Spadics
    
  TString syscoreName[3] = { "SysCore0", "SysCore1", "SysCore2" };
  TString spadicName[3]  = { "Spadic0",  "Spadic1",  "Spadic2" };
  TString channelName[32] = { "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
			      "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", 
			      "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", 
			      "30", "31"};
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
  TString infoTypes[8] = {"Channel disabled during message building", 
			  "Next grant timeout", 
			  "Next request timeout", 
			  "New grant but channel empty", 
			  "Corruption in message builder", 
			  "Empty word", 
			  "Epoch out of sync", 
			  "infoType out of array"}; //not official type, just to monitor overflows

  TString cluster_names[5]={"only self",
			    "only FN",
			    "One self and two FN",
			    "Two both and two FN",
			    "Others"};

  TString EntryTypes[2]={"Good", "Noise"};


  TString histName="";
  TString title="";


  fHM->Add("TriggerSum", new TH1F("TriggerSum", "TriggerSum", 9,0,9));
  // histograms for all syscore spadic combinations
  for(Int_t syscore = 0; syscore < 3; ++syscore) {
    for(Int_t spadic = 0; spadic < 3; ++spadic) {

      // use global selection of syscore and/or spadic
      if(fSysCore>-1 && syscore !=fSysCore) continue;
      if(fSpadic>-1  && spadic  !=fSpadic && spadic  !=fSpadic+1)  continue;

      LOG(info) <<  Form("create histograms for syscore %d/%d and spadic %d/%d ",syscore,fSysCore, spadic,fSpadic);

      fHM->H1("TriggerSum")->GetXaxis()->SetBinLabel(3*syscore+spadic+1,TString(syscoreName[syscore]+"_"+spadicName[spadic]));

      histName = "TriggerCounter_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";TimeSlice;Trigger / TimeSlice";
      fHM->Add(histName.Data(), new TH1F(histName, title, 500, 0, 500));

      histName = "CountRate_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + ";Channel;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, fTimeBins, 0, fTimeBins));

      histName = "ErrorCounter_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + ";Channel;Errors";
      fHM->Add(histName.Data(), new TH1F(histName, title, fTimeBins, 0, fTimeBins));

      histName = "TriggerTypes_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + "; ;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, 4, 0, 4));
      for (Int_t tType=0; tType < 4; tType++)
	fHM->H1(histName.Data())->GetXaxis()->SetBinLabel(tType+1,triggerTypes[tType]);

      histName = "StopTypes_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + "; ;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, 6, 0, 6));
      for (Int_t sType=0; sType < 6; sType++)
	fHM->H1(histName.Data())->GetXaxis()->SetBinLabel(sType+1,stopTypes[sType]);

      histName = "InfoTypes_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + "; ;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, 8, 0, 8));
      for (Int_t iType=0; iType < 8; iType++)
	fHM->H1(histName.Data())->GetXaxis()->SetBinLabel(iType+1,infoTypes[iType]);

      histName = "GroupId_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + "; ;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, 2, 0, 2));

      // baseline
      histName = "BaseLine_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + ";Channel;ADC value";
      fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 511, -256, 255));

      histName = "AvgBaseline_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";channel;<ADC>";
      fHM->Add(histName.Data(), new TProfile(histName, title,fTimeBins,0,fTimeBins, "S"));

      // noise
      histName = "Noise_AvgBaseline_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";channel;ADC";
      fHM->Add(histName.Data(), new TProfile(histName, title,fTimeBins,0,fTimeBins, "S"));

      histName = "Running_AvgBaseline_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";time intverall;channel";
      fHM->Add(histName.Data(), new TProfile2D(histName, title,100,0,100*50000,fTimeBins,0,fTimeBins));
      fHM->P2(histName.Data())->SetMinimum(-260.);
      fHM->P2(histName.Data())->SetMaximum(-200.);

      histName = "Running_Baseline_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";TimeCounter;baseline";
      fHM->Add(histName.Data(), new TH2F(histName, title,50000,0,50000,100,-300,-200));

      histName = "Noise_Shape_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";bin;ADC";
      fHM->Add(histName.Data(), new TH2F(histName, title,fTimeBins,0,fTimeBins,60,-260,-200));

      // signals - channel by channel
      for(Int_t  channel = 0; channel < fTimeBins; channel++) {
	histName = "Noise_Shape_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
	title = histName + ";Time Bin;ADC value";
	fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 60, -260, -200));
      }

      histName = "Noise_AvgAmplitude_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";channel;amplitude (ADC)";
      fHM->Add(histName.Data(), new TProfile(histName, title,fTimeBins,0,fTimeBins, "S"));

      histName = "NoiseCounter_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";Types;counts";
      fHM->Add(histName.Data(), new TH1F(histName, title,2,0,2));
      for (Int_t tType=0; tType < 2; tType++)
	fHM->H1(histName.Data())->GetXaxis()->SetBinLabel(tType+1,EntryTypes[tType]);

      histName = "NoiseCounter_chID_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";Channel;Type";
      fHM->Add(histName.Data(), new TH2F(histName, title,fTimeBins,0,fTimeBins,2,0,2));
      for (Int_t tType=0; tType < 2; tType++)
	fHM->H1(histName.Data())->GetYaxis()->SetBinLabel(tType+1,EntryTypes[tType]);

      histName = "NoiseCounter_TimeCounter_"+ syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";Timecounter;Noisecounter";
      fHM->Add(histName.Data(), new TH1F(histName, title,50000,0,50000));

      //


      histName = "ClusterSize_" + syscoreName[syscore] + "_" + spadicName[spadic]; //OK
      title = histName + ";Cluster Size [Channel] ;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, 10, -0.5, 9.5));


      histName = "Integrated_ADC_Spectrum_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Channel;Integr. ADC values in Bin [1,31]";
      fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 2000, -500, 1500));

      //good signals
      histName = "AvgAdcMax_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Channel;MaxValue in ADC";
      fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));

      histName = "AvgAdcMax_self_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Channel;MaxValue in ADC";
      fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));

      histName = "AvgAdcMax_neighbour_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Channel;MaxValue in ADC";
      fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));

      // signals - channel by channel
      for(Int_t  channel = 0; channel < fTimeBins; channel++) {
	histName = "Signal_Shape_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
	title = histName + ";Time Bin;ADC value";
	fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 511, -256, 255));

	histName = "AvgSignal_Shape_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
	title = histName + ";Time Bin;<ADC value>";
	fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));

	histName = "Signal_Shape_self_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
	title = histName + ";Time Bin;ADC value";
	fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 511, -256, 255));

	histName = "AvgSignal_Shape_self_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
	title = histName + ";Time Bin;<ADC value>";
	fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));

	histName = "Signal_Shape_neighbour_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
	title = histName + ";Time Bin;ADC value";
	fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 511, -256, 255));

	histName = "AvgSignal_Shape_neighbour_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
	title = histName + ";Time Bin;<ADC value>";
	fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));
      }

      // signals - ingegrated
      histName = "AdcInt_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";ADCIntegral; Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, 1825, 0, 1825));

      histName = "AvgAdcInt_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Channel;AvgIntegr. ADC values";
      fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins,"S"));

      histName = "AvgAdcInt_self_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Channel;AvgIntegr. ADC values";
      fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins,"S"));

      histName = "AvgAdcInt_neighbour_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Channel;AvgIntegr. ADC values";
      fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins,"S"));

      // integrated signals - channel by channel
      for(Int_t  channel = 0; channel < fTimeBins; channel++) {
	histName = "AdcInt_" + syscoreName[syscore] + "_" + spadicName[spadic]+ "_Ch" + channelName[channel];
	title = histName + ";ADCIntegral; Counts";
	fHM->Add(histName.Data(), new TH1F(histName, title, 1825, 0, 1825));

	histName = "AdcInt_self_" + syscoreName[syscore] + "_" + spadicName[spadic]+ "_Ch" + channelName[channel];
	title = histName + ";ADCIntegral; Counts";
	fHM->Add(histName.Data(), new TH1F(histName, title, 1825, 0, 1825));

	histName = "AdcInt_neighbour_" + syscoreName[syscore] + "_" + spadicName[spadic]+ "_Ch" + channelName[channel];
	title = histName + ";ADCIntegral; Counts";
	fHM->Add(histName.Data(), new TH1F(histName, title, 1825, 0, 1825));
      }

      /// OBSOLETE

      histName = "Trigger_Heatmap_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";sum;Int Counter";
      //fHM->Add(histName.Data(), new TH1F(histName, title, 16, 0, 16, 2, 0, 2));

      // for(Int_t  channel = 0; channel < fTimeBins; channel++) {
      // 	histName = "Pulse_" + syscoreName[syscore] + "_" + spadicName[spadic] + "_Ch" + channelName[channel];
      // 	title = histName + ";Time Bin;ADC value";
      // 	fHM->Add(histName.Data(), new TH1F(histName, title, fTimeBins, 0, fTimeBins));
      // }

      // Cluster
      histName = "Clustersize_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Clustersize;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title, 10,0,10));

      histName = "Cluster_Types_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName +";Types;counts";
      fHM->Add(histName.Data(), new TH1F(histName, title,5,0,5));
      for (Int_t tType=0; tType < 5; tType++)
	fHM->H1(histName.Data())->GetXaxis()->SetBinLabel(tType+1,cluster_names[tType]);

      // histName = "Cluster_Charge_" + syscoreName[syscore] + "_" + spadicName[spadic];
      // title = histName +";timebin;Integral";
      // fHM->Add(histName.Data(), new TH2F(histName, title,fTimeBins,0,fTimeBins,2000,0,2000));

      histName = "Cluster_Charge_" + syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";Integral;Counts";
      fHM->Add(histName.Data(), new TH1F(histName, title,500,0,5000));

      histName = "ClusterIntegral_shape_" + syscoreName[syscore] + "_" + spadicName[spadic]; //??
      title = histName +";timebin;Integral";
      fHM->Add(histName.Data(), new TH2F(histName, title,fTimeBins,0,fTimeBins,600,0,600));

      // PRF
      histName = "PRF_AdcInt_"+ syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";d;Qi/Qi-1*Qi*Qi+1";
      fHM->Add(histName.Data(), new TH2F(histName, title,300,-15,15,100,0,1));

      histName = "AvgPRF_AdcInt_"+ syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";d;<Qi/Qi-1*Qi*Qi+1>";
      fHM->Add(histName.Data(), new TProfile(histName, title,300,-15,15,"S"));

      histName = "PRF_AdcAmpl_"+ syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";d (mm);Qi/Qi-1*Qi*Qi+1";
      fHM->Add(histName.Data(), new TH2F(histName, title,300,-15,15,100,0,1));

      histName = "AvgPRF_AdcAmpl_"+ syscoreName[syscore] + "_" + spadicName[spadic];
      title = histName + ";d (mm);<Qi/Qi-1*Qi*Qi+1>";
      fHM->Add(histName.Data(), new TProfile(histName, title,300,-15,15,"S"));

    }
  }
}

TString CbmTrdTestBeamAnalysis::GetSysCore(Int_t eqID)
{
  TString syscore="";
  //  Int_t SysId=-1;
  switch (eqID) {
  case kFlesMuenster:  // Muenster
    syscore="SysCore0_";
    //SysId = 0;
    break;
  case kFlesFrankfurt: // Frankfurt
    syscore="SysCore1_";
    //SysId = 1;
    break;
  case kFlesBucarest: // Bucarest
    syscore="SysCore2_";
    //SysId = 2;
    break;
  default:
    LOG(error) << "EquipmentID " << eqID << "not known.";
    break;
  }
  return syscore;
}

Int_t   CbmTrdTestBeamAnalysis::GetSysCoreID(Int_t eqID)
{
  //TString syscore="";
  Int_t SysId=-1;
  switch (eqID) {
  case kFlesMuenster:  // Muenster
    //syscore="SysCore0_";
    SysId = 0;
    break;
  case kFlesFrankfurt: // Frankfurt
    //syscore="SysCore1_";
    SysId = 1;
    break;
  case kFlesBucarest: // Bucarest
    //syscore="SysCore2_";
    SysId = 2;
    break;
  default:
    LOG(error) << "EquipmentID " << eqID << "not known.";
    break;
  }
  return SysId;
}

TString CbmTrdTestBeamAnalysis::GetSpadic(Int_t sourceA)
{
  TString spadic="";
  //Int_t SpaId = -1;
  switch (sourceA) {
  case (SpadicBaseAddress+0):  // first spadic
    spadic="Spadic0";
    //SpaId = 0;
    break;
  case (SpadicBaseAddress+1):  // first spadic
    spadic="Spadic0";
    //SpaId = 1;
    break;
  case (SpadicBaseAddress+2):  // second spadic
    spadic="Spadic1";
    //SpaId = 2;
    break;
  case (SpadicBaseAddress+3):  // second spadic
    spadic="Spadic1";
    //SpaId = 3;
    break;
  case (SpadicBaseAddress+4):  // third spadic
    spadic="Spadic2";
    //SpaId = 4;
    break;
  case (SpadicBaseAddress+5):  // third spadic
    spadic="Spadic2";
    //SpaId = 5;
    break;
  default:
    LOG(error) << "Source Address " << sourceA << "not known.";
    break;
  }
  return spadic;
}

Int_t   CbmTrdTestBeamAnalysis::GetSpadicID(Int_t sourceA)
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
    LOG(error) << "Source Address " << sourceA << "not known.";
    break;
  }
  return SpaId;
}
