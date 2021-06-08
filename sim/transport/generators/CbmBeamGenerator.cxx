/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmBeamGenerator.h
 ** @author V. Friese <v.friese@gsi.de>
 ** @since 8 September 2020
 **/


#include "CbmBeamGenerator.h"

#include "CbmEventGenerator.h"

#include "FairLogger.h"
#include "FairMCEventHeader.h"
#include "FairPrimaryGenerator.h"
#include "FairRunSim.h"

#include "TFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TVector3.h"
#include <TDatabasePDG.h>
#include <TParticlePDG.h>

#include <cassert>
#include <sstream>

#include "UEvent.h"
#include "UParticle.h"
#include "URun.h"


// -----   Default constructor   --------------------------------------------
CbmBeamGenerator::CbmBeamGenerator()
  : FairGenerator("BeamGenerator", "CBM generator")
  , fP(0.)
  , fStartZ(0.)
  , fIon(nullptr)
{
}
// --------------------------------------------------------------------------


// -----   Standard constructor   --------------------------------------------
CbmBeamGenerator::CbmBeamGenerator(UInt_t beamZ, UInt_t beamA, UInt_t beamQ, Double_t momentum, Double_t startZ)
  : FairGenerator("BeamGenerator", "CBM generator")
  , fP(momentum * Double_t(beamA))
  , fStartZ(startZ)
  , fIon(nullptr)
{

  // --- Create the ion species and add it to the particle list
  char name[20];
  sprintf(name, "Beam_%d_%d_%d", beamZ, beamA, beamQ);
  fIon            = new FairIon(name, beamZ, beamA, beamQ);
  FairRunSim* run = FairRunSim::Instance();
  assert(run);
  run->AddNewIon(fIon);

  // --- Tell the event generator to force the vertex at the given z
  FairPrimaryGenerator* primGen = run->GetPrimaryGenerator();
  CbmEventGenerator* evGen      = dynamic_cast<CbmEventGenerator*>(primGen);
  assert(evGen);
  evGen->ForceVertexAtZ(startZ);
  evGen->SmearVertexZ(kFALSE);
}
// --------------------------------------------------------------------------


// -----   Destructor   -----------------------------------------------------
CbmBeamGenerator::~CbmBeamGenerator() {}
// --------------------------------------------------------------------------


// -----   Print info   -----------------------------------------------------
void CbmBeamGenerator::Print(Option_t*) const { LOG(info) << ToString(); }
// --------------------------------------------------------------------------


// -----   Generate input event   -------------------------------------------
Bool_t CbmBeamGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{

  TParticlePDG* ion = TDatabasePDG::Instance()->GetParticle(fIon->GetName());
  assert(ion);

  // Note that the beam position in x and y and the beam angle are generated
  // by CbmEventGenerator. Here, we generate the ion thus in z direction
  // and at zero coordinates. It will be properly placed and rotated
  // according to the beam profile specified to CbmEventGenerator.
  primGen->AddTrack(ion->PdgCode(), 0., 0., fP, 0., 0., 0.);

  return kTRUE;
}
// --------------------------------------------------------------------------


// -----   Info   -----------------------------------------------------------
std::string CbmBeamGenerator::ToString() const
{

  std::stringstream ss;
  ss << GetName() << " ion: " << fIon->GetName() << " Z " << fIon->GetZ() << " A " << fIon->GetA() << " Q "
     << fIon->GetQ() << " mass " << fIon->GetMass() << ", momentum " << fP << " GeV/u, start Z = " << fStartZ;

  return ss.str();
}
// --------------------------------------------------------------------------


ClassImp(CbmBeamGenerator);
