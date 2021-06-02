/** @file CbmBinnedTrackerConfigurator.h
  * @copyright Copyright (C) 2017-2020 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef CBMBINNEDTRACKERCONFIGURATOR_H
#define CBMBINNEDTRACKERCONFIGURATOR_H

#include "CbmPixelHit.h"

#include "FairTask.h"

#include "TClonesArray.h"
#include "Tracker.h"

#include "Settings.h"

class CbmBinnedTrackerConfigurator : public FairTask {
public:
  explicit CbmBinnedTrackerConfigurator(bool useAllDetectors);
  CbmBinnedTrackerConfigurator(const CbmBinnedTrackerConfigurator&) = delete;
  CbmBinnedTrackerConfigurator& operator=(const CbmBinnedTrackerConfigurator&) = delete;
  ~CbmBinnedTrackerConfigurator();

  InitStatus Init();         // Overridden from FairTask
  void Exec(Option_t* opt);  // Overridden from FairTask
  void Finish();             // Overridden from FairTask
  void SetParContainers();
  void SetUse(Int_t m, bool v) { fUseModules[m] = v; }

  void SetPrimaryParticle(EPrimaryParticleId v)
  {
    fPrimaryParticleIds.clear();
    fPrimaryParticleIds.push_back(v);
  }

  void AddPrimaryParticle(EPrimaryParticleId v) { fPrimaryParticleIds.push_back(v); }

private:
  bool fUseAllDetectors;
  bool fUseModules[ToIntegralType(ECbmModuleId::kLastModule)];
  CbmBinnedTracker* fTracker;
  CbmBinnedSettings* fSettings;
  std::list<EPrimaryParticleId> fPrimaryParticleIds;

  TClonesArray* fMCTracks;
  TClonesArray* fStsPoints;
  TClonesArray* fMuchPoints;
  TClonesArray* fTrdPoints;
  TClonesArray* fTofPoints;
  ClassDef(CbmBinnedTrackerConfigurator, 1)
};

#endif /* CBMBINNEDTRACKERCONFIGURATOR_H */
