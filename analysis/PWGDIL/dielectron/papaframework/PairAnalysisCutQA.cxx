//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Authors:
//   Julian Book   <Julian.Book@cern.ch>
//
/*
   Allow to monitor how many tracks, pairs, events pass the selection criterion 
   in any of the cuts added to the corresponding filters. Further it automatically
   calculates the MC matching efficiency seperately for each detector and base PDG 
   particle after each cut.

   All you need to add to your config is the following:

     PairAnalysis::SetCutQA();


*/
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "PairAnalysisCutQA.h"

#include <TList.h>
#include <TCollection.h>
#include <TVectorD.h>

#include "CbmMCTrack.h"

#include "PairAnalysisCutGroup.h"
#include "AnalysisCuts.h"

#include "PairAnalysisEvent.h"
#include "PairAnalysisTrack.h"
#include "PairAnalysisPairLV.h"
#include "PairAnalysisPairKF.h"

#include "PairAnalysisHelper.h"


ClassImp(PairAnalysisCutQA)


PairAnalysisCutQA::PairAnalysisCutQA() :
  PairAnalysisCutQA("QAcuts","QAcuts")
{
  //
  // Default constructor
  //
}

//_____________________________________________________________________
PairAnalysisCutQA::PairAnalysisCutQA(const char* name, const char* title) :
  TNamed(name, title),
  fQAHistList()
{
  //
  // Named Constructor
  //
  for(Int_t itype=0; itype<kNtypes; itype++) {
    fNCuts[itype]=1;
    for(Int_t i=0; i<20; i++) {
      fCutNames[i][itype]="";
    }
  }
  fTypeKeys[kTrack]  = "Track";
  fTypeKeys[kTrack2] = "FinalTrack";
  fTypeKeys[kTrackMC]= "MCTrack";
  fTypeKeys[kPair]   = "Pair";
  fTypeKeys[kPrePair]= "PrePair";
  fTypeKeys[kEvent]  = "Event";
  fQAHistList.SetOwner(kFALSE);
}

//_____________________________________________________________________
PairAnalysisCutQA::~PairAnalysisCutQA() 
{
  //
  //Default Destructor
  //
  fQAHistList.Clear();
}

