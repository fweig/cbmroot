/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmTofTBClusterizer.h"
#include "CbmDefs.h"
#include "CbmMatch.h"
#include "CbmTofGeoHandler.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "FairEventHeader.h"
#include <Logger.h>
#include "FairMCEventHeader.h"
#include "FairRunAna.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TH1.h"

#ifdef __MACH__
#include <mach/mach_time.h>
#include <sys/time.h>
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif
inline int clock_gettime(int clk_id, struct timespec* t) {
  mach_timebase_info_data_t timebase;
  mach_timebase_info(&timebase);
  uint64_t time;
  time = mach_absolute_time();
  double nseconds =
    ((double) time * (double) timebase.numer) / ((double) timebase.denom);
  double seconds =
    ((double) time * (double) timebase.numer) / ((double) timebase.denom * 1e9);
  t->tv_sec  = seconds;
  t->tv_nsec = nseconds;
  return 0;
}
#else
#include <time.h>
#endif

ClassImp(CbmTofTBClusterizer)

  const Int_t nbClWalkBinX = 20;
const Int_t nbClDelTofBinX = 50;
const Int_t iNTrg          = 1;
extern Double_t TOTMax;
extern Double_t TOTMin;

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::pair;
using std::set;
using std::vector;

CbmTofTBClusterizer::CbmTofTBClusterizer()
  : fGeoHandler(0)
  , fTofId(0)
  , fDigiPar(0)
  , fChannelInfo(0)
  , fDigiBdfPar(0)
  , fvCPSigPropSpeed()
  , fvCPDelTof()
  , fvCPTOff()
  , fvCPTotGain()
  , fvCPWalk()
  , fTofDigis(0)
  , fTofPoints(0)
  , fTofHits(0)
  , fTofDigiMatchs(0)
  , fStorDigiExp()
  , fStorDigiExpOld()
  , fOutTimeFactor(1) {}

Bool_t CbmTofTBClusterizer::InitCalibParameter() {
  // dimension and initialize calib parameter
  //
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  fvCPSigPropSpeed.resize(iNbSmTypes);
  for (Int_t iT = 0; iT < iNbSmTypes; iT++) {
    Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iT);
    fvCPSigPropSpeed[iT].resize(iNbRpc);
    for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++)
      if (0.0 < fDigiBdfPar->GetSigVel(iT, 0, iRpc))
        fvCPSigPropSpeed[iT][iRpc] = /*1000.0 * */ fDigiBdfPar->GetSigVel(
          iT, 0, iRpc);  // convert in cm/ns
      else
        fvCPSigPropSpeed[iT][iRpc] = fDigiBdfPar->GetSignalSpeed();
  }  // for (Int_t iT=0; iT<iNbSmTypes; iT++)

  fvCPTOff.resize(iNbSmTypes);
  fvCPTotGain.resize(iNbSmTypes);
  fvCPWalk.resize(iNbSmTypes);
  fvCPDelTof.resize(iNbSmTypes);
  for (Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++) {
    Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
    fvCPTOff[iSmType].resize(iNbSm * iNbRpc);
    fvCPTotGain[iSmType].resize(iNbSm * iNbRpc);
    fvCPWalk[iSmType].resize(iNbSm * iNbRpc);
    fvCPDelTof[iSmType].resize(iNbSm * iNbRpc);
    for (Int_t iSm = 0; iSm < iNbSm; iSm++) {
      for (Int_t iRpc = 0; iRpc < iNbRpc; iRpc++) {
        //          LOG(info)<<Form(" fvCPDelTof resize for SmT %d, R %d, B %d ",iSmType,iNbSm*iNbRpc,nbClDelTofBinX)
        //           ;
        fvCPDelTof[iSmType][iSm * iNbRpc + iRpc].resize(nbClDelTofBinX);
        for (Int_t iBx = 0; iBx < nbClDelTofBinX; iBx++) {
          // LOG(info)<<Form(" fvCPDelTof for SmT %d, R %d, B %d",iSmType,iSm*iNbRpc+iRpc,iBx);
          fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx].resize(iNTrg);
          for (Int_t iTrg = 0; iTrg < iNTrg; iTrg++)
            fvCPDelTof[iSmType][iSm * iNbRpc + iRpc][iBx][iTrg] =
              0.;  // initialize
        }

        Int_t iNbChan = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        fvCPTOff[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        fvCPTotGain[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        fvCPWalk[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
        Int_t nbSide = 2 - fDigiBdfPar->GetChanType(iSmType, iRpc);
        for (Int_t iCh = 0; iCh < iNbChan; iCh++) {
          fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh].resize(nbSide);
          for (Int_t iSide = 0; iSide < nbSide; iSide++) {
            fvCPTOff[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] =
              0.;  //initialize
            fvCPTotGain[iSmType][iSm * iNbRpc + iRpc][iCh][iSide] =
              1.;  //initialize
            fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide].resize(
              nbClWalkBinX);
            for (Int_t iWx = 0; iWx < nbClWalkBinX; iWx++) {
              fvCPWalk[iSmType][iSm * iNbRpc + iRpc][iCh][iSide][iWx] = 0.;
            }
          }
        }
      }
    }
  }

  LOG(info) << "CbmTofSimpClusterizer::InitCalibParameter: defaults set";

  /// Save old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

