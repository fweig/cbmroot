/** @file CbmBasicFemtoPairCut.h
  * @copyright Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * CbmBasicFemtoPairCut.h
 *
 *  Created on: 22 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMBASICFEMTOPAIRCUT_H_
#define CBMBASICFEMTOPAIRCUT_H_

#include <TVector3.h>

#include "NicaCutsAndMonitors.h"

class CbmStsExitSepCut;
class CbmDeltaPhiDeltaThetaStarCut;

class CbmBasicFemtoPairCut : public NicaCutsAndMonitors {
  TVector3 fMonitorPhiPair[2];
  TVector3 fExitSeparation;
  CbmStsExitSepCut* GetStsExitCut() const { return (CbmStsExitSepCut*) CutAt(0); }
  CbmDeltaPhiDeltaThetaStarCut* GetDeltaPhiEtaStarCut() const { return (CbmDeltaPhiDeltaThetaStarCut*) CutAt(1); }

public:
  CbmBasicFemtoPairCut();
  void SetSeparationMonitorAxis(Int_t nbins, Double_t min, Double_t max) { fExitSeparation.SetXYZ(nbins, min, max); }
  void SetDeltaPhiStarAxis(Int_t bin, Double_t min, Double_t max) { fMonitorPhiPair[0].SetXYZ(bin, min, max); }
  void SetDeltaEtaStarAxis(Int_t bin, Double_t min, Double_t max) { fMonitorPhiPair[1].SetXYZ(bin, min, max); }
  void SetDeltaPhiStarCut(Double_t min, Double_t max);
  void SetDeltaEtaStarCut(Double_t min, Double_t max);
  void SetR(Double_t R);
  void SetSTSExitSeparationCut(Double_t min, Double_t max);
  void CreateBasicMonitors();
  virtual ~CbmBasicFemtoPairCut();
  ClassDef(CbmBasicFemtoPairCut, 1)
};

#endif /* CBMBASICFEMTOPAIRCUT_H_ */
