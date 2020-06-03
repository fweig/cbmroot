///////////////////////////////////////////////////////////////////////////
//
//
// Authors:
//   Julian Book   <Julian.Book@cern.ch>
/*

  Advanced cut class.

  Add cuts that depend on some variable or formula of them. The minimum and
  maximum cut value can be expressed by a TForumla, TGraph or a THnBase.


*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////


#include <TAxis.h>
#include <TFormula.h>
#include <TGraph.h>
#include <THnBase.h>
#include <TSpline.h>

#include "PairAnalysisObjectCuts.h"
#include "PairAnalysisHelper.h"

ClassImp(PairAnalysisObjectCuts)


PairAnalysisObjectCuts::PairAnalysisObjectCuts() :
  PairAnalysisObjectCuts("objcuts","objcuts")
{
  //
  // Default costructor
  //
}

//________________________________________________________________________
PairAnalysisObjectCuts::PairAnalysisObjectCuts(const char* name, const char* title) :
  AnalysisCuts(name,title),
  fUsedVars(new TBits(PairAnalysisVarManager::kNMaxValuesMC))
{
  //
  // Named contructor
  //
  for (Int_t i=0; i<PairAnalysisObjectCuts::kNMaxCuts; ++i){
    fActiveCuts[i]=0;
    fCutExclude[i]=kFALSE;
    fCutMin[i]=0x0;
    fCutMax[i]=0x0;
    fVarFormula[i]=0x0;
  }
  PairAnalysisVarManager::InitFormulas();
}

//________________________________________________________________________
PairAnalysisObjectCuts::~PairAnalysisObjectCuts()
{
  //
  // Destructor
  //
  if (fUsedVars) delete fUsedVars;
  for (Int_t i=0; i<PairAnalysisObjectCuts::kNMaxCuts; ++i){
    fActiveCuts[i]=0;
    if(fCutMin[i]) delete fCutMin[i];
    if(fCutMax[i]) delete fCutMax[i];
    if(fVarFormula[i]) delete fVarFormula[i];
  }
}

//________________________________________________________________________
Bool_t PairAnalysisObjectCuts::IsSelected(Double_t * const values)
{
  //
  // Make cut decision
  //

  //reset
  fSelectedCutsMask=0;
  SetSelected(kFALSE);

  for (Int_t iCut=0; iCut<fNActiveCuts; ++iCut){
    Int_t cut=fActiveCuts[iCut];
    SETBIT(fSelectedCutsMask,iCut);

    // variable you want to cut on
    Double_t compValue = 0.;
    if(fVarFormula[iCut])   compValue = PairAnalysisHelper::EvalFormula(fVarFormula[iCut], values);
    else                    compValue = values[cut];

    // cut limits
    Double_t cutMin = -9.e30;
    Double_t cutMax = +9.e30;


    /// use a THnBase inherited cut object //
    if ( (fCutMin[iCut] && fCutMin[iCut]->IsA() == THnBase::Class()) ||
	 (fCutMax[iCut] && fCutMax[iCut]->IsA() == THnBase::Class())   ) {

	THnBase *hn = static_cast<THnBase*>(fCutMin[iCut]);
	if(fCutMin[iCut]) {
	  Double_t *vals = new Double_t[hn->GetNdimensions()];//={-1};
	  // get array of values for the corresponding dimensions using axis names
	  for(Int_t idim=0; idim<hn->GetNdimensions(); idim++) {
	    vals[idim] = values[PairAnalysisVarManager::GetValueType(hn->GetAxis(idim)->GetName())]; /// TODO: what about formulas in the axis
	  }
	  // find bin for values (w/o creating it in case it is not filled)
	  Long_t bin = hn->GetBin(vals,kFALSE);
	  if(bin>0)  cutMin = hn->GetBinContent(bin);
	  delete [] vals;
	}

	THnBase *hx = static_cast<THnBase*>(fCutMax[iCut]);
	if(fCutMax[iCut]) {
	  Double_t *vals = new Double_t[hx->GetNdimensions()];//={-1};
	  // get array of values for the corresponding dimensions using axis names
	  for(Int_t idim=0; idim<hx->GetNdimensions(); idim++) {
	    vals[idim] = values[PairAnalysisVarManager::GetValueType(hx->GetAxis(idim)->GetName())]; /// TODO: what about formulas in the axis
	  }
	  // find bin for values (w/o creating it in case it is not filled)
	  Long_t bin = hx->GetBin(vals,kFALSE);
	  if(bin>0)  cutMax = hx->GetBinContent(bin);
	  delete [] vals;
	}

      }
    else if ( (fCutMin[iCut] && fCutMin[iCut]->IsA() == TFormula::Class()) ||
	      (fCutMax[iCut] && fCutMax[iCut]->IsA() == TFormula::Class()) ) {
      /// use formulas for the cut //
      TFormula *formN = static_cast<TFormula*>(fCutMin[iCut]);
      if(formN)  cutMin   = PairAnalysisHelper::EvalFormula(formN,values);

      TFormula *formM = static_cast<TFormula*>(fCutMax[iCut]);
      if(formM)  cutMax   = PairAnalysisHelper::EvalFormula(formM,values);

    }
    else if ( (fCutMin[iCut] && fCutMin[iCut]->IsA() == TGraph::Class()) ||
	      (fCutMax[iCut] && fCutMax[iCut]->IsA() == TGraph::Class()) ) {
      /// use graph for the cut //
      /// NOTE: binary search or a linear interpolation is used,
      ///       spline creation at each eval is too cpu expensive
      TGraph *graphN  = static_cast<TGraph*>(fCutMin[iCut]);
      TGraph *graphM  = static_cast<TGraph*>(fCutMax[iCut]);

      // get x-value from formula
      TFormula *formX=0;
      Double_t xval =0.;
      if(graphN) {
	formX = static_cast<TFormula*>(graphN->GetListOfFunctions()->At(0));
	if(formX)  xval = PairAnalysisHelper::EvalFormula(formX,values);
      }
      if(!formX && graphM) {
	formX = static_cast<TFormula*>(graphM->GetListOfFunctions()->At(0));
	if(formX)  xval = PairAnalysisHelper::EvalFormula(formX,values);
      }

      /// use a linear interpolation
      // if(graphN)  cutMin   = graphN->Eval(xval);
      // if(graphM)  cutMax   = graphM->Eval(xval);

      /// binary search
      Int_t idx=TMath::BinarySearch(graphN->GetN(),graphN->GetX(),xval);
      cutMin = graphN->GetY()[idx];
      idx=TMath::BinarySearch(graphM->GetN(),graphM->GetX(),xval);
      cutMax = graphM->GetY()[idx];

    }
    else if ( (fCutMin[iCut] && fCutMin[iCut]->IsA() == TSpline3::Class()) ||
	      (fCutMax[iCut] && fCutMax[iCut]->IsA() == TSpline3::Class()) ) {
      /// TODO: think about poper implementation, how to store the x-variable in the spline
      /// use spline for the cut //
//      TSpline3 *splineN = static_cast<TSpline3*>(fCutMin[iCut]);
      //      if(splineN)  cutMin   = splineN->Eval(xval);
//      TSpline3 *splineM = static_cast<TSpline3*>(fCutMax[iCut]);
      //      if(splineM)  cutMax   = splineM->Eval(xval);
    }
    else {
      Error("IsSelected:","Cut object not supported (this message should never appear)");
      return kTRUE;
    }

    // protection against NaN (e.g. outside formula ranges, exclude them)
    if(TMath::IsNaN(cutMin)) cutMin=compValue+1.;
    if(TMath::IsNaN(cutMax)) cutMax=compValue-1.;

    // apply cut
    if ( ((compValue<cutMin) || (compValue>cutMax))^fCutExclude[iCut] ) CLRBIT(fSelectedCutsMask,iCut);

    // cut type and decision
    if ( fCutType==kAll && !TESTBIT(fSelectedCutsMask,iCut) ) return kFALSE; // option to (minor) speed improvement
  }

  Bool_t isSelected=(fSelectedCutsMask==fActiveCutsMask);
  if ( fCutType==kAny ) isSelected=(fSelectedCutsMask>0);
  SetSelected(isSelected);
  return isSelected;
}

//________________________________________________________________________
Bool_t PairAnalysisObjectCuts::IsSelected(TObject* track)
{
  //
  // Make cut decision
  //

  //reset
  fSelectedCutsMask=0;
  SetSelected(kFALSE);
  if (!track) return kFALSE;

  //Fill values
  Double_t *values=PairAnalysisVarManager::GetData();
  PairAnalysisVarManager::SetFillMap(fUsedVars);
  PairAnalysisVarManager::Fill(track,values);

  return (IsSelected(values));

}

//________________________________________________________________________
void PairAnalysisObjectCuts::AddCut(PairAnalysisVarManager::ValueTypes type, const char *formulaMin, const char *formulaMax, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  fCutMin[fNActiveCuts]=PairAnalysisHelper::GetFormula(formulaMin, formulaMin);
  fCutMax[fNActiveCuts]=PairAnalysisHelper::GetFormula(formulaMax, formulaMax);
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)type;
  fUsedVars->SetBitNumber(type,kTRUE);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisObjectCuts::AddCut(const char *formula, const char *formulaMin, const char *formulaMax, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  fCutMin[fNActiveCuts]=PairAnalysisHelper::GetFormula(formulaMin, formulaMin);
  fCutMax[fNActiveCuts]=PairAnalysisHelper::GetFormula(formulaMax, formulaMax);
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)PairAnalysisVarManager::kNMaxValuesMC;
  fVarFormula[fNActiveCuts]=PairAnalysisHelper::GetFormula(formula, formula);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisObjectCuts::AddCut(PairAnalysisVarManager::ValueTypes type, TGraph *const graphMin, TGraph *const graphMax, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  // if(graphMin) {
  //   TSpline3 *splineMin = new TSpline3("splineMin",graphMin); //spline w/o begin and end point conditions
  //   fCutMin[fNActiveCuts]=splineMin;
  // }
  // if(graphMax) {
  //   TSpline3 *splineMax = new TSpline3("splineMax",graphMax); //spline w/o begin and end point conditions
  //   fCutMax[fNActiveCuts]=splineMax;
  // }
  fCutMin[fNActiveCuts]=graphMin;
  fCutMax[fNActiveCuts]=graphMax;
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)type;
  fUsedVars->SetBitNumber(type,kTRUE);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisObjectCuts::AddCut(const char *formula, TGraph *const graphMin, TGraph *const graphMax, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  // if(graphMin) {
  //   TSpline3 *splineMin = new TSpline3("splineMin",graphMin); //spline w/o begin and end point conditions
  //   fCutMin[fNActiveCuts]=splineMin;
  // }
  // if(graphMax) {
  //   TSpline3 *splineMax = new TSpline3("splineMax",graphMax); //spline w/o begin and end point conditions
  //   fCutMax[fNActiveCuts]=splineMax;
  // }
  fCutMin[fNActiveCuts]=graphMin;
  fCutMax[fNActiveCuts]=graphMax;
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)PairAnalysisVarManager::kNMaxValuesMC;
  fVarFormula[fNActiveCuts]=PairAnalysisHelper::GetFormula(formula, formula);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisObjectCuts::AddCut(PairAnalysisVarManager::ValueTypes type, THnBase *const histMin, THnBase *const histMax, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  fCutMin[fNActiveCuts]=histMin;
  fCutMax[fNActiveCuts]=histMax;
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)type;
  fUsedVars->SetBitNumber(type,kTRUE);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisObjectCuts::AddCut(const char *formula, THnBase *const histMin, THnBase *const histMax, Bool_t excludeRange)
{
  //
  // Set cut range and activate it
  //
  fCutMin[fNActiveCuts]=histMin;
  fCutMax[fNActiveCuts]=histMax;
  fCutExclude[fNActiveCuts]=excludeRange;
  SETBIT(fActiveCutsMask,fNActiveCuts);
  fActiveCuts[fNActiveCuts]=(UShort_t)PairAnalysisVarManager::kNMaxValuesMC;
  fVarFormula[fNActiveCuts]=PairAnalysisHelper::GetFormula(formula, formula);
  ++fNActiveCuts;
}

//________________________________________________________________________
void PairAnalysisObjectCuts::Print(const Option_t* /*option*/) const
{
  //
  // Print cuts and the range
  //
  printf("cut ranges for '%s'\n",GetTitle());
  if (fCutType==kAll){
    printf("All Cuts have to be fulfilled\n");
  } else {
    printf("Any Cut has to be fulfilled\n");
  }

  /// loop over all cuts
  for (Int_t iCut=0; iCut<fNActiveCuts; ++iCut){

    // variable
    Int_t cut=(Int_t)fActiveCuts[iCut];
    TString tit = PairAnalysisVarManager::GetValueName((Int_t)cut);

    Bool_t fvar=fVarFormula[iCut];
    if(fvar) {
      TFormula *form = static_cast<TFormula*>(fVarFormula[iCut]);
      tit = form->GetExpFormula();
      // replace parameter variables with names labels
      for(Int_t j=0;j<form->GetNpar();j++) tit.ReplaceAll(Form("[%d]",j),form->GetParName(j));
    }

    // cut logic
    Bool_t inverse=fCutExclude[iCut];

    // cut limits
    Bool_t bCutGraph  = (fCutMin[iCut] && fCutMin[iCut]->IsA() == TGraph::Class());
    Bool_t bCutForm   = (fCutMin[iCut] && fCutMin[iCut]->IsA() == TFormula::Class());
    Bool_t bCutHn     = (fCutMin[iCut] && fCutMin[iCut]->IsA() == THnBase::Class());
//    Bool_t bCutSpline = (fCutMin[iCut] && fCutMin[iCut]->IsA() == TSpline::Class());

    TString dep="";
    // HnBase
    if(bCutHn) {
      THnBase *obj = static_cast<THnBase*>(fCutMin[iCut]);
      for(Int_t idim=0; idim<obj->GetNdimensions(); idim++)
	dep+=Form("%s%s",(idim?",":""),obj->GetAxis(idim)->GetName());
      dep.Prepend("histogram(");
      dep.Append(")");
    }
    // Graph
    if(bCutGraph) {
      TGraph *obj = static_cast<TGraph*>(fCutMin[iCut]);
      TFormula *form = static_cast<TFormula*>(obj->GetListOfFunctions()->At(0));
      dep = form->GetExpFormula();
      // replace parameter variables with names labels
      for(Int_t j=0;j<form->GetNpar();j++) dep.ReplaceAll(Form("[%d]",j),form->GetParName(j));
      dep.Prepend("graph(");
      dep.Append(")");
    }
    // formula
    if(bCutForm) {
      TFormula *obj = static_cast<TFormula*>(fCutMin[iCut]);
      dep = obj->GetExpFormula();
      // replace parameter variables with names labels
      for(Int_t j=0;j<obj->GetNpar();j++) dep.ReplaceAll(Form("[%d]",j),obj->GetParName(j));
      dep.Prepend("formula(");
      dep.Append(")");
    }

    // stdout
    if (!inverse) printf("Cut %02d: %s < %s < %s\n",    iCut, dep.Data(), tit.Data(), dep.Data());
    else          printf("Cut %02d: !(%s < %s < %s)\n", iCut, dep.Data(), tit.Data(), dep.Data());

  } //loop over cuts

}
