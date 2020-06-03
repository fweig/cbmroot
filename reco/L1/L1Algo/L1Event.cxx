#include "L1Event.h"
//#include <vector>
// #include "L1Track.h"
//#include "L1StsHit.h"

void L1Event::Clear(){

  fStsHit.clear();
}

void L1Event::TopoReconstructor(){
   
  fTopoReconstructor.Init(fKFPTrack,fKFPTrack);
  fTopoReconstructor.ReconstructPrimVertex();
  fTopoReconstructor.SortTracks();
  fTopoReconstructor.ReconstructParticles();
  //cout<<"fTopoReconstructor "<<fTopoReconstructor.GetParticles().size();
}

