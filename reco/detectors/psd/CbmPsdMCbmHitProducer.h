
#ifndef CBM_PSD_MCBM_HIT_PRODUCER
#define CBM_PSD_MCBM_HIT_PRODUCER

#include "FairTask.h"
//#include "CbmRichRecGeoPar.h"

class TClonesArray;
class TVector3;
class CbmEvent;
class CbmDigiManager;
class CbmPsdDigi;

class CbmPsdMCbmHitProducer : public FairTask {
public:
  /**
     * \brief Default constructor.
     */
  CbmPsdMCbmHitProducer();

  /**
     * \brief Destructor.
     */
  virtual ~CbmPsdMCbmHitProducer();

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
  virtual void Exec(Option_t* option);

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
  //void SetMappingFile(const string& mappingFile){fMappingFile = mappingFile;}


  /**
    * Set Energy Limits.
    */
  void setEnRange(double low, double high)
  {
    fEnLimitLow  = low;
    fEnLimitHigh = high;
  }

  /**
    * Apply Energy Cut
    */
  void applyEnCut() { fDoEnCut = true; }

private:
  CbmDigiManager* fDigiMan = nullptr;
  TClonesArray* fPsdHits;              // PSD hits
  TClonesArray* fCbmEvents = nullptr;  // CbmEvent for time-based simulations
  bool fDoEnCut            = false;
  double fEnLimitLow       = 0.;
  double fEnLimitHigh      = 100000.;

  //map<Int_t,CbmRichMCbmMappingData> fPsdMapping; //TODO

  Int_t fEventNum;  // event number

  Double_t fHitError;

  TString fMappingFile;

  void InitMapping();

  bool isInEnRange(const double energy);

  /**
     * \brief Add hit to the output array (and) CbmEvent if it is not NULL.
     */

  void AddHit(CbmEvent* event, Double_t time, Double_t energy, UInt_t moduleId, UInt_t sectionId, Int_t index);

  /**
     * \brief Copy constructor.
     */
  CbmPsdMCbmHitProducer(const CbmPsdMCbmHitProducer&);

  /**
     * \brief Assignment operator.
     */
  CbmPsdMCbmHitProducer& operator=(const CbmPsdMCbmHitProducer&);

  ClassDef(CbmPsdMCbmHitProducer, 1)
};

#endif
