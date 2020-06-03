// ------------------------------------------------------------------
// -----                     TTofOrGenUnpacker                   -----
// -----              Created 22/11/2015 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TTOFORGENUNPACKER_H_
#define TTOFORGENUNPACKER_H_

#include "TObject.h"

#include <vector>

class TMbsUnpackTofPar;

class TClonesArray;
class TH1;
class TH2;
class TDirectory;

class TTofOrGenUnpacker : public TObject
{
   public:
      TTofOrGenUnpacker();
      TTofOrGenUnpacker( TMbsUnpackTofPar * parIn );
      ~TTofOrGenUnpacker();
      
      virtual void Clear(Option_t *option);
      
      void ProcessOrGen( Int_t iScalIndex, UInt_t* pMbsData, UInt_t uLength );
      
      void CreateHistos();
      void FillHistos();
      void WriteHistos( TDirectory* inDir);
      void DeleteHistos();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofOrGenUnpacker(const TTofOrGenUnpacker&);
      TTofOrGenUnpacker& operator=(const TTofOrGenUnpacker&);

      TMbsUnpackTofPar * fParUnpack;
      UInt_t         fuNbOrGen;
      TClonesArray * fScalerBoardCollection;
      
      // Histograms
      std::vector<TH1 *> fhScalers;     //! last value of each scaler = integrated counts
      std::vector<TH1 *> fhScalersEclo;  //! last value of each Eclo scaler = integrated counts
      std::vector<TH1 *> fhScalersLemo;  //! last value of each Lemo scaler = integrated counts
      
      Bool_t bFirstEvent;
      std::vector< std::vector< UInt_t > > fvuFirstScalers;  //! First value of each scaler <= integrated counts
      std::vector< std::vector< UInt_t > > fvuFirstScalersEclo;  //! First value of each Eclo scaler <= integrated counts
      std::vector< std::vector< UInt_t > > fvuFirstScalersLemo;  //! First value of each Lemo scaler <= integrated counts
      
   ClassDef(TTofOrGenUnpacker, 1)
};

#endif // TTOFORGENUNPACKER_H_
