/** @file CbmUnigenGenerator.h
  * @copyright Copyright (C) 2006-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors D. Kresan, Volker Friese **/

/** @file CbmUnigenGenerator.h
 ** @author D. Kresan <d.kresan@gsi.de>
 ** @since 4 July 2006
 **/

#ifndef CBMUNIGENGENERATOR
#define CBMUNIGENGENERATOR


#include "FairGenerator.h"

#include "TMath.h"
#include "TString.h"
#include "TVector3.h"

#include <map>

class TFile;
class TTree;
class UEvent;
class UParticle;
class FairIon;
class FairPrimaryGenerator;


/** @class CbmUnigenGenerator
 ** @brief Generates input to transport simulation from files in Unigen format
 ** @author D. Kresan <d.kresan@gsi.de>
 ** @since 4 July 2006
 ** @date 4 September 2019 (revision by V. Friese <v.friese@gsi.de>)
 **
 ** This class reads ROOT files containing Unigen data (UEvent, UParticle)
 ** and inputs them to the transport simulation.
 **
 ** A Lorentz transformation to the lab (target) system is applied.
 ** Depending on the chosen mode, the event plane angle is rotated by a
 ** fixed value (kRotateFixed) or by a random value (kReuseEvents).
 ** In the mode kRotateToZero, the event plane will be rotated
 ** by the negative angle found in UEvent, such that the resulting event plane
 ** angle is zero (impact parameter in x direction).
 **
 ** In the mode kReuseEvents, the input events are be used multiple times
 ** if more events are requested than present in the input tree. This implies
 ** random event plane rotation.
 **/
class CbmUnigenGenerator : public FairGenerator {

public:
  /** @brief Mode enumerator **/
  enum EMode
  {
    kStandard,     ///< Rotate events to zero event plane (default)
    kNoRotation,   ///< No event rotation
    kRotateFixed,  ///< Rotate events around z by a fixed angle
    kReuseEvents   ///< Reuse events if more are requested than present
  };


  /** @brief Default constructor
     ** @param fileName  Name of input file in UniGen format
     ** @param mode  Execution mode (see EMode)
     **
     ** This constructor cannot be used for mode kRotateFixed.
     **/
  CbmUnigenGenerator(const char* fileName = "", EMode mode = kStandard);


  /** @brief Constructor with fixed rotation angle
     ** @param fileName  Name of input file in UniGen format
     ** @param mode  Execution mode (see EMode)
     ** @param phi Event plane rotation angle (only for mode fRotateFixed)
     */
  CbmUnigenGenerator(const char* fileName, EMode mode, Double_t phi);


  /** @brief Destructor **/
  virtual ~CbmUnigenGenerator();


  /** @brief Read one event from the input file
     ** @param primGen  Pointer to FairPrimaryGenerator instance
     ** @return kTRUE if successful; else kFALSE
     **
     ** Framework interface to define one input event for the
     ** transport simulation. Is called from the FairPrimaryGenerator
     ** instance. It adds all particles in the input event to the stack using
     ** FairPrimaryGenerator::AddTrack after having applied the Lorentz
     ** transformation and (if required) event plane rotation.
     **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);


private:
  TString fFileName;                    ///< Input file name
  EMode fMode;                          ///< Rotation mode
  Double_t fPhi;                        ///< Event plane rotation angle
  Bool_t fIsInit;                       ///< Flag whether generator is initialised
  TFile* fFile;                         //!< Input ROOT file
  TTree* fTree;                         //!< Input ROOT tree
  Int_t fCurrentEntry;                  ///< Current entry number
  UEvent* fEvent;                       //!< Current input event
  Int_t fNofPrimaries;                  //!< Number of primaries registered in current event
  Int_t fNofEvents;                     ///< Number of processed events
  Double_t fBetaCM;                     ///< CM velocity in the lab frame
  Double_t fGammaCM;                    ///< Gamma factor of CM in lab frame
  std::map<TString, FairIon*> fIonMap;  //!< Map from ion name to FairIon

  // Constants for decimal decomposition of ion PDG.
  // For ions the PDG code is +-10LZZZAAAI, with L = number of Lambdas,
  // ZZZ = charge (number of protons), AAA = mass (sum of numbers
  // of Lambdas, protons and neutrons, I = isomer level
  static const Int_t kPdgLambda = 10000000;  ///< Decomposition of ion PDG code
  static const Int_t kPdgCharge = 10000;     ///< Decomposition of ion PDG code
  static const Int_t kPdgMass   = 10;        ///< Decomposition of ion PDG code


  /** @brief Add a primary particle to the event generator
     ** @param primGen  FairPrimaryGenerator instance
     ** @param pdgCode  Particle ID (PDG code)
     ** @param momentum Momentum vector [GeV]
     **/
  void AddPrimary(FairPrimaryGenerator* primGen, Int_t pdgCode, const TVector3& momentum);


