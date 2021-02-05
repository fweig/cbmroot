#include "CbmTrdAddress.h"

#include <TString.h>

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>

#include <iostream>
using std::cout;
using std::endl;

// Structure to pass filenames together with expected response into the
// parametrized test
struct InOutStructure {
  int layer;
  int module;
  int sector;
  int row;
  int column;
  int result;
};

// Base class to use the same basic setup for parameterized and
// non-parameterized tests
// Here one defines everything which is common for all the different
// test cases
template<class T>
class _TestCbmTrdAddressBase : public T {
protected:
  CbmTrdAddress fTrdId;


  virtual void SetUp() {}

  virtual void TearDown() {}
};

// This is the derived class for the non-parameterized test cases.
class CbmTrdAddressTest : public _TestCbmTrdAddressBase<testing::Test> {};

TEST_F(CbmTrdAddressTest, CheckDefaultSettings) {
  Int_t layerid          = 0;
  Int_t moduleid         = 0;
  Int_t sectorid         = 0;
  Int_t rowid            = 0;
  Int_t columnid         = 0;
  Int_t detInfo_array[5] = {layerid, moduleid, sectorid, rowid, columnid};

  Int_t retVal =
    fTrdId.GetAddress(layerid, moduleid, sectorid, rowid, columnid);
  EXPECT_EQ(0, retVal);
}

// This is the derived class for the parameterized test cases.
class CbmTrdAddressParamTest :
  public _TestCbmTrdAddressBase<testing::TestWithParam<InOutStructure>> {
protected:
  Int_t detInfo_array[5];
  Int_t modInfo_array[5];
  Int_t result;

  virtual void SetUp() {
    InOutStructure const& p = GetParam();

    detInfo_array[0] = p.layer;
    detInfo_array[1] = p.module;
    detInfo_array[2] = p.sector;
    detInfo_array[3] = p.row;
    detInfo_array[4] = p.column;

    result = p.result;

    modInfo_array[0] = detInfo_array[0];
    modInfo_array[1] = detInfo_array[1];
    modInfo_array[2] = detInfo_array[2];
    modInfo_array[3] = detInfo_array[3];
    modInfo_array[4] = detInfo_array[4];
  }
};


TEST_P(CbmTrdAddressParamTest, checkUniqueIdCreation) {
  Int_t uniqueId = fTrdId.GetAddress(detInfo_array[0],
                                     detInfo_array[1],
                                     detInfo_array[2],
                                     detInfo_array[3],
                                     detInfo_array[4]);
  EXPECT_EQ(result, uniqueId);

  Int_t systemId = fTrdId.GetSystemId(uniqueId);
  EXPECT_EQ(kTrd, systemId);

  /*
  Int_t sectorNr = fTrdId.GetSector(uniqueId);
  EXPECT_EQ(detInfo_array[5], sectorNr);

  Int_t modId = fTrdId.SetDetectorInfo(modInfo_array);
  Int_t newUniqueId = fTrdId.SetSector(modId, detInfo_array[5]);
  EXPECT_EQ(result, newUniqueId);

  Int_t newModId = fTrdId.GetModuleId(newUniqueId);
  EXPECT_EQ(modId, newModId);
*/
}

InOutStructure val1  = {0, 0, 0, 0, 0, 0};
InOutStructure val2  = {kTrd, 0, 0, 0, 0, 5};
InOutStructure val3  = {0, 1, 0, 0, 0, 32};
InOutStructure val4  = {0, 0, 1, 0, 0, 512};
InOutStructure val5  = {0, 0, 0, 1, 0, 4096};
InOutStructure val6  = {0, 0, 0, 0, 1, 131072};
InOutStructure val7  = {0, 0, 0, 0, 0, 33554432};
InOutStructure val8  = {kTrd, 1, 1, 1, 1, 33690149};
InOutStructure val9  = {kTrd, 3, 3, 2, 34, 105129573};
InOutStructure val10 = {kTrd, 2, 3, 3, 17, 69350981};
InOutStructure val11 = {0, 0, 0, 8, 0, 32768};
InOutStructure val12 = {kTrd, 2, 3, 5, 17, 69359173};


/*
INSTANTIATE_TEST_CASE_P(TestAllParameters,
                        CbmTrdAddressParamTest,
			::testing::Values(val1, val2, val3, val4, val5,
					  val6, val7, val8, val9, val10,
					  val11, val12));

// This is the derived class for the parameterized test cases.
class CbmTrdAddressParamTest1 : public _TestCbmTrdAddressBase<
  testing::TestWithParam<InOutStructure> >
{
 protected:  
  Int_t detInfo_array[5];
  Int_t result_array[5];
  Int_t uniqueId;

  virtual void SetUp() {
    InOutStructure const& p = GetParam();
    uniqueId=p.result;
    result_array[0] = p.layer;
    result_array[1] = p.module;
    result_array[2] = p.sector;
    result_array[3] = p.row;
    result_array[4] = p.column;
  }
};


TEST_P(CbmTrdAddressParamTest1, checkExtractInfoFromUniqueId)
{
  Int_t* retVal = fTrdId.GetAddress(uniqueId);
  for ( int i=0;i<5;i++) {
    EXPECT_EQ(result_array[i], retVal[i]);
  }
}


INSTANTIATE_TEST_CASE_P(TestAllParameters,
                        CbmTrdAddressParamTest1,
			::testing::Values(val1, val2, val3, val4, val5,
					  val6, val7, val8, val9, val10,
					  val11, val12));

*/
