/**
 * \file CbmTrdParManager.cxx
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \date 06/06/2008
 */
#include "CbmTrdParManager.h"

#include "CbmTrdGeoHandler.h"    // for CbmTrdGeoHandler
#include "CbmTrdPads.h"          // for fst1_pad_type, fst1_sect_count
#include "CbmTrdParAsic.h"       // for CbmTrdParAsic, CbmTrdParAsic::kCriId...
#include "CbmTrdParFasp.h"       // for CbmTrdParFasp
#include "CbmTrdParModDigi.h"    // for CbmTrdParModDigi
#include "CbmTrdParModGain.h"    // for CbmTrdParModGain
#include "CbmTrdParModGas.h"     // for CbmTrdParModGas
#include "CbmTrdParSet.h"        // for CbmTrdParSet
#include "CbmTrdParSetAsic.h"    // for CbmTrdParSetAsic
#include "CbmTrdParSetDigi.h"    // for CbmTrdParSetDigi
#include "CbmTrdParSetGain.h"    // for CbmTrdParSetGain
#include "CbmTrdParSetGas.h"     // for CbmTrdParSetGas
#include "CbmTrdParSpadic.h"     // for CbmTrdParSpadic

#include <FairParAsciiFileIo.h>  // for FairParAsciiFileIo
#include <FairParRootFileIo.h>   // for FairParRootFileIo
#include <FairRunAna.h>          // for FairRunAna
#include <FairRuntimeDb.h>       // for FairRuntimeDb
#include <FairTask.h>            // for FairTask, InitStatus, kSUCCESS

#include <TArrayD.h>             // for TArrayD
#include <TFile.h>               // for TFile
#include <TGeoManager.h>         // for TGeoManager, gGeoManager
#include <TGeoNode.h>            // for TGeoNode
#include <TList.h>               // for TList
#include <TObjArray.h>           // for TObjArray
#include <TObject.h>             // for TObject
#include <TRandom.h>             // for TRandom, gRandom

#include <stdio.h>               // for printf
#include <stdlib.h>              // for getenv
#include <vector>                // for vector

CbmTrdParManager::CbmTrdParManager(Bool_t fasp)
  :FairTask("TrdParManager")
  ,fMaxSectors(0)
  ,fFASP(fasp)
//   fModuleMap(), 
  ,fAsicPar(nullptr)
  ,fDigiPar(nullptr)
  ,fGasPar(nullptr)
  ,fGainPar(nullptr)
  ,fGeoHandler(new CbmTrdGeoHandler())
{
  // Get the maximum number of sectors. All arrays will have this number of entries.
  fMaxSectors = fst1_sect_count;
}

CbmTrdParManager::~CbmTrdParManager()
{
}

void CbmTrdParManager::SetParContainers()
{
  FairRuntimeDb* rtdb = FairRunAna::Instance()->GetRuntimeDb();
  fAsicPar = (CbmTrdParSetAsic*)(rtdb->getContainer("CbmTrdParSetAsic"));
  fDigiPar = (CbmTrdParSetDigi*)(rtdb->getContainer("CbmTrdParSetDigi"));
  fGasPar = (CbmTrdParSetGas*)(rtdb->getContainer("CbmTrdParSetGas"));
  fGainPar = (CbmTrdParSetGain*)(rtdb->getContainer("CbmTrdParSetGain"));
}

