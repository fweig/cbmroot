#include "L1MCEvent.h"

int L1MCEvent::IsReconstructed()
{
  if(fRecoEvents.size()>0) return 1;
  else return 0;
}

int L1MCEvent::NClones()
{
  if(fRecoEvents.size()>1) {return fRecoEvents.size()-1;}
  else {return 0;}
}