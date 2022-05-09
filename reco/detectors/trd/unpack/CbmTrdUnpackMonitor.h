/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmTrdUnpackMonitor.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Monitor class to monitor the data from the Trd unpacker algorithms
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * This class can be attached to an unpacker algorithm class. It will than take the 
 * CbmTrdDigi and CbmTrdRawMessageSpadic object to fill predefined histograms with the 
 * given information.
 * 
 * 
*/

#ifndef CbmTrdUnpackMonitor_H
#define CbmTrdUnpackMonitor_H

#include "CbmTrdDigi.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdSpadic.h"

#include <MicrosliceDescriptor.hpp>
#include <Timeslice.hpp>

#include <FairRunOnline.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>  // for types
#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <THttpServer.h>  // for histogram server

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <cmath>

class CbmTrdUnpackMonitor {
public:
  /** @brief Enum for the predefined digi histograms. */
  enum class eDigiHistos : size_t
  {
    kMap = 0,
    kMap_St,
    kMap_Nt,
    kChannel,
    kChannel_St,
    kChannel_Nt,
    kCharge,
    kCharge_St,
    kCharge_Nt,
    kTriggerType,
    kDigiDeltaT,
    kDigiNtCorr
  };

  /** @brief Enum for the predefined raw histograms. */
  enum class eRawHistos : size_t
  {
    kSignalshape = 0,
    kSignalshape_St,
    kSignalshape_Nt,
    kMap,
    kMap_St,
    kMap_Nt,
    kElinkId,
    kSampleDistStdDev,
    kSample0perChannel,
    kHitType,
  };

  /** @brief Enum for the predefined other histograms. */
  enum class eOtherHistos : size_t
  {
    kSpadic_Info_Types = 0,
    kMs_Flags
  };

  /** @brief Create the Cbm Trd Unpack AlgoBase object */
  CbmTrdUnpackMonitor(/* args */);

