/** @file CbmStsSetup.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 27.05.2013
 **/
#include "CbmStsSetup.h"

#include "CbmDefs.h"                 // for kSts
#include "CbmStsAddress.h"           // for GetElementId, kStsSensor, GetAdd...
#include "CbmStsModule.h"            // for CbmStsModule
#include "CbmStsParSensorCond.h"     // for CbmStsParSensorCond
#include "CbmStsParSetModule.h"      // for CbmStsParSetModule
#include "CbmStsParSetSensor.h"      // for CbmStsParSetSensor
#include "CbmStsParSetSensorCond.h"  // for CbmStsParSetSensorCond
#include "CbmStsSensor.h"            // for CbmStsSensor
#include "CbmStsStation.h"           // for CbmStsStation

#include <FairField.h>               // for FairField
#include <FairRun.h>                 // for FairRun
#include <FairLogger.h>              // for FairLogger

#include <TCollection.h>             // for TIter
#include <TGeoMatrix.h>              // for TGeoHMatrix
#include <TGeoNode.h>                // for TGeoNode
#include <TGeoPhysicalNode.h>        // for TGeoPhysicalNode
#include <TGeoVolume.h>              // for TGeoVolume
#include <TList.h>                   // for TList
#include <TString.h>                 // for TString, operator<<, operator+
#include <TFile.h>                   // for TFile
#include <TGeoManager.h>             // for TGeoManager, gGeoManager
#include <TKey.h>                    // for TKey
#include <TSystem.h>                 // for TSystem, gSystem

#include <string.h>                  // for strcmp
#include <cassert>                   // for assert
#include <iomanip>                   // for setw, __iom_t6
#include <iostream>                  // for fstream, string, char_traits

using namespace std;

// -----   Initialisation of static singleton pointer   --------------------
CbmStsSetup* CbmStsSetup::fgInstance = nullptr;
// -------------------------------------------------------------------------



// -----   Constructor   ---------------------------------------------------
CbmStsSetup::CbmStsSetup() : CbmStsElement(ToIntegralType(ECbmModuleId::kSts), kStsSystem),
			     fSensors(),
			     fModules(),
			     fModuleVector(),
			     fStations() 
{
}
// -------------------------------------------------------------------------





// -----   Create station objects   ----------------------------------------
Int_t CbmStsSetup::CreateStations() {

  // For old geometries: the station corresponds to the unit
  if ( fHasStations ) {
    for (Int_t iUnit = 0; iUnit < GetNofDaughters(); iUnit++) {
      CbmStsElement* unit = GetDaughter(iUnit);
      // Create one station for each unit
      Int_t stationId = unit->GetIndex();
      TString name = Form("STS_S%02i", stationId+1);
      TString title = Form("STS Station %i", stationId+1);
      CbmStsStation* station = new CbmStsStation(name, title,
                                                 unit->GetPnode());
      // Add all ladders of the unit to the station
      for (Int_t iLadder = 0; iLadder < unit->GetNofDaughters(); iLadder++)
        station->AddLadder(unit->GetDaughter(iLadder));
      // Initialise station parameters
      station->Init();
      // Add station to station map
      assert ( fStations.find(stationId) == fStations.end() );
      fStations[stationId] = station;
    } //# units
    return fStations.size();
  } //? is old geometry?


  // Loop over all ladders. They are associated to a station.
  for (Int_t iUnit = 0; iUnit < GetNofDaughters(); iUnit++) {
    CbmStsElement* unit = GetDaughter(iUnit);
    for ( Int_t iLadder = 0; iLadder < unit->GetNofDaughters(); iLadder++) {
      CbmStsElement* ladder = unit->GetDaughter(iLadder);
      // This convention must be followed by the STS geometry
      Int_t nodeNumber = ladder->GetPnode()->GetNode()->GetNumber();
      Int_t stationId = nodeNumber / 100 - 1;
      // Get the station from the map. If not there, create it.
      CbmStsStation* station = nullptr;
      if ( fStations.find(stationId) == fStations.end() ) {
        TString name = Form("STS_S%02i", stationId+1);
        TString title = Form("STS Station %i", stationId+1);
        station = new CbmStsStation(name, title);
        fStations[stationId] = station;
      } //? station not yet in map
      else station = fStations[stationId];

      // Add ladder to station
      station->AddLadder(ladder);

    } //# ladders
  } //# units

  // Initialise the stations
  auto it = fStations.begin();
  while ( it != fStations.end() ) {
    it->second->Init();
    it++;
  } //# stations

  // Check that the station number is set consecutively and that the
  // stations are ordered w.r.t. position along the beam
  Bool_t isOk = kTRUE;
  Double_t zPrevious = -999999;
  for (UInt_t iStation = 0; iStation < fStations.size(); iStation++) {
    if ( fStations.find(iStation) == fStations.end() ) {
      LOG(error) << GetName() << ": Number of stations is "
          << fStations.size() << ", but station " << iStation
          << "is not present!";
      isOk = kFALSE;
    } //? station present?
    if ( fStations[iStation]->GetZ() <= zPrevious ) {
      LOG(error) << GetName() << ": Disordered stations. Station "
          << iStation << " is at z = " << fStations[iStation]->GetZ()
          << "cm , previous is at z = " << zPrevious << " cm.";
      isOk = kFALSE;
    } //? disordered in z
  } //# stations
  if ( ! isOk ) LOG(fatal) << GetName() << ": Error in creation of stations.";

  return fStations.size();
}
// -------------------------------------------------------------------------



