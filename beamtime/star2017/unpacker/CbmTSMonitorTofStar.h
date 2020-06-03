// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSMonitorTofStar                        -----
// -----               Created 15.12.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTSMonitorTofStar_H
#define CbmTSMonitorTofStar_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"
#include "CbmTofStarData.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

#include "TClonesArray.h"
#include "Rtypes.h"

#include <vector>
#include <map>
#include <chrono>

class CbmTofUnpackPar;

#ifdef STAR_SUBEVT_BUILDER
   /*
    ** Function to send sub-event block to the STAR DAQ system
    *       trg_word received is packed as:
    *
    *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
   */
   extern "C" int star_rhicf_write(unsigned int trg_word, void *dta, int bytes);
#endif // STAR_SUBEVT_BUILDER

class CbmTSMonitorTofStar: public CbmTSUnpack {
  public:

    CbmTSMonitorTofStar();
    virtual ~CbmTSMonitorTofStar();

    virtual Bool_t Init();

    virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

    virtual void Reset();

    virtual void Finish();

    void SetParContainers();

    Bool_t InitContainers();

    Bool_t ReInitContainers();

    void FillOutput(boost::any);

    void SetMsLimitLevel(size_t uAcceptBoundaryPct = 100) {
      fuMsAcceptsPercent = uAcceptBoundaryPct;
    }
    size_t GetMsLimitLevel() {
      return fuMsAcceptsPercent;
    }

    void SetMsOverlap(size_t uOverlapMsNb = 1) {
      fuOverlapMsNb = uOverlapMsNb;
    }
    size_t GetMsOverlap() {
      return fuOverlapMsNb;
    }

    void SetPerTsSpillOnThr( Int_t iThrIn = 10 ) { fiSpillOnThr = iThrIn; }
    void SetPerTsSpillOffThr( Int_t iThrIn = 3 ) { fiSpillOffThr = iThrIn; }
    void SetTsNbSpillOffThr( Int_t iThrIn = 10 ) { fiTsUnderOffThr = iThrIn; }

    void SetEpochSuppressedMode( Bool_t bEnable = kTRUE ) { fbEpochSuppModeOn = bEnable; }
    void SetStarSortAndCutMode( Bool_t bEnable = kTRUE ) { fbStarSortAndCutMode = bEnable; }

