/** @file CbmTransportConfig.cxx
  * @copyright Copyright (C) 2021 National Research Nuclear University MEPhI (Moscow Engineering Physics Institute), Moscow
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Oleg Golosov [orginator] **/

#include "CbmTransportConfig.h"

#include "CbmBeamGenerator.h"
#include "CbmGeant4Settings.h"
#include "CbmPlutoGenerator.h"
#include "CbmTransport.h"

using namespace std;

string CbmTransportConfig::GetModuleTag() { return "transport"; }

CbmTransportConfig::TagSet_t CbmTransportConfig::GetValidationTags()
{
  return {"logScreenLevel",
          "logVerbosityLevel",
          "input.file",
          "input.generator",
          "input.plutoPdg",
          "input.beamA",
          "input.beamZ",
          "input.beamQ",
          "input.beamP",
          "input.beamStartZ",
          "output.path",
          "target.material",
          "target.thickness",
          "target.diameter",
          "target.position.x",
          "target.position.y",
          "target.position.z",
          "target.rotation.y",
          "beam.position.x",
          "beam.position.y",
          "beam.position.z",
          "beam.position.sigmaX",
          "beam.position.sigmaY",
          "beam.angle.x",
          "beam.angle.y",
          "beam.angle.sigmaX",
          "beam.angle.sigmaY",
          "geantVersion",
          "geant4vmcSettings.physicsLists",
          "geant4vmcSettings.specialProcesses",
          "geant4vmcSettings.maxNsteps",
          "geant4vmcSettings.geant4commands",
          "randomRP",
          "geometry.magneticField.position.x",
          "geometry.magneticField.position.y",
          "geometry.magneticField.position.z",
          "geometry.magneticField.scale",
          "geometry.magneticField.tag",
          "geometry.baseSetup",
          "geometry.subsystems",
          "geometry.subsystems.cave",
          "geometry.subsystems.magnet",
          "geometry.subsystems.pipe",
          "geometry.subsystems.target",
          "geometry.subsystems.mvd",
          "geometry.subsystems.sts",
          "geometry.subsystems.rich",
          "geometry.subsystems.much",
          "geometry.subsystems.trd",
          "geometry.subsystems.tof",
          "geometry.subsystems.psd",
          "geometry.subsystems.hodo",
          "geometry.subsystems.shield",
          "geometry.subsystems.platform",
          "stackFilter.storeAllPrimaries",
          "stackFilter.storeAllMothers",
          "stackFilter.storeAllDecays"};
}

CbmTransportConfig::TagSet_t CbmTransportConfig::GetAcceptedGenerators() { return {"unigen", "pluto", "beam"}; }

bool CbmTransportConfig::SetIO(CbmTransport& obj, const pt::ptree& moduleTree)
{
  auto inputs      = moduleTree.get_child("input");
  int inputCounter = 0;
  for (auto& input : inputs) {
    pt::ptree pt_input    = input.second;
    string inputGenerator = pt_input.get<string>("generator", "unigen");
    string inputFile      = GetStringValue(pt_input, "file", "");

    if (inputFile == "") { LOG(error) << "CbmTransportConfig: no input path specified for input #" << inputCounter; }

    if (inputGenerator == "unigen") obj.AddInput(inputFile.c_str(), kUnigen);
    else if (inputGenerator == "pluto") {
      auto plutoGen  = new CbmPlutoGenerator(inputFile.c_str());
      auto manualPdg = pt_input.get_optional<int>("plutoPdg");
      if (manualPdg) plutoGen->SetManualPDG(manualPdg.get());
      obj.AddInput(plutoGen);
    }
    else if (inputGenerator == "beam") {
      auto confBeamA      = pt_input.get_optional<int>("beamA");
      auto confBeamZ      = pt_input.get_optional<int>("beamZ");
      auto confBeamQ      = pt_input.get_optional<int>("beamQ");
      auto confBeamP      = pt_input.get_optional<float>("beamP");
      auto confBeamStartZ = pt_input.get_optional<float>("beamStartZ");
      auto targetZ        = moduleTree.get_optional<float>("target.position.z");
      int beamA, beamZ, beamQ;
      float beamP, beamStartZ;
      if (confBeamA && confBeamZ && confBeamP) {
        beamA = confBeamA.get();
        beamZ = confBeamZ.get();
        beamP = confBeamP.get();
        if (confBeamQ) beamQ = confBeamQ.get();
        else
          beamQ = confBeamA.get();
        if (confBeamStartZ) beamStartZ = confBeamStartZ.get();
        else if (targetZ) {
          beamStartZ = targetZ.get() - 1.;
          LOG(warning) << "CbmTransportConfig: beamStartZ=targetZ-1. cm";
        }
        else {
          LOG(error) << "CbmTransportConfig: beam start Z not set!";
          return false;
        }
        obj.AddInput(new CbmBeamGenerator(beamZ, beamA, beamQ, beamP, beamStartZ));
        LOG(info) << Form("CbmTransportConfig: add beam generator: Z = %d, A = %d, Q = %d, P = %f, startZ = %f", beamZ,
                          beamA, beamQ, beamP, beamStartZ);
      }
      else {
        LOG(error) << "CbmTransportConfig: Incomplete beam generator configuration: A, Z and P required!";
        return false;
      }
    }
    else {
      LOG(error) << "CbmTransportConfig: Unknown generator type " << inputGenerator << endl
                 << "Accepted generator types:";
      for (auto& gen : GetAcceptedGenerators())
        cout << gen << ", ";
      cout << endl;
      return false;
    }
    inputCounter++;
  }

  string outputPath = GetStringValue(moduleTree, "output.path", "test");
  LOG(info) << "CbmTransportConfig: Output path: " << outputPath;
  obj.SetOutFileName(outputPath + ".tra.root");
  obj.SetParFileName(outputPath + ".par.root");
  obj.SetGeoFileName(outputPath + ".geo.root");

  return true;
}

