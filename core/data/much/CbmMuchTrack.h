/** @file CbmMuchTrack.h
  * @copyright Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev, Florian Uhlig [committer] **/

/**  CbmMuchTrack.h
 *@author A.Lebedev <andrey.lebedev@gsi.de>
 *@since 2007
 **
 ** Base class for local tracks in the MUCH detector.
 ** Derives from CbmTrack.
 **/

#ifndef CBMMUCHTRACK_H_
#define CBMMUCHTRACK_H_ 1

#include "CbmTrack.h"  // for CbmTrack

#include <Rtypes.h>  // for ClassDef

class CbmMuchTrack : public CbmTrack {
public:
  /** Default constructor **/
  CbmMuchTrack();

  /** Destructor **/
  virtual ~CbmMuchTrack();

  ClassDef(CbmMuchTrack, 3);
};

#endif
