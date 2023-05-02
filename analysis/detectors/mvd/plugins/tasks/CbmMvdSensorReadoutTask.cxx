/* Copyright (C) 2017-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                  CbmMvdSensorReadoutTask source file          -----
// -----                  Created 15.05.2017 by P. Sitzmann            -----
// -------------------------------------------------------------------------
#include "CbmMvdSensorReadoutTask.h"

#include "CbmMvdDigi.h"  // for CbmMvdDigi

#include <Logger.h>

#include <TClonesArray.h>  // for TClonesArray
#include <TH1.h>           // for TH1I

// -----   Default constructor   -------------------------------------------
CbmMvdSensorReadoutTask::CbmMvdSensorReadoutTask()
  : CbmMvdSensorTask()
  , fInputBuffer(nullptr)
  , fOutputBuffer(nullptr)
  , fSensor(nullptr)
  , fSensorBanks()
{
  //    fSensorBanks[maxBanks] = {0};
  fPluginIDNumber = 600;
}
// -------------------------------------------------------------------------
// -----  constructor   -------------------------------------------
CbmMvdSensorReadoutTask::CbmMvdSensorReadoutTask(const char* name)
  : CbmMvdSensorTask(name)
  , fInputBuffer(nullptr)
  , fOutputBuffer(nullptr)
  , fSensor(nullptr)
  , fSensorBanks()
{
  //    fSensorBanks[maxBanks] = {0};
  fPluginIDNumber = 600;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdSensorReadoutTask::~CbmMvdSensorReadoutTask()
{
  fInputBuffer->Delete();
  fOutputBuffer->Delete();
}
// -------------------------------------------------------------------------

// -----    Virtual private method Init   ----------------------------------
void CbmMvdSensorReadoutTask::InitTask(CbmMvdSensor* mysensor)
{

  fSensor       = mysensor;
  fInputBuffer  = new TClonesArray("CbmMvdDigi", 10000);
  fOutputBuffer = new TClonesArray("TH1", 1);


  initialized = kTRUE;
}
// -------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void CbmMvdSensorReadoutTask::SetInputArray(TClonesArray* inputStream)
{

  Int_t i       = 0;
  Int_t nInputs = inputStream->GetEntriesFast();
  while (nInputs > i) {
    new ((*fInputBuffer)[fInputBuffer->GetEntriesFast()]) CbmMvdDigi(*((CbmMvdDigi*) inputStream->At(i)));
    ++i;
  }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void CbmMvdSensorReadoutTask::SetInputDigi(CbmMvdDigi* digi)
{

  new ((*fInputBuffer)[fInputBuffer->GetEntriesFast()]) CbmMvdDigi(*((CbmMvdDigi*) digi));
}
// -----------------------------------------------------------------------------

// -----   Virtual public method ExecChain   --------------
void CbmMvdSensorReadoutTask::ExecChain() { Exec(); }
// -------------------------------------------------------------------------

// -----   Virtual public method Exec   --------------
void CbmMvdSensorReadoutTask::Exec()
{

  for (Int_t i = 0; i < fInputBuffer->GetEntriesFast(); i++) {
    CbmMvdDigi* digi = (CbmMvdDigi*) fInputBuffer->At(i);
    Int_t iBank      = GetBankNumber(digi->GetPixelX());
    fSensorBanks[iBank]++;
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
Int_t CbmMvdSensorReadoutTask::GetBankNumber(const Int_t& xPixelNr) const
{
  Int_t iBank = 0;
  for (; iBank < maxBanks; ++iBank) {
    if (xPixelNr - (iBank * fPixelsPerBank) <= fPixelsPerBank) return iBank;
  }

  return iBank;
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdSensorReadoutTask::Reset() {}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdSensorReadoutTask::Finish()
{
  for (Int_t iBank = 0; iBank < maxBanks; ++iBank) {
    LOG(info) << "Number of fired pixels in bank : " << iBank << " is " << fSensorBanks[iBank];
    TH1I* hist = (TH1I*) fOutputBuffer->ConstructedAt(0);
    hist->Fill(iBank, fSensorBanks[iBank]);
  }
}
// -------------------------------------------------------------------------

ClassImp(CbmMvdSensorReadoutTask)
