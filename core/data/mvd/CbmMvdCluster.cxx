// -------------------------------------------------------------------------
// -----                CbmMvdCluster source file                  -----

// -------------------------------------------------------------------------

#include "CbmMvdCluster.h"

using std::map;
using std::pair;

// -----   Default constructor   -------------------------------------------
CbmMvdCluster::CbmMvdCluster() : CbmCluster(), fPixelMap(), fRefId(-1), fClusterCharge(0) {}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMvdCluster::CbmMvdCluster(const CbmMvdCluster& rhs) : CbmCluster(), fPixelMap(), fRefId(-1), fClusterCharge(0)
{
  fPixelMap      = rhs.fPixelMap;
  fRefId         = rhs.fRefId;
  fClusterCharge = rhs.fClusterCharge;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdCluster::~CbmMvdCluster() {}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdCluster::SetPixelMap(map<pair<Int_t, Int_t>, Int_t> PixelMap)
{
  fPixelMap = PixelMap;
  for (map<pair<Int_t, Int_t>, Int_t>::iterator iter = fPixelMap.begin(); iter != fPixelMap.end(); iter++)
    fClusterCharge += iter->second;
}

ClassImp(CbmMvdCluster)
