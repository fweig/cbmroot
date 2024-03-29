/* Copyright (C) 2014-2021 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#include "Lx.h"

#include "CbmKFParticle.h"
#include "CbmKFTrack.h"
#include "CbmMCTrack.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPoint.h"
#include "CbmMuchTrack.h"
#include "CbmStsAddress.h"
#include "CbmStsPoint.h"

#include "TDatabasePDG.h"
#include "TGeoManager.h"

#include <iostream>

#include <sys/time.h>

#include "LxDraw.h"

ClassImp(LxFinder)

  using namespace std;

LxFinder* LxFinder::fInstance = 0;

#ifdef MAKE_HISTOS
TH1F* LxFinder::massHisto = 0;
//TH1F* LxFinder::backgroundMassHisto = 0;
TTree* LxFinder::superEventTracks                = 0;
TH1F* LxFinder::signalChi2Histo                  = 0;
TH1F* LxFinder::backgroundChi2Histo              = 0;
TProfile* LxFinder::effByMomentumProfile         = 0;
TH1F* LxFinder::muPlusDtxHisto                   = 0;
TH1F* LxFinder::muMinusDtxHisto                  = 0;
TH1F* LxFinder::signalXAtZ0                      = 0;
TH1F* LxFinder::signalYAtZ0                      = 0;
TH1F* LxFinder::bgrXAtZ0                         = 0;
TH1F* LxFinder::bgrYAtZ0                         = 0;
TH1F* LxFinder::signalInterTracksDistance        = 0;
TH1F* LxFinder::bgrInterTracksDistance           = 0;
TH1F* LxFinder::signalTanSigns                   = 0;
TH1F* LxFinder::signalCoordSigns                 = 0;
TH1F* LxFinder::bgrTanSigns                      = 0;
TH1F* LxFinder::bgrCoordSigns                    = 0;
TH1F* LxFinder::numberOfTracks                   = 0;
TH1F* LxFinder::signalInterTracksDistanceOn1st   = 0;
TH1F* LxFinder::bgrInterTracksDistanceOn1st      = 0;
TH1F* LxFinder::bgrInterTracksDistanceOn1stSigns = 0;
TH1F* LxFinder::signalInterTracksAngle           = 0;
TH1F* LxFinder::bgrInterTracksAngle              = 0;
TH2F* LxFinder::signalInterTrackCorrDA           = 0;
TH2F* LxFinder::bgrInterTrackCorrDA              = 0;
TH1F* LxFinder::muchMomErrSig                    = 0;
TH1F* LxFinder::muchMomErrBgr                    = 0;
#endif  //MAKE_HISTOS

#ifdef MAKE_DISPERSE_2D_HISTOS
TProfile2D* disperseLHistos[LXSTATIONS];
TProfile2D* disperseRHistos[LXSTATIONS];
TProfile2D* disperseDHistos[LXSTATIONS];
#endif  //MAKE_DISPERSE_2D_HISTOS

#ifdef MAKE_TRIGGERING_HISTOS
TH1F* LxFinder::triggeringAllTracksVertices  = 0;
TH1F* LxFinder::triggeringDistTracksVertices = 0;
TH1F* LxFinder::triggeringSignTracksVertices = 0;
TH1F* LxFinder::triggeringTrigTracksVertices = 0;
#endif  //MAKE_TRIGGERING_HISTOS

#ifdef MAKE_HISTOS
static TH1F* stsTrackChi2 = 0;
static TH1F* stsTrackX    = 0;
static TH1F* stsTrackY    = 0;
#endif  //MAKE_HISTOS

LxHitFile::LxHitFile() : useForWrite(false), fd(-1), evBuf(0), bufSize(0), evPtr(0), evEnd(0) {}

LxHitFile::~LxHitFile() { delete[] evBuf; }

bool LxHitFile::Open(TString fileName, bool forWrite)
{
  useForWrite = forWrite;

  if (forWrite) fd = open(fileName.Data(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  else
    fd = open(fileName.Data(), O_RDONLY);

  return -1 != fd;
}

bool LxHitFile::StartEvent(Int_t nEnt)
{
  off_t curOff = lseek(fd, 0, SEEK_CUR);

  if (-1 == curOff) return false;

  eventOffs.push_back(curOff);
  bufSize = (2 * sizeof(Int_t) + 6 * sizeof(Double_t)) * nEnt;
  evBuf   = new char[bufSize];
  evPtr   = evBuf;
  evEnd   = evBuf + bufSize;
  return true;
}

bool LxHitFile::EndEvent()
{
  ssize_t res = write(fd, evBuf, bufSize);
  delete[] evBuf;
  evBuf   = 0;
  bufSize = 0;
  evPtr   = 0;
  evEnd   = 0;
  return -1 != res;
}

bool LxHitFile::WriteHit(Int_t stationNumber, Int_t layerNumber, Double_t x, Double_t y, Double_t z, Double_t xErr,
                         Double_t yErr, Double_t zErr)
{
  if (evPtr + 2 * sizeof(Int_t) + 6 * sizeof(Double_t) > evEnd) return false;

  memcpy(evPtr, &stationNumber, sizeof(stationNumber));
  evPtr += sizeof(stationNumber);
  memcpy(evPtr, &layerNumber, sizeof(layerNumber));
  evPtr += sizeof(layerNumber);
  memcpy(evPtr, &x, sizeof(x));
  evPtr += sizeof(x);
  memcpy(evPtr, &y, sizeof(y));
  evPtr += sizeof(y);
  memcpy(evPtr, &z, sizeof(z));
  evPtr += sizeof(z);
  memcpy(evPtr, &xErr, sizeof(xErr));
  evPtr += sizeof(xErr);
  memcpy(evPtr, &yErr, sizeof(yErr));
  evPtr += sizeof(yErr);
  memcpy(evPtr, &zErr, sizeof(zErr));
  evPtr += sizeof(zErr);
  return true;
}

bool LxHitFile::ReadEvent(Int_t eventNumber)
{
  off_t footerOfFooterOff = lseek(fd, -sizeof(off_t), SEEK_END);

  if (-1 == footerOfFooterOff) return false;

  off_t footerOff;
  ssize_t readBytes = read(fd, &footerOff, sizeof(footerOff));

  if (-1 == readBytes) return false;

  Int_t numFooterEntries = (footerOfFooterOff - footerOff) / sizeof(off_t);

  if (eventNumber >= numFooterEntries) return false;

  off_t eventOff;
  readBytes = pread(fd, &eventOff, sizeof(eventOff), footerOff + eventNumber * sizeof(off_t));

  if (-1 == readBytes) return false;

  off_t nextEvOff;

  if (eventNumber == numFooterEntries - 1) nextEvOff = footerOff;
  else {
    readBytes = pread(fd, &nextEvOff, sizeof(nextEvOff), footerOff + (eventNumber + 1) * sizeof(off_t));

    if (-1 == readBytes) return false;
  }

  delete[] evBuf;
  evBuf   = 0;
  bufSize = 0;

  bufSize   = nextEvOff - eventOff;
  evBuf     = new char[bufSize];
  readBytes = pread(fd, evBuf, bufSize, eventOff);

  if (readBytes != bufSize) {
    delete[] evBuf;
    evBuf   = 0;
    bufSize = 0;
    evPtr   = 0;
    evEnd   = 0;
    return false;
  }

  evPtr = evBuf;
  evEnd = evBuf + bufSize;
  return true;
}

bool LxHitFile::ReadHit(Int_t& stationNumber, Int_t& layerNumber, Double_t& x, Double_t& y, Double_t& z, Double_t& xErr,
                        Double_t& yErr, Double_t& zErr)
{
  if (evPtr + 2 * sizeof(Int_t) + 6 * sizeof(Double_t) > evEnd) return false;

  memcpy(&stationNumber, evPtr, sizeof(stationNumber));
  evPtr += sizeof(stationNumber);
  memcpy(&layerNumber, evPtr, sizeof(layerNumber));
  evPtr += sizeof(layerNumber);
  memcpy(&x, evPtr, sizeof(x));
  evPtr += sizeof(x);
  memcpy(&y, evPtr, sizeof(y));
  evPtr += sizeof(y);
  memcpy(&z, evPtr, sizeof(z));
  evPtr += sizeof(z);
  memcpy(&xErr, evPtr, sizeof(xErr));
  evPtr += sizeof(xErr);
  memcpy(&yErr, evPtr, sizeof(yErr));
  evPtr += sizeof(yErr);
  memcpy(&zErr, evPtr, sizeof(zErr));
  evPtr += sizeof(zErr);
  return true;
}

bool LxHitFile::Close()
{
  if (!useForWrite) return -1 != close(fd);

  off_t footerOff = lseek(fd, 0, SEEK_CUR);
  ssize_t res;
  int res2;

  for (list<off_t>::iterator i = eventOffs.begin(); i != eventOffs.end(); ++i) {
    off_t o = *i;
    res     = write(fd, &o, sizeof(o));

    if (-1 == res) goto end;
  }

  res = write(fd, &footerOff, sizeof(footerOff));

end:
  res2 = close(fd);
  return -1 != res && -1 != res2;
}

LxFinder* LxFinder::Instance() { return fInstance; }

LxFinder::LxFinder() : muchPixelHits(0), listMCTracks(0), listMuchPts(0), listMuchClusters(0),
    listMuchPixelDigiMatches(0), listStsTracks(0), listStsMatches(0), listStsPts(0), listRecoTracks(0),
    superEventData(0), extFitter(), fPrimVtx(0), positiveTracks(), negativeTracks(),
    generateInvMass(false), generateBackground(false), generateChi2(false),
    linkWithSts(true), useMCPInsteadOfHits(false), calcMiddlePoints(true), cutCoeff(4.0), verbosity(1),
    parallMode(false), hitFileName(""), fileSaveSuffix(""), particleType("jpsi"), pPtCut(true),
    MCPoints(), MCTracks(), MCStsPoints()
    , MCStsPointsByStations()
#ifdef MAKE_DISPERSE_2D_HISTOS
    , MCPointsByStations(),
    , zCoordsByStations(),
#endif  //MAKE_DISPERSE_2D_HISTOS
    , caSpace()
#ifdef MAKE_EFF_CALC
    , incomplete_events()
    , falseSignalTriggerings(0)
    , trueSignalTriggerings(0)
    , hasSignalInEvent(false)
    , signalCounter(0)
#endif  //MAKE_EFF_CALC
    , eventNumber(0)
#ifdef CALC_MUCH_DETECTORS_EFF
    , mcPointsCount(0), mcPointsTriggered(0)
#endif  //CALC_MUCH_DETECTORS_EFF
    , saveOnlyTriggeringTracks(true)
{
  fInstance = this;
}

LxFinder::~LxFinder() {}

TString lxFinderParticleType = "jpsi";

static bool GetHistoRMS(const char* histoNameBase, Int_t histoNumber, scaltype& retVal)
{
  bool result = false;
#pragma omp critical
  {
    char name[256];
    char dir_name[256];
    sprintf(dir_name, "configuration.%s", lxFinderParticleType.Data());

    if (histoNumber < 0) sprintf(name, "%s/%s.root", dir_name, histoNameBase);
    else
      sprintf(name, "%s/%s_%d.root", dir_name, histoNameBase, histoNumber);

    TFile* curFile = TFile::CurrentFile();

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    TFile* f = new TFile(name);

    if (!f->IsZombie()) {
      if (histoNumber < 0) sprintf(name, "%s", histoNameBase);
      else
        sprintf(name, "%s_%d", histoNameBase, histoNumber);

      TH1F* h = f->Get<TH1F>(name);
      if (h) {
        retVal = h->GetRMS();
        result = true;
      }
    }

    delete f;
    TFile::CurrentFile() = curFile;

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;
  }
  return result;
}

static bool GetHistoCOV(const char* histoNameBase, Int_t histoNumber, Int_t axis1, Int_t axis2, scaltype& retVal)
{
  bool result = false;
#pragma omp critical
  {
    char name[256];
    char dir_name[256];
    sprintf(dir_name, "configuration.%s", lxFinderParticleType.Data());
    sprintf(name, "%s/%s_%d.root", dir_name, histoNameBase, histoNumber);
    TFile* curFile = TFile::CurrentFile();

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    TFile* f = new TFile(name);

    if (!f->IsZombie()) {
      sprintf(name, "%s_%d", histoNameBase, histoNumber);
      TH2F* h = f->Get<TH2F>(name);
      if (h) {
        retVal = h->GetCovariance(axis1, axis2);
        result = true;
      }
    }

    delete f;
    TFile::CurrentFile() = curFile;

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;
  }
  return result;
}

InitStatus LxFinder::Init()
{
  static Int_t nTimes = 1;
  cout << "LxFinder::Init() called at " << nTimes++ << " time" << endl;

  if (parallMode) hitFile.Open(hitFileName, false);
  else if (hitFileName != "")
    hitFile.Open(hitFileName, true);

  superEventData = new CbmStsTrack;

  FairRootManager* fRootManager = 0;

  if (!parallMode) {
    fRootManager             = FairRootManager::Instance();
    muchPixelHits            = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("MuchPixelHit"));
    listMCTracks             = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("MCTrack"));
    listMuchPts              = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("MuchPoint"));
    listMuchClusters         = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("MuchCluster"));
    listMuchPixelDigiMatches = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("MuchDigiMatch"));
    listStsTracks            = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("StsTrack"));
    listStsMatches           = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("StsTrackMatch"));
    listStsPts               = LX_DYNAMIC_CAST<TClonesArray*>(fRootManager->GetObject("StsPoint"));
    //fPrimVtx = LX_DYNAMIC_CAST<CbmVertex*> (fManager->GetObject("PrimaryVertex"));
    /*
    // Get pointer to PrimaryVertex object from IOManager if it exists
    // The old name for the object is "PrimaryVertex" the new one
    // "PrimaryVertex." Check first for the new name
    fPrimVtx = LX_DYNAMIC_CAST<CbmVertex*>(fManager->GetObject("PrimaryVertex."));
    if (nullptr == fPrimVtx) {
      fPrimVtx = LX_DYNAMIC_CAST<CbmVertex*>(fManager->GetObject("PrimaryVertex"));
    }
    if (nullptr == fPrimVtx) {
      Error("CbmL1SttTrackFinder::ReInit","vertex not found!");
      return kERROR;
    }
    */
  }  // if (!parallMode)

  fPrimVtx = new CbmVertex;
  scaltype rms;

  // Read Z-positions of MUCH station layers and save them in LxLayer objects.

  for (Int_t i = 0; i < LXSTATIONS; ++i) {
    LxStation* aStation = caSpace.stations[i];

    for (Int_t j = 0; j < LXLAYERS; ++j) {
#pragma omp critical
      {
        TString muchStLrPath = Form("/cave_1/much_0/muchstation%02i_0/muchstation%02ilayer%01i_0", i + 1, i + 1, j + 1);
        gGeoManager->cd(muchStLrPath.Data());
        Double_t localCoords[3] = {0., 0., 0.};
        Double_t globalCoords[3];
        gGeoManager->LocalToMaster(localCoords, globalCoords);
        aStation->layers[j]->zCoord = globalCoords[2];
#ifdef MAKE_DISPERSE_2D_HISTOS
        zCoordsByStations[i][j] = globalCoords[2];
#endif  //MAKE_DISPERSE_2D_HISTOS
      }
    }

    aStation->zCoord = aStation->layers[LXMIDDLE]->zCoord;

    //if (0 == i)
    //cout << "First MUCH station Z-coord = " << aStation->zCoord << endl;

    if (!GetHistoRMS("muchInStationXDispLeft", i, rms)) return kFATAL;

    x_disp_left_limits[i]    = cutCoeff * rms;
    x_disp_left_limits_sq[i] = x_disp_left_limits[i] * x_disp_left_limits[i];

    if (!GetHistoRMS("muchInStationYDispLeft", i, rms)) return kFATAL;

    y_disp_left_limits[i]    = cutCoeff * rms;
    y_disp_left_limits_sq[i] = y_disp_left_limits[i] * y_disp_left_limits[i];

    if (!GetHistoRMS("muchInStationXDispRight", i, rms)) return kFATAL;

    x_disp_right_limits[i]    = cutCoeff * rms;
    x_disp_right_limits_sq[i] = x_disp_right_limits[i] * x_disp_right_limits[i];

    if (!GetHistoRMS("muchInStationYDispRight", i, rms)) return kFATAL;

    y_disp_right_limits[i]    = cutCoeff * rms;
    y_disp_right_limits_sq[i] = y_disp_right_limits[i] * y_disp_right_limits[i];

    if (i > 0) {
      if (!GetHistoRMS("muchLongSegmentTxHisto", i, rms)) return kFATAL;

      aStation->txLimit = cutCoeff * rms;
      tx_limits[i]      = aStation->txLimit;
      tx_limits_sq[i]   = aStation->txLimit * aStation->txLimit;

      if (!GetHistoRMS("muchLongSegmentTyHisto", i, rms)) return kFATAL;

      aStation->tyLimit = cutCoeff * rms;
      ty_limits[i]      = aStation->tyLimit;
      ty_limits_sq[i]   = aStation->tyLimit * aStation->tyLimit;

#ifdef CLUSTER_MODE
      if (!GetHistoRMS("muchClusterXDispHisto", i, rms)) return kFATAL;

      aStation->clusterXLimit  = rms;
      aStation->clusterXLimit2 = rms * rms;

      if (!GetHistoRMS("muchClusterYDispHisto", i, rms)) return kFATAL;

      aStation->clusterYLimit  = rms;
      aStation->clusterYLimit2 = rms * rms;

      if (!GetHistoRMS("muchClusterTxDispHisto", i, rms)) return kFATAL;

      aStation->clusterTxLimit  = rms;
      aStation->clusterTxLimit2 = rms * rms;

      if (!GetHistoRMS("muchClusterTyDispHisto", i, rms)) return kFATAL;

      aStation->clusterTyLimit  = rms;
      aStation->clusterTyLimit2 = rms * rms;
#endif  //CLUSTER_MODE

      if (i < LXSTATIONS - 1) {
        if (!GetHistoRMS("muchSegmentTxBreakHisto", i, rms)) return kFATAL;

        aStation->txBreakSigma = rms;
        aStation->txBreakLimit = cutCoeff * rms;

        if (!GetHistoRMS("muchSegmentTyBreakHisto", i, rms)) return kFATAL;

        aStation->tyBreakSigma = rms;
        aStation->tyBreakLimit = cutCoeff * rms;
      }
    }

#ifdef USE_KALMAN_FIT
    if (i < LXSTATIONS - 1) {
      for (Int_t j = 0; j <= 1; ++j) {
        for (Int_t k = 0; k <= 1; ++k) {
          if (!GetHistoCOV("muchXTxCovHisto", i, j + 1, k + 1, aStation->MSNoise[0][j][k])) return kFATAL;

          if (!GetHistoCOV("muchYTyCovHisto", i, j + 1, k + 1, aStation->MSNoise[1][j][k])) return kFATAL;
        }
      }
    }
#endif  //USE_KALMAN_FIT
  }

  if (!GetHistoRMS("muchStsBreakX", -1, caSpace.muchStsBreakX)) {
    cout << "LxFinder::Init(): Failed to read RMS from muchStsBreakX" << endl;
    return kFATAL;
  }

  if (!GetHistoRMS("muchStsBreakY", -1, caSpace.muchStsBreakY)) {
    cout << "LxFinder::Init(): Failed to read RMS from muchStsBreakY" << endl;
    return kFATAL;
  }

  if (!GetHistoRMS("muchStsBreakTx", -1, caSpace.muchStsBreakTx)) {
    cout << "LxFinder::Init(): Failed to read RMS from muchStsBreakTx" << endl;
    return kFATAL;
  }

  if (!GetHistoRMS("muchStsBreakTy", -1, caSpace.muchStsBreakTy)) {
    cout << "LxFinder::Init(): Failed to read RMS from muchStsBreakTy" << endl;
    return kFATAL;
  }

  // Create an output array.
  if (!parallMode) {
    listRecoTracks = new TClonesArray("CbmMuchTrack", 100);
    fRootManager->Register("MuchTrack", "Much", listRecoTracks, IsOutputBranchPersistent("MuchTrack"));
  }

