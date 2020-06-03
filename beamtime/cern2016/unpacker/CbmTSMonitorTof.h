#// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSMonitorTof                            -----
// -----               Created 11.11.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmTSMonitorTof_H
#define CbmTSMonitorTof_H

#include "Timeslice.hpp"
#include "rocMess_wGet4v1.h"

#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

#include "TClonesArray.h"
#include "Rtypes.h"

#include <vector>
#include <map>

class CbmTofUnpackPar;

class CbmTSMonitorTof: public CbmTSUnpack {
  public:

    CbmTSMonitorTof();
    virtual ~CbmTSMonitorTof();

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

    void SetDiamondChannels( UInt_t uGdpb = 0, UInt_t uFeet = 2, UInt_t uChannelA = 78,
                            UInt_t uChannelB = 70, UInt_t uChannelC = 94, UInt_t uChannelD = 86 );
    void SetDiamondPerTsSpillOnThr( Int_t iThrIn = 10 ) {  fiDiamSpillOnThr = iThrIn; }
    void SetDiamondPerTsSpillOffThr( Int_t iThrIn = 3 ) { fiDiamSpillOffThr = iThrIn; }
    void SetDiamondTsNbSpillOffThr( Int_t iThrIn = 10 ) { fiTsUnderOffThr = iThrIn; }

    void SetEpochSuppressedMode( Bool_t bEnable = kTRUE ) { fbEpochSuppModeOn = bEnable; }

    void SetBeamTuningMode( Bool_t bEnable = kTRUE ) { fbBeamTuningMode = bEnable; }

    void SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5 );

    void ResetAllHistos();

  private:

    size_t fuMsAcceptsPercent; /** Reject Ms with index inside TS above this, assumes 100 MS per TS **/
    size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
    UInt_t fuMinNbGdpb;
    UInt_t fuCurrNbGdpb;

    UInt_t fNrOfGdpbs;           // Total number of GDPBs in the system
    UInt_t fNrOfFebsPerGdpb;     // Number of FEBs per GDPB
    UInt_t fNrOfGet4PerFeb;      // Number of GET4s per FEB
    UInt_t fNrOfChannelsPerGet4; // Number of channels in each GET4

    UInt_t fNrOfGet4;            // Total number of Get4 chips in the system
    UInt_t fNrOfGet4PerGdpb;     // Number of GET4s per GDPB

    UInt_t fDiamondGdpb;
    UInt_t fDiamondFeet;
    UInt_t fDiamondChanA;
    UInt_t fDiamondChanB;
    UInt_t fDiamondChanC;
    UInt_t fDiamondChanD;
    Int_t fDiamondTimeLastReset;
    Int_t fiDiamCountsLastTs;
    Int_t fiDiamSpillOnThr;
    Int_t fiDiamSpillOffThr;
    Int_t fiTsUnderOff;
    Int_t fiTsUnderOffThr;
    Double_t fdDiamondLastTime;
    Double_t fdDiamondTimeLastTs;
    Bool_t fbSpillOn;
    UInt_t fSpillIdx;
    Bool_t fbBeamTuningMode;

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
    Int_t* fCurrentEpoch; //!

    Int_t fNofEpochs; /** Current epoch marker for each ROC **/
    ULong_t fCurrentEpochTime; /** Time stamp of current epoch **/
    Double_t fdStartTime; /** Time of first valid hit (epoch available), used as reference for evolution plots**/
    Double_t fdStartTimeMsSz; /** Time of first microslice, used as reference for evolution plots**/
    TCanvas* fcMsSizeAll;

    /** Used only if the channel rate plots are enabled **/
    /** Last Hit time for each ROC/GET4/Channel (first hit in the stream initializes the map item) **/
    std::map<Int_t, std::map<Int_t, std::map<Int_t, Double_t> > > fTsLastHit; // * 6.25 ns

    Int_t fEquipmentId;

    CbmTofUnpackPar* fUnpackPar;      //!

    TH1* fHistMessType;
    TH1* fHistSysMessType;
    TH2* fHistGet4MessType;
    TH2* fHistGet4ChanErrors;
    TH2* fHistGet4EpochFlags;
    TH2* fHistDiamond;
    TH2* fHistDiamondSpill;
    TH1* fHistDiamondSpillLength;
    TH1* fHistDiamondSpillCount;
    TH2* fHistDiamondSpillQA;

    std::vector<TH2*> fRaw_Tot_gDPB;
    std::vector<TH1*> fChCount_gDPB;
    std::vector<TH2*> fChannelRate_gDPB;
    std::vector<TH1*> fFeetRate_gDPB;

    std::vector<TH1*> fFeetRateDate_gDPB;
    Int_t             fiRunStartDateTimeSec;
    Int_t             fiBinSizeDatePlots;

    void CreateHistograms();

    void FillHitInfo(ngdpb::Message);
    void FillEpochInfo(ngdpb::Message);
    void PrintSlcInfo(ngdpb::Message);
    void PrintSysInfo(ngdpb::Message);
    void PrintGenInfo(ngdpb::Message);

    inline Int_t GetArrayIndex(Int_t gdpbId, Int_t get4Id) {
      return gdpbId * fNrOfGet4PerGdpb + get4Id;
    }

    CbmTSMonitorTof(const CbmTSMonitorTof&);
    CbmTSMonitorTof operator=(const CbmTSMonitorTof&);

  ClassDef(CbmTSMonitorTof, 1)
};

#endif
