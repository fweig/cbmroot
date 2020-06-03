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



class CbmMvdMimosis : public CbmMvdSensorDataSheet
{

 public:

  /** Default constructor **/
  CbmMvdMimosis();
    
   /** Destructor **/
  ~CbmMvdMimosis();

 
  ClassDef(CbmMvdMimosis,1);

};



#endif
