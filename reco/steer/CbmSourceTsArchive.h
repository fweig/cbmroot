/** @file CbmSourceTsArchive.h
 ** @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
 ** @license SPDX-License-Identifier: GPL-3.0-only
 ** @author Volker Friese [originator]
 **/


#ifndef CBMSOURCETSARCHIVE_H
#define CBMSOURCETSARCHIVE_H 1

#include "CbmRecoUnpack.h"

#include <TimesliceSource.hpp>

#include <FairSource.h>

#include <cstdint>
#include <string>


/** @class CbmSourceTsArchive
 ** @brief Source class for reading from archived time slice data
 ** @author Volker Friese
 ** @since 2 June 2021
 **
 ** This class allows to read time-slice data from file(s) and hands them to
 ** the unpacking stage. It interfaces fles::TimesliceAutoSource to cbmroot.
 **/
class CbmSourceTsArchive : public FairSource {

public:
  /** @brief Constructor
   ** @param fileName  Name of (single) input file.
   **
   ** More input files can be added by the method AddInputFile.
   */
  CbmSourceTsArchive(const char* fileName = "");


  /** @brief Destructor **/
  virtual ~CbmSourceTsArchive() {};


  /** @brief Copy constructor - not implemented **/
  CbmSourceTsArchive(const CbmSourceTsArchive&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmSourceTsArchive& operator=(const CbmSourceTsArchive&) = delete;


  /** @brief Add an input file
   ** @param fileName  Input file name
   ** @return Number of input files after adding this one
   **/
  size_t AddInputFile(const char* fileName);


  /** @brief Close source after end of run **/
  virtual void Close();


  /** @brief Source type
   ** @return FairSource::Source_Type
   **/
  virtual Source_Type GetSourceType() { return fSourceType; }

  /**
   * @brief Get the Reco Unpack
   * Access the CbmRecoUnpack class to add unpacker configs
   * @return CbmRecoUnpack* 
  */
  CbmRecoUnpack* GetRecoUnpack() { return &fUnpack; }


  /** @brief Initialisation **/
  virtual Bool_t Init();


  /** @brief Initialise unpackers (forced by base class) **/
  virtual Bool_t InitUnpackers() { return kTRUE; }


  /** @brief Read one time slice from file **/
  Int_t ReadEvent(UInt_t = 0);


  /** @brief Re-Initialise unpackers (forced by base class) **/
  virtual Bool_t ReInitUnpackers() { return kTRUE; }


  /** @brief Reset clear the output vectors as preparation for the next timeslice. Forwarded to CbmRecoUnpack **/
  virtual void Reset() { fUnpack.Reset(); }

  /** @brief Set unpacker parameters (forced by base class) **/
  virtual void SetParUnpackers() {}

  /** @brief Set the Source Type @param type */
  void SetSourceType(Source_Type type) { fSourceType = type; }

  /** @brief Provide dummy implementation of this virtual function as not relevant in our case **/
  Bool_t SpecifyRunId() { return kTRUE; }

private:
  /** List of input file names **/
  std::vector<std::string> fFileNames = {};

  /** @brief type of source that is currently used @remark currently we use kONLINE as default, since, kFILE skipps the first TS probably due to obsolete reasons (to be checked PR072021) */
  Source_Type fSourceType = Source_Type::kONLINE;

  /** Time-slice source interface **/
  fles::TimesliceSource* fTsSource = nullptr;  //!

  /** Time-slice counter **/
  ULong64_t fTsCounter = 0;

  /** Unpack steering class **/
  CbmRecoUnpack fUnpack = {};


  ClassDef(CbmSourceTsArchive, 1)
};


#endif