bool CbmTransportConfig::SetTarget(CbmTransport& obj, const pt::ptree& moduleTree)
{
  auto material  = moduleTree.get_optional<string>("target.material");
  auto thickness = moduleTree.get_optional<float>("target.thickness");
  auto diameter  = moduleTree.get_optional<float>("target.diameter");
  auto x         = moduleTree.get_optional<float>("target.position.x");
  auto y         = moduleTree.get_optional<float>("target.position.y");
  auto z         = moduleTree.get_optional<float>("target.position.z");
  auto rotY      = moduleTree.get_optional<float>("target.rotation.y");
  // one can set a target only if all 3 parameters are present in config file
  if (material && thickness && diameter) {
    if (x && y && z && rotY)
      obj.SetTarget(material.get().c_str(), thickness.get(), diameter.get(), x.get(), y.get(), z.get(), rotY.get());
    else if (x && y && z)
      obj.SetTarget(material.get().c_str(), thickness.get(), diameter.get(), x.get(), y.get(), z.get());
    else if (x && y)
      obj.SetTarget(material.get().c_str(), thickness.get(), diameter.get(), x.get(), y.get());
    else if (x)
      obj.SetTarget(material.get().c_str(), thickness.get(), diameter.get(), x.get());
    else
      obj.SetTarget(material.get().c_str(), thickness.get(), diameter.get());
  }
  else {
    LOG(error) << "CbmTransportConfig: Incomplete target configuration (material, thickness and diameter required)\n";
    return false;
  }
  return true;
}

bool CbmTransportConfig::SetBeamProfile(CbmTransport& obj, const pt::ptree& moduleTree)
{
  auto posX      = moduleTree.get_optional<float>("beam.position.x");
  auto posY      = moduleTree.get_optional<float>("beam.position.y");
  auto posZ      = moduleTree.get_optional<float>("beam.position.z");
  auto posSigmaX = moduleTree.get_optional<float>("beam.position.sigmaX");
  auto posSigmaY = moduleTree.get_optional<float>("beam.position.sigmaY");
  if (posX && posY) {
    if (posSigmaX && posSigmaY && posZ)
      obj.SetBeamPosition(posX.get(), posY.get(), posSigmaX.get(), posSigmaY.get(), posZ.get());
    else if (posSigmaX && posSigmaY)
      obj.SetBeamPosition(posX.get(), posY.get(), posSigmaX.get(), posSigmaY.get());
    else if (posSigmaX)
      obj.SetBeamPosition(posX.get(), posY.get(), posSigmaX.get());
    else if (posX && posY)
      obj.SetBeamPosition(posX.get(), posY.get());
  }
  else {
    LOG(error) << "CbmTransportConfig: Incomplete beam position configuration (x and y required)\n";
    return false;
  }
  auto angX      = moduleTree.get_optional<float>("beam.angle.x");
  auto angY      = moduleTree.get_optional<float>("beam.angle.y");
  auto angSigmaX = moduleTree.get_optional<float>("beam.angle.sigmaX");
  auto angSigmaY = moduleTree.get_optional<float>("beam.angle.sigmaY");
  if (angX && angY) {
    if (angSigmaX && angSigmaY) obj.SetBeamAngle(angX.get(), angY.get(), angSigmaX.get(), angSigmaY.get());
    else if (angSigmaX)
      obj.SetBeamAngle(angX.get(), angY.get(), angSigmaX.get());
    else if (angX && angY)
      obj.SetBeamAngle(angX.get(), angY.get());
  }
  else {
    LOG(error) << "CbmTransportConfig: Incomplete beam angle configuration (x and y required)\n";
    return false;
  }
  return true;
}

