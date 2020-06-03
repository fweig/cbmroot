// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmTofStarMonitorShift2018                     -----
// -----               Created 19.01.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTofStarMonitorShift2018_H
#define CbmTofStarMonitorShift2018_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v2.h"
#include "CbmTofStarData.h"
#include "CbmTofStarData2018.h"

#include "CbmTSUnpack.h"

#include "TClonesArray.h"
#include "Rtypes.h"

#include <vector>
#include <map>
#include <chrono>

class CbmTofStar2018Par;

class TCanvas;
class TH1;
class TH2;
class TProfile;

class CbmTofStarMonitorShift2018: public CbmTSUnpack {
   public:

      CbmTofStarMonitorShift2018();
      virtual ~CbmTofStarMonitorShift2018();

      virtual Bool_t Init();

      virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

      virtual void Reset();

      virtual void Finish();

      void SetParContainers();

      Bool_t InitContainers();

      Bool_t ReInitContainers();

      void FillOutput(boost::any);

      void SetMsLimitLevel(size_t uAcceptBoundaryPct = 100) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
      size_t GetMsLimitLevel() { return fuMsAcceptsPercent; }

      void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
      size_t GetMsOverlap() { return fuOverlapMsNb; }

      inline void SetFitZoomWidthPs( Double_t inZoomWidth = 1000.0 ) { fdFitZoomWidthPs = inZoomWidth; }
      inline void SetHistoryHistoSize( UInt_t inHistorySizeSec = 1800 ) { fuHistoryHistoSize = inHistorySizeSec; }
      inline void SetHistoryHistoSizeLong( UInt_t inHistorySizeMin = 1800 ) { fuHistoryHistoSizeLong = inHistorySizeMin; }

      void SaveAllHistos( TString sFileName = "" );
      void ResetAllHistos();
      void ResetEvolutionHistograms();
      void ResetLongEvolutionHistograms();
      void UpdateNormedFt();
      void UpdateZoomedFit();

   private:

      size_t   fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
      size_t   fuTotalMsNb;        /** Total nb of MS per link in timeslice **/
      size_t   fuOverlapMsNb;      /** Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
      size_t   fuCoreMs;           /** Number of non overlap MS at beginning of TS **/
      Double_t fdMsSizeInNs;
      Double_t fdTsCoreSizeInNs;
      UInt_t   fuMinNbGdpb;
      UInt_t   fuCurrNbGdpb;

      /** Settings from parameter file **/
      CbmTofStar2018Par* fUnpackPar;      //!
      UInt_t fuNrOfGdpbs;           // Total number of GDPBs in the system
      UInt_t fuNrOfFeetPerGdpb;     // Number of FEBs per GDPB
      UInt_t fuNrOfGet4PerFeb;      // Number of GET4s per FEB
      UInt_t fuNrOfChannelsPerGet4; // Number of channels in each GET4

      UInt_t fuNrOfChannelsPerFeet; // Number of channels in each FEET
      UInt_t fuNrOfGet4;            // Total number of Get4 chips in the system
      UInt_t fuNrOfGet4PerGdpb;     // Number of GET4s per GDPB
      UInt_t fuNrOfChannelsPerGdpb; // Number of channels per GDPB


      /** Running indices **/
      uint64_t fulCurrentTsIndex;  // Idx of the current TS
      size_t   fuCurrentMs; // Idx of the current MS in TS (0 to fuTotalMsNb)
      Double_t fdMsIndex;   // Time in ns of current MS from its index
      UInt_t   fuGdpbId;    // Id (hex number) of the GDPB for current message
      UInt_t   fuGdpbNr;    // running number (0 to fuNrOfGdpbs) of the GDPB for current message
      UInt_t   fuGet4Id;    // running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
      UInt_t   fuGet4Nr;    // running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
      Int_t    fiEquipmentId;
      std::vector<int> fviMsgCounter;

      ///* STAR TRIGGER detection *///
      std::vector< ULong64_t > fvulGdpbTsMsb;
      std::vector< ULong64_t > fvulGdpbTsLsb;
      std::vector< ULong64_t > fvulStarTsMsb;
      std::vector< ULong64_t > fvulStarTsMid;
      std::vector< ULong64_t > fvulGdpbTsFullLast;
      std::vector< ULong64_t > fvulStarTsFullLast;
      std::vector< UInt_t    > fvuStarTokenLast;
      std::vector< UInt_t    > fvuStarDaqCmdLast;
      std::vector< UInt_t    > fvuStarTrigCmdLast;

      /** Current epoch marker for each GDPB and GET4
        * (first epoch in the stream initializes the map item)
        * pointer points to an array of size fuNrOfGdpbs * fuNrOfGet4PerGdpb
        * The correct array index is calculated using the function
        * GetArrayIndex(gdpbId, get4Id)
        **/
      std::vector< ULong64_t > fvulCurrentEpoch; //!
      std::vector< Bool_t >    fvbFirstEpochSeen; //!

