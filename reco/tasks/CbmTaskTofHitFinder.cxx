/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer], Pierre-Alain Loizeau, Norbert Herrmann */

#include "CbmTaskTofHitFinder.h"

// TOF Classes and includes
#include "CbmTofAddress.h"  // in cbmdata/tof
#include "CbmTofCell.h"     // in tof/TofData
#include "CbmTofCreateDigiPar.h"
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v21a.h"  // in cbmdata/tof
#include "CbmTofDigi.h"             // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"              // in cbmdata/tof

// CBMroot classes and includes
#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmMatch.h"

// FAIR classes and includes
#include "FairEventHeader.h"    // from CbmStsDigitize, for GetEventInfo
#include "FairMCEventHeader.h"  // from CbmStsDigitize, for GetEventInfo
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunSim.h"  // from CbmStsDigitize, for GetEventInfo
#include "FairRuntimeDb.h"
#include <Logger.h>

// ROOT Classes and includes
#include "TClonesArray.h"

// C++ Classes and includes
#include <iomanip>
#include <iostream>

using std::fixed;
using std::left;
using std::pair;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;

using cbm::algo::HitFinderTofRpcPar;
using cbm::algo::TofCell;
using cbm::algo::TofCluster;

const int32_t numClWalkBinX = 20;
const double TTotMean       = 2.E4;

/************************************************************************************/
CbmTaskTofHitFinder::CbmTaskTofHitFinder() : FairTask("CbmTaskTofHitFinder"), fGeoHandler(new CbmTofGeoHandler()) {}

CbmTaskTofHitFinder::CbmTaskTofHitFinder(const char* name, int32_t verbose)
  : FairTask(TString(name), verbose)
  , fGeoHandler(new CbmTofGeoHandler())
{
}

CbmTaskTofHitFinder::~CbmTaskTofHitFinder()
{
  if (fGeoHandler) delete fGeoHandler;
}

InitStatus CbmTaskTofHitFinder::Init()
{
  fDigiMan = CbmDigiManager::Instance(), fDigiMan->Init();
  if (false == RegisterInputs()) return kFATAL;
  if (false == RegisterOutputs()) return kFATAL;
  if (false == InitParameters()) return kFATAL;
  if (false == InitCalibParameter()) return kFATAL;
  if (false == InitAlgos()) return kFATAL;
  return kSUCCESS;
}

void CbmTaskTofHitFinder::SetParContainers()
{
  LOG(info) << " CbmTaskTofHitFinder => Get the digi parameters for tof";

  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  LOG(info) << "  CbmTaskTofHitFinder::SetParContainers found " << fDigiPar->GetNrOfModules() << " cells ";

  fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
}

void CbmTaskTofHitFinder::Exec(Option_t* /*option*/)
{
  // Start timer counter
  fTimer.Start();

  fTofHitsColl->Clear("C");
  fTofDigiMatchColl->Delete();

  fStart.Set();

  // --- Local variables
  int32_t nDigisAll = CbmDigiManager::GetNofDigis(ECbmModuleId::kTof);
  int32_t nEvents   = 0;
  int32_t nDigis    = 0;
  int32_t nHits     = 0;
  CbmEvent* event   = nullptr;
  pair<int32_t, int32_t> result;

  // --- Time-slice mode: process entire digi array
  if (!fEvents) {
    result = BuildClusters(nullptr);
    nDigis = result.first;
    nHits  = result.second;
  }

  // --- Event-based mode: read and process event after event
  else {
    nEvents = fEvents->GetEntriesFast();
    for (int32_t iEvent = 0; iEvent < nEvents; iEvent++) {
      event = dynamic_cast<CbmEvent*>(fEvents->At(iEvent));
      assert(event);
      result = BuildClusters(event);
      nDigis += result.first;
      nHits += result.second;
    }  //# events
  }    //? event mode

  fStop.Set();
  fTimer.Stop();

  // --- Timeslice log
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << fTimer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fiNofTs;
  if (fEvents) logOut << ", events " << nEvents;
  logOut << ", digis " << nDigis << " / " << nDigisAll;
  logOut << ", hits " << nHits;
  LOG(info) << logOut.str();

  // --- Update Counters
  fiNofTs++;
  fiNofEvents += nEvents;
  fNofDigisAll += nDigisAll;
  fNofDigisUsed += nDigis;
  fdNofHitsTot += nHits;
  fdTimeTot += fTimer.RealTime();
}

