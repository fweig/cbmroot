/** @file GeoReader.cxx
  * @copyright Copyright (C) 2017-2020 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "GeoReader.h"

#include "TClonesArray.h"
#include "TGeoBBox.h"
#include "TMath.h"
#include "Tracker.h"

#include "Station3D.h"
#include "Station4D.h"

using namespace std;

CbmBinnedGeoReader* CbmBinnedGeoReader::Instance()
{
  static CbmBinnedGeoReader* theInstance = 0;

  if (0 != theInstance) return theInstance;

  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) return 0;

  theInstance = new CbmBinnedGeoReader(ioman, CbmBinnedTracker::Instance());
  return theInstance;
}

CbmBinnedGeoReader::CbmBinnedGeoReader(FairRootManager*, CbmBinnedTracker* tracker)
  : fNavigator(0)
  , fDetectorReaders()
  , fTracker(tracker)
  , fLastStationNumber(0)
{
  Int_t nofChildren = gGeoManager->GetTopNode()->GetNdaughters();

  for (Int_t i = 0; i < nofChildren; ++i) {
    TGeoNode* child = gGeoManager->GetTopNode()->GetDaughter(i);
    TString childName(child->GetName());
    cout << "Have detecting system: " << childName.Data() << endl;
  }

  fNavigator                  = gGeoManager->GetCurrentNavigator();
  CbmBinnedSettings* settings = CbmBinnedSettings::Instance();

  if (settings->Use(ECbmModuleId::kSts)) {
    fDetectorReaders["sts"] = &CbmBinnedGeoReader::ReadSts;

    if (settings->IsConfiguring()) CbmBinnedHitReader::AddReader("stsmc");
    else
      CbmBinnedHitReader::AddReader("sts");
  }

  if (settings->Use(ECbmModuleId::kRich)) {
    fDetectorReaders["rich"] = &CbmBinnedGeoReader::ReadRich;

    if (settings->IsConfiguring()) CbmBinnedHitReader::AddReader("richmc");
    else
      CbmBinnedHitReader::AddReader("rich");
  }

  if (settings->Use(ECbmModuleId::kMuch)) {
    fDetectorReaders["much"] = &CbmBinnedGeoReader::ReadMuch;

    if (settings->IsConfiguring()) CbmBinnedHitReader::AddReader("muchmc");
    else
      CbmBinnedHitReader::AddReader("much");
  }

  if (settings->Use(ECbmModuleId::kTrd)) {
    fDetectorReaders["trd"] = &CbmBinnedGeoReader::ReadTrd;

    if (settings->IsConfiguring()) CbmBinnedHitReader::AddReader("trdmc");
    else
      CbmBinnedHitReader::AddReader("trd");
  }

  if (settings->Use(ECbmModuleId::kTof)) {
    fDetectorReaders["tof"] = &CbmBinnedGeoReader::ReadTof;

    if (settings->IsConfiguring()) CbmBinnedHitReader::AddReader("tofmc");
    else
      CbmBinnedHitReader::AddReader("tof");
  }
}

void CbmBinnedGeoReader::FindGeoChild(TGeoNode* node, const char* name, list<TGeoNode*>& results)
{
  Int_t nofChildren = node->GetNdaughters();

  for (Int_t i = 0; i < nofChildren; ++i) {
    TGeoNode* child = node->GetDaughter(i);
    TString childName(child->GetName());

    if (childName.Contains(name, TString::kIgnoreCase)) results.push_back(child);
  }
}

Double_t cbmBinnedSOL = 0;

void CbmBinnedGeoReader::Read()
{
  CbmBinnedSettings* settings = CbmBinnedSettings::Instance();
  CbmBinnedHitReader::SetSettings(settings);

  if (settings->Use(ECbmModuleId::kSts)) ReadDetector("sts");

  if (settings->Use(ECbmModuleId::kRich)) ReadDetector("rich");

  if (settings->Use(ECbmModuleId::kMuch)) ReadDetector("much");

  if (settings->Use(ECbmModuleId::kTrd)) ReadDetector("trd");

  if (settings->Use(ECbmModuleId::kTof)) ReadDetector("tof");

  ReadTarget();
  cbmBinnedSOL = 1.e-7 * TMath::C();  // Speed of light in cm/ns
}

void CbmBinnedGeoReader::ReadDetector(const char* name) { (this->*fDetectorReaders[name])(); }

static const int gNofZbins = 5;
static const int gNofYbins = 20;
static const int gNofXbins = 20;
static const int gNofTbins = 1;

void CbmBinnedGeoReader::SearchStation(ECbmModuleId stationType, TGeoNode* node, CbmBinnedHitReader* hitReader,
                                       list<const char*>::const_iterator stationPath,
                                       list<const char*>::const_iterator stationPathEnd,
                                       const std::list<const char*>& geoPath, bool is4d)
{
  if (stationPath == stationPathEnd) {
    //Double_t localCoords[3] = { 0, 0, 0 };
    //Double_t globalCoords[3];
    //gGeoManager->LocalToMaster(localCoords, globalCoords);
    //Double_t z = globalCoords[2];
    //const char* name1 = node->GetName();
    //const char* name2 = gGeoManager->GetCurrentNode()->GetName();
    Double_t left   = 10000;
    Double_t right  = -10000;
    Double_t top    = -10000;
    Double_t bottom = 10000;
    Double_t front  = 10000;
    Double_t back   = -10000;
    HandleStation(node, geoPath.begin(), geoPath.end(), left, right, top, bottom, front, back);
    CbmBinnedStation* station = 0;

    if (is4d) {
      CbmBinned4DStation* station4d =
        new CbmBinned4DStation(stationType, front, back, gNofZbins, gNofYbins, gNofXbins, gNofTbins);
      station = station4d;
    }
    else {
      CbmBinned3DStation* station3d = new CbmBinned3DStation(stationType, front, back, gNofYbins, gNofXbins, gNofTbins);
      station                       = station3d;
    }

    station->SetMinY(bottom);
    station->SetMaxY(top);
    station->SetMinX(left);
    station->SetMaxX(right);
    CbmBinnedSettings* settings = CbmBinnedSettings::Instance();

    if (!settings->IsConfiguring()) {
      Double_t xScat = settings->GetXScat(fLastStationNumber);
      station->SetScatX(xScat);
      Double_t yScat = settings->GetYScat(fLastStationNumber);
      station->SetScatY(yScat);
    }

    /*if (!CbmBinnedSettings::Instance()->IsConfiguring())
      {
         station->SetDx(CbmBinnedSettings::Instance()->GetXError(fLastStationNumber));
         station->SetDy(CbmBinnedSettings::Instance()->GetYError(fLastStationNumber));
         station->SetDt(CbmBinnedSettings::Instance()->GetTError(fLastStationNumber));
      }*/

    //station->SetScatX(gStationScats[fLastStationNumber][0]);
    //station->SetScatY(gStationScats[fLastStationNumber][1]);

    //station->SetNofSigmaX(gStationNofSigmas[stationNumber][0]);
    //station->SetNofSigmaY(gStationNofSigmas[stationNumber][1]);

    station->Init();
    fTracker->AddStation(station);
    hitReader->AddStation(station);
    ++fLastStationNumber;
    settings->SetNofStations(fLastStationNumber);
  }
  else {
    const char* name = *stationPath++;
    list<TGeoNode*> subNodes;
    FindGeoChild(node, name, subNodes);

    for (list<TGeoNode*>::iterator i = subNodes.begin(); i != subNodes.end(); ++i) {
      fNavigator->CdDown(*i);
      SearchStation(stationType, *i, hitReader, stationPath, stationPathEnd, geoPath);
      fNavigator->CdUp();
    }
  }
}

