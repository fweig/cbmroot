/* Copyright (C) 2008-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Christina Dritsa [committer], Florian Uhlig, Philipp Sitzmann */

// -------------------------------------------------------------------------
// -----                      CbmMvdCluster header file            -----
// -----
// -------------------------------------------------------------------------


/** CbmMvdCluster.h



**/


#ifndef CBMMVDCLUSTER_H
#define CBMMVDCLUSTER_H 1

#include "CbmCluster.h"        // for CbmCluster
#include "CbmMvdDetectorId.h"  // for CbmMvdDetectorId

#include <Rtypes.h>      // for ClassDef

#include <cstdint>
#include <map>      // for map
#include <utility>  // for pair


class CbmMvdCluster : public CbmCluster, public CbmMvdDetectorId {
public:
  /** Default constructor **/
  CbmMvdCluster();

  CbmMvdCluster(const CbmMvdCluster&);

  CbmMvdCluster& operator=(const CbmMvdCluster&) { return *this; };

  /** Destructor **/
  virtual ~CbmMvdCluster();

  /** Setters **/
  void SetPixelMap(std::map<std::pair<int32_t, int32_t>, int32_t> PixelMap);
  void SetRefId(int32_t RefId) { fRefId = RefId; };  //* stores the index to the global TClonesArray

  /** Accessors **/
  int16_t GetTotalDigisInCluster() { return fPixelMap.size(); };
  std::map<std::pair<int32_t, int32_t>, int32_t> GetPixelMap() { return fPixelMap; };
  int32_t GetStationNr() { return (int32_t) GetAddress() / 1000; };
  int32_t GetRefId() { return fRefId; };
  int32_t GetDetectorId() { return DetectorId(GetStationNr()); };
  int32_t GetSensorNr() { return (GetAddress() % 1000); };

  float GetClusterCharge() { return fClusterCharge; };

protected:
  std::map<std::pair<int32_t, int32_t>, int32_t> fPixelMap;
  int32_t fRefId;
  float fClusterCharge;

  ClassDef(CbmMvdCluster, 4);
};

#endif
