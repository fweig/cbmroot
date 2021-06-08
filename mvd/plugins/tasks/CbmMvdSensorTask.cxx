/* Copyright (C) 2012-2015 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: M. Deveaux, Philipp Sitzmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                  CbmMvdSensorTask source file              -----
// -----                  Created 02.02.2012 by M. Deveaux            -----
// -------------------------------------------------------------------------

#include "CbmMvdSensorTask.h"

#include "TClonesArray.h"
#include "TObjArray.h"


// -----   Default constructor   -------------------------------------------
CbmMvdSensorTask::CbmMvdSensorTask() : CbmMvdSensorPlugin(), fInputBuffer(NULL), fOutputBuffer(NULL), fSensor(NULL) {}
// -------------------------------------------------------------------------
// -----  constructor   -------------------------------------------
CbmMvdSensorTask::CbmMvdSensorTask(const char* name)
  : CbmMvdSensorPlugin(name)
  , fInputBuffer(NULL)
  , fOutputBuffer(NULL)
  , fSensor(NULL)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdSensorTask::~CbmMvdSensorTask()
{
  fInputBuffer->Delete();

  fOutputBuffer->Delete();
}
// -------------------------------------------------------------------------

ClassImp(CbmMvdSensorTask)
