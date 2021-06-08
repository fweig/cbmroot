/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmDigitizeInfo.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 18.05.2018
 **/


#ifndef CBMDIGITIZEINFO_H
#define CBMDIGITIZEINFO_H 1

#include "CbmDigitizeBase.h"

#include "TObject.h"
#include "TString.h"


/** @class CbmDigitizeInfo
 ** @brief Auxiliary class for CbmDigitize
 **
 ** The class CbmDigitizeInfo keeps information on the digitisation for
 ** a detector system, like the instance of the digitizer, the input
 ** branch name (MCPoint), and flags indicating the presence of the input
 ** branch and the persistence of the output data (digis).
 **/
class CbmDigitizeInfo : public TObject {

public:
  /** @brief Constructor
     ** @param system     System ID (ECbmModuleId)
     ** @param present    kTRUE if MC data branch is present
     ** @param active     Digitizer will be run if kTRUE
     ** @param persistent Output data will be persistent if kTRUE
     ** @param branch     Name of MCPoint branch as input
     ** @param digitizer  Pointer to digitizer instance
     **/
  CbmDigitizeInfo(ECbmModuleId system, TString branch = "", CbmDigitizeBase* digitizer = nullptr,
                  Bool_t present = kFALSE, Bool_t active = kTRUE, Bool_t persistent = kTRUE)
    : fSystem(system)
    , fBranch(branch)
    , fDigitizer(digitizer)
    , fPresent(present)
    , fActive(active)
    , fPersistent(persistent)
  {
  }


  /** Destructor **/
  virtual ~CbmDigitizeInfo() {};


  /** @brief Get input branch name
     ** @value Input branch name
     **/
  TString GetBranchName() const { return fBranch; }


  /** @brief Get digitizer
     ** @value Pointer to digitizer instance
     **/
  CbmDigitizeBase* GetDigitizer() const { return fDigitizer; }


  /** @brief Activity flag
     ** @value If kTRUE, if digitizer will be active
     **/
  Bool_t IsActive() const { return fActive; }


  /** @brief Data presence flag
     ** @value kTRUE if input branch is present in the tree
     **/
  Bool_t IsPresent() const { return fPresent; }

  /** @brief Set activity flag
     ** @param choice If kTRUE, the digitizer will be active.
     **/
  void SetActive(Bool_t choice = kTRUE) { fActive = choice; }


  /** @brief Set input branch name
     ** @param branch  Name of input branch (MCPoint array)
     **/
  void SetBranchName(TString branch) { fBranch = branch; }


  /** @brief Set digitizer instance
     ** @param digitizer Pointer to digitizer instance
     **/
  void SetDigitizer(CbmDigitizeBase* digitizer) { fDigitizer = digitizer; }


  /** @brief Set persistence flag
     ** @param choice If kTRUE, the output data (digis) will be persistent.
     */
  void SetPersistent(Bool_t choice = kTRUE) { fPersistent = choice; }


  /** @brief Set data presence flag
     ** @param choice kTRUE if the input data branch is present in the tree
     **/
  void SetPresent(Bool_t choice = kTRUE) { fPresent = choice; }


  /** @brief Info to string **/
  std::string ToString() const;


private:
  ECbmModuleId fSystem;         // System ID (ECbmModuleId)
  TString fBranch;              // Branch (array) name
  CbmDigitizeBase* fDigitizer;  // Pointer to digitizer instance
  Bool_t fPresent;              // Data branch is present in input tree
  Bool_t fActive;               // Digitizer is active
  Bool_t fPersistent;           // Digitizer output is persistent


  /** @brief Copy constructor forbidden **/
  CbmDigitizeInfo(const CbmDigitizeInfo&) = delete;


  /** @brief Assignment operator forbidden **/
  CbmDigitizeInfo operator=(const CbmDigitizeInfo&) = delete;


  ClassDef(CbmDigitizeInfo, 2);
};

#endif /* CBMDIGITIZEINFO_H */
