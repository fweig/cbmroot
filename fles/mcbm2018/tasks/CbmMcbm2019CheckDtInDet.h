/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBM2019CHECKDTINDET_H
#define CBMMCBM2019CHECKDTINDET_H

/// CBMROOT headers
#include "CbmDefs.h"
#include "TimesliceMetaData.h"
#include "CbmTofDigi.h"

/// FAIRROOT headers
#include "FairTask.h"

/// External headers (ROOT, boost, ...)
#include "TString.h"

/// C/C++ headers
#include <vector>

class TClonesArray;
class TH1;
class TH2;
class TProfile;
class CbmDigiManager;

class CbmMcbm2019CheckDtInDet : public FairTask
{
  public:

    CbmMcbm2019CheckDtInDet();

    CbmMcbm2019CheckDtInDet(const CbmMcbm2019CheckDtInDet&) = delete;
    CbmMcbm2019CheckDtInDet operator=(const CbmMcbm2019CheckDtInDet&) = delete;

    /** Constructor with parameters (Optional) **/
    //  CbmMcbm2019CheckDtInDet(Int_t verbose);


    /** Destructor **/
    ~CbmMcbm2019CheckDtInDet();


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

    void SetNbChanT0(Int_t val = 8)
       { fuNbChanT0 = val; }

    void SetNbChanSts(Int_t val = 5120)
       { fuNbChanSts = val; }

    void SetNbChanMuch(Int_t val = 1000)
       { fuNbChanMuch = val; }

    void SetNbChanTrd(Int_t val = 1000)
       { fuNbChanTrd = val; }

    void SetNbChanTof(Int_t val = 1000)
       { fuNbChanTof = val; }

    void SetNbChanRich(Int_t val = 1000)
       { fuNbChanRich = val; }

    void SetNbChanPsd(Int_t val = 1000)
       { fuNbChanPsd = val; }

    inline void SetOutFilename( TString sNameIn ) { fOutFileName = sNameIn; }

  private:
    template <class Digi> void FillHistosPerDet( TH1* histoSameTime, TH1* histoDt,
                                                  TH1* histoDtLog ,TH2* histoDtPerChan,
                                                  ECbmModuleId iDetId = ECbmModuleId::kLastModule
                                                );
    void CreateHistos();
    void WriteHistos();


    /** Digi data **/
    CbmDigiManager* fDigiMan = nullptr; //!
    const std::vector<CbmTofDigi>* fT0DigiVector = nullptr;   //!
    TClonesArray* fT0DigiArray = nullptr;         //!
    TClonesArray* fTimeSliceMetaDataArray = nullptr; //!
    const TimesliceMetaData* pTsMetaData = nullptr;

    /// Constants
    static const UInt_t kuNbChanSMX      =        128;
    static const UInt_t kuMaxNbStsDpbs   =          2;
    static const UInt_t kuMaxNbMuchDpbs  =          6;
    static const UInt_t kuMaxNbMuchAsics =         36;
    static const UInt_t kuDefaultAddress = 0xFFFFFFFF;
    static const UInt_t kuMaxChannelSts  =       3000;

    /// Variables to store the previous digi time
    Double_t fPrevTimeT0   = 0.;
    Double_t fPrevTimeSts  = 0.;
    Double_t fPrevTimeMuch = 0.;
    Double_t fPrevTimeTrd  = 0.;
    Double_t fPrevTimeTof  = 0.;
    Double_t fPrevTimeRich = 0.;
    Double_t fPrevTimePsd  = 0.;

    /// User settings: Data correction parameters
    UInt_t fuNbChanT0  =        8;
    UInt_t fuNbChanSts =     5120;
    UInt_t fuNbChanMuch =    5120;
    UInt_t fuNbChanTrd =     5120;
    UInt_t fuNbChanTof =     5120;
    UInt_t fuNbChanRich =    5120;
    UInt_t fuNbChanPsd =     5120;
    //
    Int_t fNrTs = 0;

    TH1* fT0T0SameTime     = nullptr;
    TH1* fStsStsSameTime   = nullptr;
    TH1* fMuchMuchSameTime = nullptr;
    TH1* fTrdTrdSameTime   = nullptr;
    TH1* fTofTofSameTime   = nullptr;
    TH1* fRichRichSameTime = nullptr;
    TH1* fPsdPsdSameTime   = nullptr;

    TH1* fT0T0Diff     = nullptr;
    TH1* fStsStsDiff   = nullptr;
    TH1* fMuchMuchDiff = nullptr;
    TH1* fTrdTrdDiff   = nullptr;
    TH1* fTofTofDiff   = nullptr;
    TH1* fRichRichDiff = nullptr;
    TH1* fPsdPsdDiff   = nullptr;

    TH1* fT0T0DiffLog     = nullptr;
    TH1* fStsStsDiffLog   = nullptr;
    TH1* fMuchMuchDiffLog = nullptr;
    TH1* fTrdTrdDiffLog   = nullptr;
    TH1* fTofTofDiffLog   = nullptr;
    TH1* fRichRichDiffLog = nullptr;
    TH1* fPsdPsdDiffLog   = nullptr;


    TH2* fT0T0DiffPerChan     = nullptr;
    TH2* fStsStsDiffPerChan   = nullptr;
    TH2* fMuchMuchDiffPerChan = nullptr;
    TH2* fTrdTrdDiffPerChan   = nullptr;
    TH2* fTofTofDiffPerChan   = nullptr;
    TH2* fRichRichDiffPerChan = nullptr;
    TH2* fPsdPsdDiffPerChan   = nullptr;


    TString fOutFileName{"data/HistosDtInDet.root"};

    ClassDef(CbmMcbm2019CheckDtInDet,1);
};

#endif // CBMMCBM2019CHECKDTINDET_H