      ULong64_t fulCurrentEpochTime;     /** Time stamp of current epoch **/

      /// Map of ID to index for the gDPBs
      std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

      /// Buffer for suppressed epoch processing
      std::vector< std::vector < gdpb::Message > > fvmEpSupprBuffer;

      /// Buffer for pulser channels
      std::vector<  Double_t  > fdTsLastPulserHit; // [ fuFeetNr ]

      /// Histograms and histogram control variables
         // Default value for nb bins in Pulser time difference histos
      const UInt_t kuNbBinsDt    = 300;
      Double_t dMinDt;
      Double_t dMaxDt;
         // Default number of FEET per channels histograms
      UInt_t fuNbFeetPlot;
      UInt_t fuNbFeetPlotsPerGdpb;
         // Evolution plots control
      Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
      Double_t fdStartTimeLong; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
      Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
      UInt_t   fuHistoryHistoSize; /** Size in seconds of the evolution histograms **/
      UInt_t   fuHistoryHistoSizeLong; /** Size in minutes of the long evolution histograms **/
         // Pulser plots
      Double_t fdLastRmsUpdateTime;
      Double_t fdFitZoomWidthPs;
         // Flesnet
      TCanvas* fcMsSizeAll;
      std::vector< TH1      * > fvhMsSzPerLink;
      std::vector< TProfile * > fvhMsSzTimePerLink;
         // Messages types and flags
      TH1* fhMessType;
      TH1* fhSysMessType;
      TH2* fhGet4MessType;
      TH2* fhGet4ChanScm;
      TH2* fhGet4ChanErrors;
      TH2* fhGet4EpochFlags;
         // Slow control messages
      TH2* fhScmScalerCounters;
      TH2* fhScmDeadtimeCounters;
      TH2* fhScmSeuCounters;
      TH2* fhScmSeuCountersEvo;
         // Hit messages
            /// TODO: Channel rate plots!
      std::vector< TH2      * > fvhRawFt_gDPB;
      std::vector< TH2      * > fvhRawTot_gDPB;
      std::vector< TH1      * > fvhChCount_gDPB;
      std::vector< TH2      * > fvhChannelRate_gDPB;
      std::vector< TH2      * > fvhRemapTot_gDPB;
      std::vector< TH1      * > fvhRemapChCount_gDPB;
      std::vector< TH2      * > fvhRemapChRate_gDPB;
      std::vector< TH1      * > fvhFeetRate_gDPB;
      std::vector< TH1      * > fvhFeetErrorRate_gDPB;
      std::vector< TProfile * > fvhFeetErrorRatio_gDPB;
      std::vector< TH1      * > fvhFeetRateLong_gDPB;
      std::vector< TH1      * > fvhFeetErrorRateLong_gDPB;
      std::vector< TProfile * > fvhFeetErrorRatioLong_gDPB;

      ///* STAR TRIGGER detection *///
      std::vector< TH1 *      > fvhTokenMsgType;
      std::vector< TH1 *      > fvhTriggerRate;
      std::vector< TH2 *      > fvhCmdDaqVsTrig;
      std::vector< TH2 *      > fvhStarTokenEvo;
      std::vector< TProfile * > fvhStarTrigGdpbTsEvo;
      std::vector< TProfile * > fvhStarTrigStarTsEvo;

      ///* Pulser monitoring *///
      const Double_t kdMaxDtPulserPs = 100e3;
      std::vector< std::vector< TH1 * > > fvhTimeDiffPulser;
      TH2 * fhTimeRmsPulser;
      TH2 * fhTimeRmsZoomFitPuls;
      TH2 * fhTimeResFitPuls;

      void CreateHistograms();

      void FillHitInfo(       gdpb::Message );
      void FillEpochInfo(     gdpb::Message );
      void PrintSlcInfo(      gdpb::Message );
      void PrintSysInfo(      gdpb::Message );
      void PrintGenInfo(      gdpb::Message );
      void FillStarTrigInfo(  gdpb::Message );

      inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id)
      {
         return gdpbId * fuNrOfGet4PerGdpb + get4Id;
      }

      ///* PADI channel to GET4 channel mapping and reverse *///
      std::vector< UInt_t > fvuPadiToGet4;
      std::vector< UInt_t > fvuGet4ToPadi;

      ///* Periodic histos saving *///
      std::chrono::time_point<std::chrono::system_clock> fTimeLastHistoSaving;

      CbmTofStarMonitorShift2018(const CbmTofStarMonitorShift2018&);
      CbmTofStarMonitorShift2018 operator=(const CbmTofStarMonitorShift2018&);

   ClassDef(CbmTofStarMonitorShift2018, 1)
};

#endif