InitStatus CbmTrdParManager::Init()
{
  // The geometry structure is treelike with cave as
  // the top node. For the TRD there are keeping volume
  // trd_vXXy_1 which is only container for the different layers.
  // The trd layer is again only a container for all volumes of this layer.
  // Loop over all nodes below the top node (cave). If one of
  // the nodes contains a string trd it must be TRD detector.
  // Now loop over the layers and
  // then over all modules of the layer to extract in the end
  // all active regions (gas) of the complete TRD. For each
  // of the gas volumes get the information about size and
  // position from the geomanager and the sizes of the sectors
  // and pads from the definitions in CbmTrdPads. This info
  // is then stored in a CbmTrdModuleSim object for each of the
  // TRD modules.

  TGeoNode* topNode = gGeoManager->GetTopNode();
  TObjArray* nodes = topNode->GetNodes();
  for (Int_t iNode = 0; iNode < nodes->GetEntriesFast(); iNode++) {
    TGeoNode* node = static_cast<TGeoNode*>(nodes->At(iNode));
    if (!TString(node->GetName()).Contains("trd")) continue; // trd_vXXy top node, e.g. trd_v13a, trd_v14b
    TGeoNode* station = node;
    TObjArray* layers = station->GetNodes();
    for (Int_t iLayer = 0; iLayer < layers->GetEntriesFast(); iLayer++) {
        TGeoNode* layer = static_cast<TGeoNode*>(layers->At(iLayer));
        if (!TString(layer->GetName()).Contains("layer")) continue; // only layers

        TObjArray* modules = layer->GetNodes();
        for (Int_t iModule = 0; iModule < modules->GetEntriesFast(); iModule++) {
          TGeoNode* module = static_cast<TGeoNode*>(modules->At(iModule));
          TObjArray* parts = module->GetNodes();
          for (Int_t iPart = 0; iPart < parts->GetEntriesFast(); iPart++) {
              TGeoNode* part = static_cast<TGeoNode*>(parts->At(iPart));
              if (!TString(part->GetName()).BeginsWith("gas_")) continue; // only active gas volume

              // Put together the full path to the interesting volume, which
              // is needed to navigate with the geomanager to this volume.
              // Extract the geometry information (size, global position)
              // from this volume.
              TString path = TString("/") + topNode->GetName() + "/" + station->GetName() + "/"
                + layer->GetName() + "/" + module->GetName() + "/" + part->GetName();

              CreateModuleParameters(path);
          }
        }
    }
  }

  return kSUCCESS;
}

void CbmTrdParManager::Finish()
{
  FairRuntimeDb* rtdb = FairRunAna::Instance()->GetRuntimeDb();
  //   fDigiPar = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));
  //   fDigiPar->print();
  fAsicPar = (CbmTrdParSetAsic*) (rtdb->getContainer("CbmTrdParSetAsic"));
  fAsicPar->Print();
}

void CbmTrdParManager::Exec(
      Option_t *)
{
}


