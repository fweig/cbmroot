// ------------------------------------------------------------------
// -----                     TTofVftxUnpacker                   -----
// -----              Created 14/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TTOFVFTXUNPACKER_H_
#define TTOFVFTXUNPACKER_H_

#include "TObject.h"

#include <vector>

class TMbsUnpackTofPar;

class TClonesArray;
class TH1;
class TH2;
class TDirectory;

class TTofVftxUnpacker : public TObject
{
   public:
      TTofVftxUnpacker();
      TTofVftxUnpacker( TMbsUnpackTofPar * parIn );
      ~TTofVftxUnpacker();
      
      virtual void Clear(Option_t *option);
      
      void ProcessVFTX( Int_t num, UInt_t* pMbsData, UInt_t uLength );
      
      void CreateHistos();
      void FillHistos();
      void WriteHistos( TDirectory* inDir);
      void DeleteHistos();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TTofVftxUnpacker(const TTofVftxUnpacker&);
      TTofVftxUnpacker& operator=(const TTofVftxUnpacker&);
      
      TMbsUnpackTofPar * fParUnpack;
      UInt_t   fuNbTdc;
      TClonesArray * fVftxBoardCollection;
      std::vector<Int_t> fiLastFpgaTdcCoarse;
      
      // Histograms
      std::vector<TH1 *> fh1VftxRawChMap;
      std::vector< std::vector<TH1 *> > fh1VftxChFt;
      
   ClassDef(TTofVftxUnpacker, 1)
};

#endif // TTOFTDCUNPACKER_H_
