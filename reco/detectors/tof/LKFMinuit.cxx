/** @file LKFMinuit.cxx
  * @copyright Copyright (C) 2015-2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

#include "LKFMinuit.h"

#include <Logger.h>

#include <TFitter.h>
#include <TGraph2D.h>
#include <TMath.h>
#include <TPolyLine3D.h>

#include <Math/Vector3D.h>

using namespace ROOT::Math;

static LKFMinuit* LKF_obj;
LKFMinuit* LKFMinuit::fInstance = 0;
TFitter* LKFMinuit::fMyFit      = 0;
TGraph2DErrors* LKFMinuit::fgr  = 0;

int LKFMinuit::Initialize()
{
  LOG(info) << "LKFMinuit::Initialize ";
  fMyFit = new TFitter(2);
  fMyFit->SetFCN(LKFMinuit::minuitFunction);

  Double_t arglist[10];
  arglist[0] = -2;
  fMyFit->ExecuteCommand("SET PRINT", arglist, 1);
  fMyFit->ExecuteCommand("SIMPLEX", 0, 0);
  return 0;
}

int LKFMinuit::DoFit(TGraph2DErrors* gr, double pStart[])
{
  fgr = gr;
  //TFitter* min = new TFitter(2);
  TFitter* min = fMyFit;
  if (NULL == min) LOG(fatal) << "DoFit: no TFitter specified!";

  min->SetObjectFit(gr);

  //double pStart[4] = {0.,0.,0.,0.};
  min->SetParameter(0, "x0", pStart[0], 0.1, -10000., 10000.);
  min->SetParameter(1, "Ax", pStart[1], 0.01, -10., 10.);
  min->SetParameter(2, "y0", pStart[2], 0.1, -10000.0, 10000.);
  min->SetParameter(3, "Ay", pStart[3], 0.01, -10., 10.);

  Double_t arglist[10];
  arglist[0] = 10000;  // number of function calls
  arglist[1] = 0.001;  // tolerance
  min->ExecuteCommand("MIGRAD", arglist, 2);
  arglist[0] = 0;
  arglist[1] = 0;
  arglist[2] = 0;
  min->ExecuteCommand("SET NOWarnings", arglist, 3);  //turn off warning messages
  //if (minos) min->ExecuteCommand("MINOS",arglist,0);
  int nvpar, nparx;
  double amin, edm, errdef;
  min->GetStats(amin, edm, errdef, nvpar, nparx);
  fChi2    = amin;
  fChi2DoF = amin / (double) nvpar;
  //min->PrintResults(1,amin);
  // get fit parameters
  //double parFit[4];
  for (int i = 0; i < 4; ++i)
    fparFit[i] = min->GetParameter(i);

  return 0;
}

double LKFMinuit::myFunction(double /*par*/)
{
  double result = 0;

  return result;
}

// Temporary add on
//Fitting of a TGraph2D with a 3D straight line
//
// run this macro by doing:
//
// root>.x line3Dfit.C+
//
//Author: L. Moneta
//

// define the parameteric line equation
void LKFMinuit::line(double t, double* p, double& x, double& y, double& z)
{
  // a parameteric line is define from 6 parameters but 4 are independent
  // x0,y0,z0,z1,y1,z1 which are the coordinates of two points on the line
  // can choose z0 = 0 if line not parallel to x-y plane and z1 = 1;
  x = p[0] + p[1] * t;
  y = p[2] + p[3] * t;
  z = t;
}

// calculate distance line-point
double LKFMinuit::distance2(double x, double y, double z, double* p)
{
  // distance line point is D= | (xp-x0) cross  ux |
  // where ux is direction of line and x0 is a point in the line (like t = 0)
  XYZVector xp(x, y, z);
  XYZVector x0(p[0], p[2], 0.);
  XYZVector x1(p[0] + p[1], p[2] + p[3], 1.);
  XYZVector u = (x1 - x0).Unit();
  double d2   = ((xp - x0).Cross(u)).Mag2();
  return d2;
}

