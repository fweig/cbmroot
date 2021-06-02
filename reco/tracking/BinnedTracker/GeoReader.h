/** @file GeoReader.h
  * @copyright Copyright (C) 2017-2019 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef GEOREADER_H
#define GEOREADER_H

#include "FairRootManager.h"

#include "TGeoManager.h"
#include "Tracker.h"

#include <list>
#include <map>
#include <string>

#include "HitReader.h"

class CbmBinnedTracker;

class CbmBinnedGeoReader {
public:
  static CbmBinnedGeoReader* Instance();

public:
  CbmBinnedGeoReader(FairRootManager*, CbmBinnedTracker* tracker);
  CbmBinnedGeoReader(const CbmBinnedGeoReader&) = delete;
  CbmBinnedGeoReader& operator=(const CbmBinnedGeoReader&) = delete;
  void FindGeoChild(TGeoNode* node, const char* name, std::list<TGeoNode*>& results);
  void Read();

private:
  void ReadDetector(const char* name);
  void SearchStation(ECbmModuleId stationType, TGeoNode* node, CbmBinnedHitReader* hitReader,
                     std::list<const char*>::const_iterator stationPath,
                     std::list<const char*>::const_iterator stationPathEnd, const std::list<const char*>& geoPath,
                     bool is4d = false);
  void HandleStation(TGeoNode* node, std::list<const char*>::const_iterator geoPath,
                     std::list<const char*>::const_iterator geoPathEnd, Double_t& left, Double_t& right, Double_t& top,
                     Double_t& bottom, Double_t& front, Double_t& back);
  void HandleActive(TGeoNode* node, Double_t& left, Double_t& right, Double_t& top, Double_t& bottom, Double_t& front,
                    Double_t& back);
  void ReadSts();
  void ReadRich();
  void ReadMuch();
  void ReadTrd();
  void ReadTof();
  void ReadTarget();
  void FindNode(std::list<const char*> nodePath, std::list<TGeoNode*>& results, TGeoNode* rootNode = 0);

private:
  //FairRootManager* fIoman; (VF) unused
  TGeoNavigator* fNavigator;
  std::map<std::string, void (CbmBinnedGeoReader::*)()> fDetectorReaders;
  CbmBinnedTracker* fTracker;
  int fLastStationNumber;
};

#endif /* GEOREADER_H */
