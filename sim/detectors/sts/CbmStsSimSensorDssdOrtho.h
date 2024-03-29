/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Pierre-Alain Loizeau */

/** @file CbmStsSimSensorDssdOrtho.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.03.2020
 **/

#ifndef CBMSTSSIMSENSORDSSDORTHO_H
#define CBMSTSSIMSENSORDSSDORTHO_H 1


#include "CbmStsSimSensorDssd.h"

#include <cassert>


/** @class CbmStsSimSensorDssdOrtho
 ** @brief Detector response for DSSD sensors with orthogonal strips
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 16.03.2020
 **
 ** This class describes the response of double-sided silicon
 ** strip sensors with vertical strips on the front side, which are
 ** read out at the top edge, and horizontal strips at the back side,
 ** which are read out at the left edge.
 **
 ** For these sensors, a daisy-chain with other sensors is not allowed.
 ** The mapping of strip number and module channel is thus trivial.
 **/
class CbmStsSimSensorDssdOrtho : public CbmStsSimSensorDssd {

public:
  /** @brief Standard constructor
     ** @param element Pointer to element in geometry setup
     **/
  CbmStsSimSensorDssdOrtho(CbmStsElement* element = nullptr);


  /** @brief Constructor with parameters
     ** @param nStripsF     Number of strips front side (vertical)
     ** @param pitchF       Strip pitch front side [cm]
     ** @param nStripsB     Number of strips back side (horizontal)
     ** @param pitchB       Strip pitch back side [cm]
     ** @param element      Pointer to element in geometry setup
     **/
  CbmStsSimSensorDssdOrtho(Int_t nStripsF, Double_t pitchF, Int_t nStripsB, Double_t pitchB,
                           CbmStsElement* element = nullptr);


  /** @brief Copy constructor (disabled)  **/
  CbmStsSimSensorDssdOrtho(CbmStsSimSensorDssdOrtho& rhs) = delete;


  /** @brief Assignment operator (disabled)  **/
  CbmStsSimSensorDssdOrtho& operator=(const CbmStsSimSensorDssdOrtho& rhs) = delete;


  /** @brief Destructor  **/
  virtual ~CbmStsSimSensorDssdOrtho() {};


  /** @brief Number of strips on front or back side
     ** @param side  0 = front side, 1 = back side
     ** @return  Number of strips
     **/
  virtual Int_t GetNofStrips(Int_t side) const
  {
    assert(side == 0 || side == 1);
    return fNofStrips[side];
  }


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


  /** @brief Set the internal sensor parameters
     ** @param nStripsF          Number of strips front side (vertical)
     ** @param pitchF            Strip pitch front side [cm]
     ** @param nStripsB          Number of strips back side (horizontal)
     ** @param pitchB            Strip pitch back side [cm]
     ** @value kTRUE if parameters are successfully set; else kFALSE
     **/
  Bool_t SetParameters(Int_t nStripsF, Double_t pitchF, Int_t nStripsB, Double_t pitchB);


  /** String output **/
  std::string ToString() const;


protected:
  Int_t fNofStrips[2];  ///< Number of strips on front/back side
  Double_t fPitch[2];   ///< Strip pitch front/back side [cm]


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


  ClassDef(CbmStsSimSensorDssdOrtho, 1);
};


#endif
