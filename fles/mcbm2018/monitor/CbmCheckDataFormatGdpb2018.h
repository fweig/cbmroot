// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmCheckDataFormatGdpb2018                          -----
// -----               Created 10.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmCheckDataFormatGdpb2018_H
#define CbmCheckDataFormatGdpb2018_H

#include "Timeslice.hpp"
#include "gDpbMessv100.h"
//#include "CbmTofStarData.h"
//#include "CbmTofStarData2018.h"

#include "CbmMcbmUnpack.h"

#include "TClonesArray.h"
#include "Rtypes.h"

#include <vector>
#include <map>
#include <chrono>

class CbmMcbm2018TofPar;

class TCanvas;
class TH1;
class TH2;
class TProfile;
class TProfile2D;

class CbmCheckDataFormatGdpb2018: public CbmMcbmUnpack {
   public:

      CbmCheckDataFormatGdpb2018();
      virtual ~CbmCheckDataFormatGdpb2018();

      virtual Bool_t Init();

      virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

      virtual void Reset();

      virtual void Finish();

      void SetParContainers();

      Bool_t InitContainers();

      Bool_t ReInitContainers();

      void SetMsLimitLevel(size_t uAcceptBoundaryPct = 100) { fuMsAcceptsPercent = uAcceptBoundaryPct; }
      size_t GetMsLimitLevel() { return fuMsAcceptsPercent; }

      virtual void AddMsComponentToList( size_t component, UShort_t usDetectorId );
      virtual void SetNbMsInTs( size_t uCoreMsNb, size_t uOverlapMsNb );
      void SetIgnoreMsOverlap( Bool_t bEnaFlag = kTRUE ) { fbIgnoreOverlapMs = bEnaFlag; }
      void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
      size_t GetMsOverlap() { return fuOverlapMsNb; }

      inline void SetDiamondDpbIdx( UInt_t uIdx = 2 ) { fuDiamondDpbIdx = uIdx; }

      inline void SetHistoFilename( TString sNameIn ) { fsHistoFilename = sNameIn; }

      void SaveAllHistos( TString sFileName = "" );
      void ResetAllHistos();

      void UseDaqBuffer(Bool_t) {};
   private:
      /// FLES containers
      std::vector< size_t >    fvMsComponentsList; //!
      size_t                   fuNbCoreMsPerTs; //!
      size_t                   fuNbOverMsPerTs; //!
      Bool_t                   fbIgnoreOverlapMs; //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/

      /// OLD, to be cleaned out !!!!!
      size_t   fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
      size_t   fuTotalMsNb;        /** Total nb of MS per link in timeslice **/
      size_t   fuOverlapMsNb;      /** Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
      size_t   fuCoreMs;           /** Number of non overlap MS at beginning of TS **/
      Double_t fdMsSizeInNs;
      Double_t fdTsCoreSizeInNs;
      UInt_t   fuMinNbGdpb;
      UInt_t   fuCurrNbGdpb;

      /** Settings from parameter file **/
      CbmMcbm2018TofPar* fUnpackPar;      //!
      UInt_t fuNrOfGdpbs;           // Total number of GDPBs in the system
      UInt_t fuNrOfFeePerGdpb;      // Number of FEBs per GDPB
      UInt_t fuNrOfGet4PerFee;      // Number of GET4s per FEE
      UInt_t fuNrOfChannelsPerGet4; // Number of channels in each GET4

      UInt_t fuNrOfChannelsPerFee;  // Number of channels in each FEE
      UInt_t fuNrOfGet4;            // Total number of Get4 chips in the system
      UInt_t fuNrOfGet4PerGdpb;     // Number of GET4s per GDPB
      UInt_t fuNrOfChannelsPerGdpb; // Number of channels per GDPB

      UInt_t fuNrOfGbtx;
      UInt_t fuNrOfModules;
      std::vector< Int_t > fviNrOfRpc;
      std::vector< Int_t > fviRpcType;
      std::vector< Int_t > fviRpcSide;
      std::vector< Int_t > fviModuleId;

      const UInt_t kuNbFeePerGbtx  = 5;
      const UInt_t kuNbGbtxPerGdpb = 6;

      /** Control Flags **/
      UInt_t fuDiamondDpbIdx;
      TString fsHistoFilename;

