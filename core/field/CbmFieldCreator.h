/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Mohammad Al-Turany [committer] */

// -------------------------------------------------------------------------
// -----                    CbmFieldCreator header file                  -----
// -----                Created 15/01/07  by M. Al-Turany              -----
// -------------------------------------------------------------------------


#ifndef CBMFIELDCREATOR_H
#define CBMFIELDCREATOR_H

#include <FairFieldFactory.h>  // for FairFieldFactory

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

class CbmFieldPar;
class FairField;

class CbmFieldCreator : public FairFieldFactory {

public:
  CbmFieldCreator();
  virtual ~CbmFieldCreator();
  virtual FairField* createFairField();
  virtual void SetParm();
  ClassDef(CbmFieldCreator, 1);

protected:
  CbmFieldPar* fFieldPar;

private:
  CbmFieldCreator(const CbmFieldCreator&);
  CbmFieldCreator& operator=(const CbmFieldCreator&);
};
#endif  //CBMFIELDCREATOR_H
