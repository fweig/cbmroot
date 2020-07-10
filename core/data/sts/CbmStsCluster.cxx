/**
 ** \file CbmStsCluster.cxx
 ** \author V.Friese <v.friese@gsi.de>
 ** \since 28.08.06
 **
 ** Updated 25/06/2008 by R. Karabowicz.
 ** Updated 04/03/2014 by A. Lebedev <andrey.lebedev@gsi.de>
 ** Updated 10/06/2014 by V.Friese <v.friese@gsi.de>
 **/
#include "CbmStsCluster.h"

#include <sstream>  // for operator<<, basic_ostream, char_traits

using namespace std;


// --- Constructor
CbmStsCluster::CbmStsCluster()
  : CbmCluster()
  , fCharge(0.)
  , fSize(0)
  , fPosition(0.)
  , fPositionError(0.)
  , fTime(0.)
  , fTimeError(0.)
  , fIndex(-1) {}


// --- Destructor
CbmStsCluster::~CbmStsCluster() {}


// --- String output
string CbmStsCluster::ToString() const {
  stringstream ss;
  ss << "StsCluster: address " << GetAddress() << " | digis " << GetNofDigis()
     << " | charge " << fCharge << " | time " << fTime << " +- " << fTimeError
     << " | position " << GetPosition() << " | error " << GetPositionError()
     << " | Index " << fIndex << "\n  " << CbmCluster::ToString();
  return ss.str();
}


ClassImp(CbmStsCluster)
