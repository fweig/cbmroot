/** @file CbmPlatform.h
  * @copyright Copyright (C) 2013-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator], David Emschermann **/

#ifndef PLATFORM_H
#define PLATFORM_H

#include "FairModule.h"

#include <string>

class TGeoCombiTrans;

class CbmPlatform : public FairModule {
public:
  CbmPlatform();
  CbmPlatform(const char* name, const char* Title = "CBM Platform");
  CbmPlatform(const CbmPlatform&) = delete;
  CbmPlatform& operator=(const CbmPlatform&) = delete;
  virtual ~CbmPlatform();

  virtual void ConstructGeometry();

  virtual void ConstructAsciiGeometry();

private:
  TGeoCombiTrans* fCombiTrans;  //! Transformation matrix for geometry positioning
  std::string fVolumeName;

  ClassDef(CbmPlatform, 1)  //CBMPLATFORM
};

#endif  //PLATFORM_H
