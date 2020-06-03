#include "CbmTrdClusterFinder.h"

#include "CbmTrdParSetDigi.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetGas.h"
#include "CbmTrdParSetGeo.h"
#include "CbmTrdParSetGain.h"
#include "CbmTrdParAsic.h"
#include "CbmTrdParModGas.h"
#include "CbmTrdParModGain.h"
#include "CbmDigiManager.h"

#include "CbmTrdDigi.h"
#include "CbmTrdCluster.h"
#include "CbmTrdModuleRecR.h"
#include "CbmTrdModuleRecT.h"
#include "CbmTrdGeoHandler.h"

#include <FairLogger.h>
#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>

#include <TStopwatch.h>
#include <TClonesArray.h>
#include <TArray.h>
#include <TBits.h>
#include <TGeoPhysicalNode.h>
// #include "TCanvas.h"
// #include "TImage.h"

#include <iostream>
#include <iomanip>
#include <cmath>
using std::fabs;
using std::cout;
using std::endl;
using std::setprecision;

Int_t CbmTrdClusterFinder::fgConfig = 0;
Float_t CbmTrdClusterFinder::fgMinimumChargeTH = .5e-06;
//_____________________________________________________________________
CbmTrdClusterFinder::CbmTrdClusterFinder()
  :FairTask("TrdClusterFinder",1),
   fClusters(NULL),
   fDigiMap(),
   fModuleMap(),
   fNeighbours(),
   fModDigiMap(),
   fDigiRow(),
   fDigiCol(),
   fDigiCharge(),
   fClusterBuffer(),
   fModClusterDigiMap(),
//=======================
  fModules()
  ,fAsicPar(NULL)
  ,fGasPar(NULL)
  ,fDigiPar(NULL)
  ,fGainPar(NULL)
  ,fGeoPar(NULL)

{
}
// --------------------------------------------------------------------

// ---- Destructor ----------------------------------------------------
CbmTrdClusterFinder::~CbmTrdClusterFinder()
{

  if(fClusters){
    fClusters->Clear("C");
    fClusters->Delete();
    delete fClusters;
  }
  if(fDigiPar){
    delete fDigiPar;
  }
  if(fGeoPar){
    delete fGeoPar;
  }
//   if(fModuleInfo){
//     delete fModuleInfo;
//   }

}

//_____________________________________________________________________
Bool_t CbmTrdClusterFinder::AddCluster(CbmTrdCluster* c)
{
  Int_t ncl(fClusters->GetEntriesFast()); 
  new((*fClusters)[ncl++]) CbmTrdCluster(*c); 
  return kTRUE;
}

//____________________________________________________________________________________
CbmTrdModuleRec* CbmTrdClusterFinder::AddModule(const CbmTrdDigi *digi)
{
  Int_t address = digi->GetAddressModule();
  CbmTrdModuleRec *module(NULL);
  if(digi->GetType()==CbmTrdDigi::kFASP) module = fModules[address] = new CbmTrdModuleRecT(address);
  else module = fModules[address] = new CbmTrdModuleRecR(address);

  // try to load Geometry parameters for module
  const CbmTrdParModGeo *pGeo(NULL);
  if(!fGeoPar || !(pGeo = (const CbmTrdParModGeo *)fGeoPar->GetModulePar(address))){
    LOG(fatal) << GetName() << "::AddModule : No Geo params for module "<< address <<". Using default.";
  } else module->SetGeoPar(pGeo);

  // try to load read-out parameters for module
  const CbmTrdParModDigi *pDigi(NULL);
  if(!fDigiPar || !(pDigi = (const CbmTrdParModDigi *)fDigiPar->GetModulePar(address))){
    LOG(warn) << GetName() << "::AddModule : No Read-Out params for modAddress "<< address <<". Using default.";
  } else module->SetDigiPar(pDigi);

  // try to load ASIC parameters for module
  CbmTrdParSetAsic *pAsic(NULL);
  if(!fAsicPar || !(pAsic = (CbmTrdParSetAsic *)fAsicPar->GetModuleSet(address))){
    LOG(warn) << GetName() << "::AddModule : No ASIC params for modAddress "<< address <<". Using default.";
//    module->SetAsicPar(); // map ASIC channels to read-out channels - need ParModDigi already loaded
  } else module->SetAsicPar(pAsic);

  // try to load Chamber parameters for module
  const CbmTrdParModGas *pChmb(NULL);
  if(!fGasPar || !(pChmb = (const CbmTrdParModGas *)fGasPar->GetModulePar(address))){
    LOG(warn) << GetName() << "::AddModule : No Gas params for modAddress "<< address <<". Using default.";
  } else module->SetChmbPar(pChmb);

  // try to load Gain parameters for module
  if(digi->GetType()==CbmTrdDigi::kFASP){
    const CbmTrdParModGain *pGain(NULL);
    if(!fGainPar || !(pGain = (const CbmTrdParModGain *)fGainPar->GetModulePar(address))){
      //LOG(warn) << GetName() << "::AddModule : No Gain params for modAddress "<< address <<". Using default.";
    } else module->SetGainPar(pGain);
  }
  return module;
}

