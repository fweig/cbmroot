/** @file CbmTofDigiPar.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmTofDigiPar.h"

#include "CbmTofCell.h"  // for CbmTofCell

#include <FairParGenericSet.h>  // for FairParGenericSet
#include <FairParamList.h>      // for FairParamList
#include <Logger.h>             // for LOG, Logger

#include <TArrayD.h>  // for TArrayD
#include <TArrayI.h>  // for TArrayI
#include <TString.h>  // for TString

#include <utility>  // for pair

ClassImp(CbmTofDigiPar)

  CbmTofDigiPar::CbmTofDigiPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name, title, context)
  , fCellMap()
  , fCellIdArray()
  , fCellXArray()
  , fCellYArray()
  , fCellZArray()
  , fCellDxArray()
  , fCellDyArray()
  , fNrOfCells(-1)
{
  detName = "Tof";
}

CbmTofDigiPar::~CbmTofDigiPar(void)
{
  LOG(debug4) << "Enter CbmTofDigiPar destructor";
  std::map<Int_t, CbmTofCell*>::iterator fCellMapIt;
  for (fCellMapIt = fCellMap.begin(); fCellMapIt != fCellMap.end(); ++fCellMapIt) {
    delete fCellMapIt->second;
  }
  fCellMap.clear();
  clear();
  LOG(debug4) << "Leave CbmTofDigiPar destructor";
}

void CbmTofDigiPar::clear(void)
{
  status = kFALSE;
  resetInputVersions();
}

void CbmTofDigiPar::putParams(FairParamList* l)
{
  if (!l) { return; }

  l->add("NrOfCells", fNrOfCells);
  l->add("CellIdArray", fCellIdArray);
  l->add("CellXArray", fCellXArray);
  l->add("CellYArray", fCellYArray);
  l->add("CellZArray", fCellZArray);
  l->add("CellDxArray", fCellDxArray);
  l->add("CellDyArray", fCellDyArray);
}

Bool_t CbmTofDigiPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }

  LOG(debug2) << "Get the tof digitization parameters.";

  if (!l->fill("NrOfCells", &fNrOfCells)) return kFALSE;

  LOG(debug2) << "There are " << fNrOfCells << " cells to be read.";

  fCellIdArray.Set(fNrOfCells);
  if (!l->fill("CellIdArray", &fCellIdArray)) return kFALSE;

  fCellXArray.Set(fNrOfCells);
  if (!l->fill("CellXArray", &fCellXArray)) return kFALSE;

  fCellYArray.Set(fNrOfCells);
  if (!l->fill("CellYArray", &fCellYArray)) return kFALSE;

  fCellZArray.Set(fNrOfCells);
  if (!l->fill("CellZArray", &fCellZArray)) return kFALSE;

  fCellDxArray.Set(fNrOfCells);
  if (!l->fill("CellDxArray", &fCellDxArray)) return kFALSE;

  fCellDyArray.Set(fNrOfCells);
  if (!l->fill("CellDyArray", &fCellDyArray)) return kFALSE;


  for (Int_t i = 0; i < fNrOfCells; i++) {
    fCellMap[fCellIdArray[i]] =
      new CbmTofCell(fCellIdArray[i], fCellXArray[i], fCellYArray[i], fCellZArray[i], fCellDxArray[i], fCellDyArray[i]);
  }
  return kTRUE;
}
