/** @file CbmTarget.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 08.10.2013
 **/

#ifndef CBMTARGET_H
#define CBMTARGET_H 1


#include <string>
#include "TVector3.h"
#include "FairModule.h"


/** @class CbmTarget
 ** @brief Class for constructing the geometry of the CBM target
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 08.10.2013
 **
 ** The target of CBM can be created either on the fly, by specifying
 ** the target material (element), its thickness, its diameter and
 ** the density of the material. In this case, a single volume of shape
 ** TUBE will be positioned at the target position, as daughter of the
 ** node already present there. If no position is specified by the method
 ** SetPosition, the target is placed at the origin of the global coordinate
 ** system.
 **
 ** In case a geometry file is specified, the geometry will be constructed
 ** from this file in the same manner as for all other modules.
 **/
class CbmTarget : public FairModule {

	public:

		/** @brief Default constructor **/
		CbmTarget();


		/** @brief Constructor with file name
		 ** @param fileName  Name of geometry file
		 **
		 ** This constructor will force the target geometry to be
		 ** constructed from a geometry file (ROOT format only).
		 **/
		CbmTarget(const char* fileName);


		/** @brief Constructor with target properties
		 ** @param element   Element name of target material
		 **                  (Full name or element symbol, e.g. "Gold", "Au");
		 ** @param thickness Thickness in z [cm]
		 ** @param diameter  Diameter [cm]
		 ** @param density   Density [g/cm^3]. If negative (default), density under standard
		 **                  conditions will be taken, if available.
		 **
		 ** By using this constructor, the target geometry will be constructed
		 ** as a tube with material and parameters specified as arguments.
		 **/
		CbmTarget(const char* element, Double_t thickness,
							Double_t diameter, Double_t density = -1.);


		/** @brief Constructor with target properties
		 ** @param z         Atomic charge of target material element
		 ** @param thickness Thickness in z [cm]
		 ** @param diameter  Diameter [cm]
		 ** @param density   Density [g/cm^3]. If negative, density under standard
		 **                  conditions will be taken.
		 **
		 ** By using this constructor, the target geometry will be constructed
		 ** as a tube with material and parameters specified as arguments.
		 **/
		CbmTarget(Int_t z, Double_t thickness,
							Double_t diameter, Double_t density = -1.);


		/** @brief Destructor **/
		virtual ~CbmTarget() { };


		/** @brief Built the ROOT geometry **/
		virtual void ConstructGeometry();


		/** @brief Get target diameter
		 ** @return Target diameter [cm]
		 **/
		Double_t GetDiameter() const { return fDiameter; }


		/** @brief Normal vector
		 ** @return Normale vector to the target surface
		 **/
		TVector3 GetNormal() const;


		/** Get target centre position
		 ** @return Target position vector in global c.s. [cm]
		 **/
		TVector3 GetPosition() const { return TVector3(fPosX, fPosY, fPosZ); }


		/** @brief Get target rotation angle
		 ** @return  Target rotation angle around the y axis [rad]
		 **/
		Double_t GetRotation() const { return fRotY; }


		/** @brief Downstream surface centre
		 ** @return Coordinate vector of downstream surface centre
		 **/
		TVector3 GetSurfaceCentreDown() const;


		/** @brief Upstream surface centre
		 ** @return Coordinate vector of upstream surface centre
		 **/
		TVector3 GetSurfaceCentreUp() const;


		/** @brief Get target thickness
		 ** @value  Full target thickness in z [cm]
		 **/
		Double_t GetThickness() const { return fThickness; }


		/** @brief Get x coordinate of target centre
		 ** @return x coordinate of target centre in global c.s. [cm]
		 **/
		Double_t GetX() const { return fPosX; }


		/** @brief Get y coordinate of target centre
		 ** @return y coordinate of target centre in global c.s. [cm]
		 **/
		Double_t GetY() const { return fPosY; }


		/** @brief Get z coordinate of target centre
		 ** @return z coordinate of target centre in global c.s. [cm]
		 **/
		Double_t GetZ() const { return fPosZ; }


		/** @brief Output to stdout **/
		//virtual void Print(Option_t* = "") const;


		/** @brief Set the geometry file name
		 ** @param name    Name of geometry file
		 ** @param geoVer  Not used
		 **
		 ** If a geometry file is set by this method, the target geometry
		 ** will be built from this file; the parameters will have no effect.
		 **/
		virtual void SetGeometryFileName(TString name, TString geoVer = "0");


		/** @brief Set the position of the target w.r.t. the global coordinate system.
		 ** @param posX  target centre position in x [cm]
		 ** @param posY  target centre position in y [cm]
		 ** @param posZ  target centre position in z [cm]
		 **/
		void SetPosition(Double_t x, Double_t y, Double_t z) {
			fPosX = x;
			fPosY = y;
			fPosZ = z;
		}


		/** @brief Set the rotation of the target w.r.t. the global coordinate system.
		 ** @param angle target rotation angle around the y axis [rad]
		 **/
		void SetRotation(Double_t angle) {
			fRotY = angle;
		}


		/** @brief Info **/
		std::string ToString() const;


	private:

		Int_t    fZ;              ///< Atomic charge of target material
		Double_t fThickness;      ///< Thickness [cm]
		Double_t fDiameter;       ///< Diameter [cm]
		Double_t fDensity;        ///< Density of target material [g/cm^3]
		Double_t fPosX;           ///< Target centre position in x [cm]
		Double_t fPosY;           ///< Target centre position in y [cm]
		Double_t fPosZ;           ///< Target centre position in z [cm]
		Double_t fRotY;           ///< Target rotation angle around the y axis [rad]
		TString  fMaterial;       ///< Material name
		Bool_t   fBuildFromFile;  ///< Flag for building from geometry file


		/** Get the standard density (at standard conditions)
		 ** @param  charge  atomic charge of material
		 ** @return density [g/cm^3]
		 **/
		Double_t GetStandardDensity(Int_t charge) const;


		ClassDef(CbmTarget,3)

};

#endif //CBMTARGET_H

