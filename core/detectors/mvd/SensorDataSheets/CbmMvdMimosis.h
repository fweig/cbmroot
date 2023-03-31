/* Copyright (C) 2017 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

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

#include "CbmMvdSensorDataSheet.h"  // for CbmMvdSensorDataSheet

#include <Rtypes.h>  // for ClassDef

class TBuffer;
class TClass;
class TMemberInspector;

class CbmMvdMimosis : public CbmMvdSensorDataSheet {

public:
  /** Default constructor **/
  CbmMvdMimosis();

  /** Destructor **/
  ~CbmMvdMimosis();


  ClassDef(CbmMvdMimosis, 1);
};


#endif
