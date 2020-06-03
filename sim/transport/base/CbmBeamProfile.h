/** @file CbmBeamProfile.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 1 August 2019
 **/

#ifndef CBMBEAMPROFILE_H
#define CBMBEAMPROFILE_H 1


#include <memory>
#include <string>
#include "Rtypes.h"
#include "TVector3.h"
#include "CbmBeam.h"
#include "CbmTarget.h"


/** @class CbmBeamProfile
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 1 August 2019
 ** @date 1 August 2019
 **
 ** Class defining beam properties (position, width, angle, emittance) in CBM.
 ** All parameters are defined w.r.t. the global coordinate system in
 ** the focal plane of the beam, which is by default at z = 0.
 **
 ** The beam profile is assumed to be Gaussian in x and y. The distribution
 ** of beam angles is also assumed to be Gaussian. Negative widths mean
 ** fixed position and / or angle.
 **/
class CbmBeamProfile
{

	public:

		/** @brief Default constructor  **/
		CbmBeamProfile();


		/** @brief Destructor  **/
		virtual ~CbmBeamProfile() { };


		/** @brief Check consistency with a target
		 ** @param target  Pointer to target class
		 ** @return true if average beam hits the target; else false.
		 **/
		Bool_t CheckWithTarget(const CbmTarget& target) const;


		/** @brief Extrapolate the average beam to a plane
		 ** @param point  Coordinates of a point in the plane
		 ** @param norm   A vector perpendicular to the plane
		 ** @return       Extrapolation of average beam
		 **
		 ** The method returns the intersection point of the average
		 ** beam with a plane, specified by the point and a vector
		 ** perpendicular to the plane. The latter need not be normalised.
		 **/
		TVector3 ExtrapolateToPlane(const TVector3& point,
																const TVector3& norm) const;


		/** @brief Generate a beam trajectory
		 **
		 ** The beam parameters x, y, tx and ty in the focal plane are
		 ** sampled from the specified distributions.
		 **/
		std::unique_ptr<CbmBeam> GenerateBeam();



		/** @brief Set the parameters for the beam angle distribution
		 ** @param x0      Mean angle in x-z [rad]
		 ** @param y0      Mean angle in y-z [rad]
		 ** @param sigmaX  Gauss RMS of angle distribution in x-z [rad]
		 ** @param sigmaY  Gauss RMS of angle distribution in y-z [rad]
		 **
		 ** A non-positive value for sigma means a fixed angle (no sampling).
		 **/
		void SetAngle(Double_t x0, Double_t y0,
									Double_t sigmaX = -1., Double_t sigmaY = -1.);


		/** @brief Set the parameters for the beam position distribution
		 ** @param x0      Mean x position [cm]
		 ** @param y0      Mean y position [cm]
		 ** @param sigmaX  Gauss RMS of x position distribution [cm]
		 ** @param sigmaY  Gauss RMS of y position distribution [cm]
		 ** @param zF      z position of focal plane [cm]
		 **
		 ** A non-positive value for sigma means a fixed position (no sampling).
		 **/
		void SetPosition(Double_t x0, Double_t y0, Double_t sigmaX = -1.,
										 Double_t sigmaY = -1., Double_t zF = 0.);


		/** @brief Info to string **/
		std::string ToString() const;



	private:

		Double_t fFocalZ;       ///< z coordinate of focal plane [cm]
		Double_t fMeanPosX;     ///< Mean position in x [cm]
		Double_t fMeanPosY;     ///< Mean position in y [cm]
		Double_t fSigmaPosX;    ///< RMS of position in x [cm]
		Double_t fSigmaPosY;    ///< RMS of position in y [cm]
		Double_t fMeanThetaX;   ///< Mean angle in x-z plane [rad]
		Double_t fMeanThetaY;   ///< Mean angle in y-z plane [rad]
		Double_t fSigmaThetaX;  ///< RMS of angle in x-z plane [rad]
		Double_t fSigmaThetaY;  ///< RMS of angle in y-z plane [rad]

};

#endif /* CBMBEAMPROFILE_H */
