/* Copyright (C) 2014-2017 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer] */

// ------------------------------------------------------------------------
// -----                  CbmMvdSensorTask header file              -----
// -----                   Created 02/02/12  by M. Deveaux            -----
// ------------------------------------------------------------------------

/**  CbmMvdSensorTask.h
 *@author M.Deveaux <deveaux@physik.uni-frankfurt.de>
 **
 ** Base class for the plugins of the MVD sensor 
 **
 **/

#ifndef CBMMVDSENSORTASK_H
#define CBMMVDSENSORTASK_H 1

#include "plugins/CbmMvdSensorPlugin.h"

#include "TObject.h"

#include <iostream>


class TClonesArray;
class CbmMvdSensor;
class CbmMvdPoint;
class CbmMvdDigi;

class CbmMvdSensorTask : public CbmMvdSensorPlugin {

public:
  /** Default constructor **/
  CbmMvdSensorTask();
  CbmMvdSensorTask(const char* name);

  /** Destructor **/
  virtual ~CbmMvdSensorTask();


  virtual void SendInputBuffer(TClonesArray* inputBuffer) { fInputBuffer = inputBuffer; };
  virtual void SetInput(CbmMvdPoint*) { ; };
  virtual void SetInputDigi(CbmMvdDigi*) { ; };
  virtual void CallBufferForInputData() { ; };  //See comment in ExecChain() in .cxx
  virtual TClonesArray* GetOutputArray() { return fOutputBuffer; };

  virtual void InitTask(CbmMvdSensor* mySensor) { fSensor = mySensor; };
  virtual void Exec() { ; };
  virtual void Finish() { ; };
  virtual void ExecChain() { ; };
  virtual bool PluginReady() { return (true); };
  /** Returns task type to a upper control unit **/
  MvdSensorPluginType GetPluginType() { return task; };


protected:
  TClonesArray* fInputBuffer;  // Buffer of background events
  TClonesArray* fOutputBuffer;
  CbmMvdSensor* fSensor;

private:
  CbmMvdSensorTask(const CbmMvdSensorTask&);
  CbmMvdSensorTask operator=(const CbmMvdSensorTask&);

  ClassDef(CbmMvdSensorTask, 1);
};


#endif
