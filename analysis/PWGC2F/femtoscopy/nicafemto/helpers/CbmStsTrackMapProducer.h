/*
 * CbmStsTrackMapProducer.h
 *
 *  Created on: 7 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMSTSTRACKMAPPRODUCER_H_
#define CBMSTSTRACKMAPPRODUCER_H_

#include "CbmStsHit.h"
#include "FairTask.h"
#include "NicaTrackClones.h"

class CbmStsTrackMapProducer : public FairTask {
  NicaTrackClones* fStsHits;
  NicaTrackClones* fStsTracks;
  Int_t GetStation(CbmStsHit* hit) const;

public:
  CbmStsTrackMapProducer();
  InitStatus Init();
  void Exec(Option_t* /*opt*/);
  virtual ~CbmStsTrackMapProducer();
  ClassDef(CbmStsTrackMapProducer, 1)
};

#endif /* CBMSTSTRACKMAPPRODUCER_H_ */
