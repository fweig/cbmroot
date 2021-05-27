/** @file CbmL1MuchTrack.h
  * @copyright Copyright (C) 2007 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Sergey Gorbunov [orginator] **/

#ifndef CBM_L1_MuchTrack_h
#define CBM_L1_MuchTrack_h

#include "CbmKFTrackInterface.h"
#include "CbmL1MuchHit.h"

#include <vector>

class CbmStsTrack;


class CbmL1MuchTrack : public CbmKFTrackInterface {
public:
  CbmL1MuchTrack() {}
  ~CbmL1MuchTrack() {}

  double* GetTrack() { return T; }
  double* GetCovMatrix() { return C; }
  double& GetRefChi2() { return chi2; }
  int& GetRefNDF() { return NDF; }
  double GetMass() { return 0.1057; }
  bool IsElectron() { return 0; }
  int GetNOfHits() { return vHits.size(); }
  CbmKFHit* GetHit(int i) { return vHits[i]; }

  void SetStsTrack(CbmStsTrack* track);

  double T[6], C[15], chi2;
  int NDF;
  std::vector<CbmL1MuchHit*> vHits;
  int NHits, NMissed, NMissedStations;
  bool ok;
  bool stopped;
  int StsID;

  static bool Compare(const CbmL1MuchTrack* p1, const CbmL1MuchTrack* p2)
  {
    return (p1->NHits > p2->NHits) || (p1->NHits == p2->NHits) && (p1->chi2 < p2->chi2);
  }

public:
  ClassDef(CbmL1MuchTrack, 1);
};

#endif
