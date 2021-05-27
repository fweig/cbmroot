#include "CbmTofCreateDigiPar.h"

#include "CbmTofCell.h"        // for CbmTofCell
#include "CbmTofDigiPar.h"     // for CbmTofDigiPar
#include "CbmTofGeoHandler.h"  // for CbmTofGeoHandler, k07a, k12b, k14a

#include <FairRootManager.h>  // for FairRootManager
#include <FairRunAna.h>       // for FairRunAna
#include <FairRuntimeDb.h>    // for FairRuntimeDb
#include <FairTask.h>         // for FairTask, InitStatus, kSUCCESS
#include <Logger.h>           // for LOG, Logger

#include <TArrayD.h>      // for TArrayD
#include <TArrayI.h>      // for TArrayI
#include <TGeoManager.h>  // for TGeoManager, gGeoManager
#include <TGeoNode.h>     // for TGeoNode
#include <TObjArray.h>    // for TObjArray
#include <TObject.h>      // for TObject

#include <utility>  // for pair

// ---- Default constructor -------------------------------------------
CbmTofCreateDigiPar::CbmTofCreateDigiPar()
  : FairTask("TofCreateDigiPar")
  , fSMType(-1)
  , fSModule(-1)
  , fCounter(-1)
  , fGap(-1)
  , fCell(-1)
  , fRegion(-1)
  , fSizex(-1.)
  , fSizey(-1.)
  , fX(-1.)
  , fY(-1.)
  , fZ(-1.)
  , fDetID(-1)
  , fCellID(-1)
  , fCellMap()
  , fCellMapIt()
  , fDigiPar(nullptr)
  , fGeoHandler(new CbmTofGeoHandler())
{
}
// --------------------------------------------------------------------

// ---- Constructor ----------------------------------------------------
CbmTofCreateDigiPar::CbmTofCreateDigiPar(const char* name, const char* /*title*/)
  : FairTask(name)
  , fSMType(-1)
  , fSModule(-1)
  , fCounter(-1)
  , fGap(-1)
  , fCell(-1)
  , fRegion(-1)
  , fSizex(-1.)
  , fSizey(-1.)
  , fX(-1.)
  , fY(-1.)
  , fZ(-1.)
  , fDetID()
  , fCellID(-1)
  , fCellMap()
  , fCellMapIt()
  , fDigiPar(nullptr)
  , fGeoHandler(new CbmTofGeoHandler())
{
}
// --------------------------------------------------------------------

// ---- Destructor ----------------------------------------------------
CbmTofCreateDigiPar::~CbmTofCreateDigiPar() {}
// --------------------------------------------------------------------

// ----  Initialisation  ----------------------------------------------
void CbmTofCreateDigiPar::SetParContainers()
{

  LOG(info) << " * CbmTofCreateDigiPar:: SetParContainers() ";

  // Get Base Container
  //FairRunAna* ana     = FairRunAna::Instance();
  //FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  FairRuntimeDb* rtdb = FairRuntimeDb::instance();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  LOG(info) << " * CbmTofCreateDigiPar:: fDigiPar " << fDigiPar;
}
// --------------------------------------------------------------------

// ---- ReInit  -------------------------------------------------------
InitStatus CbmTofCreateDigiPar::ReInit()
{

  LOG(info) << " * CbmTofCreateDigiPar * :: ReInit() ";

  // Get Base Container
  //FairRunAna* ana     = FairRunAna::Instance();
  //FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  FairRuntimeDb* rtdb = FairRuntimeDb::instance();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  return kSUCCESS;
}
// --------------------------------------------------------------------