#ifdef MAKE_HISTOS
  if (generateInvMass) massHisto = new TH1F("jpsi_mass", "jpsi_mass", 100, 2., 4.);

  if (generateBackground) {
    //backgroundMassHisto = new TH1F("background_mass", "background_mass", 400, 2., 4.);
    superEventTracks = new TTree("SuperEventTracks", "Tracks for building a super event");
    superEventTracks->Branch("tracks", "CbmStsTrack", &superEventData);
  }

  if (generateChi2) {
    signalChi2Histo     = new TH1F("signal_chi2", "signal_chi2", 100, 0., 20.);
    backgroundChi2Histo = new TH1F("background_chi2", "background_chi2", 100, 0., 20.);
  }
#endif  //MAKE_HISTOS

#ifdef MAKE_HISTOS
  //  char histoName[128];

  for (int i = 0; i < 6; ++i) {
#ifdef MAKE_DISPERSE_2D_HISTOS
    sprintf(histoName, "disperseL_%d", i);
    disperseLHistos[i] = new TProfile2D(histoName, histoName, 30, -3., 3., 30, -3., 3.);
    disperseLHistos[i]->StatOverflows();
    sprintf(histoName, "disperseR_%d", i);
    disperseRHistos[i] = new TProfile2D(histoName, histoName, 30, -3., 3., 30, -3., 3.);
    disperseRHistos[i]->StatOverflows();
    sprintf(histoName, "disperseD_%d", i);
    disperseDHistos[i] = new TProfile2D(histoName, histoName, 30, -3., 3., 30, -3., 3.);
    disperseDHistos[i]->StatOverflows();
#endif  //MAKE_DISPERSE_2D_HISTOS
  }

