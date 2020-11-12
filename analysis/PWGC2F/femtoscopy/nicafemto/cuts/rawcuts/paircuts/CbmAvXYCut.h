/*
 * CbmAvXYCut.h
 *
 *  Created on: 15 cze 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBM_NOV_ANALYSIS_CBMFEMTO_CUTS_PAIRCUTS_CBMAVXYCUT_H_
#define CBM_NOV_ANALYSIS_CBMFEMTO_CUTS_PAIRCUTS_CBMAVXYCUT_H_

#include "NicaCbmTrack.h"
#include "NicaTwoTrackCut.h"

class CbmAvXYCut : public NicaTwoTrackCut {
public:
  CbmAvXYCut();
  virtual Bool_t Init(Int_t task_id = 0);
  Bool_t Pass(NicaTwoTrack* pair);
  virtual ~CbmAvXYCut();
  ClassDef(CbmAvXYCut, 1)
};

#endif /* CBM_NOV_ANALYSIS_CBMFEMTO_CUTS_PAIRCUTS_CBMAVXYCUT_H_ */
