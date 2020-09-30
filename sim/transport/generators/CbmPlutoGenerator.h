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

#include "Rtypes.h"  // for Char_t, etc

#include <string>
#include <vector>

class FairPrimaryGenerator;

class TClonesArray;
class TChain;

class PStaticData;
class PDataBase;

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


private:
  PStaticData* fdata;  //! pluto static data
  PDataBase* fbase;    //! pluto data base

  Int_t iEvent;              //! Event number
  const Char_t* fFileName;   //! Input file name
  TChain* fInputChain;       //! Pointer to input file
  TClonesArray* fParticles;  //! Particle array from PLUTO
  Int_t fPDGmanual;          //! forced pdg value for undefined pluto codes

  /** Private method CloseInput. Just for convenience. Closes the
     ** input file properly. Called from destructor and from ReadEvent. **/
  void CloseInput();

  /** Check if file exists. Break fatal when it doesn't exist **/
  Bool_t CheckFileExist(std::string filename);

  ClassDef(CbmPlutoGenerator, 4);
};

#endif
