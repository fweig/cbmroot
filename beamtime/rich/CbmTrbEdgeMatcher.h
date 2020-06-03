#ifndef CBMTRBEDGEMATCHER_H
#define CBMTRBEDGEMATCHER_H

#include <map>
#include <vector>

#include "FairTask.h"

#include "CbmTrbRawMessage.h"

// The TDC channels of which are drawn for debug
#define DEBUGTDCID 0x0081

class TH1D;
class TClonesArray;

class CbmTrbEdgeMatcher : public FairTask
{
public: // methods

   CbmTrbEdgeMatcher();
   ~CbmTrbEdgeMatcher();

   virtual InitStatus Init();
   virtual void Exec(Option_t* option);
   virtual void FinishEvent();
   virtual void FinishTask();

   void SetDrawHits(Bool_t toDraw = kTRUE) { fDrawHist = toDraw; }

private: // methods

   // Add the leading edge into the buffer
   void AddPossibleLeadingEdge(CbmTrbRawMessage rawMessage);

   // Return: pair, first - epoch, second - data word (with channel, coarse and fine)
   std::pair<UInt_t, CbmTrbRawMessage> FindLeadingEdge(UInt_t tdcId, UInt_t lChannel, CbmTrbRawMessage tRawMessage);

   // Create an output digi with only leading edge
   void CreateLeadingEdgeOnlyDigi(CbmTrbRawMessage rawMessage);

   // Interface to the Calibrator::GetFullTime
   Double_t GetFullTime(UInt_t tdcId, UInt_t channel, UInt_t epoch, UInt_t coarse, UInt_t fine);

   void DrawDebugHistos();

private: // data members

   // Input raw hits from the unpacker
   TClonesArray* fTrbRawHits;

   // Output pairs (if possible) of leading and trailing edges
   TClonesArray* fRichTrbDigi;	// class CbmRichTrbDigi

   std::map< UInt_t, std::vector< std::pair< UInt_t, CbmTrbRawMessage > >* > tdcIdToStoredEdges;

   // For statistics - maximum number of consequent leading edges found in the run
   //UInt_t fMaxBufFill;
   
   
   // Buffer for trailing edges
   std::vector<CbmTrbRawMessage*> fTrBuf;
   

   // For statistics - number of cases when there were several leading edges within time window
   UInt_t fMultiCounter;

   // --- Debug histograms

   // If true - draw debug histograms
   Bool_t fDrawHist;

   // For each possible TDCid
   TH1D* fhTtimeMinusLtime[68];

   // For all the channels of a single certain TDC with id DEBUGTDCID
   TH1D* fhTtimeMinusLtimeCH[16];

   TH1D* fhMultiDist;

   CbmTrbEdgeMatcher(const CbmTrbEdgeMatcher&);
   CbmTrbEdgeMatcher operator=(const CbmTrbEdgeMatcher&);

   ClassDef(CbmTrbEdgeMatcher,1)
};

#endif // CBMTRBEDGEMATCHER_H
