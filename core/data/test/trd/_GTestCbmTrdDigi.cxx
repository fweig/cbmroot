/** @file _GTestCbmTrdDigi.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer], Pascal Raisig **/

#include "CbmTrdDigi.h"

#include "compareTrdDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmTrdDigi, CheckDefaultConstructor)
{
  // Create object
  CbmTrdDigi test;
  compareTrdDigiDataMembers(test, 0, ECbmModuleId::kTrd, 0, 0);

  CbmTrdDigi* test1 = new CbmTrdDigi();

  compareTrdDigiDataMembers(*test1, 0, ECbmModuleId::kTrd, 0, 0);
}

TEST(_GTestCbmTrdDigi, CheckStandardConstructor)
{
  // Create object
  // This creates a Spadic standard digi, fasp test to be added here
  Int_t padChNr        = 42;
  Int_t uniqueModuleId = (Int_t) ECbmModuleId::kTrd;  // Unique ModuleId of first module
  Double_t charge      = 42.42;
  ULong64_t digiTime   = 42001;
  Int_t errClass       = 0;
  CbmTrdDigi test(padChNr, uniqueModuleId, charge, digiTime, ((Int_t) CbmTrdDigi::kSelf), errClass);
  compareTrdDigiDataMembers(test, padChNr, ECbmModuleId::kTrd, digiTime, charge);

  CbmTrdDigi* test1 = new CbmTrdDigi(padChNr, uniqueModuleId, charge, digiTime, ((Int_t) CbmTrdDigi::kSelf), errClass);

  compareTrdDigiDataMembers(*test1, padChNr, ECbmModuleId::kTrd, digiTime, charge);
  ;
}


TEST(_GTestCbmTrdDigi, CheckGetClassName)
{
  // Create object
  CbmTrdDigi test;

  compareTrdDigiDataMembers(test, 0, ECbmModuleId::kTrd, 0, 0);

  EXPECT_STREQ("CbmTrdDigi", test.GetClassName());
}
