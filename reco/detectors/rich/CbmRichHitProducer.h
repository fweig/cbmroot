/**
 * \file CbmRichHitProducer.h
 *
 * \brief Class for producing RICH hits directly from MCPoints.
 *
 * \author B. Polichtchouk
 * \date 2004
 **/

#ifndef CBM_RICH_HIT_PRODUCER
#define CBM_RICH_HIT_PRODUCER

#include "FairTask.h"
#include "CbmRichRecGeoPar.h"

#include <TClonesArray.h>    // for ROOTCLING

class TVector3;
class CbmEvent;
class CbmDigiManager;

/**
 * \class CbmRichHitProducer
 *
 * \brief Class for producing RICH hits directly from MCPoints.
 *
 * \author B. Polichtchouk
 * \date 2004
 **/
class CbmRichHitProducer : public FairTask
{
public:
    /**
     * \brief Default constructor.
     */
    CbmRichHitProducer();
    
    /**
     * \brief Destructor.
     */
    virtual ~CbmRichHitProducer();
    
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

    void SetRotationNeeded(Bool_t b) {fRotationNeeded = b;}
    
    
    
private:
    
    CbmDigiManager* fDigiMan; //!
    TClonesArray* fRichHits; // RICH hits
    TClonesArray* fCbmEvents; // CbmEvent for time-based simulations
    
    Int_t fEventNum; // event number
    
    Bool_t fRotationNeeded;
    
    Double_t fHitError;

    /**
     * \brief Add hit to the output array (and) CbmEvent if it is not NULL.
     */
    
    void AddHit(
                CbmEvent* event,
                TVector3 &posHit,
                Double_t time,
                Int_t index);
    
    /**
     * \brief Copy constructor.
     */
    CbmRichHitProducer(const CbmRichHitProducer&);
    
    /**
     * \brief Assignment operator.
     */
    CbmRichHitProducer& operator=(const CbmRichHitProducer&);
    
    ClassDef(CbmRichHitProducer,1)
};

#endif
