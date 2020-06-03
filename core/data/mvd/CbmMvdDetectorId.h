// -------------------------------------------------------------------------
// -----                   CbmMvdDetectorId header file                -----
// -----                 Created 22/04/09  by V. Friese                -----
// -------------------------------------------------------------------------


/** CbmMvdDetectorId.h
 ** Defines unique detector identifier for all MVD components. 
 ** Classes using this ID scheme should derive from this class.
 ** @author V.Friese <v.friese@gsi.de>
 **/
 
 
/** Current definition:
 ** System ID (kMVD=1) on bits 0-4
 ** Station number on bits 5-31
 **/
 

#ifndef CBMMVDDETECTORID_H
#define CBMMVDDETECTORID_H 1


#include <Rtypes.h>             // for ClassDef
#include <RtypesCore.h>         // for Int_t

#include <FairLogger.h>         // for LOG

#include "CbmDefs.h"            // for ECbmModuleId::kMvd


class CbmMvdDetectorId
{

  public:
  
  
  /** Constructor **/
  CbmMvdDetectorId();
  
  
  /** Destructor **/
  virtual ~CbmMvdDetectorId()  {}
  
  
  /** Create unique detector ID from station number **/
  Int_t DetectorId(Int_t iStation) const {
     return ( ToIntegralType(ECbmModuleId::kMvd) | (iStation << 5) );
  }
  
  
  /** Get System identifier from detector ID **/
  Int_t SystemId(Int_t detectorId) const {
    Int_t iSystem = detectorId & 31;
    if ( iSystem != ToIntegralType(ECbmModuleId::kMvd) ) {
    	LOG(error) << "wrong system ID " << iSystem;
	return -1;
    }
    return iSystem;
  }

  
  /** Get station number from detector ID **/
  Int_t StationNr(Int_t detectorId) const {
    return ( ( detectorId & ( ~31 ) ) >> 5 );
  }
  
  
  
  ClassDef(CbmMvdDetectorId,1);
  
};


#endif