void CbmTaskTofHitFinder::Finish()
{
  // Screen log
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices          : " << fiNofTs;
  LOG(info) << "Digis / TS           : " << fixed << setprecision(2) << fNofDigisAll / double(fiNofTs);
  LOG(info) << "Hits  / TS           : " << fixed << setprecision(2) << fdNofHitsTot / double(fiNofTs);
  LOG(info) << "Time  / TS           : " << fixed << setprecision(2) << 1000. * fdTimeTot / double(fiNofTs) << " ms";
  if (fEvents) {
    double unusedFrac = 100. * (1. - fNofDigisUsed / fNofDigisAll);
    LOG(info) << "Digis outside events : " << fNofDigisAll - fNofDigisUsed << " = " << unusedFrac << " %";
    LOG(info) << "Events               : " << fiNofEvents;
    LOG(info) << "Events / TS          : " << fixed << setprecision(2) << double(fiNofEvents) / double(fiNofTs);
    LOG(info) << "Digis  / event       : " << fixed << setprecision(2) << fNofDigisUsed / double(fiNofEvents);
    LOG(info) << "Hits   / event       : " << fixed << setprecision(2) << fdNofHitsTot / double(fiNofEvents);
  }
  LOG(info) << "=====================================\n";
}

/************************************************************************************/
// Functions common for all clusters approximations
bool CbmTaskTofHitFinder::RegisterInputs()
{
  FairRootManager* fManager = FairRootManager::Instance();

  // --- Check input branch (TofDigiExp). If not present, set task inactive.
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(error) << GetName() << ": No TofDigi input array present; "
               << "task will be inactive.";
    return kERROR;
  }

  // --- Look for event branch
  fEvents = dynamic_cast<TClonesArray*>(fManager->GetObject("Event"));
  if (fEvents) LOG(info) << GetName() << ": Found Event branch; run event-based";
  else {
    fEvents = dynamic_cast<TClonesArray*>(fManager->GetObject("CbmEvent"));
    if (fEvents) LOG(info) << GetName() << ": Found CbmEvent branch; run event-based";
    else
      LOG(info) << GetName() << ": No event branch found; run time-based";
  }
  return true;
}

bool CbmTaskTofHitFinder::RegisterOutputs()
{
  FairRootManager* rootMgr = FairRootManager::Instance();
  fTofHitsColl             = new TClonesArray("CbmTofHit");

  // Flag check to control whether digis are written in output root file
  rootMgr->Register("TofHit", "Tof", fTofHitsColl, IsOutputBranchPersistent("TofHit"));

  fTofDigiMatchColl = new TClonesArray("CbmMatch", 100);
  rootMgr->Register("TofHitDigiMatch", "Tof", fTofDigiMatchColl, IsOutputBranchPersistent("TofHitDigiMatch"));

  return true;
}

bool CbmTaskTofHitFinder::InitParameters()
{
  if (fDigiBdfPar->UseExpandedDigi() == false) {
    LOG(fatal) << " Compressed Digis not implemented ... ";
    return false;
  }

  // Initialize the TOF GeoHandler
  bool isSimulation   = false;
  int32_t iGeoVersion = fGeoHandler->Init(isSimulation);
  LOG(info) << "CbmTaskTofHitFinder::InitParameters with GeoVersion " << iGeoVersion;

  if (k14a > iGeoVersion) {
    LOG(error) << "CbmTaskTofHitFinder::InitParameters => Only compatible "
                  "with geometries after v12b !!!";
    return false;
  }

  if (nullptr != fTofId)
    LOG(info) << "CbmTaskTofHitFinder::InitParameters with GeoVersion " << fGeoHandler->GetGeoVersion();
  else {
    switch (iGeoVersion) {
      case k14a: fTofId = new CbmTofDetectorId_v14a(); break;
      case k21a: fTofId = new CbmTofDetectorId_v21a(); break;
      default: LOG(error) << "CbmTaskTofHitFinder::InitParameters => Invalid geometry!!!" << iGeoVersion; return false;
    }
  }

  LOG(info) << "=> Get the digi parameters for tof";
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  // create digitization parameters from geometry file
  CbmTofCreateDigiPar* tofDigiPar = new CbmTofCreateDigiPar("TOF Digi Producer", "TOF task");
  LOG(info) << "Create DigiPar ";
  tofDigiPar->Init();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
  if (0 == fDigiPar) {
    LOG(error) << "CbmTofSimpleClusterizer::InitParameters => Could not obtain "
                  "the CbmTofDigiPar ";
    return false;
  }
  return true;
}

