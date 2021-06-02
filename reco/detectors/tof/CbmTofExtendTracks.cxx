/** @file CbmTofExtendTracks.cxx
  * @copyright Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann **/

// -------------------------------------------------------------------------
// -----                  CbmTofExtendTracks source file                -----
// -----                  Created 22/12/20  by N. Herrmann             -----
// -------------------------------------------------------------------------

#include "CbmTofExtendTracks.h"

#include "CbmDefs.h"
#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmTofAddress.h"  // in cbmdata/tof
#include "CbmTofCalibrator.h"
#include "CbmTofCell.h"             // in tof/TofData
#include "CbmTofCreateDigiPar.h"    // in tof/TofTools
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v21a.h"  // in cbmdata/tof
#include "CbmTofDigiBdfPar.h"       // in tof/TofParam
#include "CbmTofDigiPar.h"          // in tof/TofParam
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofHit.h"
#include "CbmTofTrackFinder.h"
#include "CbmTofTrackFinderNN.h"
#include "CbmTofTrackFitter.h"
#include "CbmTofTracklet.h"
#include "CbmTofTrackletParam.h"
#include "CbmTofTrackletTools.h"
#include "CbmVertex.h"

#include "FairLogger.h"
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TGeoManager.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TString.h"

#include <iostream>
#include <vector>

#include "LKFMinuit.h"

using std::cout;
using std::endl;
using std::vector;

const Int_t NDefSetup = 100;
static LKFMinuit fMinuit;
static Int_t fiTS      = 0;
static Double_t dSUT_z = 0.;

ClassImp(CbmTofExtendTracks);

CbmTofExtendTracks* CbmTofExtendTracks::fInstance = 0;

// -----   Default constructor   -------------------------------------------
CbmTofExtendTracks::CbmTofExtendTracks() : CbmTofExtendTracks::CbmTofExtendTracks("TofExtendTracks", "Main", NULL)
{
  if (!fInstance) fInstance = this;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTofExtendTracks::CbmTofExtendTracks(const char* name, const char* /*title*/, CbmTofTrackFinder* finder)
  : FairTask(name)
  , fFinder(finder)
  , fFitter(NULL)
  , fTrackletTools(NULL)
  , fTofCalibrator(NULL)
  , fEventsColl(NULL)
  , fTofHitArrayIn(NULL)
  , fStsHitArrayIn(NULL)
  , fMuchHitArrayIn(NULL)
  , fRichHitArrayIn(NULL)
  , fTofMatchArrayIn(NULL)
  , fTofHitArray(NULL)
  , fTofTrackArrayIn(NULL)
  , fTrackArrayOut(nullptr)
  , fvTofHitIndex()
  , fvTofTrackIndex()
  , fvStsHitIndex()
  , fvMuchHitIndex()
  , fvRichHitIndex()
  , fvTofStationZ()
  , fvStsStationZ()
  , fvMuchStationZ()
  , fvRichStationZ()
  , fvAllHitPointer()
  , fvTrkCalHits()
  , fvTrkPar()
  , fvToff()
  , fvXoff()
  , fvYoff()
  , fvZoff()
  , fvTsig()
  , fvXsig()
  , fvYsig()
  , fvZsig()
  , fhMulCorTrkTof(NULL)
  , fhMulCorTrkSts(NULL)
  , fhMulCorTrkMuch(NULL)
  , fhMulCorTrkRich(NULL)
  , fhPosCorTrkTof(NULL)
  , fhPosCorTrkSts(NULL)
  , fhPosCorTrkMuch(NULL)
  , fhPosCorTrkRich(NULL)
  , fhTrkStationDX()
  , fhTrkStationDY()
  , fhTrkStationDZ()
  , fhTrkStationDT()
  , fhTrkStationNHits()
  , fhTrkStationDXDY()
  , fhTrkPullDX()
  , fhTrkPullDY()
  , fhTrkPullDT()
  , fhExt_Toff(NULL)
  , fhExt_Xoff(NULL)
  , fhExt_Yoff(NULL)
  , fhExt_Zoff(NULL)
  , fhExt_Tsig(NULL)
  , fhExt_Xsig(NULL)
  , fhExt_Ysig(NULL)
  , fhExt_Zsig(NULL)
  , fhExt_TrkSizVel()
  , fhExt_TrkSizChiSq()
  , fhExtSutXY_Found(NULL)
  , fhExtSutXY_Missed(NULL)
  , fhExtSutXY_DX(NULL)
  , fhExtSutXY_DY(NULL)
  , fhExtSutXY_DT(NULL)
  , fVTXNorm(0.)
  , fVTX_T(0.)
  , fVTX_X(0.)
  , fVTX_Y(0.)
  , fVTX_Z(0.)
  , fT0MAX(0.5)
  , fiTrkHitsMin(0)
  , fdTrkCutDX(100.)
  , fdTrkCutDY(100.)
  , fdTrkCutDT(100.)
  , fdChi2Max(5.)
  , fiCorSrc(0)
  , fiCorMode(0)
  , fiAddStations(0)
  , fiReqStations(-1)
  , fiStationUT(-1)
  , fiCutStationMaxHitMul(1000)
  , fiNTrkTofMax(1000)
  , fiEvent(0)
{
  if (!fInstance) fInstance = this;
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTofExtendTracks::~CbmTofExtendTracks()
{
  if (fInstance == this) fInstance = 0;
}
// -------------------------------------------------------------------------


// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmTofExtendTracks::Init()
{

  fTrackletTools = new CbmTofTrackletTools();  // initialize tools

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- CbmTofExtendTracks::Init: "
         << "RootManager not instantiated!" << endl;
    return kFATAL;
  }

  ioman->InitSink();

  fEventsColl = dynamic_cast<TClonesArray*>(ioman->GetObject("Event"));
  if (!fEventsColl) fEventsColl = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (!fEventsColl) { LOG(fatal) << "CbmEvent not found in input file"; }

  // Get TOF hit Array
  fTofHitArrayIn = (TClonesArray*) ioman->GetObject("TofCalHit");
  if (!fTofHitArrayIn) {
    fTofHitArrayIn = (TClonesArray*) ioman->GetObject("TofHit");
    if (!fTofHitArrayIn) {
      LOG(fatal) << "-W- CbmTofExtendTracks::Init: No TofHit array!";
      return kERROR;
    }
  }
  else {
    LOG(info) << "-I- CbmTofExtendTracks::Init: TofCalHit array!";
  }

  // Get TOF Track Array
  fTofTrackArrayIn = (TClonesArray*) ioman->GetObject("TofTracks");
  if (!fTofTrackArrayIn) {
    LOG(fatal) << "-W- CbmTofExtendTracks::Init: No TofTrack array!";
    return kERROR;
  }

  // Get Sts hit Array
  fStsHitArrayIn = (TClonesArray*) ioman->GetObject("StsHit");
  if (!fStsHitArrayIn) {
    LOG(fatal) << "-W- CbmTofExtendTracks::Init: No StsHit array!";
    return kERROR;
  }

  // Get Much hit Array
  fMuchHitArrayIn = (TClonesArray*) ioman->GetObject("MuchPixelHit");
  if (!fMuchHitArrayIn) {
    LOG(fatal) << "-W- CbmTofExtendTracks::Init: No MuchHit array!";
    return kERROR;
  }

  // Get Rich hit Array
  fRichHitArrayIn = (TClonesArray*) ioman->GetObject("RichHit");
  if (!fRichHitArrayIn) {
    LOG(fatal) << "-W- CbmTofExtendTracks::Init: No RichHit array!";
    return kERROR;
  }

  if (kFALSE == InitParameters()) return kFATAL;

  CreateHistograms();

  fMinuit.Initialize();

  LOG(info) << "CbmTofExtendTracks initialized";
  return kSUCCESS;
}
// -------------------------------------------------------------------------
/************************************************************************************/
Bool_t CbmTofExtendTracks::LoadCalParameter()
{
  UInt_t NSt = fMapStationZ.size();
  fvToff.resize(NSt);
  for (uint i = 0; i < NSt; i++)
    fvToff[i] = 0.;
  fvXoff.resize(NSt);
  for (uint i = 0; i < NSt; i++)
    fvXoff[i] = 0.;
  fvYoff.resize(NSt);
  for (uint i = 0; i < NSt; i++)
    fvYoff[i] = 0.;
  fvZoff.resize(NSt);
  for (uint i = 0; i < NSt; i++)
    fvZoff[i] = 0.;

  if (fCalParFileName.IsNull()) return kTRUE;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fCalParFile = new TFile(fCalParFileName, "");
  if (NULL == fCalParFile) {
    LOG(error) << "CbmTofExtendTracks::LoadCalParameter: "
               << "file " << fCalParFileName << " does not exist!";
    return kTRUE;
  }

  LOG(info) << "CbmTofExtendTracks::LoadCalParameter: "
            << " read from file " << fCalParFileName;

  TH1D* fht = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Toff"));
  TH1D* fhx = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Xoff"));
  TH1D* fhy = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Yoff"));
  TH1D* fhz = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Zoff"));

  TH1D* fhts = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Tsig"));
  TH1D* fhxs = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Xsig"));
  TH1D* fhys = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Ysig"));
  TH1D* fhzs = (TH1D*) gDirectory->FindObjectAny(Form("hExt_Zsig"));

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  if (NULL != fht) {
    fhExt_Toff = (TH1D*) fht->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvToff[iSt] = fhExt_Toff->GetBinContent(iSt + 1);
    }
  }
  if (NULL != fhx) {
    fhExt_Xoff = (TH1D*) fhx->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvXoff[iSt] = fhExt_Xoff->GetBinContent(iSt + 1);
    }
  }
  if (NULL != fhy) {
    fhExt_Yoff = (TH1D*) fhy->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvYoff[iSt] = fhExt_Yoff->GetBinContent(iSt + 1);
    }
  }
  if (NULL != fhz) {
    fhExt_Zoff = (TH1D*) fhz->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvZoff[iSt] = fhExt_Zoff->GetBinContent(iSt + 1);
    }
  }

  if (NULL != fhts) {
    fhExt_Tsig = (TH1D*) fhts->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvTsig[iSt] = fhExt_Tsig->GetBinContent(iSt + 1);
    }
  }
  if (NULL != fhxs) {
    fhExt_Xsig = (TH1D*) fhxs->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvXsig[iSt] = fhExt_Xsig->GetBinContent(iSt + 1);
    }
  }
  if (NULL != fhys) {
    fhExt_Ysig = (TH1D*) fhys->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvYsig[iSt] = fhExt_Ysig->GetBinContent(iSt + 1);
    }
  }
  if (NULL != fhzs) {
    fhExt_Zsig = (TH1D*) fhzs->Clone();
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fvZsig[iSt] = fhExt_Zsig->GetBinContent(iSt + 1);
    }
  }


  fCalParFile->Close();

  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t CbmTofExtendTracks::InitParameters() { return kTRUE; }
