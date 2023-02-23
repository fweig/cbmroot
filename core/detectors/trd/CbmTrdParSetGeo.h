/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Alexandru Bercuci */

#ifndef CBMTRDPARSETGEO_H
#define CBMTRDPARSETGEO_H

#include "CbmTrdParSet.h"  // for CbmTrdParSet

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Bool_t, Option_t

class CbmTrdParSetGeo : public CbmTrdParSet {
public:
  CbmTrdParSetGeo(const char* name = "CbmTrdParSetGeo", const char* title = "Trd Geometry Parameters",
                  const char* context = "TestDefaultContext");
  virtual ~CbmTrdParSetGeo(void);
  /** \brief Fill map with full geometrical description for each detector to be distributed to all processing modules. Refer to the constructor of \class CbmTrdParModGeo for the details of using this description.
   */
  bool Init();
  /** \brief Trigger loading alignment information for all nodes registered */
  bool LoadAlignVolumes();

  virtual void Print(Option_t* opt = "") const;

private:
  ClassDef(CbmTrdParSetGeo, 1)
};

#endif /* !CBMTRDPARSETGEO_H */
