/** @file CbmKresSelectAnn.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBM_KRES_SELECT_ANN
#define CBM_KRES_SELECT_ANN

#include "TMultiLayerPerceptron.h"
#include "TVector3.h"
#include <TClonesArray.h>

using namespace std;

class CbmKresSelectAnn {

public:
  //***** brief Standard constructor.
  CbmKresSelectAnn();
  //***** brief Standard destructor.
  virtual ~CbmKresSelectAnn();


  void Init();
  double DoSelect(double InvariantMass, double OpeningAngle, double PlaneAngle_last, double ZPos, TVector3 Momentum1,
                  TVector3 Momentum2);


private:
  std::string fAnnWeights;
  TMultiLayerPerceptron* fNN;

  //***** brief Copy constructor.
  CbmKresSelectAnn(const CbmKresSelectAnn&);

  //***** brief Assignment operator.
  CbmKresSelectAnn operator=(const CbmKresSelectAnn&);


  ClassDef(CbmKresSelectAnn, 1)
};

#endif
