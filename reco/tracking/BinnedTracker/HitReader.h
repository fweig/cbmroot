/** @file HitReader.h
  * @copyright Copyright (C) 2017 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef HITREADER_H
#define HITREADER_H

#include "CbmPixelHit.h"

#include "TClonesArray.h"

#include <map>
#include <string>
#include <vector>

#include "Settings.h"

class CbmBinnedStation;

class CbmBinnedHitReader {
public:
  static CbmBinnedHitReader* Instance();
  static CbmBinnedHitReader* Instance(const char* name);
  static void AddReader(const char* name);
  static void SetSettings(CbmBinnedSettings* v) { fSettings = v; }

public:
  CbmBinnedHitReader();
  CbmBinnedHitReader(const CbmBinnedHitReader&) = delete;
  CbmBinnedHitReader& operator=(const CbmBinnedHitReader&) = delete;
  virtual ~CbmBinnedHitReader();
  virtual void AddStation(CbmBinnedStation* station) { fStations.push_back(station); }
  virtual void Read() = 0;
  virtual void Handle() {}  // Called optionally and is not implemented by the most of readers.
  virtual void Finish() {}  // Called optionally and is not implemented by the most of readers.

protected:
  std::vector<CbmBinnedStation*> fStations;
  TClonesArray* fHitArray;

protected:
  static std::map<std::string, CbmBinnedHitReader*> fReaders;
  static CbmBinnedSettings* fSettings;
};

#endif /* HITREADER_H */
