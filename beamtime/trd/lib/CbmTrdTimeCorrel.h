#ifndef CBMTRDTIMECORREL_H
#define CBMTRDTIMECORREL_H

#include "FairTask.h"

#include "CbmHistManager.h"
#include "TFitResult.h"

#include "TClonesArray.h"
#include "CbmSpadicRawMessage.h"
//#include "CbmBeamDefaults.h"
#include <deque>
#include "TRegexp.h"
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>
#include <string>


typedef std::map<Int_t, std::map<ULong_t, CbmSpadicRawMessage* > > EpochMap;
typedef std::map<Int_t, std::map<Int_t,std::map<ULong_t, Long_t> > > OffsetMap;


class CbmTrdTimeCorrel : public FairTask
{
 public:

  CbmTrdTimeCorrel();
  CbmTrdTimeCorrel(const CbmTrdTimeCorrel&) = delete;
  CbmTrdTimeCorrel& operator=(const CbmTrdTimeCorrel&) = delete;
  ~CbmTrdTimeCorrel();

  virtual InitStatus Init();

  virtual InitStatus ReInit();

  virtual void Exec(Option_t* opt);

  virtual void SetParContainers();

  virtual void Finish();

  virtual void FinishEvent();

  void SetRun(Int_t run)               { fRun = run;}
  void SetRewriteSpadicName(Bool_t rewrite)  { fRewriteSpadicName = rewrite; }

  void ClusterizerSpace();

  void ClusterizerTime();

  void CleanUpBuffers();

 private:
  std::map<TString, std::map<ULong_t, std::map<Int_t, CbmSpadicRawMessage*> > > fMessageBuffer;
  std::deque<CbmSpadicRawMessage*> fLinearHitBuffer;
  Int_t   fSpadics;
  Bool_t  fRewriteSpadicName;

  const Bool_t fActivateClusterizer = true;
  const Bool_t fActivate2DClusterizer=false;
  const Bool_t fBatchAssessment = false;
  const Bool_t fDebugMode = true;
  const Bool_t fDrawSignalDebugHistograms = true;
  const Bool_t fDrawPadResponse = true;
  Bool_t fCalculateBaseline = true;
  const Bool_t fHitFrequency = true;
  const Int_t  fSignalShapeThreshold = -255	;
  std::vector<Int_t> fBaseline;
  Int_t fClusterThreshhold=0;
  const Bool_t fActivateDeltaTAnalysis = false;
  const Bool_t fDrawClustertypes = true;
  TCanvas * fBaselineMon;


  enum kSpadicType{
	  kHalfSpadic,
	  kFullSpadic
  };
  enum kInputType{
	  kRawData,
	  kProcessedData,
	  kDirectOutput
  };


  static Int_t   GetSpadicID(Int_t sourceA);
  static Int_t   GetSyscoreID(Int_t eqID);

  inline TString GetSpadicName(Int_t eqID,Int_t sourceA,kInputType InputType=kRawData,kSpadicType OutputType=kHalfSpadic);
  TString RewriteSpadicName(TString spadicName);

  TString GetStopName(Int_t stopType);

  TClonesArray* fRawSpadic;//input per timeSlice
  TClonesArray* fRawMessages;//output without MS overlaps including all message types without isStrange, isEpoch, isEpochOutOfSynch
  TClonesArray* fDigis;
  TClonesArray* fClusters;

  Int_t fiRawMessage;//counter for fRawMessages
  Int_t fiDigi;//counter for fDigis
  Int_t fiCluster;//counter for fClusters

  CbmHistManager* fHM;

  Int_t fNrTimeSlices;
  
  Int_t fRun;

  std::vector<ULong_t> fLastMessageTime;
  /* Store the last message time.
     Intended usage is for the first (outer) argument to be the SysCoreID 
     and the second (inner) to be the SPADICID of a full SPADIC.
     Third argument is the ChannelID.
     Timestamps are stored for a full spadic.
  */

  std::vector<Int_t> fEpochMarkerArray;
  std::deque<Bool_t> fFirstEpochMarker;

  // Variables for book keeping after a regress of the Epoch counter  
  std::deque<Bool_t> EpochRegressTriggered;
  std::vector<Int_t> EpochRegressOffset;
  std::vector<Int_t> EpochRegressCounter;
  inline void FillBaselineHistogram(CbmSpadicRawMessage*);
  void FillSignalShape(CbmSpadicRawMessage&,std::string Hist="");
  void FillSignalShape(CbmSpadicRawMessage* message){
	  FillSignalShape(*message);
  }
	std::vector<std::pair<TString, TString> > fCorrmaps = { std::make_pair(
			GetSpadicName(0, 2, kDirectOutput, kFullSpadic),
			GetSpadicName(1, 2, kDirectOutput, kFullSpadic)), std::make_pair(
			GetSpadicName(1, 2, kDirectOutput, kFullSpadic),
			GetSpadicName(3, 2, kDirectOutput, kFullSpadic)), std::make_pair(
			GetSpadicName(2, 1, kDirectOutput, kFullSpadic),
			GetSpadicName(3, 1, kDirectOutput, kFullSpadic)), std::make_pair(
			GetSpadicName(0, 2, kDirectOutput, kFullSpadic),
			GetSpadicName(3, 2, kDirectOutput, kFullSpadic))};

