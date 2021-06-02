/** @file CbmTransport.h
  * @copyright Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese, Florian Uhlig [committer] **/

/** @file CbmTransport.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 31.01.2019
 **/

#ifndef CBMTRANSPORT_H
#define CBMTRANSPORT_H 1


#include "CbmMCEventFilter.h"
#include "CbmSetup.h"
#include "CbmStackFilter.h"

#include "TNamed.h"
#include "TString.h"

#include <functional>
#include <iostream>
#include <memory>

class TGeant3;
class TGeant4;
class TVirtualMC;
class FairField;
class FairGenerator;
class FairRunSim;
class CbmEventGenerator;
class CbmTarget;
class CbmGeant3Settings;
class CbmGeant4Settings;

enum ECbmGenerator
{
  kUnigen,
  kUrqmd,
  kPluto
};

enum ECbmEngine
{
  kGeant3,
  kGeant4
};


/** @class CbmTransport
 ** @brief User interface class for transport simulation
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 31 January 2019
 **/
class CbmTransport : public TNamed {

public:
  /** @brief Constructor **/
  CbmTransport();


  /** @brief Destructor  **/
  virtual ~CbmTransport();


  /** @brief Add an input by file name and generator type
     ** @param fileName   Name of input file
     ** @param generator  Type of generator input
     **/
  void AddInput(const char* fileName, ECbmGenerator generator = kUnigen);


  /** @brief Add an input by generator instance
     ** @param generator Pointer to generator instance
     **/
  void AddInput(FairGenerator* generator);


  /** @brief Set the parameters for the TVirtualMC **/
  void ConfigureVMC();


  /** @brief Force the event vertex to be at a given z position
   ** @param zVertex  z position of event vertex
   **
   ** The event vertex will be determined by the beam intersection with
   ** the specified z plane. The beam properties (position in focal plane
   ** and direction) are sampled from the specified beam profile.
   **/
  void ForceVertexAtZ(Double_t zVertex);


  /** @brief Enable or disable forcing the vertex to be in the target
     ** @param choice If true, the vertex will be generated in the target
     **/
  void ForceVertexInTarget(Bool_t choice = kTRUE);


  /** @brief Trigger generation of a run info file
     ** @param choice  If kTRUE, run info file will be generated.
     **
     ** The run info file contains information on resource usage.
     **/
  void GenerateRunInfo(Bool_t choice = kTRUE) { fGenerateRunInfo = choice; }


  /** @brief Detector setup interface
     ** @return Pointer to CbmSetup class
     **/
  CbmSetup* GetSetup() const { return fSetup; }


  /** @brief Access to stack filter object
     ** @return Pointer to stack filter object
     **/
  std::unique_ptr<CbmStackFilter>& GetStackFilter() { return fStackFilter; }


  /** @brief Set the source the setup will be loaded from
     ** @param setupSource  enum value \ref ECbmSetupSource
     **/
  void SetSetupSource(ECbmSetupSource setupSource);


  /** @brief Use a standard setup
     ** @param setupName  Name of standard setup
     **/
  void LoadSetup(const char* setupName);


  /** @brief Enable registration of radiation length
     ** @param choice If kTRUE, registration is enabled.
     **
     ** If this method is used, an array of FairRadLenPoint will be created
     ** in the output tree. This is needed to analyse the material budget
     ** of the setup. It should not be used for standard transport,
     ** since it creates a lot of additional output.
     **/
  void RegisterRadLength(Bool_t choice = kTRUE);


  /** @brief Execute transport run
     ** @param nEvents  Number of events to process
     **/
  void Run(Int_t nEvents);


  /** @brief Set the beam angle (emittency at the beam position)
     ** @param x0  Beam mean angle in the x-z plane [rad]
     ** @param y0  Beam mean angle in the y-z plane [rad]
     ** @param sigmaX  Beam angle width in x-z [rad]
     ** @param sigmaY  Beam angle width in y-z [rad]
     **
     ** For each event, beam angles in x and y will be generated
     ** from Gaussian distributions with the specified parameters.
     ** The event will be rotated accordingly from the beam C.S.
     ** into the global C.S.
     **
     ** Without using this method, the default beam is always in z direction.
     **/
  void SetBeamAngle(Double_t x0, Double_t y0, Double_t sigmaX = -1., Double_t sigmaY = -1.);