void CbmBinnedGeoReader::HandleStation(TGeoNode* node, list<const char*>::const_iterator geoPath,
                                       list<const char*>::const_iterator geoPathEnd, Double_t& left, Double_t& right,
                                       Double_t& top, Double_t& bottom, Double_t& front, Double_t& back)
{
  if (geoPath == geoPathEnd) HandleActive(node, left, right, top, bottom, front, back);
  else {
    const char* name = *geoPath++;
    list<TGeoNode*> subNodes;
    FindGeoChild(node, name, subNodes);

    for (list<TGeoNode*>::iterator i = subNodes.begin(); i != subNodes.end(); ++i) {
      fNavigator->CdDown(*i);
      HandleStation(*i, geoPath, geoPathEnd, left, right, top, bottom, front, back);
      fNavigator->CdUp();
    }
  }
}

void CbmBinnedGeoReader::HandleActive(TGeoNode* node, Double_t& left, Double_t& right, Double_t& top, Double_t& bottom,
                                      Double_t& front, Double_t& back)
{
  TGeoBBox* pBox = static_cast<TGeoBBox*>(node->GetVolume()->GetShape());
  pBox->ComputeBBox();

  for (int i = -1; i <= 1; i += 2) {
    for (int j = -1; j <= 1; j += 2) {
      for (int k = -1; k <= 1; k += 2) {
        Double_t localCoords[3] = {i * pBox->GetDX(), j * pBox->GetDY(), k * pBox->GetDZ()};
        Double_t globalCoords[3];
        gGeoManager->LocalToMaster(localCoords, globalCoords);

        if (left > globalCoords[0]) left = globalCoords[0];

        if (right < globalCoords[0]) right = globalCoords[0];

        if (bottom > globalCoords[1]) bottom = globalCoords[1];

        if (top < globalCoords[1]) top = globalCoords[1];

        if (front > globalCoords[2]) front = globalCoords[2];

        if (back < globalCoords[2]) back = globalCoords[2];
      }
    }
  }
}

