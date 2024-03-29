/* Copyright (C) 2014 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Michael Deveaux, Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                      CbmMvdMimosa26AHR header file                -----
// -----                  Created 02/12/08  by M. Deveaux             -----
// -------------------------------------------------------------------------


/** CbmMvdMimosa26AHR.h
 *@author M.Deveaux <deveaux@physik.uni-frankfurt.de>
 **
 ** Data base for the Properties of MIMOSA-26 AHR
 ** This sensor is used in the CBM-MVD prototype/telescope
 **   
 **/


#ifndef CBMMVDMIMOSA26AHR_H
#define CBMMVDMIMOSA26AHR_H 1

#include "CbmMvdSensorDataSheet.h"  // for CbmMvdSensorDataSheet

#include <Rtypes.h>  // for ClassDef

class TBuffer;
class TClass;
class TMemberInspector;

class CbmMvdMimosa26AHR : public CbmMvdSensorDataSheet {

public:
  /** Default constructor **/
  CbmMvdMimosa26AHR();

  /** Destructor **/
  ~CbmMvdMimosa26AHR();


  ClassDef(CbmMvdMimosa26AHR, 1);
};


#endif
