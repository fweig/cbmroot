#ifndef TVFTXBOARDDATA_H
#define TVFTXBOARDDATA_H

#include "MbsCrateDefines.h"

#ifdef WITHGO4ROC
   #include "TGo4Version.h"
   #if __GO4BUILDVERSION__ > 40502
      #include "go4iostream.h"
   #endif
   #define NB_BIN_FTS      1024
   #define CLOCK_TIME      5000 //ps
#else
   #include "Riostream.h"
#endif // WITHGO4ROC

#include "TObject.h"


class TVftxBoardData : public TObject {
   public:
      enum { MaxMult = 8 };

      Int_t    iTriggerTime;
      Int_t    iMultiplicity[FPGA_TDC_NBCHAN]; // how many hits par channel
      Int_t    iFineTime[FPGA_TDC_NBCHAN][MaxMult];
      Int_t    iCoarseTime[FPGA_TDC_NBCHAN][MaxMult];
      UInt_t   uFutureBit[FPGA_TDC_NBCHAN][MaxMult];
      Double_t dTimeCorr[FPGA_TDC_NBCHAN][MaxMult];
      Double_t dFineTimeCorr[FPGA_TDC_NBCHAN][MaxMult];

      TVftxBoardData() : 
         TObject(),
         iTriggerTime(0)
         { Clear(); }
      void Clear(Option_t* ="");
      Bool_t   IsHitThere( UInt_t uChan, Int_t iMult ){
              return iMult < iMultiplicity[ uChan ]? kTRUE : kFALSE; };
      Double_t GetCalibratedTime( UInt_t uChan, UInt_t uMult);
      Double_t GetCoarseCorrectedTime( UInt_t uChan, UInt_t uMult);

      ClassDef(TVftxBoardData,1)
};
#endif //TVFTXBOARDDATA_H