  /** @brief Set the beam position
     ** @param x0  Beam centre position in x [cm]
     ** @param y0  Beam centre position in y [cm]
     ** @param sigmaX  Beam width in x [cm]. Default is 0.1.
     ** @param sigmaY  Beam width in y [cm]. Default is 0.1.
     ** @param zF  z coordinate of beam focal plane [cm|. Default is 0.
     **
     ** The beam parameters are used to generate the event vertex.
     ** A Gaussian beam profile in x and y is assumed.
     ** If sigmaX or sigmaY are null, the event vertex is
     ** always at (x0, y0).
     ** Smearing of the event vertex in the transverse plane can be
     ** deactivated by the method SetBeamSmearXY.
     ** Without using this method, the primary vertex is always
     ** at x=0 and y=0.
     **/
  void SetBeamPosition(Double_t x0, Double_t y0, Double_t sigmaX = -1., Double_t sigmaY = -1., Double_t zF = 0.);


  /** @brief Set a decay mode for a particle
     ** @param pdg          PDG code of particle to decay
     ** @param nDaughters   Number of daughters
     ** @param daughterPdg  Array of daughter PDG codes
     **
     ** This method will force the specified particle to always decay
     ** in the specified mode (branching ratio 100%).
     **/
  void SetDecayMode(Int_t pdg, UInt_t nDaughters, Int_t* daughterPdg);


  /** @brief Set transport engine
     ** @param engine  kGeant3 or kGeant4
     **
     ** By default, GEANT3 is used.
     **/
  void SetEngine(ECbmEngine engine) { fEngine = engine; }


  /** @brief Set a user-defined event filter class
     ** @param filter Pointer to event filter class
     **
     ** The filter class has to be derived from CbmMCEventFilter.
     ** It will be used instead of the default CbmMCEventFilter class
     ** for filtering MC events before writing to the output.
     **/
  void SetEventFilter(std::unique_ptr<CbmMCEventFilter>& filter)
  {
    fEventFilter.reset();
    fEventFilter = std::move(filter);
  }


  void SetEventFilterMinNofData(ECbmDataType type, Int_t value) { fEventFilter->SetMinNofData(type, value); }


  /** @brief Set magnetic field
     ** @param field  Pointer to FairField instance
     **
     ** By default, a field map corresponding to the magnet geometry
     ** will be used. This can be overridden by specifying a field
     ** object directly through this method.
     **/
  void SetField(FairField* field) { fField = field; }


  /** @brief Define geometry file name (output)
     ** @param name  Name for geometry file
     **
     ** If a file name is specified, a ROOT file containing the used
     ** geometry will be created (containing the TGeoManager).
     **/
  void SetGeoFileName(TString name);


  /** @brief Set the media file name
     ** @param fileName  Path to media file
     **
     ** By default, the media needed for building the geometry are
     ** read from media.geo. This method allows to use a different
     ** one for experimental purposes. If an absolute path is specified,
     ** it will be applied. A relative path will be appended to
     ** GEOMPATH if defined, else to VMCWRKDIR/geometry.
     **/
  void SetMediaFileName(TString fileName) { fSetup->SetMediaFilePath(fileName.Data()); }


  /** @brief Define parameter file name
     ** @param name  Name for parameter file
     **
     ** If the parameter file does not exist, it will be created.
     **/
  void SetParFileName(TString name);


  /** @brief Set a user-defined stack filter class
     ** @param filter Pointer to CbmStackFilter class
     **
     ** The filter class has to be derived from CbmStackFilter.
     ** It will be used instead of the default CbmStackFilter class
     ** for filtering MCTracks before writing to the output.
     **/
  void SetStackFilter(std::unique_ptr<CbmStackFilter>& filter)
  {
    fStackFilter.reset();
    fStackFilter = std::move(filter);
  }


  /** @brief Define the target
     ** @param medium     Name of target medium
     ** @param thickness  Thickness of target (in z) [cm]
     ** @param diameter   Target diameter [cm]
     ** @param x          x position of target centre in global c.s. [cm]
     ** @param y          y position of target centre in global c.s. [cm]
     ** @param z          z position of target centre in global c.s. [cm]
     ** @param rot        Rotation angle around y axis in global c.s. [deg]
     **
     ** The target is a disk with the z axis as symmetry axis.
     ** By default, it is centred at the origin. It can be rotated around
     ** the y axis.
     **/
  void SetTarget(const char* medium, Double_t thickness, Double_t diameter, Double_t x = 0., Double_t y = 0.,
                 Double_t z = 0., Double_t rot = 0.);


  /** @brief Define output file name
     ** @param name  Name for output file
     **/
  void SetOutFileName(TString name);


