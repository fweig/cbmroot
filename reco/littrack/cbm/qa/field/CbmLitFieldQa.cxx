/**
 * \file CbmLitFieldQa.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 **/
#include "CbmLitFieldQa.h"

#include "CbmHistManager.h"
#include "CbmLitFieldQaReport.h"
#include "CbmRichDetectorData.h"  // for CbmRichPmtData, CbmRichPixelData
#include "CbmRichDigiMapManager.h"
#include "CbmRichGeoManager.h"
#include "CbmUtils.h"

#include "FairField.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TGraph.h"
#include "TGraph2D.h"
#include "TVector3.h"
#include <TFile.h>

#include <boost/assign/list_of.hpp>

#include <sstream>

#include <cmath>

using boost::assign::list_of;
using Cbm::ToString;
using namespace std;

CbmLitFieldQa::CbmLitFieldQa()
  : fField(NULL)
  , fNofSlices(0)
  , fZSlicePosition()
  , fXSlicePosition()
  , fYSlicePosition()
  , fAlongZAngles()
  , fAlongZXY()
  , fZMin(-10.)
  , fZMax(300.)
  , fZStep(1.)
  ,
  //fAcceptanceAngleX(35.),
  //fAcceptanceAngleY(35.),
  fNofBinsX(100)
  , fNofBinsY(100)
  , fMinZFieldIntegral(171.)
  , fMaxZFieldIntegral(330.)
  , fHM(NULL)
  , fOutputDir("./")
{
}

CbmLitFieldQa::~CbmLitFieldQa() {}

InitStatus CbmLitFieldQa::Init()
{
  fNofSlices = fZSlicePosition.size();

  // Calculate (X, Y) window for each slice
  fXSlicePosition.resize(fNofSlices);
  fYSlicePosition.resize(fNofSlices);
  for (Int_t i = 0; i < fNofSlices; i++) {
    //        Double_t tanXangle = tan(fAcceptanceAngleX * 3.14159265 / 180); //
    //        Double_t tanYangle = tan(fAcceptanceAngleY * 3.14159265 / 180); //
    //        fXSlicePosition[i] = fZSlicePosition[i] * tanXangle;
    //        fYSlicePosition[i] = fZSlicePosition[i] * tanYangle;
    fXSlicePosition[i] = 250.;
    fYSlicePosition[i] = 250.;
  }

  vector<Double_t> tmp = list_of(0.)(10.)(20.)(30.);
  fAlongZAngles        = tmp;

  fAlongZXY.push_back(make_pair(0., 0.));
  fAlongZXY.push_back(make_pair(100., 0.));
  fAlongZXY.push_back(make_pair(0., 100.));


  fField = FairRunAna::Instance()->GetField();

  fHM = new CbmHistManager();

  CreateHistos();
  FillBHistos();

  FillRichPmtPlaneBHistos();

  CbmSimulationReport* report = new CbmLitFieldQaReport();
  report->Create(fHM, fOutputDir);
  delete report;
  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());

  // print some field values at RICH entrance
  Double_t B[3];
  vector<vector<Double_t>> pos = {{0., 0., 170.}, {0., 80., 170.}, {50., 0., 170.}, {0., 0., 250.}};
  for (Int_t i = 0; i < pos.size(); i++) {
    fField->GetFieldValue(&pos[i][0], B);
    Double_t magB = sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);
    cout << "B at (" << pos[i][0] << ", " << pos[i][1] << ", " << pos[i][2] << ") = " << magB << " kGauss" << endl;
  }

  cout << "CbmLitFieldQa::Init() end" << endl;

  return kSUCCESS;
}

void CbmLitFieldQa::Exec(Option_t* opt) {}

void CbmLitFieldQa::Finish() {}

