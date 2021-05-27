/** @file CbmMvdMimosis.h
  * @copyright Copyright (C) 2017 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Philipp Sitzmann [orginator] **/

// -------------------------------------------------------------------------
// -----                      CbmMvdMimosis header file                -----
// -----                                                               -----
// -------------------------------------------------------------------------


/** CbmMvdMimosis.h
 **
 ** Data base for the Properties of MIMOSIS
 **   
 **/


#ifndef CBMMVDMIMOSIS_H
#define CBMMVDMIMOSIS_H 1


#include "CbmMvdSensorDataSheet.h"


class CbmMvdMimosis : public CbmMvdSensorDataSheet {

public:
  /** Default constructor **/
  CbmMvdMimosis();

  /** Destructor **/
  ~CbmMvdMimosis();


  ClassDef(CbmMvdMimosis, 1);
};


#endif