// -----  SetParContainers -------------------------------------------------
void CbmTofExtendTracks::SetParContainers()
{
  /*
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  */
}
// -------------------------------------------------------------------------

Bool_t CbmTofExtendTracks::UpdateCalHistos()
{
  if (fiEvent <= NDefSetup) return kTRUE;


  while (fiCorMode > 0) {
    Int_t iCorMode = fiCorMode % 10;
    fiCorMode /= 10;
    Int_t iLev = fiCorSrc / 10;
    LOG(info) << "UpdateCalHist on level " << iLev << " from src " << fiCorSrc << " in mode " << iCorMode;
    switch (iCorMode) {
      case 0: {  // T
        TH2* hCorDT = NULL;
        if (fiCorSrc % 10 == 0) hCorDT = fhTrkStationDT[iLev];
        else
          hCorDT = fhTrkPullDT[iLev];
        if (NULL != hCorDT) {
          Double_t nx = hCorDT->GetNbinsX();
          if (NULL == fhExt_Toff) {
            fhExt_Toff = new TH1D("hExt_Toff", ";station;Toff (ns)", nx, 0, nx);
            LOG(warn) << "Created " << fhExt_Toff->GetName();
          }
          if (NULL == fhExt_Tsig) {
            fhExt_Tsig = new TH1D("hExt_Tsig", ";station;Tsig (ns)", nx, 0, nx);
            LOG(warn) << "Created " << fhExt_Tsig->GetName();
          }
          for (Int_t ix = 0; ix < nx; ix++) {
            Double_t dVal   = fhExt_Toff->GetBinContent(ix + 1);
            TH1D* hpy       = hCorDT->ProjectionY(Form("%s_py%d", hCorDT->GetName(), ix), ix + 1, ix + 1, "");
            Double_t dFMean = 0.;
            LOG(warn) << "TOff Entries for station " << ix << ": " << hpy->GetEntries();
            if (hpy->GetEntries() > 100) {
              Int_t iBmax    = hpy->GetMaximumBin();
              TAxis* xaxis   = hpy->GetXaxis();
              Double_t dMean = xaxis->GetBinCenter(iBmax);  //X-value of bin with maximal content
              Double_t dLim  = 1. * hpy->GetRMS();
              //Double_t dLim = 5. * hpy->GetBinWidth(1);
              if (dLim > 0.) {
                //TF1 * f = new TF1 ("f","gaus");
                //f->SetParLimits(2,dMean - dLim, dMean + dLim);
                //f->SetParLimits(3,0.,dLim);
                TFitResultPtr fRes =
                  //        hpy->Fit(f,"B");
                  hpy->Fit("gaus", "SQM", "", dMean - dLim, dMean + dLim);
                Int_t iFitStatus = fRes;
                //LOG(warn)<<Form("TRes 0x%08x ",(UInt_t)iFitStatus);

                if (iFitStatus != -1) {
                  dFMean = fRes->Parameter(1);
                  dVal -= dFMean;
                  fhExt_Tsig->SetBinContent(ix + 1, fRes->Parameter(2));
                }
                else
                  dVal -= dMean;
                LOG(warn) << "Update hExt_Toff Ind " << ix << ": Old " << fhExt_Toff->GetBinContent(ix + 1)
                          << ", FitMean " << dFMean << " => " << dVal << ", width "
                          << fhExt_Tsig->GetBinContent(ix + 1);
              }
            }
            else {
              LOG(warn) << "Update hExt_Toff " << ix << ": insufficient counts: " << hpy->GetEntries();
            }
            fhExt_Toff->SetBinContent(ix + 1, dVal);
          }
        }
      } break;
      case 1: {  // X
        TH2* hCorDX = NULL;
        if (fiCorSrc % 10 == 0) hCorDX = fhTrkStationDX[iLev];
        else
          hCorDX = fhTrkPullDX[iLev];
        if (NULL != hCorDX) {
          Double_t nx = hCorDX->GetNbinsX();
          if (NULL == fhExt_Xoff) {
            fhExt_Xoff = new TH1D("hExt_Xoff", ";station;Xoff (cm)", nx, 0, nx);
            LOG(warn) << "Created " << fhExt_Xoff->GetName();
          }
          if (NULL == fhExt_Xsig) {
            fhExt_Xsig = new TH1D("hExt_Xsig", ";station;Xsig (cm)", nx, 0, nx);
            LOG(warn) << "Created " << fhExt_Xsig->GetName();
          }
          for (Int_t ix = 0; ix < nx; ix++) {
            Double_t dVal   = fhExt_Xoff->GetBinContent(ix + 1);
            TH1D* hpy       = hCorDX->ProjectionY(Form("%s_py%d", hCorDX->GetName(), ix), ix + 1, ix + 1, "");
            Double_t dFMean = 0.;
            LOG(warn) << "XOff Entries for station " << ix << ": " << hpy->GetEntries();
            if (hpy->GetEntries() > 100) {
              Int_t iBmax    = hpy->GetMaximumBin();
              TAxis* xaxis   = hpy->GetXaxis();
              Double_t dMean = xaxis->GetBinCenter(iBmax);  //X-value of bin with maximal content
              //Double_t dLim = 1. * hpy->GetRMS();
              Double_t dLim = 5. * hpy->GetBinWidth(1);
              if (dLim > 0.) {
                //TF1 * f = new TF1 ("f","gaus");
                //f->SetParLimits(2,dMean - dLim, dMean + dLim);
                //f->SetParLimits(3,0.,dLim);
                TFitResultPtr fRes =
                  //  hpy->Fit(f,"B");
                  hpy->Fit("gaus", "SQM", "", dMean - dLim, dMean + dLim);
                Int_t iFitStatus = fRes;
                //LOG(warn)<<Form("XRes 0x%08x ",(UInt_t)iFitStatus);
                if (iFitStatus != -1) {  // check validity of fit
                  dFMean = fRes->Parameter(1);
                  dVal -= dFMean;
                  fhExt_Xsig->SetBinContent(ix + 1, fRes->Parameter(2));
                }
                else
                  dVal -= dMean;
                LOG(warn) << "Update hExt_Xoff Ind " << ix << ": Old " << fhExt_Xoff->GetBinContent(ix + 1)
                          << ", FitMean " << dFMean << " => " << dVal << ", width "
                          << fhExt_Xsig->GetBinContent(ix + 1);
              }
            }
            else {
              LOG(warn) << "Update hExt_Xoff " << ix << ": insufficient counts: " << hpy->GetEntries();
            }
            fhExt_Xoff->SetBinContent(ix + 1, dVal);
          }
        }
      } break;
      case 2: {  // Y
        TH2* hCorDY = NULL;
        if (fiCorSrc % 10 == 0) hCorDY = fhTrkStationDY[iLev];
        else
          hCorDY = fhTrkPullDY[iLev];
        if (NULL != hCorDY) {
          Double_t nx = hCorDY->GetNbinsX();
          if (NULL == fhExt_Yoff) {
            fhExt_Yoff = new TH1D("hExt_Yoff", ";station;Yoff (cm)", nx, 0, nx);
            LOG(warn) << "Created " << fhExt_Yoff->GetName();
          }
          if (NULL == fhExt_Ysig) {
            fhExt_Ysig = new TH1D("hExt_Ysig", ";station;Ysig (cm)", nx, 0, nx);
            LOG(warn) << "Created " << fhExt_Ysig->GetName();
          }
          for (Int_t ix = 0; ix < nx; ix++) {
            Double_t dVal   = fhExt_Yoff->GetBinContent(ix + 1);
            TH1D* hpy       = hCorDY->ProjectionY(Form("%s_py%d", hCorDY->GetName(), ix), ix + 1, ix + 1, "");
            Double_t dFMean = 0.;
            LOG(warn) << "YOff Entries for station " << ix << ": " << hpy->GetEntries();
            if (hpy->GetEntries() > 100) {
              Int_t iBmax    = hpy->GetMaximumBin();
              TAxis* xaxis   = hpy->GetXaxis();
              Double_t dMean = xaxis->GetBinCenter(iBmax);  //X-value of bin with maximal content
              //Double_t dLim = 1. * hpy->GetRMS();
              Double_t dLim = 5. * hpy->GetBinWidth(1);
              if (dLim > 0.) {
                //TF1 * f = new TF1 ("f","gaus");
                //f->SetParLimits(2,dMean - dLim, dMean + dLim);
                //f->SetParLimits(3,0.,dLim);
                // hpy->Draw("");
                LOG(warn) << "Fit gaus with " << dMean << ", " << dLim;
                TFitResultPtr fRes =
                  //  hpy->Fit(f,"B");
                  hpy->Fit("gaus", "SQM", "", dMean - dLim, dMean + dLim);
                Int_t iFitStatus = fRes;
                //LOG(warn)<<Form("YRes 0x%08x ",iFitStatus);
                if (iFitStatus != -1) {  // check validity of fit
                  dFMean = fRes->Parameter(1);
                  dVal -= dFMean;
                  fhExt_Ysig->SetBinContent(ix + 1, fRes->Parameter(2));
                }
                else
                  dVal -= dMean;

                LOG(warn) << "Update hExt_Yoff Ind " << ix << ": Old " << fhExt_Yoff->GetBinContent(ix + 1)
                          << ", FitMean " << dFMean << " => " << dVal << ", width "
                          << fhExt_Ysig->GetBinContent(ix + 1);
              }
            }
            else {
              LOG(warn) << "Update hExt_Yoff " << ix << ": insufficient counts: " << hpy->GetEntries();
            }
            fhExt_Yoff->SetBinContent(ix + 1, dVal);
          }
        }
      } break;
      default: LOG(info) << "Correction Mode not implemented";
    }  // switch end
  }
  return kTRUE;
}
Bool_t CbmTofExtendTracks::WriteHistos()
{
  if (fiCorMode < 0) return kTRUE;
  // Write calibration histogramms to the file
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;
  TFile* fHist       = new TFile(fCalOutFileName, "RECREATE");
  fHist->cd();

  if (NULL != fhExt_Toff) fhExt_Toff->Write();
  if (NULL != fhExt_Xoff) fhExt_Xoff->Write();
  if (NULL != fhExt_Yoff) fhExt_Yoff->Write();
  if (NULL != fhExt_Zoff) fhExt_Zoff->Write();

  gFile      = oldFile;
  gDirectory = oldDir;

  fHist->Close();
  return kTRUE;
}


