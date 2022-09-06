/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1Field.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "L1Utils.h"

//
// L1FieldValue methods
//

//----------------------------------------------------------------------------------------------------------------------
//
void L1FieldValue::CheckConsistency() const
{
  /* Check SIMD data vectors for consistent initialization */
  L1Utils::CheckSimdVectorEquality(x, "L1FieldValue::x");
  L1Utils::CheckSimdVectorEquality(y, "L1FieldValue::y");
  L1Utils::CheckSimdVectorEquality(z, "L1FieldValue::z");

  // TODO: Any other checks? (S.Zharko)
}

//----------------------------------------------------------------------------------------------------------------------
// TODO:
std::string L1FieldValue::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "Bx [kG]: " << std::setw(12) << std::setfill(' ') << x[0] << '\n';
  aStream << indent << "By [kG]: " << std::setw(12) << std::setfill(' ') << y[0] << '\n';
  aStream << indent << "Bz [kG]: " << std::setw(12) << std::setfill(' ') << z[0];
  return aStream.str();
}

//----------------------------------------------------------------------------------------------------------------------
//
std::ostream& operator<<(std::ostream& out, const L1FieldValue& B)
{
  return out << B.x[0] << " | " << B.y[0] << " | " << B.z[0];
};

//
// L1FieldSlice methods
//

//----------------------------------------------------------------------------------------------------------------------
//
L1FieldSlice::L1FieldSlice()
{
  for (int i = 0; i < L1Constants::size::kMaxNFieldApproxCoefficients; ++i) {
    cx[i] = L1Utils::kNaN;
    cy[i] = L1Utils::kNaN;
    cz[i] = L1Utils::kNaN;
  }
}

//----------------------------------------------------------------------------------------------------------------------
//
void L1FieldSlice::CheckConsistency() const
{
  /* Check SIMD data vectors for consistent initialization */
  for (int i = 0; i < L1Constants::size::kMaxNFieldApproxCoefficients; ++i) {
    L1Utils::CheckSimdVectorEquality(cx[i], "L1FieldSlice: cx");
    L1Utils::CheckSimdVectorEquality(cy[i], "L1FieldSlice: cy");
    L1Utils::CheckSimdVectorEquality(cz[i], "L1FieldSlice: cz");
  }
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldSlice::GetFieldValue(const fvec& x, const fvec& y, L1FieldValue& B) const
{
  fvec x2 = x * x;
  fvec y2 = y * y;
  fvec xy = x * y;

  fvec x3  = x2 * x;
  fvec y3  = y2 * y;
  fvec xy2 = x * y2;
  fvec x2y = x2 * y;

  fvec x4   = x3 * x;
  fvec y4   = y3 * y;
  fvec xy3  = x * y3;
  fvec x2y2 = x2 * y2;
  fvec x3y  = x3 * y;

  fvec x5   = x4 * x;
  fvec y5   = y4 * y;
  fvec xy4  = x * y4;
  fvec x2y3 = x2 * y3;
  fvec x3y2 = x3 * y2;
  fvec x4y  = x4 * y;

  B.x = cx[0] + cx[1] * x + cx[2] * y + cx[3] * x2 + cx[4] * xy + cx[5] * y2 + cx[6] * x3 + cx[7] * x2y + cx[8] * xy2
        + cx[9] * y3 + cx[10] * x4 + cx[11] * x3y + cx[12] * x2y2 + cx[13] * xy3 + cx[14] * y4 + cx[15] * x5
        + cx[16] * x4y + cx[17] * x3y2 + cx[18] * x2y3 + cx[19] * xy4 + cx[20] * y5;

  B.y = cy[0] + cy[1] * x + cy[2] * y + cy[3] * x2 + cy[4] * xy + cy[5] * y2 + cy[6] * x3 + cy[7] * x2y + cy[8] * xy2
        + cy[9] * y3 + cy[10] * x4 + cy[11] * x3y + cy[12] * x2y2 + cy[13] * xy3 + cy[14] * y4 + cy[15] * x5
        + cy[16] * x4y + cy[17] * x3y2 + cy[18] * x2y3 + cy[19] * xy4 + cy[20] * y5;

  B.z = cz[0] + cz[1] * x + cz[2] * y + cz[3] * x2 + cz[4] * xy + cz[5] * y2 + cz[6] * x3 + cz[7] * x2y + cz[8] * xy2
        + cz[9] * y3 + cz[10] * x4 + cz[11] * x3y + cz[12] * x2y2 + cz[13] * xy3 + cz[14] * y4 + cz[15] * x5
        + cz[16] * x4y + cz[17] * x3y2 + cz[18] * x2y3 + cz[19] * xy4 + cz[20] * y5;
}

//----------------------------------------------------------------------------------------------------------------------
//
std::string L1FieldSlice::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly it is better to place the indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "idx           CX           CY           CZ";
  for (int i = 0; i < L1Constants::size::kMaxNFieldApproxCoefficients; ++i) {
    aStream << '\n' << indent;
    aStream << std::setw(3) << std::setfill(' ') << i << ' ';
    aStream << std::setw(12) << std::setfill(' ') << cx[i][0] << ' ';
    aStream << std::setw(12) << std::setfill(' ') << cy[i][0] << ' ';
    aStream << std::setw(12) << std::setfill(' ') << cz[i][0];
  }
  return aStream.str();
}

