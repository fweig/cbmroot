///////////////////////////////////////////////////////////////////////////
//                PairAnalysis Main Analysis class                       //
//                                                                       //
// Authors:
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>
/*

Framework to perform event, single track and pair selections.

Convention for the signs of the track in fTracks:
The names are available via the function TrackClassName(Int_t i)

  0: SE+ (same event +)
  1: SE- (same event -)


Convention for the signs of the pair in fPairCandidates:
The names are available via the function PairClassName(Int_t i)

  0: SE+ SE+  (same event like sign +)
  1: SE+ SE-  (same event unlike sign)
  2: SE- SE-  (same event like sign -)

  3: SE+ ME+  (mixed event like sign +)
  4: SE- ME+  (mixed event unlike sign -+)
  5: SE+ ME-  (mixed event unlike sign +-)
  6: SE- ME-  (mixed event like sign -)

  7: SE+ SE- (same event track rotation)


Some options to configure your analysis instance:
  SetLegPdg(Int_t pdgLeg1, Int_t pdgLeg2)  // define pair production of interest
  SetRefitWithMassAssump(kTRUE)            // whether tracks should be refitted accordingly
  SetNoPairing()                           // look only at track level
  SetProcessLS(kFALSE)                     // switch off like-sign pair calculations
  SetUseKF(kTRUE)                          // use Kalman-Filter mathematics for vertexing


Some options to configure the output:
  SetHistogramManager(PairAnalysisHistos * const histos)  // default histogram manager
  SetHistogramArray(PairAnalysisHF * const histoarray)    // histogram matrix
  SetCutQA()                                              // switch on basic qa histograms


Some options to add background estimators:
  SetMixingHandler(PairAnalysisMixingHandler *mix)        // event mixing
  SetTrackRotator(PairAnalysisTrackRotator * const rot)   // track rotation


*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TRandom3.h>

#include <TString.h>
#include <TList.h>
#include <TMath.h>
#include <TObject.h>
#include <TGrid.h>

#include "FairMCPoint.h"

#include "CbmTrack.h"
#include "CbmRichRing.h"
#include "CbmMatch.h"
#include "CbmHit.h"
#include "CbmMCTrack.h"

#include "PairAnalysisEvent.h"
#include "PairAnalysisTrack.h"

#include "PairAnalysisPair.h"
#include "PairAnalysisPairLV.h"
#include "PairAnalysisPairKF.h"
#include "PairAnalysisHistos.h"
#include "PairAnalysisMC.h"
#include "PairAnalysisVarManager.h"
#include "PairAnalysisTrackRotator.h"
//#include "PairAnalysisDebugTree.h"
#include "PairAnalysisSignalMC.h"
#include "PairAnalysisMixingHandler.h"
#include "PairAnalysisPairLegCuts.h"
//#include "PairAnalysisV0Cuts.h"
#include "PairAnalysisHistos.h"

#include "PairAnalysis.h"

ClassImp(PairAnalysis)

const char* PairAnalysis::fgkTrackClassNames[2] = {
  "+",
  "-"
};

const char* PairAnalysis::fgkPairClassNames[8] = {
  "SE++",
  "SE+-",
  "SE--",
  "ME++",
  "ME-+",
  "ME+-",
  "ME--",
  "TR+-"
};

//________________________________________________________________
PairAnalysis::PairAnalysis() :
  PairAnalysis("PairAnalysis","PairAnalysis")
{
  //
  // Default constructor
  //

}

//________________________________________________________________
PairAnalysis::PairAnalysis(const char* name, const char* title) :
  TNamed(name,title),
  fEventFilter("EventFilter"),
  fTrackFilter("TrackFilter"),
  fPairPreFilterLegs("PairPreFilterLegs"),
  fPairPreFilter("PairPreFilter"),
  fFinalTrackFilter("FinalTrackFilter"),
  fPairFilter("PairFilter"),
  fTrackFilterMC("TrackFilterMC"),
  fPairFilterMC("PairFilterMC"),
  fUsedVars(new TBits(PairAnalysisVarManager::kNMaxValuesMC)),
  fPairCandidates(new TObjArray(8))
{
  //
  // Named constructor
  //
  
}

//________________________________________________________________
PairAnalysis::~PairAnalysis()
{
  //
  // Default destructor
  //
  if (fQAmonitor) delete fQAmonitor;
  if (fHistos) delete fHistos;
  if (fUsedVars) delete fUsedVars;
  if (fPairCandidates && fEventProcess) delete fPairCandidates;
  if (fMixing) delete fMixing;
  if (fSignalsMC) delete fSignalsMC;
  if (fHistoArray) delete fHistoArray;
  if (fCutStepHistos) delete fCutStepHistos;

}

//________________________________________________________________
void PairAnalysis::Init()
{
  //
  // Initialise objects
  //

  //  if(GetHasMC()) PairAnalysisMC::Instance()->SetHasMC(GetHasMC());

  if(fEventProcess) InitPairCandidateArrays();

  // compress the MC signal array
  if(fSignalsMC) fSignalsMC->Compress();

  if (fMixing) fMixing->Init(this);
  if (fHistoArray) {
    //    fHistoArray->SetSignalsMC(fSignalsMC);
    fHistoArray->Init();
  }

  // for internal train wagons
  if(!fEventProcess) {
    PairAnalysisPairLegCuts *trk2leg = new PairAnalysisPairLegCuts("trk2leg","trk2leg");
    // move all track cuts (if any) into pair leg cuts
    TIter listIterator(fTrackFilter.GetCuts());
    while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
      trk2leg->GetLeg1Filter().AddCuts((AnalysisCuts*)thisCut->Clone());
      trk2leg->GetLeg2Filter().AddCuts((AnalysisCuts*)thisCut->Clone());
    }
    TIter listIterator2(fFinalTrackFilter.GetCuts());
    while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator2()) {
      trk2leg->GetLeg1Filter().AddCuts((AnalysisCuts*)thisCut->Clone());
      trk2leg->GetLeg2Filter().AddCuts((AnalysisCuts*)thisCut->Clone());
    }
    // add pair leg cuts to pair filter
    fPairFilter.AddCuts(trk2leg);
  }

  // initialize simple cut qa
  if (fCutQA) {
    fQAmonitor = new PairAnalysisCutQA(Form("QAcuts_%s",GetName()),"QAcuts");
    fQAmonitor->AddTrackFilterMC(&fTrackFilterMC);
    fQAmonitor->AddTrackFilter(&fTrackFilter);
    fQAmonitor->AddPrePairFilter(&fPairPreFilter);
    fQAmonitor->AddTrackFilter2(&fFinalTrackFilter);
    if(!fNoPairing) fQAmonitor->AddPairFilter(&fPairFilter);
    fQAmonitor->AddEventFilter(&fEventFilter);
    fQAmonitor->Init();
  }

  if(fHistos) {
    (*fUsedVars)|= (*fHistos->GetUsedVars());
  }

  // initialize cut step histograms
  if(fTrackFilterMC.GetHistogramList() || fTrackFilter.GetHistogramList() || fFinalTrackFilter.GetHistogramList()) {
    if(fTrackFilterMC.GetHistogramList()) fCutStepHistos = fTrackFilterMC.GetHistogramList();
    if(fCutStepHistos)                    fCutStepHistos->AddAll(fTrackFilter.GetHistogramList());
    else                                  fCutStepHistos = fTrackFilter.GetHistogramList();
    if(fCutStepHistos)                    fCutStepHistos->AddAll(fFinalTrackFilter.GetHistogramList());
    else                                  fCutStepHistos = fFinalTrackFilter.GetHistogramList();
    fCutStepHistos->SetName(Form("CutSteps_%s",GetName()));
  }


}

//________________________________________________________________

void PairAnalysis::Process(TObjArray *arr)
{
  //
  // Process the pair array
  //

  // set pair arrays
  fPairCandidates = arr;

  // fill pair and pair leg histograms
  if(fHistos) FillHistograms(0x0, kTRUE);

  // apply cuts and fill output TODO: OBSOLETE!
  //  if (fHistos) FillHistogramsFromPairArray();

  /// NOTE: never clear arrays !!!!

}

//________________________________________________________________
Bool_t PairAnalysis::Process(PairAnalysisEvent *ev1)
{
  //
  // Process the events
  //

  /// event vertex is needed!
  if (!ev1->GetPrimaryVertex() ){
    Fatal("Process","No vertex found!");
    //Error("Process","No vertex found!");
    return kFALSE;
  }

  /// set reference mass for mt calculation
  PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kMPair, TDatabasePDG::Instance()->GetParticle(fPdgMother)->Mass() );

  /// set event
  PairAnalysisVarManager::SetFillMap(fUsedVars);
  PairAnalysisVarManager::SetEvent(ev1);

  /// set mixing bin to event data
  if (fMixing){
    Int_t bin=fMixing->FindBin(PairAnalysisVarManager::GetData());
    PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kMixingBin,bin);
  }


  //in case we have MC load the MC event and process the MC particles
  // why do not apply the event cuts first ????
  //  if (PairAnalysisMC::Instance()->ConnectMCEvent()){
  if (PairAnalysisMC::Instance()->HasMC()){
    ProcessMC();
  }

  /// if candidate array doesn't exist, create it
  if (!fPairCandidates->UncheckedAt(0)) {
    InitPairCandidateArrays();
  } else {
    ClearArrays();
  }

  /// mask used to require that all cuts are fulfilled
  UInt_t selectedMask=(1<<fEventFilter.GetCuts()->GetEntries())-1;

  /// apply event cuts
  UInt_t cutmask = fEventFilter.IsSelected(ev1);
  if (fCutQA) fQAmonitor->FillAll(ev1);
  if (fCutQA) fQAmonitor->Fill(cutmask,ev1);
  if (ev1 && cutmask!=selectedMask) return kFALSE;

  /// fill track arrays for tracks that pass cuts
  FillTrackArrays(ev1);

  // prefilter track
  if ( (fPreFilterAllSigns||fPreFilterUnlikeOnly) && (fPairPreFilter.GetCuts()->GetEntries()>0) )
    PairPreFilter(0, 1, fTracks[0], fTracks[1]);

  // remove tracks from arrays that DO NOT pass 2nd cut iteration
  FilterTrackArrays(fTracks[0], fTracks[1]);

  // create SE pairs and fill pair candidate arrays
  if (!fNoPairing) {
    for (Int_t itrackArr1=0; itrackArr1<2; ++itrackArr1){
      for (Int_t itrackArr2=itrackArr1; itrackArr2<2; ++itrackArr2){
	if(!fProcessLS && GetPairIndex(itrackArr1,itrackArr2)!=kSEPM) continue;
	FillPairArrays(itrackArr1, itrackArr2);
      }
    }
    // add track rotated pairs
    if (fTrackRotator) {
      FillPairArrayTR();
    }
  }

  //process event mixing
  if (fMixing) {
    fMixing->Fill(ev1,this);
    //     FillHistograms(0x0,kTRUE);
  }

  // fill candidate variables
  Double_t ntracks = fTracks[0].GetEntriesFast() + fTracks[1].GetEntriesFast();
  Double_t npairs  = PairArray(PairAnalysis::kSEPM)->GetEntriesFast();
  PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kTracks, ntracks);
  PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kPairs,  npairs);

  //in case there is a histogram manager, fill the QA histograms
  if (fHistos)               FillHistograms(ev1);
  // fill histo array with event information only
  if (fHistoArray)
    fHistoArray->Fill(0,const_cast<Double_t *>(PairAnalysisVarManager::GetData()),0x0,0x0);

  // clear arrays
  if (!fDontClearArrays) ClearArrays();

  return kTRUE;
}

//________________________________________________________________
void PairAnalysis::ProcessMC()
{
  //
  // Process the MC data
  //
  PairAnalysisMC *papaMC=PairAnalysisMC::Instance();

  // fill mc true and rec event
  //  if (fHistos) FillHistogramsMC(papaMC->GetMCEvent(), ev1);

  // there are mc tracks
  if(!papaMC->GetNMCTracks()) return;

  // signals to be studied
  if(!fSignalsMC) return;
  Int_t nSignals = fSignalsMC->GetEntries();
  if(!nSignals) return;

  //loop over all MC data and Fill the HF, CF containers and histograms if they exist

  Bool_t bFillHF   = kFALSE;
  Bool_t bFillHist = kFALSE;
  for(Int_t isig=0;isig<nSignals;isig++) {
    PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)fSignalsMC->UncheckedAt(isig);
    if(!sigMC->GetFillPureMCStep()) continue;
    TString sigName = sigMC->GetName();
    if(fHistos && !bFillHist) {
      bFillHist |= fHistos->HasHistClass(Form("Pair_%s_MCtruth",sigName.Data()));
      bFillHist |= fHistos->HasHistClass(Form("Track.Leg_%s_MCtruth",sigName.Data()));
      bFillHist |= fHistos->HasHistClass(Form("Track.%s_%s_MCtruth",fgkPairClassNames[1],sigName.Data()));
    }
    if(fHistoArray && !bFillHF) {
      bFillHF |= fHistoArray->HasHistClass(Form("Pair_%s_MCtruth",sigName.Data()));
      bFillHF |= fHistoArray->HasHistClass(Form("Track.Leg_%s_MCtruth",sigName.Data()));
      bFillHF |= fHistoArray->HasHistClass(Form("Track.%s_%s_MCtruth",fgkPairClassNames[1],sigName.Data()));
    }
  }

  // check if there is anything to fill
  if(!bFillHF && !bFillHist) return;

  // initialize 2D arrays of labels for particles from each MC signal
  Int_t** labels1;      // labels for particles satisfying branch 1
  Int_t** labels2;      // labels for particles satisfying branch 2
  Int_t** labels12;     // labels for particles satisfying both branches
  labels1 = new Int_t*[nSignals];
  labels2 = new Int_t*[nSignals];
  labels12 = new Int_t*[nSignals];
  Int_t* indexes1=new Int_t[nSignals];
  Int_t* indexes2=new Int_t[nSignals];
  Int_t* indexes12=new Int_t[nSignals];
  for(Int_t isig=0;isig<nSignals;++isig) {
    *(labels1+isig) = new Int_t[papaMC->GetNMCTracks()];
    *(labels2+isig) = new Int_t[papaMC->GetNMCTracks()];
    *(labels12+isig) = new Int_t[papaMC->GetNMCTracks()];
    for(Int_t ip=0; ip<papaMC->GetNMCTracks();++ip) {
      labels1[isig][ip] = -1;
      labels2[isig][ip] = -1;
      labels12[isig][ip] = -1;
    }
    indexes1[isig]=0;
    indexes2[isig]=0;
    indexes12[isig]=0;
  }

  Bool_t truth1=kFALSE;
  Bool_t truth2=kFALSE;
  // selection of particles
  UInt_t selectedMask=(1<<fTrackFilterMC.GetCuts()->GetEntries())-1;
  // get event data (this contains MC event informations as well)
  Double_t *values=PairAnalysisVarManager::GetData();
  // loop over the MC tracks
  for(Int_t ipart=0; ipart<papaMC->GetNMCTracks(); ++ipart) {

    //get MC particle
    CbmMCTrack *mctrk = papaMC->GetMCTrackFromMCEvent(ipart);

    // fill variables
    PairAnalysisVarManager::Fill(mctrk, values);

    //apply track cuts
    UInt_t cutmask=fTrackFilterMC.IsSelected(values);
    //fill cut QA
    if(fCutQA) fQAmonitor->FillAll(mctrk);
    if(fCutQA) fQAmonitor->Fill(cutmask,mctrk);

    /// fill detailed cut histograms
    if(fTrackFilterMC.GetHistogramList()->GetSize())
      FillCutStepHistogramsMC( &fTrackFilterMC, cutmask, ipart, values);

    // rejection
    if (cutmask!=selectedMask) continue;

    // loop over signals
    for(Int_t isig=0; isig<nSignals; ++isig) {
      PairAnalysisSignalMC *sigMC = (PairAnalysisSignalMC*)fSignalsMC->UncheckedAt(isig);
      // NOTE: Some signals can be satisfied by many particles and this leads to high
      //       computation times (e.g. secondary electrons from the GEANT transport). Be aware of this!!

      // Proceed only if this signal is required in the pure MC step
      if(!sigMC->GetFillPureMCStep()) continue;

      truth1 = papaMC->IsMCTruth(ipart, (PairAnalysisSignalMC*)fSignalsMC->UncheckedAt(isig), 1);
      /// NOTE: single particle signals staisfy only the first branch, truth2=kFALSE in this case
      truth2 = papaMC->IsMCTruth(ipart, (PairAnalysisSignalMC*)fSignalsMC->UncheckedAt(isig), 2);

      // particles satisfying both branches are treated separately to avoid double counting during pairing
      if(truth1 && truth2) {
	labels12[isig][indexes12[isig]] = ipart;
	++indexes12[isig];
      }
      else {
	if(truth1) {
	  labels1[isig][indexes1[isig]] = ipart;
	  ++indexes1[isig];
	}
	if(truth2) {
	  labels2[isig][indexes2[isig]] = ipart;
	  ++indexes2[isig];
	}
      }
    }
  }  // end loop over MC particles

  /// do the pairing and fill the output with pure MC info
  /// selection of MCtruth pairs
  /// NOTE: for MCtruth LV useage should be okay, no need to calculate KF particles
  PairAnalysisPair* 	  pair = new PairAnalysisPairLV();

  selectedMask=(1<<fPairFilterMC.GetCuts()->GetEntries())-1;
  // loop over signals
  for(Int_t isig=0; isig<nSignals; ++isig) {

    // mix the particles which satisfy only one of the signal branches
    for(Int_t i1=0;i1<indexes1[isig];++i1) {
      CbmMCTrack* part1 = papaMC->GetMCTrackFromMCEvent(labels1[isig][i1]);
      Int_t mLabel1 = part1->GetMotherId();

      // (e.g. single electrons only, no pairs)
      if(!indexes2[isig]) FillMCHistograms(labels1[isig][i1], -1, isig);
      //      if(sigMC->IsSingleParticle()) continue;

      /// loop over second branch
      /// NOTE: start from index i1 not 0, to avoid large combinatorics
      ///       this is valid because particles with same mother follow each other,
      ///       in other words they are not distributed over the stack, but the stack is sorted
      for(Int_t i2=i1;i2<indexes2[isig];++i2) {
	//	for(Int_t i2=0;i2<indexes2[isig];++i2) { /// old slower way

	CbmMCTrack* part2 = papaMC->GetMCTrackFromMCEvent(labels2[isig][i2]);
	Int_t mLabel2 = part2->GetMotherId();
	CbmMCTrack*   mother=0x0;

	/// check and get same mother MCtrack
	if(mLabel1==mLabel2) mother = papaMC->GetMCTrackFromMCEvent(mLabel1);

	// selection of MCtruth pairs
	UInt_t cutmask=0;
	if(mother)  cutmask=fPairFilterMC.IsSelected(mother);
	else {
	  /// TODO: do we really want to look at combinatorics at the genrator level?
	  ///       this might saves a lot of cpu time and combinatorics on MCtruth level is useless
	  /// NOTE: at the moment this is avoided by the 'break' after FillMCHistograms below
	  /// continue;
	  pair->SetMCTracks(part1,part2);
	  cutmask=fPairFilterMC.IsSelected(pair);
	}
	//apply MC truth pair cuts
	if (cutmask!=selectedMask) continue;

	if(bFillHF)  fHistoArray->Fill(labels1[isig][i1], labels2[isig][i2], isig);
	if(bFillHist) {
	  if(FillMCHistograms(labels1[isig][i1], labels2[isig][i2], isig)) break;
	}
      }
    }

    // mix the particles which satisfy both branches
    for(Int_t i1=0;i1<indexes12[isig];++i1) {
      for(Int_t i2=0; i2<i1; ++i2) {

	CbmMCTrack* part1 = papaMC->GetMCTrackFromMCEvent(labels12[isig][i1]);
	CbmMCTrack* part2 = papaMC->GetMCTrackFromMCEvent(labels12[isig][i2]);
	Int_t mLabel1 = part1->GetMotherId();
	Int_t mLabel2 = part2->GetMotherId();

	/// check and get same mother MCtrack
	CbmMCTrack*   mother=0x0;
	if(mLabel1==mLabel2) mother = papaMC->GetMCTrackFromMCEvent(mLabel1);

	// selection of MCtruth pairs
	UInt_t cutmask=0;
	if(mother)  cutmask=fPairFilterMC.IsSelected(mother);
	else {
	  /// NOTE: at the moment this is avoided by the 'break' after FillMCHistograms (see above NOTE)
	  pair->SetMCTracks(part1,part2);
	  cutmask=fPairFilterMC.IsSelected(pair);
	}
	//apply MC truth pair cuts
	if (cutmask!=selectedMask) continue;

	if(bFillHF)  fHistoArray->Fill(labels12[isig][i1], labels12[isig][i2], isig);
	if(bFillHist) {
	  if(FillMCHistograms(labels12[isig][i1], labels12[isig][i2], isig)) break;
	}
      }
    }
  }   // end loop over signals

  // release the memory
  delete pair;
  for(Int_t isig=0;isig<nSignals;++isig) {
    delete [] *(labels1+isig);
    delete [] *(labels2+isig);
    delete [] *(labels12+isig);
  }
  delete [] labels1;
  delete [] labels2;
  delete [] labels12;
  delete [] indexes1;
  delete [] indexes2;
  delete [] indexes12;

}

//________________________________________________________________
void PairAnalysis::FillHistogramsTracks(TObjArray **tracks)
{
  //
  // Fill Histogram information for tracks after prefilter
  // ignore mixed events - for prefilter, only single tracks +/- are relevant 
  //
  
  TString  className,className2;
  Double_t *values=PairAnalysisVarManager::GetData();
  PairAnalysisVarManager::SetFillMap(fUsedVars);

  //Fill track information, separately for the track array candidates
  for (Int_t i=0; i<2; ++i){
    className.Form("Pre_%s",fgkTrackClassNames[i]);
    if (!fHistos->GetHistogramList()->FindObject(className.Data())) continue;
    Int_t ntracks=tracks[i]->GetEntriesFast();
    for (Int_t itrack=0; itrack<ntracks; ++itrack){
      PairAnalysisVarManager::Fill(tracks[i]->UncheckedAt(itrack), values);
      fHistos->FillClass(className, values);
    }
  }
}


//________________________________________________________________
void PairAnalysis::FillHistogramsMC(const PairAnalysisEvent* /*ev*/, PairAnalysisEvent* /*ev1*/)
{
  //
  // Fill Histogram information for MCEvents
  //
  return;
  /*
    Double_t *values=PairAnalysisVarManager::GetData();
  PairAnalysisVarManager::SetFillMap(fUsedVars);

  // Fill event information
  PairAnalysisVarManager::Fill(ev1, values);    // ESD/AOD information
  PairAnalysisVarManager::Fill(ev, values);     // MC truth info
  if (fHistos->GetHistogramList()->FindObject("MCEvent"))
    fHistos->FillClass("MCEvent", values);
  */
}


