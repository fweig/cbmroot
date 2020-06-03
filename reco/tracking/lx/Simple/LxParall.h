#ifndef LX_PARALL_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"
#include "TString.h"
#include "LxSettings.h"

class LxParallFinder
{
public:
  LxParallFinder();
  ~LxParallFinder() {;}
  
  Double_t Reconstruct(Int_t numThreads);

  void SetGenInvMass(bool value)
  {
    generateInvMass = value;
  }

  void SetGenBackground(bool value)
  {
    generateBackground = value;
  }

  void SetGenChi2(bool value)
  {
    generateChi2 = value;
  }

  void SetLinkWithSts(bool value)
  {
    linkWithSts = value;
  }

  void SetUseMCPInsteadOfHits(bool value)
  {
    useMCPInsteadOfHits = value;
  }

  void SetCalcMiddlePoints(bool value)
  {
    calcMiddlePoints = value;
  }

  void SetCutCoeff(scaltype value)
  {
    cutCoeff = value;
  }

  void SetStartEvent(Int_t n)
  {
    ev_start = n;
  }

  void SetEndEvent(Int_t n)
  {
    ev_end = n;
  }

  void SetHitFileName(TString v)
  {
    hitFileName = v;
  }

private:
  bool generateInvMass;
  bool generateBackground;
  bool generateChi2;
  bool linkWithSts;
  bool useMCPInsteadOfHits;
  bool calcMiddlePoints;
  scaltype cutCoeff;
  Int_t ev_start;
  Int_t ev_end;
  TString hitFileName;
ClassDef(LxParallFinder, 1);
};

#pragma GCC diagnostic warning "-Weffc++"

#endif//LX_PARALL_INCLUDED