#endif  //MAKE_HISTOS

#ifdef MAKE_HISTOS
  muPlusDtxHisto = new TH1F("muPlusDtxHisto", "Mu+ Tx difference", 200, -0.15, 0.15);
  muPlusDtxHisto->StatOverflows();
  muMinusDtxHisto = new TH1F("muMinusDtxHisto", "Mu- Tx difference", 200, -0.15, 0.15);
  muMinusDtxHisto->StatOverflows();
  signalXAtZ0 = new TH1F("signalXAtZ0", "Signal track X at Z=0", 200, -40.0, 40.0);
  signalXAtZ0->StatOverflows();
  signalYAtZ0 = new TH1F("signalYAtZ0", "Signal track Y at Z=0", 200, -40.0, 40.0);
  signalYAtZ0->StatOverflows();
  bgrXAtZ0 = new TH1F("bgrXAtZ0", "Background track X at Z=0", 200, -40.0, 40.0);
  bgrXAtZ0->StatOverflows();
  bgrYAtZ0 = new TH1F("bgrYAtZ0", "Background track Y at Z=0", 200, -40.0, 40.0);
  bgrYAtZ0->StatOverflows();
  signalInterTracksDistance = new TH1F("signalInterTracksDistance", "Distance between signal tracks", 200, 0.0, 30.0);
  signalInterTracksDistance->StatOverflows();
  bgrInterTracksDistance = new TH1F("bgrInterTracksDistance", "Distance between background tracks", 200, 0.0, 30.0);
  bgrInterTracksDistance->StatOverflows();
  signalTanSigns = new TH1F("signalTanSigns", "Signs of signal tracks tangents products", 20, -2.0, 2.0);
  signalTanSigns->StatOverflows();
  signalCoordSigns = new TH1F("signalCoordSigns", "Signs of signal tracks coordinates products", 20, -2.0, 2.0);
  signalCoordSigns->StatOverflows();
  bgrTanSigns = new TH1F("bgrTanSigns", "Signs of background tracks tangents products", 20, -2.0, 2.0);
  bgrTanSigns->StatOverflows();
  bgrCoordSigns = new TH1F("bgrCoordSigns", "Signs of background tracks coordinates products", 20, -2.0, 2.0);
  bgrCoordSigns->StatOverflows();
  numberOfTracks = new TH1F("numberOfTracks", "The number of reconstructed tracks in an event", 6, 0.0, 6.0);
  numberOfTracks->StatOverflows();
  signalInterTracksDistanceOn1st =
    new TH1F("signalInterTracksDistanceOn1st", "Distance between signal tracks on the 1st station", 200, 0.0, 250.0);
  signalInterTracksDistanceOn1st->StatOverflows();
  bgrInterTracksDistanceOn1st =
    new TH1F("bgrInterTracksDistanceOn1st", "Distance between background tracks on the 1st station", 200, 0.0, 250.0);
  bgrInterTracksDistanceOn1st->StatOverflows();
  bgrInterTracksDistanceOn1stSigns =
    new TH1F("bgrInterTracksDistanceOn1stSigns",
             "Distance between background tracks on the 1st station separated by signs", 200, 0.0, 250.0);
  bgrInterTracksDistanceOn1stSigns->StatOverflows();
  signalInterTracksAngle =
    new TH1F("signalInterTracksAngle", "Angle between signal tracks by hits on 1st station", 200, 0.0, 60.0);
  signalInterTracksAngle->StatOverflows();
  bgrInterTracksAngle =
    new TH1F("bgrInterTracksAngle", "Angle between background tracks by hits on 1st station", 200, 0.0, 60.0);
  bgrInterTracksAngle->StatOverflows();
  signalInterTrackCorrDA =
    new TH2F("signalInterTrackCorrDA", "signalInterTrackCorrDA", 200, 0.0, 250.0, 200, 0.0, 60.0);
  signalInterTrackCorrDA->StatOverflows();
  bgrInterTrackCorrDA = new TH2F("bgrInterTrackCorrDA", "bgrInterTrackCorrDA", 200, 0.0, 250.0, 200, 0.0, 60.0);
  bgrInterTrackCorrDA->StatOverflows();
  muchMomErrSig = new TH1F("muchMomErrSig", "Momentum determination error for signal", 600, -300.0, 300.0);
  muchMomErrSig->StatOverflows();
  muchMomErrBgr = new TH1F("muchMomErrBgr", "Momentum determination error for background", 600, -300.0, 300.0);
  muchMomErrBgr->StatOverflows();

  effByMomentumProfile = new TProfile("Reconstruction efficiency by momentum", "Reconstruction efficiency by momentum",
                                      100, 0.0, 50.0, 0.0, 100.0);