//
// L1FieldRegion methdos
//

//----------------------------------------------------------------------------------------------------------------------
//
L1FieldRegion::L1FieldRegion(float reg[10]) noexcept
  : cx0(reg[0])
  , cx1(reg[1])
  , cx2(reg[2])
  , cy0(reg[3])
  , cy1(reg[4])
  , cy2(reg[5])
  , cz0(reg[6])
  , cz1(reg[7])
  , cz2(reg[8])
  , z0(reg[9])
{
}

//----------------------------------------------------------------------------------------------------------------------
//
void L1FieldRegion::CheckConsistency() const
{
  /* Check SIMD data vectors for consistent initialization */
  L1Utils::CheckSimdVectorEquality(cx0, "L1FieldRegion::cx0");
  L1Utils::CheckSimdVectorEquality(cx1, "L1FieldRegion::cx1");
  L1Utils::CheckSimdVectorEquality(cx2, "L1FieldRegion::cx2");
  L1Utils::CheckSimdVectorEquality(cy0, "L1FieldRegion::cy0");
  L1Utils::CheckSimdVectorEquality(cy1, "L1FieldRegion::cy1");
  L1Utils::CheckSimdVectorEquality(cy2, "L1FieldRegion::cy2");
  L1Utils::CheckSimdVectorEquality(cz0, "L1FieldRegion::cz0");
  L1Utils::CheckSimdVectorEquality(cz1, "L1FieldRegion::cz1");
  L1Utils::CheckSimdVectorEquality(cz2, "L1FieldRegion::cz2");
  L1Utils::CheckSimdVectorEquality(z0, "L1FieldRegion::z0");
  // TODO: Any other checks? (S.Zharko)
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
L1FieldValue L1FieldRegion::Get(const fvec z)
{
  fvec dz  = z - z0;
  fvec dz2 = dz * dz;
  L1FieldValue B;
  B.x = cx0 + cx1 * dz + cx2 * dz2;
  B.y = cy0 + cy1 * dz + cy2 * dz2;
  B.z = cz0 + cz1 * dz + cz2 * dz2;
  return B;
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldRegion::Get(const fvec z_, fvec* B) const
{
  fvec dz  = z_ - z0;
  fvec dz2 = dz * dz;
  B[0]     = cx0 + cx1 * dz + cx2 * dz2;
  B[1]     = cy0 + cy1 * dz + cy2 * dz2;
  B[2]     = cz0 + cz1 * dz + cz2 * dz2;
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldRegion::Set(const L1FieldValue& b0, const fvec b0z, const L1FieldValue& b1, const fvec b1z,
                        const L1FieldValue& b2, const fvec b2z)
{
  z0       = b0z;
  fvec dz1 = b1z - b0z;
  fvec dz2 = b2z - b0z;
  fvec det = fvec::One() / (dz1 * dz2 * (b2z - b1z));

  fvec w21 = -dz2 * det;
  fvec w22 = dz1 * det;
  fvec w11 = -dz2 * w21;
  fvec w12 = -dz1 * w22;

  fvec db1 = b1.x - b0.x;
  fvec db2 = b2.x - b0.x;
  cx0      = b0.x;
  cx1      = db1 * w11 + db2 * w12;
  cx2      = db1 * w21 + db2 * w22;

  db1 = b1.y - b0.y;
  db2 = b2.y - b0.y;
  cy0 = b0.y;
  cy1 = db1 * w11 + db2 * w12;
  cy2 = db1 * w21 + db2 * w22;

  db1 = b1.z - b0.z;
  db2 = b2.z - b0.z;
  cz0 = b0.z;
  cz1 = db1 * w11 + db2 * w12;
  cz2 = db1 * w21 + db2 * w22;
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldRegion::Set(const L1FieldValue& b0, const fvec b0z, const L1FieldValue& b1, const fvec b1z)
{
  z0       = b0z;
  fvec dzi = fvec::One() / (b1z - b0z);
  cx0      = b0.x;
  cy0      = b0.y;
  cz0      = b0.z;
  cx1      = (b1.x - b0.x) * dzi;
  cy1      = (b1.y - b0.y) * dzi;
  cz1      = (b1.z - b0.z) * dzi;
  cx2      = 0.f;
  cy2      = 0.f;
  cz2      = 0.f;
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldRegion::Shift(fvec z)
{
  fvec dz    = z - z0;
  fvec cx2dz = cx2 * dz;
  fvec cy2dz = cy2 * dz;
  fvec cz2dz = cz2 * dz;
  z0         = z;
  cx0 += (cx1 + cx2dz) * dz;
  cy0 += (cy1 + cy2dz) * dz;
  cz0 += (cz1 + cz2dz) * dz;
  cx1 += cx2dz + cx2dz;
  cy1 += cy2dz + cy2dz;
  cz1 += cz2dz + cz2dz;
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldRegion::SetOneEntry(const int i0, const L1FieldRegion& f1, const int i1)
{
  cx0[i0] = f1.cx0[i1];
  cx1[i0] = f1.cx1[i1];
  cx2[i0] = f1.cx2[i1];
  cy0[i0] = f1.cy0[i1];
  cy1[i0] = f1.cy1[i1];
  cy2[i0] = f1.cy2[i1];
  cz0[i0] = f1.cz0[i1];
  cz1[i0] = f1.cz1[i1];
  cz2[i0] = f1.cz2[i1];
  z0[i0]  = f1.z0[i1];
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldRegion::SetOneEntry(const L1FieldRegion& f1, const int i1)
{
  cx0 = f1.cx0[i1];
  cx1 = f1.cx1[i1];
  cx2 = f1.cx2[i1];
  cy0 = f1.cy0[i1];
  cy1 = f1.cy1[i1];
  cy2 = f1.cy2[i1];
  cz0 = f1.cz0[i1];
  cz1 = f1.cz1[i1];
  cz2 = f1.cz2[i1];
  z0  = f1.z0[i1];
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: Should it be inline? (S.Zharko)
void L1FieldRegion::GetOneEntry(float reg[10], const int iVec)
{
  reg[0] = cx0[iVec];
  reg[1] = cx1[iVec];
  reg[2] = cx2[iVec];
  reg[3] = cy0[iVec];
  reg[4] = cy1[iVec];
  reg[5] = cy2[iVec];
  reg[6] = cz0[iVec];
  reg[7] = cz1[iVec];
  reg[8] = cz2[iVec];
  reg[9] = z0[iVec];
}

//----------------------------------------------------------------------------------------------------------------------
//
std::string L1FieldRegion::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly it is better to place the indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "Bx(z) [kG] components: " << '\n';
  aStream << indent << indentChar << "cx0: " << cx0 << '\n';
  aStream << indent << indentChar << "cx1: " << cx1 << '\n';
  aStream << indent << indentChar << "cx2: " << cx2 << '\n';
  aStream << indent << "By(z) [kG] components: " << '\n';
  aStream << indent << indentChar << "cy0: " << cy0 << '\n';
  aStream << indent << indentChar << "cy1: " << cy1 << '\n';
  aStream << indent << indentChar << "cy2: " << cy2 << '\n';
  aStream << indent << "Bz(z) [kG] components: " << '\n';
  aStream << indent << indentChar << "cz0: " << cz0 << '\n';
  aStream << indent << indentChar << "cz1: " << cz1 << '\n';
  aStream << indent << indentChar << "cz2: " << cz2 << '\n';
  aStream << indent << "z0 [cm]: " << z0;
  return aStream.str();
}