// -----   Get an element from the STS setup   -----------------------------
CbmStsElement* CbmStsSetup::GetElement(Int_t address, Int_t level) {

	// --- Check for initialisation
	if ( ! fAddress ) LOG(fatal) << fName << ": not initialised!";

	// --- Catch non-STS addresses
	if ( CbmStsAddress::GetSystemId(address) != ECbmModuleId::kSts ) {
		LOG(warn) << fName << ": No STS address " << address;
		return nullptr;
	}

	// --- Catch illegal level numbers
	if ( level < 0 || level >= kStsNofLevels ) {
		LOG(warn) << fName << ": Illegal level " << level;
		return nullptr;
	}

	CbmStsElement* element = this;
	for (Int_t iLevel = 1; iLevel <= level; iLevel++) {
		element =
				element->GetDaughter(CbmStsAddress::GetElementId(address, iLevel));
		assert(element);
	}

	return element;
}
// -------------------------------------------------------------------------



// -----   Get hierarchy level name   ---------------------------------------
const char* CbmStsSetup::GetLevelName(Int_t level) const {

  // --- Catch legacy (setup with stations)
  if ( fHasStations && level == kStsUnit ) return "station";

  switch(level) {
    case kStsSystem: return "sts"; break;
    case kStsUnit: return "unit"; break;
    case kStsLadder: return "ladder"; break;
    case kStsHalfLadder: return "halfladder"; break;
    case kStsModule: return "module"; break;
    case kStsSensor: return "sensor"; break;
    case kStsSide: return "side"; break;
    default: return ""; break;
  }

}
// -------------------------------------------------------------------------



// -----   Get the station number from an address   ------------------------
Int_t CbmStsSetup::GetStationNumber(Int_t address) {

  // In old, station-based geometries, the station equals the unit
  if ( fHasStations ) return CbmStsAddress::GetElementId(address, kStsUnit);

  // The station is obtained from the ladder
  CbmStsElement* ladder = CbmStsSetup::GetElement(address, kStsLadder);
  assert(ladder);
  return ladder->GetPnode()->GetNode()->GetNumber() / 100 - 1;

}
// -------------------------------------------------------------------------



