/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1BaseStationInfo.h
 * @bried A base class for a L1 station interface
 * @since 18.12.2021
 *
 * The class is implemented to connect concrete experimental setup (CBM or BMN in CbmL1 
 * or BmnL1) with general L1Tracking algorithm. Each derived class must contain general
 * algorithms sutable for the particular station type.
 * 
 ***********************************************************************************************************/

#ifndef L1BaseStationInfo_h
#define L1BaseStationInfo_h 1

// L1 core
#include "L1Parameters.h"
#include "L1Station.h"
#include "L1Def.h"

// C++ std
#include <string>
#include <bitset>
#include <iomanip>
//#include <cmath>

class L1BaseStationInfo {
private:
  enum { // Here we list all the fields, which must be initialized by user
    /// v Basic fields initialization
    /// v L1Station initialization
    kEtype,
    kEtimeInfo,
    kEz,
    kERmin, 
    kERmax, 
    kEmaterialInfoThick, 
    kEmaterialInfoRL,
    kEfieldSlice,
    kEstripsFrontPhi,
    kEstripsFrontSigma,
    kEstripsBackPhi,
    kEstripsBackSigma,
    /// v The last item is equal to the number of bits in fInitFlags
    kEND
  };
  std::bitset<L1BaseStationInfo::kEND> fInitFlags; /// Set of 
  std::string fTypeName {"BASE"};
  L1Station fL1Station {}; 
    // TODO (!!!!) MUST THINK ABOUT THIS OBJECT LIFE TIME: will it be better to copy
    //             or to move it to the core? If the second, what should we do with 
    //             getters? Do we need to lock them, when the fL1Station object will 
    //             be transfered?

public:
  L1BaseStationInfo() {
    std::cout << ">>>>>> L1BaseStationInfo: Constructor called\n"; // Temporary
  }
  virtual ~L1BaseStationInfo() {
    std::cout << ">>>>>> L1BaseStationInfo: Destructor called\n"; // Temporary
  }

  ///-----------------------------------------------------------------------------------------------------///
  ///   Basic methods                                                                                     ///
  ///-----------------------------------------------------------------------------------------------------///
  
  std::string GetTypeName()   const { return fTypeName; }
  // Checks if all the necessary fields are initialized by user
  bool IsInitialized() const { return fInitFlags.size() == fInitFlags.count(); };
  // Transfers all gathered data to L1Algo
  void TransferL1Station() { 
    /**********/
  }
  void TransferData() { /*********/ }    

  ///-----------------------------------------------------------------------------------------------------///
  ///   Interface for L1Station object initialization                                                     ///
  ///-----------------------------------------------------------------------------------------------------///

  /// Setters

  // Sets type of station // TODO: Temporary method, must be selected automatically in the derived class
  void SetStationType(int inType)  { fL1Station.type = inType;         fInitFlags[kEtype] = true;}
  void SetTimeInfo(int inTimeInfo) { fL1Station.timeInfo = inTimeInfo; fInitFlags[kEtimeInfo] = true;}

  void SetZ(double inZ)        { fL1Station.z = inZ;        fInitFlags[kEz] = true; }
  void SetRmin(double inRmin)  { fL1Station.Rmin = inRmin;  fInitFlags[kERmin] = true; }
  void SetRmax(double inRmax)  { fL1Station.Rmax = inRmax;  fInitFlags[kERmax] = true; }

  // Sets material thickness and radiational length, calculates their ratio ("RadThick")
  // and its logarithm ("logRadThick")
  void SetMaterial(double inThickness, double inRL) {
#ifndef L1_NO_ASSERT // check for zero denominator
    L1_ASSERT(inRL, "Attempt of entering zero inRL (radiational length) value");
#endif 
    fL1Station.materialInfo.thick = inThickness;
    fL1Station.materialInfo.RL = inRL;
    fL1Station.materialInfo.RadThick = fL1Station.materialInfo.thick / fL1Station.materialInfo.RL;
    fL1Station.materialInfo.logRadThick = log(fL1Station.materialInfo.RadThick);
    fInitFlags[kEmaterialInfoThick] = true;
    fInitFlags[kEmaterialInfoRL] = true;
  }

  // Sets coefficients of the magnetic field approximation
  void SetFieldSlice(const double * Cx, const double * Cy, const double * Cz) {
    for (int idx = 0; idx < L1Parameters::kN_FS_COEFFS; ++idx) {
      fL1Station.fieldSlice.cx[idx] = Cx[idx];
      fL1Station.fieldSlice.cy[idx] = Cy[idx];
      fL1Station.fieldSlice.cz[idx] = Cz[idx];
      fInitFlags[kEfieldSlice] = true;
    }
  }

