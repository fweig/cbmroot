/** @file CbmMuchDigiMatch.h
 ** @author Vikas Singhal <vikas@vecc.gov.in>
 ** @since May 2016
 ** @version 4.0
 ** Created just for compatibility. As CbmMuchDigiMatch is used many places in many Much classes.
 ** Near future will remove this class whole together and only use the parent class CbmMatch.
 **/


#ifndef CBMMUCHDIGIMATCH_H
#define CBMMUCHDIGIMATCH_H 1

#include "CbmMatch.h"  // for CbmMatch

#include <Rtypes.h>  // for ClassDef

class CbmMuchDigiMatch : public CbmMatch {

public:
  /** Default constructor **/
  CbmMuchDigiMatch() : CbmMatch() {}

  /** Constructor called from CbmMuchBeamTimeDigi**/
  CbmMuchDigiMatch(CbmMuchDigiMatch* match);

  /** Destructor **/
  virtual ~CbmMuchDigiMatch() {}

  ClassDef(CbmMuchDigiMatch, 2);
};


#endif
