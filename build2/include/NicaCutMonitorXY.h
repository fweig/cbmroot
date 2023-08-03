/*
 * NicaCutMonitorXY.h
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#ifndef NICACUTMONITORXY_H_
#define NICACUTMONITORXY_H_

#include "NicaCutMonitor.h"
/**
 * class for monitoring two cuts/parameters
 */
class NicaCutMonitorXY : public NicaCutMonitor {
#ifdef MPPCUTFULL
  Bool_t fIdenticalCuts;
#endif
 protected:
  void TrueUpdate(Bool_t passed);
  void CreateHistograms();

 public:
  NicaCutMonitorXY();
  /**
   *
   * @param cut_A name of cut monitored on X axis
   * @param cut_B name of cut monitored on Y axis
   */
  NicaCutMonitorXY(TString cut_A, TString cut_B);
  /**
   *
   * @param cut_A name of cut monitored on X axis
   * @param parameter_A number of parameter monitored on X axis
   * @param cut_B name of cut monitored on Y axis
   * @param parameter_B number of parameter monitored on Y axis
   */
  NicaCutMonitorXY(TString cut_A, Int_t parameter_A, TString cut_B,
                   Int_t parameter_B);
  /**
   * copy c-tor
   * @param other object to copy
   */
  NicaCutMonitorXY(const NicaCutMonitorXY &other);
  /**
   * Assignment operator
   * @param other
   * @return
   */
  NicaCutMonitorXY &operator=(const NicaCutMonitorXY &other);
  virtual Bool_t Init(Int_t task_id);
  virtual void Update(Bool_t passed, TObject *obj);
  virtual NicaCutMonitor *MakeCopy() const;
  virtual ~NicaCutMonitorXY();
  ClassDef(NicaCutMonitorXY, 1)
};

#endif /* NICACUTMONITORXY_H_ */
