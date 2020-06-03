// ------------------------------------------------------------------
// -----                     TTofScomUnpacker                   -----
// -----              Created 03/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TTOFSCOMUNPACKER_H_
#define TTOFSCOMUNPACKER_H_

#include "TObject.h"

#include <vector>

class TMbsUnpackTofPar;

class TClonesArray;
class TH1;
class TH2;
class TDirectory;

class TTofScomUnpacker : public TObject
{
   public:
      TTofScomUnpacker();
      TTofScomUnpacker( TMbsUnpackTofPar * parIn );
      ~TTofScomUnpacker();
      
      virtual void Clear(Option_t *option);
      
      void ProcessScom( Int_t iScomIndex, UInt_t* pMbsData, UInt_t uLength );
      
      void CreateHistos();
      void FillHistos();
      void WriteHistos( TDirectory* inDir);
      void DeleteHistos();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofScomUnpacker(const TTofScomUnpacker&);
      TTofScomUnpacker& operator=(const TTofScomUnpacker&);
      
      TMbsUnpackTofPar * fParUnpack;
      UInt_t         fuNbScom;
      TClonesArray * fScalerBoardCollection;
      
      // Histograms
      std::vector<TH1 *> fhScalers;  //! last value of each scaler = integrated counts
      
      Bool_t bFirstEvent;
      std::vector< std::vector< UInt_t > > fvuFirstScalers;  //! First value of each scaler <= integrated counts
      
   ClassDef(TTofScomUnpacker, 1)
};

#endif // TTOFSCOMUNPACKER_H_ 
