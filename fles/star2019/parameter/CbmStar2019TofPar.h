// -------------------------------------------------------------------------
// -----                 CbmStar2019TofPar header file                 -----
// -----                 Created 09/09/18  by P.-A. Loizeau            -----
// -------------------------------------------------------------------------

#ifndef CBMSTAR2019TOFPAR_H
#define CBMSTAR2019TOFPAR_H

#include "FairParGenericSet.h"

#include "TArrayD.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class CbmStar2019TofPar : public FairParGenericSet {

public:
  /** Standard constructor **/
  CbmStar2019TofPar(const char* name    = "CbmStar2019TofPar",
                    const char* title   = "Tof unpacker parameters",
                    const char* context = "Default");


  /** Destructor **/
  virtual ~CbmStar2019TofPar();

  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  static constexpr UInt_t GetNbByteMessage() { return kuBytesPerMessage; }

  inline Int_t GetNrOfGdpbs() { return fiNrOfGdpb; }
  inline Int_t GetGdpbId(Int_t i) { return fiGdpbIdArray[i]; }
  /*
   inline Int_t GetNrOfFeesPerGdpb() { return fiNrOfFeesPerGdpb; }
   inline Int_t GetNrOfGet4PerFee() {return fiNrOfGet4PerFee;}
   inline Int_t GetNrOfChannelsPerGet4() {return fiNrOfChannelsPerGet4;}
   */
  static constexpr UInt_t GetNrOfChannelsPerGet4() {
    return kuNbChannelsPerGet4;
  }
  static constexpr UInt_t GetNrOfGet4PerFee() { return kuNbGet4PerFee; }
  static constexpr UInt_t GetNrOfFeePerGbtx() { return kuNbFeePerGbtx; }
  static constexpr UInt_t GetNrOfGbtxPerGdpb() { return kuNbGbtxPerGdpb; }
  static constexpr UInt_t GetNrOfChannelsPerFee() { return kuNbChannelsPerFee; }
  static constexpr UInt_t GetNrOfChannelsPerGbtx() {
    return kuNbChannelsPerGbtx;
  }
  static constexpr UInt_t GetNrOfChannelsPerGdpb() {
    return kuNbChannelsPerGdpb;
  }
  static constexpr UInt_t GetNrOfGet4PerGbtx() { return kuNbGet4PerGbtx; }
  static constexpr UInt_t GetNrOfGet4PerGdpb() { return kuNbGet4PerGdpb; }
  static constexpr UInt_t GetNrOfFeePerGdpb() { return kuNbFeePerGdpb; }

  inline UInt_t GetNumberOfChannels() {
    return kuNbChannelsPerGdpb * fiNrOfGdpb;
  }

  Int_t Get4ChanToPadiChan(UInt_t uChannelInFee);
  Int_t PadiChanToGet4Chan(UInt_t uChannelInFee);

  Int_t ElinkIdxToGet4Idx(UInt_t uElink);
  Int_t Get4IdxToElinkIdx(UInt_t uGet4);

  static constexpr UInt_t GetGdpbToSectorOffset() {
    return kuGdpbToSectorOffset;
  }

  static constexpr UInt_t GetNrOfPadiThrCodes() { return kuNbPadiThrCodes; }
  Double_t GetPadiThresholdVal(UInt_t uCode);

  inline Bool_t GetMonitorMode() {
    return (1 == fiMonitorMode ? kTRUE : kFALSE);
  }
  inline Bool_t GetDebugMonitorMode() {
    return (1 == fiDebugMonitorMode ? kTRUE : kFALSE);
  }

  inline Int_t GetNrOfGbtx() { return fiNrOfGbtx; }
  inline Int_t GetNrOfModules() { return fiNrOfModule; }
  Int_t GetNrOfRpc(UInt_t uGbtx);
  Int_t GetRpcType(UInt_t uGbtx);
  Int_t GetRpcSide(UInt_t uGbtx);
  Int_t GetModuleId(UInt_t uGbtx);

  inline Double_t GetSizeMsInNs() { return fdSizeMsInNs; }

  inline Double_t GetStarTriggAllowedSpread() {
    return fdStarTriggAllowedSpread;
  }
  Double_t GetStarTriggDeadtime(UInt_t uGdpb);
  Double_t GetStarTriggDelay(UInt_t uGdpb);
  Double_t GetStarTriggWinSize(UInt_t uGdpb);

