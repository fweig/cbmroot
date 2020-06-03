#include <sys/time.h>
#include <iostream>
#include "Lx.h"
#include "CbmMuchGeoScheme.h"
#include "LxDraw.h"
#include "CbmMCTrack.h"
#include "TDatabasePDG.h"
#include "CbmMuchPoint.h"
#include "CbmStsPoint.h"
#include "TGeoManager.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchTrack.h"
#include "CbmKFTrack.h"
#include "CbmKFParticle.h"
#include "CbmStsAddress.h"

ClassImp(LxFinderTriplet)

using namespace std;

LxFinderTriplet* LxFinderTriplet::fInstance = 0;
TH1F* LxFinderTriplet::massHisto = 0;
//TH1F* LxFinderTriplet::backgroundMassHisto = 0;
CbmStsTrack* LxFinderTriplet::superEventData = 0;
TTree* LxFinderTriplet::superEventTracks = 0;
TH1F* LxFinderTriplet::signalChi2Histo = 0;
TH1F* LxFinderTriplet::backgroundChi2Histo = 0;
TH1F* LxFinderTriplet::unknownChi2Histo = 0;
TProfile* LxFinderTriplet::effByMomentumProfile = 0;
TH1F* LxFinderTriplet::signalInterTracksDistance = 0;
TH1F* LxFinderTriplet::bgrInterTracksDistance = 0;
TH1F* LxFinderTriplet::signalSignDefect = 0;
TH1F* LxFinderTriplet::bgrSignDefect = 0;
TH1F* LxFinderTriplet::signalYAtZ0 = 0;
TH1F* LxFinderTriplet::bgrYAtZ0 = 0;
TH1F* LxFinderTriplet::signalYAtZ0_2 = 0;
TH1F* LxFinderTriplet::bgrYAtZ0_2 = 0;

#ifdef MAKE_DISPERSE_2D_HISTOS
TProfile2D* disperseLHistos[LXSTATIONS];
TProfile2D* disperseRHistos[LXSTATIONS];
TProfile2D* disperseDHistos[LXSTATIONS];
#endif//MAKE_DISPERSE_2D_HISTOS

static TH1F* stsTrackChi2 = 0;
static TH1F* stsTrackX = 0;
static TH1F* stsTrackY = 0;

LxFinderTriplet* LxFinderTriplet::Instance()
{
  return fInstance;
}

  std::list<CbmStsTrack> positiveTracks;
  std::list<CbmStsTrack> negativeTracks;
  bool generateInvMass;
  bool generateBackground;
  bool generateChi2;
  bool linkWithSts;
  bool useMCPInsteadOfHits;
  bool calcMiddlePoints;
  Double_t cutCoeff;
  bool pPtCut;
  std::vector<LxMCPoint> MCPoints;// Points should lay here in the same order as in listMuchPts.
  std::vector<LxMCTrack> MCTracks;// Tracks should lay here in the same order as in listMCTracks.
  std::list<LxStsMCPoint> MCStsPoints;
  std::list<LxStsMCPoint*> MCStsPointsByStations[8];
#ifdef MAKE_DISPERSE_2D_HISTOS
  std::list<LxMCPoint*> MCPointsByStations[LXSTATIONS][LXLAYERS];
  Double_t zCoordsByStations[LXSTATIONS][LXLAYERS];
#endif//MAKE_DISPERSE_2D_HISTOS
  LxSpace caSpace;
  std::map<Int_t, std::map<Int_t, int> > particleCounts;
#ifdef MAKE_EFF_CALC
  std::ofstream incomplete_events;// Events where not all tracks are reconstructed.
#endif//MAKE_EFF_CALC
  Int_t eventNumber;
#ifdef CALC_MUCH_DETECTORS_EFF
  Int_t mcPointsCount;
  Int_t mcPointsTriggered;
#endif//CALC_MUCH_DETECTORS_EFF


LxFinderTriplet::LxFinderTriplet() : muchPixelHits(0), listMCTracks(0), listMuchPts(0), listMuchClusters(0),
    listMuchPixelDigiMatches(0), listStsTracks(0), listStsMatches(0), listStsPts(0), listRecoTracks(0),
    effCounter(*this), extFitter(), fPrimVtx(0), positiveTracks(), negativeTracks(), generateInvMass(false), generateBackground(false), generateChi2(false),
    linkWithSts(true), useMCPInsteadOfHits(false), calcMiddlePoints(true), cutCoeff(4.0), pPtCut(true), MCPoints(), MCTracks(),
    MCStsPoints()
    , MCStsPointsByStations()
#ifdef MAKE_DISPERSE_2D_HISTOS
    , MCPointsByStations()
    , zCoordsByStations()
#endif//MAKE_DISPERSE_2D_HISTOS
    , caSpace()
    , particleCounts()
#ifdef MAKE_EFF_CALC
    , incomplete_events()
#endif//MAKE_EFF_CALC
    , eventNumber(0)
#ifdef CALC_MUCH_DETECTORS_EFF
    , mcPointsCount(0), mcPointsTriggered(0)
#endif//CALC_MUCH_DETECTORS_EFF
{
  fInstance = this;
}

LxFinderTriplet::~LxFinderTriplet()
{
}

TString lxFinderParticleType = "jpsi";

static bool GetHistoRMS(const char* histoNameBase, Int_t histoNumber, Double_t& retVal)
{
  char name[256];
  char dir_name[256];
  sprintf(dir_name, "configuration.%s", lxFinderParticleType.Data());
  bool result = false;
  sprintf(name, "%s/%s_%d.root", dir_name, histoNameBase, histoNumber);
  TFile* curFile = TFile::CurrentFile();
  TFile* f = new TFile(name);

  if (!f->IsZombie())
  {
    sprintf(name, "%s_%d", histoNameBase, histoNumber);
    TH1F* h = static_cast<TH1F*> (f->Get(name));
    retVal = h->GetRMS();
    result = true;
  }

  delete f;
  TFile::CurrentFile() = curFile;
  return result;
}

