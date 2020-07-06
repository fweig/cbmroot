#ifndef MCBM_RICH_Aerogel
#define MCBM_RICH_Aerogel

#include "FairTask.h"
#include "CbmRichRingFinderHoughImpl.h"
#include "CbmEvent.h"
class TClonesArray;
class CbmRichRing;
class CbmRichHit;
class CbmTofTracklet;
class CbmHistManager;
class TVector3;
class CbmDigiManager;

#include <vector>
#include <map>

using namespace std;



class CbmRichMCbmAerogelAna : public FairTask
{

public:
    /**
     * \brief Standard constructor.
     */
    CbmRichMCbmAerogelAna();

    /**
     * \brief Standard destructor.
     */
    virtual ~CbmRichMCbmAerogelAna() {};

    /**
     * \brief Inherited from FairTask.
     */
    virtual InitStatus Init();

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Exec(Option_t* option);

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Finish();

    /**
     * \brief Set output directory where you want to write results (figures and json).
     * \param[in] dir Path to the output directory.
     */
    void SetOutputDir(const string& dir) {fOutputDir = dir;} 
    

    /**
     * \brief Draw histogram from file
     */
    void DrawFromFile(
            const string& fileName,
            const string& outputDir);
        
    
    /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
    void DoDrawCanvas (bool val=true){
         fDoDrawCanvas = val;
    }
    
    /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
    void DoWriteHistToFile (bool val=true){
         fDoWriteHistToFile = val;
    }      
    
    
    /**
    * Move X-Position of mRICH in Histograms (e.g. for Geometry changes)
    */
    void XOffsetHistos (Double_t offset = 0.){
         fXOffsetHisto = offset;
    }  
    
private:

    CbmDigiManager* fDigiMan = nullptr;
    
    TClonesArray* fRichHits;
    
    TClonesArray* fRichRings;
    
    TClonesArray* fCbmEvent;

    CbmHistManager* fHM;

    Double_t fXOffsetHisto;
    
    Int_t fEventNum;

    Int_t fNofDrawnRings;
    
    Int_t fNofDrawnRichTofEv;
    
    Int_t fNofDrawnEvents;


    string fOutputDir; // output dir for results
    
    bool fDoWriteHistToFile = true;
    bool fDoDrawCanvas = true;

    /**
     * \brief Initialize histograms.
     */
    void InitHistograms();

    /**
     *  \brief Draw histograms.
     */
    void DrawHist();

    void RichRings();;
    
    bool doToT(CbmRichHit* hit);
    
    Bool_t cutRadius(CbmRichRing *ring);
    

    /**
     * \brief Copy constructor.
     */
    CbmRichMCbmAerogelAna(const CbmRichMCbmAerogelAna&);

    /**
     * \brief Assignment operator.
     */
    CbmRichMCbmAerogelAna& operator=(const CbmRichMCbmAerogelAna&);





    ClassDef(CbmRichMCbmAerogelAna,1)
};

#endif
