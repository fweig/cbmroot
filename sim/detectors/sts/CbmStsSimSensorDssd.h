/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsSimSensorDssd.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.03.2020
 **/

#ifndef CBMSTSSIMSENSORDSSD_H
#define CBMSTSSIMSENSORDSSD_H 1


#include "CbmStsSimSensor.h"

#include <TArrayD.h>


/** @class CbmStsSimSensorDssd
 ** @brief Abstract class for the simulation of double-sided
 ** silicon strip sensors.
 ** @author V.Friese <v.friese@gsi.de>
 ** @since 16.03.2020
 **
 ** This abstract class describes the (analogue) response of double-sided silicon
 ** strip sensors in the STS.
 **
 ** The active area does not necessarily coincide with the geometric
 ** dimensions of the sensor. It is, however, centred in the latter,
 ** meaning that the width of inactive regions (guard ring) are
 ** the same on the left and on the right side and also the same at
 ** the top and and the bottom.
 **
 ** The response to charged particles is modelled by a uniform charge
 ** distribution along the particle trajectory in the active volume,
 ** which is projected to the readout edge, where it is discretised
 ** on the active strips. The charge is then delivered to the corresponding
 ** channel of the readout module (CbmStsModule).
 **
 ** Concrete classes have to implement the pure virtual method
 ** PropagateCharge, which has to properly fill the charge arrays
 ** fStripCharge for front and back side, along with the auxiliary method
 ** Diffusion for the thermal diffusion along the drift to the readout plane.
 ** Also, the mapping from the strip numbers to the (module) channel number
 ** has to be implemented in GetModuleChannel. These methods will e.g. be
 ** different for sensors with stereo angles, where both sides are read out
 ** at the same edge, and for sensor with orthogonal strips, where the back
 ** plane is read out at the left or right edge.
 **/
class CbmStsSimSensorDssd : public CbmStsSimSensor {

public:
  /** @brief Standard constructor
     ** @param element Pointer to element of geometry setup
     **/
  CbmStsSimSensorDssd(CbmStsElement* element = nullptr);


  /** @brief Copy constructor (disabled) **/
  CbmStsSimSensorDssd(CbmStsSimSensorDssd& rhs) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmStsSimSensorDssd& operator=(const CbmStsSimSensorDssd& rhs) = delete;


  /** @brief Destructor  **/
  virtual ~CbmStsSimSensorDssd() {};


  /** @brief Number of strips on front and back side
     ** @param side  0 = front side, 1 = back side
     ** @value Number of strips on the specified sensor side
     **/
  virtual Int_t GetNofStrips(Int_t side) const = 0;


  /** @brief Modify the strip pitch
     ** @param New strip pitch [cm]
     **
     ** The number of strips is re-calculated accordingly.
     **/
  virtual void ModifyStripPitch(Double_t pitch) = 0;


  /** @brief Print charge status **/
  std::string ChargeStatus() const;


  /** @brief String output **/
  virtual std::string ToString() const = 0;


protected:
  Double_t fDx  = 0.;      ///< Dimension of active area in x [cm]
  Double_t fDy  = 0.;      ///< Dimension of active area in y [cm]
  Double_t fDz  = 0.;      ///< Thickness in z [cm]
  Bool_t fIsSet = kFALSE;  ///< Flag whether sensor is properly initialised

  /** Analog charge in strips (for front and back side).
     ** Used during analog response simulation. **/
  TArrayD fStripCharge[2] {};  //!


  /** @brief Analogue response to a track in the sensor
     ** @param point  Pointer to CbmStsSensorPoint object
     ** @value Number of analogue signals created in the strips
     **
     ** In this method, the analogue response of the sensor to a charged
     ** particle traversing it is implemented. The input is an object of
     ** type CbmStsSensorPoint, giving the geometric intersection of the
     ** track with the sensor in the sensor internal coordinate system.
     ** The method shall create charges in the internal arrays fStripCharge.
     **/
  virtual Int_t CalculateResponse(CbmStsSensorPoint* point);


  /** Cross talk
     ** @param ctCoeff  Cross-talk coefficient
     **
     ** Operates on the strip charge arrays and re-distributes charges
     ** between adjacent strips according to the cross-talk coefficient.
     **/
  void CrossTalk(Double_t ctCoeff);


  /** @brief Get the readout channel in the module for a given strip
     ** @param strip     Strip number
     ** @param side      Side (0 = front, 1 = back)
     ** @param sensorId  Index of sensor within module
     ** @return  Channel number in module
     **
     ** This method defines the mapping of the sensor strips to the
     ** readout channels in the module.
     **/
  virtual Int_t GetModuleChannel(Int_t strip, Int_t side, Int_t sensorId) const = 0;


  /** Check whether a point (x,y) is inside the active area.
     **
     ** @param x  x coordinate in the local c.s. [cm]
     ** @param y  y coordinate in the local c.s. [cm]
     ** @return  kTRUE if inside active area.
     **
     ** The coordinates have to be given in the local
     ** coordinate system (origin in the sensor centre).
     **/
  Bool_t IsInside(Double_t x, Double_t y);


  /** @brief Lorentz shift in the x coordinate
     ** @param z           Coordinate of charge origin in local c.s. [cm]
     ** @param chargeType  Type of charge carrier (0 = electron, 1 = hole)
     ** @param bY          Magnetic field (y component) [T]
     ** @value Displacement in x due to Lorentz shift [cm]
     **
     ** Calculates the displacement in x of a charge propagating to
     ** the readout plane of the sensor.
     **
     ** TODO: This assumes that the sensor is oriented vertically. It has
     ** to be implemented correctly for arbitrary orientations of the local
     ** x-y plane.
     **/
  Double_t LorentzShift(Double_t z, Int_t chargeType, Double_t bY) const;


  /** @brief Generate charge as response to a sensor point
     ** @param point  Pointer to sensor point object
     **
     ** Charge is created in the sensor volume as response to the particle
     ** trajectory and is propagated to the read-out edges.
     **/
  void ProduceCharge(CbmStsSensorPoint* point);


  /** Propagate a charge created in the sensor to the readout strips
     ** @param x       x origin of charge in local c.s. [cm]
     ** @param y       y origin of charge in local c.s. [cm]
     ** @param z       z origin of charge in local c.s. [cm]
     ** @param charge  Charge [e]
     ** @param bY      Magnetic field (y component) [T]
     ** @param side    0 = front (n) side; 1 = back (p) side
     ** @param sensor  Pointer to sensor object
     **/
  virtual void PropagateCharge(Double_t x, Double_t y, Double_t z, Double_t charge, Double_t bY, Int_t side) = 0;


  /** @brief Register the produced charge in one strip to the module
     ** @param side  0 = front, 1 = back
     ** @param strip strip number
     ** @param charge  charge in strip [e]
     ** @param time    time of registration [ns]
     **
     ** The charge in one strip resulting from the analogue response
     ** simulation is registered to the read-out chip (module).
     **/
  void RegisterCharge(Int_t side, Int_t strip, Double_t charge, Double_t time) const;


  ClassDef(CbmStsSimSensorDssd, 1);
};


#endif