  // Sets stereo angles and sigmas for front and back strips, automatically set frontInfo, backInfo, 
  // XYInfo, xInfo and yInfo
  void SetFBStripsGeometry(double f_phi, double f_sigma, double b_phi, double b_sigma) {
    // TODO: may be is it better to define separate setters for these values and then calculate the 
    //       rest somewhere below?

    //----- Original code from L1Algo ---------------------------------------------------------------------//
    double c_f     = cos(f_phi);
    double s_f     = sin(f_phi);
    double c_b     = cos(b_phi);
    double s_b     = sin(b_phi);

    // NOTE: Here additional double variables are used to save the precission

    fL1Station.frontInfo.cos_phi = c_f;
    fL1Station.frontInfo.sin_phi = s_f;
    fL1Station.frontInfo.sigma2  = f_sigma * f_sigma;

    fL1Station.backInfo.cos_phi = c_b;
    fL1Station.backInfo.sin_phi = s_b;
    fL1Station.backInfo.sigma2  = b_sigma * b_sigma;
  
    if (fabs(b_phi - f_phi) < .1) {
      std::cout << "test";
    }

    //if (fabs(b_phi - f_phi) < .1) {
    //  double th  = b_phi - f_phi;
    //  double det = cos(th);
    //  det *= det;
    //  fL1Station.XYInfo.C00 = (s_b * s_b * f_sigma * f_sigma + s_f * s_f * b_sigma * b_sigma) / det;
    //  fL1Station.XYInfo.C10 = -(s_b * c_b * f_sigma * f_sigma + s_f * c_f * b_sigma * b_sigma) / det;
    //  fL1Station.XYInfo.C11 = (c_b * c_b * f_sigma * f_sigma + c_f * c_f * b_sigma * b_sigma) / det;
    //} else {
    //  double det = c_f * s_b - s_f * c_b;
    //  det *= det;
    //  fL1Station.XYInfo.C00 = (s_b * s_b * f_sigma * f_sigma + s_f * s_f * b_sigma * b_sigma) / det;
    //  fL1Station.XYInfo.C10 = -(s_b * c_b * f_sigma * f_sigma + s_f * c_f * b_sigma * b_sigma) / det;
    //  fL1Station.XYInfo.C11 = (c_b * c_b * f_sigma * f_sigma + c_f * c_f * b_sigma * b_sigma) / det;
    //}

    double det = (fabs(b_phi - f_phi) < 0.1) ? cos(b_phi - f_phi) : (c_f * s_b - s_f * c_b);
    det *= det;
    fL1Station.XYInfo.C00 = (s_b * s_b * f_sigma * f_sigma + s_f * s_f * b_sigma * b_sigma) / det;
    fL1Station.XYInfo.C10 = -(s_b * c_b * f_sigma * f_sigma + s_f * c_f * b_sigma * b_sigma) / det;
    fL1Station.XYInfo.C11 = (c_b * c_b * f_sigma * f_sigma + c_f * c_f * b_sigma * b_sigma) / det;

    fL1Station.xInfo.cos_phi = -s_f / (c_f * s_b - c_b * s_f);
    fL1Station.xInfo.sin_phi = s_b / (c_f * s_b - c_b * s_f);
    fL1Station.xInfo.sigma2  = fL1Station.XYInfo.C00;

    fL1Station.yInfo.cos_phi = c_b / (c_b * s_f - c_f * s_b);
    fL1Station.yInfo.sin_phi = -c_f / (c_b * s_f - c_f * s_b);
    fL1Station.yInfo.sigma2  = fL1Station.XYInfo.C11;
    //-----------------------------------------------------------------------------------------------------//
 
    fInitFlags[kEstripsFrontPhi]   = true;
    fInitFlags[kEstripsFrontSigma] = true;
    fInitFlags[kEstripsBackPhi]    = true;
    fInitFlags[kEstripsBackSigma]  = true;
  }

  /// Getters
  fvec GetZ()                    const { return fL1Station.z; }
  fvec GetRmin()                 const { return fL1Station.Rmin; }
  fvec GetRmax()                 const { return fL1Station.Rmax; }

  fvec GetMaterialThick()        const { return fL1Station.materialInfo.thick; }
    // TODO: investigate if thick, RL and RadThick are useful, may be we should have only 
    //       GetMateralLogRadThick?
  fvec GetMaterialRL()           const { return fL1Station.materialInfo.RL; }             // TODO: -//-
  fvec GetMaterialRadThick()     const { return fL1Station.materialInfo.RadThick; }       // TODO: -//-
  fvec GetMaterialLogRadThick()  const { return fL1Station.materialInfo.logRadThick; }

