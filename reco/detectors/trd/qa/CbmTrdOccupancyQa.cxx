/* Copyright (C) 2012-2021 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Cyrano Bergmann [committer], Florian Uhlig */

#include "CbmTrdOccupancyQa.h"

#include "CbmDigiManager.h"
#include "CbmTrdAddress.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParModGeo.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParSetGeo.h"

#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>

#include <TBox.h>
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TColor.h>
#include <TF1.h>
#include <TFile.h>
#include <TGeoManager.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TStopwatch.h>

#include <iostream>

#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::fabs;
using std::pair;

CbmTrdOccupancyQa::CbmTrdOccupancyQa() : CbmTrdOccupancyQa("TrdOccupancy", "", "", 1e-6, kFALSE) {}

CbmTrdOccupancyQa::CbmTrdOccupancyQa(const char* name, const char*, const char* geo, Double_t triggerThreshold,
                                     Bool_t plotMergedResults)
  : FairTask(name)
  , fClusters(NULL)
  , fDigiPar(NULL)
  , fGeoPar(NULL)
  , fGeoHandler(new CbmTrdGeoHandler())
  , fDigiChargeSpectrum(new TH1I("DigiChargeSpectrum", "DigiChargeSpectrum", 1e6, 0, 1.0e-3))
  , fLayerDummy(new TH2I("LayerDummy", "", 1200, -400, 400, 1000, -300, 300))
  , fmin(0)
  , fmax(13)
  , fModuleMap()
  , fModuleMapIt()
  , fModuleOccupancyMap()
  , fModuleOccupancyMapIt()
  , fModuleOccupancyMemoryMap()
  , fModuleOccupancyMemoryMapIt()
  , fLayerOccupancyMap()
  , fLayerOccupancyMapIt()
  , fLayerAverageOccupancyMap()
  , fLayerAverageOccupancyMapIt()
  , fTriggerThreshold(triggerThreshold)
  , fNeigbourReadout(true)
  , fPlotMergedResults(plotMergedResults)
  , fGeo(geo)
{
  fLayerDummy->SetXTitle("x-coordinate [cm]");
  fLayerDummy->SetYTitle("y-coordinate [cm]");
  fLayerDummy->GetXaxis()->SetLabelSize(0.02);
  fLayerDummy->GetYaxis()->SetLabelSize(0.02);
  fLayerDummy->GetZaxis()->SetLabelSize(0.02);
  fLayerDummy->GetXaxis()->SetTitleSize(0.02);
  fLayerDummy->GetXaxis()->SetTitleOffset(1.5);
  fLayerDummy->GetYaxis()->SetTitleSize(0.02);
  fLayerDummy->GetYaxis()->SetTitleOffset(2);
  fLayerDummy->GetZaxis()->SetTitleSize(0.02);
  fLayerDummy->GetZaxis()->SetTitleOffset(-2);
  fLayerDummy->SetContour(99);
  fLayerDummy->GetZaxis()->SetRangeUser(fmin, fmax);
  for (fModuleOccupancyMapIt = fModuleOccupancyMap.begin(); fModuleOccupancyMapIt != fModuleOccupancyMap.end();
       ++fModuleOccupancyMapIt) {
    //delete fModuleOccupancyMapIt->second;
  }
}

CbmTrdOccupancyQa::~CbmTrdOccupancyQa()
{

  FairRootManager* ioman = FairRootManager::Instance();
  ioman->Write();
  //CreateLayerView();
  //SaveHistos2File();
  fClusters->Delete();
  delete fClusters;

  if (fDigiPar) delete fDigiPar;
  //   if(fModuleInfo)
  //     delete fModuleInfo;
  for (fModuleMapIt = fModuleMap.begin(); fModuleMapIt != fModuleMap.end(); ++fModuleMapIt) {
    delete fModuleMapIt->second;
  }
  fModuleMap.clear();
  for (fLayerOccupancyMapIt = fLayerOccupancyMap.begin(); fLayerOccupancyMapIt != fLayerOccupancyMap.end();
       ++fLayerOccupancyMapIt) {
    delete fLayerOccupancyMapIt->second;
  }
  fLayerOccupancyMap.clear();
  for (fModuleOccupancyMapIt = fModuleOccupancyMap.begin(); fModuleOccupancyMapIt != fModuleOccupancyMap.end();
       ++fModuleOccupancyMapIt) {
    delete fModuleOccupancyMapIt->second;
  }
  fModuleOccupancyMap.clear();
  for (fModuleOccupancyMemoryMapIt = fModuleOccupancyMemoryMap.begin();
       fModuleOccupancyMemoryMapIt != fModuleOccupancyMemoryMap.end(); ++fModuleOccupancyMemoryMapIt) {
    delete fModuleOccupancyMemoryMapIt->second;
  }
  fModuleOccupancyMemoryMap.clear();
  delete fDigiChargeSpectrum;
  delete fLayerDummy;
}
void CbmTrdOccupancyQa::SetParContainers()
{
  cout << " * CbmTrdOccupancyQa * :: SetParContainers() " << endl;
  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  fDigiPar            = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));
  fGeoPar             = (CbmTrdParSetGeo*) (rtdb->getContainer("CbmTrdParSetGeo"));
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmTrdOccupancyQa::ReInit()
{
  cout << " * CbmTrdClusterizer * :: ReInit() " << endl;
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  fDigiPar            = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));
  fGeoPar             = (CbmTrdParSetGeo*) (rtdb->getContainer("CbmTrdParSetGeo"));
  return kSUCCESS;
}
// --------------------------------------------------------------------

