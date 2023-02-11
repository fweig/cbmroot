/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov, Maksym Zyzak, Sergei Zharko */

#ifndef L1Field_h
#define L1Field_h 1

#include <string>

#include "L1Constants.h"
#include "L1Def.h"
#include "L1SimdSerializer.h"

class L1FieldValue {
public:
  fvec x {0.f};  //< x-component of the field
  fvec y {0.f};  //< y-component of the field
  fvec z {0.f};  //< z-component of the field

  /// Combines the magnetic field with another field value object using weight
  /// \param B  other field value to combine with
  /// \param w  weight from 0 to 1
  void Combine(const L1FieldValue& B, const fvec& w);

  void Combine(const L1FieldValue& B, const fmask& w);

  /// Consistency checker
  void CheckConsistency() const;

  /// Operator << overloading
  friend std::ostream& operator<<(std::ostream& out, const L1FieldValue& B);

  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel) const;

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& x;
    ar& y;
    ar& z;
  }
} _fvecalignment;

[[gnu::always_inline]] inline void L1FieldValue::Combine(const L1FieldValue& B, const fvec& w)
{
  x += w * (B.x - x);
  y += w * (B.y - y);
  z += w * (B.z - z);
}

[[gnu::always_inline]] inline void L1FieldValue::Combine(const L1FieldValue& B, const fmask& w)
{
  x(w) = B.x;
  y(w) = B.y;
  z(w) = B.z;
}

/// Class represents a set of magnetic field approximation coefficients
///
// TODO: Crosscheck the default content (S.Zharko)
class L1FieldSlice {
public:
  /// Default constructor
  L1FieldSlice();

  /// Consistency checker
  void CheckConsistency() const;

  /// Gets field value from (x, y) fvec point
  /// \param x  x-coordinate of input
  /// \param y  y-coordinate of input
  /// \param B  the L1FieldValue output
  void GetFieldValue(const fvec& x, const fvec& y, L1FieldValue& B) const;

  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;

public:
  // NOTE: We don't use an initialization of arrays here because we cannot be sure
  //       if the underlying type (fvec) has a default constructor, but
  //       we are sure, that it can be initialized with a float. (S.Zharko)
  fvec cx
    [L1Constants::size::kMaxNFieldApproxCoefficients];  ///< Polynomial coefficients for x-component of the field value
  fvec cy
    [L1Constants::size::kMaxNFieldApproxCoefficients];  ///< Polynomial coefficients for y-component of the field value
  fvec cz
    [L1Constants::size::kMaxNFieldApproxCoefficients];  ///< Polynomial coefficients for z-component of the field value

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& cx;
    ar& cy;
    ar& cz;
  }
} _fvecalignment;


class L1FieldRegion {
public:
  // NOTE: When a custom constructor is defined, default constructor also should be provided (S.Zharko)
  L1FieldRegion() = default;

  L1FieldRegion(float reg[10]) noexcept;

  /// Consistency checker
  void CheckConsistency() const;

  /// Gets the field vector and writes it into B pointer
  /// \param x  x-coordinate of the point to calculate the field
  /// \param y  y-coordinate of the point to calculate the field
  /// \param z  z-coordinate of the point to calculate the field
  /// \param B   pointer to the output fvec array of the magnetic field
  void Get(const fvec& x, const fvec& y, const fvec& z, fvec* B) const;

  /// Interpolates the magnetic field by three nodal points and sets the result to this L1FieldRegion object
  /// The result is a quadratic interpolation of the field as a function of z
  /// \param b0   field value in the first nodal point
  /// \param b0z  z-coordinate of the first nodal point
  /// \param b1   field value in the second nodal point
  /// \param b1z  z-coordinate of the second nodal point
  /// \param b2   field value in the third nodal point
  /// \param b2z  z-coordinate of the third nodal point
  /// TODO: does the sequence of b0z, b1z and b2z matter? If yes, probalby we need an assertion (S.Zharko)
  void Set(const L1FieldValue& b0, const fvec b0z, const L1FieldValue& b1, const fvec b1z, const L1FieldValue& b2,
           const fvec b2z);

  /// Interpolates the magnetic field by thwo nodal points and sets the result to this L1FieldRegion object.
  /// The result is a linear interpolation of the field as a function of z
  /// \param b0   field value in the first nodal point
  /// \param b0z  z-coordinate of the first nodal point
  /// \param b1   field value in the second nodal point
  /// \param b1z  z-coordinate of the second nodal point
  /// TODO: does the sequence of b0z and b1z matter? If yes, probalby we need an assertion (S.Zharko)
  void Set(const L1FieldValue& b0, const fvec b0z, const L1FieldValue& b1, const fvec b1z);

  /// Shifts the coefficients to new central point
  /// \param z  z-coordinate of the new central point
  void Shift(fvec z);

  /// Replaces the selected layer of the coefficients with one from another
  /// L1FieldRegion object
  /// \param i0    the index of the fvect layer in this L1FieldRegion object
  /// \param fl    the other L1FieldRegion object, which layer is going to be replaced
  /// \param i1    the index of the source fvect layer to copy
  void SetOneEntry(const int i0, const L1FieldRegion& f1, const int i1);

  /// Replaces all the layers of the coefficients with one selected layer from another
  /// L1FieldRegion object
  /// \param fl    the other L1FieldRegion object, which layer is going to be replaced
  /// \param i1    the index of the source fvect layer to copy
  void SetOneEntry(const L1FieldRegion& f1, const int i1);

  /// Saves the contents of the particular layer of the coefficients into an array of floats
  /// \param reg    output array of 10 floats
  /// \param iVec   index of the input
  // TODO: Probably, it would be better to rename "reg" into "output" and make it the second
  //       parameter of this function (S.Zharko)
  // TODO: Probably we need a const specifier here, because the method does not change the fields
  void GetOneEntry(float reg[10], const int iVec);

  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;

  /// Use original field instead of the field approximation
  void SetUseOriginalField(bool v = true) { fUseOriginalField = v; }

  /// Force using the original field
  static void ForceUseOfOriginalField(bool v = true) { fgkForceUseOfOriginalField = v; }

public:
  static bool fgkForceUseOfOriginalField;

  // TODO: Probably it's better to have arrays instead of separate fvec values? (S.Zharko)
  // Bx(z) = cx0 + cx1*(z-z0) + cx2*(z-z0)^2
  fvec cx0 {0.f};
  fvec cx1 {0.f};
  fvec cx2 {0.f};

  // By(z) = cy0 + cy1*(z-z0) + cy2*(z-z0)^2
  fvec cy0 {0.f};
  fvec cy1 {0.f};
  fvec cy2 {0.f};

  // Bz(z) = cz0 + cz1*(z-z0) + cz2*(z-z0)^2
  fvec cz0 {0.f};
  fvec cz1 {0.f};
  fvec cz2 {0.f};

  fvec z0 {0.f};  ///< z-coordinate of the field region central point

  bool fUseOriginalField {false};

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& cx0;
    ar& cx1;
    ar& cx2;
    ar& cy0;
    ar& cy1;
    ar& cy2;
    ar& cz0;
    ar& cz1;
    ar& cz2;
  }
} _fvecalignment;

#endif
