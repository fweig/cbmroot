// -------------------------------------------------------------------------
// -----                 CbmTofGeoHandler source file                  -----
// -----                 Created 13/08/10  by F. Uhlig                 -----
// -------------------------------------------------------------------------
#include "CbmTofGeoHandler.h"

#include "CbmDefs.h"                // for kTof
#include "CbmTofDetectorId_v07a.h"  // for CbmTofDetectorId_v07a
#include "CbmTofDetectorId_v12b.h"  // for CbmTofDetectorId_v12b
#include "CbmTofDetectorId_v14a.h"  // for CbmTofDetectorId_v14a
#include "CbmTofDetectorId_v21a.h"  // for CbmTofDetectorId_v21a

#include <FairLogger.h>  // for Logger, LOG

#include <TGeoBBox.h>     // for TGeoBBox
#include <TGeoManager.h>  // for gGeoManager, TGeoManager
#include <TGeoNode.h>     // for TGeoNode
#include <TGeoVolume.h>   // for TGeoVolume
#include <TObjArray.h>    // for TObjArray
#include <TObject.h>      // for TObject
#include <TVirtualMC.h>   // for TVirtualMC, gMC

#include <string.h>  // for strlen, strncpy

CbmTofGeoHandler::CbmTofGeoHandler()
  : TObject()
  , fTofId(nullptr)
  , fGeoVersion(-1)
  , fIsSimulation(kFALSE)
  , fMCVersion(-1)
  , fUseNodeName(kFALSE)
  , fLastUsedDetectorID(0)
  , fDetectorInfoArray()
  , fGeoPathHash(0)
  , fCurrentVolume(nullptr)
  , fVolumeShape(nullptr)
  , fGlobal()
  , fGlobalMatrix(nullptr) {}

Int_t CbmTofGeoHandler::Init(Bool_t isSimulation) {
  fIsSimulation = isSimulation;

  if (fIsSimulation) {
    TString tVirtualMCName = gMC->GetName();

    if (tVirtualMCName == "TGeant3TGeo") {
      fMCVersion = 0;
    } else if (tVirtualMCName == "TGeant4") {
      fMCVersion = 1;
    } else if (tVirtualMCName == "TFluka") {
      fMCVersion = 2;
    } else {
      fMCVersion = 3;  // GEANE
    }
  }

  Int_t geoVersion = CheckGeometryVersion();

  return geoVersion;
}