void CbmBinnedGeoReader::ReadSts()
{
  list<const char*> stationPath = {"sts", "Station"};
  list<const char*> geoPath     = {"Ladder", "HalfLadder", "Module", "Sensor"};
  gGeoManager->cd("/cave_1");
  int firstStationNumber = fLastStationNumber;
  const char* readerName = CbmBinnedSettings::Instance()->IsConfiguring() ? "stsmc" : "sts";
  SearchStation(ECbmModuleId::kSts, gGeoManager->GetCurrentNode(), CbmBinnedHitReader::Instance(readerName),
                stationPath.begin(), stationPath.end(), geoPath);
  CbmBinnedSettings::Instance()->SetNofStsStations(fLastStationNumber - firstStationNumber);
}

void CbmBinnedGeoReader::ReadRich()
{
  list<const char*> stationPath = {"rich"};
  list<const char*> geoPath     = {"rich_smallprototype", "Box", "Gas", "PmtContainer", "Pmt", "pmt_pixel"};
  gGeoManager->cd("/cave_1");
  const char* readerName = CbmBinnedSettings::Instance()->IsConfiguring() ? "richmc" : "rich";
  SearchStation(ECbmModuleId::kRich, gGeoManager->GetCurrentNode(), CbmBinnedHitReader::Instance(readerName),
                stationPath.begin(), stationPath.end(), geoPath);
}

void CbmBinnedGeoReader::ReadMuch()
{
  list<const char*> stationPath = {"much", "station", "muchstation", "layer"};
  list<const char*> geoPath     = {"active"};
  gGeoManager->cd("/cave_1");
  int firstStationNumber = fLastStationNumber;
  const char* readerName = CbmBinnedSettings::Instance()->IsConfiguring() ? "muchmc" : "much";
  SearchStation(ECbmModuleId::kMuch, gGeoManager->GetCurrentNode(), CbmBinnedHitReader::Instance(readerName),
                stationPath.begin(), stationPath.end(), geoPath);
  CbmBinnedSettings::Instance()->SetNofMuchStations(fLastStationNumber - firstStationNumber);
}

void CbmBinnedGeoReader::ReadTrd()
{
  list<const char*> stationPath = {"trd", "layer"};
  list<const char*> geoPath     = {"module", "gas"};
  gGeoManager->cd("/cave_1");
  int firstStationNumber = fLastStationNumber;
  const char* readerName = CbmBinnedSettings::Instance()->IsConfiguring() ? "trdmc" : "trd";
  SearchStation(ECbmModuleId::kTrd, gGeoManager->GetCurrentNode(), CbmBinnedHitReader::Instance(readerName),
                stationPath.begin(), stationPath.end(), geoPath);
  CbmBinnedSettings::Instance()->SetNofTrdStations(fLastStationNumber - firstStationNumber);
}

