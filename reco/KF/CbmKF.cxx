/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Sergey Gorbunov, Denis Bertini [committer] */

#include "CbmKF.h"

#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmKFFieldMath.h"
#include "CbmKFHit.h"
#include "CbmKFMath.h"
#include "CbmMvdTrackingInterface.h"
#include "CbmStsTrackingInterface.h"
#include "CbmTrackingDetectorInterfaceInit.h"

#include "FairBaseParSet.h"
#include "FairField.h"
#include "FairGeoNode.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoNode.h"
#include "TGeoShape.h"
#include "TGeoTube.h"
#include "TGeoVolume.h"
#include "TObjArray.h"
#include "TString.h"

#include <iostream>
#include <list>
#include <map>
#include <vector>

#include <cmath>

using std::cout;
using std::endl;
using std::fabs;
using std::map;
using std::pair;
using std::vector;


ClassImp(CbmKF)

  CbmKF* CbmKF::fInstance = 0;

CbmKF::CbmKF(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , vMaterial()
  ,

  vMvdMaterial()
  , vStsMaterial()
  , vMuchMaterial()
  , vMuchDetectors()
  , vRichMaterial()
  , vTrdMaterial()
  , vTargets()
  , vPipe()
  ,

  vPassiveTube()
  , vPassiveWall()
  , vPassiveBox()
  ,

  MvdStationIDMap()
  , StsStationIDMap()
  , TrdStationIDMap()
  , MuchMCID2StationMap()
  , MuchStation2MCIDMap()
  ,

  fMagneticField(0)
  , fMethod(1)
  , fMaterialID2IndexMap()
{
  if (!fInstance) fInstance = this;
}

CbmKF::~CbmKF() { fInstance = 0; }

void CbmKF::SetParContainers()
{
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  rtdb->getContainer("FairBaseParSet");
  rtdb->getContainer("CbmFieldPar");
}

InitStatus CbmKF::ReInit() { return Init(); }

InitStatus CbmKF::Init()
{
  if (!CbmTrackingDetectorInterfaceInit::Instance()) {
    LOG(fatal)
      << "CbmKF::Init(): CbmTrackingDetectorInterfaceInit instance was not found. Please, add it as a task to your "
         "reco macro right before the KF and L1 tasks and after all the tasks before KF:\n"
      << "\033[1;30mrun->AddTask(new CbmTrackingDetectorInterfaceInit());\033[0m";
  }

  fMagneticField = 0;

  vMvdMaterial.clear();
  vStsMaterial.clear();
  vTrdMaterial.clear();
  vRichMaterial.clear();
  vMuchMaterial.clear();
  vMuchDetectors.clear();
  vPipe.clear();
  vTargets.clear();
  vPassiveTube.clear();
  vPassiveWall.clear();
  vPassiveBox.clear();
  vMaterial.clear();

  StsStationIDMap.clear();
  TrdStationIDMap.clear();
  MvdStationIDMap.clear();
  MuchMCID2StationMap.clear();
  MuchStation2MCIDMap.clear();

  fMaterialID2IndexMap.clear();

  FairRunAna* Run = FairRunAna::Instance();
  //FairRuntimeDb *RunDB = Run->GetRuntimeDb(); (VF) not used

  if (fVerbose) cout << "KALMAN FILTER : === INIT MAGNETIC FIELD ===" << endl;

  fMagneticField = reinterpret_cast<FairField*>(Run->GetField());
  if (fVerbose && fMagneticField) cout << "Magnetic field done" << endl;

  if (!fMagneticField) cout << "No Magnetic Field Found" << endl;


  /** 					*/
  // fill vector of material

  //=== Mvd ===

  CbmDigiManager* digiMan = CbmDigiManager::Instance();
  digiMan->Init();
  Bool_t useMVD = CbmDigiManager::IsPresent(ECbmModuleId::kMvd);
  if (useMVD) {
    auto mvdInterface = CbmMvdTrackingInterface::Instance();

    if (fVerbose) cout << "KALMAN FILTER : === READ MVD MATERIAL ===" << endl;

    int NStations = mvdInterface->GetNtrackingStations();

    for (Int_t ist = 0; ist < NStations; ist++) {
      CbmKFTube tube;

      tube.ID = 1101 + ist;
      //   tube.F = 1.;
      tube.z  = mvdInterface->GetZ(ist);
      tube.dz = mvdInterface->GetThickness(ist);
      // TODO: verify the thickness of MVD stations
      tube.RadLength  = mvdInterface->GetRadLength(ist);
      tube.r          = mvdInterface->GetRmin(ist);
      tube.R          = mvdInterface->GetRmax(ist);
      tube.rr         = tube.r * tube.r;
      tube.RR         = tube.R * tube.R;
      tube.ZThickness = tube.dz;
      tube.ZReference = tube.z;

      vMvdMaterial.push_back(tube);
      MvdStationIDMap.insert(pair<Int_t, Int_t>(tube.ID, ist));

      if (fVerbose)
        cout << " Mvd material ( id, z, dz, r, R, RadL, dz/RadL )= ( " << tube.ID << ", " << tube.z << ", " << tube.dz
             << ", " << tube.r << ", " << tube.R << ", " << tube.RadLength << ", " << tube.dz / tube.RadLength << " )"
             << endl;
    }
  }


  //=== Sts stations ===

  if (fVerbose) cout << "KALMAN FILTER : === READ STS MATERIAL ===" << endl;

  auto stsInterface = CbmStsTrackingInterface::Instance();
  int NStations     = stsInterface->GetNtrackingStations();

  for (Int_t ist = 0; ist < NStations; ist++) {
    CbmKFTube tube;

    tube.ID         = 1000 + ist;
    tube.F          = 1.;
    tube.z          = stsInterface->GetZ(ist);
    tube.dz         = stsInterface->GetThickness(ist);
    tube.RadLength  = stsInterface->GetRadLength(ist);
    tube.r          = stsInterface->GetRmin(ist);
    tube.R          = stsInterface->GetRmax(ist);
    tube.rr         = tube.r * tube.r;
    tube.RR         = tube.R * tube.R;
    tube.ZThickness = tube.dz;
    tube.ZReference = tube.z;

    vStsMaterial.push_back(tube);
    StsStationIDMap.insert(pair<Int_t, Int_t>(tube.ID, ist));

    if (fVerbose)
      cout << " Sts material ( id, z, dz, r, R, RadL, dz/RadL )= ( " << tube.ID << ", " << tube.z << ", " << tube.dz
           << ", " << tube.r << ", " << tube.R << ", " << tube.RadLength << ", " << tube.dz / tube.RadLength << " )"
           << endl;
  }

  // FU 05.03.2020
  // Remove the usage of old ascii parameter containers. Since the
  // containers were empty anyway there is no change of the behaviour
  // The change was tested by Iour Vassiliev (see Redmine Issue #1566)
  // and no difference of the results was seen. This  at least holds for
  // the STS/MVD system.


  // FU 25.05.2020
  // Reimplement initializing the target information. After shifting the
  // target 4 cm upstream it was found that the KF package assumes the
  // target at (0.,0.,0.). In ancient times the target information was
  // initialized from the ascii parameter container. After the target
  // geometry was changed to ROOT geometry the information was not
  // initialized. Probably this never was a problem since the target was at
  // z=0. cm anyway.
  // Now the target information s properly initialized from the ROOT GeoManager.
  // Fixes Redmine issues #1727 and #1718

  GetTargetInfo();


  {
    for (vector<CbmKFTube>::iterator i = vTargets.begin(); i != vTargets.end(); ++i) {
      vMaterial.push_back(&*i);
    }

    for (vector<CbmKFTube>::iterator i = vMvdMaterial.begin(); i != vMvdMaterial.end(); ++i) {
      vMaterial.push_back(&*i);
    }

    for (vector<CbmKFTube>::iterator i = vStsMaterial.begin(); i != vStsMaterial.end(); ++i) {
      vMaterial.push_back(&*i);
    }
    sort(vMaterial.begin(), vMaterial.end(), CbmKFMaterial::comparePDown);
    for (unsigned i = 0; i < vMaterial.size(); i++) {
      fMaterialID2IndexMap.insert(pair<Int_t, Int_t>(vMaterial[i]->ID, i));
    }
  }
  return kSUCCESS;
}

void CbmKF::GetTargetInfo()
{
  // Loop over all nodes till a node with name "target" is found
  // Extract the required infrmation from the node and store it in the
  // proper structure
  // The complete logic depends on the naming convention of the target.
  // If the node doesn't contain the string target the procedure will fail

  CbmKFTube target {};
  target.ID = -111;
  target.F  = 1.;

  TString targetPath;
  TGeoNode* targetNode {nullptr};
  FindTargetNode(targetPath, targetNode);

  if (!targetNode) { LOG(fatal) << "Could not find the target."; }

  Double_t local[3] = {0., 0., 0.};  // target centre, local c.s.
  Double_t global[3];                // target centre, global c.s.
  gGeoManager->cd(targetPath);
  gGeoManager->GetCurrentMatrix()->LocalToMaster(local, global);
  target.x = global[0];
  target.y = global[1];
  target.z = global[2];

  fTargetXYZ[0] = target.x;
  fTargetXYZ[1] = target.y;
  fTargetXYZ[2] = target.z;

  if (fVerbose) {
    cout << "KALMAN FILTER : === READ TARGET MATERIAL ===" << endl;
    cout << " found targed \"" << targetPath << "\" at ( " << target.x << " " << target.y << " " << target.z << " ) "
         << endl;
  }

  TGeoVolume* volume = targetNode->GetVolume();

  TGeoShape* shape = volume->GetShape();
  if (shape->TestShapeBit(TGeoShape::kGeoTube)) {
    target.r  = static_cast<TGeoTube*>(shape)->GetRmin();
    target.R  = static_cast<TGeoTube*>(shape)->GetRmax();
    target.dz = 2. * static_cast<TGeoTube*>(shape)->GetDz();
  }
  else {
    LOG(fatal) << "Only a target of a tube shape is supported";
  }

  TGeoMaterial* material = volume->GetMaterial();
  Double_t radlength     = material->GetRadLen();
  target.RadLength       = radlength;
  target.Fe              = 0.02145;

  target.rr         = target.r * target.r;
  target.RR         = target.R * target.R;
  target.ZThickness = target.dz;
  target.ZReference = target.z;

  vTargets.push_back(target);
  LOG(info) << "Target info: " << target.KFInfo();
}

const std::array<float, 3> CbmKF::GetTargetPosition() { return fTargetXYZ; }

void CbmKF::FindTargetNode(TString& targetPath, TGeoNode*& targetNode)
{
  if (!targetNode) {  // init at the top of the tree
    targetNode = gGeoManager->GetTopNode();
    targetPath = "/" + TString(targetNode->GetName());
  }

  if (TString(targetNode->GetName()).Contains("target")) { return; }

  for (Int_t iNode = 0; iNode < targetNode->GetNdaughters(); iNode++) {
    TGeoNode* newNode = targetNode->GetDaughter(iNode);
    TString newPath   = targetPath + "/" + newNode->GetName();
    FindTargetNode(newPath, newNode);
    if (newNode) {
      targetPath = newPath;
      targetNode = newNode;
      return;
    }
  }
  targetPath = "";
  targetNode = nullptr;
}

Int_t CbmKF::GetMaterialIndex(Int_t uid)
{
  map<Int_t, Int_t>::iterator i = fMaterialID2IndexMap.find(uid);
  if (i != fMaterialID2IndexMap.end()) { return i->second; }
  return -1;
}


Int_t CbmKF::ReadTube(CbmKFTube& tube, FairGeoNode* node)
{

  if (!node) return 1;

  TString name            = node->getName();
  TString Sname           = node->getShapePointer()->GetName();
  FairGeoVector nodeV     = node->getLabTransform()->getTranslation();  //in cm
  FairGeoVector centerV   = node->getCenterPosition().getTranslation();
  TArrayD* P              = node->getParameters();
  Int_t NP                = node->getShapePointer()->getNumParam();
  FairGeoMedium* material = node->getMedium();
  material->calcRadiationLength();

  tube.ID        = node->getMCid();
  tube.RadLength = material->getRadiationLength();
  tube.F         = 1.;

  tube.Fe       = 0.02145;
  TString Mname = material->GetName();
  if (Mname == "MUCHWolfram") { tube.Fe = 0.009029; }
  else if (Mname == "MUCHiron") {
    tube.Fe = 0.02219;
  }
  else if (Mname == "carbon") {
    tube.Fe = 0.08043;
  }

  tube.x = nodeV.X() + centerV.X();
  tube.y = nodeV.Y() + centerV.Y();
  tube.z = nodeV.Z() + centerV.Z();
  /*   
  int np = node->getNumPoints();
  cout<<"points:"<<endl;
  for( int i=0; i<np; i++ ){
    FairGeoVector *v = node->getPoint(i);
    cout<<v->X()<<" "<<v->Y()<<" "<<v->Z()<<endl;
  }
  */
  if (Sname == "TUBS" || Sname == "TUBE") {
    tube.r  = P->At(0);
    tube.R  = P->At(1);
    tube.dz = 2. * P->At(2);
  }
  else if (Sname == "TRAP") {
    tube.r  = 0;
    tube.R  = 1000;
    tube.dz = 2. * P->At(0);
  }
  else if (Sname == "SPHE") {
    tube.r = 0;
    tube.R = 1000;
    tube.z += 0.5 * (P->At(0) + P->At(1));  // inner & outer radius
    tube.dz = (P->At(1) - P->At(0));
  }
  else if (Sname == "PCON") {
    Int_t Nz = (NP - 3) / 3;
    double Z = -100000, R = -100000, z = 100000, r = 100000;
    for (Int_t i = 0; i < Nz; i++) {
      double z1 = P->At(3 + i * 3 + 0);
      double r1 = P->At(3 + i * 3 + 1);
      double R1 = P->At(3 + i * 3 + 2);
      if (r1 < r) r = r1;
      if (R1 > R) R = R1;
      if (z1 < z) z = z1;
      if (z1 > Z) Z = z1;
    }

    tube.r = r;
    tube.R = R;
    tube.z += (z + Z) / 2.;
    tube.dz = (Z - z);
  }
  else if (Sname == "PGON") {
    Int_t Nz = (NP - 4) / 3;
    double Z = -100000, R = -100000, z = 100000, r = 100000;
    for (Int_t i = 0; i < Nz; i++) {
      double z1 = P->At(4 + i * 3 + 0);
      double r1 = P->At(4 + i * 3 + 1);
      double R1 = P->At(4 + i * 3 + 2);
      if (r1 < r) r = r1;
      if (R1 > R) R = R1;
      if (z1 < z) z = z1;
      if (z1 > Z) Z = z1;
    }
    tube.r = r;
    tube.R = R;
    tube.z += (z + Z) / 2.;
    tube.dz = (Z - z);
  }
  else if (Sname == "BOX ") {
    double dx = 2 * P->At(0);
    double dy = 2 * P->At(1);
    double dz = 2 * P->At(2);
    tube.r    = 0;
    tube.R    = TMath::Sqrt(dx * dx + dy * dy);
    tube.dz   = dz;
  }
  else {
    cout << " -E- unknown shape : " << Sname << endl;
    cout << " -E- It does not make sense to go on" << endl;
    cout << " -E- Stop execution here" << endl;
    Fatal("CbmKF::ReadTube", "Unknown Shape");
  }
  tube.rr         = tube.r * tube.r;
  tube.RR         = tube.R * tube.R;
  tube.ZThickness = tube.dz;
  tube.ZReference = tube.z;
  return 0;
}


CbmKFMaterial* CbmKF::ReadPassive(FairGeoNode* node)
{

  if (!node) return 0;

  TString name  = node->getName();
  TString Sname = node->getShapePointer()->GetName();

  FairGeoTransform trans(*node->getLabTransform());
  FairGeoNode* nxt = node;
  while ((nxt = nxt->getMotherNode())) {
    FairGeoTransform* tm = nxt->getLabTransform();
    if (!tm) break;
    trans.transFrom(*tm);
  }

  //FairGeoVector nodeV = node->getLabTransform()->getTranslation(); //in cm
  //FairGeoVector centerV = node->getCenterPosition().getTranslation();

  FairGeoVector nodeV   = trans.getTranslation();  //in cm
  FairGeoVector centerV = nodeV + node->getCenterPosition().getTranslation();

  TArrayD* P              = node->getParameters();
  Int_t NP                = node->getShapePointer()->getNumParam();
  FairGeoMedium* material = node->getMedium();
  material->calcRadiationLength();

  Int_t ID           = node->getMCid();
  Double_t RadLength = material->getRadiationLength();
  // Double_t F = 1.;
  Double_t x0 = centerV.X();
  Double_t y0 = centerV.Y();
  Double_t z0 = centerV.Z();

  CbmKFMaterial* ret = 0;

  if (Sname == "TUBS" || Sname == "TUBE") {
    CbmKFTube tube(ID, x0, y0, z0, 2. * P->At(2), P->At(0), P->At(1), RadLength);
    vPassiveTube.push_back(tube);
    ret = &(vPassiveTube.back());
  }
  else if (Sname == "SPHE") {
    CbmKFTube tube(ID, x0, y0, z0 + 0.5 * (P->At(0) + P->At(1)), (P->At(1) - P->At(0)), 0, 1000, RadLength);
    vPassiveTube.push_back(tube);
    ret = &(vPassiveTube.back());
  }
  else if (Sname == "PCON") {
    Int_t Nz = (NP - 3) / 3;
    double Z = -100000, R = -100000, z = 100000, r = 100000;
    for (Int_t i = 0; i < Nz; i++) {
      double z1 = P->At(3 + i * 3 + 0);
      double r1 = P->At(3 + i * 3 + 1);
      double R1 = P->At(3 + i * 3 + 2);
      if (r1 < r) r = r1;
      if (R1 > R) R = R1;
      if (z1 < z) z = z1;
      if (z1 > Z) Z = z1;
    }
    CbmKFTube tube(ID, x0, y0, z0 + 0.5 * (z + Z), (Z - z), r, R, RadLength);
    vPassiveTube.push_back(tube);
    ret = &(vPassiveTube.back());
  }
  else if (Sname == "PGON") {
    Int_t Nz = (NP - 4) / 3;
    double Z = -100000, R = -100000, z = 100000, r = 100000;
    for (Int_t i = 0; i < Nz; i++) {
      double z1 = P->At(4 + i * 3 + 0);
      double r1 = P->At(4 + i * 3 + 1);
      double R1 = P->At(4 + i * 3 + 2);
      if (r1 < r) r = r1;
      if (R1 > R) R = R1;
      if (z1 < z) z = z1;
      if (z1 > Z) Z = z1;
    }
    CbmKFTube tube(ID, x0, y0, z0 + 0.5 * (z + Z), (Z - z), r, R, RadLength);
    vPassiveTube.push_back(tube);
    ret = &(vPassiveTube.back());
  }
  else if (Sname == "BOX ") {
    CbmKFBox box(ID, x0, y0, z0, 2 * P->At(0), 2 * P->At(1), 2 * P->At(2), RadLength);
    vPassiveBox.push_back(box);
    ret = &(vPassiveBox.back());
  }
  else if (Sname == "TRAP") {
    int np             = node->getNumPoints();
    FairGeoVector vMin = *node->getPoint(0), vMax = vMin;
    for (int i = 0; i < np; i++) {
      FairGeoVector* v = node->getPoint(i);
      for (int j = 0; j < 3; j++) {
        if (vMin(j) > (*v)(j)) (&vMin.X())[j] = (*v)(j);
        if (vMax(j) < (*v)(j)) (&vMax.X())[j] = (*v)(j);
      }
    }
    FairGeoVector v0 = (vMin + vMax);
    v0 *= .5 / 10.;
    FairGeoVector dv = vMax - vMin;
    dv /= 10.;
    CbmKFBox box(ID, x0 + v0(0), y0 + v0(1), z0 + v0(2), dv(0), dv(1), dv(2), RadLength);
    vPassiveBox.push_back(box);
    ret = &(vPassiveBox.back());
  }
  else {
    cout << " -E- unknown shape : " << Sname << endl;
    cout << " -E- It does not make sense to go on" << endl;
    cout << " -E- Stop execution here" << endl;
    Fatal("CbmKF::ReadPassive", "Unknown Shape");
  }
  return ret;
}


Int_t CbmKF::Propagate(Double_t* T, Double_t* C, Double_t z_out, Double_t QP0)
{
  Bool_t err = 0;
  if (fabs(T[5] - z_out) < 1.e-5) return 0;

  if (!fMagneticField || (300 <= z_out && 300 <= T[5])) {
    CbmKFFieldMath::ExtrapolateLine(T, C, T, C, z_out);
    return 0;
  }
  Double_t zz = z_out;
  if (z_out < 300. && 300 <= T[5]) CbmKFFieldMath::ExtrapolateLine(T, C, T, C, 300.);

  if (T[5] < 300. && 300. < z_out) { zz = 300.; }
  Bool_t repeat = 1;
  while (!err && repeat) {
    const Double_t max_step = 5.;
    Double_t zzz;
    if (fabs(T[5] - zz) > max_step) zzz = T[5] + ((zz > T[5]) ? max_step : -max_step);
    else {
      zzz    = zz;
      repeat = 0;
    }
    switch (fMethod) {
      case 0: {
        CbmKFFieldMath::ExtrapolateLine(T, C, T, C, zzz);
        break;
      }
      case 1: {
        err = err || CbmKFFieldMath::ExtrapolateALight(T, C, T, C, zzz, QP0, fMagneticField);
        break;
      }
      case 2: {
        err = err || CbmKFFieldMath::ExtrapolateRK4(T, C, T, C, zzz, QP0, fMagneticField);
        break;
      }
        /*
	case 3:
	  { 
	    CbmKFFieldMath::ExtrapolateACentral( T, C, T, C, zzz , QP0, fMagneticField );
	    break;
	  }
	case 4:
	  { 
	    CbmKFFieldMath::ExtrapolateAnalytic( T, C, T, C, zzz , QP0, fMagneticField, 1 );
	    break;
	  }
	case 5:
	  { 
	    CbmKFFieldMath::ExtrapolateAnalytic( T, C, T, C, zzz , QP0, fMagneticField, 2 );
	    break;
	  }
	case 6:
	  { 
	    CbmKFFieldMath::ExtrapolateAnalytic( T, C, T, C, zzz , QP0, fMagneticField, 3 );
	    break;
	  }
	  */
    }
  }
  if (T[5] != z_out) CbmKFFieldMath::ExtrapolateLine(T, C, T, C, z_out);
  return err;
}

Int_t CbmKF::PassMaterial(CbmKFTrackInterface& track, Double_t& QP0, Int_t ifst, Int_t ilst)
{
  Bool_t downstream = (ilst > ifst);
  Bool_t err        = 0;
  Int_t iend        = downstream ? ilst + 1 : ilst - 1;
  for (Int_t i = ifst; i != iend; downstream ? ++i : --i) {
    err = err || vMaterial[i]->Pass(track, downstream, QP0);
  }
  return err;
}

Int_t CbmKF::PassMaterialBetween(CbmKFTrackInterface& track, Double_t& QP0, Int_t ifst, Int_t ilst)
{
  Bool_t downstream = (ilst > ifst);
  Bool_t err        = 0;
  Int_t istart      = downstream ? ifst + 1 : ifst - 1;
  for (Int_t i = istart; i != ilst; downstream ? ++i : --i) {
    err = err || vMaterial[i]->Pass(track, downstream, QP0);
  }
  return err;
}

Int_t CbmKF::PassMaterialBetween(CbmKFTrackInterface& track, Double_t& QP0, CbmKFHit* fst, CbmKFHit* lst)
{
  return PassMaterialBetween(track, QP0, fst->MaterialIndex, lst->MaterialIndex);
}