Int_t CbmTofGeoHandler::CheckGeometryVersion() {
  // Check which geometry version is used. In the moment there are 3
  // possible geometry versions.
  // v07a:
  //     Old TOF geometry at 10 m from target
  // v12a:
  //     Old TOF geometry at 6 m from target. Beside the z position
  //     the geometry is the same as v07a, so it is handled in the same way
  // v12b:
  //     New version of the tof geometrie developed by Norbert Herrmann
  //     including also a support structure
  // v14a:
  //     test beam tof geometrie developed by Norbert Herrmann
  // v20a:
  //     tof geometrie with MRPC and Module types developed by Norbert Herrmann

  if (nullptr == gGeoManager) LOG(fatal) << "No GeoManager";

  TObjArray* nodes = gGeoManager->GetTopNode()->GetNodes();
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node = (TGeoNode*) nodes->At(iNode);
    LOG(info) << "Inspect node " << iNode << " " << node->GetName();
    if (TString(node->GetName()).Contains("tof")) {
      // Since there is only one tof top node we check for full node name
      // In the old geometries v07a and v12a the name of this node is tof1_0
      // In new geometries the node name is tof_v<year><version> eg. tof_v12b
      // With this naming scheme the geometry version is completely qualified
      LOG(info) << "Found TOF geometry " << TString(node->GetName());
      if (TString(node->GetName()).EqualTo("tof1_0")) {
        LOG(info) << "Found TOF geometry v07a";
        fTofId      = new CbmTofDetectorId_v07a();
        fGeoVersion = k07a;
        return fGeoVersion;
      } else if (TString(node->GetName()).EqualTo("tof_v12b_0")) {
        LOG(info) << "Found TOF geometry v12b.";
        fTofId      = new CbmTofDetectorId_v12b();
        fGeoVersion = k12b;
        return fGeoVersion;
      } else if (TString(node->GetName()).Contains("v13")) {
        //TString(node->GetName()).EqualTo("tof_v13-3a_0")) {
        LOG(info) << "Found TOF geometry " << TString(node->GetName())
                  << ", treat as Id 12b   ";
        fTofId      = new CbmTofDetectorId_v12b();
        fGeoVersion = k12b;
        return fGeoVersion;
      } else if ((TString(node->GetName()).Contains("v14"))
                 || (TString(node->GetName()).Contains("v16a"))
                 || (TString(node->GetName()).Contains("v16c"))
                 || (TString(node->GetName()).Contains("v16d"))
                 || (TString(node->GetName()).Contains("v16e"))
                 || (TString(node->GetName()).Contains("v17a"))
                 || (TString(node->GetName()).Contains("v17c"))
                 || (TString(node->GetName()).Contains("v18"))
                 || (TString(node->GetName()).Contains("v19"))) {
          LOG(info)
            << "CbmTofGeoHandler::CheckGeometryVersion: Found TOF geometry "
            << TString(node->GetName()) << ", treat as Id 14a   ";
          //	if(nullptr!=fTofId) fTofId->Delete();
          fTofId      = new CbmTofDetectorId_v14a();
          fGeoVersion = k14a;

          if (TString(node->GetName()).Contains("v14a_n")) {
            if (fIsSimulation && 0 != fMCVersion) {
              LOG(fatal) << "Using node names instead of volume names to extract "
                            "the module type "
                         << "in a MC simulation only works with GEANT3 VMC!";
            }

            fUseNodeName = kTRUE;
          }
          return fGeoVersion;
      } else if ((TString(node->GetName()).Contains("v21"))
    	      || (TString(node->GetName()).Contains("v20"))) {
          LOG(info)
            << "CbmTofGeoHandler::CheckGeometryVersion: Found TOF geometry "
            << TString(node->GetName()) << ", treat as Id 21a   ";
          //	if(nullptr!=fTofId) fTofId->Delete();
          fTofId      = new CbmTofDetectorId_v21a();
          fGeoVersion = k21a;
          return fGeoVersion;
      } else {
        LOG(fatal) << "Found an unknown TOF geometry.";
        fGeoVersion = -1;
        return fGeoVersion;
      }
    }
  }
  LOG(fatal) << "No TOF geometry found!";
  fGeoVersion = -1;
  return fGeoVersion;
}

Int_t CbmTofGeoHandler::GetUniqueDetectorId(TString volName) {
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  return GetUniqueDetectorId();
}


Int_t CbmTofGeoHandler::GetUniqueDetectorId() {

  Int_t smtype  = 0;
  Int_t smodule = 0;
  Int_t counter = 0;
  Int_t countertype = 0;
  Int_t gap     = 0;
  Int_t cell    = 0;
  TString Volname;


  if (fGeoVersion == k07a) {
    Volname = CurrentVolName();
    smtype  = Volname[5] - '0';
    CurrentVolOffID(2, counter);
    CurrentVolOffID(1, cell);
    CurrentVolID(gap);
  } else if (fGeoVersion == k12b) {
    Volname = CurrentVolOffName(4);
    smtype  = Volname[7] - '0';
    CurrentVolOffID(4, smodule);
    CurrentVolOffID(2, counter);
    CurrentVolOffID(1, gap);
    CurrentVolID(cell);
  } else if (fGeoVersion == k14a) {  // test beam
    if (fUseNodeName) {
      Volname = CurrentNodeOffName(4);
    } else {
      Volname = CurrentVolOffName(4);
    }
    smtype = Volname[7] - '0';
    CurrentVolOffID(4, smodule);
    CurrentVolOffID(2, counter);
    CurrentVolOffID(1, gap);
    CurrentVolID(cell);
    //    counter=smodule;  // necessary for plastics
    //    smodule=smtype;   // for test beam setup
    gap = 0;
    cell--;
  } else if (fGeoVersion == k21a) {  // test beam
    if (fUseNodeName) {
      Volname = CurrentNodeOffName(4);
    } else {
      Volname = CurrentVolOffName(4);
    }
    smtype = Volname[7] - '0';
    CurrentVolOffID(4, smodule);
    CurrentVolOffID(2, counter);
    CurrentVolOffID(1, gap);
    CurrentVolID(cell);
    //    counter=smodule;  // necessary for plastics
    //    smodule=smtype;   // for test beam setup
    gap = 0;
    cell--;
  }

  LOG(debug1) << " Volname: " << Volname << ", " << CurrentVolOffName(3) << ", "
              << CurrentVolOffName(2) << ", " << CurrentVolOffName(1) << ", "
              << CurrentVolOffName(0);

  TString cTemp=CurrentVolOffName(2);
  TString cType=cTemp(8,2);  // 1 character only
  countertype=cType.Atoi();

  LOG(debug1) << " SMtype: " << smtype
		  	  << " SModule: " << smodule
			  << " CounterType: " << countertype
			  << " Counter: " << counter
			  << " Gap: " << gap
			  << " Strip: " << cell;

  CbmTofDetectorInfo detInfo(
    ECbmModuleId::kTof, smtype, smodule, counter, gap, cell, countertype);

  Int_t result = fTofId->SetDetectorInfo(detInfo);
  LOG(debug1) << " Unique ID: " << Form("0x%08x", result);
  //  return fTofId->SetDetectorInfo(detInfo);
  return result;
}


