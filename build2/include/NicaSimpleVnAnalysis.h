/*
 * NicaSimpleVnAnalysis.h
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICASIMPLEVNANALYSIS_H_
#define NICASIMPLEVNANALYSIS_H_

#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

#include "NicaHistogramManager.h"
#include "NicaTrackAna.h"
#include "NicaFlowVariable.h"

/**
 * class for making simple flow RP analysis, for this analysis user can add
 * NicaFlowVariable, flow will be drawn as a function of this variable
 */
class NicaSimpleVnAnalysis : public NicaTrackAna {
  Int_t fBinsX, fBinsY;
  NicaFlowVariable *fVarX, *fVarY;
  Double_t fMinX, fMinY, fMaxX, fMaxY, fN, fPhi;
  NicaHistogramManager_1_2D<TH2D> *fNum;
  NicaHistogramManager_1_2D<TH2D> *fDen;
  Double_t GetPhi();

 protected:
  virtual void CheckCutContainerCollections();
  virtual void ProcessTrack();
  virtual void LinkCollections();
  /**
   * not used now
   */
  virtual void FinishAllEvents();
  NicaPackage *Report() const;
  virtual InitStatus Init();

 public:
  /**
   * default constructor
   * @param n coefficient used for calculating flow default 2 - for eliptic flow
   */
  NicaSimpleVnAnalysis(Double_t n = 2);
  /**
   * copy c-tor
   * @param ana
   */
  NicaSimpleVnAnalysis(const NicaSimpleVnAnalysis &ana);
  /**
   * set axis for flow plots
   * @param nbins number of bins
   * @param min lower edge of histogram
   * @param max upper edge of histogram
   * @param axis axis id  - 0 for X axis, 1 for Y axis
   */
  void SetAxis(Int_t nbins, Double_t min, Double_t max, Int_t axis);
  void SetFlowVariableX(const NicaFlowVariable &var);
  void SetFlowVariableY(const NicaFlowVariable &var);
  virtual void FinishTask();
  virtual ~NicaSimpleVnAnalysis();
  ClassDef(NicaSimpleVnAnalysis, 1)
};

#endif /* NICASIMPLEN2ANALYSIS_H_ */
