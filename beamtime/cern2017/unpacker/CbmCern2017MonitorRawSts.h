// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCern2017MonitorRawSts                         -----
// -----                Created 25/07/17  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMCERN2017MONITORRAWSTS_H
#define CBMCERN2017MONITORRAWSTS_H

#include "Timeslice.hpp"

// Data
#include "StsXyterRawMessage.h"

// CbmRoot
#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

// C++11
#include <chrono>

// C/C++
#include <vector>
#include <map>

class CbmCern2017UnpackParSts;

class CbmCern2017MonitorRawSts: public CbmTSUnpack
{
public:

   CbmCern2017MonitorRawSts();
   virtual ~CbmCern2017MonitorRawSts();

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
                         stsxyter::RawMessagePrintMask ctrl = stsxyter::RawMessagePrintMask::msg_print_Hex |
                                                              stsxyter::RawMessagePrintMask::msg_print_Crc |
                                                              stsxyter::RawMessagePrintMask::msg_print_Human )
                        { fbPrintRawMessages = bPrintMessOn; fPrintMessCtrl = ctrl; }
   void EnableChanHitDtPlot( Bool_t bEnable = kTRUE ) { fbChanHitDtEna = bEnable; }
   void SetPulserChannels( UInt_t uAsicA, UInt_t uChanA, UInt_t uAsicB, UInt_t uChanB,
                           UInt_t uAsicC, UInt_t uChanC, UInt_t uAsicD, UInt_t uChanD,
                           UInt_t uMaxNbMicroslices = 100 );
   void SetLongDurationLimits( UInt_t uDurationSeconds = 3600, UInt_t uBinSize = 1 );

private:
   size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/

   // Parameters
   CbmCern2017UnpackParSts* fUnpackPar; //!
   UInt_t                   fuNrOfDpbs;       //! Total number of Sts DPBs in system
   std::map<UInt_t, UInt_t> fDpbIdIndexMap;   //! Map of DPB Identifier to DPB index
   UInt_t                   fuNbElinksPerDpb; //! Number of possible eLinks per DPB
   UInt_t                   fuNbStsXyters;    //! Number of StsXyter ASICs
   UInt_t                   fuNbChanPerAsic;  //! Number of channels per StsXyter ASIC => should be constant somewhere!!!!
   std::vector< std::vector< UInt_t > > fvuElinkToAsic;   //! Vector holding for each link the corresponding ASIC index [fuNrOfDpbs * fuNbElinksPerDpb]

   // Constants
   static const Int_t    kiMaxNbFlibLinks = 16;
   static const UInt_t   kuBytesPerRawMessage = 8;

   // Internal Control/status of monitor
      // Task configuration values
   Bool_t                fbPrintRawMessages;
   stsxyter::RawMessagePrintMask fPrintMessCtrl;
   Bool_t                fbChanHitDtEna;
      // Current data properties
   std::map< stsxyter::RawMessType, UInt_t > fmMsgCounter;
   UInt_t                fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
   UInt_t                fuCurrDpbId; //! Temp holder until Current equipment ID is properly filled in MS
   UInt_t                fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
   Int_t                 fiRunStartDateTimeSec; //! Start of run time since "epoch" in s, for the plots with date as X axis
   Int_t                 fiBinSizeDatePlots;    //! Bin size in s for the plots with date as X axis
   std::vector< std::vector< UInt_t > > fvuCurrentTsMsb;       //! Current TS MSB for each eLink
   std::vector< std::vector< UInt_t > > fvuCurrentTsMsbCycle;  //! Current TS MSB cycle for each eLink
   std::vector< std::vector< UInt_t > > fvuCurrentTsMsbOver;   //! Current TS MSB overlap bits for each eLink
   std::vector< std::vector< ULong64_t > > fvulChanLastHitTime; //! Last hit time in bins for each Channel
   std::vector< std::vector<Double_t> > fvdChanLastHitTime;    //! Last hit time in ns   for each Channel
   std::vector< std::vector< std::vector< UInt_t > > > fvuChanNbHitsInMs;    //! Number of hits in each MS for each Channel
   std::vector< std::vector< std::vector< Double_t > > > fvdChanLastHitTimeInMs; //! Last hit time in bins in each MS for each Channel
   std::vector< std::vector< std::vector< UShort_t > > > fvusChanLastHitAdcInMs; //! Last hit ADC in bins in each MS for each Channel
      // Starting state book-keeping
   Double_t              fdStartTime;           /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
   Double_t              fdStartTimeMsSz;       /** Time of first microslice, used as reference for evolution plots**/
   std::chrono::steady_clock::time_point ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

   // Historgrams
   CbmHistManager* fHM;                 //! Histogram manager
   TH1* fhStsMessType;
   TH1* fhStsSysMessType;
   TH2* fhStsMessTypePerDpb;
   TH2* fhStsSysMessTypePerDpb;

   std::vector<TH1*> fhStsChanCounts;
   std::vector<TH2*> fhStsChanRawAdc;
   std::vector<TProfile*> fhStsChanRawAdcProf;
   std::vector<TH2*> fhStsChanRawTs;
   std::vector<TH2*> fhStsChanMissEvt;
   std::vector<TH2*> fhStsChanOverDiff;
   std::vector<TH2*> fhStsChanHitRateEvo;
   std::vector<TH1*> fhStsXyterRateEvo;
   std::vector<TH2*> fhStsChanHitRateEvoLong;
   std::vector<TH1*> fhStsXyterRateEvoLong;
   std::vector<TH2*> fhStsChanHitDt;
   std::vector<TH2*> fhStsChanHitDtNeg;
   std::vector<TH2*> fhStsChanHitsPerMs;
   std::vector<TH2*> fhStsChanSameMs;
   std::vector<TProfile2D*> fpStsChanSameMsTimeDiff;
   std::vector<TH2*> fhStsChanSameMsTimeDiff;

   Bool_t fbPulserTimeDiffOn;
   UInt_t fuPulserMaxNbMicroslices;
   std::vector<UInt_t> fvuPulserAsic;
   std::vector<UInt_t> fvuPulserChan;
   std::vector<TH1*> fhStsPulserChansTimeDiff;

   TH2* fhStsAsicTsMsb;
   TH2* fhStsAsicTsMsbMaj;
   std::vector<TH2*> fhStsElinkTsMsbCrc;
   std::vector<TH2*> fhStsElinkTsMsbMaj;

   Bool_t fbLongHistoEnable;
   UInt_t fuLongHistoNbSeconds;
   UInt_t fuLongHistoBinSizeSec;
   UInt_t fuLongHistoBinNb;
   std::vector<TH1*> fhFebRateEvoLong;
   std::vector<TH2*> fhFebChRateEvoLong;
/*
   std::vector<TH1*> fhFebRateEvoDate;
*/

   TCanvas*  fcMsSizeAll;
   TH1*      fhMsSz[kiMaxNbFlibLinks];
   TProfile* fhMsSzTime[kiMaxNbFlibLinks];

   void CreateHistograms();

   void FillHitInfo(   stsxyter::RawMessage mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx, const UInt_t & uMsIdx );
   void FillTsMsbInfo( stsxyter::RawMessage mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx );

   CbmCern2017MonitorRawSts(const CbmCern2017MonitorRawSts&);
   CbmCern2017MonitorRawSts operator=(const CbmCern2017MonitorRawSts&);

   ClassDef(CbmCern2017MonitorRawSts, 1)
};

#endif // CBMCERN2017MONITORRAWSTS_H
