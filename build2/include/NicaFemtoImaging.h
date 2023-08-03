/*
 * NicaFemtoImaging.h
 *
 *  Created on: 20 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOIMAGING_H_
#define NICAFEMTOIMAGING_H_
#include <TObject.h>
/**
 * abstract class for imaging CF's
 */
class NicaFemtoImaging : public TObject {
 protected:
  /**
   * flag for simmetrization 1 for bosons with spin 0, -1 for fermions
   */
  Int_t fSymmetrization;

 public:
  NicaFemtoImaging();
  void SetFermions() { fSymmetrization = -1; };
  virtual ~NicaFemtoImaging();
  ClassDef(NicaFemtoImaging, 1)
};

#endif /* NICAROOT_ANALYSIS_FEMTO_IMAGING_NICAFEMTOIMAGING_H_ */