static bool GetHistoCOV(const char* histoNameBase, Int_t histoNumber, Int_t axis1, Int_t axis2, Double_t& retVal)
{
  char name[256];
  bool result = false;
  sprintf(name, "configuration/%s_%d.root", histoNameBase, histoNumber);
  TFile* curFile = TFile::CurrentFile();
  TFile* f = new TFile(name);

  if (!f->IsZombie())
  {
    sprintf(name, "%s_%d", histoNameBase, histoNumber);
    TH2F* h = static_cast<TH2F*> (f->Get(name));
    retVal = h->GetCovariance(axis1, axis2);
    result = true;
  }

  delete f;
  TFile::CurrentFile() = curFile;
  return result;
}

InitStatus LxFinderTriplet::Init()
{
  static Int_t nTimes = 1;
  cout << "LxFinderTriplet::Init() called at " << nTimes++ << " time" << endl;

  superEventData = new CbmStsTrack;

  FairRootManager *fManager = FairRootManager::Instance();

  muchPixelHits = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("MuchPixelHit"));
  listMCTracks = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("MCTrack"));
  listMuchPts = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("MuchPoint"));
  listMuchClusters = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("MuchCluster"));
  listMuchPixelDigiMatches = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("MuchDigiMatch"));
  listStsTracks = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("StsTrack"));
  listStsMatches = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("StsTrackMatch"));
  listStsPts = LX_DYNAMIC_CAST<TClonesArray*> (fManager->GetObject("StsPoint"));
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
  fPrimVtx = new CbmVertex;

  // Read Z-positions of MUCH station layers and save them in LxLayer objects.

  for (Int_t i = 0; i < LXSTATIONS; ++i)
  {
    LxStation* aStation = caSpace.stations[i];

    for (Int_t j = 0; j < LXLAYERS; ++j)
    {
      TString muchStLrPath = Form("/cave_1/much_0/muchstation%02i_0/muchstation%02ilayer%01i_0",
          i + 1, i + 1, j + 1);
      gGeoManager->cd(muchStLrPath.Data());
      Double_t localCoords[3] = {0., 0., 0.};
      Double_t globalCoords[3];
      gGeoManager->LocalToMaster(localCoords, globalCoords);
      aStation->layers[j]->zCoord = globalCoords[2];
#ifdef MAKE_DISPERSE_2D_HISTOS
      zCoordsByStations[i][j] = globalCoords[2];
#endif//MAKE_DISPERSE_2D_HISTOS
    }

    aStation->zCoord = aStation->layers[LXMIDDLE]->zCoord;

    bool readHistoResult = true;

    readHistoResult &= GetHistoRMS("muchInStationXDispLeft", i, aStation->xDispLeft);
    aStation->xDispLeft2 = aStation->xDispLeft * aStation->xDispLeft;
    readHistoResult &= GetHistoRMS("muchInStationYDispLeft", i, aStation->yDispLeft);
    aStation->yDispLeft2 = aStation->yDispLeft * aStation->yDispLeft;
    readHistoResult &= GetHistoRMS("muchInStationXDispRight", i, aStation->xDispRight);
    aStation->xDispRight2 = aStation->xDispRight * aStation->xDispRight;
    readHistoResult &= GetHistoRMS("muchInStationYDispRight", i, aStation->yDispRight);
    aStation->yDispRight2 = aStation->yDispRight * aStation->yDispRight;
    readHistoResult &= GetHistoRMS("muchInStationXDispRL", i, aStation->xDispRL);
    aStation->xDispRL2 = aStation->xDispRL * aStation->xDispRL;
    readHistoResult &= GetHistoRMS("muchInStationYDispRL", i, aStation->yDispRL);
    aStation->yDispRL2 = aStation->yDispRL * aStation->yDispRL;

    if (i > 0)
    {
#ifdef OUT_DISP_BY_TRIPLET_DIR
      readHistoResult &= GetHistoRMS("muchOutStationXDispByTriplet", i, aStation->xOutDispTriplet);
      aStation->xOutDispTriplet2 = aStation->xOutDispTriplet * aStation->xOutDispTriplet;
      readHistoResult &= GetHistoRMS("muchOutStationYDispByTriplet", i, aStation->yOutDispTriplet);
      aStation->yOutDispTriplet2 = aStation->yOutDispTriplet * aStation->yOutDispTriplet;
#else//OUT_DISP_BY_TRIPLET_DIR
      readHistoResult &= GetHistoRMS("muchOutStationXDispByVertex", i, aStation->xOutDispVertex);
      aStation->xOutDispVertex2 = aStation->xOutDispVertex * aStation->xOutDispVertex;
      readHistoResult &= GetHistoRMS("muchOutStationYDispByVertex", i, aStation->yOutDispVertex);
      aStation->yOutDispVertex2 = aStation->yOutDispVertex * aStation->yOutDispVertex;
#endif//OUT_DISP_BY_TRIPLET_DIR

#ifdef USE_SEGMENTS
      LxStation* anLStation = caSpace.stations[i - 1];
      readHistoResult &= GetHistoRMS("muchOutStationTxBreakLeft", i, anLStation->txBreakRight);
      anLStation->txBreakRight2 = anLStation->txBreakRight * anLStation->txBreakRight;
      readHistoResult &= GetHistoRMS("muchOutStationTyBreakLeft", i, anLStation->tyBreakRight);
      anLStation->tyBreakRight2 = anLStation->tyBreakRight * anLStation->tyBreakRight;
      readHistoResult &= GetHistoRMS("muchOutStationTxBreakRight", i, aStation->txBreakLeft);
      aStation->txBreakLeft2 = aStation->txBreakLeft * aStation->txBreakLeft;
      readHistoResult &= GetHistoRMS("muchOutStationTyBreakRight", i, aStation->tyBreakLeft);
      aStation->tyBreakLeft2 = aStation->tyBreakLeft * aStation->tyBreakLeft;
#else//USE_SEGMENTS
      readHistoResult &= GetHistoRMS("muchTripletTxBreak", i, aStation->txInterTripletBreak);
      aStation->txInterTripletBreak2 = aStation->txInterTripletBreak * aStation->txInterTripletBreak;
      readHistoResult &= GetHistoRMS("muchTripletTyBreak", i, aStation->tyInterTripletBreak);
      aStation->tyInterTripletBreak2 = aStation->tyInterTripletBreak * aStation->tyInterTripletBreak;
#endif//USE_SEGMENTS

#ifdef CLUSTER_MODE
      if (!GetHistoRMS("muchClusterXDispHisto", i, rms))
        return kFATAL;

      aStation->clusterXLimit = rms;
      aStation->clusterXLimit2 = rms * rms;

      if (!GetHistoRMS("muchClusterYDispHisto", i, rms))
        return kFATAL;

      aStation->clusterYLimit = rms;
      aStation->clusterYLimit2 = rms * rms;

      if (!GetHistoRMS("muchClusterTxDispHisto", i, rms))
        return kFATAL;

      aStation->clusterTxLimit = rms;
      aStation->clusterTxLimit2 = rms * rms;

      if (!GetHistoRMS("muchClusterTyDispHisto", i, rms))
        return kFATAL;

      aStation->clusterTyLimit = rms;
      aStation->clusterTyLimit2 = rms * rms;
#endif//CLUSTER_MODE

#ifdef USE_SEGMENTS
      if (i < LXSTATIONS - 1)
      {
        readHistoResult &= GetHistoRMS("muchSegmentTxBreakHisto", i, aStation->txInterStationBreak);
        aStation->txInterStationBreak2 = aStation->txInterStationBreak * aStation->txInterStationBreak;
        readHistoResult &= GetHistoRMS("muchSegmentTyBreakHisto", i, aStation->tyInterStationBreak);
        aStation->tyInterStationBreak2 = aStation->tyInterStationBreak * aStation->tyInterStationBreak;
      }
#endif//USE_SEGMENTS
    }

    if (!readHistoResult)
      return kFATAL;

#ifdef USE_KALMAN_FIT
    if (i < LXSTATIONS - 1)
    {
      for (Int_t j = 0; j <= 1; ++j)
      {
        for (Int_t k = 0; k <= 1; ++k)
        {
          //if (!GetHistoCOV("muchXTxCovHisto", i, j + 1, k + 1, aStation->MSNoise[0][j][k]))
            //return kFATAL;

          //if (!GetHistoCOV("muchYTyCovHisto", i, j + 1, k + 1, aStation->MSNoise[1][j][k]))
            //return kFATAL;
        }
      }
    }
#endif//USE_KALMAN_FIT
  }

  // Create an output array.
  listRecoTracks = new TClonesArray("CbmMuchTrack", 100);
  fManager->Register("MuchTrack", "Much", listRecoTracks, IsOutputBranchPersistent("MuchTrack"));

  if (generateInvMass)
    massHisto = new TH1F("jpsi_mass", "jpsi_mass", 100, 2., 4.);

  if (generateBackground)
  {
    //backgroundMassHisto = new TH1F("background_mass", "background_mass", 400, 2., 4.);
    superEventTracks = new TTree("SuperEventTracks", "Tracks for building a super event");
    superEventTracks->Branch("tracks", "CbmStsTrack", &superEventData);
  }

  //if (generateChi2)
  //{
    signalChi2Histo = new TH1F("signal_chi2", "signal_chi2", 200, 0., 20.);
    signalChi2Histo->StatOverflows();
    backgroundChi2Histo = new TH1F("background_chi2", "background_chi2", 200, 0., 20.);
    backgroundChi2Histo->StatOverflows();
    unknownChi2Histo = new TH1F("unknown_chi2", "unknown_chi2", 200, 0., 20.);
    unknownChi2Histo->StatOverflows();
  //}