// ---- Init ----------------------------------------------------------
InitStatus CbmTofCreateDigiPar::Init()
{

  Int_t geoVersion = fGeoHandler->Init();

  //  fModInfoMap = GetModuleInfoMap();

  LOG(info) << " * CbmTofCreateDigiPar * :: Init() ";

  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) LOG(fatal) << "No FairRootManager found";

  if (k21a == geoVersion) {
    LOG(info) << "Will now create digitization parameters for root geometry.";
    FillCellMapRootGeometry();
  }
  if (k14a == geoVersion) {
    LOG(info) << "Will now create digitization parameters for root geometry.";
    FillCellMapRootGeometry();
  }
  if (k12b == geoVersion) {
    LOG(info) << "Will now create digitization parameters for root geometry.";
    FillCellMapRootGeometry();
  }
  if (k07a == geoVersion) {
    LOG(info) << "Will now create digitization parameters for ascii geometry.";
    FillCellMapAsciiGeometry();
  }

  // fill Transformation matrices for each cell
  std::map<Int_t, TGeoNode*> nodemap;
  for (Int_t iCell = 0; iCell < fDigiPar->GetNrOfModules(); iCell++) {
    Int_t iAddr              = fDigiPar->GetCellId(iCell);
    CbmTofCell* fChannelInfo = fDigiPar->GetCell(iAddr);
    gGeoManager->FindNode(fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
    TGeoNode* tGeoNode = gGeoManager->GetCurrentNode();
    nodemap.insert(std::pair<Int_t, TGeoNode*>(iAddr, tGeoNode));
    LOG(debug) << Form("Digipar for %d, addr 0x%08x: Node=%p, x %6.2f, y %6.2f, z %6.2f ", iCell, iAddr, tGeoNode,
                       fChannelInfo->GetX(), fChannelInfo->GetY(), fChannelInfo->GetZ());
  }
  fDigiPar->SetNodeMap(nodemap);

  return kSUCCESS;
}
// --------------------------------------------------------------------
void CbmTofCreateDigiPar::FinishTask()
{

  LOG(info) << " * CbmTofCreateDigiPar * :: FinishTask() ";

  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));

  fDigiPar->print();
}

// ---- Exec ----------------------------------------------------------
void CbmTofCreateDigiPar::Exec(Option_t* /*option*/) {}

