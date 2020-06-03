#ifndef LXTRACKANASEGMENTS_INCLUDED
#define LXTRACKANASEGMENTS_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "TString.h"

struct LxSimpleTrack;
class LxTrackAna;

class LxTrackAnaSegments
{
public:
  explicit LxTrackAnaSegments(LxTrackAna& o);
  void Init();
  void Finish();
  void BuildStatistics();
  void SetParticleType(TString v);

private:
  void StatForTrack(LxSimpleTrack* track);
  LxTrackAna& owner;
  Int_t stationsInAlgo;
};

#endif//LXTRACKANASEGMENTS_INCLUDED
