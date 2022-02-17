/* Copyright (C) 2004-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                 CbmPlutoGenerator header file                 -----
// -----          Created 13/07/04  by V. Friese / D.Bertini           -----
// -------------------------------------------------------------------------

/** CbmPlutoGenerator.h
 *@author V.Friese <v.friese@gsi.de>
 *@author D.Bertini <d.bertini@gsi.de>
 *
 The CbmPlutoGenerator reads the PLUTO output file (ROOT format)
 and inserts the tracks into the CbmStack via the CbmPrimaryGenerator.
 Derived from CbmGenerator.
**/


#ifndef FAIR_PLUTOGENERATOR_H
#define FAIR_PLUTOGENERATOR_H

#include "FairGenerator.h"  // for FairGenerator

#include "Rtypes.h"        // for Char_t, etc
#include "TClonesArray.h"  // for TClonesArray

#include <string>
#include <vector>

#include "PParticle.h"    // for PParticle
#include "PStaticData.h"  // for PStaticData

class FairPrimaryGenerator;

class TChain;

class CbmPlutoGenerator : public FairGenerator {

public:
  /** Default constructor (should not be used) **/
  CbmPlutoGenerator();


  /** Standard constructor
     ** @param fileName The input (PLUTO) file name
     **/
  CbmPlutoGenerator(const Char_t* fileName);

  /** Constructor with list of input files
     ** @param fileNames A list of (PLUTO) input file names
     **/
  CbmPlutoGenerator(std::vector<std::string> fileNames);

  CbmPlutoGenerator(const CbmPlutoGenerator&) = delete;
  CbmPlutoGenerator& operator=(const CbmPlutoGenerator&) = delete;

  /** Destructor **/
  virtual ~CbmPlutoGenerator();


  /** Reads on event from the input file and pushes the tracks onto
     ** the stack. Abstract method in base class.
     ** @param primGen  pointer to the FairPrimaryGenerator
     **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
  void SetManualPDG(Int_t pdg) { fPDGmanual = pdg; }

  /** @brief Get the maximum number of events available in the input file
    ** @return number of available ebvents
    */
  Int_t GetNumAvailableEvents() { return fAvailableEvents; }

private:
  PStaticData* fdata {makeStaticData()};  //! pluto static data
  PDataBase* fbase {makeDataBase()};      //! pluto data base

  Int_t iEvent {0};                                               //! Event number
  const Char_t* fFileName {""};                                   //! Input file name
  TChain* fInputChain {nullptr};                                  //! Pointer to input file
  TClonesArray* fParticles {new TClonesArray("PParticle", 100)};  //! Particle array from PLUTO
  Int_t fPDGmanual {0};                                           //! forced pdg value for undefined pluto codes
  Int_t fAvailableEvents {0};                                     //! Maximum number of events in the input file

  /** Private method CloseInput. Just for convenience. Closes the
     ** input file properly. Called from destructor and from ReadEvent. **/
  void CloseInput();

  ClassDef(CbmPlutoGenerator, 5);
};

#endif
