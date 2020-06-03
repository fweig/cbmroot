
#ifndef CBM_RICH_MCBM_HIT_PRODUCER
#define CBM_RICH_MCBM_HIT_PRODUCER

#include "FairTask.h"
#include "CbmRichRecGeoPar.h"

#include "CbmDigiManager.h"   // for ROOTCLING

#include <map>         // for map
#include <string>      // for string

class TClonesArray;
class TVector3;
class CbmEvent;
class CbmRichDigi;

class CbmRichMCbmMappingData {
public:
    UInt_t fTrbId;
    UInt_t fChannel;
    Double_t fX;
    Double_t fY;
    Double_t fZ;
};

class CbmRichMCbmHitProducer : public FairTask
{
public:
    /**
     * \brief Default constructor.
     */
    CbmRichMCbmHitProducer();
    
    /**
     * \brief Destructor.
     */
    virtual ~CbmRichMCbmHitProducer();
    
    /**
     * \brief Inherited from FairTask.
     */
    virtual void SetParContainers();
    
    /**
     * \brief Inherited from FairTask.
     */
    virtual InitStatus Init();


    /**
     * \brief Inherited from FairTask.
     */
    virtual void Exec(
                      Option_t* option);
    
    /**
     * \brief Inherited from FairTask.
     */
    virtual void Finish();
    

    /**
     * Processblock of data either event-by-event or CbmEvent
     */
    void ProcessData(CbmEvent* event);

    
    /**
     * Process RichDigi. CbmEvent can be NULL.
     */
    void ProcessDigi(CbmEvent* event, Int_t digiIndex);


    /**
     * Set mapping file path.
     */
    void SetMappingFile(const std::string& mappingFile){fMappingFile = mappingFile;}
    
    
    /**
    * Set ToT Limits.
    */
    void setToTLimits(double low, double high){
        fToTLimitLow  = low;
        fToTLimitHigh = high;
    }
    
    
    /**
    * Set low ToT Limit.
    */
    void setToTLimitLow(double low){
        fToTLimitLow  = low;
    }
    
    
    /**
    * Set high ToT Limit.
    */
    void setToTLimitHigh(double high){
        fToTLimitHigh  = high;
    }
    
    
    /**
    * Apply ToT Cut
    */
    void applyToTCut(){
         fDoToT = true;
    }
    
    /**
    * Apply restriction to Mar2019 mRICH Acceptance (for Simulations)
    */
    void DoRestrictToAcc(bool val=true){
         fRestrictToAcc = val;
    }
    
    /**
    * Apply restriction to full mRICH Acceptance (for Simulations)
    */
    void DoRestrictToFullAcc(bool val=true){
         fRestrictToFullAcc = val;
    }
    
    
private:
    
    CbmDigiManager* fDigiMan = nullptr;
    TClonesArray* fRichHits; // RICH hits
    TClonesArray* fCbmEvents = nullptr; // CbmEvent for time-based simulations
    bool fDoToT = false;
    bool fRestrictToAcc = false;
    bool fRestrictToFullAcc = false;
    double fToTLimitLow  =   0.;
    double fToTLimitHigh = 1000.;
    
    std::map<Int_t,CbmRichMCbmMappingData> fRichMapping;

    Int_t fEventNum; // event number
    
    Double_t fHitError;

    std::string fMappingFile;

    void InitMapping();
    
    bool isInToT(const double ToT);
    
    bool RestrictToAcc(TVector3 &pos);
    bool RestrictToAcc(Double_t x, Double_t y);
    
    bool RestrictToFullAcc(TVector3 &pos);
    bool RestrictToFullAcc(Double_t x, Double_t y);


    /**
     * \brief Add hit to the output array (and) CbmEvent if it is not NULL.
     */
    
    void AddHit(
            CbmEvent* event,
            TVector3 &posHit,
            Double_t time,
            Double_t tot,
            Int_t index);
    
    /**
     * \brief Copy constructor.
     */
    CbmRichMCbmHitProducer(const CbmRichMCbmHitProducer&);
    
    /**
     * \brief Assignment operator.
     */
    CbmRichMCbmHitProducer& operator=(const CbmRichMCbmHitProducer&);
    
    ClassDef(CbmRichMCbmHitProducer,1)
};

#endif
