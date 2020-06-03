// -------------------------------------------------------------------------
// -----                        CbmTrack header file                   -----
// -----                  Created 29/11/07  by V. Friese               -----
// -----                  Modified 26/05/09  by A. Lebedev             -----
// -------------------------------------------------------------------------

/**  CbmTrack.h
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Base class for local tracks in the CBM detectors.
 ** Holds a list of CbmHits and the fitted track parameters.
 ** The fit parameters are of type FairTrackParam
 ** and can only be accessed and modified via this class.
 **/

#ifndef CBMTRACK_H_
#define CBMTRACK_H_ 1

#include <Rtypes.h>          // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>      // for Int_t, Double_t, Double32_t
#include <TObject.h>         // for TObject

#include <FairTrackParam.h>  // for FairTrackParam

#include <string>            // for string
#include <vector>            // for vector

#include "CbmHit.h"          // for HitType

class CbmMatch; 

class CbmTrack : public TObject
{
public:
	/** Default constructor **/
	CbmTrack();

	/** Destructor **/
	virtual ~CbmTrack();

	/** Copy Constructor **/
	CbmTrack(const CbmTrack&);
	
	/** Assignment Operator **/
	CbmTrack& operator=(const CbmTrack&);

	/** Add a hit to the list, using index and HitType
	 * @param index Index of the hit in the array
	 * @param type Type of the hit to be added
	 **/
	void AddHit(
			Int_t index,
			HitType type);

	/** Accessors  **/
	virtual Int_t GetNofHits() const { return fHitIndex.size(); }
	Int_t GetHitIndex(Int_t iHit) const {return fHitIndex[iHit];}
	HitType GetHitType(Int_t iHit) const {return fHitType[iHit];}
	Int_t GetPidHypo() const { return fPidHypo; }
	Int_t GetFlag() const { return fFlag; }
	Double_t GetChiSq() const { return fChiSq; }
	Int_t GetNDF() const { return fNDF; }
	Int_t GetPreviousTrackId() const { return fPreviousTrackId; }
	const FairTrackParam* GetParamFirst() const { return &fParamFirst; }
	const FairTrackParam* GetParamLast() const { return &fParamLast; }
    CbmMatch* GetMatch() const { return fMatch; }
    Double_t GetTime() const { return fTime; }
    Double_t GetTimeError() const { return fTimeError; }

	/** Modifiers  **/
	void SetPidHypo(Int_t pid){ fPidHypo = pid; }
	void SetFlag(Int_t flag) { fFlag = flag; }
	void SetChiSq(Double_t chiSq) { fChiSq = chiSq; }
	void SetNDF(Int_t ndf) { fNDF = ndf; }
	void SetPreviousTrackId(Int_t previousTrackId) { fPreviousTrackId = previousTrackId; }
	void SetParamFirst(const FairTrackParam* par) { fParamFirst = *par; }
	void SetParamLast(const FairTrackParam* par){ fParamLast  = *par; }
    void SetMatch(CbmMatch* match);
    void SetTime(Double_t time) { fTime = time; }
    void SetTimeError(Double_t error) { fTimeError = error; }

	virtual std::string ToString() const;

protected:
	/** Array contains the hit indices of the hits attached to the track **/
	std::vector<Int_t> fHitIndex;

	/** Array contains the hit types of the hits attached to the track **/
	std::vector<HitType> fHitType;

	/** PID hypothesis used by the track fitter **/
	Int_t fPidHypo;

	/** Track parameters at first and last fitted hit **/
	FairTrackParam fParamFirst;
	FairTrackParam fParamLast;

	/** Track start time **/
	Double_t fTime;
	Double_t fTimeError;

	/** Quality flag **/
	Int_t fFlag;

	/** Chi square and NDF of track fit **/
	Double32_t fChiSq;
	Int_t fNDF;

	/** Index of previous track segment **/
	Int_t fPreviousTrackId;

	/** Monte-Carlo information **/
   CbmMatch* fMatch;

	ClassDef(CbmTrack, 4);
};

#endif
