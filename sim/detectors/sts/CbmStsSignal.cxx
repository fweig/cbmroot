/** @file CbmStsSignal.cxx
  * @copyright Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsSignal.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 28.06.2014
 **/

#include "CbmStsSignal.h"


// -----   Default constructor   -------------------------------------------
CbmStsSignal::CbmStsSignal(Double_t time, Double_t charge, Int_t index, Int_t entry, Int_t file)
  : TObject()
  , fTime(time)
  , fMatch()
{
  fMatch.AddLink(charge, index, entry, file);
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsSignal::~CbmStsSignal() {}
// -------------------------------------------------------------------------


ClassImp(CbmStsSignal)
