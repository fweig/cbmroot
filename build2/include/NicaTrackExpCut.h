/*
 * NicaTrackExpCut.h
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKEXPCUT_H_
#define NICATRACKEXPCUT_H_

#include "NicaTrackCut.h"
/**
 * basic class for all track cuts that use reconstructed tracks
 */
class NicaTrackExpCut : public NicaTrackCut {
 protected:
  /**
   *
   * @param format_id
   * @return true if tracks return "tpc-track"
   */
  Bool_t TpcAvaiable(Int_t format_id) const;
  /**
   *
   * @param format_id
   * @return true if track return "tof-track"
   */
  Bool_t ToFAvaiable(Int_t format_id) const;

 public:
  NicaTrackExpCut(Int_t size = 1);
  virtual Bool_t Init(Int_t format_id = 0);
  virtual ~NicaTrackExpCut();
  ClassDef(NicaTrackExpCut, 1)
};

#endif /* NICATRACKEXPCUT_H_ */
