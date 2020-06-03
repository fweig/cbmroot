#ifndef CBMRICHPROTOANALYSIS_H
#define CBMRICHPROTOANALYSIS_H

#include "FairTask.h"

#include "CbmHistManager.h"
#include "CbmDrawHist.h"

class TClonesArray;

class CbmRichProtoAnalysis : public FairTask
{
public:

   CbmRichProtoAnalysis();
   virtual ~CbmRichProtoAnalysis();

   virtual InitStatus Init();
   virtual void Exec(Option_t* option);
   virtual void Finish();
   void InitHist();
   void DrawHist();
   void SetRunTitle(TString title) { fRunTitle = title; }
   void SetOutputDir(TString dir) {fOutputDir = dir;}
   void SetOutHistoFile(TString filename) {fOutHistoFile = filename;}


private:

   TClonesArray* fRichTrbDigi;  // input
   TClonesArray* fRichHits;     // output
   TClonesArray* fRichHitInfos; // output

   Int_t numOfPairs;
   Int_t numOfSingleEdges;
   Int_t numOfSingleEdgesSync;

   CbmHistManager* fHM;

   TString fRunTitle; // Title of the run
   TString fOutputDir; // output directory to store figures
   TString fOutHistoFile;

   //TODO implement or get rid
   CbmRichProtoAnalysis(const CbmRichProtoAnalysis&);
   CbmRichProtoAnalysis operator=(const CbmRichProtoAnalysis&);

   ClassDef(CbmRichProtoAnalysis, 1);
};

#endif // CBMRICHPROTOANALYSIS_H