// -----   Initialisation   ------------------------------------------------
Bool_t CbmStsSetup::Init(const char* geoFile) {

  // Prevent duplicate initialisation
  assert( ! fIsInitialised );

  LOG(info);
  LOG(info) << "==========================================================";
  LOG(info) << "Initialising STS Setup \n";

  // --- Set system address
  fAddress = CbmStsAddress::GetAddress();

  // --- If a geometry file was specified, read the geometry from file
  if ( geoFile ) ReadGeometry(geoFile);

  // --- Else, read the geometry from TGeoManager
  else {
    assert(gGeoManager);
    ReadGeometry(gGeoManager);
  }

  // --- Statistics
  LOG(info) << fName << ": Elements in setup: ";
  for (Int_t iLevel = 1; iLevel <= kStsSensor; iLevel++) {
      TString name = GetLevelName(iLevel);
      name += "s";
      LOG(info) << "     " << setw(12) << name << setw(5) << right
                  << GetNofElements(iLevel);
  }

  // --- Build the module and sensor maps
  for (Int_t iUnit = 0; iUnit < GetNofDaughters(); iUnit++) {
    CbmStsElement* unit = GetDaughter(iUnit);
    for (Int_t iLad = 0; iLad < unit->GetNofDaughters(); iLad++) {
      CbmStsElement* ladd = unit->GetDaughter(iLad);
      for (Int_t iHla = 0; iHla < ladd->GetNofDaughters(); iHla++) {
        CbmStsElement* hlad = ladd->GetDaughter(iHla);
        for (Int_t iMod = 0; iMod < hlad->GetNofDaughters(); iMod++) {
          CbmStsElement* modu = hlad->GetDaughter(iMod);
          CbmStsModule* module = dynamic_cast<CbmStsModule*>(modu);
          assert(module);
          Int_t moduleAddress = module->GetAddress();
          assert( ! fModules.count(moduleAddress) );
          fModules[moduleAddress] = module;
          fModuleVector.push_back(module);
          for (Int_t iSens = 0; iSens < modu->GetNofDaughters(); iSens++) {
            CbmStsElement* sens = modu->GetDaughter(iSens);
            CbmStsSensor* sensor = dynamic_cast<CbmStsSensor*>(sens);
            assert(sensor);
            Int_t sensorAddress = sensor->GetAddress();
            fSensors[sensorAddress] = sensor;
            fSensorVector.push_back(sensor);
          } //# sensors in module
        } //# modules in half-ladder
      } //# half-ladders in ladder
    } //# ladders in unit
  } //# units in system
  LOG(info) << "Sensor in map: " << fSensors.size() << " in vector "
      << fSensorVector.size();
  assert( fSensors.size() == fSensorVector.size() );

  // --- Create station objects
  Int_t nStations = CreateStations();
  LOG(info) << GetName() << ": Setup contains " << nStations
      << " stations objects.";
  if ( FairLogger::GetLogger()->IsLogNeeded(fair::Severity::debug) ) {
    auto it = fStations.begin();
    while ( it != fStations.end() ) {
      LOG(debug) << "  " << it->second->ToString();
      it++;
    } //# stations
  } //? Debug

  // --- Consistency check
  if ( GetNofSensors() != GetNofElements(kStsSensor) )
    LOG(fatal) << GetName() << ": inconsistent number of sensors! "
                   << GetNofElements(kStsSensor) << " " << GetNofSensors();
  if ( Int_t(fModules.size()) != GetNofElements(kStsModule) )
    LOG(fatal) << GetName() << ": inconsistent number of modules! "
                   << GetNofElements(kStsModule) << " "
                   << fModules.size();

  LOG(info) << "=========================================================="
            << "\n";
  LOG(info);

  fIsInitialised = kTRUE;
  return kTRUE;
}
// -------------------------------------------------------------------------



// -----   Singleton instance   --------------------------------------------
CbmStsSetup* CbmStsSetup::Instance() {
  if ( ! fgInstance ) fgInstance = new CbmStsSetup();
  return fgInstance;
}
// -------------------------------------------------------------------------



// -----   Print list of modules   -----------------------------------------
void CbmStsSetup::ListModules() const {
  for (auto it = fModules.begin(); it != fModules.end(); it++)
    LOG(info) << it->second->ToString();
}
// -------------------------------------------------------------------------



// -----   Read geometry from TGeoManager   --------------------------------
Bool_t CbmStsSetup::ReadGeometry(TGeoManager* geo) {

  // --- Catch non-existence of GeoManager
  assert (geo);
  LOG(info) << fName << ": Reading geometry from TGeoManager "
            << geo->GetName();

  // --- Get cave (top node)
  geo->CdTop();
  TGeoNode* cave = geo->GetCurrentNode();

  // --- Get top STS node
  TGeoNode* sts = nullptr;
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
    TString name = cave->GetDaughter(iNode)->GetName();
     if ( name.Contains("STS", TString::kIgnoreCase) ) {
      sts = cave->GetDaughter(iNode);
      LOG(info) << fName << ": STS top node is " << sts->GetName();
      break;
    }
  }
  if ( ! sts ) {
    LOG(error) << fName << ": No top STS node found in geometry!";
    return kFALSE;
  }

  // --- Create physical node for STS
  TString path = cave->GetName();
  path = path + "/" + sts->GetName();
  fNode = new TGeoPhysicalNode(path);

  // --- Check for old geometry (with stations) or new geometry (with units)
  Bool_t hasStation = kFALSE;
  Bool_t hasUnit = kFALSE;
  for (Int_t iDaughter = 0; iDaughter < fNode->GetNode()->GetNdaughters();
       iDaughter++) {
    TString dName = fNode->GetNode()->GetDaughter(iDaughter)->GetName();
    if ( dName.Contains("station", TString::kIgnoreCase) ) hasStation = kTRUE;
    if ( dName.Contains("unit", TString::kIgnoreCase) ) hasUnit = kTRUE;
  }
  if ( hasUnit && (! hasStation) ) fHasStations = kFALSE;
  else if ( (! hasUnit) && hasStation) fHasStations = kTRUE;
  else if ( hasUnit && hasStation) LOG(fatal) << GetName()
      << ": geometry contains both units and stations!";
  else LOG(fatal) << GetName()
                  << ": geometry contains neither units nor stations!";

  if ( fHasStations ) LOG(warn) << GetName()
                  << ": using old geometry (with stations)";

  // --- Recursively initialise daughter elements
  InitDaughters();

  return kTRUE;
}
// -------------------------------------------------------------------------