// -----   Public method Exec   --------------------------------------------
void CbmTofExtendTracks::Exec(Option_t* opt)
{

  if (!fEventsColl) {
    //    fTofHitArray = (TClonesArray*)fTofHitArrayIn->Clone();
    LOG(fatal) << "Analysis needs EventsColl ";
    // ExecExtend(opt);
  }
  else {
    LOG(info) << "ExtExec TS " << fiTS << " with " << fEventsColl->GetEntriesFast() << " evts";
    fiTS++;
    for (Int_t iEvent = 0; iEvent < fEventsColl->GetEntriesFast(); iEvent++) {
      CbmEvent* tEvent = dynamic_cast<CbmEvent*>(fEventsColl->At(iEvent));
      LOG(info) << "Process TS event " << iEvent << " with " << tEvent->GetNofData(ECbmDataType::kT0Hit) << " T0, "
                << tEvent->GetNofData(ECbmDataType::kTofHit) << " TOF, " << tEvent->GetNofData(ECbmDataType::kStsHit)
                << " STS, " << tEvent->GetNofData(ECbmDataType::kMuchPixelHit) << " MUCH hits";

      ExecExtend(opt, tEvent);
    }
  }
}

void CbmTofExtendTracks::ExecExtend(Option_t* /*opt*/, CbmEvent* tEvent)
{
  fiEvent++;
  if (fiEvent == NDefSetup) CreateHistograms();

  UInt_t iNbTofStations              = 0;
  UInt_t iNbStsStations              = 0;
  UInt_t iNbMuchStations             = 0;
  UInt_t iNbRichStations             = 0;
  const Double_t STATION_TOF_ZWIDTH  = 1.;
  const Double_t STATION_STS_ZWIDTH  = 0.5;
  const Double_t STATION_MUCH_ZWIDTH = 1.;
  const Double_t STATION_RICH_ZWIDTH = 3.;
  fvTofHitIndex.clear();
  fvStsHitIndex.clear();
  fvMuchHitIndex.clear();
  fvRichHitIndex.clear();

  LOG(info) << "Ev " << fiEvent << " has hits "
            << " Tof: " << tEvent->GetNofData(ECbmDataType::kTofHit)
            << " Sts: " << tEvent->GetNofData(ECbmDataType::kStsHit)
            << " Much: " << tEvent->GetNofData(ECbmDataType::kMuchPixelHit)
            << " Rich: " << tEvent->GetNofData(ECbmDataType::kRichHit);

  // cleanup
  /*
  if (NULL != fvAllHitPointer )
  for(UInt_t iSt=0; iSt<fvAllHitPointer.size(); iSt++) {
	for (UInt_t iHit=0; iHit<fvAllHitPointer[iSt].size(); iHit++)
	  delete(fvAllHitPointer[iSt][iHit]);
	fvAllHitPointer[iSt].clear();
  }
  */
  fvAllHitPointer.clear();
  UInt_t iNbAllStations = fMapStationZ.size();
  fvAllHitPointer.resize(iNbAllStations);

  if (tEvent->GetNofData(ECbmDataType::kTofHit) > 0) {
    // init to 1 tracking station
    iNbTofStations = 1;
    fvTofStationZ.resize(iNbTofStations);
    fvTofHitIndex.resize(iNbTofStations);
    for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kTofHit); iHit++) {
      Int_t iHitIndex = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofHit, iHit));
      CbmTofHit* tHit = dynamic_cast<CbmTofHit*>(fTofHitArrayIn->At(iHitIndex));
      LOG(debug) << Form("Inspect Ev %d, TofHit %d, Ind %d at %6.1f in %lu (%u) stations", fiEvent, iHit, iHitIndex,
                         tHit->GetZ(), fvTofStationZ.size(), iNbAllStations);

      Int_t iStZ    = (Int_t)(tHit->GetZ());
      itMapStationZ = fMapStationZ.find(iStZ);

      UInt_t iSt = 0;
      for (; iSt < fvTofStationZ.size(); iSt++) {
        if (iSt == 0 && fvTofHitIndex[iSt].size() == 1) {
          fvTofStationZ[iSt] = tHit->GetZ();
          fvTofHitIndex[iSt].resize(1);
          fvTofHitIndex[iSt][0] = iHitIndex;
          LOG(debug) << Form("Ev %d, init TofSt %d, Mul %lu at z %8.1f from Ind %d", fiEvent, iSt,
                             fvTofHitIndex[iSt].size(), fvTofStationZ[iSt], iHitIndex);
          break;
        }
        else {
          if (TMath::Abs(fvTofStationZ[iSt] - tHit->GetZ()) < STATION_TOF_ZWIDTH) {
            // update average z position of station
            fvTofStationZ[iSt] =
              (fvTofStationZ[iSt] * fvTofHitIndex[iSt].size() + tHit->GetZ()) / (fvTofHitIndex[iSt].size() + 1);
            fvTofHitIndex[iSt].resize(fvTofHitIndex[iSt].size() + 1);
            fvTofHitIndex[iSt][fvTofHitIndex[iSt].size() - 1] = iHitIndex;
            LOG(debug) << Form("Ev %d, upd  TofSt %d, Mul %lu at z %8.1f from Ind %d", fiEvent, iSt,
                               fvTofHitIndex[iSt].size(), fvTofStationZ[iSt], iHitIndex);
            break;
          }
        }
      }  // Station loop end

      if (iSt == iNbTofStations) {  // add new station
        iNbTofStations++;
        fvTofStationZ.resize(iNbTofStations);
        fvTofHitIndex.resize(iNbTofStations);
        fvTofStationZ[iSt] = tHit->GetZ();
        fvTofHitIndex[iSt].resize(1);
        fvTofHitIndex[iSt][0] = iHitIndex;
        LOG(debug) << Form("Ev %d, add  TofSt %d (%d), Mul %lu  at z %8.1f from Ind %d", fiEvent, iSt, iNbTofStations,
                           fvTofHitIndex[iSt].size(), fvTofStationZ[iSt], iHitIndex);
      }
      if (fiEvent < NDefSetup) {
        if (itMapStationZ == fMapStationZ.end()) {
          LOG(debug) << "Insert new tracking station " << Int_t(ECbmModuleId::kTof) * 100 + iSt << " at z=" << iStZ;
          fMapStationZ[iStZ] = Int_t(ECbmModuleId::kTof) * 100 + iSt;
          itMapStationZ      = fMapStationZ.begin();
          Int_t iStId        = Int_t(ECbmModuleId::kTof) * 100;
          for (; itMapStationZ != fMapStationZ.end(); ++itMapStationZ) {
            Int_t iSysId = itMapStationZ->second / 100;
            if (iSysId == Int_t(ECbmModuleId::kTof)) { itMapStationZ->second = iStId++; }
            LOG(info) << "MapStationZ: " << itMapStationZ->first << " " << itMapStationZ->second;
          }
        }
      }
      else {  // Define Setup end
        // sort hits into stations
        if (itMapStationZ != fMapStationZ.end()) {
          Int_t iAllSt = (itMapStationZ->second) % 100;
          /* TOF recalibration disabled
            pHit->SetX(pHit->GetX() + fvXoff[iAllSt]);
            pHit->SetY(pHit->GetY() + fvYoff[iAllSt]);
            pHit->SetZ(pHit->GetZ() + fvZoff[iAllSt]);
            pHit->SetTime(pHit->GetTime() + fvToff[iAllSt]);
            */
          //CbmTofHit* pHit = new CbmTofHit(*tHit); // copy construction
          // apply recalibration if necessary
          fvAllHitPointer[iAllSt].push_back(dynamic_cast<CbmPixelHit*>(tHit));
          //LOG(info)<<"TpHit: "<<pHit->ToString();
        }
      }
    }  // TofHit loop end
  }    // TofHit condition end


  if (tEvent->GetNofData(ECbmDataType::kStsHit) > 0) {
    // init to 1 tracking station
    iNbStsStations = 1;
    fvStsStationZ.resize(iNbStsStations);
    fvStsHitIndex.resize(iNbStsStations);

    for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kStsHit); iHit++) {
      Int_t iHitIndex   = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kStsHit, iHit));
      CbmPixelHit* tHit = dynamic_cast<CbmPixelHit*>(fStsHitArrayIn->At(iHitIndex));

      Int_t iStZ    = (Int_t)(tHit->GetZ());
      itMapStationZ = fMapStationZ.find(iStZ);

      UInt_t iSt = 0;
      for (; iSt < fvStsStationZ.size(); iSt++) {
        if (iSt == 0 && fvStsHitIndex[iSt].size() == 1) {
          fvStsStationZ[iSt] = tHit->GetZ();
          fvStsHitIndex[iSt].resize(1);
          fvStsHitIndex[iSt][0] = iHitIndex;
          LOG(debug) << Form("Ev %d, init StsSt %d at z %8.1f from Ind %d", fiEvent, iSt, fvStsStationZ[iSt],
                             iHitIndex);
          break;
        }
        else {
          if (TMath::Abs(fvStsStationZ[iSt] - tHit->GetZ()) < STATION_STS_ZWIDTH) {
            // update average z position of station
            fvStsStationZ[iSt] =
              (fvStsStationZ[iSt] * fvStsHitIndex[iSt].size() + tHit->GetZ()) / (fvStsHitIndex[iSt].size() + 1);
            fvStsHitIndex[iSt].resize(fvStsHitIndex[iSt].size() + 1);
            fvStsHitIndex[iSt][fvStsHitIndex[iSt].size() - 1] = iHitIndex;
            LOG(debug) << Form("Ev %d, upd  StsSt %d at z %8.1f from Ind %d", fiEvent, iSt, fvStsStationZ[iSt],
                               iHitIndex);
            break;
          }
        }
      }                             // Station loop end
      if (iSt == iNbStsStations) {  // add new station
        iNbStsStations++;
        fvStsStationZ.resize(iNbStsStations);
        fvStsHitIndex.resize(iNbStsStations);
        fvStsStationZ[iSt] = tHit->GetZ();
        fvStsHitIndex[iSt].resize(fvStsHitIndex[iSt].size() + 1);
        fvStsHitIndex[iSt][fvStsHitIndex[iSt].size() - 1] = iHitIndex;
        LOG(debug) << Form("Ev %d, add  StsSt %d (%d)  at z %8.1f from Ind %d", fiEvent, iSt, iNbStsStations,
                           fvStsStationZ[iSt], iHitIndex);
      }
      if (fiEvent < NDefSetup) {
        if (itMapStationZ == fMapStationZ.end()) {
          LOG(debug) << "Insert new tracking station " << Int_t(ECbmModuleId::kSts) * 100 + iSt << " at z=" << iStZ;
          fMapStationZ[iStZ] = Int_t(ECbmModuleId::kSts) * 100 + iSt;
          itMapStationZ      = fMapStationZ.begin();
          Int_t iStId        = Int_t(ECbmModuleId::kSts) * 100;
          for (; itMapStationZ != fMapStationZ.end(); ++itMapStationZ) {
            Int_t iSysId = itMapStationZ->second / 100;
            if (iSysId == Int_t(ECbmModuleId::kSts)) { itMapStationZ->second = iStId++; }
            LOG(info) << "MapStationZ: " << itMapStationZ->first << " " << itMapStationZ->second;
          }
        }
      }
      else {  // Define Setup end
        // sort hits into stations
        if (itMapStationZ != fMapStationZ.end()) {
          Int_t iAllSt = (itMapStationZ->second) % 100;
          UInt_t iH    = fvAllHitPointer[iAllSt].size();
          fvAllHitPointer[iAllSt].push_back(dynamic_cast<CbmPixelHit*>(tHit));
          CbmPixelHit* pHit = fvAllHitPointer[iAllSt][iH];
          pHit->SetX(pHit->GetX() + fvXoff[iAllSt]);
          pHit->SetY(pHit->GetY() + fvYoff[iAllSt]);
          pHit->SetZ(pHit->GetZ() + fvZoff[iAllSt]);
          pHit->SetTime(pHit->GetTime() + fvToff[iAllSt]);
        }
      }
    }  // Sts Hit loop end
  }    //Sts condition end

  if (tEvent->GetNofData(ECbmDataType::kMuchPixelHit) > 0) {
    // init to 1 tracking station
    iNbMuchStations = 1;
    fvMuchStationZ.resize(iNbMuchStations);
    fvMuchHitIndex.resize(iNbMuchStations);
    for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kMuchPixelHit); iHit++) {
      Int_t iHitIndex   = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kMuchPixelHit, iHit));
      CbmPixelHit* tHit = dynamic_cast<CbmPixelHit*>(fMuchHitArrayIn->At(iHitIndex));

      Int_t iStZ    = (Int_t)(tHit->GetZ());
      itMapStationZ = fMapStationZ.find(iStZ);

      UInt_t iSt = 0;
      for (; iSt < fvMuchStationZ.size(); iSt++) {
        if (iSt == 0 && fvMuchHitIndex[iSt].size() == 1) {
          fvMuchStationZ[iSt] = tHit->GetZ();
          fvMuchHitIndex[iSt].resize(1);
          fvMuchHitIndex[iSt][0] = iHitIndex;
          LOG(info) << Form("Ev %d, init MuchSt %d at z %8.1f from Ind %d", fiEvent, iSt, fvMuchStationZ[iSt],
                            iHitIndex);
          break;
        }
        else {
          if (TMath::Abs(fvMuchStationZ[iSt] - tHit->GetZ()) < STATION_MUCH_ZWIDTH) {
            // update average z position of station
            fvMuchStationZ[iSt] =
              (fvMuchStationZ[iSt] * fvMuchHitIndex[iSt].size() + tHit->GetZ()) / (fvMuchHitIndex[iSt].size() + 1);
            fvMuchHitIndex[iSt].resize(fvMuchHitIndex[iSt].size() + 1);
            fvMuchHitIndex[iSt][fvMuchHitIndex[iSt].size() - 1] = iHitIndex;
            LOG(debug) << Form("Ev %d, upd  MuchSt %d at z %8.1f from Ind %d", fiEvent, iSt, fvMuchStationZ[iSt],
                               iHitIndex);
            break;
          }
        }
      }                              // Station loop end
      if (iSt == iNbMuchStations) {  // add new station
        iNbMuchStations++;
        fvMuchStationZ.resize(iNbMuchStations);
        fvMuchHitIndex.resize(iNbMuchStations);
        fvMuchStationZ[iSt] = tHit->GetZ();
        fvMuchHitIndex[iSt].resize(fvMuchHitIndex[iSt].size() + 1);
        fvMuchHitIndex[iSt][fvMuchHitIndex[iSt].size() - 1] = iHitIndex;
        LOG(debug) << Form("Ev %d, add  MuchSt %d (%d)  at z %8.1f from Ind %d", fiEvent, iSt, iNbMuchStations,
                           fvMuchStationZ[iSt], iHitIndex);
      }

      if (fiEvent < NDefSetup) {
        if (itMapStationZ == fMapStationZ.end()) {
          LOG(debug) << "Insert new tracking station " << Int_t(ECbmModuleId::kMuch) * 100 + iSt << " at z=" << iStZ;
          fMapStationZ[iStZ] = Int_t(ECbmModuleId::kMuch) * 100 + iSt;
          itMapStationZ      = fMapStationZ.begin();
          Int_t iStId        = Int_t(ECbmModuleId::kMuch) * 100;
          for (; itMapStationZ != fMapStationZ.end(); ++itMapStationZ) {
            Int_t iSysId = itMapStationZ->second / 100;
            if (iSysId == Int_t(ECbmModuleId::kMuch)) { itMapStationZ->second = iStId++; }
            LOG(info) << "MapStationZ: " << itMapStationZ->first << " " << itMapStationZ->second;
          }
        }
      }
      else {  // Define Setup end
        // sort hits into stations
        if (itMapStationZ != fMapStationZ.end()) {
          Int_t iAllSt = (itMapStationZ->second) % 100;
          UInt_t iH    = fvAllHitPointer[iAllSt].size();
          fvAllHitPointer[iAllSt].push_back(dynamic_cast<CbmPixelHit*>(tHit));
          CbmPixelHit* pHit = fvAllHitPointer[iAllSt][iH];
          Double_t Xin      = pHit->GetX();
          pHit->SetX(pHit->GetX() + fvXoff[iAllSt]);
          Double_t Xout = pHit->GetX();
          // LOG(info) << "CalMuchX in "<<Xin<<", out "<<Xout<<", diff: "<<Xout-Xin;
          if (fvXoff[iAllSt] != 0.) assert(Xin != Xout);

          pHit->SetY(pHit->GetY() + fvYoff[iAllSt]);
          pHit->SetZ(pHit->GetZ() + fvZoff[iAllSt]);
          pHit->SetTime(pHit->GetTime() + fvToff[iAllSt]);
          //assign errors
          const TVector3 MuchHitError = {0.5, 0.5, 0.5};
          pHit->SetPositionError(MuchHitError);

          assert(iAllSt == fMapStationZ[(Int_t)(pHit->GetZ())] % 100);

          LOG(info) << Form("Proc ev %d, MuchIn St %d, H %d, MpHit: ", fiEvent, iAllSt, iH) << tHit->ToString();

          //delete(pHit);
        }
        else {
          LOG(warn) << "Undefined station for Much ";
        }
      }
    }  // MuchHit loop end
  }    // Much Hit condition end

  if (tEvent->GetNofData(ECbmDataType::kRichHit) > 0) {
    // init to 1 tracking station
    iNbRichStations = 1;
    fvRichStationZ.resize(iNbRichStations);
    fvRichHitIndex.resize(iNbRichStations);
    for (Int_t iHit = 0; iHit < tEvent->GetNofData(ECbmDataType::kRichHit); iHit++) {
      Int_t iHitIndex   = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kRichHit, iHit));
      CbmPixelHit* tHit = dynamic_cast<CbmPixelHit*>(fRichHitArrayIn->At(iHitIndex));

      Int_t iStZ    = (Int_t)(tHit->GetZ());
      itMapStationZ = fMapStationZ.find(iStZ);

      UInt_t iSt = 0;
      for (; iSt < fvRichStationZ.size(); iSt++) {
        if (iSt == 0 && fvRichHitIndex[iSt].size() == 1) {
          fvRichStationZ[iSt] = tHit->GetZ();
          fvRichHitIndex[iSt].resize(1);
          fvRichHitIndex[iSt][0] = iHitIndex;
          LOG(debug) << Form("Ev %d, init RichSt %d at z %8.1f from Ind %d", fiEvent, iSt, fvRichStationZ[iSt],
                             iHitIndex);
          break;
        }
        else {
          if (TMath::Abs(fvRichStationZ[iSt] - tHit->GetZ()) < STATION_RICH_ZWIDTH) {
            // update average z position of station
            fvRichStationZ[iSt] =
              (fvRichStationZ[iSt] * fvRichHitIndex[iSt].size() + tHit->GetZ()) / (fvRichHitIndex[iSt].size() + 1);
            fvRichHitIndex[iSt].resize(fvRichHitIndex[iSt].size() + 1);
            fvRichHitIndex[iSt][fvRichHitIndex[iSt].size() - 1] = iHitIndex;
            LOG(debug) << Form("Ev %d, upd  RichSt %d at z %8.1f from Ind %d", fiEvent, iSt, fvRichStationZ[iSt],
                               iHitIndex);
            break;
          }
        }
      }                              // Station loop end
      if (iSt == iNbRichStations) {  // add new station
        iNbRichStations++;
        fvRichStationZ.resize(iNbRichStations);
        fvRichHitIndex.resize(iNbRichStations);
        fvRichStationZ[iSt] = tHit->GetZ();
        fvRichHitIndex[iSt].resize(fvRichHitIndex[iSt].size() + 1);
        fvRichHitIndex[iSt][fvRichHitIndex[iSt].size() - 1] = iHitIndex;
        LOG(debug) << Form("Ev %d, add  RichSt %d (%d)  at z %8.1f from Ind %d", fiEvent, iSt, iNbRichStations,
                           fvRichStationZ[iSt], iHitIndex);
      }

      if (fiEvent < NDefSetup) {
        if (itMapStationZ == fMapStationZ.end()) {
          LOG(debug) << "Insert new tracking station " << Int_t(ECbmModuleId::kRich) * 100 + iSt << " at z=" << iStZ;
          fMapStationZ[iStZ] = Int_t(ECbmModuleId::kRich) * 100 + iSt;
          itMapStationZ      = fMapStationZ.begin();
          Int_t iStId        = Int_t(ECbmModuleId::kRich) * 100;
          for (; itMapStationZ != fMapStationZ.end(); ++itMapStationZ) {
            Int_t iSysId = itMapStationZ->second / 100;
            if (iSysId == Int_t(ECbmModuleId::kRich)) { itMapStationZ->second = iStId++; }
            LOG(info) << "MapStationZ: " << itMapStationZ->first << " " << itMapStationZ->second;
          }
        }
      }
      else {  // Define Setup end
        // sort hits into stations
        if (itMapStationZ != fMapStationZ.end()) {
          Int_t iAllSt = (itMapStationZ->second) % 100;
          //CbmPixelHit* pHit = new CbmPixelHit(*tHit); // copy construction
          // apply recalibration if necessary
          UInt_t iH = fvAllHitPointer[iAllSt].size();
          fvAllHitPointer[iAllSt].push_back(dynamic_cast<CbmPixelHit*>(tHit));
          CbmPixelHit* pHit = fvAllHitPointer[iAllSt][iH];
          pHit->SetX(pHit->GetX() + fvXoff[iAllSt]);
          pHit->SetY(pHit->GetY() + fvYoff[iAllSt]);
          pHit->SetZ(pHit->GetZ() + fvZoff[iAllSt]);
          pHit->SetTime(pHit->GetTime() + fvToff[iAllSt]);

          //LOG(info)<<"RpHit: "<<pHit->ToString();
          //delete(pHit);
        }
      }
    }  //RichHit loop end
  }    // Rich condition end
  /*
  LOG(info) << Form(
    "Ev %d, found %d Tof, %d Sts, %d Much, %d Rich - stations, %d tracks",
    fiEvent,
    iNbTofStations,
    iNbStsStations,
    iNbMuchStations,
    iNbRichStations,
    tEvent->GetNofData(ECbmDataType::kTofTrack));

  LOG(info) << "Station Multipliplicities:";
  for(UInt_t iSt=0; iSt<fvAllHitPointer.size();iSt++)
	  LOG(info)<<"  "<<iSt<<": "<<fvAllHitPointer[iSt].size();
  */

  // loop on tof tracklet
  Int_t iNTrks = tEvent->GetNofData(ECbmDataType::kTofTrack);
  if (iNTrks < 0) iNTrks = 0;
  fvTrkCalHits.clear();
  fvTrkCalHits.resize(iNTrks);
  fvTrkPar.clear();
  for (Int_t iTr = 0; iTr < iNTrks; iTr++) {
    Int_t iTrkIndex      = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofTrack, iTr));
    CbmTofTracklet* pTrk = dynamic_cast<CbmTofTracklet*>(fTofTrackArrayIn->At(iTrkIndex));
    //CbmTofTrackletParam* pTrkPar=new CbmTofTrackletParam(*pTrk->GetTrackParameter());
    //fvTrkPar.push_back(pTrkPar);
    fvTrkPar.push_back((CbmTofTrackletParam*) (pTrk->GetTrackParameter()));

    Int_t iNHits = pTrk->GetNofHits();
    fvTrkCalHits[iTr].resize(0);
    for (Int_t iHit = 0; iHit < iNHits; iHit++) {
      Int_t iHitInd     = pTrk->GetTofHitIndex(iHit);
      CbmTofHit* pHitIn = dynamic_cast<CbmTofHit*>(fTofHitArrayIn->At(iHitInd));
      //CbmTofHit* pHit   = new CbmTofHit(*pHitIn); // copy construction
      // apply recalibration if necessary
      fvTrkCalHits[iTr].push_back(dynamic_cast<CbmPixelHit*>(pHitIn));

      LOG(debug) << Form("Added PixHit %d, ind %d: x %6.3f, y %6.3f, z %6.3f, t %9.2f "
                         ", dx %6.3f, dy %6.3f,  dz %6.3f ",
                         iHit, pTrk->GetTofHitIndex(iHit), (fvTrkCalHits[iTr][iHit])->GetX(),
                         (fvTrkCalHits[iTr][iHit])->GetY(), (fvTrkCalHits[iTr][iHit])->GetZ(),
                         (fvTrkCalHits[iTr][iHit])->GetTime(), (fvTrkCalHits[iTr][iHit])->GetDx(),
                         (fvTrkCalHits[iTr][iHit])->GetDy(), (fvTrkCalHits[iTr][iHit])->GetDz());
    }
    Line3Dfit(fvTrkCalHits[iTr], fvTrkPar[iTr]);
    fvTrkPar[iTr]->SetTt(pTrk->GetTt());
    fvTrkPar[iTr]->SetT(pTrk->GetT0());

    // compare to input
    LOG(debug) << "CompareTrk " << iTr
               << Form(": DTx %10.8f, DTy %10.8f", pTrk->GetTrackTx() - fvTrkPar[iTr]->GetTx(),
                       pTrk->GetTrackTy() - fvTrkPar[iTr]->GetTy());
  }  // Tracklet loop end
  if (fiEvent > NDefSetup) {
    Int_t iAddStations = fiAddStations;
    while (iAddStations > 0) {
      Int_t iAddNextStation = iAddStations % 100;
      iAddStations /= 100;
      TrkAddStation(iAddNextStation);
    }

    //FindVertex();
    FillHistograms(tEvent);
    //assert(fiEvent<1);
  }
}
// -------------------------------------------------------------------------


