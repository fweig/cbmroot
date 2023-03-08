/* Copyright (C) 2016-2020 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#include "CalcStats.h"

#include "CbmMCTrack.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPoint.h"
#include "CbmTrdAddress.h"
#include "CbmTrdPoint.h"

#include <FairRootManager.h>
#include <Logger.h>

#include "TClonesArray.h"
#include <TFile.h>

#include <list>
#include <map>

ClassImp(LxCalcStats)

  using namespace std;

struct LxStatTrack {
  list<Int_t> muchPoints[4][3];
  list<Int_t> trdPoints[4];
};

LxCalcStats::LxCalcStats() : fMCTracks(0), fMuchPoints(0), fTrdPoints(0) {}

InitStatus LxCalcStats::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  fMCTracks   = static_cast<TClonesArray*>(ioman->GetObject("MCTrack"));
  fMuchPoints = static_cast<TClonesArray*>(ioman->GetObject("MuchPoint"));
  fTrdPoints  = static_cast<TClonesArray*>(ioman->GetObject("TrdPoint"));

  if (0 == fMCTracks || (0 == fMuchPoints && 0 == fTrdPoints)) LOG(fatal) << "No MC tracks or points";

  size_t buf_size = 128;
  char buf[buf_size];

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 3; ++j) {
      snprintf(buf, buf_size - 1, "noise_e_x_%d_%d", i, j);
      xHistos[i][j] = new TH1F(buf, buf, 240, -30., 30.);
      snprintf(buf, buf_size - 1, "noise_e_y_%d_%d", i, j);
      yHistos[i][j] = new TH1F(buf, buf, 240, -30., 30.);
    }
  }

  for (int i = 1; i < 4; ++i) {
    snprintf(buf, buf_size - 1, "trdDeltaThetaX_%d", i);
    trdDeltaThetaXHistos[i - 1] = new TH1F(buf, buf, 100, -1.0, 1.0);
    snprintf(buf, buf_size - 1, "trdDeltaThetaY_%d", i);
    trdDeltaThetaYHistos[i - 1] = new TH1F(buf, buf, 100, -1.0, 1.0);
  }

  return kSUCCESS;
}

void LxCalcStats::Exec(Option_t* /*opt*/)
{
  map<Int_t, LxStatTrack> eNoiseTracks;
  Int_t nofMuchPoints = 0 == fMuchPoints ? 0 : fMuchPoints->GetEntriesFast();

  for (Int_t i = 0; i < nofMuchPoints; ++i) {
    const CbmMuchPoint* point              = static_cast<const CbmMuchPoint*>(fMuchPoints->At(i));
    Int_t stationNumber                    = CbmMuchGeoScheme::GetStationIndex(point->GetDetectorId());
    Int_t layerNumber                      = CbmMuchGeoScheme::GetLayerIndex(point->GetDetectorId());
    Int_t trackId                          = point->GetTrackID();
    map<Int_t, LxStatTrack>::iterator trIt = eNoiseTracks.find(trackId);

    if (trIt == eNoiseTracks.end()) eNoiseTracks[trackId] = LxStatTrack();

    eNoiseTracks[trackId].muchPoints[stationNumber][layerNumber].push_back(i);
  }

  for (Int_t i = 0; i < nofMuchPoints; ++i) {
    const CbmMuchPoint* point = static_cast<const CbmMuchPoint*>(fMuchPoints->At(i));
    Int_t stationNumber       = CbmMuchGeoScheme::GetStationIndex(point->GetDetectorId());
    Int_t layerNumber         = CbmMuchGeoScheme::GetLayerIndex(point->GetDetectorId());
    Int_t trackId             = point->GetTrackID();
    const CbmMCTrack* track   = static_cast<const CbmMCTrack*>(fMCTracks->At(trackId));
    Int_t pdgCode             = track->GetPdgCode();

    if (11 == pdgCode) {
      Double_t x     = (point->GetXIn() + point->GetXOut()) / 2;
      Double_t y     = (point->GetYIn() + point->GetYOut()) / 2;
      Int_t parentId = track->GetMotherId();

      while (parentId >= 0) {
        const CbmMCTrack* parent    = static_cast<const CbmMCTrack*>(fMCTracks->At(parentId));
        const list<Int_t> parentPts = eNoiseTracks[parentId].muchPoints[stationNumber][layerNumber];

        for (list<Int_t>::const_iterator j = parentPts.begin(); j != parentPts.end(); ++j) {
          Int_t parentPtId             = *j;
          const CbmMuchPoint* parentPt = static_cast<const CbmMuchPoint*>(fMuchPoints->At(parentPtId));
          Double_t pX                  = (parentPt->GetXIn() + parentPt->GetXOut()) / 2;
          Double_t pY                  = (parentPt->GetYIn() + parentPt->GetYOut()) / 2;
          xHistos[stationNumber][layerNumber]->Fill(x - pX);
          yHistos[stationNumber][layerNumber]->Fill(y - pY);
        }

        parentId = parent->GetMotherId();
      }
    }
  }

  map<Int_t, LxStatTrack> jPsiMuTracks;
  Int_t nofTrdPoints = 0 == fTrdPoints ? 0 : fTrdPoints->GetEntriesFast();

  for (Int_t i = 0; i < nofTrdPoints; ++i) {
    const CbmTrdPoint* point = static_cast<const CbmTrdPoint*>(fTrdPoints->At(i));
    Int_t layerNumber        = CbmTrdAddress::GetLayerId(point->GetModuleAddress());
    Int_t trackId            = point->GetTrackID();
    const CbmMCTrack* track  = static_cast<const CbmMCTrack*>(fMCTracks->At(trackId));

    if (-11 != track->GetPdgCode() && 11 != track->GetPdgCode()) continue;

    Int_t motherId = track->GetMotherId();

    if (0 > motherId) continue;

    const CbmMCTrack* mother = static_cast<const CbmMCTrack*>(fMCTracks->At(motherId));

    if (443 != mother->GetPdgCode()) continue;

    map<Int_t, LxStatTrack>::iterator trIt = jPsiMuTracks.find(trackId);

    if (trIt == jPsiMuTracks.end()) jPsiMuTracks[trackId] = LxStatTrack();

    jPsiMuTracks[trackId].trdPoints[layerNumber].push_back(i);
  }

  for (map<Int_t, LxStatTrack>::const_iterator i = jPsiMuTracks.begin(); i != jPsiMuTracks.end(); ++i) {
    const LxStatTrack& track = i->second;

    for (int j = 1; j < 4; ++j) {
      const list<Int_t>& lPoints = track.trdPoints[j - 1];
      const list<Int_t>& rPoints = track.trdPoints[j];

      for (list<Int_t>::const_iterator k = rPoints.begin(); k != rPoints.end(); ++k) {
        const CbmTrdPoint* rPoint = static_cast<const CbmTrdPoint*>(fTrdPoints->At(*k));
        Double_t rX               = (rPoint->GetXIn() + rPoint->GetXOut()) / 2;
        Double_t rY               = (rPoint->GetYIn() + rPoint->GetYOut()) / 2;
        Double_t rZ               = (rPoint->GetZIn() + rPoint->GetZOut()) / 2;
        Double_t rTx              = rX / rZ;
        Double_t rTy              = rY / rZ;

        for (list<Int_t>::const_iterator l = lPoints.begin(); l != lPoints.end(); ++l) {
          const CbmTrdPoint* lPoint = static_cast<const CbmTrdPoint*>(fTrdPoints->At(*l));
          Double_t lX               = (lPoint->GetXIn() + lPoint->GetXOut()) / 2;
          Double_t lY               = (lPoint->GetYIn() + lPoint->GetYOut()) / 2;
          Double_t lZ               = (lPoint->GetZIn() + lPoint->GetZOut()) / 2;
          Double_t deltaZ           = lZ - rZ;
          Double_t x                = rX + rTx * deltaZ;
          Double_t y                = rY + rTy * deltaZ;
          trdDeltaThetaXHistos[j - 1]->Fill((lX - x) / deltaZ);
          trdDeltaThetaYHistos[j - 1]->Fill((lY - y) / deltaZ);
        }
      }
    }
  }
}

static void SaveHisto(TH1F* h)
{
  TFile* curFile = TFile::CurrentFile();
  size_t buf_size = 128;
  char name[buf_size];
  snprintf(name, buf_size - 1, "%s.root", h->GetName());
  TFile fh(name, "RECREATE");
  h->Write();
  fh.Close();
  delete h;
  TFile::CurrentFile() = curFile;
}

void LxCalcStats::Finish()
{
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 3; ++j) {
      SaveHisto(xHistos[i][j]);
      SaveHisto(yHistos[i][j]);
    }
  }

  for (int i = 0; i < 3; ++i) {
    SaveHisto(trdDeltaThetaXHistos[i]);
    SaveHisto(trdDeltaThetaYHistos[i]);
  }
}
