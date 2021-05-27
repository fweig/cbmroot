/** @file CbmGeoSetupDbProvider.cxx
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#include "geosetup/CbmGeoSetupDbProvider.h"

#include "CbmDefs.h"

#include <Logger.h>

#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TSQLiteServer.h"
#include "TSystem.h"

#include <iostream>

ClassImp(CbmGeoSetupDbProvider);

namespace
{  //anonymous namespace with helpers
  std::map<Int_t, ECbmModuleId> dbToCbmModuleIdMap = {
    {0, ECbmModuleId::kCave},       {1, ECbmModuleId::kMagnet},    {2, ECbmModuleId::kPipe},
    {4, ECbmModuleId::kMvd},        {8, ECbmModuleId::kSts},       {16, ECbmModuleId::kRich},
    {32, ECbmModuleId::kTrd},       {64, ECbmModuleId::kTof},      {128, ECbmModuleId::kPsd},
    {256, ECbmModuleId::kPlatform}, {512, ECbmModuleId::kMuch},    {1024, ECbmModuleId::kHodo},
    {2048, ECbmModuleId::kTarget},  {4096, ECbmModuleId::kShield},
  };

  /// Default path to field directory
  std::string FieldDir() { return "input/"; }

  /// Default path to SQLite DB and downloaded geometry files
  std::string DbGeoDir() { return "db/"; }

  /// Path to SQLite DB
  std::string LocalDbPath()
  {
    std::string path = std::string(gSystem->Getenv("VMCWORKDIR")) + "/geometry/db/local.db";
    if (gSystem->AccessPathName(path.c_str()) == kTRUE)  // does not exist
      LOG(fatal) << "Geometry DB file does not exist: " << path;
    return "sqlite://" + path;
  };

  /// Load from SQLite the detector map for the setup with setupId
  std::map<Int_t, std::string> GetSetupModuleTagMap(Int_t setupId)
  {
    TSQLiteServer db(LocalDbPath().c_str());

    std::string query = std::string()
                        + "select sms.idsd, sms.idsm, sm.idf, sm.smtag, f.idm "
                          "from sms sms "
                          "inner join setupmodule sm on sms.idsm=sm.idsm "
                          "inner join file f on sm.idf=f.idf "
                          "where sms.idsd="
                        + std::to_string(setupId);
    TSQLResult* resQ = db.Query(query.c_str());
    if (resQ == 0 || resQ->GetRowCount() == 0) return {};

    std::map<Int_t, std::string> result;
    TSQLRow* row;
    while ((row = resQ->Next()) != 0) {
      result[std::atoi(row->GetField(4))] = row->GetField(3);
    }
    delete resQ;
    delete row;
    db.Close();

    return result;
  }

  /// List table column's contents
  std::vector<std::string> ListTable(std::string table, std::string column)
  {
    TSQLiteServer db(LocalDbPath().c_str());

    std::string query = std::string("select ") + column + " from " + table;
    TSQLResult* resQ  = db.Query(query.c_str());
    if (resQ == 0 || resQ->GetRowCount() == 0) return {};

    std::vector<std::string> result;
    TSQLRow* row;
    while ((row = resQ->Next()) != 0) {
      result.push_back(row->GetField(0));
    }
    delete resQ;
    delete row;
    db.Close();

    return result;
  }
}  // end anonymous namespace

std::vector<std::string> CbmGeoSetupDbProvider::GetSetupTags()
{
  TSQLiteServer db(LocalDbPath().c_str());

  std::string query = std::string("select stag,revision from setup order by revision desc");
  TSQLResult* resQ  = db.Query(query.c_str());
  if (resQ == 0 || resQ->GetRowCount() == 0) return {};

  std::vector<std::string> result;
  TSQLRow* row;
  while ((row = resQ->Next()) != 0) {
    result.push_back(std::string(row->GetField(0)) + "@" + row->GetField(1));
  }
  delete resQ;
  delete row;
  db.Close();

  return result;
}

std::vector<std::string> CbmGeoSetupDbProvider::GetFieldTags() { return ListTable("field", "tag"); };

std::vector<std::string> CbmGeoSetupDbProvider::GetMediaTags() { return ListTable("material", "matag"); };

// Get bare setup without field, media and modules loaded
CbmGeoSetup CbmGeoSetupDbProvider::GetSetupByTag(std::string setupTag, std::string revision)
{
  if (revision.empty() && setupTag.find("@") != std::string::npos) {
    auto pos = setupTag.find("@");
    revision = setupTag.substr(pos + 1, setupTag.size() - pos - 1);
    setupTag = setupTag.substr(0, pos);
  }

  LOG(info) << "Loading CbmGeoSetup from geometry database.\nSetup tag: " << setupTag
            << " Revision: " << (revision.size() ? revision : "latest");

  std::string path = LocalDbPath();
  TSQLiteServer db(LocalDbPath().c_str());
  std::string query = "select s.idsd, s.stag, s.sdate, s.desc, "
                      "s.author, s.revision, s.idfi, s.idma, "
                      "f.idfi, f.tag, ma.idma, ma.matag from setup s "
                      "INNER JOIN field f ON s.idfi=f.idfi "
                      "INNER JOIN material ma ON s.idma=ma.idma "
                      "where s.stag='"
                      + setupTag + "' " + (revision.empty() ? "" : " and s.revision=" + revision)
                      + " order by s.revision desc limit 1";

  TSQLResult* resQ = db.Query(query.c_str());
  TSQLRow* row     = resQ ? resQ->Next() : nullptr;
  if (resQ == nullptr || row == nullptr || resQ->GetRowCount() == 0)
    LOG(fatal) << "Geometry DB: setup not found for tag: " << setupTag
               << " revision: " << (revision.size() ? revision : "latest");

  if (resQ->GetRowCount() > 1)
    LOG(fatal) << "Geometry DB: found more than one setup with tag " << setupTag
               << " revision: " << (revision.size() ? revision : "latest");

  CbmGeoSetup setup;
  setup.SetId(std::atoi(row->GetField(0)));
  setup.SetName(row->GetField(1));
  setup.SetDate(row->GetField(2));
  setup.SetDescription(row->GetField(3));
  setup.SetAuthor(row->GetField(4));
  setup.SetTag(setupTag);
  setup.SetRevision(revision);

  setup.GetField().SetTag(row->GetField(9));
  setup.GetMedia().SetTag(row->GetField(11));

  delete resQ;
  delete row;
  db.Close();

  return setup;
};

CbmGeoSetupModule CbmGeoSetupDbProvider::GetModuleByTag(ECbmModuleId moduleId, std::string tag)
{
  TSQLiteServer db(LocalDbPath().c_str());

  std::string query = std::string("select sm.idsm, sm.smtag, sm.descr, sm.author, sm.smdate, sm.idf, "
                                  "sm.r11, sm.r12, sm.r13,"
                                  "sm.r21, sm.r22, sm.r23,"
                                  "sm.r31, sm.r32, sm.r33,"
                                  "sm.tx,  sm.ty,  sm.tz,"
                                  "sm.sx,  sm.sy,  sm.sz,"
                                  "sm.idf, f.idf, f.url, m.mname, m.idm "
                                  "from setupmodule sm "
                                  "inner join file f on sm.idf=f.idf "
                                  "inner join module m on f.idm=m.idm "
                                  "where sm.smtag='"
                                  + tag + "' and f.idm=" + std::to_string(static_cast<int>(moduleId)));
  TSQLResult* resQ  = db.Query(query.c_str());
  if (resQ == 0 || resQ->GetRowCount() == 0) return {};

  if (resQ->GetRowCount() > 1)
    LOG(fatal) << "Geometry DB: found more than one entry for moduleId " << moduleId << " and tag " << tag;

  TSQLRow* row = resQ->Next();

  CbmGeoSetupModule module;
  module.SetModuleId(moduleId);
  module.SetId(std::atoi(row->GetField(0)));
  module.SetTag(row->GetField(1));
  module.SetName(row->GetField(24));
  module.SetDescription(row->GetField(2));
  module.SetAuthor(row->GetField(3));
  module.SetDate(row->GetField(4));
  Double_t rot[9] = {std::atof(row->GetField(6)),  std::atof(row->GetField(7)),  std::atof(row->GetField(8)),
                     std::atof(row->GetField(9)),  std::atof(row->GetField(10)), std::atof(row->GetField(11)),
                     std::atof(row->GetField(12)), std::atof(row->GetField(13)), std::atof(row->GetField(14))};
  module.GetMatrix().SetRotation(rot);
  Double_t trans[3] = {std::atof(row->GetField(15)), std::atof(row->GetField(16)), std::atof(row->GetField(17))};
  module.GetMatrix().SetTranslation(trans);
  Double_t scale[3] = {std::atof(row->GetField(18)), std::atof(row->GetField(19)), std::atof(row->GetField(20))};
  module.GetMatrix().SetScale(scale);
  module.SetFilePath(DbGeoDir() + row->GetField(23));
  module.SetActive(kTRUE);

  delete resQ;
  delete row;
  db.Close();

  return module;
};

CbmGeoSetupField CbmGeoSetupDbProvider::GetFieldByTag(std::string tag)
{
  TSQLiteServer db(LocalDbPath().c_str());

  std::string query = std::string("select fi.idfi, fi.tag, fi.date, fi.desc, fi.author, "
                                  "fi.scale, fi.x, fi.y, fi.z, fi.url from field fi "
                                  "where fi.tag='"
                                  + tag + "'");

  TSQLResult* resQ = db.Query(query.c_str());
  if (resQ == 0 || resQ->GetRowCount() == 0) return {};

  if (resQ->GetRowCount() > 1) LOG(fatal) << "Geometry DB: found more than one field with tag " << tag;

  TSQLRow* row = resQ->Next();

  CbmGeoSetupField field;
  field.SetId(std::atoi(row->GetField(0)));
  field.SetTag(row->GetField(1));
  field.SetDate(row->GetField(2));
  field.SetDescription(row->GetField(3));
  field.SetAuthor(row->GetField(4));
  field.SetScale(std::atof(row->GetField(5)));
  field.GetMatrix().SetTranslation(std::atof(row->GetField(6)), std::atof(row->GetField(7)),
                                   std::atof(row->GetField(8)));
  std::string url = row->GetField(9);

  // if url field is empty use the conventional field file name building from tag
  if (url.empty()) { url = std::string("field_") + row->GetField(1) + ".root"; }
  field.SetFilePath(FieldDir() + url);

  delete resQ;
  delete row;
  db.Close();

  return field;
};

CbmGeoSetupMedia CbmGeoSetupDbProvider::GetMediaByTag(std::string tag)
{
  TSQLiteServer db(LocalDbPath().c_str());

  std::string query = std::string("select ma.idma, ma.matag, ma.madata, "
                                  "ma.desc, ma.author,ma.url "
                                  "from material ma "
                                  "where ma.matag='"
                                  + tag + "'");
  TSQLResult* resQ  = db.Query(query.c_str());
  if (resQ == 0 || resQ->GetRowCount() == 0) return {};

  if (resQ->GetRowCount() > 1) LOG(fatal) << "Geometry DB: found more than one material with tag " << tag;

  TSQLRow* row = resQ->Next();

  CbmGeoSetupMedia media;
  media.SetId(std::atoi(row->GetField(0)));
  media.SetTag(row->GetField(1));
  media.SetDate(row->GetField(2));
  media.SetDescription(row->GetField(3));
  media.SetAuthor(row->GetField(4));
  media.SetFilePath(DbGeoDir() + row->GetField(5));

  delete resQ;
  delete row;
  db.Close();

  return media;
};

void CbmGeoSetupDbProvider::LoadSetup(std::string setupTag, std::string revision)
{
  if (fSetup.GetModuleMap().size()) {
    LOG(warn) << "-W- LoadSetup " << setupTag << ": overwriting existing setup " << fSetup.GetTag();
  }

  CbmGeoSetup setup = GetSetupByTag(setupTag, revision);

  std::map<ECbmModuleId, CbmGeoSetupModule> moduleMap;
  std::map<Int_t, std::string> moduleTags = GetSetupModuleTagMap(setup.GetId());
  for (auto module : moduleTags) {
    for (auto moduleId : moduleMap) {
      if (static_cast<int>(moduleId.first) == module.first) {
        moduleMap.at(dbToCbmModuleIdMap[module.first]) = GetModuleByTag(moduleId.first, module.second);
        break;
      }
    }
  }

  if (moduleMap.find(ECbmModuleId::kCave) == moduleMap.end()) {
    moduleMap[ECbmModuleId::kCave] = GetDefaultCaveModule();
  }

  setup.SetModuleMap(moduleMap);
  setup.SetField(GetFieldByTag(setup.GetField().GetTag()));
  setup.SetMedia(GetMediaByTag(setup.GetMedia().GetTag()));

  fSetup = setup;
}