//_____________________________________________________________________
void CbmTrdClusterFinder::SetParContainers()
{
  fAsicPar = static_cast<CbmTrdParSetAsic*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetAsic"));
  fGasPar = static_cast<CbmTrdParSetGas*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetGas"));
  fDigiPar = static_cast<CbmTrdParSetDigi*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetDigi"));
  fGainPar = static_cast<CbmTrdParSetGain*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetGain"));
  // Get the full geometry information of the TRD modules
  fGeoPar = new CbmTrdParSetGeo();
}

//_____________________________________________________________________
InitStatus CbmTrdClusterFinder::Init()
{

  FairRootManager *ioman = FairRootManager::Instance();
  

  CbmDigiManager::Instance()->Init();
  if(!CbmDigiManager::Instance()->IsPresent(ECbmModuleId::kTrd)) LOG(fatal);
  
  fClusters = new TClonesArray("CbmTrdCluster", 100);
  ioman->Register("TrdCluster","TRD",fClusters,IsOutputBranchPersistent("TrdCluster"));

  if(!IsTimeBased() && !ioman->GetObject("Event")){ 
    LOG(warn) << GetName()
            << ": Event mode selected but no event array found! Run in time-based mode.";
    SetTimeBased();
  }
//   // Get the full geometry information of the detector gas layers and store
//   // them with the CbmTrdModuleRec. This information can then be used for
//   // transformation calculations 
//   std::map<Int_t, TGeoPhysicalNode*> moduleMap = fGeoHandler->FillModuleMap();
// 
//   Int_t nrModules = fDigiPar->GetNrOfModules();
//   Int_t nrNodes = moduleMap.size();
//   if (nrModules != nrNodes) LOG(fatal) << "Geometry and parameter files have different number of modules.";
//   for (Int_t loop=0; loop< nrModules; ++loop) {
//      Int_t address = fDigiPar->GetModuleId(loop);
//      std::map<Int_t, TGeoPhysicalNode*>::iterator it = moduleMap.find(address);
//      if ( it  == moduleMap.end() ) {
//        LOG(fatal) << "Expected module with address " << address << " wasn't found in the map with TGeoNode information.";
//      }
//      AddModule(address, it->second);
//   }
  
//   // new call needed when parameters are initialized from ROOT file
//   fDigiPar->Initialize();

  LOG(info) << "================ TRD Cluster Finder ===============";
  LOG(info) << " Free streaming    : " << (IsTimeBased()?"yes":"no");
  LOG(info) << " Multi hit detect  : " << (HasMultiHit()?"yes":"no");
  LOG(info) << " Row merger        : " << (HasRowMerger()?"yes":"no");
  LOG(info) << " c-Neighbour enable: " << (HasNeighbourCol()?"yes":"no");
  LOG(info) << " r-Neighbour enable: " << (HasNeighbourRow()?"yes":"no");
  LOG(info) << " Write clusters    : " << (HasDumpClusters()?"yes":"no");

  return kSUCCESS;
  
} 

