/** @file CbmStsParSetSensorCond.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 26.03.2020
 **/
#include "CbmStsParSetSensorCond.h"

#include "CbmStsParSensorCond.h"  // for CbmStsParSensorCond

#include <Logger.h>  // for Logger, LOG

#include <TString.h>  // for TString, operator<<, operator+, ope...
#include <TSystem.h>  // for TSystem, gSystem

#include <sstream>  // for operator<<, fstream, stringstream
#include <string>   // for char_traits
#include <utility>  // for pair

#include <assert.h>  // for assert

using std::string;

ClassImp(CbmStsParSetSensorCond)

  // -----   Constructor   ----------------------------------------------------
  CbmStsParSetSensorCond::CbmStsParSetSensorCond(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
{
}
// --------------------------------------------------------------------------


// -----   Destructor   -----------------------------------------------------
CbmStsParSetSensorCond::~CbmStsParSetSensorCond() {}
// --------------------------------------------------------------------------


// -----   Reset   ----------------------------------------------------------
void CbmStsParSetSensorCond::clear()
{
  fParams.clear();
  fIsInit = kFALSE;
  fGlobal = kFALSE;
  status  = kFALSE;
  resetInputVersions();
}
// --------------------------------------------------------------------------


// -----   Read parameters from ASCII file   --------------------------------
Bool_t CbmStsParSetSensorCond::getParams(FairParamList*)
{
  LOG(fatal) << GetName() << ": ASCII input is not defined!";
  return kFALSE;
}
// --------------------------------------------------------------------------


// -----   Get condition parameters of a sensor   ---------------------------
const CbmStsParSensorCond& CbmStsParSetSensorCond::GetParSensor(UInt_t address)
{

  if (!fIsInit) Init();

  // --- Return global conditions, if set
  if (fGlobal) return fGlobalParams;

  // --- Else, look in the address map
  else {
    assert(fParams.count(address));
    return fParams.at(address);
  }
}
// --------------------------------------------------------------------------


// -----   Initialise the conditions   --------------------------------------
void CbmStsParSetSensorCond::Init()
{

  // --- Global conditions
  fGlobalParams.Init();

  // --- Conditions sensor by sensor
  for (auto& it : fParams)
    it.second.Init();

  fIsInit = kTRUE;
}
// --------------------------------------------------------------------------


// -----   Write parameters from ASCII file   -------------------------------
void CbmStsParSetSensorCond::putParams(FairParamList*) { LOG(fatal) << GetName() << ": ASCII output is not defined!"; }
// --------------------------------------------------------------------------


// -----   Set sensor conditions from file   --------------------------------
UInt_t CbmStsParSetSensorCond::ReadParams(const char* fileName)
{

  // --- Warn against multiple initialisation
  if (fGlobal) { LOG(warn) << GetName() << ": Previously defined global settings will be ignored."; }
  if (!fParams.empty()) {
    LOG(warn) << GetName() << ": Overwriting previously defined parameter sets.";
    fParams.clear();
  }

  // --- Input file
  std::fstream inFile;
  TString inputFile = fileName;

  // Try with argument as is (absolute path or current directory)
  inFile.open(inputFile.Data());

  // If not successful, look in the standard parameter directory
  if (!inFile.is_open()) {
    inputFile = gSystem->Getenv("VMCWORKDIR");
    inputFile += "/parameters/sts/" + TString(fileName);
    inFile.open(inputFile.Data());
  }

  // If still not open, throw an error
  if (!inFile.is_open()) {
    LOG(fatal) << GetName() << ": Cannot read file " << fileName << " nor " << inputFile;
    return 0;
  }

  string input;
  TString sName;
  UInt_t nSensors      = 0;
  UInt_t address       = 0;
  Double_t vDep        = -1.e10;
  Double_t vBias       = -1.e10;
  Double_t temperature = -1.e10;
  Double_t cCoupling   = -1.e10;
  Double_t cInterstrip = -1.e10;

  while (kTRUE) {  // read one line per sensor
    if (inFile.eof()) break;
    getline(inFile, input);
    if (input.empty() || input[0] == '#') continue;  // Comment line
    std::stringstream line(input);
    line >> sName >> address >> vDep >> vBias >> temperature >> cCoupling >> cInterstrip;

    // Check presence and validity of condition parameters
    if (vDep < 1.e-9 || vBias < 1.e-9 || temperature < 1.e-9 || cCoupling < 1.e-9 || cInterstrip < 1.e-9) {
      LOG(fatal) << GetName() << ": Missing or illegal condition parameters for address " << address << "; " << vDep
                 << " " << vBias << " " << temperature << " " << cCoupling << " " << cInterstrip;
      continue;
    }

    // Check for double occurrences of addresses
    if (fParams.count(address)) {
      LOG(fatal) << GetName() << ": Multiple occurence of address " << address << " in input file";
      continue;
    }

    // Add sensor conditions to set
    fParams[address].SetParams(vDep, vBias, temperature, cCoupling, cInterstrip);
    nSensors++;
  }  //# input lines

  inFile.close();
  LOG(info) << GetName() << ": Read conditions of " << nSensors << (nSensors == 1 ? " sensor" : " sensors") << " from "
            << inputFile;

  return nSensors;
}
// --------------------------------------------------------------------------


// -----   Set the global sensor conditions   -------------------------------
void CbmStsParSetSensorCond::SetGlobalPar(Double_t vFd, Double_t vBias, Double_t temperature, Double_t cCoupling,
                                          Double_t cInterstrip)
{
  if (fGlobal) LOG(warn) << GetName() << ": Overwriting current global settings!";
  fGlobalParams.SetParams(vFd, vBias, temperature, cCoupling, cInterstrip);
  fGlobal = kTRUE;
}
// -------------------------------------------------------------------------


// -----   Set the global sensor conditions   ------------------------------
void CbmStsParSetSensorCond::SetGlobalPar(const CbmStsParSensorCond& conditions)
{
  Double_t vFd         = conditions.GetVfd();
  Double_t vBias       = conditions.GetVbias();
  Double_t temperature = conditions.GetTemperature();
  Double_t cCoupling   = conditions.GetCcoupling();
  Double_t cInterstrip = conditions.GetCinterstrip();
  SetGlobalPar(vFd, vBias, temperature, cCoupling, cInterstrip);
}
// -------------------------------------------------------------------------


// -----   Info to string   ------------------------------------------------
std::string CbmStsParSetSensorCond::ToString()
{
  if (!fIsInit) Init();
  std::stringstream ss;
  if (!IsSet()) ss << "Empty";
  else if (fGlobal)
    ss << "(Global) " << fGlobalParams.ToString();
  else
    ss << "Conditions for " << fParams.size() << " sensors";
  return ss.str();
}
// -------------------------------------------------------------------------
