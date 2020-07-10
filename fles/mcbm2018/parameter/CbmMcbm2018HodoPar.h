// -------------------------------------------------------------------------
// -----            CbmMcbm2018HodoPar header file                     -----
// -----            Created 31/07/19  by P.-A. Loizeau                 -----
// -------------------------------------------------------------------------

#ifndef CBMMCBM2018HODOPAR_H
#define CBMMCBM2018HODOPAR_H

#include "FairParGenericSet.h"

#include "TArrayD.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class CbmMcbm2018HodoPar : public FairParGenericSet {

public:
  /** Standard constructor **/
  CbmMcbm2018HodoPar(const char* name    = "CbmMcbm2018HodoPar",
                     const char* title   = "Much parameters",
                     const char* context = "Default");


  /** Destructor **/
  virtual ~CbmMcbm2018HodoPar();

  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  static constexpr UInt_t GetNbCrobsPerDpb() { return kuNbCrobsPerDpb; }
  static constexpr UInt_t GetNbElinkPerCrob() { return kuNbElinksPerCrob; }
  static constexpr UInt_t GetNbFebsPerCrob() { return kuNbFebsPerCrob; }
  static constexpr UInt_t GetNbFebsPerDpb() {
    return kuNbCrobsPerDpb * kuNbFebsPerCrob;
  }
  static constexpr UInt_t GetNbAsicsPerFeb() { return kuNbAsicsPerFeb; }
  static constexpr UInt_t GetNbAsicsPerCrob() {
    return kuNbFebsPerCrob * kuNbAsicsPerFeb;
  }
  static constexpr UInt_t GetNbAsicsPerDpb() {
    return kuNbCrobsPerDpb * GetNbAsicsPerCrob();
  }
  static constexpr UInt_t GetNbChanPerAsic() { return kuNbChanPerAsic; }
  static constexpr UInt_t GetNbChanPerFeb() {
    return kuNbAsicsPerFeb * kuNbChanPerAsic;
  }

  Int_t ElinkIdxToFebIdx(UInt_t uElink);
  UInt_t ElinkIdxToAsicIdx(UInt_t uElink) {
    return ElinkIdxToAsicIdxFebMuch(uElink);
  }

  UInt_t ElinkIdxToAsicIdxFebMuch(UInt_t uElink);

  UInt_t ChannelToFiber(UInt_t uChan);
  UInt_t ChannelToPixel(UInt_t uChan);
  UInt_t ChannelToAxis(UInt_t uChan);

  UInt_t GetNbOfModules() { return fuNbModules; }
  Bool_t CheckModuleIndex(UInt_t uModuleIdx);
  UInt_t GetModuleAddress(UInt_t uModuleIdx);
  Double_t GetModuleCenterPosX(UInt_t uModuleIdx);
  Double_t GetModuleCenterPosY(UInt_t uModuleIdx);
  Bool_t GetModuleSwapXY(UInt_t uModuleIdx);
  Bool_t GetModuleInvertX(UInt_t uModuleIdx);
  Bool_t GetModuleInvertY(UInt_t uModuleIdx);

  UInt_t GetNrOfDpbs() { return fuNrOfDpbs; }
  UInt_t GetDpbId(UInt_t uDpbIdx);
  UInt_t GetNrOfCrobs() { return fuNrOfDpbs * kuNbCrobsPerDpb; }
  UInt_t GetNrOfFebs() { return GetNrOfCrobs() * kuNbFebsPerCrob; }
  UInt_t GetNrOfAsics() { return GetNrOfFebs() * kuNbAsicsPerFeb; }

  Bool_t IsCrobActive(UInt_t uDpbIdx, UInt_t uCrobIdx);
  Bool_t IsFebActive(UInt_t uFebInSystIdx);
  Bool_t IsFebActive(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Int_t GetFebModuleIdx(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcOffset(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcBase(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Double_t GetFebAdcThrGain(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);
  Int_t GetFebAdcThrOffs(UInt_t uDpbIdx, UInt_t uCrobIdx, UInt_t uFebIdx);

  UInt_t GetChannelInModule(UInt_t uModuleIdx, UInt_t uChan);

private:
  /// Constants
  static const UInt_t kuNbCrobsPerDpb = 1;  // Number of CROBs possible per DPB
  static const UInt_t kuNbElinksPerCrob =
    42;  // Number of elinks in each CROB ?
  static const UInt_t kuNbFebsPerCrob =
    6;  // Number of FEBs  connected to each CROB for mMuch 2019
  static const UInt_t kuNbAsicsPerFeb =
    1;  // Number of ASICs connected in each FEB for MUCH
  static const UInt_t kuNbChanPerAsic = 128;  // Number of channels in each ASIC
  static const UInt_t kuNbFiberPerAxis = 64;  // Number of Fibers per Hodo axis
  const UInt_t
    kuCrobMapElinkFebMuch
      [kuNbElinksPerCrob] =
        {
          0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0001, 0x0001,
          0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003,
          0x0003, 0x0003, 0x0003, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004,
          0x0005, 0x0005, 0x0005, 0x0005, 0x0003, 0x0003, 0x0003, 0x0003,
          0x0003, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0005, 0x0005,
          0x0005, 0x0005};  //! Map from eLink index to ASIC index within CROB ( 0 to kuNbFebsPerCrob * kuNbAsicPerFeb )
  //   static constexpr UInt_t  kuCrobMapElinkFebIdx[ kuNbElinksPerCrob ] = {
  const Int_t kiCrobMapElinkFebIdx[kuNbElinksPerCrob] = {0, 0, 0, 0, 0, 1, 1, 1,
                                                         1, 1, 2, 2, 2, 2, 3, 3,
                                                         3, 3, 3, 4, 4, 4, 4, 4,
                                                         5, 5, 5, 5, 6, 6, 6, 6,
                                                         6, 7, 7, 7, 7, 7, 8, 8,
                                                         8, 8};  //! Map from eLink index to ASIC index within CROB ( 0 to kuNbFebsPerCrob * kuNbAsicPerFeb )
  const UInt_t kuChannelToFiberMap[kuNbChanPerAsic] =
    {
      /*
         0, 32, 16, 48, 15, 47, 31, 63,  1, 33, 17, 49, 14, 46, 30, 62,
         2, 34, 18, 50, 13, 45, 29, 61,  3, 35, 19, 51, 12, 44, 28, 60,
         4, 36, 20, 52, 11, 43, 27, 59,  5, 37, 21, 53, 10, 42, 26, 58,
         6, 38, 22, 54,  9, 41, 25, 57,  7, 39, 23, 55,  8, 40, 24, 56,
         0, 32, 16, 48, 15, 47, 31, 63,  1, 33, 17, 49, 14, 46, 30, 62,
         2, 34, 18, 50, 13, 45, 29, 61,  3, 35, 19, 51, 12, 44, 28, 60,
         4, 36, 20, 52, 11, 43, 27, 59,  5, 37, 21, 53, 10, 42, 26, 58,
         6, 38, 22, 54,  9, 41, 25, 57,  7, 39, 23, 55,  8, 40, 24, 56
*/
      32, 0,  48, 16, 47, 15, 63, 31, 33, 1,  49, 17, 46, 14, 62, 30, 34,
      2,  50, 18, 45, 13, 61, 29, 35, 3,  51, 19, 44, 12, 60, 28, 36, 4,
      52, 20, 43, 11, 59, 27, 37, 5,  53, 21, 42, 10, 58, 26, 38, 6,  54,
      22, 41, 9,  57, 25, 39, 7,  55, 23, 40, 8,  56, 24, 32, 0,  48, 16,
      47, 15, 63, 31, 33, 1,  49, 17, 46, 14, 62, 30, 34, 2,  50, 18, 45,
      13, 61, 29, 35, 3,  51, 19, 44, 12, 60, 28, 36, 4,  52, 20, 43, 11,
      59, 27, 37, 5,  53, 21, 42, 10, 58, 26, 38, 6,  54, 22, 41, 9,  57,
      25, 39, 7,  55, 23, 40, 8,  56, 24};  //! Map from channel index to Hodoscope Fiber
  const UInt_t kuChannelToPixelMap[kuNbChanPerAsic] =
    {
      1,  5,  3,  7,  2,  6,  4,  8,  9,  13, 11, 15, 10, 14, 12, 16, 17,
      21, 19, 23, 18, 22, 20, 24, 25, 29, 27, 31, 26, 30, 28, 32, 33, 37,
      35, 39, 34, 38, 36, 40, 41, 45, 43, 47, 42, 46, 44, 48, 49, 53, 51,
      55, 50, 54, 52, 56, 57, 61, 59, 63, 58, 62, 60, 64, 1,  5,  3,  7,
      2,  6,  4,  8,  9,  13, 11, 15, 10, 14, 12, 16, 17, 21, 19, 23, 18,
      22, 20, 24, 25, 29, 27, 31, 26, 30, 28, 32, 33, 37, 35, 39, 34, 38,
      36, 40, 41, 45, 43, 47, 42, 46, 44, 48, 49, 53, 51, 55, 50, 54, 52,
      56, 57, 61, 59, 63, 58, 62, 60, 64};  //! Map from channel index to PMT pixel
  const UInt_t kuChannelToPlaneMap
    [kuNbChanPerAsic] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1, 1};  //! Map from channel index to Hodoscope Axis (X or Y)

  /// Variables
  UInt_t fuNbModules;    // Total number of STS modules in the setup
  TArrayI fiModAddress;  // STS address for the first strip of each module
  TArrayD
    fdModCenterPosX;  // Offset of module center in X, in mm (Should be done by geometry for the unpacker!)
  TArrayD
    fdModCenterPosY;  // Offset of module center in Y, in mm (Should be done by geometry for the unpacker!)
  TArrayI
    fiModSwapXY;  // Flag telling for each module if the X and Y axis should be swapped
  TArrayI
    fiModInvertX;  // Flag telling for each module if the X axis should be inverted
  TArrayI
    fiModInvertY;  // Flag telling for each module if the Y axis should be inverted

  UInt_t fuNrOfDpbs;  // Total number of STS DPBs in system
  TArrayI
    fiDbpIdArray;  // Array to hold the unique IDs (equipment ID) for all STS DPBs
  TArrayI
    fiCrobActiveFlag;  // Array to hold the active flag for all CROBs, [ NbDpb * kuNbCrobPerDpb ]
  TArrayI
    fiFebModuleIdx;  // Index of the STS module for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ], -1 if inactive
  TArrayD
    fdFebAdcGain;  // ADC Gain in e-/ADC bin for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]
  TArrayD
    fdFebAdcBase;  // Base at Cal. Thr. in e- for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]
  TArrayD
    fdFebAdcThrGain;  // Thr. step in e-/Thr. Unit for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]
  TArrayI
    fiFebAdcThrOffs;  // Thr. offset in Units vs Cal. Thr. for each FEB, [ NbDpb * kuNbCrobPerDpb * kuNbFebsPerCrob ]

  ClassDef(CbmMcbm2018HodoPar, 1);
};
#endif  // CBMMCBM2018HODOPAR_H