// -----   Public method Finish   ------------------------------------------
void CbmTofExtendTracks::Finish()
{

  UpdateCalHistos();

  // save everything in output file

  FairRootManager* ioman = FairRootManager::Instance();
  TList* tList           = gROOT->GetList();

  ((FairRootFileSink*) ioman->GetSink())->GetRootFile()->cd();

  TIter next(tList);
  TObject* obj;
  while ((obj = (TObject*) next())) {
    if (obj->InheritsFrom(TH1::Class())) obj->Write();
  }

  WriteHistos();

  LOG(info) << Form(" CbmTofExtendTracks::Finished  ");
}
// -------------------------------------------------------------------------

void CbmTofExtendTracks::CreateHistograms()
{
  if (fiEvent < NDefSetup) return;
  TDirectory* oldir = gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT->cd();                     // <= To prevent histos from being sucked in by the param file of the TRootManager !

  // define histos here
  // Correlation with Tof Tracklets
  //
  fhMulCorTrkTof =
    new TH2F("hMulCorTrkTof", Form("Multiplicity correlation ;  N_{Track}; N_{TofHits}"), 50, 0, 50, 150, 0, 150);

  fhMulCorTrkSts =
    new TH2F("hMulCorTrkSts", Form("Multiplicity correlation ;  N_{Track}; N_{StsHits}"), 50, 0, 50, 150, 0, 150);
  fhMulCorTrkMuch =
    new TH2F("hMulCorTrkMuch", Form("Multiplicity correlation ;  N_{Track}; N_{MuchHits}"), 50, 0, 50, 150, 0, 150);
  fhMulCorTrkRich =
    new TH2F("hMulCorTrkRich", Form("Multiplicity correlation ;  N_{Track}; N_{RichHits}"), 50, 0, 50, 150, 0, 150);
  fhPosCorTrkTof  = new TH2F("hPosCorTrkTof", Form("Tof position correlation ;  #DeltaX (cm); #DeltaY (cm)"), 100, -10,
                            10, 100, -10, 10);
  fhPosCorTrkSts  = new TH2F("hPosCorTrkSts", Form("Sts position correlation ;  #DeltaX (cm); #DeltaY (cm)"), 100, -10,
                            10, 100, -10, 10);
  fhPosCorTrkMuch = new TH2F("hPosCorTrkMuch", Form("Much position correlation ;  #DeltaX (cm); #DeltaY (cm)"), 100,
                             -10, 10, 100, -10, 10);
  fhPosCorTrkRich = new TH2F("hPosCorTrkRich", Form("Rich position correlation ;  #DeltaX (cm); #DeltaY (cm)"), 100,
                             -10, 10, 100, -10, 10);


  // Correlation with extended tracks
  // Book Histos for all tracking stations found in fMap
  LOG(info) << "Book histos for the following stations: ";
  Int_t iStNum  = 0;
  itMapStationZ = fMapStationZ.begin();
  for (; itMapStationZ != fMapStationZ.end(); ++itMapStationZ) {
    Int_t iSysId          = itMapStationZ->second / 100;
    itMapStationZ->second = iSysId * 100 + iStNum++;
    LOG(info) << Form(" station %2d, z %3d, Id %4d ", iStNum, itMapStationZ->first, itMapStationZ->second);
  }
  UInt_t NSt = fMapStationZ.size();

  Int_t NLev = 2;
  //if (fiAddStations > 0 ) NLev=2;

  fhTrkStationDX.resize(NLev);
  fhTrkStationDY.resize(NLev);
  fhTrkStationDZ.resize(NLev);
  fhTrkStationDT.resize(NLev);
  fhTrkStationNHits.resize(NLev);
  fhTrkStationDXDY.resize(NLev);
  fhTrkPullDX.resize(NLev);
  fhTrkPullDY.resize(NLev);
  fhTrkPullDT.resize(NLev);
  fhExt_TrkSizVel.resize(NLev);
  fhExt_TrkSizChiSq.resize(NLev);

  for (Int_t iLev = 0; iLev < NLev; iLev++) {
    fhExt_TrkSizVel[iLev]   = new TH2F(Form("hExt_TrkSizVel%d", iLev), ";TrkSize;v (cm/ns)", 15, 0, 15, 100, 0., 50.);
    fhExt_TrkSizChiSq[iLev] = new TH2F(Form("hExt_TrkSizChiSq%d", iLev), ";TrkSize;#chi^2", 15, 0, 15, 50, 0., 5.);

    fhTrkStationDX[iLev] = new TH2F(Form("hTrkStationDX%d", iLev), Form("TrkStationDX;  StationNr ; #DeltaX (cm)"), NSt,
                                    0, NSt, 100, -10., 10.);
    fhTrkStationDY[iLev] = new TH2F(Form("hTrkStationDY%d", iLev), Form("TrkStationDY;  StationNr ; #DeltaY (cm)"), NSt,
                                    0, NSt, 100, -10., 10.);
    fhTrkStationDZ[iLev] = new TH2F(Form("hTrkStationDZ%d", iLev), Form("TrkStationDZ;  StationNr ; #DeltaZ (cm)"), NSt,
                                    0, NSt, 100, -20., 20.);
    fhTrkStationDT[iLev] = new TH2F(Form("hTrkStationDT%d", iLev), Form("TrkStationDT;  StationNr ; #DeltaT (ns)"), NSt,
                                    0, NSt, 100, -50., 50.);
    fhTrkStationNHits[iLev] =
      new TH2F(Form("hTrkStationNHits%d", iLev), Form("TrkStationNHits;  StationNr ; Number of Hits"), NSt, 0, NSt, 100,
               0., 100.);

    fhTrkPullDX[iLev] =
      new TH2F(Form("hTrkPullDX%d", iLev), Form("TrkPullDX;  StationNr ; #DeltaX (cm)"), NSt, 0, NSt, 100, -10., 10.);
    fhTrkPullDY[iLev] =
      new TH2F(Form("hTrkPullDY%d", iLev), Form("TrkPullDY;  StationNr ; #DeltaY (cm)"), NSt, 0, NSt, 100, -10., 10.);
    fhTrkPullDT[iLev] =
      new TH2F(Form("hTrkPullDT%d", iLev), Form("TrkPullDT;  StationNr ; #DeltaT (ns)"), NSt, 0, NSt, 100, -50., 50.);

    fhTrkStationDXDY[iLev].resize(NSt);
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      fhTrkStationDXDY[iLev][iSt] = new TH2F(Form("hTrkPosCor%d_St%d", iLev, iSt),
                                             Form("Lev%d-St%d position correlation ;  "
                                                  "#DeltaX (cm); #DeltaY (cm)",
                                                  iLev, iSt),
                                             100, -10, 10, 100, -10, 10);
    }
  }
  if (fiStationUT > -1) {
    // histos for Station Under Test
    itMapStationZ = fMapStationZ.begin();
    for (Int_t iSt = 0; iSt < fiStationUT; iSt++)
      itMapStationZ++;
    dSUT_z = itMapStationZ->first;
    LOG(info) << "Create SUT histos for station " << fiStationUT << " at distance " << dSUT_z;
    const Double_t dSUT_RefDx = 0.15;
    const Double_t dSUT_RefDy = 0.3;
    const Double_t dNbinX     = 100;
    const Double_t dNbinY     = 100;
    const Double_t dNbinZ     = 50;

    Double_t dSUT_dx = dSUT_RefDx * dSUT_z;
    Double_t dSUT_dy = dSUT_RefDy * dSUT_z;
    fhExtSutXY_Found = new TH2F("hExtSutXY_Found", Form("StationUnderTest %d found hits ; X (cm); Y (cm)", fiStationUT),
                                dNbinX, -dSUT_dx, dSUT_dx, dNbinY, -dSUT_dy, dSUT_dy);
    fhExtSutXY_Missed =
      new TH2F("hExtSutXY_Missed", Form("StationUnderTest %d missed hits ; X (cm); Y (cm)", fiStationUT), dNbinX,
               -dSUT_dx, dSUT_dx, dNbinY, -dSUT_dy, dSUT_dy);
    fhExtSutXY_DX =
      new TH3F("hExtSutXY_DX", Form("StationUnderTest %d #DeltaX ; X (cm); Y (cm); #DeltaX (cm)", fiStationUT), dNbinX,
               -dSUT_dx, dSUT_dx, dNbinY, -dSUT_dy, dSUT_dy, dNbinZ, -10., 10.);
    fhExtSutXY_DY =
      new TH3F("hExtSutXY_DY", Form("StationUnderTest %d #DeltaY ; X (cm); Y (cm); #DeltaY (cm)", fiStationUT), dNbinX,
               -dSUT_dx, dSUT_dx, dNbinY, -dSUT_dy, dSUT_dy, dNbinZ, -10., 10.);
    fhExtSutXY_DT =
      new TH3F("hExtSutXY_DT", Form("StationUnderTest %d #DeltaT ; X (cm); Y (cm); #DeltaT (ns)", fiStationUT), dNbinX,
               -dSUT_dx, dSUT_dx, dNbinY, -dSUT_dy, dSUT_dy, dNbinZ, -50., 50.);
  }  // StationUT end

  gDirectory->cd(oldir->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!

  LoadCalParameter();
}