  /** @brief Activate random event plane
     ** @param phiMin  Minimum event plane angle [rad]
     ** @param phiMax  Maximum event plane angle [rad]
     **
     ** If this method is used, the input events will be rotated
     ** by a random angle around the z axis, with a flat distribution
     ** from phiMin to phiMax. In case several generators are used
     ** at the same time, this rotation applies to all inputs.
     **/
  void SetRandomEventPlane(Double_t phiMin = 0., Double_t phiMax = 2. * TMath::Pi());


  /** @brief Set global random seed value
     ** @param seedValue
     **
     ** This function allows to set the global seed value used
     ** by ROOTs random number generator TRandom
     **/
  void SetRandomSeed(const ULong_t seedValue) { fRandomSeed = seedValue; }

  /** @brief Enable smearing of event vertex in x and y.
     ** @param choice  If kTRUE(default), smearing is enabled.
     **
     ** If enabled, the event vertex will be sampled in x and y
     ** from a Gaussian distribution with the parameters specified
     ** by the method SetBeamPosition.
     **
     ** TODO: It is not guaranteed that the generated event vertex
     ** falls into the target volume. In order to ensure that,
     ** the method FairPrimaryGenerator::MakeVertex would have
     ** to be modified.
     */
  void SetVertexSmearXY(Bool_t choice = kTRUE);


  /** @brief Enable smearing of event vertex in z.
     ** @param choice  If kTRUE(default), smearing is enabled.
     **
     ** If enabled, the event vertex z coordinate will be sampled
     ** from a flat distribution inside the target.
     ** If no target is defined, the event z vertex will always be zero.
     */
  void SetVertexSmearZ(Bool_t choice = kTRUE);


  /** @brief Enable storing of trajectories
     ** @param choice  If kTRUE, trajectories will be stored.
     **
     ** When enabled, information on the trajectories of the simulated
     ** particles in stored in the output file for later visualisation.
     ** This is disabled by default because it increases the size of the
     ** output file by factors.
     */
  void StoreTrajectories(Bool_t choice = kTRUE) { fStoreTrajectories = choice; }

  /** @brief Set user defined transport settings for Geant3
     ** @param val Pointer to the user object
     **
     ** By default, a CbmGeant3Settings object is created with the default values
     ** as defined in the class. This can be overridden by specifying a CbmGeant3Settings
     ** object directly through this method.
     **/
  void SetGeant3Settings(CbmGeant3Settings* val) { fGeant3Settings = val; }

  /** @brief Set user defined transport settings for Geant4
     ** @param val Pointer to the user object
     **
     ** By default, a CbmGeant3Settings object is created with the default values
     ** as defined in the class. This can be overridden by specifying a CbmGeant3Settings
     ** object directly through this method.
     **/
  void SetGeant4Settings(CbmGeant4Settings* val) { fGeant4Settings = val; }

private:
  CbmSetup* fSetup;
  FairField* fField;                   //!
  std::shared_ptr<CbmTarget> fTarget;  //!
  CbmEventGenerator* fEventGen;
  std::unique_ptr<CbmMCEventFilter> fEventFilter;
  FairRunSim* fRun;
  TString fOutFileName;
  TString fParFileName;
  TString fGeoFileName;
  std::vector<FairGenerator*> fGenerators;
  Double_t fRealTimeInit;
  Double_t fRealTimeRun;
  Double_t fCpuTime;
  ECbmEngine fEngine;
  std::unique_ptr<CbmStackFilter> fStackFilter;
  Bool_t fGenerateRunInfo;
  Bool_t fStoreTrajectories;
  std::function<void()> fSimSetup;
  std::map<Int_t, std::vector<Int_t>> fDecayModes;

  CbmGeant3Settings* fGeant3Settings {nullptr};  //!
  CbmGeant4Settings* fGeant4Settings {nullptr};  //!

  ULong_t fRandomSeed {0};

  /** @brief Event generator initialisation **/
  void InitEventGenerator();


  /** @brief Force user-defined single-mode decays **/
  void ForceUserDecays();


  /** @brief Correct decay modes for pi0 and eta **/
  void PiAndEtaDecay(TVirtualMC* vmc);

  /** @brief Register ions
     **
     ** Since the TDatabasePDG does not contain ions we use in the
     ** transport, they are added manually by this method.
     **/
  void RegisterIons();


  /** @brief Create and register the setup modules **/
  void RegisterSetup();

  ClassDef(CbmTransport, 4);
};

#endif /* CBMTRANSPORT_H */
