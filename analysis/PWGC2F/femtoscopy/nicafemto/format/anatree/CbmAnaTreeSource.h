/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmAnaTreeSource.h
 *
 *  Created on: 15 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMANATREESOURCE_H_
#define CBMANATREESOURCE_H_
/**
 *class  only for reading pure anatree files
 */

#include <FairSource.h>

#include <TString.h>


class CbmAnaTreeMcSourceContainer;
class CbmAnaTreeRecoSourceContainer;
class TChain;

class CbmAnaTreeSource : public FairSource {
  Int_t fNFiles;
  TString fTreeName;
  TChain* fChain;
  TString* fFileName;  //[fNFiles]
  CbmAnaTreeRecoSourceContainer* fContainerReco;
  CbmAnaTreeMcSourceContainer* fContainerSim;

protected:
  void LoadConf(TString name);

public:
  /**
	 * defaut constructor should not be used
	 */
  CbmAnaTreeSource();
  /**
	 * main constructor
	 * @param inFile unigen file
	 * @param treename name of the tree with data
	 */
  CbmAnaTreeSource(TString inFile, TString treeName = "rTree");
  /**
	 * copy constructor
	 * @param source
	 */
  CbmAnaTreeSource(const CbmAnaTreeSource& source) = delete;
  CbmAnaTreeSource& operator=(const CbmAnaTreeSource&) = delete;
  virtual ~CbmAnaTreeSource();
  virtual Bool_t Init();
  virtual Int_t ReadEvent(UInt_t = 0);
  virtual void AddFile(TString file);
  virtual void Close();
  virtual void Reset() {};
  virtual Bool_t ActivateObject(TObject**, const char*) { return kFALSE; }
  virtual Source_Type GetSourceType() { return kFILE; };
  virtual void SetParUnpackers() {};
  virtual Bool_t SpecifyRunId() { return kTRUE; };
  virtual Bool_t InitUnpackers() { return kTRUE; };
  virtual Bool_t ReInitUnpackers() { return kTRUE; };
  virtual Int_t CheckMaxEventNo(Int_t = 0);
  virtual void ReadBranchEvent(const char* /*BrName*/) {};
  virtual void ReadBranchEvent(const char* /*BrName*/, Int_t /*Event*/) {};
  virtual void FillEventHeader(FairEventHeader* /*feh*/) {};
  CbmAnaTreeRecoSourceContainer* GetRecoContainer() const { return fContainerReco; };
  CbmAnaTreeMcSourceContainer* GetSimContainer() const { return fContainerSim; }
  void SetRunId(Int_t runId) { fRunId = runId; }
  ClassDef(CbmAnaTreeSource, 1)
};

#endif /* CBMANATREESOURCE_H_ */
