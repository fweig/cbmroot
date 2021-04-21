/**
 * @file CbmTrdClusterFinder.h
 * @author Alexandru Bercuci
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief FairTask to produce TrdCluster objects from TrdHit objects
 * @version 1.0
 * @date 2021-03-16
 * 
 * 
 */

#ifndef CBMTRDCLUSTERFINDER_H
#define CBMTRDCLUSTERFINDER_H

#include "CbmEvent.h"

#include "FairTask.h"

#include <RtypesCore.h>

#include <map>
#include <set>
#include <vector>

class CbmTrdCluster;
class CbmTrdDigi;
class CbmTrdParSetAsic;
class CbmTrdParSetGas;
class CbmTrdParSetGeo;
class CbmTrdParSetDigi;
class CbmTrdParSetGain;
class CbmTrdModuleRec;
class TClonesArray;
class TGeoPhysicalNode;

/** CbmTrdClusterFinder.h
 *@author Florian Uhlig <f.uhlig@gsi.de>
 **
 ** Task to find digis/pads which are not separated but
 ** touch each other. Create as an output an array of
 ** the digis belonging to the cluster.
 ** First sort the digis according to the unique sector 
 ** number. This will result in as many arrays as sectors
 ** for one detector module.
 ** Then look for each module in all sectors for clusters.
 ** If a cluster was found at the sector
 ** boundaries check if there is another cluster in the
 ** next sector which has an overlap with these cluster.
 ** If there is an overlap than merge the two clusters.
 **
 **/
class CbmTrdClusterFinder : public FairTask {
  friend class CbmTrdModuleRecR;
  friend class CbmTrdModuleRecT;

public:
  enum CbmTrdRecDef
  {
    kTime = 0,        ///< select Time based/Event by event reconstruction
    kMultiHit,        ///< multi hit detection
    kRowMerger,       ///< merge clusters over neighbour rows
    kNeighbourCol,    ///< use neighbour trigger; column wise
    kNeighbourRow,    ///< use neighbour trigger; row wise
    kDumpClusters,    ///< write clustered digis to output
    kFASP,            ///< use FASP ASIC for triangular pad plane geometry
    kOnlyEventDigis,  ///< use only digis connected to a CbmEvent
    kDebugStatements  ///< print out debug statements with LOG(info)
  };

  /**
   * \brief Default constructor.
   */
  CbmTrdClusterFinder();

  /**
   * \brief Default destructor.
   */
  ~CbmTrdClusterFinder();

  static Float_t GetMinimumChargeTH() { return fgMinimumChargeTH; }
  static Bool_t HasDumpClusters() { return TESTBIT(fgConfig, kDumpClusters); }
  static Bool_t HasMultiHit() { return TESTBIT(fgConfig, kMultiHit); }
  static Bool_t HasNeighbourCol() { return TESTBIT(fgConfig, kNeighbourCol); }
  static Bool_t HasNeighbourRow() { return TESTBIT(fgConfig, kNeighbourRow); }
  static Bool_t HasRowMerger() { return TESTBIT(fgConfig, kRowMerger); }
  static Bool_t IsTimeBased() { return TESTBIT(fgConfig, kTime); }
  static Bool_t DoDebugPrintouts() { return TESTBIT(fgConfig, kDebugStatements); }

  /**
   * @brief If true only digis connected ro a CbmEvent are processed
   * Per default this is activated on construction, such that the user can 
   * turn it off before Init(), where it will also be autmatically 
   * deactivated if no CbmEvent branch is found.
   * @return Bool_t 
   */
  static Bool_t UseOnlyEventDigis() { return TESTBIT(fgConfig, kOnlyEventDigis); }

  /** Initialisation **/
  //virtual InitStatus ReInit();
  virtual InitStatus Init();
  virtual void SetParContainers();

  /** \brief Executed task **/
  virtual void Exec(Option_t* option);

  /** Finish task **/
  virtual void Finish();


