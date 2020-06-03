/*
 * CbmRichRingHitsAnalyser.h
 *
 *  Created on: Apr 15, 2015
 *      Author: evovch
 */
 
 /*
  * There is a known bug which has to be fixed:
  * The conditions on the ring center and radius should somehow
  * switch when analysis goes to the new file in the same run.
  */

#ifndef RICH_PROTOTYPE_BEAMTIME_CBMRICHRINGHITSANALYSER_H_
#define RICH_PROTOTYPE_BEAMTIME_CBMRICHRINGHITSANALYSER_H_

#include "FairTask.h"

#include "CbmHistManager.h"
#include "CbmDrawHist.h"

class TClonesArray;

struct ringCondition
{
	Double_t cxmin;
	Double_t cxmax;
	Double_t cymin;
	Double_t cymax;
	Double_t rmin;
	Double_t rmax;
};

class CbmRichRingHitsAnalyser : public FairTask
{
public:
	CbmRichRingHitsAnalyser();
	virtual ~CbmRichRingHitsAnalyser();

	virtual InitStatus Init();
	virtual void Exec(Option_t* option);
	virtual void Finish();
	
	void SetApplyRingCcut(Bool_t doOrNot) {fApplyRingCcut=doOrNot;}
	void SetApplyRingRcut(Bool_t doOrNot) {fApplyRingRcut=doOrNot;}
	
	void SetInputFileBasename(TString filename) {fInputFileBasename=filename;}
	void SetRingCutsFile(TString filename) {fRingCutsFile=filename;}
	void SetOutHistoFile(TString filename) {fOutHistoFile = filename;}
	void SetWlsState(TString state) { fWlsState = state; }

private:
	TClonesArray* fRichHits;			// input
	TClonesArray* fRichRings;	        // input

	TClonesArray* fRichHitInfos;

	TString fWlsState; // "off" or "on"
	
	TString fInputFileBasename;			// needed for conditions
	TString fRingCutsFile;
	TString fOutHistoFile;

	// Full table of conditions
	std::map<TString, ringCondition> fConditionsMap;
	// Current condition on ring center - for the current file
	ringCondition fRingCondition;
	
	Bool_t fApplyRingCcut;
	Bool_t fApplyRingRcut;
	
	CbmHistManager* fHM;

	//TODO implement or get rid
	CbmRichRingHitsAnalyser(const CbmRichRingHitsAnalyser&);
	CbmRichRingHitsAnalyser operator=(const CbmRichRingHitsAnalyser&);

	ClassDef(CbmRichRingHitsAnalyser, 1);
};

#endif // RICH_PROTOTYPE_BEAMTIME_CBMRICHRINGHITSANALYSER_H_