void CbmTrdParManager::CreateModuleParameters(const TString& path)
{
  /**   
   * Create TRD module parameters. Add triangular support (Alex Bercuci/21.11.2017)
  */   
 
  Int_t moduleAddress = fGeoHandler->GetModuleAddress(path);
  Int_t orientation   = fGeoHandler->GetModuleOrientation(path);

  Double_t sizeX = fGeoHandler->GetSizeX(path);
  Double_t sizeY = fGeoHandler->GetSizeY(path);
  Double_t sizeZ = fGeoHandler->GetSizeZ(path);
  Double_t x = fGeoHandler->GetX(path);
  Double_t y = fGeoHandler->GetY(path);
  Double_t z = fGeoHandler->GetZ(path);

  TArrayD sectorSizeX(fMaxSectors);
  TArrayD sectorSizeY(fMaxSectors);
  TArrayD padSizeX(fMaxSectors);
  TArrayD padSizeY(fMaxSectors);
  Int_t moduleType = fGeoHandler->GetModuleType(path);

  printf("\nCbmTrdParManager::CreateModuleParameters(%s) type[%d]\n", path.Data(), moduleType);
   for (Int_t i = 0; i < fst1_sect_count; i++) {
      sectorSizeX.AddAt(fst1_pad_type[moduleType - 1][i][0], i);
      sectorSizeY.AddAt(fst1_pad_type[moduleType - 1][i][1], i);
      padSizeX.AddAt(fst1_pad_type[moduleType - 1][i][2], i);
      padSizeY.AddAt(fst1_pad_type[moduleType - 1][i][3], i);
      printf("  sec[%d] dx[%5.2f] dy[%5.2f] px[%5.2f] py[%5.2f]\n", i, sectorSizeX[i], sectorSizeY[i], padSizeX[i],padSizeY[i] );
   }

  // Orientation of the detector layers
  // Odd  layers (1,3,5..) have resolution in x-direction (isRotated == 0) - vertical pads
  // Even layers (2,4,6..) have resolution in y-direction (isRotated == 1) - horizontal pads
  //   Int_t layerNr = CbmTrdAddress::GetLayerId(moduleAddress) + 1;
  //   Int_t isRotated = fGeoHandler->GetModuleOrientation(path);
  //      printf("layer %02d %d isRotated\n", layerNr, isRotated);   // check, if even layers are isRotated == 1
  //   if( (isRotated%2) == 1 ) {  // flip pads for even layers
  //      Double_t copybuf;
  //      for (Int_t i = 0; i < fMaxSectors; i++) {
  //         copybuf = padSizeX.At(i);
  //         padSizeX.AddAt(padSizeY.At(i), i);
  //         padSizeY.AddAt(copybuf, i);
  //         copybuf = sectorSizeX.At(i);
  //         sectorSizeX.AddAt(sectorSizeY.At(i), i);
  //         sectorSizeY.AddAt(copybuf, i);
  //      }
  //   }

  // Create new digi par for this module. 
  CbmTrdParModDigi *digi = new CbmTrdParModDigi(x, y, z, sizeX, sizeY, sizeZ, fMaxSectors, orientation, sectorSizeX, sectorSizeY, padSizeX, padSizeY);
  if(moduleType>=9) { // for the Bucharest inner detector special anode wire geometry
    digi->SetAnodeWireToPadPlaneDistance(0.4);
    digi->SetAnodeWireOffset(0.);
    digi->SetAnodeWireSpacing(0.3);
  }
  digi->SetModuleId(moduleAddress);  
  digi->Print();
  fDigiPar->addParam(digi);
  
  // Create new asic par for this module
  CbmTrdParSetAsic *asics(nullptr);
  // if(moduleType>=9 && fFASP){ // I do not think this check is required, actually I think it creates a bug in parameter creation
  if(moduleType>=9){
    asics = new CbmTrdParSetAsic("TrdParModFasp", Form("Fasp set for Module %d", moduleAddress)/*, GetContext()*/);
    asics->SetAsicType(moduleType);
    Double_t par[6]; 
          par[1] = 14;
          par[4] =  4.181e-6;
    Int_t iasic(0),
          ncol(digi->GetNofColumns()), 
          asicAddress, chAddress;
    CbmTrdParFasp *asic(nullptr);
    for(Int_t r(0); r<digi->GetNofRows(); r++){
      for(Int_t c(0); c<ncol; c++){
        if(c%8==0){
          if(asic) asics->SetAsicPar(asicAddress, asic);
          asicAddress = moduleAddress*1000+iasic;
          asic = new CbmTrdParFasp(asicAddress);
          iasic++;
        }
        for(Int_t ipair(0); ipair<2; ipair++){
          par[0] = gRandom->Gaus(300, 4);
          par[2] = gRandom->Gaus(600, 40);
          par[3] = gRandom->Gaus(2580, 10);
          chAddress = 2*(r*ncol+c)+ipair;
          par[5] = ipair;
          asic->SetChannelAddress(chAddress);
          Int_t chIdFasp = asic->QueryChannel(chAddress);
          asic->SetCalibParameters(chIdFasp, par);
        }
      }
    }
    if(asic) asics->SetAsicPar(asicAddress, asic);
  } 
  else {
    asics = new CbmTrdParSetAsic("TrdParModSpadic", Form("Spadic set for Module %d", moduleAddress));     asics->SetAsicType(moduleType);
    CbmTrdParSpadic *asic(nullptr);
    Int_t nAsicsPerCrob = CbmTrdParSpadic::GetNasicsPerCrob(moduleType);
    Int_t cRobCounter(0);
    Int_t cRobId(0);
    Int_t eLinkId(98);  // default for undefined, since 98 should never be in use
    Int_t nModuleColumns(digi->GetNofColumns());
    Int_t nModuleRows(digi->GetNofRows());
    
    Int_t nAsicsAlongColumns(-1);
    
    std::vector<Int_t> chAddressesVec;
    for (Int_t iAsic = 0; iAsic < CbmTrdParSpadic::GetNasicsOnModule(moduleType); iAsic++)
    {
      asic = new CbmTrdParSpadic( 1000 *  moduleAddress + iAsic); // nTh-asic + module address define asicAddress
      eLinkId = (iAsic % nAsicsPerCrob) * 2;
      cRobCounter = iAsic / nAsicsPerCrob;
      Int_t nAsicChannels(asic->GetNchannels());
      nAsicsAlongColumns = nModuleColumns < nModuleRows ? nModuleRows / 2 : nModuleColumns / (nAsicChannels/2) ;
      Int_t nThAsicRow(iAsic / nAsicsAlongColumns);
      Int_t nThAsicColumn(iAsic % nAsicsAlongColumns);
      // TODO check if rotation is handled correctly

      chAddressesVec.clear();
      chAddressesVec.resize(nAsicChannels);
      
      Int_t iAsicChannel(0);
      for (auto channelAddress : chAddressesVec)
      {
        channelAddress = asic->GetElinkChannel(iAsicChannel);
        if((iAsicChannel % 2 == 0)) channelAddress += nModuleColumns; // one asic is split over two rows thus, with even channels in the bottom row, thus there address is placed in the next column 
        
        channelAddress += nThAsicColumn * nAsicChannels / 2; // one asic is split over two rows
        channelAddress += nThAsicRow    * nModuleColumns * 2; // one asic is split over two rows
        chAddressesVec.at(iAsicChannel) = channelAddress;
        iAsicChannel++;
      }

      asic->SetChannelAddresses(chAddressesVec);
      asic->SetComponentId(moduleAddress * CbmTrdParAsic::kCriIdPosition + cRobId * CbmTrdParAsic::kCrobIdPosition + cRobCounter * CbmTrdParAsic::kCrobNrPosition + eLinkId * CbmTrdParAsic::kElinkIdPosition); // Remark: This is and will not be the correct componentId (CRI/AFCK Id). However, every asic connected to the nTh cRob of a given module will have the same componentId. Thus, this makes it easier to change to the correct Id for all relevant asics.
      asics->SetAsicPar(asic->GetAddress(), asic);
    }
  }
  asics->Print();
  fAsicPar->AddParameters(asics);

  // Create new gas par for this module
  CbmTrdParModGas *gas(nullptr); 
  if(moduleType>=9){ 
    gas = new CbmTrdParModGas(Form("Module/%d/Ua/%d/Ud/%d/Gas/Xe", moduleAddress, 1900, 500));
    gas->SetDetType(1);
    gas->SetPidType(1);
  } else gas = new CbmTrdParModGas(Form("Module/%d/Ua/%d/Ud/%d/Gas/Xe", moduleAddress, 1600, 500));
  gas->Print();
  fGasPar->addParam(gas);

  // Create new gain par for this module
  CbmTrdParModGain *gain(nullptr); 
  if(moduleType==9) gain = new CbmTrdParModGain();
  else gain = new CbmTrdParModGain();
  gain->SetModuleId(moduleAddress);
  gain->Print();
  fGainPar->addParam(gain);
}

