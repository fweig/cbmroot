/** @file CbmGeoSetupRepoProvider.h
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Evgeny Lavrik, Florian Uhlig [committer] **/

/** @file CbmGeoSetupProvider.h
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#ifndef CBMSETUPREPOPROVIDER_H
#define CBMSETUPREPOPROVIDER_H 1

#include "CbmGeoSetupProvider.h"

/** @class CbmGeoSetupRepoProvider
 ** @brief Setup provider with local (svn) repository functionality
 **
 ** This class implements the CbmGeoSetupProvider interface to fetch
 ** available setups, fields and media definitions from filesystem.
 ** This class works with the svn repository and local filesystem.
 ** Revisions, dates and authors of changes are fetched directly from svn.
 ** Setup definition is parsed from setup files like setup_sis100_electron.C
 ** with regular expressions and follows the established scheme of setup loading.
 **/
class CbmGeoSetupRepoProvider : public CbmGeoSetupProvider {
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
  ClassDef(CbmGeoSetupRepoProvider, 1);
};

#endif /* CBMSETUPREPOPROVIDER_H */