//________________________________________________________________
void PairAnalysis::FillHistograms(const PairAnalysisEvent *ev, Bool_t pairInfoOnly)
{
  //
  // Fill Histogram information for event, pairs, tracks, hits
  //

  TString  className,className2,className3;
  TString  sigName;
  Double_t *values=PairAnalysisVarManager::GetData(); //NEW CHANGED
  PairAnalysisVarManager::SetFillMap(fUsedVars);

  //Fill event information
  if (ev){
    if (fHistos && fHistos->HasHistClass("Event"))
      fHistos      ->FillClass("Event", values);
    if (fHistoArray && fHistoArray->HasHistClass("Event"))
      fHistoArray ->FillClass("Event", values);
  }

  //Fill track information, separately for the track array candidates
  Int_t nsig = (fSignalsMC ? fSignalsMC->GetEntriesFast() : 0);
  TBits trkClassMC(  nsig);
  TBits trkClassMChf(nsig);
  TBits fillMC(  nsig);
  PairAnalysisMC *mc = (nsig ? PairAnalysisMC::Instance() : 0x0);
  PairAnalysisSignalMC *sigMC=0x0;
  if (!pairInfoOnly) {
    className2.Form("Track.%s",fgkPairClassNames[1]);  // unlike sign, SE only
    Bool_t mergedtrkClass  =  fHistos->HasHistClass(className2);
    Bool_t mergedtrkClass2 = (fHistoArray && fHistoArray->HasHistClass(className2));
    // check mc signal filling
    for(Int_t isig=0; isig<nsig; isig++) {
      sigName = className2 + "_" + fSignalsMC->At(isig)->GetName();
      trkClassMC  .SetBitNumber(isig, fHistos->HasHistClass(sigName));
      trkClassMChf.SetBitNumber(isig, fHistoArray && fHistoArray->HasHistClass(sigName));
    }
    // loop over both track arrays
    for (Int_t i=0; i<kLegTypes; ++i) {
      className.Form("Track.%s",fgkTrackClassNames[i]);
      Bool_t trkClass  =  fHistos->HasHistClass(className);
      Bool_t trkClass2 = (fHistoArray && fHistoArray->HasHistClass(className));
      Bool_t fill = (mergedtrkClass || mergedtrkClass2 || trkClass || trkClass2 );
      // skip stuff not to be filled
      if (!fill && !trkClassMC.CountBits() && !trkClassMChf.CountBits()) continue;
      Int_t ntracks=fTracks[i].GetEntriesFast();
      // loop over all tracks
      for (Int_t itrack=0; itrack<ntracks; ++itrack){
	PairAnalysisTrack *track = static_cast<PairAnalysisTrack*>(fTracks[i].UncheckedAt(itrack));
        PairAnalysisVarManager::Fill(track, values);
        if(trkClass)	    fHistos    ->FillClass(className, values);
        if(trkClass2)	    fHistoArray->FillClass(className, values);
        if(mergedtrkClass)  fHistos    ->FillClass(className2, values);
        if(mergedtrkClass2) fHistoArray->FillClass(className2, values); //TODO: check only SE?
	// check and do mc signal filling
	for(Int_t isig=0; isig<nsig; isig++) {
	  if( !trkClassMC.TestBitNumber(isig) && !trkClassMChf.TestBitNumber(isig) ) continue;
	  // test if track does correspond to the signal
	  fillMC.SetBitNumber(isig,
			      mc->IsMCTruth(track, (PairAnalysisSignalMC*)fSignalsMC->At(isig), 1) ||
			      mc->IsMCTruth(track, (PairAnalysisSignalMC*)fSignalsMC->At(isig), 2) );
	  sigName = className2 + "_" + fSignalsMC->At(isig)->GetName();
	  // fill histos
	  if(fillMC.TestBitNumber(isig)) {
	    if(trkClassMC.TestBitNumber(isig))   fHistos     ->FillClass(sigName, values);
	    if(trkClassMChf.TestBitNumber(isig)) fHistoArray ->FillClass(sigName, values);
	  }
	}

	//Fill tracks hit information
	FillHistogramsHits(ev, &fillMC, track, kFALSE, values);

      } // track loop
    } // loop leg type
  } // not pair info only
  
  //Fill Pair information, separately for all pair candidate arrays and the legs
  TBits legClassMC(  nsig);
  TBits legClassMChf(nsig);
  TBits pairClassMC(  nsig);
  TBits pairClassMChf(nsig);
  TObjArray arrLegs(100);
  for (Int_t i=0; i<(kPairTypes-1); ++i){
    className.Form("Pair.%s",fgkPairClassNames[i]);
    className2.Form("Track.Legs.%s",fgkPairClassNames[i]);
    Bool_t pairClass  =  fHistos->HasHistClass(className);
    Bool_t pairClass2 = (fHistoArray && fHistoArray->HasHistClass(className));
    Bool_t legClass   =  fHistos->HasHistClass(className2);
    Bool_t legClass2  = (fHistoArray && fHistoArray->HasHistClass(className2));

    Bool_t legClassHits =  kFALSE;

    // check leg hits and mc signal filling
    if(i==kSEPM) {

      // loop over all detectors and check for hit histos
      for (ECbmModuleId idet=ECbmModuleId::kRef; idet<ECbmModuleId::kNofSystems; ++idet){
	className3+Form("Hit.Legs.")+PairAnalysisHelper::GetDetName(idet);
	legClassHits = fHistos->HasHistClass(className3);
	if(legClassHits) break;
      }


      for(Int_t isig=0; isig<nsig; isig++) {
	sigName =  Form("Pair_%s",fSignalsMC->At(isig)->GetName());
	pairClassMC  .SetBitNumber(isig, fHistos->HasHistClass(sigName));
	pairClassMChf.SetBitNumber(isig, fHistoArray && fHistoArray->HasHistClass(sigName));
	//	Printf("fill %s: %d histos %d",sigName.Data(),pairClassMC.TestBitNumber(isig),fHistos->HasHistClass(sigName));
	sigName =  Form("Track.Legs_%s",fSignalsMC->At(isig)->GetName());
	legClassMC  .SetBitNumber(isig, fHistos->HasHistClass(sigName));
	legClassMChf.SetBitNumber(isig, fHistoArray && fHistoArray->HasHistClass(sigName));

	// check mc signal leg hits filling
	if(!legClassHits) {
	  // loop over all detectors
	  for (ECbmModuleId idet=ECbmModuleId::kRef; idet<ECbmModuleId::kNofSystems; ++idet){
	    className3=Form("Hit.Legs.")+PairAnalysisHelper::GetDetName(idet);
	    sigName = className3 + "_" + fSignalsMC->At(isig)->GetName();
	    legClassHits = fHistos->HasHistClass(className3);
	    if(legClassHits) break; // abort when at least something should be filled
	  }
	}

      }
    }

    Bool_t fill = (pairClass || pairClass2 || legClass || legClass2 || legClassHits);
    if (!fill && !legClassMC.CountBits() && !legClassMChf.CountBits() &&
	!pairClassMC.CountBits() && !pairClassMChf.CountBits()) continue;

    // loop over all pairs
    UInt_t selectedMask=(1<<fPairFilter.GetCuts()->GetEntries())-1; // for internal train wagons only
    Int_t npairs=PairArray(i)->GetEntriesFast();
    for (Int_t ipair=0; ipair<npairs; ++ipair){
      PairAnalysisPair *pair=static_cast<PairAnalysisPair*>(PairArray(i)->UncheckedAt(ipair));

      //fill pair information
      if (pairClass || pairClass2 || pairClassMC.CountBits() || pairClassMChf.CountBits()) {

	// if(i==3)
	//   printf("train: %d \t pair type: %s \t p:%p d1:%p d2:%p \n",!fEventProcess,fgkPairClassNames[i],pair,
	// 	 pair->GetFirstDaughter()->GetGlobalTrack(),pair->GetSecondDaughter()->GetGlobalTrack());
	// in case of internal train wagon do the cut selections
	if(!fEventProcess) {
	  UInt_t cutMask=fPairFilter.IsSelected(pair);
	  // apply cuts
	  if (cutMask!=selectedMask) continue;
	}

	// fill histograms
        PairAnalysisVarManager::Fill(pair, values);
        if(pairClass)  fHistos    ->FillClass(className, values);
        if(pairClass2) fHistoArray->FillClass(className, values);

	// check mc filling
	fillMC.ResetAllBits();
	if(i==kSEPM) {
	  for(Int_t isig=0; isig<nsig; isig++) {
            // next if nothing needs to be filled
	    if(!pairClassMC.TestBitNumber(isig) && !pairClassMChf.TestBitNumber(isig)) continue;
	    sigMC = (PairAnalysisSignalMC*) fSignalsMC->At(isig);
            // next if single particle signal
	    if(sigMC->IsSingleParticle()) continue;
	    Bool_t isMCtruth = mc->IsMCTruth(pair, sigMC);
	    fillMC.SetBitNumber(isig,isMCtruth);
	    if(!isMCtruth) continue;
	    sigName =  Form("Pair_%s",sigMC->GetName());
	    if(pairClassMC.TestBitNumber(isig))   fHistos     ->FillClass(sigName, values);
	    if(pairClassMChf.TestBitNumber(isig)) fHistoArray ->FillClass(sigName, values);
	    // Double_t wght = sigMC->GetWeight(values);
            // if(wght!= values[PairAnalysisVarManager::kWeight])
            //   Warning("FillHistograms","pair weight from tracks (%.3e) not matching MC weight (%.3e) for signal %s",
            //           values[PairAnalysisVarManager::kWeight],wght,sigMC->GetName());
	  }
	}
      }

      //fill leg information, don't fill the information twice
      if (legClass || legClass2 || legClassMC.CountBits() || legClassMChf.CountBits() || legClassHits){
        PairAnalysisTrack *d1=pair->GetFirstDaughter();
        PairAnalysisTrack *d2=pair->GetSecondDaughter();
        if (!arrLegs.FindObject(d1)){
          PairAnalysisVarManager::Fill(d1, values);
          if(legClass)  fHistos    ->FillClass(className2, values);
          if(legClass2) fHistoArray->FillClass(className2, values);
	  // mc signal filling
	  if(i==kSEPM) {
	    for(Int_t isig=0; isig<nsig; isig++) {
	      if(!fillMC.TestBitNumber(isig)) continue;
	      sigMC = (PairAnalysisSignalMC*) fSignalsMC->At(isig);
	      sigName = Form("Track.Legs_%s",sigMC->GetName());
	      if(legClassMC.TestBitNumber(isig))   fHistos     ->FillClass(sigName, values);
	      if(legClassMChf.TestBitNumber(isig)) fHistoArray ->FillClass(sigName, values);
	    }
	  }

	  //Fill leg hits information
	  if(legClassHits) FillHistogramsHits(ev, &fillMC, d1, kTRUE, values);

	  arrLegs.Add(d1);
        }
        if (!arrLegs.FindObject(d2)){
          PairAnalysisVarManager::Fill(d2, values);
          if(legClass)  fHistos    ->FillClass(className2, values);
          if(legClass2) fHistoArray->FillClass(className2, values);
	  // mc signal filling
	  if(i==kSEPM) {
	    for(Int_t isig=0; isig<nsig; isig++) {
	      if(!fillMC.TestBitNumber(isig)) continue;
	      sigMC = (PairAnalysisSignalMC*) fSignalsMC->At(isig);
	      sigName = Form("Track.Legs_%s",sigMC->GetName());
	      if(legClassMC.TestBitNumber(isig))   fHistos     ->FillClass(sigName, values);
	      if(legClassMChf.TestBitNumber(isig)) fHistoArray ->FillClass(sigName, values);
	    }
	  }

	  //Fill leg hits information
	  if(legClassHits) FillHistogramsHits(ev, &fillMC, d2, kTRUE, values);

          arrLegs.Add(d2);
        }
      }
    }
    if (legClass || legClass2) arrLegs.Clear();
  }

}
//________________________________________________________________
void PairAnalysis::FillHistogramsPair(PairAnalysisPair *pair,Bool_t fromPreFilter/*=kFALSE*/)
{
  //
  // Fill Histogram information for pairs and the track in the pair
  // NOTE: in this funtion the leg information may be filled multiple
  //       times. This funtion is used in the track rotation pairing
  //       and those legs are not saved!
  //
  TString  className,className2;
  Double_t *values=PairAnalysisVarManager::GetData();
  PairAnalysisVarManager::SetFillMap(fUsedVars);

  //Fill Pair information, separately for all pair candidate arrays and the legs
  TObjArray arrLegs(100);
  const Int_t type=pair->GetType();
  if (fromPreFilter) {
    className.Form("RejPair.%s",fgkPairClassNames[type]);
    className2.Form("RejTrack.%s",fgkPairClassNames[type]);
  } else {
    className.Form("Pair.%s",fgkPairClassNames[type]);
    className2.Form("Track.Legs.%s",fgkPairClassNames[type]);
  }

  Bool_t pairClass  =  fHistos->HasHistClass(className);
  Bool_t pairClass2 = (fHistoArray && fHistoArray->HasHistClass(className));
  Bool_t legClass   =  fHistos->HasHistClass(className2);
  Bool_t legClass2  = (fHistoArray &&  fHistoArray->HasHistClass(className2));

  //fill pair information
  if (pairClass || pairClass2){
    PairAnalysisVarManager::Fill(pair, values);
    if(pairClass)   fHistos     ->FillClass(className, values);
    if(pairClass2)  fHistoArray ->FillClass(className, values);
  }

  if (legClass || legClass2){
    PairAnalysisTrack *d1=pair->GetFirstDaughter();
    PairAnalysisVarManager::Fill(d1, values);
    if(legClass)    fHistos     ->FillClass(className2, values);
    if(legClass2)   fHistoArray ->FillClass(className2, values);

    PairAnalysisTrack *d2=pair->GetSecondDaughter();
    PairAnalysisVarManager::Fill(d2, values);
    if(legClass)    fHistos     ->FillClass(className2, values);
    if(legClass2)   fHistoArray ->FillClass(className2, values);
  }

}

