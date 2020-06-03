/** @file CbmDigiVector.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 11 March 2020
 **/


#ifndef CBMDIGIVECTOR_H
#define CBMDIGIVECTOR_H 1


#include <vector>
#include <FairLogger.h>

#include "CbmDigiContainer.h"
#include "CbmMatch.h"

/** @class CbmDigiVector
 ** @brief std::vector implementation of CbmDigiContainer
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 11 March 2020
 **/
template <class Digi> class CbmDigiVector : public CbmDigiContainer
{

  public:

    /** @brief Constructor
     ** @param hasMatches  If true, a match vector will be managed.
     **/
    CbmDigiVector(Bool_t hasMatches = kFALSE) : fHasMatches(hasMatches) {
      TString name = Digi::GetClassName();
      name += "_vector";
      SetName(name.Data());
    }


    /** @brief Destructor **/
    virtual ~CbmDigiVector() {
    }


    /** @brief Add a digi (and match) to the vector
     ** @param digi Pointer to digi object
     ** @param match Pointer to match object
     **
     ** After the first call to GetDigi or GetMatch, the vector is locked;
     ** adding more digis is then not possible.
     **/
    virtual void AddDigi(boost::any digi, const CbmMatch* match = nullptr) {
      if ( fIsLocked ) {
        LOG(fatal) << GetName() << "::AddDigi: Vector is locked.";
        return;
      }
      const Digi* thisDigi = boost::any_cast<const Digi*>(digi);
      if ( ! thisDigi) {
        LOG(fatal) << GetName() << "::AddDigi: Wrong argument type"
            << " (should be " << Digi::GetClassName() <<"*) !";
        return;
      } //? Any_cast successful
      fDigis.push_back(*thisDigi);
      if ( fHasMatches ) {
        if ( match == nullptr) {
          LOG(fatal) << GetName() << "::AddDigi: Valid match object required!";
          return;
        }
        fMatches.push_back(*match);
      } //? Has matches
    }


    /** @brief Connect to a ROOT TTree
     **
     ** Abstract from base class; no implementation here.
     **/
    virtual Bool_t ConnectToTree() {
      LOG(fatal) << "Digi vector cannot be connected to a TTree!";
      return false;
    }


    /** @brief Get digi object
     ** @param index  Index of digi in the vector
     ** @return Pointer to constant digi object
     **
     ** Returns a null pointer if index is out of range.
     **/
    virtual boost::any GetDigi(UInt_t index) {
      fIsLocked = kTRUE;
      const Digi* result = nullptr;
      if ( index < fDigis.size() ) result = &(fDigis[index]);
      return boost::any(result);
    }


    /** @brief Get digi match object
     ** @param index Index of digi in the vector
     ** @return Pointer to corresponding match object
     **
     ** Returns a null pointer if matches are not present or
     ** index is out of range.
     **/
    virtual const CbmMatch* GetDigiMatch(UInt_t index) {
      fIsLocked = kTRUE;
      const CbmMatch* result = nullptr;
      if ( fHasMatches && index < fMatches.size() ) result = &(fMatches[index]);
      return result;
    }


    /** @brief Presence of match objects
     ** @return True if matches are present; else false.
     **/
    virtual Bool_t HasMatches() const { return fHasMatches; }


    /** @brief Number of digis in the vector
     ** @return Number of digi objects
     **/
    virtual ULong64_t GetNofDigis() const {
      if ( fHasMatches ) assert(fMatches.size() == fDigis.size() );
      return static_cast<ULong64_t>(fDigis.size());
    }


    /** @brief String output
     ** @brief Info to string
     **/
    virtual std::string ToString() const {
      std::stringstream ss;
      ss << GetName() << ", size " << fDigis.size();
      if ( HasMatches() ) ss << ", matches present";
      return ss.str();
    }


  private:

    std::vector<Digi> fDigis { };
    std::vector<CbmMatch> fMatches { };
    Bool_t fHasMatches = kFALSE;
    Bool_t fIsLocked = kFALSE;


    ClassDef(CbmDigiVector, 1);
};

#endif /* CBMDIGIVECTOR_H */
