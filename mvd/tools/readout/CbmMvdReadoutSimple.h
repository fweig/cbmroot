// -------------------------------------------------------------------------
// -----              CbmMvdReadoutSimple  header file                            -----
// -----              Created 17/10/16  by P. Sitzmann                 -----
// -------------------------------------------------------------------------


/**  CbmMvdReadoutSimple.h
 *@author P.Sitzmann <p.sitzmann@gsi.de>
 *
 *  Simple Readout simulations for the mvd
 *
 **/

#ifndef CBMMVDREADOUTSIMPLE_H
#define CBMMVDREADOUTSIMPLE_H 1

#include "FairTask.h"
#include "TH1F.h"
#include "TH2F.h"

class CbmMvdReadoutSimple : public FairTask
{
public:

    CbmMvdReadoutSimple();
    CbmMvdReadoutSimple(const char* name, Int_t iVerbose = 0);
    CbmMvdReadoutSimple(const CbmMvdReadoutSimple&) = delete;
    CbmMvdReadoutSimple& operator=(const CbmMvdReadoutSimple&) = delete;
    
    ~CbmMvdReadoutSimple();

    InitStatus Init();

    void Exec(Option_t* opt);

    void ShowHistograms(){fshow = kTRUE;};

    void SetHistogramFile(TFile* file){foutFile = file;};

    void Finish();

private:

    TFile* foutFile;

    Bool_t fshow;
    TH2F* fMvdMCBank[63];
    TH2F* fMvdMCHitsStations[4];
    TH1F* fWordsPerRegion;
    TH2F* fWordsPerRegion2;
    TH1F* fWordsPerWorstRegion;
    TH1F* fWordsPerSuperRegion;
    TH1F* fWorstSuperPerEvent;
    TH2I* fMvdBankDist;
    TH2F* fMvdMCWorst;
    TH2F* fMvdMCWorstDelta;
    TH1I* fMvdDataLoadPerSensor;
    TH1I* fMvdDataLoadHotSensor;
    TH1F* fMvdDataPerRegion[64];
    TH1F* fMvdDataPerSuperRegion[16];

    TClonesArray* fMcPoints;
    TClonesArray* fListMCTracks;

    Int_t fEventNumber;

    void DrawHistograms();
    void WriteHistograms();
    void SetupHistograms();

ClassDef(CbmMvdReadoutSimple,1);
};
#endif