//________________________________________________________________
void PairAnalysis::FillTrackArrays(PairAnalysisEvent * const ev)
{
  //
  // select tracks and fill track candidate arrays
  // use track arrays 0 and 1
  //

  /// mc instance
  PairAnalysisMC* papaMC=0x0;
  if(fHasMC && fSignalsMC) papaMC = PairAnalysisMC::Instance();

  // get event data
  Double_t *values=PairAnalysisVarManager::GetData();
  
  Int_t ntracks=ev->GetNumberOfTracks();
  UInt_t selectedMask=(1<<fTrackFilter.GetCuts()->GetEntries())-1;
  for (Int_t itrack=0; itrack<ntracks; ++itrack){

    //get particle
    PairAnalysisTrack *particle=ev->GetTrack(itrack);

    // adapt mass hypothesis accordingly (they were initialized with PDG11)
    particle->SetMassHypo(fPdgLeg1,fPdgLeg2, fRefitMassAssump);

    // fill variables
    PairAnalysisVarManager::Fill(particle, values);

    //apply track cuts
    UInt_t cutmask=fTrackFilter.IsSelected(values);
    //UInt_t cutmask=fTrackFilter.IsSelected(particle);
    //fill cut QA
    if(fCutQA) fQAmonitor->FillAll(particle);
    if(fCutQA) fQAmonitor->Fill(cutmask,particle);

    // if raw spectra before any cuts are requested then fill
    if(fHistos && fHistos->HasHistClass("Track.noCuts")) {
      PairAnalysisVarManager::SetFillMap(fUsedVars);
      //      PairAnalysisVarManager::Fill(particle, values);
      fHistos    ->FillClass("Track.noCuts", values);
    }

    /// fill detailed cut histograms
    if(fTrackFilter.GetHistogramList()->GetSize())
      FillCutStepHistograms( &fTrackFilter, cutmask, particle, values);

    // rejection
    if (cutmask!=selectedMask) continue;

    // store signal weights in the tracks - ATTENTION later signals should be more specific
    if(fHasMC && fSignalsMC) {
      // printf("particle %p: pdg: %.0f \t mother: %.0f grand: %.0f \n", particle, values[PairAnalysisVarManager::kPdgCode],
      // 	     values[PairAnalysisVarManager::kPdgCodeMother], values[PairAnalysisVarManager::kPdgCodeGrandMother]);
      for(Int_t isig=0; isig<fSignalsMC->GetEntriesFast(); isig++) {
	PairAnalysisSignalMC *sigMC=(PairAnalysisSignalMC*)fSignalsMC->At(isig);
	Double_t wght      = sigMC->GetWeight(values);
	Bool_t useMCweight = (TMath::Abs(wght-1.) > 1.0e-15);
	if(!useMCweight) continue;
	// printf("\t temp. particle weight: %f \t signal weight for %s is %f \n",particle->GetWeight(),sigMC->GetName(),sigMC->GetWeight());
	if( papaMC->IsMCTruth(particle,sigMC,1) || papaMC->IsMCTruth(particle,sigMC,2) ) {
	  //	  printf("particle weight: %f \t signal weight for %s is %f \n",particle->GetWeight(),sigMC->GetName(),sigMC->GetWeight());
	  //	  if(sig->GetWeight(values) != 1.0) particle->SetWeight( sig->GetWeight(values) );
	  particle->SetWeight( wght );
	  //   printf("no weight set for %s, use  default (1.)",sigMC->GetName());
	}
      }
      //      printf("  -----> final particle weight: %f \n",particle->GetWeight());

      Double_t wght = particle->GetWeight();
      Bool_t hasMCweight = (TMath::Abs(wght-1.) > 1.0e-15);

      /// check for weights of coming from mother and/or grandmother particles
      /// NOTE: by this bremsstrahlung weight are automatically propagated to the cascade product
      Int_t label = particle->GetLabel();
      while ( !hasMCweight ) {

	Int_t motherLabel = papaMC->GetMothersLabel( label );
	if(motherLabel==-1) break; // stop if primary particle is reached

	/// loop over all added signals
	for(Int_t isig=0; isig<fSignalsMC->GetEntriesFast(); isig++) {
	  PairAnalysisSignalMC *sigMC=(PairAnalysisSignalMC*)fSignalsMC->At(isig);
	  Double_t wghtMC    = sigMC->GetWeight(values);
	  Bool_t useMCweight = (TMath::Abs(wghtMC-1.) > 1.0e-15);
	  if(!useMCweight) continue; // signal has no weights applied

	  if( papaMC->IsMCTruth(motherLabel,sigMC,1) || papaMC->IsMCTruth(motherLabel,sigMC,2) ) {
	    particle->SetWeight( wghtMC );
	    hasMCweight = kTRUE;
	  }
	}

	/// start from mother label to next familiy generation
	label = motherLabel;

      }

    }//end: store mc weights

    //fill selected particle into the corresponding track arrays
    Short_t charge=particle->Charge();
    if (charge>0)      fTracks[0].Add(particle); // positive tracks
    else if (charge<0) fTracks[1].Add(particle); // negative tracks
  }
}

