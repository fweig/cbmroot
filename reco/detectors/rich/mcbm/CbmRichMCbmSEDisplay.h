#ifndef MCBM_RICH_SE_DISPLAY
#define MCBM_RICH_SE_DISPLAY

#include "CbmEvent.h"
#include "CbmHistManager.h"   // for ROOTCLING

class TClonesArray;
class CbmRichHit;
class CbmRichRing;
class CbmTofTracklet;

#include <vector>

using namespace std;



class CbmRichMCbmSEDisplay
{

public:
    /**
     * \brief Standard constructor.
     */
    CbmRichMCbmSEDisplay();
    
    /**
     * \brief constructor with HistManager.
     */
    CbmRichMCbmSEDisplay(CbmHistManager * manager);

    /**
     * \brief Standard destructor.
     */
    virtual ~CbmRichMCbmSEDisplay() {};

    
    /**
     *  \brief Draw histograms.
     */
    void DrawEvent(CbmEvent *ev, std::vector<int> &ringIndx, bool full);
    
    
    /**
    * Move X-Position of mRICH in Histograms (e.g. for Geometry changes)
    */
    void XOffsetHistos (Double_t val = 0.){
         fXOffsetHisto = val;
    }  

    /**
    * Set an ToT cut of the RICH Hits. 
    */
    void SetTotRich (Double_t min, Double_t max){
        fTotRichMin = min;
        fTotRichMax = max;
    }      
    
    /**
    * Set a pointer to the loaded Rich hits
    */
    void SetRichHits(TClonesArray* hits = nullptr){
        fRichHits = hits;
    }
    
    /**
    * Set a pointer to the loaded Rich hits
    */
    void SetRichRings(TClonesArray* ring = nullptr){
        fRichRings = ring;
    }
    
    /**
    * Set a pointer to the loaded Rich hits
    */
    void SetTofTracks(TClonesArray* track = nullptr){
        fTofTracks = track;
    }
    
    /**
    * Limit of Single Event Displays that should be drawn 
    */
    void SetMaxNofDrawnEvents(Int_t val = 100){
        fMaxNofDrawnEvents = val;
    }
         
         
    /**
    * Limit of Single Event Displays that should be drawn 
    */
    void SetHistmanager(CbmHistManager* manager){
        fHM = manager;
    }
    
    
    /**
    * Set the output directory of the Analysis
    */
    void SetOutDir(std::string dir){
        fOutputDir = dir;
    }  
    
    /**
    * Set the output directory of the Canvases
    */
    
    void SetCanvasDir(std::string dir){
        fFileName = dir;
    } 
    
private:


    TClonesArray* fRichHits;

    TClonesArray* fRichRings;

    TClonesArray* fTofTracks;
    
    Double_t fXOffsetHisto;
    
    Double_t fTotRichMin;
    
    Double_t fTotRichMax;
    
    Int_t fNofDrawnEvents;
    
    Int_t fMaxNofDrawnEvents;
    


    std::string fOutputDir; // output dir for results
    
    std::string fFileName = "Ev";
    
    CbmHistManager* fHM;

        
    template<typename T=CbmRichHit>
    bool doToT(T* hit){
        bool check = false;
        if ((hit->GetToT() > 23.7) && (hit->GetToT() < 30.0)) check = true;
    
        return check;
    }


    /**
     * \brief Copy constructor.
     */
    CbmRichMCbmSEDisplay(const CbmRichMCbmSEDisplay&);

    /**
     * \brief Assignment operator.
     */
    CbmRichMCbmSEDisplay& operator=(const CbmRichMCbmSEDisplay&);





    ClassDef(CbmRichMCbmSEDisplay,1)
};

#endif
