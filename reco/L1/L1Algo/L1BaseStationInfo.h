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
#include "L1Def.h"
#include "L1Parameters.h"
#include "L1Station.h"

// FairRoot
#include "FairField.h"

// Root
#include "TMatrixD.h"
#include "TVectorD.h"

// C++ std
#include <bitset>
#include <iomanip>
#include <string>
#include <functional>
//#include <cmath>

/// A base class which provides interface to L1Algo station geometry

class L1BaseStationInfo {
private:
  enum
  {  // Here we list all the fields, which must be initialized by user
    // Basic fields initialization
    kEStationID,
    kEXmax,
    kEYmax,
    // L1Station initialization
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
    // The last item is equal to the number of bits in fInitFlags
    kEND
  };

public:
  L1BaseStationInfo()
  {
    std::cout << ">>>>>> L1BaseStationInfo: Constructor called\n";  // Temporary
  }
  virtual ~L1BaseStationInfo()
  {
    std::cout << ">>>>>> L1BaseStationInfo: Destructor called\n";  // Temporary
  }

  //-------------------------------------------------------------------------------------------------------//
  //    Basic methods                                                                                      //
  //-------------------------------------------------------------------------------------------------------//

  /// Returns the name of the station (TODO: connect official naming)
  std::string GetTypeName() const { return fTypeName; }
  /// Checks if all the necessary fields are initialized by user
  bool IsInitialized() const { return fInitFlags.size() == fInitFlags.count(); }
  /// Transfers all gathered data to L1Algo (TODO)
  void TransferL1Station()
  { /**********/
  }
  void TransferData()
  { /*********/
  }

  //-------------------------------------------------------------------------------------------------------//
  //    Interface for L1Station object initialization                                                      //
  //-------------------------------------------------------------------------------------------------------//

  //
  //  Setters
  //
  /// Sets station ID
  void SetStationID(int inID)
  {
    if (! fInitFlags[kEStationID]) {
      fStationID = inID;
      fInitFlags[kEStationID] = true;
    }
#ifndef FAST_CODE
    else {
      LOG(WARNING) << __FILE__ << ":" << __LINE__ << " Attempt of station ID redifinition\n";
    }
#endif // ! FAST_CODE
  }
  /// Sets type of station
  void SetStationType(int inType)
  {
    if (! fInitFlags[kEtype]) {
      fL1Station.type    = inType;
      fInitFlags[kEtype] = true;
    }
#ifndef FAST_CODE
    else {
      LOG(WARNING) << __FILE__ << ":" << __LINE__ << " Attempt of station type redifinition\n";
    }
#endif // ! FAST_CODE
  }
  // TODO: Temporary method, a type must be selected automatically in the derived classes
  void SetTimeInfo(int inTimeInfo)
  {
    fL1Station.timeInfo    = inTimeInfo;
    fInitFlags[kEtimeInfo] = true;
  }
  // ???

  /// Sets nominal z position of the station
  void SetZ(double inZ)
  {
    fL1Station.z = inZ; // setting simd vector of single-precision floats, which is passed to high performanced L1Algo
    fZPos = inZ; // setting precise value to use in field approximation etc
    fInitFlags[kEz] = true;
  }
  /// Sets min transverse size of the station
  void SetRmin(double inRmin)
  {
    fL1Station.Rmin    = inRmin;
    fInitFlags[kERmin] = true;
  }
  /// Sets max transverse size of the station
  void SetRmax(double inRmax)
  {
    fL1Station.Rmax    = inRmax;
    fInitFlags[kERmax] = true;
  }

