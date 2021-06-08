/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Denis Bertini [committer], Volker Friese */

// -------------------------------------------------------------------------
// -----                 CbmFindPrimaryVertex header file              -----
// -----                  Created 28/11/05  by V. Friese               -----
// -------------------------------------------------------------------------


/** CbmFindPrimaryVertex
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Task class for PV finding.
 ** Input: TClonesArray of CbmStsTracks (later CbmGlobalTracks)
 ** Output: CbmVertex
 **
 ** Uses as vertex finding algorithm classes derived from 
 ** CbmPrimaryVertexFinder.
 **/


#ifndef CBMFINDPRIMARYVERTEX_H
#define CBMFINDPRIMARYVERTEX_H 1


#include "CbmVertex.h"

#include "FairTask.h"

#include "TStopwatch.h"

class TClonesArray;
class CbmPrimaryVertexFinder;


class CbmFindPrimaryVertex : public FairTask {

public:
  /** Default constructor **/
  CbmFindPrimaryVertex();


  /** Standard constructor
   *@param pvFinder  Pointer to concrete vertex finder
   **/
  CbmFindPrimaryVertex(CbmPrimaryVertexFinder* pvFinder);


  /** Constructor with name and title
   **
   *@param name      Name of task
   *@param title     Title of task
   *@param pvFinder  Pointer to vertex finder concrete object
   **/
  CbmFindPrimaryVertex(const char* name, const char* title, CbmPrimaryVertexFinder* pvFinder);


  /** Destructor **/
  virtual ~CbmFindPrimaryVertex();


  /** Initialisation **/
  virtual InitStatus Init();


  /** Task execution **/
  virtual void Exec(Option_t* opt);


  /** Finish **/
  virtual void Finish();


private:
  TStopwatch fTimer;
  CbmPrimaryVertexFinder* fFinder;
  TClonesArray* fEvents = nullptr;
  TClonesArray* fTracks;
  CbmVertex* fPrimVert;

  Int_t fNofTs            = 0;   ///< Number of processed timeslices
  Int_t fNofEvents        = 0;   ///< Number of processed events
  Double_t fNofTracks     = 0;   ///< Number of input tracks
  Double_t fNofTracksUsed = 0.;  ///< Number of tracks used for vertex finding
  Double_t fTimeTot       = 0.;  ///< Total execution time [s]

  CbmFindPrimaryVertex(const CbmFindPrimaryVertex&);
  CbmFindPrimaryVertex& operator=(const CbmFindPrimaryVertex&);

  ClassDef(CbmFindPrimaryVertex, 1);
};

#endif
