/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmDigiBranchBase.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 4 June 2019
 **/

#ifndef CBMDIGIBRANCHBASE_H
#define CBMDIGIBRANCHBASE_H 1

#include <TString.h>  // for TString

#include <boost/any.hpp>  // for any

#include <cstddef>  // for size_t
#include <cstdint>
#include <string>  // for string

class CbmMatch;

/** @brief Abstract base class for CBM digi branches
 ** @author V. Friese <v.friese@gsi.de>
 ** @since 4 June 2019
 **
 ** Abstract interface to branches holding digi objects.
 **/
class CbmDigiBranchBase {

public:
  /** @brief Constructor
		 ** @param system  System identifier (ECbmModuleId)
		 ** @param name    Branch name
		 **/
  CbmDigiBranchBase(const char* name = "unknown") : fName(name) {}


  /** @brief Destructor **/
  virtual ~CbmDigiBranchBase() {}


  /** @brief Connect the branch to the ROOT tree
		 ** @param ioMan  Pointer to FairRootManager singleton instance
		 ** @return true if branch was found
		 **/
  virtual bool ConnectToTree() = 0;


  /** @brief Get a digi from the branch
		 ** @param index Index of digi in branch
		 ** @return Pointer to constant digi object
		 **/
  virtual boost::any GetDigi(uint32_t index) = 0;


  /** @brief Get a digi from the branch
		 ** @param index Index of digi in branch
		 ** @return Pointer to constant digi object
		 **/
  virtual const CbmMatch* GetDigiMatch(uint32_t index) = 0;


  /** @brief Presence of match branch
		 ** @return true if a match branch is connected
		 **/
  virtual bool HasMatches() = 0;


  /** @brief Name of branch **/
  TString GetName() const { return fName; }


  /** @brief Get the number of digis in the branch **/
  virtual std::size_t GetNofDigis() const = 0;


  /** @brief String output **/
  virtual std::string ToString() const { return ""; }

  /** @brief Get branch pointer **/
  virtual boost::any GetBranchContainer() const { return nullptr; }

protected:
  TString fName;  ///< Branch name
};


#endif /* CBMDIGIBRANCHBASE_H */
