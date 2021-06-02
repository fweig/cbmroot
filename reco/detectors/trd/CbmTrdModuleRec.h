/** @file CbmTrdModuleRec.h
  * @copyright Copyright (C) 2018-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBMTRDMODULEREC_H
#define CBMTRDMODULEREC_H

#include "CbmTrdModuleAbstract.h"

#include <map>
#include <vector>

class CbmTrdDigi;
class CbmTrdCluster;
class CbmTrdHit;
class TClonesArray;
/**
  * \brief Abstract class for module wise cluster finding and hit reconstruction
  **/
class CbmTrdModuleRec : public CbmTrdModuleAbstract {
public:
  /**
   * \brief Default constructor.
   **/
  CbmTrdModuleRec();
  /**
  * \brief Constructor with placement
  **/
  CbmTrdModuleRec(Int_t mod, Int_t ly = -1, Int_t rot = 0);

  virtual ~CbmTrdModuleRec();

  /**
   * \brief Add digi to local module
   **/
  virtual Bool_t AddDigi(const CbmTrdDigi* /*d*/, Int_t /*id*/) { return kTRUE; }
  /**
   * \brief Clear local storage
   **/
  virtual void Clear(Option_t* opt = "");
  /**
   * \brief Reconstruct physics observables on hits
   **/
  virtual Bool_t Finalize() { return kTRUE; }
  /**
   * \brief Steering routine for finding digits clusters
   **/
  virtual Int_t FindClusters() = 0;
  virtual Int_t GetOverThreshold() const { return 0; }
  virtual TClonesArray* GetClusters() { return fClusters; }
  virtual const TClonesArray* GetClusters() const { return fClusters; }
  virtual TClonesArray* GetHits() { return fHits; }
  virtual const TClonesArray* GetHits() const { return fHits; }

  /**
   * \brief Steering routine for building hits
   **/
  virtual Bool_t MakeHits() = 0;
  /**
   * \brief Steering routine for converting cluster to hit
   **/
  virtual CbmTrdHit* MakeHit(Int_t cId, const CbmTrdCluster* c, std::vector<const CbmTrdDigi*>* digis) = 0;

protected:
  UInt_t fCalled;           ///< counter for the number of calls
  TClonesArray* fClusters;  ///< module wise storage of reconstructed cluster
  TClonesArray* fHits;      ///< module wise storage of reconstructed hits

private:
  CbmTrdModuleRec(const CbmTrdModuleRec& ref);
  const CbmTrdModuleRec& operator=(const CbmTrdModuleRec& ref);

  ClassDef(CbmTrdModuleRec, 1)
};
#endif
