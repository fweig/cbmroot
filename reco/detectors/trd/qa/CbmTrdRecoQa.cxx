/* Copyright (C) 2007-2021 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Cyrano Bergmann [committer] */

// -----------------------------------------------------------------------
// -----                     CbmTrdRecoQa                        -----
// -----               Created 24.02.07  by F. Uhlig                 -----
// -----------------------------------------------------------------------

#include "CbmTrdRecoQa.h"

#include "CbmDigiManager.h"
#include "CbmMCTrack.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdHit.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdPoint.h"
#include "CbmTrdUtils.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TColor.h"
#include "TFrame.h"
#include "TGeoManager.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TPolyLine.h"
#include "TStopwatch.h"
#include "TVector3.h"

#include <iostream>

using std::cout;
using std::endl;
using std::map;

// ---- Default constructor -------------------------------------------------

CbmTrdRecoQa::CbmTrdRecoQa() : CbmTrdRecoQa("TrdRecoQa", "") {}

// ---- Standard constructor ------------------------------------------------
CbmTrdRecoQa::CbmTrdRecoQa(const char* name, const char*)
  : FairTask(name)
  , fTrianglePads(false)
  , fTriggerTH(1.0e-6)
  , fClusters(NULL)
  , fHits(NULL)
  , fMCPoints(NULL)
  , fDigiPar(NULL)
  , fModuleInfo(NULL)
  , fGeoHandler(new CbmTrdGeoHandler())
  , fModuleMap()
  , fModuleMapPoint()
  , fModuleMapDigi()
  , fModuleMapCluster()
  , fModuleMapHit()
  , fModuleMapTrack()
{
}
// --------------------------------------------------------------------------


// ---- Destructor ----------------------------------------------------------
CbmTrdRecoQa::~CbmTrdRecoQa() {}
// --------------------------------------------------------------------------
void CbmTrdRecoQa::SetTriggerThreshold(Double_t minCharge)
{
  fTriggerTH = minCharge;  //  To be used for test beam data processing
}

// ---- Initialisation ------------------------------------------------------
InitStatus CbmTrdRecoQa::Init()
{
  // Get pointer to the ROOT I/O manager
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (NULL == rootMgr) {
    cout << "-E- CbmTrdRecoQa::Init : "
         << "ROOT manager is not instantiated !" << endl;
    return kFATAL;
  }


  // Get pointer to TRD point array
  fMCPoints = (TClonesArray*) rootMgr->GetObject("TrdPoint");
  if (NULL == fMCPoints) {
    cout << "-W- CbmTrdRecoQa::Init : "
         << "no MC point array !" << endl;
    return kERROR;
  }

  CbmDigiManager::Instance()->Init();
  if (!CbmDigiManager::Instance()->IsPresent(ECbmModuleId::kTrd)) LOG(fatal) << GetName() << "Missing Trd digi branch.";


  // Get pointer to TRD point array
  fClusters = (TClonesArray*) rootMgr->GetObject("TrdCluster");
  if (NULL == fClusters) {
    cout << "-W- CbmTrdRecoQa::Init : "
         << "no cluster array !" << endl;
    return kERROR;
  }

  // Get pointer to TRD point array
  fHits = (TClonesArray*) rootMgr->GetObject("TrdHit");
  if (NULL == fHits) {
    cout << "-W- CbmTrdRecoQa::Init : "
         << "no hit array !" << endl;
    return kERROR;
  }

  fGeoHandler->Init();

  return kSUCCESS;
}

// --------------------------------------------------------------------------

// ----  Initialisation  ----------------------------------------------
void CbmTrdRecoQa::SetParContainers()
{
  cout << " * CbmTrdRecoQa * :: SetParContainers() " << endl;


  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));
}
// --------------------------------------------------------------------

