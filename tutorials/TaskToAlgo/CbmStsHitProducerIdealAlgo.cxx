// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmStsHitProducerIdealAlgo                    -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmStsHitProducerIdealAlgo.h"

#include "CbmTrdParSetGas.h"

#include <Logger.h>

#include <TList.h>

// -------------------------------------------------------------------------
CbmStsHitProducerIdealAlgo::CbmStsHitProducerIdealAlgo() : CbmAlgo() {}

CbmStsHitProducerIdealAlgo::~CbmStsHitProducerIdealAlgo()
{
  /// Clear buffers
}

// -------------------------------------------------------------------------
Bool_t CbmStsHitProducerIdealAlgo::Init()
{
  LOG(info) << "Initializing tutorial StsHitProducerIdeal algo";

  return kTRUE;
}

void CbmStsHitProducerIdealAlgo::Reset() {}

void CbmStsHitProducerIdealAlgo::Finish() {}

// -------------------------------------------------------------------------
Bool_t CbmStsHitProducerIdealAlgo::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmStsHitProducerIdealAlgo";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmStsHitProducerIdealAlgo::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmStsHitProducerIdealAlgo";

  fTrdGasPar = static_cast<CbmTrdParSetGas*>(fParCList->FindObject("CbmTrdParSetGas"));
  if (nullptr == fTrdGasPar) return kFALSE;

  fTrdGasPar->Dump();
  Bool_t initOK = InitParameters();

  return initOK;
}

TList* CbmStsHitProducerIdealAlgo::GetParList()
{

  if (nullptr == fParCList) { fParCList = new TList(); }

  fTrdGasPar = new CbmTrdParSetGas("CbmTrdParSetGas");
  fParCList->Add(fTrdGasPar);

  return fParCList;
}

Bool_t CbmStsHitProducerIdealAlgo::InitParameters() { return kTRUE; }
// -------------------------------------------------------------------------

std::vector<CbmStsHit> CbmStsHitProducerIdealAlgo::ProcessInputData(const std::vector<CbmStsPoint>& pointVect)
{
  fTrdGasPar->Print();

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

// -------------------------------------------------------------------------
