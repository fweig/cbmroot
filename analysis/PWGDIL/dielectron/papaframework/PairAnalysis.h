#ifndef PAIRANALYSIS_H
#define PAIRANALYSIS_H

#include <TNamed.h>
#include <TObjArray.h>
#include <THnBase.h>
#include <TSpline.h>

#include "AnalysisFilter.h"
#include "PairAnalysisHistos.h"
#include "PairAnalysisHF.h"
#include "PairAnalysisCutQA.h"

class PairAnalysisEvent;
class THashList;
class PairAnalysisTrackRotator;
class PairAnalysisPair;
class PairAnalysisSignalMC;
class PairAnalysisMixingHandler;

//________________________________________________________________
class PairAnalysis : public TNamed {

  friend class PairAnalysisMixingHandler; //mixing as friend class
public:
  enum EPairType { kSEPP=0, kSEPM, kSEMM,
		   kMEPP, kMEMP, kMEPM, kMEMM,
		   kSEPMRot, kPairTypes };
  enum ELegType  { kSEP, kSEM, kLegTypes };
  enum CutType { kBothLegs=0, kAnyLeg, kOneLeg }; /// pair prefilter leg cut logic

  PairAnalysis();
  PairAnalysis(const char* name, const char* title);
  virtual ~PairAnalysis();

  void Init();

  void Process(TObjArray *arr);
  Bool_t Process(PairAnalysisEvent *ev1);

  const AnalysisFilter& GetEventFilter() const { return fEventFilter; }
  const AnalysisFilter& GetTrackFilter() const { return fTrackFilter; }
  const AnalysisFilter& GetPairFilter()  const { return fPairFilter;  }

  const AnalysisFilter& GetTrackFilterMC() const { return fTrackFilterMC; }
  const AnalysisFilter& GetPairFilterMC()  const { return fPairFilterMC; }

  AnalysisFilter& GetEventFilter()       { return fEventFilter;       }
  AnalysisFilter& GetTrackFilter()       { return fTrackFilter;       }
  AnalysisFilter& GetPairFilter()        { return fPairFilter;        }
  AnalysisFilter& GetPairPreFilterLegs() { return fPairPreFilterLegs; }
  AnalysisFilter& GetPairPreFilter()     { return fPairPreFilter;     }
  AnalysisFilter& GetFinalTrackFilter()  { return fFinalTrackFilter;  }

  AnalysisFilter& GetTrackFilterMC()       { return fTrackFilterMC;       }
  AnalysisFilter& GetPairFilterMC()       { return fPairFilterMC;       }

  void SetPairPreFilterLegCutType(CutType type)  { fCutType=type; }
  void SetCutQA(Bool_t qa=kTRUE)            { fCutQA=qa; }
  void SetNoPairing(Bool_t noPairing=kTRUE) { fNoPairing=noPairing; }
  Bool_t IsNoPairing()                      { return fNoPairing; }
  void SetProcessLS(Bool_t doLS=kTRUE)      { fProcessLS=doLS; }
  Bool_t DoProcessLS()                      { return fProcessLS; }
  void SetUseKF(Bool_t useKF=kTRUE)         { fUseKF=useKF; }

  const TObjArray* GetTrackArray(Int_t i) const {return (i>=0&&i<4)?&fTracks[i]:0;}
  const TObjArray* GetPairArray(Int_t i)  const {return (i>=0&&i<kPairTypes) ? static_cast<TObjArray*>(fPairCandidates->UncheckedAt(i)) : 0; }

  TObjArray** GetPairArraysPointer()         { return &fPairCandidates; }
  void SetPairArraysPointer( TObjArray *arr) { fPairCandidates=arr; }

  // outputs - hist array
  void SetHistogramArray(PairAnalysisHF * const histoarray) { fHistoArray=histoarray; }
  const TObjArray * GetHistogramArray() const { return fHistoArray?fHistoArray->GetHistArray():0x0; }
  const THashList * GetQAHistList()     const { return fQAmonitor?fQAmonitor->GetQAHistList():0x0; }
  // outputs - histos
  void SetHistogramManager(PairAnalysisHistos * const histos) { fHistos=histos; }
  PairAnalysisHistos* GetHistoManager() const { return fHistos; }
  const THashList * GetHistogramList()  const { return fHistos?fHistos->GetHistogramList():0x0; }
  // outputs - cut detailed histos
  THashList * GetCutStepHistogramList() const { return fCutStepHistos->GetSize()?fCutStepHistos:0x0; }

