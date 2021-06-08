/* Copyright (C) 2015-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: //Dr.Sys, Volker Friese [committer], Florian Uhlig */

/** @file  CbmMCDataArray.h
 ** @author //Dr.Sys <mikhail.prokudin@cern.ch>
 **/

#ifndef CBMMCDATAARRAY_H
#define CBMMCDATAARRAY_H

#include "CbmLink.h"  // for CbmLink

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Long64_t
#include <TString.h>     // for TString

#include <list>    // for list
#include <map>     // for map
#include <vector>  // for vector

class TChain;
class TClonesArray;
class TObject;

/** @class CbmMCDataArray
 ** @brief Access to a MC data branch for time-based analysis
 ** @author //Dr.Sys <mikhail.prokudin@cern.ch>
 ** @date 31 March 2015
 **
 ** This class provides cached access to data for an event-by-event MC data branch,
 ** which is used when matching time-based reconstruction data to the input MC data.
 ** Data arrays from already used events are cached, which allows switching between
 ** events without the performance deterioration induced by frequent calls to
 ** TTree::GetEntry().
 ** In legacy mode, the data from the current event will be retrieved from
 ** FairRootManager.
 **/
class CbmMCDataArray {
  friend class CbmMCDataManager;

public:
  /** Destructor **/
  virtual ~CbmMCDataArray() {}


  /** Get an object by CbmLink pointer
		 ** @param link Pointer to CbmLink object
		 ** @value      Pointer to linked object
		 **/
  TObject* Get(const CbmLink* lnk) { return Get(lnk->GetFile(), lnk->GetEntry(), lnk->GetIndex()); }


  /** Get an object by CbmLink reference
		 ** @param link Reference to CbmLink object
		 ** @value      Pointer to linked object
		 **/
  TObject* Get(const CbmLink& lnk) { return Get(lnk.GetFile(), lnk.GetEntry(), lnk.GetIndex()); }


  /** Get an object by file number, event number and index
		 ** @param fileNumber	  Input file number
		 ** @param eventNumber  Event number
		 ** @param index        Index of desired object in its array
		 ** @value              Pointer to referenced object
		 **/
  TObject* Get(Int_t fileNumber, Int_t eventNumber, Int_t index);

  /** Get a size of TClonesArray . Slow if TClonesArray not in cache
		 ** @param fileNumber	Input file number
		 ** @param eventNumber  Event number
		 ** @value              Size of corresponding TClonesArray
		 **/
  Int_t Size(Int_t fileNumber, Int_t eventNumber);

  /** Get a size of TClonesArray . Slow if TClonesArray not in cache
		 ** @param link	Reference to CbmLink object, index field is ignored
		 ** @value 	Size of corresponding TClonesArray
		 **/
  Int_t Size(const CbmLink& lnk) { return Size(lnk.GetFile(), lnk.GetEntry()); }

  /** Get a size of TClonesArray . Slow if TClonesArray not in cache
		 ** @param link	Pointer to CbmLink object, index field is ignored
		 ** @value 	Size of corresponding TClonesArray
		 **/
  Int_t Size(const CbmLink* lnk) { return Size(lnk->GetFile(), lnk->GetEntry()); }

private:
  /** Default constructor. Should be called from CbmMCDataManager. **/
  CbmMCDataArray() : fLegacy(0), fLegacyArray(nullptr), fBranchName(), fSize(0), fChains(), fTArr(), fN(0), fArrays() {}

  /** Standard constructor. Should be called from CbmMCDataManager.
		 ** @param branchName   Name of data branch
		 ** @param fileList     Vector of file list (one for each input source)
		 **/
  CbmMCDataArray(const char* branchName, const std::vector<std::list<TString>>& fileList);

  /** Constructor in legacy mode. Gets the branch from FairRootManager.
	   ** @param branchName  Name of data branch
	   **/
  CbmMCDataArray(const char* branchName);


  /** Copy constructor. Should not be called. **/
  CbmMCDataArray(const CbmMCDataArray&);


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
		 ** @param index        Index of desired object in its array
		 ** @value              Pointer to referenced object
		 **/
  TObject* LegacyGet(Int_t fileNumber, Int_t eventNumber, Int_t index);


  //----   Private data members -------
  Int_t fLegacy;                     //! If true, run in legacy mode
  TClonesArray* fLegacyArray;        //! Pointer to TClonesArray for legacy mode
  TString fBranchName;               //! Name of the data branch
  Int_t fSize;                       //! Number of input file lists (one per source)
  std::vector<TChain*> fChains;      //! Arrays of chains (one per input source)
  std::vector<TClonesArray*> fTArr;  //! Data arrays from chains (one per input source)
  std::vector<Long64_t> fN;          //! Number of entries in chains

  /** Cached data arrays. The vector index is the input source number, the map
		 ** index is the event number.
		 **/
  std::vector<std::map<Int_t, TClonesArray*>> fArrays;  //!

  CbmMCDataArray& operator=(const CbmMCDataArray&);

  ClassDef(CbmMCDataArray, 1);
};

#endif
