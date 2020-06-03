// ------------------------------------------------------------------
// -----                     TMbsUnpTofMonitor                  -----
// -----              Created 16/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TMBSUNPTOFMONITOR_H_
#define TMBSUNPTOFMONITOR_H_

#include "FairTask.h"

#include <vector>

// Parameters
class TMbsUnpackTofPar;
class TMbsCalibTofPar;

// Unpackers
class TTofTriglogUnpacker;
class TTofScal2014Unpacker;
class TTofScomUnpacker;
class TTofTriglogScalUnpacker;
class TTofVftxUnpacker;
class TTofTrbTdcUnpacker;

// ROOT
class TClonesArray;
class TH1;
class TH2;
class TDirectory;
//class TTimeStamp;
#include "TTimeStamp.h"

class TMbsUnpTofMonitor : public FairTask
{
   public:
      TMbsUnpTofMonitor();
      TMbsUnpTofMonitor(const char* name, Int_t verbose = 1);
      ~TMbsUnpTofMonitor();

      // Fairtask specific functions
      virtual void SetParContainers();
      virtual InitStatus Init();
      virtual InitStatus ReInit();
      virtual void Exec(Option_t* option);
      virtual void Finish();

      void SetScalHistoUserAxis( Double_t dRangeIn, Double_t dBinSzIn )
             { fdScalersEvoRangeUser = dRangeIn; fdScalersEvoBinSzUser = dBinSzIn; };

   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TMbsUnpTofMonitor(const TMbsUnpTofMonitor&);
      TMbsUnpTofMonitor& operator=(const TMbsUnpTofMonitor&);
      
      // Unpackers
      Bool_t CreateUnpackers();
      
      // Histograms, including Task own histos
      Bool_t CreateHistogramms();
      Bool_t FillHistograms();
      Bool_t WriteHistogramms();
      Bool_t DeleteHistograms();
      
      // Input 
//      Bool_t RegisterInput();
      
      // Parameters
      Bool_t InitParameters();
      TMbsUnpackTofPar    *fMbsUnpackPar;
      TMbsCalibTofPar     *fMbsCalibPar;
      
      // Unpackers
      TTofTriglogUnpacker   * fTriglogUnp;
      TTofScomUnpacker      * fScomUnp;
      TTofScal2014Unpacker  * fScal2014Unp;
      TTofTriglogScalUnpacker * fTrloScalUnp;
      TTofVftxUnpacker      * fVftxUnp;
      TTofTrbTdcUnpacker    * fTrbTdcUnp;
      
      // Output objects
      TClonesArray        * fTriglogBoardCollection;
      TClonesArray        * fScalerBoardCollection;
      TClonesArray        * fVftxBoardCollection;
      TClonesArray        * fTrbTdcBoardCollection;
      
      // Events statistics
      Int_t                 fiNbEvents;
      Int_t                 fiFirstEventNumber;
      Int_t                 fiLastEventNumber;
      TTimeStamp fFirstCheck;
      TTimeStamp fLastCheck;
      TTimeStamp fCheck;

      Double_t fdScalersEvoRangeUser;
      Double_t fdScalersEvoBinSzUser;

   ClassDef(TMbsUnpTofMonitor, 1)
};

#endif // TMBSUNPTOFMONITOR_H_
