// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorAlgoT0                         -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018MonitorAlgoT0_H
#define CbmMcbm2018MonitorAlgoT0_H

#include "CbmStar2019Algo.h"

// Data
#include "gDpbMessv100.h"
#include "CbmTofDigi.h"

// CbmRoot

// C++11
#include <chrono>

// C/C++
#include <vector>
#include <map>

class CbmMcbm2018TofPar;
/*
class TCanvas;
class THttpServer;
*/
class TH1;
class TH2;
class TProfile;

class CbmMcbm2018MonitorAlgoT0 : public CbmStar2019Algo<CbmTofDigi>
{
   public:
      CbmMcbm2018MonitorAlgoT0();
      ~CbmMcbm2018MonitorAlgoT0();

      virtual Bool_t Init();
      virtual void Reset();
      virtual void Finish();

      Bool_t InitContainers();
      Bool_t ReInitContainers();
      TList* GetParList();

      Bool_t InitParameters();

      Bool_t ProcessTs( const fles::Timeslice& ts );
      Bool_t ProcessTs( const fles::Timeslice& ts, size_t /*component*/ ) { return ProcessTs( ts ); }
      Bool_t ProcessMs( const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx );

      void AddMsComponentToList( size_t component, UShort_t usDetectorId );

      Bool_t CreateHistograms();
      Bool_t FillHistograms();
      Bool_t ResetHistograms( Bool_t bResetTime = kTRUE );

      inline void SetMonitorMode( Bool_t bFlagIn = kTRUE ) { fbMonitorMode = bFlagIn; }
      inline void SetHistoryHistoSize( UInt_t inHistorySizeSec = 1800 ) { fuHistoryHistoSize = inHistorySizeSec; }
      inline void SetPulserTotLimits( UInt_t uMin, UInt_t uMax ) { fuMinTotPulser = uMin; fuMaxTotPulser = uMax; }
      inline void SetSpillThreshold( UInt_t uCntLimit ) { fuOffSpillCountLimit = uCntLimit; }
      inline void SetChannelMap( UInt_t uChan0, UInt_t uChan1, UInt_t uChan2, UInt_t uChan3,
                                 UInt_t uChan4, UInt_t uChan5, UInt_t uChan6, UInt_t uChan7 )
                  {
                     fuDiamChanMap[ 0 ] = uChan0;
                     fuDiamChanMap[ 1 ] = uChan1;
                     fuDiamChanMap[ 2 ] = uChan2;
                     fuDiamChanMap[ 3 ] = uChan3;
                     fuDiamChanMap[ 4 ] = uChan4;
                     fuDiamChanMap[ 5 ] = uChan5;
                     fuDiamChanMap[ 6 ] = uChan6;
                     fuDiamChanMap[ 7 ] = uChan7;
                  }

   private:
      /// Control flags
      Bool_t fbMonitorMode;      //! Switch ON the filling of a minimal set of histograms
      Bool_t fbDebugMonitorMode; //! Switch ON the filling of a additional set of histograms
      std::vector< Bool_t >    fvbMaskedComponents;

      /// Settings from parameter file
      CbmMcbm2018TofPar* fUnpackPar;      //!
         /// Readout chain dimensions and mapping
      UInt_t fuNrOfGdpbs;           //! Total number of GDPBs in the system
      std::map<UInt_t, UInt_t> fGdpbIdIndexMap; //! gDPB ID to index map
      UInt_t fuNrOfFeePerGdpb;      //! Number of FEBs per GDPB
      UInt_t fuNrOfGet4PerFee;      //! Number of GET4s per FEE
      UInt_t fuNrOfChannelsPerGet4; //! Number of channels in each GET4
      UInt_t fuNrOfChannelsPerFee;  //! Number of channels in each FEE
      UInt_t fuNrOfGet4;            //! Total number of Get4 chips in the system
      UInt_t fuNrOfGet4PerGdpb;     //! Number of GET4s per GDPB
      UInt_t fuNrOfChannelsPerGdpb; //! Number of channels per GDPB

      /// User settings: Data correction parameters
      UInt_t fuMinTotPulser;
      UInt_t fuMaxTotPulser;
      UInt_t fuOffSpillCountLimit;

      /// Constants
      static const Int_t    kiMaxNbFlibLinks  = 32;
      static const UInt_t   kuBytesPerMessage =  8;
      static const UInt_t   kuNbChanDiamond = 8;

      /// Running indices
         /// TS/MS info
      ULong64_t             fulCurrentTsIdx;
      ULong64_t             fulCurrentMsIdx;
      Double_t              fdTsStartTime;         //! Time in ns of current TS from the index of the first MS first component
      Double_t              fdTsStopTimeCore;      //! End Time in ns of current TS Core from the index of the first MS first component
      Double_t              fdMsTime;              //! Start Time in ns of current MS from its index field in header
      UInt_t                fuMsIndex;             //! Index of current MS within the TS
         /// Current data properties
      std::map< gdpbv100::MessageTypes, UInt_t > fmMsgCounter;
      UInt_t                fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
      UInt_t                fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
      UInt_t                fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
      Int_t                 fiRunStartDateTimeSec; //! Start of run time since "epoch" in s, for the plots with date as X axis
      Int_t                 fiBinSizeDatePlots;    //! Bin size in s for the plots with date as X axis
      UInt_t                fuGet4Id;              //! running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
      UInt_t                fuGet4Nr;              //! running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
         /// Data format control: Current time references for each GDPB: merged epoch marker, epoch cycle, full epoch [fuNrOfGdpbs]
      std::vector< ULong64_t > fvulCurrentEpoch;      //! Current epoch index, per DPB
      std::vector< ULong64_t > fvulCurrentEpochCycle; //! Epoch cycle from the Ms Start message and Epoch counter flip
      std::vector< ULong64_t > fvulCurrentEpochFull;  //! Epoch + Epoch Cycle