void CbmTofExtendTracks::FindVertex()
{
  fVTX_T            = 0.;  //reset
  fVTX_X            = 0.;
  fVTX_Y            = 0.;
  fVTX_Z            = 0.;
  fVTXNorm          = 0.;
  Int_t fMinNofHits = 3;

  for (Int_t iTrk = 0; iTrk < fTofTrackArrayIn->GetEntriesFast(); iTrk++) {
    CbmTofTracklet* pTrk = (CbmTofTracklet*) fTofTrackArrayIn->At(iTrk);
    if (NULL == pTrk) continue;
    Double_t w = pTrk->GetNofHits();

    if (w > (Double_t) fMinNofHits) {  // for further analysis request minimum number of hits
      fVTXNorm += w;
      fVTX_T += w * pTrk->GetFitT(0.);
      fVTX_X += w * pTrk->GetFitX(0.);
      fVTX_Y += w * pTrk->GetFitY(0.);
    }
  }
  if (fVTXNorm > 0.) {
    fVTX_T /= fVTXNorm;
    fVTX_X /= fVTXNorm;
    fVTX_Y /= fVTXNorm;
    fVTX_Z /= fVTXNorm;
  }
  LOG(debug) << Form("CbmTofExtendTracks::FindVertex: N %3.0f, T %6.2f, "
                     "X=%6.2f, Y=%6.2f Z=%6.2f ",
                     fVTXNorm, fVTX_T, fVTX_X, fVTX_Y, fVTX_Z);
}

