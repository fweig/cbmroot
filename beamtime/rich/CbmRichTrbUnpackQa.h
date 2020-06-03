#ifndef CBMRICHTRBUNPACKQA_H
#define CBMRICHTRBUNPACKQA_H

#include "FairTask.h"

#include "CbmHistManager.h"
#include "CbmDrawHist.h"

class TClonesArray;
class TH1D;

class CbmRichTrbUnpackQa : public FairTask
{
public:

   CbmRichTrbUnpackQa();
   virtual ~CbmRichTrbUnpackQa();

   virtual InitStatus Init();
   virtual void Exec(Option_t* option);
   virtual void Finish();
   void InitHist();
   void DrawHist();
   void SetRunTitle(TString title) { fRunTitle = title; }
   void SetOutputDir(TString dir) {fOutputDir = dir;}
   void SetOutHistoFile(TString filename) {fOutHistoFile = filename;}
   void SetDrawHist(Bool_t value) { fDrawHist = value; }
   void FitGaussAndDrawResults(TH1* h);

private:

   TClonesArray* fRichTrbDigi;  // input
   TClonesArray* fRichHits;     // output
   TClonesArray* fRichHitInfos; // output

   Int_t numOfPairs;
   Int_t numOfSingleEdges;
   Int_t numOfSingleEdgesSync;

   CbmHistManager* fHM;

   Bool_t fDrawHist; // if TRUE histograms are drawn

   UInt_t fEventNum; // Event counter

   TString fRunTitle; // Title of the run
   TString fOutputDir; // output directory to store figures
   TString fOutHistoFile;

   // For event building
   Double_t lastBaseTimestamp;

   ClassDef(CbmRichTrbUnpackQa, 1);
};

#endif // CBMRICHTRBUNPACKQA_H