// ---- CreateParFilesFromGeometry ------------------------------------------------
bool CbmTrdParManager::CreateParFilesFromGeometry(bool createRootFileOutput, TString outDir)
{
  // This function creates the trd parameter files based on a given geometry file, which has to be passed in a macro to the FairRunAna instance. Such a geometry file is produced by FairRunSim bases on the trd geometry input.
  
  if(!createRootFileOutput) return CreateParFilesFromGeometry(outDir);

  FairRunAna *run = FairRunAna::Instance();
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  SetParContainers();
  
  TString inputDirectory = run->GetGeoFile()->GetName();
  inputDirectory.Resize((inputDirectory.Last('/')+1));
  TString geoName = run->GetGeoFile()->GetName();
  geoName.ReplaceAll(inputDirectory.Data(), "");
  geoName.ReplaceAll("geofile_", "");
  geoName.ReplaceAll(".root", "");

  if(outDir.IsNull())
  {
    outDir = Form("%s/../src/parameters/trd", getenv("CBM_ROOT"));
  }

  TList *containerList = rtdb->getListOfContainers();

  TString currentPar = "";
  CbmTrdParSet *currentContainer = nullptr;
  FairParRootFileIo parOut;
  parOut.open(Form("%s/%s.par.root", outDir.Data(), geoName.Data()), "RECREATE");
  rtdb->setOutput(&parOut);

  for (auto iContainerIt : *containerList)   
  {
    currentPar = iContainerIt->GetName();
    if(!currentPar.Contains("CbmTrd")) continue; // make sure that we only edit Trd container
    currentContainer = (CbmTrdParSet*) iContainerIt;
    currentContainer->setChanged();
    currentContainer->setInputVersion(0, 1);
  }
  rtdb->saveOutput();
  rtdb->closeOutput();

  return true; // check if rtdb->writeContainers() could be used to run a bool check and return that bool instead of saveOutput void
}

