/** @file CbmTofTracklet.h
 ** @author nh <N.Herrmann@gsi.de>
 ** @date 28.02.2020
 **/

#ifndef CBMTOFTRACKLETTOOLS_H
#define CBMTOFTRACKLETTOOLS_H 1

#include "TMath.h"
#include "CbmTofTrackletParam.h"
#include "FairTrackParam.h"
#include "CbmTofHit.h"
#include "CbmTofTracklet.h"

#include <vector>

/** @class CbmTofTrackletTools
 ** @brief  contains fits and resolution functions
 ** @author nh
 **/
class CbmTofTrackletTools
{

  public:
	/**   Constructor   **/
	CbmTofTrackletTools();

	/**   Destructor   **/
	virtual ~CbmTofTrackletTools();

	Double_t FitTt(CbmTofTracklet *pTrk, Int_t iDetId);
	Double_t* Line3DFit(CbmTofTracklet *pTrk, Int_t iDetId);
		       
	virtual Double_t GetXdif(CbmTofTracklet *pTrk, Int_t iDetId,  CbmTofHit* pHit);
	virtual Double_t GetYdif(CbmTofTracklet *pTrk, Int_t iDetId,  CbmTofHit* pHit);
	virtual Double_t GetTdif(CbmTofTracklet *pTrk, Int_t iDetId,  CbmTofHit* pHit);
	virtual Double_t GetTexpected(CbmTofTracklet *pTrk, Int_t iDetId, CbmTofHit* pHit);
	
 private:

	ClassDef(CbmTofTrackletTools, 1);
};

#endif /* CBMTOFTRACKLETTOOLS_H */
