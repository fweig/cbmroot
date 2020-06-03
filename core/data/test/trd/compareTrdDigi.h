#ifndef COMPARETRDDIGI_H
#define COMPARETRDDIGI_H 1

void compareTrdDigiDataMembers(CbmTrdDigi& test, 
    Int_t address, ECbmModuleId systemid, ULong64_t time,
    Int_t charge)
{
  Int_t retValInt{-222};
  Double_t retValDouble{-222.};
  ECbmModuleId retVal{ECbmModuleId::kNotExist};

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(static_cast<Double_t>(time), retValDouble);

  retValDouble = test.GetCharge();
  EXPECT_EQ(static_cast<Double_t>(charge), retValDouble);
}

#endif // COMPARETRDDIGI_H