#ifdef MAKE_HISTOS
  char histoName[128];

  for (int i = 0; i < 6; ++i)
  {
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
#endif//MAKE_DISPERSE_2D_HISTOS
  }

#endif//MAKE_HISTOS

  effByMomentumProfile = new TProfile("Reconstruction efficiency by momentum", "Reconstruction efficiency by momentum",
      100, 0.0, 50.0, 0.0, 100.0);

  stsTrackChi2 = new TH1F("stsTrackChi2", "stsTrackChi2", 200, 0., 10.);
  stsTrackChi2->StatOverflows();
  stsTrackX = new TH1F("stsTrackX", "stsTrackX", 200, -0.2, 0.2);
  stsTrackX->StatOverflows();
  stsTrackY = new TH1F("stsTrackY", "stsTrackY", 200, -0.2, 0.2);
  stsTrackY->StatOverflows();

  signalInterTracksDistance = new TH1F("signalInterTracksDistance", "Distance between signal tracks", 200, 0.0, 40.0);
  signalInterTracksDistance->StatOverflows();
  bgrInterTracksDistance = new TH1F("bgrInterTracksDistance", "Distance between background tracks", 200, 0.0, 40.0);
  bgrInterTracksDistance->StatOverflows();
  signalSignDefect = new TH1F("signalSignDefect", "signalSignDefect", 200, -0.15, 0.15);
  signalSignDefect->StatOverflows();
  bgrSignDefect = new TH1F("bgrSignDefect", "bgrSignDefect", 200, -0.15, 0.15);
  bgrSignDefect->StatOverflows();
  signalYAtZ0 = new TH1F("signalYAtZ0", "Signal track Y at Z=0", 200, -40.0, 40.0);
  signalYAtZ0->StatOverflows();
  bgrYAtZ0 = new TH1F("bgrYAtZ0", "Background track Y at Z=0", 200, -40.0, 40.0);
  bgrYAtZ0->StatOverflows();
  signalYAtZ0_2 = new TH1F("signalYAtZ0_2", "Signal track Y at Z=0 (2)", 200, -40.0, 40.0);
  signalYAtZ0_2->StatOverflows();
  bgrYAtZ0_2 = new TH1F("bgrYAtZ0_2", "Background track Y at Z=0 (2)", 200, -40.0, 40.0);
  bgrYAtZ0_2->StatOverflows();

