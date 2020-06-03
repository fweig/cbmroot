// -------------------------------------------------------------------------
// -----              CbmMvdReadoutCluster  header file                -----
// -----              Created 17/10/16  by P. Sitzmann                 -----
// -------------------------------------------------------------------------


/**  CbmMvdReadoutSimple.h
 *@author P.Sitzmann <p.sitzmann@gsi.de>
 *
 *  Readout simulations for the mvd
 *
 **/

#ifndef CBMMVDREADOUTCLUSTER_H
#define CBMMVDREADOUTCLUSTER_H 1

#include "FairTask.h"
#include "TH1F.h"
#include "TH2F.h"

class CbmMvdReadoutCluster : public FairTask
{
public:

    CbmMvdReadoutCluster();
    CbmMvdReadoutCluster(const char* name, Int_t iVerbose = 0);
    CbmMvdReadoutCluster(const CbmMvdReadoutCluster&) = delete;
    CbmMvdReadoutCluster& operator=(const CbmMvdReadoutCluster&) = delete;
    
    ~CbmMvdReadoutCluster();

    InitStatus Init();

    void Exec(Option_t* opt);

    void ShowHistograms(){fshow = kTRUE;};

    void SetHistogramFile(TFile* file){foutFile = file;};

    void Finish();

private:

    TFile* foutFile;

    Bool_t fshow;

    TH1F* fWordsPerRegion[350];
    TH1F* fWordsPerSuperRegion[350];

    TClonesArray* fMvdCluster;

    Int_t fEventNumber;
    const Int_t fPixelsPerRegion = 16;
    const Int_t fPixelsPerSuperRegion = 64;

    void DrawHistograms();
    void WriteHistograms();
    void SetupHistograms();

ClassDef(CbmMvdReadoutCluster,1);
};
#endif
