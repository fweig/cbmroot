/** @file CbmGeoSetupRepoProvider.cxx
  * @copyright Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Evgeny Lavrik, Florian Uhlig [committer] **/

/** @file CbmGeoSetupRepoProvider.cxx
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#include "geosetup/CbmGeoSetupRepoProvider.h"

#include "CbmDefs.h"

#include <Logger.h>

#include "TSystem.h"
#include "TSystemDirectory.h"

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <regex>

ClassImp(CbmGeoSetupRepoProvider);

namespace
{  //anonymous namespace with helpers

  /*
  /// helper structure to represent the svn file info
  struct RepoInfo {
    std::string author;
    std::string date;
    std::string revision;
  };

  /// get svn file info
  RepoInfo GetRepoInfo(std::string fileName) {
    std::string svnInfo = gSystem->GetFromPipe(("svn info " + fileName).c_str()).Data();
    std::smatch match;
    if (std::regex_search(svnInfo, match, std::regex(R"(Last Changed Author: (.*)\nLast Changed Rev: (.*)\nLast Changed Date: (.*))")))
      return {match[1], match[2], match[3]};

    return {"", "", ""};
  }
*/

  /// helper structure to represent the technical information about the module
  struct DetInfo {
    std::string tag;
    std::string dir;
    std::string system;
    std::string name;
  };

  /// lookup map for various module properties
  std::map<ECbmModuleId, DetInfo> detectorMap {
    {ECbmModuleId::kCave, {"caveGeoTag", "cave", "cave", "CAVE"}},
    {ECbmModuleId::kMagnet, {"magnetGeoTag", "magnet", "magnet", "MAGNET"}},
    {ECbmModuleId::kPipe, {"pipeGeoTag", "pipe", "pipe", "PIPE"}},
    {ECbmModuleId::kTarget, {"targetGeoTag", "target", "target", "TARGET"}},

    {ECbmModuleId::kMvd, {"mvdGeoTag", "mvd", "mvd", "MVD"}},
    {ECbmModuleId::kSts, {"stsGeoTag", "sts", "sts", "STS"}},
    {ECbmModuleId::kRich, {"richGeoTag", "rich", "rich", "RICH"}},
    {ECbmModuleId::kMuch, {"muchGeoTag", "much", "much", "MUCH"}},
    {ECbmModuleId::kTrd, {"trdGeoTag", "trd", "trd", "TRD"}},
    {ECbmModuleId::kTof, {"tofGeoTag", "tof", "tof", "TOF"}},
    //    { ECbmModuleId::kEcal, {"ecalGeoTag", "ecal", "ecal", "ECAL"} },
    {ECbmModuleId::kPsd, {"psdGeoTag", "psd", "psd", "PSD"}},
    {ECbmModuleId::kHodo, {"hodoGeoTag", "sts", "sts", "HODO"}},

    {ECbmModuleId::kShield, {"shieldGeoTag", "much", "shield", "SHIELD"}},
    {ECbmModuleId::kPlatform, {"platGeoTag", "passive", "platform", "PLATFORM"}},
  };

  /// Get files in the directory geven the search pattern
  std::vector<std::string> ListDirectory(std::string path, std::string searchPattern)
  {
    TList* fileList = TSystemDirectory("", path.c_str()).GetListOfFiles();

    std::regex setupRegex(searchPattern);
    std::smatch match;
    std::vector<std::string> result;
    for (const auto&& file : *fileList) {
      std::string fileName = file->GetName();

      if (std::regex_search(fileName, match, setupRegex)) result.push_back(match[1]);
    }

    delete fileList;

    return result;
  }
}  // end anonymous namespace

std::vector<std::string> CbmGeoSetupRepoProvider::GetSetupTags()
{
  std::string path = gSystem->Getenv("VMCWORKDIR");
  path += "/geometry/setup";
  return ListDirectory(path, R"(setup_(.*).C)");
};

std::vector<std::string> CbmGeoSetupRepoProvider::GetFieldTags()
{
  std::string path = gSystem->Getenv("VMCWORKDIR");
  path += "/input";
  return ListDirectory(path, R"(field_(.*).root)");
};

std::vector<std::string> CbmGeoSetupRepoProvider::GetMediaTags()
{
  std::string path = gSystem->Getenv("VMCWORKDIR");
  path += "/geometry/media";
  return ListDirectory(path, R"(media_(.*).geo)");
}

