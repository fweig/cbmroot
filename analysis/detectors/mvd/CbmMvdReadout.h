/* Copyright (C) 2017-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// ----------------------------------------------------------------------------
// -----               CbmMvdReadout header file                          -----
// -----               Created by P.Sitzmann 12.06.2017                   -----
// ----------------------------------------------------------------------------


#ifndef CBMMVDREADOUT_H
#define CBMMVDREADOUT_H 1

#include <FairTask.h>  // for InitStatus, FairTask

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Bool_t, Option_t, kTRUE

class CbmDigiManager;
class CbmMvdDetector;
class TBuffer;
class TClass;
class TMemberInspector;

class CbmMvdReadout : public FairTask {

public:
  /** Default constructor **/
  CbmMvdReadout();


  /** Standard constructor 
  *@param name  Task name
  *@param mode  0 = MAPS, 1 = Ideal
  **/
  CbmMvdReadout(const char* name);


  /** Destructor **/
  ~CbmMvdReadout();

  void Exec(Option_t* opt);

  void ShowDebugHistos() { fShowDebugHistos = kTRUE; }

private:
  /** Hit producer mode (0 = MAPS, 1 = Ideal) **/
  Int_t fMode;
  Int_t fPluginNr;
  Bool_t fShowDebugHistos;
  CbmMvdDetector* fDetector;
  CbmDigiManager* fDigiMan;


  // -----   Private methods   ---------------------------------------------
  /** Intialisation **/
  virtual InitStatus Init();


  /** Reinitialisation **/
  virtual InitStatus ReInit();


  /** Virtual method Finish **/
  virtual void Finish();


  /** Register the output arrays to the IOManager **/
  void Register();

  void GetMvdGeometry();


  /** Clear the hit arrays **/
  void Reset();


  /** Print digitisation parameters **/
  void PrintParameters() const;

  CbmMvdReadout(const CbmMvdReadout&);
  CbmMvdReadout operator=(const CbmMvdReadout&);

  ClassDef(CbmMvdReadout, 1);
};


#endif