      /** Running indices **/
      uint64_t fulCurrentTsIndex;  // Idx of the current TS
      size_t   fuCurrentMs; // Idx of the current MS in TS (0 to fuTotalMsNb)
      size_t   fuCurrentMsSysId; // SysId of the current MS in TS (0 to fuTotalMsNb)
      Double_t fdMsIndex;   // Time in ns of current MS from its index
      UInt_t   fuGdpbId;    // Id (hex number) of the GDPB for current message
      UInt_t   fuGdpbNr;    // running number (0 to fuNrOfGdpbs) of the GDPB for current message
      UInt_t   fuGet4Id;    // running number (0 to fuNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB for current message
      UInt_t   fuGet4Nr;    // running number (0 to fuNrOfGet4) of the Get4 chip in the system for current message
      Int_t    fiEquipmentId;

      /** Current epoch marker for each GDPB and GET4
        * (first epoch in the stream initializes the map item)
        * pointer points to an array of size fuNrOfGdpbs * fuNrOfGet4PerGdpb
        * The correct array index is calculated using the function
        * GetArrayIndex(gdpbId, get4Id)
        **/
      std::vector< ULong64_t > fvulCurrentEpoch; //!
/*
      std::vector< Bool_t >    fvbFirstEpochSeen; //!
      std::vector< ULong64_t > fvulCurrentEpochCycle; //! Epoch cycle from the Ms Start message and Epoch counter flip
      std::vector< ULong64_t > fvulCurrentEpochFull; //! Epoch + Epoch Cycle
*/
      ULong64_t fulCurrentEpochTime;     /** Time stamp of current epoch **/

      /// Map of ID to index for the gDPBs
      std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

      /// Histograms and histogram control variables
         // Flesnet
      TCanvas* fcMsSizeAll;
      Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
      std::vector< TH1      * > fvhMsSzPerLink;
      std::vector< TProfile * > fvhMsSzTimePerLink;

         // Messages types and flags
            /// In System
      TH1* fhMessType;
      TH1* fhSysMessType;
            /// Per GET4 in system
      TH2* fhGet4MessType;
      TH2* fhGet4ChanScm;
      TH2* fhGet4ChanErrors;
      TH2* fhGet4EpochFlags;
            /// Per Gdpb
      TH2* fhGdpbMessType;
      TH2* fhGdpbSysMessType;
      TH2* fhGdpbSysMessPattType;
      TH2* fhGdpbEpochFlags;
      TH2* fhGdpbEpochSyncEvo;
      TH2* fhGdpbEpochMissEvo;
            /// Per GET4 in gDPB
      std::vector< TH2* > fvhGdpbGet4MessType;
      std::vector< TH2* > fvhGdpbGet4ChanErrors;
            /// Pattern messages per gDPB
      TH2* fhPatternMissmatch;
      TH2* fhPatternEnable;
      TH2* fhPatternResync;

      std::vector< UInt_t >       fvuGdpbNbEpochPerMs;
      std::vector< std::vector< UInt_t > > fvvuChanNbHitsPerMs;
      std::vector< TH1 *        > fhEpochsPerMs_gDPB;
      std::vector< TH2 *        > fhEpochsPerMsPerTs_gDPB;
      std::vector< TH1 *        > fhEpochsDiff_gDPB;
      std::vector< TH2 *        > fhEpochsDiffPerTs_gDPB;
      std::vector< TH2 *        > fhEpochsJumpBitsPre_gDPB;
      std::vector< TH2 *        > fhEpochsJumpBitsNew_gDPB;
      std::vector< TH2 *        > fhEpochsJumpDigitsPre_gDPB;
      std::vector< TH2 *        > fhEpochsJumpDigitsNew_gDPB;
      std::vector< TH2 *        > fhStartEpochPerMs_gDPB;
      std::vector< TH2 *        > fhCloseEpochPerMs_gDPB;
      std::vector< TH2 *        > fhHitsPerMsFirstChan_gDPB;
      std::vector< TProfile2D * > fvhChannelRatePerMs_gDPB;

      /// Canvases
      TCanvas* fcSummary = nullptr;
      std::vector< TCanvas* > fcFormatGdpb;


      void CreateHistograms();

      ///* Periodic histos saving *///
      std::chrono::time_point<std::chrono::system_clock> fTimeLastHistoSaving;

      CbmCheckDataFormatGdpb2018(const CbmCheckDataFormatGdpb2018&);
      CbmCheckDataFormatGdpb2018 operator=(const CbmCheckDataFormatGdpb2018&);

   ClassDef(CbmCheckDataFormatGdpb2018, 1)
};

#endif
