/* Copyright (C) 2009-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Alexandru Bercuci, Etienne Bechtel */

#include "CbmTrdDigitizer.h"

#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmTrdAddress.h"
#include "CbmTrdCheckUtil.h"
#include "CbmTrdDigi.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdModuleSim.h"
#include "CbmTrdModuleSimR.h"
#include "CbmTrdModuleSim2D.h"
#include "CbmTrdPads.h"
#include "CbmTrdParAsic.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParModGain.h"
#include "CbmTrdParModGas.h"
#include "CbmTrdParModGeo.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParSetGain.h"
#include "CbmTrdParSetGas.h"
#include "CbmTrdParSetGeo.h"
#include "CbmTrdPoint.h"
#include "CbmTrdRadiator.h"
#include "CbmTrdRawToDigiR.h"

#include <FairBaseParSet.h>
#include <FairEventHeader.h>
#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRunSim.h>
#include <FairRuntimeDb.h>
#include <Logger.h>

#include <TClonesArray.h>
#include <TRandom.h>
#include <TStopwatch.h>
#include <TVector3.h>

#include <iomanip>
#include <iostream>
#include <memory>

#include <cmath>

using std::map;
using std::pair;
using std::shared_ptr;
using namespace std;
Int_t CbmTrdDigitizer::fConfig = 0;

//________________________________________________________________________________________
CbmTrdDigitizer::CbmTrdDigitizer(shared_ptr<CbmTrdRadiator> radiator)
  : CbmDigitize<CbmTrdDigi>("TrdDigitize")
  , fLastEventTime(0)
  , fpoints(0)
  , nofBackwardTracks(0)
  , fEfficiency(1.)
  , fPoints(NULL)
  , fTracks(NULL)
  , fDigis(nullptr)
  , fDigiMatches(nullptr)
  , fAsicPar(NULL)
  , fGasPar(NULL)
  , fDigiPar(NULL)
  , fGainPar(NULL)
  , fGeoPar(NULL)
  , fRadiator(radiator)
  , fConverter(NULL)
  , fQA(NULL)
  //  ,fConverter()
  //  ,fGeoHandler(new CbmTrdGeoHandler())
  , fModuleMap()
  , fDigiMap()
{
  if (fRadiator == NULL) fRadiator = make_shared<CbmTrdRadiator>(kTRUE, "tdr18");
}

//________________________________________________________________________________________
CbmTrdDigitizer::CbmTrdDigitizer(CbmTrdRadiator* radiator)
  : CbmTrdDigitizer(std::make_shared<CbmTrdRadiator>(radiator)) {};


//________________________________________________________________________________________
CbmTrdDigitizer::~CbmTrdDigitizer()
{
  ResetArrays();
  delete fDigis;
  delete fDigiMatches;
  for (map<Int_t, CbmTrdModuleSim*>::iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++)
    delete imod->second;
  fModuleMap.clear();

  delete fConverter;
  delete fQA;
}


//________________________________________________________________________________________
void CbmTrdDigitizer::SetParContainers()
{
  fAsicPar = static_cast<CbmTrdParSetAsic*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetAsic"));
  fGasPar  = static_cast<CbmTrdParSetGas*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetGas"));
  fDigiPar = static_cast<CbmTrdParSetDigi*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetDigi"));
  fGainPar = static_cast<CbmTrdParSetGain*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetGain"));
  fGeoPar  = new CbmTrdParSetGeo();  //fGeoPar->Print();
}

//________________________________________________________________________________________
InitStatus CbmTrdDigitizer::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) LOG(fatal) << "CbmTrdDigitizer::Init: No FairRootManager";

  fPoints = (TClonesArray*) ioman->GetObject("TrdPoint");
  if (!fPoints) LOG(fatal) << "CbmTrdDigitizer::Init(): No TrdPoint array!";

  fTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (!fTracks) LOG(fatal) << "CbmTrdDigitizer::Init(): No MCTrack array!";

  if (fRadiator) fRadiator->Init();

  fConverter = new CbmTrdRawToDigiR();
  fQA        = new CbmTrdCheckUtil();

  // Set time-based mode if appropriate
  SetTimeBased(fEventMode ? kFALSE : kTRUE);

  RegisterOutput();


  LOG(info) << "================ TRD Digitizer ===============";
  LOG(info) << " Free streaming    : " << (IsTimeBased() ? "yes" : "no");
  LOG(info) << " Add Noise         : " << (AddNoise() ? "yes" : "no");
  LOG(info) << " Weighted distance : " << (UseWeightedDist() ? "yes" : "no");

  return kSUCCESS;
}

