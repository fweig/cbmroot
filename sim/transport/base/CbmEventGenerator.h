/** @file CbmEventGenerator.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 29 July 2019
 **/


#ifndef CBMEVENTGENERATOR_H
#define CBMEVENTGENERATOR_H 1


#include "CbmBeamProfile.h"
#include "CbmTarget.h"
#include <FairPrimaryGenerator.h>
#include <memory>

class FairGenericStack;


/** @brief CbmEventGenerator
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 29 July 2019
 ** @date 29 July 2019
 **
 ** The EventGenerator defines the primary particles as input
 ** to the transport simulation. Several event generator objects can be
 ** registered to the EventGenerator, each generating primary particles.
 ** The EventGenerator generates the common event vertex and rotation
 ** according to the user specification.
 **
 ** CbmEventGenerator derives from FairPrimaryGenerator. It re-implements
 ** the methods MakeBeam and MakeVertex such as to ensure that the generated
 ** vertex falls into the target volume.
 **/
class CbmEventGenerator : public FairPrimaryGenerator {

public:
  /** @brief Default constructor  **/
  CbmEventGenerator();


  /** @brief Destructor  **/
  virtual ~CbmEventGenerator();


  /** @brief Force event vertex to be at a given z
   ** @param zVertex  z coordinate of event vertex
   **
   ** The event vertex will be sampled in x and y from the specified
   ** beam properties (profile in focal plane and angular distribution),
   **/
  void ForceVertexAtZ(Double_t zVertex);


  /** @brief Enable or disable forcing the vertex to be in the target
		 ** @param choice If true, the vertex will be generated in the target
		 **/
  void ForceVertexInTarget(Bool_t choice = kTRUE) {
    fForceVertexInTarget = choice;
  }


  /** @brief Beam profile
		 ** @return Reference to beam profile object
		 **/
  const CbmBeamProfile& GetBeamProfile() { return fBeamProfile; }


  /** @brief Generate the input event
		 ** @param stack  Pointer to stack object
		 **
		 ** This is the main functionality, invoked from FairRunSim.
		 ** It fills the stack at the beginning of each event with
		 ** the primary particles provided by the generator instances.
		 ** All primary track momenta are rotated according to the
		 ** beam direction (in x-z and y-z) and and by the event plane (in x-y).
		 **
		 ** Re-implemented from base-class FairPrimaryGenerator.
		 **/
  virtual Bool_t GenerateEvent(FairGenericStack* stack);


  /** @brief Log output **/
  virtual void Print(Option_t* opt = "") const;


  /** @brief Set the beam angle in the focal plane
		 ** @param meanThetaX   Mean angle in x-z [rad]
		 ** @param meanThetaY   Mean angle in y-z [rad]
		 ** @param sigmaThetaX  RMS of beam angle in x-z [rad]
		 ** @param sigmaThetaY  RMS of beam angle in y-z [rad]
		 **
		 ** The beam angles will be sampled from Gaussian distributions
		 ** with the specified mean values and RMS. If the latter are negative,
		 ** the beam angles will be fixed to their mean values.
		 **
		 ** Default is (0., 0.), no sampling.
		 **
		 ** Note: Re-implements the non-virtual method in FairPrimaryGenerator.
		 */
  void SetBeamAngle(Double_t meanThetaX,
                    Double_t meanThetaY,
                    Double_t sigmaThetaX = -1.,
                    Double_t sigmaThetaY = -1.);


  /** @brief Set the beam position in the focal plane
		 ** @param meanX    Mean x position [cm]
		 ** @param meanY    Mean y position [cm|
		 ** @param sigmaX   RMS of x position [cm]
		 ** @param sigmaY   RMS of y position [cm]
		 ** @param zF       z position of focal plane [cm]
		 **
		 ** If the beam widths in x and/or y are positive, the beam
		 ** position will be sampled randomly for each event from a
		 ** Gauss distribution. Otherwise, it is fixed for all events.
		 **
		 ** Default is (0.,0.,0.) for the position, no sampling.
		 */
  void SetBeamPosition(Double_t meanX,
                       Double_t meanY,
                       Double_t sigmaX = -1.,
                       Double_t sigmaY = -1.,
                       Double_t zF     = 0.);


  /** @brief Set target properties
		 ** @param target  Pointer to CbmTarget instance
		 **
		 ** When a target is specified, the event vertex will be sampled
		 ** by a constant distribution along the (straight) trajectory
		 ** of the beam inside the target. If SmearVertexZ(kFALSE) is
		 ** set afterwards, the vertex will always be in the target
		 ** centre plane.
		 **/
  void SetTarget(std::shared_ptr<const CbmTarget> target) { fTarget = target; }


private:
  CbmBeamProfile fBeamProfile;               ///< Beam properties
  std::shared_ptr<const CbmTarget> fTarget;  //! Target properties
  Bool_t fForceVertexInTarget;  ///< If set, vertex must be in target
  Bool_t fForceVertexAtZ;       ///< If set, vertex must be at given z
  Double_t fVertexZ;            ///< forced z coordinate of event vertex


  /** @brief Generate beam angle
		 **
		 ** Will be called from FairPrimaryGenerator::GenerateEvent().
		 ** The method is re-implemented here as empty. The beam angle
		 ** will be generated along with the beam position in the method
		 ** MakeVertex().
		 **/
  virtual void MakeBeamAngle() {};


  /** @brief Generate event vertex position
		 **
		 ** Will be called from FairPrimaryGenerator::GenerateEvent().
		 ** Beam position and angles in the focal plane are sampled
		 ** from the specified distributions. There are three options to generate
		 ** the event vertex:
		 ** 1. If a vertex z position is specified by a call to ForceVertexAtZ,
     ** the event vertex is the beam extrapolation to the specified z.
		 ** 2. Else, if a target is specified, the event vertex is sampled from
		 ** the beam trajectory within the target - flat distributions between
		 ** entry and exit point, or always in the target centre plane, depending
		 ** on the choice set with SetSmearVertexZ().
		 ** 3. Else, the event vertex is the beam position in the focal plane.
		 **/
  virtual void MakeVertex();


  /** @brief Generate event vertex position at a given z
     **
     ** Will be used if ForceVertexAtZ was called.
     **/
  void MakeVertexAtZ();


  /** @brief Generate event vertex position in the beam focal plane
		 **
		 ** Will be used if no target was specified.
		 **/
  virtual void MakeVertexInFocalPlane();


  /** @brief Generate event vertex position in the target
		 **
		 ** Will be called if a target was specified.
		 **/
  virtual void MakeVertexInTarget();


  ClassDef(CbmEventGenerator, 2);
};


#endif /* CBMEVENTGENERATOR_H */
