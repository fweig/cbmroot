// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMcbmRate                      -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMMCBM2018MONITORMCBMRATE_H
#define CBMMCBM2018MONITORMCBMRATE_H

#include "Timeslice.hpp"

// Data
#include "StsXyterMessage.h"
#include "StsXyterFinalHit.h"
#include "gDpbMessv100.h"

// CbmRoot
#include "CbmMcbmUnpack.h"
#include "CbmHistManager.h"

// C++11
#include <chrono>

// C/C++
#include <vector>
#include <set>
#include <map>

class CbmMcbm2018StsPar;

class CbmMcbm2018TofPar;

class CbmMcbm2018MonitorMcbmRate: public CbmMcbmUnpack
{
public:

   CbmMcbm2018MonitorMcbmRate();
   virtual ~CbmMcbm2018MonitorMcbmRate();

   virtual Bool_t Init();

   virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

   virtual void Reset();

   virtual void Finish();

   void SetParContainers();

   Bool_t InitContainers();

   Bool_t ReInitContainers();

   virtual void AddMsComponentToList( size_t component, UShort_t usDetectorId );
   void AddMsComponentToListSts( size_t component );
   void AddMsComponentToListTof( size_t component );

   virtual void SetNbMsInTs( size_t uCoreMsNb, size_t uOverlapMsNb );
   void SetMsOverlap(size_t uOverlapMsNb = 1) { fuNbOverMsPerTs = uOverlapMsNb; }
   size_t GetMsOverlap()                      { return fuNbOverMsPerTs; }
   void SetIgnoreMsOverlap( Bool_t bIgnoreOver = kTRUE ) { fbIgnoreOverlapMs = bIgnoreOver; }

   void ResetAllHistos();
   void SaveAllHistos( TString sFileName = "" );
   void SavePulserHistos( TString sFileName = "" );
   void SetHistoFileName( TString sFileName = "data/SetupHistos.root" ) { fsHistoFileFullname = sFileName; }

   inline void SetDiamondDpbIdx( UInt_t uIdx = 2 ) { fuDiamondDpbIdx = uIdx; }
   inline void SetMuchDpbIdx( UInt_t uIdx = 1 ) { fuMuchDpbIdx = uIdx; }
   inline void SetStsTofOffsetNs( Double_t dOffsetIn = 0.0 ) { fdStsTofOffsetNs = dOffsetIn; }
   inline void SetMuchTofOffsetNs( Double_t dOffsetIn = 0.0 ) { fdMuchTofOffsetNs = dOffsetIn; }

   void UseDaqBuffer(Bool_t) {};
private:
   /// Parameters
      /// FLES containers
   std::vector< size_t >    fvMsComponentsListSts; //!
   std::vector< size_t >    fvMsComponentsListTof; //!
   size_t                   fuNbCoreMsPerTs; //!
   size_t                   fuNbOverMsPerTs; //!
   Bool_t                   fbIgnoreOverlapMs; //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
      /// Unpacking and mapping parameters for STS
   CbmMcbm2018StsPar*        fUnpackParSts;  //!
   UInt_t                    fuStsNrOfDpbs;       //! Total number of Sts DPBs in system
   std::map<UInt_t, UInt_t>  fmStsDpbIdIndexMap;  //! Map of DPB Identifier to DPB index
   UInt_t                    fuMuchDpbIdx;
      /// Unpacking and mapping parameters for TOF
   CbmMcbm2018TofPar*        fUnpackParTof;      //!
   UInt_t                    fuTofNrOfDpbs;         //! Total number of GDPBs in the system
   std::map<UInt_t, UInt_t>  fmTofDpbIdIndexMap;
   UInt_t                    fuDiamondDpbIdx;
      /// Global parameters
   UInt_t                    fuTotalNrOfDpb;
   Double_t                  fdStsTofOffsetNs;
   Double_t                  fdMuchTofOffsetNs;

   // Constants
   static const UInt_t   kuStsBytesPerMessage = 4; //! TODO => move to the message class!!
   static const UInt_t   kuTofBytesPerMessage = 8;
   static const UInt_t   kuTimeBinSizeNs =    10000;
   static const UInt_t   kuTsSizeInNs    = 10400000;
   static const UInt_t   kuNbTimeBins    = kuTsSizeInNs / kuTimeBinSizeNs;

   // Internal Control/status of monitor
      // Histo File name and path
   TString fsHistoFileFullname;
      // Task configuration values
   Bool_t                fbPrintMessages;
   stsxyter::MessagePrintMask fPrintMessCtrlSts;
      // TS/MS info
   ULong64_t             fulCurrentTsIdx;
   ULong64_t             fulCurrentMsIdx;
   ULong64_t             fulCurrentTsStartTime;
      // Current data properties
   std::map< stsxyter::MessType, UInt_t > fmMsgCounter;
   UInt_t                fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
   UInt_t                fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
   UInt_t                fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
   Int_t                 fiRunStartDateTimeSec; //! Start of run time since "epoch" in s, for the plots with date as X axis
   Int_t                 fiBinSizeDatePlots;    //! Bin size in s for the plots with date as X axis

/****************** STS Sync ******************************************/
      // Data format control
   std::vector< ULong64_t > fvulStsCurrentTsMsb;                   //! Current TS MSB for each DPB
   std::vector< UInt_t    > fvuStsCurrentTsMsbCycle;               //! Current TS MSB cycle for DPB