#endif  //MAKE_HISTOS

#ifdef MAKE_TRIGGERING_HISTOS
  triggeringAllTracksVertices =
    new TH1F("triggeringAllTracksVertices", "triggeringAllTracksVertices", 330, -10.0, 320.0);
  triggeringAllTracksVertices->StatOverflows();
  triggeringDistTracksVertices =
    new TH1F("triggeringDistTracksVertices", "triggeringDistTracksVertices", 330, -10.0, 320.0);
  triggeringDistTracksVertices->StatOverflows();
  triggeringSignTracksVertices =
    new TH1F("triggeringSignTracksVertices", "triggeringSignTracksVertices", 330, -10.0, 320.0);
  triggeringSignTracksVertices->StatOverflows();
  triggeringTrigTracksVertices =
    new TH1F("triggeringTrigTracksVertices", "triggeringTrigTracksVertices", 330, -10.0, 320.0);
  triggeringTrigTracksVertices->StatOverflows();
#endif  //MAKE_TRIGGERING_HISTOS

#ifdef MAKE_HISTOS
  stsTrackChi2 = new TH1F("stsTrackChi2", "stsTrackChi2", 200, 0., 10.);
  stsTrackChi2->StatOverflows();
  stsTrackX = new TH1F("stsTrackX", "stsTrackX", 200, -0.2, 0.2);
  stsTrackX->StatOverflows();
  stsTrackY = new TH1F("stsTrackY", "stsTrackY", 200, -0.2, 0.2);
  stsTrackY->StatOverflows();
#endif  //MAKE_HISTOS

#ifdef MAKE_EFF_CALC
  incomplete_events.open("incomplete_events.txt");
#endif  //MAKE_EFF_CALC

  return kSUCCESS;
}

static Int_t nTimes          = 1;
static Int_t wholeDuration   = 0;
static Int_t averageDuration = 0;

void LxFinder::Exec(Option_t* opt)
{
  cout << "LxFinder::Exec() called at " << nTimes << " time" << endl;
  timeval bTime, eTime;
  int exeDuration;
  gettimeofday(&bTime, 0);

  // Clean all previously created data.
  MCTracks.clear();
  MCPoints.clear();
  MCStsPoints.clear();

  for (int i = 0; i < 8; ++i)
    MCStsPointsByStations[i].clear();

#ifdef MAKE_DISPERSE_2D_HISTOS
  for (int i = 0; i < LXSTATIONS; ++i) {
    for (int j = 0; j < LXLAYERS; ++j)
      MCPointsByStations[i][j].clear();
  }
#endif  //MAKE_DISPERSE_2D_HISTOS

  caSpace.Clear();

  if (!parallMode) listRecoTracks->Clear();

  Int_t nEnt = 0;

#ifdef MAKE_EFF_CALC
  // Read MC tracks
  nEnt = listMCTracks->GetEntriesFast();
  cout << "There are: " << nEnt << " of MC tracks" << endl;
  LxMCTrack mcTrack;

  MCTracks.reserve(nEnt);
  Int_t* root2lxmctrackmap = new Int_t[nEnt];
  Int_t mapCnt             = 0;

  for (int i = 0; i < nEnt; ++i) {
    CbmMCTrack* mct = LX_DYNAMIC_CAST<CbmMCTrack*>(listMCTracks->At(i));
    mcTrack.p       = mct->GetP();
    Int_t pdgCode   = mct->GetPdgCode();

    if (abs(pdgCode) >= 10000) {
      root2lxmctrackmap[i] = -1;
      continue;
    }

    root2lxmctrackmap[i] = mapCnt++;

    mcTrack.q             = TDatabasePDG::Instance()->GetParticle(pdgCode)->Charge() / 3.0;
    mcTrack.x             = mct->GetStartX();
    mcTrack.y             = mct->GetStartY();
    mcTrack.z             = mct->GetStartZ();
    mcTrack.px            = mct->GetPx();
    mcTrack.py            = mct->GetPy();
    mcTrack.pz            = mct->GetPz();
    mcTrack.mother_ID     = mct->GetMotherId();
    mcTrack.fUniqueID     = mct->GetUniqueID();
    mcTrack.pdg           = pdgCode;
    mcTrack.externalTrack = 0;
    MCTracks.push_back(mcTrack);
  }

  nEnt = listStsPts->GetEntriesFast();
  cout << "There are: " << nEnt << " of STS MC points" << endl;

  for (int i = 0; i < nEnt; ++i) {
    TVector3 xyzI, PI, xyzO, PO;
    CbmStsPoint* stsPt = LX_DYNAMIC_CAST<CbmStsPoint*>(listStsPts->At(i));

    if (0 == stsPt) continue;

    LxStsMCPoint stsMCPoint;
    stsPt->Position(xyzI);
    stsPt->Momentum(PI);
    stsPt->PositionOut(xyzO);
    stsPt->MomentumOut(PO);
    TVector3 xyz  = .5 * (xyzI + xyzO);
    TVector3 P    = .5 * (PI + PO);
    stsMCPoint.x  = xyz.X();
    stsMCPoint.y  = xyz.Y();
    stsMCPoint.z  = xyz.Z();
    stsMCPoint.px = P.X();
    stsMCPoint.py = P.Y();
    stsMCPoint.pz = P.Z();
    stsMCPoint.p =
      sqrt(fabs(stsMCPoint.px * stsMCPoint.px + stsMCPoint.py * stsMCPoint.py + stsMCPoint.pz * stsMCPoint.pz));
    stsMCPoint.stationNumber = CbmStsAddress::GetElementId(stsPt->GetDetectorID(), kStsStation);
    Int_t trackId            = root2lxmctrackmap[stsPt->GetTrackID()];

    if (-1 != trackId) {
      stsMCPoint.mcTrack = &MCTracks[trackId];
      MCStsPoints.push_back(stsMCPoint);
      MCTracks[trackId].stsPoints[stsMCPoint.stationNumber].push_back(&MCStsPoints.back());
      MCStsPointsByStations[stsMCPoint.stationNumber].push_back(&MCStsPoints.back());
    }
  }

  nEnt = listMuchPts->GetEntriesFast();
  cout << "There are: " << nEnt << " of MUCH MC points" << endl;
  LxMCPoint mcPoint;

  MCPoints.reserve(nEnt);
  Int_t* root2lxmcpointmap = new Int_t[nEnt];  // Unfortunately we have to use this map because in the loop
                                               // below some iterations can not to produce a point.
  mapCnt = 0;
  //  Int_t mcPtsCount = nEnt;
  Int_t maxReferencedPtsIndex = 0;

  for (int i = 0; i < nEnt; ++i) {
    TVector3 xyzI, PI, xyzO, PO;
    CbmMuchPoint* pt = LX_DYNAMIC_CAST<CbmMuchPoint*>(listMuchPts->At(i));

    if (!pt) {
      root2lxmcpointmap[i] = -1;
      continue;
    }

    Int_t trackId = root2lxmctrackmap[pt->GetTrackID()];

    if (-1 == trackId) {
      root2lxmcpointmap[i] = -1;
      continue;
    }

    mcPoint.trackId = trackId;
    mcPoint.track   = &MCTracks[trackId];

    root2lxmcpointmap[i] = mapCnt++;

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    TVector3 xyz          = .5 * (xyzI + xyzO);
    TVector3 P            = .5 * (PI + PO);
    mcPoint.x             = xyz.X();
    mcPoint.y             = xyz.Y();
    mcPoint.z             = xyz.Z();
    mcPoint.px            = P.X();
    mcPoint.py            = P.Y();
    mcPoint.pz            = P.Z();
    mcPoint.p             = sqrt(fabs(mcPoint.px * mcPoint.px + mcPoint.py * mcPoint.py + mcPoint.pz * mcPoint.pz));
    mcPoint.stationNumber = CbmMuchGeoScheme::GetStationIndex(pt->GetDetectorId());
    mcPoint.layerNumber   = CbmMuchGeoScheme::GetLayerIndex(pt->GetDetectorId());
    MCPoints.push_back(mcPoint);
    Int_t ptId = root2lxmcpointmap[i];

    /*
#ifdef MAKE_HISTOS
    scaltype trackPt2 = MCTracks[trackId].px * MCTracks[trackId].px + MCTracks[trackId].py * MCTracks[trackId].py;
#endif//MAKE_HISTOS
*/

    MCTracks[trackId].Points.push_back(&MCPoints[ptId]);
#ifdef MAKE_DISPERSE_2D_HISTOS
    MCPointsByStations[mcPoint.stationNumber][mcPoint.layerNumber].push_back(&MCPoints[ptId]);
#endif  //MAKE_DISPERSE_2D_HISTOS
  }

#ifdef MAKE_HISTOS
  // Build angle (tangent) breaks distribution <
  for (vector<LxMCTrack>::iterator i = MCTracks.begin(); i != MCTracks.end(); ++i) {
    LxMCTrack& track = *i;

    if ((13 != track.pdg && -13 == track.pdg) || track.mother_ID >= 0) continue;

    if (track.p < 3) continue;

    scaltype pt2 = track.px * track.px + track.py * track.py;

    if (pt2 < 1) continue;

    LxMCPoint* points[LXSTATIONS][LXLAYERS];
    memset(points, 0, sizeof(points));

    for (vector<LxMCPoint*>::iterator j = track.Points.begin(); j != track.Points.end(); ++j) {
      LxMCPoint* point                                 = *j;
      points[point->stationNumber][point->layerNumber] = point;
    }

    for (int j = 0; j < LXSTATIONS; ++j) {
#ifdef MAKE_DISPERSE_2D_HISTOS
      if (0 != points[j][1]) {
        LxMCPoint* mPoint         = points[j][1];
        scaltype tx               = mPoint->x / mPoint->z;
        scaltype ty               = mPoint->y / mPoint->z;
        scaltype diffZ            = zCoordsByStations[j][0] - mPoint->z;
        scaltype x                = mPoint->x + tx * diffZ;
        scaltype y                = mPoint->y + ty * diffZ;
        list<LxMCPoint*>& lPoints = MCPointsByStations[j][0];

        for (list<LxMCPoint*>::iterator k = lPoints.begin(); k != lPoints.end(); ++k) {
          LxMCPoint* lPoint = *k;

          if (lPoint->trackId == mPoint->trackId) disperseLHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 100.);
          else
            disperseLHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 0.);
        }

        diffZ                     = zCoordsByStations[j][2] - mPoint->z;
        x                         = mPoint->x + tx * diffZ;
        y                         = mPoint->y + ty * diffZ;
        list<LxMCPoint*>& rPoints = MCPointsByStations[j][2];

        for (list<LxMCPoint*>::iterator k = rPoints.begin(); k != rPoints.end(); ++k) {
          LxMCPoint* rPoint = *k;

          if (rPoint->trackId == mPoint->trackId) disperseRHistos[j]->Fill(rPoint->x - x, rPoint->y - y, 100.);
          else
            disperseRHistos[j]->Fill(rPoint->x - x, rPoint->y - y, 0.);
        }
      }

      if (0 != points[j][2]) {
        LxMCPoint* rPoint         = points[j][2];
        scaltype tx               = rPoint->x / rPoint->z;
        scaltype ty               = rPoint->y / rPoint->z;
        scaltype diffZ            = zCoordsByStations[j][0] - rPoint->z;
        scaltype x                = rPoint->x + tx * diffZ;
        scaltype y                = rPoint->y + ty * diffZ;
        list<LxMCPoint*>& lPoints = MCPointsByStations[j][0];

        for (list<LxMCPoint*>::iterator k = lPoints.begin(); k != lPoints.end(); ++k) {
          LxMCPoint* lPoint = *k;

          if (lPoint->trackId == rPoint->trackId) disperseDHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 100.);
          else
            disperseDHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 0.);
        }
      }