void CbmTofExtendTracks::FillHistograms(CbmEvent* tEvent)
{
  // event selection: limit number of hits per added station
  Int_t iAddStations = fiAddStations;
  while (iAddStations > 0) {
    Int_t iAddNextStation = iAddStations % 100;
    iAddStations /= 100;
    if ((Int_t) fvAllHitPointer[iAddNextStation].size() > fiCutStationMaxHitMul) return;
  }

  // limit track multiplicity
  Int_t NTrkTof = tEvent->GetNofData(ECbmDataType::kTofTrack);
  if (NTrkTof > fiNTrkTofMax) return;

  Int_t iLev = 0;
  //
  fhMulCorTrkTof->Fill(tEvent->GetNofData(ECbmDataType::kTofTrack), tEvent->GetNofData(ECbmDataType::kTofHit));

  fhMulCorTrkSts->Fill(tEvent->GetNofData(ECbmDataType::kTofTrack), tEvent->GetNofData(ECbmDataType::kStsHit));

  fhMulCorTrkMuch->Fill(tEvent->GetNofData(ECbmDataType::kTofTrack), tEvent->GetNofData(ECbmDataType::kMuchPixelHit));

  fhMulCorTrkRich->Fill(tEvent->GetNofData(ECbmDataType::kTofTrack), tEvent->GetNofData(ECbmDataType::kRichHit));


  // correlation with TOF Tracklets
  for (Int_t iTr = 0; iTr < NTrkTof; iTr++) {
    Int_t iTrkIndex      = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofTrack, iTr));
    CbmTofTracklet* tTrk = dynamic_cast<CbmTofTracklet*>(fTofTrackArrayIn->At(iTrkIndex));

    // tTrk->PrintInfo();

    if (tTrk->GetNofHits() < fiTrkHitsMin) continue;

    // correlation with all TofHits
    for (UInt_t iSt = 0; iSt < fvTofHitIndex.size(); iSt++) {
      for (UInt_t iHit = 0; iHit < fvTofHitIndex[iSt].size(); iHit++) {
        CbmTofHit* tHit = dynamic_cast<CbmTofHit*>(fTofHitArrayIn->At(fvTofHitIndex[iSt][iHit]));
        if (NULL == tHit) {
          LOG(warn) << Form("Invalid TofHit %d(%lu) ind %d in station %d(%lu)", iHit, fvTofHitIndex[iSt].size(),
                            fvTofHitIndex[iSt][iHit], iSt, fvTofHitIndex.size());
          assert(tHit);
        }
        Int_t iTrSt = fMapStationZ[(Int_t)(tHit->GetZ())] % 100;
        if (iTrSt < 0 || iTrSt >= (Int_t) fMapStationZ.size()) {
          LOG(error) << "Invalid station index " << iTrSt;
          continue;
        }

        Double_t dDX = tHit->GetX() + fvXoff[iTrSt] - tTrk->GetFitX(tHit->GetZ());
        if (TMath::Abs(dDX) > fdTrkCutDX) continue;
        Double_t dDY = tHit->GetY() + fvYoff[iTrSt] - tTrk->GetFitY(tHit->GetZ());
        if (TMath::Abs(dDY) > fdTrkCutDY) continue;
        Double_t dDT = tHit->GetTime() + fvToff[iTrSt] - tTrk->GetFitT(tHit->GetZ());
        if (TMath::Abs(dDT) > fdTrkCutDT) continue;

        fhPosCorTrkTof->Fill(dDX, dDY);
        // fill tracking station histos
        fhTrkStationDX[iLev]->Fill(iTrSt, dDX);
        fhTrkStationDY[iLev]->Fill(iTrSt, dDY);
        fhTrkStationDT[iLev]->Fill(iTrSt, dDT);
        fhTrkStationNHits[iLev]->Fill(iTrSt, (Double_t) fvTofHitIndex[iSt].size());
      }
    }  // Tof Station loop end

    // correlation with all StsHits
    for (UInt_t iSt = 0; iSt < fvStsHitIndex.size(); iSt++) {
      for (UInt_t iHit = 0; iHit < fvStsHitIndex[iSt].size(); iHit++) {
        CbmPixelHit* tHit = dynamic_cast<CbmPixelHit*>(fStsHitArrayIn->At(fvStsHitIndex[iSt][iHit]));
        if (NULL == tHit) {
          LOG(warn) << Form("Invalid StsHit %d(%lu) ind %d in station %d(%lu)", iHit, fvStsHitIndex[iSt].size(),
                            fvStsHitIndex[iSt][iHit], iSt, fvStsHitIndex.size());
          assert(tHit);
        }
        Int_t iTrSt = fMapStationZ[(Int_t)(tHit->GetZ())] % 100;

        Double_t dDX = tHit->GetX() + fvXoff[iTrSt] - tTrk->GetFitX(tHit->GetZ());
        if (TMath::Abs(dDX) > fdTrkCutDX) continue;
        Double_t dDY = tHit->GetY() + fvYoff[iTrSt] - tTrk->GetFitY(tHit->GetZ());
        if (TMath::Abs(dDY) > fdTrkCutDY) continue;
        Double_t dDT = tHit->GetTime() + fvToff[iTrSt] - tTrk->GetFitT(tHit->GetZ());
        if (TMath::Abs(dDT) > fdTrkCutDT) continue;

        fhPosCorTrkSts->Fill(dDX, dDY);
        // fill tracking station histos
        fhTrkStationDX[iLev]->Fill(iTrSt, dDX);
        fhTrkStationDY[iLev]->Fill(iTrSt, dDY);
        fhTrkStationDT[iLev]->Fill(iTrSt, dDT);
        fhTrkStationNHits[iLev]->Fill(iTrSt, (Double_t) fvStsHitIndex[iSt].size());
      }
    }  // Sts Station loop end

    // correlation with all MuchHits
    for (UInt_t iSt = 0; iSt < fvMuchHitIndex.size(); iSt++) {
      for (UInt_t iHit = 0; iHit < fvMuchHitIndex[iSt].size(); iHit++) {
        CbmPixelHit* tHit = dynamic_cast<CbmPixelHit*>(fMuchHitArrayIn->At(fvMuchHitIndex[iSt][iHit]));
        if (NULL == tHit) {
          LOG(warn) << Form("Invalid MuchHit %d(%lu) ind %d in station %d(%lu)", iHit, fvMuchHitIndex[iSt].size(),
                            fvMuchHitIndex[iSt][iHit], iSt, fvMuchHitIndex.size());
          assert(tHit);
        }
        Int_t iTrSt = fMapStationZ[(Int_t)(tHit->GetZ())] % 100;

        LOG(info) << Form("Proc ev %d, Trk %d, St %d, iLe%d MtHit: ", fiEvent, iTr, iTrSt, iLev) << tHit->ToString();

        Double_t dDX = tHit->GetX() + fvXoff[iTrSt] - tTrk->GetFitX(tHit->GetZ());
        if (TMath::Abs(dDX) > fdTrkCutDX) continue;
        Double_t dDY = tHit->GetY() + fvYoff[iTrSt] - tTrk->GetFitY(tHit->GetZ());
        if (TMath::Abs(dDY) > fdTrkCutDY) continue;
        Double_t dDT = tHit->GetTime() + fvToff[iTrSt] - tTrk->GetFitT(tHit->GetZ());
        if (TMath::Abs(dDT) > fdTrkCutDT) continue;
        /*
        LOG(info)<<"Got MuchCorI Tr "<<iTr
        		<<", St "<<iTrSt
        		<<", Hit "<< iHit <<", ind "<<fvMuchHitIndex[iSt][iHit]
				<<", poi "<< tHit
				<<", dx " << dDX
				<<", dy " << dDY;
        */
        LOG(info) << Form("Proc ev %d, Trk %d, St %d, iLe%d, DX %6.3f, dY %6.3f, MtHit: ", fiEvent, iTr, iTrSt, iLev,
                          dDX, dDY)
                  << tHit->ToString();

        fhPosCorTrkMuch->Fill(dDX, dDY);
        // fill tracking station histos
        fhTrkStationDX[iLev]->Fill(iTrSt, dDX);
        fhTrkStationDY[iLev]->Fill(iTrSt, dDY);
        fhTrkStationDT[iLev]->Fill(iTrSt, dDT);
        fhTrkStationNHits[iLev]->Fill(iTrSt, (Double_t) fvMuchHitIndex[iSt].size());
      }
    }  // Much Station loop end

    // correlation with all RichHits
    for (UInt_t iSt = 0; iSt < fvRichHitIndex.size(); iSt++) {
      for (UInt_t iHit = 0; iHit < fvRichHitIndex[iSt].size(); iHit++) {
        CbmPixelHit* tHit = dynamic_cast<CbmPixelHit*>(fRichHitArrayIn->At(fvRichHitIndex[iSt][iHit]));
        if (NULL == tHit) {
          LOG(warn) << Form("Invalid RichHit %d(%lu) ind %d in station %d(%lu)", iHit, fvRichHitIndex[iSt].size(),
                            fvRichHitIndex[iSt][iHit], iSt, fvRichHitIndex.size());
          assert(tHit);
        }
        Int_t iTrSt = fMapStationZ[(Int_t)(tHit->GetZ())] % 100;

        Double_t dDX = tHit->GetX() + fvXoff[iTrSt] - tTrk->GetFitX(tHit->GetZ());
        if (TMath::Abs(dDX) > fdTrkCutDX) continue;
        Double_t dDY = tHit->GetY() + fvYoff[iTrSt] - tTrk->GetFitY(tHit->GetZ());
        if (TMath::Abs(dDY) > fdTrkCutDY) continue;
        Double_t dDT = tHit->GetTime() + fvToff[iTrSt] - tTrk->GetFitT(tHit->GetZ());
        if (TMath::Abs(dDT) > fdTrkCutDT) continue;

        fhPosCorTrkRich->Fill(dDX, dDY);
        // fill tracking station histos
        fhTrkStationDX[iLev]->Fill(iTrSt, dDX);
        fhTrkStationDY[iLev]->Fill(iTrSt, dDY);
        fhTrkStationDT[iLev]->Fill(iTrSt, dDT);
        fhTrkStationNHits[iLev]->Fill(iTrSt, (Double_t) fvRichHitIndex[iSt].size());
      }
    }  // Rich Station loop end

  }  // TOF Tracklet loop end

  iLev = 1;
  // Fully assembled tracks
  UInt_t NSt  = fMapStationZ.size();
  UInt_t NTrk = fvTrkPar.size();
  if (NTrkTof < 0) NTrkTof = 0;
  assert(NTrk == (UInt_t) NTrkTof);

  for (UInt_t iTr = 0; iTr < NTrk; iTr++) {
    //select tracks
    // minimal number of TOF hits
    Int_t NTofHits = 0;
    for (UInt_t iH = 0; iH < fvTrkCalHits[iTr].size(); iH++) {
      if (fMapStationZ[(Int_t)(fvTrkCalHits[iTr][iH]->GetZ())] / 100 == Int_t(ECbmModuleId::kTof)) NTofHits++;
    }
    if (NTofHits < fiTrkHitsMin) continue;
    // contain requested stations
    if (fiReqStations > -1) {
      Int_t iReqStations = fiReqStations;
      Int_t iHit         = (Int_t) fvTrkCalHits[iTr].size() - 1;
      Int_t iReqStation  = 0;
      while (iReqStations > 0) {
        iReqStation = iReqStations % 100;
        iHit        = (Int_t) fvTrkCalHits[iTr].size() - 1;
        //	    LOG(info)<<"Check tr "<<iTr<<" for station "<<iReqStation
        //	    		 <<" starting from hit "<<iHit<<" in station "
        //				 <<fMapStationZ[(Int_t)(fvTrkCalHits[iTr][iHit]->GetZ())]%100;
        iReqStations /= 100;
        for (; iHit > 0; iHit--)
          if (iReqStation == fMapStationZ[(Int_t)(fvTrkCalHits[iTr][iHit]->GetZ())] % 100) {
            //			  if (iReqStations ==0 )
            //				LOG(info)<<"Check tr "<<iTr<<" accepted for analysis";
            break;
          }
        //	    LOG(info)<<"Check last Hit "<<iHit<<", ReqSt "<<iReqStations
        //	             <<"("<<fiReqStations<<") ";
        if (iReqStation != 0 && iHit == 0) break;  // station not found
        //	    LOG(info)<<"CheckI with "<<iReqStations<<","<<iHit;
      }
      //	  LOG(info)<<"CheckII with "<<iReqStations<<","<<iHit;
      if (iReqStation != 0 && iHit == 0) continue;  //skip this track
    }

    LOG(info) << "CheckIII accepted tr " << iTr << " with " << fiReqStations;
    for (UInt_t iHit = 0; iHit < fvTrkCalHits[iTr].size(); iHit++) {
      LOG(info) << "  Hit " << iHit << ", station " << fMapStationZ[(Int_t)(fvTrkCalHits[iTr][iHit]->GetZ())] % 100;
    }

    fhExt_TrkSizChiSq[iLev]->Fill((Double_t) fvTrkCalHits[iTr].size(), fvTrkPar[iTr]->GetChiSq());

    fhExt_TrkSizVel[iLev]->Fill((Double_t) fvTrkCalHits[iTr].size(), 1. / (fvTrkPar[iTr]->GetTt()));

    // Pulls
    CbmTofTrackletParam* tTrkPar = fvTrkPar[iTr];
    Bool_t bSUT_Found            = kFALSE;
    for (UInt_t iHit = 0; iHit < fvTrkCalHits[iTr].size(); iHit++) {
      CbmPixelHit* tHit = fvTrkCalHits[iTr][iHit];
      Int_t iTrSt       = fMapStationZ[(Int_t)(tHit->GetZ())] % 100;

      Double_t dDX = tHit->GetX() - GetFitX(tHit->GetZ(), tTrkPar);
      Double_t dDY = tHit->GetY() - GetFitY(tHit->GetZ(), tTrkPar);
      Double_t dDT = tHit->GetTime() - GetFitT(tHit->GetZ(), tTrkPar);

      if (TMath::Abs(dDX) > fdTrkCutDX) continue;
      if (TMath::Abs(dDY) > fdTrkCutDY) continue;
      if (TMath::Abs(dDT) > fdTrkCutDT) continue;

      fhTrkPullDX[iLev]->Fill(iTrSt, dDX);
      fhTrkPullDY[iLev]->Fill(iTrSt, dDY);
      fhTrkPullDT[iLev]->Fill(iTrSt, dDT);

      if (iTrSt == fiStationUT) {  // hit in SUT
        bSUT_Found = kTRUE;
        fhExtSutXY_Found->Fill(tHit->GetX(), tHit->GetY());
        fhExtSutXY_DX->Fill(tHit->GetX(), tHit->GetY(), dDX);
        fhExtSutXY_DY->Fill(tHit->GetX(), tHit->GetY(), dDY);
        fhExtSutXY_DT->Fill(tHit->GetX(), tHit->GetY(), dDT);
      }

      // pulls against original tracklets
      Int_t iTrkInd        = static_cast<Int_t>(tEvent->GetIndex(ECbmDataType::kTofTrack, iTr));
      CbmTofTracklet* pTrk = dynamic_cast<CbmTofTracklet*>(fTofTrackArrayIn->At(iTrkInd));
      dDX                  = tHit->GetX() - pTrk->GetFitX(tHit->GetZ());
      dDY                  = tHit->GetY() - pTrk->GetFitY(tHit->GetZ());
      dDT                  = tHit->GetTime() - pTrk->GetFitT(tHit->GetZ());
      fhTrkPullDX[0]->Fill(iTrSt, dDX);
      fhTrkPullDY[0]->Fill(iTrSt, dDY);
      fhTrkPullDT[0]->Fill(iTrSt, dDT);
      fhTrkStationDXDY[0][iTrSt]->Fill(dDX, dDY);
      LOG(info) << Form("Proc ev %d, Trk %d, St %d, Lev%d, DX %6.3f, dY %6.3f, MtHit: ", fiEvent, iTr, iTrSt, 0, dDX,
                        dDY)
                << tHit->ToString();
    }  // fvTrkCalHits loop end

    if (bSUT_Found == kFALSE) { fhExtSutXY_Missed->Fill(GetFitX(dSUT_z, tTrkPar), GetFitY(dSUT_z, tTrkPar)); }

    //Deviations to all hits in all stations
    for (UInt_t iSt = 0; iSt < NSt; iSt++) {
      for (UInt_t iHit = 0; iHit < fvAllHitPointer[iSt].size(); iHit++) {
        CbmPixelHit* tHit = fvAllHitPointer[iSt][iHit];
        /*
	    LOG(info)<<"Got AllHit for Tr "<<iTr
			                <<", St "<<iSt
							<<", StZ "<<fMapStationZ[(Int_t)(tHit->GetZ())]%100
	                		<<", Hit "<< iHit <<", poi "<< tHit;
	    LOG(info)<<"tHit: "<<tHit->ToString();
	    */
        assert((Int_t) iSt == fMapStationZ[(Int_t)(tHit->GetZ())] % 100);

        Double_t dDX = tHit->GetX() - GetFitX(tHit->GetZ(), tTrkPar);
        if (TMath::Abs(dDX) > fdTrkCutDX) continue;
        Double_t dDY = tHit->GetY() - GetFitY(tHit->GetZ(), tTrkPar);
        if (TMath::Abs(dDY) > fdTrkCutDY) continue;
        Double_t dDT = tHit->GetTime() - GetFitT(tHit->GetZ(), tTrkPar);
        if (TMath::Abs(dDT) > fdTrkCutDT) continue;

        LOG(info) << Form("Proc ev %d, Trk %d, St %d, Lev%d, DX %6.3f, dY %6.3f, MtHit: ", fiEvent, iTr, iSt, iLev, dDX,
                          dDY)
                  << tHit->ToString();

        // fill tracking station histos
        fhTrkStationDX[iLev]->Fill(iSt, dDX);
        fhTrkStationDY[iLev]->Fill(iSt, dDY);
        fhTrkStationDT[iLev]->Fill(iSt, dDT);
        fhTrkStationNHits[iLev]->Fill(iSt, (Double_t) fvAllHitPointer[iSt].size());
        fhTrkStationDXDY[iLev][iSt]->Fill(dDX, dDY);
      }
    }
  }  // loop on extended tracks end

}  // FillHist end

