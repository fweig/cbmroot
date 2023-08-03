/*
 * NicaPropertyMonitorX.h
 *
 *  Created on: 21 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAPROPERTYMONITORX_H_
#define NICAPROPERTYMONITORX_H_

#include "NicaCutMonitorX.h"

/**
 * class to monitor data properties (in contrast to cut monitor doesnt require
 * cuts
 */
class NicaPropertyMonitorX : public NicaCutMonitorX {
  TString fXaxisName;
  TString fYaxisName;

 protected:
  ENicaFormatType fFormatType;
  virtual void CreateHistograms();

 public:
  /**
   *
   * @param xLabel name of x axis
   * @param yLabel name of y axis
   * @param ratio update ratio
   */
  NicaPropertyMonitorX(TString xLabel = "", TString yLabel = "",
                       ENicaCutUpdate ratio = ENicaCutUpdate::kEventUpdate);
  NicaPropertyMonitorX(const NicaPropertyMonitorX& other);
  /**
   * Assignment operator
   * @param other
   * @return
   */
  NicaPropertyMonitorX& operator=(const NicaPropertyMonitorX& other);
  virtual Bool_t Init(Int_t task_id);
  virtual Bool_t ObjMonitor() const { return kTRUE; };
  virtual NicaCutMonitor* MakeCopy() const {
    return (NicaCutMonitor*)this->Clone();
  };
  virtual NicaPackage* Report() const;
  virtual ~NicaPropertyMonitorX();
  ClassDef(NicaPropertyMonitorX, 1)
};

#endif /* NICAPROPERTYMONITORX_H_ */