#endif  //MAKE_DISPERSE_2D_HISTOS
    }  //for (int j = 0; j < LXSTATIONS; ++j)
  }    //for (vector<LxMCTrack>::iterator i = MCTracks.begin(); i != MCTracks.end(); ++i)
  // > angle (tangent) breaks distribution
#endif  //MAKE_HISTOS

#endif  //MAKE_EFF_CALC

  // Read MUCH pixel hits.
  scaltype minXErr = 1000;
  scaltype minYErr = 1000;

  // At last start invocation of CA procedures.
  gettimeofday(&bTime, 0);
  caSpace.InitGlobalCAArrays();
  gettimeofday(&eTime, 0);
  exeDuration   = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  Int_t runTime = exeDuration;
  bTime.tv_sec  = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;

  if (parallMode) {
    nEnt = 0;

    Int_t stationNumber, layerNumber, i = 0;
    Double_t x, y, z, errX, errY, errZ;

    while (hitFile.ReadHit(stationNumber, layerNumber, x, y, z, errX, errY, errZ)) {
      LxPoint* lxPoint = caSpace.AddPoint(stationNumber, layerNumber, i++, x, y, z, errX, errY, errZ);
      int point_number = caSpace.points_counts[stationNumber][layerNumber];
      caSpace.x_coords[stationNumber][layerNumber][point_number]   = x;
      caSpace.x_errs[stationNumber][layerNumber][point_number]     = errX;
      caSpace.y_coords[stationNumber][layerNumber][point_number]   = y;
      caSpace.y_errs[stationNumber][layerNumber][point_number]     = errY;
      caSpace.z_coords[stationNumber][layerNumber][point_number]   = z;
      caSpace.point_refs[stationNumber][layerNumber][point_number] = lxPoint;
      ++caSpace.points_counts[stationNumber][layerNumber];
      ++nEnt;
    }
  }
  else  // (!parallMode)
  {
    nEnt = muchPixelHits->GetEntriesFast();
    cout << "There are: " << nEnt << " of MUCH pixel hits" << endl;

    if (hitFileName != "") hitFile.StartEvent(nEnt);

    for (Int_t i = 0; i < nEnt; ++i) {
      CbmMuchPixelHit* mh = LX_DYNAMIC_CAST<CbmMuchPixelHit*>(muchPixelHits->At(i));

      Int_t stationNumber = CbmMuchGeoScheme::GetStationIndex(mh->GetAddress());
      Int_t layerNumber   = CbmMuchGeoScheme::GetLayerIndex(mh->GetAddress());
      TVector3 pos, err;
      mh->Position(pos);
      mh->PositionError(err);
      scaltype x = pos.X();
      scaltype y = pos.Y();
      scaltype z = pos.Z();

      //if (x != x || y != y || z != z)// Test for NaN
      //continue;

      if (minXErr > err.X()) minXErr = err.X();

      if (minYErr > err.Y()) minYErr = err.Y();

      LxPoint* lxPoint = 0;

      if (!useMCPInsteadOfHits) {
        lxPoint          = caSpace.AddPoint(stationNumber, layerNumber, i, x, y, z, err.X(), err.Y(), err.Z());
        int point_number = caSpace.points_counts[stationNumber][layerNumber];
        caSpace.x_coords[stationNumber][layerNumber][point_number]   = x;
        caSpace.x_errs[stationNumber][layerNumber][point_number]     = err.X();
        caSpace.y_coords[stationNumber][layerNumber][point_number]   = y;
        caSpace.y_errs[stationNumber][layerNumber][point_number]     = err.Y();
        caSpace.z_coords[stationNumber][layerNumber][point_number]   = z;
        caSpace.point_refs[stationNumber][layerNumber][point_number] = lxPoint;
        ++caSpace.points_counts[stationNumber][layerNumber];

        if (hitFileName != "")
          hitFile.WriteHit(stationNumber, layerNumber, pos.X(), pos.Y(), pos.Z(), err.X(), err.Y(), err.Z());
      }

#ifdef MAKE_EFF_CALC
      Int_t clusterId         = mh->GetRefId();
      CbmMuchCluster* cluster = LX_DYNAMIC_CAST<CbmMuchCluster*>(listMuchClusters->At(clusterId));
      Int_t nDigis            = cluster->GetNofDigis();

      for (Int_t j = 0; j < nDigis; ++j) {
        CbmMuchDigiMatch* digiMatch =
          LX_DYNAMIC_CAST<CbmMuchDigiMatch*>(listMuchPixelDigiMatches->At(cluster->GetDigi(j)));
        Int_t nMCs = digiMatch->GetNofLinks();

        for (Int_t k = 0; k < nMCs; ++k) {
          const CbmLink& lnk = digiMatch->GetLink(k);
          Int_t mcIndex      = lnk.GetIndex();

          if (-1 == mcIndex) continue;

          if (mcIndex > maxReferencedPtsIndex) maxReferencedPtsIndex = mcIndex;

          Int_t mcIndexMapped = root2lxmcpointmap[mcIndex];

          if (-1 == mcIndexMapped) continue;

          if (!useMCPInsteadOfHits) {
            MCPoints[mcIndexMapped].lxPoints.push_back(lxPoint);
#ifdef MAKE_EFF_CALC
            lxPoint->mcPoints.push_back(&MCPoints[mcIndexMapped]);
#endif  //MAKE_EFF_CALC
          }
#ifdef MAKE_EFF_CALC
          else if (MCPoints[mcIndexMapped].lxPoints.empty()) {
            LxMCPoint& mcp = MCPoints[mcIndexMapped];
            lxPoint = caSpace.AddPoint(stationNumber, layerNumber, i, mcp.x, mcp.y, mcp.z, err.X(), err.Y(), err.Z());
            MCPoints[mcIndexMapped].lxPoints.push_back(lxPoint);
            lxPoint->mcPoints.push_back(&MCPoints[mcIndexMapped]);
          }
#endif  //MAKE_EFF_CALC
        }
      }
#endif  //MAKE_EFF_CALC
    }  // for (Int_t i = 0; i < nEnt; ++i)

    if (hitFileName != "") hitFile.EndEvent();
  }  //else (!parallMode)

  cout << "minXErr = " << minXErr << " ; minYErr = " << minYErr << endl << endl;

  // At last start invocation of CA procedures.
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;

  if (verbosity > 0) cout << "Execution duration 1 was: " << exeDuration << endl;

  if (calcMiddlePoints) caSpace.RestoreMiddlePoints();

  bTime.tv_sec  = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;

  if (verbosity > 0) cout << "Execution duration 2 was: " << exeDuration << endl;
