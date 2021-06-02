/** @file CbmNicaCompression.h
  * @copyright Copyright (C) 2020 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * CbmNicaCompression.h
 *
 *  Created on: 7 lut 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMNICACOMPRESSION_H_
#define CBMNICACOMPRESSION_H_

#include "FairTask.h"

#include "TClonesArray.h"

#include "NicaArray.h"
#include "NicaTrackClones.h"

class CbmNicaCompression : public FairTask {
  NicaTrackClones* fStsMatches;
  NicaTrackClones* fTofMatches;
  NicaTrackClones* fMCTracks;
  TClonesArray* fStsLinks;
  TClonesArray* fTofLinks;
  NicaArray_1<Int_t> fMapUse;
  NicaArray_1<Int_t> fMapIndex;
  Bool_t fAllDep;
  void NoDep();
  void WithDep();

public:
  CbmNicaCompression();
  InitStatus Init();
  void AllDependencies() { fAllDep = kTRUE; };
  void Exec(Option_t* opt);
  virtual ~CbmNicaCompression();
  ClassDef(CbmNicaCompression, 1)
};

#endif /* CBMNICACOMPRESSION_H_ */
