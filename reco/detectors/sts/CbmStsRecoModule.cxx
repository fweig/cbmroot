/** @file CbmStsRecoModule.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 21.03.2020
 **/

#include "CbmStsRecoModule.h"

#include <TGeoBBox.h>
#include <TGeoPhysicalNode.h>
#include <TMath.h>

#include <FairField.h>
#include <FairLogger.h>
#include <FairRun.h>

#include "CbmStsAlgoAnaCluster.h"
#include "CbmStsAlgoFindClusters.h"
#include "CbmStsAlgoFindHits.h"
#include "CbmStsDigi.h"
#include "CbmStsModule.h"
#include "CbmStsParSensor.h"
#include "CbmStsSensor.h"

using std::pair;


ClassImp(CbmStsRecoModule)


  // -----   Standard constructor   ------------------------------------------
  CbmStsRecoModule::CbmStsRecoModule() {}
// -------------------------------------------------------------------------


// -----   Default constructor   -------------------------------------------
CbmStsRecoModule::CbmStsRecoModule(CbmStsModule* setupModule,
                                   const CbmStsParModule& parModule,
                                   const CbmStsParSensor& parSensor,
                                   Double_t lorentzShiftF,
                                   Double_t lorentzShiftB)
  : fSetupModule(setupModule)
  , fParModule(&parModule)
  , fParSensor(&parSensor)
  , fLorentzShiftF(lorentzShiftF)
  , fLorentzShiftB(lorentzShiftB) {
  Init();
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsRecoModule::~CbmStsRecoModule() {}
// -------------------------------------------------------------------------


// ---------------   Add digi to queues   ----------------------------------
void CbmStsRecoModule::AddDigiToQueue(const CbmStsDigi* digi, Int_t digiIndex) {
  fLock.lock();
  Int_t moduleAddress =
    CbmStsAddress::GetMotherAddress(digi->GetAddress(), kStsModule);
  assert(moduleAddress == fSetupModule->GetAddress());
  assert(digi->GetChannel() < 2 * fNofStrips);
  if (digi->GetChannel() < fNofStrips)
    fDigisF.push_back({digi, digiIndex});
  else
    fDigisB.push_back({digi, digiIndex});
  fLock.unlock();
}
// -------------------------------------------------------------------------


// -----   Reconstruction   ------------------------------------------------
void CbmStsRecoModule::Reconstruct() {

  // --- Sort the digi queues by digi time stamp
  std::sort(fDigisF.begin(),
            fDigisF.end(),
            [](pair<const CbmStsDigi*, Int_t> digi1,
               pair<const CbmStsDigi*, Int_t> digi2) {
              return digi1.first->GetTime() < digi2.first->GetTime();
            });
  std::sort(fDigisB.begin(),
            fDigisB.end(),
            [](pair<const CbmStsDigi*, Int_t> digi1,
               pair<const CbmStsDigi*, Int_t> digi2) {
              return digi1.first->GetTime() < digi2.first->GetTime();
            });

  // --- Perform cluster finding
  fClusterFinder->Exec(fDigisF,
                       fClustersF,
                       fSetupModule->GetAddress(),
                       fNofStrips,
                       0,
                       fTimeCutDigiSig,
                       fTimeCutDigiAbs,
                       fConnectEdgeFront,
                       fParModule);
  fClusterFinder->Exec(fDigisB,
                       fClustersB,
                       fSetupModule->GetAddress(),
                       fNofStrips,
                       fNofStrips,
                       fTimeCutDigiSig,
                       fTimeCutDigiAbs,
                       fConnectEdgeBack,
                       fParModule);

  // --- Perform cluster analysis
  for (auto& cluster : fClustersF)
    fClusterAna->Exec(cluster, fParModule);
  for (auto& cluster : fClustersB)
    fClusterAna->Exec(cluster, fParModule);

  // --- Sort clusters by time
  std::sort(fClustersF.begin(),
            fClustersF.end(),
            [](const CbmStsCluster& cluster1, const CbmStsCluster& cluster2) {
              return (cluster1.GetTime() < cluster2.GetTime());
            });
  std::sort(fClustersB.begin(),
            fClustersB.end(),
            [](const CbmStsCluster& cluster1, const CbmStsCluster& cluster2) {
              return (cluster1.GetTime() < cluster2.GetTime());
            });

  // --- Perform hit finding
  assert(fHitFinder);
  fHitFinder->Exec(fClustersF,
                   fClustersB,
                   fHits,
                   fSetupModule->GetAddress(),
                   fTimeCutClusterSig,
                   fTimeCutClusterAbs,
                   fDyActive,
                   fNofStrips,
                   fStripPitch,
                   fStereoFront,
                   fStereoBack,
                   fLorentzShiftF,
                   fLorentzShiftB,
                   fMatrix);
}
// -------------------------------------------------------------------------


// -----   Reset before new time slice   -----------------------------------
void CbmStsRecoModule::Reset() {
  fDigisF.clear();
  fDigisB.clear();
}
// -------------------------------------------------------------------------


// -----   Get the sensor parameters   -------------------------------------
void CbmStsRecoModule::Init() {

  // Reconstruction is currently implemented for double-sided strip
  // sensor with the same number of strips and strip pitch on the
  // front and on the back side.

  // --- Sensor class must be DssdStereo
  assert(fParSensor->GetClass() == CbmStsSensorClass::kDssdStereo);

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
  Int_t nStripsF = fParSensor->GetParInt(4);
  Int_t nStripsB = fParSensor->GetParInt(5);
  assert(nStripsF > 0 && nStripsF == nStripsB);
  fNofStrips = nStripsF;

  // --- Strip pitch must be the same on both sides
  fStripPitch = fParSensor->GetPar(6);
  assert(fStripPitch > 0. && fStripPitch == fParSensor->GetPar(7));

  // --- Check that active size in x fits the geometrical extension
  assert(Double_t(fNofStrips) * fStripPitch <= fParSensor->GetPar(0));

  // --- Check that active size in y fits the geometrical extension
  fDyActive = fParSensor->GetPar(3);
  assert(fDyActive <= fParSensor->GetPar(1));

  // --- Horizontal cross-connection for non-vanishing stereo angles
  fStereoFront = fParSensor->GetPar(8);
  fStereoBack  = fParSensor->GetPar(9);
  if (fStereoFront > 1.) fConnectEdgeFront = kTRUE;
  if (fStereoBack > 1.) fConnectEdgeBack = kTRUE;

  // Disable cross-connection (needs bug fixing still)
  // TODO: Enable again
  fConnectEdgeFront = kFALSE;
  fConnectEdgeBack  = kFALSE;

  // Algorithms
  fClusterAna    = new CbmStsAlgoAnaCluster();
  fClusterFinder = new CbmStsAlgoFindClusters();
  fHitFinder     = new CbmStsAlgoFindHits();

  // Name
  fName = fSetupModule->GetName();
}
// -------------------------------------------------------------------------


// -----   Info to string  -------------------------------------------------
std::string CbmStsRecoModule::ToString() const {
  std::stringstream ss;
  ss << fSetupModule->ToString() << " Strips " << fNofStrips;
  return ss.str();
}
// -------------------------------------------------------------------------
