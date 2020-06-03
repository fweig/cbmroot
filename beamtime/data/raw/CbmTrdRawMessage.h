#ifndef CBMTRDRAWMESSAGE_H
#define CBMTRDRAWMESSAGE_H

#include "CbmRawMessage.h"

class CbmTrdRawMessage : public CbmRawMessage
{
public:
  enum CbmTrdAsicType{
    kSPADIC20 = 0   //< SPADIC 2.0
   ,kSPADIC21       //< SPADIC 2.1
   ,kSPADIC22       //< SPADIC 2.2
   ,kFASP           //< FASP 2.1
  };
  CbmTrdRawMessage();
  /** \brief Build the core of a TRD message
   * \param[in] asicType define ASIC type according of the CbmTrdAsicType
   * \param[in] messType define message type. ASIC specific
   * \param[in] asicAddress unique address of the ASIC on the module
   * \param[in] chAddress unique channel address on the ASIC
   * \param[in] timeStamp time inside one epoch
   * \param[in] data memory to be used during inheritance
   */
  CbmTrdRawMessage(UChar_t asicType, UChar_t messType, 
                    UChar_t asicAddress, UChar_t chAddress,
                    UShort_t timeStamp, UInt_t data=0);
  
  virtual UChar_t   GetAddressAsic() const    { return fMeta&0xff;}
  virtual UChar_t   GetAddressChannel() const { return (fMeta&0xf00)>>8;}
  virtual CbmTrdAsicType   GetAsicType() const = 0;     // { return (fMeta&0x3000)>>12;}
  virtual UChar_t   GetMessType() const       { return fMeta>>26;}
  virtual UInt_t    GetData() const           { return fData;}
  virtual UChar_t   GetTimeStamp() const      { return (fMeta&0x3ffc000)>>14;}
  
protected:
  UInt_t    fMeta;    //< Meta data describing the message
  UInt_t    fData;    //< allocated for memory alignment; reserved for derived classes
  
  ClassDef(CbmTrdRawMessage, 1)    // Abstract class for TRD raw messages
};

#endif