      // Hits time-sorting
   std::vector< std::vector< stsxyter::FinalHit > > fvmStsSdpbHitsInMs; //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted with "<" operator
   std::vector< std::vector< stsxyter::FinalHit > > fvmStsSdpbHitsInTs; //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted with "<" operator

   /// Processing methods
   void CreateStsHistograms();

   Bool_t ProcessStsMs( const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx );

   void FillStsHitInfo(   stsxyter::Message mess, const UInt_t & uMsIdx );
   void FillStsTsMsbInfo( stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
   void FillStsEpochInfo( stsxyter::Message mess );
/****************** STS Sync ******************************************/

/****************** TOF Sync ******************************************/

   /// Running indices
   uint64_t fulTofCurrentTsIndex;  // Idx of the current TS
   size_t   fuTofCurrentMs; // Idx of the current MS in TS (0 to fuTotalMsNb)
   Double_t fdTofMsIndex;   // Time in ns of current MS from its index
   UInt_t   fuTofGdpbId;    // Id (hex number) of the GDPB for current message
   UInt_t   fuTofGdpbNr;    // running number (0 to fuNrOfGdpbs) of the GDPB for current message
   Int_t    fiTofEquipmentId;
   std::vector<int> fviTofMsgCounter;

   /** Current epoch marker for each GDPB and GET4
     * (first epoch in the stream initializes the map item)
     * pointer points to an array of size fuNrOfGdpbs * fuNrOfGet4PerGdpb
     * The correct array index is calculated using the function
     * GetArrayIndex(gdpbId, get4Id)
     **/
   std::vector< ULong64_t > fvulTofCurrentEpoch; //!
   std::vector< ULong64_t > fvulTofCurrentEpochCycle; //!
   std::vector< ULong64_t > fvulTofCurrentEpochFull; //!

   ULong64_t fulTofCurrentEpochTime;     /** Time stamp of current epoch **/

   /// Buffer for suppressed epoch processing
   std::vector< std::vector < gdpbv100::Message > > fvmTofEpSupprBuffer;

   /// Buffer for system sync check
   std::vector< std::vector< gdpbv100::FullMessage > > fvmTofGdpbHitsInMs;
   std::vector< std::vector< gdpbv100::FullMessage > > fvmTofGdpbHitsInTs;

   std::vector< UInt_t > fvuHitsNbInTimeBinSts;
   std::vector< UInt_t > fvuHitsNbInTimeBinMuch;
   std::vector< UInt_t > fvuHitsNbInTimeBinTof;
   std::vector< UInt_t > fvuHitsNbInTimeBinDiam;

   /// Processing methods
   void CreateTofHistograms();

   Bool_t ProcessTofMs( const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx );

   void FillTofEpochCycle( uint64_t ulCycleData );
   void FillTofHitInfo(      gdpbv100::Message );
   void FillTofEpochInfo(    gdpbv100::Message );
/****************** TOF Sync ******************************************/

/****************** mCBM Sync *****************************************/
   TH2 * fhMcbmHitsNbPerTsEvo;
   TH2 * fhMcbmHitsNbFineEvo;
   TH2 * fhMcbmHitsRateEvo;

   TH2 * fhDiamondHitsRateMapEvo;
   TH1 * fhDiamondHitsRateDerivative;

   TH2 * fhMuchVsStsHitsNbPerTimeBin;
   TH2 * fhTofVsStsHitsNbPerTimeBin;
   TH2 * fhDiamVsStsHitsNbPerTimeBin;

   TH2 * fhStsVsMuchHitsNbPerTimeBin;
   TH2 * fhTofVsMuchHitsNbPerTimeBin;
   TH2 * fhDiamVsMuchHitsNbPerTimeBin;

   TH2 * fhStsVsTofHitsNbPerTimeBin;
   TH2 * fhMuchVsTofHitsNbPerTimeBin;
   TH2 * fhDiamVsTofHitsNbPerTimeBin;

   TH2 * fhStsVsDiamHitsNbPerTimeBin;
   TH2 * fhTofVsDiamHitsNbPerTimeBin;
   TH2 * fhMuchVsDiamHitsNbPerTimeBin;

   void CreateMcbmHistograms();
/****************** mCBM Sync *****************************************/


   CbmMcbm2018MonitorMcbmRate(const CbmMcbm2018MonitorMcbmRate&);
   CbmMcbm2018MonitorMcbmRate operator=(const CbmMcbm2018MonitorMcbmRate&);

   ClassDef(CbmMcbm2018MonitorMcbmRate, 1)
};

#endif // CBMMCBM2018MONITORMCBMSYNC_H