/************************************************************************************/
bool CbmTaskTofHitFinder::InitCalibParameter()
{
  // dimension and initialize calib parameter
  int32_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  fvCPSigPropSpeed.resize(iNbSmTypes);
  for (int32_t iT = 0; iT < iNbSmTypes; iT++) {
    int32_t iNbRpc = fDigiBdfPar->GetNbRpc(iT);
    fvCPSigPropSpeed[iT].resize(iNbRpc);
    for (int32_t iRpc = 0; iRpc < iNbRpc; iRpc++)
      if (0.0 < fDigiBdfPar->GetSigVel(iT, 0, iRpc)) fvCPSigPropSpeed[iT][iRpc] = fDigiBdfPar->GetSigVel(iT, 0, iRpc);
      else
        fvCPSigPropSpeed[iT][iRpc] = fDigiBdfPar->GetSignalSpeed();
  }

  // Other calibration constants can be set here. Currently set to default values
  fvCPTOff.resize(iNbSmTypes);
  fvCPTotGain.resize(iNbSmTypes);
  fvCPWalk.resize(iNbSmTypes);
  for (int32_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
    int32_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    int32_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
    fvCPTOff[iSmType].resize(iNbSm * iNbRpc);
    fvCPTotGain[iSmType].resize(iNbSm * iNbRpc);
    fvCPWalk[iSmType].resize(iNbSm * iNbRpc);
    for (int32_t iSm = 0; iSm < iNbSm; iSm++) {
      for (int32_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
        int32_t iNbChan = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        fvCPTOff[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        fvCPTotGain[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        fvCPWalk[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        int32_t nbSide = 2 - fDigiBdfPar->GetChanType(iSmType, iRpc);
        for (int32_t iCh = 0; iCh < iNbChan; iCh++) {
          fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          for (int32_t iSide = 0; iSide < nbSide; iSide++) {
            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide]    = 0.;
            fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] = 1.;
            fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide].resize(numClWalkBinX);
            for (int32_t iWx = 0; iWx < numClWalkBinX; iWx++) {
              fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide][iWx] = 0.;
            }
          }
        }
      }
    }
  }
  LOG(info) << "CbmTaskTofHitFinder::InitCalibParameter: defaults set";
  LOG(info) << "CbmTaskTofHitFinder::InitCalibParameter: initialization done";
  return true;
}

bool CbmTaskTofHitFinder::InitAlgos()
{
  /// Go to Top volume of the geometry in the GeoManager to make sure our nodes are found
  gGeoManager->CdTop();

  //Prepare storage vectors
  int32_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
  fStorDigiExp.resize(iNbSmTypes);
  fStorDigiInd.resize(iNbSmTypes);

  for (int32_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
    int32_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    int32_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
    fStorDigiExp[iSmType].resize(iNbSm * iNbRpc);
    fStorDigiInd[iSmType].resize(iNbSm * iNbRpc);
  }

  // Create one algorithm per RPC and configure it with parameters
  for (int32_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
    int32_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    int32_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
    for (int32_t iSm = 0; iSm < iNbSm; iSm++) {
      for (int32_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
        std::unique_ptr<HitFinderTofRpcPar> par(new HitFinderTofRpcPar());
        par->FeeTimeRes     = fDigiBdfPar->GetFeeTimeRes();
        par->SysTimeRes     = 0.080;
        par->CPSigPropSpeed = fvCPSigPropSpeed[iSmType][iRpc];
        par->outTimeFactor  = 1.0;
        par->numClWalkBinX  = numClWalkBinX;
        par->TOTMax         = 5.E4;
        par->TOTMin         = 2.E4;
        par->maxTimeDist    = fDigiBdfPar->GetMaxTimeDist();
        par->maxSpaceDist   = fDigiBdfPar->GetMaxDistAlongCh();
        par->numGaps        = fDigiBdfPar->GetNbGaps(iSmType, iRpc);
        par->gapSize        = fDigiBdfPar->GetGapSize(iSmType, iRpc);
        int32_t iNbChan     = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        par->fChanPar.resize(iNbChan);
        for (int32_t iCh = 0; iCh < iNbChan; iCh++) {

          //get channel info from iSmType, iSm, iRpc, iCh
          CbmTofDetectorInfo xDetInfo(ECbmModuleId::kTof, iSmType, iSm, iRpc, 0, iCh);
          const int32_t iChId     = fTofId->SetDetectorInfo(xDetInfo);
          CbmTofCell* channelInfo = fDigiPar->GetCell(iChId);

          //init Tof cell
          TofCell& cell = par->fChanPar[iCh].cell;
          cell.pos.SetX(channelInfo->GetX());
          cell.pos.SetY(channelInfo->GetY());
          cell.pos.SetZ(channelInfo->GetZ());
          cell.sizeX = channelInfo->GetSizex();
          cell.sizeY = channelInfo->GetSizey();

          // prepare local->global trafo
          gGeoManager->FindNode(channelInfo->GetX(), channelInfo->GetY(), channelInfo->GetZ());
          double* rot_ptr = gGeoManager->GetCurrentMatrix()->GetRotationMatrix();
          cell.rotation   = ROOT::Math::Rotation3D(&rot_ptr[0], &rot_ptr[9]);

          par->fChanPar[iCh].fvCPTOff    = std::move(fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh]);
          par->fChanPar[iCh].fvCPTotGain = std::move(fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh]);
          par->fChanPar[iCh].fvCPWalk    = std::move(fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh]);
          par->fChanPar[iCh].address     = CbmTofAddress::GetUniqueAddress(iSm, iRpc, iCh, 0, iSmType);
        }
        fAlgo[iSmType][iSm * iNbRpc + iRpc].SetParams(std::move(par));
      }
    }
  }
  return true;
}

