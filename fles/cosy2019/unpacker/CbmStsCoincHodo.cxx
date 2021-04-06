/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmStsCoincHodo.h"

#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THttpServer.h"

#include <iomanip>
using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmStsCoincHodo::CbmStsCoincHodo() : FairTask("CbmStsCoincHodo"), arrayClusters {nullptr}, arrayHits {nullptr} {}

// ---- Destructor ----------------------------------------------------
CbmStsCoincHodo::~CbmStsCoincHodo() {}


// ---- Init ----------------------------------------------------------
InitStatus CbmStsCoincHodo::Init()
{

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get a pointer to the previous already existing data level
  arrayClusters = static_cast<TClonesArray*>(ioman->GetObject("StsCluster"));
  arrayHits     = static_cast<TClonesArray*>(ioman->GetObject("StsHit"));

  if (!arrayClusters) { LOG(info) << "No TClonesArray with STS clusters found."; }
  if (!arrayHits) { LOG(info) << "No TClonesArray with STS hits found."; }

  CreateHistos();

  return kSUCCESS;
}

void CbmStsCoincHodo::CreateHistos()
{
  phHitsStsTime   = new TH1F("phHitsStsTime", "phHitsStsTime", 1000, 0, 1000);
  phHitsHodoATime = new TH1F("phHitsHodoATime", "phHitsHodoATime", 1000, 0, 1000);
  phHitsHodoBTime = new TH1F("phHitsHodoBTime", "phHitsHodoBTime", 1000, 0, 1000);

  ///---------------------------------------------------------------------///

  phHitsPositionHodoA = new TH2F("phHitsPositionHodoA", "Position of the hits in hodoscope A; X [cm]; Y [cm]", 80, -4.0,
                                 4.0, 80, -4.0, 4.0);
  phHitsPositionSts =
    new TH2F("phHitsPositionSts", "Position of the hits in hodoscope B; X [cm]; Y [cm]", 80, -4.0, 4.0, 80, -4.0, 4.0);
  phHitsPositionHodoB = new TH2F("phHitsPositionHodoB", "Position of the hits in hodoscope B; X [cm]; Y [cm]", 80, -4.0,
                                 4.0, 80, -4.0, 4.0);
  ///---------------------------------------------------------------------///

  phNbHitsCompHodo =
    new TH2F("phNbHitsCompHodo", "Number of hits per TS in Hodo A vs Hodo B; Nb Hits A[]; Nb Hits B []", 100, 0.0,
             20000.0, 100, 0.0, 20000.0);
  phNbHitsCompStsHodoA =
    new TH2F("phNbHitsCompStsHodoA", "Number of hits per TS in STS vs Hodo A; Nb Hits STS[]; Nb Hits A []", 100, 0.0,
             20000.0, 100, 0.0, 20000.0);
  phNbHitsCompStsHodoB =
    new TH2F("phNbHitsCompStsHodoB", "Number of hits per TS in STS vs Hodo B; Nb Hits STS[]; Nb Hits B []", 100, 0.0,
             20000.0, 100, 0.0, 20000.0);
  ///---------------------------------------------------------------------///

  phHitsCoincCorrXX = new TH2F("phHitsCoincCorrXX", "XX correlation of the coincident hits; X_A [cm]; X_B [cm]", 160,
                               -8.0, 8.0, 160, -8.0, 8.0);
  phHitsCoincCorrYY = new TH2F("phHitsCoincCorrYY", "YY correlation of the coincident hits; Y_A [cm]; Y_B [cm]", 160,
                               -8.0, 8.0, 160, -8.0, 8.0);
  phHitsCoincCorrXY = new TH2F("phHitsCoincCorrXY", "XY correlation of the coincident hits; X_A [cm]; Y_B [cm]", 160,
                               -8.0, 8.0, 160, -8.0, 8.0);
  phHitsCoincCorrYX = new TH2F("phHitsCoincCorrYX", "YX correlation of the coincident hits; Y_A [cm]; X_B [cm]", 160,
                               -8.0, 8.0, 160, -8.0, 8.0);

  phHitsPositionCoincA =
    new TH2F("phHitsPositionCoincA", "Position of the coincident hits in hodoscope A; X [cm]; Y [cm]", 80, -4.0, 4.0,
             80, -4.0, 4.0);
  phHitsPositionCoincB =
    new TH2F("phHitsPositionCoincB", "Position of the coincident hits in hodoscope B; X [cm]; Y [cm]", 80, -4.0, 4.0,
             80, -4.0, 4.0);
  phHitsPositionDiff =
    new TH2F("phHitsPositionDiff", "Position difference of the coincident hits; X_B - X_A [cm]; Y_B- Y_A [cm]", 160,
             -8.0, 8.0, 160, -8.0, 8.0);
  phHitsTimeDiff = new TH1F("phHitsTimeDiff", "Time difference of the coincident hits; t_B - t_A [ns]",
                            2 * iCoincLimitClk, -dCoincLimit, dCoincLimit);

  phHitsCoincDist = new TH1F("phHitsCoincDist", "XY distance of the coincident hits; Dist. [cm]", 100, 0.0, 10.0);
  phHitsCoincAngle =
    new TH1F("phHitsCoincAngle", "Vertical angle of the coincident hits; Angle [deg.]", 180, -90.0, 90.0);
  ///---------------------------------------------------------------------///

  phHitsSingleCoincCorrXX =
    new TH2F("phHitsSingleCoincCorrXX", "XX correlation of the coincident hits; X_A [cm]; X_B [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);
  phHitsSingleCoincCorrYY =
    new TH2F("phHitsSingleCoincCorrYY", "YY correlation of the coincident hits; Y_A [cm]; Y_B [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);
  phHitsSingleCoincCorrXY =
    new TH2F("phHitsSingleCoincCorrXY", "XY correlation of the coincident hits; X_A [cm]; Y_B [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);
  phHitsSingleCoincCorrYX =
    new TH2F("phHitsSingleCoincCorrYX", "YX correlation of the coincident hits; Y_A [cm]; X_B [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);

  phHitsSinglePositionCoincA =
    new TH2F("phHitsSinglePositionCoincA", "Position of the coincident hits in hodoscope A; X [cm]; Y [cm]", 80, -4.0,
             4.0, 80, -4.0, 4.0);
  phHitsSinglePositionCoincB =
    new TH2F("phHitsSinglePositionCoincB", "Position of the coincident hits in hodoscope B; X [cm]; Y [cm]", 80, -4.0,
             4.0, 80, -4.0, 4.0);
  phHitsSinglePositionDiff =
    new TH2F("phHitsSinglePositionDiff", "Position difference of the coincident hits; X_B - X_A [cm]; Y_B- Y_A [cm]",
             160, -8.0, 8.0, 160, -8.0, 8.0);
  phHitsSingleTimeDiff = new TH1F("phHitsSingleTimeDiff", "Time difference of the coincident hits; t_B - t_A [ns]",
                                  2 * iCoincLimitClk, -dCoincLimit, dCoincLimit);

  phHitsSingleCoincDist =
    new TH1F("phHitsSingleCoincDist", "XY distance of the coincident hits; Dist. [cm]", 100, 0.0, 10.0);
  phHitsSingleCoincAngle =
    new TH1F("phHitsSingleCoincAngle", "Vertical angle of the coincident hits; Angle [deg.]", 180, -90.0, 90.0);
  ///---------------------------------------------------------------------///

  phHitsBestCoincCorrXX = new TH2F("phHitsBestCoincCorrXX", "XX correlation of the coincident hits; X_A [cm]; X_B [cm]",
                                   160, -8.0, 8.0, 160, -8.0, 8.0);
  phHitsBestCoincCorrYY = new TH2F("phHitsBestCoincCorrYY", "YY correlation of the coincident hits; Y_A [cm]; Y_B [cm]",
                                   160, -8.0, 8.0, 160, -8.0, 8.0);
  phHitsBestCoincCorrXY = new TH2F("phHitsBestCoincCorrXY", "XY correlation of the coincident hits; X_A [cm]; Y_B [cm]",
                                   160, -8.0, 8.0, 160, -8.0, 8.0);
  phHitsBestCoincCorrYX = new TH2F("phHitsBestCoincCorrYX", "YX correlation of the coincident hits; Y_A [cm]; X_B [cm]",
                                   160, -8.0, 8.0, 160, -8.0, 8.0);

  phHitsBestPositionCoincA =
    new TH2F("phHitsBestPositionCoincA", "Position of the coincident hits in hodoscope A; X [cm]; Y [cm]", 80, -4.0,
             4.0, 80, -4.0, 4.0);
  phHitsBestPositionCoincB =
    new TH2F("phHitsBestPositionCoincB", "Position of the coincident hits in hodoscope B; X [cm]; Y [cm]", 80, -4.0,
             4.0, 80, -4.0, 4.0);
  phHitsBestPositionDiff =
    new TH2F("phHitsBestPositionDiff", "Position difference of the coincident hits; X_B - X_A [cm]; Y_B- Y_A [cm]", 160,
             -8.0, 8.0, 160, -8.0, 8.0);
  phHitsBestTimeDiff = new TH1F("phHitsBestTimeDiff", "Time difference of the coincident hits; t_B - t_A [ns]",
                                2 * iCoincLimitClk, -dCoincLimit, dCoincLimit);

  phHitsBestCoincDist =
    new TH1F("phHitsBestCoincDist", "XY distance of the coincident hits; Dist. [cm]", 100, 0.0, 10.0);
  phHitsBestCoincAngle =
    new TH1F("phHitsBestCoincAngle", "Vertical angle of the coincident hits; Angle [deg.]", 180, -90.0, 90.0);

  ///---------------------------------------------------------------------///
  phHitsPositionCoincExtr = new TH2F("phHitsPositionCoincExtr", "Position of the extrapolated hits ; X [cm]; Y [cm]",
                                     80, -4.0, 4.0, 80, -4.0, 4.0);
  ///---------------------------------------------------------------------///

  phHitsStsCoincCorrXX =
    new TH2F("phHitsStsCoincCorrXX", "XX correlation of the coincident hits; X_extr [cm]; X_STS [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);
  phHitsStsCoincCorrYY =
    new TH2F("phHitsStsCoincCorrYY", "YY correlation of the coincident hits; Y_extr [cm]; Y_STS [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);
  phHitsStsCoincCorrXY =
    new TH2F("phHitsStsCoincCorrXY", "XY correlation of the coincident hits; X_extr [cm]; Y_STS [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);
  phHitsStsCoincCorrYX =
    new TH2F("phHitsStsCoincCorrYX", "YX correlation of the coincident hits; Y_extr [cm]; X_STS [cm]", 160, -8.0, 8.0,
             160, -8.0, 8.0);

  phHitsStsPositionCoincExtr = new TH2F(
    "phHitsStsPositionCoincExtr", "Position of the extrapolated hits ; X [cm]; Y [cm]", 80, -4.0, 4.0, 80, -4.0, 4.0);
  phHitsStsPositionCoinc = new TH2F("phHitsStsPositionCoinc", "Position of the coincident hits in Sts; X [cm]; Y [cm]",
                                    80, -4.0, 4.0, 80, -4.0, 4.0);
  phHitsStsTimeDiff =
    new TH1F("phHitsStsTimeDiff", "Position difference of STS hit with the best coincident hits; t_Sts - t_(AB) [ns]",
             2 * iCoincLimitClk, -dCoincLimit, dCoincLimit);
  phHitsStsPositionDiff = new TH2F(
    "phHitsStsPositionDiff", "Position difference of the coincident hits; X_STS - X_extr [cm]; Y_STS- Y_extr [cm]", 160,
    -8.0, 8.0, 160, -8.0, 8.0);
  phHitsStsPositionDiffInv = new TH2F(
    "phHitsStsPositionDiffInv", "Position difference of the coincident hits; X_STS - Y_extr [cm]; Y_STS- Y_extr [cm]",
    160, -8.0, 8.0, 160, -8.0, 8.0);

  ///---------------------------------------------------------------------///


  phHitsStsBestCoincCorrXX =
    new TH2F("phHitsStsBestCoincCorrXX", "XX correlation of the coincident hits; X_extr [cm]; X_STS [cm]", 160, -8.0,
             8.0, 160, -8.0, 8.0);
  phHitsStsBestCoincCorrYY =
    new TH2F("phHitsStsBestCoincCorrYY", "YY correlation of the coincident hits; Y_extr [cm]; Y_STS [cm]", 160, -8.0,
             8.0, 160, -8.0, 8.0);
  phHitsStsBestCoincCorrXY =
    new TH2F("phHitsStsBestCoincCorrXY", "XY correlation of the coincident hits; X_extr [cm]; Y_STS [cm]", 160, -8.0,
             8.0, 160, -8.0, 8.0);
  phHitsStsBestCoincCorrYX =
    new TH2F("phHitsStsBestCoincCorrYX", "YX correlation of the coincident hits; Y_extr [cm]; X_STS [cm]", 160, -8.0,
             8.0, 160, -8.0, 8.0);

  phHitsStsBestPositionCoincExtr =
    new TH2F("phHitsStsBestPositionCoincExtr", "Position of the extrapolated hits ; X [cm]; Y [cm]", 80, -4.0, 4.0, 80,
             -4.0, 4.0);
  phHitsStsBestPositionCoinc =
    new TH2F("phHitsStsBestPositionCoinc", "Position of the coincident hits in Sts; X [cm]; Y [cm]", 80, -4.0, 4.0, 80,
             -4.0, 4.0);
  phHitsStsBestPositionShiftCoinc =
    new TH2F("phHitsStsBestPositionShiftCoinc", "Position of the coincident hits in Sts; X [cm]; Y [cm]", 80, -4.0, 4.0,
             80, -4.0, 4.0);

  phHitsStsBestTimeDiff     = new TH1F("phHitsStsBestTimeDiff",
                                   "Position difference of STS hit with the best coincident hits; t_Sts - t_(AB) [ns]",
                                   2 * iCoincLimitClk, -dCoincLimit, dCoincLimit);
  phHitsStsBestPositionDiff = new TH2F(
    "phHitsStsBestPositionDiff",
    "Position difference of STS hit with the best coincident hits; X_Sts - X_extr(AB) [cm]; Y_Sts- Y_extr(AB) [cm]",
    400, -8.0, 8.0, 400, -8.0, 8.0);
  phHitsStsBestPositionDiffInv = new TH2F("phHitsStsBestPositionDiffInv",
                                          "Position difference of STS hit (inverted) with the best coincident hits; "
                                          "X_Sts - Y_extr(AB) [cm]; Y_Sts- X_extr(AB) [cm]",
                                          400, -8.0, 8.0, 400, -8.0, 8.0);
  phHitsStsBestDiff            = new TH3F("phHitsStsBestDiff",
                               "Difference of STS hit with the best coincident hits; X_Sts - X_extr(AB) [cm]; Y_Sts- "
                               "Y_extr(AB) [cm]; t_Sts - t_mean(AB) [ns]",
                               160, -8.0, 8.0, 160, -8.0, 8.0, 2 * iCoincLimitClk, -dCoincLimit, dCoincLimit);

  ///---------------------------------------------------------------------///
  phHitsStsEff =
    new TH2F("phHitsStsEff", "Position of the coincident hits in Sts; X [cm]; Y [cm]", 80, -4.0, 4.0, 80, -4.0, 4.0);


  /// Register the histos in the HTTP server
  FairRunOnline* run = FairRunOnline::Instance();
  if (run) {
    THttpServer* server = run->GetHttpServer();
    if (nullptr != server) {

      server->Register("CheckCoinc", phHitsStsTime);
      server->Register("CheckCoinc", phHitsHodoATime);
      server->Register("CheckCoinc", phHitsHodoBTime);

      server->Register("CheckCoinc", phHitsPositionHodoA);
      server->Register("CheckCoinc", phHitsPositionSts);
      server->Register("CheckCoinc", phHitsPositionHodoB);

      server->Register("CheckCoinc", phNbHitsCompHodo);
      server->Register("CheckCoinc", phNbHitsCompStsHodoA);
      server->Register("CheckCoinc", phNbHitsCompStsHodoB);

      server->Register("CheckCoinc", phHitsCoincCorrXX);
      server->Register("CheckCoinc", phHitsCoincCorrYY);
      server->Register("CheckCoinc", phHitsCoincCorrXY);
      server->Register("CheckCoinc", phHitsCoincCorrYX);
      server->Register("CheckCoinc", phHitsPositionCoincA);
      server->Register("CheckCoinc", phHitsPositionCoincB);
      server->Register("CheckCoinc", phHitsPositionDiff);
      server->Register("CheckCoinc", phHitsTimeDiff);
      server->Register("CheckCoinc", phHitsCoincDist);
      server->Register("CheckCoinc", phHitsCoincAngle);

      server->Register("CheckCoinc", phHitsSingleCoincCorrXX);
      server->Register("CheckCoinc", phHitsSingleCoincCorrYY);
      server->Register("CheckCoinc", phHitsSingleCoincCorrXY);
      server->Register("CheckCoinc", phHitsSingleCoincCorrYX);
      server->Register("CheckCoinc", phHitsSinglePositionCoincA);
      server->Register("CheckCoinc", phHitsSinglePositionCoincB);
      server->Register("CheckCoinc", phHitsSinglePositionDiff);
      server->Register("CheckCoinc", phHitsSingleTimeDiff);
      server->Register("CheckCoinc", phHitsSingleCoincDist);
      server->Register("CheckCoinc", phHitsSingleCoincAngle);

      server->Register("CheckCoinc", phHitsBestCoincCorrXX);
      server->Register("CheckCoinc", phHitsBestCoincCorrYY);
      server->Register("CheckCoinc", phHitsBestCoincCorrXY);
      server->Register("CheckCoinc", phHitsBestCoincCorrYX);
      server->Register("CheckCoinc", phHitsBestPositionCoincA);
      server->Register("CheckCoinc", phHitsBestPositionCoincB);
      server->Register("CheckCoinc", phHitsBestPositionDiff);
      server->Register("CheckCoinc", phHitsBestTimeDiff);
      server->Register("CheckCoinc", phHitsBestCoincDist);
      server->Register("CheckCoinc", phHitsBestCoincAngle);

      server->Register("CheckCoinc", phHitsPositionCoincExtr);

      server->Register("CheckCoinc", phHitsStsCoincCorrXX);
      server->Register("CheckCoinc", phHitsStsCoincCorrYY);
      server->Register("CheckCoinc", phHitsStsCoincCorrXY);
      server->Register("CheckCoinc", phHitsStsCoincCorrYX);
      server->Register("CheckCoinc", phHitsStsPositionCoincExtr);
      server->Register("CheckCoinc", phHitsStsPositionCoinc);
      server->Register("CheckCoinc", phHitsStsTimeDiff);
      server->Register("CheckCoinc", phHitsStsPositionDiff);
      server->Register("CheckCoinc", phHitsStsPositionDiffInv);

      server->Register("CheckCoinc", phHitsStsBestCoincCorrXX);
      server->Register("CheckCoinc", phHitsStsBestCoincCorrYY);
      server->Register("CheckCoinc", phHitsStsBestCoincCorrXY);
      server->Register("CheckCoinc", phHitsStsBestCoincCorrYX);
      server->Register("CheckCoinc", phHitsStsBestPositionCoincExtr);
      server->Register("CheckCoinc", phHitsStsBestPositionCoinc);
      server->Register("CheckCoinc", phHitsStsBestPositionShiftCoinc);
      server->Register("CheckCoinc", phHitsStsBestTimeDiff);
      server->Register("CheckCoinc", phHitsStsBestPositionDiff);
      server->Register("CheckCoinc", phHitsStsBestPositionDiffInv);
      server->Register("CheckCoinc", phHitsStsBestDiff);

      server->Register("CheckCoinc", phHitsStsEff);
    }
  }
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmStsCoincHodo::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmStsCoincHodo::Exec(Option_t* /*option*/)
{

  fNbTs++;

  Double_t dOffsetX    = 0.0;
  Double_t dOffsetY    = 0.0;
  Double_t dOffsetT    = -6.38091e+00;
  Double_t dOffsetXSts = -2.55;
  Double_t dOffsetYSts = -1.46;
  Double_t dOffsetTSts = -5.22608e+00;
  Double_t dSigmaXSts  = 6.79000e-02;
  Double_t dSigmaYSts  = 2.81408e-02;

  Int_t iNbClusters = arrayClusters->GetEntriesFast();
  Int_t iNbHits     = arrayHits->GetEntriesFast();
  std::cout << "executing TS " << fNbTs << " StsClusters: " << iNbClusters << " StsHits: " << iNbHits << std::endl;

  // Fill vectors for Sts and HodoA,B
  std::vector<CbmStsHit*> vHitsHodoA;
  std::vector<CbmStsHit*> vHitsSts;
  std::vector<CbmStsHit*> vHitsHodoB;

  vHitsHodoA.clear();
  vHitsSts.clear();
  vHitsHodoB.clear();
  for (Int_t iHit = 0; iHit < iNbHits; ++iHit) {
    CbmStsHit* pHit = dynamic_cast<CbmStsHit*>(arrayHits->UncheckedAt(iHit));
    Double_t dX     = pHit->GetX();
    Double_t dY     = pHit->GetY();
    Double_t dZ     = pHit->GetZ();
    //std::cout << "TS: " << fNbTs << " " << pHit->GetAddress() << std::endl;
    /// Check if the hit is in Hodo A or B or in STS
    //         if( dZ < dMidStsHodoA )
    if (pHit->GetAddress() == 0x10008002) {
      //std::cout << "TS: " << fNbTs << " HODOA" << std::endl;
      vHitsHodoA.push_back(pHit);
      phHitsPositionHodoA->Fill(dX, dY);
    }  // if( dZ < dMidStsHodoA ) => if Hodo A
    //      else if( dZ < dMidStsHodoB )
    else if (pHit->GetAddress() == 0x10008012) {
      //std::cout << "TS: " << fNbTs << " STS" << std::endl;
      vHitsSts.push_back(pHit);
      phHitsPositionSts->Fill(dX, dY);
    }  // else if( dZ < dMidStsHodoB ) of if( dZ < dMidStsHodoA ) => if STS
    //else
    else if (pHit->GetAddress() == 0x10008022) {
      //std::cout << "TS: " << fNbTs << " HODOB" << std::endl;
      vHitsHodoB.push_back(pHit);
      phHitsPositionHodoB->Fill(dX, dY);
    }  // else of if( dZ < dMidStsHodoB ) => if Hodo B
  }    // for( Int_t iHit = 0; iHit < iNbHits; ++iHit)
  std::cout << "TS: " << fNbTs << " " << vHitsHodoA.size() << " " << vHitsHodoB.size() << " " << vHitsSts.size()
            << std::endl;

  phNbHitsCompHodo->Fill(vHitsHodoA.size(), vHitsHodoB.size());
  phNbHitsCompStsHodoA->Fill(vHitsSts.size(), vHitsHodoA.size());
  phNbHitsCompStsHodoB->Fill(vHitsSts.size(), vHitsHodoB.size());

  phHitsStsTime->Fill(fNbTs, vHitsSts.size());
  phHitsHodoATime->Fill(fNbTs, vHitsHodoA.size());
  phHitsHodoBTime->Fill(fNbTs, vHitsHodoB.size());

  //-----------------------------------------------------------------------


  // Look for coincidence HODOA - HODOB
  for (UInt_t uHitA = 0; uHitA < vHitsHodoA.size(); ++uHitA) {
    Double_t dBestTime = 1e9;
    UInt_t uBestB      = vHitsHodoB.size();

    Double_t dTimeA = vHitsHodoA[uHitA]->GetTime();
    for (UInt_t uHitB = 0; uHitB < vHitsHodoB.size(); ++uHitB) {
      Double_t dTimeB = vHitsHodoB[uHitB]->GetTime();

      // time difference of the hits in HODOB - HODOA
      phHitsTimeDiff->Fill(dTimeB - dTimeA);

      if (TMath::Abs(dTimeB - dTimeA) < dCoincLimit) {
        // HODOA vs HODOB
        phHitsCoincCorrXX->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoB[uHitB]->GetX());
        phHitsCoincCorrYY->Fill(vHitsHodoA[uHitA]->GetY(), vHitsHodoB[uHitB]->GetY());
        phHitsCoincCorrXY->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoB[uHitB]->GetY());
        phHitsCoincCorrYX->Fill(vHitsHodoA[uHitA]->GetY(), vHitsHodoB[uHitB]->GetX());
        // position X vs Y of the hits in coincidence HODOA and HODOB
        phHitsPositionCoincA->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoA[uHitA]->GetY());
        phHitsPositionCoincB->Fill(vHitsHodoB[uHitB]->GetX(), vHitsHodoB[uHitB]->GetY());
        // position difference DeltaX vs DeltaY of the hits in coincidence HODOB - HODOA
        phHitsPositionDiff->Fill(vHitsHodoB[uHitB]->GetX() - vHitsHodoA[uHitA]->GetX() - dOffsetX,
                                 vHitsHodoB[uHitB]->GetY() - vHitsHodoA[uHitA]->GetY() - dOffsetY);

        Double_t dHitsDistXY = TMath::Sqrt((vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uHitB]->GetX() - dOffsetX)
                                             * (vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uHitB]->GetX() - dOffsetX)
                                           + (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uHitB]->GetY() - dOffsetY)
                                               * (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uHitB]->GetY() - dOffsetY));
        Double_t dHitsDistZ  = TMath::Abs(vHitsHodoA[uHitA]->GetZ() - vHitsHodoB[uHitB]->GetZ());
        Double_t dAngle      = TMath::RadToDeg() * TMath::ATan2(dHitsDistXY, dHitsDistZ);
        phHitsCoincDist->Fill(dHitsDistXY);
        phHitsCoincAngle->Fill(dAngle);

        if (TMath::Abs(dTimeB - dTimeA - dOffsetT) < dBestTime) {
          dBestTime = TMath::Abs(dTimeB - dTimeA);
          uBestB    = uHitB;
        }  // if( TMath::Abs( dTimeB - dTimeA )  < dBestTime )
      }    // if( TMath::Abs( dTimeB - dTimeA ) < dCoincLimit )
    }      // for( UInt_t uHitB = 0; uHitB < vHitsHodoB.size(); ++uHitB )
    if (uBestB < vHitsHodoB.size()) {
      Double_t dTimeB = vHitsHodoB[uBestB]->GetTime();

      phHitsBestTimeDiff->Fill(dTimeB - dTimeA);

      if (TMath::Abs(dTimeB - dTimeA) < dCoincLimit) {
        phHitsBestCoincCorrXX->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoB[uBestB]->GetX());
        phHitsBestCoincCorrYY->Fill(vHitsHodoA[uHitA]->GetY(), vHitsHodoB[uBestB]->GetY());
        phHitsBestCoincCorrXY->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoB[uBestB]->GetY());
        phHitsBestCoincCorrYX->Fill(vHitsHodoA[uHitA]->GetY(), vHitsHodoB[uBestB]->GetX());

        phHitsBestPositionCoincA->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoA[uHitA]->GetY());
        phHitsBestPositionCoincB->Fill(vHitsHodoB[uBestB]->GetX(), vHitsHodoB[uBestB]->GetY());
        phHitsBestPositionDiff->Fill(vHitsHodoB[uBestB]->GetX() - vHitsHodoA[uHitA]->GetX() - dOffsetX,
                                     vHitsHodoB[uBestB]->GetY() - vHitsHodoA[uHitA]->GetY() - dOffsetY);


        Double_t dHitsDistXY = TMath::Sqrt((vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uBestB]->GetX() - dOffsetX)
                                             * (vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uBestB]->GetX() - dOffsetX)
                                           + (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uBestB]->GetY() - dOffsetY)
                                               * (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uBestB]->GetY() - dOffsetY));
        Double_t dHitsDistZ  = TMath::Abs(vHitsHodoA[uHitA]->GetZ() - vHitsHodoB[uBestB]->GetZ());
        Double_t dAngle      = TMath::RadToDeg() * TMath::ATan2(dHitsDistXY, dHitsDistZ);
        phHitsBestCoincDist->Fill(dHitsDistXY);
        phHitsBestCoincAngle->Fill(dAngle);

        // only one hit per TS (never happens in data!)
        if (1 == vHitsHodoA.size() && 1 == vHitsHodoB.size()) {
          phHitsSingleCoincCorrXX->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoB[uBestB]->GetX());
          phHitsSingleCoincCorrYY->Fill(vHitsHodoA[uHitA]->GetY(), vHitsHodoB[uBestB]->GetY());
          phHitsSingleCoincCorrXY->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoB[uBestB]->GetY());
          phHitsSingleCoincCorrYX->Fill(vHitsHodoA[uHitA]->GetY(), vHitsHodoB[uBestB]->GetX());

          phHitsSinglePositionCoincA->Fill(vHitsHodoA[uHitA]->GetX(), vHitsHodoA[uHitA]->GetY());
          phHitsSinglePositionCoincB->Fill(vHitsHodoB[uBestB]->GetX(), vHitsHodoB[uBestB]->GetY());
          phHitsSinglePositionDiff->Fill(vHitsHodoB[uBestB]->GetX() - vHitsHodoA[uHitA]->GetX() - dOffsetX,
                                         vHitsHodoB[uBestB]->GetY() - vHitsHodoA[uHitA]->GetY() - dOffsetY);
          phHitsSingleTimeDiff->Fill(dTimeB - dTimeA);
          phHitsSingleCoincDist->Fill(dHitsDistXY);
          phHitsSingleCoincAngle->Fill(dAngle);
        }  // if( 1 == vHitsHodoA.size() && 1 == vHitsHodoB.size() )

      }  // if( TMath::Abs( dTimeB - dTimeA ) < dCoincLimit )


      /// now search coincidence in Sts
      Double_t dBestTimeSts = 1e9;
      UInt_t uBestSts       = vHitsSts.size();

      Double_t dHodoMeanTime = (dTimeA + dTimeB) / 2.0;
      Double_t dHodoExtrX =
        vHitsHodoA[uHitA]->GetX() + (vHitsHodoB[uBestB]->GetX() - vHitsHodoA[uHitA]->GetX()) * dStsDistZ / dHodoDistZ;
      Double_t dHodoExtrY =
        vHitsHodoA[uHitA]->GetY() + (vHitsHodoB[uBestB]->GetY() - vHitsHodoA[uHitA]->GetY()) * dStsDistZ / dHodoDistZ;

      phHitsPositionCoincExtr->Fill(dHodoExtrX, dHodoExtrY);

      for (UInt_t uHitSts = 0; uHitSts < vHitsSts.size(); ++uHitSts) {
        Double_t dTimeSts     = vHitsSts[uHitSts]->GetTime();
        Double_t dTimeDiffSts = dTimeSts - dHodoMeanTime;

        phHitsStsTimeDiff->Fill(dTimeDiffSts);

        if (TMath::Abs(dTimeDiffSts) < dCoincLimit) {

          phHitsStsCoincCorrXX->Fill(dHodoExtrX, vHitsSts[uHitSts]->GetX());
          phHitsStsCoincCorrYY->Fill(dHodoExtrY, vHitsSts[uHitSts]->GetY());
          phHitsStsCoincCorrXY->Fill(dHodoExtrX, vHitsSts[uHitSts]->GetY());
          phHitsStsCoincCorrYX->Fill(dHodoExtrY, vHitsSts[uHitSts]->GetX());

          phHitsStsPositionCoincExtr->Fill(dHodoExtrX, dHodoExtrY);
          phHitsStsPositionCoinc->Fill(vHitsSts[uHitSts]->GetX(), vHitsSts[uHitSts]->GetY());

          phHitsStsPositionDiff->Fill(vHitsSts[uHitSts]->GetX() - dHodoExtrX - dOffsetX,
                                      vHitsSts[uHitSts]->GetY() - dHodoExtrY - dOffsetY);
          phHitsStsPositionDiffInv->Fill(vHitsSts[uHitSts]->GetX() - dHodoExtrY - dOffsetX,
                                         vHitsSts[uHitSts]->GetY() - dHodoExtrX - dOffsetY);


          if (TMath::Abs(dTimeDiffSts - dOffsetTSts) < dBestTimeSts) {
            dBestTimeSts = TMath::Abs(dTimeDiffSts);
            uBestSts     = uHitSts;
          }  // if( TMath::Abs( dTimeB - dTimeA )  < dBestTimeSts )
          // else std::cout << fNbTs << " " << vHitsHodoA.size() << " " << vHitsSts.size() << " " << vHitsHodoB.size()
          //                << " " << TMath::Abs( dTimeDiffSts ) << " " << dBestTimeSts
          //                << std::endl;
        }  // if( TMath::Abs( dTimeSts - dHodoMeanTime ) < dCoincLimit )
      }    // for( UInt_t uHitSts = 0; uHitSts < vHitsSts.size(); ++uHitSts )

      if (uBestSts < vHitsSts.size()) {
        phHitsStsBestCoincCorrXX->Fill(dHodoExtrX, vHitsSts[uBestSts]->GetX());
        phHitsStsBestCoincCorrYY->Fill(dHodoExtrY, vHitsSts[uBestSts]->GetY());
        phHitsStsBestCoincCorrXY->Fill(dHodoExtrX, vHitsSts[uBestSts]->GetY());
        phHitsStsBestCoincCorrYX->Fill(dHodoExtrY, vHitsSts[uBestSts]->GetX());

        phHitsStsBestPositionCoincExtr->Fill(dHodoExtrX, dHodoExtrY);
        phHitsStsBestPositionCoinc->Fill(vHitsSts[uBestSts]->GetX(), vHitsSts[uBestSts]->GetY());
        phHitsStsBestPositionShiftCoinc->Fill(vHitsSts[uBestSts]->GetX() - dOffsetXSts,
                                              vHitsSts[uBestSts]->GetY() - dOffsetYSts);

        phHitsStsBestTimeDiff->Fill(vHitsSts[uBestSts]->GetTime() - dHodoMeanTime);
        phHitsStsBestPositionDiff->Fill(vHitsSts[uBestSts]->GetX() - dHodoExtrX,
                                        vHitsSts[uBestSts]->GetY() - dHodoExtrY);
        phHitsStsBestPositionDiffInv->Fill(vHitsSts[uBestSts]->GetX() - dHodoExtrY,
                                           vHitsSts[uBestSts]->GetY() - dHodoExtrX);
        phHitsStsBestDiff->Fill(vHitsSts[uBestSts]->GetX() - dHodoExtrX, vHitsSts[uBestSts]->GetY() - dHodoExtrY,
                                vHitsSts[uBestSts]->GetTime() - dHodoMeanTime);

        if ((TMath::Abs(vHitsSts[uBestSts]->GetX() - dHodoExtrX - dOffsetXSts) < 3 * dSigmaXSts)
            && (TMath::Abs(-vHitsSts[uBestSts]->GetY() - dHodoExtrY - dOffsetYSts) < 3 * dSigmaYSts)) {
          phHitsStsEff->Fill(dHodoExtrX, dHodoExtrY);
        }
      }
    }  //  if( uBestB < vHitsHodoB.size() )
  }    // for( UInt_t uHitA = 0; uHitA < vHitsHodoA.size(); ++uHitA )
}


