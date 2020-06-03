// ------------------------------------------------------------------
// -----                    TGet4UnpackTof                      -----
// -----              Created 24/07/2013 by P.-A. Loizeau       -----
// -----    Get4 Data unpacker using the fairroot MBS classes   -----
// ------------------------------------------------------------------
#ifndef _TGET4UNPACKTOF_H_
#define _TGET4UNPACKTOF_H_

// General Fairroot unpack of GSI/MBS event
#include "FairUnpack.h"

// Old Go4 includes
   // ROC
#include "roc/Message.h"
#include "roc/Board.h"
#include "roc/Iterator.h"

// SubEvent ProcId from ROC library
#include "commons.h"

// FAIR headers
#include "FairRootManager.h"

// Parameters
class TMbsUnpackTofPar;

// Unpackers
class TTofGet4Unpacker;

// ROOT
class TClonesArray;
class TH1;
class TH2;
class TString;

class TGet4UnpackTof : public FairUnpack {
   public:
      TGet4UnpackTof();
      TGet4UnpackTof( Int_t verbose );
      TGet4UnpackTof( Int_t type, Int_t subType, Short_t sRocId = 0, Int_t verbose = 1,
                        Short_t procId = roc3587::proc_RocEvent, Short_t control = 
roc::formatOptic2);
      virtual ~TGet4UnpackTof();

      // FairUnpack specific functions
      virtual Bool_t Init();
      virtual Bool_t DoUnpack(Int_t* data, Int_t size);
      virtual void   Reset();
      virtual Bool_t Finish();

      void WriteHistogramms();

   protected:
      virtual void Register();

   private:
      TGet4UnpackTof(const TGet4UnpackTof&);
      TGet4UnpackTof operator=(const TGet4UnpackTof&);

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

      // Unpackers
      TTofGet4Unpacker    * fGet4Unp;
      // Output objects
      TClonesArray        * fGet4BoardCollection;

      ClassDef(TGet4UnpackTof, 1);
};
#endif // _TGET4UNPACKTOF_H_
