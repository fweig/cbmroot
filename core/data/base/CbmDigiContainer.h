/** @file CbmDigiContainer.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 11 March 2020
 **/

#ifndef CBMDIGICONTAINER_H
#define CBMDIGICONTAINER_H 1

#include <Rtypes.h>       // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>   // for Bool_t, UInt_t, ULong64_t, kFALSE
#include <TNamed.h>       // for TNamed
#include <TString.h>      // for TString

#include <boost/any.hpp>  // for any

#include <string>         // for string

class CbmMatch;

/** @class CbmDigiContainer
 ** @brief Abstract container for digis in CBM
 ** @author V. Friese <v.friese@gsi.de>
 ** @since 11 March 2020
 **
 ** This abstract base class provides the access interface to containers
 ** of digi objects and their MC match objects.
 ** The choice of the actual container format (e.g. std::vector or TClonesArray)
 ** is left to the implementation of the concrete class.
 **/
class CbmDigiContainer : public TNamed
{

  public:

    /** @brief Constructor **/
    CbmDigiContainer(const char* name = "") : TNamed(name, "") {
    }


    /** @brief Destructor **/
    virtual ~CbmDigiContainer() {
    }


    /** @Add data to the digi container
     ** @param digi  Pointer to digi object
     ** @param match Pointer to corresponding match object
     **/
    virtual void AddDigi(boost::any, const CbmMatch*) {
    }


    /** @brief Connect the container to ROOT tree branch
     ** @return kTRUE if branch was found
     **/
    virtual Bool_t ConnectToTree() { return kFALSE; }


    /** @brief Get a digi from the container
     ** @param index Index of digi
     ** @return Pointer to constant digi object
     **
     ** The return value has to be cast to the digi class using
     ** boost::any_cast. It is in the responsibility of the implementation
     ** of the derived classes to ensure the correct data type.
     **/
    virtual boost::any GetDigi(UInt_t index) = 0;


    /** @brief Get a match object from the container
     ** @param index Index of digi
     ** @return Pointer to constant match object
     **/
    virtual const CbmMatch* GetDigiMatch(UInt_t index) = 0;


    /** @brief Presence of match branch
     ** @return kTRUE if a match branch is connected
     **/
    virtual Bool_t HasMatches() const = 0;


    /** @brief Name of container **/
    virtual const char* GetName() const { return fName; }


    /** @brief Get the number of digis in the container **/
    virtual ULong64_t GetNofDigis() const = 0;


    /** @brief String output **/
    virtual std::string ToString() const { return GetName(); }


    ClassDef(CbmDigiContainer, 1);
};


#endif /* CBMDIGICONTAINER_H */