bool CbmTransportConfig::SetTransportParameters(CbmTransport& obj, const pt::ptree& moduleTree)
{
  bool randomRP       = true;
  auto configRandomRP = moduleTree.get_optional<float>("randomRP");
  if (configRandomRP) randomRP = configRandomRP.get();
  if (randomRP) obj.SetRandomEventPlane();

  ECbmEngine engine = kGeant3;
  auto geantVersion = moduleTree.get_optional<float>("geantVersion");
  if (geantVersion) {
    if (geantVersion.get() == 4) engine = kGeant4;
    else if (geantVersion.get() != 3) {
      LOG(error) << "CbmTransportConfig: Unknown Geant version: " << geantVersion.get();
      return false;
    }
  }
  obj.SetEngine(engine);

  if (engine == kGeant4) {
    auto g4settings = moduleTree.get_child_optional("geant4vmcSettings");
    if (g4settings) {
      auto g4settingsTree   = g4settings.get();
      auto physicsLists     = g4settingsTree.get_optional<string>("physicsLists");
      auto specialProcesses = g4settingsTree.get_optional<string>("specialProcesses");
      auto maxNsteps        = g4settingsTree.get_optional<int>("maxNsteps");

      CbmGeant4Settings* settings = new CbmGeant4Settings();

      if ((physicsLists && !specialProcesses) || (!physicsLists && specialProcesses)) {
        LOG(error)
          << "CbmTransportConfig: Incomplete Geant4 configuration: physicsLists and specialProcesses required!";
        return false;
      }
      else if (physicsLists && specialProcesses)
        settings->SetG4RunConfig("geomRoot", physicsLists.get(), specialProcesses.get());

      if (maxNsteps) settings->SetMaximumNumberOfSteps(maxNsteps.get());

      auto g4commands = g4settingsTree.get_child_optional("geant4commands");
      if (g4commands)
        for (auto& command : g4commands.get())
          settings->AddG4Command(command.second.data());
      settings->Dump();
      obj.SetGeant4Settings(settings);
    }
  }
  return true;
}

bool CbmTransportConfig::SetGeometry(CbmTransport& obj, const pt::ptree& moduleTree)
{
  return SetGeometry(obj.GetSetup(), moduleTree);
};

bool CbmTransportConfig::SetGeometry(CbmSetup* setup, const pt::ptree& moduleTree)
{
  auto geometry = moduleTree.get_child_optional("geometry");
  if (!geometry) {
    LOG(error) << "CbmTransportConfig: geometry settings missing!";
    return false;
  }
  auto geometryTree = geometry.get();
  auto baseSetup    = geometryTree.get_optional<string>("baseSetup");
  if (baseSetup) setup->LoadSetup(baseSetup.get().c_str());

  auto fieldTag   = geometryTree.get_optional<string>("magneticField.tag");
  auto fieldScale = geometryTree.get_optional<float>("magneticField.scale");
  auto fieldX     = geometryTree.get_optional<float>("magneticField.position.x");
  auto fieldY     = geometryTree.get_optional<float>("magneticField.position.y");
  auto fieldZ     = geometryTree.get_optional<float>("magneticField.position.z");

  if (fieldTag && fieldScale && fieldX && fieldY && fieldZ)
    setup->SetField(fieldTag.get().c_str(), fieldScale.get(), fieldX.get(), fieldY.get(), fieldZ.get());
  else if (fieldScale) {
    if (baseSetup) setup->SetFieldScale(fieldScale.get());
    else {
      LOG(error) << "CbmTransportConfig: Incomplete magnetic field configuration: fieldTag, fieldScale, fieldX, fieldY "
                    "and fieldZ are required if base setup is not defined!";
      return false;
    }
  }
  auto modules = geometryTree.get_child_optional("subsystems");
  if (modules)
    for (auto& module : modules.get()) {
      auto moduleId = stringToECbmModuleId(module.first);
      if (module.second.data() == "") setup->RemoveModule(moduleId);
      else
        setup->SetModule(moduleId, module.second.data().c_str());
    }
  return true;
}

bool CbmTransportConfig::SetStackFilter(CbmTransport& obj, const pt::ptree& moduleTree)
{
  auto stackFilterSettings = moduleTree.get_child_optional("stackFilter");
  if (!stackFilterSettings) return true;
  auto settingsTree      = stackFilterSettings.get();
  auto& filter           = obj.GetStackFilter();
  auto storeAllPrimaries = settingsTree.get_optional<bool>("storeAllPrimaries");
  auto storeAllMothers   = settingsTree.get_optional<bool>("storeAllMothers");
  auto storeAllDecays    = settingsTree.get_optional<bool>("storeAllDecays");
  if (storeAllPrimaries) filter->SetStoreAllPrimaries(storeAllPrimaries.get());
  if (storeAllMothers) filter->SetStoreAllPrimaries(storeAllMothers.get());
  if (storeAllDecays) filter->SetStoreAllPrimaries(storeAllDecays.get());
  return true;
}

bool CbmTransportConfig::LoadImpl(CbmTransport& obj, const pt::ptree& moduleTree)
{
  return SetIO(obj, moduleTree) && SetTarget(obj, moduleTree) && SetBeamProfile(obj, moduleTree)
         && SetTransportParameters(obj, moduleTree) && SetGeometry(obj, moduleTree) && SetStackFilter(obj, moduleTree);
}

ClassImp(CbmTransportConfig)