#if 0
  if(0<fCalMode){
    LOG(info) << "CbmTofSimpClusterizer::InitCalibParameter: read histos from "
                 << "file " << fCalParFileName ;

  // read parameter from histos
    if(fCalParFileName.IsNull()) return kTRUE;

    fCalParFile = new TFile(fCalParFileName,"");
    if(NULL == fCalParFile) {
      LOG(error) << "CbmTofSimpClusterizer::InitCalibParameter: "
                 << "file " << fCalParFileName << " does not exist!" ;
      return kTRUE;
    }
    /*
    gDirectory->Print();
    fCalParFile->cd();
    fCalParFile->ls();
    */

    for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
    {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(  iSmType );
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc( iSmType );
      for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
        for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
        {
          TH2F *htempPos_pfx =(TH2F*) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_Pos_pfx",iSmType,iSm,iRpc));
          TH2F *htempTOff_pfx=(TH2F*) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_TOff_pfx",iSmType,iSm,iRpc));
          TH2F *htempTot_pfx =(TH2F*) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_pfx",iSmType,iSm,iRpc));
          if(NULL != htempPos_pfx && NULL != htempTOff_pfx && NULL != htempTot_pfx)
          {
            Int_t iNbCh = fDigiBdfPar->GetNbChan( iSmType, iRpc );
            Int_t iNbinTot = htempTot_pfx->GetNbinsX();
            for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
              {
                Double_t YMean=((TProfile *)htempPos_pfx)->GetBinContent(iCh+1);  //nh +1 empirical(?)
                Double_t TMean=((TProfile *)htempTOff_pfx)->GetBinContent(iCh+1);
                Double_t dTYOff=YMean/fvCPSigPropSpeed[iSmType][iRpc] ;
                fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][0] += -dTYOff + TMean ;
                fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][1] += +dTYOff + TMean ;

                Double_t TotMean=((TProfile *)htempTot_pfx)->GetBinContent(iCh+1);  //nh +1 empirical(?)
                if(1<TotMean){
                  fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][0] *= TTotMean / TotMean;
                  fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][1] *= TTotMean / TotMean;
                }
                LOG(debug1)<<"CbmTofSimpClusterizer::InitCalibParameter:"
                           <<" SmT "<< iSmType<<" Sm "<<iSm<<" Rpc "<<iRpc<<" Ch "<<iCh
                           <<": YMean "<<YMean<<", TMean "<< TMean
                           <<" -> " << fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][0]
                           <<", "   << fvCPTOff[iSmType][iSm*iNbRpc+iRpc][iCh][1]
                           <<", "   << fvCPTotGain[iSmType][iSm*iNbRpc+iRpc][iCh][0]
                           <<", NbinTot "<< iNbinTot
                           ;

                TH1D *htempWalk0=(TH1D*)gDirectory->FindObjectAny(
                                 Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S0_Walk_px", iSmType, iSm, iRpc, iCh ));
                TH1D *htempWalk1=(TH1D*)gDirectory->FindObjectAny(
                                 Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px", iSmType, iSm, iRpc, iCh ));
                if(NULL != htempWalk0 && NULL != htempWalk1 ) { // reinitialize Walk array
                  LOG(info)<<"Initialize Walk correction for "
                            <<Form(" SmT%01d_sm%03d_rpc%03d_Ch%03d",iSmType, iSm, iRpc, iCh)
                            ;
                  if(htempWalk0->GetNbinsX() != nbClWalkBinX)
                    LOG(error)<<"CbmTofSimpClusterizer::InitCalibParameter: Inconsistent Walk histograms"
                              ;
                   for( Int_t iBin = 0; iBin < nbClWalkBinX; iBin++ )
                   {
                     fvCPWalk[iSmType][iSm*iNbRpc+iRpc][iCh][0][iBin]=htempWalk0->GetBinContent(iBin+1);
                     fvCPWalk[iSmType][iSm*iNbRpc+iRpc][iCh][1][iBin]=htempWalk1->GetBinContent(iBin+1);
                     LOG(debug1)<<Form(" SmT%01d_sm%03d_rpc%03d_Ch%03d bin %d walk %f ",iSmType, iSm, iRpc, iCh, iBin,
                                       fvCPWalk[iSmType][iSm*iNbRpc+iRpc][iCh][0][iBin])
                              ;

                   }
                }
            }
          }
          else {
            LOG(error)<<" Histos " << Form("cl_SmT%01d_sm%03d_rpc%03d_XXX", iSmType, iSm, iRpc) << " not found. "
                      ;
          }
          for(Int_t iTrg=0; iTrg<iNTrg; iTrg++){
           TH1D *htmpDelTof =(TH1D*) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_Trg%02d_DelTof",iSmType,iSm,iRpc,iTrg));
           if (NULL==htmpDelTof) {
            LOG(info)<<" Histos " << Form("cl_CorSmT%01d_sm%03d_rpc%03d_Trg%02d_DelTof", iSmType, iSm, iRpc, iTrg) << " not found. "
                      ;
            continue;
           }
           LOG(info)<<" Load DelTof from histos "<< Form("cl_CorSmT%01d_sm%03d_rpc%03d_Trg%02d_DelTof",iSmType,iSm,iRpc,iTrg)<<"."
                    ;
           for(Int_t iBx=0; iBx<nbClDelTofBinX; iBx++){
            fvCPDelTof[iSmType][iSm*iNbRpc+iRpc][iBx][iTrg] += htmpDelTof->GetBinContent(iBx+1);
           }

           // copy Histo to memory
           TDirectory * curdir = gDirectory;
           gDirectory->cd( oldDir->GetPath() );
           TH1D *h1DelTof=(TH1D *)htmpDelTof->Clone(Form("cl_CorSmT%01d_sm%03d_rpc%03d_Trg%02d_DelTof",iSmType,iSm,iRpc,iTrg));

           LOG(info)<<" copy histo "
                     <<h1DelTof->GetName()
                     <<" to directory "
                    <<oldDir->GetName()
                    ;

           gDirectory->cd( curdir->GetPath() );
          }
        }
    }
  }
