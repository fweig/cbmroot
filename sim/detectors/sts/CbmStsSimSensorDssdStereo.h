/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsSimSensorDssdStereo.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 07.08.2017
 **/

#ifndef CBMSTSSIMSENSORDSSDSTEREO_H
#define CBMSTSSIMSENSORDSSDSTEREO_H 1


#include "CbmStsSimSensorDssd.h"


/** @class CbmStsSimSensorDssdStereo
 ** @brief Detector response for DSSD sensors with stereo angles and
 ** cross-connection by double metal layers.
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 1.0
 **
 ** This class describes the response of double-sided silicon
 ** strip sensors with stereo angles ( < 90 degrees) on front and/or
 ** back side. .
 **
 ** The stereo angle is defined with respect to the y (vertical) axis.
 ** Readout is performed at the top edge of the sensor. In case of
 ** finite stereo angle, the corner strips not reaching the top edge are
 ** connected horizontally to the corresponding strip in the other corner.
 **
 ** A sensor of this type may be part of a daisy chain of several sensors
 ** arranged vertically on top of each others and being connected to the
 ** same module.
 **
 ** The mapping of strip number and module channel is trivial in the case
 ** of just one sensor per module. In case of several daisy-chained sensors,
 ** the top-edge strip is vertically connected vertically to the corresponding
 ** strip on the bottom edge of the sensor above. This results in an offset
 ** of strip number to channel number which depends on the position of the'
 ** sensor in the daisy chain. This behaviour is implemented in the methods
 ** GetStrip and GetModuleChannel.
 **/
class CbmStsSimSensorDssdStereo : public CbmStsSimSensorDssd {

public:
  /** Constructor
     ** @param address Unique element address
     ** @param node    Pointer to geometry node
     ** @param mother  Pointer to mother element (module)
     **/
  CbmStsSimSensorDssdStereo(CbmStsElement* element = nullptr);


  /** Constructor
     ** @param dy Length of active area in y [cm]
     ** @param nStrips Number of strips (same front and back)
     ** @param pitch   Strip pitch [cm]
     ** @param stereoF Stereo angle front side [degrees]
     ** @param stereoB Stereo angle back side [degrees]
     **/
  CbmStsSimSensorDssdStereo(Double_t dy, Int_t nStrips, Double_t pitch, Double_t stereoF, Double_t stereoB,
                            CbmStsElement* element = nullptr);


  /** @brief Copy constructor (disabled)  **/
  CbmStsSimSensorDssdStereo(CbmStsSimSensorDssdStereo& rhs) = delete;


  /** @brief Assignment operator (disabled)  **/
  CbmStsSimSensorDssdStereo& operator=(const CbmStsSimSensorDssdStereo& rhs) = delete;


  /** Destructor  **/
  virtual ~CbmStsSimSensorDssdStereo() {};


  /** @brief Number of strips (same for front and back side)
     ** @param side  Not used
     ** @value Number of strips
     **/
  virtual Int_t GetNofStrips(Int_t) const { return fNofStrips; }


  /** @brief Stereo angle for front and back side
     ** @param side  0 = front side, 1 = back side
     ** @value Stereo angle [degrees] on the specified sensor side
     **/
  Double_t GetStereoAngle(Int_t side) const { return (side == 0 ? fStereoF : fStereoB); }


  /** @brief Initialisation
     ** @value kTRUE if parameters and node are consistent
     **
     ** The consistency of geometric node and sensor parameters is checked;
     ** derived parameters are calculated.
     **/
  virtual Bool_t Init();


  /** @brief Modify the strip pitch
     ** @param New strip pitch [cm]
     **
     ** The number of strips is re-calculated accordingly.
     **/
  virtual void ModifyStripPitch(Double_t pitch);


  /** @brief String output **/
  std::string ToString() const;


protected:
  Int_t fNofStrips  = 0;     ///< Number of strips (same for front and back)
  Double_t fPitch   = 0.;    ///< Strip pitch /same for front and back)
  Double_t fStereoF = 100.;  ///< Stereo angle front side [degrees]
  Double_t fStereoB = 100;   ///< Stereo angle front back side [degrees]

  /** Temporary variables to avoid frequent calculations **/
  Double_t fTanStereo[2] {};  //! tangent of stereo angle front/back side
  Double_t fCosStereo[2] {};  //! cosine of stereo angle front/back side
  Int_t fStripShift[2] {};    //! Shift in number of strips from bottom to top
  Double_t fErrorFac = 0.;    //! Used for calculation of hit errors


  /** Charge diffusion into adjacent strips
     ** @param[in] x      x coordinate of charge centre (local c.s.) [cm]
     ** @param[in] y      y coordinate of charge centre (local c.s.) [cm]
     ** @param[in] sigma  Diffusion width [cm]
     ** @param[in] side   0 = front (p) side, 1 = back (n) side
     ** @param[out] fracL  Fraction of charge in left neighbour strip
     ** @param[out] fracC  Fraction of charge in centre strip
     ** @param[out] fracR  Fraction of charge in right neighbour strip
     **
     ** Calculates the fraction of charge in the most significant (centre)
     ** strip and its left and right neighbours. The charge distribution is
     ** assumed to be a 2-d Gaussian (resulting from thermal diffusion)
     ** with centre (x,y) and width sigma in both dimensions. The integration
     ** is performed in the coordinate across the strips. For simplicity,
     ** all charge left (right) of the centre strip is accumulated in the left
     ** (right) neighbour; this is justified since typical values of the
     ** diffusion width are much smaller than the strip pitch. The charge in
     ** the neighbouring strip is neglected if it is more distant than 3 sigma
     ** from the charge centre.
     ** Edge effects are neglected, i.e. diffusion into the inactive area is
     ** allowed.
     **/
  virtual void Diffusion(Double_t x, Double_t y, Double_t sigma, Int_t side, Double_t& fracL, Double_t& fracC,
                         Double_t& fracR);


  /** @brief Get the readout channel in the module for a given strip
     ** @param strip     Strip number
     ** @param side      Side (0 = front, 1 = back)
     ** @param sensorId  Index of sensor within module
     ** @return  Channel number in module
     **
     ** This method defines the mapping of the sensor strips to the
     ** readout channels in the module.
     **/
  virtual Int_t GetModuleChannel(Int_t strip, Int_t side, Int_t sensorId) const;


  /** @brief Get strip number from point coordinates
     ** @param x     x coordinate [cm]
     ** @param y     y coordinate [cm]
     ** @param side  0 = front side, 1 = back side
     ** @return strip number on selected side
     **/
  virtual Int_t GetStripNumber(Double_t x, Double_t y, Int_t side) const;


  /** Propagate a charge created in the sensor to the readout strips
     ** @param x       x origin of charge in local c.s. [cm]
     ** @param y       y origin of charge in local c.s. [cm]
     ** @param z       z origin of charge in local c.s. [cm]
     ** @param charge  Charge [e]
     ** @param bY      Magnetic field (y component) [T]
     ** @param side    0 = front (n) side; 1 = back (p) side
     ** @param sensor  Pointer to sensor object
     **/
  virtual void PropagateCharge(Double_t x, Double_t y, Double_t z, Double_t charge, Double_t bY, Int_t side);


  ClassDef(CbmStsSimSensorDssdStereo, 1);
};


#endif