  // Methods to get particular element of Cx, Cy or Cz by its index
  fvec GetFieldSliceCx(int idx)  const { return fL1Station.fieldSlice.cx[idx]; }
  fvec GetFieldSliceCy(int idx)  const { return fL1Station.fieldSlice.cy[idx]; }
  fvec GetFieldSliceCz(int idx)  const { return fL1Station.fieldSlice.cz[idx]; }

  // Methods to get ptr to array of Cx, Cy or Cz
  const fvec * GetFieldSliceCx() const { return fL1Station.fieldSlice.cx; }
  const fvec * GetFieldSliceCy() const { return fL1Station.fieldSlice.cy; }
  const fvec * GetFieldSliceCz() const { return fL1Station.fieldSlice.cz; }

  // Methods to get strip layers geometry

  ///-----------------------------------------------------------------------------------------------------///
  ///   Other methods                                                                                     ///
  ///-----------------------------------------------------------------------------------------------------///

  void Print() { 
    LOG(INFO) << "== L1Algo: station info ===========================================================";
    LOG(INFO) << "";
    LOG(INFO) << "  L1Station fields:";
    LOG(INFO) << "    z position:              " << fL1Station.z[0] ;
    LOG(INFO) << "    Rmin:                    " << fL1Station.Rmin[0] ;
    LOG(INFO) << "    Rmax:                    " << fL1Station.Rmax[0] ;
    LOG(INFO) << "    Thickness (X):           " << fL1Station.materialInfo.thick[0] ;
    LOG(INFO) << "    Radiational length (X0): " << fL1Station.materialInfo.RL[0] ;
    LOG(INFO) << "    X / X0:                  " << fL1Station.materialInfo.RadThick[0] ;
    LOG(INFO) << "    log(X / X0):             " << fL1Station.materialInfo.logRadThick[0] ;
    LOG(INFO) << "    Field approximation coefficients:";
    LOG(INFO) << "      idx         CX         CY         CZ";
    for (int idx = 0; idx < L1Parameters::kN_FS_COEFFS; ++idx) {
      LOG(INFO) << std::setw(9)  << std::setfill(' ') << idx << ' '
                << std::setw(10) << std::setfill(' ') << fL1Station.fieldSlice.cx[idx][0] << ' '
                << std::setw(10) << std::setfill(' ') << fL1Station.fieldSlice.cy[idx][0] << ' '
                << std::setw(10) << std::setfill(' ') << fL1Station.fieldSlice.cz[idx][0];
    }
    LOG(INFO) << "    Strips geometry:";
    LOG(INFO) << "      Front:";
    LOG(INFO) << "        cos(phi):            " << fL1Station.frontInfo.cos_phi[0];
    LOG(INFO) << "        sin(phi):            " << fL1Station.frontInfo.sin_phi[0];
    LOG(INFO) << "        sigma2:              " << fL1Station.frontInfo.sigma2[0];
    LOG(INFO) << "      Back:";
    LOG(INFO) << "        cos(phi):            " << fL1Station.backInfo.cos_phi[0];
    LOG(INFO) << "        sin(phi):            " << fL1Station.backInfo.sin_phi[0];
    LOG(INFO) << "        sigma2:              " << fL1Station.backInfo.sigma2[0];
    LOG(INFO) << "      XY cov matrix:";
    LOG(INFO) << "        C00:                 " << fL1Station.XYInfo.C00[0];
    LOG(INFO) << "        C10:                 " << fL1Station.XYInfo.C10[0];
    LOG(INFO) << "        C11:                 " << fL1Station.XYInfo.C11[0];
    LOG(INFO) << "      X layer:";
    LOG(INFO) << "        cos(phi):            " << fL1Station.xInfo.cos_phi[0];
    LOG(INFO) << "        sin(phi):            " << fL1Station.xInfo.sin_phi[0];
    LOG(INFO) << "        sigma2:              " << fL1Station.xInfo.sigma2[0];
    LOG(INFO) << "      Y layer:";
    LOG(INFO) << "        cos(phi):            " << fL1Station.yInfo.cos_phi[0];
    LOG(INFO) << "        sin(phi):            " << fL1Station.yInfo.sin_phi[0];
    LOG(INFO) << "        sigma2:              " << fL1Station.yInfo.sigma2[0];
    LOG(INFO) << "";
    LOG(INFO) << "===================================================================================";
  }
  

};

#endif // L1BaseStationInfo_h
