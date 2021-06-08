/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: V.Friese, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----            CbmStsHitProducerTaskIdeal header file             -----
// -----                  Created 10/01/06  by V. Friese               -----
// -------------------------------------------------------------------------


/** CbmStsHitProducerTaskIdeal.h
 *@author V.Friese <v.friese@gsi.de>
 **
 ** The ideal hit producer produces hits of type CbmStsMapsHit by copying
 ** the MCPoint position. The position error is set to 1 mum, much 
 ** smaller than can be obtained by any detector. Using the hits from 
 ** this HitProducer is thus equivalent to using MC information
 ** directly, but with the correct data interface.
 **/


#ifndef CBMSTSHITPRODUCERTASKIDEAL_H
#define CBMSTSHITPRODUCERTASKIDEAL_H 1


#include "CbmStsHit.h"
#include "CbmStsPoint.h"

#include "FairTask.h"

#include <vector>

class TClonesArray;
class CbmTrdParSetGas;

class CbmStsHitProducerTaskIdeal : public FairTask {

public:
  /** Default constructor **/
  CbmStsHitProducerTaskIdeal();

  CbmStsHitProducerTaskIdeal(const CbmStsHitProducerTaskIdeal&) = delete;
  CbmStsHitProducerTaskIdeal& operator=(const CbmStsHitProducerTaskIdeal&) = delete;

  /** Destructor **/
  ~CbmStsHitProducerTaskIdeal();


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
  TClonesArray* fPointArray;

  /** Output array of CbmStsHits **/
  TClonesArray* fHitArray;

  CbmTrdParSetGas* fTrdGasPar;

  std::vector<CbmStsHit> Algo(const std::vector<CbmStsPoint>&);
  std::vector<CbmStsPoint> Convert(TClonesArray* arr);

  ClassDef(CbmStsHitProducerTaskIdeal, 1);
};

#endif
