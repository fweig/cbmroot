/** @file CudaTest.h
  * @copyright Copyright (C) 2010-2011 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer] **/

#ifndef CUDATEST_H_
#define CUDATEST_H_

#include "TObject.h"

#include "LitCudaTest.h"

class CudaTest : public TObject {
public:
  CudaTest() { ; }
  virtual ~CudaTest() { ; }

  void MyDeviceInfo() { fCuda.MyDeviceInfo(); }
  void MyAddVec() { fCuda.MyAddVec(); }

private:
  LitCudaTest fCuda;

  ClassDef(CudaTest, 1)
};


#endif
