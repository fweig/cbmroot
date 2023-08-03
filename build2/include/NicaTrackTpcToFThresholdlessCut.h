/*
 * NicaTrackTpcToFCut2.h
 *
 *  Created on: 17 lip 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKTPCTOFTHRESHOLDLESSCUT_H_
#define NICATRACKTPCTOFTHRESHOLDLESSCUT_H_

#include "NicaTrackTpcToFCut.h"

class NicaTrackTpcToFThresholdlessCut : public NicaTrackTpcToFCut {
 public:
  NicaTrackTpcToFThresholdlessCut(NicaTrackTpcCut *tpc = NULL,
                                  NicaTrackToFMass2Cut *tof = NULL);
  virtual Bool_t Pass(NicaTrack *tr);
  virtual ~NicaTrackTpcToFThresholdlessCut();
  ClassDef(NicaTrackTpcToFThresholdlessCut, 1)
};

#endif /* NICATRACKTPCTOFTHRESHOLDLESSCUT_H_ */