  void CreateHistograms();
  void FitBaseline();
  void FitPRF();

  void ReLabelAxis(TAxis* axis, TString type, Bool_t underflow, Bool_t overflow);

  Int_t GetMessageType(CbmSpadicRawMessage* raw);

  static Int_t GetChannelOnPadPlane(Int_t SpadicChannel);
  static Int_t GetChannelonPadPlaneMessage(CbmSpadicRawMessage &message){
	  Int_t SpaID = GetSpadicID(message.GetSourceAddress());
	  Int_t ChID = message.GetChannelID();
	  return GetChannelOnPadPlane(ChID + (SpaID%2)*16);
  };

  // The following block is used to generate TrdDigis
  Int_t GetSectorID(CbmSpadicRawMessage* raw);
  Int_t GetRowID(CbmSpadicRawMessage* raw);
  Int_t GetLayerID(CbmSpadicRawMessage* raw);
  Int_t GetColumnID(CbmSpadicRawMessage* raw);
  Int_t GetModuleID(CbmSpadicRawMessage* raw);
  static Int_t GetMaxADC(CbmSpadicRawMessage&,Bool_t = true,std::vector<Int_t>* =nullptr);
  Int_t GetAvgBaseline(CbmSpadicRawMessage&,Int_t n=1);
  Int_t GetAvgBaseline(CbmSpadicRawMessage* raw){
	  return GetAvgBaseline(*raw);
  };


  OffsetMap timestampOffsets;
  OffsetMap CalculateTimestampOffsets(const EpochMap &epochBuffer);

  Bool_t fGraph;
  Bool_t fOutputCloneArrays;
  
  friend class Cluster;
 public: class Cluster : public TObject
  {
  public:
    Cluster():Cluster(0){};
    Cluster(Int_t initWindowsize):Cluster(nullptr,initWindowsize, 120) {};
    Cluster(std::vector<Int_t>*,Int_t,Int_t);
    ~Cluster();
    Size_t size();
    Int_t Type();
    Int_t fMaxStopType;
    Int_t Windowsize();
    ULong_t GetFulltime();
    Float_t GetHorizontalPosition();
    Bool_t Get2DStatus();
    Int_t GetSpadic();
    Int_t GetRow();
    std::pair<Int_t,Float_t> GetPosition();
    Int_t GetTotalCharge();
    Int_t GetTotalIntegralCharge();
    std::pair<std::vector<CbmSpadicRawMessage>::iterator,std::vector<CbmSpadicRawMessage>::iterator> GetEntries();
    Bool_t AddEntry (CbmSpadicRawMessage);
    Bool_t FillChargeDistribution(TH2*,TH2* CentralMap=nullptr);
  private:
    std::vector<CbmSpadicRawMessage> fEntries;
    Bool_t fParametersCalculated,fIs2D=true;
    Bool_t fPreCalculatedBaseline;
    Int_t fSyscore,fSpadic, fRow, fType, fTotalCharge, fTotalIntegralCharge, fMaxADC, fMaxCharge, fWindowsize, fClusterChargeThreshhold;
    std::vector<Int_t>*  fBaseline;
    ULong_t fFullTime;
    Float_t fHorizontalPosition;
    void CalculateParameters();
    Int_t GetHorizontalMessagePosition(CbmSpadicRawMessage&);
    inline Int_t GetMaxADC(CbmSpadicRawMessage&,Bool_t = true);
    Int_t GetMessageChargeIntegral(CbmSpadicRawMessage& message);
    void Veto();
  };
 private:
  std::deque<Cluster> fClusterBuffer;


//  CbmTrdTimeCorrel(const CbmTrdTimeCorrel&);
//  CbmTrdTimeCorrel operator=(const CbmTrdTimeCorrel&);

  ClassDef(CbmTrdTimeCorrel,1);
};


TString CbmTrdTimeCorrel::GetSpadicName(Int_t eqID,Int_t sourceA,kInputType InputType,kSpadicType OutputType){
	/*	Get a String of the Form "Syscore_0_Spadic_0" describing the specific SPADIC corresponding to the input parameters.
	 *  The Parameter InputType allows either the Equipment ID/Source Address or the final Syscore/Spadic ID to be used.
	 *  	kRawData (default) is the parameter that allows the raw EqID/Source Address to be used, kProcessedData takes Syscore/SpadicID.
	 *  The Parameter OutputType allows adressing either the corresponding FullSpadic, via kFullSpadic (default), or the original HalfSpadic,
	 *  	via kHalfSpadic.
	 * */
  TString spadicName="";
  Int_t SpadicID=0;
  if(InputType==kRawData){
	  spadicName="SysCore_"+std::to_string(GetSyscoreID(eqID))+"_";
	  SpadicID=GetSpadicID(sourceA);
  }
  else {
	  spadicName="SysCore_"+std::to_string((eqID))+"_";
	  SpadicID=(sourceA);
  }

  if(OutputType==kFullSpadic&&InputType!=kDirectOutput){
	  SpadicID/=2;
  }
  ;
  if(OutputType==kFullSpadic){
	  spadicName += "Spadic_";
  }else{
	  spadicName += "Half_Spadic_";

  }
  spadicName+=std::to_string(SpadicID);

  return spadicName;
}


#endif


