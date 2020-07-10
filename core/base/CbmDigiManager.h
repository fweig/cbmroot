/** @file CbmDigiManager.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 8 May 2007
 **/


#ifndef CBMDIGIMANAGER_H
#define CBMDIGIMANAGER_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, Cla...
#include <RtypesCore.h>  // for UInt_t, Bool_t, Int_t, kTRUE

#include <FairLogger.h>  // for LOG
#include <FairTask.h>    // for InitStatus

#include <boost/any.hpp>                  // for any_cast, bad_any_cast (ptr...
#include <boost/exception/exception.hpp>  // for clone_impl, error_info_inje...

#include <assert.h>  // for assert
#include <iosfwd>    // for string
#include <map>       // for map, map<>::mapped_type
#include <vector>    // for vector

#include "CbmDefs.h"            // for ECbmModuleId
#include "CbmDigiBranchBase.h"  // for CbmDigiBranchBase

class CbmMatch;

/** @brief CbmDigiManager
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 08.05.07
 ** @date 5 June 2019
 **
 ** Interface class to provide access to CbmDigis.
 ** The storage model (STL vector or TClonesArray) is abstracted from.
 **/
class CbmDigiManager {

public:
  /**  Destructor  **/
  virtual ~CbmDigiManager();


  /** @brief Get a digi object
     ** @param  index Index of digi in its container
     ** @return Pointer to constant digi object
     **
     ** Requirement to the template type Digi is that its pointer can be
     ** cast to CbmDigi* and that it has a static method GetSystem().
     **/
  template<class Digi>
  const Digi* Get(Int_t index) const {
    assert(fIsInitialised);
    ECbmModuleId system = Digi::GetSystem();
    if (fBranches.find(system) == fBranches.end()) return nullptr;
    try {
      return boost::any_cast<const Digi*>(fBranches[system]->GetDigi(index));
    } catch (
      const boost::exception_detail::clone_impl<
        boost::exception_detail::error_info_injector<boost::bad_any_cast>>&) {
      LOG(fatal)
        << "Failed boost any_cast in Digimanager::Get for a digi of type "
        << Digi::GetClassName();
    }  // catch only boost::bad_any_cast which can be triggered by CbmMuchDigi/CbmMuchBeamTimeDigi
    return nullptr;
  }


  /** @brief Get a match object
     ** @param  System identifier (ECbmModuleId)
     ** @param  index Index of digi/match in their container
     ** @return Pointer to constant match object
     **/
  const CbmMatch* GetMatch(ECbmModuleId systemId, UInt_t index) const;


  /** Number of digis for a given system
     ** @param  System identifier (ECbmModuleId)
     ** @return Number of digis for system
     **/
  static Int_t GetNofDigis(ECbmModuleId systemId);


  /** @brief Initialisation
     ** @return kSUCCESS is successful
     **
     ** The input tree is checked for digi branches.
     **/
  InitStatus Init();


  /** @brief Static instance **/
  static CbmDigiManager* Instance() {
    if (!fgInstance) fgInstance = new CbmDigiManager();
    return fgInstance;
  }


  /** @brief Presence of a digi branch
     ** @param  System identifier (ECbmModuleId)
     ** @return kTRUE if digi branch is present
     **/
  static Bool_t IsPresent(ECbmModuleId systemId);


  /** @brief Presence of a digi match branch
     ** @param  System identifier (ECbmModuleId)
     ** @return kTRUE if digi match branch is present
     **/
  static Bool_t IsMatchPresent(ECbmModuleId systemId);


  /** @brief Set the digi branch name for a system
     ** @param system  System identifier
     ** @param name    Branch name
     **
     ** This can be used if the branch name in the input does
     ** not follow the convention (default).
     **/
  void SetBranchName(ECbmModuleId system, const char* name) {
    fBranchNames[system] = std::string(name);
  }


  /** @brief Use CbmMuchBeamTimeDigi instead of CbmMuchDigi for MUCH
     ** @param choice If true, use CbmMuchBeamTimeDigi
     **
     ** Temporary solution until the classes are unified.
     **/
  void UseMuchBeamTimeDigi(Bool_t choice = kTRUE) {
    fUseMuchBeamTimeDigi = choice;
  }


private:
  static std::map<ECbmModuleId, CbmDigiBranchBase*> fBranches;  //!
  static CbmDigiManager* fgInstance;                            //!
  static Bool_t fIsInitialised;
  std::map<ECbmModuleId, std::string> fBranchNames {};
  static Bool_t fUseMuchBeamTimeDigi;


  /**  Default constructor  **/
  CbmDigiManager();


  /** Copy constructor forbidden **/
  CbmDigiManager(const CbmDigiManager&) = delete;


  /** Assignment operator forbidden **/
  CbmDigiManager& operator=(const CbmDigiManager&) = delete;


  /** @brief Set a digi branch **/
  template<class Digi>
  void SetBranch();


  ClassDef(CbmDigiManager, 5);
};


#endif /* CBMDIGIMANAGER_H */
