/** @file CbmTrdSetTracksPidWkn.h
  * @copyright Copyright (C) 2007-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer], Philipp Kähler **/

// -------------------------------------------------------------------------
// -----                    CbmTrdSetTracksPidWkn header file          -----
// -----                  Created 13/02/2007  by F. Uhlig              -----
// -------------------------------------------------------------------------


/** CbmTrdSetTracksPidWkn
 **
 *@author F.Uhlig <F.Uhlig@gsi.de>
 **
 ** Task class for PID calculation in the TRD using the
 ** Wkn method (see V. Ivanovs presentation) at CBM
 ** Collaboration meeting in March 2007
 ** http://)
 ** Input: TClonesArray of CbmTrdTrack
 ** Parameters: The parameters of these method are fixed 
 ** using the energy loss spectra of pions. 
 ** The parameters set in the constructor
 ** are the default values for the SVN with the new corrected
 ** enrgy loss in the TRD. These parameters should only be
 ** changed when one knows how to extract them correctly from
 ** the pion enrgy loss spectra.
 **
 **/


#ifndef CBMTRDSETTRACKSPIDWKN
#define CBMTRDSETTRACKSPIDWKN 1


#include "FairTask.h"


class TClonesArray;

class CbmTrdSetTracksPidWkn : public FairTask {

public:
  /** Default constructor **/
  CbmTrdSetTracksPidWkn();


  /** Standard constructor 
   **
   *@param name   Name of class
   *@param title  Task title
   **/
  CbmTrdSetTracksPidWkn(const char* name, const char* title = "FairTask");


  /** Destructor **/
  virtual ~CbmTrdSetTracksPidWkn();


  /** Initialisation at beginning of each event **/
  virtual InitStatus Init();


  /** Task execution **/
  virtual void Exec(Option_t* opt);


  /** Finish at the end of each event **/
  virtual void Finish();


  void SetType(const std::string& SISType) { fSISType = SISType; }
  void SetSIS100() { fSISType = "sis100"; }
  void SetSIS300() { fSISType = "sis300"; }

  void SetParameters();
  /** SetParContainers **/
  virtual void SetParContainers();

private:
  // Parametrs
  int fnSet,  //  number of the layers with TR
    fdegWkn,  //  statistics degree
    fNofTracks;
  float fk1, fwkn0,
    fEmp,  // energy loss transformation parameter
    fXi,   // energy loss transformation parameter
    fWmin, fWmax, fDiff;


  // Settings
  // Set-up
  // "sis100" - TRD with 10 layers
  // "sis300" - TRD with 4 layers
  std::string fSISType;

  TClonesArray* fTrackArray;   // Input array of TRD tracks
  TClonesArray* fTrdHitArray;  // Input array of TRD Hits

  CbmTrdSetTracksPidWkn(const CbmTrdSetTracksPidWkn&);
  CbmTrdSetTracksPidWkn& operator=(const CbmTrdSetTracksPidWkn&);

  ClassDef(CbmTrdSetTracksPidWkn, 1);
};

#endif