void CbmLitFieldQa::CreateHistos()
{
  string names[]  = {"Bx", "By", "Bz", "Mod"};
  string zTitle[] = {"B_{x} [kGauss]", "B_{y} [kGauss]", "B_{z} [kGauss]", "|B| [kGauss]"};
  for (Int_t v = 0; v < 4; v++) {
    for (Int_t i = 0; i < fNofSlices; i++) {
      TGraph2D* graph = new TGraph2D();
      graph->SetNpx(200);
      graph->SetNpy(200);
      string name  = "hmf_" + names[v] + "_Graph2D_" + ToString<Int_t>(fZSlicePosition[i]);
      string title = name + ";X [cm];Y [cm];" + zTitle[v];
      graph->SetNameTitle(name.c_str(), title.c_str());
      fHM->Add(name, graph);
    }
  }

  for (Int_t v = 0; v < 4; v++) {
    for (Int_t i = 0; i < fAlongZAngles.size(); i++) {
      TGraph* graph = new TGraph();
      string name   = "hmf_" + names[v] + "AlongZAngle_Graph_" + ToString<Int_t>(fAlongZAngles[i]);
      string title  = name + ";Z [cm];B [kGauss]";
      graph->SetNameTitle(name.c_str(), title.c_str());
      fHM->Add(name, graph);
    }
    for (Int_t i = 0; i < fAlongZXY.size(); i++) {
      TGraph* graph = new TGraph();
      string name   = "hmf_" + names[v] + "AlongZXY_Graph_" + ToString<Int_t>(fAlongZXY[i].first) + "_"
                    + ToString<Int_t>(fAlongZXY[i].second);
      string title = name + ";Z [cm];B [kGauss]";
      graph->SetNameTitle(name.c_str(), title.c_str());
      fHM->Add(name, graph);
    }
    for (Int_t i = 0; i < fAlongZXY.size(); i++) {
      TGraph* graph = new TGraph();
      string name   = "hmf_" + names[v] + "AlongZXYIntegral_Graph_" + ToString<Int_t>(fAlongZXY[i].first) + "_"
                    + ToString<Int_t>(fAlongZXY[i].second);
      string title = name + ";Z [cm];B_{Int_t} [kGauss*m]";
      graph->SetNameTitle(name.c_str(), title.c_str());
      fHM->Add(name, graph);
    }
  }

  for (Int_t p = 0; p < 2; p++) {
    for (Int_t v = 0; v < 4; v++) {
      TGraph2D* graph = new TGraph2D();
      graph->SetNpx(200);
      graph->SetNpy(200);
      string name  = "hmf_" + names[v] + "_Rich_Pmt_" + ((p == 0) ? "up" : "down");
      string title = name + ";X [cm];Y [cm];" + zTitle[v];
      graph->SetNameTitle(name.c_str(), title.c_str());
      fHM->Add(name, graph);
    }
  }
}

