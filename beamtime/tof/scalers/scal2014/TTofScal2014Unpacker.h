// ------------------------------------------------------------------
// -----                     TTofScal2014Unpacker                   -----
// -----              Created 19/04/2014 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TTOFSCAL2014UNPACKER_H_
#define TTOFSCAL2014UNPACKER_H_

#include "TObject.h"

#include <vector>

class TMbsUnpackTofPar;

class TClonesArray;
class TH1;
class TH2;
class TDirectory;

class TTofScal2014Unpacker : public TObject
{
   public:
      TTofScal2014Unpacker();
      TTofScal2014Unpacker( TMbsUnpackTofPar * parIn );
      ~TTofScal2014Unpacker();
      
      virtual void Clear(Option_t *option);
      
      void ProcessScal2014( Int_t iScalIndex, UInt_t* pMbsData, UInt_t uLength );
      
      void CreateHistos();
      void FillHistos();
      void WriteHistos( TDirectory* inDir);
      void DeleteHistos();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofScal2014Unpacker(const TTofScal2014Unpacker&);
      TTofScal2014Unpacker& operator=(const TTofScal2014Unpacker&);

      TMbsUnpackTofPar * fParUnpack;
      UInt_t         fuNbScal2014;
      TClonesArray * fScalerBoardCollection;
      
      // Histograms
      std::vector<TH1 *> fhScalers;     //! last value of each scaler = integrated counts
      std::vector<TH1 *> fhScalersAnd;  //! last value of each AND scaler = integrated counts
      
      Bool_t bFirstEvent;
      std::vector< std::vector< UInt_t > > fvuFirstScalers;  //! First value of each scaler <= integrated counts
      std::vector< std::vector< UInt_t > > fvuFirstScalersAnd;  //! First value of each AND scaler <= integrated counts
      
   ClassDef(TTofScal2014Unpacker, 1)
};

#endif // TTOFSCAL2014UNPACKER_H_