// ---- ReInit  -------------------------------------------------------
InitStatus CbmTrdRecoQa::ReInit()
{

  cout << " * CbmTrdRecoQa * :: ReInit() " << endl;


  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));

  return kSUCCESS;
}
void CbmTrdRecoQa::SetTriangularPads(Bool_t triangles) { fTrianglePads = triangles; }
/*
TPolyLine *CbmTrdRecoQa::CreateTriangularPad(Int_t column, Int_t row, Double_t content){
  const Int_t nCoordinates = 4;
  Double_t x[nCoordinates] = {column-0.5,column+0.5,column+0.5,column-0.5};
  Double_t y[nCoordinates] = {row-0.5,   row-0.5,   row+0.5,   row-0.5   };
  if (row%2 != 0){
    y[1] = row+0.5;
    y[2] = row+0.5;
  }
  TPolyLine *pad = new TPolyLine(nCoordinates,x,y);
  pad->SetFillColor(content);
  return pad;
}
*/
// ---- Task execution ------------------------------------------------------
void CbmTrdRecoQa::Exec(Option_t*)
{
  CbmTrdUtils* utils = new CbmTrdUtils();
  TStopwatch timer;
  timer.Start();
  cout << "================CbmTrdRecoQa==============" << endl;
  // Declare variables
  CbmTrdPoint* point     = NULL;
  const CbmTrdDigi* digi = NULL;
  CbmTrdCluster* cluster = NULL;
  CbmTrdHit* hit         = NULL;

  Int_t nPoints(0), nDigis(0), nClusters(0), nHits(0);
  nDigis = CbmDigiManager::Instance()->GetNofDigis(ECbmModuleId::kTrd);
  if (fClusters) nClusters = fClusters->GetEntriesFast();
  if (fMCPoints) nPoints = fMCPoints->GetEntriesFast();
  if (fHits) nHits = fHits->GetEntriesFast();

  TH1D* digiMaxSpectrum = new TH1D("digiMaxSpectrum", "digiMaxSpectrum", 10000, 0, 1e-4);
  TH1D* digiSpectrum    = new TH1D("digiSpectrum", "digiSpectrum", 10000, 0, 1e-4);

  LOG(info) << "CbmTrdRecoQa::Exec : MC-Points:" << nPoints;
  LOG(info) << "CbmTrdRecoQa::Exec : Digis:    " << nDigis;
  LOG(info) << "CbmTrdRecoQa::Exec : Cluster:  " << nClusters;
  LOG(info) << "CbmTrdRecoQa::Exec : Hits:     " << nHits;
  Int_t moduleAddress(-1), moduleId(-1);

  //  Double_t dummy_x(-1), dummy_y(-1);
  TString name;
  //cout << "Points" << endl;
  Int_t pointCounter = 0;
  for (Int_t iPoint = 0; iPoint < nPoints; iPoint++) {
    //cout << iPoint << endl;
    point           = (CbmTrdPoint*) fMCPoints->At(iPoint);
    Double_t in[3]  = {point->GetXIn(), point->GetYIn(), point->GetZIn()};
    Double_t out[3] = {point->GetXOut(), point->GetYOut(), point->GetZOut()};
    gGeoManager->FindNode((out[0] + in[0]) / 2, (out[1] + in[1]) / 2, (out[2] + in[2]) / 2);
    if (!TString(gGeoManager->GetPath()).Contains("gas")) {
      LOG(error) << "CbmTrdRecoQa::Exec: MC-track not in TRD! Node:" << TString(gGeoManager->GetPath()).Data()
                 << " gGeoManager->MasterToLocal() failed!";
      continue;
    }
    //std::map<Int_t, std::vector<TH2D*> >::iterator it = fModuleMap.find(moduleAddress);
    moduleAddress = CbmTrdAddress::GetModuleAddress(point->GetDetectorID());  //
    moduleId      = CbmTrdAddress::GetModuleId(point->GetDetectorID());
    //printf("Address:%i ID:%i\n",moduleAddress,moduleId);
    fModuleInfo = (CbmTrdParModDigi*) fDigiPar->GetModulePar(moduleAddress);  ////point->GetDetectorID());
    //printf("Address:%i ID:%i\n",moduleAddress,moduleId);
    if (fModuleInfo) {
      std::map<Int_t, TGraphErrors*>::iterator it = fModuleMapPoint.find(moduleAddress);
      if (it == fModuleMapPoint.end()) {
        name.Form("ModuleAddress%05i", moduleAddress);
        fModuleMap[moduleAddress] = new TCanvas(name, name, 1000, 1000);
        fModuleMap[moduleAddress]->Divide(2, 2);
        //name.Form("ModuleAddress%iPoints",moduleAddress);
        fModuleMapPoint[moduleAddress] = new TGraphErrors();
        fModuleMapPoint[moduleAddress]->SetMarkerStyle(20);
        fModuleMapPoint[moduleAddress]->SetMarkerSize(0.5);
        fModuleMapPoint[moduleAddress]->SetMarkerColor(15);
        //fModuleMapPoint[moduleAddress] = new TH2I(name,name,fModuleInfo->GetNofColumns(),-0.5,fModuleInfo->GetNofColumns()-0.5,fModuleInfo->GetNofRows(),-0.5,fModuleInfo->GetNofRows()-0.5);
        TH2I* dummy = new TH2I(name, name, fModuleInfo->GetNofColumns(), -0.5, fModuleInfo->GetNofColumns() - 0.5,
                               fModuleInfo->GetNofRows(), -0.5, fModuleInfo->GetNofRows() - 0.5);
        dummy->SetStats(kFALSE);
        name.Form("ModuleAddress%05iDigis", moduleAddress);
        fModuleMapDigi[moduleAddress] =
          new TH2D(name, name, fModuleInfo->GetNofColumns(), -0.5, fModuleInfo->GetNofColumns() - 0.5,
                   fModuleInfo->GetNofRows(), -0.5, fModuleInfo->GetNofRows() - 0.5);
        fModuleMapDigi[moduleAddress]->SetContour(99);
        fModuleMapDigi[moduleAddress]->SetStats(kFALSE);
        name.Form("ModuleAddress%05iClusters", moduleAddress);
        fModuleMapCluster[moduleAddress] =
          new TH2I(name, name, fModuleInfo->GetNofColumns(), -0.5, fModuleInfo->GetNofColumns() - 0.5,
                   fModuleInfo->GetNofRows(), -0.5, fModuleInfo->GetNofRows() - 0.5);
        fModuleMapCluster[moduleAddress]->SetContour(99);
        fModuleMapCluster[moduleAddress]->SetStats(kFALSE);
        name.Form("ModuleAddress%05iHits", moduleAddress);
        fModuleMapHit[moduleAddress] =
          new TGraphErrors();  //name,name,fModuleInfo->GetNofColumns(),-0.5,fModuleInfo->GetNofColumns()-0.5,fModuleInfo->GetNofRows(),-0.5,fModuleInfo->GetNofRows()-0.5);
        fModuleMapHit[moduleAddress]->SetMarkerStyle(24);
        fModuleMapTrack[moduleAddress] = new std::vector<TLine*>;
        fModuleMap[moduleAddress]->cd(1);
        dummy->Draw(); /*
			fModuleMapPoint[moduleAddress]->Draw("AP");
			fModuleMapPoint[it->first]->SetMaximum(fModuleMapDigi[it->first]->GetYaxis()->GetXmax());
			fModuleMapPoint[it->first]->SetMinimum(fModuleMapDigi[it->first]->GetYaxis()->GetXmin());
			fModuleMapPoint[it->first]->GetXaxis()->SetLimits(fModuleMapDigi[it->first]->GetXaxis()->GetXmin(), 
			fModuleMapDigi[it->first]->GetXaxis()->GetXmax());
			fModuleMapPoint[moduleAddress]->Draw("AP");
		      */
      }
      //cout << iPoint << endl;


      Double_t local_in[3];
      Double_t local_out[3];
      gGeoManager->MasterToLocal(in, local_in);
      gGeoManager->MasterToLocal(out, local_out);

      for (Int_t i = 0; i < 3; i++)
        local_out[i] =
          local_in[i]
          + 0.975 * (local_out[i] - local_in[i]);  // cut last 2.5% of tracklet, to move exit point within gas volume

      Int_t row_in(0), row_out(0), col_in(0), col_out(0), sec_in(0), sec_out(0);
      Double_t x_in(0), y_in(0), x_out(0), y_out(0);
      if (!fModuleInfo->GetPadInfo(local_in, sec_in, col_in, row_in)) continue;
      //printf("a: local_in (%f,%f,%f) sec:%i, col:%i row:%i\n",local_in[0],local_in[1],local_in[2], sec_in, col_in, row_in);
      if ((sec_in < 0) || (sec_in > 2)) {
        cout << "sec_in:" << sec_in << endl;
        continue;
      }
      fModuleInfo->TransformToLocalPad(local_in, x_in, y_in);
      //printf("b: local_in (%f,%f,%f) sec:%i, col:%i row:%i  (%f,%f)\n",local_in[0],local_in[1],local_in[2], sec_in, col_in, row_in, x_in, y_in);
      row_in = fModuleInfo->GetModuleRow(sec_in, row_in);
      //printf("c: local_in (%f,%f,%f) sec:%i, col:%i row:%i/%i (%f,%f)\n",local_in[0],local_in[1],local_in[2], sec_in, col_in, row_in, fModuleInfo->GetNofRows(), x_in, y_in);
      if ((sec_out < 0) || (sec_out > 2)) {
        cout << "sec_out:" << sec_out << endl;
        continue;
      }
      //printf("d1: local_out (%f,%f,%f) sec:%i, col:%i row:%i\n",local_out[0],local_out[1],local_out[2], sec_out, col_out, row_out);
      if (!fModuleInfo->GetPadInfo(local_out, sec_out, col_out, row_out)) continue;
      //printf("d: local_out (%f,%f,%f) sec:%i, col:%i row:%i\n",local_out[0],local_out[1],local_out[2], sec_out, col_out, row_out);
      fModuleInfo->TransformToLocalPad(local_out, x_out, y_out);
      //printf("e: local_out (%f,%f,%f) sec:%i, col:%i row:%i  (%f,%f)\n",local_out[0],local_out[1],local_out[2], sec_out, col_out, row_out, x_out, y_out);
      row_out = fModuleInfo->GetModuleRow(sec_out, row_out);
      //printf("f: local_out (%f,%f,%f) sec:%i, col:%i row:%i/%i (%f,%f)\n",local_out[0],local_out[1],local_out[2], sec_out, col_out,row_out, fModuleInfo->GetNofRows(), x_out, y_out);
      Double_t W_in(fModuleInfo->GetPadSizeX(sec_in)), W_out(fModuleInfo->GetPadSizeX(sec_out));
      Double_t H_in(fModuleInfo->GetPadSizeY(sec_in)), H_out(fModuleInfo->GetPadSizeY(sec_out));


      pointCounter = fModuleMapPoint[moduleAddress]->GetN();
      fModuleMapPoint[moduleAddress]->SetPoint(pointCounter, ((col_in + x_in / W_in) + (col_out + x_out / W_out)) / 2.,
                                               ((row_in + y_in / H_in) + (row_out + y_out / H_out)) / 2.);
      //pointCounter++;
      fModuleMap[moduleAddress]->cd();
      TLine* l =
        new TLine(col_in + x_in / W_in, row_in + y_in / H_in, col_out + x_out / W_out, row_out + y_out / H_out);
      l->SetLineWidth(2);
      l->SetLineColor(15);
      fModuleMapTrack[moduleAddress]->push_back(l);
      fModuleMap[moduleAddress]->cd(1);
      l->Draw("same");
      /*
	fModuleMap[moduleAddress]->cd(2);
	l->Draw("same");
	fModuleMap[moduleAddress]->cd(3);
	l->Draw("same");
	fModuleMap[moduleAddress]->cd(4);
	l->Draw("same");
      */
    }
    else {
      printf("Address:%i ID:%i\n", moduleAddress, moduleId);
      break;
    }
  }
  //fModuleMap[moduleAddress]->cd(1);
  //name.Form("%i Points",pointCounter);
  //TPaveText *textpoints = new TPaveText(10,2.5,50,3);
  //textpoints->SetTextSize(0.035);
  //textpoints->AddText(name);
  //textpoints->Draw("same");
  //cout << "Digis" << endl;
  for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
    digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(iDigi);
    digiSpectrum->Fill(digi->GetCharge());
    Int_t digiAddress = digi->GetAddress();
    moduleAddress     = CbmTrdAddress::GetModuleAddress(digiAddress);
    fModuleInfo       = (CbmTrdParModDigi*) fDigiPar->GetModulePar(moduleAddress);  ////point->GetDetectorID());
    Int_t sec(CbmTrdAddress::GetSectorId(digiAddress)), row(CbmTrdAddress::GetRowId(digiAddress));
    row = fModuleInfo->GetModuleRow(sec, row);
    fModuleMapDigi[moduleAddress]->Fill(CbmTrdAddress::GetColumnId(digiAddress), row, digi->GetCharge());
  }
  //cout << "Clusters" << endl;
  Int_t lastModule(0), iCounter(0);
  for (Int_t iCluster = 0; iCluster < nClusters; iCluster++) {
    Double_t charge(0), chargeMax(0);
    cluster               = (CbmTrdCluster*) fClusters->At(iCluster);
    Int_t nDigisInCluster = cluster->GetNofDigis();
    iCounter++;
    for (Int_t iDigi = 0; iDigi < nDigisInCluster; iDigi++) {
      digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(cluster->GetDigi(iDigi));
      //      digi = (CbmTrdDigi*)fDigis->At(cluster->GetDigi(iDigi));
      Int_t digiAddress = digi->GetAddress();
      moduleAddress     = CbmTrdAddress::GetModuleAddress(digiAddress);
      if (iDigi == 0 && lastModule != moduleAddress) {
        iCounter   = 0;
        lastModule = moduleAddress;
      }
      //cout << moduleAddress << endl;
      fModuleInfo = (CbmTrdParModDigi*) fDigiPar->GetModulePar(moduleAddress);  ////point->GetDetectorID());
      Int_t sec(CbmTrdAddress::GetSectorId(digiAddress)), row(CbmTrdAddress::GetRowId(digiAddress));
      row = fModuleInfo->GetModuleRow(sec, row);
      fModuleMapCluster[moduleAddress]->Fill(CbmTrdAddress::GetColumnId(digiAddress), row, iCounter + 1);
      //fModuleMapCluster[moduleAddress]->SetBinContent(CbmTrdAddress::GetColumnId(digiAddress)+1, row+1, iCounter+1);
      charge = digi->GetCharge();
      if (charge > chargeMax) chargeMax = charge;
    }
    digiMaxSpectrum->Fill(chargeMax);
  }

  //cout << "Hits" << endl;
  Int_t modHit = 0;
  for (Int_t iHit = 0; iHit < nHits; iHit++) {
    hit = (CbmTrdHit*) fHits->At(iHit);

    moduleAddress = hit->GetAddress();                                          //GetDetectorID();//?????
    fModuleInfo   = (CbmTrdParModDigi*) fDigiPar->GetModulePar(moduleAddress);  ////point->GetDetectorID());
    if (fModuleInfo == NULL) {
      //printf("MA:%6i not found!\n",moduleAddress);
      continue;
    }

    Double_t pos[3] = {hit->GetX(), hit->GetY(), hit->GetZ()};
    gGeoManager->FindNode(pos[0], pos[1], pos[2] - 0.3);
    Double_t local_pos[3];
    gGeoManager->MasterToLocal(pos, local_pos);
    Int_t sec(-1), col(-1), row(-1);
    fModuleInfo->GetPadInfo(local_pos, sec, col, row);
    row = fModuleInfo->GetModuleRow(sec, row);
    Double_t x(-1), y(-1);
    fModuleInfo->TransformToLocalPad(local_pos, x, y);

    Double_t W(fModuleInfo->GetPadSizeX(sec)), H(fModuleInfo->GetPadSizeY(sec));
    modHit = fModuleMapHit[moduleAddress]->GetN();
    fModuleMapHit[moduleAddress]->SetPoint(modHit, col + x / W, row + y / H);

    if (hit->GetDx() <= W) fModuleMapHit[moduleAddress]->SetPointError(modHit, hit->GetDx() / W, hit->GetDy() / H);
    else
      fModuleMapHit[moduleAddress]->SetPointError(modHit, hit->GetDy() / W, hit->GetDx() / H);
  }

  gDirectory->mkdir("TrdRecoQA");
  gDirectory->cd("TrdRecoQA");
  digiMaxSpectrum->Write("", TObject::kOverwrite);
  digiSpectrum->Write("", TObject::kOverwrite);
  map<Int_t, TCanvas*>::iterator it;
  for (it = fModuleMap.begin(); it != fModuleMap.end(); it++) {
    it->second->cd(1);
    name.Form("%i Points", fModuleMapPoint[it->first]->GetN());
    fModuleMapPoint[it->first]->SetNameTitle(name, name);
    fModuleMapPoint[it->first]->SetFillStyle(0);
    TPaveText* ptext = new TPaveText(10, 3.5, 11, 4.0);
    ptext->SetTextSize(0.035);
    ptext->SetTextColor(15);
    ptext->SetFillStyle(0);
    ptext->SetLineColor(0);
    ptext->AddText(name);
    //fModuleMapPoint[it->first]->Draw("P");


    //fModuleMapPoint[it->first]->Draw("P,same");
    ptext->Draw("same");
    it->second->cd(1)->Update();
    it->second->cd(2);
    fModuleMapDigi[it->first]->GetZaxis()->SetRangeUser(0, 0.0001);
    fModuleMapDigi[it->first]->DrawCopy("colz");

    {
      TPolyLine* pad           = NULL;
      const Int_t nRow         = fModuleMapDigi[it->first]->GetNbinsY();
      const Int_t nCol         = fModuleMapDigi[it->first]->GetNbinsX();
      const Double_t max_Range = fModuleMapDigi[it->first]->GetBinContent(fModuleMapDigi[it->first]->GetMaximumBin());
      for (Int_t iRow = 1; iRow <= nRow; iRow++) {
        for (Int_t iCol = 1; iCol <= nCol; iCol++) {
          Double_t charge = fModuleMapDigi[it->first]->GetBinContent(iCol, iRow);
          if (charge > 0.0) {
            if (fTrianglePads) pad = utils->CreateTriangularPad(iCol - 1, iRow - 1, charge, 0.0, max_Range, false);
            else
              pad = utils->CreateRectangularPad(iCol - 1, iRow - 1, charge, 0.0, max_Range, false);
            pad->Draw("f,same");
          }
        }
      }
    }

    for (UInt_t t = 0; t < fModuleMapTrack[it->first]->size(); t++)
      fModuleMapTrack[it->first]->at(t)->Draw("same");
    /*
    it->second->cd(3)->SetLogz(1);
    fModuleMapDigi[it->first]->GetZaxis()->SetRangeUser(fTriggerTH,fModuleMapDigi[it->first]->GetBinContent(fModuleMapDigi[it->first]->GetMaximumBin()));
    fModuleMapDigi[it->first]->DrawCopy("colz");
    {
      TPolyLine *pad = NULL;
      const Int_t nRow = fModuleMapDigi[it->first]->GetNbinsY();
      const Int_t nCol = fModuleMapDigi[it->first]->GetNbinsX();
      const Double_t max_Range = fModuleMapDigi[it->first]->GetBinContent(fModuleMapDigi[it->first]->GetMaximumBin());
      for (Int_t iRow = 1; iRow <= nRow; iRow++){
	for (Int_t iCol = 1; iCol <= nCol; iCol++){
	  Double_t charge = fModuleMapDigi[it->first]->GetBinContent(iCol, iRow);
	  if (charge >= fTriggerTH){
	    if (fTrianglePads)
	      pad = utils->CreateTriangularPad(iCol-1, iRow-1, charge, 0, max_Range, true);
	    else
	      pad = utils->CreateRectangularPad(iCol-1, iRow-1, charge, 0.0, max_Range, false);
	    pad->Draw("f,same");
	  }
	}
      }
    }
    */


    for (UInt_t t = 0; t < fModuleMapTrack[it->first]->size(); t++)
      fModuleMapTrack[it->first]->at(t)->Draw("same");

    it->second->cd(3);
    fModuleMapCluster[it->first]->DrawCopy("colz");
    {
      TPolyLine* pad   = NULL;
      const Int_t nRow = fModuleMapCluster[it->first]->GetNbinsY();
      const Int_t nCol = fModuleMapCluster[it->first]->GetNbinsX();
      const Double_t max_Range =
        fModuleMapCluster[it->first]->GetBinContent(fModuleMapCluster[it->first]->GetMaximumBin());
      for (Int_t iRow = 1; iRow <= nRow; iRow++) {
        for (Int_t iCol = 1; iCol <= nCol; iCol++) {
          Double_t clusterId = fModuleMapCluster[it->first]->GetBinContent(iCol, iRow);
          if (clusterId > 0) {
            if (fTrianglePads) pad = utils->CreateTriangularPad(iCol - 1, iRow - 1, clusterId, 0, max_Range, false);
            else
              pad = utils->CreateRectangularPad(iCol - 1, iRow - 1, clusterId, 0.0, max_Range, false);
            pad->Draw("f,same");
          }
        }
      }
    }
    for (UInt_t t = 0; t < fModuleMapTrack[it->first]->size(); t++)
      fModuleMapTrack[it->first]->at(t)->Draw("same");
    it->second->cd(3)->Update();
    it->second->cd(4);
    //fModuleMapPoint[it->first]->Draw();
    //fModuleMapHit[it->first]->Draw("P,same");
    if (fHits) {
      /*
	fModuleMapHit[it->first]->SetMaximum(fModuleMapDigi[it->first]->GetYaxis()->GetXmax());
	fModuleMapHit[it->first]->SetMinimum(fModuleMapDigi[it->first]->GetYaxis()->GetXmin());
	fModuleMapHit[it->first]->GetXaxis()->SetLimits(fModuleMapDigi[it->first]->GetXaxis()->GetXmin(), 
	fModuleMapDigi[it->first]->GetXaxis()->GetXmax());
      */
      name.Form("%i Hits", fModuleMapHit[it->first]->GetN());
      fModuleMapHit[it->first]->SetNameTitle(name, name);
      TPaveText* text = new TPaveText(10, 2.0, 11, 2.5);
      text->SetFillStyle(0);
      text->SetLineColor(0);
      text->SetTextSize(0.035);
      text->AddText(name);
      TMultiGraph* mg = new TMultiGraph();

      //fModuleMapHit[it->first]->Draw("AP");
      //fModuleMapPoint[it->first]->Draw("P,same");
      mg->Draw("AP");
      mg->Add(fModuleMapHit[it->first]);
      mg->Add(fModuleMapPoint[it->first]);
      mg->SetMaximum(fModuleMapDigi[it->first]->GetYaxis()->GetXmax());
      mg->SetMinimum(fModuleMapDigi[it->first]->GetYaxis()->GetXmin());
      mg->GetXaxis()->SetLimits(fModuleMapDigi[it->first]->GetXaxis()->GetXmin(),
                                fModuleMapDigi[it->first]->GetXaxis()->GetXmax());
      text->Draw("same");
      ptext->Draw("same");
    }
    it->second->Update();
    it->second->SaveAs("pics/" + (TString)(it->second->GetName()) + ".png");
    it->second->Write("", TObject::kOverwrite);
    it->second->Close();
  }
  gDirectory->cd("..");
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();

  printf("\n\n******************** Reading Test  **********************\n");
  printf("   RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);
  printf("*********************************************************\n\n");
}

