/*
 * NicaEventBinCut.h
 *
 *  Created on: 1 paź 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEVENTBINNINGCUT_H_
#define NICAEVENTBINNINGCUT_H_

#include "NicaArray.h"
#include "NicaEventCut.h"

#include <initializer_list>
#include <vector>


class NicaEventBinningCut : public NicaEventCut {
protected:
  Int_t fTotalBinsNo;
  NicaArray_1<Float_t> fMinTotal;
  NicaArray_1<Float_t> fMaxTotal;
  NicaArray_1<Float_t> fStep;
  NicaArray_1<Int_t> fBinConv;
  NicaArray_1<Int_t> fStepsNo;
  NicaEventCut* fEventCut;
  NicaEventBinningCut(Int_t nparamNo);
  NicaEventBinningCut(const NicaEventCut& cut, const std::vector<int> init);

public:
  NicaEventBinningCut();
  NicaEventBinningCut(const NicaEventCut& cut,
                      const std::initializer_list<int>& init);
  NicaEventBinningCut(const NicaEventBinningCut& other);
  NicaEventBinningCut* MakeCopy() const;
  NicaEventBinningCut* MakeCopyReal() const;
  NicaEventBinningCut* MakeCopyImg() const;
  NicaEventBinningCut& operator=(const NicaEventBinningCut& other);
  Int_t GetBinsNo() const { return fTotalBinsNo; };
  virtual NicaPackage* Report() const;
  virtual Bool_t Pass(NicaEvent* event) { return kTRUE; };
  virtual Int_t CheckBin(NicaEvent* event);
  virtual Bool_t Init(Int_t task_id);
  virtual ~NicaEventBinningCut();
  ClassDef(NicaEventBinningCut, 1)
};

#endif /* NICAEVENTBINNINGCUT_H_ */
