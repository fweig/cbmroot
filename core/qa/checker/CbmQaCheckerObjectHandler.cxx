/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerObjectHandler.h
/// @brief  Base handler class (implementation)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  09.02.2023

#include "CbmQaCheckerObjectHandler.h"

#include "CbmQaCanvas.h"

#include "Logger.h"

#include "TDirectory.h"
#include "TNamed.h"

using cbm::qa::checker::ObjectHandler;

// ---------------------------------------------------------------------------------------------------------------------
//
ObjectHandler::ObjectHandler(int iObject, int iFile, int iDataset, const char* objType)
  : fObjectID(iObject)
  , fFileID(iFile)
  , fDatasetID(iDataset)
  , fsObjType(objType)
{
}

// ---------------------------------------------------------------------------------------------------------------------
//
ObjectHandler::~ObjectHandler() {}

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectHandler::AddObjects(const std::vector<TNamed*>& vpObj)
{
  // ----- Check input
  LOG_IF(fatal, !fpObjDB) << "ObjectHandler: object database was not defined";
  LOG_IF(fatal, !fpOutDir) << "ObjectHandler: output directory was not defined";
  LOG_IF(fatal, (int) vpObj.size() != fpObjDB->GetNofVersions())
    << "ObjectHandler: Attempt to add vector with object pointers of different to the one of version labels";

  TDirectory* pCurrDir = gDirectory;
  gDirectory           = fpOutDir;
  for (int iVer = 0; iVer < fpObjDB->GetNofVersions(); ++iVer) {
    if (!fsBaseName.size()) { fsBaseName = vpObj[iVer]->GetName(); }
    else {
      LOG_IF(fatal, strcmp(fsBaseName.c_str(), vpObj[iVer]->GetName()))
        << "Hist1DHandler: attempt to add object of different name " << fsBaseName << " vs. " << vpObj[iVer]->GetName();
    }
    const char* cloneName = Form("%s_orig_%s", fsBaseName.data(), fpObjDB->GetVersionLabel(iVer).data());
    fvpObjects.push_back((TNamed*) vpObj[iVer]->Clone(cloneName));
  }
  gDirectory = pCurrDir;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectHandler::CompareWithDefault()
{
  int iDef = fpObjDB->GetDefaultID();
  for (int iV = 0; iV < (int) fvpObjects.size(); ++iV) {
    if (iV == iDef) { continue; }
    auto res = this->Compare(fvpObjects[iV], fvpObjects[iDef]);
    fpObjDB->SetCmpResult(fDatasetID, fFileID, fObjectID, iV, res);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectHandler::SetOutputDirectory(TDirectory* pDir)
{
  LOG_IF(fatal, !pDir) << "ObjectHandler: attempt to pass nullptr as a folder";
  fpOutDir = pDir;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void ObjectHandler::Write()
{
  fpOutDir->cd();
  for (auto pObj : fvpObjects) {
    pObj->Write();
  }
  if (fpCanvas.get()) { fpCanvas->Write(); }
}
