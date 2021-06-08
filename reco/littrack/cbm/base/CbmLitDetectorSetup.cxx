/* Copyright (C) 2012-2020 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Florian Uhlig */

/**
 * \file CbmLitDetectorSetup.xx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2012
 */
#include "CbmLitDetectorSetup.h"

#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TObjArray.h"
//#include "CbmSetup.h"

#include <cassert>
#include <iostream>

using std::cout;

CbmLitDetectorSetup::CbmLitDetectorSetup() : fIsElectronSetup(false), fIsMuonSetup(false), fDet() {}

CbmLitDetectorSetup::~CbmLitDetectorSetup() {}

void CbmLitDetectorSetup::SetDet(ECbmModuleId detId, bool isDet) { fDet[detId] = isDet; }

bool CbmLitDetectorSetup::GetDet(ECbmModuleId detId) const
{
  assert(fDet.count(detId) != 0);
  return fDet.find(detId)->second;
}

bool CbmLitDetectorSetup::CheckDetectorPresence(const std::string& name) const
{
  assert(gGeoManager != NULL);

  TObjArray* nodes = gGeoManager->GetTopNode()->GetNodes();
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node = (TGeoNode*) nodes->At(iNode);
    if (TString(node->GetName()).Contains(name.c_str())) { return true; }
  }

  if (name == "mvd") {
    TGeoNode* node1 = gGeoManager->GetTopVolume()->FindNode("pipevac1_0");
    if (node1) {
      if (node1->GetVolume()->FindNode("mvdstation01_0")) { return true; }
    }
    else {
      TObjArray* nodes = gGeoManager->GetTopNode()->GetNodes();
      for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
        TGeoNode* node   = (TGeoNode*) nodes->At(iNode);
        TString nodeName = node->GetName();
        nodeName.ToLower();
        if (nodeName.Contains("pipe")) {
          TObjArray* nodes2 = node->GetNodes();
          for (Int_t iiNode = 0; iiNode < nodes2->GetEntriesFast(); iiNode++) {
            TGeoNode* node2   = (TGeoNode*) nodes2->At(iiNode);
            TString nodeName2 = node2->GetName();
            nodeName2.ToLower();
            if (nodeName2.Contains("pipevac1")) {
              // check if there is a mvd in the pipevac
              // if there are no nodes return immediately
              TObjArray* nodes3 = node2->GetNodes();
              if (!nodes3) return false;
              for (Int_t iiiNode = 0; iiiNode < nodes3->GetEntriesFast(); iiiNode++) {
                TGeoNode* node3   = (TGeoNode*) nodes3->At(iiiNode);
                TString nodeName3 = node3->GetName();
                nodeName3.ToLower();
                if (nodeName3.Contains("mvd")) { return true; }
              }
            }
          }
        }
      }
    }
  }

  return false;
}

void CbmLitDetectorSetup::DetermineSetup()
{
  fIsElectronSetup          = !CheckDetectorPresence("much");
  fIsMuonSetup              = CheckDetectorPresence("much");
  fDet[ECbmModuleId::kMvd]  = CheckDetectorPresence("mvd");
  fDet[ECbmModuleId::kSts]  = CheckDetectorPresence("sts") || CheckDetectorPresence("STS");
  fDet[ECbmModuleId::kRich] = CheckDetectorPresence("rich");
  fDet[ECbmModuleId::kTrd]  = CheckDetectorPresence("trd");
  fDet[ECbmModuleId::kMuch] = CheckDetectorPresence("much");
  fDet[ECbmModuleId::kTof]  = CheckDetectorPresence("tof");
  /*fIsElectronSetup = !CbmSetup::Instance()->IsActive(kMuch);
   fIsMuonSetup = CbmSetup::Instance()->IsActive(kMuch);
   fDet[ECbmModuleId::kMvd] = CbmSetup::Instance()->IsActive(kMvd);
   fDet[ECbmModuleId::kSts] = CbmSetup::Instance()->IsActive(kSts);
   fDet[ECbmModuleId::kRich] = CbmSetup::Instance()->IsActive(kRich);
   fDet[ECbmModuleId::kTrd] = CbmSetup::Instance()->IsActive(kTrd);
   fDet[ECbmModuleId::kMuch] = CbmSetup::Instance()->IsActive(kMuch);
   fDet[ECbmModuleId::kTof] = CbmSetup::Instance()->IsActive(kTof);*/
}

string CbmLitDetectorSetup::ToString() const
{
  string str = "LitDetectorSetup: ";
  if (fIsMuonSetup) str += "Muon setup; ";
  if (fIsElectronSetup) str += "Electron setup; ";
  str += "detectors in geometry: ";
  if (fDet.find(ECbmModuleId::kMvd)->second) str += "MVD ";
  if (fDet.find(ECbmModuleId::kSts)->second) str += "STS ";
  if (fDet.find(ECbmModuleId::kRich)->second) str += "RICH ";
  if (fDet.find(ECbmModuleId::kMuch)->second) str += "MUCH ";
  if (fDet.find(ECbmModuleId::kTrd)->second) str += "TRD ";
  if (fDet.find(ECbmModuleId::kTof)->second) str += "TOF ";
  return str;
}
