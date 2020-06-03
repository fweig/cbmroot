// ------------------------------------------------------------------
// -----                    TTriglogUnpackTof                   -----
// -----              Created 07/07/2013 by P.-A. Loizeau       -----
// ----- Triglog Data unpacker using the fairroot MBS classes   -----
// ------------------------------------------------------------------
#ifndef _TTRIGLOGUNPACKTOF_H_
#define _TTRIGLOGUNPACKTOF_H_

// General Fairroot unpack of GSI/MBS event
#include "FairUnpack.h"

// FAIR headers
//#include "FairLogger.h"      // error: field ‘fLastCheck’ has incomplete type otherwise ?!?!?
#include "FairRootManager.h" // error: field ‘fLastCheck’ has incomplete type otherwise ?!?!?

// Parameters
class TMbsUnpackTofPar;

// Unpackers
class TTofTriglogUnpacker;

// ROOT
class TClonesArray;
class TH1;
class TH2;
class TString;
class TDatime;

class TTriglogUnpackTof : public FairUnpack {
   public:
      TTriglogUnpackTof();
      TTriglogUnpackTof( Int_t verbose );
      TTriglogUnpackTof( Int_t type, Int_t subType, Short_t procId, Int_t verbose = 1,
                            Short_t subCrate = 1, Short_t control = 9 );
      virtual ~TTriglogUnpackTof();

      // FairUnpack specific functions
      virtual Bool_t Init();
      virtual Bool_t DoUnpack(Int_t* data, Int_t size);
      virtual void   Reset();
      virtual Bool_t Finish();
   
      void WriteHistogramms();

      void   SetSaveTriglog( Bool_t bSaveTrlo=kTRUE );
      void   SetSaveScalers( Bool_t bSaveScal=kTRUE );
   protected:
      virtual void Register();
    
   private:
      TTriglogUnpackTof(const TTriglogUnpackTof&);
      TTriglogUnpackTof operator=(const TTriglogUnpackTof&);
      
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

      Int_t                 fiNbEvents;
      Int_t                 fiFirstEventNumber;
      Int_t                 fiLastEventNumber;
      TDatime fLastCheck;
      TDatime fCheck;
      
      // Unpackers
      TTofTriglogUnpacker * fTriglogUnp;
      // Output objects
      TClonesArray        * fTriglogBoardCollection;
      TClonesArray        * fScalerBoardCollection;
      Bool_t                fbSaveTriglogBoard;
      Bool_t                fbSaveScalerBoards;
      
      ClassDef(TTriglogUnpackTof, 2);
};
#endif // _TTRIGLOGUNPACKTOF_H_
