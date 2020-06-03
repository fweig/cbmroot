///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Authors:
//   Julian Book   <Julian.Book@cern.ch>
/*
  
  Advanced cut class.

  Add cuts that are applied only for certain types of e.g. tracks or under 
  some defined condition:   AddCut(AnalysisCuts *cuts, AnalysisCuts *range)

  Example - apply TRD PID only for track with 3-4 reconstructed hits
  
    // TRD reconstruction cuts
    PairAnalysisVarCuts   *recTRD = new PairAnalysisVarCuts("recTRD","recTRD");
    recTRD->AddCut(PairAnalysisVarManager::kTRDHits,         3.,   4.);

    // TRD Pid - 1-dimensional
    PairAnalysisVarCuts  *pidTRD     = new PairAnalysisVarCuts("pidTRD","pidTRD");
    pidTRD->AddCut(PairAnalysisVarManager::kTRDPidANN,       0.5,   1.5);

    // build PID cut depending on track quality
    PairAnalysisCutCombi *pidTRDavai = new PairAnalysisCutCombi("TRDPidAvai","TRDPidAvai");
    pidTRDavai->AddCut(pidTRD, recTRD);
  

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "PairAnalysisVarManager.h"

#include "PairAnalysisCutCombi.h"

ClassImp(PairAnalysisCutCombi)

PairAnalysisCutCombi::PairAnalysisCutCombi() :
  AnalysisCuts(),
  fNActiveCuts(0),
  fActiveCutsMask(0),
  fSelectedCutsMask(0),
  fCutType(kAll)
{
  //
  // Default Constructor
  //
  for (Int_t i=0; i<kNmaxCuts; ++i){
    fRangeCuts[i]=0x0;
    fCuts[i]  =0x0;
  }

}

//______________________________________________
PairAnalysisCutCombi::PairAnalysisCutCombi(const char* name, const char* title) :
  AnalysisCuts(name, title),
  fNActiveCuts(0),
  fActiveCutsMask(0),
  fSelectedCutsMask(0),
  fCutType(kAll)
{
  //
  // Named Constructor
  //
  for (Int_t i=0; i<kNmaxCuts; ++i){
    fRangeCuts[i]=0x0;
    fCuts[i]  =0x0;
  }
}

//______________________________________________
PairAnalysisCutCombi::~PairAnalysisCutCombi()
{
  //
  // Default Destructor
  //
  for (Int_t i=0; i<kNmaxCuts; ++i){
    if(fRangeCuts[i]) delete fRangeCuts[i];
    if(fCuts[i])   delete fCuts[i];
  }
}

//______________________________________________
void PairAnalysisCutCombi::AddCut(AnalysisCuts *cuts, AnalysisCuts *range)
{
  //
  // add CutCombi cuts
  //
  if(fNActiveCuts==kNmaxCuts) { Warning("AddCut","Too many cuts added!"); return; }

  SETBIT(fActiveCutsMask,fNActiveCuts);
  fRangeCuts[fNActiveCuts] = range;
  fCuts[fNActiveCuts]   = cuts;
  ++fNActiveCuts;

}


//______________________________________________
Bool_t PairAnalysisCutCombi::IsSelected(TObject* track)
{
  //
  // make cut decision
  //

  if (!track) return kFALSE;

  //Fill values
  Double_t *values=PairAnalysisVarManager::GetData();
  PairAnalysisVarManager::Fill(track,values);

  /// selection
  return (IsSelected(values));

}

//________________________________________________________________________
Bool_t PairAnalysisCutCombi::IsSelected(Double_t * const values)
{
  //
  // Make cut decision
  //

  //reset
  fSelectedCutsMask=0;
  SetSelected(kFALSE);

  // loop overe all cuts
  for (Int_t iCut=0; iCut<fNActiveCuts; ++iCut){
    SETBIT(fSelectedCutsMask,iCut);

    if( !fRangeCuts[iCut] || !fCuts[iCut] ) continue;

    // check the range(s) where cuts should be applied
    if ( !fRangeCuts[iCut]->IsSelected(values) ) continue;

    // check decision
    if ( !fCuts[iCut]->IsSelected(values) )
      CLRBIT(fSelectedCutsMask,iCut);

    // cut type and intermediate decision
    if ( fCutType==kAll && !TESTBIT(fSelectedCutsMask,iCut) ) return kFALSE;

  }

  // cut type and final decision
  Bool_t isSelected=(fSelectedCutsMask==fActiveCutsMask);
  if ( fCutType==kAny ) isSelected=(fSelectedCutsMask>0);
  SetSelected(isSelected);
  return isSelected;

}

//________________________________________________________________________
void PairAnalysisCutCombi::Print(const Option_t* /*option*/) const
{
  //
  // Print cuts and the range
  //
  printf("-----------------------------------------------------------------\n");
  printf("cut ranges for '%s'\n",GetTitle());
  if (fCutType==kAll){
    printf("All Cuts have to be fulfilled\n");
  } else {
    printf("Any Cut has to be fulfilled\n");
  }

  for (Int_t iCut=0; iCut<fNActiveCuts; ++iCut){
    fCuts[iCut]->Print();
    printf("For the following conditions:\n");
    fRangeCuts[iCut]->Print();
  }
  printf("-----------------------------------------------------------------\n");

}



