/* Copyright (C) 2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexander Zinchenko [committer] */

/** CbmMuchToTrdVectors.h
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2018
 **
 ** Task class for MUCH-to-TRD vector merging in CBM.
 ** Input: 
 ** Output:
 **
 **/

#ifndef CBMMUCHTOTRDVECTORS_H_
#define CBMMUCHTOTRDVECTORS_H_ 1

#include "FairTask.h"
//#include "FairTrackParam.h"

#include <map>

class CbmMuchTrack;
class FairTrackParam;
class TClonesArray;
//class CbmTrdDigiPar;

class CbmMuchToTrdVectors : public FairTask {
public:
  /** Default constructor **/
  CbmMuchToTrdVectors();

  /** Destructor **/
  virtual ~CbmMuchToTrdVectors();

  /** Initialisation **/
  virtual InitStatus Init();

  /** Task execution **/
  virtual void Exec(Option_t* opt);

  /** Finish at the end **/
  virtual void Finish();

  /** SetParContainers **/
  virtual void SetParContainers();

  /** Accessors **/
  Int_t GetNofTracks() { return fNofTracks; };

private:
  // Some constants

private:
  //CbmTrdDigiPar *fDigiPar;   // digi params
  TClonesArray* fTrackArray;             // Output array of CbmMuchVectors
  Int_t fNofTracks;                      // Number of tracks created
  TClonesArray* fMuchTracks;             // Input array of CbmMuchTrack
  TClonesArray* fTrdTracks;              // Input array of CbmMuchTrack
  TClonesArray* fDigiMatches;            // Input array of
  Double_t fZ0;                          // Z-position of the TRD first layer
  std::multimap<Double_t, Int_t> fXmap;  // track indices vs X-coord.

  void GetMuchVectors();  // get MUCH vectors
  void MergeVectors();    // merge vectors
  void AddTrack(CbmMuchTrack* tr1, CbmMuchTrack* tr2, Int_t indx1, Int_t indx2, FairTrackParam& parOk,
                Double_t c2);  // add track
  void RemoveClones();         // remove clones

  CbmMuchToTrdVectors(const CbmMuchToTrdVectors&);
  CbmMuchToTrdVectors& operator=(const CbmMuchToTrdVectors&);

  ClassDef(CbmMuchToTrdVectors, 0)
};

#endif
