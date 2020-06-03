// ------------------------------------------------------------------
// -----                    TMbsUnpackTof                       -----
// -----              Created 05/07/2013 by P.-A. Loizeau       -----
// -----     Mbs Data unpacker using the fairroot MBS classes   -----
// ------------------------------------------------------------------
#ifndef _TMBSUNPACKTOF_H_
#define _TMBSUNPACKTOF_H_

// General Fairroot unpack of GSI/MBS event
#include "FairUnpack.h"

// FAIR headers
//#include "FairLogger.h"      // error: field ‘fLastCheck’ has incomplete type otherwise ?!?!?
#include "FairRootManager.h" // error: field ‘fLastCheck’ has incomplete type otherwise ?!?!?

// Parameters
class TMbsUnpackTofPar;

// Unpackers
class TTofOrGenUnpacker;
class TTofScal2014Unpacker;
class TTofScomUnpacker;
class TTofVftxUnpacker;
class TTofTriglogScalUnpacker;

// ROOT
class TClonesArray;
class TH1;
class TH2;
class TString;
class TDatime;

class TMbsUnpackTof : public FairUnpack {
   public:
      TMbsUnpackTof();
      TMbsUnpackTof( Int_t verbose );
      TMbsUnpackTof( Int_t type, Int_t subType, Short_t procId, Int_t verbose = 1,
                       Short_t subCrate = 1, Short_t control = 9 );
      virtual ~TMbsUnpackTof();

      // FairUnpack specific functions
      virtual Bool_t Init();
      virtual Bool_t DoUnpack(Int_t* data, Int_t size);
      virtual void   Reset();
      virtual Bool_t Finish();
   
      void WriteHistogramms();

      void   SetSaveScalers( Bool_t bSaveScal=kTRUE );
      void   SetSaveRawVftx( Bool_t bSaveVftx=kTRUE );
   protected:
      virtual void Register();
    
   private:
      TMbsUnpackTof(const TMbsUnpackTof&);
      TMbsUnpackTof operator=(const TMbsUnpackTof&);
      
      // Parameters
      Int_t   fiVerbosity;
      Bool_t InitParameters();
      TMbsUnpackTofPar    *fMbsUnpackPar;
      
      Bool_t CreateHistogramms();
      Bool_t FillHistograms();
      void DeleteHistograms();
      
      Bool_t RegisterOutput();
      Bool_t CreateUnpackers();
      Bool_t ClearOutput();

      Int_t                  fiNbEvents;
      Int_t                  fiFirstEventNumber;
      Int_t                  fiLastEventNumber;
      TDatime fLastCheck;
      TDatime fCheck;
      
      // Unpackers
      TTofOrGenUnpacker       * fOrGenUnp;
      TTofScal2014Unpacker    * fScal2014Unp;
      TTofScomUnpacker        * fScomUnp;
      TTofVftxUnpacker        * fVftxUnp;
      TTofTriglogScalUnpacker * fTrloScalUnp;
      // Output objects
      TClonesArray         * fScalerBoardCollection;
      TClonesArray         * fVftxBoardCollection;

      Bool_t                 fbSaveScalers;
      Bool_t                 fbSaveRawVftx;
      
      ClassDef(TMbsUnpackTof, 1);
};
#endif // _TMBSUNPACKTOF_H_
