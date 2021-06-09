/* Copyright (C) 2020-2021 GSI, IKF-UFra
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Oleksii Lubynets [committer], Viktor Klochkov, Ilya Selyuzhenkov */

//
// @class CutsContainer
// @brief Container with values of cuts.
//
// The meaning of quantities to be cut is described in the OutputContainer.h
//


#ifndef CutsContainer_H
#define CutsContainer_H

class CutsContainer {

public:
  CutsContainer()          = default;
  virtual ~CutsContainer() = default;

  //  lambda candidate parameters setters
  void SetCutChi2Prim(float value) { cut_chi2_prim_ = value; };
  void SetCutDistance(float value) { cut_distance_ = value; };
  void SetCutChi2Geo(float value) { cut_chi2_geo_ = value; };
  void SetCutLDown(float value) { cut_l_down_ = value; };
  void SetCutLdL(float value) { cut_ldl_ = value; };

  //  lambda candidate parameters getters
  float GetCutChi2Prim() const { return cut_chi2_prim_; };
  float GetCutDistance() const { return cut_distance_; };
  float GetCutChi2Geo() const { return cut_chi2_geo_; };
  float GetCutLDown() const { return cut_l_down_; };
  float GetCutLdL() const { return cut_ldl_; };

protected:
  // Cuts with their default values
  float cut_chi2_prim_ {18.4207};
  float cut_distance_ {1.};
  float cut_chi2_geo_ {3.};
  float cut_l_down_ {-5.};
  ;
  float cut_ldl_ {5.};
};
#endif  //CutsContainer_H
