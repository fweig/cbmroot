/** @file CbmPhsdGenerator.h
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors V.Vovchenko, Florian Uhlig [committer] **/

// -------------------------------------------------------------------------
// -----                CbmPHSDGenerator header file                  -----
// -----          Created 24/01/14, updated on 17/11/16  by V. Vovchenko           -----
// -------------------------------------------------------------------------


/** CbmPhsdGenerator.h
 *@author V.Vovchenko <V.Vovchenko@gsi.de>
 *
 The CbmPhsdGenerator reads from HSD the input file used to generate events, baryons output file fort.300
 and meson output file fort.301. The HSD
 calculation has to be performed in the CM system of the collision; Lorentz
 transformation into the lab is performed by this class.
 Derived from FairGenerator.
**/


#ifndef CBMPHSDGENERATOR_H
#define CBMPHSDGENERATOR_H


#include "FairGenerator.h"

#include <fstream>
#include <map>

class TVirtualMCStack;
class FairPrimaryGenerator;

// This structure holds information about hadron from HSD output file
struct Hadron {
  bool init;  // Are parameters initialised?
  int id;     // HSD particle id
  int charge;
  int ISUB;               // Number of subsequent parallel run
  int IRUN;               // Number of run in subsequent run
  int globalEvent;        // = (ISUB-1)*IRUNS + IRUN
  double px, py, pz, p0;  // 4-momentum
  double b;               // impact parameter of event
};


class CbmPhsdGenerator : public FairGenerator {

public:
  /** Default constructor without arguments should not be used. **/
  CbmPhsdGenerator();


  /** Standard constructor for reading .300 and .301 PHSD output files.
     * @param fileNameInput The file name of INPUT file used in HSD
     * @param fileNameBaryons The file name of baryons output (fort.300)
     * @param fileNameMesons The file name of mesons output (fort.301)
     **/
  CbmPhsdGenerator(const char* fileNameInput, const char* fileNameBaryons, const char* fileNameMesons);

  /** Standard constructor for reading .dat PHSD output file.
     * @param fileNameDat The file name of .dat PHSD output file
     **/
  CbmPhsdGenerator(const char* fileNameInput, const char* fileNameDat);


  /** Destructor. **/
  ~CbmPhsdGenerator();


  /** Reads on event from the input file and pushes the tracks onto
     ** the stack. Abstract method in base class.
     ** @param pStack    pointer to the stack
     ** @param ver       not used
     **/
  Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  /** Reads the event from .300 and .301 PHSD output files.
     **/
  Bool_t ReadEvent300(FairPrimaryGenerator* primGen);

  /** Reads the event from .dat PHSD output file.
     **/
  Bool_t ReadEventDat(FairPrimaryGenerator* primGen);

  /** Skip some events in file
     ** TODO Implement
     **/
  Bool_t SkipEvents(Int_t count);

private:
  Bool_t fReadDat;  //!  Whether phsd.dat or .300/301 files are used.

  FILE *fBaryonsFile, *fMesonsFile;  //!  HSD output files

  FILE* fDatFile;  //!  HSD output files

  std::map<Int_t, Int_t> fParticleTable;  //!  Map from HSD PID to PDGPID

  const Char_t* fFileName;  //!  Input file name

  // Necessary to have because of HSD output structure
  Hadron nextBaryon, nextMeson;  // Baryons and mesons read from next event
  int nextEvent;                 // Id of the next event


  // Data about HSD simulation, is read from HSD input file
  int Ap, Zp, At, Zt;
  int ISUBS;    // Number of subsequent parallel runs in HSD
  int IRUNS;    // Number of runs in one parallel batch in HSD
  double ekin;  // Kinetic energy per projectile nucleon in lab frame


  /** Private method ReadConversionTable. Reads the conversion table
        from HSD particle code to PDG particle code and fills the
        conversion map. The conversion is mostly based on subroutine TRANSPOSECODES from fritzi.F in HSD-2.5.
		Open charm is not included.
		Is called from the constructor. **/
  void ReadConversionTable();

  /** Private method ReadCollisionData. Reads the following information about colliding
	system: ebeam, Ap, Zp, At, Zt. Is called from the constructor. **/
  void ReadCollisionData(const char* fileNameInput);

  CbmPhsdGenerator(const CbmPhsdGenerator&);
  CbmPhsdGenerator& operator=(const CbmPhsdGenerator&);

  ClassDef(CbmPhsdGenerator, 1);
};

#endif