//_____________________________________________________________________
void CbmTrdClusterFinder::Exec(Option_t* /*option*/)
{
/**
* Digis are sorted according to the moduleAddress. A combiId is calculted based 
* on the rowId and the colId to have a neighbouring criterion for digis within 
* the same pad row. The digis of each module are sorted according to this combiId.
* All sorted digis of one pad row are 'clustered' into rowCluster. For a new row
* the rowClusters are compared to the rowClusters of the last row. If an overlap 
* is found they are marked to be parents(last row) and childrens(activ row) 
* (mergeRowCluster()). After this, the finale clusters are created. Therefor 
* walkCluster() walks along the list of marked parents and markes every visited 
* rowCluster to avoid multiple usage of one rowCluster. drawCluster() can be used to
* get a visual output.
*/

  fClusters->Delete();
    
  TStopwatch timer;
  timer.Start();
  
  //  Int_t nentries = fDigis->GetEntries();  
  //printf("processing %d entries\n", nentries);
  CbmTrdModuleRec *mod(NULL);
  Int_t nDigis = CbmDigiManager::Instance()->GetNofDigis(ECbmModuleId::kTrd);
  for (Int_t iDigi=0; iDigi < nDigis; iDigi++ ) {

    const CbmTrdDigi *digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(iDigi);
    
    
    //CbmTrdDigi *digi = (CbmTrdDigi*) fDigis->At(iDigi);
    Int_t moduleAddress = digi->GetAddressModule();

    std::map<Int_t, CbmTrdModuleRec*>::iterator imod = fModules.find(moduleAddress);
    if(imod == fModules.end()) mod = AddModule(digi);
    else mod=imod->second;
    //    std::cout<<digi->GetTime()<<std::endl;
    
    mod->AddDigi(digi, iDigi);    
  }
  
  Int_t digiCounter(0), clsCounter(0);
  for(std::map<Int_t, CbmTrdModuleRec*>::iterator imod = fModules.begin(); imod!=fModules.end(); imod++){
    mod = imod->second;
    digiCounter += mod->GetOverThreshold();
    clsCounter += mod->FindClusters();
    AddClusters(mod->GetClusters(), kTRUE);
  }

  // remove local data from all modules
  for(std::map<Int_t, CbmTrdModuleRec*>::iterator imod = fModules.begin(); imod!=fModules.end(); imod++) imod->second->Clear("cls");
  
  timer.Stop();

  LOG(info) << GetName() << "::Exec : Digis    : " << nDigis << " / " << digiCounter << " above threshold (" << 1e6*fgMinimumChargeTH << " keV)";
  LOG(info) << GetName() << "::Exec : Clusters : " << clsCounter;
  LOG(info) << GetName() << "::Exec : real time=" << timer.RealTime() << " CPU time=" << timer.CpuTime();

            //   //cout << "  " << counterI << " (" << counterI*100.0/Float_t(counterJ) << "%)" <<  " are reconstructed with fRowClusterMerger" << endl;
//   //printf("   %4d modules (%6.3f%%) are reconstructed with fRowClusterMerger\n",counterI,counterI*100/Float_t(counterJ));
//   LOG(info) << "CbmTrdClusterFinder::Exec : RowClusterMerger are used " << fRowMergerCounter << " times";
}

//_____________________________________________________________________
Int_t CbmTrdClusterFinder::AddClusters(TClonesArray *clusters, Bool_t/* move*/) 
{
  if(!clusters) return 0;
  CbmTrdCluster *cls(NULL), *clsSave(NULL);
  const CbmTrdDigi *digi(NULL);
  CbmTrdParModDigi *digiPar(NULL);
  TBits cols, rows;
  Int_t ncl(fClusters->GetEntriesFast()), mcl(0), ncols(0);

  for(Int_t ic(0); ic<clusters->GetEntriesFast(); ic++){
    if(!(cls=(CbmTrdCluster*)(*clusters)[ic])) continue;
    
    if(!cls->HasTrianglePads()){ // only for rectangular clusters
      if(!ncols){
        digiPar = (CbmTrdParModDigi*)fDigiPar->GetModulePar(cls->GetAddress());
        if(!digiPar){
          LOG(error) << "CbmTrdClusterFinder::AddClusters : Can't find ParModDigi for address"<<cls->GetAddress();
          continue;
        }
        ncols=digiPar->GetNofColumns();
      }
      cols.Clear(); rows.Clear();
      for(Int_t id=0; id<cls->GetNofDigis(); id++) {
        digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(cls->GetDigi(id)); 
        Int_t digiChannel = digi->GetAddressChannel();
        Int_t colId     = digiChannel % ncols;
        Int_t globalRow = digiChannel / ncols;

        Int_t combiId   = globalRow * ncols + colId;
        cols.SetBitNumber(combiId);
        rows.SetBitNumber(globalRow);
      }
      // store information in cluster
      cls->SetNCols( cols.CountBits() );
      cls->SetNRows( rows.CountBits() );
    }
    clsSave = new((*fClusters)[ncl++]) CbmTrdCluster(*cls); // TODO implement copy constructor
    clsSave->SetTrianglePads(cls->HasTrianglePads());
    if(cls->GetMatch()!=NULL) delete cls; //only the matches have pointers to allocated memory, so otherwise the clear does the trick
    mcl++;
  }
  //clusters->Clear();
  return mcl;
}

//_____________________________________________________________________
void CbmTrdClusterFinder::Finish()
{

  
}

ClassImp(CbmTrdClusterFinder)

