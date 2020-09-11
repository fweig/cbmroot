#ifndef CBMTRDMODULERECR_H
#define CBMTRDMODULERECR_H

#include "CbmTrdModuleRec.h"

#include <deque>
#include <list>
#include <map>
#include <vector>

using std::pair;
using std::tuple;

/**
  * \brief Rectangular pad module; Cluster finding and hit reconstruction algorithms
  **/
class CbmTrdModuleRecR : public CbmTrdModuleRec {
public:
  /**
   * \brief Default constructor.
   **/
  CbmTrdModuleRecR();
  /**
  * \brief Constructor with placement
  **/
  CbmTrdModuleRecR(Int_t mod, Int_t ly = -1, Int_t rot = 0);
  virtual ~CbmTrdModuleRecR();

  virtual Bool_t AddDigi(const CbmTrdDigi* d, Int_t id);

  /**
   * \brief Clear local storage.
   * \sa CbmTrdModuleRec::Clear()
   **/
  virtual void Clear(Option_t* opt = "");
  /**
   * \brief Steering routine for finding digits clusters
   **/
  virtual Int_t FindClusters();

  Int_t GetOverThreshold() const { return fDigiCounter; }
  Double_t GetSpaceResolution(Double_t val = 3.0);
  /**
   * \brief Steering routine for building hits
   **/
  virtual Bool_t MakeHits();
  /**
   * \brief Steering routine for converting cluster to hit
   **/
  virtual CbmTrdHit* MakeHit(Int_t cId,
                             const CbmTrdCluster* c,
                             std::vector<const CbmTrdDigi*>* digis);

protected:
private:
  CbmTrdModuleRecR(const CbmTrdModuleRecR& ref);
  const CbmTrdModuleRecR& operator=(const CbmTrdModuleRecR& ref);

  void addClusters(std::deque<std::pair<Int_t, const CbmTrdDigi*>> cluster);
  Int_t fDigiCounter;  // digits over threshold
  Int_t fCheck = 0;

  // different error classes for the position resolution based on the simulation results
  // the error classes are defined for the different module types
  // TODO: move to parameter file
  static constexpr Double_t kxVar_Value[2][5] = {
    {0.0258725, 0.0267693, 0.0344325, 0.0260322, 0.040115},
    {0.0426313, 0.0426206, 0.0636962, 0.038981, 0.0723851}};
  static constexpr Double_t kyVar_Value[2][5] = {
    {0.024549, 0.025957, 0.0250713, 0.0302682, 0.0291146},
    {0.0401438, 0.0407502, 0.0397242, 0.0519485, 0.0504586}};

  std::deque<std::tuple<Int_t, Bool_t, const CbmTrdDigi*>>
    fDigiMap;  //map to sort all digis from the Array into a deque; different module are separated; the tuple contains the digi indice, a bool to flag processed digis and the digi itself
  std::deque<std::deque<std::pair<Int_t, const CbmTrdDigi*>>>
    fClusterMap;  //map to store the clusters and the digi indices for later matching

  ClassDef(
    CbmTrdModuleRecR,
    1)  // Rectangular pad module; Cluster finding and hit reconstruction algorithms
};
#endif