#endif  //0
  //   fCalParFile->Delete();
  /// Restore old global file and folder pointer to avoid messing with FairRoot
  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gFile      = oldFile;
  gDirectory = oldDir;
  LOG(info) << "CbmTofSimpClusterizer::InitCalibParameter: initialization done";
  return kTRUE;
}

static TH1F* deltaChannelTHisto = 0;
static TH1F* deltaPointTHisto   = 0;
static TH1F* nofChannelsTHisto  = 0;
static TH1F* digiTimeHisto      = 0;

InitStatus CbmTofTBClusterizer::Init() {
  if (0 == fDigiBdfPar) LOG(fatal) << "No CbmTofDigiBdfPar found";

  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  fTofDigis = static_cast<TClonesArray*>(ioman->GetObject("TofDigiExp"));

  if (0 == fTofDigis) LOG(fatal) << "No TofDigi array found";

  fGeoHandler         = new CbmTofGeoHandler;
  Bool_t isSimulation = kFALSE;
  Int_t iGeoVersion   = fGeoHandler->Init(isSimulation);

  switch (iGeoVersion) {
    case k12b: fTofId = new CbmTofDetectorId_v12b(); break;

    case k14a: fTofId = new CbmTofDetectorId_v14a(); break;

    default: LOG(fatal) << "Invalid geometry!!";
  }

  Int_t iNrOfCells = fDigiPar->GetNrOfModules();
  fGeoHandler->CheckGeometryVersion();

  for (Int_t icell = 0; icell < iNrOfCells; ++icell) {
    Int_t cellId =
      fDigiPar->GetCellId(icell);  // cellId is assigned in CbmTofCreateDigiPar
    fChannelInfo = fDigiPar->GetCell(cellId);

    Int_t smtype  = fGeoHandler->GetSMType(cellId);
    Int_t smodule = fGeoHandler->GetSModule(cellId);
    Int_t module  = fGeoHandler->GetCounter(cellId);
    Int_t cell    = fGeoHandler->GetCell(cellId);

    Double_t x  = fChannelInfo->GetX();
    Double_t y  = fChannelInfo->GetY();
    Double_t z  = fChannelInfo->GetZ();
    Double_t dx = fChannelInfo->GetSizex();
    Double_t dy = fChannelInfo->GetSizey();
  }

  if (!InitCalibParameter()) LOG(fatal) << "Failed to read calib parameters";

  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
  fStorDigiExp.resize(iNbSmTypes);
  LOG(info) << "Number of supermodule types is " << iNbSmTypes;

  for (Int_t iSmType = 0; iSmType < iNbSmTypes; ++iSmType) {
    Int_t iNbSm  = fDigiBdfPar->GetNbSm(iSmType);
    Int_t iNbRpc = fDigiBdfPar->GetNbRpc(iSmType);
    fStorDigiExp[iSmType].resize(iNbSm * iNbRpc);
    LOG(info) << "For the supermodule with the type: " << iSmType
              << ", the number of such supermodules is: " << iNbSm
              << ", and the number of Rpc is: " << iNbRpc;

    for (Int_t iSm = 0; iSm < iNbSm; ++iSm) {
      for (Int_t iRpc = 0; iRpc < iNbRpc; ++iRpc) {
        Int_t iNbChan = fDigiBdfPar->GetNbChan(iSmType, iRpc);
        fStorDigiExp[iSmType][iSm * iNbRpc + iRpc].resize(iNbChan);
      }
    }
  }

  deltaChannelTHisto =
    new TH1F("deltaChannelTHisto", "deltaChannelTHisto", 100, 0., 10.);
  deltaPointTHisto =
    new TH1F("deltaPointTHisto", "deltaPointTHisto", 100, 0., 1.);
  nofChannelsTHisto =
    new TH1F("nofChannelsTHisto", "nofChannelsTHisto", 20, 0., 20.);
  digiTimeHisto =
    new TH1F("digiTimeHisto", "digiTimeHisto", 10100, -100., 10000.);

  fTofHits = new TClonesArray("CbmTofHit");
  ioman->Register(
    "TofHit", "Tof", fTofHits, IsOutputBranchPersistent("TofHit"));
  fTofDigiMatchs = new TClonesArray("CbmMatch", 100);
  ioman->Register("TofDigiMatch",
                  "Tof",
                  fTofDigiMatchs,
                  IsOutputBranchPersistent("TofDigiMatch"));

  return kSUCCESS;
}