void CbmTofExtendTracks::Line3Dfit(std::vector<CbmPixelHit*> pHits, CbmTofTrackletParam* pTrkPar)
{
  TGraph2DErrors* gr = new TGraph2DErrors();
  // Fill the 2D graph
  // generate graph with the 3d points
  for (UInt_t N = 0; N < pHits.size(); N++) {
    double x, y, z = 0;
    x = pHits[N]->GetX();
    y = pHits[N]->GetY();
    z = pHits[N]->GetZ();
    gr->SetPoint(N, x, y, z);
    double dx, dy, dz = 0.;
    dx = pHits[N]->GetDx();
    dy = pHits[N]->GetDy();
    dz = pHits[N]->GetDz();  //FIXME
    gr->SetPointError(N, dx, dy, dz);
    LOG(debug) << "Line3Dfit add N = " << N << ",\t" << x << ",\t" << y << ",\t" << z << ",\t" << dx << ",\t" << dy
               << ",\t" << dz;
  }
  // fit the graph now

  Double_t pStart[4] = {0., 0., 0., 0.};
  pStart[0]          = pTrkPar->GetX();
  pStart[1]          = pTrkPar->GetTx();
  pStart[2]          = pTrkPar->GetY();
  pStart[3]          = pTrkPar->GetTy();
  LOG(debug) << "Line3Dfit init: X0 " << pStart[0] << ", TX " << pStart[1] << ", Y0 " << pStart[2] << ", TY "
             << pStart[3];

  fMinuit.DoFit(gr, pStart);
  //gr->Draw("err p0");
  gr->Delete();
  Double_t* dRes;
  dRes = fMinuit.GetParFit();
  LOG(debug) << "Line3Dfit result(" << pHits.size() << ") " << gMinuit->fCstatu << " : X0 "
             << Form(" %7.4f, TX %7.4f, Y0 %7.4f, TY %7.4f, Chi2DoF %7.4f ", dRes[0], dRes[1], dRes[2], dRes[3],
                     fMinuit.GetChi2DoF());

  pTrkPar->SetX(dRes[0]);
  pTrkPar->SetY(dRes[2]);
  pTrkPar->SetZ(0.);
  pTrkPar->SetTx(dRes[1]);
  pTrkPar->SetTy(dRes[3]);
  pTrkPar->SetQp(1.);
  pTrkPar->SetChiSq(fMinuit.GetChi2DoF());  // / (Double_t)pHits.size());
}