//________________________________________________________________________________________
void CbmTrdDigitizer::Exec(Option_t*)
{
  // start timer
  TStopwatch timer;
  timer.Start();

  // get event info (once per event, used for matching)
  GetEventInfo();

  // reset private monitoring counters
  ResetCounters();

  // loop tracks in current event
  CbmTrdModuleSim* mod(NULL);
  Int_t nofPoints = fPoints->GetEntriesFast();
  gGeoManager->CdTop();
  for (Int_t iPoint = 0; iPoint < nofPoints; iPoint++) {
    fpoints++;
    //fMCPointId = iPoint;

    CbmTrdPoint* point = static_cast<CbmTrdPoint*>(fPoints->At(iPoint));
    if (!point) continue;
    const CbmMCTrack* track = static_cast<const CbmMCTrack*>(fTracks->At(point->GetTrackID()));
    if (!track) continue;

    Double_t dz = point->GetZOut() - point->GetZIn();
    if (dz < 0) {
      LOG(debug2) << GetName() << "::Exec: MC-track points towards target!";
      nofBackwardTracks++;
    }

    // get link to the module working class
    map<Int_t, CbmTrdModuleSim*>::iterator imod = fModuleMap.find(point->GetDetectorID());
    if (imod == fModuleMap.end()) {
      // Looking for gas node corresponding to current point in geo manager
      Double_t meanX = (point->GetXOut() + point->GetXIn()) / 2.;
      Double_t meanY = (point->GetYOut() + point->GetYIn()) / 2.;
      Double_t meanZ = (point->GetZOut() + point->GetZIn()) / 2.;
      gGeoManager->FindNode(meanX, meanY, meanZ);
      if (!TString(gGeoManager->GetPath()).Contains("gas")) {
        LOG(error) << GetName() << "::Exec: MC-track not in TRD! Node:" << TString(gGeoManager->GetPath()).Data()
                   << " gGeoManager->MasterToLocal() failed!";
        continue;
      }
      mod = AddModule(point->GetDetectorID());
    }
    else
      mod = imod->second;
    mod->SetLinkId(fCurrentInput, fCurrentMCEntry, iPoint);
    Double_t gamma = TMath::Sqrt(1 + TMath::Power(track->GetP() / (3.e8 * track->GetMass()), 2));
    mod->SetGamma(gamma);
    mod->MakeDigi(point, fCurrentEventTime, TMath::Abs(track->GetPdgCode()) == 11);
  }

  // Fill data from internally used stl map into CbmDaqBuffer.
  // Calculate final event statistics
  Int_t nDigis(0), nofElectrons(0), nofLatticeHits(0), nofPointsAboveThreshold(0), n0, n1, n2;
  for (map<Int_t, CbmTrdModuleSim*>::iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++) {
    // in streaming mode flush buffers only up to a certain point in time wrt to current event time (allow for event pile-ups)
    //printf("Processing data for module %d\n", imod->first);
    if (IsTimeBased()) nDigis += imod->second->FlushBuffer(fCurrentEventTime);
    // in event-by-event mode flush all buffers
    if (!IsTimeBased()) imod->second->FlushBuffer();
    imod->second->GetCounters(n0, n1, n2);
    nofElectrons += n0;
    nofLatticeHits += n1;
    nofPointsAboveThreshold += n2;
    std::map<Int_t, std::pair<CbmTrdDigi*, CbmMatch*>>* digis = imod->second->GetDigiMap();
    //printf("  Digits[%d] %d\n", imod->first, digis->size());
    for (std::map<Int_t, pair<CbmTrdDigi*, CbmMatch*>>::iterator it = digis->begin(); it != digis->end(); it++) {
      assert(it->second.second);
      CbmTrdDigi* digi = it->second.first;
      SendData(digi->GetTime(), digi, it->second.second);
      nDigis++;
    }  //# modules
    digis->clear();
  }  //# digis
  fLastEventTime = fCurrentEventTime;


  Double_t digisOverPoints          = (nofPoints > 0) ? Double_t(nDigis) / Double_t(nofPoints) : 0;
  Double_t latticeHitsOverElectrons = (nofElectrons > 0) ? (Double_t) nofLatticeHits / (Double_t) nofElectrons : 0;
  LOG(debug) << "CbmTrdDigitizer::Exec Points:               " << nofPoints;
  LOG(debug) << "CbmTrdDigitizer::Exec PointsAboveThreshold: " << nofPointsAboveThreshold;
  LOG(debug) << "CbmTrdDigitizer::Exec Digis:                " << nDigis;
  LOG(debug) << "CbmTrdDigitizer::Exec digis/points:         " << digisOverPoints;
  LOG(debug) << "CbmTrdDigitizer::Exec BackwardTracks:       " << nofBackwardTracks;
  LOG(debug) << "CbmTrdDigitizer::Exec LatticeHits:          " << nofLatticeHits;
  LOG(debug) << "CbmTrdDigitizer::Exec Electrons:            " << nofElectrons;
  LOG(debug) << "CbmTrdDigitizer::Exec latticeHits/electrons:" << latticeHitsOverElectrons;
  timer.Stop();
  LOG(debug) << "CbmTrdDigitizer::Exec real time=" << timer.RealTime() << " CPU time=" << timer.CpuTime();

  // --- Event log
  LOG(info) << "+ " << setw(15) << GetName() << ": Event " << setw(6) << right << fCurrentEvent << " at " << fixed
            << setprecision(3) << fCurrentEventTime << " ns, points: " << nofPoints << ", digis: " << nDigis
            << ". Exec time " << setprecision(6) << timer.RealTime() << " s.";
}