//________________________________________________________________
void PairAnalysis::PairPreFilter(Int_t arr1, Int_t arr2, TObjArray &arrTracks1, TObjArray &arrTracks2)
{
  //
  // Prefilter tracks from pairs
  // Needed for datlitz rejections
  // remove all tracks from the Single track arrays that pass the cuts in this filter
  //

  //// for random rejection, set as an event variable
  //// add one pos. & neg. test particle to the arrays
  //// this needs to be removed at the end!
  //// 
  PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kRndmRej, 0.);
  /// first test particle
  Double_t mass = TDatabasePDG::Instance()->GetParticle(fPdgLeg1)->Mass();
  Double_t pt   = gRandom->Exp(3.); // return t from exp(-t/3.)
  if(pt<0.075) pt = 0.075;
  Double_t eta  = -TMath::Log(TMath::Tan( (gRandom->Uniform(2.5,25.)/180.*TMath::Pi()) /2));
  Double_t phi  = gRandom->Uniform(TMath::TwoPi());
  PairAnalysisTrack *t1 = new PairAnalysisTrack();
  t1->SetPdgCode(fPdgLeg1);
  t1->GetMomentum()->SetPtEtaPhiM(pt, eta, phi, mass);
  fTracks[0].Add(t1); // positive tracks
  /// second test particle
  mass=TDatabasePDG::Instance()->GetParticle(fPdgLeg2)->Mass();
  pt   = gRandom->Exp(3.); // return t from exp(-t/3.)
  if(pt<0.075) pt = 0.075;
  eta  = -TMath::Log(TMath::Tan( (gRandom->Uniform(2.5,25.)/180.*TMath::Pi()) /2));
  phi  = gRandom->Uniform(TMath::TwoPi());
  PairAnalysisTrack *t2 = new PairAnalysisTrack();
  t2->SetPdgCode(fPdgLeg2);
  t2->GetMomentum()->SetPtEtaPhiM(pt, eta, phi, mass);
  fTracks[1].Add(t2); // negative tracks
  /////

  /// pairprefilter leg cuts
  /// TODO: cut logic exceptions for test particles
  /// mask used to require that all cuts are fulfilled
  UInt_t selectedMaskLeg=(1<<fPairPreFilterLegs.GetCuts()->GetEntries())-1;
  Bool_t isLeg1selected = kTRUE;
  Bool_t isLeg2selected = kTRUE;


  Int_t ntrack1=arrTracks1.GetEntriesFast();
  Int_t ntrack2=arrTracks2.GetEntriesFast();

  /// flag arrays for track removal
  Bool_t *bTracks1 = new Bool_t[ntrack1];
  for (Int_t itrack1=0; itrack1<ntrack1; ++itrack1) bTracks1[itrack1]=kFALSE;
  Bool_t *bTracks2 = new Bool_t[ntrack2];
  for (Int_t itrack2=0; itrack2<ntrack2; ++itrack2) bTracks2[itrack2]=kFALSE;

  //// random rejection: mark test particles for removal
  bTracks1[ntrack1-1]=kTRUE;
  bTracks2[ntrack2-1]=kTRUE;

  // candiate
  PairAnalysisPair *candidate;
  if(fUseKF) candidate = new PairAnalysisPairKF();
  else       candidate = new PairAnalysisPairLV();
  candidate->SetKFUsage(fUseKF);

  UInt_t selectedMask=(1<<fPairPreFilter.GetCuts()->GetEntries())-1;

  Int_t nRejPasses = 1; //for fPreFilterUnlikeOnly and no set flag 
  if (fPreFilterAllSigns) nRejPasses = 3;

  // loop over rejection passes: OS (+ 2xLS)
  for (Int_t iRP=0; iRP < nRejPasses; ++iRP) {

    // default rejection pass OS
    Int_t arr1RP=arr1, arr2RP=arr2;
    TObjArray *arrTracks1RP=&arrTracks1;
    TObjArray *arrTracks2RP=&arrTracks2;
    Bool_t *bTracks1RP = bTracks1;
    Bool_t *bTracks2RP = bTracks2;

    // change for LS rejection passes
    switch (iRP) {
    case 1: arr1RP=arr1;arr2RP=arr1;
      arrTracks1RP=&arrTracks1;
      arrTracks2RP=&arrTracks1;
      bTracks1RP = bTracks1;
      bTracks2RP = bTracks1;
      break;
    case 2: arr1RP=arr2;arr2RP=arr2;
      arrTracks1RP=&arrTracks2;
      arrTracks2RP=&arrTracks2;
      bTracks1RP = bTracks2;
      bTracks2RP = bTracks2;
      break;
    default: ;//nothing to do
    }
	
    Int_t ntrack1RP=(*arrTracks1RP).GetEntriesFast();
    Int_t ntrack2RP=(*arrTracks2RP).GetEntriesFast();
    
    Int_t pairIndex=GetPairIndex(arr1RP,arr2RP);
    // loop over all tracks in both arrays
    for (Int_t itrack1=0; itrack1<ntrack1RP; ++itrack1){
      Int_t end=ntrack2RP;
      if (arr1RP==arr2RP) end=itrack1;

      TObject *track1=(*arrTracks1RP).UncheckedAt(itrack1);
      if(!track1) continue;

      /// pair prefilter leg cuts
      if(selectedMaskLeg) {
	isLeg1selected=(fPairPreFilterLegs.IsSelected(static_cast<PairAnalysisTrack*>(track1))==selectedMaskLeg);
	if(fCutType==kBothLegs && !isLeg1selected) continue;
      }


      for (Int_t itrack2=0; itrack2<end; ++itrack2){
	TObject *track2=(*arrTracks2RP).UncheckedAt(itrack2);
	if (!track2) continue;

	/// pair prefilter leg cuts
	/// TODO: check rejection of pairleg only, when non prepair cut is applied!
	/// NOTE: in mode kAnyLeg do the next check only if track1 was not selected
	if( (selectedMaskLeg && fCutType!=kAnyLeg)                        ||
	    (selectedMaskLeg && fCutType==kAnyLeg && !isLeg1selected) ) {
	  isLeg2selected=(fPairPreFilterLegs.IsSelected(static_cast<PairAnalysisTrack*>(track2))==selectedMaskLeg);        // apply selection
	  switch(fCutType) {
	  case kBothLegs:  if(!isLeg2selected)                continue; break;
	  case kAnyLeg:    if(!isLeg2selected)                continue; break;
	  case kOneLeg:    if(isLeg1selected==isLeg2selected) continue; break;
	  }
	}

	/// create the pair
	candidate->SetTracks(static_cast<PairAnalysisTrack*>(track1), fPdgLeg1,
			     static_cast<PairAnalysisTrack*>(track2), fPdgLeg2);

	candidate->SetType(pairIndex);
	candidate->SetLabel(PairAnalysisMC::Instance()->GetLabelMotherWithPdg(candidate,fPdgMother));

	/// check if test particles are used
	Bool_t testParticle = (track1==t1 || track1==t2 || track2==t1 || track2==t2 );

	/// pre filter pair cuts
	UInt_t cutmask=fPairPreFilter.IsSelected(candidate);

	/// fill cut QA TODO: cheeeck for test particle
	if (!testParticle) {
	  if(fCutQA) fQAmonitor->FillAll(candidate,     1);
	  if(fCutQA) fQAmonitor->Fill(cutmask,candidate,1);
	}

	/// apply cut
	if (cutmask!=selectedMask) {
	  continue;
	}

	/// check for test particles
	if (testParticle) {
	  // set variable to randomrejection probability to 1
	  PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kRndmRej, 1.);
	  continue;
	}

	/// fill histos
	if (fHistos) FillHistogramsPair(candidate,kTRUE); // kTRUE: fromPrefilter

	/// set flags for track removal
	bTracks1RP[itrack1]=kTRUE;
	bTracks2RP[itrack2]=kTRUE;
      }
    }

  } // end rejection passes

  //clear surplus candiate
  delete candidate;

  //remove the tracks from the Track arrays
  for (Int_t itrack1=0; itrack1<ntrack1; ++itrack1){
    if(bTracks1[itrack1]) arrTracks1.AddAt(0x0, itrack1);
  }
  for (Int_t itrack2=0; itrack2<ntrack2; ++itrack2){
    if(bTracks2[itrack2]) arrTracks2.AddAt(0x0, itrack2);
  }

  // clean up
  delete [] bTracks1;
  delete [] bTracks2;

  //compress the track arrays
  arrTracks1.Compress();
  arrTracks2.Compress();
  
  /*
  //apply leg cuts after the pre filter
  if ( fFinalTrackFilter.GetCuts()->GetEntries()>0 ) {
    selectedMask=(1<<fFinalTrackFilter.GetCuts()->GetEntries())-1;
    //loop over tracks from array 1
    for (Int_t itrack=0; itrack<arrTracks1.GetEntriesFast();++itrack){
      //test cuts
      UInt_t cutMask=fFinalTrackFilter.IsSelected(arrTracks1.UncheckedAt(itrack));
      
      //apply cut
      if (cutMask!=selectedMask) arrTracks1.AddAt(0x0,itrack);
    }
    arrTracks1.Compress();
    
    //in case of like sign don't loop over second array
    if (arr1==arr2) {
      arrTracks2=arrTracks1;
    } else {
      
      //loop over tracks from array 2
      for (Int_t itrack=0; itrack<arrTracks2.GetEntriesFast();++itrack){
      //test cuts
        UInt_t cutMask=fFinalTrackFilter.IsSelected(arrTracks2.UncheckedAt(itrack));
      //apply cut
        if (cutMask!=selectedMask) arrTracks2.AddAt(0x0,itrack);
      }
      arrTracks2.Compress();
      
    }
  }
  //For unlike-sign monitor track-cuts:
  if (arr1!=arr2&&fHistos) {
    TObjArray *unlikesignArray[2] = {&arrTracks1,&arrTracks2};
    FillHistogramsTracks(unlikesignArray);
  }
  */

}

