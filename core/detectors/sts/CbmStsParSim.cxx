/** @file CbmStsParSim.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 02.04.2020
 **/

#include "CbmStsParSim.h"

#include <FairLogger.h>  // for LOG, Logger

#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

ClassImp(CbmStsParSim)

  // -----   Constructor   ----------------------------------------------------
  CbmStsParSim::CbmStsParSim(const char* name,
                             const char* title,
                             const char* context)
  : FairParGenericSet(name, title, context) {}
// --------------------------------------------------------------------------


// -----   Reset   ----------------------------------------------------------
void CbmStsParSim::clear() {
  status = kFALSE;
  resetInputVersions();
}
// --------------------------------------------------------------------------


// -----   Read parameters from ASCII file   --------------------------------
Bool_t CbmStsParSim::getParams(FairParamList*) {
  LOG(fatal) << GetName() << ": ASCII input is not defined!";
  return kFALSE;
}
// --------------------------------------------------------------------------


// -----   Write parameters from ASCII file   -------------------------------
void CbmStsParSim::putParams(FairParamList*) {
  LOG(fatal) << GetName() << ": ASCII output is not defined!";
}
// --------------------------------------------------------------------------


// -----   String output   --------------------------------------------------
std::string CbmStsParSim::ToString() const {
  std::stringstream ss;
  ss << "Mode: " << (fEventMode ? "event" : "stream");
  ss << " | Diffusion: " << (fDiffusion ? "ON" : "OFF");
  ss << " | Lorentz shift: " << (fLorentzShift ? "ON" : "OFF");
  ss << " | Cross-talk: " << (fCrossTalk ? "ON" : "OFF");
  ss << " | Energy loss: ";
  if (fELossModel == CbmStsELoss::kIdeal)
    ss << "ideal";
  else if (fELossModel == CbmStsELoss::kUniform)
    ss << "uniform";
  else if (fELossModel == CbmStsELoss::kUrban)
    ss << "Urban model";
  else
    ss << "unknown";
  ss << " | Noise: " << (fNoise ? "ON" : "OFF");
  if (fOnlyPrimaries) ss << " | ONLY PRIMARIES";
  return ss.str();
}
// --------------------------------------------------------------------------
