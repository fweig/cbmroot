#ifndef COMPAREMUCHBEAMTIMEDIGI_H
#define COMPAREMUCHBEAMTIMEDIGI_H 1

#include "compareMuchDigi.h"

void compareMuchBeamTimeDigiDataMembers(CbmMuchBeamTimeDigi& test, 
    Int_t address, ECbmModuleId systemid, ULong64_t time, Int_t charge,
    Int_t padx, Int_t pady, Int_t rocid, Int_t nxid, Int_t nxch,
    Int_t elink)
{
  CbmMuchDigi bla = static_cast<CbmMuchDigi>(test);
  compareMuchDigiDataMembers(bla , address, systemid, time, charge);
 
  Int_t retValInt{-222};

  retValInt = test.GetPadX();
  EXPECT_EQ(padx, retValInt);

  retValInt = test.GetPadY();
  EXPECT_EQ(pady, retValInt);

  retValInt = test.GetRocId();
  EXPECT_EQ(rocid, retValInt);

  retValInt = test.GetNxId();
  EXPECT_EQ(nxid, retValInt);

  retValInt = test.GetNxCh();
  EXPECT_EQ(nxch, retValInt);

  retValInt = test.GetElink();
  EXPECT_EQ(elink, retValInt);
}

#endif // COMPAREMUCHBEAMTIMEDIGI_H
