/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBM2019TIMEWINEVENTBUILDERTASK_H
#define CBMMCBM2019TIMEWINEVENTBUILDERTASK_H

/// CBM headers
#include "CbmMcbm2019TimeWinEventBuilderAlgo.h"

/// FAIRROOT headers
#include "FairTask.h"

/// FAIRSOFT headers (geant, boost, ...)

/// C/C++ headers
#include <array>
#include <map>
#include <set>
#include <tuple>
#include <vector>
#include "CbmTofDigi.h"

class TClonesArray;

class CbmMcbm2019TimeWinEventBuilderTask : public FairTask
{
  public:

    /** Default constructor **/
    CbmMcbm2019TimeWinEventBuilderTask();

    CbmMcbm2019TimeWinEventBuilderTask(const CbmMcbm2019TimeWinEventBuilderTask&) = delete;
    CbmMcbm2019TimeWinEventBuilderTask operator=(const CbmMcbm2019TimeWinEventBuilderTask&) = delete;

    /** Constructor with parameters (Optional) **/
    //  CbmMcbm2019TimeWinEventBuilderTask(Int_t verbose);


    /** Destructor **/
    ~CbmMcbm2019TimeWinEventBuilderTask();


    /** Initiliazation of task at the beginning of a run **/
    virtual InitStatus Init();

    /** ReInitiliazation of task when the runID changes **/
    virtual InitStatus ReInit();


    /** Executed for each event. **/
    virtual void Exec(Option_t*);

    /** Load the parameter container from the runtime database **/
    virtual void SetParContainers();

    /** Finish task called at the end of the run **/
    virtual void Finish();

    void SetFillHistos( Bool_t bFlag = kTRUE );
    void SetOutFilename( TString sNameIn );

    void SetReferenceDetector( ECbmModuleId refDet );
    void AddDetector( ECbmModuleId selDet );
    void RemoveDetector( ECbmModuleId selDet );

    void SetTriggerMinNumberT0(   UInt_t uVal );
    void SetTriggerMinNumberSts(  UInt_t uVal );
    void SetTriggerMinNumberMuch( UInt_t uVal );
    void SetTriggerMinNumberTrd(  UInt_t uVal );
    void SetTriggerMinNumberTof(  UInt_t uVal );
    void SetTriggerMinNumberRich( UInt_t uVal );
    void SetTriggerMinNumberPsd(  UInt_t uVal );

    void SetTriggerWindow( ECbmModuleId det, Double_t dWinBeg, Double_t dWinEnd );

    void SetEventOverlapMode( EOverlapMode mode );
    void SetIgnoreTsOverlap( Bool_t bFlagIn );

  private:
    void FillOutput();
    void SaveHistos();

    CbmMcbm2019TimeWinEventBuilderAlgo * fpAlgo = nullptr;

    TClonesArray* fEvents = nullptr;    //! output container of CbmEvents

    Bool_t fbFillHistos{kTRUE};              //! Switch ON/OFF filling of histograms

    /** Name of the histogram output file **/
    TString fsOutFileName{"data/HistosEvtWin.root"};

    ClassDef(CbmMcbm2019TimeWinEventBuilderTask,1);
};

#endif // CBMMCBM2019TIMEWINEVENTBUILDERTASK_H