  Bool_t HasCandidates()   const { return GetPairArray(1)?GetPairArray(1)->GetEntriesFast()>0:0; }
  Bool_t HasCandidatesTR() const { return GetPairArray(7)?GetPairArray(7)->GetEntriesFast()>0:0; }
  Bool_t HasCandidatesLikeSign() const {
    return (GetPairArray(0)&&GetPairArray(2)) ? (GetPairArray(0)->GetEntriesFast()>0 || GetPairArray(2)->GetEntriesFast()>0) : 0;
  }

  // prefilter
  void SetPreFilterUnlikeOnly(Bool_t setValue=kTRUE)  { fPreFilterUnlikeOnly=setValue;};
  void SetPreFilterAllSigns(Bool_t setValue=kTRUE)    { fPreFilterAllSigns=setValue;};

  // background estimator - track rotation
  void SetTrackRotator(PairAnalysisTrackRotator * const rot) { fTrackRotator=rot; }
  PairAnalysisTrackRotator* GetTrackRotator()          const { return fTrackRotator; }
  void SetStoreRotatedPairs(Bool_t storeTR)                  { fStoreRotatedPairs = storeTR; }
  // background estimator - mixed events
  void SetMixingHandler(PairAnalysisMixingHandler *mix)      { fMixing=mix; }
  PairAnalysisMixingHandler* GetMixingHandler()        const { return fMixing; }

  void SetDontClearArrays(Bool_t dontClearArrays=kTRUE)      { fDontClearArrays=dontClearArrays; }
  Bool_t DontClearArrays()                             const { return fDontClearArrays; }

  // mc specific
  void SetHasMC(Bool_t hasMC)                                { fHasMC = hasMC; }
  void AddSignalMC(PairAnalysisSignalMC* signal);
  void SetMotherPdg( Int_t pdgMother )                       { fPdgMother=pdgMother; }
  void SetLegPdg(Int_t pdgLeg1, Int_t pdgLeg2)               { fPdgLeg1=pdgLeg1; fPdgLeg2=pdgLeg2; }
  void SetRefitWithMassAssump(Bool_t setValue=kTRUE)         { fRefitMassAssump=setValue; }
  const TObjArray* GetMCSignals()                      const { return fSignalsMC; }
  Bool_t GetHasMC()                                    const { return fHasMC;  }
  Int_t GetMotherPdg()                                 const { return fPdgMother; }
  Int_t GetLeg1Pdg()                                   const { return fPdgLeg1;   }
  Int_t GetLeg2Pdg()                                   const { return fPdgLeg2;   }

  static const char* TrackClassName(Int_t i)  { return (i>=0&&i<2)?fgkTrackClassNames[i]:""; }
  static const char* PairClassName(Int_t i)   { return (i>=0&&i<8)?fgkPairClassNames[i]:""; }

  Bool_t DoEventProcess()               const { return fEventProcess; }
  void SetEventProcess(Bool_t setValue=kTRUE) { fEventProcess=setValue; }
  void  FillHistogramsFromPairArray(Bool_t pairInfoOnly=kFALSE);

private:

  Bool_t fCutQA = kFALSE;                 // monitor cuts
  PairAnalysisCutQA *fQAmonitor = NULL;   // monitoring of cuts

  AnalysisFilter fEventFilter;            // Event cuts
  AnalysisFilter fTrackFilter;            // leg cuts
  AnalysisFilter fPairPreFilterLegs;      // leg filter before pair prefilter cuts
  AnalysisFilter fPairPreFilter;          // pair prefilter cuts
  AnalysisFilter fFinalTrackFilter;       // Leg filter after the pair prefilter cuts
  AnalysisFilter fPairFilter;             // pair cuts

  AnalysisFilter fTrackFilterMC;          // MCtruth leg cuts
  AnalysisFilter fPairFilterMC;           // MCtruth pair cuts

  Int_t  fPdgMother = 443;                // pdg code of mother tracks
  Int_t  fPdgLeg1   = 11;                 // pdg code leg1
  Int_t  fPdgLeg2   = 11;                 // pdg code leg2
  Bool_t fRefitMassAssump = kFALSE;       // wether refit under pdgleg mass assumption should be done

  TObjArray* fSignalsMC = NULL;           // array of PairAnalysisSignalMC

  CutType fCutType  = kBothLegs;          // type of pairprefilterleg cut logic
  Bool_t fNoPairing = kFALSE;             // if to skip pairing, can be used for track QA only
  Bool_t fProcessLS = kTRUE;              // do the like-sign pairing
  Bool_t fUseKF     = kFALSE;             // use KF particle for pairing

  THashList *fCutStepHistos   = NULL;     // list of histogram managers
  PairAnalysisHF *fHistoArray = NULL;     // matrix of histograms
  PairAnalysisHistos *fHistos = NULL;     // Histogram manager
                                  //  Streaming and merging should be handled
                                  //  by the analysis framework
  TBits *fUsedVars;               // used variables