#ifdef MAKE_EFF_CALC
  incomplete_events.open("incomplete_events.txt");
#endif//MAKE_EFF_CALC

  return kSUCCESS;
}

static Int_t nTimes = 1;

void LxFinderTriplet::Exec(Option_t*)
{
  cout << "LxFinderTriplet::Exec() called at " << nTimes++ << " time" << endl;
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
  for (int i = 0; i < LXSTATIONS; ++i)
  {
    for (int j = 0; j < LXLAYERS; ++j)
      MCPointsByStations[i][j].clear();
  }
#endif//MAKE_DISPERSE_2D_HISTOS

  caSpace.Clear();
  listRecoTracks->Clear();

  // Read MC tracks
  Int_t nEnt = listMCTracks->GetEntries();
  cout << "There are: " << nEnt << " of MC tracks" << endl;
  LxMCTrack mcTrack;

  MCTracks.reserve(nEnt);
  Int_t* root2lxmctrackmap = new Int_t[nEnt];
  Int_t mapCnt = 0;

  for (int i = 0; i < nEnt; ++i)
  {
    CbmMCTrack* mct = LX_DYNAMIC_CAST<CbmMCTrack*> (listMCTracks->At(i));
    mcTrack.p = mct->GetP();
    Int_t pdgCode = mct->GetPdgCode();

    if (abs(pdgCode) >= 10000)
    {
      root2lxmctrackmap[i] = -1;
      continue;
    }

    root2lxmctrackmap[i] = mapCnt++;

    mcTrack.q = TDatabasePDG::Instance()->GetParticle(pdgCode)->Charge() / 3.0;
    mcTrack.x = mct->GetStartX();
    mcTrack.y = mct->GetStartY();
    mcTrack.z = mct->GetStartZ();
    mcTrack.px = mct->GetPx();
    mcTrack.py = mct->GetPy();
    mcTrack.pz = mct->GetPz();
    mcTrack.mother_ID = mct->GetMotherId();
    mcTrack.fUniqueID = mct->GetUniqueID();
    mcTrack.pdg = pdgCode;
    mcTrack.externalTrack = 0;
    MCTracks.push_back(mcTrack);
  }

  nEnt = listStsPts->GetEntries();
  cout << "There are: " << nEnt << " of STS MC points" << endl;

  for (int i = 0; i < nEnt; ++i)
  {
    TVector3 xyzI, PI, xyzO, PO;
    CbmStsPoint* stsPt = LX_DYNAMIC_CAST<CbmStsPoint*> (listStsPts->At(i));

    if (0 == stsPt)
      continue;

    LxStsMCPoint stsMCPoint;
    stsPt->Position(xyzI);
    stsPt->Momentum(PI);
    stsPt->PositionOut(xyzO);
    stsPt->MomentumOut(PO);
    TVector3 xyz = .5 * (xyzI + xyzO);
    TVector3 P = .5 * (PI + PO);
    stsMCPoint.x  = xyz.X();
    stsMCPoint.y  = xyz.Y();
    stsMCPoint.z  = xyz.Z();
    stsMCPoint.px = P.X();
    stsMCPoint.py = P.Y();
    stsMCPoint.pz = P.Z();
    stsMCPoint.p = sqrt(fabs(stsMCPoint.px * stsMCPoint.px + stsMCPoint.py * stsMCPoint.py + stsMCPoint.pz * stsMCPoint.pz));
    stsMCPoint.stationNumber = CbmStsAddress::GetElementId(stsPt->GetDetectorID(), kStsStation);
    Int_t trackId = root2lxmctrackmap[stsPt->GetTrackID()];

    if (-1 != trackId)
    {
      stsMCPoint.mcTrack = &MCTracks[trackId];
      MCStsPoints.push_back(stsMCPoint);
      MCTracks[trackId].stsPoints[stsMCPoint.stationNumber].push_back(&MCStsPoints.back());
      MCStsPointsByStations[stsMCPoint.stationNumber].push_back(&MCStsPoints.back());
    }
  }

  nEnt = listMuchPts->GetEntries();
  cout << "There are: " << nEnt << " of MUCH MC points" << endl;
  LxMCPoint mcPoint;

  MCPoints.reserve(nEnt);
  Int_t* root2lxmcpointmap = new Int_t[nEnt];// Unfortunately we have to use this map because in the loop
                                             // below some iterations can not to produce a point.
  mapCnt = 0;
//  Int_t mcPtsCount = nEnt;
  Int_t maxReferencedPtsIndex = 0;

  for (int i = 0; i < nEnt; ++i)
  {
    TVector3 xyzI, PI, xyzO, PO;
    CbmMuchPoint* pt = LX_DYNAMIC_CAST<CbmMuchPoint*> (listMuchPts->At(i));

    if (!pt)
    {
      root2lxmcpointmap[i] = -1;
      continue;
    }

    Int_t trackId = root2lxmctrackmap[pt->GetTrackID()];

    if (-1 == trackId)
    {
      root2lxmcpointmap[i] = -1;
      continue;
    }

    mcPoint.trackId = trackId;
    mcPoint.track = &MCTracks[trackId];

    root2lxmcpointmap[i] = mapCnt++;

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    TVector3 xyz = .5 * (xyzI + xyzO);
    TVector3 P = .5 * (PI + PO);
    mcPoint.x  = xyz.X();
    mcPoint.y  = xyz.Y();
    mcPoint.z  = xyz.Z();
    mcPoint.px = P.X();
    mcPoint.py = P.Y();
    mcPoint.pz = P.Z();
    mcPoint.p = sqrt(fabs(mcPoint.px * mcPoint.px + mcPoint.py * mcPoint.py + mcPoint.pz * mcPoint.pz));
    mcPoint.stationNumber = CbmMuchGeoScheme::GetStationIndex(pt->GetDetectorId());
    mcPoint.layerNumber = CbmMuchGeoScheme::GetLayerIndex(pt->GetDetectorId());
    MCPoints.push_back(mcPoint);
    Int_t ptId = root2lxmcpointmap[i];

/*
#ifdef MAKE_HISTOS
    Double_t trackPt2 = MCTracks[trackId].px * MCTracks[trackId].px + MCTracks[trackId].py * MCTracks[trackId].py;
#endif//MAKE_HISTOS
*/

    MCTracks[trackId].Points.push_back(&MCPoints[ptId]);
#ifdef MAKE_DISPERSE_2D_HISTOS
    MCPointsByStations[mcPoint.stationNumber][mcPoint.layerNumber].push_back(&MCPoints[ptId]);
#endif//MAKE_DISPERSE_2D_HISTOS
  }

#ifdef MAKE_HISTOS
  // Build angle (tangent) breaks distribution <
  for (vector<LxMCTrack>::iterator i = MCTracks.begin(); i != MCTracks.end(); ++i)
  {
    LxMCTrack& track = *i;

    if ((13 != track.pdg && -13 == track.pdg) || track.mother_ID >= 0)
      continue;

    if (track.p < 3)
      continue;

    Double_t pt2 = track.px * track.px + track.py * track.py;

    if (pt2 < 1)
      continue;

    LxMCPoint* points[LXSTATIONS][LXLAYERS];
    memset(points, 0, sizeof(points));

    for (vector<LxMCPoint*>::iterator j = track.Points.begin(); j != track.Points.end(); ++j)
    {
      LxMCPoint* point = *j;
      points[point->stationNumber][point->layerNumber] = point;
    }

    for (int j = 0; j < LXSTATIONS; ++j)
    {
#ifdef MAKE_DISPERSE_2D_HISTOS
      if (0 != points[j][1])
      {
        LxMCPoint* mPoint = points[j][1];
        Double_t tx = mPoint->x / mPoint->z;
        Double_t ty = mPoint->y / mPoint->z;
        Double_t diffZ = zCoordsByStations[j][0] - mPoint->z;
        Double_t x = mPoint->x + tx * diffZ;
        Double_t y = mPoint->y + ty * diffZ;
        list<LxMCPoint*>& lPoints = MCPointsByStations[j][0];

        for (list<LxMCPoint*>::iterator k = lPoints.begin(); k != lPoints.end(); ++k)
        {
          LxMCPoint* lPoint = *k;

          if (lPoint->trackId == mPoint->trackId)
            disperseLHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 100.);
          else
            disperseLHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 0.);
        }

        diffZ = zCoordsByStations[j][2] - mPoint->z;
        x = mPoint->x + tx * diffZ;
        y = mPoint->y + ty * diffZ;
        list<LxMCPoint*>& rPoints = MCPointsByStations[j][2];

        for (list<LxMCPoint*>::iterator k = rPoints.begin(); k != rPoints.end(); ++k)
        {
          LxMCPoint* rPoint = *k;

          if (rPoint->trackId == mPoint->trackId)
            disperseRHistos[j]->Fill(rPoint->x - x, rPoint->y - y, 100.);
          else
            disperseRHistos[j]->Fill(rPoint->x - x, rPoint->y - y, 0.);
        }
      }

      if (0 != points[j][2])
      {
        LxMCPoint* rPoint = points[j][2];
        Double_t tx = rPoint->x / rPoint->z;
        Double_t ty = rPoint->y / rPoint->z;
        Double_t diffZ = zCoordsByStations[j][0] - rPoint->z;
        Double_t x = rPoint->x + tx * diffZ;
        Double_t y = rPoint->y + ty * diffZ;
        list<LxMCPoint*>& lPoints = MCPointsByStations[j][0];

        for (list<LxMCPoint*>::iterator k = lPoints.begin(); k != lPoints.end(); ++k)
        {
          LxMCPoint* lPoint = *k;

          if (lPoint->trackId == rPoint->trackId)
            disperseDHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 100.);
          else
            disperseDHistos[j]->Fill(lPoint->x - x, lPoint->y - y, 0.);
        }
      }