//________________________________________________________________
void PairAnalysis::FilterTrackArrays(TObjArray &arrTracks1, TObjArray &arrTracks2)
{
  //
  // select tracks and adapt track candidate arrays
  // second and final track selection
  //

  // get event data
  Double_t *values=PairAnalysisVarManager::GetData();

  //apply leg cuts after the pre filter
  if ( fFinalTrackFilter.GetCuts()->GetEntries()<1 ) return;

  UInt_t selectedMask=(1<<fFinalTrackFilter.GetCuts()->GetEntries())-1;
  //loop over tracks from array 1
  for (Int_t itrack=0; itrack<arrTracks1.GetEntriesFast();++itrack){

    //get particle
    PairAnalysisTrack *particle= static_cast<PairAnalysisTrack*>(arrTracks1.UncheckedAt(itrack));

    // fill variables
    PairAnalysisVarManager::Fill(particle, values);

    //apply cuts
    UInt_t cutmask=fFinalTrackFilter.IsSelected(values);
    //    UInt_t cutmask=fFinalTrackFilter.IsSelected(particle);
    //fill cut QA
    if(fCutQA) fQAmonitor->FillAll(particle,     1);
    if(fCutQA) fQAmonitor->Fill(cutmask,particle,1);

    /// fill detailed cut histograms
    if(fFinalTrackFilter.GetHistogramList()->GetSize())
      FillCutStepHistograms( &fFinalTrackFilter, cutmask, particle, values);

    // rejection
    if (cutmask!=selectedMask) arrTracks1.AddAt(0x0,itrack);

  }
  arrTracks1.Compress();

  //loop over tracks from array 2
  for (Int_t itrack=0; itrack<arrTracks2.GetEntriesFast();++itrack){

    //get particle
    PairAnalysisTrack *particle= static_cast<PairAnalysisTrack*>(arrTracks2.UncheckedAt(itrack));

    // fill variables
    PairAnalysisVarManager::Fill(particle, values);

    //apply cuts
    UInt_t cutmask=fFinalTrackFilter.IsSelected(values);
    //    UInt_t cutmask=fFinalTrackFilter.IsSelected(particle);

    //fill cut QA
    if(fCutQA) fQAmonitor->FillAll(particle,     1);
    if(fCutQA) fQAmonitor->Fill(cutmask,particle,1);

    /// fill detailed cut histograms
    if(fFinalTrackFilter.GetHistogramList()->GetSize())
      FillCutStepHistograms( &fFinalTrackFilter, cutmask, particle, values);

    // rejection
    if (cutmask!=selectedMask) arrTracks2.AddAt(0x0,itrack);

  }
  arrTracks2.Compress();

}

