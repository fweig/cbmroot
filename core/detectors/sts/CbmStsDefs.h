/** @file CbmDefs.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 30.05.2017
 **
 ** Header for definition of CBM enumerators and constants
 **/

#ifndef CBMSTSDEFS_H
#define CBMSTSDEFS_H 1

#include "Rtypes.h"


// -----   Namespace CbmSts   ---------------------------------------------
namespace CbmSts
{

  /** @brief Silicon charge [e] **/
  const Double_t kSiCharge = 14.;


  /** @brief Silicon density [g/cm^3] **/
  const Double_t kSiDensity = 2.336;


  /** @brief Proton mass [GeV] **/
  const Double_t kProtonMass = 0.938272081;


}  // namespace CbmSts

// ------------------------------------------------------------------------


/** @brief Energy loss model used in simulation
 **
 ** This enumerator denotes the model of the distributions of the charge
 ** created by the passage of a charged particle through a Silicon sensor.
 **
 ** Ideal: All charge is created in the centre plane of the sensor
 ** Uniform: The charge distribution is uniform
 ** Urban: The charge fluctuates according the Urban model
 **/
enum class CbmStsELoss
{
  kIdeal   = 0,
  kUniform = 1,
  kUrban   = 2,
};


/** @brief Sensor classes
 **
 ** DssdStereo: Double-sided strip sensor with stereo angles on one or both
 ** sides. The strips are read-out at the top edge. In case of stereo angle
 ** on a side, there is a cross-connection by a double metal layer,
 ** connecting strips ending at the sides of the sides of the sensor.
 **
 ** DssdOrtho: Double-sided strip sensor with vertical strips on the front
 ** side and horizontal strips at the back side. The front side is read-out
 ** at the top edge, the back side at the left edge.
 **/
enum class CbmStsSensorClass
{
  kUnknown    = 0,
  kDssdStereo = 1,
  kDssdOrtho  = 2
};

#endif
