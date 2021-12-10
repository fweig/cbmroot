/* Copyright (C) 2004-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                      CbmMCTrack source file                   -----
// -----                  Created 03/08/04  by V. Friese               -----
// -------------------------------------------------------------------------
#include "CbmMCTrack.h"

#include <Logger.h>  // for LOG, Logger

#include <TDatabasePDG.h>  // for TDatabasePDG
#include <TMCProcess.h>    // for kPNoProcess, TMCProcessName
#include <TObject.h>       // for TObject
#include <TParticle.h>     // for TParticle
#include <TParticlePDG.h>  // for TParticlePDG

#include <sstream>  // for operator<<, basic_ostream, endl, stri...
#include <string>   // for char_traits

using std::stringstream;


// -----   Default constructor   -------------------------------------------
CbmMCTrack::CbmMCTrack()
  : TObject()
  , fProcessId(kPNoProcess)
  , fPdgCode(0)
  , fMotherId(-1)
  , fPx(0.)
  , fPy(0.)
  , fPz(0.)
  , fE(-1.)
  , fStartX(0.)
  , fStartY(0.)
  , fStartZ(0.)
  , fStartT(0.)
  , fNPoints(0)
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMCTrack::CbmMCTrack(int32_t pdgCode, int32_t motherId, double px, double py, double pz, double x, double y, double z,
                       double t, int32_t nPoints = 0)
  : TObject()
  , fProcessId(kPNoProcess)
  , fPdgCode(pdgCode)
  , fMotherId(motherId)
  , fPx(px)
  , fPy(py)
  , fPz(pz)
  , fE(-1.)
  , fStartX(x)
  , fStartY(y)
  , fStartZ(z)
  , fStartT(t)
  , fNPoints(0)
{
  if (nPoints >= 0) fNPoints = nPoints;
  //  else              fNPoints = 0;
}
// -------------------------------------------------------------------------


// -----   Copy constructor   ----------------------------------------------
CbmMCTrack::CbmMCTrack(const CbmMCTrack& track)
  : TObject(track)
  , fProcessId(track.fProcessId)
  , fPdgCode(track.fPdgCode)
  , fMotherId(track.fMotherId)
  , fPx(track.fPx)
  , fPy(track.fPy)
  , fPz(track.fPz)
  , fE(track.GetEnergy())
  , fStartX(track.fStartX)
  , fStartY(track.fStartY)
  , fStartZ(track.fStartZ)
  , fStartT(track.fStartT)
  , fNPoints(track.fNPoints)
{
  //  *this = track;
}
// -------------------------------------------------------------------------


// -----   Constructor from TParticle   ------------------------------------
CbmMCTrack::CbmMCTrack(TParticle* part)
  : TObject()
  , fProcessId(part->GetUniqueID())
  , fPdgCode(part->GetPdgCode())
  , fMotherId(part->GetMother(0))
  , fPx(part->Px())
  , fPy(part->Py())
  , fPz(part->Pz())
  , fE(part->Energy())
  , fStartX(part->Vx())
  , fStartY(part->Vy())
  , fStartZ(part->Vz())
  , fStartT(part->T() * 1e09)
  , fNPoints(0)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMCTrack::~CbmMCTrack() {}
// -------------------------------------------------------------------------


// -----   Public method GetMass   -----------------------------------------
double CbmMCTrack::GetMass() const
{

  if (TDatabasePDG::Instance()) {
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(fPdgCode);

    // Particle found in TDatabasePDG
    if (particle) return particle->Mass();

    // Ions may not be in the TDatabasePDG, but their mass number is encoded
    // in the PDG code like 10LZZZAAAI, where L is strangeness, Z is charge,
    // A is number of nucleons, and I is isomer level.
    else if (fPdgCode > 1000000000) {
      int32_t a = (fPdgCode % 10000) / 10;
      return double(a) * CbmProtonMass();
    }

    // Cherenkov photons
    else if (fPdgCode == 50000050)
      return 0.;

    // Unknown particle type
    else {
      LOG(error) << "CbmMCTrack: Unknown PDG code " << fPdgCode;
      return 0.;
    }
  }  //? Instance of TDatabasePDG

  LOG(fatal) << "CbmMCTrack: No TDatabasePDG";
  return -1.;
}
// -------------------------------------------------------------------------


// -----   Public method GetCharge   ---------------------------------------
double CbmMCTrack::GetCharge() const
{

  if (TDatabasePDG::Instance()) {
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(fPdgCode);

    // Particle found in TDatabasePDG
    if (particle) return particle->Charge();

    // Ions may not be in the TDatabasePDG, but their charge number is encoded
    // in the PDG code like 10LZZZAAAI, where L is strangeness, Z is charge,
    // A is number of nucleons, and I is isomer level.
    else if (fPdgCode > 1000000000) {
      return double((fPdgCode % 10000000) / 10000);
    }

    // Cherenkov photons
    else if (fPdgCode == 50000050)
      return 0.;

    // Unknown particle type
    else {
      LOG(error) << "CbmMCTrack: Unknown PDG code " << fPdgCode;
      return 0.;
    }
  }  //? Instance of TDatabasePDG

  LOG(fatal) << "CbmMCTrack: No TDatabasePDG";
  return 0.;
}
// -------------------------------------------------------------------------


// -----   Public method GetRapidity   -------------------------------------
double CbmMCTrack::GetRapidity() const
{
  double e = GetEnergy();
  double y = 0.5 * log((e + fPz) / (e - fPz));
  return y;
}
// -------------------------------------------------------------------------


// -----   Public method GetNPoints   --------------------------------------
int32_t CbmMCTrack::GetNPoints(ECbmModuleId detId) const
{
  if (detId == ECbmModuleId::kRef) return (fNPoints & 1);
  else if (detId == ECbmModuleId::kMvd)
    return ((fNPoints & (7 << 1)) >> 1);
  else if (detId == ECbmModuleId::kSts)
    return ((fNPoints & (31 << 4)) >> 4);
  else if (detId == ECbmModuleId::kRich)
    return ((fNPoints & (1 << 9)) >> 9);
  else if (detId == ECbmModuleId::kMuch)
    return ((fNPoints & (31 << 10)) >> 10);
  else if (detId == ECbmModuleId::kTrd)
    return ((fNPoints & (31 << 15)) >> 15);
  else if (detId == ECbmModuleId::kTof)
    return ((fNPoints & (15 << 20)) >> 20);
  else if (detId == ECbmModuleId::kEcal)
    return ((fNPoints & (1 << 24)) >> 24);
  else if (detId == ECbmModuleId::kPsd)
    return ((fNPoints & (1 << 25)) >> 25);
  else {
    LOG(error) << "GetNPoints: Unknown detector ID " << detId;
    return 0;
  }
}
// -------------------------------------------------------------------------


// -----   Public method SetNPoints   --------------------------------------
void CbmMCTrack::SetNPoints(ECbmModuleId iDet, int32_t nPoints)
{

  if (iDet == ECbmModuleId::kRef) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 1)
      nPoints = 1;
    fNPoints = (fNPoints & (~1)) | nPoints;
  }

  else if (iDet == ECbmModuleId::kMvd) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 7)
      nPoints = 7;
    fNPoints = (fNPoints & (~(7 << 1))) | (nPoints << 1);
  }

  else if (iDet == ECbmModuleId::kSts) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 31)
      nPoints = 31;
    fNPoints = (fNPoints & (~(31 << 4))) | (nPoints << 4);
  }

  else if (iDet == ECbmModuleId::kRich) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 1)
      nPoints = 1;
    fNPoints = (fNPoints & (~(1 << 9))) | (nPoints << 9);
  }

  else if (iDet == ECbmModuleId::kMuch) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 31)
      nPoints = 31;
    fNPoints = (fNPoints & (~(31 << 10))) | (nPoints << 10);
  }

  else if (iDet == ECbmModuleId::kTrd) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 31)
      nPoints = 31;
    fNPoints = (fNPoints & (~(31 << 15))) | (nPoints << 15);
  }

  else if (iDet == ECbmModuleId::kTof) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 15)
      nPoints = 15;
    fNPoints = (fNPoints & (~(15 << 20))) | (nPoints << 20);
  }

  else if (iDet == ECbmModuleId::kEcal) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 1)
      nPoints = 1;
    fNPoints = (fNPoints & (~(1 << 24))) | (nPoints << 24);
  }

  else if (iDet == ECbmModuleId::kPsd) {
    if (nPoints < 0) nPoints = 0;
    else if (nPoints > 1)
      nPoints = 1;
    fNPoints = (fNPoints & (~(1 << 25))) | (nPoints << 25);
  }

  else
    LOG(error) << "Unknown detector ID " << iDet;
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
std::string CbmMCTrack::ToString() const
{
  stringstream ss;
  ss << "MCTrack: mother  " << fMotherId << ", GeantProcess " << TMCProcessName[fProcessId] << ", Type " << fPdgCode
     << ", momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << std::endl;
  ss << "       Ref " << GetNPoints(ECbmModuleId::kRef) << ", MVD " << GetNPoints(ECbmModuleId::kMvd) << ", STS "
     << GetNPoints(ECbmModuleId::kSts) << ", RICH " << GetNPoints(ECbmModuleId::kRich) << ", MUCH "
     << GetNPoints(ECbmModuleId::kMuch) << ", TRD " << GetNPoints(ECbmModuleId::kTrd) << ", TOF "
     << GetNPoints(ECbmModuleId::kTof) << ", ECAL " << GetNPoints(ECbmModuleId::kEcal) << ", PSD "
     << GetNPoints(ECbmModuleId::kPsd) << std::endl;
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmMCTrack)
