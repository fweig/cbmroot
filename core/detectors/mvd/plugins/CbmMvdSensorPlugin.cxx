/* Copyright (C) 2012-2015 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                  CbmMvdSensorPlugin source file              -----
// -----                  Created 02.02.2012 by M. Deveaux            -----
// -------------------------------------------------------------------------
#include "CbmMvdSensorPlugin.h"

#include "TClonesArray.h"
#include "TObjArray.h"


// -----   Default constructor   -------------------------------------------
CbmMvdSensorPlugin::CbmMvdSensorPlugin()
  : TObject()
  , fNextPlugin(NULL)
  , fPreviousPlugin(NULL)
  , bFlag(false)
  , initialized(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fName("CbmMvdSensorPlugin")
  , fPluginIDNumber(-1)
{
}
// -------------------------------------------------------------------------
// -----   constructor   -------------------------------------------
CbmMvdSensorPlugin::CbmMvdSensorPlugin(const char* name)
  : TObject()
  , fNextPlugin(NULL)
  , fPreviousPlugin(NULL)
  , bFlag(false)
  , initialized(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fName(name)
  , fPluginIDNumber(-1)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdSensorPlugin::~CbmMvdSensorPlugin() {}
// -------------------------------------------------------------------------


ClassImp(CbmMvdSensorPlugin)
