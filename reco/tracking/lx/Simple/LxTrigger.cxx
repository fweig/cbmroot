#include "Lx.h"
#include "TDatabasePDG.h"

using namespace std;

bool LxFinder::Trigger()
{
  bool trigging = false;

  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)
  {
    LxTrack* first = *i;

    if (first->clone)
      continue;

    list<LxTrack*>::iterator j = i;
    ++j;

    for (; j != caSpace.tracks.end(); ++j)
    {
      LxTrack* second = *j;

      if (second->clone)
        continue;

      LxRay* firstRay = first->rays[0];
      LxPoint* firstPoint = firstRay->end;
      LxRay* secondRay = second->rays[0];
      LxPoint* secondPoint = secondRay->end;

      scaltype firstParticleCharge = firstRay->tx - firstPoint->x / firstPoint->z;
      scaltype secondParticleCharge = secondRay->tx - secondPoint->x / secondPoint->z;
      bool distanceOk = true;

      if (pPtCut)
      {
        scaltype deltaX = firstPoint->x - secondPoint->x;
        scaltype deltaY = firstPoint->y - secondPoint->y;
        scaltype distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        distanceOk = distance >= 50.0;
      }

      first->distanceOk = first->distanceOk || distanceOk;
      second->distanceOk = second->distanceOk || distanceOk;
      bool oppCharged = firstParticleCharge * secondParticleCharge < 0;
      first->oppCharged = first->oppCharged || oppCharged;
      second->oppCharged = second->oppCharged || oppCharged;
      bool triggering = distanceOk && oppCharged;
      first->triggering = first->triggering || triggering;
      second->triggering = second->triggering || triggering;
      trigging = trigging || triggering;
    }
  }

#ifdef MAKE_TRIGGERING_HISTOS
  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)
  {
    LxTrack* recoTrack = *i;

    if (recoTrack->clone)
      continue;

    LxMCTrack* mcTrack = recoTrack->mcTrack;
    string particleName(mcTrack ? TDatabasePDG::Instance()->GetParticle(mcTrack->pdg)->GetName() : "ghost");

    if (!particleName.empty() && (particleName.back() == '+' || particleName.back() == '-'))
      particleName.pop_back();

    map<string, unsigned int>::iterator j = particlesCountAll.find(particleName);

    if (j != particlesCountAll.end())
      ++(j->second);
    else
      particlesCountAll[particleName] = 1;

    if (recoTrack->distanceOk)
    {
      j = particlesCountDist.find(particleName);

      if (j != particlesCountDist.end())
        ++(j->second);
      else
        particlesCountDist[particleName] = 1;
    }

    if (recoTrack->oppCharged)
    {
      j = particlesCountSign.find(particleName);

      if (j != particlesCountSign.end())
        ++(j->second);
      else
        particlesCountSign[particleName] = 1;
    }

    if (recoTrack->triggering)
    {
      j = particlesCountTrig.find(particleName);

      if (j != particlesCountTrig.end())
        ++(j->second);
      else
        particlesCountTrig[particleName] = 1;
    }

    if (0 == mcTrack)
      continue;

    if (13 != mcTrack->pdg && -13 != mcTrack->pdg)
      continue;

    triggeringAllTracksVertices->Fill(mcTrack->z);// Save arbitrary track info.

    if (recoTrack->distanceOk)
      triggeringDistTracksVertices->Fill(mcTrack->z);// Save track fulfilling distance criterion info.

    if (recoTrack->oppCharged)
      triggeringSignTracksVertices->Fill(mcTrack->z);// Save track fulfilling signs criterion info.

    if (recoTrack->triggering)
      triggeringTrigTracksVertices->Fill(mcTrack->z);// Save track fulfilling the both criteria above (triggering track) info.
  }
#endif//MAKE_TRIGGERING_HISTOS

  return trigging;
}
