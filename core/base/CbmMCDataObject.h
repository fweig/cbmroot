/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mikhail Prokudin, Volker Friese [committer] */

/** @file  CbmMCDataObject.h
 ** @author //Dr.Sys <mikhail.prokudin@cern.ch>
 **/

#ifndef CBMMCDATAOBJECT_H
#define CBMMCDATAOBJECT_H

#include "CbmLink.h"  // for CbmLink

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Long64_t
#include <TString.h>     // for TString

#include <list>    // for list
#include <map>     // for map
#include <vector>  // for vector

class TChain;
class TObject;

/** @class CbmMCDataObject
 ** @brief Access to a MC data branch for time-based analysis
 ** @author //Dr.Sys <mikhail.prokudin@cern.ch>
 ** @date 31 March 2015
 **
 ** This class provides cached access to data for an event-by-event MC data branch,
 ** which is used when matching time-based reconstruction data to the input MC data.
 ** Data objects from already used events are cached, which allows switching between
 ** events without the performance deterioration induced by frequent calls to
 ** TTree::GetEntry().
 ** In legacy mode, the data from the current event will be retrieved from
 ** FairRootManager.
 **/
class CbmMCDataObject {
  friend class CbmMCDataManager;

public:
  /** Destructor **/
  virtual ~CbmMCDataObject() {}


  /** Get an object by CbmLink pointer
		 ** @param link Pointer to CbmLink object
		 ** @value      Pointer to linked object
		 **/
  TObject* Get(const CbmLink* lnk) { return Get(lnk->GetFile(), lnk->GetEntry()); }


  /** Get an object by CbmLink reference
		 ** @param link Reference to CbmLink object
		 ** @value      Pointer to linked object
		 **/
  TObject* Get(const CbmLink& lnk) { return Get(lnk.GetFile(), lnk.GetEntry()); }


  /** Get an object by file number and event number
		 ** @param fileNumber	  Input file number
		 ** @param eventNumber  Event number
		 ** @value              Pointer to referenced object
		 **/
  TObject* Get(Int_t fileNumber, Int_t eventNumber);

private:
  /** Default constructor. Should be called from CbmMCDataManager. **/
  CbmMCDataObject() : fLegacy(0), fLegacyObject(nullptr), fBranchName(), fSize(0), fChains(), fTArr(), fN(0), fArrays()
  {
  }

  /** Standard constructor. Should be called from CbmMCDataManager.
		 ** @param branchName   Name of data branch
		 ** @param fileList     Vector of file list (one for each input source)
		 **/
  CbmMCDataObject(const char* branchName, const std::vector<std::list<TString>>& fileList);

  /** Constructor in legacy mode. Gets the branch from FairRootManager.
	   ** @param branchName  Name of data branch
	   **/
  CbmMCDataObject(const char* branchName);


  /** Copy constructor. Should not be called. **/
  CbmMCDataObject(const CbmMCDataObject&);


  /** Done. Clear all arrays and delete file chain. **/
  void Done();


  /** Finish event. Delete all arrays to free memory. **/
  void FinishEvent();

  /** Make TChain number chainNum2 friend of TChain number chainNum2
		 ** @param chainNum1 a number of chain for which AddFriend is called
		 ** @param chainNum2 number of chain which became a friend
		 **/
  void AddFriend(Int_t chainNum1, Int_t chainNum2);


  /** Get an object in legacy mode
		 ** @param fileNumber   Input number
		 ** @param eventNumber  Event number
		 ** @value              Pointer to referenced object
		 **/
  TObject* LegacyGet(Int_t fileNumber, Int_t eventNumber);


  //----   Private data members -------
  Int_t fLegacy;                 //! If true, run in legacy mode
  TObject* fLegacyObject;        //! Pointer to TObject for legacy mode
  TString fBranchName;           //! Name of the data branch
  Int_t fSize;                   //! Number of input file lists (one per source)
  std::vector<TChain*> fChains;  //! Arrays of chains (one per input source)
  std::vector<TObject*> fTArr;   //! Data objects from chains (one per input source)
  std::vector<Long64_t> fN;      //! Number of entries in chains

  /** Cached data objects. The vector index is the input source number, the map
		 ** index is the event number.
		 **/
  std::vector<std::map<Int_t, TObject*>> fArrays;  //!

  CbmMCDataObject& operator=(const CbmMCDataObject&);

  ClassDef(CbmMCDataObject, 1);
};

#endif
