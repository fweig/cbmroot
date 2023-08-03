/*
 * NicaFluctuationsAna.h
 *
 *  Created on: 18 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFLUCTUATIONSANA_H_
#define NICAFLUCTUATIONSANA_H_

#include <FairTask.h>
#include <TH1.h>

#include "NicaArray.h"
#include "NicaHistogramManager.h"
#include "NicaTrackAna.h"

class NicaFluctuationsAna : public NicaTrackAna {
  NicaArray_1<Int_t> fArray;
  Int_t fBins;
  Int_t fTackCollectionsPerEvent;
  Double_t fMin;
  Double_t fMax;
  NicaHistogramManager_2_1D<TH1D>* fHistograms;

 protected:
  virtual void CheckCutContainerCollections();
  virtual void ProcessEvent();
  InitStatus Init();

 public:
  NicaFluctuationsAna();
  /**
   * set histogram axis
   * @param bins number of bins
   * @param min min
   * @param max max
   */
  void SetHistogramAxis(Int_t bins, Double_t min, Double_t max);
  /**
   * set histogram axis -  recommended method - in this case range is made in
   * following way that bin centers corresponds from 0, 1.. nbisn
   * @param bins number of bins
   * @param min min
   * @param max max
   */
  void SetHistogramAxis(Int_t bins);
  virtual NicaPackage* Report() const;
  virtual ~NicaFluctuationsAna();
  ClassDef(NicaFluctuationsAna, 1)
};

#endif /* NICAFLUCTUATIONSANA_H_ */