//_____________________________________________________________________
void PairAnalysisCutQA::Init()
{

  fQAHistList.SetName(Form("%s",GetName()));

  THashList *table=new THashList;
  table->SetOwner(kTRUE);
  table->SetName("Event");
  fQAHistList.Add(table);

  table=new THashList;
  table->SetOwner(kTRUE);
  table->SetName("Track");
  fQAHistList.Add(table);

  table=new THashList;
  table->SetOwner(kTRUE);
  table->SetName("Pair");
  fQAHistList.Add(table);


  TH1I *fCutQA=0x0;     // qa histogram for counts
  TH2I *fPdgCutQA=0x0;  // qa histogram for PDG codes
  TProfile2D *fEffCutQA=0x0; // qa histogram for matching efficicy

  const TVectorD *binsPdg = PairAnalysisHelper::MakeLinBinning(5,0,5);
  const TVectorD *binsDet = PairAnalysisHelper::MakeLinBinning(6,0,6);
  // loop over all types
  for(Int_t itype=0; itype<kNtypes; itype++) {
    //    printf("\n type: %d\n",itype);
    TString logic = "passed";
    if(itype==kPrePair) logic="rejected";

    const TVectorD *binsX = PairAnalysisHelper::MakeLinBinning(fNCuts[itype],0,fNCuts[itype]);
    // create histogram based on added cuts
    fCutQA = new TH1I(fTypeKeys[itype],
		      Form("%sQA;cuts;# %s %ss",fTypeKeys[itype],logic.Data(),fTypeKeys[itype]),
		      fNCuts[itype], binsX->GetMatrixArray());

    if(itype==kTrack || itype==kTrack2) {
      fPdgCutQA = new TH2I(Form("%sPDG",fTypeKeys[itype]),
				  Form("%sPDG;cuts;PDG code;# %s %ss",
				       fTypeKeys[itype],logic.Data(),fTypeKeys[itype]),
			   fNCuts[itype],binsX->GetMatrixArray(),
			   binsPdg->GetNrows()-1,binsPdg->GetMatrixArray() );

      fEffCutQA = new TProfile2D(Form("%sMatchEff",fTypeKeys[itype]),
					Form("%sMatchEff;cuts;detector;<#epsilon_{match}^{MC}>",
					     fTypeKeys[itype]),
				 fNCuts[itype],binsX->GetMatrixArray(),
				 binsDet->GetNrows()-1,binsDet->GetMatrixArray() );
    }
    else {
      fPdgCutQA=0x0;
      fEffCutQA=0x0;
    }

    // delete surplus vector
    delete binsX;

    // Set labels to histograms
    fCutNames[0][itype]="no cuts";
    if(fNCuts[kPrePair]>1) fCutNames[0][kTrack2]="pair prefilter";
    else                   fCutNames[0][kTrack2]="1st track filter";
    // loop over all cuts
    for(Int_t i=0; i<fNCuts[itype]; i++) {
      fCutQA->GetXaxis()->SetBinLabel(i+1,fCutNames[i][itype]);
      if(fPdgCutQA) fPdgCutQA->GetXaxis()->SetBinLabel(i+1,fCutNames[i][itype]);
      if(fEffCutQA) fEffCutQA->GetXaxis()->SetBinLabel(i+1,fCutNames[i][itype]);
      //      printf(" itype:%s %d -> cut:%s \n",fTypeKeys[itype],itype,fCutNames[i][itype]);
    }

    // pdg label
    if(fPdgCutQA) {
      TString pdglbl="";
      for(Int_t i=0; i<binsPdg->GetNrows()-1; i++) {
	switch(i+1) {
	case 1:  pdglbl="electron"; break; // electron
	case 2:  pdglbl="muon";     break; // muon
	case 3:  pdglbl="pion";     break; // pion
	case 4:  pdglbl="kaon";     break; // kaon
	case 5:  pdglbl="proton";   break; // proton
	}
	fPdgCutQA->GetYaxis()->SetBinLabel(i+1,pdglbl.Data());
      }
    }

    // detector label
    if(fEffCutQA) {
      TString detlbl="";
      for(Int_t i=0; i<binsDet->GetNrows()-1; i++) {
	switch(i+1) {
	case 1:  detlbl=PairAnalysisHelper::GetDetName(ECbmModuleId::kMvd);  break;
	case 2:  detlbl=PairAnalysisHelper::GetDetName(ECbmModuleId::kSts);  break;
	case 3:  detlbl=PairAnalysisHelper::GetDetName(ECbmModuleId::kRich); break;
	case 4:  detlbl=PairAnalysisHelper::GetDetName(ECbmModuleId::kTrd);  break;
	case 5:  detlbl=PairAnalysisHelper::GetDetName(ECbmModuleId::kTof);  break;
	case 6:  detlbl=PairAnalysisHelper::GetDetName(ECbmModuleId::kMuch); break;
	}
	fEffCutQA->GetYaxis()->SetBinLabel(i+1,detlbl.Data());
      }
    }

    // add to output list
    switch(itype) {
      case kEvent: 
	static_cast<THashList*>(fQAHistList.FindObject("Event"))->AddLast(fCutQA);
	break;
      case kTrack: 
      case kTrack2: 
      case kTrackMC: 
	static_cast<THashList*>(fQAHistList.FindObject("Track"))->AddLast(fCutQA);
	if(fPdgCutQA) static_cast<THashList*>(fQAHistList.FindObject("Track"))->AddLast(fPdgCutQA);
	if(fEffCutQA) static_cast<THashList*>(fQAHistList.FindObject("Track"))->AddLast(fEffCutQA);
	break;
      case kPair: 
      case kPrePair: 
	static_cast<THashList*>(fQAHistList.FindObject("Pair"))->AddLast(fCutQA);
	break;
    }

  }

  // delete surplus
  delete binsPdg;
  delete binsDet;

}

//_____________________________________________________________________
void PairAnalysisCutQA::AddTrackFilter(AnalysisFilter *filter)
{
  //
  // add track filter cuts to the qa histogram
  //


  TIter listIterator(filter->GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    Bool_t addCut=kTRUE;

    // add new cut class to the list
    if(addCut) {
      fCutNames[fNCuts[kTrack]][kTrack]=thisCut->GetTitle();
      //      printf("add cut %s to %d \n",thisCut->GetTitle(),fNCuts[kTrack]);
      fNCuts[kTrack]++;
    }

  } // pair filter loop

}

//_____________________________________________________________________
void PairAnalysisCutQA::AddTrackFilterMC(AnalysisFilter *filter)
{
  //
  // add MC track filter cuts to the qa histogram
  //


  TIter listIterator(filter->GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    Bool_t addCut=kTRUE;

    // add new cut class to the list
    if(addCut) {
      fCutNames[fNCuts[kTrackMC]][kTrackMC]=thisCut->GetTitle();
      //      printf("add cut %s to %d \n",thisCut->GetTitle(),fNCuts[kTrack]);
      fNCuts[kTrackMC]++;
    }

  } // pair filter loop

}

