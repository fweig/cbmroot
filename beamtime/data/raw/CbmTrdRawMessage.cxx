#include "CbmTrdRawMessage.h"


//___________________________________________________________________________
CbmTrdRawMessage::CbmTrdRawMessage()
  : CbmRawMessage()
  ,fMeta(0)
  ,fData(0)
{
/** Default constructor
 */
}

//___________________________________________________________________________
CbmTrdRawMessage::CbmTrdRawMessage(UChar_t asicType, UChar_t messType, 
                    UChar_t asicAddress, UChar_t chAddress,
                    UShort_t timeStamp, UInt_t data)
  : CbmRawMessage()
  ,fMeta(0)
  ,fData(data)
{
  /** Pack readable data to a 64 bits word of the core message;\n 
   * 32 bits meta data structure \n
   * MMMM.MMtt tttt.tttt ttAA.cccc aaaa.aaaa \n
   * A - ASIC type 2 bits see CbmTrdAsicType definition\n
   * M - Message type defined by implementation\n
   * a - ASIC address in the module\n
   * c - channel address in the ASIC\n
   * t - time stamp within one epoch
   * 
   * The last 14 bits should be kept for all inherited classes while the "M" and "t" bits can be realocated. 
   */
  
  fMeta = timeStamp; fMeta<<=6;
  fMeta|= (messType&0x3f); fMeta<<=2;
  fMeta|= (asicType&0x3); fMeta<<=4;
  fMeta|= (chAddress&0xf); fMeta<<=8;
  fMeta|= asicAddress;
}
 
ClassImp(CbmTrdRawMessage)
