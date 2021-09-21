/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#ifndef CbmMcbm2020TrdTshiftPar_H
#define CbmMcbm2020TrdTshiftPar_H

#include "FairParGenericSet.h"  // mother class

// STD
#include <map>

// ROOT
#include <TArrayD.h>
#include <TArrayI.h>
#include <TFile.h>
#include <TH2.h>
#include <TH3.h>

class CbmMcbm2020TrdTshiftPar : public FairParGenericSet {
public:
  CbmMcbm2020TrdTshiftPar(const char* name    = "CbmMcbm2020TrdTshiftPar",
                          const char* title   = "TRD timeshift unpacker parameters mCbm 2020",
                          const char* context = "Default");

  virtual ~CbmMcbm2020TrdTshiftPar();

  virtual void printparams();

  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  bool GetTimeshifts(std::shared_ptr<TH2> timeshiftHisto);
  ///< Extract the timeshift values from a histo containing the information and write them to fTimeshifts.

  double GetNEvents(std::shared_ptr<TFile> mcbmanafile);
  ///< Extract the number of events from a mcbmana task output file, which has to contain the required histogram.


  std::vector<Int_t> GetTimeshiftsVec(size_t tsidx);
  ///< Return the timeshift vector for a given Timeslice Index. Works only if getParams() was run before

  std::map<size_t, std::vector<Int_t>>* GetTimeshiftsMap() { return &fmapTimeshifts; }
  ///< Return the timeshift map.


  std::shared_ptr<TH3> GetCalibHisto(std::shared_ptr<TFile> mcbmanafile);
  ///< Extract the required base histogram from a mcbmana task output file.

  std::shared_ptr<TH2> GetCalibHisto(std::shared_ptr<TH3> calibbasehisto);
  ///< Extract the timeshiftHisto from the calibbase histogram. The calibbase histogram is a TH3* with the tsIdx, the module channels and the timeshifts on the axes.

  Int_t GetDominantShift(std::shared_ptr<TH3> calibbasehisto, size_t itsidx, size_t ichannel);
  ///< Extract the dominant shift value from the calibbase histogram. For a give tsIdx and channel.

  bool FillTimeshiftArray(std::shared_ptr<TFile> mcbmanafile);
  ///< Extract the timeshift values from a TAF output file that contains the required histograms and write them to fTimeshifts.

private:
  const std::vector<std::string> pararraynames = {"nTimeslices", "TrdTimeshifts"};
  ///< Names of the parameter arrays

  const size_t fgNchannels = 768;
  ///< Number of channels on the single Trd module used during mCbm2020
  // In principle this could be retrieved from the standard parameters. But since we hopefully need these ugly parameters only for mCbm2020 it is hardcoded, because a handling via the parameters would require full linking of the standard Trd parameter classes

  const UInt_t fgMicroSliceLength = 1.280e6;
  ///< Length of a single microslice during mCbm 2020 data taking

  TArrayI fTimeshifts = {};
  ///< Array contains the timeshifts. Everytime a timeshift appears the tsIdx from the TS MetaData is followed by the correct shift value for each single channel

  TArrayI fNtimeslices;
  ///< Number of timeslices in the given run. This is required to fill a complete fmapTimeshifts, when reading the compressed information from the ascii parameter file

  std::map<size_t, std::vector<Int_t>> fmapTimeshifts = {};  //!
  ///< Keys of the map are the tsIdx when a shift value for any channel changes, the vector contains the shift values for the given period for all channels. It is represented by fvecCurrentTimeshifts

  std::vector<Int_t> fvecCurrentTimeshifts = {};  //!
  ///< Vector containing the "current" timeshift values for all Trd channles of the mCbm2020 module

public:
  ClassDef(CbmMcbm2020TrdTshiftPar, 2);
};

#endif  // CbmMcbm2020TrdTshiftPar_H
