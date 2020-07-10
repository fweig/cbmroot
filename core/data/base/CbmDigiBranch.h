/** @file CbmDigiBranch.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 4 June 2019
 **/

#include <vector>

#include <FairLogger.h>
#include <FairRootManager.h>
#include <TClonesArray.h>

#include "CbmDefs.h"
#include "CbmDigiBranchBase.h"
#include "CbmMatch.h"

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
    , fMatchArray(nullptr) {}
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
  virtual Bool_t ConnectToTree() {

    FairRootManager* frm = FairRootManager::Instance();

    // Try to find a vector branch for the digi
    fDigiVector = frm->InitObjectAs<std::vector<Digi> const*>(fName.Data());

    // Try to find a TClonesArray branch for the digi
    if (!fDigiVector) {
      fDigiArray = dynamic_cast<TClonesArray*>(frm->GetObject(fName));
    }

    // Try to find a vector branch for the match
    TString mBranch = fName + "Match";
    fMatchVector =
      frm->InitObjectAs<std::vector<CbmMatch> const*>(mBranch.Data());

    // Try to find a TClonesArray branch for the match
    if (!fMatchVector) {
      fMatchArray = dynamic_cast<TClonesArray*>(frm->GetObject(mBranch.Data()));
    }

    if (fDigiVector || fDigiArray) return kTRUE;
    return kFALSE;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Number of digis
		 ** @return Current number of digis in the branch container
		 **/
  virtual std::size_t GetNofDigis() const {
    std::size_t nDigis = 0;
    if (fDigiVector)
      nDigis = fDigiVector->size();
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
  virtual boost::any GetDigi(UInt_t index) {
    const Digi* digi = nullptr;
    if (index < GetNofDigis()) {
      if (fDigiVector)
        digi = &((*fDigiVector)[index]);
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
  virtual const CbmMatch* GetDigiMatch(UInt_t index) {
    const CbmMatch* match = nullptr;
    if (index < GetNofDigis()) {
      if (fMatchVector)
        match = &((*fMatchVector)[index]);
      else if (fMatchArray)
        match = static_cast<const CbmMatch*>(fMatchArray->At(index));
    }
    return match;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief Presence of match branch
		 ** @return kTRUE if match branch is present
		 **/
  virtual Bool_t HasMatches() {
    if (fMatchVector || fMatchArray) return kTRUE;
    return kFALSE;
  }
  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  /** @brief String output **/
  virtual std::string ToString() const {
    std::stringstream ss;
    ss << "Branch " << fName << " (";
    if (fDigiVector)
      ss << "vector";
    else if (fDigiArray)
      ss << "TClonesArray";
    else
      ss << "not connected";
    ss << "), match branch " << fName + "Match (";
    if (fMatchVector)
      ss << "vector";
    else if (fMatchArray)
      ss << "TClonesArray";
    else
      ss << "not connected";
    ss << ")";
    return ss.str();
  }
  // -----------------------------------------------------------------------


private:
  const std::vector<Digi>* fDigiVector;       //! Vector of Digi objects
  const std::vector<CbmMatch>* fMatchVector;  //! Vector of match objects
  TClonesArray* fDigiArray;                   //! TClonesArray of Digi objects
  TClonesArray* fMatchArray;                  //! TClonesArray of match objects
};