//________________________________________________________________________________________
void CbmTrdDigitizer::FlushBuffers()
{
  LOG(info) << GetName() << ": Processing analogue buffers";
  Int_t nDigis(0);
  for (map<Int_t, CbmTrdModuleSim*>::iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++) {
    nDigis += imod->second->FlushBuffer();
    std::map<Int_t, std::pair<CbmTrdDigi*, CbmMatch*>>* digis = imod->second->GetDigiMap();
    for (std::map<Int_t, pair<CbmTrdDigi*, CbmMatch*>>::iterator it = digis->begin(); it != digis->end(); it++) {
      assert(it->second.second);
      CbmTrdDigi* digi = it->second.first;
      SendData(digi->GetTime(), digi, it->second.second);
      nDigis++;
    }  //# modules
    digis->clear();
  }  //# digis
  LOG(info) << GetName() << ": " << nDigis << (nDigis == 1 ? " digi " : " digis ") << "created and sent to DAQ ";
}

//________________________________________________________________________________________
void CbmTrdDigitizer::Finish()
{
  // flush buffers in streaming mode
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Finish run";
  if (IsTimeBased()) FlushBuffers();
  LOG(info) << GetName() << ": Run summary ";
  LOG(info) << "=====================================";

  fQA->DumpPlots();
}

