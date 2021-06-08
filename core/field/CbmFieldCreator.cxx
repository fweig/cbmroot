/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mohammad Al-Turany [committer], Florian Uhlig */

#include "CbmFieldCreator.h"

#include "CbmBsField.h"            // for CbmBsField
#include "CbmFieldConst.h"         // for CbmFieldConst
#include "CbmFieldMap.h"           // for CbmFieldMap
#include "CbmFieldMapDistorted.h"  // for CbmFieldMapDistorted
#include "CbmFieldMapSym1.h"       // for CbmFieldMapSym1
#include "CbmFieldMapSym2.h"       // for CbmFieldMapSym2
#include "CbmFieldMapSym3.h"       // for CbmFieldMapSym3
#include "CbmFieldPar.h"           // for CbmFieldPar, kTypeDistorted

#include <FairField.h>         // for FairField
#include <FairFieldFactory.h>  // for FairFieldFactory
#include <FairRunAna.h>        // for FairRunAna
#include <FairRuntimeDb.h>     // for FairRuntimeDb

#include <RtypesCore.h>  // for Int_t

#include <iostream>  // for operator<<, basic_ostream, endl

using std::cerr;
using std::cout;
using std::endl;

static CbmFieldCreator gCbmFieldCreator;

CbmFieldCreator::CbmFieldCreator() : FairFieldFactory(), fFieldPar(nullptr) { fCreator = this; }

CbmFieldCreator::~CbmFieldCreator() {}

void CbmFieldCreator::SetParm()
{
  FairRunAna* Run      = FairRunAna::Instance();
  FairRuntimeDb* RunDB = Run->GetRuntimeDb();
  fFieldPar            = (CbmFieldPar*) RunDB->getContainer("CbmFieldPar");
}

FairField* CbmFieldCreator::createFairField()
{
  FairField* fMagneticField = 0;

  if (!fFieldPar) { cerr << "-E-  No field parameters available!" << endl; }
  else {
    // Instantiate correct field type
    Int_t fType = fFieldPar->GetType();
    if (fType == 0) fMagneticField = new CbmFieldConst(fFieldPar);
    else if (fType == 1)
      fMagneticField = new CbmFieldMap(fFieldPar);
    else if (fType == 2)
      fMagneticField = new CbmFieldMapSym2(fFieldPar);
    else if (fType == 3)
      fMagneticField = new CbmFieldMapSym3(fFieldPar);
    else if (fType == kTypeDistorted)
      fMagneticField = new CbmFieldMapDistorted(fFieldPar);
    else if (fType == 5)
      fMagneticField = new CbmFieldMapSym1(fFieldPar);
    else if (fType == 6)
      fMagneticField = new CbmBsField(fFieldPar);
    else
      cerr << "-W- FairRunAna::GetField: Unknown field type " << fType << endl;
    cout << "New field at " << fMagneticField << ", type " << fType << endl;
    // Initialise field
    if (fMagneticField) {
      fMagneticField->Init();
      fMagneticField->Print("");
    }
  }
  return fMagneticField;
}


ClassImp(CbmFieldCreator)