Int_t CbmTofGeoHandler::GetUniqueCounterId() {

  Int_t smtype  = 0;
  Int_t smodule = 0;
  Int_t countertype = 0;
  Int_t counter = 0;
  Int_t gap     = 0;
  Int_t cell    = 0;
  TString Volname;


  if (fGeoVersion == k07a) {
    Volname = CurrentVolName();
    smtype  = Volname[5] - '0';
    CurrentVolOffID(2, counter);
    CurrentVolOffID(1, cell);
    CurrentVolID(gap);
  } else if (fGeoVersion == k12b) {
    Volname = CurrentVolOffName(4);
    smtype  = Volname[7] - '0';
    CurrentVolOffID(4, smodule);
    CurrentVolOffID(2, counter);
    CurrentVolOffID(1, gap);
    CurrentVolID(cell);
  } else if (fGeoVersion == k14a) {  // test beam
	    if (fUseNodeName) {
	      Volname = CurrentNodeOffName(4);
	    } else {
	      Volname = CurrentVolOffName(4);
	    }
	    smtype = Volname[7] - '0';
	    CurrentVolOffID(4, smodule);
	    CurrentVolOffID(2, counter);
	    CurrentVolOffID(1, gap);
	    CurrentVolID(cell);
	    //    counter=smodule;  // necessary for plastics
	    //    smodule=smtype;   // for test beam setup
  }	else if (fGeoVersion == k21a) {  // test beam
		if (fUseNodeName) {
		  Volname = CurrentNodeOffName(4);
		} else {
		  Volname = CurrentVolOffName(4);
		}
		smtype = Volname[7] - '0';
		CurrentVolOffID(4, smodule);
		TString cTemp=CurrentVolOffName(2);
		TString cType=cTemp(8,2);  // 1 character only
		countertype=cType.Atoi();
		CurrentVolOffID(2, counter);
		CurrentVolOffID(1, gap);
		CurrentVolID(cell);
  }

  cell = 0;

  fDetectorInfoArray =
    CbmTofDetectorInfo(ECbmModuleId::kTof, smtype, smodule, counter, gap, cell, countertype);

  gap = 0;

  LOG(debug1) << "GetUniqueCounterId: ";
  LOG(debug1) << " Volname: " << Volname << ", " << CurrentVolOffName(3) << ", "
              << CurrentVolOffName(2) << ", " << CurrentVolOffName(1) << ", "
              << CurrentVolOffName(0);
  LOG(debug1) << " SMtype: " << smtype
		  	  << " SModule: " << smodule
			  << " CounterType: " << countertype
			  << " Counter: " << counter
			  << " Gap: " << gap
			  << " Cell: " << cell;

  CbmTofDetectorInfo detInfo(
    ECbmModuleId::kTof, smtype, smodule, counter, gap, cell, countertype);

  Int_t result        = fTofId->SetDetectorInfo(detInfo);
  fLastUsedDetectorID = result;
  LOG(debug1) << " Unique ID: " << Form("0x%08x", result);
  //  return fTofId->SetDetectorInfo(detInfo);
  return result;
}

