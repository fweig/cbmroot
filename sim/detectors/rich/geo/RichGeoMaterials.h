/* Copyright (C) 2022-2022 UGiessen/GSI, Giessen/Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

#ifndef RICH_GEO_MATERIALS
#define RICH_GEO_MATERIALS

#include "FairGeoBuilder.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoMedia.h"
#include "FairGeoMedium.h"
#include "FairLogger.h"

#include "TGeoManager.h"
#include "TGeoMedium.h"
#include "TSystem.h"

#include <map>
#include <string>


class RichGeoMaterials {
public:
  void Init()
  {
    LOG(info) << "RichGeoMaterials init materials";

    FairGeoLoader* geoLoad    = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    std::string mediaPath     = std::string(gSystem->Getenv("VMCWORKDIR")) + "/geometry/media.geo";
    geoFace->setMediaFile(mediaPath.c_str());
    geoFace->readMedia();

    fGeoMedia = geoFace->getMedia();
    fGeoBuild = geoLoad->getGeoBuilder();

    fMediums["aluminium"]        = InitMedium("aluminium");
    fMediums["CsI"]              = InitMedium("CsI");
    fMediums["RICHgas_CO2_dis"]  = InitMedium("RICHgas_CO2_dis");
    fMediums["vacuum"]           = InitMedium("vacuum");
    fMediums["RICHglass"]        = InitMedium("RICHglass");
    fMediums["kapton"]           = InitMedium("kapton");
    fMediums["iron"]             = InitMedium("iron");
    fMediums["RICHgas_CO2_dis+"] = InitMedium("RICHgas_CO2_dis+");
  }

  TGeoMedium* GetMedium(const std::string& name)
  {
    if (fMediums.find(name) == fMediums.end()) LOG(fatal) << "RichGeoMaterials::GetMedium " << name << " not found";
    return fMediums[name];
  }

private:
  std::map<std::string, TGeoMedium*> fMediums;

  FairGeoMedia* fGeoMedia   = nullptr;
  FairGeoBuilder* fGeoBuild = nullptr;

  TGeoMedium* InitMedium(const std::string& name)
  {
    FairGeoMedium* fairMedium = fGeoMedia->getMedium(name.c_str());
    if (fairMedium == nullptr) LOG(fatal) << "RichGeoMaterials FairMedium " << name << " not found";
    fGeoBuild->createMedium(fairMedium);
    TGeoMedium* rootMedium = gGeoManager->GetMedium(name.c_str());
    if (rootMedium == nullptr) LOG(fatal) << "RichGeoMaterials Medium " << name << " not found";
    return rootMedium;
  }
};

#endif
