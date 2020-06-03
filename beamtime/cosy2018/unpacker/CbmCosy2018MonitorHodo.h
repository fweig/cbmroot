// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2018MonitorHodo                        -----
// -----                Created 15/02/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMCOSY2018MONITORHODO_H
#define CBMCOSY2018MONITORHODO_H

#include "Timeslice.hpp"

// Data
#include "StsXyterMessage.h"
#include "StsXyterFinalHit.h"

// CbmRoot
#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

// C++11
#include <chrono>

// C/C++
#include <vector>
#include <set>
#include <map>

class CbmCern2017UnpackParHodo;

class CbmCosy2018MonitorHodo: public CbmTSUnpack
{
public:

   CbmCosy2018MonitorHodo();
   virtual ~CbmCosy2018MonitorHodo();

   virtual Bool_t Init();

   virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

   virtual void Reset();

   virtual void Finish();

   void SetParContainers();

   Bool_t InitContainers();

   Bool_t ReInitContainers();

   void FillOutput(boost::any);

   void SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5 );

   void ResetAllHistos();
   void SaveAllHistos( TString sFileName = "" );

   void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
   size_t GetMsOverlap()                      { return fuOverlapMsNb; }

   void SetPrintMessage( Bool_t bPrintMessOn = kTRUE,
                         stsxyter::MessagePrintMask ctrl = stsxyter::MessagePrintMask::msg_print_Hex |
                                                           stsxyter::MessagePrintMask::msg_print_Human )
                        { fbPrintMessages = bPrintMessOn; fPrintMessCtrl = ctrl; }
   void SetLongDurationLimits( UInt_t uDurationSeconds = 3600, UInt_t uBinSize = 1 );
   void SetCoincidenceBorder( Double_t dNewValue ){ fdCoincBorder = dNewValue;}

private:
   size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/

   // Parameters
   CbmCern2017UnpackParHodo* fUnpackPar; //!
   UInt_t                   fuNrOfDpbs;       //! Total number of Sts DPBs in system
   std::map<UInt_t, UInt_t> fDpbIdIndexMap;   //! Map of DPB Identifier to DPB index
   UInt_t                   fuNbElinksPerDpb; //! Number of possible eLinks per DPB
   UInt_t                   fuNbStsXyters;    //! Number of StsXyter ASICs
   UInt_t                   fuNbChanPerAsic;  //! Number of channels per StsXyter ASIC => should be constant somewhere!!!!
   std::vector< std::vector< UInt_t > > fvuElinkToAsic;   //! Vector holding for each link the corresponding ASIC index [fuNrOfDpbs * fuNbElinksPerDpb]

   // Constants
   static const Int_t    kiMaxNbFlibLinks = 16;
   static const UInt_t   kuBytesPerMessage = 4;

   // Internal Control/status of monitor
      // Task configuration values
   Bool_t                fbPrintMessages;
   stsxyter::MessagePrintMask fPrintMessCtrl;
      // TS/MS info
   ULong64_t             fulCurrentTsIdx;
   ULong64_t             fulCurrentMsIdx;
      // Current data properties
   std::map< stsxyter::MessType, UInt_t > fmMsgCounter;
   UInt_t                fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
   UInt_t                fuCurrDpbId; //! Temp holder until Current equipment ID is properly filled in MS
   UInt_t                fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
   Int_t                 fiRunStartDateTimeSec; //! Start of run time since "epoch" in s, for the plots with date as X axis
   Int_t                 fiBinSizeDatePlots;    //! Bin size in s for the plots with date as X axis

      // Data format control
   std::vector< ULong64_t > fvulCurrentTsMsb;                   //! Current TS MSB for each DPB
   std::vector< UInt_t    > fvuCurrentTsMsbCycle;               //! Current TS MSB cycle for DPB
   std::vector< UInt_t    > fvuElinkLastTsHit;                  //! TS from last hit for DPB

   std::vector< std::vector< ULong64_t > > fvulChanLastHitTime;   //! Last hit time in bins for each Channel
   std::vector< std::vector< Double_t  > > fvdChanLastHitTime;    //! Last hit time in ns   for each Channel
   std::vector< Double_t >                               fvdMsTime;                  //! Header time of each MS
   std::vector< std::vector< std::vector< UInt_t > > >   fvuChanNbHitsInMs;          //! Number of hits in each MS for each Channel
   std::vector< std::vector< std::vector< Double_t > > > fvdChanLastHitTimeInMs;     //! Last hit time in bins in each MS for each Channel
   std::vector< std::vector< std::vector< UShort_t > > > fvusChanLastHitAdcInMs;     //! Last hit ADC in bins in each MS for each Channel