  /// Sets station thickness and radiational length
  void SetMaterial(double inThickness, double inRL)
  {
#ifndef L1_NO_ASSERT  // check for zero denominator
    L1_ASSERT(inRL, "Attempt of entering zero inRL (radiational length) value");
#endif
    fL1Station.materialInfo.thick       = inThickness;
    fL1Station.materialInfo.RL          = inRL;
    fL1Station.materialInfo.RadThick    = fL1Station.materialInfo.thick / fL1Station.materialInfo.RL;
    fL1Station.materialInfo.logRadThick = log(fL1Station.materialInfo.RadThick);
    fInitFlags[kEmaterialInfoThick]     = true;
    fInitFlags[kEmaterialInfoRL]        = true;
  }

  /// Sets arrays of the approximation coefficients for the field x, y and z-components, respectively
  void SetFieldSlice(const double* Cx, const double* Cy, const double* Cz)
  {
    for (int idx = 0; idx < L1Parameters::kMaxNFieldApproxCoefficients; ++idx) {
      fL1Station.fieldSlice.cx[idx] = Cx[idx];
      fL1Station.fieldSlice.cy[idx] = Cy[idx];
      fL1Station.fieldSlice.cz[idx] = Cz[idx];
    }
    fInitFlags[kEfieldSlice]      = true;
  }
  /// Sets arrays of the approcimation 
  /// \param getField A user function, which gets a xyz array of position coordinates and fills B array
  ///                 of magnetic field components in position
  void SetFieldSlice(const std::function<void(const double (&xyz)[3], double (&B)[3])>& getFieldValue)
  {
    std::cout << ">>>>>>> CALL: SetFieldSlice\n";
#ifndef L1_NO_ASSERT  // check for zero denominator
    L1_ASSERT(fInitFlags[kEz], "Attempt to set magnetic field slice before setting z position of the station");
    L1_ASSERT(fInitFlags[kEXmax], "Attempt to set magnetic field slice before setting Xmax size of the station");
    L1_ASSERT(fInitFlags[kEYmax], "Attempt to set magnetic field slice before setting Ymax size of the station");
#endif
    constexpr int M = L1Parameters::kMaxFieldApproxPolynomialOrder;
    constexpr int N = L1Parameters::kMaxNFieldApproxCoefficients;
    constexpr int D = 3; ///> number of dimensions

    // SLE initialization
    double A[N][N + D] = {}; // augmented matrix
    double dx = (fXmax / N / 2 < 1.) ? fXmax / N / 4. : 1.;
    double dy = (fYmax / N / 2 < 1.) ? fYmax / N / 4. : 1.;
    
    for (double x = -fXmax; x <= fXmax; x += dx) {
      for (double y = -fYmax; y <= fYmax; y += dy) {
        double r = sqrt(fabs(x * x / fXmax / fXmax + y / fYmax * y / fYmax));
        if (r > 1.) {
          continue;
        }
        double p[D] = {x, y, fZPos};
        double B[D] = {};
        getFieldValue(p, B);

        double m[N] = {1};
        m[0] = 1;
        for (int i = 1; i <= M; ++i) {
          int k = (i - 1) * (i) / 2;
          int l = i * (i + 1) / 2;
          for (int j = 0; j < i; ++j) {
            m[l + j] = x * m[k + j];
          }
          m[l + i] = y * m[k + i - 1];
        }

        double w = 1. / (r * r + 1);
        for (int i = 0; i < N; ++i) {
          // fill the left part of the matrix 
          for (int j = 0; j < N; ++j) {
            A[i][j] += w * m[i] * m[j];
          }
          // fill the right part of the matrix
          for (int j = 0; j < D; ++j) {
            A[i][N + j] += w * B[j] * m[i];
          }
        }
      }
    }

    // SLE solution (Gaussian elimination)
    //   
    for (int kCol = 0; kCol < N - 1; ++kCol) {
      for (int jRow = kCol + 1; jRow < N; ++jRow) {
        double factor = A[jRow][kCol] / A[kCol][kCol];
        for (int iCol = kCol; iCol < N + D; ++iCol) {
          A[jRow][iCol] -= factor * A[kCol][iCol];
        }
      }
    }
    for (int kCol = N - 1; kCol > 0; --kCol) {
      for (int jRow = kCol - 1; jRow >= 0; --jRow) {
        double factor = A[jRow][kCol] / A[kCol][kCol];
        for (int iCol = kCol; iCol < N + D; ++iCol) {
          A[jRow][iCol] -= factor * A[kCol][iCol];
        }
      }
    }

    for (int j = 0; j < N; ++j) {
      fL1Station.fieldSlice.cx[j] = A[j][N + 0] / A[j][j];
      fL1Station.fieldSlice.cy[j] = A[j][N + 1] / A[j][j];
      fL1Station.fieldSlice.cz[j] = A[j][N + 2] / A[j][j];
    }

    fInitFlags[kEfieldSlice] = true;
  }

