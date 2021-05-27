/** @file CbmBeamGenerator.h
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 8 September 2020
 **/


#ifndef CBMBEAMGENERATOR_H
#define CBMBEAMGENERATOR_H 1


#include <FairGenerator.h>
#include <FairIon.h>

#include <Rtypes.h>

#include <string>
#include <vector>

class FairPrimaryGenerator;


/** @class CbmBeamGenerator.h
 ** @brief Generates beam ions for transport simulation
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 8 September 2020
 **
 ** The BeamGenerator is intended for the simulation of (non-interacting)
 ** beam particles through the setup, to study the beam-related background
 ** e.g. from delta electrons created by the beam passing the target or
 ** other materials in the setup. One beam particle (ion) per event is created.
 ** The user has to specify the ion species, the momentum or kinetic energy,
 ** and the starting point of the beam. The beam properties (profile and angular
 ** distribution) have to be specified to the CbmEventGenerator.
 **
 ** It is not recommended to use the BeamGenerator together with other
 ** generators in the same transport run, because it will force the event
 ** vertex to be at the specified z position and will deactivate vertex smearing
 ** in z as well as event plane rotation.
 **/
class CbmBeamGenerator : public FairGenerator {

public:
  /** @brief Default constructor (should not be used) **/
  CbmBeamGenerator();


  /** @brief Standard constructor
   ** @param beamZ     Atomic number (number of protons)
   ** @param beamA     Atomic mass number (number of nucleons)
   ** @param beamQ     Electric charge
   ** @param momentum  Momentum per nucleon [GeV]
   ** @param zStart    z coordinate of beam start position
   **/
  CbmBeamGenerator(UInt_t beamZ, UInt_t beamA, UInt_t beamQ, Double_t momentum, Double_t zStart);


  /** @brief Destructor **/
  virtual ~CbmBeamGenerator();


  /** @brief Print info to logger **/
  virtual void Print(Option_t* opt = "") const;


  /** @brief Generate one event (abstract in base class)
   ** @param primGen  Pointer to the FairPrimaryGenerator
   **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);


  /** @brief Info to string **/
  std::string ToString() const;


private:
  Double_t fP;       ///< Total momentum [GeV]
  Double_t fStartZ;  ///< z coordinate of start point
  FairIon* fIon;     ///< Ion type


  ClassDef(CbmBeamGenerator, 1);
};

#endif
