/* Copyright (C) 2010-2011 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

#include "LitCudaTest.h"

#include <cstdlib>

#include <cuda.h>

extern "C" void DeviceInfo(void);
extern "C" void AddVec(void);

LitCudaTest::LitCudaTest() {}

LitCudaTest::~LitCudaTest() {}

void LitCudaTest::MyDeviceInfo() const { DeviceInfo(); }

void LitCudaTest::MyAddVec() const { AddVec(); }
