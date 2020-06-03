#ifndef CBMTRDCLUSTERFINDER_H
#define CBMTRDCLUSTERFINDER_H 

#include "FairTask.h"

#include <map>
#include <vector>
#include <set>

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
class CbmTrdClusterFinder : public FairTask
{
  friend class CbmTrdModuleRecR;
  friend class CbmTrdModuleRecT;
public:
  enum CbmTrdRecDef{
    kTime           = 0,    ///< select Time based/Event by event reconstruction
    kMultiHit,              ///< multi hit detection
    kRowMerger,             ///< merge clusters over neighbour rows
    kNeighbourCol,          ///< use neighbour trigger; column wise 
    kNeighbourRow,          ///< use neighbour trigger; row wise 
    kDumpClusters,          ///< write clustered digis to output
    kFASP                   ///< use FASP ASIC for triangular pad plane geometry
  };
  
  /**
   * \brief Default constructor.
   */
  CbmTrdClusterFinder();
  
  /**
   * \brief Default destructor.
   */
  ~CbmTrdClusterFinder();
 
  static Float_t    GetMinimumChargeTH()    { return fgMinimumChargeTH;}
  static Bool_t     HasDumpClusters()       { return TESTBIT(fgConfig, kDumpClusters); }
  static Bool_t     HasMultiHit()           { return TESTBIT(fgConfig, kMultiHit); }
  static Bool_t     HasNeighbourCol()       { return TESTBIT(fgConfig, kNeighbourCol); }
  static Bool_t     HasNeighbourRow()       { return TESTBIT(fgConfig, kNeighbourRow); }
  static Bool_t     HasRowMerger()          { return TESTBIT(fgConfig, kRowMerger); }
  static Bool_t     IsTimeBased()           { return TESTBIT(fgConfig, kTime); }
  
 /** Initialisation **/
  //virtual InitStatus ReInit();
  virtual InitStatus Init();
  virtual void      SetParContainers();
  
  /** \brief Executed task **/
  virtual void      Exec(Option_t * option);
  
  /** Finish task **/
  virtual void      Finish();

  
  static void       SetDumpClusters(Bool_t set=kTRUE) { set?SETBIT(fgConfig, kDumpClusters):CLRBIT(fgConfig, kDumpClusters);}
  static void       SetRowMerger(Bool_t set=kTRUE)    { set?SETBIT(fgConfig, kRowMerger):CLRBIT(fgConfig, kRowMerger);}
  static void       SetMultiHit(Bool_t set=kTRUE)     { set?SETBIT(fgConfig, kMultiHit):CLRBIT(fgConfig, kMultiHit);}
  static void       SetNeighbourEnable(Bool_t col=kTRUE, Bool_t row=kFALSE) 
                      { col?SETBIT(fgConfig, kNeighbourCol):CLRBIT(fgConfig, kNeighbourCol);
                        row?SETBIT(fgConfig, kNeighbourRow):CLRBIT(fgConfig, kNeighbourRow);}
  static void       SetMinimumChargeTH(Float_t th)    { fgMinimumChargeTH = th;}
  static void       SetTimeBased(Bool_t set=kTRUE)    { set?SETBIT(fgConfig, kTime):CLRBIT(fgConfig, kTime);}
protected:
  /** \brief Save one finished cluster to the output*/
  Bool_t            AddCluster(CbmTrdCluster* c);

private:
  CbmTrdClusterFinder(const CbmTrdClusterFinder&);
  CbmTrdClusterFinder& operator=(const CbmTrdClusterFinder&);

  Int_t             AddClusters(TClonesArray* clusters, Bool_t moveOwner=kTRUE);
  CbmTrdModuleRec*  AddModule(const CbmTrdDigi *d);
  
  static Int_t      fgConfig;         ///< Configuration map for the clusterizer. See CbmTrdRecDef for details
  static Float_t    fgMinimumChargeTH;  ///<
  

  TClonesArray*     fClusters;    /** Output array of CbmTrdCluster **/
  
  std::map<Int_t, std::set<Int_t> > fDigiMap;//! /** sector digis **/
  std::map<Int_t, std::set<Int_t> > fModuleMap;//! /** sector id per module **/
  
  std::set<Int_t> fNeighbours;
  std::map<Int_t, std::set<Int_t> > fModDigiMap;//std::map<Int_t ModuleID, std::vector<Int_t DigiID> >
 
  std::map<Int_t, Int_t> fDigiRow;
  std::map<Int_t, Int_t> fDigiCol;
  std::map<Int_t, Double_t> fDigiCharge;
  
  std::vector< std::set<Int_t> > fClusterBuffer;
  std::map< Int_t, std::vector< std::set<Int_t> > > fModClusterDigiMap;

  //==================================================================
  std::map<Int_t, CbmTrdModuleRec*> fModules; ///< list of modules being processed
  CbmTrdParSetAsic* fAsicPar;   ///< parameter list for ASIC characterization
  CbmTrdParSetGas*  fGasPar;    ///< parameter list for HV status
  CbmTrdParSetDigi* fDigiPar;   ///< parameter list for read-out geometry
  CbmTrdParSetGain* fGainPar;   ///< parameter list for keV->ADC gain conversion
  CbmTrdParSetGeo*  fGeoPar;    ///< parameter list for modules geometry

  ClassDef(CbmTrdClusterFinder,1);
  
};
#endif