#ifdef CLUSTER_MODE
  caSpace.BuildRays2();
#else  //CLUSTER_MODE
  //caSpace.BuildRays();
  caSpace.RefineMiddlePoints();
  caSpace.BuildRaysGlobal();
#endif  //CLUSTER_MODE
  bTime.tv_sec  = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  //Int_t runTime = exeDuration;
  runTime += exeDuration;

  if (verbosity > 0) cout << "Execution duration 3 was: " << exeDuration << endl;

#ifdef CLUSTER_MODE
  caSpace.ConnectNeighbours2();
#else  //CLUSTER_MODE
  caSpace.ConnectNeighbours();
#endif  //CLUSTER_MODE
  bTime.tv_sec  = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  runTime += exeDuration;

  if (verbosity > 0) cout << "Execution duration 4 was: " << exeDuration << endl;
#ifdef CLUSTER_MODE
  caSpace.Reconstruct2();
#else  //CLUSTER_MODE
  caSpace.Reconstruct();
#endif  //CLUSTER_MODE
  //caSpace.FitTracks();
  bTime.tv_sec  = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  runTime += exeDuration;

  wholeDuration += runTime;
  averageDuration = wholeDuration / nTimes;
  cout << "Average execution duration 3 + 4 + 5 was: " << averageDuration << endl;

  if (parallMode) {
    Int_t* pOutRunTime = reinterpret_cast<Int_t*>(const_cast<char*>(opt));
    *pOutRunTime       = runTime;
  }

  if (verbosity > 0) cout << "Execution duration 5 was: " << exeDuration << endl;

  //caSpace.FitTracks();
  //cout << "maxReferencedPtsIndex=" << maxReferencedPtsIndex << " mcPtsCount=" << mcPtsCount << " GEF=" << listMuchPts->GetEntriesFast() << endl;
  //cout << "Hits=" << muchPixelHits->GetEntriesFast() << " clusters=" << listMuchClusters->GetEntriesFast() << " digi ms=" << listMuchPixelDigiMatches->GetEntriesFast() << endl;
  //sleep(3);

  /////////////////////////////////////////////////////////////////////////////////
  // Join MUCH-tracks with STS-tracks.
  /////////////////////////////////////////////////////////////////////////////////

  if (linkWithSts) {
    // STS-tracks fulfilling the limitations peculiar for muons are picked up in the loop below.
    nEnt = listStsTracks->GetEntriesFast();

    for (int i = 0; i < nEnt; ++i) {
      CbmStsTrack* stsTrack = LX_DYNAMIC_CAST<CbmStsTrack*>(listStsTracks->At(i));

      scaltype lpa[5] = {static_cast<Float_t>(stsTrack->GetParamLast()->GetX()),
                         static_cast<Float_t>(stsTrack->GetParamLast()->GetY()),
                         static_cast<Float_t>(stsTrack->GetParamLast()->GetTx()),
                         static_cast<Float_t>(stsTrack->GetParamLast()->GetTy()),
                         static_cast<Float_t>(stsTrack->GetParamLast()->GetQp())};

      if (lpa[0] != lpa[0] || lpa[1] != lpa[1] || lpa[2] != lpa[2] || lpa[3] != lpa[3] || lpa[4] != lpa[4]) continue;

      CbmStsTrack aTrack = *stsTrack;

      Int_t pdgHypo = 13;

      if (stsTrack->GetParamLast()->GetTx() > stsTrack->GetParamFirst()->GetTx()) pdgHypo = -13;

      extFitter.DoFit(&aTrack, pdgHypo);
      scaltype chi2Prim = extFitter.GetChiToVertex(&aTrack, fPrimVtx);
      FairTrackParam params;
      extFitter.Extrapolate(&aTrack, fPrimVtx->GetZ(), &params);
      scaltype p  = 1 / params.GetQp();
      scaltype p2 = p * p;

      if (p2 < 3.0 * 3.0) continue;

      /*
      scaltype xDelta = 0.05;//5.0 * sqrt(params.GetCovariance(0, 0));
      scaltype yDelta = 0.05;//5.0 * sqrt(params.GetCovariance(1, 1));

      if (params.GetX() < -xDelta  || params.GetX() > xDelta || params.GetY() < -yDelta || params.GetY() > yDelta)
        continue;
*/

      scaltype tx2 = params.GetTx() * params.GetTx();
      scaltype ty2 = params.GetTy() * params.GetTy();
      scaltype pt2 = p2 * (tx2 + ty2) / (1 + tx2 + ty2);

      if (pt2 < 1.0) continue;

      LxExtTrack extTrack;
      extTrack.track = stsTrack;
      extTrack.extId = i;

#ifdef MAKE_EFF_CALC
      CbmTrackMatch* match = LX_DYNAMIC_CAST<CbmTrackMatch*>(listStsMatches->At(i));
      Int_t numberOfHits   = match->GetNofTrueHits() + match->GetNofWrongHits() + match->GetNofFakeHits();

      if (match->GetNofTrueHits() >= 0.7 * numberOfHits) {
        Int_t mcTrackId = match->GetMCTrackId();
        Int_t mappedId  = root2lxmctrackmap[mcTrackId];

        if (-1 != mappedId) {
          MCTracks[mappedId].externalTrack = stsTrack;
          extTrack.mcTrack                 = &MCTracks[mappedId];

          if (13 == MCTracks[mappedId].pdg || -13 == MCTracks[mappedId].pdg) {
#ifdef MAKE_HISTOS
            stsTrackChi2->Fill(chi2Prim);
            stsTrackX->Fill(params.GetX());
            stsTrackY->Fill(params.GetY());
#endif  //MAKE_HISTOS
          }
        }
      }
#endif  //MAKE_EFF_CALC

      caSpace.extTracks.push_back(extTrack);
    }

    cout << "External tracks are read" << endl;

    // Join reconstructed tracks with external tracks.
    caSpace.JoinExtTracks();
    cout << "External tracks are connected" << endl;
  }  // if(linkWithSts)

  // Measure a reconstruction efficiency.
#ifdef MAKE_EFF_CALC
  if (!parallMode) {
    MatchMCToReco();
    MatchRecoToMC();
  }
