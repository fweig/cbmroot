/** @file CbmStsParSensor.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.03.2020
 **/

#ifndef CBMSTSPARSENSOR_H
#define CBMSTSPARSENSOR_H 1

#define NPARAMS 10

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDefNV
#include <RtypesCore.h>  // for UInt_t, Float_t, Int_t

#include <array>         // for array
#include <string>        // for string

#include "CbmStsDefs.h"  // for CbmStsSensorClass, CbmStsSensorClass::kDssdS...

/** @class CbmStsParSensor
 ** @brief Constructional parameters of a STS sensor
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 23.03.2020
 **
 ** This class represents the internal design parameters of a sensor
 ** in the STS. The meaning of the parameters depends on the sensor class.
 ** For DSSD classes (stereo and ortho), the parameters describe:
 ** - 00  geometrical extension in x [cm]
 ** - 01  geometrical extension in y [cm]
 ** - 02  geometrical extension in z [cm]
 ** - 03  size of active area in y [cm]
 ** - 04  number of strips front side
 ** - 05  number of strips back side
 ** - 06  strip pitch front side [cm]
 ** - 07  strip pitch back side [cm]
 ** - 08  stereo angle front side [deg]; is zero for DssdOrtho
 ** - 09  stereo angle back side [deg]; is 90 for DssdOrtho
 **/
class CbmStsParSensor
{

  public:

    /** @brief Constructor
     ** @param name  Parameter set name
     ** @param title Parameter set title
     ** @param context  Parameter context
     **/
    CbmStsParSensor(CbmStsSensorClass sClass = CbmStsSensorClass::kDssdStereo);


    /** @brief Destructor **/
    ~CbmStsParSensor() {
    };


    /** @brief Get the sensor class
     ** @return Sensor class
     **/
    CbmStsSensorClass GetClass() const { return fClass; }


    /** @brief Get a parameter
     ** @param index  Parameter index
     ** @return Parameter value
     **
     ** Returns zero if index is out of range
     **/
    Float_t GetPar(UInt_t index) const;


    /** @brief Get the nearest integer value of a parameter
     ** @param index  Parameter index
     ** @return Nearest integer to parameter value
     **
     ** Returns zero if index is out of range
     **/
    Int_t GetParInt(UInt_t index) const;


    /** @brief Set a parameter
     ** @param index  Parameter index
     ** @param value  Parameter value
     **
     ** No action if index is out of range.
     **/
    void SetPar(UInt_t index, Float_t value) {
      if ( index < fPar.size() ) fPar[index] = value;
    }


    /** @brief Info to string **/
    std::string ToString() const;


  private:

    CbmStsSensorClass fClass = CbmStsSensorClass::kUnknown;
    std::array<float, NPARAMS> fPar { { 0., 0., 0., 0., 0.,
                                        0., 0., 0., 0., 0.} };


    ClassDefNV(CbmStsParSensor, 1);
};

#endif /* CBMSTSPARSENSOR_H */
