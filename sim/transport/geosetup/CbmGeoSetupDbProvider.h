/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Lavrik, Florian Uhlig [committer] */

/** @file CbmGeoSetupDbProvider.h
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#ifndef CBMSETUPDBPROVIDER_H
#define CBMSETUPDBPROVIDER_H 1

#include "CbmDefs.h"
#include "CbmGeoSetupProvider.h"

/** @class CbmGeoSetupDbProvider
 ** @brief Setup provider with database functionality
 **
 ** This class implements the CbmGeoSetupProvider interface to fetch
 ** available setups, fields and media definitions from DB.
 ** Database functionality is described in http://lt-jds.jinr.ru/record/72420
 ** This class works with a local SQLite replica of the real geometry database
 ** hosted elsewhere. See CMakeLists.txt for replica loading configuration.
 **/
class CbmGeoSetupDbProvider : public CbmGeoSetupProvider {
public:
  virtual std::vector<std::string> GetSetupTags();
  virtual std::vector<std::string> GetFieldTags();
  virtual std::vector<std::string> GetMediaTags();

  virtual CbmGeoSetup GetSetupByTag(std::string setupTag, std::string revision);
  virtual CbmGeoSetupModule GetModuleByTag(ECbmModuleId moduleId, std::string tag);
  virtual CbmGeoSetupField GetFieldByTag(std::string tag);
  virtual CbmGeoSetupMedia GetMediaByTag(std::string tag);

  virtual void LoadSetup(std::string setupTag, std::string revision = "");

private:
  ClassDef(CbmGeoSetupDbProvider, 1);
};

#endif /* CBMSETUPDBPROVIDER_H */
