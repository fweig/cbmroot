/*
 * NicaCbmMCTrack.h
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMMCTRACK_H_
#define NICACBMMCTRACK_H_

#include "CbmMCTrack.h"
#include "NicaMCTrack.h"

class NicaCbmMCTrack : public NicaMCTrack {
  Int_t fNPoints;

public:
  NicaCbmMCTrack();
  void SetNPoints(ECbmModuleId iDet, Int_t np);
  Int_t GetNPoints(ECbmModuleId detId) const;
  virtual void CopyData(NicaTrack* other);
  virtual void Update(CbmMCTrack* mc, Double_t charge);
  virtual ~NicaCbmMCTrack();
  ClassDef(NicaCbmMCTrack, 1)
};

#endif /* BMN_NICA_BMN_FORMAT_MC_NICACBMMCTRACK_H_ */
