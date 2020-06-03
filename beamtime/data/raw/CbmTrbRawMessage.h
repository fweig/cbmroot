#ifndef CBMTRBRAWMESSAGE_H
#define CBMTRBRAWMESSAGE_H 1


#include "CbmRawMessage.h"

class CbmTrbRawMessage : public CbmRawMessage
{

 public:

  /** Default constructor  **/
  CbmTrbRawMessage();

  /** Constructor  **/
  CbmTrbRawMessage(Int_t FlibLink, Int_t FebId, Int_t ChannelId,
                   Int_t EpochMarker, Int_t CoarseTime,
                   Int_t FineTime, Bool_t Edge, Double_t Correction=0., Int_t MicrosliceID=0);

  /** Destructor  **/
  virtual ~CbmTrbRawMessage() { };

  virtual void Print(Option_t* option="") const;

  void CopyMessage(CbmTrbRawMessage* source);

  Int_t GetTDCfine() { return fTDCfine;}
  Int_t GetTDCcoarse() { return fTime;}
  Int_t GetEdge() { return fEdge;}
  Double_t GetCorr() { return fCorrection; }
  Int_t GetMicrosliceID() const { return fMicrosliceID; }

  ULong_t GetFullTime();

  Double_t GetFullTime2() const;

 private:

  /** Data members **/
  Int_t fTDCfine;
  Bool_t fEdge;
  Double_t fCorrection;
  Int_t fMicrosliceID;

  ClassDef(CbmTrbRawMessage,2);

};

#endif // CBMTRBRAWMESSAGE_H