// --------------------------------------------------------------------------
//Int_t CbmTrdRecoQa::SecRowToGlobalRow(Int_t secRow)
//{

//}

// ---- Finish --------------------------------------------------------------
void CbmTrdRecoQa::Finish() { WriteHistograms(); }
// --------------------------------------------------------------------------

// ---- Write test histograms ------------------------------------------------

void CbmTrdRecoQa::WriteHistograms()
{
  /*
  // Write histos to output
  gDirectory->mkdir("TrdTracksPidQA");
  gDirectory->cd("TrdTracksPidQA");

  if(WknPI)         WknPI->Write();
  if(WknEL)         WknEL->Write();
  if(WknALL)        WknALL->Write();
  if(WknLowPI)      WknLowPI->Write();
  if(WknLowEL)      WknLowEL->Write();
  if(WknLowALL)     WknLowALL->Write();
  if(WknHighPI)     WknHighPI->Write();
  if(WknHighEL)     WknHighEL->Write();
  if(WknHighALL)    WknHighALL->Write();

  if(AnnPI)         AnnPI->Write();
  if(AnnEL)         AnnEL->Write();
  if(AnnALL)        AnnALL->Write();
  if(AnnLowPI)      AnnLowPI->Write();
  if(AnnLowEL)      AnnLowEL->Write();
  if(AnnLowALL)     AnnLowALL->Write();
  if(AnnHighPI)     AnnHighPI->Write();
  if(AnnHighEL)     AnnHighEL->Write();
  if(AnnHighALL)    AnnHighALL->Write();

  if(LikePI)        LikePI->Write();
  if(LikeEL)        LikeEL->Write();
  if(LikeALL)       LikeALL->Write();
  if(LikeHighPI)    LikeHighPI->Write();
  if(LikeHighEL)    LikeHighEL->Write();
  if(LikeHighALL)   LikeHighALL->Write();
  if(LikeLowPI)     LikeLowPI->Write();
  if(LikeLowEL)     LikeLowEL->Write();
  if(LikeLowALL)    LikeLowALL->Write();

  if(PartID)        PartID->Write();
  if(NrTRDHits)     NrTRDHits->Write();
  if(ELossPI)       ELossPI->Write();
  if(ELossEL)       ELossEL->Write();
  if(ELossALL)      ELossALL->Write();
  if(MomPI)         MomPI->Write();
  if(MomEL)         MomEL->Write();
  if(MomALL)        MomALL->Write();
  if(MOMvsELossPI)  MOMvsELossPI->Write();
  if(MOMvsELossEL)  MOMvsELossEL->Write();
  if(MOMvsELossALL) MOMvsELossALL->Write();

  gDirectory->cd("..");
  */
}

// --------------------------------------------------------------------------

ClassImp(CbmTrdRecoQa);
