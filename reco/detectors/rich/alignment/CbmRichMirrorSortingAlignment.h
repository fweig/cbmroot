#ifndef CBMRICHMIRRORSORTINGALIGNMENT_H
#define CBMRICHMIRRORSORTINGALIGNMENT_H

#include "FairTask.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmRichMirror.h"
#include "TGeoNavigator.h"

#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

class TClonesArray;
class TH1D;
class TH2D;


class CbmRichMirrorSortingAlignment : public FairTask
{
public:
	/*
	 * Constructor.
	 */
	CbmRichMirrorSortingAlignment();

	/*
	 * Destructor.
	 */
    virtual ~CbmRichMirrorSortingAlignment();

    /**
     * \brief Inherited from FairTask.
     */
    virtual InitStatus Init();

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Exec(
    		Option_t* option);

    void ComputeAngles();

    void GetPmtNormal(Int_t NofPMTPoints, vector<Double_t> &normalPMT, Double_t &normalCste);

    void ComputeR2(vector<Double_t> &ptR2Center, vector<Double_t> &ptR2Mirr, vector<Double_t> ptM, vector<Double_t> ptC, vector<Double_t> ptR1, TGeoNavigator* navi, TString s);

    void ComputeP(vector<Double_t> &ptPMirr, vector<Double_t> &ptPR2, vector<Double_t> normalPMT, vector<Double_t> ptM, vector<Double_t> ptR2Mirr, Double_t constantePMT);

    void CreateHistoMap(std::map<string, vector<CbmRichMirror*>> mirrorMap, std::map<string, TH2D*> &histoMap);

    void DrawFitAndExtractAngles(std::map<string, vector<Double_t>> &anglesMap, std::map<string, TH2D*> histoMap);

    void setOutputDir(TString s) { fOutputDir = s; }

    void setStudyName(TString s) { fStudyName = s; }

    void setThreshold(Int_t t) { fThreshold = t; }

    /**
     * \brief Inherited from FairTask.
     */
    virtual void Finish();


private:

    UInt_t fEventNb;
    CbmRichRingFitterCOP* fCopFit;
    CbmRichRingFitterEllipseTau* fTauFit;
    TString fOutputDir;
    TString fStudyName;
    Int_t fThreshold;
	std::map<string, vector<CbmRichMirror*>> fMirrorMap;

    TClonesArray* fGlobalTracks;
    TClonesArray* fRichRings;
    TClonesArray* fMCTracks;
    TClonesArray* fMirrorPoints;
    TClonesArray* fRefPlanePoints;
    TClonesArray* fPmtPoints;
    TClonesArray* fRichProjections;
    TClonesArray* fTrackParams;
    TClonesArray* fRichRingMatches;
    TClonesArray* fStsTrackMatches;

    CbmRichMirrorSortingAlignment(const CbmRichMirrorSortingAlignment&);
    CbmRichMirrorSortingAlignment operator=(const CbmRichMirrorSortingAlignment&);

    ClassDef(CbmRichMirrorSortingAlignment, 1);
};

#endif