#endif//MAKE_DISPERSE_2D_HISTOS
    }//for (int j = 0; j < LXSTATIONS; ++j)
  }//for (vector<LxMCTrack>::iterator i = MCTracks.begin(); i != MCTracks.end(); ++i)
  // > angle (tangent) breaks distribution
#endif//MAKE_HISTOS

  // Read MUCH pixel hits.
  nEnt = muchPixelHits->GetEntries();
  cout << "There are: " << nEnt << " of MUCH pixel hits" << endl;

  Double_t minXErr = 1000;
  Double_t minYErr = 1000;

  for (Int_t i = 0; i < nEnt; ++i)
  {
    CbmMuchPixelHit* mh = LX_DYNAMIC_CAST<CbmMuchPixelHit*> (muchPixelHits->At(i));

    Int_t stationNumber = CbmMuchGeoScheme::GetStationIndex(mh->GetAddress());
    Int_t layerNumber = CbmMuchGeoScheme::GetLayerIndex(mh->GetAddress());
    TVector3 pos, err;
    mh->Position(pos);
    mh->PositionError(err);
    Double_t x = pos.X();
    Double_t y = pos.Y();
    Double_t z = pos.Z();

    if (x != x || y != y || z != z)// Test for NaN
      continue;

    if (minXErr > err.X())
      minXErr = err.X();

    if (minYErr > err.Y())
      minYErr = err.Y();

    LxPoint* lxPoint = 0;

    if (!useMCPInsteadOfHits)
      lxPoint = caSpace.AddPoint(stationNumber, layerNumber, i, x, y, z, err.X(), err.Y(), err.Z());

    Int_t clusterId = mh->GetRefId();
    CbmMuchCluster* cluster = LX_DYNAMIC_CAST<CbmMuchCluster*> (listMuchClusters->At(clusterId));
    Int_t nDigis = cluster->GetNofDigis();

    for (Int_t j = 0; j < nDigis; ++j)
    {
      CbmMuchDigiMatch* digiMatch = LX_DYNAMIC_CAST<CbmMuchDigiMatch*> (listMuchPixelDigiMatches->At(cluster->GetDigi(j)));
      Int_t nMCs = digiMatch->GetNofLinks();

      for (Int_t k = 0; k < nMCs; ++k)
      {
        const CbmLink& lnk = digiMatch->GetLink(k);
        Int_t mcIndex = lnk.GetIndex();

        if (mcIndex > maxReferencedPtsIndex)
          maxReferencedPtsIndex = mcIndex;

        Int_t mcIndexMapped = root2lxmcpointmap[mcIndex];

        if (-1 == mcIndexMapped)
          continue;

        if (!useMCPInsteadOfHits)
        {
          MCPoints[mcIndexMapped].lxPoints.push_back(lxPoint);
          lxPoint->mcPoints.push_back(&MCPoints[mcIndexMapped]);
        }
        else if (MCPoints[mcIndexMapped].lxPoints.empty())
        {
          LxMCPoint& mcp = MCPoints[mcIndexMapped];
          lxPoint = caSpace.AddPoint(stationNumber, layerNumber, i, mcp.x, mcp.y, mcp.z, err.X(), err.Y(), err.Z());
          MCPoints[mcIndexMapped].lxPoints.push_back(lxPoint);
          lxPoint->mcPoints.push_back(&MCPoints[mcIndexMapped]);
        }
      }
    }
  }// for (Int_t i = 0; i < nEnt; ++i)

  cout << "minXErr = " << minXErr << " ; minYErr = " << minYErr << endl << endl;

  // At last start invocation of CA procedures.
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  cout << "Execution duration 1 was: " << exeDuration << endl;

  //if (calcMiddlePoints)
    //caSpace.RestoreMiddlePoints();

  bTime.tv_sec = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  cout << "Execution duration 2 was: " << exeDuration << endl;