/*
void CbmTofGeoHandler::FillInternalStructures()
{
  // Extract geometry information from Virtual MC.
  // All geometry handling should be done now in the
  // separate utility class CbmTofGeoHandler

  Int_t stationNr = 1;
  char volumeName[10];
  Bool_t result;
  Int_t MCid;

  fStationMap.clear();
  fModuleTypeMap.clear();

  if (fGeoVersion == kNewMonolithic) {

    do {
      sprintf(volumeName, "trd%dgas", stationNr);
      MCid = VolId(volumeName);
      if ( 0 != MCid) {
	fStationMap.insert(pair<Int_t,Int_t>(MCid,stationNr));
      }
      stationNr++;
    } while ( 0 != MCid);

  } else {

    do {
      sprintf(volumeName, "trd%d", stationNr);
      MCid = VolId(volumeName);
      if ( 0 != MCid) {
	fStationMap.insert(pair<Int_t,Int_t>(MCid,stationNr));
      }
      stationNr++;
    }
    while ( 0 != MCid);

    Int_t maxStationNr = --stationNr;

    Int_t maxModuleTypes;
    if (fGeoVersion == kSegmentedSquaredOneKeepingVolume) {
      maxModuleTypes = 8;
    } else {
      maxModuleTypes = 3;
    }

    for (Int_t iStation = 1; iStation < maxStationNr; iStation++) {
      for (Int_t iModule = 1; iModule <= maxModuleTypes; iModule++) {
	sprintf(volumeName, "trd%dmod%d", iStation, iModule);
	MCid = VolId(volumeName);
        if ( 0 != MCid ) {
	  fModuleTypeMap.insert(pair<Int_t,Int_t>(MCid,iModule));
	}
      }
    }
  }
}
*/

Int_t CbmTofGeoHandler::VolIdGeo(const char* name) const {
  //
  // Return the unique numeric identifier for volume name
  //

  Int_t uid = gGeoManager->GetUID(name);
  if (uid < 0) {
    LOG(info) << "VolId: Volume " << name << " not found";
    return 0;
  }
  return uid;
}

Int_t CbmTofGeoHandler::VolId(const Text_t* name) const {
  if (fIsSimulation) {
    return gMC->VolId(name);
  } else {
    //
    // Return the unique numeric identifier for volume name
    //
    char sname[20];
    Int_t len = strlen(name) - 1;
    if (name[len] != ' ') { return VolIdGeo(name); }
    strncpy(sname, name, len);
    sname[len] = 0;
    return VolIdGeo(sname);
  }
}

Int_t CbmTofGeoHandler::CurrentVolID(Int_t& copy) const {
  if (fIsSimulation) {
    return gMC->CurrentVolID(copy);
  } else {
    //
    // Returns the current volume ID and copy number
    //
    if (gGeoManager->IsOutside()) { return 0; }
    TGeoNode* node = gGeoManager->GetCurrentNode();
    copy           = node->GetNumber();
    Int_t id       = node->GetVolume()->GetNumber();
    return id;
  }
}

//_____________________________________________________________________________
Int_t CbmTofGeoHandler::CurrentVolOffID(Int_t off, Int_t& copy) const {
  if (fIsSimulation) {
    return gMC->CurrentVolOffID(off, copy);
  } else {
    //
    // Return the current volume "off" upward in the geometrical tree
    // ID and copy number
    //
    if (off < 0 || off > gGeoManager->GetLevel()) { return 0; }
    if (off == 0) { return CurrentVolID(copy); }
    TGeoNode* node = gGeoManager->GetMother(off);
    if (!node) { return 0; }
    copy = node->GetNumber();
    return node->GetVolume()->GetNumber();
  }
}

//_____________________________________________________________________________
const char* CbmTofGeoHandler::CurrentVolName() const {
  if (fIsSimulation) {
    return gMC->CurrentVolName();
  } else {
    //
    // Returns the current volume name
    //
    if (gGeoManager->IsOutside()) return gGeoManager->GetTopVolume()->GetName();
    return gGeoManager->GetCurrentVolume()->GetName();
  }
}

//_____________________________________________________________________________
const char* CbmTofGeoHandler::CurrentVolOffName(Int_t off) const {
  if (fIsSimulation) {
    return gMC->CurrentVolOffName(off);
  } else {
    //
    // Return the current volume "off" upward in the geometrical tree
    // ID, name and copy number
    // if name=0 no name is returned
    //
    if (off < 0 || off > gGeoManager->GetLevel()) return 0;
    if (off == 0) return CurrentVolName();
    TGeoNode* node = gGeoManager->GetMother(off);
    if (!node) return 0;
    return node->GetVolume()->GetName();
  }
}

