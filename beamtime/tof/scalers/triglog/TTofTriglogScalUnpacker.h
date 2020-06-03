// ------------------------------------------------------------------
// -----                     TTofTriglogScalUnpacker                -----
// -----              Created 20/03/2015 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFTRIGLOGSCALUNPACKER_H_
#define TTOFTRIGLOGSCALUNPACKER_H_

#include "TObject.h"

#include <vector>

class TMbsUnpackTofPar;

class TClonesArray;
class TH1;
class TH2;
class TDirectory;

class TTofTriglogScalUnpacker : public TObject
{
   public:
      TTofTriglogScalUnpacker();
      TTofTriglogScalUnpacker( TMbsUnpackTofPar * parIn );
      ~TTofTriglogScalUnpacker();

      virtual void Clear(Option_t *option);

      void ProcessTriglogScal( Int_t iTrigScalIndex, UInt_t* pMbsData, UInt_t uLength );
      void UpdateStats( Int_t iTrigScalIndex, UInt_t uMbsTime );
      void FinishTriglog( );

      void CreateHistos();
      void FillHistos();
      void WriteHistos( TDirectory* inDir);
      void DeleteHistos();

      void   SetHistoUserAxis( Double_t dRangeIn, Double_t dBinSzIn )
             { fdEvoRangeUser = dRangeIn; fdEvoBinSzUser = dBinSzIn; };

   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofTriglogScalUnpacker(const TTofTriglogScalUnpacker&);
      TTofTriglogScalUnpacker& operator=(const TTofTriglogScalUnpacker&);
      
      TMbsUnpackTofPar * fParUnpack;
      TClonesArray * fScalerBoardCollection;

      UInt_t   fuNbTriglogScal;
      UInt_t   fuTrigOff;

      // Final output
      std::vector< UInt_t >  fuTotalTriggerCount;   //!
      std::vector< UInt_t >  fuFirstMbsTime;        //! first mbs time
      std::vector< UInt_t >  fuLastMbsTime;         //! last mbs time
      std::vector< std::vector< std::vector< UInt_t > > > fuFirstScaler; //! TRIGLOG scaler contents
      std::vector< std::vector< std::vector< UInt_t > > > fuLastScaler;  //! TRIGLOG scaler contents

      // Histograms
      std::vector< std::vector< TH1* > > fhScalers;  //! last value of each scaler = integrated counts

      std::vector< std::vector< std::vector< UInt_t > > > fvuFirstScalers;  //! First value of each scaler <= integrated counts

      std::vector< Double_t > fdFirstMbsTime;        // First point for rate measurement
      std::vector< Double_t > fdPrevMbsTime;         // previous point for rate measurement
      std::vector< Double_t > fdCurrMbsTime;         // current point for rate measurement
      std::vector< UInt_t   > fuLastRefClk;          //! last reference clock number
      std::vector< TH1*     > fhRefClkRate;
      std::vector< TH1*     > fhRefClkRateEvo;

      TClonesArray * fTriglogBoardCollection;
      Double_t fdEvoRangeUser;
      Double_t fdEvoBinSzUser;
      std::vector< std::vector< std::vector< TH1* > > > fhScalersCountsEvo; // [NbBoards][NbScalers][NbChan]

   ClassDef(TTofTriglogScalUnpacker, 1)
};

#endif // TTOFTRIGLOGSCALUNPACKER_H_
