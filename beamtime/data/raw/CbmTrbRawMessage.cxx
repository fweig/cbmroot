#include "CbmTrbRawMessage.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
CbmTrbRawMessage::CbmTrbRawMessage()
  : CbmRawMessage(),
    fTDCfine(0),
    fEdge(kFALSE),
    fCorrection(0.),
    fMicrosliceID(0)
{
}

// ------ Constructor -----------------------------------------------------
CbmTrbRawMessage::CbmTrbRawMessage(Int_t FlibLink, Int_t FebId,
                                   Int_t ChannelId,
                                   Int_t EpochMarker, Int_t CoarseTime,
                                   Int_t FineTime, Bool_t Edge, Double_t Correction,
                                   Int_t MicrosliceID)
  : CbmRawMessage(FlibLink, FebId, ChannelId, EpochMarker, CoarseTime),
    fTDCfine(FineTime),
    fEdge(Edge),
    fCorrection(Correction),
	fMicrosliceID(MicrosliceID)
{
}

// -------------------------------------------------------------------------
void CbmTrbRawMessage::Print(Option_t*) const
{
	std::cout << "CbmTrbRawMessage: "
	          << "eqID=0x" << std::hex << fEquipmentID << std::dec << "\t"
              << "msID=" << fMicrosliceID << "\t"
              << "addr=" << fSourceAddress << "\t"
	          << "ch=" << fChannelID << "\t"
	          << "edge=" << fEdge << "\t"
	          << "ep=" << fEpochMarker << "\t"
	          << "ct=" << fTime << "\t"
	          << "ft=" << fTDCfine << "\t"
	          << "corr=" << fCorrection << std::endl;
}

// -------------------------------------------------------------------------

void CbmTrbRawMessage::CopyMessage(CbmTrbRawMessage* source)
{
  /* From CbmRawMessage */
  fEquipmentID = source->GetEquipmentID();
  fSourceAddress = source->GetSourceAddress();
  fChannelID = source->GetChannelID();
  fEpochMarker = source->GetEpochMarker();
  fTime = source->GetTime();

  /* From CbmTrbRawMessage */
  fTDCfine = source->GetTDCfine();
  fEdge = source->GetEdge();
  fCorrection = source->GetCorr();
  fMicrosliceID = source->GetMicrosliceID();
}

/*
 * For information see comments in class CbmTrbCalibrator
 * There you can find the info about how to compute full time from
 * epoch, coarse, fine and correction.
 */
ULong_t CbmTrbRawMessage::GetFullTime()
{
  Double_t coarseUnit = 5.;
  Double_t epochUnit = coarseUnit * 0x800;

  ULong_t time = fEpochMarker * epochUnit + fTime * coarseUnit + fCorrection;

  return time;
}

Double_t CbmTrbRawMessage::GetFullTime2() const
{
  Double_t coarseUnit = 5.; // 5ns
  Double_t epochUnit = coarseUnit * 0x800;
  Double_t fineUnit = 0.005; // 5ps

  Double_t time = fEpochMarker * epochUnit + fTime * coarseUnit - fTDCfine * fineUnit + fCorrection;

  return time;
}

ClassImp(CbmTrbRawMessage)
