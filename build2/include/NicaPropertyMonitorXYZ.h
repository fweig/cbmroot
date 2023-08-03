/*
 * NicaPropertyMonitorXYZ.h
 *
 *  Created on: 21 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAPROPERTYMONITORXYZ_H_
#define NICAPROPERTYMONITORXYZ_H_

#include "NicaCutMonitorXYZ.h"

/**
 * class to monitor data properties (in contrast to cut monitor doesnt require
 * cuts
 */
class NicaPropertyMonitorXYZ : public NicaCutMonitorXYZ {
  TString fXaxisName;
  TString fYaxisName;
  TString fZaxisName;

 protected:
  ENicaFormatType fFormatType;
  virtual void CreateHistograms();

 public:
  /**
   *
   * @param xLabel x-axis name
   * @param yLabel y-axis name
   * @param zLabel z-axis name
   * @param update update ratio
   */
  NicaPropertyMonitorXYZ(TString xLabel = "", TString yLabel = "",
                         TString zLabel = "",
                         ENicaCutUpdate update = ENicaCutUpdate::kEventUpdate);
  NicaPropertyMonitorXYZ(const NicaPropertyMonitorXYZ& other);
  /**
   * Assignment operator
   * @param other
   * @return
   */
  NicaPropertyMonitorXYZ& operator=(const NicaPropertyMonitorXYZ& other);
  virtual Bool_t Init(Int_t task_id);
  virtual Bool_t ObjMonitor() const { return kTRUE; };
  virtual NicaPackage* Report() const;
  virtual NicaCutMonitor* MakeCopy() const {
    return (NicaCutMonitor*)this->Clone();
  };
  virtual ~NicaPropertyMonitorXYZ();
  ClassDef(NicaPropertyMonitorXYZ, 1)
};

#endif /* NICAPROPERTYMONITORXYZ_H_ */
