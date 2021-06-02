/** @file CbmGeoSetupMedia.h
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Evgeny Lavrik, Florian Uhlig [committer] **/

/** @file CbmGeoSetupMedia.h
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#ifndef CBMGEOSETUPMATERIAL_H
#define CBMGEOSETUPMATERIAL_H 1

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t

#include <string>  // for string

/** @class CbmGeoSetup
 ** @brief Data transfer object to represent the CBM Detector setup material properties
 **
 ** Has no real functionality apart from getters and setters
 **/
class CbmGeoSetupMedia {
public:
  Int_t GetId() { return fId; };
  std::string GetTag() { return fTag; };
  std::string GetAuthor() { return fAuthor; };
  std::string GetDate() { return fDate; };
  std::string GetDescription() { return fDescription; };
  std::string GetFilePath() { return fFilePath; };
  std::string GetRevision() { return fRevision; };

  void SetId(Int_t value) { fId = value; };
  void SetTag(std::string value) { fTag = value; };
  void SetAuthor(std::string value) { fAuthor = value; };
  void SetDate(std::string value) { fDate = value; };
  void SetDescription(std::string value) { fDescription = value; };
  void SetFilePath(std::string value) { fFilePath = value; };
  void SetRevision(std::string value) { fRevision = value; };

private:
  Int_t fId {};
  std::string fTag {};
  std::string fAuthor {};
  std::string fDate {};
  std::string fDescription {};
  std::string fFilePath {};
  std::string fRevision {};

  ClassDefNV(CbmGeoSetupMedia, 1);
};

#endif /* CBMGEOSETUPMATERIAL_H */
