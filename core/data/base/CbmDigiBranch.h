/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmDigiBranch.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 4 June 2019
 **/

#include "CbmDefs.h"
#include "CbmDigiBranchBase.h"
#include "CbmMatch.h"

#include <FairRootManager.h>
#include <Logger.h>

#include <TClonesArray.h>

#include <vector>

/** @brief Class template for CBM digi branches
 ** @author V. Friese <v.friese@gsi.de>
 ** @since 4 June 2019
 **
 ** Interface to branches holding CbmDigis. The branch may hold a std::vector
 ** or a TClonesArray. The requirement to the specialisation is that the
 ** class Digi derives from CbmDigi (its pointer can be cast to CbmDigi*).
 **/
template<class Digi>
class CbmDigiBranch : public CbmDigiBranchBase {

public:
  // -----------------------------------------------------------------------
  /** @brief Constructor
		 ** @param name  Branch name
		 **/
  CbmDigiBranch(const char* name = "unknown")
    : CbmDigiBranchBase(name)
    , fDigiVector(nullptr)
    , fMatchVector(nullptr)
    , fDigiArray(nullptr)
    , fMatchArray(nullptr)
  {
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Destructor **/
  virtual ~CbmDigiBranch() {}
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Connect the branch to the ROOT tree
		 ** @return true if branch is found in the ROOT tree
		 **
		 ** A std::vector is first looked for; if not found, a TClonesArray
		 ** is looked for.
		 **/
  virtual bool ConnectToTree()
  {

    FairRootManager* frm = FairRootManager::Instance();

    // Try to find a vector branch for the digi
    fDigiVector = frm->InitObjectAs<std::vector<Digi> const*>(fName.Data());

    // Try to find a TClonesArray branch for the digi
    if (!fDigiVector) { fDigiArray = dynamic_cast<TClonesArray*>(frm->GetObject(fName)); }

    // Try to find a vector branch for the match
    TString mBranch = fName + "Match";
    fMatchVector    = frm->InitObjectAs<std::vector<CbmMatch> const*>(mBranch.Data());

    // Try to find a TClonesArray branch for the match
    if (!fMatchVector) { fMatchArray = dynamic_cast<TClonesArray*>(frm->GetObject(mBranch.Data())); }

    if (fDigiVector || fDigiArray) return true;
    return false;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Number of digis
		 ** @return Current number of digis in the branch container
		 **/
  virtual std::size_t GetNofDigis() const
  {
    std::size_t nDigis = 0;
    if (fDigiVector) nDigis = fDigiVector->size();
    else if (fDigiArray) {
      assert(fDigiArray->GetEntriesFast() >= 0);
      nDigis = fDigiArray->GetEntriesFast();
    }
    return nDigis;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Get digi object
		 ** @param index  Index of digi in branch container
		 ** @return Pointer to constant digi object.
		 **
		 ** Returns a null pointer if the branch is not present.
		 **/
  virtual boost::any GetDigi(uint32_t index)
  {
    const Digi* digi = nullptr;
    if (index < GetNofDigis()) {
      if (fDigiVector) digi = &((*fDigiVector)[index]);
      else if (fDigiArray)
        digi = dynamic_cast<const Digi*>(fDigiArray->At(index));
    }
    return digi;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Get match object
		 ** @param index  Index of match in branch container
		 ** @return Pointer to constant match object.
		 **
		 ** Returns a null pointer if the branch is not present.
		 **/
  virtual const CbmMatch* GetDigiMatch(uint32_t index)
  {
    const CbmMatch* match = nullptr;
    if (index < GetNofDigis()) {
      if (fMatchVector) match = &((*fMatchVector)[index]);
      else if (fMatchArray)
        match = static_cast<const CbmMatch*>(fMatchArray->At(index));
    }
    return match;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Presence of match branch
		 ** @return true if match branch is present
		 **/
  virtual bool HasMatches()
  {
    if (fMatchVector || fMatchArray) return true;
    return false;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief String output **/
  virtual std::string ToString() const
  {
    std::stringstream ss;
    ss << "Branch " << fName << " (";
    if (fDigiVector) ss << "vector";
    else if (fDigiArray)
      ss << "TClonesArray";
    else
      ss << "not connected";
    ss << "), match branch " << fName + "Match (";
    if (fMatchVector) ss << "vector";
    else if (fMatchArray)
      ss << "TClonesArray";
    else
      ss << "not connected";
    ss << ")";
    return ss.str();
  }
  // -----------------------------------------------------------------------

  // -----------------------------------------------------------------------
  /** @brief Get branch pointer
		 ** @return Pointer to the connected data container (const)
		 ** A std::vector is first looked for; if not found, a TClonesArray
		 ** is looked for.
		 ** Returns a null pointer if the branch is not present.
		 **/
  virtual boost::any GetBranchContainer() const
  {
    if (fDigiVector) return fDigiVector;
    else if (fDigiArray)
      return fDigiArray;
    return nullptr;
  }
  // -----------------------------------------------------------------------


private:
  const std::vector<Digi>* fDigiVector;       //! Vector of Digi objects
  const std::vector<CbmMatch>* fMatchVector;  //! Vector of match objects
  TClonesArray* fDigiArray;                   //! TClonesArray of Digi objects
  TClonesArray* fMatchArray;                  //! TClonesArray of match objects
};
