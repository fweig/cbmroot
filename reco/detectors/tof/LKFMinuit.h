#ifndef LKFMinuit_h
#define LKFMinuit_h

#include "TFitter.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TRandom.h"
#include <TGraph2D.h>
#include <TGraph2DErrors.h>
#include <TMath.h>
#include <TPolyLine3D.h>

#include <iostream>

#include <Math/Vector3D.h>


class TFitter;

class LKFMinuit {
public:  // öffentlich
  inline static LKFMinuit* Instance() { return fInstance; }

  LKFMinuit();  // der Default-Konstruktor
  int DoFit(TGraph2DErrors* gr, double pStart[]);
  int Initialize();
  double SumDistance2(double par[]);
  double distance2(double x, double y, double z, double* p);
  double distance2err(double x, double y, double z, double ex, double ey, double ez, double* p);
  void line(double t, double* p, double& x, double& y, double& z);

  inline double* GetParFit() { return fparFit; }
  inline double GetChi2() { return fChi2; }
  inline double GetChi2DoF() { return fChi2DoF; }

private:
  static LKFMinuit* fInstance;

  static TGraph2DErrors* fgr;
  static TFitter* fMyFit;
  double fparFit[4];
  double fChi2;
  double fChi2DoF;
  double myFunction(double);
  static void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg);
};

#endif
