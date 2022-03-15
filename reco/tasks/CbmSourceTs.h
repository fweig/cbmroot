/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese[committer] */

#ifndef CBMSOURCETS_H
#define CBMSOURCETS_H 1


#include <TimesliceSource.hpp>

#include <FairSource.h>

#include <cstdint>
#include <memory>
#include <string>


/** @class CbmSourceTs
 ** @brief Source class for reading from archived time slice data
 ** @author Volker Friese
 ** @since 14 February 2022
 **
 ** This class allows to read time-slice data from file(s).
 ** It interfaces fles::TimesliceAutoSource to cbmroot.
 **/
class CbmSourceTs : public FairSource {

public:
  /** @brief Constructor
   ** @param fileName  Name of (single) input file.
   **
   ** More input files can be added by the method AddInputFile.
   */
  CbmSourceTs(const char* fileName = "");

  /** @brief Constructor
   ** @param fileName  Vector with name(s) of input file(s).
   **
   ** More input files can be added by the method AddInputFile.
   */
  CbmSourceTs(std::vector<std::string> fileNames);


  /** @brief Destructor **/
  virtual ~CbmSourceTs() {};


  /** @brief Copy constructor - not implemented **/
  CbmSourceTs(const CbmSourceTs&) = delete;


  /** @brief Assignment operator - not implemented **/
  CbmSourceTs& operator=(const CbmSourceTs&) = delete;


  /** @brief Add an input file
   ** @param fileName  Input file name
   ** @return Number of input files after adding this one
   **/
  size_t AddInputFile(const char* fileName);


  /** @brief Demanded by base class **/
  virtual void Close();


  /** @brief Number of processed timeslices
   ** @return Number of timeslices
   **
   ** The first call to ReadEvent is in Init, so not timeslice is processed.
   **/
  size_t GetNumTs() const { return fNumCalls - 1; }


  /** @brief Demanded by base class **/
  virtual Source_Type GetSourceType() { return kFILE; }


  /** @brief Pointer to current FLES timeslice **/
  //std::shared_ptr<fles::Timeslice> GetTimeslice() { return fFlesTs; }
  fles::Timeslice* GetTimeslice() { return fFlesTs.get(); }


  /** @brief Initialisation **/
  virtual Bool_t Init();


  /** @brief Demanded by base class **/
  virtual Bool_t InitUnpackers() { return kTRUE; }


  /** @brief Read one time slice from file **/
  virtual Int_t ReadEvent(UInt_t = 0);


  /** @brief Demanded by base class **/
  virtual Bool_t ReInitUnpackers() { return kTRUE; }


  /** @brief Demanded by base class **/
  virtual void Reset() {}


  /** @brief Demanded by base class **/
  virtual void SetParUnpackers() {}


  /** @brief Demanded by base class **/
  virtual Bool_t SpecifyRunId() { return kTRUE; }


private:  // member variables
  /** List of input file names **/
  std::vector<std::string> fFileNames = {};

  /** FLES interface **/
  fles::TimesliceSource* fFlesSource = nullptr;  //!

  /** Pointer to current time slice **/
  std::unique_ptr<fles::Timeslice> fFlesTs = nullptr;  //!

  /** ReadEvent call counter **/
  size_t fNumCalls = 0;


  ClassDef(CbmSourceTs, 1)
};


#endif
