/** @file CbmStsPhysics.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 02.12.2014
 ** @date 13.03.2019
 **/

#ifndef CBMSTSPHYSICS_H
#define CBMSTSPHYSICS_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t

#include <map>  // for map

/** @class CbmStsPhysics
 ** @brief Auxiliary class for physics processes in Silicon
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 02.12.2014
 **
 ** This singleton class is auxiliary for the detector response simulation
 ** of the CBM-STS, but can also be used from reconstruction ar analysis
 ** (e.g., Lorentz shift).
 **/
class CbmStsPhysics {

public:
  /** Destructor **/
  virtual ~CbmStsPhysics();


  /** Diffusion width as function of z
     ** @param z           Distance from p side [cm]
     ** @param d           Thickness of sensor [cm]
     ** @param vBias       Bias voltage [V]
     ** @param vFd         Full depletion voltage [V]
     ** @param temperature Temperature [K]
     ** @param chargeType  0 = electron, 1 = hole
     ** @return Drift time [s]
     **
     ** Calculates the diffusion width (sigma) for a charge drifting
     ** from z to the readout (z = 0 for hole, z = d for electrons).
     **
     ** For the reference to the formulae, see the STS digitiser note.
     **/
  static Double_t DiffusionWidth(Double_t z,
                                 Double_t d,
                                 Double_t vBias,
                                 Double_t vFd,
                                 Double_t temperature,
                                 Int_t chargeType);


  /** @brief Electric field magnitude in a silicon sensor as function of z
     ** @param vBias  Bias voltage [V]
     ** @param vFd    Full depletion voltage [V]
     ** @param dZ     Thickness of sensor [cm]
     ** @param z      z coordinate, measured from the p side [cm]
     ** @return       z component of electric field [V/cm]
     **/
  static Double_t
  ElectricField(Double_t vBias, Double_t vFd, Double_t dZ, Double_t z);


  /** @brief Energy loss in a Silicon layer
     ** @param dz    Layer thickness [cm]
     ** @param mass  Particle mass [GeV]
     ** @param eKin  Kinetic energy [GeV]
     ** @param dedx  Average specific energy loss [GeV/cm]
     ** @return Energy loss in the layer [GeV]
     **
     ** The energy loss is sampled from the Urban fluctuation model
     ** described in the GEANT3 manual (PHYS333 2.4, pp. 262-264).
     */
  Double_t
  EnergyLoss(Double_t dz, Double_t mass, Double_t eKin, Double_t dedx) const;


  /** @brief Accessor to singleton instance
     ** @return  Pointer to singleton instance
     **
     ** Will instantiate a singleton object if not yet existing.
     **/
  static CbmStsPhysics* Instance();


  /** @brief Half width at half max of Landau distribution
     ** in ultra-relativistic case
     ** @param mostProbableCharge [e]
     ** @return half width [e]
     **/
  Double_t LandauWidth(Double_t mostProbableCharge);


  /** @brief Energy for electron-hole pair creation in silicon
     ** @return Pair creation energy [GeV]
     **/
  static Double_t PairCreationEnergy() { return 3.57142e-9; }


  /** @brief Particle charge from PDG particle ID
     ** @param pid   PID (PDG code)
     ** @return Particle charge [e]
     **
     ** For particles in the TDataBasePDG, the charge is taken from there.
     ** For ions, it is calculated following the PDG code convention.
     ** If not found, zero is returned.
     **/
  static Double_t ParticleCharge(Int_t pid);


  /** @brief Particle mass from PDG particle ID
     ** @param pid   PID (PDG code)
     ** @return Particle mass [GeV]
     **
     ** For particles in the TDataBasePDG, the mass is taken from there.
     ** For ions, it is calculated following the PDG code convention.
     ** If not found, zero is returned.
     **/
  static Double_t ParticleMass(Int_t pid);


  /** @brief Stopping power (average specific energy loss) in Silicon
     ** @param eKin  Kinetic energy pf the particle [GeV]
     ** @param pid   Particle ID (PDG code)
     ** @return Stopping power [GeV/cm]
     **
     ** This function calculates the stopping power
     ** (average specific energy loss) in Silicon of a particle specified
     ** by its PDG code. For an unknown pid, null is returned.
     **/
  Double_t StoppingPower(Double_t eKin, Int_t pid);


  /** Stopping power in Silicon
     ** @param energy      Energy of particle [GeV]
     ** @param mass        Particle mass [GeV]
     ** @param charge      Electric charge [e]
     ** @param isElectron  kTRUE if electron, kFALSE else
     ** @return            Stopping power [GeV/cm]
     **
     ** This function calculates the stopping power
     ** (average specific energy loss) in Silicon of a particle
     ** with given mass and charge.
     **/
  Double_t StoppingPower(Double_t energy,
                         Double_t mass,
                         Double_t charge,
                         Bool_t isElectron);


private:
  /** @brief Constructor **/
  CbmStsPhysics();


  /** @brief Copy constructor (disabled) **/
  CbmStsPhysics(const CbmStsPhysics&) = delete;
  ;


  /** @brief Assignment operator (disabled) **/
  CbmStsPhysics operator=(const CbmStsPhysics&) = delete;


  /** @brief Interpolate a value from the data tables
     ** @param eKin   Equivalent kinetic energy [GeV]
     ** @param table  Reference to data map (fStoppingElectron or fStoppingProton)
     ** @return Interpolated value from data table
     **
     ** The eEquiv is below the tabulated range, the first table value is
     ** returned; if it is above the range, the last value is returned.
     **/
  Double_t InterpolateDataTable(Double_t eKin,
                                std::map<Double_t, Double_t>& table);


  /** @brief Read stopping power data table from file **/
  void ReadDataTablesStoppingPower();


  /** @brief Read Landau width data table from file **/
  void ReadDataTablesLandauWidth();


  /** @brief Calculate the parameters for the Urban model
     ** @param z  Atomic charge of material element
     **
     ** The parameters are set according to the GEANT3 manual (PHYS332 and PHYS333)
     **/
  void SetUrbanParameters(Double_t z);


private:
  static CbmStsPhysics* fgInstance;  ///< Singleton instance

  // --- Parameters for the Urban model
  Double_t fUrbanI = 0.;  ///< Urban model: mean ionisation potential of Silicon
  Double_t fUrbanE1 = 0.;  ///< Urban model: first atomic energy level
  Double_t fUrbanE2 = 0.;  ///< Urban model: second atomic energy level
  Double_t fUrbanF1 = 0.;  ///< Urban model: oscillator strength first level
  Double_t fUrbanF2 = 0.;  ///< Urban model: oscillator strength second level
  Double_t fUrbanEmax =
    0.;  ///< Urban model: cut-off energy (delta-e threshold)
  Double_t fUrbanR =
    0.;  ///< Urban model: weight parameter excitation/ionisation

  // --- Data tables for stopping power
  std::map<Double_t, Double_t>
    fStoppingElectron {};  ///< E [GeV] -> <-dE/dx> [GeV*g/cm^2]
  std::map<Double_t, Double_t>
    fStoppingProton {};  ///< E [GeV] -> <-dE/dx> [GeV*g/cm^2]

  // --- Data tables for width of Landau distribution
  std::map<Double_t, Double_t> fLandauWidth;  ///< q [e] -> width [e]


  ClassDef(CbmStsPhysics, 2);
};

#endif /* CBMSTSPHYSICS_H_ */
