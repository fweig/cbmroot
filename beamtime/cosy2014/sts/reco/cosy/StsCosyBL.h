#ifndef STSCOSYBL_H
#define STSCOSYBL_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Bool_t, Double_t, Option_t
#include <TChain.h>      // for TChain
#include <vector>        // for vector
#include <FairTask.h>    // for InitStatus, FairTask

#include "CbmAuxDigi.h"
#include "CbmStsDigi.h"
#include "CbmFiberHodoDigi.h"

class TFile;
class TH1F;
class TH2F;

class StsCosyBL : public FairTask
{
  public:

    /** Default constructor **/
    StsCosyBL();

    /** Constructor with parameters (Optional)
    //  StsCosyBL(Int_t verbose);
    **/

    /** Destructor **/
    ~StsCosyBL();


    /** Initiliazation of task at the beginning of a run **/
    virtual InitStatus Init();

    /** ReInitiliazation of task when the runID changes **/
    virtual InitStatus ReInit();


    /** Executed for each event. **/
    virtual void Exec(Option_t* opt);

    /** Load the parameter container from the runtime database **/
    virtual void SetParContainers();

    /** Recursive FinishEvent of subtasks **/
    virtual void FinishEvent();

    /** Finish task called at the end of the run **/
    virtual void Finish();
    //    virtual Int_t AddFile( const char* name ); 
    void BLInit();
    void BLClean();
    void BaseLine(const std::vector<CbmStsDigi>* fBaselineDigis,
		  std::vector< std::vector < std::vector < double> > > base_line_array);
    void HodoBaseLine(const std::vector<CbmFiberHodoDigi>* fBaselineDigis,
		      std::vector< std::vector < std::vector < double> > > base_line);

    void SetTriggeredMode(Bool_t mode) { fTriggeredMode = mode; }
    void SetTriggeredStation(Int_t station) { fTriggeredStation = station ; }

    virtual void Reset();    

    Int_t GetEntries () { return fChain->GetEntries();}

 private:
    const std::vector<CbmAuxDigi>*        auxDigis;
    const std::vector<CbmStsDigi>*        fDigis;
    const std::vector<CbmStsDigi>*        fBLDigis;
    const std::vector<CbmFiberHodoDigi>*  hBLDigis;
    const std::vector<CbmFiberHodoDigi>*  hDigis;
    std::vector<CbmStsDigi>*        cDigis;
    std::vector<CbmFiberHodoDigi>*  chDigis;

    TChain *fChain;                   //!
    TFile *outFile;                   //!
    static Double_t GetBlPos( TH1F * hist );
    static const Int_t kBaselineMinAdc = 0;
    static const Int_t kBaselineMaxAdc = 4096;
    static const Int_t kBaselineNBins = 4096;


    TH2F *baseline_ch[3][2];       //!
    TH2F *raw_ch[3][2];            //!
    TH2F *raw_ch_woBL[3][2];       //!
    TH2F *calibr_ch[3][2];         //!
    TH1F *calibr_ch1D[3][2];       //!
    
    TH2F *hodo_baseline_ch[2][2];  //!
    TH2F *hodo_calib_ch[2][2];     //!

    Bool_t calib;
    Bool_t hodo_calib;

    Bool_t fTriggeredMode; ///< Flag if data is taken in triggered mode
    Int_t  fTriggeredStation;
    Int_t  fNofEvent;

    std::vector< std::vector < std::vector < double> > > base_line_array;
    std::vector< std::vector < std::vector < double> > > hodo_BL_array;

    StsCosyBL(const StsCosyBL&);
    StsCosyBL operator=(const StsCosyBL&);

    ClassDef(StsCosyBL,2);
};

#endif
