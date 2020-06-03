#include "CbmTrdTestBeamAnalysis2015SPS.h"
#include "CbmSpadicRawMessage.h"
#include "CbmHistManager.h"
#include "CbmBeamDefaults.h"

#include "FairLogger.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TNtuple.h"
#include "TString.h"
#include "TMath.h"
#include <cmath>
#include <map>
#include <vector>
#include <iostream>
// ----              -------------------------------------------------------
CbmTrdTestBeamAnalysis2015SPS::CbmTrdTestBeamAnalysis2015SPS()
  : FairTask("CbmTrdTestBeamAnalysis2015SPS"),
    fTimeBins(32),
    fRun(0),
    fSpadics(0),
    fRewriteSpadicName(false),
    fPlotSignalShape(true),
    fPlotClusterFinder(true),
    fRawSpadic(NULL),
    fHM(new CbmHistManager()),
    fTimeCounter(0),
    fBaseMethod(kLastTimeBins),
    fBaselineBins(1),
    fBaseline(),
    fNoiseCounter(0),
    fNrTimeSlices(0),
    fTimeMax(0),
    fTimeMin(0)
    //fNT(0x0)
    //*/
{
 LOG(debug) << "Default constructor of CbmTrdTestBeamAnalysis2015SPS";
}
// ----              -------------------------------------------------------
CbmTrdTestBeamAnalysis2015SPS::~CbmTrdTestBeamAnalysis2015SPS()
{
 LOG(debug) << "Destructor of CbmTrdTestBeamAnalysis2015SPS";
}
// ----              -------------------------------------------------------
void CbmTrdTestBeamAnalysis2015SPS::SetParContainers()
{
 LOG(debug) << "SetParContainers of CbmTrdTestBeamAnalysis2015SPS";
}
// ----              -------------------------------------------------------
InitStatus CbmTrdTestBeamAnalysis2015SPS::Init()
{
 LOG(debug) << "Initilization of CbmTrdTestBeamAnalysis2015SPS";
 FairRootManager* ioman = FairRootManager::Instance();
 fRawSpadic = static_cast<TClonesArray*>(ioman->GetObject("SpadicRawMessage"));
 if ( !fRawSpadic ) {
    LOG(fatal) << "No InputDataLevelName array!\n CbmTrdTestBeamAnalysis2015SPS will be inactive";
    return kERROR;
  }
  CreateHistograms();
  return kSUCCESS;
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmTrdTestBeamAnalysis2015SPS::ReInit()
{
  LOG(debug) << "Initilization of CbmTrdTestBeamAnalysis2015SPS";
  return kSUCCESS;
}
// ---- Exec  -------------------------------------------------------
void CbmTrdTestBeamAnalysis2015SPS::Exec(Option_t* /*option*/)
{
  // Analysis based on single SPADIC data streams can be done here!!!


  /*if(fNrTimeSlices==0){
    if(fHM->G1("TsCounter")->GetN()!=0){
      LOG(info) << "Expected empty TsCounter before first TimeSlice, but found " << fHM->G1("TsCounter")->GetN() << " entries.";
    }
  }*/
  
  std::map<TString, std::map<ULong_t, std::vector<CbmSpadicRawMessage*> > > timeBuffer;
  //LOG(error) << "CbmTrdTestBeamAnalysis2015SPS: Run: " << fRun << " / Number of current TimeSlice: " << fNrTimeSlices;
  Int_t nSpadicMessages = fRawSpadic->GetEntriesFast();//SPADIC messages per TimeSlice
  //  Int_t nSpadicMessages0(0),nSpadicMessages1(0); //SPADIC messages per TimeSlice for single SPADICS
  //  Int_t nSpadicMessagesHit0(0), nSpadicMessagesHit1(0), nSpadicMessagesInfo0(0), nSpadicMessagesInfo1(0), nSpadicMessagesEpoch0(0), nSpadicMessagesEpoch1(0),  nSpadicMessagesLost0(0), nSpadicMessagesLost1(0); // SPADIC message types per TimeSlice for single SPADICS 
  Bool_t isHit = false;
  Bool_t isHitAborted = false;
  Bool_t isOverflow = false;
  Bool_t isInfo = false;
  Bool_t isStrange = false;
  Bool_t isEpoch = false;

  Int_t eqID(-1), sourceA(-1), chID(0), triggerType(-1), stopType(-1), infoType(-1), groupId(-1), sysID(-1), spaID(-1);
  ULong_t time = 0;
  Int_t timeStamp(0), epoch(0), superEpoch(0);
  LOG(info) << "nSpadicMessages: " << nSpadicMessages;
  
  TString histName="";
  TString title="";

  TString spadicName = "";
  
  ULong_t lastSpadicTime[3][6] = {{0}}; //[sys][spa]
  Int_t clusterSize[3][6] = {{1}};
  Int_t hitCounter[3][6]={{0}};
  
  fNrTimeSlices++;
  
  for (Int_t iSpadicMessage=0; iSpadicMessage < nSpadicMessages; ++iSpadicMessage) {
    // Get Message
    CbmSpadicRawMessage* raw = static_cast<CbmSpadicRawMessage*>(fRawSpadic->At(iSpadicMessage));
    // Get Message propertys
    isHit = raw->GetHit();
    isHitAborted = raw->GetHitAborted();
    isOverflow = raw->GetOverFlow();
    isInfo = raw->GetInfo();
    isStrange = raw->GetStrange();
    isEpoch = raw->GetEpoch();
    // Seriously guys, a message can only be of one type.
    //if(Int_t(isHit+isInfo+isEpoch+isHitAborted+isOverflow+isStrange)!=1) LOG(error) << "SpadicMessage " << iSpadicMessage << " is classified from CbmSpadicRawMessage to be: HIT " << Int_t(isHit) << " / INFO " << (Int_t)isInfo << " / EPOCH " << (Int_t)isEpoch << " / HITaborted " << (Int_t)isHitAborted << " / OVERFLOW " << (Int_t)isOverflow << " / STRANGE " << (Int_t)isStrange;




    // Get SysCore & Spadic propertys
    eqID = raw->GetEquipmentID();
    sourceA = raw->GetSourceAddress();
    groupId=raw->GetGroupId();
    chID = raw->GetChannelID();
    sysID = GetSysCoreID(eqID);
    spaID = GetSpadicID(sourceA);
    if(spaID%2) chID+=16; // eqID ?
    
    Int_t nrSamples=raw->GetNrSamples();

    time = raw->GetFullTime();
    
    if(fTimeMax<time) fTimeMax = time;
    
    
    timeStamp = raw->GetTime();
    
    epoch = raw->GetEpochMarker();// is copied to each SpadicRawMessage by the unpacker not only epoch messages
    superEpoch = raw->GetSuperEpoch();// is copied to each SpadicRawMessage by the unpacker not only epoch message

    triggerType=raw->GetTriggerType();
    stopType = raw->GetStopType();
    infoType=raw->GetInfoType();
    if (infoType > 6) {
      LOG(error) << " InfoType " << infoType << "is larger 6, set to 7!";
      infoType = 7;
    }
    
    //chID = GetChannelOnPadPlane(chID);// Remapping from ASIC to pad-plane channel numbers.
    TString channelId=Form("_Ch%02d", chID);

    spadicName = GetSpadicName(eqID,sourceA);
    
    //Counter ..
    
    if(isHit) hitCounter[sysID][spaID]++;
    
    
    

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

    //LOG(error) << " time " << time << " / timeStamp " << timeStamp;

    if(spadicName!="") {

      //
      //  DEBUG PLOTS
      //

      fHM->H2("Debug_NrSamples_vs_StopType")->Fill(nrSamples, stopType);
      
      if(isStrange) fHM->H1("Debug_NrSamples_of_Strange_Messages")->Fill(nrSamples, 1);
    
      if(stopType>-1)fHM->H1("Debug_MessageCount")->Fill(TString(spadicName+"_"+stopTypes[stopType]),1);
      if(infoType>-1) fHM->H1("Debug_MessageCount")->Fill(TString(spadicName+"_"+infoTypes[infoType]),1);
      if(isOverflow) fHM->H1("Debug_MessageCount")->Fill(TString(spadicName+"_Overflow"),1);
      if(isHitAborted) fHM->H1("Debug_MessageCount")->Fill(TString(spadicName+"_HitAborted"),1);
      if(isStrange) fHM->H1("Debug_MessageCount")->Fill(TString(spadicName+"_Strange"),1);
  
      if(isHit) {
        histName = spadicName + "_Time_vs_TimeSlice";
        //fHM->H2(histName.Data())->Fill(fNrTimeSlices,time); //timeStamp(0), epoch(0), superEpoch(0);
        
        histName = spadicName + "_TimeStamp_vs_TimeSlice";
        //fHM->H2(histName.Data())->Fill(fNrTimeSlices,timeStamp); //timeStamp(0), epoch(0), superEpoch(0);
        
        histName = spadicName + "_Epoch_vs_TimeSlice";
        //fHM->H2(histName.Data())->Fill(fNrTimeSlices,epoch); //timeStamp(0), epoch(0), superEpoch(0);
        
        histName = spadicName + "_SuperEpoch_vs_TimeSlice";
        //fHM->H2(histName.Data())->Fill(fNrTimeSlices,superEpoch); //timeStamp(0), epoch(0), superEpoch(0);
        

      }
      
      //
      //  PHYSICS PLOTS
      //  
      
      
      
      if(isHit) {
      
        // Heat map
        
        
        Int_t padID = GetChannelOnPadPlane(chID);
        if(padID<16) {
          fHM->H2(TString(spadicName + "_Heatmap").Data())->Fill(padID, 1);
        }else{
          fHM->H2(TString(spadicName + "_Heatmap").Data())->Fill(padID-15, 2);
        }
        
      
        // Cluster finder
        // add raw message to map sorted by timestamps, syscore and spadic
        timeBuffer[TString(spadicName)][time].push_back(raw);
      
        if (time > lastSpadicTime[sysID][spaID]){
          // new hit
          fHM->H1(TString(spadicName + "_SimpleClusterSize").Data())->Fill(clusterSize[sysID][2*spaID]);
          fHM->H1(TString(spadicName + "_SimpleClusterSize").Data())->Fill(clusterSize[sysID][2*spaID+1]);
          clusterSize[sysID][spaID] = 1;
        } else if (time == lastSpadicTime[sysID][spaID]) {
          if (stopType == 0){
            // normal ended hit
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
          }
        } else {
          // something went wrong
          //LOG(error) << fNrTimeSlices << ": time travel occurred !!! (time < lastTime) time: " << time << " lastTime: " << lastSpadicTime[sysID][spaID];
        }
      
      }
      
      lastSpadicTime[sysID][spaID] = time;
      
      
      if(isHit && stopType == 0 && fPlotSignalShape) {
        Double_t adcInt = 0;
        for (Int_t bin = 0; bin < nrSamples; bin++) {
          Int_t adc = raw->GetSamples()[bin];
          adcInt += adc;
          histName = spadicName + "_Signal_Shape" + channelId;
          fHM->H2(histName.Data())->Fill(bin,adc);
          histName = spadicName + "_AvgSignal_Shape" + channelId;
          fHM->P1(histName.Data())->Fill(bin,adc);
          if(triggerType==1) {
            histName = spadicName + "_Signal_Shape_self" + channelId;
            fHM->H2(histName.Data())->Fill(bin,adc);
            histName = spadicName + "_AvgSignal_Shape_self" + channelId;
            fHM->P1(histName.Data())->Fill(bin,adc);
          }else if(triggerType==2){
            histName = spadicName + "_Signal_Shape_neighbour" + channelId;
            fHM->H2(histName.Data())->Fill(bin,adc);
            histName = spadicName + "_AvgSignal_Shape_neighbour" + channelId;
            fHM->P1(histName.Data())->Fill(bin,adc);
          }
        }
      } 
      

    }

  }
  
  //
  //  HitCounter
  //
  
  /*
  hitCounter[0][0][0]=1;
  hitCounter[0][0][1]=2;
  hitCounter[0][1][0]=3;
  hitCounter[0][1][1]=4;
  */
  
  for(Int_t syscore = 0; syscore < 3; ++syscore) {
    for(Int_t spadic = 0; spadic < 3; ++spadic) {
      spadicName = RewriteSpadicName(Form("SysCore%01d_Spadic%01d", syscore, spadic));
      if(spadicName != "") {
        histName = spadicName + "_Hits_per_TimeSlice_Ch00-15";
        //fHM->G1(histName.Data())->SetPoint(fHM->G1(histName.Data())->GetN(),fNrTimeSlices,hitCounter[syscore][2*spadic]);
        histName = spadicName + "_Hits_per_TimeSlice_Ch16-31";
        //fHM->G1(histName.Data())->SetPoint(fHM->G1(histName.Data())->GetN(),fNrTimeSlices,hitCounter[syscore][2*spadic+1]);
        
        //LOG(error) <<  spadicName << " A: " << hitCounter[syscore][2*spadic] << " B: " << hitCounter[syscore][2*spadic+1];
      }
    }
  }
  
  
  
  //
  //  Cluster finder
  //

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
      Int_t clsTriggerInfo[15]; // trigger info
      Double_t clsAdcInt[15];   // adc integral
      Double_t clsAdcAmpl[15];  // max amplitude
      Double_t clsStopType[15]; // stop type
      Bool_t   clsNoise[15];    // noisy channel
      for(Int_t n=0;n<15;n++){
	     clsChID[n]=0;
	     clsTriggerInfo[n]=0;
	     clsAdcInt[n]=0;
	     clsAdcAmpl[n]=0;
	     clsStopType[n]=0;
	     clsNoise[n]=kTRUE;
      }
      //      for(Int_t i=0;i<nrSamples;i++) Integral_cluster[i]=0;


      // loop over all pads in the cluster
      for (Int_t iPad = 0; iPad < Clustersize; iPad++){
	     CbmSpadicRawMessage* raw = static_cast<CbmSpadicRawMessage*>(it2->second.at(iPad));
	     eqID = raw->GetEquipmentID();
	     sourceA = raw->GetSourceAddress();
	     chID = raw->GetChannelID();
	     Int_t nrSamples=raw->GetNrSamples();
	     triggerType=raw->GetTriggerType();
             stopType=raw->GetStopType();
	     infoType=raw->GetInfoType();
	     
	     spadicName = GetSpadicName(eqID,sourceA);
	     
	     if (infoType > 6) infoType = 7;
	     //	Int_t groupId=raw->GetGroupId();
	     //	ULong_t time = raw->GetFullTime();

	     spaID     = GetSpadicID(sourceA);
	     if(spaID%2) chID+=16;
	     TString channelId=Form("_Ch%02d", chID);

	     // fill cluster histograms
	     if(!iPad) {
	       histName = spadicName + "_ClusterSize";
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
	  //          Int_t ampl = max-min;

          // fill noise histograms
          histName = "Noise_AvgBaseline" ;
          //fHM->P1(histName.Data())->Fill(chID,middle);

          histName = "Running_Baseline" ;
          //fHM->H2(histName.Data())->Fill(fTimeCounter,middle,chID);

          histName = "Running_AvgBaseline" ;
          //fHM->P2(histName.Data())->Fill(fTimeCounter,chID,middle);

          histName = "Noise_Shape" ;
          for(Int_t bin = 0; bin < nrSamples; bin++) {
	    //            Int_t adc=raw->GetSamples()[bin];
            //fHM->H2(histName.Data())->Fill(bin,adc);
            //fHM->H2((histName+channelId).Data())->Fill(bin,adc);
          }

          histName = "Noise_AvgAmplitude" ;
          //fHM->P1(histName.Data())->Fill(chID,ampl);
        }

        // fill noise counter
        histName = "NoiseCounter" ;
        //fHM->H1(histName.Data())->Fill(isNoise);

        histName = "NoiseCounter_chID" ;
        //fHM->H2(histName.Data())->Fill(chID,isNoise);

        histName = "NoiseCounter_TimeCounter" ;
        //fHM->H1(histName.Data())->SetBinContent(fTimeCounter,fNoiseCounter);


        // buffer channel/pad information for cluster contributor
        clsChID[iPad]        = chID;
        clsTriggerInfo[iPad] = triggerType;
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
          case kFirstTimeBin:  fBaseline[chID] = raw->GetSamples()[0];
          break;
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
            histName = "Running_AvgBaseline";
            //Int_t bin = fHM->P2(histName.Data())->FindBin(fTimeCounter,chID);
            //fBaseline[chID]=fHM->P2(histName.Data())->GetBinContent(bin);
            break;
          }
        }

	

        // fill baseline histograms
        histName = "BaseLine" ;
        //fHM->H2(histName.Data())->Fill(chID,fBaseline[chID]);

        histName = "AvgBaseline" ;
        //fHM->P1(histName.Data())->Fill(chID,fBaseline[chID]);

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
            histName = "Signal_Shape" + channelId;
            //fHM->H2(histName.Data())->Fill(bin,adc);

            histName = "AvgSignal_Shape" + channelId;
            //fHM->P1(histName.Data())->Fill(bin,adc);

            if(triggerType==1) {
              histName = "Signal_Shape_self" + channelId;
              //fHM->H2(histName.Data())->Fill(bin,adc);

              histName = "AvgSignal_Shape_self" + channelId;
              //fHM->P1(histName.Data())->Fill(bin,adc);
            }
            else if(triggerType==2){
              histName = "Signal_Shape_neighbour" + channelId;
              //fHM->H2(histName.Data())->Fill(bin,adc);

              histName = "AvgSignal_Shape_neighbour" + channelId;
              //fHM->P1(histName.Data())->Fill(bin,adc);
            }

          }//end time bin loop

          // fill max adc values
          histName = "AvgAdcMax" ;
          //fHM->P1(histName.Data())->Fill(chID, maxAdc);

          histName = "AvgAdcMax_self" ;
          //fHM->P1(histName.Data())->Fill(chID, maxAdc_self);

          histName = "AvgAdcMax_neighbour" ;
          //fHM->P1(histName.Data())->Fill(chID, maxAdc_neighbour);

          // fill integrated spectra
          histName = "AdcInt" ;
          //fHM->H1(histName.Data())->Fill(intAdc);

          histName = "AvgAdcInt" ;
          //fHM->P1(histName.Data())->Fill(chID, intAdc);

          histName = "AvgAdcInt_self" ;
          //if(triggerType==1) fHM->P1(histName.Data())->Fill(chID, intAdc_self);

          histName = "AvgAdcInt_neighbour" ;
          //if(triggerType==2) fHM->P1(histName.Data())->Fill(chID, intAdc_neighbour);

          // integrated spectra - channel by channel
          histName = "AdcInt" + channelId;
          //fHM->H1(histName.Data())->Fill(intAdc,1.);

          histName = "AdcInt_self" + channelId;
          //if(triggerType==1) fHM->H1(histName.Data())->Fill(intAdc_self,1.);

          histName = "AdcInt_neighbour" + channelId;
          //if(triggerType==2)fHM->H1(histName.Data())->Fill(intAdc_neighbour,1.);

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
          histName=spadicName + "_Cluster_Types" ;
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
                    histName = spadicName + "_Cluster_Charge";
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
                        Double_t d = (w/2) * (TMath::Log(clsAdcInt[2]/clsAdcInt[0]) / TMath::Log(clsAdcInt[1]*clsAdcInt[1] / (clsAdcInt[2]*clsAdcInt[0])) ) - (w*(i-1));
                        Double_t q = clsAdcInt[i] / clsAdcInt_sum;

                        // fill pad response histograms
                        histName = "PRF_AdcInt" ;
                        //fHM->H2(histName.Data())->Fill(d,q);

                        histName = "AvgPRF_AdcInt" ;
                        //fHM->P1(histName.Data())->Fill(d,q);

                        // PRF using ADC maxima
                        d = (w/2) * (TMath::Log(clsAdcAmpl[2]/clsAdcAmpl[0]) / TMath::Log(clsAdcAmpl[1]*clsAdcAmpl[1] / (clsAdcAmpl[2]*clsAdcAmpl[0])) ) - (w*(i-1));
                        q = clsAdcAmpl[i] / clsAdcAmpl_max;

                        // fill pad response histograms
                        histName = "PRF_AdcAmpl" ;
                        //fHM->H2(histName.Data())->Fill(d,q);

                        histName = "AvgPRF_AdcAmpl" ;
                        //fHM->P1(histName.Data())->Fill(d,q);

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

        //*/
      
      } //end cluster loop

	

	// increment time counter
      fTimeCounter++;
      

    } //end timestamp loop
    
    
    
  } //end Syscore/spadic loop
  
}
// ---- Finish  -------------------------------------------------------
void CbmTrdTestBeamAnalysis2015SPS::Finish()
{

  //Buffer (map) or multi SPADIC data streams based analyis have to be done here!!
  LOG(debug) << "Finish of CbmTrdTestBeamAnalysis2015SPS";
  LOG(info) << "Write histo list to " << FairRootManager::Instance()->GetOutFile()->GetName();
  LOG(error) << "fNrTimeSlices: " << fNrTimeSlices;
  LOG(error) << "fTimeMax:      " << fTimeMax;
  FairRootManager::Instance()->GetOutFile()->cd();
  fHM->WriteToFile();
  //delete c1;
}
// ---- FinishEvent  -------------------------------------------------------
void CbmTrdTestBeamAnalysis2015SPS::FinishEvent()
{
  LOG(debug) << "FinishEvent of CbmTrdTestBeamAnalysis2015SPS";
}
// ----              -------------------------------------------------------
void CbmTrdTestBeamAnalysis2015SPS::CreateHistograms()
{

  // Only Spadic0 and Spadic1 on Syscore0 where used

 
  TString channelName[32] = { "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
			      "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", 
			      "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", 
			      "30", "31"};

  /*
  TString triggerTypes[4] = { "Global trigger",
			      "Self triggered",
			      "Neighbor triggered",
			      "Self and neighbor triggered"};
  */			      
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

  TString spadicName="";
  TString histName="";
  TString runName="";
  if(0 != fRun) runName=Form(" (Run %d)",fRun);
  TString title="";

  fSpadics = 0;
  
  for(Int_t syscore = 0; syscore < 3; ++syscore) {
    for(Int_t spadic = 0; spadic < 3; ++spadic) {
      spadicName = RewriteSpadicName(Form("SysCore%01d_Spadic%01d", syscore, spadic));
      if(spadicName!="") fSpadics++;
    }   
  }

  //
  //  DEBUG PLOTS
  //

  fHM->Add("Debug_NrSamples_vs_StopType", new TH2I("Debug_NrSamples_vs_StopType","Debug_NrSamples_vs_StopType",34,-1.5,32.5,7,-1.5,5.5));
  fHM->H2("Debug_NrSamples_vs_StopType")->GetXaxis()->SetTitle("Nr. of Samples per Message");
  fHM->H2("Debug_NrSamples_vs_StopType")->GetYaxis()->SetTitle("StopType");
  
  fHM->Add("Debug_NrSamples_of_Strange_Messages", new TH1D("Debug_NrSamples_of_Strange_Messages","Debug_NrSamples_of_Strange_Messages",32,0,32));
  fHM->H1("Debug_NrSamples_of_Strange_Messages")->GetXaxis()->SetTitle("Nr. of Samples");
  fHM->H1("Debug_NrSamples_of_Strange_Messages")->GetYaxis()->SetTitle("Count");
  
  fHM->Add("Debug_MessageCount", new TH1D("Debug_MessageCount","Debug_MessageCount",fSpadics*17,0,fSpadics*17));
  Int_t n = 0;
  for(Int_t syscore = 0; syscore < 3; ++syscore) {
    for(Int_t spadic = 0; spadic < 3; ++spadic) {
      spadicName = RewriteSpadicName(Form("SysCore%01d_Spadic%01d", syscore, spadic));
      if(spadicName != "") {
        for(Int_t stopType = 0; stopType < 6; ++stopType) {
          n++;
          fHM->H1("Debug_MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_"+stopTypes[stopType]));
        }
        for(Int_t infoType = 0; infoType < 8; ++infoType) {
          n++;
          fHM->H1("Debug_MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_"+infoTypes[infoType]));
        }
        n++;
        fHM->H1("Debug_MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_Overflow"));
        n++;
        fHM->H1("Debug_MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_HitAborted"));
        n++;
        fHM->H1("Debug_MessageCount")->GetXaxis()->SetBinLabel(n,TString(spadicName+"_Strange"));
      }
    }
  }
  
  for(Int_t syscore = 0; syscore < 3; ++syscore) {
    for(Int_t spadic = 0; spadic < 3; ++spadic) {
      spadicName = RewriteSpadicName(Form("SysCore%01d_Spadic%01d", syscore, spadic));
      if(spadicName != "") {
        histName = spadicName + "_Time_vs_TimeSlice";
        title = histName + runName + ";TimeSlice;Time";
        //fHM->Add(histName.Data(), new TH2F(histName, title, 1000, 0, 5000, 9000, 0, 90000000000));
        histName = spadicName + "_TimeStamp_vs_TimeSlice";
        title = histName + runName + ";TimeSlice;TimeStamp";
        //fHM->Add(histName.Data(), new TH2F(histName, title, 5000, 0, 5000, 4096, 0, 4096));
        
        histName = spadicName + "_Epoch_vs_TimeSlice";
        title = histName + runName + ";TimeSlice;Epoch";
        //fHM->Add(histName.Data(), new TH2F(histName, title, 5000, 0, 5000, 4096, 0, 4096));
        
        histName = spadicName + "_SuperEpoch_vs_TimeSlice";
        title = histName + runName + ";TimeSlice;SuperEpoch";
        //fHM->Add(histName.Data(), new TH2F(histName, title, 5000, 0, 5000, 4096, 0, 4096));
        
        histName = spadicName + "_Hits_per_TimeSlice_Ch00-15";
        title = histName + runName + ";TimeSlice;Hits";
        //fHM->Add(histName.Data(), new TGraph());
        //fHM->G1(histName.Data())->SetTitle(title);
        
        histName = spadicName + "_Hits_per_TimeSlice_Ch16-31"; 
        title = histName + runName + ";TimeSlice;Hits";
        //fHM->Add(histName.Data(), new TGraph());
        //fHM->G1(histName.Data())->SetTitle(title);
       
        histName = spadicName + "_SimpleClusterSize";
        title = histName + runName + ";Cluster Size [Channel] ;Counts";
        fHM->Add(histName.Data(), new TH1F(histName, title, 10, -0.5, 9.5));
        
        histName = spadicName + "_ClusterSize";
        title = histName + runName + ";Cluster Size [Channel] ;Counts";
        fHM->Add(histName.Data(), new TH1F(histName, title, 10, -0.5, 9.5));
      }
    }
  }


  //
  //  PHYSICS PLOTS
  //

  
  
  for(Int_t syscore = 0; syscore < 3; ++syscore) {
    for(Int_t spadic = 0; spadic < 3; ++spadic) {
      spadicName = RewriteSpadicName(Form("SysCore%01d_Spadic%01d", syscore, spadic));
      if(spadicName != "") {
        if(fPlotSignalShape) {
          for(Int_t  channel = 0; channel < fTimeBins; channel++) {
            histName = spadicName + "_Signal_Shape_Ch" + channelName[channel];
            title = histName + runName +  ";Time Bin;ADC value";
            fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 511, -256, 255));

            histName = spadicName + "_AvgSignal_Shape_Ch" + channelName[channel];
            title = histName + runName +  ";Time Bin;<ADC value>";
            fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));

            histName = spadicName + "_Signal_Shape_self_Ch" + channelName[channel];
            title = histName + runName + ";Time Bin;ADC value";
            fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 511, -256, 255));

            histName = spadicName + "_AvgSignal_Shape_self_Ch" + channelName[channel];
            title = histName + runName + ";Time Bin;<ADC value>";
            fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));

            histName = spadicName + "_Signal_Shape_neighbour_Ch" + channelName[channel];
            title = histName + runName + ";Time Bin;ADC value";
            fHM->Add(histName.Data(), new TH2F(histName, title, fTimeBins, 0, fTimeBins, 511, -256, 255));

            histName = spadicName + "_AvgSignal_Shape_neighbour_Ch" + channelName[channel];
            title = histName + runName + ";Time Bin;<ADC value>";
            fHM->Add(histName.Data(), new TProfile(histName, title, fTimeBins, 0, fTimeBins));
          }
        }
        
        // Heatmap
        
        //new TH2I("Debug_NrSamples_vs_StopType","Debug_NrSamples_vs_StopType",34,-1.5,32.5,7,-1.5,5.5));
        
        histName = spadicName + "_Heatmap";
        title = histName + runName + ";X;Y";
        fHM->Add(histName.Data(), new TH2F(histName, title, 16, -0.5, 15.5, 2, 0.5, 2.5));
        
        
        //
        
        histName = spadicName + "_Cluster_Types";
        title = histName + runName +";Types;counts";
        fHM->Add(histName.Data(), new TH1F(histName, title,5,0,5));
        for (Int_t tType=0; tType < 5; tType++)
          fHM->H1(histName.Data())->GetXaxis()->SetBinLabel(tType+1,cluster_names[tType]);
	
        histName = spadicName + "_Cluster_Charge";
        title = histName + runName + ";Integral;Counts";
        fHM->Add(histName.Data(), new TH1F(histName, title,500,0,5000));
      }
    }
  }

  std::cout << ">>> CbmTrdTestBeamAnalysis2015SPS::CreateHistograms: OK..." << std::endl;

}