#ifdef CLUSTER_MODE
  caSpace.BuildRays2();
#else//CLUSTER_MODE
  //caSpace.BuildRays();
#endif//CLUSTER_MODE
  bTime.tv_sec = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  cout << "Execution duration 3 was: " << exeDuration << endl;
#ifdef CLUSTER_MODE
  caSpace.ConnectNeighbours2();
#else//CLUSTER_MODE
  //caSpace.ConnectNeighbours();
#endif//CLUSTER_MODE
  bTime.tv_sec = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  cout << "Execution duration 4 was: " << exeDuration << endl;
#ifdef CLUSTER_MODE
  caSpace.Reconstruct2();
#else//CLUSTER_MODE
  caSpace.Reconstruct();
#endif//CLUSTER_MODE
  //caSpace.FitTracks();
  bTime.tv_sec = eTime.tv_sec;
  bTime.tv_usec = eTime.tv_usec;
  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  cout << "Execution duration 5 was: " << exeDuration << endl;
  caSpace.FitTracks();
  //cout << "maxReferencedPtsIndex=" << maxReferencedPtsIndex << " mcPtsCount=" << mcPtsCount << " GEF=" << listMuchPts->GetEntriesFast() << endl;
  //cout << "Hits=" << muchPixelHits->GetEntries() << " clusters=" << listMuchClusters->GetEntries() << " digi ms=" << listMuchPixelDigiMatches->GetEntries() << endl;
  //sleep(3);

  /////////////////////////////////////////////////////////////////////////////////
  // Join MUCH-tracks with STS-tracks.
  /////////////////////////////////////////////////////////////////////////////////

  // STS-tracks fulfilling the limitations peculiar for muons are picked up in the loop below.
  nEnt = listStsTracks->GetEntries();

  for (int i = 0; i < nEnt; ++i)
  {
    CbmStsTrack* stsTrack = LX_DYNAMIC_CAST<CbmStsTrack*> (listStsTracks->At(i));

    Double_t lpa[5] = { stsTrack->GetParamLast()->GetX(), stsTrack->GetParamLast()->GetY(),
        stsTrack->GetParamLast()->GetTx(), stsTrack->GetParamLast()->GetTy(), stsTrack->GetParamLast()->GetQp() };

    if (lpa[0] != lpa[0] || lpa[1] != lpa[1] || lpa[2] != lpa[2] || lpa[3] != lpa[3] || lpa[4] != lpa[4])
      continue;

    CbmStsTrack aTrack = *stsTrack;

    Int_t pdgHypo = 13;

    if (stsTrack->GetParamLast()->GetTx() > stsTrack->GetParamFirst()->GetTx())
      pdgHypo = -13;

    extFitter.DoFit(&aTrack, pdgHypo);
    Double_t chi2Prim = extFitter.GetChiToVertex(&aTrack, fPrimVtx);
    FairTrackParam params;
    extFitter.Extrapolate(&aTrack, fPrimVtx->GetZ(), &params);
    Double_t p = 1 / params.GetQp();
    Double_t p2 = p * p;

    if (p2 < 3.0 * 3.0)
      continue;

/*
    Double_t xDelta = 0.05;//5.0 * sqrt(params.GetCovariance(0, 0));
    Double_t yDelta = 0.05;//5.0 * sqrt(params.GetCovariance(1, 1));

    if (params.GetX() < -xDelta  || params.GetX() > xDelta || params.GetY() < -yDelta || params.GetY() > yDelta)
      continue;
*/
    Double_t tx2 = params.GetTx() * params.GetTx();
    Double_t ty2 = params.GetTy() * params.GetTy();
    Double_t pt2 = p2 * (tx2 + ty2) / (1 + tx2 + ty2);

    if (pt2 < 1.0)
      continue;

    LxExtTrack extTrack;
    extTrack.track = stsTrack;
    extTrack.extId = i;

    CbmTrackMatch* match = LX_DYNAMIC_CAST<CbmTrackMatch*> (listStsMatches->At(i));
    Int_t numberOfHits = match->GetNofTrueHits() + match->GetNofWrongHits() + match->GetNofFakeHits();

    if (match->GetNofTrueHits() >= 0.7 * numberOfHits)
    {
      Int_t mcTrackId = match->GetMCTrackId();
      Int_t mappedId = root2lxmctrackmap[mcTrackId];

      if (-1 != mappedId)
      {
        MCTracks[mappedId].externalTrack = stsTrack;
        extTrack.mcTrack = &MCTracks[mappedId];

        if (13 == MCTracks[mappedId].pdg || -13 == MCTracks[mappedId].pdg)
        {
          stsTrackChi2->Fill(chi2Prim);
          stsTrackX->Fill(params.GetX());
          stsTrackY->Fill(params.GetY());
        }
      }
    }

    caSpace.extTracks.push_back(extTrack);
  }

  cout << "External tracks are read" << endl;

  // Join reconstructed tracks with external tracks.
  caSpace.JoinExtTracks();
  cout << "External tracks are connected" << endl;

  // Measure a reconstruction efficiency.
  effCounter.CalcRecoEff(linkWithSts);
  MatchRecoToMC();

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

  if (generateInvMass)
    CalcInvMass();

  if (generateBackground)
    SaveEventTracks();

  // Finally save reconstructed tracks.
  //SaveRecoTracks();

  // Hopefully we don't need these two maps any more.
  delete[] root2lxmctrackmap;
  delete[] root2lxmcpointmap;
  ++eventNumber;
}