//________________________________________________________________
void PairAnalysis::FillPairArrays(Int_t arr1, Int_t arr2)
{
  //
  // select pairs and fill pair candidate arrays
  //

  TObjArray arrTracks1=fTracks[arr1];
  TObjArray arrTracks2=fTracks[arr2];

  //process pre filter if set
  //  if ((!fPreFilterAllSigns) && (!fPreFilterUnlikeOnly) && ( fPairPreFilter.GetCuts()->GetEntries()>0 ))  PairPreFilter(arr1, arr2, arrTracks1, arrTracks2);

  Int_t pairIndex=GetPairIndex(arr1,arr2);

  Int_t ntrack1=arrTracks1.GetEntriesFast();
  Int_t ntrack2=arrTracks2.GetEntriesFast();

  /// NOTE: use KF particle only for same event (SE) pair types
  ///       mixed event pairs use the default LV vertexing
  ///       otherwise this will crash in the refiting done in PairAnalysisPairKF
  ///       track rotation is not done here!
  PairAnalysisPair *candidate;
  if(fUseKF && pairIndex<=kSEMM) candidate = new PairAnalysisPairKF();
  else       candidate = new PairAnalysisPairLV();
  candidate->SetKFUsage(fUseKF);
  candidate->SetType(pairIndex);

  UInt_t selectedMask=(1<<fPairFilter.GetCuts()->GetEntries())-1;

  for (Int_t itrack1=0; itrack1<ntrack1; ++itrack1){
    Int_t end=ntrack2;
    if (arr1==arr2) end=itrack1;
    for (Int_t itrack2=0; itrack2<end; ++itrack2){
      //create the pair (direct pointer to the memory by this daughter reference are kept also for ME)
      candidate->SetTracks(&(*static_cast<PairAnalysisTrack*>(arrTracks1.UncheckedAt(itrack1))), fPdgLeg1,
       			   &(*static_cast<PairAnalysisTrack*>(arrTracks2.UncheckedAt(itrack2))), fPdgLeg2);
      // TODO: maybe set here the mother pdg code and remove fPdgMother
      Int_t label=PairAnalysisMC::Instance()->GetLabelMotherWithPdg(candidate,fPdgMother);
      candidate->SetLabel(label);
      if (label>-1) candidate->SetPdgCode(fPdgMother);
      else candidate->SetPdgCode(0);

      //pair cuts
      UInt_t cutMask=fPairFilter.IsSelected(candidate);

      // cut qa
      if(pairIndex==kSEPM && fCutQA) {
	fQAmonitor->FillAll(candidate);
	fQAmonitor->Fill(cutMask,candidate);
      }

      //apply cut
      if (cutMask!=selectedMask) continue;

      // if(pairIndex==3)
      // 	printf("fill pair array \t train: %d \t pair type: %s \t p:%p d1:%p d2:%p \n",!fEventProcess,fgkPairClassNames[pairIndex],candidate,
      // 	       candidate->GetFirstDaughter()->GetGlobalTrack(),candidate->GetSecondDaughter()->GetGlobalTrack());
      //histogram array for the pair
      ////TODO: still needed?  if (fHistoArray) fHistoArray->Fill(pairIndex,candidate);

      //add the candidate to the candidate array
      PairArray(pairIndex)->Add(candidate);
      //get a new candidate
      if(fUseKF && pairIndex<=kSEMM) candidate = new PairAnalysisPairKF();
      else       candidate = new PairAnalysisPairLV();
      candidate->SetKFUsage(fUseKF);
      candidate->SetType(pairIndex);
    }
  }
  //delete the surplus candidate
  delete candidate;
}

//________________________________________________________________
void PairAnalysis::FillPairArrayTR()
{
  //
  // rotate pairs and fill into pair candidate arrays
  //

  Int_t ntrack1=fTracks[0].GetEntriesFast();
  Int_t ntrack2=fTracks[1].GetEntriesFast();

  PairAnalysisPair *candidate;
  if(fUseKF) candidate = new PairAnalysisPairKF();
  else       candidate = new PairAnalysisPairLV();
  candidate->SetKFUsage(fUseKF);
  candidate->SetType(kSEPMRot);

  UInt_t selectedMask=(1<<fPairFilter.GetCuts()->GetEntries())-1;
  // loop over track arrays
  for (Int_t itrack1=0; itrack1<ntrack1; ++itrack1){
    for (Int_t itrack2=0; itrack2<ntrack2; ++itrack2){

      // loop over iterations
      for (Int_t irot=0; irot<fTrackRotator->GetIterations(); ++irot){
	// build candidate
	candidate->SetTracks(&(*static_cast<PairAnalysisTrack*>(fTracks[0].UncheckedAt(itrack1))), fPdgLeg1,
			     &(*static_cast<PairAnalysisTrack*>(fTracks[1].UncheckedAt(itrack2))), fPdgLeg2);
	// rotate the candidates daughter track
	candidate->RotateTrack(fTrackRotator);

	//pair cuts
	UInt_t cutMask=fPairFilter.IsSelected(candidate);

	//apply cut
	if (cutMask!=selectedMask) continue;

	//histogram array for the pair
	if (fHistoArray) fHistoArray->Fill((Int_t)kSEPMRot,candidate);

	if(fHistos) FillHistogramsPair(candidate);
	if(fStoreRotatedPairs) {
	  if(fUseKF) PairArray(kSEPMRot)->Add(static_cast<PairAnalysisPairKF*>(candidate->Clone()));
	  else       PairArray(kSEPMRot)->Add(static_cast<PairAnalysisPairLV*>(candidate->Clone()));
	  // if(fUseKF) PairArray(kSEPMRot)->Add(new PairAnalysisPairKF(*candidate);
	  // else       PairArray(kSEPMRot)->Add(new PairAnalysisPairLV(*candidate);
	}

      }	// end of iterations
    } //arr0
  } //arr1

  //delete the surplus candidate
  delete candidate;

}

//________________________________________________________________
void PairAnalysis::AddSignalMC(PairAnalysisSignalMC* signal) {
  //
  //  Add an MC signal to the signals list
  //
  if(!fSignalsMC) {
    fSignalsMC = new TObjArray();
    fSignalsMC->SetOwner();
  }
  // sort mc signal (first single particle, then pair signals)
  if(signal->IsSingleParticle()) fSignalsMC->AddAtFree(signal);
  else fSignalsMC->AddAtAndExpand( signal, fSignalsMC->GetLast()<10?10:fSignalsMC->GetLast()+1 );
  //fSignalsMC->Add(signal);
}

