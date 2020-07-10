/** @file CbmBeam.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 1 August 2019
 **/

#ifndef CBMBEAM_H
#define CBMBEAM_H 1


#include "Rtypes.h"
#include "TVector3.h"
#include <string>


/** @class CbmBeam
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 1 August 2019
 ** @date 1 August 2019
 **
 ** Class describing a beam trajectory, which is assumed to be a straight line
 ** defined by a point and a direction vector.
 ** All parameters are defined w.r.t. the global coordinate system.
 **
 **/
class CbmBeam {

public:
  /** @brief Default constructor
		 ** @param x   x position [cm]
		 ** @param y   y position [cm]
		 ** @param z   z position [cm]
		 ** @param thetaX  Angle in x-z plane [rad]
		 ** @param thetaY  Angle in y-z plane [rad]
		 **/
  CbmBeam(Double_t x      = 0.,
          Double_t y      = 0.,
          Double_t z      = 0.,
          Double_t thetaX = 0.,
          Double_t thetaY = 0.);


  /** @brief Destructor  **/
  virtual ~CbmBeam() {};


  /** @brief Extrapolation of the beam to a plane
		 ** @param point  Coordinates of a point in the plane
		 ** @param norm   A vector perpendicular to the plane
		 ** @return       Intersection point of beam with the plane
		 **
		 ** The method returns the intersection point of the beam trajectory
		 ** with a plane specified by an anchor point and a vector perpendicular
		 ** to the plane. The latter need not be normalised.
		 **/
  TVector3 ExtrapolateToPlane(const TVector3& point,
                              const TVector3& normal) const;


  /** @brief Beam direction vector
		 ** return Direction vector (tx, ty, 1.)
		 **/
  TVector3 GetDirection() const { return fDirection; }


  /** @brief Beam position vector in the focal plane
		 ** @return Position vector [cm]
		 **/
  TVector3 GetPosition() const { return fPosition; }


  /** @brief Beam angle in the x-z plane
		 ** @return Beam angle in x-z [rad]
		 **/
  Double_t GetThetaX() const { return TMath::ATan(fDirection.X()); }


  /** @brief Beam angle in the y-z plane
		 ** @return Beam angle in y-z [rad]
		 **/
  Double_t GetThetaY() const { return TMath::ATan(fDirection.Y()); }


  /** @brief Info on current beam trajectory **/
  std::string ToString() const;


private:
  TVector3 fPosition;   ///< Position vector
  TVector3 fDirection;  ///< Direction vector (dx/dz, dy/dz, 1.)
};

#endif /* CBMBEAM_H */