// ---- Finish --------------------------------------------------------
void CbmStsCoincHodo::Finish() { WriteHistos(); }

void CbmStsCoincHodo::WriteHistos()
{
  TFile* old     = gFile;
  TFile* outfile = TFile::Open(fOutFileName, "RECREATE");

  phHitsStsTime->Write();
  phHitsHodoATime->Write();
  phHitsHodoBTime->Write();

  phHitsPositionHodoA->Write();
  phHitsPositionSts->Write();
  phHitsPositionHodoB->Write();

  phNbHitsCompHodo->Write();
  phNbHitsCompStsHodoA->Write();
  phNbHitsCompStsHodoB->Write();

  phHitsCoincCorrXX->Write();
  phHitsCoincCorrYY->Write();
  phHitsCoincCorrXY->Write();
  phHitsCoincCorrYX->Write();
  phHitsPositionCoincA->Write();
  phHitsPositionCoincB->Write();
  phHitsPositionDiff->Write();
  phHitsTimeDiff->Write();
  phHitsCoincDist->Write();
  phHitsCoincAngle->Write();

  // phHitsSingleCoincCorrXX->Write();
  // phHitsSingleCoincCorrYY->Write();
  // phHitsSingleCoincCorrXY->Write();
  // phHitsSingleCoincCorrYX->Write();
  // phHitsSinglePositionCoincA->Write();
  // phHitsSinglePositionCoincB->Write();
  // phHitsSinglePositionDiff->Write();
  // phHitsSingleTimeDiff->Write();
  // phHitsSingleCoincDist->Write();
  // phHitsSingleCoincAngle->Write();

  phHitsBestCoincCorrXX->Write();
  phHitsBestCoincCorrYY->Write();
  phHitsBestCoincCorrXY->Write();
  phHitsBestCoincCorrYX->Write();
  phHitsBestPositionCoincA->Write();
  phHitsBestPositionCoincB->Write();
  phHitsBestPositionDiff->Write();
  phHitsBestTimeDiff->Write();
  phHitsBestCoincDist->Write();
  phHitsBestCoincAngle->Write();

  phHitsPositionCoincExtr->Write();

  phHitsStsCoincCorrXX->Write();
  phHitsStsCoincCorrYY->Write();
  phHitsStsCoincCorrXY->Write();
  phHitsStsCoincCorrYX->Write();
  phHitsStsPositionCoincExtr->Write();
  phHitsStsPositionCoinc->Write();
  phHitsStsTimeDiff->Write();
  phHitsStsPositionDiff->Write();
  phHitsStsPositionDiffInv->Write();

  phHitsStsBestCoincCorrXX->Write();
  phHitsStsBestCoincCorrYY->Write();
  phHitsStsBestCoincCorrXY->Write();
  phHitsStsBestCoincCorrYX->Write();
  phHitsStsBestPositionCoincExtr->Write();
  phHitsStsBestPositionCoinc->Write();
  phHitsStsBestPositionShiftCoinc->Write();
  phHitsStsBestTimeDiff->Write();
  phHitsStsBestPositionDiff->Write();
  phHitsStsBestPositionDiffInv->Write();
  phHitsStsBestDiff->Write();

  phHitsStsEff->Write();

  outfile->Close();
  delete outfile;

  gFile = old;
}

ClassImp(CbmStsCoincHodo)
