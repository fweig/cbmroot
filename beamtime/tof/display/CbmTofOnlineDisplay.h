#ifndef CBMTOFONLINEDISPLAY_H
#define CBMTOFONLINEDISPLAY_H

#include "FairTask.h"

#include <vector>

// Parameters
class TMbsUnpackTofPar;
class TMbsCalibTofPar;

class TClonesArray;
class TCanvas;
class TH2;
class THStack;
class TLegend;

class CbmTofOnlineDisplay : public FairTask
{
  public:

    /** Default constructor **/
    CbmTofOnlineDisplay();

    /** Constructor with parameters (Optional) **/
    //  CbmTofOnlineDisplay(Int_t verbose);


    /** Destructor **/
    ~CbmTofOnlineDisplay();


    /** Initiliazation of task at the beginning of a run **/
    virtual InitStatus Init();

    /** ReInitiliazation of task when the runID changes **/
    virtual InitStatus ReInit();


    /** Executed for each event. **/
    virtual void Exec(Option_t* opt);

    /** Load the parameter container from the runtime database **/
    virtual void SetParContainers();

    /** Finish task called at the end of the run **/
    virtual void Finish();

    void SetNumberOfTDC(Int_t val) { fNumberOfTDC = val; }
    void SetUpdateInterval(Int_t val) { fUpdateInterval = val; }

    void SetResMonitorEna( Bool_t bInEna = kTRUE ){ fbMonitorRes = bInEna; };

    void MonitorTdcOccupancy( Bool_t bMonitor = kTRUE){ fbMonitorTdcOcc = bMonitor; };
    void MonitorCts( Bool_t bMonitor = kTRUE){ fbMonitorCts = bMonitor; };
    void MonitorSebStatus( Bool_t bMonitor = kTRUE){ fbMonitorSebStatus = bMonitor; };
    void MonitorTdcStatus( Bool_t bMonitor = kTRUE){ fbMonitorTdcStatus = bMonitor; };
    void MonitorFSMockup( Bool_t bMonitor = kTRUE){ fbMonitorFSMockup = bMonitor; };
    void MonitorDigiStatus( Bool_t bMonitor = kTRUE){ fbMonitorDigiStatus = bMonitor; };
    void MonitorInspection( Bool_t bMonitor = kTRUE){ fbMonitorInspection = bMonitor; };
    void MonitorCalibration( Bool_t bMonitor = kTRUE){ fbMonitorCalibration = bMonitor; };

    void SetNumberOfSEB(Int_t val) { fNumberOfSEB = val; }

    void SetRateMonitorEna( Bool_t bInEna = kTRUE ){ fbMonitorRates = bInEna; };
    void SetFreeTrloNames( TString sChA, TString sChB, TString sChC, TString sChD, 
                           TString sChE, TString sChF, TString sChG, TString sChH, 
                           TString sChI, TString sChJ, TString sChK, TString sChL, 
                           TString sChM, TString sChN, TString sChO, TString sChP );
    void RatesSlidingScaleEna( Bool_t bInEna = kTRUE ){ fbRatesSlidingScale = bInEna; };

  private:

    /** Input array from previous already existing data level **/
    //  TClonesArray* <InputDataLevel>;

    /** Output array to  new data level**/
    //  TClonesArray* <OutputDataLevel>;

    Bool_t InitParameters();
    TMbsUnpackTofPar    *fMbsUnpackPar;
    TMbsCalibTofPar     *fMbsCalibPar;

    Bool_t   fbMonitorTdcOcc;
    TCanvas* fTdcChannelOccupancy; //!

    Int_t fNumberOfTDC;
    Int_t fNumberOfSEB;
    Int_t fUpdateInterval;
    Int_t fEventCounter;

    Bool_t   fbMonitorCts;
    TCanvas* fCtsTriggerMonitor; //!

    Bool_t   fbMonitorSebStatus;
    TCanvas* fSebSizeMonitor; //!
    TCanvas* fSebStatusMonitor; //!
    TCanvas* fSebDataRateMonitor; //!

    Bool_t   fbMonitorTdcStatus;
    TCanvas* fTdcSizeMonitor; //!
    TCanvas* fTdcStatusMonitor; //!
    
    Bool_t   fbMonitorFSMockup;
    TCanvas* fFSMockupMonitor; //!

    // Monitoring of the TDC resolution stability using reference channel
    Bool_t   fbMonitorRes;
    TCanvas* fOverviewRes; //!
    TH2*     fhResolutionSummary; //!
    TH2*     fhResolutionRmsSummary; //!

    Bool_t   fbMonitorDigiStatus;
    TCanvas* fDigiSizeMonitor; //!
    TCanvas* fDigiStatusMonitor; //!

    // Monitoring of the rates from Triglog and/or scalers
    Bool_t   fbMonitorRates;
    TString  fsFreeTrloNames[16];
    TCanvas* fCanvRatesMonitor; //!
    THStack* fStackMbsTrloA; //!
    THStack* fStackMbsTrloB; //!
    THStack* fStackFreeTrloA; //!
    THStack* fStackFreeTrloB; //!
    THStack* fStackFreeTrloOutA; //!
    THStack* fStackFreeTrloOutB; //!
    TLegend* fLegStackMbsTrloA; //!
    TLegend* fLegStackMbsTrloB; //!
    TLegend* fLegStackFreeTrloA; //!
    TLegend* fLegStackFreeTrloB; //!
    TLegend* fLegStackFreeTrloOutA; //!
    TLegend* fLegStackFreeTrloOutB; //!
    Bool_t   fbRatesSlidingScale; //!

    Bool_t   fbMonitorInspection;
    TCanvas* fLeadingEdgeOnly; //!
    TCanvas* fTrailingEdgeOnly; //!
    TCanvas* fUnequalEdgeCounts; //!
    TCanvas* fTimeOverThreshold; //!
    TCanvas* fLeadingPosition; //!
    TCanvas* fTrailingPosition; //!
    TCanvas* fBoardFineTime; //!
    TCanvas* fRefChFineTime; //!

    Bool_t   fbMonitorCalibration;
    TCanvas* fBoardOffsetGraphs; //!
    TCanvas* fBoardOffsetLinear; //!
    TCanvas* fBoardOffsetBinCenter; //!
    TCanvas* fBoardOffsetBinEdge; //!

    CbmTofOnlineDisplay(const CbmTofOnlineDisplay&);
    CbmTofOnlineDisplay operator=(const CbmTofOnlineDisplay&);

    ClassDef(CbmTofOnlineDisplay,1);
};

#endif
