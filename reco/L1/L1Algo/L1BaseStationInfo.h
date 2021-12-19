/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/**************************************************************************************************************
 * @file L1BaseStationInfo.h
 * @bried A base class for a L1 station interface
 * @since 18.12.2021
 *
 * The class is implemented to connect concrete experimental setup (CBM or BMN in CbmL1 
 * or BmnL1) with general L1Tracking algorithm. Each derived class must contain general
 * algorithms sutable for the particular station type.
 * 
 *************************************************************************************************************/

#ifndef L1BaseStationInfo_h
#define L1BaseStationInfo_h 1

// L1 core
#include "L1Station.h"
#include "L1Def.h"

// C++ std
#include <string>
#include <bitset>
#include <iomanip>

class L1BaseStationInfo {
private:
  enum { // Here we list all the fields, which must be initialized by user
    /// v Basic fields initialization
    /// v L1Station initialization
    kEz, 
    kERmin, 
    kERmax, 
    kEmaterialInfoThick, 
    kEmaterialInfoRL,
    kEfieldSlice,
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

  ///-------------------------------------------------------------------------------------------------------///
  ///   Basic methods                                                                                       ///
  ///-------------------------------------------------------------------------------------------------------///
  
  std::string GetTypeName()   const { return fTypeName; }
  bool        IsInitialized() const { return fInitFlags.size() == fInitFlags.count(); };

  ///-------------------------------------------------------------------------------------------------------///
  ///   Interface for L1Station object initialization                                                       ///
  ///-------------------------------------------------------------------------------------------------------///

  /// Setters

  void SetZ(fscal inZ)        { fL1Station.z = inZ;        fInitFlags[kEz] = true; }
  void SetRmin(fscal inRmin)  { fL1Station.Rmin = inRmin;  fInitFlags[kERmin] = true; }
  void SetRmax(fscal inRmax)  { fL1Station.Rmax = inRmax;  fInitFlags[kERmax] = true; }

  // Sets material thickness and radiational length, calculates their ratio ("RadThick")
  // and its logarithm ("logRadThick")
  void SetMaterial     (fscal inThickness, fscal inRL) {
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
  // TODO: Do something with N parameter
  void SetFieldSlice (int N, const fscal * Cx, const fscal * Cy, const fscal * Cz) {
    for (int idx = 0; idx < N; ++idx) {
      fL1Station.fieldSlice.cx[idx] = Cx[idx];
      fL1Station.fieldSlice.cy[idx] = Cy[idx];
      fL1Station.fieldSlice.cz[idx] = Cz[idx];
      fInitFlags[kEfieldSlice] = true;
    }
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

  ///-------------------------------------------------------------------------------------------------------///
  ///   Other methods                                                                                       ///
  ///-------------------------------------------------------------------------------------------------------///

  void Print() { // TODO: use here LOG() macro instead of std::cout
    LOG(INFO) << "== L1Algo: station info ===========================================================";
    LOG(INFO) << "";
    LOG(INFO) << "  L1Station fields:";
    LOG(INFO) << "    z                    " << fL1Station.z ;
    LOG(INFO) << "===================================================================================";
  }
};

#endif // L1BaseStationInfo_h