//________________________________________________________________________________________
CbmTrdModuleSim* CbmTrdDigitizer::AddModule(Int_t detId)
{
  /**  The geometry structure is treelike with cave as
 * the top node. For the TRD there are keeping volume
 * trd_vXXy_1 which is only container for the different layers.
 * The trd layer is again only a container for all volumes of this layer.
 * Loop over all nodes below the top node (cave). If one of
 * the nodes contains a string trd it must be TRD detector.
 * Now loop over the layers and
 * then over all modules of the layer to extract in the end
 * all active regions (gas) of the complete TRD. For each
 * of the gas volumes get the information about size and
 * position from the geomanager and the sizes of the sectors
 * and pads from the definitions in CbmTrdPads. This info
 * is then stored in a CbmTrdModule object for each of the TRD modules.
 **/

  const char* path = gGeoManager->GetPath();
  CbmTrdGeoHandler geoHandler;
  Int_t moduleAddress = geoHandler.GetModuleAddress(path), moduleType = geoHandler.GetModuleType(path),
        orientation = geoHandler.GetModuleOrientation(path), lyId = CbmTrdAddress::GetLayerId(detId);
  if (moduleAddress != detId) {
    LOG(error) << "CbmTrdDigitizer::AddModule: MC module ID " << detId << " does not match geometry definition "
               << moduleAddress << ". Module init failed!";
    return NULL;
  }
  LOG(debug) << GetName() << "::AddModule(" << path << " " << (moduleType < 9 ? 'R' : 'T') << "] mod[" << moduleAddress
             << "] ly[" << lyId << "] det[" << detId << "]";
  CbmTrdModuleSim* module(NULL);
  if (moduleType >= 9) {
    // temporary fix for TRD-2Dh @ mCBM 2021
    if (moduleType == 10) SetUseFASP(kFALSE);
    else
      SetUseFASP();
    module = fModuleMap[moduleAddress] = new CbmTrdModuleSim2D(moduleAddress, lyId, orientation, UseFASP());
    Int_t rType(-1);
    if ((rType = geoHandler.GetRadiatorType(path)) >= 0) {
      if (!fRadiator2D) {  // strong TRD-2D entrance window
        //   const Char_t *ewin = "Al;C;Air;C;Al";
        const Char_t* ewin = "Al;C;HC;C;Al";
        Float_t widths[]   = {
          1.2e-3,  // 12 µm aluminized polyester foil
          0.02,    // carbon laminate sheets of 0.2 mm thickness
          0.9,     // 9mm Nomex honeycom
          0.02,    // carbon laminate sheets of 0.2 mm thickness
          1.2e-3,  // 12 µm aluminized polyester foil
        };

        //   // light TRD-2D entrance window
        //   const Char_t *ewin = "Al;C;HC;Po;Al";
        //   Float_t widths[] = {
        //     1.2e-3, // 12 µm aluminized polyester foil
        //     0.02,   // carbon laminate sheets of 0.2 mm thickness
        //     0.9,    // 9mm Nomex honeycom
        //     0.0025, // polyethylen sheets of 50 µm thickness
        //     1.2e-3, // 12 µm aluminized polyester foil
        //   };  pwidth = widths;
        fRadiator2D = make_shared<CbmTrdRadiator>(kTRUE, "tdr18", ewin);
        fRadiator2D->SetEWwidths(5, widths);
        fRadiator2D->Init();
      }
      module->SetRadiator(fRadiator2D);
    }
    //((CbmTrdModuleSim2D*)module)->SetLabMeasurement();
  }
  else {
    module = fModuleMap[moduleAddress] = new CbmTrdModuleSimR(moduleAddress, lyId, orientation);
    module->SetMessageConverter(fConverter);
    module->SetQA(fQA);
  }

  // try to load Geometry parameters for module
  const CbmTrdParModGeo* pGeo(NULL);
  if (!fGeoPar || !(pGeo = (const CbmTrdParModGeo*) fGeoPar->GetModulePar(detId))) {
    LOG(debug) << GetName() << "::AddModule : No Geo params for module @ " << path << ". Using default.";
    module->SetGeoPar(new CbmTrdParModGeo(Form("TRD_%d", detId), path));
  }
  else
    module->SetGeoPar(pGeo);

  // try to load read-out parameters for module
  const CbmTrdParModDigi* pDigi(NULL);
  if (!fDigiPar || !(pDigi = (const CbmTrdParModDigi*) fDigiPar->GetModulePar(detId))) {
    LOG(debug) << GetName() << "::AddModule : No Read-Out params for module @ " << path << ". Using default.";
  }
  else
    module->SetDigiPar(pDigi);

  // TODO check if this works also for ModuleR (moduleType < 9) modules
  if (moduleType >= 9) {
    // try to load ASIC parameters for module
    CbmTrdParSetAsic* pAsic(NULL);
    if (!fAsicPar || !(pAsic = (CbmTrdParSetAsic*) fAsicPar->GetModuleSet(detId))) {
      LOG(debug) << GetName() << "::AddModule : No ASIC params for module @ " << path << ". Using default.";
      module->SetAsicPar();  // map ASIC channels to read-out channels - need ParModDigi already loaded
    }
    else
      module->SetAsicPar(pAsic);

    // try to load Chamber parameters for module
    const CbmTrdParModGas* pChmb(NULL);
    if (!fGasPar || !(pChmb = (const CbmTrdParModGas*) fGasPar->GetModulePar(detId))) {
      LOG(debug) << GetName() << "::AddModule : No Gas params for module @ " << path << ". Using default.";
    }
    else
      module->SetChmbPar(pChmb);

    // try to load Gain parameters for module
    const CbmTrdParModGain* pGain(NULL);
    if (!fGainPar || !(pGain = (const CbmTrdParModGain*) fGainPar->GetModulePar(detId))) {
      LOG(debug) << GetName() << "::AddModule : No Gain params for module @ " << path << ". Using default.";
    }
    else
      module->SetGainPar(pGain);
  }

  if (fRadiator) module->SetRadiator(fRadiator);

  // Register this class to the module. For data transport through SendData().
  module->SetDigitizer(this);


  return module;
}

//________________________________________________________________________________________
void CbmTrdDigitizer::ResetCounters()
{
  /** Loop over modules and calls ResetCounters on each
 */
  fpoints           = 0;
  nofBackwardTracks = 0;
  for (std::map<Int_t, CbmTrdModuleSim*>::iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++)
    imod->second->ResetCounters();
}


void CbmTrdDigitizer::ResetArrays()
{

  if (fDigis) fDigis->clear();
  if (fDigiMatches) fDigiMatches->clear();
}


ClassImp(CbmTrdDigitizer)