Int_t CbmTrdTestBeamAnalysis2015SPS::GetSysCoreID(Int_t eqID)
{
  Int_t SysId=-1;
  switch (eqID) {
  case kFlesMuenster:
    SysId = 0;
    break;
  case kFlesFrankfurt:
    SysId = 1;
    break;
  case kFlesBucarest:
    SysId = 2;
    break;
  default:
    LOG(error) << "EquipmentID " << eqID << "not known.";
    break;
  }
  return SysId;
}
// ----              -------------------------------------------------------
Int_t CbmTrdTestBeamAnalysis2015SPS::GetSpadicID(Int_t sourceA)
{
  //TString spadic="";
  Int_t SpaId = -1;
  switch (sourceA) {
  case (SpadicBaseAddress+0):  // first spadic
    SpaId = 0;
    break;
  case (SpadicBaseAddress+1):  // first spadic
    SpaId = 1;
    break;
  case (SpadicBaseAddress+2):  // second spadic
    SpaId = 2;
    break;
  case (SpadicBaseAddress+3):  // second spadic
    SpaId = 3;
    break;
  case (SpadicBaseAddress+4):  // third spadic
    SpaId = 4;
    break;
  case (SpadicBaseAddress+5):  // third spadic
    SpaId = 5;
    break;
  default:
    LOG(error) << "Source Address " << sourceA << "not known.";
    break;
  }
  return SpaId;
}

