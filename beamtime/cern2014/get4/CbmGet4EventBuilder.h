// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmGet4EventBuilder                          -----
// -----                    Created 23.02.2015 by                          -----
// -----                        P.-A. Loizeau                              -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMGET4EVENTBUILDER_H
#define CBMGET4EVENTBUILDER_H

// Specific headers
#include "CbmGet4v1xHackDef.h"

// FAIR/CBMROOT headers
#include "FairTask.h"

// ROOT headers
#include "TTimeStamp.h"

// C++ std headers
#include <vector>
#include <set>

// Enable debug online histos for multiplicity trigger
#define MUL_TRIGG_DEBUG

// 3 epochs max in use: Prev. Ep., Curr. Ep., Next Ep.
const Int_t kiMaxEpochInUse   = 3;

class CbmGet4EpochBuffer;
class CbmGet4EventBuffer;
class TH1;

#ifdef MUL_TRIGG_DEBUG
#include "TCanvas.h"
class TH2;
#endif // MUL_TRIGG_DEBUG

class CbmGet4EventBuilder : public FairTask
{
   public:
      CbmGet4EventBuilder();
      CbmGet4EventBuilder(const char* name, Int_t verbose = 1);
      virtual ~CbmGet4EventBuilder();

      virtual InitStatus Init();
      virtual void Exec(Option_t* option);
      virtual void Finish();

      // Setters
         // Common
      void SetMaxTimePerLoop( Double_t dTimeLimitIn ) { fdMaxTimePerLoopS = dTimeLimitIn; }
      void SetTriggerMode(    Bool_t   bTrigModeIn );
      void SetTriggerWinNeg(  Double_t dWinNeg, Int_t iTrigger = 0 );
      void SetTriggerWinPos(  Double_t dWinPos, Int_t iTrigger = 0 );
      void SetTriggerDeadT(   Double_t dDeadT,  Int_t iTrigger = 0 );
      void SetTriggerType(    Int_t    iType,   Int_t iTrigger = 0 );
         // Multiplicity trigger
      void SetMultiThreshold( Double_t dMulThrIn)  { fiMultiThreshold = dMulThrIn; }
      void SetMultiHistBinSzClk( Int_t iMulHistBin){ fiMultiHistBinSz = iMulHistBin; }
         // Channel Coincidence trigger
      void SetNbTriggers( Int_t iNbTriggers );
      void SetTrigNbComponents( Int_t iTrigIndex, Int_t iNbComp );
      void SetTrigCompChan( Int_t iTrigIndex, Int_t iCompIndex, Int_t iChIndex);
      void SetCoincWin( Int_t iTrigIndex, Double_t dCoincWin);

      void SetHistoFilename( TString sNameIn );

      // Getters
      Double_t GetMaxTimePerLoop() const { return fdMaxTimePerLoopS; }
      Bool_t   GetTriggerMode()    const { return fbTriggerMode; }
      Double_t GetTriggerWinNeg( Int_t iTrigger = 0 );
      Double_t GetTriggerWinPos( Int_t iTrigger = 0 );
      Double_t GetTriggerDeadT(  Int_t iTrigger = 0 );
      Int_t    GetTriggerType(   Int_t iTrigger = 0 );
         // Multiplicity trigger
      Double_t GetMultiThreshold() const { return fiMultiThreshold; }
      Int_t    GetMultiHistBin()   const { return fiMultiHistBinSz; }
         // Channel Coincidence trigger
      Int_t    GetNbTriggers();
      Int_t    GetTrigNbComponents( Int_t iTrigIndex );
      Int_t    GetTrigCompChan( Int_t iTrigIndex, Int_t iCompIndex );
      Double_t GetCoincWin( Int_t iTrigIndex );

   protected:

   private:
      void     ResizeTriggerVectors();
      Int_t    PrevEpId();
      Int_t    CurrEpId();
      Int_t    NextEpId();

      Bool_t   RegisterInputs();
      Bool_t   RegisterOutputs();

      Bool_t   CreateHistos();
      Bool_t   FillHistos();
      Bool_t   WriteHistos();
      Bool_t   DeleteHistos();

      CbmGet4EventBuilder(const CbmGet4EventBuilder&);
      CbmGet4EventBuilder operator=(const CbmGet4EventBuilder&);

