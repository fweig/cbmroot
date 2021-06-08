/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Lavrik, Florian Uhlig [committer] */

/** @file CbmGeoSetupProvider.h
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#ifndef CBMGEOSETUPPROVIDER_H
#define CBMGEOSETUPPROVIDER_H 1

#include "CbmDefs.h"
#include "CbmGeoSetup.h"
#include "CbmGeoSetupModule.h"

#include "TObject.h"

#include <vector>

/** @class CbmGeoSetupProvider
 ** @brief Abstract interface class for providing the CBM detector setup
 ** description, module list, magnetic field, material descriptions etc.
 **
 ** This class implements the CbmGeoSetupProvider interface to fetch
 ** available setups, fields and media definitions from filesystem.
 ** This class works with the svn repository and local filesystem.
 ** Revisions, dates and authors of changes are fetched directly from svn.
 ** Setup definition is parsed from setup files like setup_sis100_electron.C
 ** with regular expressions and follows the established scheme of setup loading.
 **/
class CbmGeoSetupProvider {
public:
  /** @brief Default destructor is necessary **/
  virtual ~CbmGeoSetupProvider() = default;

  /** @brief Loads the detector with a tag into setup, will invoke GetModuleByTag
     ** @param moduleId \ref ECbmModuleId
     ** @patam tag module tag to load
     ** @param active indicates if the module will be treated as active in
     ** Monte-Carlo, exact treatment depends on actual implementation of a FairModule
     **
     ** @note This method has a side effect for backward compatibility -when loading
     ** the magnet module, the field with the same tag will be auto-loaded.
     **/
  void SetModuleTag(ECbmModuleId moduleId, std::string tag, Bool_t active);

  /** @brief Removes the module from setup **/
  void RemoveModule(ECbmModuleId moduleId);

  /** @brief Loads the field with a tag and adds it to the setup
     ** @param tag field tag to load
     **/
  void SetFieldTag(std::string tag);

  /** @brief Registers the previously loaded setup with FairRoot.
     ** Replaces the registerSetup.C macro
     **/
  void RegisterSetup();

  /** @brief Resets the setup to default (empty)
     **/
  void Reset();

  /** @brief Direct access to underlying geometry setup representation.
     ** Allows for fine-tuning of parameters, for exmaple, transformation matrices.
     **/
  CbmGeoSetup& GetSetup() { return fSetup; };

  /** @brief Abstract method to get the list of setup tags **/
  virtual std::vector<std::string> GetSetupTags() = 0;
  /** @brief Abstract method to get the list of media tags **/
  virtual std::vector<std::string> GetMediaTags() = 0;
  /** @brief Abstract method to get the list of field tags **/
  virtual std::vector<std::string> GetFieldTags() = 0;

  /** @brief Abstract method for constructing the setup by id and tag **/
  virtual CbmGeoSetup GetSetupByTag(std::string setupTag, std::string revision) = 0;
  /** @brief Abstract method for constructing the module by id and tag **/
  virtual CbmGeoSetupModule GetModuleByTag(ECbmModuleId moduleId, std::string tag) = 0;
  /** @brief Abstract method for constructing the field by tag **/
  virtual CbmGeoSetupField GetFieldByTag(std::string tag) = 0;
  /** @brief Abstract method for constructing the media by tag **/
  virtual CbmGeoSetupMedia GetMediaByTag(std::string tag) = 0;

  /** @brief Abstract method to load the setup with a tag and revision version
     **/
  virtual void LoadSetup(std::string setupTag, std::string revision = "") = 0;

protected:
  /** @brief Gets defauk cave if none was provided by the other means **/
  CbmGeoSetupModule GetDefaultCaveModule();

  CbmGeoSetup fSetup {};  /// underlying geometry setup representation.

private:
  ClassDef(CbmGeoSetupProvider, 1);
};

#endif /* CBMGEOSETUPPROVIDER_H */
