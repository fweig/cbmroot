/** @file CbmMvdHitfinder.h
  * @copyright Copyright (C) 2014-2020 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Philipp Sitzmann [committer] **/

// ----------------------------------------------------------------------------
// -----                    CbmMvdHitfinder header file                    -----
// -----                   Created by C. Dritsa (2009)                    -----
// -----                   Maintained by M.Deveaux (m.deveaux(att)gsi.de) -----
// ----------------------------------------------------------------------------

#ifndef CBMMVDHITFINDER_H
#define CBMMVDHITFINDER_H 1

#include "FairTask.h"

#include "TStopwatch.h"

class CbmMvdDetector;
class CbmDigiManager;


class CbmMvdHitfinder : public FairTask {

public:
  /** Default constructor **/
  CbmMvdHitfinder();


  /** Standard constructor 
  *@param name  Task name

  **/
  CbmMvdHitfinder(const char* name, Int_t iVerbose = 1);
  CbmMvdHitfinder(const char* name, Int_t mode, Int_t iVerbose);

  /** Destructor **/
  virtual ~CbmMvdHitfinder();

  void Exec(Option_t* opt);
  void UseClusterfinder(Bool_t clusterfinderFlag)
  {
    useClusterfinder = clusterfinderFlag;
  };  //* enable use of external clusterfinder
  void ShowDebugHistos() { fShowDebugHistos = kTRUE; }

protected:
private:
  CbmMvdDetector* fDetector;
  CbmDigiManager* fDigiMan;  //!

  TClonesArray* fInputCluster;
  TClonesArray* fHits;

  UInt_t fHitfinderPluginNr;
  Bool_t useClusterfinder;
  Bool_t fShowDebugHistos;
  TStopwatch fTimer;  ///< ROOT timer
  Int_t fmode;

  // -----   Private methods   ---------------------------------------------
  /** Intialisation **/
  virtual InitStatus Init();


  /** Reinitialisation **/
  virtual InitStatus ReInit();


  /** Virtual method Finish **/
  virtual void Finish();


  /** Register the output arrays to the IOManager **/
  void Register();


  /** Clear the hit arrays **/
  void Reset();

  void GetMvdGeometry();

  /** Print digitisation parameters **/
  void PrintParameters();

private:
  CbmMvdHitfinder(const CbmMvdHitfinder&);
  CbmMvdHitfinder operator=(const CbmMvdHitfinder&);

  ClassDef(CbmMvdHitfinder, 1);
};


#endif