//________________________________________________________________
Bool_t PairAnalysis::FillMCHistograms(Int_t label1, Int_t label2, Int_t nSignal) {
  //
  // fill QA MC TRUTH histograms for pairs and legs of all added mc signals
  //
  PairAnalysisSignalMC* sigMC = (PairAnalysisSignalMC*)fSignalsMC->At(nSignal);

  TString className  = Form("Pair_%s_MCtruth",       sigMC->GetName());
  TString className2 = Form("Track.Legs_%s_MCtruth",sigMC->GetName());
  TString className3 = Form("Track.%s_%s_MCtruth",fgkPairClassNames[1],sigMC->GetName());
  Bool_t pairClass=fHistos->HasHistClass(className.Data());
  Bool_t legClass =fHistos->HasHistClass(className2.Data());
  Bool_t trkClass =fHistos->HasHistClass(className3.Data());
  //  printf("fill signal %d: pair %d legs %d trk %d \n",nSignal,pairClass,legClass,trkClass);
  if(!pairClass && !legClass && !trkClass) return kFALSE;

  PairAnalysisMC* papaMC = PairAnalysisMC::Instance();
  CbmMCTrack* part1 = papaMC->GetMCTrackFromMCEvent(label1);
  CbmMCTrack* part2 = papaMC->GetMCTrackFromMCEvent(label2);
  if(!part1 && !part2) return kFALSE;
  if(part1&&part2) {
    // fill only unlike sign (and only SE)
    if(part1->GetCharge()*part2->GetCharge()>0) return kFALSE;
  }

  Int_t mLabel1 = papaMC->GetMothersLabel(label1);
  Int_t mLabel2 = papaMC->GetMothersLabel(label2);
  //  printf("leg/mother labels: %d/%d %d/%d \t part %p,%p \n",label1,mLabel1,label2,mLabel2,part1,part2);

  // check the same mother option
  if(sigMC->GetMothersRelation()==PairAnalysisSignalMC::kSame      && mLabel1!=mLabel2) return kFALSE;
  if(sigMC->GetMothersRelation()==PairAnalysisSignalMC::kDifferent && mLabel1==mLabel2) return kFALSE;

  // fill event values
  Double_t *values = PairAnalysisVarManager::GetData(); //NEW CHANGED
  PairAnalysisVarManager::SetFillMap(fUsedVars);
  ///TODO:    PairAnalysisVarManager::Fill(papaMC->GetMCEvent(), values); // get event informations
  // fill the leg variables
  if (legClass || trkClass) {
    if(part1) PairAnalysisVarManager::Fill(part1,values);
    PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kWeight, sigMC->GetWeight(values));
    if(part1 && trkClass)          fHistos->FillClass(className3, values);
    if(part1 && part2 && legClass) fHistos->FillClass(className2, values);
    if(part2) PairAnalysisVarManager::Fill(part2,values);
    PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kWeight, sigMC->GetWeight(values));
    if(part2 && trkClass)          fHistos->FillClass(className3, values);
    if(part1 && part2 && legClass) fHistos->FillClass(className2, values);
  }

  /// loop over all detectors and fill point histograms
  FairMCPoint *pnt=NULL;
  TString className4;
  // loop over daughters
  for(Int_t ipart=0; ipart<2; ipart++) {

    CbmMCTrack *part = (!ipart ? part1  : part2);
    Int_t      label = (!ipart ? label1 : label2);
    if(!part) continue;
    if(ipart && sigMC->IsSingleParticle()) continue;

    // leg and no leg loop
    for(Int_t ileg=0; ileg<2; ileg++) {

      // loop over all detectors
      for (ECbmModuleId idet=ECbmModuleId::kRef; idet<ECbmModuleId::kNofSystems; ++idet){
	className4=Form("Hit.%s",(ileg?"Legs.":""));
	className4+= PairAnalysisHelper::GetDetName(idet) + "_" + sigMC->GetName() + "_MCtruth";
	if(!fHistos->HasHistClass(className4)) continue;

	Int_t npnts = part->GetNPoints(idet);
	//printf("track %p(%d) \t has %d %s mc points \n",part,label1,npnts,PairAnalysisHelper::GetDetName(idet).Data());
	if(!npnts) continue;

	TClonesArray *points = PairAnalysisVarManager::GetCurrentEvent()->GetPoints(idet);    // get point array
	Int_t psize = points->GetSize();
	if(!points || psize<1) continue;

	Int_t nfnd=0;
	for(Int_t idx=0; idx<psize; idx++) {
	  if(nfnd==npnts) break; // all points found

	  pnt = static_cast<FairMCPoint*>( points->At(idx));
	  if(pnt->GetTrackID() == label) {
	    // printf("det %s \t point index: %d/%d found! \n",PairAnalysisHelper::GetDetName(idet).Data(),idx,psize);
	    nfnd++;   // found point
	    PairAnalysisVarManager::Fill(pnt,values);
	    fHistos->FillClass(className4, values);
	  }
	}

      } //idet loop
    } // leg or not loop
  } // daughters loop

  //fill pair information
  if (pairClass && part1 && part2) {
    PairAnalysisVarManager::FillVarMCParticle(part1,part2,values);
    PairAnalysisVarManager::SetValue(PairAnalysisVarManager::kWeight, sigMC->GetWeight(values));
    fHistos->FillClass(className, values);
  }

  return kTRUE;
}

//________________________________________________________________
void PairAnalysis::FillHistogramsFromPairArray(Bool_t pairInfoOnly/*=kFALSE*/)
{
  //
  // Fill Histogram information for tracks and pairs
  //

  TString  className,className2;
  Double_t *values=PairAnalysisVarManager::GetData();
  PairAnalysisVarManager::SetFillMap(fUsedVars);

  //Fill event information
  if(!pairInfoOnly) {
    if(fHistos->GetHistogramList()->FindObject("Event")) {
      fHistos->FillClass("Event", PairAnalysisVarManager::GetData());
    }
  }

  UInt_t selectedMask=(1<<fPairFilter.GetCuts()->GetEntries())-1;

  //Fill Pair information, separately for all pair candidate arrays and the legs
  TObjArray arrLegs(100);
  for (Int_t i=0; i<(kPairTypes-1); ++i){
    Int_t npairs=PairArray(i)->GetEntriesFast();
    if(npairs<1) continue;

    className.Form("Pair.%s",fgkPairClassNames[i]);
    className2.Form("Track.Legs_%s",fgkPairClassNames[i]);
    Bool_t pairClass=fHistos->HasHistClass(className);
    Bool_t legClass=fHistos->HasHistClass(className2);

    //    if (!pairClass&&!legClass) continue;
    for (Int_t ipair=0; ipair<npairs; ++ipair){
      PairAnalysisPair *pair=static_cast<PairAnalysisPair*>(PairArray(i)->UncheckedAt(ipair));

      // apply cuts
      UInt_t cutMask=fPairFilter.IsSelected(pair);

      // cut qa
      if(i==kSEPM && fCutQA) {
	fQAmonitor->FillAll(pair);
	fQAmonitor->Fill(cutMask,pair);
      }

      //apply cut
      if (cutMask!=selectedMask) continue;

      //histogram array for the pair
      if (fHistoArray) fHistoArray->Fill(i,pair);

      // fill map
      PairAnalysisVarManager::SetFillMap(fUsedVars);

      //fill pair information
      if (pairClass){
        PairAnalysisVarManager::Fill(pair, values);
        fHistos->FillClass(className, values);
      }

      //fill leg information, don't fill the information twice
      if (legClass){
        PairAnalysisTrack *d1=pair->GetFirstDaughter();
        PairAnalysisTrack *d2=pair->GetSecondDaughter();
        if (!arrLegs.FindObject(d1)){
          PairAnalysisVarManager::Fill(d1, values);
          fHistos->FillClass(className2, values);
          arrLegs.Add(d1);
        }
        if (!arrLegs.FindObject(d2)){
          PairAnalysisVarManager::Fill(d2, values);
          fHistos->FillClass(className2, values);
          arrLegs.Add(d2);
        }
      }
    }
    if (legClass) arrLegs.Clear();
  }

}

//________________________________________________________________
void  PairAnalysis::FillCutStepHistograms(AnalysisFilter *filter, UInt_t cutmask, PairAnalysisTrack *trk, const Double_t * values)
{

  /// mc instance
  PairAnalysisMC* papaMC=0x0;
  if(fHasMC && fSignalsMC) papaMC = PairAnalysisMC::Instance();

  /// hist classes
  TString className;
  TString classNameMC;
  TString classNamePM=Form("Track.%s",fgkPairClassNames[1]);

  PairAnalysisHistos histo;
  AnalysisCuts *cuts;
  TIter next(filter->GetCuts());
  Int_t iCut=0;

  /// loop over mc signals
  Int_t nsig = (fSignalsMC ? fSignalsMC->GetEntriesFast() : 1);
  PairAnalysisSignalMC *sigMC;
  for(Int_t isig=0; isig<nsig; isig++) {
    if(fSignalsMC) {
      sigMC = (PairAnalysisSignalMC*)fSignalsMC->At(isig);
      classNameMC = classNamePM + "_" + sigMC->GetName();
      //	  printf("fill cut details for %s \n",classNameMC.Data());
    }
    // check if machtes mc signal
    Bool_t isMCtruth = fSignalsMC && (papaMC->IsMCTruth(trk, sigMC, 1) || papaMC->IsMCTruth(trk, sigMC, 2) );
    if(isig && !isMCtruth) continue;

    /// store mc signal weights in track - ATTENTION later signals should be more specific
    //    if(sigMC->GetWeight(values) != 1.0) trk->SetWeight( sigMC->GetWeight(values) );

    /// reset iterator
    next.Reset();
    iCut=0;

    /// loop over cuts
    while((cuts = (AnalysisCuts*)next())) {
      ///    UInt_t cutMask=1<<iCut;         // for each cut
      UInt_t cutRef=(1<<(iCut+1))-1; // increasing cut match
      // printf("      fill cut %s for track %p in hist details \n",cuts->GetName(),trk);

      /// passed cut
      if ((cutmask&cutRef)==cutRef) {

	//	    printf("    track %p passed cut \n",trk);
	/// find histogram list of current track
	histo.SetHistogramList( *(THashList*)filter->GetHistogramList()->FindObject( cuts->GetName() ),
				kFALSE );

	/// fill track histos only once
	if(!isig)	{
	  histo.FillClass(classNamePM, values);
	  for (Int_t i=0; i<kLegTypes; ++i){
	    className.Form("Track.%s",fgkTrackClassNames[i]);
	    histo.FillClass(className, values);
	  }
	}
	/// fill mc
	if(isMCtruth)  histo.FillClass(classNameMC, values);
      } ///end passed cut
      iCut++;
    } ///end cuts
  } ///end mc signals

}