  /// Sets stereo angles and sigmas for front and back strips, automatically set frontInfo, backInfo,
  /// XYInfo, xInfo and yInfo
  void SetFrontBackStripsGeometry(double f_phi, double f_sigma, double b_phi, double b_sigma)
  {
    // TODO: may be is it better to define separate setters for these values and then calculate the
    //       rest somewhere below?
    // TODO: move into .cxx file

    //----- Original code from L1Algo ---------------------------------------------------------------------//
    double c_f = cos(f_phi);
    double s_f = sin(f_phi);
    double c_b = cos(b_phi);
    double s_b = sin(b_phi);

    // NOTE: Here additional double variables are used to save the precission

    fL1Station.frontInfo.cos_phi = c_f;
    fL1Station.frontInfo.sin_phi = s_f;
    fL1Station.frontInfo.sigma2  = f_sigma * f_sigma;

    fL1Station.backInfo.cos_phi = c_b;
    fL1Station.backInfo.sin_phi = s_b;
    fL1Station.backInfo.sigma2  = b_sigma * b_sigma;

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
  /// Sets maximum distance between station center and its edge in x direction
  void SetXmax(double aSize)
  {
    fXmax = aSize;
    fInitFlags[kEXmax] = true;
  }
  /// Sets maximum distance between station center and its edge in y direction
  void SetYmax(double aSize)
  {
    fYmax = aSize;
    fInitFlags[kEYmax] = true;
  }

  //
  // Getters
  //

  /// Gets station ID
  int GetStationID() const { return fStationID; }
  /// Gets station type
  int GetStationType() const { return fL1Station.type; }
  /// Gets SIMD vectorized z position of the station
  fvec GetZsimdVec() const { return fL1Station.z; }
  /// Gets double precised z position of the station
  double GetZdouble() const { return fZPos; }
  /// Gets min transverse size of the station
  fvec GetRmin() const { return fL1Station.Rmin; }
  /// Gets max transverse size of the station
  fvec GetRmax() const { return fL1Station.Rmax; }

  /// Gets material thickness
  fvec GetMaterialThick() const { return fL1Station.materialInfo.thick; }
  // TODO: investigate if thick, RL and RadThick are useful, may be we should have only
  //       GetMateralLogRadThick?
  /// Gets the radiation length of the station material
  fvec GetMaterialRL() const { return fL1Station.materialInfo.RL; }  // TODO: -//-
  /// Gets the relative material thickness in units of the radiational length
  fvec GetMaterialRadThick() const { return fL1Station.materialInfo.RadThick; }  // TODO: Rename?
  /// Gets log of the relative material thickness in units of the radiational length
  fvec GetMaterialLogRadThick() const { return fL1Station.materialInfo.logRadThick; }

  /// Gets a coefficient with idx for the field x-component approximation
  fvec GetFieldSliceCx(int idx) const { return fL1Station.fieldSlice.cx[idx]; }
  /// Gets a coefficient with idx for the field y-component approximation
  fvec GetFieldSliceCy(int idx) const { return fL1Station.fieldSlice.cy[idx]; }
  /// Gets a coefficient with idx for the field z-component approximation
  fvec GetFieldSliceCz(int idx) const { return fL1Station.fieldSlice.cz[idx]; }

  /// Gets array of the coefficients for the field x-component approximation
  const fvec* GetFieldSliceCx() const { return fL1Station.fieldSlice.cx; }
  /// Gets array of the coefficients for the field y-component approximation
  const fvec* GetFieldSliceCy() const { return fL1Station.fieldSlice.cy; }
  /// Gets array of the coefficients for the field z-component approximation
  const fvec* GetFieldSliceCz() const { return fL1Station.fieldSlice.cz; }

  /// Gets maximum distance between station center and its edge in x direction
  double GetXmax() const { return fXmax; }
  /// Gets maximum distance between station center and its edge in y direction
  double GetYmax() const { return fYmax; }


  //-------------------------------------------------------------------------------------------------------//
  //    Other methods                                                                                      //
  //-------------------------------------------------------------------------------------------------------//

  /// Prints registered fields (TODO: tmp, may be one needs to wrap it into debug directives)
  void Print()
  {
    LOG(INFO) << "== L1Algo: station info ===========================================================";
    LOG(INFO) << "";
    LOG(INFO) << "    Station ID:              " << fStationID;
    LOG(INFO) << "  L1Station fields:";
    LOG(INFO) << "    Station type ID:         " << fL1Station.type;
    LOG(INFO) << "    z position:              " << fL1Station.z[0];
    LOG(INFO) << "    Rmin:                    " << fL1Station.Rmin[0];
    LOG(INFO) << "    Rmax:                    " << fL1Station.Rmax[0];
    LOG(INFO) << "    Thickness (X):           " << fL1Station.materialInfo.thick[0];
    LOG(INFO) << "    Radiational length (X0): " << fL1Station.materialInfo.RL[0];
    LOG(INFO) << "    X / X0:                  " << fL1Station.materialInfo.RadThick[0];
    LOG(INFO) << "    log(X / X0):             " << fL1Station.materialInfo.logRadThick[0];
    LOG(INFO) << "    Field approximation coefficients:";
    LOG(INFO) << "      idx         CX         CY         CZ";
    for (int idx = 0; idx < L1Parameters::kMaxNFieldApproxCoefficients; ++idx) {
      LOG(INFO) << std::setw(9) << std::setfill(' ') << idx << ' ' << std::setw(10) << std::setfill(' ')
                << fL1Station.fieldSlice.cx[idx][0] << ' ' << std::setw(10) << std::setfill(' ')
                << fL1Station.fieldSlice.cy[idx][0] << ' ' << std::setw(10) << std::setfill(' ')
                << fL1Station.fieldSlice.cz[idx][0];
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
    LOG(INFO) << "  Additiona fields:";
    LOG(INFO) << "    Xmax:                    " << fXmax;
    LOG(INFO) << "    Ymax:                    " << fYmax;
    LOG(INFO) << "";
    LOG(INFO) << "===================================================================================";
  }
  void Reset() {
    L1BaseStationInfo other;
    std::swap(*this, other);
  }

private:
  
  std::bitset<L1BaseStationInfo::kEND> fInitFlags;  ///< Class fileds initialization flags
  std::string fTypeName {"BASE"};  ///< Station type name (TODO: probably should be replaced with function of type)
  L1Station fL1Station {};  ///< L1Station structure, describes a station in L1Algo
  int fStationID {-1}; ///< Station ID
  double fXmax {0};  ///< Maximum distance between station center and its edge in x direction
  double fYmax {0};  ///< Maximum distance between station center and its edge in y direction
  double fZPos {0};  ///< z position of the station in double precision, used in field approximation
  // TODO (!!!!) MUST THINK ABOUT THIS OBJECT LIFE TIME: will it be better to copy
  //             or to move it to the core? If the second, what should we do with
  //             getters? Do we need to lock them, when the fL1Station object will
  //             be transfered?
};


#endif  // L1BaseStationInfo_h