TString CbmTrdTestBeamAnalysis2015SPS::GetSpadicName(Int_t eqID,Int_t sourceA)
{
  TString spadicName="";
  
  switch (eqID) {
  case kFlesMuenster:
    spadicName="SysCore0_";
    break;
  case kFlesFrankfurt:
    spadicName="SysCore1_";
    break;
  case kFlesBucarest:
    spadicName="SysCore2_";
    break;
  default:
    LOG(error) << "EquipmentID " << eqID << "not known.";
    break;
  }
  
  switch (sourceA) {
  case (SpadicBaseAddress+0):  // first spadic
  case (SpadicBaseAddress+1):  // first spadic
    spadicName+="Spadic0";
    break;
  case (SpadicBaseAddress+2):  // second spadic
  case (SpadicBaseAddress+3):  // second spadic
    spadicName+="Spadic1";
    break;
  case (SpadicBaseAddress+4):  // third spadic
  case (SpadicBaseAddress+5):  // third spadic
    spadicName+="Spadic2";
    break;
  default:
    LOG(error) << "Source Address " << sourceA << "not known.";
    break;
  }
  
  spadicName = RewriteSpadicName(spadicName);
  
  return spadicName;
}

TString CbmTrdTestBeamAnalysis2015SPS::RewriteSpadicName(TString spadicName) 
{
  if(spadicName=="SysCore0_Spadic0") {
    if(fRewriteSpadicName) spadicName="Frankfurt";
  }else if(spadicName=="SysCore0_Spadic1"){
    if(fRewriteSpadicName) spadicName="Muenster";
  }else{
    spadicName="";
  }
  
  return spadicName;
}

// ----              -------------------------------------------------------
  Int_t CbmTrdTestBeamAnalysis2015SPS::GetChannelOnPadPlane(Int_t SpadicChannel)
  {
    /*
    
    |
    |00|02|04
    |
    
    
    
    */
  
    //Int_t channelMapping[32] = {31,15,30,14,29,13,28,12,27,11,26,10,25, 9,24, 8, 23, 7,22, 6,21, 5,20, 4,19, 3,18, 2,17, 1,16, 0};
    Int_t channelMapping[32] = {  0, 16,  1, 17,  2, 18,  3, 19,  4, 20,  5, 21,  6, 22,  7, 23,  8, 24,  9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31};
    if (SpadicChannel < 0 || SpadicChannel > 31){
      LOG(error) << "CbmTrdTestBeamAnalysis2015SPS::GetChannelOnPadPlane ChId " << SpadicChannel;
      return -1;
    } else {
      return channelMapping[SpadicChannel];
    }
  }
// ----              -------------------------------------------------------