void LxFinderTriplet::SaveRecoTracks()
{
  Int_t trackNo = listRecoTracks->GetEntriesFast();

  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)
  {
    LxTrack* recoTrack = *i;
    LxExtTrack* stsTrack = recoTrack->externalTrack;

    if (0 == stsTrack)
      continue;

    CbmMuchTrack muchTrack;

    for (int j = 0; j < LXSTATIONS * LXLAYERS; ++j)
    {
      LxPoint* pPoint = recoTrack->points[j];

      if (0 != pPoint)
        muchTrack.AddHit(pPoint->hitId, kMUCHPIXELHIT);
    }

    muchTrack.SetChiSq(recoTrack->chi2);
    muchTrack.SetNDF(4 * LXSTATIONS);// Probably need to calculate it more accurately.
    muchTrack.SetPreviousTrackId(stsTrack->extId);

    FairTrackParam parFirst;
    parFirst = *stsTrack->track->GetParamLast();
    muchTrack.SetFlag(0);// Good track.

    FairTrackParam parLast(parFirst);
    muchTrack.SetParamFirst(&parFirst);
    muchTrack.SetParamLast(&parLast);

    new ((*listRecoTracks)[trackNo++]) CbmMuchTrack(muchTrack);
  }
}

void LxFinderTriplet::CalcInvMass()
{
  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)
  {
    LxTrack* firstTrack = *i;
    LxMCTrack* mcTrack1 = firstTrack->mcTrack;

    if (0 == firstTrack->externalTrack)
      continue;

    LxMCTrack* mcTrack2 = firstTrack->externalTrack->mcTrack;

    if (mcTrack1)
    {
      Int_t pdg1 = mcTrack1 ? mcTrack1->pdg : 10000;
      map<Int_t, map<Int_t, int> >::iterator j = particleCounts.find(pdg1);

      if (j == particleCounts.end())
        j = particleCounts.insert(pair<Int_t, map<Int_t, int> > (pdg1, map<Int_t, int> ())).first;

      Int_t pdg2 = mcTrack2 ? mcTrack2->pdg : 10000;

      map<Int_t, int>::iterator k = j->second.find(pdg2);

      if (k != j->second.end())
        ++(k->second);
      else
        j->second.insert(pair<Int_t, int> (pdg2, 1));
    }

    CbmStsTrack t1 = *firstTrack->externalTrack->track;

    // Add entry to the respective chi2 histogram if needed. This can be used for placing chi2 cuts determining J/psi.
    // pdg == 13 | -13 -- it is a muon.
    // mother_ID < 0 -- it is a primary particle. URQMD doesn't produce primary muons. So it from PLUTO <=> is J/psi.

    if (generateChi2)
    {
//      Double_t normalizedChi2 = firstTrack->chi2 / (LXSTATIONS * 4);// length * 4 == NDF.

      //if (mcTrack2 && (mcTrack2->pdg == 13 || mcTrack2->pdg == -13) && mcTrack2->mother_ID < 0)
        //signalChi2Histo->Fill(normalizedChi2);
      //else
        //backgroundChi2Histo->Fill(normalizedChi2);
    }

    if (!mcTrack2 || (mcTrack2->pdg != 13 && mcTrack2->pdg != -13) || mcTrack2->mother_ID >= 0)
      continue;

    extFitter.DoFit(&t1, 13);
    Double_t chi2Prim = extFitter.GetChiToVertex(&t1, fPrimVtx);
    FairTrackParam t1param;
    extFitter.Extrapolate(&t1, fPrimVtx->GetZ(), &t1param);

    if (t1param.GetQp() <= 0)
      continue;

    Double_t p1 = 1 / t1param.GetQp();
    Double_t tx12 = t1param.GetTx() * t1param.GetTx();
    Double_t ty12 = t1param.GetTy() * t1param.GetTy();
    Double_t pt12 = p1 * p1 * (tx12 + ty12) / (1 + tx12 + ty12);

    if (pt12 < 1)
      continue;

    CbmKFTrack muPlus(t1);

    for (list<LxTrack*>::iterator j = caSpace.tracks.begin(); j != caSpace.tracks.end(); ++j)
    {
      LxTrack* secondTrack = *j;

      if (0 == secondTrack->externalTrack)
        continue;

      LxMCTrack* mcSecondTrack = secondTrack->externalTrack->mcTrack;

      if (!mcSecondTrack || (mcSecondTrack->pdg != 13 && mcSecondTrack->pdg != -13) || mcSecondTrack->mother_ID >= 0)
        continue;

      CbmStsTrack t2 = *secondTrack->externalTrack->track;
      extFitter.DoFit(&t2, 13);
      chi2Prim = extFitter.GetChiToVertex(&t2, fPrimVtx);
      FairTrackParam t2param;
      extFitter.Extrapolate(&t2, fPrimVtx->GetZ(), &t2param);

      if (t2param.GetQp() >= 0)
        continue;

      Double_t p2 = 1 / t2param.GetQp();
      Double_t tx22 = t2param.GetTx() * t2param.GetTx();
      Double_t ty22 = t2param.GetTy() * t2param.GetTy();
      Double_t pt22 = p2 * p2 * (tx22 + ty22) / (1 + tx22 + ty22);

      if (pt22 < 1)
        continue;

      CbmKFTrack muMinus(t2);
      vector<CbmKFTrackInterface*> kfData;
      kfData.push_back(&muPlus);
      kfData.push_back(&muMinus);
      /*CbmKFParticle DiMu;
      DiMu.Construct(kfData, 0);
      DiMu.TransportToDecayVertex();
      Double_t m, merr;
      DiMu.GetMass(m, merr);

      massHisto->Fill(m);*/
    }
  }
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
//#endif//MAKE_HISTOS

