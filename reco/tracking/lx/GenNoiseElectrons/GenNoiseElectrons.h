/* Copyright (C) 2016-2017 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#ifndef LX_GEN_NOISE_ELECTRONS
#define LX_GEN_NOISE_ELECTRONS

#include "FairTask.h"

class TClonesArray;

class LxGenNoiseElectrons : public FairTask {
public:
  LxGenNoiseElectrons();
  LxGenNoiseElectrons(const LxGenNoiseElectrons& orig) = delete;
  LxGenNoiseElectrons& operator=(const LxGenNoiseElectrons& orig) = delete;
  ~LxGenNoiseElectrons();
  InitStatus Init();         // Overridden from FairTask
  void Exec(Option_t* opt);  // Overridden from FairTask
  void Finish();             // Overridden from FairTask

  void SetNofNoiseE(Int_t v) { fNofNoiseE = v; }
  void SetNofStations(Int_t v) { fNofStations = v; }

private:
  Int_t fNofNoiseE;
  Int_t fNofStations;
  TClonesArray* fMCTracks;
  TClonesArray* fMuchPoints;
  TClonesArray* fTrdPoints;
  TClonesArray* fOutMCTracks;
  TClonesArray* fOutMuchPoints;
  TClonesArray* fOutTrdPoints;
  ClassDef(LxGenNoiseElectrons, 1)
};

#endif  //LX_GEN_NOISE_ELECTRONS