void CbmTofTBClusterizer::SetParContainers() {
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = static_cast<CbmTofDigiPar*>(rtdb->getContainer("CbmTofDigiPar"));
  fDigiBdfPar =
    static_cast<CbmTofDigiBdfPar*>(rtdb->getContainer("CbmTofDigiBdfPar"));
}

static void AddPts(set<pair<Int_t, Int_t>>& sPtsRef,
                   const CbmTofDigiExp* digi) {
  CbmMatch* match = digi->GetMatch();
  assert(match);
  Int_t nofLinks = match->GetNofLinks();

  for (int i = 0; i < nofLinks; ++i) {
    const CbmLink& link = match->GetLink(i);
    sPtsRef.insert(pair<Int_t, Int_t>(link.GetEntry(), link.GetIndex()));
  }
}

static Int_t currentEvN  = 0;
static long fullDuration = 0;

void CbmTofTBClusterizer::Exec(Option_t* option) {
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  long beginTime = ts.tv_sec * 1000000000 + ts.tv_nsec;
  // --- MC Event info (input file, entry number, start time)
  Int_t iInputNr      = 0;
  Int_t iEventNr      = 0;
  Double_t dEventTime = 0.;
  GetEventInfo(iInputNr, iEventNr, dEventTime);
  LOG(debug) << GetName() << ": Input " << iInputNr << ", event " << iEventNr
             << ", event time " << dEventTime << " ns";

  fTofHits->Clear("C");
  //fTofHits->Delete();
  fTofDigiMatchs->Delete();
  //fTofDigiMatchs->Clear("C");
  //Double_t dMaxTimeDist = fDigiBdfPar->GetMaxTimeDist();
  Double_t dMaxPairTimeDist  = 3.2;
  Double_t dMaxClustTimeDist = 0.2;
  Double_t dMaxSpaceDist     = fDigiBdfPar->GetMaxDistAlongCh();
  Int_t iNbTofDigi           = fTofDigis->GetEntriesFast();

  LOG(debug) << GetName() << ": Input " << iInputNr << ", event " << iEventNr
             << ", event time " << dEventTime << " ns"
             << ", TOF digis: " << iNbTofDigi;
  /*map<pair<Int_t, Int_t>, list<Int_t> > tofPointDigiInds;
   
   Int_t nofTofPoints = fTofPoints->GetEntriesFast();
   
   for(Int_t iDigInd = 0; iDigInd < iNbTofDigi; ++iDigInd)
   {
      CbmTofDigiExp* pDigi = static_cast<CbmTofDigiExp*> (fTofDigis->At(iDigInd));
      const CbmMatch* pMatch = pDigi->GetMatch();
      Int_t nofLinks = pMatch->GetNofLinks();

      for (Int_t iLink = 0; iLink < nofLinks; ++iLink)
      {
         const CbmLink& link = pMatch->GetLink(iLink);
         Int_t tpi = link.GetIndex();
         Int_t tpe = link.GetEntry();

         //if (tpi < nofTofPoints)
            tofPointDigiInds[pair<Int_t, Int_t> (tpe, tpi)].push_back(iDigInd);
      }
   }

   for (map<pair<Int_t, Int_t>, list<Int_t> >::const_iterator i = tofPointDigiInds.begin(); i != tofPointDigiInds.end(); ++i)
   {
      //const CbmTofPoint* pTofPoint = static_cast<const CbmTofPoint*> (fTofPoints->At(i->first));
      const list<Int_t>& pointDigis = i->second;
      set<Int_t> firedChannels;
      map<Int_t, pair<Double_t, Double_t> > channelDigis;

      for (list<Int_t>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
      {
         const CbmTofDigiExp* pTofDigi = static_cast<const CbmTofDigiExp*> (fTofDigis->At(*j));
         firedChannels.insert(pTofDigi->GetChannel());

         if (0 == pTofDigi->GetSide())
            channelDigis[pTofDigi->GetChannel()].first = pTofDigi->GetTime();
         else
            channelDigis[pTofDigi->GetChannel()].second = pTofDigi->GetTime();

         //Double_t deltaT = pTofDigi->GetTime() - pTofPoint->GetTime();
         //deltaTHisto->Fill(deltaT);
      }

      Double_t minDigiTime = std::numeric_limits<Double_t>::max();
      Double_t maxDigiTime = std::numeric_limits<Double_t>::min();

      for (map<Int_t, pair<Double_t, Double_t> >::iterator j = channelDigis.begin(); j != channelDigis.end(); ++j)
      {
         if (0 == j->second.first || 0 == j->second.second)
            continue;

         Double_t channelDelta = TMath::Abs(j->second.first - j->second.second);
         deltaChannelTHisto->Fill(channelDelta);
         Double_t channelTime = (j->second.first + j->second.second) / 2;

         if (channelTime < minDigiTime)
            minDigiTime = channelTime;

         if (channelTime > maxDigiTime)
            maxDigiTime = channelTime;
      }

      if (maxDigiTime > minDigiTime)
         deltaPointTHisto->Fill(maxDigiTime - minDigiTime);

      nofChannelsTHisto->Fill(firedChannels.size());
   }

   return;*/

  for (Int_t iDigInd = 0; iDigInd < iNbTofDigi; ++iDigInd) {
    CbmTofDigiExp* pDigi = static_cast<CbmTofDigiExp*>(fTofDigis->At(iDigInd));
    LOG(debug) << GetName() << ": digi " << iDigInd << " pointer " << pDigi;
    digiTimeHisto->Fill(pDigi->GetTime());

    if (0 == pDigi->GetSide()) {  // 0 - top side
      LOG(debug) << "top side";
      LOG(debug) << "Type" << pDigi->GetType();
      LOG(debug) << "RPC" << pDigi->GetRpc();
      LOG(debug) << "Channel" << pDigi->GetChannel();
      LOG(debug) << "Time" << pDigi->GetTime();
      LOG(debug) << "SM " << pDigi->GetSm();
      LOG(debug) << "NbRpc " << fDigiBdfPar->GetNbRpc(pDigi->GetType());
      Int_t type        = pDigi->GetType();
      Int_t superModule = pDigi->GetSm();
      Int_t rpc         = pDigi->GetRpc();
      Int_t nofRpc      = fDigiBdfPar->GetNbRpc(type + rpc);
      Int_t channel     = pDigi->GetChannel();
      Double_t time     = pDigi->GetTime();
      Int_t index1      = type;
      Int_t index2      = superModule * nofRpc;
      Int_t index3      = channel;
      LOG(debug) << "Index 1 " << index1;
      LOG(debug) << "Index 2 " << index2;
      LOG(debug) << "Index 2 " << index3;

      LOG(debug) << "Size 1 " << fStorDigiExp.size();
      LOG(debug) << "Size 2 " << fStorDigiExp[index1].size();
      LOG(debug) << "Size 3 " << fStorDigiExp[index1][index2].size();


      fStorDigiExp[pDigi->GetType()]
                  [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                   + pDigi->GetRpc()][pDigi->GetChannel()]
                    .topDigis[pDigi->GetTime()] = {pDigi, iDigInd};
      LOG(debug) << "done";
    } else {
      LOG(debug) << "bottom side";
      fStorDigiExp[pDigi->GetType()]
                  [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType())
                   + pDigi->GetRpc()][pDigi->GetChannel()]
                    .bottomDigis[pDigi->GetTime()] = {pDigi, iDigInd};
    }

    // apply calibration vectors
    /*pDigi->SetTime(pDigi->GetTime() - // calibrate Digi Time
         fvCPTOff[pDigi->GetType()]
         [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
         [pDigi->GetChannel()]
         [pDigi->GetSide()]);

      pDigi->SetTot(pDigi->GetTot() * // calibrate Digi ToT
         fvCPTotGain[pDigi->GetType()]
         [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
         [pDigi->GetChannel()]
         [pDigi->GetSide()]);

      // walk correction
      Double_t dTotBinSize = (TOTMax - TOTMin) / 2. / nbClWalkBinX;
      Int_t iWx = (Int_t) ((pDigi->GetTot() - TOTMin / 2.) / dTotBinSize);

      if (0 > iWx)
         iWx = 0;

      if (iWx > nbClWalkBinX)
         iWx = nbClWalkBinX - 1;

      Double_t dDTot = (pDigi->GetTot() - TOTMin / 2.) / dTotBinSize - (Double_t) iWx - 0.5;
      Double_t dWT = fvCPWalk[pDigi->GetType()]
         [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
         [pDigi->GetChannel()]
         [pDigi->GetSide()]
         [iWx];

      if (dDTot > 0)
      { // linear interpolation to next bin
         dWT += dDTot * (fvCPWalk[pDigi->GetType()]
            [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
            [pDigi->GetChannel()]
            [pDigi->GetSide()]
            [iWx + 1]
            - fvCPWalk[pDigi->GetType()]
            [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
            [pDigi->GetChannel()]
            [pDigi->GetSide()]
            [iWx]);
      }
      else // dDTot < 0,  linear interpolation to next bin
      {
         dWT -= dDTot * (fvCPWalk[pDigi->GetType()]
            [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
            [pDigi->GetChannel()]
            [pDigi->GetSide()]
            [iWx - 1]
            - fvCPWalk[pDigi->GetType()]
            [pDigi->GetSm() * fDigiBdfPar->GetNbRpc(pDigi->GetType()) + pDigi->GetRpc()]
            [pDigi->GetChannel()]
            [pDigi->GetSide()]
            [iWx]);
      }

      pDigi->SetTime(pDigi->GetTime() - dWT); // calibrate Digi Time*/
  }  // iDigiInd

  LOG(debug) << GetName() << ": TOF digis sorted";

  Double_t hitpos_local[3];
  Double_t hitpos[3];
  Int_t fiNbHits   = 0;
  Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();

  for (Int_t iSmType = 0; iSmType < iNbSmTypes; ++iSmType) {
    vector<vector<ChannelDigis>>& digisOfType = fStorDigiExp[iSmType];
    Int_t iNbSm                               = fDigiBdfPar->GetNbSm(iSmType);
    Int_t iNbRpc                              = fDigiBdfPar->GetNbRpc(iSmType);

    for (Int_t iSm = 0; iSm < iNbSm; ++iSm) {
      for (Int_t iRpc = 0; iRpc < iNbRpc; ++iRpc) {
        vector<ChannelDigis>& digisOfRpc =
          digisOfType[iSm * fDigiBdfPar->GetNbRpc(iSmType) + iRpc];
        Int_t iNbCh = fDigiBdfPar->GetNbChan(iSmType, iRpc);

        // Vertical strips => Y comes from bottom top time difference
        for (Int_t iCh = 0; iCh < iNbCh; ++iCh) {
          ChannelDigis& digisOfChannel = digisOfRpc[iCh];
          map<Double_t, ChannelDigis::DigiDesc>& topDigis =
            digisOfChannel.topDigis;
          map<Double_t, ChannelDigis::DigiDesc>& bottomDigis =
            digisOfChannel.bottomDigis;

          if (bottomDigis.empty()) {
            topDigis.clear();
            continue;
          }

          map<Double_t, ChannelDigis::DigiPair>& digiPairs =
            digisOfChannel.digiPairs;

          for (map<Double_t, ChannelDigis::DigiDesc>::iterator topDigiIter =
                 topDigis.begin();
               topDigiIter != topDigis.end();
               ++topDigiIter) {
            Double_t topTime = topDigiIter->first;
            map<Double_t, ChannelDigis::DigiDesc>::iterator bottomDigiIter =
              bottomDigis.lower_bound(topTime);

            if (bottomDigiIter == bottomDigis.end())
              --bottomDigiIter;
            else if (bottomDigiIter->first > topTime) {
              Double_t deltaT = bottomDigiIter->first - topTime;

              if (deltaT > dMaxPairTimeDist
                  && bottomDigiIter != bottomDigis.begin())
                --bottomDigiIter;
              else {
                map<Double_t, ChannelDigis::DigiDesc>::iterator
                  bottomDigiIter2 = bottomDigiIter;
                --bottomDigiIter2;

                Double_t deltaT2 = topTime - bottomDigiIter2->first;

                if (deltaT2 < deltaT) bottomDigiIter = bottomDigiIter2;
              }
            }

            if (TMath::Abs(bottomDigiIter->first - topTime) > dMaxPairTimeDist)
              continue;

            Double_t y = fvCPSigPropSpeed[iSmType][iRpc]
                         * (bottomDigiIter->first - topTime) / 2;

            CbmTofDetectorInfo xDetInfo(kTof, iSmType, iSm, iRpc, 0, iCh);
            Int_t iChId  = fTofId->SetDetectorInfo(xDetInfo);
            fChannelInfo = fDigiPar->GetCell(iChId);

            if (0 == fChannelInfo) continue;

            if (TMath::Abs(y) > fChannelInfo->GetSizey() / 2) continue;

            Double_t digiPairTime   = (topTime + bottomDigiIter->first) / 2;
            digiPairs[digiPairTime] = {
              y, topDigiIter->second, bottomDigiIter->second};
          }  // for (map<Double_t, CbmTofDigiExp*>::iterator topDigiIter = topDigis.begin(); topDigiIter != topDigis.end(); ++topDigiIter)

          topDigis.clear();
          bottomDigis.clear();
        }  // for (Int_t iCh = 0; iCh < iNbCh; ++iCh)

        gGeoManager->FindNode(
          fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
        gGeoManager->GetCurrentMatrix();
        gGeoManager->CdUp();
        gGeoManager->GetCurrentMatrix();

        for (Int_t iCh = 0; iCh < iNbCh; ++iCh)  // 2
        {
          map<Double_t, ChannelDigis::DigiPair>& digiPairs =
            digisOfRpc[iCh].digiPairs;

          for (map<Double_t, ChannelDigis::DigiPair>::iterator chIter =
                 digiPairs.begin();
               chIter != digiPairs.end();
               ++chIter) {
            Double_t chTime = chIter->first;
            Double_t chY    = chIter->second.y;
            Double_t dTotS  = chIter->second.topDigi.pDigi->GetTot()
                             + chIter->second.bottomDigi.pDigi->GetTot();
            Double_t dWeightedTime = chTime * dTotS;
            Double_t dWeightedPosY = chY * dTotS;
            Double_t dWeightedPosX =
              (iCh - Double_t(iNbCh) / 2) * fChannelInfo->GetSizex() * dTotS;
            Double_t dWeightedTimeErrorS =
              chIter->second.topDigi.pDigi->GetTot()
                * chIter->second.topDigi.pDigi->GetTot()
              + chIter->second.bottomDigi.pDigi->GetTot()
                  * chIter->second.bottomDigi.pDigi->GetTot();
            Double_t dWeightsSum = dTotS;
            CbmMatch* digiMatch  = new CbmMatch;
            digiMatch->AddLink(
              CbmLink(0., chIter->second.topDigi.digiInd, iEventNr, iInputNr));
            digiMatch->AddLink(CbmLink(
              0., chIter->second.bottomDigi.digiInd, iEventNr, iInputNr));
            set<pair<Int_t, Int_t>> sPtsRef;
            AddPts(sPtsRef, chIter->second.topDigi.pDigi);
            AddPts(sPtsRef, chIter->second.bottomDigi.pDigi);

            for (Int_t iNeighCh = iCh + 1;
                 iNeighCh < iNbCh /* && iNeighCh - iCh < 4*/;
                 ++iNeighCh) {
              map<Double_t, ChannelDigis::DigiPair>& neighDigiPairs =
                digisOfRpc[iNeighCh].digiPairs;

              if (neighDigiPairs.empty()) break;

              map<Double_t, ChannelDigis::DigiPair>::iterator neighIter =
                neighDigiPairs.lower_bound(chTime);

              if (neighIter == neighDigiPairs.end())
                --neighIter;
              else if (neighIter->first > chTime
                       && neighIter != neighDigiPairs.begin()) {
                Double_t deltaTHigh = neighIter->first - chTime;
                map<Double_t, ChannelDigis::DigiPair>::iterator neighIter_1 =
                  neighIter;
                --neighIter_1;
                Double_t deltaTLow = chTime - neighIter_1->first;

                if (deltaTLow < deltaTHigh) neighIter = neighIter_1;
              }

              if (TMath::Abs(neighIter->first - chTime) > dMaxClustTimeDist
                  || TMath::Abs(neighIter->second.y - chY) > dMaxSpaceDist)
                break;

              Double_t dTotNeighS =
                neighIter->second.topDigi.pDigi->GetTot()
                + neighIter->second.bottomDigi.pDigi->GetTot();
              dWeightedTimeErrorS +=
                neighIter->second.topDigi.pDigi->GetTot()
                  * neighIter->second.topDigi.pDigi->GetTot()
                + neighIter->second.bottomDigi.pDigi->GetTot()
                    * neighIter->second.bottomDigi.pDigi->GetTot();
              dWeightedTime += neighIter->first * dTotNeighS;
              dWeightedPosY += neighIter->second.y * dTotNeighS;
              dWeightedPosX += (iNeighCh - Double_t(iNbCh) / 2)
                               * fChannelInfo->GetSizex() * dTotNeighS;
              dWeightsSum += dTotNeighS;

              digiMatch->AddLink(CbmLink(
                0., neighIter->second.topDigi.digiInd, iEventNr, iInputNr));
              digiMatch->AddLink(CbmLink(
                0., neighIter->second.bottomDigi.digiInd, iEventNr, iInputNr));
              AddPts(sPtsRef, neighIter->second.topDigi.pDigi);
              AddPts(sPtsRef, neighIter->second.bottomDigi.pDigi);
              neighDigiPairs.erase(neighIter);
            }  // for (Int_t iNeighCh = iCh + 1; iNeighCh < iNbCh; ++iNeighCh)

            Double_t clusterTime = dWeightedTime / dWeightsSum;
            Double_t timeRes     = fDigiBdfPar->GetFeeTimeRes();
            Double_t clusterTimeError =
              TMath::Sqrt(dWeightedTimeErrorS) * timeRes / dWeightsSum;
            Double_t clusterY = dWeightedPosY / dWeightsSum;
            Double_t clusterX = dWeightedPosX / dWeightsSum;
            hitpos_local[0]   = clusterX;
            hitpos_local[1]   = clusterY;
            hitpos_local[2]   = 0;
            hitpos[0]         = 0;
            hitpos[1]         = 0;
            hitpos[2]         = 0;
            gGeoManager->LocalToMaster(hitpos_local, hitpos);
            TVector3 hitPos(hitpos[0], hitpos[1], hitpos[2]);
            // Simple errors, not properly done at all for now
            // Right way of doing it should take into account the weight distribution
            // and real system time resolution
            TVector3 hitPosErr(
              fChannelInfo->GetSizex()
                / sqrt(12.0),  // Single strips approximation
              fDigiBdfPar->GetFeeTimeRes()
                * fvCPSigPropSpeed[iSmType]
                                  [iRpc],  // Use the electronics resolution
              fDigiBdfPar->GetNbGaps(iSmType, iRpc)
                * fDigiBdfPar->GetGapSize(iSmType, iRpc) / 10.0
                /             // Change gap size in cm
                sqrt(12.0));  // Use full RPC thickness as "Channel" Z size
            Int_t iChm = floor((clusterX + Double_t(iNbCh) / 2)
                               / fChannelInfo->GetSizex());
            Int_t iDetId =
              CbmTofAddress::GetUniqueAddress(iSm, iRpc, iChm, 0, iSmType);
            new ((*fTofHits)[fiNbHits]) CbmTofHit(
              iDetId,
              hitPos,
              hitPosErr,  //local detector coordinates
              fiNbHits,
              clusterTime * fOutTimeFactor,
              sPtsRef
                .size(),  // flag  = number of TofPoints generating the cluster
              0);
            static_cast<CbmTofHit*>(fTofHits->At(fiNbHits))
              ->SetTimeError(clusterTimeError);
            new ((*fTofDigiMatchs)[fiNbHits]) CbmMatch(*digiMatch);
            delete digiMatch;
            ++fiNbHits;
          }  // for (map<Double_t, pair<CbmTofDigiExp*, CbmTofDigiExp*> >::iterator chIter = digiPairs.begin(); chIter != digiPairs.end(); ++chIter)

          digiPairs.clear();
        }  // for (Int_t iCh = 0; iCh < iNbCh; ++iCh) 2
      }    // for (Int_t iRpc = 0; iRpc < iNbRpc; ++iRpc)
    }      // for (Int_t iSm = 0; iSm < iNbSm; ++iSm)
  }        // for (Int_t iSmType = 0; iSmType < iNbSmTypes; ++iSmType)

  ++currentEvN;
  clock_gettime(CLOCK_REALTIME, &ts);
  long endTime = ts.tv_sec * 1000000000 + ts.tv_nsec;
  fullDuration += endTime - beginTime;
}

static void SaveHisto(TH1* histo) {
  TFile* curFile    = TFile::CurrentFile();
  TString histoName = histo->GetName();
  histoName += ".root";
  TFile fh(histoName.Data(), "RECREATE");
  histo->Write();
  fh.Close();
  delete histo;
  TFile::CurrentFile() = curFile;
}

void CbmTofTBClusterizer::Finish() {
  SaveHisto(deltaChannelTHisto);
  SaveHisto(deltaPointTHisto);
  SaveHisto(nofChannelsTHisto);
  SaveHisto(digiTimeHisto);
  cout << "ToF Time Based clustering runtime: " << fullDuration << endl;
}

void CbmTofTBClusterizer::GetEventInfo(Int_t& inputNr,
                                       Int_t& eventNr,
                                       Double_t& eventTime) {

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
    if (!FairRunSim::Instance())
      LOG(fatal) << GetName() << ": neither SIM nor ANA run.";
    FairMCEventHeader* event = FairRunSim::Instance()->GetMCEventHeader();
    inputNr                  = 0;
    eventNr                  = event->GetEventID();
    eventTime                = 0.;
  }
}
