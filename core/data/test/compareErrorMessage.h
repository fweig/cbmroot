/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef COMPAREERRORMESSAGE_H
#define COMPAREERRORMESSAGE_H 1

#include "CbmErrorMessage.h"

#include "gtest/gtest.h"

void compareErrorMessageDataMembers(CbmErrorMessage& test, ECbmModuleId sysId, double dTime, uint32_t uAddress,
                                    uint32_t uFlags, uint32_t uPayload)
{
  EXPECT_EQ(sysId, test.GetSystemId());
  EXPECT_EQ(uAddress, test.GetAddress());
  EXPECT_EQ(uFlags, test.GetFlags());
  EXPECT_EQ(uPayload, test.GetPayload());
  EXPECT_DOUBLE_EQ(dTime, test.GetTime());
}


#endif  // COMPAREERRORMESSAGE_H
