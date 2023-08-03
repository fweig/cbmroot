/*
 * NicaCutMonitor.h
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#ifndef NICACUTMONITOR_H_
#define NICACUTMONITOR_H_

#include <TObject.h>
#include <TString.h>

#include "NicaStd.h"

class NicaCut;
class NicaPackage;

/**
 * abstract class for all cut monitors
 */
class NicaCutContainer;
class NicaCutMonitor : public TObject {
  friend class NicaCutContainer;

 protected:
  /**
   * number of axis in cut monitor
   */
  const Int_t fAxisNo;
  /**
   * number of currently added cuts
   */
  Int_t fCuts;
  /**
   * collection number of this cut monitor
   */
  Int_t fCollectionID;
  /**
   * array with numbers of axis bins
   */
  Int_t *fAxisBins;  //[fAxisNo]
  /**
   * array with parameters numbers used from cuts for all axes
   */
  Int_t *fOptionAxis;  //[fAxisNo]
  /**
   * histogram with passed objects
   */
  TH1 *fHistoPassed;
  /**
   * histogram with failed objects
   */
  TH1 *fHistoFailed;
  /**
   * array with lower edges of axes
   */
  Double_t *fAxisMin;  //[fAxisNo]
  /**
   * array with upper edges of axes
   */
  Double_t *fAxisMax;  //[fAxisNo]
  /**
   * init flag, true if monitor has been initialized
   */
  Bool_t fInit;
  /**
   * exclusive flag, true if monitor works in exclusive mode
   */
  Bool_t fExUpdate;
  /**
   * array with pointers to monitored cuts
   */
  NicaCut **fNicaCut;  //[fAxisNo]
  /**
   * array with pointers to names of monitored cuts
   */
  TString *fCutNames;  //[fAxisNo]
  /**
   * update ratio of this cut monitor
   */
  ENicaCutUpdate fUpdateRatio;
  /**
   * allocate histograms
   */
  virtual void CreateHistograms();
  /**
   * make update of histogram
   * @param passed true if object goes to pass false otherwise
   */
  virtual void TrueUpdate(Bool_t passed);
  /**
   * confgiure axis of histogram
   * @param bins number of bins
   * @param min lower edge of axis
   * @param max upper edge of axis
   * @param opt axis numger
   */
  void SetAxis(Int_t bins, Double_t min, Double_t max, Int_t opt);
  /**
   *
   * @param i cut number
   * @return cut monitred by this cut monitor
   */
  NicaCut *GetCut(Int_t i) const;

 public:
  /**
   * default constructor
   * @param size number of axes
   */
  NicaCutMonitor(Int_t size = 0);
  /**
   * copy constructor
   * @param other object to copy
   */
  NicaCutMonitor(const NicaCutMonitor &other);
  /**
   * Assignment operator
   * @param other
   * @return
   */
  NicaCutMonitor &operator=(const NicaCutMonitor &other);
  /**
   * reset this cut monitor
   */
  void Reset();
  /**
   * add cut to this monitor
   * @param cut cut name
   * @param parameter_no number of paramter that should be monitored from given
   * cut
   */
  void AddCut(TString cut, Int_t parameter_no = 0);
  /**
   * setup X axis
   * @param bins number of bins
   * @param min lower limit of axis
   * @param max upper limit of axis
   */
  void SetXaxis(Int_t bins, Double_t min, Double_t max);
  /**
   * setup Y axis
   * @param bins number of bins
   * @param min lower limit of axis
   * @param max upper limit of axis
   */
  void SetYaxis(Int_t bins, Double_t min, Double_t max);
  /**
   * setup Z axis
   * @param bins number of bins
   * @param min lower limit of axis
   * @param max upper limit of axis
   */
  void SetZaxis(Int_t bins, Double_t min, Double_t max);
  /**
   * add pointer to moniter cut, this option should be used inside of cut
   * monitors/applications not in ROOT macros
   * @param cut pointer to monitored cut
   * @param no axis number
   */
  void AddForcedCut(NicaCut *cut, Int_t no);
  /**
   * copy setting of axis from different cut
   * @param original cut with is used for taking values
   */
  void CopySettings(const NicaCutMonitor *original);
  /**
   * set collection number for this cut monitor
   * @param i collection number
   */
  void SetCollectionID(Int_t i);
  /**
   * enable exclusive updates mode
   */
  void EnableExclusiveUpdate();
  /**
   *
   * @return this collection no
   */
  Int_t GetCollectionID() const { return fCollectionID; };
  /**
   *
   * @return number of axes
   */
  Int_t GetAxisNo() const { return fAxisNo; };
  /**
   *
   * @param i axis number / cut number
   * @return number of parameter monitored for given cut/axis
   */
  Int_t GetCutParameter(Int_t i) const;
  /**
   *
   * @param other
   * @return true if both monitors are similar
   */
  Bool_t AreSimilar(NicaCutMonitor *other) const;
  /**
   *
   * @param i axes no
   * @return cut name
   */
  TString GetCutName(Int_t i) const { return fCutNames[i]; };
  /**
   *
   * @return update ratio of this monitor
   */
  ENicaCutUpdate GetUpdateRatio() const;
  /**
   *
   * @return true if method used UpdateByObject instead of Update
   */
  virtual Bool_t ObjMonitor() const { return kFALSE; };
  /**
   * initialize this cut monitor
   * @param task_id task number
   */
  virtual Bool_t Init(Int_t task_id);
  /**
   * make update of cut monitor
   * @param passed state used for update
   * @param obj - passed object (not used in non-obj cuts
   */
  virtual void Update(Bool_t passed, TObject *obj);
  /**
   * make copy of this object
   * @return copy of this
   */
  virtual NicaCutMonitor *MakeCopy() const;
  /**
   *
   * @return report from this cut monitor
   */
  virtual NicaPackage *Report() const;
  /**
   *
   * @param i axis no
   * @return pointer to linked cut
   */
  NicaCut *GetCutLink(Int_t i) const { return fNicaCut[i]; };
  virtual ~NicaCutMonitor();
  ClassDef(NicaCutMonitor, 1)
};

#endif /* NICACUTMONITOR_H_ */