Double_t CbmTofExtendTracks::GetFitX(Double_t dZ, CbmTofTrackletParam* fTrkPar)
{
  return fTrkPar->GetX() + fTrkPar->GetTx() * (dZ - fTrkPar->GetZ());
}

Double_t CbmTofExtendTracks::GetFitY(Double_t dZ, CbmTofTrackletParam* fTrkPar)
{
  return fTrkPar->GetY() + fTrkPar->GetTy() * (dZ - fTrkPar->GetZ());
}

Double_t CbmTofExtendTracks::GetFitT(Double_t dZ, CbmTofTrackletParam* fTrkPar)
{
  return fTrkPar->GetT()
         + fTrkPar->GetTt() * (dZ - fTrkPar->GetZ())
             * TMath::Sqrt(1. + fTrkPar->GetTx() * fTrkPar->GetTx() + fTrkPar->GetTy() * fTrkPar->GetTy());
}

void CbmTofExtendTracks::TrkAddStation(Int_t iStation)
{

  LOG(debug) << "Add " << fvAllHitPointer[iStation].size() << " hits from station " << iStation << " to "
             << fvTrkPar.size() << " tracks ";

  const Int_t NCand = 100;
  // tbd: get matching width from histos !!
  Double_t tSIGX               = 1.;
  Double_t tSIGY               = 1.;
  Double_t tSIGT               = 1000.;
  UInt_t NTr                   = fvTrkPar.size();
  Double_t MatchChi2Min[NCand] = {NCand * fdChi2Max};
  Int_t MatchHit[NCand]        = {NCand * -1};
  Int_t MatchTrk[NCand]        = {NCand * -1};
  // find list of best matches for each track, max NCand
  Int_t iMatch = 0;
  for (UInt_t iTr = 0; iTr < NTr; iTr++) {
    for (UInt_t iHit = 0; iHit < fvAllHitPointer[iStation].size(); iHit++) {
      CbmPixelHit* tHit = fvAllHitPointer[iStation][iHit];
      LOG(debug) << " Hit " << iHit << ": X " << tHit->GetX() << ", Y " << tHit->GetY() << ", Z " << tHit->GetZ()
                 << ", T " << tHit->GetTime();
      Double_t MatchChi2 =
        TMath::Power((tHit->GetX() + fvXoff[iStation] - GetFitX(tHit->GetZ(), fvTrkPar[iTr])) / tSIGX, 2)
        + TMath::Power((tHit->GetY() + fvYoff[iStation] - GetFitY(tHit->GetZ(), fvTrkPar[iTr])) / tSIGY, 2);
      TMath::Power((tHit->GetTime() + fvToff[iStation] - GetFitT(tHit->GetZ(), fvTrkPar[iTr])) / tSIGT, 2);
      MatchChi2 /= 3.;
      LOG(debug) << "Match Tr " << iTr << ", Hit " << iHit << ": " << MatchChi2;
      if (MatchChi2 < fdChi2Max) {
        Int_t iCand = 0;
        for (; iCand < iMatch + 1; iCand++) {
          if (MatchChi2 < MatchChi2Min[iCand]) {
            LOG(debug) << "Better Match found for iTr " << iTr << " Chi2 " << MatchChi2;
            for (Int_t i = iMatch - 1; i >= iCand; i--) {
              MatchChi2Min[i + 1] = MatchChi2Min[i];
              MatchHit[i + 1]     = MatchHit[i];
              MatchTrk[i + 1]     = MatchTrk[i];
            }
            MatchChi2Min[iCand] = MatchChi2;
            MatchHit[iCand]     = (Int_t) iHit;
            MatchTrk[iCand]     = (Int_t) iTr;
            break;
          }
        }  // end candidate loop
        iMatch++;
        if (iMatch == NCand) iMatch--;  // cutoff
        LOG(debug) << "New Match " << iMatch << " stored as candidate " << iCand << " for Tr " << iTr << " with hit "
                   << iHit;
      }
    }  // station hit loop end
  }    // track loop end

  // append best matches to track
  for (Int_t iCand = 0; iCand < iMatch; iCand++) {
    if (MatchHit[iCand] > -1) {
      CbmPixelHit* tHit = fvAllHitPointer[iStation][MatchHit[iCand]];
      /*
	  LOG(info)<<"Add hit "<<MatchHit[iCand]
			   <<", station "<<fMapStationZ[(Int_t)(tHit->GetZ())]%100
			   <<" to track "<<MatchTrk[iCand]
			   <<" with chi2 "<<MatchChi2Min[iCand];
	  */
      //LOG(info)<<"MatHit: "<<tHit->ToString();

      fvTrkCalHits[MatchTrk[iCand]].push_back(dynamic_cast<CbmPixelHit*>(tHit));
      // prevent match with other tracks
      for (Int_t i = iCand + 1; i < iMatch; i++) {
        if (MatchHit[i] == MatchHit[iCand] || MatchTrk[i] == MatchTrk[iCand]) MatchHit[i] = -1;
      }
      // refit track, update TrkPar
      fvTrkPar[MatchTrk[iCand]]->SetX(0.);
      fvTrkPar[MatchTrk[iCand]]->SetY(0.);

      Line3Dfit(fvTrkCalHits[MatchTrk[iCand]], fvTrkPar[MatchTrk[iCand]]);
      // Inspect pulls (debugging)
      /*
	  Double_t dDX=tHit->GetX() - GetFitX(tHit->GetZ(),fvTrkPar[MatchTrk[iCand]]);
	  Double_t dDY=tHit->GetY() - GetFitY(tHit->GetZ(),fvTrkPar[MatchTrk[iCand]]);
	  LOG(info)<<"MatchPulls X "<<dDX<<", Y "<<dDY;
	  */
    }
  }  // end of loop on match candidates
}
