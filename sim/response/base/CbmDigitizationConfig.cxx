/** @file CbmDigitizationConfig.cxx
  * @copyright Copyright (C) 2021 National Research Nuclear University MEPhI (Moscow Engineering Physics Institute), Moscow
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Oleg Golosov [orginator] **/

#include "CbmDigitizationConfig.h"

#include "CbmDigitization.h"

using namespace std;

string CbmDigitizationConfig::GetModuleTag() { return "digitization"; }

CbmDigitizationConfig::TagSet_t CbmDigitizationConfig::GetValidationTags()
{
  return {"logScreenLevel",
          "logVerbosityLevel",
          "generateRunInfo",
          "storeAllTimeSlices",
          "eventMode",
          "startTime",
          "timeSliceLength",
          "produceNoise",
          "input.id",
          "input.path",
          "input.rate",
          "input.embedToId",
          "input.treeAccessMode",
          "input.parameterSource",
          "output.overwrite",
          "output.path",
          "geometry.path",
          "geometry.sourceId",
          "geometry.deactivate",
          "geometry.deactivateAllBut"};
}

bool CbmDigitizationConfig::SetIO(CbmDigitization& obj, const pt::ptree& moduleTree)
{
  map<string, ECbmTreeAccess> stringToECbmTreeAccess = {{"regular", ECbmTreeAccess::kRegular},
                                                        {"repeat", ECbmTreeAccess::kRepeat},
                                                        {"random", ECbmTreeAccess::kRandom}};
  bool eventMode                                     = moduleTree.get<bool>("eventMode", false);
  auto inputs                                        = moduleTree.get_child("input");
  uint inputCounter                                  = 0;
  vector<string> paths;
  bool parametersSet    = false;
  string parametersPath = "";
  ECbmTreeAccess treeAccessMode;
  for (auto& input : inputs) {
    pt::ptree pt_input          = input.second;
    int id                      = pt_input.get<int>("id", inputCounter);
    string path                 = GetStringValue(pt_input, "path", "");
    auto configRate             = pt_input.get_optional<float>("rate");
    string treeAccessModeString = pt_input.get<string>("treeAccessMode", "regular");
    bool parameterSource        = pt_input.get<bool>("parameterSource", false);
    auto configEmbedToId        = pt_input.get_optional<int>("embedToId");

    if (id < 0) continue;
    if (path == "") {
      LOG(error) << "CbmDigitizationConfig: no path specified for input #" << id;
      return false;
    }
    paths.push_back(path);
    string traFileName = path + ".tra.root";
    if (stringToECbmTreeAccess.find(treeAccessModeString) != stringToECbmTreeAccess.end())
      treeAccessMode = stringToECbmTreeAccess.at(treeAccessModeString);
    else {
      LOG(error) << "CbmDigitizationConfig: invalid tree access mode: " << treeAccessModeString;
      cout << "Available access modes:\n";
      for (auto& p : stringToECbmTreeAccess)
        cout << p.first << endl;
      return false;
    }

    if (configEmbedToId) {
      if (configRate) {
        LOG(error) << "CbmDigitizationConfig: input.embedToId and input.rate should not be used simultaneously!";
        return false;
      }
      else {
        int embedToId = configEmbedToId.get();
        LOG(info) << "CbmDigitizationConfig: Embedding input: " << traFileName;
        obj.EmbedInput(id, traFileName, embedToId);
      }
    }
    else {
      if (eventMode && inputCounter > 0) {
        LOG(error) << "CbmDigitizationConfig: event mixing is not possible in event-by-event mode!";
        return false;
      }
      float rate = pt_input.get<float>("rate", -1.);
      LOG(info) << "CbmDigitizationConfig: Adding input: " << traFileName;
      obj.AddInput(id, path + ".tra.root", rate, treeAccessMode);
    }
    if (parameterSource) {
      if (!parametersSet) { parametersPath = path; }
      else {
        LOG(error) << "CbmDigitizationConfig: only one parameter source is allowed!";
        return false;
      }
    }
    inputCounter++;
  }

  string outputPath = GetStringValue(moduleTree, "output.path", paths.at(0));
  bool overwrite    = moduleTree.get<bool>("output.overwrite", false);

  if (!parametersSet) parametersPath = paths.at(0);
  LOG(info) << "CbmDigitizationConfig: Parameter source:\n" << parametersPath;
  obj.SetParameterRootFile(parametersPath + ".par.root");
  LOG(info) << "CbmDigitizationConfig: Output path:\n" << outputPath;
  obj.SetOutputFile(outputPath + ".raw.root", overwrite);
  obj.SetMonitorFile((outputPath + ".moni_digi.root").c_str());

  return true;
}

bool CbmDigitizationConfig::SetDigitizationParameters(CbmDigitization& obj, const pt::ptree& moduleTree)
{

  auto produceNoise       = moduleTree.get_optional<bool>("produceNoise");
  bool eventMode          = moduleTree.get<bool>("eventMode", false);
  auto storeAllTimeSlices = moduleTree.get_optional<bool>("storeAllTimeSlices");
  auto timeSliceLength    = moduleTree.get_optional<float>("timeSliceLength");
  auto startTime          = moduleTree.get_optional<float>("startTime");

  if (eventMode) {
    if (storeAllTimeSlices || timeSliceLength || startTime) {
      LOG(error) << "CbmDigitizationConfig: time slice settings should not be used in event mode!";
      return false;
    }
  }
  obj.SetEventMode(eventMode);

  if (timeSliceLength) obj.SetTimeSliceLength(timeSliceLength.get());
  if (startTime) obj.SetStartTime(startTime.get());
  if (produceNoise) obj.SetProduceNoise(produceNoise.get());
  return true;
}

bool CbmDigitizationConfig::SetGeometry(CbmDigitization& obj, const pt::ptree& moduleTree)
{
  auto modulesToDeactivate = moduleTree.get_child_optional("geometry.deactivate");
  auto deactivateAllBut    = moduleTree.get_optional<string>("geometry.deactivateAllBut");

  if (modulesToDeactivate && deactivateAllBut) {
    LOG(error)
      << "CbmDigitizationConfig: geometry.deactivate and geometry.deactivateAllBut should not be used simultaneously!";
    return false;
  }

  if (deactivateAllBut && deactivateAllBut.get() != "")
    obj.DeactivateAllBut(stringToECbmModuleId(deactivateAllBut.get()));

  if (modulesToDeactivate)
    for (auto& module : modulesToDeactivate.get())
      if (module.first != "") obj.Deactivate(stringToECbmModuleId(module.first));

  return true;
}

bool CbmDigitizationConfig::LoadImpl(CbmDigitization& obj, const pt::ptree& moduleTree)
{
  return SetIO(obj, moduleTree) && SetDigitizationParameters(obj, moduleTree) && SetGeometry(obj, moduleTree);
}

ClassImp(CbmDigitizationConfig)