  /** @brief Close the input file **/
  void CloseInput();


  /** @brief Charge number of an ion
     ** @param pdgCode Particle ID (PDG code)
     ** @return Charge number (numbers of protons)
     **
     ** For ions the PDG code is +-10LZZZAAAI, with ZZZ the number of protons
     **/
  Int_t GetIonCharge(Int_t pdgCode) const { return (pdgCode % kPdgLambda) / kPdgCharge; }


  /** @brief Number of Lambdas in an ion
     ** @param pdgCode Particle ID (PDG code)
     ** @return Number of Lambdas
     **
     ** For ions the PDG code is +-10LZZZAAAI, with L the number of Lambdas
     **/
  Int_t GetIonLambdas(Int_t pdgCode) const { return (pdgCode % (10 * kPdgLambda)) / kPdgLambda; }


  /** @brief Mass number of an ion
     ** @param pdgCode Particle ID (PDG code)
     ** @return Mass number (sum of numbers of protons, neutrons and Lambdas)
     **
     ** For ions the PDG code is +-10LZZZAAAI, with AAA the mass number
     **/
  Int_t GetIonMass(Int_t pdgCode) const { return (pdgCode % kPdgCharge) / kPdgMass; }


  /** @brief Get next entry from input tree
     ** @return true if valid entry is available; else false
     **/
  Bool_t GetNextEntry();


  /** @brief Initialisation
     ** @return kTRUE is initialised successfully
     **
     ** The input file is opened, run information is retrieved, and the
     ** input branch is connected.
     **/
  Bool_t Init();


  /** @brief Treat a composite particle (ion)
     ** @param primGen  FairPrimaryGenerator instance
     ** @param pdgCode  Particle ID (PDG code)
     ** @param momentum Momentum vector [GeV]
     **
     ** Composite particle need special treatment because hyper-nuclei
     **  ** are not supported by FairRoot, and neutral ions are not supported
     ** by GEANT4. Hyper-nuclei are thus replaced by their non-strange
     ** analogue, and neutral ions are decomposed into neutrons.
     **/
  void ProcessIon(FairPrimaryGenerator* primGen, Int_t pdgCode, const TVector3& momentum);


  /** @brief Register ions to the simulation
     ** @return Number of registered ions
     **
     ** The input may contain ions, which are not known to the
     ** simulation engine. They are added using the method FairRunSim::AddIon.
     ** This has to be done at initialisation. The method scans the entire
     ** input for ions and registers them before the start of the transport run.
     */
  Int_t RegisterIons();


  /** @brief Copy constructor forbidden **/
  CbmUnigenGenerator(const CbmUnigenGenerator&) = delete;


  /** @brief Assignment operator forbidden **/
  CbmUnigenGenerator& operator=(const CbmUnigenGenerator&) = delete;


  ClassDef(CbmUnigenGenerator, 4);
};

#endif