//________________________________________________________________
void  PairAnalysis::FillCutStepHistogramsMC(AnalysisFilter *filter, UInt_t cutmask, Int_t label, const Double_t * values)
{

  /// mc instance
  PairAnalysisMC* papaMC=0x0;
  if(fHasMC && fSignalsMC) papaMC = PairAnalysisMC::Instance();

  /// hist classes
  TString className;
  TString classNameMC;
  TString classNamePM=Form("Track.%s",fgkPairClassNames[1]);

  PairAnalysisHistos histo;
  AnalysisCuts *cuts;
  TIter next(filter->GetCuts());
  Int_t iCut=0;

  /// loop over mc signals
  Int_t nsig = (fSignalsMC ? fSignalsMC->GetEntriesFast() : 0);
  PairAnalysisSignalMC *sigMC;
  for(Int_t isig=0; isig<nsig; isig++) {

    sigMC = (PairAnalysisSignalMC*)fSignalsMC->At(isig);
    classNameMC = classNamePM + "_" + sigMC->GetName() + "_MCtruth";
    //	  printf("fill cut details for %s \n",classNameMC.Data());

    // Proceed only if this signal is required in the pure MC step
    if(!sigMC->GetFillPureMCStep()) continue;

    // check if matches mc signal
    Bool_t isMCtruth = fSignalsMC && (papaMC->IsMCTruth(label, sigMC, 1) || papaMC->IsMCTruth(label, sigMC, 2) );
    if(isig && !isMCtruth) continue;

    /// store mc signal weights in track - ATTENTION later signals should be more specific
    //    if(sigMC->GetWeight(values) != 1.0) trk->SetWeight( sigMC->GetWeight(values) );

    /// reset iterator
    next.Reset();
    iCut=0;

    /// loop over cuts
    while((cuts = (AnalysisCuts*)next())) {
      ///    UInt_t cutMask=1<<iCut;         // for each cut
      UInt_t cutRef=(1<<(iCut+1))-1; // increasing cut match

      /// passed cut
      if ((cutmask&cutRef)==cutRef) {

	/// find histogram list of current track
	histo.SetHistogramList( *(THashList*)filter->GetHistogramList()->FindObject( cuts->GetName() ),
				kFALSE );

	/// fill mc truth
	if(isMCtruth)  histo.FillClass(classNameMC, values);
      } ///end passed cut
      iCut++;
    } ///end cuts
  } ///end mc signals

}

//________________________________________________________________
void PairAnalysis::FillHistogramsHits(const PairAnalysisEvent *ev,
				      TBits *fillMC,
				      PairAnalysisTrack *track,
				      Bool_t trackIsLeg,
				      Double_t * values)
{
  //
  // Fill Histogram information for hits and hits of legs
  //
  TString  className;

  Int_t nsig = (fSignalsMC ? fSignalsMC->GetEntriesFast() : 0);
  TBits hitClassMC(  nsig);
  TBits hitClassMChf(nsig);

  TString  sigName;
  PairAnalysisMC *mc = (nsig ? PairAnalysisMC::Instance() : 0x0);


  // loop over all detectors
  for (ECbmModuleId idet=ECbmModuleId::kRef; idet<ECbmModuleId::kNofSystems; ++idet){

    // detectors implemented
    switch(idet) {
    case ECbmModuleId::kMvd:
    case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
    case ECbmModuleId::kRich:
    case ECbmModuleId::kTof:  /* */ break;
    default:
      continue;
    }

    /// histogram name convention
    className.Form("Hit.%s",(trackIsLeg?"Legs.":""));
    className+=PairAnalysisHelper::GetDetName(idet);  // detector hit

    Bool_t hitClass  =  fHistos->HasHistClass(className);
    Bool_t hitClass2 = (fHistoArray && fHistoArray->HasHistClass(className));

    // check mc signal filling
    for(Int_t isig=0; isig<nsig; isig++) {
      sigName = className + "_" + fSignalsMC->At(isig)->GetName();
      hitClassMC  .SetBitNumber(isig, fHistos->HasHistClass(sigName));
      hitClassMChf.SetBitNumber(isig, fHistoArray && fHistoArray->HasHistClass(sigName));
    }
    if(!hitClass && !hitClass2 && !hitClassMC.CountBits() && !hitClassMChf.CountBits()) continue;

    // get hit array
    TClonesArray *hits = ev->GetHits(idet);
    if(!hits || hits->GetSize()<1) continue;

    // get matched track and mc track index/id
    CbmTrackMatchNew *tmtch = track->GetTrackMatch(idet);
    Int_t mctrk = (tmtch?tmtch->GetMatchedLink().GetIndex():-1);
    //	  Printf("mc track id via track match (%p) link: %d",tmtch,mctrk);

    // get detector tracks
    CbmTrack    *trkl = 0x0;
    CbmRichRing *ring = 0x0;
    switch(idet) {
    case ECbmModuleId::kMvd:
    case ECbmModuleId::kSts:
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:  trkl = track->GetTrack(idet); break;
    case ECbmModuleId::kRich: ring = track->GetRichRing();   break;
    case ECbmModuleId::kTof:  /* */ break;
    default:
      continue;
    }

    // get number of hits
    Int_t nhits = 0;
    switch(idet) {
    case ECbmModuleId::kMvd:  if(trkl) nhits = static_cast<CbmStsTrack*>(trkl)->GetNofMvdHits(); break;
    case ECbmModuleId::kSts:  if(trkl) nhits = static_cast<CbmStsTrack*>(trkl)->GetNofStsHits(); break;
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:  if(trkl) nhits = trkl->GetNofHits();    break;
    case ECbmModuleId::kTof:  nhits = 1; /* one is maximum */         break;
    case ECbmModuleId::kRich: if(ring) nhits = ring->GetNofHits();    break;
    default:
      continue;
    }

    // loop over all reconstructed hits
    CbmHit      *hit  = 0x0;
    CbmMatch    *mtch = 0x0;
    FairMCPoint *pnt  = 0x0;
    for (Int_t ihit=0; ihit < nhits; ihit++) {
      Int_t idx=-1;
      switch(idet) {
      case ECbmModuleId::kMvd:  idx = static_cast<CbmStsTrack*>(trkl)->GetMvdHitIndex(ihit); break;
      case ECbmModuleId::kSts:  idx = static_cast<CbmStsTrack*>(trkl)->GetStsHitIndex(ihit); break;
      case ECbmModuleId::kMuch:
      case ECbmModuleId::kTrd:  idx = trkl->GetHitIndex(ihit);                               break;
      case ECbmModuleId::kTof:  hit = track->GetTofHit();                                    break;
      case ECbmModuleId::kRich: idx = ring->GetHit(ihit);                                    break;
      default:
	continue;
      }
      // get hit
      if(idet!=ECbmModuleId::kTof && idx>-1)  {
	hit = dynamic_cast<CbmHit*>(hits->At(idx));
      }
      if(!hit) continue;

      // fill rec hit variables
      PairAnalysisVarManager::Fill(hit, values);
      Bool_t trueHit=kTRUE;
      Bool_t fakeHit=kTRUE;
      // access to mc points
      if( (mtch=hit->GetMatch()) && ev->GetPoints(idet)) {
	Int_t nlinks=mtch->GetNofLinks();

	// check if mc point corresponds to the matched track (true or fake pnt)
	// DEFINITION: always a fake hit if you link to >1 mc points?
	//  trueHit = (pnt->GetTrackID() == mctrk && mctrk>-1 && nlinks==1);

	// fill MC hit variables
	// NOTE: the sum of all linked mc points is stored, you have to normlize to the mean
	// loop over all linked mc points
	for (Int_t iLink = 0; iLink < nlinks; iLink++) {
	  pnt = static_cast<FairMCPoint*>( ev->GetPoints(idet)
					   ->At(mtch->GetLink(iLink).GetIndex())
					   );

	  // Fill the MC hit variables
	  if(!iLink) PairAnalysisVarManager::Fill(pnt, values);
	  else       PairAnalysisVarManager::FillSum(pnt, values);

	  // hit type defintion
	  if(!pnt) trueHit=kFALSE;
	  else if(mc){
	    Int_t lbl  = pnt->GetTrackID();
	    Int_t lblM = mc->GetMothersLabel(lbl);
	    Int_t lblG = mc->GetMothersLabel(lblM);
	    if(lbl!=mctrk && lblM!=mctrk && lblG!=mctrk) trueHit=kFALSE;
	    else                                         fakeHit=kFALSE;
	  }

	} //end links

      } //end match found

      // fill rec hit histos
      if(hitClass)	    fHistos    ->FillClass(className, values);
      if(hitClass2)	    fHistoArray->FillClass(className, values);
      // fill true, distorted or fake hit histos
      if(trueHit) {
	if(hitClass)	    fHistos    ->FillClass(className+"_true", values);
	if(hitClass2)       fHistoArray->FillClass(className+"_true", values);
      }
      else if(fakeHit){
	if(hitClass)	    fHistos    ->FillClass(className+"_fake", values);
	if(hitClass2)       fHistoArray->FillClass(className+"_fake", values);
      }
      else {
	if(hitClass)	    fHistos    ->FillClass(className+"_dist", values);
	if(hitClass2)       fHistoArray->FillClass(className+"_dist", values);
      }
      // check and fill mc signal histos
      for(Int_t isig=0; isig<nsig; isig++) {
	sigName = className + "_" + fSignalsMC->At(isig)->GetName();
	if(fillMC->TestBitNumber(isig)) { // track is MC signal truth
	  if(hitClassMC.TestBitNumber(isig))   fHistos     ->FillClass(sigName, values);
	  if(hitClassMChf.TestBitNumber(isig)) fHistoArray ->FillClass(sigName, values);
	}
      }

      // reset pointer (needed for tof)
      hit=0x0;
    } // rec hit loop
  } // det loop

}

