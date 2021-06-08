/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: E.Kryshen, Denis Bertini [committer], Florian Uhlig, Mikhail Ryzhinskiy, Mohammad Al-Turany */

/** CbmGeoMuchPar class
 * @author  E.Kryshen <e.kryshen@gsi.de>
 * @version 1.0
 * @since   12.02.08
 *
 * @author  A.Kiseleva
 * @version 0.0
 * @since   13.04.06
 * 
 *  Class for geometry parameters of MUon CHambers
 *
 */

#ifndef CBMGEOMUCHPAR_H
#define CBMGEOMUCHPAR_H

#include <FairParGenericSet.h>  // for FairParGenericSet

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Bool_t, Int_t
#include <TObjArray.h>   // for TObjArray

class FairParamList;

class CbmGeoMuchPar : public FairParGenericSet {
public:
  TObjArray* fGeoSensNodes;  // List of FairGeoNodes for sensitive volumes
  TObjArray* fGeoPassNodes;  // List of FairGeoNodes for passive volumes
  TObjArray* fStations;

  CbmGeoMuchPar(const char* name = "CbmGeoMuchPar", const char* title = "Much Geometry Parameters",
                const char* context = "TestDefaultContext");
  ~CbmGeoMuchPar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  TObjArray* GetGeoSensitiveNodes() { return fGeoSensNodes; }
  TObjArray* GetGeoPassiveNodes() { return fGeoPassNodes; }
  TObjArray* GetStations() { return fStations; }
  Int_t GetNStations() const { return fStations->GetEntriesFast(); }

private:
  CbmGeoMuchPar(const CbmGeoMuchPar&);
  CbmGeoMuchPar& operator=(const CbmGeoMuchPar&);

  ClassDef(CbmGeoMuchPar, 1)
};

#endif /* !CBMGEOMUCHPAR_H */
