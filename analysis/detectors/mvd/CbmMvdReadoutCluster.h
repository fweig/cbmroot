/* Copyright (C) 2017 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----              CbmMvdReadoutCluster  header file                -----
// -----              Created 17/10/16  by P. Sitzmann                 -----
// -------------------------------------------------------------------------


/**  CbmMvdReadoutSimple.h
 *@author P.Sitzmann <p.sitzmann@gsi.de>
 *
 *  Readout simulations for the mvd
 *
 **/

#ifndef CBMMVDREADOUTCLUSTER_H
#define CBMMVDREADOUTCLUSTER_H 1

#include <FairTask.h>  // for FairTask, InitStatus

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Bool_t, Option_t, kTRUE

class TBuffer;
class TClass;
class TClonesArray;
class TFile;
class TH1F;
class TMemberInspector;

class CbmMvdReadoutCluster : public FairTask {
public:
  CbmMvdReadoutCluster();
  CbmMvdReadoutCluster(const char* name, Int_t iVerbose = 0);
  CbmMvdReadoutCluster(const CbmMvdReadoutCluster&) = delete;
  CbmMvdReadoutCluster& operator=(const CbmMvdReadoutCluster&) = delete;

  ~CbmMvdReadoutCluster();

  InitStatus Init();

  void Exec(Option_t* opt);

  void ShowHistograms() { fshow = kTRUE; };

  void SetHistogramFile(TFile* file) { foutFile = file; };

  void Finish();

private:
  TFile* foutFile;

  Bool_t fshow;

  TH1F* fWordsPerRegion[350];
  TH1F* fWordsPerSuperRegion[350];

  TClonesArray* fMvdCluster;

  Int_t fEventNumber;
  const Int_t fPixelsPerRegion      = 16;
  const Int_t fPixelsPerSuperRegion = 64;

  void DrawHistograms();
  void WriteHistograms();
  void SetupHistograms();

  ClassDef(CbmMvdReadoutCluster, 1);
};
#endif
