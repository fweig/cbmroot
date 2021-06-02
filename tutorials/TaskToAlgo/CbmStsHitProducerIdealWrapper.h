/** @file CbmStsHitProducerIdealWrapper.h
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

// -------------------------------------------------------------------------
// -----            CbmStsHitProducerIdealWrapper header file          -----
// -------------------------------------------------------------------------

#ifndef CBMSTSHITPRODUCERIDEALWRAPPER_H
#define CBMSTSHITPRODUCERIDEALWRAPPER_H 1


#include "CbmStsHitProducerIdealAlgo.h"
#include "CbmStsPoint.h"

#include "FairTask.h"

#include <vector>

class TClonesArray;
class CbmTrdParSetGas;

class CbmStsHitProducerIdealWrapper : public FairTask {

public:
  /** Default constructor **/
  CbmStsHitProducerIdealWrapper();

  CbmStsHitProducerIdealWrapper(const CbmStsHitProducerIdealWrapper&) = delete;
  CbmStsHitProducerIdealWrapper& operator=(const CbmStsHitProducerIdealWrapper&) = delete;

  /** Destructor **/
  ~CbmStsHitProducerIdealWrapper();


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /**
  * \brief Inherited from FairTask.
  */
  virtual void SetParContainers();

private:
  /** Input array of CbmStsPoints **/
  TClonesArray* fPointArray {nullptr};

  /** Output array of CbmStsHits **/
  TClonesArray* fHitArray {nullptr};

  CbmStsHitProducerIdealAlgo* fAlgo {new CbmStsHitProducerIdealAlgo()};

  CbmTrdParSetGas* fTrdGasPar {nullptr};

  std::vector<CbmStsPoint> Convert(TClonesArray* arr);

  ClassDef(CbmStsHitProducerIdealWrapper, 1);
};

#endif
