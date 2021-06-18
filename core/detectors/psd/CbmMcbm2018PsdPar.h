/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                 CbmMcbm2018PsdPar header file                 -----
// -----              Created 26.09.2019 by N.Karpushkin               -----
// -----         based on CbmMcbm2018TofPar by P.-A. Loizeau           -----
// -------------------------------------------------------------------------

#ifndef CbmMcbm2018PsdPar_H
#define CbmMcbm2018PsdPar_H

#include "FairParGenericSet.h"

#include "TArrayD.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class CbmMcbm2018PsdPar : public FairParGenericSet {

public:
  /** Standard constructor **/
  CbmMcbm2018PsdPar(const char* name = "CbmMcbm2018PsdPar", const char* title = "Psd unpacker parameters",
                    const char* context = "Default");

  /** Destructor **/
  virtual ~CbmMcbm2018PsdPar();

  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  static constexpr UInt_t GetNbByteMessage() { return kuBytesPerMessage; }

  //static constexpr UInt_t GetNrOfChannelsPerFee()  { return kuNbChannelsPerFee; }
  static constexpr UInt_t GetNrOfFeePerGbtx() { return kuNbFeePerGbtx; }
  static constexpr UInt_t GetNrOfGbtxPerGdpb() { return kuNbGbtxPerGdpb; }
  static constexpr UInt_t GetNrOfChannelsPerGbtx() { return kuNbChannelsPerGbtx; }
  static constexpr UInt_t GetNrOfChannelsPerGdpb() { return kuNbChannelsPerGdpb; }
  static constexpr UInt_t GetNrOfFeePerGdpb() { return kuNbFeePerGdpb; }
  inline UInt_t GetNumberOfChannels() { return kuNbChannelsPerGdpb * fiNrOfGdpb; }

  Int_t FeeChanToGbtChan(UInt_t uChannelInFee);

  inline Int_t GetDataVersion() { return fiDataVersion; }

  inline Bool_t GetMonitorMode() { return (1 == fiMonitorMode ? kTRUE : kFALSE); }
  inline Bool_t GetDebugMonitorMode() { return (1 == fiDebugMonitorMode ? kTRUE : kFALSE); }

  inline Int_t GetNrOfGdpbs() { return fiNrOfGdpb; }
  inline Int_t GetGdpbId(Int_t i) { return fiGdpbIdArray[i]; }
  inline Int_t GetNrOfFeesPerGdpb() { return fiNrOfFeesPerGdpb; }
  inline Int_t GetNrOfChannelsPerFee() { return fiNrOfChannelsPerFee; }

  inline Int_t GetNrOfGbtx() { return fiNrOfGbtx; }
  inline Int_t GetNrOfModules() { return fiNrOfModules; }
  Int_t GetModuleId(UInt_t uGbtx);

  inline Int_t GetNrOfSections() { return fiNrOfSections; }
  inline Double_t GetMipCalibration(UInt_t i) { return fdMipCalibration[i]; }

  inline Int_t GetNbMsTot() { return fiNbMsTot; }
  inline Int_t GetNbMsOverlap() { return fiNbMsOverlap; }
  inline Double_t GetSizeMsInNs() { return fdSizeMsInNs; }

  Double_t GetTsDeadtimePeriod();  // { return fdTsDeadtimePeriod;}


private:
  /// Constants
  /// Data format
  static const uint32_t kuBytesPerMessage = 8;
  /// Readout chain
  static const uint32_t kuNbChannelsPerFee  = 10;
  static const uint32_t kuNbFeePerGbtx      = 1;
  static const uint32_t kuNbGbtxPerGdpb     = 1;
  static const uint32_t kuNbChannelsPerGbtx = kuNbChannelsPerFee * kuNbFeePerGbtx;
  static const uint32_t kuNbChannelsPerGdpb = kuNbChannelsPerGbtx * kuNbGbtxPerGdpb;
  static const uint32_t kuNbFeePerGdpb      = kuNbFeePerGbtx * kuNbGbtxPerGdpb;
  /// Mapping
  const UInt_t kuFeeToGbt[kuNbChannelsPerFee] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  //! Map from Psd channel to Gbt channel

  Int_t fiDataVersion;  // Data Version
  Int_t fiMonitorMode;  // Enable histograms in event builder processes and algo, 0 = OFF / 1 = ON
  Int_t
    fiDebugMonitorMode;  // Enable extra debuging histos in bth event builder and monitor processes and algo, 0 = OFF / 1 = ON
  Int_t fiNrOfGdpb;       // Total number of GDPBs
  TArrayI fiGdpbIdArray;  // Array to hold the unique IDs for all Psd GDPBs

  Int_t fiNrOfFeesPerGdpb;     // Number of FEEs which are connected to one GDPB
  Int_t fiNrOfChannelsPerFee;  // Number of channels per FEE

  Int_t fiNrOfGbtx;     // Total number of Gbtx links
  Int_t fiNrOfModules;  // Total number of Modules
  TArrayI fiModuleId;   // Module Identifier connected to Gbtx link, has to match geometry

  Int_t fiNrOfSections; // Nr of sections
  TArrayD fdMipCalibration;   // Calibration array

  Int_t fiNbMsTot;        // Total number of MS per link in TS
  Int_t fiNbMsOverlap;    // Number of overlap MS per TS
  Double_t fdSizeMsInNs;  // Size of the MS in ns, needed for MS border detection

  Double_t
    fdTsDeadtimePeriod;  // Period (ns) in the first MS of each TS where events with missing triggers should be built using the overlap MS of previous TS (overlap events)

  ClassDef(CbmMcbm2018PsdPar, 1);
};
#endif  // CbmMcbm2018PsdPar_H
