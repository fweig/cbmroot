/* Copyright (C) 2014-2016 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer], Florian Uhlig */

// ------------------------------------------------------------------------
// -----                  CbmMvdSensorPlugin header file              -----
// -----                   Created 02/02/12  by M. Deveaux            -----
// ------------------------------------------------------------------------

/**  CbmMvdSensorPlugin.h
 *@author M.Deveaux <deveaux@physik.uni-frankfurt.de>
 **
 ** Base class for the plugins of the MVD sensor 
 **
 **/

#ifndef CBMMVDSENSORPLUGIN_H
#define CBMMVDSENSORPLUGIN_H 1

#include <Logger.h>  // for LOG

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Bool_t, Int_t, kTRUE
#include <TObject.h>     // for TObject

class TBuffer;
class TClass;
class TClonesArray;  // lines 24-24
class TMemberInspector;

enum MvdSensorPluginType
{
  task,
  buffer
};

class CbmMvdSensorPlugin : public TObject {

public:
  /** Default constructor **/
  CbmMvdSensorPlugin();
  CbmMvdSensorPlugin(const char* name);

  /** Destructor **/
  virtual ~CbmMvdSensorPlugin();

  virtual MvdSensorPluginType GetPluginType() = 0;
  virtual Int_t GetPluginIDNumber() { return fPluginIDNumber; }
  virtual void Init() { ; }
  virtual void Exec() { ; }
  virtual void ExecChain() { ; }
  virtual void Finish() { ; };

  virtual TClonesArray* GetInputArray() { return 0; }
  virtual TClonesArray* GetOutputArray() { return 0; }
  virtual TClonesArray* GetMatchArray() { return 0; }
  virtual TClonesArray* GetWriteArray() { return 0; }

  virtual void SetInputArray(TClonesArray*) { ; }
  virtual void SetInput(TObject*) { LOG(error) << "You are sending input to the base class instead to your plugin!"; }
  virtual void SetOutputArray(TClonesArray*) { ; }

  virtual void SetNextPlugin(CbmMvdSensorPlugin* plugin) { fNextPlugin = plugin; }
  virtual void SetPreviousPlugin(CbmMvdSensorPlugin* plugin) { fPreviousPlugin = plugin; }

  virtual CbmMvdSensorPlugin* GetNextPlugin() { return fNextPlugin; }
  virtual CbmMvdSensorPlugin* GetPrevousPlugin() { return fPreviousPlugin; }

  bool PluginReady() { return (bFlag); };
  void SetPluginReady(bool flag) { bFlag = flag; }
  void ShowDebugHistos() { fShowDebugHistos = kTRUE; }
  virtual const char* GetName() const { return fName; }
  Bool_t IsInit() { return (initialized); }
  /** data members **/

  CbmMvdSensorPlugin* fNextPlugin;
  CbmMvdSensorPlugin* fPreviousPlugin;

protected:
  bool bFlag;
  Bool_t initialized;
  Bool_t fShowDebugHistos;
  const char* fName;
  Int_t fPluginIDNumber;  //Identifier for the Plugin for debugging purposes. Hardcode in implementation please.

private:
  CbmMvdSensorPlugin& operator=(const CbmMvdSensorPlugin&);
  CbmMvdSensorPlugin(const CbmMvdSensorPlugin&);

  ClassDef(CbmMvdSensorPlugin, 1);
};


#endif