  static void SetDumpClusters(Bool_t set = kTRUE)
  {
    set ? SETBIT(fgConfig, kDumpClusters) : CLRBIT(fgConfig, kDumpClusters);
  }
  static void SetRowMerger(Bool_t set = kTRUE) { set ? SETBIT(fgConfig, kRowMerger) : CLRBIT(fgConfig, kRowMerger); }
  static void SetMultiHit(Bool_t set = kTRUE) { set ? SETBIT(fgConfig, kMultiHit) : CLRBIT(fgConfig, kMultiHit); }
  static void SetNeighbourEnable(Bool_t col = kTRUE, Bool_t row = kFALSE)
  {
    col ? SETBIT(fgConfig, kNeighbourCol) : CLRBIT(fgConfig, kNeighbourCol);
    row ? SETBIT(fgConfig, kNeighbourRow) : CLRBIT(fgConfig, kNeighbourRow);
  }
  static void SetMinimumChargeTH(Float_t th) { fgMinimumChargeTH = th; }
  static void SetTimeBased(Bool_t set = kTRUE) { set ? SETBIT(fgConfig, kTime) : CLRBIT(fgConfig, kTime); }

  static void SetDoDebugPrintouts(Bool_t set = kTRUE)
  {
    set ? SETBIT(fgConfig, kDebugStatements) : CLRBIT(fgConfig, kDebugStatements);
  }


  /**
   * @brief Set the Use Only Event Digis
   * Per default this is activated on construction, such that the user can 
   * turn it off before Init(), where it will also be autmatically 
   * deactivated if no CbmEvent branch is found.
   * @param set 
   */
  static void SetUseOnlyEventDigis(Bool_t set = kTRUE)
  {
    set ? SETBIT(fgConfig, kOnlyEventDigis) : CLRBIT(fgConfig, kOnlyEventDigis);
  }

protected:
  /** \brief Save one finished cluster to the output*/
  Bool_t AddCluster(CbmTrdCluster* c);

private:
  CbmTrdClusterFinder(const CbmTrdClusterFinder&);
  CbmTrdClusterFinder& operator=(const CbmTrdClusterFinder&);

  Int_t AddClusters(TClonesArray* clusters, CbmEvent* event, Bool_t moveOwner = kTRUE);

  /**
   * @brief Add all digis available from CbmDigiManager to the corresponding modules
   * 
   * @return UInt_t 
   */
  UInt_t addDigisToModules();

  /**
   * @brief Add all digis connected to the passed event to the corresponding modules
   * 
   * @param event 
   * @return UInt_t 
   */
  UInt_t addDigisToModules(CbmEvent* event);

  /**
   * @brief Add the digi in the TrdDigi branch at the passed digiIdx to its corresponding module
   * 
   * @param digiIdx index in the std::vector
   */
  void addDigiToModule(UInt_t digiIdx);

  /**
   * @brief Call the clusterizer function of each module
   * 
   * @param ndigis 
   * @param event 
   */
  void processDigisInModules(UInt_t ndigis, CbmEvent* event = nullptr);

  /**
   * @brief Adds the module corresponding to the address of the passed digi to the ModuleMap (fModules)
   * 
   * @param digi 
   * @return CbmTrdModuleRec* 
   */
  CbmTrdModuleRec* AddModule(const CbmTrdDigi* digi);


  static Int_t fgConfig;             ///< Configuration map for the clusterizer. See CbmTrdRecDef for details
  static Float_t fgMinimumChargeTH;  ///<


  TClonesArray* fClusters = nullptr; /** Output array of CbmTrdCluster **/

  /** @brief Array connected to the CbmEvent branch */
  TClonesArray* fEvents = nullptr;

  //==================================================================
  std::map<Int_t, CbmTrdModuleRec*> fModules = {};       ///< list of modules being processed
  CbmTrdParSetAsic* fAsicPar                 = nullptr;  ///< parameter list for ASIC characterization
  CbmTrdParSetGas* fGasPar                   = nullptr;  ///< parameter list for HV status
  CbmTrdParSetDigi* fDigiPar                 = nullptr;  ///< parameter list for read-out geometry
  CbmTrdParSetGain* fGainPar                 = nullptr;  ///< parameter list for keV->ADC gain conversion
  CbmTrdParSetGeo* fGeoPar                   = nullptr;  ///< parameter list for modules geometry

  /** @brief Number of processed time slices */
  UInt_t fNrTs = 0;

  /** @brief Number of processed events (without CbmEvent corresponds to nr of exec calls) */
  UInt_t fNrEvents = 0;

  /** @brief Number of digis as input for the hit production. */
  UInt_t fNrDigis = 0;

  /** @brief Number of produced clusters. */
  UInt_t fNrClusters = 0;

  /** @brief Total processing time [RealTime]. */
  Float_t fProcessTime = 0;

  ClassDef(CbmTrdClusterFinder, 2);
};
#endif
