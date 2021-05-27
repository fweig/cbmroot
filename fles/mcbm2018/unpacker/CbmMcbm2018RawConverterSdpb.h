// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018RawConverterSdpb                   -----
// -----               Created 28.06.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmMcbm2018RawConverterSdpb_H
#define CbmMcbm2018RawConverterSdpb_H

// Data
#include "Timeslice.hpp"

#include "StsXyterMessage.h"

// CbmRoot
#include "CbmMcbmUnpack.h"

// C/C++
#include <map>
#include <vector>

class CbmMcbm2018StsPar;

class CbmMcbm2018RawConverterSdpb : public CbmMcbmUnpack {
public:
  CbmMcbm2018RawConverterSdpb(UInt_t uNbSdpb = 1);

  CbmMcbm2018RawConverterSdpb(const CbmMcbm2018RawConverterSdpb&) = delete;
  CbmMcbm2018RawConverterSdpb operator=(const CbmMcbm2018RawConverterSdpb&) = delete;

  virtual ~CbmMcbm2018RawConverterSdpb();

  virtual Bool_t Init();
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  virtual void Reset();

  virtual void Finish();

  void SetParContainers();

  Bool_t InitContainers();

  Bool_t ReInitContainers();

  /// Temp until we change from CbmMcbmUnpack to something else
  void AddMsComponentToList(size_t component, UShort_t usDetectorId);
  void SetNbMsInTs(size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/) {};

  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE) { fbIgnoreOverlapMs = bFlagIn; }

private:
  /// Settings from parameter file
  TList* fParCList;
  CbmMcbm2018StsPar* fUnpackPar;            //!
                                            /// Readout chain dimensions and mapping
  UInt_t fuNrOfDpbs;                        //! Total number of sDPBs to convert
  std::map<UInt_t, UInt_t> fDpbIdIndexMap;  //! Map of DPB Identifier to DPB index

  /// Constants
  static const Int_t kiMaxNbFlibLinks   = 32;
  static const UInt_t kuBytesPerMessage = 4;

  /// Control flags
  std::vector<Bool_t> fvbMaskedComponents;

  /// Parameters related to FLES containers
  std::vector<size_t> fvMsComponentsList;  //!
  size_t fuNbCoreMsPerTs;                  //!
  size_t fuNbOverMsPerTs;                  //!
  size_t fuNbMsLoop;                       //!
  Bool_t fbIgnoreOverlapMs;                //! /** Ignore Overlap Ms: all fuNbOverMsPerTs MS at the end of timeslice **/
  Double_t fdMsSizeInNs;                   //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs;               //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs;               //! Total size of the core MS in a TS, [nanoseconds]

  /// Running indices
  /// TS/MS info
  ULong64_t fulCurrentTsIdx;
  ULong64_t fulCurrentMsIdx;
  Double_t fdTsStartTime;       //! Time in ns of current TS from the index of the first MS first component
  Double_t fdTsStopTimeCore;    //! End Time in ns of current TS Core from the index of the first MS first component
  Double_t fdMsTime;            //! Start Time in ns of current MS from its index field in header
  UInt_t fuMsIndex;             //! Index of current MS within the TS
                                /// Current data properties
  UInt_t fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
  UInt_t fuCurrDpbId;           //! Temp holder until Current equipment ID is properly filled in MS
  UInt_t fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming

  /// Output vector
  std::vector<std::vector<stsxyter::Message>*> fvSdpbMessages;  //! Output arrays, 1 per DPB

  ClassDef(CbmMcbm2018RawConverterSdpb, 1)
};

#endif
