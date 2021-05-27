/** @file CbmLitTestMatrixMath.h
  * @copyright Copyright (C) 2011-2012 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [orginator] **/

#ifndef CBMLITTESTMATRIXMATH_H_
#define CBMLITTESTMATRIXMATH_H_

#include "base/CbmLitFloat.h"

#include <string>
#include <vector>


class CbmLitTestMatrixMath {
public:
  CbmLitTestMatrixMath();
  virtual ~CbmLitTestMatrixMath();

  void RunTest();

private:
  void TestInvSym15(int testId);

  void TestMult15On5(int testId);

  void Convert15To25(const litfloat* a15, litfloat* a25);

  void Convert25To15(const litfloat* a25, litfloat* a15);

  std::string VectorToString(const std::vector<litfloat>& a);

  std::string ArrayToString(const litfloat* a, int n);

  litfloat fEpsilon;
};

#endif /* CBMLITTESTMATRIXMATH_H_ */