  /** @brief Destroy the Cbm Trd Unpack Task object */
  virtual ~CbmTrdUnpackMonitor();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackMonitor(const CbmTrdUnpackMonitor&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackMonitor& operator=(const CbmTrdUnpackMonitor&) = delete;

  /** @brief fill the stored digi histograms @param digi pointer to the digi @param raw pointer to the raw msg */
  void FillHistos(CbmTrdDigi* digi, CbmTrdRawMessageSpadic* raw = nullptr);

  /**
   * @brief Fill the given histo with the information on the info type
   * @param[in] type Spadic::MsInfoType
   * @param[in] moduleid unique CbmAddress moduleid
  */
  void FillHisto(Spadic::MsInfoType type, std::uint32_t moduleid);

  /**
   * @brief Fill the given histo with the information on the flag
   * @param[in] flag fles::MicrosliceFlags
   * @param[in] moduleid unique CbmAddress moduleid
  */
  void FillHisto(fles::MicrosliceFlags flag, std::uint32_t moduleid);

  /** @brief Actions at the end of the run, e.g. write histos to file if flag is set. */
  void Finish();

  // Runtime functions
  /** @brief Init all required parameter informations */
  Bool_t Init(CbmTrdParSetDigi* digiParSet, CbmTrdParSetAsic* asicParSet = nullptr);

  /** @brief transfer the enums for the histos to be activated to the member vector */
  void SetActiveHistos(std::vector<eDigiHistos> vec) { fActiveDigiHistos.swap(vec); }

  /** @brief transfer the enums for the histos to be activated to the member vector */
  void SetActiveHistos(std::vector<eRawHistos> vec) { fActiveRawHistos.swap(vec); }

  /** @brief transfer the enums for the histos to be activated to the member vector */
  void SetActiveHistos(std::vector<eOtherHistos> vec) { fActiveOtherHistos.swap(vec); }

  /** @brief Set the Spadic Object @param value */
  void SetSpadicObject(std::shared_ptr<CbmTrdSpadic> value) { fSpadic = value; }

  /** @brief Set the output filename, automatically also sets the flag to create an output file. @param filename Absolute path for the output file */
  void SetWriteToFile(std::string filename)
  {
    fOutfilename   = filename;
    fDoWriteToFile = true;
  }

  /** @brief Set digi outpout vector (to make it usable for correlations) */
  void SetDigiOutputVec(std::vector<CbmTrdDigi>* digiOutputVec) { fDigiOutputVec = digiOutputVec; }


protected:
  template<class histotype>
  void addHistoToMap(std::shared_ptr<TH1> histo,
                     std::map<histotype, std::map<std::uint32_t, std::shared_ptr<TH1>>>* histomap,
                     std::uint32_t moduleid, histotype kHisto)
  {
    // If the histogram already exists we stop here
    if (checkIfHistoExists(kHisto, histomap, moduleid)) return;

    // Create a histo module pair
    auto histopair = std::make_pair(moduleid, histo);

    // Check if already have a histo map for the histo category
    auto histomapIt = histomap->find(kHisto);
    if (histomapIt == histomap->end()) {
      // There is no map yet for the given histogram category
      std::map<std::uint32_t, std::shared_ptr<TH1>> newmap = {};
      newmap.emplace(histopair);
      // Create a pair with the map and the histogram category
      auto pair = std::make_pair(kHisto, newmap);
      // And put it to the digi histo map
      histomap->emplace(pair);
    }
    else {
      // We found a map where we can put the histopair into
      histomapIt->second.emplace(histopair);
    }
    // And finally if we have HttpServer we pass the histogram pointer to it
    if (fHistoServer) {

      std::string directory = std::to_string(moduleid) + "/" + getHistoType(histo) + "/";
      fHistoServer->Register(directory.data(), histo.get());
    }
  }

  template<typename THistotype>
  bool checkIfHistoExists(THistotype etype,
                          std::map<THistotype, std::map<std::uint32_t, std::shared_ptr<TH1>>>* histomap,
                          std::uint32_t moduleid)
  {
    // First check if the map knows about the type, if not the histo does not exist yet.
    auto histotypemapIt = histomap->find(etype);
    if (histotypemapIt == histomap->end()) return false;

    // Check if at the moduleId position we find something
    auto histopair = histotypemapIt->second.find(moduleid);
    if (histopair == histotypemapIt->second.end()) return false;

    // Check if there is a pointer to the histo which not null
    if (histopair->second != nullptr) return true;

    return false;
  }

  /** @brief Create the actual TH1 shared_ptrs */
  virtual void createHistos();

  /** @brief Create the actual TH1 shared_ptrs of the Digi histos */
  virtual void createHisto(eDigiHistos kHisto);

  /** @brief Create the actual TH1 shared_ptrs of the Raw histos */
  virtual void createHisto(eRawHistos kHisto);

  /** @brief Create the actual TH1 shared_ptrs of the Others histos */
  virtual void createHisto(eOtherHistos kHisto);

  /**
   * @brief Fill the given histo with the information from the digi
   * 
   * @param[in] digi CbmTrdDigi 
   * @param[in] kHisto Histo definition
   * @param[in] moduleid Unique module Id from which the digi came
   * @param[out] histo pointer to the histo (we do not want to extract it a snd time from the map)
  */
  virtual void fillHisto(CbmTrdDigi* digi, eDigiHistos kHisto, std::uint32_t moduleid, std::shared_ptr<TH1> histo);

  /**
   * @brief Fill the given histo with the information from the raw message
   * 
   * @param[in] raw CbmTrdRawMessageSpadic 
   * @param[in] kHisto Histo definition
   * @param[out] histo pointer to the histo (we do not want to extract it a snd time from the map)
  */
  void fillHisto(CbmTrdRawMessageSpadic* raw, eRawHistos kHisto, std::shared_ptr<TH1> histo, CbmTrdDigi* digi);

  /**
   * @brief Fill the passed histo with the samples as function of time
   * 
   * @param histo 
   * @param raw 
  */
  void fillSamplesHisto(std::shared_ptr<TH1> histo, CbmTrdRawMessageSpadic* raw);

  /** @brief Get the time difference between this digi and the previous one from the channel of this digi @param digi CbmTrdDigi @return delta t [ns]*/
  std::double_t getDeltaT(CbmTrdDigi* digi);

  /** @brief Get the Histo Name for the given histo @param kHisto eDigiHistos @return std::string */
  std::string getHistoName(eDigiHistos kHisto);

  /** @brief Get the Histo Name for the given histo @param kHisto eRawHistos @return std::string */
  std::string getHistoName(eRawHistos kHisto);

  /** @brief Get the Histo Name for the given histo @param kHisto eOtherHistos @return std::string */
  std::string getHistoName(eOtherHistos kHisto);

  /** @brief Get the Type Name for the given histo @param kHisto eDigiHistos @return std::string */
  static std::string getTypeName(eDigiHistos kHisto)
  {
    (void) kHisto;
    return "Digi";
  };

  /** @brief Get the Type Name for the given histo @param kHisto eRawHistos @return std::string */
  static std::string getTypeName(eRawHistos kHisto)
  {
    (void) kHisto;
    return "Raw";
  };

  /** @brief Get the Type Name for the given histo @param kHisto eOtherHistos @return std::string */
  static std::string getTypeName(eOtherHistos kHisto)
  {
    (void) kHisto;
    return "Other";
  };

  /** @brief Get the Histo Type, i.e. "Digi/Raw/Other", deduced from the histo name. @param histo @return std::string */
  std::string getHistoType(std::shared_ptr<TH1> histo);

  /**
   * @brief Get the row and column ids (potentially rotated chambers are adjusted to humand readable rotations)
   * 
   * @param moduleid 
   * @param channelid 
   * @return std::pair<std::uint32_t, std::uint32_t> {row,col}
  */
  std::pair<std::uint32_t, std::uint32_t> getRowAndCol(std::uint32_t moduleid, std::uint32_t channelid);

  /** @brief Extract the std deviation of all samples in the message */
  std::float_t getSamplesStdDev(CbmTrdRawMessageSpadic* raw);

  template<class histotype>
  size_t writeHistosToFile(std::map<histotype, std::map<std::uint32_t, std::shared_ptr<TH1>>>* histomap, TFile* file)
  {
    // Counter of written histos
    size_t nhistos = 0;

    // Make sure we are in the file to which we want to write our histos
    file->cd();
    for (auto typemappair : *histomap) {
      for (auto histopair : typemappair.second) {

        // Make sure we end up in chosen folder
        std::string moduleidname = std::to_string(histopair.first);
        if (nullptr == gDirectory->Get(moduleidname.data())) gDirectory->mkdir(moduleidname.data());
        gDirectory->cd(moduleidname.data());

        // Now move(create) to the histotype folder (digi, raw or other histo)
        std::string histotypename = getTypeName(typemappair.first);
        // (Create and) Move to the directory of the type
        if (nullptr == gDirectory->Get(histotypename.data())) gDirectory->mkdir(histotypename.data());
        gDirectory->cd(histotypename.data());

        // Write histogram
        LOG(debug) << Class_Name() << "::Finish() Write histo " << histopair.second->GetName() << " to file "
                   << file->GetName() << " in folder " << moduleidname.data() << "/" << histotypename.data();
        histopair.second->Write();
        nhistos++;
        // Move back to root directory of the output file
        file->cd();
      }
    }
    return nhistos;
  }

  /** @brief Fill the NeighborTrigger Checking Histogram */
  void fillNtCorrHisto(std::shared_ptr<TH1> histo, CbmTrdDigi* digi);

  // Member variables
  /** @brief Digi histogram pointers stored in a map together with the module id */
  std::map<eDigiHistos, std::map<std::uint32_t, std::shared_ptr<TH1>>> fDigiHistoMap = {};

  /** @brief Raw histogram pointers stored in a map together with the module id */
  std::map<eRawHistos, std::map<std::uint32_t, std::shared_ptr<TH1>>> fRawHistoMap = {};

  /** @brief Other histogram pointers stored in a map together with the module id */
  std::map<eOtherHistos, std::map<std::uint32_t, std::shared_ptr<TH1>>> fOtherHistoMap = {};

  /** @brief Enums of Digi histos to be activated */
  std::vector<eDigiHistos> fActiveDigiHistos = {};

  /** @brief Enums of Raw histos to be activated */
  std::vector<eRawHistos> fActiveRawHistos = {};

  /** @brief Enums of Raw histos to be activated */
  std::vector<eOtherHistos> fActiveOtherHistos = {};

  /** @brief Pointer to the histogram server, in case we run the online monitoring, the pointer is automatically deduced from the run. */
  THttpServer* fHistoServer = nullptr;

  /** @brief Flag whether to write histos to file or not, gets activated if a output filename gets set. */
  bool fDoWriteToFile = false;

  /** @brief File name for the output file */
  std::string fOutfilename = "";

  // ---- TRD parameters ----
  /** @brief Vector with the unique module Ids */
  std::vector<std::uint32_t> fModuleIdsVec = {};

  /** @brief Map with the orientations of the modules. Performance helper to not go through the extraction from ParModDigi everytime */
  std::map<std::uint32_t, std::uint8_t> fModuleOrientation = {};

  /** @brief Map with the number of rows of the modules. Performance helper to not go through the extraction from ParModDigi everytime */
  std::map<std::uint32_t, std::uint8_t> fModuleNrRows = {};

  /** @brief Map with the number of columns of the modules. Performance helper to not go through the extraction from ParModDigi everytime */
  std::map<std::uint32_t, std::uint8_t> fModuleNrColumns = {};

  /** @brief Map with the last digi time for each channel of a given module */
  std::map<std::uint32_t, std::vector<size_t>> fLastDigiTimeMap = {};

  /** @brief Variable which holds a reference to the TRD digi output vector (for correlations) */
  std::vector<CbmTrdDigi>* fDigiOutputVec = {};

  // All other parameters and containers
  std::shared_ptr<CbmTrdSpadic> fSpadic = nullptr;

private:
  ClassDef(CbmTrdUnpackMonitor, 2)
};

#endif  // CbmTrdUnpackMonitor_H