#endif  //MAKE_EFF_CALC
  if (Trigger()) {
    if (hasSignalInEvent) ++trueSignalTriggerings;

    ++falseSignalTriggerings;
  }

  {
    Double_t result = 100 * trueSignalTriggerings;
    result /= signalCounter;
    cout << "Signal true hypotheses: " << result << "% ( " << trueSignalTriggerings << " / " << signalCounter << " )"
         << endl;
    cout << "Signal all hypotheses: " << falseSignalTriggerings << endl;
  }

  // Draw some result.
  //static LxDraw drawer;

  //drawer.ClearView();
  //drawer.DrawMuch();
  //drawer.SaveCanvas("Geometry_");

  //drawer.ClearView();
  //drawer.DrawInputHits();
  //drawer.SaveCanvas("Hits_");
  //drawer.Ask();

  //drawer.ClearView();
  //drawer.DrawInputHits();
  //drawer.DrawMCTracks();
  //gPad->WaitPrimitive();
  //drawer.ClearView();
  //drawer.DrawInputHits();
  //drawer.DrawRecoTracks();
  //drawer.DrawExtTracks();
  //drawer.SaveCanvas("MC_");
  //gPad->WaitPrimitive();

  /*drawer.ClearView();
  drawer.DrawInputHits();
  drawer.DrawMCTracks();
  drawer.DrawRays();
  gPad->WaitPrimitive();*/

  //drawer.ClearView();
  //drawer.DrawInputHits();
  //drawer.DrawRecoTracks();
  //drawer.SaveCanvas("Reco_");
  //drawer.Ask();

  //drawer.ClearView();
  //drawer.DrawInputHits();
  //drawer.DrawRecoTracks();
  //drawer.DrawExtTracks();
  //drawer.Ask();

  if (generateInvMass) CalcInvMass();

  if (generateBackground) SaveEventTracks();

  // Finally save reconstructed tracks.
  if (!parallMode) SaveRecoTracks();

#ifdef MAKE_EFF_CALC
  // Hopefully we don't need these two maps any more.
  delete[] root2lxmctrackmap;
  delete[] root2lxmcpointmap;
#endif  //MAKE_EFF_CALC

  ++eventNumber;
  ++nTimes;
}

void LxFinder::SaveRecoTracks()
{
  Int_t trackNo = listRecoTracks->GetEntriesFast();

  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i) {
    LxTrack* recoTrack = *i;

    if (recoTrack->clone) continue;

    if (saveOnlyTriggeringTracks && !recoTrack->triggering) continue;

    LxExtTrack* stsTrack = recoTrack->externalTrack;

    if (0 == stsTrack) continue;

    CbmMuchTrack muchTrack;

    for (int j = 0; j < LXSTATIONS * LXLAYERS; ++j) {
      LxPoint* pPoint = recoTrack->points[j];

      if (0 != pPoint) muchTrack.AddHit(pPoint->hitId, kMUCHPIXELHIT);
    }

    muchTrack.SetChiSq(recoTrack->chi2);
    muchTrack.SetNDF(4 * recoTrack->length);  // Probably need to calculate it more accurately.
    muchTrack.SetPreviousTrackId(stsTrack->extId);

    FairTrackParam parFirst;
    parFirst = *stsTrack->track->GetParamLast();
    muchTrack.SetFlag(0);  // Good track.

    FairTrackParam parLast(parFirst);
    muchTrack.SetParamFirst(&parFirst);
    muchTrack.SetParamLast(&parLast);

    new ((*listRecoTracks)[trackNo++]) CbmMuchTrack(muchTrack);
  }
}

void LxFinder::CalcInvMass()
{
#ifdef MAKE_HISTOS
  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i) {
    LxTrack* firstTrack = *i;
    LxMCTrack* mcTrack1 = firstTrack->mcTrack;

    if (0 == firstTrack->externalTrack) continue;

    LxMCTrack* mcTrack2 = firstTrack->externalTrack->mcTrack;

    if (mcTrack1) {
      //      Int_t pdg1 = mcTrack1 ? mcTrack1->pdg : 10000;
      //map<Int_t, map<Int_t, int> >::iterator j = particleCounts.find(pdg1);

      //if (j == particleCounts.end())
      //j = particleCounts.insert(pair<Int_t, map<Int_t, int> > (pdg1, map<Int_t, int> ())).first;

      //      Int_t pdg2 = mcTrack2 ? mcTrack2->pdg : 10000;

      //map<Int_t, int>::iterator k = j->second.find(pdg2);

      //if (k != j->second.end())
      //++(k->second);
      //else
      //j->second.insert(pair<Int_t, int> (pdg2, 1));
    }

    CbmStsTrack t1 = *firstTrack->externalTrack->track;

    // Add entry to the respective chi2 histogram if needed. This can be used for placing chi2 cuts determining J/psi.
    // pdg == 13 | -13 -- it is a muon.
    // mother_ID < 0 -- it is a primary particle. URQMD doesn't produce primary muons. So it from PLUTO <=> is J/psi.

    if (generateChi2) {
      scaltype normalizedChi2 = firstTrack->chi2 / (firstTrack->length * 4);  // length * 4 == NDF.

      if (mcTrack2 && (mcTrack2->pdg == 13 || mcTrack2->pdg == -13) && mcTrack2->mother_ID < 0)
        signalChi2Histo->Fill(normalizedChi2);
      else
        backgroundChi2Histo->Fill(normalizedChi2);
    }

    if (!mcTrack2 || (mcTrack2->pdg != 13 && mcTrack2->pdg != -13) || mcTrack2->mother_ID >= 0) continue;

    extFitter.DoFit(&t1, 13);
    scaltype chi2Prim = extFitter.GetChiToVertex(&t1, fPrimVtx);
    FairTrackParam t1param;
    extFitter.Extrapolate(&t1, fPrimVtx->GetZ(), &t1param);

    if (t1param.GetQp() <= 0) continue;

    scaltype p1   = 1 / t1param.GetQp();
    scaltype tx12 = t1param.GetTx() * t1param.GetTx();
    scaltype ty12 = t1param.GetTy() * t1param.GetTy();
    scaltype pt12 = p1 * p1 * (tx12 + ty12) / (1 + tx12 + ty12);

    if (pt12 < 1) continue;

    CbmKFTrack muPlus(t1);

    for (list<LxTrack*>::iterator j = caSpace.tracks.begin(); j != caSpace.tracks.end(); ++j) {
      LxTrack* secondTrack = *j;

      if (0 == secondTrack->externalTrack) continue;

      LxMCTrack* mcSecondTrack = secondTrack->externalTrack->mcTrack;

      if (!mcSecondTrack || (mcSecondTrack->pdg != 13 && mcSecondTrack->pdg != -13) || mcSecondTrack->mother_ID >= 0)
        continue;

      CbmStsTrack t2 = *secondTrack->externalTrack->track;
      extFitter.DoFit(&t2, 13);
      chi2Prim = extFitter.GetChiToVertex(&t2, fPrimVtx);
      FairTrackParam t2param;
      extFitter.Extrapolate(&t2, fPrimVtx->GetZ(), &t2param);

      if (t2param.GetQp() >= 0) continue;

      scaltype p2   = 1 / t2param.GetQp();
      scaltype tx22 = t2param.GetTx() * t2param.GetTx();
      scaltype ty22 = t2param.GetTy() * t2param.GetTy();
      scaltype pt22 = p2 * p2 * (tx22 + ty22) / (1 + tx22 + ty22);

      if (pt22 < 1) continue;

      CbmKFTrack muMinus(t2);
      vector<CbmKFTrackInterface*> kfData;
      kfData.push_back(&muPlus);
      kfData.push_back(&muMinus);
      //CbmKFParticle DiMu;
      //DiMu.Construct(kfData, 0);
      //DiMu.TransportToDecayVertex();
      //scaltype m, merr;
      //iMu.GetMass(m, merr);

      //massHisto->Fill(m);
    }
  }
#endif  //MAKE_HISTOS
}

//#ifdef MAKE_HISTOS
// It also deletes the histogram.
static void SaveHisto(TH1* histo, const char* name)
{
  TFile fh(name, "RECREATE");
  histo->Write();
  fh.Close();
  delete histo;
}

static void SaveHisto(TH2* histo, const char* name)
{
  TFile fh(name, "RECREATE");
  histo->Write();
  fh.Close();
  delete histo;
}
//#endif//MAKE_HISTOS

