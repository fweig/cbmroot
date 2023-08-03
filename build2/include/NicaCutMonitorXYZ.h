/*
 * NicaCutMonitorXYZ.h
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#ifndef NICACUTMONITORXYZ_H_
#define NICACUTMONITORXYZ_H_

#include "NicaCutMonitor.h"
/**
 * class for monitoring 3 cuts/paramters
 */
class NicaCutMonitorXYZ : public NicaCutMonitor {
#ifdef MPPCUTFULL
  Int_t fWeightFlag;
#endif
 protected:
  void TrueUpdate(Bool_t pass);
  void CreateHistograms();

 public:
  NicaCutMonitorXYZ();
  /**
   *
   * @param cut_A name of cut monitored on X axis
   * @param cut_B name of cut monitored on Y axis
   * @param cut_C name of cut monitored on Z axis
   */
  NicaCutMonitorXYZ(TString cut_A, TString cut_B, TString cut_C);
  /**
   *
   * @param cut_A name of cut monitored on X axis
   * @param par_A number of parameter monitored on X axis
   * @param cut_B name of cut monitored on B axis
   * @param par_B number of parameter monitored on Y axis
   * @param cut_C name of cut monitored on C axis
   * @param par_C number of parameter monitored on Z axis
   */
  NicaCutMonitorXYZ(TString cut_A, Int_t par_A, TString cut_B, Int_t par_B,
                    TString cut_C, Int_t par_C);
  /**
   * copy c-tor
   * @param other object to copy
   */
  NicaCutMonitorXYZ(const NicaCutMonitorXYZ &other);
  /**
   * Assignment operator
   * @param other
   * @return
   */
  NicaCutMonitorXYZ &operator=(const NicaCutMonitorXYZ &other);
  virtual Bool_t Init(Int_t task_id);
  virtual void Update(Bool_t pass, TObject *obj);
  virtual NicaCutMonitor *MakeCopy() const;
  virtual ~NicaCutMonitorXYZ();
  ClassDef(NicaCutMonitorXYZ, 1)
};

#endif /* NICACUTMONITORXYZ_H_ */
