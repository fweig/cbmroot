#ifndef MCBM_RICH_QA_REAL
#define MCBM_RICH_QA_REAL

#include "FairTask.h"
#include "CbmRichRingFinderHoughImpl.h"
#include "CbmEvent.h"

#include "CbmHistManager.h"   // for ROOTCLING

class TClonesArray;
class CbmRichRing;
class CbmRichHit;
class CbmTofTracklet;
class TVector3;
class CbmDigiManager;

#include <vector>
#include <map>

using namespace std;



class CbmRichMCbmQaReal : public FairTask
{

public:
    /**
     * \brief Standard constructor.
     */
    CbmRichMCbmQaReal();

    /**
     * \brief Standard destructor.
     */
    virtual ~CbmRichMCbmQaReal() {};

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
    * Apply restriction to current mRICH Acceptance (for Simulations)
    */
    void DoRestrictToAcc(){
         fRestrictToAcc = true;
    }

    
    /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
    void DoRestrictToFullAcc(bool val=true){
         fRestrictToFullAcc = val;
    }   
    
    
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
    TClonesArray* fT0Digis;
    TClonesArray* fRichHits;
    TClonesArray* fRichRings;
    TClonesArray* fTofHits;
    TClonesArray* fTofTracks;
    TClonesArray* fCbmEvent;

    CbmHistManager* fHM;

    Double_t fXOffsetHisto;
    
    Int_t fEventNum;

    Int_t fNofDrawnRings;
    
    Int_t fNofDrawnRichTofEv;
    
    Int_t fNofDrawnEvents;


    string fOutputDir; // output dir for results
    
    bool fRestrictToAcc = false;
    bool fRestrictToFullAcc = false;
    
    bool fDoWriteHistToFile = true;
    bool fDoDrawCanvas = true;
    
    bool RestrictToFullAcc(CbmTofTracklet *track);
    bool RestrictToFullAcc(TVector3 &pos);
    bool RestrictToFullAcc(Double_t x, Double_t y);

    /**
     * \brief Initialize histograms.
     */
    void InitHistograms();

    /**
     *  \brief Draw histograms.
     */
    void DrawHist();

    void RichRings();

    void DrawRing(CbmRichRing* ring);
    
    void DrawEvent(CbmEvent *ev, std::vector<int> &ringIndx, bool full);
    
    void DrawRing(CbmRichRing* ring, std::vector<CbmTofTracklet*> track) { DrawRing(ring, track, false); };
    
    void DrawRing(CbmRichRing* ring, std::vector<CbmTofTracklet*> track, bool full);
    
    void DrawRichTofEv(const std::vector<int> richHitIndx, const std::vector<int> tofTrackIndx);

    std::pair<int, double> FindClosestTrack(const CbmRichRing* ring, const std::vector<CbmTofTracklet*> track);
    
    std::pair<int, double> FindClosestRing(CbmTofTracklet* track, std::vector<int> &ringIndx);
    
    bool isAccmRICH(CbmTofTracklet *track);
    
    bool doToT(CbmRichHit* hit);
    
    Double_t getBeta(CbmTofTracklet *track);
    
    void analyseRing(CbmRichRing *ring, CbmEvent *ev,std::pair<int, double> &clTrack);
    
    Bool_t cutRadius(CbmRichRing *ring);
    Bool_t cutDistance(std::pair<int, double> &clTrack);
    

    /**
     * \brief Copy constructor.
     */
    CbmRichMCbmQaReal(const CbmRichMCbmQaReal&);

    /**
     * \brief Assignment operator.
     */
    CbmRichMCbmQaReal& operator=(const CbmRichMCbmQaReal&);





    ClassDef(CbmRichMCbmQaReal,1)
};

#endif