pair<int32_t, int32_t> CbmTaskTofHitFinder::BuildClusters(CbmEvent* event)
{
  // --- MC Event info (input file, entry number, start time)
  int32_t iInputNr  = 0;
  int32_t iEventNr  = 0;
  double dEventTime = 0.;
  GetEventInfo(iInputNr, iEventNr, dEventTime);

  // Local variables
  int32_t nDigis = 0;
  int32_t nHits  = 0;

  nDigis = (event ? event->GetNofData(ECbmDataType::kTofDigi) : fDigiMan->GetNofDigis(ECbmModuleId::kTof));
  LOG(debug) << "Number of TOF digis: " << nDigis;

  // Loop over the digis array and store the Digis in separate vectors for each RPC modules
  for (int32_t iDigi = 0; iDigi < nDigis; iDigi++) {
    const uint32_t digiIndex = (event ? event->GetIndex(ECbmDataType::kTofDigi, iDigi) : iDigi);
    assert(fDigiMan->Get<CbmTofDigi>(digiIndex));
    CbmTofDigi* pDigi = new CbmTofDigi(*(fDigiMan->Get<CbmTofDigi>(digiIndex)));
    assert(pDigi);

    // These are doubles in the digi class
    const double smType  = pDigi->GetType();
    const double smId    = pDigi->GetSm();
    const double rpcId   = pDigi->GetRpc();
    const int32_t numRpc = fDigiBdfPar->GetNbRpc(smType);
    fStorDigiExp[smType][smId * numRpc + rpcId].push_back(*pDigi);
    fStorDigiInd[smType][smId * numRpc + rpcId].push_back(digiIndex);
  }

  const uint32_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  for (uint32_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
    const uint32_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    const uint32_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
    for (uint32_t iSm = 0; iSm < iNbSm; iSm++) {
      for (uint32_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
        //read digis
        const uint32_t rpcIdx            = iSm * iNbRpc + iRpc;
        std::vector<CbmTofDigi>& digiExp = fStorDigiExp[iSmType][rpcIdx];
        std::vector<int32_t>& digiInd    = fStorDigiInd[iSmType][rpcIdx];

        //call cluster finder
        std::vector<TofCluster> clusters = fAlgo[iSmType][rpcIdx](digiExp, digiInd);

        //Store hits and match
        for (auto const& cluster : clusters) {
          const int32_t hitIndex = fTofHitsColl->GetEntriesFast();
          TVector3 hitpos        = TVector3(cluster.globalPos.X(), cluster.globalPos.Y(), cluster.globalPos.Z());
          TVector3 hiterr        = TVector3(cluster.globalErr.X(), cluster.globalErr.Y(), cluster.globalErr.Z());
          new ((*fTofHitsColl)[hitIndex])
            CbmTofHit(cluster.detId, hitpos, hiterr, nHits, cluster.weightedTime, cluster.weightedTimeErr, 0, 0);
          nHits++;
          if (event) event->AddData(ECbmDataType::kTofHit, hitIndex);
          CbmMatch* digiMatch = new CbmMatch();
          for (uint32_t i = 0; i < cluster.vDigiIndRef.size(); i++) {
            digiMatch->AddLink(CbmLink(0., cluster.vDigiIndRef.at(i), iEventNr, iInputNr));
          }
          new ((*fTofDigiMatchColl)[hitIndex]) CbmMatch(*digiMatch);
          delete digiMatch;
        }
      }
    }
  }

  return std::make_pair(nDigis, nHits);
}

void CbmTaskTofHitFinder::GetEventInfo(int32_t& inputNr, int32_t& eventNr, double& eventTime)
{
  // --- In a FairRunAna, take the information from FairEventHeader
  if (FairRunAna::Instance()) {
    FairEventHeader* event = FairRunAna::Instance()->GetEventHeader();
    inputNr                = event->GetInputFileId();
    eventNr                = event->GetMCEntryNumber();
    eventTime              = event->GetEventTime();
  }

  // --- In a FairRunSim, the input number and event time are always zero;
  // --- only the event number is retrieved.
  else {
    if (!FairRunSim::Instance()) LOG(fatal) << GetName() << ": neither SIM nor ANA run.";
    FairMCEventHeader* event = FairRunSim::Instance()->GetMCEventHeader();
    inputNr                  = 0;
    eventNr                  = event->GetEventID();
    eventTime                = 0.;
  }
}

ClassImp(CbmTaskTofHitFinder)
