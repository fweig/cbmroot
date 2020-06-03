#ifndef CBMRICHEVENTDEBUG_H
#define CBMRICHEVENTDEBUG_H

#include "FairTask.h"

#include "CbmHistManager.h"
#include "CbmDrawHist.h"

class TClonesArray;


class CbmRichEventDebug : public FairTask
{
public:
   CbmRichEventDebug();
   virtual ~CbmRichEventDebug();

   virtual InitStatus Init();
   virtual void Exec(Option_t* option);
   virtual void Finish();

   void SetOutHistoFile(TString filename) {fOutHistoFile = filename;}


private:

   TClonesArray* fRichHits;        // input
   TClonesArray* fRichHitInfos;    // input

   Bool_t fSingleEventFilled;

   TString fOutHistoFile;

   CbmHistManager* fHM;

   //TODO implement or get rid
   CbmRichEventDebug(const CbmRichEventDebug&);
   CbmRichEventDebug operator=(const CbmRichEventDebug&);

   ClassDef(CbmRichEventDebug, 1);
};


#endif // CBMRICHEVENTDEBUG_H
