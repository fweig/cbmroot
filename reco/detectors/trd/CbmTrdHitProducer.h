/**
 * @file CbmTrdHitProducer.h
 * @author Alexandru Bercuci
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief FairTask to produce TrdHit objects from TrdCluster objects
 * @version 1.0
 * @date 2021-03-16
 * 
 * 
 */

#ifndef CBMTRDHITPRODUCER_H
#define CBMTRDHITPRODUCER_H

#include "CbmEvent.h"
#include "CbmTrdCluster.h"

#include <FairTask.h>

#include <RtypesCore.h>
#include <TClonesArray.h>

#include <limits>

class TGeoPhysicalNode;
class CbmTrdParSetAsic;
class CbmTrdParSetGas;
class CbmTrdParSetDigi;
class CbmTrdParSetGain;
class CbmTrdParSetGeo;
class CbmTrdModuleRec;
class CbmTrdHitProducer : public FairTask {
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
  virtual void SetParContainers();

private:
  CbmTrdHitProducer(const CbmTrdHitProducer&);
  CbmTrdHitProducer& operator=(const CbmTrdHitProducer&);

  CbmTrdModuleRec* AddModule(Int_t address, TGeoPhysicalNode* node);

  /**
   * @brief Process all clusters found in the TrdClusters branch
   * 
   * @return UInt_t 
   */
  UInt_t processClusters();

  /**
   * @brief Process all clusters attached to the given event
   * 
   * @param event 
   * @return UInt_t 
   */
  UInt_t processClusters(CbmEvent* event);

  /**
   * @brief Produce a hit from the cluster found at clusterIdx in fClusters
   * 
   * @param clusterIdx 
   */
  void processCluster(const Int_t clusterIdx);

  /**
   * @brief Pass all hits produced by the given module to the TrdHit branch
   * 
   * @param mod 
   * @param hitCounter 
   * @param event 
   */
  void addModuleHits(CbmTrdModuleRec* mod, Int_t* hitCounter, CbmEvent* event);

  /**
   * @brief Loop over all modules in the given geometry and call addModuleHits(imodule)
   * 
   * @param event 
   * @return Int_t 
   */
  Int_t addHits(CbmEvent* event = nullptr);


  /** @brief Input array of CbmTrdCluster */
  TClonesArray* fClusters = nullptr;
  /** @brief Output array of CbmTrdHit */
  TClonesArray* fHits = nullptr;
  /** @brief Array connected to the CbmEvent branch */
  TClonesArray* fEvents = nullptr;
  //================================================================== = {}
  std::map<Int_t, CbmTrdModuleRec*> fModules = {};       ///< list of modules being processed
  CbmTrdParSetAsic* fAsicPar                 = nullptr;  ///< parameter list for ASIC characterization
  CbmTrdParSetGas* fGasPar                   = nullptr;  ///< parameter list for HV status
  CbmTrdParSetDigi* fDigiPar                 = nullptr;  ///< parameter list for read-out geometry
  CbmTrdParSetGain* fGainPar                 = nullptr;  ///< parameter list for keV->ADC gain conversion
  CbmTrdParSetGeo* fGeoPar                   = nullptr;  ///< parameter list for modules geometry

  /** @brief Number of processed events (without CbmEvent corresponds to nr of exec calls) */
  UInt_t fNrEvents = 0;

  /** @brief Number of clusters as input for the hit production. */
  UInt_t fNrClusters = 0;

  /** @brief Number of produced hits. */
  UInt_t fNrHits = 0;

  /** @brief Total processing time [RealTime]. */
  Float_t fProcessTime = 0;


  ClassDef(CbmTrdHitProducer, 2);
};

#endif