void LxFinderTriplet::FinishTask()
{
  if (generateInvMass)
    SaveInvMass();

  if (generateBackground)
    SaveBackground();

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
  char histoFileName[128];

  for (int i = 0; i < 6; ++i)
  {
#ifdef MAKE_DISPERSE_2D_HISTOS
    sprintf(histoFileName, "disperseL_histo_%d.root", i);
    SaveHisto(disperseLHistos[i], histoFileName);
    sprintf(histoFileName, "disperseR_histo_%d.root", i);
    SaveHisto(disperseRHistos[i], histoFileName);
    sprintf(histoFileName, "disperseD_histo_%d.root", i);
    SaveHisto(disperseDHistos[i], histoFileName);
#endif//MAKE_DISPERSE_2D_HISTOS
  }
#endif//MAKE_HISTOS

  {
    TFile fh("effByMomentumProfile.root", "RECREATE");
    effByMomentumProfile->Write();
    fh.Close();
    delete effByMomentumProfile;
  }

  SaveHisto(stsTrackChi2, "stsTrackChi2.root");
  SaveHisto(stsTrackX, "stsTrackX.root");
  SaveHisto(stsTrackY, "stsTrackY.root");

  SaveHisto(signalInterTracksDistance, "signalInterTracksDistance.root");
  SaveHisto(bgrInterTracksDistance, "bgrInterTracksDistance.root");
  SaveHisto(signalChi2Histo, "signalChi2Histo.root");
  SaveHisto(backgroundChi2Histo, "backgroundChi2Histo.root");
  SaveHisto(unknownChi2Histo, "unknownChi2Histo.root");
  SaveHisto(signalSignDefect, "signalSignDefectHisto.root");
  SaveHisto(bgrSignDefect, "bgrSignDefectHisto.root");
  SaveHisto(signalYAtZ0, "signalYAtZ0.root");
  SaveHisto(bgrYAtZ0, "bgrYAtZ0.root");
  SaveHisto(signalYAtZ0_2, "signalYAtZ0_2.root");
  SaveHisto(bgrYAtZ0_2, "bgrYAtZ0_2.root");

  TFile::CurrentFile() = curFile;

#ifdef MAKE_EFF_CALC
  incomplete_events.close();
#endif//MAKE_EFF_CALC

  delete superEventData;

  FairTask::FinishTask();
}

void LxFinderTriplet::SaveInvMass()
{
  TFile* curFile = TFile::CurrentFile();

  TFile fh("jpsi_inv_mass_histo.root", "RECREATE");
  massHisto->Write();
  fh.Close();
  delete massHisto;

  TFile::CurrentFile() = curFile;
}

void LxFinderTriplet::SaveBackground()
{
  for (list<CbmStsTrack>::iterator i = positiveTracks.begin(); i != positiveTracks.end(); ++i)
  {
    CbmStsTrack& t1 = *i;
    CbmKFTrack muPlus(t1);

    for (list<CbmStsTrack>::iterator j = negativeTracks.begin(); j != negativeTracks.end(); ++j)
    {
      CbmStsTrack& t2 = *j;
      CbmKFTrack muMinus(t2);
      vector<CbmKFTrackInterface*> kfData;
      kfData.push_back(&muPlus);
      kfData.push_back(&muMinus);
      /*CbmKFParticle DiMu;
      DiMu.Construct(kfData, 0);
      DiMu.TransportToDecayVertex();
      Double_t m, merr;
      DiMu.GetMass(m, merr);*/

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
}