//_____________________________________________________________________
void PairAnalysisCutQA::AddTrackFilter2(AnalysisFilter *filter)
{
  //
  // add track filter cuts to the qa histogram
  //
  if(!filter) return;

  TIter listIterator(filter->GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    Bool_t addCut=kTRUE;

    // add new cut class to the list
    if(addCut) {
      fCutNames[fNCuts[kTrack2]][kTrack2]=thisCut->GetTitle();
      //      printf("add cut %s to %d \n",thisCut->GetTitle(),fNCuts[kTrack]);
      fNCuts[kTrack2]++;
    }

  } // pair filter loop

}


//_____________________________________________________________________
void PairAnalysisCutQA::AddPairFilter(AnalysisFilter *pairFilter)
{
  //
  // add track filter cuts to the qa histogram
  //

  TIter listIterator(pairFilter->GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    Bool_t addCut=kTRUE;

    // add new cut class to the list
    if(addCut) {
      fCutNames[fNCuts[kPair]][kPair]=thisCut->GetTitle();
      //  printf("add cut %s to %d \n",thisCut->GetTitle(),fNCuts[kPair]);
      fNCuts[kPair]++;
    }

  } // trk filter loop

}

//_____________________________________________________________________
void PairAnalysisCutQA::AddPrePairFilter(AnalysisFilter *pairFilter)
{
  //
  // add track filter cuts to the qa histogram
  //
  if(!pairFilter) return;

  TIter listIterator(pairFilter->GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    Bool_t addCut=kTRUE;

    // add new cut class to the list
    if(addCut) {
      fCutNames[fNCuts[kPrePair]][kPrePair]=thisCut->GetTitle();
      //  printf("add cut %s to %d \n",thisCut->GetTitle(),fNCuts[kPair]);
      fNCuts[kPrePair]++;
    }

  } // trk filter loop

}

//_____________________________________________________________________
void PairAnalysisCutQA::AddEventFilter(AnalysisFilter *eventFilter)
{
  //
  // add track filter cuts to the qa histogram
  //


  TIter listIterator(eventFilter->GetCuts());
  while (AnalysisCuts *thisCut = (AnalysisCuts*) listIterator()) {
    Bool_t addCut=kTRUE;

    // add new cut class to the list
    if(addCut) {
      fCutNames[fNCuts[kEvent]][kEvent]=thisCut->GetTitle();
      //      printf("add cut %s to %d \n",thisCut->GetTitle(),fNCuts[kEvent]);
      fNCuts[kEvent]++;
    }

  } // trk filter loop

}

//_____________________________________________________________________
void PairAnalysisCutQA::Fill(UInt_t mask, TObject *obj, UInt_t addIdx)
{
  //
  // fill the corresponding step in the qa histogram
  //

  UInt_t idx = GetObjIndex(obj)+addIdx;

  // pdg to pdg label
  Int_t pdg = 0;
  TString pdglbl="";
  if(idx==kTrack || idx==kTrack2) {
    pdg = (Int_t) (static_cast<PairAnalysisTrack*>(obj)->PdgCode());
    switch(TMath::Abs(pdg)) {
    case 11:  pdglbl="electron"; break; // electron
    case 13:  pdglbl="muon";     break; // muon
    case 211: pdglbl="pion";     break; // pion
    case 321: pdglbl="kaon";     break; // kaon
    case 2212:pdglbl="proton";   break; // proton
    }
  }

  // find histolist
  THashList *histos=0x0;
  switch(idx) {
  case kEvent: 
    histos = static_cast<THashList*>(fQAHistList.FindObject("Event"));
    break;
  case kTrack: 
  case kTrack2: 
  case kTrackMC: 
    histos = static_cast<THashList*>(fQAHistList.FindObject("Track"));
    break;
  case kPair: 
  case kPrePair: 
    histos = static_cast<THashList*>(fQAHistList.FindObject("Pair"));
    break;
  }

  // loop over cutmask and check decision
  Int_t cutstep=1;
  for (Int_t iCut=0; iCut<fNCuts[idx]-1;++iCut) {
    //    UInt_t cutMask=1<<iCut;         // for each cut
    UInt_t cutMask=(1<<(iCut+1))-1; // increasing cut match

    // passed
    if ((mask&cutMask)==cutMask) {
      static_cast<TH1I*>(histos->FindObject(fTypeKeys[idx])) ->Fill(cutstep);
      if(pdg) static_cast<TH2I*>(histos->FindObject(Form("%sPDG",fTypeKeys[idx]))) ->Fill(cutstep, pdglbl.Data(),1.);

      // fill detector dependent
      if(idx==kTrack || idx==kTrack2) {
	TProfile2D *detQA = static_cast<TProfile2D*>(histos->FindObject(Form("%sMatchEff",fTypeKeys[idx])));
	PairAnalysisTrack *t = static_cast<PairAnalysisTrack*>(obj);
	detQA->Fill(cutstep, PairAnalysisHelper::GetDetName(ECbmModuleId::kMvd), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kMvd)) ) );
	detQA->Fill(cutstep, PairAnalysisHelper::GetDetName(ECbmModuleId::kSts), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kSts)) ) );
	detQA->Fill(cutstep, PairAnalysisHelper::GetDetName(ECbmModuleId::kRich),t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kRich))) );
	detQA->Fill(cutstep, PairAnalysisHelper::GetDetName(ECbmModuleId::kTrd), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kTrd)) ) );
	detQA->Fill(cutstep, PairAnalysisHelper::GetDetName(ECbmModuleId::kTof), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kTof)) ) );
	detQA->Fill(cutstep, PairAnalysisHelper::GetDetName(ECbmModuleId::kMuch),t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kMuch))) );
      }

      ++cutstep;
    }

  }

}