      // Class Members: Input and output
      UInt_t              fuInputEvents; // Number of processed input "events"
      UInt_t              fuOutputEvents; // Number of processed output "events"
      UInt_t              fuInputEpochs; // Number of received input epochs
      CbmGet4EpochBuffer* fInputBuffer;  // Epoch buffer
      CbmGet4EventBuffer* fOutputBuffer; // Event buffer
      std::multiset< get4v1x::FullMessage >* fpEpoch[kiMaxEpochInUse]; //! Pointers on epochs in use
      Int_t               fiCurrentEpoch;// Which of the 3 epoch is in use

      // Control
      Double_t   fdMaxTimePerLoopS;
      TTimeStamp fStart;
      TTimeStamp fStop;
      TString fsHistoFilename;

      Bool_t     fbTriggerMode; // kFALSE => Hit Multi, kTRUE => Channels Coinc.
      /************* Common event definition params ****************/
         // Number of defined triggers (1 if multiplicity, 1 to N if coinc)
      Int_t                                    fiNbTriggers;
         // Trigger window past    [-]: save all hits (h) with [-] < t(h) - t(T)
      std::vector< Double_t >                  fvdTrigWinNeg;
         // Trigger window future  [+]: save all hits (h) with [+] > t(h) - t(T)
      std::vector< Double_t >                  fvdTrigWinPos;
         // Deadtime [d]: individual deadtime for each trigger, prevent new events closer than [d]
            // could be later used with trigger type to define priorities
      std::vector< Double_t >                  fvdTrigDeadtime;
         // Trigger type: would possibly allow priority or trigger merging in later stage
      std::vector< Int_t >                     fviTrigType;
         // Epoch in which the first possible hit in a new event can be found
         // 3 possible values: -1 = previous epoch, 0 = current epoch, 1 = next epoch
         // This hit is defined as the first hit for which in latest trigger we have:
         //           t(1) + [d] - [-] < t(h)
         // OBSOLETE if trigger priority is implemented
      Int_t                                    fiEpochStartHitScan;
         // Index within epoch vector of the first possible hit in new event
         // This hit is defined as the first hit for which in latest trigger we have:
         //           t(1) + [d] - [-] < t(h)
         // OBSOLETE if trigger priority is implemented
      Int_t                                    fiIndexStartHitScan;
      /*************************************************************/

      /***************** Hit multiplicity trigger ******************/
      TH1*  fhHitMultiplicity; // Hit multiplicity evolution within current epoch
      Int_t fiMultiHistBinSz;  // Bin size for the Multi histo in clock cycles
      Int_t fiNbBinsMultiHist; // Number of bins in multiplicity histogram
      Int_t fiMultiThreshold;  // Threshold above which an event is "found"
      TH1*  fhMaxMulPerEpochDist; // max Hit multiplicity/bin in each epoch
      TH1*  fhEventSizeDist; // nb Hits per event
#ifdef MUL_TRIGG_DEBUG
      TCanvas* canvasDeb;
      TH2*  fhMaxMulEpoch; // max Hit multiplicity/bin in each epoch
      TH2*  fhBinThrEpoch; // bin over threshold in each epoch
      TH2*  fhMaxEvtSizeEpoch; // max event size in each epoch
      TH2*  fhEvtSizeEvo;  // Evolution of event size distribution VS epoch/1000
#endif // MUL_TRIGG_DEBUG
      /*************************************************************/

      /*************** Channel coincidence trigger *****************/
      // Event definition parameters
         // Trigger components =
         // - 1 "Main" channel(1) giving Trigger time (T)
         // - n "other" channels (2)
      std::vector< std::vector < UInt_t > >    fvvuTriggerComp;
         // Coincidence window => max dt = |t(1) - t(2)|, in ps
      std::vector< Double_t >                  fvdCoincWin;

      // Event definition variables
         // Index of last trigger found
      UInt_t                                   fuLastTriggerId;
         // Flag indicating if a trigger component was found since last occurrence of its trigger
      std::vector< std::vector < Bool_t > >    fvvbTrigCompFound;
         // Epoch of the last hit for each trigger component, 0 if not found yet
      std::vector< std::vector < ULong64_t > > fvvulTrigCompEp;
         // Coarse counter of the last hit for each trigger component, 0 if not found yet
      std::vector< std::vector < UInt_t > >    fvvuTrigCompTs;
      /*************************************************************/

   ClassDef(CbmGet4EventBuilder, 1)
};

#endif // CBMGET4EVENTBUILDER_H