void LxFinder::FinishTask()
{
  TString complexFileName;

  if (generateInvMass) SaveInvMass();

  if (generateBackground) SaveBackground();

  /*if (generateChi2)
  {
    SaveSignalChi2();
    SaveBackgroundChi2();
  }

  cout << "Statistics on particles found:" << endl;

  for (map<Int_t, map<Int_t, int> >::iterator i = particleCounts.begin(); i != particleCounts.end(); ++i)
  {
    string muchParticle = (10000 <= i->first || -10000 >= i->first) ? "Undefined MUCH particle" : TDatabasePDG::Instance()->GetParticle(i->first)->GetName();
    cout << "For MUCH particle [ " << muchParticle << " ] ( PDG = " << i->first << " ) found STS joints to:" << endl;

    for (map<Int_t, int>::iterator j = i->second.begin(); j != i->second.end(); ++j)
    {
      string stsParticle = (10000 <= j->first || -10000 >= j->first) ? "Undefined STS particle" : TDatabasePDG::Instance()->GetParticle(j->first)->GetName();
      cout << "\t" << j->second << " [ " << stsParticle << " ] ( PDG = " << j->first << " )" << endl;
    }
  }*/
  TFile* curFile = TFile::CurrentFile();
#ifdef MAKE_HISTOS
#ifdef MAKE_DISPERSE_2D_HISTOS
  char histoFileName[128];

  for (int i = 0; i < 6; ++i) {
    sprintf(histoFileName, "disperseL_histo_%d.root", i);
    SaveHisto(disperseLHistos[i], histoFileName);
    sprintf(histoFileName, "disperseR_histo_%d.root", i);
    SaveHisto(disperseRHistos[i], histoFileName);
    sprintf(histoFileName, "disperseD_histo_%d.root", i);
    SaveHisto(disperseDHistos[i], histoFileName);
  }
#endif  //MAKE_DISPERSE_2D_HISTOS
#endif  //MAKE_HISTOS

#ifdef MAKE_HISTOS
  {
    TFile fh("effByMomentumProfile.root", "RECREATE");
    effByMomentumProfile->Write();
    fh.Close();
    delete effByMomentumProfile;
  }

  SaveHisto(stsTrackChi2, "stsTrackChi2.root");
  SaveHisto(stsTrackX, "stsTrackX.root");
  SaveHisto(stsTrackY, "stsTrackY.root");

  SaveHisto(muPlusDtxHisto, "muPlusDtxHisto.root");
  SaveHisto(muMinusDtxHisto, "muMinusDtxHisto.root");
  SaveHisto(signalXAtZ0, "signalXAtZ0.root");
  SaveHisto(signalYAtZ0, "signalYAtZ0.root");
  SaveHisto(bgrXAtZ0, "bgrXAtZ0.root");
  SaveHisto(bgrYAtZ0, "bgrYAtZ0.root");
  SaveHisto(signalInterTracksDistance, "signalInterTracksDistance.root");
  SaveHisto(bgrInterTracksDistance, "bgrInterTracksDistance.root");
  SaveHisto(signalTanSigns, "signalTanSigns.root");
  SaveHisto(signalCoordSigns, "signalCoordSigns.root");
  SaveHisto(bgrTanSigns, "bgrTanSigns.root");
  SaveHisto(bgrCoordSigns, "bgrCoordSigns.root");
  SaveHisto(numberOfTracks, "numberOfTracks.root");
  SaveHisto(signalInterTracksDistanceOn1st, "signalInterTracksDistanceOn1st.root");
  SaveHisto(bgrInterTracksDistanceOn1st, "bgrInterTracksDistanceOn1st.root");
  SaveHisto(bgrInterTracksDistanceOn1stSigns, "bgrInterTracksDistanceOn1stSigns.root");
  SaveHisto(signalInterTracksAngle, "signalInterTracksAngle.root");
  SaveHisto(bgrInterTracksAngle, "bgrInterTracksAngle.root");
  SaveHisto(signalInterTrackCorrDA, "signalInterTrackCorrDA.root");
  SaveHisto(bgrInterTrackCorrDA, "bgrInterTrackCorrDA.root");
  SaveHisto(muchMomErrSig, "muchMomErrSig.root");
  SaveHisto(muchMomErrBgr, "muchMomErrBgr.root");
#ifdef MAKE_TRIGGERING_HISTOS
  complexFileName = "triggeringAllTracksVertices";
  complexFileName += fileSaveSuffix;
  complexFileName += ".root";
  SaveHisto(triggeringAllTracksVertices, complexFileName);
  complexFileName = "triggeringDistTracksVertices";
  complexFileName += fileSaveSuffix;
  complexFileName += ".root";
  SaveHisto(triggeringDistTracksVertices, complexFileName);
  complexFileName = "triggeringSignTracksVertices";
  complexFileName += fileSaveSuffix;
  complexFileName += ".root";
  SaveHisto(triggeringSignTracksVertices, complexFileName);
  complexFileName = "triggeringTrigTracksVertices";
  complexFileName += fileSaveSuffix;
  complexFileName += ".root";
  SaveHisto(triggeringTrigTracksVertices, complexFileName);
#endif  //MAKE_TRIGGERING_HISTOS

#endif  //MAKE_HISTOS

  TFile::CurrentFile() = curFile;

#ifdef MAKE_EFF_CALC
  incomplete_events.close();
#endif  //MAKE_EFF_CALC

  delete superEventData;

#ifdef MAKE_EFF_CALC
  ofstream fjtofs("../false_jpsi_triggerings.txt", ios_base::out | ios_base::app);
  fjtofs << falseSignalTriggerings << endl;
#endif  //MAKE_EFF_CALC

  complexFileName = "particles_count_all";
  complexFileName += fileSaveSuffix;
  complexFileName += ".txt";
  ofstream pcaofs(complexFileName);

  for (map<string, unsigned int>::iterator i = particlesCountAll.begin(); i != particlesCountAll.end(); ++i)
    pcaofs << i->first << ": " << i->second << endl;

  complexFileName = "particles_count_sign";
  complexFileName += fileSaveSuffix;
  complexFileName += ".txt";
  ofstream pcsofs(complexFileName);

  for (map<string, unsigned int>::iterator i = particlesCountSign.begin(); i != particlesCountSign.end(); ++i)
    pcsofs << i->first << ": " << i->second << endl;

  complexFileName = "particles_count_dist";
  complexFileName += fileSaveSuffix;
  complexFileName += ".txt";
  ofstream pcdofs(complexFileName);

  for (map<string, unsigned int>::iterator i = particlesCountDist.begin(); i != particlesCountDist.end(); ++i)
    pcdofs << i->first << ": " << i->second << endl;

  complexFileName = "particles_count_trig";
  complexFileName += fileSaveSuffix;
  complexFileName += ".txt";
  ofstream pctofs(complexFileName);

  for (map<string, unsigned int>::iterator i = particlesCountTrig.begin(); i != particlesCountTrig.end(); ++i)
    pctofs << i->first << ": " << i->second << endl;

  if (hitFileName != "") hitFile.Close();

  FairTask::FinishTask();
}

void LxFinder::SaveInvMass()
{
#ifdef MAKE_HISTOS
  TFile* curFile = TFile::CurrentFile();

  TFile fh("jpsi_inv_mass_histo.root", "RECREATE");
  massHisto->Write();
  fh.Close();
  delete massHisto;

  TFile::CurrentFile() = curFile;
#endif  //MAKE_HISTOS
}

void LxFinder::SaveBackground()
{
#ifdef MAKE_HISTOS
  for (list<CbmStsTrack>::iterator i = positiveTracks.begin(); i != positiveTracks.end(); ++i) {
    CbmStsTrack& t1 = *i;
    CbmKFTrack muPlus(t1);

    for (list<CbmStsTrack>::iterator j = negativeTracks.begin(); j != negativeTracks.end(); ++j) {
      CbmStsTrack& t2 = *j;
      CbmKFTrack muMinus(t2);
      vector<CbmKFTrackInterface*> kfData;
      kfData.push_back(&muPlus);
      kfData.push_back(&muMinus);
      //CbmKFParticle DiMu;
      //DiMu.Construct(kfData, 0);
      //DiMu.TransportToDecayVertex();
      //scaltype m, merr;
      //DiMu.GetMass(m, merr);

      //for (int k = 0; k < 1000; ++k)
      //backgroundMassHisto->Fill(m);
    }
  }

  TFile* curFile = TFile::CurrentFile();

  TFile fh("tracks_tree.root", "RECREATE");
  superEventTracks->Write();
  fh.Close();
  delete superEventTracks;

  TFile::CurrentFile() = curFile;
#endif  //MAKE_HISTOS
}

void LxFinder::SaveSignalChi2()
{
#ifdef MAKE_HISTOS
  TFile* curFile = TFile::CurrentFile();

  TFile fh("signal_chi2_histo.root", "RECREATE");
  signalChi2Histo->Write();
  fh.Close();
  delete signalChi2Histo;

  TFile::CurrentFile() = curFile;
#endif  //MAKE_HISTOS
}

void LxFinder::SaveBackgroundChi2()
{
#ifdef MAKE_HISTOS
  TFile* curFile = TFile::CurrentFile();

  TFile fh("background_chi2_histo.root", "RECREATE");
  backgroundChi2Histo->Write();
  fh.Close();
  delete backgroundChi2Histo;

  TFile::CurrentFile() = curFile;
#endif  //MAKE_HISTOS
}

bool LxFinder::ReadEvent(Int_t evNum) { return hitFile.ReadEvent(evNum); }