  TObjArray fTracks[4];           //! Selected track candidates
                                  //  0: SameEvent, positive particles
                                  //  1: SameEvent, negative particles
                                  //  2: MixedEvent, positive particles (not used)
                                  //  3: MixedEvent, negative particles (not used)

  TObjArray *fPairCandidates;     //! Pair candidate arrays
                                  //TODO: better way to store it? TClonesArray?

  PairAnalysisTrackRotator *fTrackRotator = NULL;  //Track rotator
  PairAnalysisMixingHandler *fMixing      = NULL;  // handler for event mixing

  Bool_t fPreFilterUnlikeOnly = kFALSE;  // Apply PreFilter either in +- or to ++/--/+- individually
  Bool_t fPreFilterAllSigns   = kFALSE;  // Apply PreFilter find in  ++/--/+- and remove from all
  Bool_t fHasMC = kFALSE;                // If we run with MC, at the moment only needed in AOD
  Bool_t fStoreRotatedPairs = kFALSE;    // If the rotated pairs should be stored in the pair array
  Bool_t fDontClearArrays   = kFALSE;    // Don't clear the arrays at the end of the Process function, needed for external use of pair and tracks
  Bool_t fEventProcess = kTRUE;          // Process event (or pair array)

  void FillTrackArrays(PairAnalysisEvent * const ev);
  void PairPreFilter(Int_t arr1, Int_t arr2, TObjArray &arrTracks1, TObjArray &arrTracks2);
  void FilterTrackArrays(TObjArray &arrTracks1, TObjArray &arrTracks2);
  void FillPairArrays(Int_t arr1, Int_t arr2);
  void FillPairArrayTR();

  Int_t GetPairIndex(Int_t arr1, Int_t arr2) const;

  void InitPairCandidateArrays();
  void ClearArrays();

  TObjArray* PairArray(Int_t i);

  static const char* fgkTrackClassNames[2];   // Names for track arrays
  static const char* fgkPairClassNames[8];    // Names for pair arrays

  void ProcessMC();

  void  FillHistograms(const PairAnalysisEvent *ev, Bool_t pairInfoOnly=kFALSE);
  Bool_t FillMCHistograms(Int_t label1, Int_t label2, Int_t nSignal);
  void  FillHistogramsMC(const PairAnalysisEvent *ev,  PairAnalysisEvent *ev1);
  void  FillHistogramsPair(PairAnalysisPair *pair,Bool_t fromPreFilter=kFALSE);
  void  FillHistogramsTracks(TObjArray **tracks);
  void  FillHistogramsHits(const PairAnalysisEvent *ev, TBits *fillMC, PairAnalysisTrack *track, Bool_t trackIsLeg, Double_t * values);
  void  FillCutStepHistograms(AnalysisFilter *filter, UInt_t cutmask, PairAnalysisTrack *trk, const Double_t * values);
  void  FillCutStepHistogramsMC(AnalysisFilter *filter, UInt_t cutmask, Int_t label,          const Double_t * values);

  PairAnalysis(const PairAnalysis &c);
  PairAnalysis &operator=(const PairAnalysis &c);

  ClassDef(PairAnalysis,2); //Steering class to process the data
};

inline Int_t PairAnalysis::GetPairIndex(Int_t arr1, Int_t arr2) const
{
  //
  // get pair index
  //
  if(arr1==0 && arr2==arr1) return kSEPP;
  if(arr1==0 && arr2==1)    return kSEPM;
  if(arr1==1 && arr2==arr1) return kSEMM;
  if(arr1==0 && arr2==2)    return kMEPP;
  if(arr1==1 && arr2==2)    return kMEMP;
  if(arr1==0 && arr2==3)    return kMEPM;
  if(arr1==1 && arr2==3)    return kMEMM;
  return kSEPMRot;
}


inline void PairAnalysis::InitPairCandidateArrays()
{
  //
  // initialise all pair candidate arrays
  //
  fPairCandidates->SetOwner();
  for (Int_t i=0;i<8;++i){
    TObjArray *arr=new TObjArray;
    arr->SetName(fgkPairClassNames[i]);
    fPairCandidates->AddAt(arr,i);
    arr->SetOwner();
  }
}

inline TObjArray* PairAnalysis::PairArray(Int_t i)
{
  //
  // for internal use only: unchecked return of pair array for fast access
  //
  return static_cast<TObjArray*>(fPairCandidates->UncheckedAt(i));
}

inline void PairAnalysis::ClearArrays()
{
  //
  // Reset the Arrays
  //
  for (Int_t i=0;i<4;++i){
    fTracks[i].Clear();
  }
  for (Int_t i=0;i<8;++i){
    if (PairArray(i)) PairArray(i)->Delete();
  }
}

#endif
