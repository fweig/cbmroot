/** @file TTofTrbTdcCalib.h
 ** @author Christian Simon <csimon@physi.uni-heidelberg.de>
 ** @date 2016-04-18
 **/

#ifndef TTOFTRBTDCCALIB_H
#define TTOFTRBTDCCALIB_H 1

#include "FairTask.h"

#include <vector>

class TMbsUnpackTofPar;
class TMbsCalibTofPar;
class TClonesArray;
class TTree;
class TFile;
class TTrbHeader;
class TH1;

/** @class TTofTrbTdcCalib
 ** @brief ...
 ** @author C. Simon <csimon@physi.uni-heidelberg.de>
 ** @version 1.0
 **
 ** ...
 **
 **/
class TTofTrbTdcCalib : public FairTask
{
  public:

    /** default constructor **/
    TTofTrbTdcCalib();

    /** default destructor **/
    ~TTofTrbTdcCalib() {};

    virtual void Exec(Option_t *option);

    void CreateCalibration(Bool_t bCalibration = kTRUE) {fbCreateCalib = bCalibration;}
    void SaveCalibData(Bool_t bSaveData = kTRUE) {fbSaveOutput = bSaveData;}
    void SetFineTimeMethod(Int_t iMethod = 0) {fiFineTimeMethod = iMethod;}
    void SetToTMethod(Int_t iMethod = 0) {fiToTMethod = iMethod;}
    void SetMinEntriesBinByBin(Int_t iEntries = 100000) {fiMinEntriesBinByBin = iEntries;}
    void SetMinEntriesLocalFineLimits(Int_t iEntries = 10000) {fiMinEntriesLocalLimits = iEntries;}
    void SetTimeContinuum(Bool_t bContinuum = kTRUE) {fbTimeContinuum = bContinuum;}
    void SetTrailingOffsetCycles(Int_t iCycles = 2) {fiTrailingOffset = iCycles;}
    void SetLowerLinearFineLimit(UInt_t uLowerLimit) {fuLowerLinearFineLimit = uLowerLimit;}
    void SetUpperLinearFineLimit(UInt_t uUpperLimit) {fuUpperLinearFineLimit = uUpperLimit;}
    void SetReferenceBoard(Int_t iBoard = 0) {fiReferenceBoard = iBoard;}
    void SetToTSingleLeading(Double_t dToT = -100.) {fdToTSingleLeading = dToT;}

  protected:

    /** Intialisation at begin of run. To be implemented in the derived class.
    *   If return value not kSUCCESS, task will be set inactive.
    **/
    virtual InitStatus Init();

    /** Intialise parameter containers.
    *   To be implemented in the derived class.
    **/
    virtual void SetParContainers();

    /** Action after each event. To be implemented in the derived class **/
    virtual void Finish();

  private:
    /** private methods **/

    /** copy constructor **/
    TTofTrbTdcCalib(const TTofTrbTdcCalib&);

    /** assignment operator **/
    TTofTrbTdcCalib& operator=(const TTofTrbTdcCalib&);

    /** private members **/

    TMbsUnpackTofPar* fMbsUnpackPar;
    TMbsCalibTofPar* fMbsCalibPar;

    Bool_t fbCreateCalib;
    Bool_t fbSaveOutput;

    TClonesArray* fTrbTdcBoardCollection;
    TTrbHeader* fTrbHeader;
    TClonesArray* fTrbTdcCalibCollection;

    Int_t fiFineTimeMethod;
    Int_t fiToTMethod;
    Int_t fiMinEntriesBinByBin;
    Int_t fiMinEntriesLocalLimits;
    Bool_t fbTimeContinuum;
    Int_t fiTrailingOffset;
    UInt_t fuLowerLinearFineLimit;
    UInt_t fuUpperLinearFineLimit;
    Int_t fiReferenceBoard;

    std::vector<Bool_t>* fbDisableBoard;
    std::vector<Double_t>* fdFinalOffset;

    std::vector< std::vector<Double_t> > fdRefFineTime;
    std::vector< std::vector< std::vector<Double_t> > > fdFineTime;

    Long64_t fliFullCoarseOverflows;
    Double_t fdPreviousRefCoarseTime;
    Double_t fdInitialRefOffset;
    Bool_t fbFirstEvent;

    Double_t fdToTSingleLeading;

    TFile* fBufferFile;
    TTree* fBufferTree;

    Int_t fiPreviousSpillIndex;
    Double_t fdPreviousEventTime;
    Double_t fdSpillStartTime;

    TH1* fCtsIdleTimeSpill;
    TH1* fCtsSpillLength;

    ClassDef(TTofTrbTdcCalib,1)
};

#endif