/// Gets setup from local repository by tag. A file like setup_sis100_electron.C
/// will be parsed for its content with regular expressions and information about
/// modules, media and field will be extracted.
/// revision is ignored
CbmGeoSetup CbmGeoSetupRepoProvider::GetSetupByTag(std::string setupTag, std::string revision)
{
  if (fSetup.GetModuleMap().size()) {
    LOG(warn) << "-W- LoadSetup " << setupTag << ": overwriting existing setup " << fSetup.GetTag();
  }

  LOG(info) << "Loading CbmGeoSetup from svn repository.\nSetup tag: " << setupTag
            << " Revision: " << (revision.empty() ? "latest" : revision);

  std::string base          = gSystem->Getenv("VMCWORKDIR");
  std::string geoDir        = base + "/geometry/";
  std::string fieldDir      = base + "/input/";
  std::string setupFilePath = base;
  setupFilePath += "/geometry/setup/setup_" + setupTag + ".C";
  std::ifstream setupFile(setupFilePath);
  std::string fileContents((std::istreambuf_iterator<char>(setupFile)), std::istreambuf_iterator<char>());

  // remove commented out-lines
  std::regex commentRegex("/[/]+.*");
  std::string replacementString {""};
  fileContents = std::regex_replace(fileContents, commentRegex, replacementString);

  // setup name
  std::smatch match;
  std::regex_search(fileContents, match, std::regex(R"(.*setup->SetTitle\(\"(\w+)\"\);)"));
  std::string setupName = match[1];

  // field tag
  std::regex_search(fileContents, match, std::regex(R"(fieldTag\s+=\s+\"(\w+)\";)"));
  std::string fieldTag      = match[1];
  std::string fieldFilePath = fieldDir + "field_" + fieldTag + ".root";

  // field origin x, y, z; field scale
  Double_t fieldX = 0., fieldY = 0., fieldZ = 40., fieldScale = 1.;

  if (std::regex_search(fileContents, match, std::regex(R"(fieldX\s+=\s+([-+]?(\d+)?(\.\d+)?(\.)?);)")))
    fieldX = std::stod(match[1]);
  if (std::regex_search(fileContents, match, std::regex(R"(fieldY\s+=\s+([-+]?(\d+)?(\.\d+)?(\.)?);)")))
    fieldY = std::stod(match[1]);
  if (std::regex_search(fileContents, match, std::regex(R"(fieldZ\s+=\s+([-+]?(\d+)?(\.\d+)?(\.)?);)")))
    fieldZ = std::stod(match[1]);
  if (std::regex_search(fileContents, match, std::regex(R"(fieldScale\s+=\s+([-+]?(\d+)?(\.\d+)?(\.)?);)")))
    fieldScale = std::stod(match[1]);

  // media tag, if present
  std::string mediaTag;
  std::string mediaFilePath;
  if (std::regex_search(fileContents, match, std::regex(R"(mediaTag\s+=\s+\"(\w+)\";)"))) {
    mediaTag      = match[1];
    mediaFilePath = geoDir + "media/media_" + mediaTag + ".geo";
  }

  // detector tags
  std::map<ECbmModuleId, CbmGeoSetupModule> moduleMap;
  for (auto detector : detectorMap) {
    //    std::regex tagRegex(R"(.*)" + detector.second.tag + R"(\s+=\s+\"([a-zA-Z_0-9:]+)\";)");
    std::regex tagRegex(R"(.*)" + detector.second.tag + R"(\s+=\s+\"([\w:]+)\";)");
    std::regex setModuleRegex(R"(.*SetModule\(.*)" + detector.second.tag + R"(\);)");
    std::string tag;
    bool added = false;
    if (std::regex_search(fileContents, match, tagRegex)) { tag = match[1]; }
    if (std::regex_search(fileContents, match, setModuleRegex)) { added = true; }

    if (tag.size() && added) {
      ECbmModuleId moduleId = detector.first;
      moduleMap[moduleId]   = GetModuleByTag(moduleId, tag);
    }
  }

  // default cave, if none was found
  if (moduleMap.find(ECbmModuleId::kCave) == moduleMap.end()) {
    moduleMap[ECbmModuleId::kCave] = GetDefaultCaveModule();
  }

  // field creation and configuration
  CbmGeoSetupField field = GetFieldByTag(fieldTag);
  field.GetMatrix().SetTranslation(fieldX, fieldY, fieldZ);
  field.SetScale(fieldScale);

  // media creation and configuration
  CbmGeoSetupMedia media = GetMediaByTag(mediaTag);

  // actual setup creation and configuration
  //  RepoInfo setupInfo = GetRepoInfo(setupFilePath);
  CbmGeoSetup setup;
  setup.SetName(setupName);
  setup.SetTag(setupTag);
  setup.SetModuleMap(moduleMap);
  setup.SetField(field);
  setup.SetMedia(media);
  //  setup.SetAuthor(setupInfo.author);
  //  setup.SetRevision(setupInfo.revision);
  //  setup.SetDate(setupInfo.date);

  return setup;
}