//_____________________________________________________________________________
const char* CbmTofGeoHandler::CurrentNodeName() const {
  //
  // Returns the current node name
  //
  if (gGeoManager->IsOutside()) return gGeoManager->GetTopNode()->GetName();
  return gGeoManager->GetCurrentNode()->GetName();
}

//_____________________________________________________________________________
const char* CbmTofGeoHandler::CurrentNodeOffName(Int_t off) const {
  //
  // Return the current node "off" upward in the geometrical tree
  // if name=0 no name is returned
  //
  if (off < 0 || off > gGeoManager->GetLevel()) return 0;
  if (off == 0) return CurrentNodeName();
  TGeoNode* node = gGeoManager->GetMother(off);
  if (!node) return 0;
  return node->GetName();
}

void CbmTofGeoHandler::FillDetectorInfoArray(Int_t uniqueId) {
  fDetectorInfoArray  = fTofId->GetDetectorInfo(uniqueId);
  fLastUsedDetectorID = uniqueId;
}

Int_t CbmTofGeoHandler::GetDetSystemId(Int_t uniqueId) {
  if (fLastUsedDetectorID != uniqueId) { FillDetectorInfoArray(uniqueId); }
  return fDetectorInfoArray.fDetectorSystem;
}

Int_t CbmTofGeoHandler::GetSMType(Int_t uniqueId) {
  if (fLastUsedDetectorID != uniqueId) { FillDetectorInfoArray(uniqueId); }
  return fDetectorInfoArray.fSMtype;
}

Int_t CbmTofGeoHandler::GetSModule(Int_t uniqueId) {
  if (fLastUsedDetectorID != uniqueId) { FillDetectorInfoArray(uniqueId); }
  return fDetectorInfoArray.fSModule;
}

Int_t CbmTofGeoHandler::GetCounter(Int_t uniqueId) {
  if (fLastUsedDetectorID != uniqueId) { FillDetectorInfoArray(uniqueId); }
  return fDetectorInfoArray.fCounter;
}

Int_t CbmTofGeoHandler::GetGap(Int_t uniqueId) {
  if (fLastUsedDetectorID != uniqueId) { FillDetectorInfoArray(uniqueId); }
  return fDetectorInfoArray.fGap;
}

Int_t CbmTofGeoHandler::GetCell(Int_t uniqueId) {
  if (fLastUsedDetectorID != uniqueId) { FillDetectorInfoArray(uniqueId); }
  return fDetectorInfoArray.fCell;
}

Int_t CbmTofGeoHandler::GetRegion(Int_t uniqueId) {
  /*
  if (fLastUsedDetectorID != uniqueId) {
    FillDetectorInfoArray(uniqueId);
  }
*/
  return GetSMType(uniqueId);
}

Int_t CbmTofGeoHandler::GetCellId(Int_t uniqueId) {
  return fTofId->GetCellId(uniqueId);
}

Float_t CbmTofGeoHandler::GetSizeX(TString volName) {
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  Float_t sizex = fVolumeShape->GetDX();
  return sizex;
}

Float_t CbmTofGeoHandler::GetSizeY(TString volName) {
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  Float_t sizey = fVolumeShape->GetDY();
  return sizey;
}

Float_t CbmTofGeoHandler::GetSizeZ(TString volName) {
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  Float_t sizez = fVolumeShape->GetDZ();
  return sizez;
}

Float_t CbmTofGeoHandler::GetZ(TString volName) {
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  return fGlobal[2];
}

Float_t CbmTofGeoHandler::GetY(TString volName) {
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  return fGlobal[1];
}

Float_t CbmTofGeoHandler::GetX(TString volName) {
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  return fGlobal[0];
}

void CbmTofGeoHandler::NavigateTo(TString volName) {
  if (fIsSimulation) {
    LOG(fatal) << "This methode is not supported in simulation mode";
  } else {
    gGeoManager->cd(volName.Data());
    fGeoPathHash      = volName.Hash();
    fCurrentVolume    = gGeoManager->GetCurrentVolume();
    fVolumeShape      = (TGeoBBox*) fCurrentVolume->GetShape();
    Double_t local[3] = {0., 0., 0.};  // Local centre of volume
    gGeoManager->LocalToMaster(local, fGlobal);
    LOG(debug2) << "GeoNav: Pos: " << fGlobal[0] << " , " << fGlobal[1] << " , "
                << fGlobal[2];
    //    fGlobalMatrix = gGeoManager->GetCurrentMatrix();
  }
}


ClassImp(CbmTofGeoHandler)
