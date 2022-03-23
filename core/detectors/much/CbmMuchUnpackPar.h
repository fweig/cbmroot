/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----            CbmMuchUnpackPar header file                       -----
// -----            Created 22/02/22  by P.-A. Loizeau                 -----
// -----            Modified 07/12/18  by A Kumar                      -----
// -------------------------------------------------------------------------

#ifndef CBMMUCHUNPACKPAR_H
#define CBMMUCHUNPACKPAR_H

#include "FairParGenericSet.h"

#include "TArrayD.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class CbmMuchUnpackPar : public FairParGenericSet {

public:
  /** Standard constructor **/
  CbmMuchUnpackPar(const char* name = "CbmMuchUnpackPar", const char* title = "Much parameters",
                   const char* context = "Default");


  /** Destructor **/
  virtual ~CbmMuchUnpackPar();

  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  static constexpr UInt_t GetNbCrobsPerDpb() { return kuNbCrobsPerDpb; }
  static constexpr UInt_t GetNbElinkPerCrob() { return kuNbElinksPerCrob; }
  static constexpr UInt_t GetNbElinkPerDpb() { return kuNbCrobsPerDpb * kuNbElinksPerCrob; }
  static constexpr UInt_t GetNbFebsPerCrob() { return kuNbFebsPerCrob; }
  static constexpr UInt_t GetNbFebsPerDpb() { return kuNbCrobsPerDpb * kuNbFebsPerCrob; }
  static constexpr UInt_t GetNbAsicsPerFeb() { return kuNbAsicsPerFeb; }
  static constexpr UInt_t GetNbAsicsPerCrob() { return kuNbFebsPerCrob * kuNbAsicsPerFeb; }
  static constexpr UInt_t GetNbAsicsPerDpb() { return kuNbCrobsPerDpb * GetNbAsicsPerCrob(); }
  static constexpr UInt_t GetNbChanPerAsic() { return kuNbChanPerAsic; }
  static constexpr UInt_t GetNbChanPerFeb() { return kuNbAsicsPerFeb * kuNbChanPerAsic; }


  //! Convert from eLink index to FEB Connection ( 0 to kuNbFebsPerCrob)
  Int_t ElinkIdxToFebIdx(UInt_t uElink);
  /*UInt_t ElinkIdxToAsicIdx( Bool_t bFebType, UInt_t uElink )
         { return kTRUE == bFebType ? ElinkIdxToAsicIdxFebB( uElink ) :
                                      ElinkIdxToAsicIdxFebA( uElink );
         }*/
  //UInt_t ElinkIdxToAsicIdx(UInt_t uElink) { return ElinkIdxToAsicIdxFebMuch(uElink); }

  //UInt_t ElinkIdxToAsicIdxFebMuch(UInt_t uElink);
  /*UInt_t ElinkIdxToAsicIdxFebA( UInt_t uElink );
   UInt_t ElinkIdxToAsicIdxFebB( UInt_t uElink );*/

  UInt_t GetNrOfDpbs() { return fuNrOfDpbs; }
  UInt_t GetDpbId(UInt_t uDpbIdx);
  UInt_t GetNrOfCrobs() { return fuNrOfDpbs * kuNbCrobsPerDpb; }
  UInt_t GetNrOfFebs() { return GetNrOfCrobs() * kuNbFebsPerCrob; }
  UInt_t GetNrOfAsics() { return GetNrOfFebs() * kuNbAsicsPerFeb; }
  //inline Int_t GetFebsIdsFromArray(Int_t i) { return fnFebsIdsArray[i]; }
  UInt_t GetNrOfFebsInGemA() { return fuFebsInGemA; }
  UInt_t GetNrOfFebsInGemB() { return fuFebsInGemB; }
  Int_t GetNrOfChannels() { return kuNbChanPerAsic; }

  int32_t GetFebId(UInt_t);
  UInt_t GetModule(UInt_t);


  Short_t GetPadXA(UShort_t febid, UShort_t channelid);
  Short_t GetPadYA(UShort_t febid, UShort_t channelid);
  Short_t GetPadXB(UShort_t febid, UShort_t channelid);
  Short_t GetPadYB(UShort_t febid, UShort_t channelid);
  Double_t GetRealX(Int_t);
  Double_t GetRealPadSize(Int_t);

  Double_t GetRealX(Int_t Channel, Int_t Sector);
  Double_t GetRealPadSize(Int_t Channel, Int_t Sector);

  //RPC Module Related Functions
  UInt_t GetNrOfFebsInRpc() { return fuFebsInRpc; }
  Short_t GetPadXRpc(UShort_t febid, UShort_t channelid);
  Short_t GetPadYRpc(UShort_t febid, UShort_t channelid);

  Bool_t IsCrobActive(UInt_t uDpbIdx, UInt_t uCrobIdx);
  Bool_t IsFebActive(UInt_t uFebInSystIdx);
  Bool_t IsFebActive(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Bool_t IsFebPulser(UInt_t uFebInSystIdx);
  Bool_t IsFebPulser(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcOffset(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);


private:
  /// Constants
  static const UInt_t kuNbCrobsPerDpb   = 1;    // Number of CROBs possible per DPB
  static const UInt_t kuNbElinksPerCrob = 42;   // Number of elinks in each CROB ?
  static const UInt_t kuNbFebsPerCrob   = 9;    // Number of FEBs  connected to each CROB for mMuch 2019
  static const UInt_t kuNbAsicsPerFeb   = 1;    // Number of ASICs connected in each FEB for MUCH
  static const UInt_t kuNbChanPerAsic   = 128;  // Number of channels in each ASIC
  //   static constexpr UInt_t  kuCrobMapElinkFebIdx[ kuNbElinksPerCrob ] = {
  const Int_t kiCrobMapElinkFebIdx[kuNbElinksPerCrob] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
                                                         4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8};
  //! Map from eLink index to ASIC index within CROB ( 0 to kuNbFebsPerCrob * kuNbAsicPerFeb )


  /// Variables

  UInt_t fuNrOfDpbs;           // Total number of MUCH DPBs in system
  TArrayI fiDbpIdArray;        // Array to hold the unique IDs (equipment ID) for all MUCH DPBs
  TArrayI fiCrobActiveFlag;    // Array to hold the active flag for all CROBs, [ NbDpb * kuNbCrobPerDpb ]
  UInt_t fuFebsInGemA;         // Number of FEBs connected in GEM Module A
  UInt_t fuFebsInGemB;         // Number of FEBs connected in GEM Module B
  UInt_t fuFebsInRpc;          // Number of FEBs connected in RPC Module
  TArrayI fnFebsIdsArrayGemA;  // Array to hold FEB IDs connected to GEM Module A
  TArrayI fnFebsIdsArrayGemB;  // Array to hold FEB IDs connected to GEM Module B
  TArrayI fnFebsIdsArrayRpc;   // Array to hold FEB IDs connected to RPC Module
  TArrayI fChannelsToPadXA;    // Array which stores the corresponding x position of PAD of entire module A
  TArrayI fChannelsToPadYA;    // Array which stores the corresponding y position of PAD of entire module A
  TArrayI fChannelsToPadXB;    // Array which stores the corresponding x position of PAD of entire module B
  TArrayI fChannelsToPadYB;    // Array which stores the corresponding y position of PAD of entire module B
  TArrayI fChannelsToPadXRpc;  // Array which stores the corresponding x position of PAD of RPC module
  TArrayI fChannelsToPadYRpc;  // Array which stores the corresponding y position of PAD of RPC module
  TArrayD fRealX;              // Array which stores the Real X (starting 18.733 cm) position of PAD
  TArrayD fRealPadSize;        // Array which stores the Real Progressive size of each padX (starting .327 cm )

  ClassDef(CbmMuchUnpackPar, 1);
};
#endif  // CBMMUCHUNPACKPAR_H
