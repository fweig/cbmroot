/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsRecoModule.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 21.03.2020
 **/

#include "CbmStsRecoModule.h"

#include "CbmStsAlgoAnaCluster.h"
#include "CbmStsAlgoFindClusters.h"
#include "CbmStsAlgoFindHits.h"
#include "CbmStsAlgoFindHitsOrtho.h"
#include "CbmStsDigi.h"
#include "CbmStsModule.h"
#include "CbmStsParSensor.h"
#include "CbmStsSensor.h"

#include <FairField.h>
#include <FairRun.h>
#include <Logger.h>

#include <TGeoBBox.h>
#include <TGeoPhysicalNode.h>
#include <TMath.h>
#include <TStopwatch.h>

using std::pair;

ClassImp(CbmStsRecoModule)


  // -----   Standard constructor   ------------------------------------------
  CbmStsRecoModule::CbmStsRecoModule()
{
}
// -------------------------------------------------------------------------


// -----   Default constructor   -------------------------------------------
CbmStsRecoModule::CbmStsRecoModule(CbmStsModule* setupModule, const CbmStsParModule& parModule,
                                   const CbmStsParSensor& parSensor, Double_t lorentzShiftF, Double_t lorentzShiftB)
  : fSetupModule(setupModule)
  , fParModule(&parModule)
  , fParSensor(&parSensor)
  , fLorentzShiftF(lorentzShiftF)
  , fLorentzShiftB(lorentzShiftB)
{
  Init();
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsRecoModule::~CbmStsRecoModule() {}
// -------------------------------------------------------------------------


// ---------------   Add digi to queues   ----------------------------------
void CbmStsRecoModule::AddDigiToQueue(const CbmStsDigi* digi, Int_t digiIndex)
{
  fLock.lock();
  Int_t moduleAddress = CbmStsAddress::GetMotherAddress(digi->GetAddress(), kStsModule);
  assert(moduleAddress == fSetupModule->GetAddress());
  assert(digi->GetChannel() < fNofStripsF + fNofStripsB);
  if (digi->GetChannel() < fNofStripsF) fDigisF.push_back({digi, digiIndex});
  else
    fDigisB.push_back({digi, digiIndex});
  fLock.unlock();
}
// -------------------------------------------------------------------------


// -----   Reconstruction   ------------------------------------------------
void CbmStsRecoModule::Reconstruct()
{
  SortDigis();
  FindClusters();
  SortClusters();
  FindHits();
}

void CbmStsRecoModule::SortDigis()
{
  TStopwatch timer;

  timer.Start();
  // --- Sort the digi queues by digi time stamp
  std::sort(fDigisF.begin(), fDigisF.end(),
            [](pair<const CbmStsDigi*, Int_t> digi1, pair<const CbmStsDigi*, Int_t> digi2) {
              return digi1.first->GetTime() < digi2.first->GetTime();
            });
  std::sort(fDigisB.begin(), fDigisB.end(),
            [](pair<const CbmStsDigi*, Int_t> digi1, pair<const CbmStsDigi*, Int_t> digi2) {
              return digi1.first->GetTime() < digi2.first->GetTime();
            });
  timer.Stop();
  fTimings.timeSortDigi = timer.RealTime();
}

void CbmStsRecoModule::FindClusters()
{
  // --- Perform cluster finding
  TStopwatch timer;

  timer.Start();
  fClusterFinder->Exec(fDigisF, fClustersF, fSetupModule->GetAddress(), fNofStripsF, 0, fTimeCutDigisSig,
                       fTimeCutDigisAbs, fConnectEdgeFront, fParModule);
  fClusterFinder->Exec(fDigisB, fClustersB, fSetupModule->GetAddress(), fNofStripsB, fNofStripsF, fTimeCutDigisSig,
                       fTimeCutDigisAbs, fConnectEdgeBack, fParModule);

  // --- Perform cluster analysis
  for (auto& cluster : fClustersF)
    fClusterAna->Exec(cluster, fParModule);
  for (auto& cluster : fClustersB)
    fClusterAna->Exec(cluster, fParModule);

  timer.Stop();
  fTimings.timeCluster = timer.RealTime();
}

void CbmStsRecoModule::SortClusters()
{
  // --- Sort clusters by time
  TStopwatch timer;

  timer.Start();
  std::sort(fClustersF.begin(), fClustersF.end(), [](const CbmStsCluster& cluster1, const CbmStsCluster& cluster2) {
    return (cluster1.GetTime() < cluster2.GetTime());
  });
  std::sort(fClustersB.begin(), fClustersB.end(), [](const CbmStsCluster& cluster1, const CbmStsCluster& cluster2) {
    return (cluster1.GetTime() < cluster2.GetTime());
  });
  timer.Stop();
  fTimings.timeSortCluster = timer.RealTime();
}

void CbmStsRecoModule::FindHits()
{
  // --- Perform hit finding
  TStopwatch timer;

  timer.Start();
  if (fHitFinder)
    fHitFinder->Exec(fClustersF, fClustersB, fHits, fSetupModule->GetAddress(), fTimeCutClustersSig,
                     fTimeCutClustersAbs, fDyActive, fNofStripsF, fStripPitchF, fStereoFront, fStereoBack,
                     fLorentzShiftF, fLorentzShiftB, fMatrix);
  else if (fHitFinderOrtho)
    fHitFinderOrtho->Exec(fClustersF, fClustersB, fHits, fSetupModule->GetAddress(), fTimeCutClustersSig,
                          fTimeCutClustersAbs, fNofStripsF, fNofStripsB, fStripPitchF, fStripPitchB, fLorentzShiftF,
                          fLorentzShiftB, fMatrix);
  timer.Stop();
  fTimings.timeHits = timer.RealTime();
}

// -------------------------------------------------------------------------


// -----   Reset before new time slice   -----------------------------------
void CbmStsRecoModule::Reset()
{
  fDigisF.clear();
  fDigisB.clear();
}
// -------------------------------------------------------------------------


// -----   Get the sensor parameters   -------------------------------------
void CbmStsRecoModule::Init()
{

  // Reconstruction is currently implemented for double-sided strip
  // sensors (class DssdStereo or DssdOrtho)

  // --- Sensor class must be DssdStereo
  auto type = fParSensor->GetClass();
  assert(type == CbmStsSensorClass::kDssdStereo || type == CbmStsSensorClass::kDssdOrtho);

  // --- Check for physical node of sensor
  assert(fSetupModule);
  assert(fSetupModule->GetNofDaughters() == 1);
  TGeoPhysicalNode* sensorNode = fSetupModule->GetDaughter(0)->GetPnode();
  assert(sensorNode);

  // --- Check consistency of parameters with geometry
  TGeoBBox* shape = dynamic_cast<TGeoBBox*>(sensorNode->GetShape());
  assert(shape);
  assert(TMath::Abs(2. * shape->GetDX() - fParSensor->GetPar(0)) < 0.001);
  assert(TMath::Abs(2. * shape->GetDY() - fParSensor->GetPar(1)) < 0.001);
  assert(TMath::Abs(2. * shape->GetDZ() - fParSensor->GetPar(2)) < 0.001);

  // --- Positioning of sensor in global C.S.
  fMatrix = sensorNode->GetMatrix();

  // --- Number of strips must be the same on both sides
  // --- Number of strips, strip pitch and stereo angle
  fNofStripsF  = fParSensor->GetParInt(4);
  fNofStripsB  = fParSensor->GetParInt(5);
  fStripPitchF = fParSensor->GetPar(6);
  fStripPitchB = fParSensor->GetPar(7);
  fStereoFront = fParSensor->GetPar(8);
  fStereoBack  = fParSensor->GetPar(9);
  assert(fNofStripsF > 0);
  assert(fNofStripsB > 0);
  assert(fStripPitchF > 0.);
  assert(fStripPitchB > 0.);

  // --- For DssdStereo, number of strips and pitch must be the same on both sides
  if (type == CbmStsSensorClass::kDssdStereo) {
    assert(fNofStripsB == fNofStripsF);
    assert(fStripPitchB == fStripPitchF);
  }

  // --- Check consistency with geometric extensions
  if (type == CbmStsSensorClass::kDssdStereo) {
    assert(Double_t(fNofStripsF) * fStripPitchF <= fParSensor->GetPar(0));
    fDyActive = fParSensor->GetPar(3);
    assert(fDyActive <= fParSensor->GetPar(1));
  }
  else if (type == CbmStsSensorClass::kDssdOrtho) {
    assert(Double_t(fNofStripsF) * fStripPitchF <= fParSensor->GetPar(0));
    assert(Double_t(fNofStripsB) * fStripPitchB <= fParSensor->GetPar(1));
  }

  // --- Horizontal cross-connection for non-vanishing stereo angles
  if (fStereoFront > 1.) fConnectEdgeFront = kTRUE;
  if (fStereoBack > 1.) fConnectEdgeBack = kTRUE;

  // Disable cross-connection (needs bug fixing still)
  // TODO: Enable again
  fConnectEdgeFront = kFALSE;
  fConnectEdgeBack  = kFALSE;

  // Algorithms
  fClusterAna    = new CbmStsAlgoAnaCluster();
  fClusterFinder = new CbmStsAlgoFindClusters();
  if (type == CbmStsSensorClass::kDssdStereo) fHitFinder = new CbmStsAlgoFindHits();
  else
    fHitFinderOrtho = new CbmStsAlgoFindHitsOrtho();

  // Name
  fName = fSetupModule->GetName();
}
// -------------------------------------------------------------------------


// -----   Info to string  -------------------------------------------------
std::string CbmStsRecoModule::ToString() const
{
  std::stringstream ss;
  ss << fSetupModule->ToString() << " Strips " << fNofStripsF << " / " << fNofStripsB;
  return ss.str();
}
// -------------------------------------------------------------------------
