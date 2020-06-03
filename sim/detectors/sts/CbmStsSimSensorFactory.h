/** @file CbmStsSimSensorFactory.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.03.2020
 **/

#ifndef CBMSTSSIMSENSORFACTORY_H
#define CBMSTSSIMSENSORFACTORY_H 1


#include <memory>
#include <Rtypes.h>

#include "CbmStsSimSensor.h"

class CbmStsParSensor;


/** @class CbmStsSimSensorFactory
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.03.2020
 **
 ** The factory creates objects of types derived from CbmStsSimSensors
 ** used for the simulation of the STS detector response. Currently,
 ** two sensor types are supported: DssdStereo (double-sided strip sensor
 ** with stereo angles on both sides, read out at the top edge) and
 ** DssdOrtho (double-sided strip sensor with vertical strips on the front
 ** side, read out at the top edge) and horizontal strips on the back side
 ** (read out at the left edge).
 */
class CbmStsSimSensorFactory {

  public:

    typedef std::unique_ptr<CbmStsSimSensor> UP_sensor;

    /** @brief Constructor **/
    CbmStsSimSensorFactory();


    /** @Destructor **/
    virtual ~CbmStsSimSensorFactory();


    /** @brief Create a sensor
     ** @param par Sensor parameter object
     ** @return unique_ptr to sensor object
     **/
    UP_sensor CreateSensor(const CbmStsParSensor& par);

    ClassDef(CbmStsSimSensorFactory, 2)

};


#endif /* CBMSTSSIMSENSORFACTORY_H */
