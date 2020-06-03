// ------------------------------------------------------------------
// -----                     TTofTriglogUnpacker                -----
// -----              Created 03/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TTOFTRIGLOGUNPACKER_H_
#define TTOFTRIGLOGUNPACKER_H_

#include "TObject.h"

#include <vector>

class TMbsUnpackTofPar;

class TClonesArray;
class TH1;
class TH2;
class TDirectory;

class TTofTriglogUnpacker : public TObject
{
   public:
      TTofTriglogUnpacker();
      TTofTriglogUnpacker( TMbsUnpackTofPar * parIn );
      ~TTofTriglogUnpacker();
      
      virtual void Clear(Option_t *option);
      
      void ProcessTriglog( UInt_t* pMbsData, UInt_t uLength );
      void UpdateStats();
      void FinishTriglog( );
      
      void CreateHistos();
      void FillHistos();
      void WriteHistos( TDirectory* inDir);
      void DeleteHistos();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofTriglogUnpacker(const TTofTriglogUnpacker&);
      TTofTriglogUnpacker& operator=(const TTofTriglogUnpacker&);
      
      TMbsUnpackTofPar * fParUnpack;
      TClonesArray * fTriglogBoardCollection;
      TClonesArray * fScalerBoardCollection;
      
      // Final output
      UInt_t   fuTotalTriggerCount;   //!
      UInt_t   fuFirstMbsTime;        //! first mbs time
      UInt_t   fuLastMbsTime;         //! last mbs time
      UInt_t **fuFirstScaler;         //! TRIGLOG scaler contents
      UInt_t **fuLastScaler;          //! TRIGLOG scaler contents
      
      // Histograms
      TH1    * fhTriglogSyncs;
      UInt_t   fuLastSync;            //! last sync number
      TH1    * fhSpillRate;           //! events per second
      TH1    * fTriggerPattern;
      TH1    * fInputPattern;
      std::vector< TH1* > fhScalers;  //! last value of each scaler = integrated counts
      
      std::vector< std::vector< UInt_t > > fvuFirstScalers;  //! First value of each scaler <= integrated counts
      
      Double_t fdFirstMbsTime;        // First point for rate measurement
      Double_t fdPrevMbsTime;         // previous point for rate measurement
      UInt_t   fuLastRefClk;          //! last reference clock number
      TH1    * fhRefClkRate;
      TH1    * fhRefClkRateEvo;
      
   ClassDef(TTofTriglogUnpacker, 1)
};

#endif // TTOFTRIGLOGUNPACKER_H_ 