// ---- private - CreateParFilesFromGeometry --------------------------------------
bool CbmTrdParManager::CreateParFilesFromGeometry(TString outDir)
{
  // This function creates the trd parameter files based on a given geometry file, which has to be passed in a macro to the FairRunAna instance. Such a geometry file is produced by FairRunSim bases on the trd geometry input.
  
  FairRunAna *run = FairRunAna::Instance();
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  SetParContainers();
  
  TString inputDirectory = run->GetGeoFile()->GetName();
  inputDirectory.Resize((inputDirectory.Last('/')+1));
  TString geoName = run->GetGeoFile()->GetName();
  geoName.ReplaceAll(inputDirectory.Data(), "");
  geoName.ReplaceAll("geofile_", "");
  geoName.ReplaceAll(".root", "");

  if(outDir.IsNull())
  {
    outDir = Form("%s/../src/parameters/trd", getenv("CBM_ROOT"));
  }

  TList *containerList = rtdb->getListOfContainers();

  TString currentPar = "";
  TString currentFile = "";
  CbmTrdParSet *currentContainer = nullptr;

  for (auto iContainerIt : *containerList)   
  {
    currentPar = iContainerIt->GetName();
    if(!currentPar.Contains("CbmTrd")) continue; // make sure that we only edit Trd container
    currentContainer = (CbmTrdParSet*) iContainerIt;
    currentPar.ReplaceAll("CbmTrdParSet", "");
    currentPar.ToLower();
    currentFile.Form("%s/%s.%s.par", outDir.Data(), geoName.Data(), currentPar.Data());
    FairParAsciiFileIo parOut;
    parOut.open(currentFile, "out");
    rtdb->setOutput(&parOut);
    currentContainer->setChanged();
    currentContainer->setInputVersion(0, 1);
    rtdb->saveOutput();
    rtdb->closeOutput();
  }
  return true; // check if rtdb->writeContainers() could be used to run a bool check and return that bool instead of saveOutput void 
}

// void CbmTrdParManager::FillDigiPar()
// {
//   printf("CbmTrdParManager::FillDigiPar()\n");
// //    Int_t nofModules = fModuleMap.size();
// //    fDigiPar->SetNrOfModules(nofModules);
// //    fDigiPar->SetMaxSectors(fMaxSectors);
// // 
// //    TArrayI moduleId(nofModules);
// //    Int_t iModule = 0;
// //    std::map<Int_t, CbmTrdModuleSim*>::iterator it;
// //    for (it = fModuleMap.begin() ; it != fModuleMap.end(); it++) {
// //       moduleId.AddAt(it->second->GetModuleAddress(), iModule);
// //       iModule++;
// //    }
// // 
// //    fDigiPar->SetModuleIdArray(moduleId);
// //    fDigiPar->SetModuleMap(fModuleMap);
// }

ClassImp(CbmTrdParManager)
