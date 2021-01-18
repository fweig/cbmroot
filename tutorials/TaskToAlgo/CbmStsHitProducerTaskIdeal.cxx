// -------------------------------------------------------------------------
// -----                CbmStsHitProducerTaskIdeal source file             -----
// -----                  Created 10/01/06  by V. Friese               -----
// -------------------------------------------------------------------------
#include <iostream>

#include "TClonesArray.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "CbmStsHit.h"
#include "CbmStsHitProducerTaskIdeal.h"
#include "CbmStsPoint.h"

#include "CbmTrdParSetGas.h"

using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
CbmStsHitProducerTaskIdeal::CbmStsHitProducerTaskIdeal()
  : FairTask("Ideal STS Hit Producer Task")
  , fPointArray(NULL)
  , fHitArray(NULL)
  , fTrdGasPar(NULL) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsHitProducerTaskIdeal::~CbmStsHitProducerTaskIdeal() {}
// -------------------------------------------------------------------------

void CbmStsHitProducerTaskIdeal::SetParContainers() {
  fTrdGasPar = static_cast<CbmTrdParSetGas*>(
    FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetGas"));
}

// -----   Public method Init   --------------------------------------------
InitStatus CbmStsHitProducerTaskIdeal::Init() {

  // Get RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- CbmStsHitProducerTaskIdeal::Init: "
         << "RootManager not instantised!" << endl;
    return kFATAL;
  }

  // Get input array
  fPointArray = (TClonesArray*) ioman->GetObject("StsPoint");
  if (!fPointArray) {
    cout << "-W- CbmStsHitProducerTaskIdeal::Init: "
         << "No STSPoint array!" << endl;
    return kERROR;
  }

  // Create and register output array
  fHitArray = new TClonesArray("CbmStsHit");
  ioman->Register(
    "StsHit", "STS", fHitArray, IsOutputBranchPersistent("StsHit"));


  if (fTrdGasPar) fTrdGasPar->Dump();

  cout << "-I- CbmStsHitProducerTaskIdeal: Intialisation successfull" << endl;


  return kSUCCESS;
}
// -------------------------------------------------------------------------


std::vector<CbmStsPoint>
CbmStsHitProducerTaskIdeal::Convert(TClonesArray* arr) {

  std::vector<CbmStsPoint> vec;
  Int_t entries = arr->GetEntriesFast();
  if (entries > 0) {
    CbmStsPoint* point = static_cast<CbmStsPoint*>(arr->At(0));
    LOG(info) << "Entries in TCA for data type " << point->GetName() << ": "
              << entries;
  }
  for (int i = 0; i < entries; ++i) {
    CbmStsPoint* point = static_cast<CbmStsPoint*>(arr->At(i));
    vec.emplace_back(*point);
  }
  return vec;
}


std::vector<CbmStsHit>
CbmStsHitProducerTaskIdeal::Algo(const std::vector<CbmStsPoint>& pointVect) {
  // Declare some variables
  //  CbmStsPoint* point{nullptr};
  Int_t detID {0};  // Detector ID
  Double_t x {0.};
  Double_t y {0.};
  Double_t z {0.1};      // Position
  Double_t dx {0.0001};  // Position error
  TVector3 pos {};
  TVector3 dpos {};  // Position and error vectors

  std::vector<CbmStsHit> hitVect {};


  //  for(auto point: pointVect) {
  for (unsigned long iPoint = 0; iPoint < pointVect.size(); ++iPoint) {

    // Detector ID
    detID = pointVect.at(iPoint).GetDetectorID();

    // Determine hit position (centre plane of station)
    x = 0.5 * (pointVect.at(iPoint).GetXOut() + pointVect.at(iPoint).GetXIn());
    y = 0.5 * (pointVect.at(iPoint).GetYOut() + pointVect.at(iPoint).GetYIn());
    z = 0.5 * (pointVect.at(iPoint).GetZOut() + pointVect.at(iPoint).GetZIn());

    // Create new hit
    pos.SetXYZ(x, y, z);
    dpos.SetXYZ(dx, dx, 0.);

    hitVect.emplace_back(detID, pos, dpos, 0., iPoint, iPoint, 0., 0.);
  }  // Loop over MCPoints

  return hitVect;
}
// -----   Public method Exec   --------------------------------------------
void CbmStsHitProducerTaskIdeal::Exec(Option_t* /*opt*/) {

  // Reset output array
  if (!fHitArray) Fatal("Exec", "No StsHit array");

  //  fHitArray->Clear();
  fHitArray->Delete();

  if (fTrdGasPar) fTrdGasPar->Print();

  // ConvertToVector
  std::vector<CbmStsPoint> points = Convert(fPointArray);

  // Pass the vector to the algorithm
  // Get the vector with the newly created data objects from the algorithm
  std::vector<CbmStsHit> hits = Algo(points);

  // Fill the content of vector into TCA

  int iPoint = 0;
  for (const auto& hit : hits) {
    new ((*fHitArray)[iPoint]) CbmStsHit(hit);
    iPoint++;
  }


  // Event summary
  cout << "-I- CbmStsHitProducerTaskIdeal: " << points.size() << " StsPoints, "
       << hits.size() << " Hits created." << endl;
}
// -------------------------------------------------------------------------


ClassImp(CbmStsHitProducerTaskIdeal)