void CbmBinnedGeoReader::ReadTof()
{
  list<const char*> stationPath = {"tof"};
  list<const char*> geoPath     = {"module", "gas_box", "counter"};
  gGeoManager->cd("/cave_1");
  const char* readerName = CbmBinnedSettings::Instance()->IsConfiguring() ? "tofmc" : "tof";
  SearchStation(ECbmModuleId::kTof, gGeoManager->GetCurrentNode(), CbmBinnedHitReader::Instance(readerName),
                stationPath.begin(), stationPath.end(), geoPath, true);
}

void CbmBinnedGeoReader::ReadTarget()
{
  gGeoManager->cd("/cave_1");
  list<TGeoNode*> pipeNodes;
  FindGeoChild(gGeoManager->GetCurrentNode(), "pipe", pipeNodes);

  for (list<TGeoNode*>::iterator i = pipeNodes.begin(); i != pipeNodes.end(); ++i) {
    TGeoNode* pipeNode = *i;
    fNavigator->CdDown(pipeNode);
    list<TGeoNode*> pipeVacNodes;
    FindGeoChild(pipeNode, "pipevac", pipeVacNodes);

    for (list<TGeoNode*>::iterator j = pipeVacNodes.begin(); j != pipeVacNodes.end(); ++j) {
      TGeoNode* pipeVacNode = *j;
      fNavigator->CdDown(pipeVacNode);
      list<TGeoNode*> targetNodes;
      FindGeoChild(pipeVacNode, "target", targetNodes);

      for (list<TGeoNode*>::iterator k = targetNodes.begin(); k != targetNodes.end(); ++k) {
        TGeoNode* targetNode = *k;
        fNavigator->CdDown(targetNode);
        Double_t localCoords[3] = {0, 0, 0};
        Double_t globalCoords[3];
        gGeoManager->LocalToMaster(localCoords, globalCoords);
        //Double_t targetX = globalCoords[0]; (VF) unused
        //Double_t targetY = globalCoords[1]; (VF) unused
        //Double_t targetZ = globalCoords[2]; (VF) unused
        TGeoBBox* targetBox = static_cast<TGeoBBox*>(targetNode->GetVolume()->GetShape());
        targetBox->ComputeBBox();
        //Double_t targetDx = targetBox->GetDX(); (VF) unused
        //Double_t targetDy = targetBox->GetDY(); (VF) unused
        //Double_t targetDz = targetBox->GetDZ(); (VF) unused
        fNavigator->CdUp();
      }

      fNavigator->CdUp();
    }

    fNavigator->CdUp();
  }

  list<const char*> stsTest = {"sts", "station", "ladder", "halfladder", "module", "sensor"};
  list<TGeoNode*> stsTestResults;
  FindNode(stsTest, stsTestResults);
  //int nofStsSensors = stsTestResults.size(); (VF) unused

  /*
   for (list<TGeoNode*>::iterator i = stsTestResults.begin(); i != stsTestResults.end(); ++i)
   {
      //TGeoNode* node = *i; (VF) unused
      //const char* name = node->GetName(); (VF) unused
      //TGeoVolume* vol = node->GetVolume(); (VF) unused
   }
*/
}

void CbmBinnedGeoReader::FindNode(list<const char*> nodePath, list<TGeoNode*>& results, TGeoNode* rootNode)
{
  if (0 == rootNode) {
    rootNode = gGeoManager->GetTopNode();
    gGeoManager->CdTop();
  }

  results.push_back(rootNode);

  for (list<const char*>::const_iterator i = nodePath.begin(); i != nodePath.end(); ++i) {
    const char* name = *i;
    list<TGeoNode*> work;

    for (list<TGeoNode*>::iterator j = results.begin(); j != results.end(); ++j) {
      TGeoNode* node = *j;
      FindGeoChild(node, name, work);
    }

    results.clear();
    results.splice(results.begin(), work);
  }
}
