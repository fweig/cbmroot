/*
 * NicaPropertyMonitorXY.h
 *
 *  Created on: 21 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAPROPERTYMONITORXY_H_
#define NICAPROPERTYMONITORXY_H_

/**
 * class to monitor data properties (in contrast to cut monitor doesnt require
 * cuts
 */
#include "NicaCutMonitorXY.h"
class NicaPropertyMonitorXY : public NicaCutMonitorXY {
  TString fXaxisName;
  TString fYaxisName;

 protected:
  ENicaFormatType fFormatType;
  virtual void CreateHistograms();

 public:
  /**
   *
   * @param xLabel x-axis name
   * @param yLabel y-axis name
   * @param update update ratio
   */
  NicaPropertyMonitorXY(TString xLabel = "", TString yLabel = "",
                        ENicaCutUpdate update = ENicaCutUpdate::kEventUpdate);
  NicaPropertyMonitorXY(const NicaPropertyMonitorXY& other);
  /**
   * Assignment operator
   * @param other
   * @return
   */
  NicaPropertyMonitorXY& operator=(const NicaPropertyMonitorXY& other);
  virtual Bool_t Init(Int_t task_id);
  virtual Bool_t ObjMonitor() const { return kTRUE; };
  virtual NicaPackage* Report() const;
  virtual NicaCutMonitor* MakeCopy() const {
    return (NicaCutMonitor*)this->Clone();
  };
  virtual ~NicaPropertyMonitorXY();
  ClassDef(NicaPropertyMonitorXY, 1)
};

#endif /* NICAPROPERTYMONITORXY_H_ */
