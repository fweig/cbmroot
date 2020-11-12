/*
 * CbmSailorCowboyCut.h
 *
 *  Created on: 9 cze 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBM_NOV_ANALYSIS_CBMFEMTO_CUTS_PAIRCUTS_CBMSAILORCOWBOYCUT_H_
#define CBM_NOV_ANALYSIS_CBMFEMTO_CUTS_PAIRCUTS_CBMSAILORCOWBOYCUT_H_

#include "NicaCbmTrack.h"
#include "NicaTwoTrackCut.h"

class CbmSailorCowboyCut : public NicaTwoTrackCut {

public:
  CbmSailorCowboyCut();
  static Int_t Cowboy() { return 1; };
  static Int_t Sailor() { return 0; };
  void AcceptSailor() { SetMinAndMax(Sailor()); };
  void AcceptCowboy() { SetMinAndMax(Cowboy()); };
  virtual ~CbmSailorCowboyCut();
  virtual Bool_t Init(Int_t task_id = 0);
  Bool_t Pass(NicaTwoTrack* pair);
  ClassDef(CbmSailorCowboyCut, 1)
};

#endif /* CBM_NOV_ANALYSIS_CBMFEMTO_CUTS_PAIRCUTS_CBMSAILORCOWBOYCUT_H_ */
