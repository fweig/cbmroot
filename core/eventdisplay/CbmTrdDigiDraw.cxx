/* Copyright (C) 2008-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mohammad Al-Turany, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                        CbmTrdDigiDraw source file                  -----
// -----                  Created 03/01/08  by M. Al-Turany            -----
// -------------------------------------------------------------------------
#include "CbmTrdDigiDraw.h"

#include "CbmTrdAddress.h"     // for CbmTrdAddress
#include "CbmTrdDigi.h"        // for CbmTrdDigi
#include "CbmTrdParModDigi.h"  // for CbmTrdParModDigi
#include "CbmTrdParSetDigi.h"  // for CbmTrdParSetDigi

#include <FairEventManager.h>  // for FairEventManager
#include <FairRootManager.h>   // for FairRootManager
#include <FairRunAna.h>        // for FairRunAna
#include <FairRuntimeDb.h>     // for FairRuntimeDb
#include <Logger.h>            // for Logger, LOG

#include <TClonesArray.h>  // for TClonesArray
#include <TEveBoxSet.h>    // for TEveBoxSet, TEveBoxSet::kBT_AABox
#include <TEveManager.h>   // for TEveManager, gEve
#include <TVector3.h>      // for TVector3

#include <iomanip>  // for setprecision

using std::setprecision;

// -----   Default constructor   -------------------------------------------
CbmTrdDigiDraw::CbmTrdDigiDraw()
  : FairTask()
  , fVerbose(0)
  , fPointList(nullptr)
  , fDigiPar(nullptr)
  , fModuleInfo(nullptr)
  , fEventManager(nullptr)
  , fq(nullptr)
  , fColor()
  , fStyle()
  , fActiveLayers()
{
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTrdDigiDraw::CbmTrdDigiDraw(const char* name, Color_t color, Style_t mstyle, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fVerbose(iVerbose)
  , fPointList(nullptr)
  , fDigiPar(nullptr)
  , fModuleInfo(nullptr)
  , fEventManager(nullptr)
  , fq(nullptr)
  , fColor(color)
  , fStyle(mstyle)
  , fActiveLayers()
{
  for (Int_t i = 0; i < 15; i++) {
    fActiveLayers[i] = kTRUE;
  }
}
// ----  Initialisation  ----------------------------------------------
void CbmTrdDigiDraw::SetParContainers()
{
  LOG(debug) << " * CbmTrdDigiDraw * :: SetParContainers() ";


  // Get Base Container
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();

  fDigiPar = (CbmTrdParSetDigi*) (rtdb->getContainer("CbmTrdParSetDigi"));
}
// --------------------------------------------------------------------

// -------------------------------------------------------------------------
InitStatus CbmTrdDigiDraw::Init()
{
  LOG(debug) << "CbmTrdDigiDraw::Init()";
  FairRootManager* fManager = FairRootManager::Instance();
  fPointList                = (TClonesArray*) fManager->GetObject(GetName());
  if (fPointList == 0) {
    LOG(warn) << "CbmTrdDigiDraw::Init()  branch " << GetName() << " Not found! Task will be deactivated ";
    SetActive(kFALSE);
  }
  LOG(debug1) << "CbmTrdDigiDraw::Init() get track list" << fPointList;
  fEventManager = FairEventManager::Instance();
  LOG(debug1) << "CbmTrdDigiDraw::Init() get instance of FairEventManager ";
  fq = 0;

  // gEve->AddElement(fq, fEventManager );

  return kSUCCESS;
}
// -------------------------------------------------------------------------
void CbmTrdDigiDraw::Exec(Option_t* /*option*/)
{
  if (IsActive()) {

    CbmTrdDigi* digi = 0;

    Int_t npoints = fPointList->GetEntriesFast();
    Reset();

    TEveBoxSet* q = new TEveBoxSet(GetName(), "");
    q->Reset(TEveBoxSet::kBT_AABox, kTRUE, npoints);
    q->SetMainColor(kBlue);

    //   q->SetMainAlpha(1.);
    //   q->SetMarkerSize(1.5);
    //   q->SetMarkerStyle(fStyle);

    //   q->SetDefDepth();
    //   q->SetDefHeight();
    //   q->SetDefDepth();

    Int_t refCounter = 0;

    for (Int_t i = 0; i < npoints; ++i) {

      digi = (CbmTrdDigi*) fPointList->At(i);


      if (digi != 0) {


        //Double_t ELoss = digi-> GetCharge();

        Int_t address  = digi->GetAddress();
        Int_t Col      = CbmTrdAddress::GetColumnId(address);
        Int_t Row      = CbmTrdAddress::GetRowId(address);
        Int_t Sector   = CbmTrdAddress::GetSectorId(address);
        Int_t moduleId = CbmTrdAddress::GetModuleAddress(address);
        Int_t layerId  = CbmTrdAddress::GetLayerId(address);

        fModuleInfo = (CbmTrdParModDigi*) fDigiPar->GetModulePar(moduleId);

        TVector3 posHit, padSize;
        fModuleInfo->GetPosition(
          /*moduleId, */ Sector, Col, Row, posHit, padSize);

        // The given point is used as the edge of the box but it is
        // the middle point of the pad. So we have to do a transformation


        Double_t X = posHit.X() - (padSize.X() / 2);
        Double_t Y = posHit.Y() - (padSize.Y() / 2);


        //       LOG(bebug1)<<"*** CbmTrdHitProducerDigi::CalculateHitPosition ***";
        LOG(debug1) << "Col: " << Col;
        LOG(debug1) << "Row: " << Row;
        LOG(debug1) << setprecision(5) << "fPadX: " << padSize.X();
        LOG(debug1) << setprecision(5) << "fPadY: " << padSize.Y();
        LOG(debug1) << setprecision(5) << "fPosX: " << posHit.X();
        LOG(debug1) << setprecision(5) << "fPosY: " << posHit.Y();
        LOG(debug1) << setprecision(5) << "fPosZ: " << posHit.Z();

        if (fActiveLayers[layerId]) {
          q->AddBox(X, Y, posHit.Z(), padSize.X(), padSize.Y(), 0.);
          refCounter++;
        }
      }
    }
    gEve->AddElement(q);
    gEve->Redraw3D(kFALSE);
    fq = q;
  }
}
// -----   Destructor   ----------------------------------------------------
CbmTrdDigiDraw::~CbmTrdDigiDraw() {}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
/** Action after each event**/
void CbmTrdDigiDraw::Finish() {}
// -------------------------------------------------------------------------
void CbmTrdDigiDraw::Reset()
{
  if (fq != 0) {
    fq->Reset();
    gEve->RemoveElement(fq, fEventManager);
  }
}

void CbmTrdDigiDraw::SetActiveLayer(Bool_t Layer1, Bool_t Layer2, Bool_t Layer3, Bool_t Layer4, Bool_t Layer5,
                                    Bool_t Layer6, Bool_t Layer7, Bool_t Layer8, Bool_t Layer9, Bool_t Layer10,
                                    Bool_t Layer11, Bool_t Layer12, Bool_t Layer13, Bool_t Layer14, Bool_t Layer15)

{
  fActiveLayers[0]  = Layer1;
  fActiveLayers[1]  = Layer2;
  fActiveLayers[2]  = Layer3;
  fActiveLayers[3]  = Layer4;
  fActiveLayers[4]  = Layer5;
  fActiveLayers[5]  = Layer6;
  fActiveLayers[6]  = Layer7;
  fActiveLayers[7]  = Layer8;
  fActiveLayers[8]  = Layer9;
  fActiveLayers[9]  = Layer10;
  fActiveLayers[10] = Layer11;
  fActiveLayers[11] = Layer12;
  fActiveLayers[12] = Layer13;
  fActiveLayers[13] = Layer14;
  fActiveLayers[14] = Layer15;
}


ClassImp(CbmTrdDigiDraw)
