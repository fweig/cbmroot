#ifndef CBMTRDHITPRODUCER_H
#define CBMTRDHITPRODUCER_H

#include "FairTask.h"

class TClonesArray;
class TGeoPhysicalNode;
class CbmTrdParSetAsic;
class CbmTrdParSetGas;
class CbmTrdParSetDigi;
class CbmTrdParSetGain;
class CbmTrdParSetGeo;
class CbmTrdModuleRec;
class CbmTrdCluster;
class CbmTrdHitProducer : public FairTask
{
public:
  /**
  * \brief Constructor.
  */
  CbmTrdHitProducer();

  /**
    * \brief Destructor.
    */
  virtual ~CbmTrdHitProducer();

  /**
    * \brief Inherited form FairTask.
    */
  virtual InitStatus Init();

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Exec(Option_t* option = "");

  /**
    * \brief Inherited from FairTask.
    */
  virtual void Finish();
  virtual void      SetParContainers();

private:
  CbmTrdHitProducer(const CbmTrdHitProducer&);
  CbmTrdHitProducer& operator=(const CbmTrdHitProducer&);
  
  Int_t             AddHits(TClonesArray* hits, Bool_t moveOwner=kTRUE);
  CbmTrdModuleRec*  AddModule(Int_t address, TGeoPhysicalNode* node);


  TClonesArray*     fClusters;    /** Input array of CbmTrdCluster **/
  TClonesArray*     fHits;        /** Output array of CbmTrdHit **/
  //==================================================================
  std::map<Int_t, CbmTrdModuleRec*> fModules; ///< list of modules being processed
  CbmTrdParSetAsic* fAsicPar;   ///< parameter list for ASIC characterization
  CbmTrdParSetGas*  fGasPar;    ///< parameter list for HV status
  CbmTrdParSetDigi* fDigiPar;   ///< parameter list for read-out geometry
  CbmTrdParSetGain* fGainPar;   ///< parameter list for keV->ADC gain conversion
  CbmTrdParSetGeo*  fGeoPar;    ///< parameter list for modules geometry

  ClassDef(CbmTrdHitProducer, 1);
};

#endif

