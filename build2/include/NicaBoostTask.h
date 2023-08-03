/*
 * NicaBoostTask.h
 *
 *  Created on: 23 lip 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICABOOSTTASK_H_
#define NICABOOSTTASK_H_

#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>

#include "NicaEventAna.h"

class NicaEventInterface;
class NicaTrackInterface;

/**
 * basic class for boosting  events, currently boost only oryginal event in tree
 */
class NicaBoostTask : public NicaEventAna {
  Double_t fBoostVx;
  Double_t fBoostVy;
  Double_t fBoostVz;
  NicaEventInterface *fEventInterface;
  NicaTrackInterface *fTrackInterface;

 protected:
  InitStatus Init();
  virtual NicaPackage *Report() const;

 public:
  /**
   * default ctor
   */
  NicaBoostTask();
  /**
   * constructor
   * @param vx x boost component
   * @param vy y boost component
   * @param vz z boost component
   */
  NicaBoostTask(Double_t vx, Double_t vy, Double_t vz);
  /**
   * constructor
   * @param vz x boost component
   */
  NicaBoostTask(Double_t vz);
  /**
   * set boost direction
   * @param vx x component of velocity
   * @param vy y component of velocity
   * @param vz z component of velocity
   * @param opt not used
   */
  void SetBoost(Double_t vx, Double_t vy, Double_t vz, Option_t *opt = "");
  /**
   *
   * @param vz  z component of velocity
   * @param opt not used
   */
  void SetBoost(Double_t vz, Option_t *opt = "");
  void Exec(Option_t *opt);
  virtual ~NicaBoostTask();
  ClassDef(NicaBoostTask, 1)
};

#endif /* NICABOOSTTASK_H_ */
