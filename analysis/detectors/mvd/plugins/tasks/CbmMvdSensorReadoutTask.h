/* Copyright (C) 2017-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// ------------------------------------------------------------------------
// -----                  CbmMvdSensorTask header file              -----
// -----                   Created 02/02/12  by M. Deveaux            -----
// ------------------------------------------------------------------------

/**  CbmMvdSensorReadoutTask.h
 **
 **
 ** class for the readout plugins of the MVD sensor
 **
 **/

#ifndef CBMMVDSENSORREADOUTTASK_H
#define CBMMVDSENSORREADOUTTASK_H 1

#include "CbmMvdSensorTask.h"  // for CbmMvdSensorTask

#include <Rtypes.h>            // for ClassDef
#include <RtypesCore.h>        // for Int_t

class CbmMvdDigi;
class CbmMvdSensor;
class TBuffer;
class TClass;
class TClonesArray;
class TMemberInspector;

class CbmMvdSensorReadoutTask : public CbmMvdSensorTask {

public:
  /** Default constructor **/
  CbmMvdSensorReadoutTask();
  CbmMvdSensorReadoutTask(const char* name);

  /** Destructor **/
  virtual ~CbmMvdSensorReadoutTask();

  /** fill buffer **/
  void SetInputArray(TClonesArray* inputStream);
  void SetInputDigi(CbmMvdDigi* digi);

  virtual TClonesArray* GetOutputArray() { return fOutputBuffer; };

  void InitTask(CbmMvdSensor* mySensor);

  void Exec();
  void Finish();
  void ExecChain();

  void Reset();

private:
  static const Int_t maxBanks = 64;

  TClonesArray* fInputBuffer;  // Buffer of background events
  TClonesArray* fOutputBuffer;
  CbmMvdSensor* fSensor;

  Int_t fSensorBanks[maxBanks];
  const Int_t fPixelsPerBank = 22;

  Int_t GetBankNumber(const Int_t& yPixelNr) const;

  CbmMvdSensorReadoutTask(const CbmMvdSensorReadoutTask&);
  CbmMvdSensorReadoutTask operator=(const CbmMvdSensorReadoutTask&);

  ClassDef(CbmMvdSensorReadoutTask, 1);
};


#endif