// calculate distance line-point with errors
double LKFMinuit::distance2err(double x, double y, double z, double ex, double ey, double ez, double* p)
{
  // distance line point is D= | (xp-x0) cross  ux |
  // where ux is direction of line and x0 is a point in the line (like t = 0)
  XYZVector xp(x, y, z);
  XYZVector x0(p[0], p[2], 0.);
  XYZVector x1(p[0] + p[1], p[2] + p[3], 1.);
  XYZVector u  = (x1 - x0).Unit();
  XYZVector xr = xp - x0;
  XYZVector D  = xr.Cross(u);
  //   double d2 = D.Mag2();
  double d2e = TMath::Power(D.X(), 2) / ex / ex;
  d2e += TMath::Power(D.Y(), 2) / ey / ey;
  d2e += TMath::Power(D.Z(), 2) / ez / ez;
  return d2e;
  /*
   // previous inconsistent version ...
   //XYZVector v = (xp-x0).Unit();
   //XYZVector xpe(ex,ey,ez); 
   XYZVector xpe(ex,ey,0.); 
   //   v *= xpe.Dot(v);
   //double d2e = 1./(xpe.Cross(u)).Mag2();
   XYZVector xre(xr.X(),xr.Y(),0.);  
   double  er = TMath::Abs( (xre.Unit()).Dot(xpe) ); // error in xr direction
   er = TMath::Max(er, 1.E-1); // prevent dividing by zero, avoid too strong weights 
   double d2e = 1./ er / er; 
   //double d2e = 1.;
   //XYZVector v = xr.Unit();
   //std::cout << "distance2err: d2 "<<d2<<", d2e "<<d2e<<"\t "<<v.X()<<"\t"<<v.Y()<<"\t"<<v.Z()<<"\t "<<u.X()<<"\t"<<u.Y()<<"\t"<<u.Z()<<std::endl;  
   return d2*d2e;
   */
}

bool first = true;
double LKFMinuit::SumDistance2(double par[])
{
  // the TGraph must be a global variable
  if (NULL == fgr) LOG(fatal) << "Invalid TGraph2Errors";
  TGraph2DErrors* gr = fgr;  //dynamic_cast<TGraph2D*>( (TVirtualFitter::GetFitter())->GetObjectFit() );
  assert(gr != 0);
  double* x   = gr->GetX();
  double* y   = gr->GetY();
  double* z   = gr->GetZ();
  double* ex  = gr->GetEX();
  double* ey  = gr->GetEY();
  double* ez  = gr->GetEZ();
  int npoints = gr->GetN();
  double sum  = 0;
  for (int i = 0; i < npoints; ++i) {
    //double  d = distance2(x[i],y[i],z[i],par);
    double d = distance2err(x[i], y[i], z[i], ex[i], ey[i], ez[i], par);
    sum += d;
    //#ifdef DEBUG
    if (0)
      if (first)
        std::cout << " -D- LKFMinuit::SumDistance2: point " << i << "\t" << x[i] << "\t" << ex[i] << "\t" << y[i]
                  << "\t" << ey[i] << "\t" << z[i] << "\t" << ez[i] << "\t" << std::sqrt(d) << std::endl;
    //#endif
  }
  if (0)
    if (first) std::cout << " -D- LKFMinuit::SumDistance2: Total sum2 = " << sum << std::endl;
  first = false;
  return sum;
}


void LKFMinuit::minuitFunction(int& /*nDim*/, double* /*gout*/, double& result, double par[], int /*flg*/)
{
  //    result = LKF_obj->myFunction(par[0]);
  result = LKF_obj->SumDistance2(par);
}

LKFMinuit::LKFMinuit()
  :  // fgr(NULL),
  //   fMyFit(NULL),
  fChi2(0.)
  , fChi2DoF(0.)
{
  //std::cout << "LKFMinuit at " << this << std::endl;
  LKF_obj = this;
  if (!fInstance) fInstance = this;
}
