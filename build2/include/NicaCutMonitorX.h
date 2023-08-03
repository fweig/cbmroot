/*
 * NicaCutMonitorX.h
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#ifndef NICACUTMONITORX_H_
#define NICACUTMONITORX_H_

#include "NicaCutMonitor.h"
/**
 * class for monitoring one cut/one paramter
 */
class NicaCutMonitorX : public NicaCutMonitor {
 protected:
  void TrueUpdate(Bool_t);
  virtual void CreateHistograms();

 public:
  NicaCutMonitorX();

  /**
   *
   * @param cut_name name of monitored cut
   * @param parameter_no number of monitored parameter
   */
  NicaCutMonitorX(TString cut_name, Int_t parameter_no = 0);
  /**
   * copy ct-or
   * @param other object that should be copied
   */
  NicaCutMonitorX(const NicaCutMonitorX &other);
  /**
   * Assignment operator
   * @param other
   * @return
   */
  NicaCutMonitorX &operator=(const NicaCutMonitorX &other);
  virtual Bool_t Init(Int_t task_id);
  virtual void Update(Bool_t passed, TObject *obj);
  virtual NicaCutMonitor *MakeCopy() const;
  virtual ~NicaCutMonitorX();
  ClassDef(NicaCutMonitorX, 1)
};

#endif /* NICACUTMONITORX_H_ */
