#ifndef COMPAREERRORMESSAGE_H
#define COMPAREERRORMESSAGE_H 1

#include "CbmErrorMessage.h"

void compareErrorMessageDataMembers(CbmErrorMessage& test,
                               ECbmModuleId sysId, Double_t dTime, UInt_t uAddress,
                               UInt_t uFlags, UInt_t uPayload)
{
  EXPECT_EQ( sysId,    test.GetSystemId() );
  EXPECT_EQ( uAddress, test.GetAddress() );
  EXPECT_EQ( uFlags,   test.GetFlags() );
  EXPECT_EQ( uPayload, test.GetPayload() );
  EXPECT_DOUBLE_EQ( dTime, test.GetTime() );
}


#endif // COMPAREERRORMESSAGE_H
