/*
 * NicaFemtoEventBinsAna.h
 *
 *  Created on: 4 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_ANALYSIS_FEMTO_ANA_NICAFEMTOEVENTBINSANA_H_
#define NICAFEMTO_ANALYSIS_FEMTO_ANA_NICAFEMTOEVENTBINSANA_H_

#include "NicaFemtoBasicAna.h"
#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>

#include "NicaArray.h"

class NicaEventBinningCut;

class NicaFemtoEventBinsAna : public NicaFemtoBasicAna {
protected:
  /**
   * holds array of event cuts used for binning mixing
   */
  TObjArray* fEventBinningCuts;
  /**
   * Contains number of bins in event cuts used for binning
   */
  NicaArray_1<Int_t>* fEventBinnngsBinsNo;
  Int_t fTotalEventBins;

  virtual void ProcessEvent();
  virtual NicaPackage* Report() const;
  virtual Int_t GetEventBin();
  virtual InitStatus Init();
  virtual void InitMemoryMap();

public:
  NicaFemtoEventBinsAna();
  virtual ~NicaFemtoEventBinsAna();
  virtual void AddEventBinCut(const NicaEventBinningCut& bin, Option_t* opt);
  NicaFemtoEventBinsAna(const NicaFemtoEventBinsAna& other);
  NicaFemtoEventBinsAna& operator=(const NicaFemtoEventBinsAna& other);
  ClassDef(NicaFemtoEventBinsAna, 1)
};


#endif /* NICAFEMTO_ANALYSIS_FEMTO_ANA_NICAFEMTOEVENTBINSANA_H_ */
