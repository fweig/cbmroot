/* Copyright (C) 2017-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// ----------------------------------------------------------------------------
// -----               CbmMvdClusterfinderTB header file                    -----
// -----                   Created by P.Sitzmann 03.12.2014               -----
// ----------------------------------------------------------------------------


#ifndef CBMMVDCLUSTERFINDERTB_H
#define CBMMVDCLUSTERFINDERTB_H 1


#include "FairTask.h"

#include "TStopwatch.h"

#include <string>;

class CbmMvdDetector;
class TClonesArray;
class TString;
class CbmDigiManager;


class CbmMvdClusterfinderTB : public FairTask {

public:
  /** Default constructor **/
  CbmMvdClusterfinderTB();


  /** Standard constructor 
  *@param name  Task name
  *@param mode  0 = MAPS, 1 = Ideal
  **/
  CbmMvdClusterfinderTB(const char* name, Int_t mode = 0, Int_t iVerbose = 1);


  /** Destructor **/
  ~CbmMvdClusterfinderTB();

  void Exec(Option_t* opt);

  void ShowDebugHistos() { fShowDebugHistos = kTRUE; }

private:
  /** Hit producer mode (0 = MAPS, 1 = Ideal) **/
  Int_t fMode;
  Bool_t fShowDebugHistos;
  CbmMvdDetector* fDetector;

  TClonesArray* fEvents;
  CbmDigiManager* fDigiMan;  //!
  TClonesArray* fEventDigis;
  TClonesArray* fCluster;


  UInt_t fClusterPluginNr;

  TString fBranchName;  // Name of input branch (MvdDigi)


  TStopwatch fTimer;  ///< ROOT timer


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
  std::string ParametersToString() const;

private:
  CbmMvdClusterfinderTB(const CbmMvdClusterfinderTB&);
  CbmMvdClusterfinderTB operator=(const CbmMvdClusterfinderTB&);

  ClassDef(CbmMvdClusterfinderTB, 1);
};


#endif