// --------------------------------------------------------------------
void CbmTofCreateDigiPar::FillCellMapAsciiGeometry()
{

  // The geometry structure is treelike with cave as
  // the top node. For the TOF there is a keeping volume
  // tof1. Inside there is a region (supermodule) with cells
  // which are constructed out of glass and the active gaps.
  // To extract the gap information one has to navigate there.
  // Loop over all nodes below the top node (cave). If one of
  // the nodes contains a string tof it must be the tof keeping
  // volume. Now loop over all modules. For each module loop over
  // all cells and for each cell loop now over the parts of this cell.
  // Each cell is build out of inactive glass plates and the active gaps.
  // since we are not interested in the inactive parts store only
  // the relevant information about the gaps.
  // Example for full path to gap
  //   /cave/tof1_0/t1reg1mod_1/t1reg1cel_1/t1reg1gap_1

  /*  Int_t nrCells = 0;*/
  std::vector<CbmTofCell*> cellVector;
  CbmTofCell* tofCell;

  TString TopNode = gGeoManager->GetTopNode()->GetName();
  LOG(debug2) << TopNode;

  // Loop over all detector systems to find tof part
  TObjArray* detSystems = gGeoManager->GetTopNode()->GetNodes();
  for (Int_t iSys = 0; iSys < detSystems->GetEntriesFast(); ++iSys) {
    TGeoNode* node = (TGeoNode*) detSystems->At(iSys);
    LOG(debug2) << node->GetName();

    // Only do something useful for tof part of geometry
    // The node name contains a string "tof"
    // e.g. tof_vXXy or tof_v12b
    if (TString(node->GetName()).Contains("tof")) {
      TString TofNode = node->GetName();
      LOG(info) << "Found keeping node " << TofNode;
      if (TString(((node->GetNodes())->At(0))->GetName()).Contains("Stand")) {
        LOG(info) << " Found Tof Stand " << ((node->GetNodes())->At(0))->GetName();
        node    = (TGeoNode*) (node->GetNodes())->At(0);
        TofNode = TofNode + "/" + node->GetName();
        LOG(info) << "Modified  keeping node " << TofNode;
      }
      TGeoNode* keep     = node;
      TObjArray* keeping = keep->GetNodes();

      LOG(info) << "Building Tof Digi Par database ... ";

      // Loop over tof keeping volume. There should be only one.
      for (Int_t ikeep = 0; ikeep < keeping->GetEntriesFast(); ikeep++) {
        TGeoNode* keepvol   = (TGeoNode*) keeping->At(ikeep);
        TString KeepNode    = keepvol->GetName();
        TObjArray* modarray = keepvol->GetNodes();

        // Loop over the different found modules
        for (Int_t imodule = 0; imodule < modarray->GetEntriesFast(); imodule++) {
          TGeoNode* module     = (TGeoNode*) modarray->At(imodule);
          TString ModuleNode   = module->GetName();
          TObjArray* cellarray = module->GetNodes();

          // Loop over all cells of each module
          for (Int_t icell = 0; icell < cellarray->GetEntriesFast(); icell++) {
            TGeoNode* cell      = (TGeoNode*) cellarray->At(icell);
            TString CellNode    = cell->GetName();
            TObjArray* gaparray = cell->GetNodes();

            // Loop over all elements belonging to one cell
            // Relevant are only the gaps which are the active parts of the detector
            for (Int_t igap = 0; igap < gaparray->GetEntriesFast(); igap++) {
              TGeoNode* gap   = (TGeoNode*) gaparray->At(igap);
              TString GapNode = gap->GetName();

              if (GapNode.Contains("gap")) {

                // Construct full path name for the gap
                // Extract the necessary geometrical information and store
                // this information in member variables
                TString FullPath =
                  "/" + TopNode + "/" + TofNode + "/" + KeepNode + "/" + ModuleNode + "/" + CellNode + "/" + GapNode;
                LOG(debug2) << "Path: " << FullPath;
                FillCellInfoFromGeoHandler(FullPath);

                // Since there are 8 gaps per cell, the information for all these
                // gaps are stored. After all the information is available the
                // position of the cell is calculated as mean position of the
                // 8 individual gaps. The size of each cell should be the same for
                // all 8 gaps.

                fCellMapIt = fCellMap.find(fCellID);
                if (fCellMapIt == fCellMap.end()) {
                  // new tof cell
                  tofCell = new CbmTofCell(fCellID, fX, fY, fZ, fSizex, fSizey);
                  cellVector.clear();
                  cellVector.push_back(tofCell);
                  fCellMap.insert(std::pair<Int_t, std::vector<CbmTofCell*>>(fCellID, cellVector));
                }
                else {
                  // already existing cell
                  tofCell = new CbmTofCell(fCellID, fX, fY, fZ, fSizex, fSizey);
                  fCellMap[fCellID].push_back(tofCell);
                }
              }
            }
          }
        }
      }
    }
  }
  // Calculate the mean position for each cell and fill the tof digi parameters
  FillDigiPar();
}