private:
  /// Constants
  /// Data format
  static const uint32_t kuBytesPerMessage = 8;
  /// Readout chain
  static const uint32_t kuNbChannelsPerGet4 = 4;
  static const uint32_t kuNbGet4PerFee      = 8;
  static const uint32_t kuNbFeePerGbtx      = 5;
  static const uint32_t kuNbGbtxPerGdpb     = 6;
  static const uint32_t kuNbChannelsPerFee =
    kuNbChannelsPerGet4 * kuNbGet4PerFee;
  static const uint32_t kuNbChannelsPerGbtx =
    kuNbChannelsPerFee * kuNbFeePerGbtx;
  static const uint32_t kuNbChannelsPerGdpb =
    kuNbChannelsPerGbtx * kuNbGbtxPerGdpb;
  static const uint32_t kuNbGet4PerGbtx = kuNbGet4PerFee * kuNbFeePerGbtx;
  static const uint32_t kuNbGet4PerGdpb = kuNbGet4PerGbtx * kuNbGbtxPerGdpb;
  static const uint32_t kuNbFeePerGdpb  = kuNbFeePerGbtx * kuNbGbtxPerGdpb;
  /// Mapping in Readout chain PCBs
  /*
   const UInt_t kuGet4topadi[ kuNbChannelsPerFee ] = {  // provided by Jochen
           4,  3,  2,  1,
         24, 23, 22, 21,
          8,  7,  6,  5,
         28, 27, 26, 25,
         12, 11, 10,  9,
         32, 31, 30, 29,
         16, 15, 14, 13,
         20, 19, 18, 17
      }; //! Map from GET4 channel to PADI channel
*/
  const UInt_t kuGet4topadi[kuNbChannelsPerFee] = {
    // provided by Jochen
    4,  3,  2,  1,  // provided by Jochen
    8,  7,  6,  5,  12, 11, 10, 9,  16, 15, 14, 13, 20, 19, 18,
    17, 24, 23, 22, 21, 28, 27, 26, 25, 32, 31, 30, 29};  //! Map from GET4 channel to PADI channel

  const UInt_t
    kuPaditoget4[kuNbChannelsPerFee]          = {// provided by Jochen
                                        4,  3,  2,  1,  12, 11, 10, 9,  20,
                                        19, 18, 17, 28, 27, 26, 25, 32, 31,
                                        30, 29, 8,  7,  6,  5,  16, 15, 14,
                                        13, 24, 23, 22, 21};  //! Map from PADI channel to GET4 channel
  const UInt_t kuElinkToGet4[kuNbGet4PerGbtx] = {
    27, 2,  7,  3,  31, 26, 30, 1,  33, 37, 32, 13, 9,  14,
    10, 15, 17, 21, 16, 35, 34, 38, 25, 24, 0,  6,  20, 23,
    18, 22, 28, 4,  29, 5,  19, 36, 39, 8,  12, 11};
  const UInt_t kuGet4ToElink[kuNbGet4PerGbtx] = {
    24, 7,  1,  3,  31, 33, 25, 2,  37, 12, 14, 39, 38, 11,
    13, 15, 18, 16, 28, 34, 26, 17, 29, 27, 23, 22, 5,  0,
    30, 32, 6,  4,  10, 8,  20, 19, 35, 9,  21, 36};
  /// Mapping in STAR
  static const uint32_t kuGdpbToSectorOffset = 13;

  /// PADI threshold measures and extrapolated code to value map
  static const uint32_t kuNbPadiThrCodes        = 1024;  // 0x3FF + 1
  static const uint32_t kuNbThrMeasPoints       = 65;
  const UInt_t kuThrMeasCode[kuNbThrMeasPoints] = {
    0x000, 0x010, 0x020, 0x030, 0x040, 0x050, 0x060, 0x070, 0x080, 0x090, 0x0A0,
    0x0B0, 0x0C0, 0x0D0, 0x0E0, 0x0F0, 0x100, 0x110, 0x120, 0x130, 0x140, 0x150,
    0x160, 0x170, 0x180, 0x190, 0x1A0, 0x1B0, 0x1C0, 0x1D0, 0x1E0, 0x1F0, 0x200,
    0x210, 0x220, 0x230, 0x240, 0x250, 0x260, 0x270, 0x280, 0x290, 0x2A0, 0x2B0,
    0x2C0, 0x2D0, 0x2E0, 0x2F0, 0x300, 0x310, 0x320, 0x330, 0x340, 0x350, 0x360,
    0x370, 0x380, 0x390, 0x3A0, 0x3B0, 0x3C0, 0x3D0, 0x3E0, 0x3F0, 0x3FF};
  const Double_t kdThrMeasVal[kuNbThrMeasPoints] = {
    -652.6, -631.2, -611.4, -590.6, -570.9, -550.0, -529.9, -509.4, -490.6,
    -469.5, -449.3, -428.5, -408.5, -388.2, -367.8, -347.2, -329.2, -308.2,
    -287.5, -266.8, -246.9, -226.0, -205.6, -185.0, -165.7, -144.9, -124.4,
    -103.8, -83.4,  -62.9,  -42.4,  -21.2,  -5.3,   15.5,   36.2,   56.8,
    77.3,   97.8,   118.4,  139.1,  158.7,  179.2,  199.7,  220.2,  240.8,
    261.1,  281.7,  302.2,  321.3,  341.4,  362.0,  382.2,  402.9,  422.8,
    443.4,  463.7,  483.7,  503.7,  524.1,  544.3,  565.0,  585.0,  605.5,
    626.0,  646.1};
  std::vector<Double_t> fvdPadiThrCodeToValue;

  /// Variables
  Int_t
    fiMonitorMode;  // Enable histograms in event builder processes and algo, 0 = OFF / 1 = ON
  Int_t
    fiDebugMonitorMode;  // Enable extra debuging histos in bth event builder and monitor processes and algo, 0 = OFF / 1 = ON
  Int_t fiNrOfGdpb;      // Total number of GDPBs
  TArrayI fiGdpbIdArray;  // Array to hold the unique IDs for all Tof GDPBs
  Int_t fiNrOfGbtx;       // Total number of Gbtx links
  Int_t fiNrOfModule;     // Total number of Modules
  TArrayI fiNrOfRpc;      // number of Rpcs connected to Gbtx link, i.e. 3 or 5
  TArrayI fiRpcType;      // type of Rpcs connected to Gbtx link
  TArrayI fiRpcSide;      // side of Rpcs connected to Gbtx link, i.e. 0 or 1
  TArrayI
    fiModuleId;  // Module Identifier connected to Gbtx link, has to match geometry

  Double_t
    fdSizeMsInNs;  // Size of the MS in ns, needed for MS border detection

  Double_t
    fdStarTriggAllowedSpread;  // Allowed "Jitter" of the triggers in different gDPB relative to each other, used to give a bit of flexibility on TS edges
  TArrayD
    fdStarTriggerDeadtime;  // STAR: Array to hold for each gDPB the deadtime between triggers in ns
  TArrayD
    fdStarTriggerDelay;  // STAR: Array to hold for each gDPB the Delay in ns to subtract when looking for beginning of coincidence of data with trigger window
  TArrayD
    fdStarTriggerWinSize;  // STAR: Array to hold for each gDPB the Size of the trigger window in ns

  ClassDef(CbmStar2019TofPar, 1);
};
#endif  // CBMSTAR2019TOFPAR_H