    void SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5 );

    inline void SetGet4Mode24b( Bool_t inGet4Mode24b = kTRUE ) { fbGet4M24b = inGet4Mode24b; }
    inline void SetGet4v20Mode( Bool_t inGet4v20Mode = kTRUE ) { fbGet4v20 = inGet4v20Mode; }
    inline void SetMergedEpochs( Bool_t inMergedEpochs = kTRUE ) { fbMergedEpochsOn = inMergedEpochs; }
    inline void SetPulserMode( Bool_t inPulserMode = kTRUE ) { fbPulserMode = inPulserMode; SetPulserChans(); }
    inline void SetPulserFee( UInt_t inPulserGdpb, UInt_t inPulserFee ) { fuPulserGdpb = inPulserGdpb; fuPulserFee = inPulserFee; }
           void SetPulserChans( UInt_t inPulserChanA =  0, UInt_t inPulserChanB =  1, UInt_t inPulserChanC =  2,
                                UInt_t inPulserChanD =  3, UInt_t inPulserChanE =  4, UInt_t inPulserChanF =  5,
                                UInt_t inPulserChanG =  6, UInt_t inPulserChanH =  7, UInt_t inPulserChanI =  8,
                                UInt_t inPulserChanJ =  9, UInt_t inPulserChanK = 10, UInt_t inPulserChanL = 11,
                                UInt_t inPulserChanM = 12, UInt_t inPulserChanN = 13, UInt_t inPulserChanO = 14,
                                UInt_t inPulserChanP = 15, UInt_t inPulserChanQ = 16, UInt_t inPulserChanR = 17 );
    inline void SetFitZoomWidthPs( Double_t inZoomWidth = 1000.0 ) { fdFitZoomWidthPs = inZoomWidth; }
    inline void SetHistoryHistoSize( UInt_t inHistorySizeSec = 1800 ) { fuHistoryHistoSize = inHistorySizeSec; }
    inline void SetHistoryHistoSizeLong( UInt_t inHistorySizeMin = 1800 ) { fuHistoryHistoSizeLong = inHistorySizeMin; }

    void SaveAllHistos( TString sFileName = "" );
    void ResetAllHistos();
    void CyclePulserFee();
    void UpdateNormedFt();
    void UpdateZoomedFit();

  private:

    size_t fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
    size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
    UInt_t fuMinNbGdpb;
    UInt_t fuCurrNbGdpb;

    UInt_t fNrOfGdpbs;           // Total number of GDPBs in the system
    UInt_t fNrOfFebsPerGdpb;     // Number of FEBs per GDPB
    UInt_t fNrOfGet4PerFeb;      // Number of GET4s per FEB
    UInt_t fNrOfChannelsPerGet4; // Number of channels in each GET4

    UInt_t fNrOfChannelsPerFeet; // Number of channels in each FEET
    UInt_t fNrOfGet4;            // Total number of Get4 chips in the system
    UInt_t fNrOfGet4PerGdpb;     // Number of GET4s per GDPB
    UInt_t fuNbChannelsPerGdpb; // Number of channels in each gDPB

    Int_t fiCountsLastTs;
    Int_t fiSpillOnThr;
    Int_t fiSpillOffThr;
    Int_t fiTsUnderOff;
    Int_t fiTsUnderOffThr;
    Double_t fdDetLastTime;
    Double_t fdDetTimeLastTs;
    Bool_t fbSpillOn;
    UInt_t fSpillIdx;

    Bool_t fbEpochSuppModeOn;
    std::vector< std::vector < ngdpb::Message > > fvmEpSupprBuffer;

    UInt_t fGdpbId; // Id (hex number)of the GDPB which is read from the message
    UInt_t fGdpbNr; // running number (0 to fNrOfGdpbs) of the GDPB in the
    UInt_t fGet4Id; // running number (0 to fNrOfGet4PerGdpb) of the Get4 chip of a unique GDPB
    UInt_t fGet4Nr; // running number (0 to fNrOfGet4) of the Get4 chip in the system

    std::vector<int> fMsgCounter;

    std::map<UInt_t, UInt_t> fGdpbIdIndexMap;

    CbmHistManager* fHM;  ///< Histogram manager

    /** Current epoch marker for each GDPB and GET4
     * (first epoch in the stream initializes the map item)
     * pointer points to an array of size fNrOfGdpbs * fNrOfGet4PerGdpb
     * The correct array index is calculated using the function
     * GetArrayIndex(gdpbId, get4Id)
     **/
    Long64_t* fCurrentEpoch; //!

    Int_t fNofEpochs; /** Current epoch marker for each ROC **/
    ULong_t fCurrentEpochTime; /** Time stamp of current epoch **/
    Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
    Double_t fdStartTimeLong; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
    Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
    TCanvas* fcMsSizeAll;

    /** Used only if the channel rate or pulse hit difference plots are enabled **/
    /** Last Hit time for each gDPB/GET4/Channel **/
    std::vector< std::vector< std::vector< Double_t > > > fTsLastHit; // * 6.25 ns

    Int_t fEquipmentId;
    Double_t fdMsIndex;

    CbmTofUnpackPar* fUnpackPar;      //!

    TH1* fHistMessType;
    TH1* fHistSysMessType;
    TH2* fHistGet4MessType;
    TH2* fHistGet4ChanScm;
    TH2* fHistGet4ChanErrors;
    TH2* fHistGet4EpochFlags;
    TH2* fHistSpill;
    TH1* fHistSpillLength;
    TH1* fHistSpillCount;
    TH2* fHistSpillQA;

    TH2* fhScmScalerCounters;
    TH2* fhScmDeadtimeCounters;
    TH2* fhScmSeuCounters;
    TH2* fhScmSeuCountersEvo;

    Double_t fdTimeLastStartMessage;
    TH1* fhScmStartMessDist;
    TH1* fhScmStartMessEvo;

    std::vector<TH2*> fRaw_Tot_gDPB;
    std::vector<TH1*> fChCount_gDPB;
    std::vector<TH2*> fChannelRate_gDPB;
    std::vector<TH1*> fFeetRate_gDPB;
    std::vector<TH1*> fFeetErrorRate_gDPB;
    UInt_t            fuHistoryHistoSize;

    std::vector<TH1*> fFeetRateLong_gDPB;
    std::vector<TH1*> fFeetErrorRateLong_gDPB;
    UInt_t            fuHistoryHistoSizeLong;

    std::vector<TH1*> fFeetRateDate_gDPB;
    Int_t             fiRunStartDateTimeSec;
    Int_t             fiBinSizeDatePlots;

    Double_t fdFirstMsIndex;
    std::vector<Bool_t> fbFirstEpochInMsFound;
    std::vector<TH1*> fRealMsFineQa_gDPB;
    std::vector<TProfile*> fRealMsMidQa_gDPB;
    std::vector<TProfile*> fRealMsCoarseQa_gDPB;

    ///* ASIC coincidences & offsets mapping *///