void CbmTofCreateDigiPar::FillCellMapRootGeometry()
{

  // The geometry structure is treelike with cave as
  // the top node. For the TOF there is a keeping volume
  // tof1_v<version>. <version is the geometry version which is constructed
  // from the year when this geometry was developed and a running index starting
  // with a,b .. . So tof_v12b denotes the second tof geometry version
  // developed in 2012.
  // Inside the tof keeping volumes there is one or more (supermodules)
  // with a gas box inside which contain the different counters.
  // Each of this counters in now build of a stack of glass plates
  // and active gas gaps. Interesting for the parameters are only the
  // gas gaps. Each gap is then subdivided into several detector cells.
  // To extract the gap information one has to navigate there.
  // Loop over all nodes below the top node (cave). If one of
  // the nodes contains a string tof_v it must be the tof keeping
  // volume. Now loop over all super-modules. For each super-module
  // loop over all counters. For each counter loop over all gaps and
  // for each gap loop over all cells.
  // For each cell/gap store now the relevant information.
  // Example for full path to gap
  //   /cave_0/tof_v12b_0/module_0_0/gas_box_0/counter_0/Gap_0/Cell_1

  //   /TOP_1/tof_v21b_mcbm_1/tof_v21b_mcbmStand_1/module_9_0/gas_box_0/counter_1/Gap_17/Cell_18

  /*  Int_t nrCells = 0;*/
  std::vector<CbmTofCell*> cellVector;
  CbmTofCell* tofCell;


  TString TopNode = gGeoManager->GetTopNode()->GetName();
  LOG(debug2) << "TopNode: " << TopNode;

  // Loop over all detector systems to find tof part
  TObjArray* detSystems = gGeoManager->GetTopNode()->GetNodes();
  for (Int_t iSys = 0; iSys < detSystems->GetEntriesFast(); ++iSys) {
    TGeoNode* node = (TGeoNode*) detSystems->At(iSys);
    LOG(debug2) << "Det system: " << node->GetName();

    // Only do something useful for tof_v part of geometry
    // The node name contains a string "tof_v"
    // e.g. tof_v12b
    if (TString(node->GetName()).Contains("tof")) {
      TString TofNode = node->GetName();
      LOG(info) << "Found tof keeping volume: " << TofNode;

      if (TString(((node->GetNodes())->At(0))->GetName()).Contains("Stand")) {
        LOG(info) << " Found Tof Stand " << ((node->GetNodes())->At(0))->GetName();
        node    = (TGeoNode*) (node->GetNodes())->At(0);
        TofNode = TofNode + "/" + node->GetName();
        LOG(info) << "Modified  tof keeping node " << TofNode;
      }

      TGeoNode* keep      = node;
      TObjArray* modarray = keep->GetNodes();

      if (nullptr == modarray) {
        LOG(WARNING) << " No modules found in geometry ";
        return;
      }
      // Loop over the different found modules
      for (Int_t imodule = 0; imodule < modarray->GetEntriesFast(); imodule++) {
        TGeoNode* module        = (TGeoNode*) modarray->At(imodule);
        TString ModuleNode      = module->GetName();
        TObjArray* modpartarray = module->GetNodes();

        // Loop over the different parts of a module
        for (Int_t imodpart = 0; imodpart < modpartarray->GetEntriesFast(); imodpart++) {
          TGeoNode* modpart   = (TGeoNode*) modpartarray->At(imodpart);
          TString ModPartNode = modpart->GetName();

          if (ModPartNode.Contains("gas_box")) {
            TObjArray* counterarray = modpart->GetNodes();

            // Loop over the different counters
            for (Int_t icounter = 0; icounter < counterarray->GetEntriesFast(); icounter++) {
              TGeoNode* counter   = (TGeoNode*) counterarray->At(icounter);
              TString CounterNode = counter->GetName();
              if (!CounterNode.Contains("counter")) continue;
              TObjArray* gaparray = counter->GetNodes();
              if (nullptr == gaparray) LOG(error) << " no gaps for counter " << CounterNode;
              // Loop over the different gaps
              for (Int_t igap = 0; igap < gaparray->GetEntriesFast(); igap++) {
                TGeoNode* gap   = (TGeoNode*) gaparray->At(igap);
                TString GapNode = gap->GetName();
                if (GapNode.Contains("Gap")) {
                  TObjArray* cellarray = gap->GetNodes();

                  // Loop over the different cells
                  for (Int_t icell = 0; icell < cellarray->GetEntriesFast(); icell++) {
                    TGeoNode* cell   = (TGeoNode*) cellarray->At(icell);
                    TString CellNode = cell->GetName();

                    // Construct full path name for the gap
                    // Extract the necessary geometrical information and store
                    // this information in member variables
                    TString FullPath = "/" + TopNode + "/" + TofNode + "/" + +ModuleNode + "/" + ModPartNode + "/"
                                       + CounterNode + "/" + GapNode + "/" + CellNode;
                    LOG(debug2) << "Path: " << FullPath;

                    FillCellInfoFromGeoHandler(FullPath);

                    // Since there are 8 gaps per cell, the information for all these
                    // gaps are stored. After all the information is available the
                    // position of the cell is calculated as mean position of the
                    // 8 individual gaps. The size of each cell should be the same for
                    // all 8 gaps.

                    fCellMapIt = fCellMap.find(fCellID);
                    if (fCellMapIt == fCellMap.end()) {
                      // new tof cell
                      tofCell = new CbmTofCell(fCellID, fX, fY, fZ, fSizex, fSizey);
                      cellVector.clear();
                      cellVector.push_back(tofCell);
                      fCellMap.insert(std::pair<Int_t, std::vector<CbmTofCell*>>(fCellID, cellVector));
                    }
                    else {
                      // already existing cell
                      tofCell = new CbmTofCell(fCellID, fX, fY, fZ, fSizex, fSizey);
                      fCellMap[fCellID].push_back(tofCell);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  // Calculate the mean position for each cell and fill the tof digi parameters
  FillDigiPar();
}

void CbmTofCreateDigiPar::FillCellInfoFromGeoHandler(TString FullPath)
{
  // Calculate the unique detector ID including the gap information.
  // Since all gaps for a given cell are added up to one channel the
  // id and corresponding information stored in the parameter container
  // should be calculated without the gap information.

  fDetID = fGeoHandler->GetUniqueDetectorId(FullPath);

  fSMType  = fGeoHandler->GetSMType(fDetID);
  fSModule = fGeoHandler->GetSModule(fDetID);
  fCounter = fGeoHandler->GetCounter(fDetID);
  fGap     = fGeoHandler->GetGap(fDetID);
  fCell    = fGeoHandler->GetCell(fDetID);
  fRegion  = fGeoHandler->GetRegion(fDetID);

  fSizex = fGeoHandler->GetSizeX(FullPath);
  fSizey = fGeoHandler->GetSizeY(FullPath);

  fX = fGeoHandler->GetX(FullPath);
  fY = fGeoHandler->GetY(FullPath);
  fZ = fGeoHandler->GetZ(FullPath);

  LOG(debug2) << "FCI: " << FullPath.Data();
  LOG(debug2) << "FCI: X: " << fX << " Y: " << fY << " Z: " << fZ << " SizeX: " << fSizex << " SizeY: " << fSizey;
  LOG(debug2) << Form(" DetID: 0x%08x", fDetID) << " Region: " << fRegion << " Counter: " << fCounter
              << " Gap: " << fGap << " Cell: " << fCell;

  fCellID = fGeoHandler->GetCellId(fDetID);

  fSMType  = fGeoHandler->GetSMType(fCellID);
  fSModule = fGeoHandler->GetSModule(fCellID);
  fCounter = fGeoHandler->GetCounter(fCellID);
  fGap     = fGeoHandler->GetGap(fCellID);
  fCell    = fGeoHandler->GetCell(fCellID);
  fRegion  = fGeoHandler->GetRegion(fCellID);

  LOG(debug2) << "FCI: Cell ID: " << Form("0x%08x", fCellID) << " detId " << Form("0x%08x", fDetID);
  LOG(debug2) << " Region:  " << fGeoHandler->GetRegion(fCellID) << " SMTYP:   " << fGeoHandler->GetSMType(fCellID)
              << " SModule: " << fGeoHandler->GetSModule(fCellID) << " Module:  " << fGeoHandler->GetCounter(fCellID)
              << " Gap:     " << fGeoHandler->GetGap(fCellID) << " Cell: " << fGeoHandler->GetCell(fCellID);
}


void CbmTofCreateDigiPar::FillDigiPar()
{

  /*
  ofstream fout;
  fout.open("output.txt");
  fout << "#####################################################################################"<<"\n";
  fout << "# Geometry for the TOF detector"<<"\n";
  fout << "# Format:"<<"\n";
  fout << "#"<<"\n";
  fout << "# Region Module Cell   type       X[mm]           Y[mm]         Dx[mm]  Dy[mm]"<<"\n";
  fout << "#####################################################################################"<<"\n";
  fout << "[TofGeoPar]"<<"\n";
*/

  Int_t Nrcells = (Int_t) fCellMap.size();
  LOG(debug) << "FillDigiPar:: Nr. of tof cells: " << Nrcells;
  SetParContainers();
  if (NULL == fDigiPar) LOG(fatal) << "Tof Digi Parameter container not available ";
  fDigiPar->SetNrOfCells(Nrcells);  //transfer info to DigiPar

  TArrayI* CellId = new TArrayI(Nrcells);
  TArrayD* CellX  = new TArrayD(Nrcells);
  TArrayD* CellY  = new TArrayD(Nrcells);
  TArrayD* CellZ  = new TArrayD(Nrcells);
  TArrayD* CellDx = new TArrayD(Nrcells);
  TArrayD* CellDy = new TArrayD(Nrcells);

  Int_t iDigi = 0;


  std::map<Int_t, CbmTofCell*> singleCellMap;
  CbmTofCell* singlecell;

  for (fCellMapIt = fCellMap.begin(); fCellMapIt != fCellMap.end(); fCellMapIt++) {

    CellId->AddAt(fCellMapIt->first, iDigi);

    std::vector<CbmTofCell*> vcell = fCellMapIt->second;
    Int_t cellId                   = fCellMapIt->first;
    // sanity check
    std::vector<CbmTofCell*>::iterator vcellIt;
    Int_t id;
    /*    Int_t oldid;*/
    /*    Bool_t first=kTRUE;*/
    CbmTofCell* tofcell;
    Double_t x     = 0.;
    Double_t y     = 0.;
    Double_t z     = 0.;
    Double_t sizex = 0.;
    Double_t sizey = 0.;
    for (vcellIt = vcell.begin(); vcellIt != vcell.end(); ++vcellIt) {
      tofcell = (*vcellIt);
      id      = tofcell->GetDetectorId();
      if (id != cellId) {
        LOG(info) << "id, cellId: " << id << " , " << cellId << "\n";
        LOG(info) << "id and CellId differ";
      }
      x += tofcell->GetX();
      y += tofcell->GetY();
      z += tofcell->GetZ();
      sizex += 2. * tofcell->GetSizex();  //nh: factor 2
      sizey += 2. * tofcell->GetSizey();
    }

    CellX->AddAt(x / vcell.size(), iDigi);
    CellY->AddAt(y / vcell.size(), iDigi);
    CellZ->AddAt(z / vcell.size(), iDigi);
    CellDx->AddAt(sizex / vcell.size(), iDigi);
    CellDy->AddAt(sizey / vcell.size(), iDigi);

    /**/
    singlecell = new CbmTofCell(cellId, x / vcell.size(), y / vcell.size(), z / vcell.size(), sizex / vcell.size(),
                                sizey / vcell.size());
    singleCellMap.insert(std::pair<Int_t, CbmTofCell*>(cellId, singlecell));
    /**/

    fRegion  = fGeoHandler->GetRegion(cellId);
    fSMType  = fGeoHandler->GetSMType(cellId);
    fSModule = fGeoHandler->GetSModule(cellId);
    fCounter = fGeoHandler->GetCounter(cellId);
    fCell    = fGeoHandler->GetCell(cellId);

    if (0) {
      LOG(info) << "FillDigiPar " << iDigi << ", cellId = " << cellId << ", t " << fSMType << "  m " << fSModule
                << "  c " << fCounter << "  s " << fCell << "   " << x / vcell.size() * 10 << "   "
                << y / vcell.size() * 10 << "   " << z / vcell.size() * 10 << "   " << sizex / vcell.size() * 10
                << "   " << sizey / vcell.size() * 10;
    }
    iDigi++;
  }


  fDigiPar->SetNrOfCells(Nrcells);
  fDigiPar->SetCellIdArray(*CellId);
  fDigiPar->SetCellXArray(*CellX);
  fDigiPar->SetCellYArray(*CellY);
  fDigiPar->SetCellZArray(*CellZ);
  fDigiPar->SetCellDxArray(*CellDx);
  fDigiPar->SetCellDyArray(*CellDy);
  fDigiPar->SetCellMap(singleCellMap);
}


ClassImp(CbmTofCreateDigiPar)
