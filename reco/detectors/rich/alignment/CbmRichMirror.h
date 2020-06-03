#ifndef CBMRICHMIRROR_H
#define CBMRICHMIRROR_H


#include "FairTask.h"
#include "TVector3.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "CbmRichRingLight.h"

using namespace std;

class TClonesArray;
class TH1D;
class TH2D;


class CbmRichMirror
{
public:
	/*
	 * Constructor.
	 */
	CbmRichMirror() : fMirrorId(""), fMomentum(), fProjHit(), fExtrapHit(), fRotAngles(), fRingL()
	{
	}

	/*
	 * Destructor.
	 */
    virtual ~CbmRichMirror() {}

    // Modifiers:
	void setMirrorId(string s) { fMirrorId = s; }
	void setMomentum(TVector3 v) { fMomentum = v; }
	void setProjHit(Double_t xX, Double_t yY) { fProjHit.push_back(xX), fProjHit.push_back(yY); }
	void setExtrapHit(Double_t xX, Double_t yY) { fExtrapHit.push_back(xX), fExtrapHit.push_back(yY); }
	void setRotAngles(Double_t xX, Double_t yY) { fRotAngles.push_back(xX), fRotAngles.push_back(yY); }
	void setRingLight(CbmRichRingLight ringL) { fRingL = ringL; }

	// Accessors:
	string getMirrorId() { return fMirrorId; }
	TVector3 getMomentum() { return fMomentum; }
	vector<Double_t> getProjHit() { return fProjHit; }
	vector<Double_t> getExtrapHit() { return fExtrapHit; }
	vector<Double_t> getRotAngles() { return fRotAngles; }
	CbmRichRingLight getRingLight() { return fRingL; }


private:
	string fMirrorId;
	TVector3 fMomentum;
	vector<Double_t> fProjHit;
	vector<Double_t> fExtrapHit;
	vector<Double_t> fRotAngles;
	CbmRichRingLight fRingL;

    CbmRichMirror(const CbmRichMirror&);
    CbmRichMirror operator=(const CbmRichMirror&);

    ClassDef(CbmRichMirror, 1);
};

#endif
