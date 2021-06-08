/* Copyright (C) 2015-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: //Dr.Sys, Volker Friese [committer], Florian Uhlig */

/** @file  CbmMCDataManager
 ** @brief An entry point for time-based analysis
 ** @author //Dr.Sys <mikhail.prokudin@cern.ch>
**/

#ifndef CBMMCDATAMANAGER_H
#define CBMMCDATAMANAGER_H 1

#include <FairTask.h>  // for FairTask, InitStatus

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Option_t
#include <TString.h>     // for TString

#include <list>    // for list
#include <map>     // for map
#include <vector>  // for vector

class CbmMCDataArray;
class CbmMCDataObject;

/** @class CbmMCDataManager
 ** @brief Task class creating and managing CbmMCDataArray objects
 ** @author //Dr.Sys <mikhail.prokudin@cern.ch>
 ** @date 31 March 2015
 **/
class CbmMCDataManager : public FairTask {

public:
  /** Default constructor **/
  CbmMCDataManager();


  /** Standard constructor
		 ** @param name    Task name
		 ** @param legacy  If true, run in legacy mode (using FairRootManager)
	   **/
  CbmMCDataManager(const char* name, Int_t legacy = 0);


  /** Destructor **/
  ~CbmMCDataManager();


  /** Add a MC input file
		 ** @param fileName  Input file name
		 ** @value           Running number of input (chain)
		 **
		 ** A new input chain will be started with this file.
		 **/
  Int_t AddFile(const char* fileName);

  /** Add a MC file to an existing input chain
		 ** @param fileName     Input file name
		 ** @param chainNumber  Number of input chain to which the file will be added
		 ** @value              Size of input chain (number of files in chain)
		 **/
  Int_t AddFileToChain(const char* fileName, Int_t chainNumber);

  /** Make TChain number chainNum2 friend of TChain number chainNum2
		 ** @param chainNum1 a number of chain for which AddFriend is called
		 ** @param chainNum2 number of chain which became a friend
		 **/
  void AddFriend(Int_t chainNum1, Int_t chainNum2) { fFriends[chainNum1] = chainNum2; }

  /** Execution (empty; just to match FairTask structure) **/
  virtual void Exec(Option_t* /*option*/) {};


  /** End of event action **/
  virtual void FinishEvent();


  /** Initialisation **/
  virtual InitStatus Init();


  /** Create a CbmMCDataArray for a given branch name
		 ** @param branchName   Name of data branch
		 ** @value              Pointer to CbmMCDataArray object for this branch
		 **/
  CbmMCDataArray* InitBranch(const char* name);

  /** Create a CbmMCDataObject for a given branch name
		 ** @param branchName   Name of data branch
		 ** @value              Pointer to CbmMCDataObject object for this branch
		 **/
  CbmMCDataObject* GetObject(const char* name);

private:
  Int_t fLegacy;  ///< If true, use FairRootManager in event-by-event mode

  /** Array of chains, each with a list of file names **/
  std::vector<std::list<TString>> fFileList;  //! Array of filename chains

  /** List of created CbmMCDataArrays **/
  std::map<TString, CbmMCDataArray*> fActive;  //!
  /** List of created CbmMCDataObjects **/
  std::map<TString, CbmMCDataObject*> fActiveObj;  //!
  /** Map of friends **/
  std::map<Int_t, Int_t> fFriends;  //!

  ClassDef(CbmMCDataManager, 1);
};

#endif
