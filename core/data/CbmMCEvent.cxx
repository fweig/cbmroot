/* Copyright (C) 2009 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** CbmMCEvent.cxx
 *@author V.Friese <v.friese@gsi.de>
 ** Data class (level MC) containing information about the input event.
 ** 15.05.2008 change the event time to ns (M. Al-Turany)
 ** 11.05.2009 New CBM class derived from FairMCEventHeader
 **/


#include "CbmMCEvent.h"


// -----   Default constructor   ------------------------------------------
CbmMCEvent::CbmMCEvent()
  : TNamed("MC Event", "CBM MC Event")
  , fRunId(0)
  , fEventId(0)
  , fX(0.)
  , fY(0.)
  , fZ(0.)
  , fT(0.)
  , fPhi(0.)
  , fB(0.)
  , fNPrim(0)
  , fIsSet(false)

{
}
// ------------------------------------------------------------------------


// -----   Constructor with run identifier   ------------------------------
CbmMCEvent::CbmMCEvent(uint32_t runId)
  : TNamed("MC Event", "CBM MC Event")
  , fRunId(runId)
  , fEventId(0)
  , fX(0.)
  , fY(0.)
  , fZ(0.)
  , fT(0.)
  , fB(0.)
  , fPhi(0.)
  , fNPrim(0)
  , fIsSet(false)

{
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
CbmMCEvent::CbmMCEvent(uint32_t runId, int32_t iEvent, double x, double y, double z, double t, double b, double phi,
                       int32_t nPrim)
  : TNamed("MCEvent", "MC")
  , fRunId(0)
  , fEventId(iEvent)
  , fX(x)
  , fY(y)
  , fZ(z)
  , fT(t)
  , fB(b)
  , fPhi(phi)
  , fNPrim(nPrim)
  , fIsSet(false)

{
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
CbmMCEvent::~CbmMCEvent() {}
// ------------------------------------------------------------------------


// -----   Public method Reset   ------------------------------------------
void CbmMCEvent::Reset()
{
  fEventId = fNPrim = 0;
  fX = fY = fZ = fT = fB = fPhi = 0.;
  fIsSet                        = false;
}
// ------------------------------------------------------------------------


ClassImp(CbmMCEvent)