/*
    ULong64_t fulLastMsIdx;
    Bool_t    fbHitsInLastTs;
    std::vector< std::vector< Long64_t > > fvulHitEpochBuffLastTs;  //! Dims: [gDPB][hits]
    std::vector< TH2 * >                   fvhCoincOffsetEpochGet4; //! Dims: [gDPB - 1]
*/

    ///* STAR TRIGGER detection *///
    ULong64_t fulGdpbTsMsb;
    ULong64_t fulGdpbTsLsb;
    ULong64_t fulStarTsMsb;
    ULong64_t fulStarTsMid;
    ULong64_t fulGdpbTsFullLast;
    ULong64_t fulStarTsFullLast;
    UInt_t    fuStarTokenLast;
    UInt_t    fuStarDaqCmdLast;
    UInt_t    fuStarTrigCmdLast;
    TH1 *     fhTokenMsgType;
    TH1 *     fhTriggerRate;
    TH2 *     fhCmdDaqVsTrig;
    TH2 *     fhStarTokenEvo;

    ///* STAR and pulser monitoring *///
    static const UInt_t kuNbChanTest = 18;
    Bool_t fbGet4M24b;
    Bool_t fbGet4v20;
    Bool_t fbMergedEpochsOn;
    Bool_t fbPulserMode;
    UInt_t fuPulserGdpb;
    UInt_t fuPulserFee;
    UInt_t fuPulserChan[kuNbChanTest]; //! Always in first gDPB !!!
    std::vector<TH1 *> fhTimeDiffPulserChosenFee;
    std::vector<TH1 *> fhTimeDiffPulserChosenChPairs;
    TH2 * fhTimeRmsPulserChosenFee;
    TH1 * fhTimeRmsPulserChosenChPairs;
    Double_t fdLastRmsUpdateTime;
    Double_t fdFitZoomWidthPs;
    TH2 * fhTimeRmsZoomPulsChosenFee;
    TH1 * fhTimeRmsZoomFitPulsChosenChPairs;
    TH2 * fhTimeResFitPulsChosenFee;
    TH1 * fhTimeResFitPulsChosenChPairs;
    std::vector<TH2 *> fhFtDistribPerCh;
    std::vector<TH1*>  fChCountFall_gDPB;
    std::vector<TH2 *> fhFtDistribPerChFall;
    std::vector<TH1*>  fSelChFtNormDnlRise;
    std::vector<TH1*>  fSelChFtNormDnlFall;
    std::vector<TH1*>  fFtNormDnlMinRise;
    std::vector<TH1*>  fFtNormDnlMaxRise;
    std::vector<TH1*>  fFtNormDnlMinFall;
    std::vector<TH1*>  fFtNormDnlMaxFall;
    TH1 *              fhTempHistInlRise;
    TH1 *              fhTempHistInlFall;
    std::vector<TH1*>  fSelChFtNormInlRise;
    std::vector<TH1*>  fSelChFtNormInlFall;
    std::vector<TH1*>  fFtNormInlMinRise;
    std::vector<TH1*>  fFtNormInlMaxRise;
    std::vector<TH1*>  fFtNormInlMinFall;
    std::vector<TH1*>  fFtNormInlMaxFall;
    std::vector< std::vector<Int_t> > fviFtLastRise24b;
    std::vector< std::vector<Int_t> > fviFtLastFall24b;
    std::vector< std::vector<Double_t> > fvdTimeLastRise24b;
    std::vector< std::vector<Double_t> > fvdTimeLastFall24b;
    UInt_t fuRiseFallChSel;
    std::vector<TH2 *> fhFtLastRiseCurrFall;
    std::vector<TH2 *> fhFtCurrRiseLastFall;
    std::vector<TH2 *> fhFtLastRiseDistRise;
    std::vector<TH2 *> fhFtLastRiseDistFall;

    ///* STAR event building/cutting *///
    Bool_t  fbStarSortAndCutMode;
    std::vector< UInt_t >    fuStarActiveAsicMask;
    std::vector< Double_t >  fdStarTriggerDelay;
    std::vector< Double_t >  fdStarTriggerWinSize;
    std::vector< UInt_t >    fuCurrentEpGdpb;
    std::vector< UInt_t >    fuStarCurrentEpFound;
    std::vector< UInt_t >    fuStarNextBufferUse;
    std::vector< Double_t >  fdStarLastTrigTimeG;
    std::vector< Int_t >     fiStarBuffIdxPrev;
    std::vector< Int_t >     fiStarBuffIdxCurr;
    std::vector< Int_t >     fiStarBuffIdxNext;
    std::vector< ULong64_t > fuLastTriggerFullTs;
    std::vector< std::vector< std::vector < ngdpb::Message > > >    fvGdpbEpMsgBuffer; //! Dims: [gDPB][buffer][msgs], 1 buff. for Prev, Curr and Next
    std::vector< std::vector< std::vector < ngdpb::Message > > >    fvGdpbEpHitBuffer; //! Dims: [gDPB][buffer][msgs], 1 buff. for Prev, Curr and Next
    std::vector< std::vector< std::vector < CbmTofStarTrigger > > > fvGdpbEpTrgBuffer; //! Dims: [gDPB][buffer][trig], 1 buff. for Prev, Curr and Next
    CbmTofStarSubevent      fStarSubEvent;
    std::vector<TH2*> fhStarEpToTrig_gDPB;
    std::vector<TH1*> fhStarHitToTrigAll_gDPB;
    std::vector<TH1*> fhStarHitToTrigWin_gDPB;
    std::vector<TH1*> fhStarEventSize_gDPB;
    std::vector<TH2*> fhStarEventSizeTime_gDPB;

    void CreateHistograms();

    void FillHitInfo(ngdpb::Message);
    void FillEpochInfo(ngdpb::Message);
    void PrintSlcInfo(ngdpb::Message);
    void PrintSysInfo(ngdpb::Message);
    void PrintGenInfo(ngdpb::Message);
    void FillStarTrigInfo(ngdpb::Message);
    void FillTrigEpochInfo(ngdpb::Message);

    Bool_t StarSort( Int_t iGdpbIdx );
    Bool_t StarSelect( Int_t iGdpbIdx );
    Bool_t StarGenEmptyEvt( Int_t iGdpbIdx, CbmTofStarTrigger triggerIn );
    ULong64_t fulNbStarEvent;
    ULong64_t fulNbStarEventLastPrintout;
    std::chrono::time_point<std::chrono::system_clock> fTimeLastPrintoutNbStarEvent;

    inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id) {
      return gdpbId * fNrOfGet4PerGdpb + get4Id;
    }

    CbmTSMonitorTofStar(const CbmTSMonitorTofStar&);
    CbmTSMonitorTofStar operator=(const CbmTSMonitorTofStar&);

  ClassDef(CbmTSMonitorTofStar, 1)
};

#endif
