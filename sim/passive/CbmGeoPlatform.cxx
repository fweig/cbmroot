/** @file CbmGeoPlatform.cxx
  * @copyright Copyright (C) 2013 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

/////////////////////////////////////////////////////////////
// CbmGeoPlatform
//
// Class for the geometry of detector part Platform
/////////////////////////////////////////////////////////////

#include "CbmGeoPlatform.h"

ClassImp(CbmGeoPlatform)

  CbmGeoPlatform::CbmGeoPlatform()
  : FairGeoSet()
  , modName()
  , eleName()
{
  // Constructor
  fName      = "platform";
  maxSectors = 0;
  maxModules = 1;
  strcpy(modName, "p");
  strcpy(eleName, "p");
}