// -----   Read geometry from geometry file   ------------------------------
Bool_t CbmStsSetup::ReadGeometry(const char* fileName) {

  LOG(info) << fName << ": Reading geometry from file " << fileName;

  // Exit if a TGeoManager is already present
  assert ( ! gGeoManager );

  // --- Open geometry file
  TFile geoFile(fileName);
  if ( ! geoFile.IsOpen() ) {
    LOG(fatal) << GetName() << ": Could not open geometry file "
        << fileName;
    return kFALSE;
  }

  // Create a new TGeoManager
  TGeoManager* stsGeometry = new TGeoManager("StsGeo",
                                             "STS stand-alone geometry");

  // --- Get top volume from file
  TGeoVolume* topVolume = nullptr;
  TList* keyList = geoFile.GetListOfKeys();
  TKey* key = nullptr;
  TIter keyIter(keyList);
  while ( (key = (TKey*) keyIter() ) ) {
    if ( strcmp( key->GetClassName(), "TGeoVolumeAssembly" ) == 0 ) {
      TGeoVolume* volume = (TGeoVolume*) key->ReadObj();
      if ( strcmp(volume->GetName(), "TOP") == 0 ) {
        topVolume = volume;
        break;
      }  //? volume name is "TOP"
    }    //? object class is TGeoVolumeAssembly
  }
  if ( ! topVolume) {
    LOG(fatal) << GetName() << ": No TOP volume in file!";
    return kFALSE;
  }
  stsGeometry->SetTopVolume(topVolume);

  // --- Get cave (top node)
  stsGeometry->CdTop();
  TGeoNode* cave = stsGeometry->GetCurrentNode();

  // --- Get top STS node
  TGeoNode* sts = nullptr;
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
    TString name = cave->GetDaughter(iNode)->GetName();
    if ( name.Contains("STS", TString::kIgnoreCase) ) {
      sts = cave->GetDaughter(iNode);
      stsGeometry->CdDown(iNode);
      LOG(info) << fName << ": STS top node is " << sts->GetName();
      break;
    }
  }
  if ( ! sts ) {
    LOG(error) << fName << ": No top STS node found in geometry!";
    return kFALSE;
  }

  // --- Create physical node for STS
  TString path = cave->GetName();
  path = path + "/" + sts->GetName();
  fNode = new TGeoPhysicalNode(path);

  // --- Check for old geometry (with stations) or new geometry (with units)
  TString dName = fNode->GetNode()->GetDaughter(0)->GetName();
  if ( dName.Contains("station", TString::kIgnoreCase) ) fHasStations = kTRUE;
  else if ( dName.Contains("unit", TString::kIgnoreCase) ) fHasStations = kFALSE;
  else LOG(fatal) << GetName() << ": unknown geometry type; first level name is "
      << dName;
  if ( fHasStations ) LOG(warn) << GetName()
                  << ": using old geometry (with stations)";

  // --- Recursively initialise daughter elements
  InitDaughters();

  return kTRUE;
}
// -------------------------------------------------------------------------



// -----   Set module parameters from parameter set   ----------------------
UInt_t CbmStsSetup::SetModuleParameters(CbmStsParSetModule* params) {
  UInt_t nModules = 0;
  for ( auto& moduleIt : fModules ) {
    UInt_t address = moduleIt.first;
    moduleIt.second->SetParameters(params->GetParModule(address));
    nModules++;
  }
  LOG(info) << GetName() << ": Set parameters for " << nModules << " modules";
  fIsModuleParsInit = kTRUE;
  return nModules;
}
// -------------------------------------------------------------------------



// -----   Set the sensor conditions from the parameter set   --------------
UInt_t CbmStsSetup::SetSensorConditions(CbmStsParSetSensorCond* conds) {

  UInt_t nSensors = 0;
  for ( auto& sensorIt : fSensors ) {
    UInt_t address = sensorIt.first;
    sensorIt.second->SetConditions(&(conds->GetParSensor(address)));
    nSensors++;
  }
  LOG(info) << GetName() << ": Set conditions for " << nSensors << " sensors";
  fIsSensorCondInit = kTRUE;
  return nSensors;

}
// -------------------------------------------------------------------------



// -----   Set the sensor parameters   -------------------------------------
UInt_t CbmStsSetup::SetSensorParameters(CbmStsParSetSensor* parSet) {
  UInt_t nSensors = 0;
  for ( auto& sensorIt : fSensors ) {
    UInt_t address = sensorIt.first;
    sensorIt.second->SetParameters(&(parSet->GetParSensor(address)));
    nSensors++;
  }
  LOG(info) << GetName() << ": Set parameters for " << nSensors << " sensors";
  fIsSensorParsInit = kTRUE;
  return nSensors;
}
// -------------------------------------------------------------------------


ClassImp(CbmStsSetup)

