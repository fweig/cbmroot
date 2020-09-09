#include "GenNoiseElectrons.h"
#include "CbmMCTrack.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPoint.h"
#include "CbmTrdPoint.h"
#include <Logger.h>
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TH1F.h"
#include "TRandom3.h"
#include <TFile.h>

ClassImp(LxGenNoiseElectrons)

  LxGenNoiseElectrons::LxGenNoiseElectrons()
  : fNofNoiseE(1)
  , fNofStations(4)
  , fMCTracks(0)
  , fMuchPoints(0)
  , fTrdPoints(0)
  , fOutMCTracks(0)
  , fOutMuchPoints(0)
  , fOutTrdPoints(0) {}

LxGenNoiseElectrons::~LxGenNoiseElectrons() {
  delete fOutMCTracks;
  delete fOutMuchPoints;
  delete fOutTrdPoints;
}

static Double_t x_rmss[4][3];
static Double_t y_rmss[4][3];

static Double_t GetRMS(const char* name) {
  Double_t result = -1.;
  char fileName[128];
  sprintf(fileName, "%s.root", name);
  TFile* curFile = TFile::CurrentFile();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* f       = new TFile(fileName);

  if (!f->IsZombie()) {
    TH1F* h = static_cast<TH1F*>(f->Get(name));

    if (0 != h) result = h->GetRMS();
  }

  delete f;
  TFile::CurrentFile() = curFile;

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  return result;
}

InitStatus LxGenNoiseElectrons::Init() {
  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  fMCTracks   = static_cast<TClonesArray*>(ioman->GetObject("MCTrack"));
  fMuchPoints = static_cast<TClonesArray*>(ioman->GetObject("MuchPoint"));
  fTrdPoints  = static_cast<TClonesArray*>(ioman->GetObject("TrdPoint"));

  if (0 == fMCTracks || 0 == fMuchPoints || 0 == fTrdPoints)
    LOG(fatal) << "No MC tracks or points";

  char name[128];

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 3; ++j) {
      sprintf(name, "noise_e_x_%d_%d", i, j);
      x_rmss[i][j] = GetRMS(name);

      if (x_rmss[i][j] < 0) {
        sprintf(name, "Couldn't read noise_e_x_%d_%d", i, j);
        LOG(fatal) << name;
      }

      sprintf(name, "noise_e_y_%d_%d", i, j);
      y_rmss[i][j] = GetRMS(name);

      if (y_rmss[i][j] < 0) {
        sprintf(name, "Couldn't read noise_e_y_%d_%d", i, j);
        LOG(fatal) << name;
      }
    }
  }

  time_t initTime;
  gRandom->SetSeed(time(&initTime));

  fOutMCTracks = new TClonesArray("CbmMCTrack", 1000);
  ioman->Register("MCTrack", "MC", fOutMCTracks, kTRUE);
  fOutMuchPoints = new TClonesArray("CbmMuchPoint", 1000);
  ioman->Register("MuchPoint", "MUCH", fOutMuchPoints, kTRUE);
  fOutTrdPoints = new TClonesArray("CbmTrdPoint", 1000);
  ioman->Register("TrdPoint", "TRD", fOutTrdPoints, kTRUE);

  return kSUCCESS;
}

void LxGenNoiseElectrons::Exec(Option_t* /*opt*/) {
  fOutMCTracks->Delete();
  fOutMuchPoints->Delete();
  fOutTrdPoints->Delete();

  Int_t nofMCTracks    = fMCTracks->GetEntriesFast();
  TClonesArray& mctref = *fOutMCTracks;

  for (Int_t i = 0; i < nofMCTracks; ++i) {
    const CbmMCTrack* mcTrack =
      static_cast<const CbmMCTrack*>(fMCTracks->At(i));
    new (mctref[i]) CbmMCTrack(*mcTrack);
  }

  Int_t nofMuchPoints = fMuchPoints->GetEntriesFast();
  TClonesArray& mpref = *fOutMuchPoints;

  for (Int_t i = 0; i < nofMuchPoints; ++i) {
    const CbmMuchPoint* muchPoint =
      static_cast<const CbmMuchPoint*>(fMuchPoints->At(i));
    new (mpref[i]) CbmMuchPoint(*muchPoint);
  }

  Int_t nofTrdPoints  = fTrdPoints->GetEntriesFast();
  TClonesArray& tpref = *fOutTrdPoints;

  for (Int_t i = 0; i < nofTrdPoints; ++i) {
    const CbmTrdPoint* trdPoint =
      static_cast<const CbmTrdPoint*>(fTrdPoints->At(i));
    new (tpref[i]) CbmTrdPoint(*trdPoint);
  }

  Int_t ind = nofMuchPoints;

  for (Int_t i = 0; i < nofMuchPoints; ++i) {
    const CbmMuchPoint* pMuchPt =
      static_cast<const CbmMuchPoint*>(fMuchPoints->At(i));
    Int_t stationNumber =
      CbmMuchGeoScheme::GetStationIndex(pMuchPt->GetDetectorId());
    Int_t layerNumber =
      CbmMuchGeoScheme::GetLayerIndex(pMuchPt->GetDetectorId());

    for (int j = 0; j < fNofNoiseE; ++j) {
      TVector3 posIn;
      pMuchPt->PositionIn(posIn);
      TVector3 posOut;
      pMuchPt->PositionOut(posOut);
      TVector3 posDelta = posOut - posIn;

      for (int k = 0; k < 20; ++k) {
        TVector3 noiseDelta(
          gRandom->Gaus(0, x_rmss[stationNumber][layerNumber]),
          gRandom->Gaus(0, y_rmss[stationNumber][layerNumber]),
          0);
        TVector3 newIn = posIn + noiseDelta;
        TGeoNode* node = gGeoManager->FindNode(newIn.X(), newIn.Y(), newIn.Z());

        if (0 == node) continue;

        TString nodeName(node->GetName());

        if (!nodeName.Contains("active", TString::kIgnoreCase)) continue;

        TVector3 newOut = newIn + 0.5 * posDelta;
        node = gGeoManager->FindNode(newOut.X(), newOut.Y(), newOut.Z());

        if (0 == node) continue;

        TString nodeName2(node->GetName());

        if (!nodeName2.Contains("active", TString::kIgnoreCase)) continue;

        CbmMuchPoint noiseMuchPt(*pMuchPt);
        noiseMuchPt.SetPosition(newIn);
        noiseMuchPt.SetPositionOut(newOut);
        noiseMuchPt.SetTrackID(-1);
        new (mpref[ind++]) CbmMuchPoint(noiseMuchPt);
        break;
      }
    }
  }
}

void LxGenNoiseElectrons::Finish() {}