//_____________________________________________________________________
void PairAnalysisCutQA::FillAll(TObject *obj, UInt_t addIdx)
{
  //
  // fill the corresponding step in the qa histogram
  //

  UInt_t idx = GetObjIndex(obj)+addIdx;

  // pdg to pdg label
  Int_t pdg = 0;
  TString pdglbl="";
  if(idx==kTrack || idx==kTrack2) {
    pdg = (Int_t) (static_cast<PairAnalysisTrack*>(obj)->PdgCode());
    switch(TMath::Abs(pdg)) {
    case 11:  pdglbl="electron"; break; // electron
    case 13:  pdglbl="muon";     break; // muon
    case 211: pdglbl="pion";     break; // pion
    case 321: pdglbl="kaon";     break; // kaon
    case 2212:pdglbl="proton";   break; // proton
    }
  }

  // find histolist
  THashList *histos=0x0;
  switch(idx) {
  case kEvent: 
    histos = static_cast<THashList*>(fQAHistList.FindObject("Event"));
    break;
  case kTrack: 
  case kTrack2: 
  case kTrackMC: 
    histos = static_cast<THashList*>(fQAHistList.FindObject("Track"));
    break;
  case kPair: 
  case kPrePair: 
    histos = static_cast<THashList*>(fQAHistList.FindObject("Pair"));
    break;
  }

  // fill
  static_cast<TH1I*>(histos->FindObject(fTypeKeys[idx])) ->Fill(0);
  if(pdg) static_cast<TH2I*>(histos->FindObject(Form("%sPDG",fTypeKeys[idx]))) ->Fill(0., pdglbl.Data(),1.);

  // fill detector dependent
  if(idx==kTrack || idx==kTrack2) {
    TProfile2D *detQA = static_cast<TProfile2D*>(histos->FindObject(Form("%sMatchEff",fTypeKeys[idx])));
    PairAnalysisTrack *t = static_cast<PairAnalysisTrack*>(obj);
    detQA->Fill(0., PairAnalysisHelper::GetDetName(ECbmModuleId::kMvd), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kMvd)) ) );
    detQA->Fill(0., PairAnalysisHelper::GetDetName(ECbmModuleId::kSts), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kSts)) ) );
    detQA->Fill(0., PairAnalysisHelper::GetDetName(ECbmModuleId::kRich),t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kRich))) );
    detQA->Fill(0., PairAnalysisHelper::GetDetName(ECbmModuleId::kTrd), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kTrd)) ) );
    detQA->Fill(0., PairAnalysisHelper::GetDetName(ECbmModuleId::kTof), t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kTof)) ) );
    detQA->Fill(0., PairAnalysisHelper::GetDetName(ECbmModuleId::kMuch),t->TestBit(BIT(14+ToIntegralType(ECbmModuleId::kMuch))) );
  }

}

//______________________________________________________________________
UInt_t PairAnalysisCutQA::GetObjIndex(TObject *obj)
{
  //
  // return the corresponding idex
  //
  //  printf("INFO: object type is a %s \n", obj->IsA()->GetName());
  if(obj->IsA()==CbmMCTrack::Class()               )  return kTrackMC;
  else if(obj->IsA()==PairAnalysisTrack::Class()   )  return kTrack;
  else if(obj->IsA()==PairAnalysisPairLV::Class()  )  return kPair;
  else if(obj->IsA()==PairAnalysisPairKF::Class()  )  return kPair;
  else if(obj->IsA()==PairAnalysisEvent::Class()   )  return kEvent;
  else
    fprintf(stderr,"ERROR: object type not supported, please let the author know about it\n");//, obj->IsA()->GetName());
  return -1;
}