void CbmLitFieldQa::FillBHistos()
{
  string names[] = {"Bx", "By", "Bz", "Mod"};
  // Fill graphs for magnetic field for each (X, Y) slice
  for (UInt_t iSlice = 0; iSlice < fNofSlices; iSlice++) {  // loop over slices
    Double_t Z = fZSlicePosition[iSlice];

    Int_t cnt   = 0;
    Double_t HX = 2 * fXSlicePosition[iSlice] / fNofBinsX;  // step size for X position
    Double_t HY = 2 * fYSlicePosition[iSlice] / fNofBinsY;  // step size for Y position
    for (Int_t iX = 0; iX < fNofBinsX; iX++) {              // loop over x position
      Double_t X = -fXSlicePosition[iSlice] + (iX + 0.5) * HX;
      for (Int_t iY = 0; iY < fNofBinsY; iY++) {  // loop over y position
        Double_t Y = -fYSlicePosition[iSlice] + (iY + 0.5) * HY;

        // Get field value
        Double_t pos[3] = {X, Y, Z};
        Double_t B[4];
        fField->GetFieldValue(pos, B);

        B[3] = sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);
        for (Int_t v = 0; v < 4; v++) {
          string name = "hmf_" + names[v] + "_Graph2D_" + ToString<Int_t>(fZSlicePosition[iSlice]);
          fHM->G2(name)->SetPoint(cnt, X, Y, B[v]);
        }
        cnt++;
      }
    }
  }

  // Fill histograms for magnetic field along Z for different angles
  for (Int_t i = 0; i < fAlongZAngles.size(); i++) {
    Int_t nofSteps = Int_t((fZMax - fZMin) / fZStep);
    for (Int_t istep = 0; istep < nofSteps; istep++) {
      Double_t Z         = fZMin + istep * fZStep;
      Double_t tanXangle = tan(fAlongZAngles[i] * 3.14159265 / 180);  //
      Double_t tanYangle = tan(fAlongZAngles[i] * 3.14159265 / 180);  //
      Double_t X         = Z * tanXangle;
      Double_t Y         = Z * tanYangle;

      // Get field value
      Double_t pos[3] = {X, Y, Z};
      Double_t B[4];
      fField->GetFieldValue(pos, B);

      B[3] = sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);
      for (Int_t v = 0; v < 4; v++) {
        string name = "hmf_" + names[v] + "AlongZAngle_Graph_" + ToString<Int_t>(fAlongZAngles[i]);
        fHM->G1(name)->SetPoint(istep, Z, B[v]);
      }
    }
  }
  // Fill histograms for magnetic field along Z for different X position
  for (Int_t i = 0; i < fAlongZXY.size(); i++) {
    Int_t nofSteps        = Int_t((fZMax - fZMin) / fZStep);
    Double_t integralB[4] = {0., 0., 0., 0.};
    for (Int_t istep = 0; istep < nofSteps; istep++) {
      Double_t Z = fZMin + istep * fZStep;
      Double_t X = fAlongZXY[i].first;
      Double_t Y = fAlongZXY[i].second;

      // Get field value
      Double_t pos[3] = {X, Y, Z};
      Double_t B[4];
      fField->GetFieldValue(pos, B);

      B[3] = sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);

      for (Int_t v = 0; v < 4; v++) {
        string name = "hmf_" + names[v] + "AlongZXY_Graph_" + ToString<Int_t>(fAlongZXY[i].first) + "_"
                      + ToString<Int_t>(fAlongZXY[i].second);
        fHM->G1(name)->SetPoint(istep, Z, B[v]);
        // Calculate field integral in the RICH detector
        if (Z >= fMinZFieldIntegral && Z <= fMaxZFieldIntegral) {
          integralB[v] += 0.01 * fZStep * fabs(B[v]);  // in kGauss * meter
          string name = "hmf_" + names[v] + "AlongZXYIntegral_Graph_" + ToString<Int_t>(fAlongZXY[i].first) + "_"
                        + ToString<Int_t>(fAlongZXY[i].second);
          fHM->G1(name)->SetPoint(istep, Z, integralB[v]);
          fHM->G1(name)->SetMaximum(1.1 * integralB[v]);
        }
      }
    }
  }
}

void CbmLitFieldQa::FillRichPmtPlaneBHistos()
{
  string names[]       = {"Bx", "By", "Bz", "Mod"};
  vector<Int_t> pixels = CbmRichDigiMapManager::GetInstance().GetPixelAddresses();
  if (pixels.size() == 0) return;

  Double_t maxModB = 0.;
  int iS           = -1;
  for (Int_t i = 0; i < pixels.size(); i++) {
    TVector3 inPos;
    CbmRichPixelData* pixelData = CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(pixels[i]);
    inPos.SetXYZ(pixelData->fX, pixelData->fY, pixelData->fZ);
    string ud = "up";
    if (inPos.Y() < 0) ud = "down";
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);

    // Get field value
    // Take B-field values from real PMT position
    Double_t pos[3] = {inPos.X(), inPos.Y(), inPos.Z()};

    Double_t B[4];
    fField->GetFieldValue(pos, B);

    B[3] = sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);
    if (maxModB < B[3]) maxModB = B[3];
    for (Int_t v = 0; v < 4; v++) {
      string name = "hmf_" + names[v] + "_Rich_Pmt_" + ud;
      // Take B-field values from real PMT position, but display them for rotated X and Y
      fHM->G2(name)->SetPoint(fHM->G2(name)->GetN(), outPos.X(), outPos.Y(), B[v]);
    }
  }

  cout << "Maximum Bmod onto RICH PMT:" << maxModB << " kGauss" << endl;
}

ClassImp(CbmLitFieldQa);