         /// Starting state book-keeping
      Double_t              fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
      Double_t              fdStartTimeMsSz;       /** Time of first microslice, used as reference for evolution plots**/
      std::chrono::steady_clock::time_point ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

      /// Buffers
      std::vector< std::vector< gdpbv100::Message > >    fvvmEpSupprBuffer;         //! [DPB]
      std::vector< gdpbv100::FullMessage > fvmHitsInMs; //! All hits (time in bins, TOT in bins, asic, channel) in last MS, sorted with "<" operator

      /// Histograms related variables
      UInt_t   fuHistoryHistoSize; /** Size in seconds of the evolution histograms **/

      /// Histograms
         /// Channel rate plots
      std::vector< UInt_t > fvuHitCntChanMs;
      std::vector< UInt_t > fvuErrorCntChanMs;
      std::vector< UInt_t > fvuEvtLostCntChanMs;
      std::vector< TH1      * > fvhMsgCntEvoChan;
      std::vector< TH2      * > fvhMsgCntPerMsEvoChan;
      std::vector< TH1      * > fvhHitCntEvoChan;
      std::vector< TH2      * > fvhHitCntPerMsEvoChan;
      std::vector< TH1      * > fvhErrorCntEvoChan;
      std::vector< TH2      * > fvhErrorCntPerMsEvoChan;
      std::vector< TH1      * > fvhEvtLostCntEvoChan;
      std::vector< TH2      * > fvhEvtLostCntPerMsEvoChan;
      std::vector< TProfile * > fvhErrorFractEvoChan;
      std::vector< TH2      * > fvhErrorFractPerMsEvoChan;
      std::vector< TProfile * > fvhEvtLostFractEvoChan;
      std::vector< TH2      * > fvhEvtLostFractPerMsEvoChan;
         /// Channels map
      Bool_t   fbSpillOn;
      UInt_t   fuCurrentSpillIdx;
      UInt_t   fuCurrentSpillPlot;
      Double_t fdStartTimeSpill;
      Double_t fdLastSecondTime;
      UInt_t   fuCountsLastSecond;
      static const UInt_t kuNbSpillPlots = 5;
//      UInt_t  kuDiamChanMap[ kuNbChanDiamond ] = { 2, 3, 4, 5, 0, 1, 6, 7 }; //! Map from electronics channel to Diamond strip
      UInt_t  fuDiamChanMap[ kuNbChanDiamond ] = { 0, 1, 2, 3, 4, 5, 6, 7 }; //! Map from electronics channel to Diamond strip
      TH1      * fhChannelMap;
      TH2      * fhHitMapEvo;
      TH2      * fhHitTotEvo;
      TH2      * fhChanHitMapEvo;
      std::vector< TH1      * > fvhChannelMapSpill;
      TH1      * fhHitsPerSpill;
         /// Global Rate
      TH1      * fhMsgCntEvo;
      TH1      * fhHitCntEvo;
      TH1      * fhErrorCntEvo;
      TH1      * fhLostEvtCntEvo;
      TProfile * fhErrorFractEvo;
      TProfile * fhLostEvtFractEvo;

      TH2      * fhMsgCntPerMsEvo;
      TH2      * fhHitCntPerMsEvo;
      TH2      * fhErrorCntPerMsEvo;
      TH2      * fhLostEvtCntPerMsEvo;
      TH2      * fhErrorFractPerMsEvo;
      TH2      * fhLostEvtFractPerMsEvo;

            /// Pulser
      TH1      * fhChannelMapPulser;
      TH2      * fhHitMapEvoPulser;

      /// Canvases
      TCanvas * fcSummary;
      TCanvas * fcHitMaps;
      TCanvas * fcGenCntsPerMs;
      TCanvas * fcSpillCounts;
      TCanvas * fcSpillCountsHori;

/*
      void ProcessEpochCycle( uint64_t ulCycleData );
      void ProcessEpoch(       gdpbv100::Message mess );

      void ProcessEpSupprBuffer();

      void ProcessHit(     gdpbv100::FullMessage mess );
      void ProcessSysMess( gdpbv100::FullMessage mess );

      void ProcessError(   gdpbv100::FullMessage mess );

      inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id)
      {
         return gdpbId * fuNrOfGet4PerGdpb + get4Id;
      }
*/
      CbmMcbm2018MonitorAlgoT0(const CbmMcbm2018MonitorAlgoT0&);
      CbmMcbm2018MonitorAlgoT0 operator=(const CbmMcbm2018MonitorAlgoT0&);

      ClassDef(CbmMcbm2018MonitorAlgoT0, 1)
};

#endif
