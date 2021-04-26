/*
 * CbmTofM2PolyCut.h
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef COMMONCUTS_TRACK_CBMTOFM2POLYCUT_H_
#define COMMONCUTS_TRACK_CBMTOFM2POLYCUT_H_

#include "NicaTrackToFMass2Cut.h"

class CbmTofM2PolyCut : public NicaTrackToFMass2Cut {
public:
  CbmTofM2PolyCut();
  Bool_t Pass(NicaTrack* track);
  virtual ~CbmTofM2PolyCut();
  ClassDef(CbmTofM2PolyCut, 1)
};

#endif /* CBMROOT_ANALYSIS_PWGC2F_FEMTOSCOPY_NICAFEMTO_CUTS_COMMONCUTS_TRACK_CBMTOFM2POLYCUT_H_ */
