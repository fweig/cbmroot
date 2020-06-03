#include "LxParall.h"
#include "Lx.h"
#include <sys/time.h>
#include <iostream>
#include <omp.h>

using namespace std;

ClassImp(LxParallFinder)

LxParallFinder::LxParallFinder() : generateInvMass(false), generateBackground(false), generateChi2(false),
  linkWithSts(true), useMCPInsteadOfHits(false), calcMiddlePoints(true), cutCoeff(4.0), ev_start(0), ev_end(0),
  hitFileName("")
{
}

class FinderCapsule
{
  friend class LxParallFinder;
public:
  FinderCapsule() : finder(0) {}
  ~FinderCapsule()
  {
    if (finder)
    {
      finder->FinishTask();
      delete finder;
    }
  }

private:
    LxFinder* finder;
    FinderCapsule(const FinderCapsule&);
    FinderCapsule& operator=(const FinderCapsule&);
};

//#define LXPARALL_SEPARATE_SINGLE

Double_t LxParallFinder::Reconstruct(Int_t numThreads)
{
  omp_set_num_threads(numThreads);

  timeval bTime, eTime;
  gettimeofday(&bTime, 0);

  Int_t i;
  Int_t createdFinderInstances = 0;
  Int_t totalRunTime = 0;
  FinderCapsule finderCapsule;

#ifdef LXPARALL_SEPARATE_SINGLE
  if (1 == numThreads)
  {
    for (i = ev_start; i < ev_end; ++i)
    {
      if (0 == finderCapsule.finder)
      {
        finderCapsule.finder = new LxFinder;
        finderCapsule.finder->SetLinkWithSts(linkWithSts);
        finderCapsule.finder->SetUseMCPInsteadOfHits(useMCPInsteadOfHits);
        finderCapsule.finder->SetCalcMiddlePoints(calcMiddlePoints);
        finderCapsule.finder->SetCutCoeff(cutCoeff);
        finderCapsule.finder->SetGenBackground(generateBackground);
        finderCapsule.finder->SetGenInvMass(generateInvMass);
        finderCapsule.finder->SetVerbosity(0);
        finderCapsule.finder->SetParallMode(true);
        finderCapsule.finder->SetHitFileName(hitFileName);
        finderCapsule.finder->Init();
        createdFinderInstances++;
      }

      finderCapsule.finder->ReadEvent(i);
      Int_t runTime = 0;
      finderCapsule.finder->Exec(reinterpret_cast<Option_t*> (&runTime));
      totalRunTime += runTime;
    }
  }
  else
  {
#endif//LXPARALL_SEPARATE_SINGLE
#pragma omp parallel for firstprivate(finderCapsule)
  for (i = ev_start; i < ev_end; ++i)
  {
    if (0 == finderCapsule.finder)
    {
      finderCapsule.finder = new LxFinder;
      finderCapsule.finder->SetLinkWithSts(linkWithSts);
      finderCapsule.finder->SetUseMCPInsteadOfHits(useMCPInsteadOfHits);
      finderCapsule.finder->SetCalcMiddlePoints(calcMiddlePoints);
      finderCapsule.finder->SetCutCoeff(cutCoeff);
      finderCapsule.finder->SetGenBackground(generateBackground);
      finderCapsule.finder->SetGenInvMass(generateInvMass);
      finderCapsule.finder->SetVerbosity(0);
      finderCapsule.finder->SetParallMode(true);
      finderCapsule.finder->SetHitFileName(hitFileName);
      finderCapsule.finder->Init();
#pragma omp atomic
      createdFinderInstances++;
    }

    finderCapsule.finder->ReadEvent(i);
    Int_t runTime = 0;
    finderCapsule.finder->Exec(reinterpret_cast<Option_t*> (&runTime));
#pragma omp atomic
    totalRunTime += runTime;
  }
#ifdef LXPARALL_SEPARATE_SINGLE
  }
#endif//LXPARALL_SEPARATE_SINGLE

  gettimeofday(&eTime, 0);
  int exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;
  cout << "Execution duration was: " << exeDuration << endl;
  cout << "Created finder instances: " << createdFinderInstances << endl;
  cout << "Total run time: " << totalRunTime << endl;
  Double_t result = totalRunTime;
  result /= (ev_end - ev_start) * numThreads;
  cout << "Average run time: " << result << endl;
  return result;
}
