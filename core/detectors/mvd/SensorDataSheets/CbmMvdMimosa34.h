/* Copyright (C) 2014 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                      CbmMvdMimosa34 header file               -----
// -----                  Created 10/12/14  by B. Linnik               -----
// -------------------------------------------------------------------------


/** CbmMvdMimosa34.h
 **
 ** Data base for the Properties of MIMOSA-34 
 **   
 **/


#ifndef CBMMVDMIMOSA34_H
#define CBMMVDMIMOSA34_H 1

#include "CbmMvdSensorDataSheet.h"  // for CbmMvdSensorDataSheet

#include <Rtypes.h>                 // for ClassDef

class TBuffer;
class TClass;
class TMemberInspector;


class CbmMvdMimosa34 : public CbmMvdSensorDataSheet {

public:
  /** Default constructor **/
  CbmMvdMimosa34();

  /** Destructor **/
  ~CbmMvdMimosa34();


  ClassDef(CbmMvdMimosa34, 1);
};


#endif
