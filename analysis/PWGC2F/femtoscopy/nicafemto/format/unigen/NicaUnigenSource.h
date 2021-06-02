/** @file NicaUnigenSource.h
  * @copyright Copyright (C) 2017-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer], Daniel Wielanek [committer] **/

/*
 * NicaUnigenSource.h
 *
 *  Created on: 27 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAUNIGENSOURCE_H_
#define NICAUNIGENSOURCE_H_
/**
 *class  only for reading pure unigen files
 */


#include <FairFileSource.h>

#include <TChain.h>

#include "UEvent.h"

class NicaUnigenSource : public FairSource {
  TChain* fUnigenChain;
  std::vector<TString> fFileName;
  UEvent* fEvent;

public:
  /**
	 * defaut constructor should not be used
	 */
  NicaUnigenSource();
  /**
	 * main constructor
	 * @param inFile unigen file
	 */
  NicaUnigenSource(TString inFile);
  /**
	 * copy constructor
	 * @param source
	 */
  NicaUnigenSource(const NicaUnigenSource& source) = delete;
  void AddFile(TString name) { fFileName.push_back(name); };
  NicaUnigenSource& operator=(const NicaUnigenSource&) = delete;
  virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
  virtual ~NicaUnigenSource();
  virtual Bool_t Init();
  virtual Int_t ReadEvent(UInt_t = 0);
  virtual void Close();
  virtual void Reset() {};
  virtual Bool_t ActivateObject(TObject**, const char*) { return kFALSE; }
  virtual Source_Type GetSourceType() { return kFILE; };
  virtual void SetParUnpackers() {};
  virtual Bool_t InitUnpackers() { return kTRUE; };
  virtual Bool_t ReInitUnpackers() { return kTRUE; };
  virtual Int_t CheckMaxEventNo(Int_t = 0);
  virtual void ReadBranchEvent(const char* /*BrName*/) {};
  virtual void ReadBranchEvent(const char* /*BrName*/, Int_t /*Event*/) {};
  virtual void FillEventHeader(FairEventHeader* /*feh*/) {};
  void SetRunId(Int_t runId) { fRunId = runId; }
  Int_t GetRunId() const { return fRunId; }
  ClassDef(NicaUnigenSource, 1)
};

#endif /* NICAUNIGENSOURCE_H_ */
