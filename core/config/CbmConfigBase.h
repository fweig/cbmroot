/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Oleg Golosov [committer] */

#pragma once

#include "CbmDefs.h"

#include <FairLogger.h>

#include <TSystem.h>

// Add the define statement teporarily to silence the compiler
// warnings till the problem is fixed in boost
#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1
#include <boost/property_tree/json_parser.hpp>

#include <cassert>
#include <iostream>
#include <map>
#include <regex>
#include <set>

namespace pt = boost::property_tree;

template<class Config_t, class Obj_t>
class CbmConfigBase {
private:
public:
  using TagSet_t = std::set<std::string>;

  virtual ~CbmConfigBase() = default;

  static bool Load(Obj_t& obj, const std::string& path)
  {
    pt::ptree tree;
    LoadFromFile(path, tree);
    if (!Load(obj, tree)) return false;
    return true;
  }

  static bool Load(Obj_t& obj, const pt::ptree& tree)
  {
    SetLogLevel(tree);
    auto moduleTree {tree.get_child_optional(Config_t::GetModuleTag())};
    if (!moduleTree) {
      LOG(error) << "CbmConfig: module tag not found:" << Config_t::GetModuleTag();
      return false;
    }
    if (!Validate(moduleTree.get())) return false;
    if (!Config_t::LoadImpl(obj, moduleTree.get())) return false;
    return true;
  }

  static void LoadFromFile(const std::string& path, pt::ptree& tree)
  {
    std::string absPath = path;
    if (absPath.at(0) != '/') absPath = gSystem->GetWorkingDirectory() + "/" + path;
    LOG(info) << "CbmConfig: loading config from file: " << absPath << std::endl;
    pt::read_json(path, tree);
  }

  static bool Validate(const pt::ptree& tree)
  {
    const auto validationSet {Config_t::GetValidationTags()};
    TagSet_t treeSet;
    ParseTree(tree, "", treeSet);

    std::vector<std::string> diff;
    std::set_difference(treeSet.begin(), treeSet.end(), validationSet.begin(), validationSet.end(),
                        std::inserter(diff, diff.begin()));

    if (!diff.empty()) {
      LOG(error) << "CbmConfig: Invalid tags: ";
      for (auto s : diff)
        std::cout << s << ", ";
      std::cout << std::endl;
      PrintAvailableTags();
      return false;
    }
    return true;
  }

  static void ParseTree(const pt::ptree& pt, std::string key, TagSet_t& treeSet)
  {
    std::string nkey;

    if (!key.empty()) {
      nkey = key;
      if (nkey.back() != '.') nkey += ".";
      LOG(debug) << "CbmConfig: key: " << key << "\tnkey: " << nkey;
    }

    if (pt.empty() /* && !pt.data().empty()*/) {
      if (key.back() == '.') key.pop_back();
      LOG(debug) << "CbmConfig: Insert: " << key;
      if (key.find("#") < key.size())  //used for comments
        return;
      else
        treeSet.insert(key);
    }

    for (auto node : pt) {
      LOG(debug) << "CbmConfig: Try: " << nkey + node.first;
      ParseTree(node.second, nkey + node.first, treeSet);
    }
  }

  static void PrintAvailableTags()
  {
    auto tags = Config_t::GetValidationTags();
    std::cout << "\nAvailable config tags:\n";
    for (auto& tag : tags)
      std::cout << tag << std::endl;
  }

  static ECbmModuleId stringToECbmModuleId(std::string s)
  {
    std::map<std::string, ECbmModuleId> stringToModuleId = {
      {"cave", ECbmModuleId::kCave},     {"magnet", ECbmModuleId::kMagnet},    {"pipe", ECbmModuleId::kPipe},
      {"target", ECbmModuleId::kTarget}, {"mvd", ECbmModuleId::kMvd},          {"sts", ECbmModuleId::kSts},
      {"rich", ECbmModuleId::kRich},     {"much", ECbmModuleId::kMuch},        {"trd", ECbmModuleId::kTrd},
      {"tof", ECbmModuleId::kTof},       {"psd", ECbmModuleId::kPsd},          {"hodo", ECbmModuleId::kHodo},
      {"shield", ECbmModuleId::kShield}, {"platform", ECbmModuleId::kPlatform}};
    if (stringToModuleId.find(s) == stringToModuleId.end()) {
      LOG(error) << "CbmConfig: detector subsystem not recognized: " << s;
      std::cout << "list of available detector subsystems:\n";
      for (auto& p : stringToModuleId)
        std::cout << p.first << std::endl;
      assert(0);
      return ECbmModuleId::kNotExist;
    }
    else
      return stringToModuleId.at(s);
  }

  static std::string GetStringValue(boost::optional<std::string> opt) { return ParseString(opt.get()); }

  static std::string GetStringValue(pt::ptree tree, std::string key, std::string fallback)
  {
    return ParseString(tree.get<std::string>(key, fallback));
  }

  static std::string ParseString(std::string s)
  {
    std::regex rgx("\\$\\{?\\w+\\}?");
    std::smatch match;
    while (s.find("$") < s.size()) {
      std::regex_search(s, match, rgx);
      std::string varString = match[0];
      std::string varName   = std::regex_replace(varString, std::regex("\\$|\\{|\\}"), "");
      const char* varValue  = gSystem->Getenv(varName.c_str());
      if (!varValue) varValue = "";
      s.replace(s.find(varString), varString.size(), varValue);
    }
    return s;
  }

  static void SetLogLevel(const pt::ptree& moduleTree)
  {
    auto logScreenLevel = moduleTree.get_optional<std::string>("logScreenLevel");
    if (logScreenLevel) FairLogger::GetLogger()->SetLogScreenLevel(logScreenLevel.get().c_str());
    auto logVerbosityLevel = moduleTree.get_optional<std::string>("logVerbosityLevel");
    if (logVerbosityLevel) FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosityLevel.get().c_str());
  }
};