// ---- Init ----------------------------------------------------------
InitStatus CbmTrdOccupancyQa::Init()
{
  cout << " * CbmTrdOccupancyQa * :: Init() " << endl;
  FairRootManager* ioman = FairRootManager::Instance();

  CbmDigiManager::Instance()->Init();
  if (!CbmDigiManager::Instance()->IsPresent(ECbmModuleId::kTrd)) LOG(fatal) << GetName() << "Missing Trd digi branch.";

  fClusters = (TClonesArray*) ioman->GetObject("TrdCluster");
  if (!fClusters) {
    cout << "-W CbmTrdHitDensityQa::Init: No TrdCluster array!" << endl;
    cout << "                             Task will be inactive" << endl;
    return kERROR;
  }
  fGeoHandler->Init();

  return kSUCCESS;
}
// --------------------------------------------------------------------
void CbmTrdOccupancyQa::SetNeighbourTrigger(Bool_t trigger) { fNeigbourReadout = trigger; }

// ---- Exec ----------------------------------------------------------
void CbmTrdOccupancyQa::Exec(Option_t*)
{
  printf("================CbmTrdOccupancyQa=====================\n");
  //fTriggerThreshold = CbmTrdClusterFinderFast::GetTriggerThreshold();
  printf("TriggerThreshold: %.2E\n", fTriggerThreshold);
  printf("NeigbourReadout:%i\n", Int_t(fNeigbourReadout));
  //  Bool_t debug = false;

  // /// Save old global file and folder pointer to avoid messing with FairRoot
  // TFile* oldFile     = gFile;
  // TDirectory* oldDir = gDirectory;
  // TFile *outFile = new TFile("data/CbmTrdOccupancyQa.root","UPDATE","output of CbmTrdOccupancyQa");

  TStopwatch timer;
  timer.Start();
  Int_t nEntries = CbmDigiManager::Instance()->GetNofDigis(ECbmModuleId::kTrd);
  Int_t digiCounter(0), digiTriggerCounter(0);
  TString title;
  /*
    fLayerDummy = new TH2I("LayerDummy","",1200,-600,600,1000,-500,500);
    fLayerDummy->SetXTitle("x-coordinate [cm]");
    fLayerDummy->SetYTitle("y-coordinate [cm]");
    //fDigiChargeSpectrum = outFile->Get<TH1I>("DigiChargeSpectrum");
    //if (!fDigiChargeSpectrum)
    fDigiChargeSpectrum = new TH1I("DigiChargeSpectrum","DigiChargeSpectrum",1e6,0,1e-3);
  */
  const CbmTrdDigi* digi = NULL;
  CbmTrdCluster* cluster = NULL;
  if (fNeigbourReadout == true) {
    Int_t nCluster = fClusters->GetEntriesFast();
    for (Int_t iCluster = 0; iCluster < nCluster; iCluster++) {
      //cout << iCluster << endl;
      cluster               = (CbmTrdCluster*) fClusters->At(iCluster);  //pointer to the acvit cluster
      Int_t nDigisInCluster = cluster->GetNofDigis();
      for (Int_t iDigi = 0; iDigi < nDigisInCluster; iDigi++) {
        digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(cluster->GetDigi(iDigi));
        //digi = (CbmTrdDigi*)fDigis->At(cluster->GetDigi(iDigi));
        if (digi->GetCharge() > fTriggerThreshold) digiTriggerCounter++;
        else
          digiCounter++;
        Int_t digiAddress   = digi->GetAddress();
        Int_t moduleAddress = CbmTrdAddress::GetModuleAddress(digiAddress);
        //	Int_t moduleId = CbmTrdAddress::GetModuleId(moduleAddress);
        Int_t Station = CbmTrdAddress::GetLayerId(moduleAddress) / 4 + 1;  //fGeoHandler->GetStation(moduleId);
        Int_t Layer   = CbmTrdAddress::GetLayerId(moduleAddress) % 4 + 1;  //fGeoHandler->GetLayer(moduleId);
        Int_t combiId = 10 * Station + Layer;
        CbmTrdParModDigi* fModuleInfo = (CbmTrdParModDigi*) fDigiPar->GetModulePar(moduleAddress);
        CbmTrdParModGeo* fModuleGeo   = (CbmTrdParModGeo*) fGeoPar->GetModulePar(moduleAddress);
        Int_t nRows                   = fModuleInfo->GetNofRows();
        Int_t nCols                   = fModuleInfo->GetNofColumns();
        if (fModuleOccupancyMap.find(moduleAddress) == fModuleOccupancyMap.end()) {
          title.Form("Module_%i", moduleAddress);
          //fModuleOccupancyMap[moduleAddress] = outFile->Get<TH2F>(title);
          //if (!fModuleOccupancyMap[moduleAddress])
          fModuleOccupancyMap[moduleAddress] =
            new TH2I(title, title, nCols, -0.5, nCols - 0.5, nRows, -0.5, nRows - 0.5);
          //fModuleOccupancyMap[moduleAddress]->Reset();
          title.Form("M_%i", moduleAddress);
          fModuleOccupancyMemoryMap[moduleAddress]   = new TH1F(title, title, 10000, 0, 100);
          fModuleMap[moduleAddress]                  = new OccupancyModule();
          fModuleMap[moduleAddress]->Station         = Station;
          fModuleMap[moduleAddress]->Layer           = Layer;
          fModuleMap[moduleAddress]->ModuleSizeX     = fModuleInfo->GetSizeX();
          fModuleMap[moduleAddress]->ModuleSizeY     = fModuleInfo->GetSizeY();
          fModuleMap[moduleAddress]->ModulePositionX = fModuleGeo->GetX();
          fModuleMap[moduleAddress]->ModulePositionY = fModuleGeo->GetY();
          fModuleMap[moduleAddress]->moduleAddress   = moduleAddress;
        }

        if (fLayerOccupancyMap.find(combiId) == fLayerOccupancyMap.end()) {
          title.Form("S%i_L%i", Station, Layer);
          printf("   new %s\n", title.Data());
          //fLayerOccupancyMap[combiId] = new TH2F(title,title,1200,-600,600,1000,-500,500);
          fLayerOccupancyMap[combiId] = new TCanvas(title, title, 1200, 1000);
          fLayerOccupancyMap[combiId]->cd();
          fLayerDummy->DrawCopy("colz");
          title.Form("Station%i_Layer%i", Station, Layer);
          printf("       new %s\n", title.Data());
          fLayerAverageOccupancyMap[combiId] = new TProfile(title, title, 1e6, 0, 1.0e-3);
          fLayerAverageOccupancyMap[combiId]->SetYTitle("Average layer occupancy [%]");
          fLayerAverageOccupancyMap[combiId]->SetXTitle("Trigger threshold");
        }

        Int_t iCol(CbmTrdAddress::GetColumnId(digiAddress)), iRow(CbmTrdAddress::GetRowId(digiAddress)),
          iSec(CbmTrdAddress::GetSectorId(digiAddress));
        iRow = fModuleInfo->GetModuleRow(iSec, iRow);
        Int_t ixBin(iCol + 1), iyBin(iRow + 1);
        if (fModuleOccupancyMap[moduleAddress]->GetBinContent(ixBin, iyBin) == 0)
          fModuleOccupancyMap[moduleAddress]->Fill(iCol, iRow);
      }
    }
  }
  else {
    for (Int_t iDigi = 0; iDigi < nEntries; iDigi++) {
      digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(iDigi);
      //      digi = (CbmTrdDigi*) fDigis->At(iDigi);

      Int_t moduleAddress = CbmTrdAddress::GetModuleAddress(digi->GetAddress());  //digi->GetDetId();
      Int_t Station       = CbmTrdAddress::GetLayerId(moduleAddress) / 4 + 1;     //fGeoHandler->GetStation(moduleId);
      Int_t Layer         = CbmTrdAddress::GetLayerId(moduleAddress) % 4 + 1;     //fGeoHandler->GetLayer(moduleId);
      Int_t combiId       = 10 * Station + Layer;
      //printf("Station %i Layer %i combiId %i\n", Station, Layer, combiId);
      CbmTrdParModDigi* fModuleInfo = (CbmTrdParModDigi*) fDigiPar->GetModulePar(moduleAddress);
      CbmTrdParModGeo* fModuleGeo   = (CbmTrdParModGeo*) fGeoPar->GetModulePar(moduleAddress);
      Int_t nRows                   = fModuleInfo->GetNofRows();
      Int_t nCols                   = fModuleInfo->GetNofColumns();
      fDigiChargeSpectrum->Fill(digi->GetCharge());
      if (digi->GetCharge() > fTriggerThreshold) {
        digiTriggerCounter++;
        digiCounter++;
        if (fModuleOccupancyMap.find(moduleAddress) == fModuleOccupancyMap.end()) {
          title.Form("Module_%i", moduleAddress);
          //fModuleOccupancyMap[moduleAddress] = outFile->Get<TH2F>(title);
          //if (!fModuleOccupancyMap[moduleAddress])
          fModuleOccupancyMap[moduleAddress] =
            new TH2I(title, title, nCols, -0.5, nCols - 0.5, nRows, -0.5, nRows - 0.5);
          //fModuleOccupancyMap[moduleAddress]->Reset();
          title.Form("M_%i", moduleAddress);
          fModuleOccupancyMemoryMap[moduleAddress]   = new TH1F(title, title, 10000, 0, 100);
          fModuleMap[moduleAddress]                  = new OccupancyModule();
          fModuleMap[moduleAddress]->Station         = Station;
          fModuleMap[moduleAddress]->Layer           = Layer;
          fModuleMap[moduleAddress]->ModuleSizeX     = fModuleInfo->GetSizeX();
          fModuleMap[moduleAddress]->ModuleSizeY     = fModuleInfo->GetSizeY();
          fModuleMap[moduleAddress]->ModulePositionX = fModuleGeo->GetX();
          fModuleMap[moduleAddress]->ModulePositionY = fModuleGeo->GetY();
          fModuleMap[moduleAddress]->moduleAddress   = moduleAddress;
        }
        if (fLayerOccupancyMap.find(combiId) == fLayerOccupancyMap.end()) {
          title.Form("S%i_L%i", Station, Layer);
          printf("   new %s\n", title.Data());
          //fLayerOccupancyMap[combiId] = new TH2F(title,title,1200,-600,600,1000,-500,500);
          fLayerOccupancyMap[combiId] = new TCanvas(title, title, 1200, 1000);
          fLayerOccupancyMap[combiId]->cd();
          fLayerDummy->DrawCopy("colz");
          title.Form("Station%i_Layer%i", Station, Layer);
          printf("       new %s\n", title.Data());
          fLayerAverageOccupancyMap[combiId] = new TProfile(title, title, 1e6, 0, 1.0e-3);
          fLayerAverageOccupancyMap[combiId]->SetYTitle("Average layer occupancy [%]");
          fLayerAverageOccupancyMap[combiId]->SetXTitle("Trigger threshold");
        }

        Int_t digiAddress(digi->GetAddress()), iCol(CbmTrdAddress::GetColumnId(digiAddress)),
          iRow(CbmTrdAddress::GetRowId(digiAddress)), iSec(CbmTrdAddress::GetSectorId(digiAddress));
        iRow = fModuleInfo->GetModuleRow(iSec, iRow);
        Int_t ixBin(iCol + 1), iyBin(iRow + 1);
        if (fModuleOccupancyMap[moduleAddress]->GetBinContent(ixBin, iyBin) == 0)
          fModuleOccupancyMap[moduleAddress]->Fill(iCol, iRow);
        /*
	  if(fNeigbourReadout) {
	  if (nCols > nRows) { // Neigbour readout does not depend on a signal on the neighbouring channel
	  if (ixBin > 1 && fModuleOccupancyMap[moduleAddress]->GetBinContent(ixBin-1,iyBin) == 0){//left
	  fModuleOccupancyMap[moduleAddress]->Fill(iCol-1,iRow);
	  digiCounter++;
	  }
	  if (ixBin < nCols && fModuleOccupancyMap[moduleAddress]->GetBinContent(ixBin+1,iyBin) == 0){//right
	  fModuleOccupancyMap[moduleAddress]->Fill(iCol+1,iRow);
	  digiCounter++;
	  }
	  }
	  else {
	  if (iyBin < nRows && fModuleOccupancyMap[moduleAddress]->GetBinContent(ixBin,iyBin+1) == 0){//upper
	  fModuleOccupancyMap[moduleAddress]->Fill(iCol,iRow+1);
	  digiCounter++;
	  }
	  if (iyBin > 1 && fModuleOccupancyMap[moduleAddress]->GetBinContent(ixBin,iyBin-1) == 0){//lower
	  fModuleOccupancyMap[moduleAddress]->Fill(iCol,iRow-1);
	  digiCounter++;
	  }
	  }
	  }
	*/
        //fModuleOccupancyMap[moduleAddress]->SetBinContent(digi->GetCol()+1, digi->GetRow()+1, 1);
      }
    }
  }
  printf("%6i primary triggered digis\n%6i including neigbouring triggered digis\n", digiTriggerCounter, digiCounter);
  CopyEvent2MemoryMap();
  //CreateLayerView();
  //SaveHistos2File();
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();

  printf("\n\n******************** Reading Test  **********************\n");
  printf("   RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);
  printf("*********************************************************\n\n");
}
// ---- FinishTask-----------------------------------------------------
void CbmTrdOccupancyQa::FinishEvent()
{
  if (fClusters) {
    fClusters->Clear("C");
    fClusters->Delete();
  }
  for (fModuleOccupancyMapIt = fModuleOccupancyMap.begin(); fModuleOccupancyMapIt != fModuleOccupancyMap.end();
       ++fModuleOccupancyMapIt) {
    fModuleOccupancyMapIt->second->Reset();
  }
}
void CbmTrdOccupancyQa::FinishTask()
{
  CreateLayerView();
  SaveHistos2File();
}
// ---- Register ------------------------------------------------------
void CbmTrdOccupancyQa::Register()
{
  //FairRootManager::Instance()->Register("TrdDigi","Trd Digi", fDigiCollection, IsOutputBranchPersistent("TrdDigi"));
  //FairRootManager::Instance()->Register("TrdDigiMatch","Trd Digi Match", fDigiMatchCollection, IsOutputBranchPersistent("TrdDigiMatch"));
}
// --------------------------------------------------------------------
void CbmTrdOccupancyQa::CopyEvent2MemoryMap()
{
  for (fModuleOccupancyMapIt = fModuleOccupancyMap.begin(); fModuleOccupancyMapIt != fModuleOccupancyMap.end();
       ++fModuleOccupancyMapIt) {
    fModuleOccupancyMemoryMap[fModuleOccupancyMapIt->first]->Fill(
      Float_t(fModuleOccupancyMapIt->second->Integral())
      / Float_t(fModuleOccupancyMapIt->second->GetNbinsX() * fModuleOccupancyMapIt->second->GetNbinsY()) * 100.);
    fLayerAverageOccupancyMap[10 * (fModuleMap[fModuleOccupancyMapIt->first]->Station)
                              + (fModuleMap[fModuleOccupancyMapIt->first]->Layer)]
      ->Fill(fTriggerThreshold,
             Float_t(fModuleOccupancyMapIt->second->Integral())
               / Float_t(fModuleOccupancyMapIt->second->GetNbinsX() * fModuleOccupancyMapIt->second->GetNbinsY())
               * 100.);
  }
}
void CbmTrdOccupancyQa::SwitchToMergedFile()
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TString filename = "data/result.root";
  TFile* Target    = new TFile(filename, "READ");
  if (Target) {
    for (fModuleOccupancyMapIt = fModuleOccupancyMap.begin(); fModuleOccupancyMapIt != fModuleOccupancyMap.end();
         ++fModuleOccupancyMapIt) {
      TString histName = fModuleOccupancyMapIt->second->GetName();
      std::cout << histName << std::endl;
      ;
      fModuleOccupancyMap[fModuleOccupancyMapIt->first] =
        static_cast<TH2I*>(Target->Get<TH2I>("TrdOccupancy/Module/" + histName)->Clone(histName + "_result"));
    }
  }
  else {
    cout << "No merged data/result.root found" << endl;
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}
void CbmTrdOccupancyQa::CreateLayerView()
{
  if (fPlotMergedResults) SwitchToMergedFile();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile   = gFile;
  TString origpath = gDirectory->GetPath();
  printf("\n%s\n", origpath.Data());

  TString newpath = origpath;
  newpath.ReplaceAll("eds", "oc_qa");
  newpath.ReplaceAll(":/", "");

  TFile* tempFile = new TFile(newpath, "recreate");
  gDirectory->pwd();
  cout << "CreateLayerView" << endl;
  Bool_t debug = false;
  TString title;
  for (fModuleOccupancyMemoryMapIt = fModuleOccupancyMemoryMap.begin();
       fModuleOccupancyMemoryMapIt != fModuleOccupancyMemoryMap.end(); ++fModuleOccupancyMemoryMapIt) {
    /*
      TBox *b = new TBox(
      fModuleMap[fModuleOccupancyMapIt->first]->ModulePositionX - fModuleMap[fModuleOccupancyMapIt->first]->ModuleSizeX,
      fModuleMap[fModuleOccupancyMapIt->first]->ModulePositionY - fModuleMap[fModuleOccupancyMapIt->first]->ModuleSizeY,
      fModuleMap[fModuleOccupancyMapIt->first]->ModulePositionX + fModuleMap[fModuleOccupancyMapIt->first]->ModuleSizeX,
      fModuleMap[fModuleOccupancyMapIt->first]->ModulePositionY + fModuleMap[fModuleOccupancyMapIt->first]->ModuleSizeY
      );
    */
    /*
      Float_t occupancy = Float_t(fModuleOccupancyMapIt->second->Integral()) / Float_t(fModuleOccupancyMapIt->second->GetNbinsX() * fModuleOccupancyMapIt->second->GetNbinsY()) * 100.;
      if (debug)
      printf("M: %i O:%6.2f  ",fModuleOccupancyMapIt->first,occupancy);
      fModuleOccupancyMemoryMap[fModuleOccupancyMapIt->first]->Fill(occupancy);
    */
    Float_t occupancy  = fModuleOccupancyMemoryMapIt->second->GetMean(1);
    Float_t occupancyE = fModuleOccupancyMemoryMapIt->second->GetRMS(1);
    if (debug) printf(" <O>:%6.2f  ", occupancy);
    title.Form("%.1f#pm%.1f%%", occupancy, occupancyE);
    if (debug) printf("%s\n", title.Data());
    TPaveText* text = new TPaveText(fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModulePositionX
                                      - fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModuleSizeX,
                                    fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModulePositionY
                                      - fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModuleSizeY,
                                    fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModulePositionX
                                      + fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModuleSizeX,
                                    fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModulePositionY
                                      + fModuleMap[fModuleOccupancyMemoryMapIt->first]->ModuleSizeY);
    text->SetFillStyle(1001);
    text->SetLineColor(1);

    //Double_t min(0), max(12);
    std::vector<Int_t> fColors;
    std::vector<Double_t> fZLevel;
    for (Int_t i = 0; i < TColor::GetNumberOfColors(); i++) {
      fColors.push_back(TColor::GetColorPalette(i));
      //fZLevel.push_back(min + TMath::Power(10, TMath::Log10(max) / TColor::GetNumberOfColors() * i));// log scale
      fZLevel.push_back(fmin + (fmax / TColor::GetNumberOfColors() * i));  // lin scale
    }
    //    Int_t color(0), j(0);
    Int_t j(0);
    while ((occupancy > fZLevel[j]) && (j < (Int_t) fZLevel.size())) {
      //printf ("              %i<%i %i    %E <= %E\n",j,(Int_t)fZLevel.size(),fColors[j], rate, fZLevel[j]);
      j++;
    }
    text->SetFillColor(fColors[j]);
    if (j >= (Int_t) fZLevel.size()) text->SetFillColor(2);

    if (fColors[j] < 65) text->SetTextColor(kWhite);
    /*
      Int_t maxCol = 4;
      if (occupancy > 0 && occupancy <= 2.5)
      text->SetFillColor(kViolet
      //+Int_t((occupancy-0)/6.*maxCol)
      );
      if (occupancy > 2.5 && occupancy <= 5){
      text->SetFillColor(kAzure
      //+Int_t((occupancy-6)/6.*maxCol)
      );
      text->SetTextColor(kWhite);
      }
      if (occupancy > 5 && occupancy <= 7.5)
      text->SetFillColor(kTeal
      //+Int_t((occupancy-12)/6.*maxCol)
      );
      if (occupancy > 7.5 && occupancy <= 10)
      text->SetFillColor(kSpring
      //+Int_t((occupancy-18)/6.*maxCol)
      );
      if (occupancy > 10 && occupancy <= 12.5)
      text->SetFillColor(kYellow
      //+Int_t((occupancy-24)/6.*maxCol)
      );
      if (occupancy > 12.5 && occupancy <= 15)
      text->SetFillColor(kOrange
      //+Int_t((occupancy-24)/6.*maxCol)
      );
      if (occupancy > 15)
      text->SetFillColor(kRed);
    */
    /*
      if (occupancy > 0 && occupancy <= 5)
      text->SetFillColor(kViolet
      //+Int_t((occupancy-0)/6.*maxCol)
      );
      if (occupancy > 5 && occupancy <= 10){
      text->SetFillColor(kAzure
      //+Int_t((occupancy-6)/6.*maxCol)
      );
      text->SetTextColor(kWhite);
      }
      if (occupancy > 10 && occupancy <= 15)
      text->SetFillColor(kTeal
      //+Int_t((occupancy-12)/6.*maxCol)
      );
      if (occupancy > 15 && occupancy <= 20)
      text->SetFillColor(kSpring
      //+Int_t((occupancy-18)/6.*maxCol)
      );
      if (occupancy > 20 && occupancy <= 25)
      text->SetFillColor(kYellow
      //+Int_t((occupancy-24)/6.*maxCol)
      );
      if (occupancy > 25 && occupancy <= 30)
      text->SetFillColor(kOrange
      //+Int_t((occupancy-24)/6.*maxCol)
      );
      if (occupancy > 30)
      text->SetFillColor(kRed);
    */
    fLayerOccupancyMap[fModuleMap[fModuleOccupancyMemoryMapIt->first]->Station * 10
                       + fModuleMap[fModuleOccupancyMemoryMapIt->first]->Layer]
      ->cd();
    //b->Draw("same");
    //text->SetFillStyle(0);
    text->AddText(title);
    text->Draw("same");
  }
  gDirectory->pwd();

  TDirectoryFile* TopLayerDir;
  gDirectory->GetObject("Occupancy", TopLayerDir);
  if (!TopLayerDir) {
    std::cout << "unknown Directory Occupancy" << std::endl;
    gDirectory->mkdir("Occupancy");
    gDirectory->GetObject("Occupancy", TopLayerDir);
  }
  TopLayerDir->cd();
  gDirectory->pwd();
  TDirectoryFile* Triggerdir;
  title.Form("TH%.2E", fTriggerThreshold);
  gDirectory->GetObject(title, Triggerdir);
  if (!Triggerdir) {
    std::cout << "unknown Directory Trigger Dir" << std::endl;
    gDirectory->mkdir(title);
    gDirectory->GetObject(title, Triggerdir);
  }
  Triggerdir->cd();
  gDirectory->pwd();
  for (fLayerOccupancyMapIt = fLayerOccupancyMap.begin(); fLayerOccupancyMapIt != fLayerOccupancyMap.end();
       ++fLayerOccupancyMapIt) {
    fLayerOccupancyMapIt->second->Write("", TObject::kOverwrite);
    title.Form("pics/Occupancy_%.2E_%s_%s.pdf", fTriggerThreshold,
               TString(fLayerOccupancyMapIt->second->GetTitle()).Data(), fGeo.Data());
    fLayerOccupancyMapIt->second->SaveAs(title);
    title.Form("pics/Occupancy_%.2E_%s_%s.png", fTriggerThreshold,
               TString(fLayerOccupancyMapIt->second->GetTitle()).Data(), fGeo.Data());
    fLayerOccupancyMapIt->second->SaveAs(title);
  }
  gDirectory->Cd("..");
  TCanvas* c    = new TCanvas("c", "c", 800, 600);
  Int_t counter = 0;
  for (fLayerAverageOccupancyMapIt = fLayerAverageOccupancyMap.begin();
       fLayerAverageOccupancyMapIt != fLayerAverageOccupancyMap.end(); ++fLayerAverageOccupancyMapIt) {
    fLayerAverageOccupancyMapIt->second->Write("", TObject::kOverwrite);

    counter++;
    fLayerAverageOccupancyMapIt->second->SetLineColor(counter);
    fLayerAverageOccupancyMapIt->second->SetMarkerColor(counter);
    fLayerAverageOccupancyMapIt->second->SetMarkerStyle(20);
    c->cd()->SetLogy(1);
    c->cd()->SetLogx(1);
    if (fLayerAverageOccupancyMapIt == fLayerAverageOccupancyMap.begin()) {
      fLayerAverageOccupancyMapIt->second->GetXaxis()->SetRangeUser(1.0e-10, 1.0e-3);
      fLayerAverageOccupancyMapIt->second->GetYaxis()->SetRangeUser(0.1, 100);
      fLayerAverageOccupancyMapIt->second->DrawCopy();
    }
    else
      fLayerAverageOccupancyMapIt->second->DrawCopy("same");
  }
  title.Form("pics/Occupancy_%.2E_%s.pdf", fTriggerThreshold, fGeo.Data());
  c->SaveAs(title);
  title.Form("pics/Occupancy_%.2E_%s.png", fTriggerThreshold, fGeo.Data());
  c->SaveAs(title);
  gDirectory->Cd("..");
  c->Close();

  tempFile->Close();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile = oldFile;
  gDirectory->Cd(origpath);
  gDirectory->pwd();
}
void CbmTrdOccupancyQa::SetNeighbourReadout(Bool_t neighbourReadout) { fNeigbourReadout = neighbourReadout; }
void CbmTrdOccupancyQa::SetTriggerThreshold(Double_t triggerthreshold) { fTriggerThreshold = triggerthreshold; }
void CbmTrdOccupancyQa::SaveHistos2File()
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile   = gFile;
  TString origpath = gDirectory->GetPath();
  printf("\n%s\n", origpath.Data());

  TString newpath = origpath;
  newpath.ReplaceAll("eds", "oc_qa");
  newpath.ReplaceAll(":/", "");

  TFile* tempFile = new TFile(newpath, "Update");
  gDirectory->pwd();

  cout << "SaveHistos2File" << endl;
  TString title;
  /*
    TFile *outFile = new TFile("data/CbmTrdOccupancyQa.root","UPDATE","output of CbmTrdOccupancyQa");
    outFile->cd();
  */
  gDirectory->pwd();
  TDirectoryFile* Basedir;
  gDirectory->GetObject("TrdOccupancy", Basedir);
  if (!Basedir) {
    gDirectory->mkdir("TrdOccupancy");
    std::cout << "unknown Directory Basedir" << std::endl;
    gDirectory->GetObject("TrdOccupancy", Basedir);
  }
  Basedir->cd();

  fDigiChargeSpectrum->Write("", TObject::kOverwrite);
  /*
  for (fLayerOccupancyMapIt = fLayerOccupancyMap.begin();
       fLayerOccupancyMapIt != fLayerOccupancyMap.end(); ++fLayerOccupancyMapIt) {
    fLayerOccupancyMapIt->second->Write("", TObject::kOverwrite);
  }
  */
  /*
    if (!gDirectory->Cd("Module2D"))
    gDirectory->mkdir("Module2D");
    gDirectory->Cd("Module2D");
    for (fModuleOccupancyMapIt = fModuleOccupancyMap.begin();
    fModuleOccupancyMapIt != fModuleOccupancyMap.end(); ++fModuleOccupancyMapIt) {
    title.Form("Occupancy: %.2f%%",Float_t(fModuleOccupancyMapIt->second->Integral()) / Float_t(fModuleOccupancyMapIt->second->GetNbinsX() * fModuleOccupancyMapIt->second->GetNbinsY()) * 100.);
    fModuleOccupancyMapIt->second->SetTitle(title);
    fModuleOccupancyMapIt->second->Write("", TObject::kOverwrite);
    }
    gDirectory->Cd("..");
  */
  TDirectoryFile* Moduledir;
  gDirectory->GetObject("Module1D", Moduledir);
  if (!Moduledir) {
    gDirectory->mkdir("Module1D");
    std::cout << "unknown Directory Module1D" << std::endl;
    gDirectory->GetObject("Module1D", Moduledir);
  }
  Moduledir->cd();
  gDirectory->pwd();

  for (fModuleOccupancyMemoryMapIt = fModuleOccupancyMemoryMap.begin();
       fModuleOccupancyMemoryMapIt != fModuleOccupancyMemoryMap.end(); ++fModuleOccupancyMemoryMapIt) {
    fModuleOccupancyMemoryMapIt->second->Write("", TObject::kOverwrite);
  }
  gDirectory->Cd("..");
  gDirectory->Cd("..");
  //outFile->Close();

  tempFile->Close();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile = oldFile;
  gDirectory->Cd(origpath);
  gDirectory->pwd();
}
ClassImp(CbmTrdOccupancyQa)
