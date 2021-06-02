/** @file CbmTemplateAlgo.h
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                            CbmTemplateAlgo                        -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef Cbm_TemplateAlgo_H
#define Cbm_TemplateAlgo_H

#include "CbmAlgo.h"

// Data
#include "CbmStsHit.h"
#include "CbmStsPoint.h"

// CbmRoot

// C/C++
#include <vector>

class CbmTemplateAlgo : public CbmAlgo<CbmStsPoint, CbmStsHit> {
public:
  CbmTemplateAlgo();
  ~CbmTemplateAlgo();
  CbmTemplateAlgo(const CbmTemplateAlgo&) = delete;
  CbmTemplateAlgo& operator=(const CbmTemplateAlgo&) = delete;

  virtual Bool_t Init();
  virtual void Reset();
  virtual void Finish();

  Bool_t InitContainers();
  Bool_t ReInitContainers();
  TList* GetParList();

  Bool_t InitParameters();

  virtual std::vector<CbmStsHit> ProcessInputData(std::vector<CbmStsPoint>);

private:
  /// Settings from parameter file
  //      CbmTemplatePar* fTemplatePar;      //!
};

#endif
