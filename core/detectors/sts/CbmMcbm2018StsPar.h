/** @file CbmMcbm2018StsPar.h
  * @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

// -------------------------------------------------------------------------
// -----            CbmMcbm2018StsPar header file                      -----
// -----            Created 25/09/18  by P.-A. Loizeau                 -----
// -------------------------------------------------------------------------

#ifndef CBMMCBM2018STSPAR_H
#define CBMMCBM2018STSPAR_H

#include "FairParGenericSet.h"

#include "TArrayD.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class CbmMcbm2018StsPar : public FairParGenericSet {

public:
  /** Standard constructor **/
  CbmMcbm2018StsPar(const char* name = "CbmMcbm2018StsPar", const char* title = "Sts parameters",
                    const char* context = "Default");


  /** Destructor **/
  virtual ~CbmMcbm2018StsPar();

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

  static constexpr Double_t GetStereoAngle() { return kdStereoAngle; }
  static constexpr Double_t GetPitchMm() { return kdPitchMm; }
  static constexpr Double_t GetSensorSzX() { return kdSensorsSzX; }
  static constexpr Double_t GetSensorSzY() { return kdSensorsSzY; }
  static constexpr Double_t GetCenterStripP() { return kiCenterStripP; }
  static constexpr Double_t GetCenterStripN() { return kiCenterStripN; }
  static constexpr Double_t GetCenterPosX() { return kdCenterPosX; }
  static constexpr Double_t GetCenterPosY() { return kdCenterPosY; }

  Double_t GetStereoAngleTan() const { return kdStereoAngleTan; }

  Int_t ElinkIdxToFebIdx(UInt_t uElink);
  UInt_t ElinkIdxToAsicIdx(Bool_t bFebType, UInt_t uElink)
  {
    return kTRUE == bFebType ? ElinkIdxToAsicIdxFebB(uElink) : ElinkIdxToAsicIdxFebA(uElink);
  }
  UInt_t ElinkIdxToAsicIdxFebA(UInt_t uElink);
  UInt_t ElinkIdxToAsicIdxFebB(UInt_t uElink);

  UInt_t GetNbOfModules() { return fuNbModules; }
  Bool_t CheckModuleIndex(UInt_t uModuleIdx);
  UInt_t GetModuleType(UInt_t uModuleIdx);
  UInt_t GetModuleAddress(UInt_t uModuleIdx);
  Double_t GetModuleCenterPosX(UInt_t uModuleIdx);
  Double_t GetModuleCenterPosY(UInt_t uModuleIdx);

  UInt_t GetNrOfDpbs() { return fuNrOfDpbs; }
  UInt_t GetDpbId(UInt_t uDpbIdx);
  UInt_t GetNrOfCrobs() { return fuNrOfDpbs * kuNbCrobsPerDpb; }
  UInt_t GetNrOfFebs() { return GetNrOfCrobs() * kuNbFebsPerCrob; }
  UInt_t GetNrOfAsics() { return GetNrOfFebs() * kuNbAsicsPerFeb; }

  Bool_t IsCrobActive(UInt_t uDpbIdx, UInt_t uCrobIdx);
  Bool_t IsFebActive(UInt_t uFebInSystIdx);
  Bool_t IsFebActive(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Int_t GetFebModuleIdx(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Int_t GetFebModuleSide(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcOffset(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcBase(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcThrGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Int_t GetFebAdcThrOffs(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);

  //   Bool_t ComputeModuleCoordinates( UInt_t uModuleIdx, Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY );

private:
  /// Constants
  static const UInt_t kuNbCrobsPerDpb   = 1;    // Number of CROBs possible per DPB
  static const UInt_t kuNbElinksPerCrob = 42;   // Number of elinks in each CROB
  static const UInt_t kuNbFebsPerCrob   = 5;    // Number of FEBs  connected to each DPB
  static const UInt_t kuNbAsicsPerFeb   = 8;    // Number of ASICs connected in each FEB
  static const UInt_t kuNbChanPerAsic   = 128;  // Number of channels in each ASIC
  //   static constexpr UInt_t  kuCrobMapElinkFebA[ kuNbElinksPerCrob ] = {
  const UInt_t kuCrobMapElinkFebA[kuNbElinksPerCrob] = {
    0x0021, 0x0023, 0x0025, 0x0020, 0x0022, 0x0018, 0x001A, 0x001C, 0x0024, 0x0027, 0xFFFF, 0xFFFF, 0x0026, 0x001E,
    0x0010, 0x0012, 0x0019, 0x001B, 0x001F, 0x000E, 0x0011, 0x0013, 0x000C, 0x0015, 0x0017, 0x0016, 0x001D, 0x0014,
    0x0009, 0x000D, 0x000F, 0x0008, 0x000A, 0x0002, 0x0004, 0x0006, 0x000B, 0x0005, 0x0000, 0x0003, 0x0007, 0x0001,
  };  //! Map from eLink index to ASIC index within CROB ( 0 to kuNbFebsPerCrob * kuNbAsicPerFeb )
  //   static constexpr UInt_t  kuCrobMapElinkFebB[ kuNbElinksPerCrob ] = {
  const UInt_t kuCrobMapElinkFebB[kuNbElinksPerCrob] = {0x0027, 0x0025, 0x0023, 0x0026, 0x0024, 0x001E, 0x001C, 0x001A,
                                                        0x0022, 0x0021, 0xFFFF, 0xFFFF, 0x0020, 0x0018, 0x0016, 0x0014,
                                                        0x001F, 0x001D, 0x0019, 0x0008, 0x0017, 0x0015, 0x000A, 0x0013,
                                                        0x0011, 0x0010, 0x001B, 0x0012, 0x000F, 0x000B, 0x0009, 0x000E,
                                                        0x000C, 0x0004, 0x0002, 0x0000, 0x000D, 0x0003, 0x0006, 0x0005,
                                                        0x0001, 0x0007};  //! Map from eLink index to ASIC index within CROB ( 0 to kuNbFebsPerCrob * kuNbAsicPerFeb )
  //   static constexpr UInt_t  kuCrobMapElinkFebIdx[ kuNbElinksPerCrob ] = {
  const Int_t kiCrobMapElinkFebIdx[kuNbElinksPerCrob] = {
    4, 4, 4, 4, 4, 3, 3, 3, 4, 4, -1, -1, 4, 3, 2, 2, 3, 3, 3, 1, 2, 2,
    1, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1,  0,  0, 0, 1, 0, 0, 0, 0, 0};  //! Map from eLink index to ASIC index within CROB ( 0 to kuNbFebsPerCrob * kuNbAsicPerFeb )

  /// Modules properties (assumes 1 FEB per side!)
  static constexpr Double_t kdStereoAngle = 7.5;          // [Deg]
  static const Double_t kdStereoAngleTan;                 // [] See cxx file for assignation
  static constexpr Double_t kdPitchMm    = 0.058;         // [mm]
  static constexpr Double_t kdSensorsSzX = 60;            // [mm], active is 59.570 mm (kiNbStrips*kdPitchMm)
  static constexpr Double_t kdSensorsSzY = 60;            // [mm], active is 59.570 mm
  static constexpr Int_t kiCenterStripP  = 512;           // []
  static constexpr Int_t kiCenterStripN  = 512;           // []
  static constexpr Double_t kdCenterPosX = 0.0;           // [mm] Top Center
  static constexpr Double_t kdCenterPosY = 59.570 / 2.0;  // [mm] Top Center

  /// Variables
  UInt_t fuNbModules;       // Total number of STS modules in the setup
  TArrayI fiModuleType;     // Type of each module: 0 for connectors on the right, 1 for connectors on the left
  TArrayI fiModAddress;     // STS address for the first strip of each module
  TArrayD fdModCenterPosX;  // Offset of module center in X, in mm (Should be done by geometry for the unpacker!)
  TArrayD fdModCenterPosY;  // Offset of module center in Y, in mm (Should be done by geometry for the unpacker!)

  UInt_t fuNrOfDpbs;         // Total number of STS DPBs in system
  TArrayI fiDbpIdArray;      // Array to hold the unique IDs (equipment ID) for all STS DPBs
  TArrayI fiCrobActiveFlag;  // Array to hold the active flag for all CROBs, [ NbDpb * kuNbCrobPerDpb ]
  TArrayI
    fiFebModuleIdx;  // Index of the STS module for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ], -1 if inactive
  TArrayI
    fiFebModuleSide;  // STS module side for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ], 0 = P, 1 = N, -1 if inactive
  TArrayD fdFebAdcGain;     // ADC Gain in e-/ADC bin for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]
  TArrayD fdFebAdcBase;     // Base at Cal. Thr. in e- for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]
  TArrayD fdFebAdcThrGain;  // Thr. step in e-/Thr. Unit for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]
  TArrayI
    fiFebAdcThrOffs;  // Thr. offset in Units vs Cal. Thr. for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]

  ClassDef(CbmMcbm2018StsPar, 1);
};
#endif  // CBMMCBM2018STSPAR_H