//   std::vector< std::vector< std::multiset< stsxyter::FinalHit > > > fvmChanHitsInTs; //! All hits (time & ADC) in bins in last TS for each Channel
      // Starting state book-keeping
   Double_t              fdStartTime;           /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
   Double_t              fdStartTimeMsSz;       /** Time of first microslice, used as reference for evolution plots**/
   std::chrono::steady_clock::time_point ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

      // Hits time-sorting
   std::multiset< stsxyter::FinalHit > fvmHitsInTs; //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted by multiset with "<" operator
   stsxyter::FinalHit fLastSortedHit1X; //! Last sorted hit for Hodo 1 X
   stsxyter::FinalHit fLastSortedHit1Y; //! Last sorted hit for Hodo 1 Y
   stsxyter::FinalHit fLastSortedHit2X; //! Last sorted hit for Hodo 2 X
   stsxyter::FinalHit fLastSortedHit2Y; //! Last sorted hit for Hodo 2 Y
      // Coincidence histos
   UInt_t fuMaxNbMicroslices;
      // Rate evolution histos
   Bool_t fbLongHistoEnable;
   UInt_t fuLongHistoNbSeconds;
   UInt_t fuLongHistoBinSizeSec;
   UInt_t fuLongHistoBinNb;

   // Histograms
   CbmHistManager* fHM;                 //! Histogram manager
   TH1* fhHodoMessType;
   TH1* fhHodoSysMessType;
   TH2* fhHodoMessTypePerDpb;
   TH2* fhHodoSysMessTypePerDpb;
/*
   TH2* fhStsDpbRawTsMsb;
   TH2* fhStsDpbRawTsMsbSx;
   TH2* fhStsDpbRawTsMsbDpb;
*/
   TH2* fhHodoMessTypePerElink;
   TH2* fhHodoSysMessTypePerElink;

   std::vector<TH1 *> fhHodoChanCntRaw;
   std::vector<TH2 *> fhHodoChanAdcRaw;
   std::vector<TProfile*> fhHodoChanAdcRawProf;
   std::vector<TH2*> fhHodoChanRawTs;
   std::vector<TH2*> fhHodoChanMissEvt;
   std::vector<TH2*> fhHodoChanHitRateEvo;
   std::vector<TH1*> fhHodoFebRateEvo;
   std::vector<TH2*> fhHodoChanHitRateEvoLong;
   std::vector<TH1*> fhHodoFebRateEvoLong;

   TH1 * fhHodoChanCounts1X;
   TH1 * fhHodoChanCounts1Y;
   TH1 * fhHodoChanCounts2X;
   TH1 * fhHodoChanCounts2Y;
   TH2 * fhHodoChanAdcRaw1X;
   TH2 * fhHodoChanAdcRaw1Y;
   TH2 * fhHodoChanAdcRaw2X;
   TH2 * fhHodoChanAdcRaw2Y;
   TH2 * fhHodoChanHitRateEvo1X;
   TH2 * fhHodoChanHitRateEvo1Y;
   TH2 * fhHodoChanHitRateEvo2X;
   TH2 * fhHodoChanHitRateEvo2Y;
   TH1 * fhHodoRateEvo1X;
   TH1 * fhHodoRateEvo1Y;
   TH1 * fhHodoRateEvo2X;
   TH1 * fhHodoRateEvo2Y;

      // Coincidences in sorted hits
   Double_t fdCoincBorder; // ns, +/-
   TH1 * fhHodoSortedDtX1Y1;
   TH1 * fhHodoSortedDtX2Y2;
   TH1 * fhHodoSortedDtX1X2;
   TH1 * fhHodoSortedDtY1Y2;
   TH1 * fhHodoSortedDtX1Y2;
   TH1 * fhHodoSortedDtY1X2;
   TH2 * fhHodoSortedMapX1Y1;
   TH2 * fhHodoSortedMapX2Y2;
   TH2 * fhHodoSortedMapX1X2;
   TH2 * fhHodoSortedMapY1Y2;
   TH2 * fhHodoSortedMapX1Y2;
   TH2 * fhHodoSortedMapY1X2;
   TH1 * fhHodoSortedCntEvoX1Y1;
   TH1 * fhHodoSortedCntEvoX2Y2;
   TH1 * fhHodoSortedCntEvoX1X2;
   TH1 * fhHodoSortedCntEvoY1Y2;
   TH1 * fhHodoSortedCntEvoX1Y2;
   TH1 * fhHodoSortedCntEvoY1X2;

   TCanvas*  fcMsSizeAll;
   TH1*      fhMsSz[kiMaxNbFlibLinks];
   TProfile* fhMsSzTime[kiMaxNbFlibLinks];

   void CreateHistograms();

   void FillHitInfo(   stsxyter::Message mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx, const UInt_t & uMsIdx );
   void FillTsMsbInfo( stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
   void FillEpochInfo( stsxyter::Message mess );

   CbmCosy2018MonitorHodo(const CbmCosy2018MonitorHodo&);
   CbmCosy2018MonitorHodo operator=(const CbmCosy2018MonitorHodo&);

   ClassDef(CbmCosy2018MonitorHodo, 1)
};

#endif // CBMCOSY2018MONITORHODO_H