CbmGeoSetupModule CbmGeoSetupRepoProvider::GetModuleByTag(ECbmModuleId moduleId, std::string tag)
{
  std::string base   = gSystem->Getenv("VMCWORKDIR");
  std::string geoDir = base + "/geometry/";

  auto detector = detectorMap[moduleId];
  //  RepoInfo info;
  std::string full_file_path;
  // split the input string at the character ";" which divides the string
  // into different geometry files
  std::vector<std::string> _geom;
  boost::split(_geom, tag, [](char c) { return c == ':'; });
  for (auto& string : _geom) {

    std::string geoFilePath =
      geoDir + (detector.dir.size() ? detector.dir + "/" : detector.dir) + detector.system + "_" + string + ".geo.root";

    if (gSystem->AccessPathName(geoFilePath.c_str()) == kTRUE) {  // doesn't exist
      geoFilePath.erase(geoFilePath.size() - 5);
      if (gSystem->AccessPathName(geoFilePath.c_str()) == kTRUE) {
        LOG(error) << "Geometry file not found for " << detector.system;
      }
    }

    //    info = GetRepoInfo(geoFilePath);

    // strip base path
    if (geoFilePath.find(geoDir) != std::string::npos) geoFilePath.replace(0, geoDir.size(), "");

    full_file_path += geoFilePath;
    full_file_path += ":";
  }
  full_file_path.pop_back();  // Remove the last ;

  CbmGeoSetupModule module;
  module.SetName(detector.name);
  module.SetFilePath(full_file_path);
  module.SetTag(tag);
  module.SetModuleId(moduleId);
  //  module.SetAuthor(info.author);
  //  module.SetRevision(info.revision);
  //  module.SetDate(info.date);
  module.SetActive(kTRUE);

  return module;
}

CbmGeoSetupField CbmGeoSetupRepoProvider::GetFieldByTag(std::string tag)
{
  std::string base     = std::string(gSystem->Getenv("VMCWORKDIR")) + "/";
  std::string fieldDir = base + "input/";

  std::string fieldFilePath = fieldDir + "field_" + tag + ".root";

  if (gSystem->AccessPathName(fieldFilePath.c_str()) == kTRUE) {  // doesn't exist
    LOG(error) << "Field file not found for tag " << tag;
  }

  // strip base path
  if (fieldFilePath.find(base) != std::string::npos) fieldFilePath.replace(0, base.size(), "");

  CbmGeoSetupField field = fSetup.GetField();
  field.SetTag(tag);
  field.SetFilePath(fieldFilePath);
  // field svn author, revision and date are not availabe

  return field;
}

CbmGeoSetupMedia CbmGeoSetupRepoProvider::GetMediaByTag(std::string tag)
{
  std::string base     = std::string(gSystem->Getenv("VMCWORKDIR")) + "/";
  std::string geoDir   = base + "geometry/";
  std::string mediaDir = base + "geometry/media/";

  std::string mediaFilePath = mediaDir + "media_" + tag + ".geo";

  if (gSystem->AccessPathName(mediaFilePath.c_str()) == kTRUE) {  // doesn't exist
    LOG(warn) << "Media file not found for tag " << (tag.size() ? "(empty)" : tag) << " using default media.geo";

    mediaFilePath = geoDir + "media.geo";
  }

  //  RepoInfo info = GetRepoInfo(mediaFilePath);

  // strip base path
  if (mediaFilePath.find(geoDir) != std::string::npos) mediaFilePath.replace(0, geoDir.size(), "");

  CbmGeoSetupMedia media = fSetup.GetMedia();
  media.SetTag(tag);
  media.SetFilePath(mediaFilePath);
  //  media.SetAuthor(info.author);
  //  media.SetRevision(info.revision);
  //  media.SetDate(info.date);

  return media;
}

/// Loads setup from local repository by tag.
/// See GetSetupByTag for actual implementation.
/// revision is ignored
void CbmGeoSetupRepoProvider::LoadSetup(std::string setupTag, std::string revision)
{
  fSetup = GetSetupByTag(setupTag, revision);
}
