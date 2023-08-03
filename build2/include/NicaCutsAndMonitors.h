/*
 * NicaCuts.h
 *
 *  Created on: 21 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACUTSANDMONITORS_NICACUTS_H_
#define NICACUTSANDMONITORS_NICACUTS_H_

#include <TString.h>

#include "NicaCutMonitor.h"
#include "NicaCut.h"

class TObjString;

/**
 * class that stores collection of hits and cut monitors
 * after adding to task cuts/cut monitors are extracted and added with "local"
 * and "global" option. E.g. NicaCutsAndMonitors::AddCut(some_cut,"{0}) and
 * NicaCutsAndMonitors::SetOptionForAllCuts("re") is equivalent of
 * NicaEventAna::AddCut(some_cut,"{0}+re")
 *
 */

class NicaCutsAndMonitors : public TObject {
  TObjArray *fCuts;
  TObjArray *fCutsOptions;
  TObjArray *fCutMonitors;
  TObjArray *fCutMonitorsOptions;
  TString fGlobalOptionCuts;
  TString fGlobalOptionsCutMonitors;

 protected:
  /**
   * return cut at i
   * @param i
   * @return
   */
  NicaCut *CutAt(Int_t i) const;
  /**
   *
   * @param i
   * @return monitor at
   */
  NicaCutMonitor *MonitorAt(Int_t i) const;
  /**
   * clear cuts and cuts options
   */
  void ClearCuts();
  /**
   * clear cut monitors and their options
   */
  void ClearMonitors();
  /**
   * add  cut without copying the cut, cut will be owned by this class
   * @param cut
   * @param opt
   */
  void AddRawCut(NicaCut *cut, TObjString *opt = nullptr);
  /**
   * add cut monitor without copying, this monitor will be owned by this class
   * @param mon
   * @param opt
   */
  void AddRawCutMonitor(NicaCutMonitor *mon, TObjString *opt = nullptr);

 public:
  NicaCutsAndMonitors();
  NicaCutsAndMonitors(const NicaCutsAndMonitors &other);
  NicaCutsAndMonitors &operator=(const NicaCutsAndMonitors &other);
  /**
   * set global option for all cuts
   * @param opt
   */
  void SetOptionForAllCuts(TString opt) { fGlobalOptionCuts = opt; };
  /**
   * set global option for all cut monitors
   * @param opt
   */
  void SetOptionForAllMonitors(TString opt) { fGlobalOptionsCutMonitors = opt; }
  /**
   * add cut to this container
   * @param cut
   * @param opt
   */
  void AddCut(const NicaCut &cut, Option_t *opt = "");
  /**
   * add cut monitor to this container note: this class doesn't have to containe
   * monitored cuts, they can be added later to the task
   * @param monitor
   * @param opt
   */
  void AddCutMonitor(const NicaCutMonitor &monitor, Option_t *opt = "");
  /**
   *
   * @param i
   * @return cut
   */
  const NicaCut *GetCut(Int_t i) const;
  /**
   *
   * @param i
   * @return "local" option for given cut
   */
  TString GetCutOption(Int_t i) const;
  /**
   *
   * @return "global" option for all cuts
   */
  TString GetGlobalCutOption() { return fGlobalOptionCuts; };
  /**
   *
   * @param i
   * @return i-th monitor
   */
  const NicaCutMonitor *GetMonitor(Int_t i) const;
  /**
   *
   * @return global cut monitor option
   */
  TString GetGlobalCutMonitorOption() { return fGlobalOptionsCutMonitors; };
  /**
   *
   * @param i
   * @return local cut monitor option
   */
  TString GetCutMonitorOption(Int_t i) const;
  /**
   *
   * @return number of cuts
   */
  Int_t GetNCuts() const;
  /**
   *
   * @return number of cut monitors
   */
  Int_t GetNCutMonitors() const;
  virtual ~NicaCutsAndMonitors();
  ClassDef(NicaCutsAndMonitors, 1)
};

#endif /* NICACUTSANDMONITORS_NICACUTS_H_ */
