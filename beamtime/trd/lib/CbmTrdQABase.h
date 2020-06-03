#ifndef CBMTRDQABASE_H
#define CBMTRDQABASE_H

#include "FairTask.h"
#include "CbmTrdTestBeamTools.h"
#include <CbmHistManager.h>
#include "CbmSpadicRawMessage.h"
#include "TClonesArray.h"
#include <algorithm>

class CbmDigiManager;

/** @Class CbmTrdQABase
 ** @author Philipp Munkes <p.munkes@uni-muenster.de>
 ** @date 10th April 2017
 ** @brief Base class for CBM TRD Testbeam analysis.
 **
 ** This class provides infrastructure for the QA Analysis of CBM TRD test beam activity.
 ** It provides three pointers TClonesArray with input data in the form of CbmSpadicRawMessages,
 ** CbmTrdDigis or CbmTrdClusters and initializes them if available.
 ** It also provides an instance of CbmHistManager fHm and stores the Histograms in a folder with the
 ** name of the Class, which has to be explicitly set in the constructor via this->SetName.
 **
 ** Parametrisation is done via an instance of CbmTrdTestBeamTools, of which the appropriate version
 ** has to be set before initialization via CbmTrdTestBeamTools::Instance.
 ** It is available in all derived classes via the pointer fBT.
 **
 ** To implement your own analysis based on this frame work, derive a new class from CbmTrdBase and
 ** implement your own Exec(Option_t*) and CreateHistograms() functions.
 ** **/


class CbmTrdQABase : public FairTask
{
    protected:

    /** Input array from previous already existing data level **/
    TClonesArray* fRaw;
    CbmDigiManager* fDigiMan;
    TClonesArray* fClusters;
    CbmTrdTestBeamTools* fBT;
    CbmHistManager* fHm;
    Int_t fNrTimeslice;

 public:
  /** Constructor with parameters (Optional) **/
     CbmTrdQABase(TString _ClassName):CbmTrdQABase(nullptr, _ClassName){};

    CbmTrdQABase(CbmTrdTestBeamTools*,TString);

    /** Default constructor **/
 CbmTrdQABase():CbmTrdQABase(nullptr,TString("CbmTrdQABase")){};

    /** Destructor **/
    ~CbmTrdQABase();


    /** Initiliazation of task at the beginning of a run **/
    virtual InitStatus Init();

    /** ReInitiliazation of task when the runID changes **/
    virtual InitStatus ReInit();


    /** Executed for each event. **/
    virtual void Exec(Option_t* opt);

    /** Load the parameter container from the runtime database **/
    virtual void SetParContainers();

    /** Finish task called at the end of the run **/
    virtual void Finish();

    /*To ie implemented by inheriting Class*/
    virtual void CreateHistograms();
/*
    * Executed after each event. *
    virtual void FinishEvent();

    * Action at end of run. For this task and all of the subtasks. *
    virtual void FinishTask();
*/
//    inline Int_t GetSpadicID(Int_t sourceA);
    //Functions to analyze Spadic Messages
    TString GetSpadicName(Int_t RobID,Int_t SpadicID,TString RobName="SysCore",Bool_t FullSpadic=true){
      /**
       * Provides a TString containing the name of a Spadic on a specific ROB.
       *
       * Default ROB is the SysCore.
       *
       * @param RobID Numeric id of the Read Out Board
       * @param SpadicID Numeric id of the Read Out Board
       * @param RobName String naming the Type of ROB in use
       * @param FullSpadic Selects wether the string contains 'Spadic' (default case) or 'Half_Spadic'
       * @return TString containing a unique name for the specified Spadic.
       */
      return fBT->GetSpadicName(RobID,SpadicID,RobName,FullSpadic);  
    };
    Int_t GetRobID(CbmSpadicRawMessage* raw){
      return fBT->GetRobID(raw);
    };
    Int_t GetSpadicID(CbmSpadicRawMessage* raw){
      return fBT->GetSpadicID(raw);
    };
    inline TString GetTriggerName(Int_t Triggertype){
      TString triggerTypes[4] = { "Global_ trigger",
				  "Self_triggered",
				  "Neighbor_triggered",
				  "Self_and_neighbor_triggered"};
      if(Triggertype<-1||Triggertype>3){
	LOG(fatal)<< this->GetName() << ": Unknown Triggertype "<< Triggertype;
      }
      if(Triggertype==-1)
	return TString("No_Hit");
      return triggerTypes[Triggertype];
    }
    inline TString GetStopName(Int_t Stoptype){    

      TString stopTypes[6] = {"Normal_end_of_message", 
			  "Channel_buffer_full", 
			  "Ordering_FIFO_full", 
			  "Multi_hit", 
			  "Multi_hit_and_channel_buffer_full", 
			  "Multi_hit_and_ordering_FIFO_full"};
    if(Stoptype<0||Stoptype>5){
	LOG(fatal)<< this->GetName() << ": Unknown Stoptype "<< Stoptype;
      }
      return stopTypes[Stoptype];
    }
    Int_t GetModuleID(CbmSpadicRawMessage* raw){
      return fBT->GetModuleID(raw);}
    ;
    Int_t GetLayerID(CbmSpadicRawMessage* raw){
      return fBT->GetLayerID(raw);
    };
    Int_t GetRowID(CbmSpadicRawMessage* raw){
      return fBT->GetRowID(raw);
    };
    Int_t GetColumnID(CbmSpadicRawMessage* raw){
      return fBT->GetColumnID(raw);
    };
    Int_t GetChannelOnPadPlane(CbmSpadicRawMessage* raw){
      return fBT->GetChannelOnPadPlane(raw);
    };
    Int_t GetMaximumAdc(CbmSpadicRawMessage* raw){
      return fBT->GetMaximumAdc(raw);
    };
    Float_t GetIntegratedCharge(CbmSpadicRawMessage* raw,Double_t Base=0.0){
      return fBT->GetIntegratedCharge(raw,Base);
    };
    


  public:

    CbmTrdQABase(const CbmTrdQABase&);
    CbmTrdQABase operator=(const CbmTrdQABase&);

    ClassDef(CbmTrdQABase,1);
};

#endif
